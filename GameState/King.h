#pragma once

#if !defined(King_h__)
#define King_h__

#include "Piece.h"

class King : public Piece
{
public:

    explicit King(Color c) : Piece(PieceTypeId::KING, c) {}

    // Generates all legal moves for this piece
    virtual void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const override;

    // Returns true if the move is valid
    virtual bool isValidMove(GameState const & state, Move const & move) const override;

private:
    static int constexpr MAX_POSSIBLE_MOVES = 10; // The maximum number of possible moves for a king
};

#endif // !defined(King_h__)
