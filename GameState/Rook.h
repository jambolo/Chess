#pragma once

#if !defined(Rook_h__)
#define Rook_h__

#include "Piece.h"

class Rook : public Piece
{
public:

    explicit Rook(Color c) : Piece(PieceTypeId::ROOK, c) {}

    // Generates all legal moves for this piece
    virtual void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const override;

    // Counts the number of legal moves for this piece
    virtual int countPossibleMoves(GameState const & state, Position const & from) const override;

    // Returns true if the move is valid
    virtual bool isValidMove(GameState const & state, Move const & move) const override;

private:
    static int constexpr MAX_POSSIBLE_MOVES = 10; // The maximum number of possible moves for a rook
};

#endif // !defined(Rook_h__)
