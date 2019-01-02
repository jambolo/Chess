#include "Rook.h"

#include "GameState.h"
#include "Move.h"

void Rook::generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const
{
    Board const & board = state.board_;
    moves.reserve(moves.size() + MAX_POSSIBLE_MOVES);
    generateSpanMoves(board, from, (int)Direction::UP,   0, moves); // up
    generateSpanMoves(board, from, (int)Direction::DOWN, 0, moves); // down
    generateSpanMoves(board, from, 0, (int)Direction::LEFT, moves); // left
    generateSpanMoves(board, from, 0, (int)Direction::RIGHT, moves); // right
}

int Rook::countPossibleMoves(GameState const & state, Position const & from) const
{
    Board const & board = state.board_;
    int           count = 0;
    count += countSpanMoves(board, from, (int)Direction::UP, 0); // up
    count += countSpanMoves(board, from, (int)Direction::DOWN, 0); // down
    count += countSpanMoves(board, from, 0, (int)Direction::LEFT); // left
    count += countSpanMoves(board, from, 0, (int)Direction::RIGHT); // right
    return count;
}

int Rook::countThreats(GameState const & state, Position const & from) const
{
    Board const & board = state.board_;
    int           count = 0;
    count += countSpanThreats(board, from, (int)Direction::UP, 0); // up
    count += countSpanThreats(board, from, (int)Direction::DOWN, 0); // down
    count += countSpanThreats(board, from, 0, (int)Direction::LEFT); // left
    count += countSpanThreats(board, from, 0, (int)Direction::RIGHT); // right
    return count;
}

bool Rook::isValidMove(GameState const & state, Move const & move) const
{
    Board const &    board = state.board_;
    Position const & from  = move.from();
    Position const & to    = move.to();

    // Check if the destination square can be occupied

    if (!state.canBeOccupied(to, color_))
        return false;

    // Check movement (at least 1 square in 4 directions)

    int dr = to.row - from.row;
    int dc = to.column - from.column;

    if (!Move::isMoved(dr, dc) || !Move::isSquare(dr, dc))
        return false;

    // Check for moving through a piece (note: this doesn't check for castling)

    if (!board.spanIsEmpty(from, to))
        return false;

    return true;
}
