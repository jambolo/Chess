#pragma once

#if !defined(Queen_h__)
#define Queen_h__

#include "Piece.h"

class Queen : public Piece
{
public:

    Queen(Color c) : Piece(PieceTypeId::QUEEN, c) {}

    // Generates all legal moves for this piece
    virtual void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const override;

    // Returns true if the move is valid
    virtual bool isValidMove(GameState const & state, Move const & move) const override;
};

#endif // !defined(Queen_h__)
