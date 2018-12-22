#pragma once

#if !defined(Pawn_h__)
#define Pawn_h__

#include "Piece.h"

class Pawn : public Piece
{
public:

    static int constexpr STARTING_ROW_BLACK = 1;
    static int constexpr STARTING_ROW_WHITE = 6;

    explicit Pawn(Color c) : Piece(PieceTypeId::PAWN, c) {}

    // Generates all legal moves for this piece
    virtual void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const override;

    // Returns true if the move is valid
    virtual bool isValidMove(GameState const & state, Move const & move) const override;

private:
    static int constexpr MAX_POSSIBLE_MOVES = 6; // The maximum number of possible moves for a pawn
};

#endif // !defined(Pawn_h__)
