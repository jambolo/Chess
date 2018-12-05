#include "Sequence.h"

#include "GameState/Piece.h"

SequenceEntry::SequenceEntry()
    : pieceTypeId_(-1)
{
}

SequenceEntry::SequenceEntry(Color color, PieceTypeId piece, Move const & move)
    : color_((int)color)
    , pieceTypeId_((int)piece)
    , fromRow_(move.from().row)
    , fromColumn_(move.from().column)
    , toRow_(move.to().row)
    , toColumn_(move.to().column)
{
}

Move SequenceEntry::GetMove() const
{
    return Move(Piece::piece(static_cast<PieceTypeId>(pieceTypeId_), static_cast<Color>(color_)), Position(fromRow_,
                                                                                                           fromColumn_),
                Position(toRow_, toColumn_));
}

Color SequenceEntry::GetColor() const
{
    return static_cast <Color>(color_);
}

PieceTypeId SequenceEntry::GetPieceTypeId() const
{
    return (pieceTypeId_ != 7) ? static_cast <PieceTypeId>(pieceTypeId_) : PieceTypeId::INVALID;
}
