#include "Queen.h"

#include "GameState.h"
#include "Move.h"

void Queen::generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const
{
    Board const & board = state.board_;

    moves.reserve(moves.size() + MAX_POSSIBLE_MOVES);

    generateSpanMoves(board, from, (int)Direction::UP,   0,                     moves);
    generateSpanMoves(board, from, (int)Direction::UP,   (int)Direction::RIGHT, moves);
    generateSpanMoves(board, from, 0,                    (int)Direction::RIGHT, moves);
    generateSpanMoves(board, from, (int)Direction::DOWN, (int)Direction::RIGHT, moves);
    generateSpanMoves(board, from, (int)Direction::DOWN, 0,                     moves);
    generateSpanMoves(board, from, (int)Direction::DOWN, (int)Direction::LEFT,  moves);
    generateSpanMoves(board, from, 0,                    (int)Direction::LEFT,  moves);
    generateSpanMoves(board, from, (int)Direction::UP,   (int)Direction::LEFT,  moves);
}

int Queen::countPossibleMoves(GameState const & state, Position const & from) const
{
    Board const & board = state.board_;
    int           count = 0;
    count += countSpanMoves(board, from, (int)Direction::UP, 0);
    count += countSpanMoves(board, from, (int)Direction::UP, (int)Direction::RIGHT);
    count += countSpanMoves(board, from, 0, (int)Direction::RIGHT);
    count += countSpanMoves(board, from, (int)Direction::DOWN, (int)Direction::RIGHT);
    count += countSpanMoves(board, from, (int)Direction::DOWN, 0);
    count += countSpanMoves(board, from, (int)Direction::DOWN, (int)Direction::LEFT);
    count += countSpanMoves(board, from, 0, (int)Direction::LEFT);
    count += countSpanMoves(board, from, (int)Direction::UP, (int)Direction::LEFT);
    return count;
}

int Queen::countThreats(GameState const & state, Position const & from) const
{
    Board const & board = state.board_;
    int           count = 0;
    count += countSpanThreats(board, from, (int)Direction::UP, 0);
    count += countSpanThreats(board, from, (int)Direction::UP, (int)Direction::RIGHT);
    count += countSpanThreats(board, from, 0, (int)Direction::RIGHT);
    count += countSpanThreats(board, from, (int)Direction::DOWN, (int)Direction::RIGHT);
    count += countSpanThreats(board, from, (int)Direction::DOWN, 0);
    count += countSpanThreats(board, from, (int)Direction::DOWN, (int)Direction::LEFT);
    count += countSpanThreats(board, from, 0, (int)Direction::LEFT);
    count += countSpanThreats(board, from, (int)Direction::UP, (int)Direction::LEFT);
    return count;
}

bool Queen::isValidMove(GameState const & state, Move const & move) const
{
    Board const &    board = state.board_;
    Position const & from  = move.from();
    Position const & to    = move.to();

    // Check if the destination square can be occupied

    if (!state.canBeOccupied(to, color_))
        return false;

    // Check movement (at least 1 square in 8 directions)

    int dr = to.row - from.row;
    int dc = to.column - from.column;

    if (!Move::isMoved(dr, dc) || (!Move::isDiagonal(dr, dc) && !Move::isSquare(dr, dc)))
        return false;

    // Check for moving through a piece

    if (!board.spanIsEmpty(from, to))
        return false;

    return true;
}
