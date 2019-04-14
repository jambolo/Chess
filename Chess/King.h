#if !defined(CHESS_KING_H)
#define CHESS_KING_H

#pragma once

#include "Piece.h"

class King : public Piece
{
public:

    explicit King(Color c) : Piece(PieceTypeId::KING, c) {}

    //!@{
    //! @name Overrides Piece

    // Generates all legal moves for this piece
    void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const override;

    // Counts the number of legal moves for this piece
    int countPossibleMoves(GameState const & state, Position const & from) const override;

    // Counts the number of legal moves for this piece
    int countThreats(GameState const & state, Position const & from) const override;

    // Returns true if the move is valid
    bool isValidMove(GameState const & state, Move const & move) const override;

    //!@}

private:
    static int constexpr MAX_POSSIBLE_MOVES = 10; // The maximum number of possible moves for a king
};

#endif // !defined(CHESS_KING_H)
