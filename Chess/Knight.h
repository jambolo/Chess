#if !defined(CHESS_KNIGHT_H)
#define CHESS_KNIGHT_H

#pragma once

#include "Piece.h"

class Knight : public Piece
{
public:

    explicit Knight(Color c) : Piece(PieceTypeId::KNIGHT, c) {}

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
    static int constexpr MAX_POSSIBLE_MOVES = 8; // The maximum number of possible moves for a knight
};

#endif // !defined(CHESS_KNIGHT_H)
