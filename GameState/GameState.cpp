#include "GameState.h"

#include "Board.h"
#include "ChessTypes.h"
#include "Move.h"
#include "Piece.h"

#include "Misc/exceptions.h"
#include "Misc/Etc.h"
#include "ZHash/ZHash.h"

#if defined(FEATURE_INCREMENTAL_STATIC_EVALUATION)
#include "ComputerPlayer/StaticEvaluator.h"
#endif // defined(FEATURE_INCREMENTAL_STATIC_EVALUATION)

#include <regex>

GameState::GameState(Board const & board,
                     Color         whoseTurn,
                     CastleStatus  castleStatus,
                     int           fiftyMoveTimer,
                     Move const &  move,
                     bool          inCheck,
                     int           moveNumber)
    : board_(board)
    , castleStatus_(castleStatus)
    , whoseTurn_(whoseTurn)
    , fiftyMoveTimer_(fiftyMoveTimer)
    , move_(move)
    , inCheck_(inCheck)
    , zhash_(board)
{
}

void GameState::initialize()
{
    board_.initialize();
    whoseTurn_      = Color::WHITE;
    castleStatus_   = 0;
    fiftyMoveTimer_ = 0;
    makeMove(Color::WHITE, Move::reset());
    inCheck_    = false;
    moveNumber_ = 1;
    zhash_      = ZHash(board_);

#if defined(FEATURE_GAME_STATE_ANALYSIS)
    analysisData_.reset();
#endif
}

#if defined(FEATURE_GAME_STATE_ANALYSIS)

void GameState::resetAnalysisData()
{
    analysisData_.reset();
}

void GameState::AnalysisData::reset()
{
    memset(expected, -1, sizeof(expected));
}

#endif // defined( FEATURE_GAME_STATE_ANALYSIS )

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

bool GameState::castleIsAllowed(Color c) const
{
    unsigned mask = (c == Color::WHITE) ? WHITE_CASTLE_UNAVAILABLE : BLACK_CASTLE_UNAVAILABLE;
    return (castleStatus_ & mask) == 0;
}

bool GameState::kingSideCastleIsAllowed(Color c) const
{
    unsigned mask = (c == Color::WHITE) ? WHITE_KINGSIDE_CASTLE_UNAVAILABLE : BLACK_KINGSIDE_CASTLE_UNAVAILABLE;
    return (castleStatus_ & mask) == 0;
}

bool GameState::queenSideCastleIsAllowed(Color c) const
{
    unsigned mask = (c == Color::WHITE) ? WHITE_QUEENSIDE_CASTLE_UNAVAILABLE : BLACK_QUEENSIDE_CASTLE_UNAVAILABLE;
    return (castleStatus_ & mask) == 0;
}

bool GameState::canBeOccupied(Position const & p, Color myColor) const
{
    Piece const * piece = board_.pieceAt(p);
    return (piece == NO_PIECE) || (piece->color() != myColor);
}

ZHash GameState::zhash() const
{
    return zhash_;
}

void GameState::makeMove(Color color, Move const & move, int depth /*= 0*/)
{
#if defined(FEATURE_GAME_STATE_ANALYSIS)
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
#else // if defined(FEATURE_GAME_STATE_ANALYSIS)
    (void)depth;
#endif // if defined(FEATURE_GAME_STATE_ANALYSIS)

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
        whoseTurn_ = Color::WHITE;
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
    result += enPassant_.notation();
    result += ' ';
    result += std::to_string(fiftyMoveTimer_);
    result += ' ';
    result += std::to_string(moveNumber_);
    result += ' ';
    return result;
}

