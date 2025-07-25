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

#include "Board.h"
#include "Piece.h"
#include "Position.h"

#include <cassert>
#include <random>

ZHash::ZValueTable const ZHash::zValueTable_;

ZHash::ZHash(Board const & board,
             Color         whoseTurn,
             unsigned      castleStatus /* = 0*/,
             Color         epColor /* = INVALID*/,
             int           epColumn /* = -1*/,
             bool          fiftyMoveRule /* = false */)
{
    value_ = EMPTY;

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
        castleAvailability(castleStatus);

    if ((epColor != Color::INVALID) && (epColumn >= 0))
        enPassant(epColor, epColumn);

    if (fiftyMoveRule)
        fifty();
}

//! @param	piece      Pointer to the piece to add.
//! @param	position   The position where the piece is placed.
//!
//! @return	Reference to this ZHash object.
ZHash & ZHash::add(Piece const * piece, Position const & position)
{
    assert(piece);
    value_ ^= zValueTable_.pieceValue((int)piece->color(), (int)piece->type(), position.row, position.column);
    return *this;
}

//! @param	piece      Pointer to the piece to remove.
//! @param	position   The position from which the piece is removed.
//!
//! @return	Reference to this ZHash object.
ZHash & ZHash::remove(Piece const * piece, Position const & position)
{
    assert(piece);
    value_ ^= zValueTable_.pieceValue((int)piece->color(), (int)piece->type(), position.row, position.column);
    return *this;
}

//! @param	piece      Pointer to the piece to move.
//! @param	from       The starting position.
//! @param	to         The destination position.
//!
//! @return	Reference to this ZHash object.
ZHash & ZHash::move(Piece const * piece, Position const & from, Position const & to)
{
    assert(piece);
    remove(piece, from);
    return add(piece, to);
}

//! Changes the turn in the hash (switches the active color).
//!
//! @return	Reference to this ZHash object.
ZHash & ZHash::turn()
{
    value_ ^= zValueTable_.turnValue();
    return *this;
}

//! @param	mask       Bitmask indicating which castling rights have changed. See CASTLE_AVAILABILITY_MASK.
//!
//! @return	Reference to this ZHash object.
//!
//! @note Only availability changes are recorded here. Castles are handled by piece moves.
ZHash & ZHash::castleAvailability(unsigned mask)
{
    static_assert(NUMBER_OF_CASTLE_BITS == 8);
    static_assert(CASTLE_AVAILABILITY_MASK == 0xf0);
    for (int i = 0; i < 4; ++i)
    {
        if (mask & (0x10 << i))
            value_ ^= zValueTable_.castleValue(i);
    }
    return *this;
}

//! @param	color      The color eligible for en passant.
//! @param	column     The column eligible for en passant.
//!
//! @return	Reference to this ZHash object.
ZHash & ZHash::enPassant(Color color, int column)
{
    value_ ^= zValueTable_.enPassantValue((int)color, column);
    return *this;
}

//! Updates the hash to reflect the fifty-move rule status.
//!
//! @return	Reference to this ZHash object.
ZHash & ZHash::fifty()
{
    value_ ^= zValueTable_.fiftyValue();
    return *this;
}

ZHash::ZValueTable::ZValueTable()
{
    std::mt19937_64 rng;
    static_assert(sizeof(std::mt19937_64::result_type) == 8, "The random number generator must generate 64 bits.");
    // Note: We don't seed the generator. This means that the same values are generated each time the program is run.

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

    for (int i = 0; i < 4; ++i)
    {
        castleValues_[i] = rng();
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
    assert(color >= 0 && color < NUMBER_OF_COLORS);
    assert(type >= 0 && type < NUMBER_OF_PIECE_TYPES);
    assert(row >= 0 && row < Board::SIZE);
    assert(column >= 0 && column < Board::SIZE);
    return pieceValues_[color][row][column][type];
}

ZHash::Z ZHash::ZValueTable::castleValue(int castle) const
{
    assert(castle >= 0 && castle < 4);
    return castleValues_[castle];
}

ZHash::Z ZHash::ZValueTable::enPassantValue(int color, int column) const
{
    assert(color >= 0 && color < NUMBER_OF_COLORS);
    assert(column >= 0 && column < Board::SIZE);
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
