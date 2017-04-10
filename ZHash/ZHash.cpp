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
inline uint64_t generate64BitRandomNumber(RandomMT & rng)
{
    // Z is 63 bits and Random is 32 bits so we have to concatenate two numbers together to make a Z value.
    return (uint64_t(rng()) << 32) | rng();
}
}

ZHash::ZHash(Board const & board, unsigned castle /* = 0*/, Color ePColor /* = INVALID*/, int ePColumn /* = -1*/)
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

    for (int i = 0; i < NUMBER_OF_CASTLES; ++i)
    {
        if ((castle & (1 << i)) != 0)
        {
            this->castle(static_cast<CastleId>(i));
        }
    }

    if (ePColor != Color::INVALID && ePColumn >= 0)
    {
        enPassant(ePColor, ePColumn);
    }
}

ZHash::ZValueTable::ZValueTable()
{
    // Bit 64 is always 0 so that special values can be available by setting the high bit.
    static uint64_t const MASK = 0X7FFFFFFFFFFFFFFFui64;

    // Generate Z-values

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
                    pieceValues_[i][j][k][m] = generate64BitRandomNumber(rng) & MASK;
                }
            }
        }
    }

    // Generate en passant values

    for (int i = 0; i < NUMBER_OF_COLORS; ++i)
    {
        for (int j = 0; j < Board::SIZE; ++j)
        {
            enPassantValues_[i][j] = generate64BitRandomNumber(rng) & MASK;
        }
    }

    // Generate castle values

    for (int i = 0; i < NUMBER_OF_CASTLES; ++i)
    {
        castleValues_[i] = generate64BitRandomNumber(rng) & MASK;
    }
}
