#if !defined(CHESS_QUEEN_H)
#define CHESS_QUEEN_H

#pragma once

#include "Piece.h"

class Queen : public Piece
{
public:

    explicit Queen(Color c) : Piece(PieceTypeId::QUEEN, c) {}

    //!@{
    //! @name Overrides Piece

    // Generates all legal moves for this piece
    virtual void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const override;

    // Counts the number of legal moves for this piece
    virtual int countPossibleMoves(GameState const & state, Position const & from) const override;

    // Counts the number of legal moves for this piece
    virtual int countThreats(GameState const & state, Position const & from) const override;

    // Returns true if the move is valid
    virtual bool isValidMove(GameState const & state, Move const & move) const override;

    //!@}

private:
    static int constexpr MAX_POSSIBLE_MOVES = 28; // The maximum number of possible moves for a queen
};

#endif // !defined(CHESS_QUEEN_H)
