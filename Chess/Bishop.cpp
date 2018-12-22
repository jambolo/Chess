#include "Bishop.h"

#include "Board.h"
#include "GameState.h"
#include "Move.h"
#include "Types.h"

#include <BitBoard/BitBoard.h>

void Bishop::generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const
{
    Board const & board = state.board_;
    moves.reserve(moves.size() + MAX_POSSIBLE_MOVES);

#if defined(FEATURE_BITBOARD_MOVE_GENERATION)
    BitBoard destinations = BitBoard::destinations(BitBoard::BISHOP,
                                                   from.row,
                                                   from.column,
                                                   (color_ == Color::WHITE) ? board.white() : board.black(),
                                                   (color_ == Color::WHITE) ? board.black() : board.white());
#else // defined(FEATURE_BITBOARD_MOVE_GENERATION)
    generateSpanMoves(board, from, (int)Direction::UP, (int)Direction::RIGHT, moves);
    generateSpanMoves(board, from, (int)Direction::DOWN, (int)Direction::RIGHT, moves);
    generateSpanMoves(board, from, (int)Direction::DOWN, (int)Direction::LEFT, moves);
    generateSpanMoves(board, from, (int)Direction::UP, (int)Direction::LEFT, moves);
#endif // defined(FEATURE_BITBOARD_MOVE_GENERATION)
}

int Bishop::countPossibleMoves(GameState const & state, Position const & from) const
{
    Board const & board = state.board_;
    int           count = 0;
    count += countSpanMoves(board, from, (int)Direction::UP, (int)Direction::RIGHT);
    count += countSpanMoves(board, from, (int)Direction::DOWN, (int)Direction::RIGHT);
    count += countSpanMoves(board, from, (int)Direction::DOWN, (int)Direction::LEFT);
    count += countSpanMoves(board, from, (int)Direction::UP, (int)Direction::LEFT);
    return count;
}

int Bishop::countThreats(GameState const & state, Position const & from) const
{
    Board const & board = state.board_;
    int           count = 0;
    count += countSpanThreats(board, from, (int)Direction::UP, (int)Direction::RIGHT);
    count += countSpanThreats(board, from, (int)Direction::DOWN, (int)Direction::RIGHT);
    count += countSpanThreats(board, from, (int)Direction::DOWN, (int)Direction::LEFT);
    count += countSpanThreats(board, from, (int)Direction::UP, (int)Direction::LEFT);
    return count;
}

bool Bishop::isValidMove(GameState const & state, Move const & move) const
{
    Board const &    board = state.board_;
    Position const & from  = move.from();
    Position const & to    = move.to();

    // Check if the destination square can be occupied

    if (!state.canBeOccupied(to, color_))
        return false;

    // Check movement (at least 1 square diagonally)

    int dr = to.row - from.row;
    int dc = to.column - from.column;

    if (!Move::isMoved(dr, dc) || !Move::isDiagonal(dr, dc))
        return false;

    // Check for moving through a piece

    if (!board.spanIsEmpty(from, to))
        return false;

    return true;
}
