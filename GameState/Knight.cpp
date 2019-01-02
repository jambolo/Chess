#include "Knight.h"

#include "GameState.h"
#include "Move.h"

static Position const OFFSETS[] =
{
    { 2 * (int)Direction::UP,   1 * (int)Direction::LEFT  },
    { 2 * (int)Direction::UP,   1 * (int)Direction::RIGHT },
    { 1 * (int)Direction::UP,   2 * (int)Direction::RIGHT },
    { 1 * (int)Direction::DOWN, 2 * (int)Direction::RIGHT },
    { 2 * (int)Direction::DOWN, 1 * (int)Direction::LEFT  },
    { 2 * (int)Direction::DOWN, 1 * (int)Direction::RIGHT },
    { 1 * (int)Direction::UP,   2 * (int)Direction::LEFT  },
    { 1 * (int)Direction::DOWN, 2 * (int)Direction::LEFT  }
};

void Knight::generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const
{
    Board const & board = state.board_;

    moves.reserve(moves.size() + MAX_POSSIBLE_MOVES);

    for (auto const & o : OFFSETS)
    {
        Position to(from.row + o.row, from.column + o.column);
        if (board.isValidPosition(to) && state.canBeOccupied(to, color_))
            moves.emplace_back(this, from, to, (board.pieceAt(to) != nullptr));
    }
}

int Knight::countPossibleMoves(GameState const & state, Position const & from) const
{
    Board const & board = state.board_;
    int           count = 0;

    for (auto const & o : OFFSETS)
    {
        Position to(from.row + o.row, from.column + o.column);
        if (board.isValidPosition(to) && state.canBeOccupied(to, color_))
            ++count;
    }

    return count;
}

int Knight::countThreats(GameState const & state, Position const & from) const
{
    Board const & board = state.board_;
    int           count = 0;

    for (auto const & o : OFFSETS)
    {
        Position to(from.row + o.row, from.column + o.column);
        if (board.isValidPosition(to) && board.pieceAt(to))
            ++count;
    }

    return count;
}

bool Knight::isValidMove(GameState const & state, Move const & move) const
{
    Position const & from = move.from();
    Position const & to   = move.to();

    // Check if the destination square can be occupied
    if (!state.canBeOccupied(to, color_))
        return false;

    // Check movement

    int adr = abs(to.row - from.row);
    int adc = abs(to.column - from.column);

    if (!Move::isMoved(adr, adc))
        return false;

    if (((adr != 2) || (adc != 1)) && ((adc != 2) || (adr != 1)))
        return false;

    return true;
}
