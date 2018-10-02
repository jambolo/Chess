#include "Pawn.h"

#include "GameState.h"
#include "Move.h"

void Pawn::generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const
{
    Board const & board = state.board_;

    int direction       = (color_ == Color::BLACK) ? (int)Direction::DOWN : (int)Direction::UP;

    moves.reserve(moves.size() + MAX_POSSIBLE_MOVES);

    Position to;

    // Diagonal to left (must capture)

    to.row    = from.row + direction;
    to.column = from.column + (int)Direction::LEFT;

    if (board.isValidPosition(to))
    {
        Piece const * captured = board.pieceAt(to);

        if ((captured != NO_PIECE) && (captured->color() != color_))
        {
            moves.emplace_back(this, from, to, true);
        }
        else
        {
            // Note: If a piece can be captured normally, then it is impossible for a pawn to have moved two
            // spaces (and be open to capture by en passant).
            // Check en passant -- if the piece next to the destination square is a pawn and the previous move was
            // a two-space move to that square (assumed to be made by the pawn), then en passant is possible.

            captured = board.pieceAt(from.row, to.column);

            if ((captured != NO_PIECE) &&
                (captured->type() == PieceTypeId::PAWN) &&
                (state.move_.from() == Position(to.row + direction, to.column)) &&
                (state.move_.to() == Position(from.row, to.column)))
            {
                moves.emplace_back(Move::ENPASSANT, color_, from, to, true);
            }
        }
    }

    // Diagonal to right (must capture)

    to.row    = from.row + direction;
    to.column = from.column + (int)Direction::RIGHT;

    if (board.isValidPosition(to))
    {
        Piece const * captured = board.pieceAt(to);

        if ((captured != NO_PIECE) && (captured->color() != color_))
        {
            moves.emplace_back(this, from, to, (captured != NO_PIECE));
        }
        else
        {
            // Note: If a piece can be captured normally, then it is impossible for a pawn to have moved two
            // spaces (and be open to capture by en passant).
            // Check en passant -- if the piece next to the destination square is a pawn and the previous move was
            // a two-space move to that square (assumed to be made by the pawn), then en passant is possible.

            captured = board.pieceAt(from.row, to.column);

            if ((captured != NO_PIECE) &&
                (captured->type() == PieceTypeId::PAWN) &&
                (state.move_.from() == Position(to.row + direction, to.column)) &&
                (state.move_.to() == Position(from.row, to.column)))
            {
                moves.emplace_back(Move::ENPASSANT, color_, from, to, true);
            }
        }
    }

    // Ahead 1 row (must be empty) and ahead 2 rows (must be empty)

    to.row    = from.row + direction;
    to.column = from.column;

    if (board.isValidPosition(to) && (board.pieceAt(to) == NO_PIECE))
    {
        moves.emplace_back(this, from, to);

        // Ahead 2 rows if in its original spot (must be empty)

        if ((from.row == STARTING_ROW_BLACK) && (color_ == Color::BLACK) ||
            (from.row == STARTING_ROW_WHITE) && (color_ == Color::WHITE))
        {
            to.row += direction;
            if (board.pieceAt(to) == NO_PIECE)
            {
                moves.emplace_back(this, from, to);
            }
        }
    }
}

bool Pawn::isValidMove(GameState const & state, Move const & move) const
{
    Board const & board   = state.board_;
    Position const & from = move.from();
    Position const & to   = move.to();

    int direction         = (color_ == Color::BLACK) ? (int)Direction::DOWN : (int)Direction::UP;

    // Ahead 1?

    if ((to.row == from.row + direction) &&
        (to.column == from.column) &&
        (board.pieceAt(to) == NO_PIECE))
    {
        return true;
    }

    // Capture diagonally?

    if ((to.row == from.row + direction) &&
        ((to.column == from.column + (int)Direction::RIGHT) || (to.column == from.column + (int)Direction::LEFT)))
    {
        Piece const * captured = board.pieceAt(to);

        if ((captured != NO_PIECE) && (captured->color() != color_))
        {
            return true;
        }

        // Check en passant

        captured = board.pieceAt(from.row, to.column);

        if ((captured != NO_PIECE) &&
            (captured->type() == PieceTypeId::PAWN) &&
            (state.move_.from() == Position(to.row + direction, to.column)) &&
            (state.move_.to() == Position(from.row, to.column)))
        {
            return true;
        }
    }

    // Ahead 2?

    if ((to.row == from.row + 2 * direction) &&
        (to.column == from.column) &&
        (board.pieceAt(Position(from.row + direction, from.column)) == NO_PIECE) &&
        (board.pieceAt(to) == NO_PIECE))
    {
        return true;
    }

    return false;
}
