#if !defined(CHESS_PAWN_H)
#define CHESS_PAWN_H

#pragma once

#include "Piece.h"

class Pawn : public Piece
{
public:

    static int constexpr STARTING_ROW_BLACK = 1;
    static int constexpr STARTING_ROW_WHITE = 6;

    explicit Pawn(Color c) : Piece(PieceTypeId::PAWN, c) {}

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
    static int constexpr MAX_POSSIBLE_MOVES = 6; // The maximum number of possible moves for a pawn
};

#endif // !defined(CHESS_PAWN_H)
