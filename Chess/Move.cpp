#include "Move.h"

#include "Move.h"
#include "Piece.h"
#include "Types.h"

#include <cassert>
#include <cstdlib>

Move::Move(Piece const * piece, Position const & from, Position const & to, bool capture /*= false*/)
    : piece_(piece)
    , from_(from)
    , to_(to)
    , capture_(capture)
    , special_(NORMAL)
{
}

Move::Move(Special          special,
           Color            color /*= Color::INVALID*/,
           Position const & from /*= Position()*/,
           Position const & to /*= Position()*/,
           bool             capture /*= false*/)
    : from_(from)
    , to_(to)
    , capture_(capture)
    , special_(special)

{
    switch (special)
    {
        case NORMAL:             piece_ = nullptr;                               break;
        case RESIGN:             piece_ = Piece::get(PieceTypeId::KING, color);  break;
        case UNDO:               piece_ = nullptr;                               break;
        case RESET:              piece_ = nullptr;                               break;
        case KINGSIDE_CASTLE:    piece_ = Piece::get(PieceTypeId::KING, color);  break;
        case QUEENSIDE_CASTLE:   piece_ = Piece::get(PieceTypeId::KING, color);  break;
        case PROMOTION:          piece_ = Piece::get(PieceTypeId::PAWN, color);  break;
        case ENPASSANT:          piece_ = Piece::get(PieceTypeId::PAWN, color);  break;
    }
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

std::string Move::notation(Notation notation /*= Notation::STANDARD*/) const
{
    std::string result;
    switch (notation)
    {
        case Notation::STANDARD:
            result = standardNotation();
            break;
        case Notation::LONG:
            result = longNotation();
            break;
        case Notation::PGN:
            result = pgnNotation();
            break;
        case Notation::FIGURINE:
            result = figurineNotation();
            break;
        case Notation::ICCF:
            result = iccfNotation();
        case Notation::UCI:
            result = uciNotation();
        default:
            break;
    }

    return result;
}

Move Move::kingSideCastleKing(Color c)
{
    Piece const * king = Piece::get(PieceTypeId::KING, c);
    if (c == Color::WHITE)
        return Move(king, { 7, 4 }, { 7, 6 });
    else
        return Move(king, { 0, 4 }, { 0, 6 });
}

Move Move::kingSideCastleRook(Color c)
{
    Piece const * rook = Piece::get(PieceTypeId::ROOK, c);
    if (c == Color::WHITE)
        return Move(rook, { 7, 7 }, { 7, 5 });
    else
        return Move(rook, { 0, 7 }, { 0, 5 });
}

Move Move::queenSideCastleKing(Color c)
{
    Piece const * king = Piece::get(PieceTypeId::KING, c);
    if (c == Color::WHITE)
        return Move(king, { 7, 4 }, { 7, 2 });
    else
        return Move(king, { 0, 4 }, { 0, 2 });
}

Move Move::queenSideCastleRook(Color c)
{
    Piece const * rook = Piece::get(PieceTypeId::ROOK, c);
    if (c == Color::WHITE)
        return Move(rook, { 7, 0 }, { 7, 3 });
    else
        return Move(rook, { 0, 0 }, { 0, 3 });
}

std::string Move::standardNotation() const
{
    std::string result;
    if (isKingSideCastle())
    {
        result = "0-0";
    }
    else if (isQueenSideCastle())
    {
        result = "0-0-0";
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
    else
    {
        PieceTypeId id = piece_->type();
        if (id != PieceTypeId::PAWN)
            result = Piece::symbol(piece_->type());
        if (capture_)
        {
            if (id == PieceTypeId::PAWN)
                result += from().notation()[0];
            result += 'x';
        }
        result += to().notation();
        if (isPromotion())
            result += "Q";
    }
    return result;
}

std::string Move::longNotation() const
{
    std::string result;
    if (isKingSideCastle())
    {
        result = "0-0";
    }
    else if (isQueenSideCastle())
    {
        result = "0-0-0";
    }
    else if (isEnPassant())
    {
        result  = from().notation();
        result += 'x';
        result += to().notation();
    }
    else if (isResignation())
    {
        result = (piece_->color() == Color::WHITE) ? "1-0" : "0-1";
    }
    else
    {
        if (piece_->type() != PieceTypeId::PAWN)
            result = Piece::symbol(piece_->type());
        result += from().notation();
        result += capture_ ? 'x' : '-';
        result += to().notation();
        if (isPromotion())
            result += "Q";
    }
    return result;
}

std::string Move::pgnNotation() const
{
    std::string result;
    if (isKingSideCastle())
    {
        result = "O-O";
    }
    else if (isQueenSideCastle())
    {
        result = "O-O-O";
    }
    else if (isEnPassant())
    {
        result  = from().notation();
        result += 'x';
        result += to().notation();
    }
    else if (isResignation())
    {
        result = (piece_->color() == Color::WHITE) ? "1-0" : "0-1";
    }
    else
    {
        PieceTypeId id = piece_->type();
        if (id != PieceTypeId::PAWN)
            result = Piece::symbol(id);
        result += from().notation();
        result += capture_ ? 'x' : '-';
        result += to().notation();
        if (isPromotion())
            result += "=Q";
    }
    return result;
}

std::string Move::figurineNotation() const
{
    std::string result;
    if (isKingSideCastle())
    {
        result = "0-0";
    }
    else if (isQueenSideCastle())
    {
        result = "0-0-0";
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
    else
    {
        PieceTypeId id = piece_->type();
        if (id != PieceTypeId::PAWN)
            result = piece_->figurine();
        if (capture_)
        {
            if (id == PieceTypeId::PAWN)
                result += from().notation()[0];
            result += 'x';
        }
        result += to().notation();
        if (isPromotion())
            result += Piece::figurine(PieceTypeId::QUEEN, piece_->color());
    }
    return result;
}

std::string Move::iccfNotation() const
{
    std::string result;
    if (isKingSideCastle())
    {
        result = (piece_->color() == Color::WHITE) ? "5171" : "5878";
    }
    else if (isQueenSideCastle())
    {
        result = (piece_->color() == Color::WHITE) ? "5131" : "5838";
    }
    else if (isResignation())
    {
        // nothing
    }
    else
    {
        result += from().column + '1';
        result += '8' - from().row;
        result += to().column + '1';
        result += '8' - to().row;
        if (isPromotion())
            result += '1';
    }
    return result;
}

std::string Move::uciNotation() const
{
    std::string result;
    if (isKingSideCastle())
    {
        result = "0-0";
    }
    else if (isQueenSideCastle())
    {
        result = "0-0-0";
    }
    else if (isEnPassant())
    {
        result  = from().notation();
        result += 'x';
        result += to().notation();
    }
    else if (isResignation())
    {
        result = (piece_->color() == Color::WHITE) ? "1-0" : "0-1";
    }
    else
    {
        if (piece_->type() != PieceTypeId::PAWN)
            result = Piece::symbol(piece_->type());
        result += from().notation();
        if (capture_)
            result += 'x';
        result += to().notation();
        if (isPromotion())
            result += "Q";
    }
    return result;
}
