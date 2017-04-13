#pragma once

#if !defined(Board_inl__)
#define Board_inl__

#include "Board.h"

#include "gamestate/Piece.h"
#include "gamestate/PieceList.h"
#include "gamestate/Position.h"

#include <cassert>

inline void Board::clear()
{
    assert(NO_PIECE == 0);
    memset(board_, 0, sizeof board_);
}

inline bool Board::isValidPosition(Position const & p)
{
    return p.row >= 0 &&
           p.row < SIZE &&
           p.column >= 0 &&
           p.column < SIZE;
}

inline Piece const * Board::pieceAt(Position const & p) const
{
    return pieceAt(p.row, p.column);
}

inline Piece const * Board::pieceAt(int r, int c) const
{
    return board_[r][c];
}

inline void Board::putPiece(Piece const * piece, Position const & p)
{
    putPiece(piece, p.row, p.column);
}

inline void Board::putPiece(Piece const * piece, int r, int c)
{
    board_[r][c] = piece;
}

inline void Board::removePiece(Position const & p)
{
    removePiece(p.row, p.column);
}

inline void Board::removePiece(int r, int c)
{
    board_[r][c] = NO_PIECE;
}

inline void Board::movePiece(Position const & from, Position const & to)
{
    board_[to.row][to.column]     = board_[from.row][from.column];
    board_[from.row][from.column] = NO_PIECE;
}

inline bool operator ==(Board const & x, Board const & y)
{
    return memcmp(x.board_, y.board_, sizeof x.board_) == 0;
}

#endif // !defined(Board_inl__)
