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

#include "GameState/ChessTypes.h"
#include <algorithm>

struct Position;

class BitBoard
{
public:

    // Number of squares on a bit board
    static int const NUMBER_OF_SQUARES = 64;

    BitBoard() = default;

    explicit BitBoard(uint64_t i) : board_(i) {}

    //! Converts the bitboard to a uint64_t
    operator uint64_t() const { return board_; }

    //! Sets the value of the specified square to 1
    BitBoard set(int r, int c)
    {
        board_ |= mask(r, c); return *this;
    }

    //! Sets the value of the specified square to 0
    BitBoard clear(int r, int c)
    {
        board_ &= ~mask(r, c); return *this;
    }

    //! Returns the value of the specified square
    int test(int r, int c) const
    {
        return int((board_ >> index(r, c)) & 1);
    }

    //! Returns the contents of the given row
    unsigned row(int r) const
    {
        return unsigned((board_ >> (8*r)) & 0xff);
    }

    //! Returns the contents of the given column
    unsigned column(int c) const;

    //! Reflects the bitboard vertically
    BitBoard const & flip();

    //! Reflects the bitboard horizontally
    BitBoard const & mirror();

    //! Shifts the bitboard up to 7 squares to the right
    BitBoard const & shiftRight(int n = 1)
    {
        board_ <<= n; board_ &= rightMask(8 - n);
        return *this;
    }

    //! Rotates the bitboard up to 7 squares to the right
    BitBoard const & rotateRight(int n = 1)
    {
        uint64_t wrap = board_ & rightMask(n);
        shiftRight(n);
        board_ |= wrap >> (8 - n);
        return *this;
    }

    //! Shifts the bitboard up to 7 squares up and right
    BitBoard const & shiftUR(int n = 1)
    {
        board_ <<= 9 * n; board_ &= rightMask(8 - n);
        return *this;
    }

    //! Rotates the bitboard up to 7 squares up and right
    BitBoard const & rotateUR(int n = 1)
    {
        rotateUp(n); rotateRight(n);
        return *this;
    }

    //! Shifts the bitboard up to 7 squares up
    BitBoard const & shiftUp(int n = 1)
    {
        board_ <<= 8 * n;
        return *this;
    }

    //! Rotates the bitboard up to 7 squares up
    BitBoard const & rotateUp(int n = 1)
    {
        uint64_t wrap = board_ & upperMask(1); shiftUp(n);
        board_ |= wrap >> (8 * (8 - n));
        return *this;
    }

    //! Shifts the bitboard up to 7 squares up and left
    BitBoard const & shiftUL(int n = 1)
    {
        board_ <<= 7 * n; board_ &= leftMask(8 - n);
        return *this;
    }

    //! Rotates the bitboard up to 7 squares up and left
    BitBoard const & rotateUL(int n = 1)
    {
        rotateUp(n); rotateLeft(n);
        return *this;
    }

    //! Shifts the bitboard up to 7 squares to the left
    BitBoard const & shiftLeft(int n = 1)
    {
        board_ >>= n; board_ &= leftMask(8 - n);
        return *this;
    }

    //! Rotates the bitboard up to 7 squares to the left
    BitBoard const & rotateLeft(int n = 1)
    {
        uint64_t wrap = board_ & leftMask(n); shiftLeft(n); board_ |= wrap << (8 - n);
        return *this;
    }

    //! Shifts the bitboard up to 7 squares down and left
    BitBoard const & shiftDL(int n = 1)
    {
        board_ >>= 9 * n; board_ &= leftMask(8 - n);
        return *this;
    }

    //! Rotates the bitboard up to 7 squares down and left
    BitBoard const & rotateDL(int n = 1)
    {
        rotateDown(n); rotateLeft(n);
        return *this;
    }

    //! Shifts the bitboard up to 7 squares down
    BitBoard const & shiftDown(int n = 1)
    {
        board_ >>= 8 * n;
        return *this;
    }

    //! Rotates the bitboard up to 7 squares down
    BitBoard const & rotateDown(int n = 1)
    {
        uint64_t wrap = board_ & lowerMask(n); shiftDown(n);
        board_ |= wrap << (8 * (8 - n));
        return *this;
    }

