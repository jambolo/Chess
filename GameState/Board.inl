#pragma once

#include "Board.h"

#include "gamestate/Piece.h"
#include "gamestate/PieceList.h"

#include <cassert>

inline void Board::clear()
{
    assert((int)NO_PIECE == (int)0);
    memset(board_, 0, sizeof board_);
}

inline bool Board::isValidPosition(Position const & p)
{
    return p.m_Row >= 0 &&
           p.m_Row < SIZE &&
           p.m_Column >= 0 &&
           p.m_Column < SIZE;
}

inline Piece const * Board::pieceAt(Position const & p) const
{
    return pieceAt(p.m_Row, p.m_Column);
}

inline Piece const * Board::pieceAt(int r, int c) const
{
    return board_[r][c];
}

inline void Board::putPiece(Piece const * id, Position const & p)
{
    // Place the piece
    board_[p.m_Row][p.m_Column] = id;
}

inline void Board::putPiece(Piece const * piece, Position const & p)
{
    // Place the piece
    putPiece(piece, p);
}

inline void Board::removePiece(Position const & p)
{
    // Remove the piece
    removePiece(p.m_Row, p.m_Column);
}

inline void Board::removePiece(int r, int c)
{
    // Remove the piece
    board_[r][c] = NO_PIECE;
}

inline void Board::movePiece(Position const & from, Position const & to)
{
    // Move the piece
    board_[to.m_Row][to.m_Column]     = board_[from.m_Row][from.m_Column];
    board_[from.m_Row][from.m_Column] = NO_PIECE;
}

inline bool operator ==(Board const & x, Board const & y)
{
    return memcmp(x.board_, y.board_, sizeof x.board_) == 0;
}
