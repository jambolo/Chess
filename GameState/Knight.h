#pragma once

#if !defined(Knight_h__)
#define Knight_h__

#include "Piece.h"

class Knight : public Piece
{
public:

    explicit Knight(Color c) : Piece(PieceTypeId::KNIGHT, c) {}

    // Generates all legal moves for this piece
    virtual void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const override;

    // Returns true if the move is valid
    virtual bool isValidMove(GameState const & state, Move const & move) const override;
};

#endif // !defined(Knight_h__)