    //! Shifts the bitboard up to 7 squares down and right
    BitBoard const & shiftDR(int n = 1)
    {
        board_ <<= 7 * n; board_ &= rightMask(8 - n);
        return *this;
    }

    //! Rotates the bitboard up to 7 squares down and right
    BitBoard const & rotateDR(int n = 1)
    {
        rotateDown(); rotateRight();
        return *this;
    }

    //! Returns a bit board showing all squares threatened by a piece at a position, if it isn't blocked.
    //!
    //! This function returns a BitBoard showing all squares on an empty board that the specified piece can attack
    //! from the specified position. Generally, this function returns the same value as DestinationSquares(),
    //! except that pawns move differently when attacking.
    //!
    //! @param	type			Type of piece to be moved
    //! @param	position		The piece's position
    //!
    //! @return	BitBoard showing all threatened squares
    //!
    //! @note	En passant is included.

    static BitBoard threatenedSquares(PieceTypeId type, Position const & position);

    //! Returns a bit board showing all squares threatened by a piece at a position.
    //!
    //! This function returns a BitBoard showing all squares that the specified piece can attack from the specified
    //! position, given the locations of other pieces on the board. Generally, this function returns the same value as
    //! DestinationSquares(), except that pawns move differently when attacking.
    //!
    //! @param	type			Type of piece to be moved
    //! @param	position		The piece's position
    //! @param	friends         The positions of friendly pieces
    //! @param	foes			The positions of enemy pieces
    //!
    //! @return	BitBoard showing all threatened squares
    //!
    //! @note	En passant is included.

    static BitBoard threatenedSquares(PieceTypeId type, Position const & position, BitBoard const & friends, BitBoard const & foes);

    //! Returns a bit board showing all squares that a piece at a position can move to, if it isn't blocked.
    //!
    //! This function returns a BitBoard showing all squares on an empty board that the specified piece can move to
    //! from the specified position. Generally, this function returns the same value as ThreatenedSquares(), except
    //! that pawns move differently when attacking.
    //!
    //! @param	type			Type of piece to be moved
    //! @param	position		The piece's position
    //!
    //! @return	BitBoard showing all possible destination squares
    //!
    //! @note	Castling is not included.
    //! @note	The pawn's initial two-square move is included.

    static BitBoard destinationSquares(PieceTypeId type, Position const & position);

    //! Returns a bit board showing all squares that a piece at a position can move to.
    //!
    //! This function returns a BitBoard showing all squares that the specified piece can move to from the specified
    //! position, given the locations of the other pieces on the board. Capturing moves are included. Generally, this
    //! function returns the same value as ThreatenedSquares(), except that it also includes non-capturing pawn moves.
    //!
    //! @param	type			Type of piece to be moved
    //! @param	position		The piece's position
    //! @param	friends         The positions of friendly pieces
    //! @param	foes            The positions of enemy pieces
    //!
    //! @return	BitBoard showing all possible destination squares
    //!
    //! @note	Castling is not included
    //! @note	En passant is included.
    //! @note	The pawn's initial two-square move is included.

    static BitBoard destinationSquares(PieceTypeId      type,
                                       Position const & position,
                                       BitBoard const & friends,
                                       BitBoard const & foes);

    //! Returns a bit board showing all squares normally accessible by a piece at a position but blocked.
    //!
    //! @param	type			Type of piece to be moved
    //! @param	position		The piece's position
    //! @param	block
    //!
    //! @return

    static BitBoard blockedSquares(PieceTypeId type, Position const & position, Position block);

private:

    //! Returns the index for the given row and column
    static uint64_t index(int r, int c) { return r * 8 + c; }

    //! Returns the row and column for the given index
    static void rowAndColumn(int index, int & r, int & c) { r = index / 8; c = index % 8; }

    static uint64_t mask(int r, int c) { return (uint64_t)1 << index(r, c); }
    static uint64_t leftMask(int n)    { return 0x0101010101010101 * (((uint64_t)1 << n) - 1); }
    static uint64_t rightMask(int n)   { return ~leftMask(8 - n); }
    static uint64_t lowerMask(int n)   { return ((uint64_t)1 << (8 * n)) - 1; }
    static uint64_t upperMask(int n)   { return ~lowerMask(8 - n); }

    uint64_t board_;
};

#endif // !defined(BitBoard_h__)
