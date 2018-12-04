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

#if !defined(ZHash_inl__)
#define ZHash_inl__

#include "GameState/Board.h"
#include "ZHash.h"

inline ZHash::ZHash(Z z /* = EMPTY */)
    : value_(z)
{
}

inline bool operator ==(ZHash const & x, ZHash const & y)
{
    return x.value_ == y.value_;
}

inline ZHash ZHash::add(Piece const & piece, Position const & position)
{
    value_ ^= zValueTable_.pieceValue((int)piece.color(), (int)piece.type(), position.row, position.column);

    return *this;
}

inline ZHash ZHash::remove(Piece const & piece, Position const & position)
{
    value_ ^= zValueTable_.pieceValue((int)piece.color(), (int)piece.type(), position.row, position.column);

    return *this;
}

inline ZHash ZHash::castle(unsigned mask)
{
    assert(NUMBER_OF_CASTLE_BITS == 8);
    assert(CASTLE_AVAILABILITY_MASK == 0xf0);
    for (int i = 0; i < 4; ++i)
    {
        if (mask & (0x10 << i))
            value_ ^= zValueTable_.castleValue(i);
    }

    return *this;
}

inline ZHash ZHash::enPassant(Color color, int column)
{
    value_ ^= zValueTable_.enPassantValue((int)color, column);
    
    return *this;
}

inline ZHash ZHash::fifty()
{
    value_ ^= zValueTable_.fifty();
    
    return *this;
}

inline bool ZHash::isUndefined() const
{
    // The value is undefined if the high order bit is set
    return static_cast<int64_t>(value_) < 0;
}

inline ZHash::Z ZHash::ZValueTable::pieceValue(int color, int type, int row, int column) const
{
    return pieceValues_[color][row][column][type];
}

inline ZHash::Z ZHash::ZValueTable::castleValue(int castle) const
{
    return castleValues_[castle];
}

inline ZHash::Z ZHash::ZValueTable::enPassantValue(int color, int column) const
{
    return enPassantValues_[color][column];
}

inline ZHash::Z ZHash::ZValueTable::fifty() const
{
    return fiftyValue_;
}
#endif // !defined(ZHash_inl__)
