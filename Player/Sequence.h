#pragma once

#if !defined(Sequence_h__)
#define Sequence_h__

#include "gamestate/Move.h"
#include <vector>

class SequenceEntry
{
public:

    SequenceEntry();
    SequenceEntry(Color color, PieceTypeId piece, Move const & move);

    Move GetMove() const;
    Color GetColor() const;
    PieceTypeId GetPieceTypeId() const;

    uint16_t color_        : 1;
    uint16_t pieceTypeId_  : 3; // Note: 7 is PieceTypeId::INVALID
    uint16_t fromRow_      : 3;
    uint16_t fromColumn_   : 3;
    uint16_t toRow_        : 3;
    uint16_t toColumn_     : 3;
};

typedef std::vector<SequenceEntry> Sequence;

#include "Sequence.inl"

#endif // !defined(Sequence_h__)
