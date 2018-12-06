#include "Move.h"

#include "ChessTypes.h"
#include "Move.h"
#include "Piece.h"

#include <cassert>

Move::Move(Piece const * piece, Position const & from, Position const & to, bool capture /*= false*/)
    : piece_(piece)
    , from_(from)
    , to_(to)
    , capture_(capture)
{
}

Move::Move(int              special,
           Color            color /*= Color::INVALID*/,
           Position const & from /*= Position()*/,
           Position const & to /*= Position()*/,
           bool             capture /*= false*/)
    : from_(from)
    , to_(to)
    , capture_(capture)
{
    switch (special)
    {
        case RESIGN:             piece_ = Piece::get(PieceTypeId::KING, color);  break;
        case UNDO:               piece_ = nullptr;                               break;
        case RESET:              piece_ = nullptr;                               break;
        case KINGSIDE_CASTLE:    piece_ = Piece::get(PieceTypeId::KING, color);  break;
        case QUEENSIDE_CASTLE:   piece_ = Piece::get(PieceTypeId::KING, color);  break;
        case PROMOTION:          piece_ = Piece::get(PieceTypeId::PAWN, color);  break;
        case ENPASSANT:          piece_ = Piece::get(PieceTypeId::PAWN, color);  break;
    }
    from_.row |= special;
}

Position const Move::from() const
{
    return Position(from_.row & NORMAL_MOVE_MASK, from_.column);
}

Position const Move::to() const
{
    return Position(to_.row, to_.column);
}

bool Move::isSpecial() const
{
    return (from_.row & ~NORMAL_MOVE_MASK) != 0;
}

bool Move::isMoved(int dr, int dc)
{
    return dr != 0 || dc != 0;
}

bool Move::isDiagonal(int dr, int dc)
{
    return dr == dc || dr == -dc;
}

bool Move::inRange(int dr, int dc, int range)
{
    return dr > range || dr < -range || dc > range || dc < -range;
}

bool Move::isSquare(int dr, int dc)
{
    return dr == 0 || dc == 0;
}

std::string Move::notation() const
{
#if defined(FEATURE_NOTATION_LONG)
    std::string result;
    if (isSpecial())
    {
        if (isKingSideCastle())
        {
            result = "0-0";
}
        else if (isQueenSideCastle())
        {
            result = "0-0-0";
        }
        else if (isPromotion())
        {
            result = to().notation() + "=Q";
        }
        else if (isEnPassant())
        {
            result += char(from().column + 'a');
            result += 'x';
            result += to().notation();
        }
        else if (isResignation())
        {
            result = (piece_->color() == Color::WHITE) ? "1-0" : "0-1";
        }
    }
    else
    {
        if (piece_->type() != PieceTypeId::PAWN)
            result = Piece::symbol(piece_->type());
        result += from().notation();
        result += capture_ ? 'x' : '-';
        result += to().notation();
#else
    std::string result;
    if (isSpecial())
    {
        if (isKingSideCastle())
        {
            result = "0-0";
        }
        else if (isQueenSideCastle())
        {
            result = "0-0-0";
        }
        else if (isPromotion())
        {
            result = to().notation() + "=Q";
        }
        else if (isEnPassant())
        {
            result += char(from().column + 'a');
            result += 'x';
            result += to().notation();
        }
        else if (isResignation())
        {
            result = (piece_->color() == Color::WHITE) ? "1-0" : "0-1";
        }
    }
    else
    {
        if (piece_->type() != PieceTypeId::PAWN)
            result = Piece::symbol(piece_->type());
        if (capture_)
        {
            if (piece_->type() == PieceTypeId::PAWN)
                result += char(from().column + 'a');
            result += 'x';
        }
        result += to().notation();
    }
#endif
    return result;
}

Move Move::kingSideCastleKing(Color c)
{
    if (c == Color::WHITE)
        return Move(KINGSIDE_CASTLE, Color::WHITE, { 7, 4 }, { 7, 6 });
    else
        return Move(KINGSIDE_CASTLE, Color::BLACK, { 0, 4 }, { 0, 6 });
}

Move Move::kingSideCastleRook(Color c)
{
    if (c == Color::WHITE)
        return Move(KINGSIDE_CASTLE, Color::WHITE, { 7, 7 }, { 7, 5 });
    else
        return Move(KINGSIDE_CASTLE, Color::BLACK, { 0, 7 }, { 0, 5 });
}

Move Move::queenSideCastleKing(Color c)
{
    if (c == Color::WHITE)
        return Move(QUEENSIDE_CASTLE, Color::WHITE, { 7, 4 }, { 7, 2 });
    else
        return Move(QUEENSIDE_CASTLE, Color::BLACK, { 0, 4 }, { 0, 2 });
}

Move Move::queenSideCastleRook(Color c)
{
    if (c == Color::WHITE)
        return Move(QUEENSIDE_CASTLE, Color::WHITE, { 7, 0 }, { 7, 3 });
    else
        return Move(QUEENSIDE_CASTLE, Color::BLACK, { 0, 0 }, { 0, 3 });
}

bool Move::isSpecial(int move) const
{
    return (from_.row & ~NORMAL_MOVE_MASK) == move;
}
