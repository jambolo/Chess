#pragma once

#if !defined(Bishop_h__)
#define Bishop_h__

#include "Piece.h"

class Bishop : public Piece
{
public:

    explicit Bishop(Color c) : Piece(PieceTypeId::BISHOP, c) {}

    // Generates all legal moves for this piece
    virtual void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const override;

    // Returns true if the move is valid
    virtual bool isValidMove(GameState const & state, Move const & move) const override;
};

#endif // !defined(Bishop_h__)
