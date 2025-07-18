/*
   Copyright(c) 2004 John J.Bolton

   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
   (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify,
   merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
   LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#if !defined(BitBoard_h__)
#define BitBoard_h__

#include <algorithm>

struct Position;

class BitBoard
{
public:

    static int constexpr SQUARES_PER_ROW    = 8;
    static int constexpr SQUARES_PER_COLUMN = 8;
    static int constexpr SIZE               = SQUARES_PER_ROW * SQUARES_PER_COLUMN;
    static unsigned constexpr ROW_MASK      = (1 << SQUARES_PER_ROW) - 1;



    enum Piece
    {
        INVALID = -1,
        KING    = 0,
        QUEEN,
        BISHOP,
        KNIGHT,
        ROOK,
        PAWN
    };
    static int constexpr NUMBER_OF_PIECES = PAWN - KING + 1;

    explicit BitBoard(uint64_t i = 0) : board_(i) {}

    //! Converts the bitboard to a uint64_t
    operator uint64_t() const { return board_; }

    //! Sets the value of the specified square to 1
    void set(int r, int c) { board_ |= mask(r, c); }

    //! Sets the value of the specified square to 0
    void clear(int r, int c) { board_ &= ~mask(r, c); }

    //! Returns the value of the specified square
    int test(int r, int c) const { return int((board_ >> index(r, c)) & 1); }

    //! Reflects the bitboard vertically (for dealing with white pawns)
    void flip();

    //! Returns a BitBoard showing all squares threatened by a piece at a position, assuming it isn't blocked.
    //!
    //! This function returns a BitBoard showing all squares on an empty board that the specified piece can attack from
    //! the specified position.
    //!
    //! @param	type		    Type of piece to be moved
    //! @param  r               The piece's row
    //! @param  c               The piece's column
    //!
    //! @return	BitBoard showing all threatened squares
    //!
    //! @note	En passant is included.
    //! @note   Assumes playing black (which only matters for pawns)

    static BitBoard threatened(int type, int r, int c);

    //! Returns a BitBoard showing all squares threatened by a piece at a position.
    //!
    //! This function returns a BitBoard showing all squares that the specified piece can attack from the specified
    //! position, given the locations of other pieces on the board. This includes squares occupied by friendly pieces.
    //!
    //! @param  type			Type of piece to be moved
    //! @param  r               The piece's row
    //! @param  c               The piece's column
    //! @param	friends         The positions of friendly pieces
    //! @param	foes			The positions of enemy pieces
    //!
    //! @return	BitBoard showing all threatened squares
    //!
    //! @note	En passant is included.
    //! @note   Assumes playing black (which only matters for pawns)

    static BitBoard threatened(int type, int r, int c, const BitBoard& friends, const BitBoard& foes);

    //! Returns a BitBoard showing all squares that a piece at a position can move to, assuming it isn't blocked.
    //!
    //! This function returns a BitBoard showing all squares on an empty board that the specified piece can move to
    //! from the specified position.
    //!
    //! @param  type            Type of piece to be moved
    //! @param  r               The piece's row
    //! @param  c               The piece's column
    //!
    //! @return	BitBoard showing all possible destination squares
    //!
    //! @note   Pawn capture, including en passant, is not considered.
    //! @note   Castling is not considered
    //! @note   Assumes playing black (which only matters for pawns)

    static BitBoard destinations(int type, int r, int c);

    //! Returns a BitBoard showing all squares that a piece at a position can move to.
    //!
    //! This function returns a BitBoard showing all squares that the specified piece can move to from the specified
    //! position, given the other pieces on the board. Capturing moves are included.
    //!
    //! @param  type            Type of piece to be moved
    //! @param  r               The pawn's row
    //! @param  c               The pawn's column
    //! @param	friends         The positions of friendly pieces
    //! @param	foes            The positions of enemy pieces
    //!
    //! @return	BitBoard showing all possible destination squares
    //!
    //! @note   Pawn capture, including en passant, is not considered.
    //! @note   Castling is not considered
    //! @note   Assumes playing black (which only matters for pawns)

    static BitBoard destinations(int type, int r, int c, const BitBoard& friends, const BitBoard& foes);

private:

    // Returns the index for the given row and column
    static int index(int r, int c) { return r * SQUARES_PER_ROW + c; }

    // Returns the row and column for the given index
    static void rowAndColumn(int index, int & r, int & c) { r = index / SQUARES_PER_ROW; c = index % SQUARES_PER_ROW; }

    // Returns a mask for the square at the given row and column
    static uint64_t mask(int r, int c) { return (uint64_t)1 << index(r, c); }

    // Returns the contents of the given row
    unsigned row(int r) const { return unsigned((board_ >> (SQUARES_PER_ROW * r)) & ROW_MASK); }

    // Returns the contents of the given column
    unsigned column(int c) const;

    uint64_t board_;
};

#endif // !defined(BitBoard_h__)
