#include "Board.h"

#include "Move.h"
#include "Piece.h"
#include "PieceList.h"

#include "Misc/Etc.h"

#include <algorithm>

Piece const * Board::pieces_[1 + NUMBER_OF_COLORS * NUMBER_OF_PIECE_TYPES];

void Board::initialize(PieceList const & whitePieces, PieceList const & blackPieces)
{
    clear();

    pieces_[0] = NO_PIECE;
    for (int i = 0; i < NUMBER_OF_PIECE_TYPES; ++i) {
        pieces_[i + 1] = whitePieces[static_cast<PieceTypeId>(i)];
    }
    for (int i = 0; i < NUMBER_OF_PIECE_TYPES; ++i) {
        pieces_[i + NUMBER_OF_PIECE_TYPES + 1] = blackPieces[static_cast<PieceTypeId>(i)];
    }

    putPiece(pieceId(PieceTypeId::ROOK, Color::BLACK), Position(0, 0));
    putPiece(pieceId(PieceTypeId::KNIGHT, Color::BLACK), Position(0, 1));
    putPiece(pieceId(PieceTypeId::BISHOP, Color::BLACK), Position(0, 2));
    putPiece(pieceId(PieceTypeId::QUEEN, Color::BLACK), Position(0, 3));
    putPiece(pieceId(PieceTypeId::KING, Color::BLACK), Position(0, 4));
    putPiece(pieceId(PieceTypeId::BISHOP, Color::BLACK), Position(0, 5));
    putPiece(pieceId(PieceTypeId::KNIGHT, Color::BLACK), Position(0, 6));
    putPiece(pieceId(PieceTypeId::ROOK, Color::BLACK), Position(0, 7));
    putPiece(pieceId(PieceTypeId::PAWN, Color::BLACK), Position(1, 0));
    putPiece(pieceId(PieceTypeId::PAWN, Color::BLACK), Position(1, 1));
    putPiece(pieceId(PieceTypeId::PAWN, Color::BLACK), Position(1, 2));
    putPiece(pieceId(PieceTypeId::PAWN, Color::BLACK), Position(1, 3));
    putPiece(pieceId(PieceTypeId::PAWN, Color::BLACK), Position(1, 4));
    putPiece(pieceId(PieceTypeId::PAWN, Color::BLACK), Position(1, 5));
    putPiece(pieceId(PieceTypeId::PAWN, Color::BLACK), Position(1, 6));
    putPiece(pieceId(PieceTypeId::PAWN, Color::BLACK), Position(1, 7));

    putPiece(pieceId(PieceTypeId::ROOK, Color::WHITE), Position(7, 0));
    putPiece(pieceId(PieceTypeId::KNIGHT, Color::WHITE), Position(7, 1));
    putPiece(pieceId(PieceTypeId::BISHOP, Color::WHITE), Position(7, 2));
    putPiece(pieceId(PieceTypeId::QUEEN, Color::WHITE), Position(7, 3));
    putPiece(pieceId(PieceTypeId::KING, Color::WHITE), Position(7, 4));
    putPiece(pieceId(PieceTypeId::BISHOP, Color::WHITE), Position(7, 5));
    putPiece(pieceId(PieceTypeId::KNIGHT, Color::WHITE), Position(7, 6));
    putPiece(pieceId(PieceTypeId::ROOK, Color::WHITE), Position(7, 7));
    putPiece(pieceId(PieceTypeId::PAWN, Color::WHITE), Position(6, 0));
    putPiece(pieceId(PieceTypeId::PAWN, Color::WHITE), Position(6, 1));
    putPiece(pieceId(PieceTypeId::PAWN, Color::WHITE), Position(6, 2));
    putPiece(pieceId(PieceTypeId::PAWN, Color::WHITE), Position(6, 3));
    putPiece(pieceId(PieceTypeId::PAWN, Color::WHITE), Position(6, 4));
    putPiece(pieceId(PieceTypeId::PAWN, Color::WHITE), Position(6, 5));
    putPiece(pieceId(PieceTypeId::PAWN, Color::WHITE), Position(6, 6));
    putPiece(pieceId(PieceTypeId::PAWN, Color::WHITE), Position(6, 7));
}

bool Board::spanIsEmpty(Position const & from, Position const & to) const
{
    // Get the distance in rows and columns of the move (sign determines direction)

    int row_move    = to.row - from.row;
    int column_move = to.column - from.column;

    // Determine the row and column increment for each square tested. If the
    // distance moved in a direction is < 0, the increment is -1, if the distance
    // is > 0, then the increment is 1, otherwise the increment is 0.

    int row_delta    = (row_move < 0) ? -1 : (row_move == 0) ? 0 : 1;
    int column_delta = (column_move < 0) ? -1 : (column_move == 0) ? 0 : 1;

    // Determine the number of squares moved (excluding the last square)

    int squares_moved = std::max(abs(row_move), abs(column_move)) - 1;

    // Check every square except the first one. If there is a piece occupying a
    // square, then return false.

    Position p = from;

    for (int i = 0; i < squares_moved; ++i) {
        p.row    += row_delta;
        p.column += column_delta;

        if (pieceAt(p) != NO_PIECE)
            return false;
    }

    return true;
}

Board::PieceId Board::pieceId(Piece const * piece)
{
    // Search for the piece in the list of pieces

    for (int i = 0; i < elementsof(pieces_); ++i) {
        if (pieces_[i] == piece) {
            return i;
        }
    }

    return EMPTY_SQUARE;
//		// For now refs are just the pointers to pieces
//
//	return reinterpret_cast< PieceId >( piece );
}
