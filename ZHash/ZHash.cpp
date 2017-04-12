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

#include "ZHash.h"

#include "GameState/Board.h"
#include "GameState/Piece.h"

#include "Misc/Random.h"

ZHash::ZValueTable const ZHash::zValueTable_;

namespace
{
inline uint64_t generateRandomZ(RandomMT & rng)
{
    // Z is 63 bits and RandomMT is 32 bits so we have to concatenate two numbers together to make a Z value.
    return ((uint64_t(rng()) << 32) | rng()) & 0x7fffffffffffffff;
}
}

ZHash::ZHash(Board const & board, int castleStatus /* = 0*/, Color ePColor /* = INVALID*/, int ePColumn /* = -1*/)
{
    value_ = 0;

    for (int i = 0; i < Board::SIZE; ++i)
    {
        for (int j = 0; j < Board::SIZE; ++j)
        {
            Piece const * const pPiece = board.pieceAt(i, j);
            if (pPiece != NO_PIECE)
            {
                add(*pPiece, Position(i, j));
            }
        }
    }

    for (int i = 0; i < NUMBER_OF_CASTLE_BITS; ++i)
    {
        int mask = 1 << i;
        if ((castleStatus & mask) != 0)
        {
            castle(i);
        }
    }

    if ((ePColor != Color::INVALID) && (ePColumn >= 0))
    {
        enPassant(ePColor, ePColumn);
    }
}

ZHash::ZValueTable::ZValueTable()
{
    RandomMT rng(0);

    // Generate piece values

    for (int i = 0; i < NUMBER_OF_COLORS; ++i)
    {
        for (int j = 0; j < Board::SIZE; ++j)
        {
            for (int k = 0; k < Board::SIZE; ++k)
            {
                for (int m = 0; m < NUMBER_OF_PIECE_TYPES; ++m)
                {
                    pieceValues_[i][j][k][m] = generateRandomZ(rng);
                }
            }
        }
    }

    // Generate en passant values

    for (int i = 0; i < NUMBER_OF_COLORS; ++i)
    {
        for (int j = 0; j < Board::SIZE; ++j)
        {
            enPassantValues_[i][j] = generateRandomZ(rng);
        }
    }

    // Generate castle values

    for (auto & v : castleValues_)
    {
        v = generateRandomZ(rng);
    }
}
