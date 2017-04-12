#include "GameState.h"

#include "Board.h"
#include "Move.h"
#include "Piece.h"
#include "Piecelist.h"

#include "misc/Etc.h"
#include "misc/exceptions.h"
#include "staticevaluator/StaticEvaluator.h"
#include "zhash/ZHash.h"

#include <regex>


#if defined(GAME_STATE_ANALYSIS_ENABLED)

GameState::GameState(GameState const & old_state, Color color, Move const & move, int depth)
{
    analysisData_ = old_state.analysisData_;
    board_        = old_state.board_;
    castleStatus_ = old_state.castleStatus_;
    zhash_        = old_state.zhash_;

#if defined(INCREMENTAL_STATIC_EVALUATION_ENABLED)
    value_ = old_state.value_;
#endif

    // Add this move to the sequence

    int const sequenceIndex = depth - 1;

    if (sequenceIndex < elementsof(analysisData_.expected))
    {
        if (!move.isResignation() && !move.isUndo())
        {
            if (move.isKingSideCastle() || move.isQueenSideCastle())
            {
                analysisData_.expected[sequenceIndex] = SequenceEntry(color, PieceTypeId::INVALID, move);
            }
            else
            {
                Piece const * const pPiece = board_.pieceAt(move.from());
                analysisData_.expected[sequenceIndex] = SequenceEntry(color, pPiece->type(), move);
            }
        }
    }

    // Make the move
    makeMove(color, move);

    // Note: value_, quality_, and priority_ are left uninitialized
}

#else // defined( GAME_STATE_ANALYSIS_ENABLED )

GameState::GameState(GameState const & old_state, Color color, Move const & move)
{
    board_          = old_state.board_;
    castleStatus_   = old_state.castleStatus_;
    zhash_          = old_state.zhash_;

    makeMove(color, move);

    // Note: value_, quality_, and priority_ are left uninitialized
}

#endif // defined( GAME_STATE_ANALYSIS_ENABLED )

GameState::GameState(char const * fen)
{
    char const * start = fen;
    char const * end   = start;

    // Extract the piece placement field
    end = strchr(start, ' ');
    if (!end)
        throw ConstructorFailedException();
    {
        std::string placement(start, end);
        initialize(placement);
    }

    // Extract the active color
    std::string color;

    // Extract the castling availability
    std::string castle;

    // Extract the en passant target square
    std::string enPassant;

    // Extract the fifty-move rule timer
    std::string fiftyMoveRuleTimer;

    // Extract the move number
    std::string moveNumber;
}

void GameState::makeMove(Color color, Move const & move)
{
    // Save the move

    move_ = move;

    // These special moves don't do anything

    if (move.isResignation() || move.isUndo() || move.isStartingPosition())
    {
        // Do nothing
    }

    // Castle is a special case move. Handle it separately

    else if (move.isKingSideCastle() || move.isQueenSideCastle())
    {
        makeCastleMove(color, move);
    }

    // Normal move

    else
    {
        makeNormalMove(color, move);
    }
}

std::string GameState::fen() const
{
    return std::string();
}

void GameState::makeCastleMove(Color color, Move const & move)
{
    CastleStatus oldStatus = castleStatus_;

    Move kingsMove;
    Move rooksMove;

    if (move.isKingSideCastle())
    {
        kingsMove = Move::kingSideCastleKing(color);
        rooksMove = Move::kingSideCastleRook(color);
    }
    else
    {
        kingsMove = Move::queenSideCastleKing(color);
        rooksMove = Move::queenSideCastleRook(color);
    }

    // Move the king

    Piece const * pKing = board_.pieceAt(kingsMove.from());

    zhash_.remove(*pKing, kingsMove.from());
    zhash_.add(*pKing, kingsMove.to());

    board_.movePiece(kingsMove.from(), kingsMove.to());

    // Move the rook

    Piece const * const pRook = board_.pieceAt(rooksMove.from());

    zhash_.remove(*pRook, rooksMove.from());
    zhash_.add(*pRook, rooksMove.to());

    board_.movePiece(rooksMove.from(), rooksMove.to());

    // Update castle status

    if (color == Color::WHITE)
    {
        castleStatus_ |= move.isKingSideCastle() ? WHITE_KINGSIDE_CASTLE : WHITE_QUEENSIDE_CASTLE;
        castleStatus_ |= WHITE_CASTLE_UNAVAILABLE;
    }
    else
    {
        castleStatus_ |= move.isKingSideCastle() ? BLACK_KINGSIDE_CASTLE : BLACK_QUEENSIDE_CASTLE;
        castleStatus_ |= BLACK_CASTLE_UNAVAILABLE;
    }

    CastleStatus castleStatusChange = oldStatus ^ castleStatus_;

    // Update check status
    // @todo check for in check

#if defined(INCREMENTAL_STATIC_EVALUATION_ENABLED)
    value_ += StaticEvaluator::incremental(move, castleStatusChange);
#endif // defined( INCREMENTAL_STATIC_EVALUATION_ENABLED )
}

