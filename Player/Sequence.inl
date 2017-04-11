#pragma once

#if !defined(Sequence_inl__)
#define Sequence_inl__

#include "Sequence.h"

inline SequenceEntry:: SequenceEntry()
    : pieceTypeId_(-1)
{
}

inline SequenceEntry:: SequenceEntry(Color color, PieceTypeId piece, Move const & move)
    : color_( (int)color)
    , pieceTypeId_((int)piece)
    , fromRow_(move.from().row)
    , fromColumn_(move.from().column)
    , toRow_(move.to().row)
    , toColumn_(move.to().column)
{
}

inline Move SequenceEntry:: GetMove() const
{
    return Move(Position(fromRow_, fromColumn_), Position(toRow_, toColumn_) );
}

inline Color SequenceEntry:: GetColor() const
{
    return static_cast <Color> ( color_ );
}

inline PieceTypeId SequenceEntry:: GetPieceTypeId() const
{
    return ( pieceTypeId_ != 7 ) ? static_cast <PieceTypeId> ( pieceTypeId_ ) : PieceTypeId:: INVALID;
}

#endif // !defined(Sequence_inl__)
