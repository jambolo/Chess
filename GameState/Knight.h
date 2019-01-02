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

    // Counts the number of legal moves for this piece
    virtual int countPossibleMoves(GameState const & state, Position const & from) const override;

    // Returns true if the move is valid
    virtual bool isValidMove(GameState const & state, Move const & move) const override;

private:
    static int constexpr MAX_POSSIBLE_MOVES = 8; // The maximum number of possible moves for a knight
};

#endif // !defined(Knight_h__)
