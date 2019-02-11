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

#include "Chess/Board.h"
#include "Chess/Piece.h"
#include "Chess/Position.h"

#include <cassert>
#include <random>

ZHash::ZValueTable const ZHash::zValueTable_;

ZHash::ZHash(Z z /* = EMPTY */)
    : value_(z)
{
}

ZHash::ZHash(Board const & board,
             Color         whoseTurn,
             unsigned      castleStatus /* = 0*/,
             Color         ePColor /* = INVALID*/,
             int           ePColumn /* = -1*/,
             bool          fiftyMoveRule /* = false */)
{
    value_ = 0;

    for (int i = 0; i < Board::SIZE; ++i)
    {
        for (int j = 0; j < Board::SIZE; ++j)
        {
            Piece const * const piece = board.pieceAt(i, j);
            if (piece)
                add(piece, Position(i, j));
        }
    }

    if (whoseTurn != Color::WHITE)
        turn();

    if (castleStatus != 0)
        castle(castleStatus);

    if ((ePColor != Color::INVALID) && (ePColumn >= 0))
        enPassant(ePColor, ePColumn);

    if (fiftyMoveRule)
        fifty();
}

ZHash ZHash::add(Piece const * piece, Position const & position)
{
    value_ ^= zValueTable_.pieceValue((int)piece->color(), (int)piece->type(), position.row, position.column);
    return *this;
}

ZHash ZHash::remove(Piece const * piece, Position const & position)
{
    value_ ^= zValueTable_.pieceValue((int)piece->color(), (int)piece->type(), position.row, position.column);
    return *this;
}

ZHash ZHash::move(Piece const * piece, Position const & from, Position const & to)
{
    remove(piece, from);
    return add(piece, to);
}

ZHash ZHash::turn()
{
    value_ ^= zValueTable_.turnValue();
    return *this;
}

ZHash ZHash::castle(unsigned mask)
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

ZHash ZHash::enPassant(Color color, int column)
{
    value_ ^= zValueTable_.enPassantValue((int)color, column);
    return *this;
}

ZHash ZHash::fifty()
{
    value_ ^= zValueTable_.fiftyValue();
    return *this;
}

bool ZHash::isUndefined() const
{
    // The value is undefined if the high order bit is set
    return static_cast<int64_t>(value_) < 0;
}

ZHash::ZValueTable::ZValueTable()
{
    std::mt19937_64 rng;
    static_assert(sizeof(std::mt19937_64::result_type) == 8);

    // Generate piece values

    for (int i = 0; i < NUMBER_OF_COLORS; ++i)
    {
        for (int j = 0; j < Board::SIZE; ++j)
        {
            for (int k = 0; k < Board::SIZE; ++k)
            {
                for (int m = 0; m < NUMBER_OF_PIECE_TYPES; ++m)
                {
                    pieceValues_[i][j][k][m] = rng();
                }
            }
        }
    }

    // Generate castle values

    for (auto & v : castleValues_)
    {
        v = rng();
    }

    // Generate en passant values

    for (int i = 0; i < NUMBER_OF_COLORS; ++i)
    {
        for (int j = 0; j < Board::SIZE; ++j)
        {
            enPassantValues_[i][j] = rng();
        }
    }

    // Generate fifty-move rule value

    fiftyValue_ = rng();

    // Generate turn value

    turnValue_ = rng();
}

ZHash::Z ZHash::ZValueTable::pieceValue(int color, int type, int row, int column) const
{
    return pieceValues_[color][row][column][type];
}

ZHash::Z ZHash::ZValueTable::castleValue(int castle) const
{
    return castleValues_[castle];
}

ZHash::Z ZHash::ZValueTable::enPassantValue(int color, int column) const
{
    return enPassantValues_[color][column];
}

ZHash::Z ZHash::ZValueTable::fiftyValue() const
{
    return fiftyValue_;
}

ZHash::Z ZHash::ZValueTable::turnValue() const
{
    return turnValue_;
}

bool operator ==(ZHash const & x, ZHash const & y)
{
    return x.value_ == y.value_;
}