void GameState::makeNormalMove(Color color, Move const & move)
{
    Piece const * movedPiece = board_.pieceAt(move.from());

    // Capture a piece
    Position capturedPosition = move.isEnPassant() ? Position(move.from().row, move.to().column) : move.to();
    Piece const * capturedPiece = board_.pieceAt(capturedPosition);
    if (capturedPiece != NO_PIECE)
    {
        zhash_.remove(capturedPiece, capturedPosition);
        board_.removePiece(capturedPosition);
    }

    // Move the piece on the board
    zhash_.move(movedPiece, move.from(), move.to());
    board_.movePiece(move.from(), move.to());

    // Handle a promotion
    Piece const * addedPiece;

    if (move.isPromotion())
        addedPiece = promote(color, move.to());
    else
        addedPiece = NO_PIECE;

    // Update castle status
    
#if defined(FEATURE_INCREMENTAL_STATIC_EVALUATION)
    CastleStatus oldStatus = castleStatus_;
#endif // defined( FEATURE_INCREMENTAL_STATIC_EVALUATION )

    if (movedPiece->type() == PieceTypeId::KING)
        castleStatus_ |= (color == Color::WHITE) ? WHITE_CASTLE_UNAVAILABLE : BLACK_CASTLE_UNAVAILABLE;
    else if (move.from() == Board::INITIAL_WHITE_ROOK_QUEENSIDE_POSITION || move.to() == Board::INITIAL_WHITE_ROOK_QUEENSIDE_POSITION)
        castleStatus_ |= WHITE_QUEENSIDE_CASTLE_UNAVAILABLE;
    else if (move.from() == Board::INITIAL_WHITE_ROOK_KINGSIDE_POSITION || move.to() == Board::INITIAL_WHITE_ROOK_KINGSIDE_POSITION)
        castleStatus_ |= WHITE_KINGSIDE_CASTLE_UNAVAILABLE;
    else  if (move.from() == Board::INITIAL_BLACK_ROOK_QUEENSIDE_POSITION || move.to() == Board::INITIAL_BLACK_ROOK_QUEENSIDE_POSITION)
        castleStatus_ |= BLACK_QUEENSIDE_CASTLE_UNAVAILABLE;
    else if (move.from() == Board::INITIAL_BLACK_ROOK_KINGSIDE_POSITION || move.to() == Board::INITIAL_BLACK_ROOK_KINGSIDE_POSITION)
        castleStatus_ |= BLACK_KINGSIDE_CASTLE_UNAVAILABLE;

#if defined(FEATURE_INCREMENTAL_STATIC_EVALUATION)
    CastleStatus castleStatusChange = oldStatus ^ castleStatus_;
#endif // defined( FEATURE_INCREMENTAL_STATIC_EVALUATION )

    // Update check status
    // @todo check for in check

#if defined(FEATURE_INCREMENTAL_STATIC_EVALUATION)
    value_ += StaticEvaluator::incremental(move, castleStatusChange, movedPiece, &capturedPosition, capturedPiece, addedPiece);
#endif // defined( FEATURE_INCREMENTAL_STATIC_EVALUATION )
}

void GameState::makeCastleMove(Color color, Move const & move)
{
#if defined(FEATURE_INCREMENTAL_STATIC_EVALUATION)
    CastleStatus oldStatus = castleStatus_;
#endif // defined( FEATURE_INCREMENTAL_STATIC_EVALUATION )

    // Move the king
    
    {
        Move kingsMove = move.isKingSideCastle() ? Move::kingSideCastleKing(color) : Move::queenSideCastleKing(color);
        Piece const * king = Piece::get(PieceTypeId::KING, color);
        zhash_.move(king, kingsMove.from(), kingsMove.to());
        board_.movePiece(kingsMove.from(), kingsMove.to());
    }
    
    // Move the rook
    
    {
        Move rooksMove = move.isKingSideCastle() ? Move::kingSideCastleRook(color) : Move::queenSideCastleRook(color);
        Piece const * rook = Piece::get(PieceTypeId::ROOK, color);
        zhash_.move(rook, rooksMove.from(), rooksMove.to());
        board_.movePiece(rooksMove.from(), rooksMove.to());
    }
    
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

#if defined(FEATURE_INCREMENTAL_STATIC_EVALUATION)
    CastleStatus castleStatusChange = oldStatus ^ castleStatus_;
#endif // defined( FEATURE_INCREMENTAL_STATIC_EVALUATION )

    // Update check status
    // @todo check for in check

#if defined(FEATURE_INCREMENTAL_STATIC_EVALUATION)
    value_ += StaticEvaluator::incremental(move, castleStatusChange);
#endif // defined( FEATURE_INCREMENTAL_STATIC_EVALUATION )
}

Piece const * GameState::promote(Color color, Position const & position)
{
    Piece const * pPiece = board_.pieceAt(position);

    // Remove the pawn

    zhash_.remove(pPiece, position);
    board_.removePiece(position);

    // Replace with a queen

    Piece const * addedPiece = Piece::get(PieceTypeId::QUEEN, color); // @todo Not always promoted to a queen

    zhash_.add(addedPiece, position);
    board_.putPiece(addedPiece, position);

    return addedPiece;
}

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

bool operator ==(GameState const & x, GameState const & y)
{
    // Easy quick test
    if (x.zhash_.value() != y.zhash_.value())
        return false;

    //! @todo	Actually this is not correct because en passant validity and fifty-move state are part of the state, too
    return x.board_ == y.board_ &&
           (x.castleStatus_ & CASTLE_AVAILABILITY_MASK) == (y.castleStatus_ & CASTLE_AVAILABILITY_MASK);
}
