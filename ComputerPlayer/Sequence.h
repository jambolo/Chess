#pragma once

#if !defined(Sequence_h__)
#define Sequence_h__

#include "GameState/Move.h"
#include <vector>

class SequenceEntry
{
public:

    SequenceEntry();
    SequenceEntry(Color color, PieceTypeId piece, Move const & move);

    Move        move() const;
    Color       color() const;
    PieceTypeId piece() const;
    
private:
    uint16_t color_        : 1;
    uint16_t pieceTypeId_  : 3; // Note: 7 is PieceTypeId::INVALID
    uint16_t fromRow_      : 3;
    uint16_t fromColumn_   : 3;
    uint16_t toRow_        : 3;
    uint16_t toColumn_     : 3;
};

using Sequence = std::vector<SequenceEntry>;

#endif // !defined(Sequence_h__)
