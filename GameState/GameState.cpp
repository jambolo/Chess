#include "GameState.h"

#include "Board.h"
#include "Move.h"
#include "Piece.h"

#include "Misc/Etc.h"
#include "Misc/exceptions.h"
#include "StaticEvaluator/StaticEvaluator.h"
#include "ZHash/ZHash.h"

#include <regex>

bool GameState::initializeFromFen(char const * fen)
{
    char const * start = fen;
    char const * end   = start;

    // Extract the piece placement field
    end = strchr(start, ' ');
    if (!end || !board_.initializeFromFen(start, end))
        return false;
    start = end + 1;

    // Extract the active color
    end = strchr(start, ' ');
    if (!end || !whoseTurnFromFen(start, end))
        return false;
    start = end + 1;

    // Extract the castling availability
    end = strchr(start, ' ');
    if (!end || !castleStatusFromFen(start, end))
        return false;
    start = end + 1;

    // Extract the en passant target square
    Position enpassant;
    end = strchr(start, ' ');
    if (!end && !enpassant.initializeFromFen(start, end))
        return false;
    start = end + 1;

    // Extract the fifty-move rule timer
    end = strchr(start, ' ');
    if (!end || !fiftyMoveTimerFromFen(start, end))
        return false;
    start = end + 1;

    // Extract the move number
    end = strchr(start, 0);
    if (!end || !moveNumberFromFen(start, end))
        return false;

    return true;
}

void GameState::makeMove(Color color, Move const & move, int depth /*= 0*/)
{
#if defined(GAME_STATE_ANALYSIS_ENABLED)
    // Add this move to the sequence
    assert(depth > 0);
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
#else // if defined(GAME_STATE_ANALYSIS_ENABLED)
    (void)depth;
#endif // if defined(GAME_STATE_ANALYSIS_ENABLED)

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

    if (color == Color::WHITE)
    {
        whoseTurn_ = Color::BLACK;
    }
    else
    {
        whoseTurn_ = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
        ++moveNumber_;
    }
}

std::string GameState::fen() const
{
    std::string result;

    result  = board_.fen();
    result += ' ';
    result += (whoseTurn_ == Color::BLACK) ? 'b' : 'w';
    result += ' ';
    result += castleStatusToFen();
    result += ' ';
    result += enPassant_.san();
    result += ' ';
    result += std::to_string(fiftyMoveTimer_);
    result += ' ';
    result += std::to_string(moveNumber_);
    result += ' ';
    return result;
}

void GameState::makeCastleMove(Color color, Move const & move)
{
#if defined(INCREMENTAL_STATIC_EVALUATION_ENABLED)
    CastleStatus oldStatus = castleStatus_;
#endif // defined( INCREMENTAL_STATIC_EVALUATION_ENABLED )

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

#if defined(INCREMENTAL_STATIC_EVALUATION_ENABLED)
    CastleStatus castleStatusChange = oldStatus ^ castleStatus_;
#endif // defined( INCREMENTAL_STATIC_EVALUATION_ENABLED )

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

#if defined(INCREMENTAL_STATIC_EVALUATION_ENABLED)
    // Update castle status
    CastleStatus oldStatus = castleStatus_;
#endif // defined( INCREMENTAL_STATIC_EVALUATION_ENABLED )

    if (color == Color::WHITE)
    {
        if (pMoved->type() == PieceTypeId::KING)
        {
            castleStatus_ |= WHITE_CASTLE_UNAVAILABLE;
        }
        else if (pMoved->type() == PieceTypeId::ROOK)
        {
            if ((move.from().row == Board::SIZE - 1) && (move.from().column == 0))
            {
                castleStatus_ |= WHITE_QUEENSIDE_CASTLE_UNAVAILABLE;
            }
            else if ((move.from().row == Board::SIZE - 1) && (move.from().column == Board::SIZE - 1))
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
            if ((move.from().row == 0) && (move.from().column == 0))
            {
                castleStatus_ |= BLACK_KINGSIDE_CASTLE_UNAVAILABLE;
            }
            else if ((move.from().row == 0) && (move.from().column == Board::SIZE - 1))
            {
                castleStatus_ |= BLACK_QUEENSIDE_CASTLE_UNAVAILABLE;
            }
        }
    }

#if defined(INCREMENTAL_STATIC_EVALUATION_ENABLED)
    CastleStatus castleStatusChange = oldStatus ^ castleStatus_;
#endif // defined( INCREMENTAL_STATIC_EVALUATION_ENABLED )

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

    Piece const * pAdded = Piece::piece(PieceTypeId::QUEEN, color); // @todo Not always promoted to a queen

    zhash_.add(*pAdded, position);
    board_.putPiece(pAdded, position);

    return pAdded;
}