void GameState::makeNormalMove(Color color, Move const & move)
{
    Piece const * pMoved = board_.pieceAt(move.from());

    // Capture a piece

    Piece const * pCaptured;
    Position capturedPosition;

    if (move.isEnPassant())
    {
        capturedPosition = Position(move.from().row, move.to().column);
    }
    else
    {
        capturedPosition = move.to();
    }

    pCaptured = board_.pieceAt(capturedPosition);
    if (pCaptured != NO_PIECE)
    {
        zhash_.remove(*pCaptured, capturedPosition);
        board_.removePiece(capturedPosition);
    }

    // Move the piece on the board

    zhash_.remove(*pMoved, move.from());
    zhash_.add(*pMoved, move.to());

    board_.movePiece(move.from(), move.to());

    // Handle promotion

    Piece const * pAdded;

    if (move.isPromotion())
    {
        pAdded = promote(color, move.to());
    }
    else
    {
        pAdded = NO_PIECE;
    }

    // Update castle status

    CastleStatus oldStatus = castleStatus_;

    if (color == Color::WHITE)
    {
        if (pMoved->type() == PieceTypeId::KING)
        {
            castleStatus_ |= WHITE_CASTLE_UNAVAILABLE;
        }
        else if (pMoved->type() == PieceTypeId::ROOK)
        {
            if (( move.from().row == Board::SIZE - 1) && ( move.from().column == 0) )
            {
                castleStatus_ |= WHITE_QUEENSIDE_CASTLE_UNAVAILABLE;
            }
            else if (( move.from().row == Board::SIZE - 1) && ( move.from().column == Board::SIZE - 1) )
            {
                castleStatus_ |= WHITE_KINGSIDE_CASTLE_UNAVAILABLE;
            }
        }
    }
    else
    {
        if (pMoved->type() == PieceTypeId::KING)
        {
            castleStatus_ |= BLACK_CASTLE_UNAVAILABLE;
        }
        else if (pMoved->type() == PieceTypeId::ROOK)
        {
            if (( move.from().row == 0) && ( move.from().column == 0) )
            {
                castleStatus_ |= BLACK_KINGSIDE_CASTLE_UNAVAILABLE;
            }
            else if (( move.from().row == 0) && ( move.from().column == Board::SIZE - 1) )
            {
                castleStatus_ |= BLACK_QUEENSIDE_CASTLE_UNAVAILABLE;
            }
        }
    }

    CastleStatus castleStatusChange = oldStatus ^ castleStatus_;

    // Update check status
    // @todo check for in check

#if defined(INCREMENTAL_STATIC_EVALUATION_ENABLED)
    value_ += StaticEvaluator::incremental(move, castleStatusChange, pMoved, &capturedPosition, pCaptured, pAdded);
#endif // defined( INCREMENTAL_STATIC_EVALUATION_ENABLED )
}

Piece const * GameState::promote(Color color, Position const & position)
{
    Piece const * pPiece = board_.pieceAt(position);

    // Remove the pawn

    zhash_.remove(*pPiece, position);
    board_.removePiece(position);

    // Replace with a queen

    Piece const * pAdded = queen(color);

    zhash_.add(*pAdded, position);
    board_.putPiece(pAdded, position);

    return pAdded;
}

void GameState::initialize()
{
    board_.initialize();
    castleStatus_.status = 0;

    zhash_ = ZHash(board_);

    makeMove(Color::WHITE, Move::reset());

    value_    = 0;
    quality_  = std::numeric_limits<int8_t>::min();
#if defined(USING_PRIORITIZED_MOVE_ORDERING)
    priority_ = 0;
#endif
}

#if defined(GAME_STATE_ANALYSIS_ENABLED)

void GameState::resetAnalysisData()
{
    analysisData_.reset();
}

void GameState::AnalysisData::reset()
{
    memset(expected, -1, sizeof(expected));
}

#endif // defined( GAME_STATE_ANALYSIS_ENABLED )
