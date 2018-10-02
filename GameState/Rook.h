#pragma once

#if !defined(Rook_h__)
#define Rook_h__

#include "Piece.h"

class Rook : public Piece
{
public:

    Rook(Color c) : Piece(PieceTypeId::ROOK, c) {}

    // Generates all legal moves for this piece
    void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const override;

    // Returns true if the move is valid
    bool isValidMove(GameState const & state, Move const & move) const override;
};

#endif // !defined(Rook_h__)
