#include "King.h"

#include "GameState.h"
#include "Move.h"

void King::generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const
{
    Board const & board = state.board_;

    moves.reserve(moves.size() + MAX_POSSIBLE_MOVES);

    Position to = from;

    // Up

    --to.row;
    if ((to.row >= 0) && state.canBeOccupied(to, color_))
    {
        moves.emplace_back(this, from, to, (board.pieceAt(to) != nullptr));
    }

    // Up-right

    ++to.column;
    if ((to.row >= 0) && (to.column < Board::SIZE) && state.canBeOccupied(to, color_))
    {
        moves.emplace_back(this, from, to, (board.pieceAt(to) != nullptr));
    }

    // Right

    ++to.row;
    if ((to.column < Board::SIZE) && state.canBeOccupied(to, color_))
    {
        moves.emplace_back(this, from, to, (board.pieceAt(to) != nullptr));
    }

    // Down-right

    ++to.row;
    if ((to.row < Board::SIZE) && (to.column < Board::SIZE) && state.canBeOccupied(to, color_))
    {
        moves.emplace_back(this, from, to, (board.pieceAt(to) != nullptr));
    }

    // Down

    --to.column;
    if ((to.row < Board::SIZE) && state.canBeOccupied(to, color_))
    {
        moves.emplace_back(this, from, to, (board.pieceAt(to) != nullptr));
    }

    // Down-left

    --to.column;
    if ((to.row < Board::SIZE) && (to.column >= 0) && state.canBeOccupied(to, color_))
    {
        moves.emplace_back(this, from, to, (board.pieceAt(to) != nullptr));
    }

    // Left

    --to.row;
    if ((to.column >= 0) && state.canBeOccupied(to, color_))
    {
        moves.emplace_back(this, from, to, (board.pieceAt(to) != nullptr));
    }

    // Up-left

    --to.row;
    if ((to.row >= 0) && (to.column >= 0) && state.canBeOccupied(to, color_))
    {
        moves.emplace_back(this, from, to, (board.pieceAt(to) != nullptr));
    }

    // Castles

    if (state.kingSideCastleIsAllowed(color_) && !state.inCheck_)
    {
        //! @todo Must check all squares that the kings moves through
        moves.emplace_back(Move::KINGSIDE_CASTLE, color_);
    }

    if (state.queenSideCastleIsAllowed(color_) && !state.inCheck_)
    {
        //! @todo Must check all squares that the kings moves through
        moves.emplace_back(Move::QUEENSIDE_CASTLE, color_);
    }
}

bool King::isValidMove(GameState const & state, Move const & move) const
{
    Board const & board   = state.board_;
    Position const & from = move.from();
    Position const & to   = move.to();

    // Check castles

    if (move.isKingSideCastle())
    {
        //! @todo Must check all squares that the kings moves through
        return state.kingSideCastleIsAllowed(color_) && !state.inCheck_;
    }

    if (move.isQueenSideCastle())
    {
        //! @todo Must check all squares that the kings moves through
        return state.queenSideCastleIsAllowed(color_) && !state.inCheck_;
    }

    // Check if the destination square can be occupied

    if (!state.canBeOccupied(to, color_))
        return false;

    // Check movement (exactly one square in any direction)

    int dr = to.row - from.row;
    int dc = to.column - from.column;

    if (!Move::isMoved(dr, dc) || !Move::inRange(dr, dc, 1))
        return false;

    return true;
}