void GameState::initialize()
{
    board_.initialize();
    whoseTurn_      = Color::WHITE;
    castleStatus_   = 0;
    fiftyMoveTimer_ = 0;
    makeMove(Color::WHITE, Move::reset());
    value_          = 0;
    quality_        = std::numeric_limits<int8_t>::min();
#if defined(USING_PRIORITIZED_MOVE_ORDERING)
    priority_       = 0;
#endif
    inCheck_        = false;
    moveNumber_     = 1;
    zhash_          = ZHash(board_);

#if defined(GAME_STATE_ANALYSIS_ENABLED)
    analysisData_.reset();
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

bool GameState::whoseTurnFromFen(char const * start, char const * end)
{
    if ((end != start + 1) || ((*start != 'w') && (*start != 'b')))
        return false;
    whoseTurn_ = (*start == 'w') ? Color::WHITE : Color::BLACK;
    return true;
}

bool GameState::castleStatusFromFen(char const * start, char const * end)
{
    castleStatus_ = WHITE_CASTLE_UNAVAILABLE | BLACK_CASTLE_UNAVAILABLE;
    if (*start != '-')
    {
        for (auto c = start; c != end; ++c)
        {
            switch (*c)
            {
                case 'K': castleStatus_ &= ~WHITE_KINGSIDE_CASTLE_UNAVAILABLE;  break;
                case 'Q': castleStatus_ &= ~WHITE_QUEENSIDE_CASTLE_UNAVAILABLE; break;
                case 'k': castleStatus_ &= ~BLACK_KINGSIDE_CASTLE_UNAVAILABLE;  break;
                case 'q': castleStatus_ &= ~BLACK_QUEENSIDE_CASTLE_UNAVAILABLE; break;
                default:  return false;
            }
        }
    }
    return true;
}

bool GameState::fiftyMoveTimerFromFen(char const * start, char const * end)
{
    fiftyMoveTimer_ = std::stoi(std::string(start, end));
    if (fiftyMoveTimer_ < 0)
        return false;
    return true;
}

bool GameState::moveNumberFromFen(char const * start, char const * end)
{
    moveNumber_ = std::stoi(std::string(start, end));
    if (moveNumber_ < 1)
        return false;

    return true;
}

std::string GameState::castleStatusToFen() const
{
    std::string result;
    if ((castleStatus_ & CASTLE_AVAILABILITY_MASK) == CASTLE_AVAILABILITY_MASK)
    {
        result = "-";
    }
    else
    {
        if ((castleStatus_ & WHITE_KINGSIDE_CASTLE_UNAVAILABLE) == 0)
            result += 'K';
        if ((castleStatus_ & WHITE_QUEENSIDE_CASTLE_UNAVAILABLE) == 0)
            result += 'Q';
        if ((castleStatus_ & BLACK_KINGSIDE_CASTLE_UNAVAILABLE) == 0)
            result += 'k';
        if ((castleStatus_ & BLACK_QUEENSIDE_CASTLE_UNAVAILABLE) == 0)
            result += 'q';
    }
    return result;
}
