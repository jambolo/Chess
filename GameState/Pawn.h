#pragma once

#if !defined(Pawn_h__)
#define Pawn_h__

#include "Piece.h"

class Pawn : public Piece
{
public:

    static int constexpr STARTING_ROW_BLACK = 1;
    static int constexpr STARTING_ROW_WHITE = 6;

    Pawn(Color c) : Piece(PieceTypeId::PAWN, c) {}

    // Generates all legal moves for this piece
    void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const override;

    // Returns true if the move is valid
    bool isValidMove(GameState const & state, Move const & move) const override;
};

#endif // !defined(Pawn_h__)
