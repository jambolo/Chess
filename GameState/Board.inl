#pragma once

#if !defined(Board_inl__)
#define Board_inl__

#include "Board.h"

#include "GameState/Piece.h"
#include "GameState/PieceList.h"

inline void Board::clear()
{
    memset(board_, (int)EMPTY_SQUARE, sizeof board_);
}

inline bool Board::isValidPosition(Position const & p)
{
    return p.row >= 0 &&
           p.row < SIZE &&
           p.column >= 0 &&
           p.column < SIZE;
}

inline Board::PieceId Board::pieceIdAt(Position const & p) const
{
    return pieceIdAt(p.row, p.column);
}

inline Board::PieceId Board::pieceIdAt(int r, int c) const
{
    return board_[r][c];
}

inline Piece const * Board::pieceAt(Position const & p) const
{
    return piece(pieceIdAt(p));
}

inline Piece const * Board::pieceAt(int r, int c) const
{
    return piece(pieceIdAt(r, c));
}

inline void Board::putPiece(PieceId id, Position const & p)
{
    // Place the piece
    board_[p.row][p.column] = id;
}

inline void Board::putPiece(Piece const * piece, Position const & p)
{
    // Place the piece
    putPiece(pieceId(piece), p);
}

inline void Board::removePiece(Position const & p)
{
    // Remove the piece
    board_[p.row][p.column] = EMPTY_SQUARE;
}

inline void Board::movePiece(Position const & from, Position const & to)
{
    // Move the piece
    board_[to.row][to.column]     = board_[from.row][from.column];
    board_[from.row][from.column] = EMPTY_SQUARE;
}

inline bool operator == (Board const & x, Board const & y)
{
    return memcmp(x.board_, y.board_, sizeof x.board_) == 0;
}

inline Board::PieceId Board::pieceId(PieceTypeId type, Color color)
{
    return (int)color * NUMBER_OF_PIECE_TYPES + (int)type + 1;
}

inline Piece const * Board::piece(PieceId piece)
{
    return pieces_[piece];
}

#endif // !defined(Board_inl__)
