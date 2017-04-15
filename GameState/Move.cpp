#include "Move.h"

#include "Piece.h"

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
        case RESIGN:             piece_ = Piece::piece(PieceTypeId::KING, color);  break;
        case UNDO:               piece_ = nullptr;                                 break;
        case RESET:              piece_ = nullptr;                                 break;
        case KINGSIDE_CASTLE:    piece_ = Piece::piece(PieceTypeId::KING, color);  break;
        case QUEENSIDE_CASTLE:   piece_ = Piece::piece(PieceTypeId::KING, color);  break;
        case PROMOTION:          piece_ = Piece::piece(PieceTypeId::PAWN, color);  break;
        case ENPASSANT:          piece_ = Piece::piece(PieceTypeId::PAWN, color);  break;
    }
    from_.row |= special;
}

//#define LONG_FORM

std::string Move::san() const
{
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
            result = to().san() + "=Q";
        }
        else if (isEnPassant())
        {
            result += char(from().column + 'a');
            result += 'x';
            result += to().san();
        }
        else if (isResignation())
        {
            result = (piece_->color() == Color::WHITE) ? "1-0" : "0-1";
        }
    }
    else
    {
#if defined(LONG_FORM)
        if (piece_->type() != PieceTypeId::PAWN)
            result = Piece::symbol(piece_->type());
        result += from().san();
        result += capture_ ? 'x' : '-';
        result += to().san();
#else
        if (piece_->type() != PieceTypeId::PAWN)
            result = Piece::symbol(piece_->type());
        if (capture_)
        {
            if (piece_->type() == PieceTypeId::PAWN)
                result += char(from().column + 'a');
            result += 'x';
        }
        result += to().san();
#endif  // LONG_FORM
    }
    return result;
}
