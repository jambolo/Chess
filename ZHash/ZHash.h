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

#include "GameState/Board.h"

class Piece;
class Position;

//! Zobrist Hash Coding.
class ZHash
{
public:

    // Type of the hash value
    typedef uint64_t Z;

    // An impossible value which will always be "undefined"
    static Z const INVALID = 0xFFFFFFFFFFFFFFFFui64;

    // Constructor
    explicit ZHash(Z z = 0);

    // Constructor
    explicit ZHash(Board const & board, unsigned castle = 0, Color ePColor = Color::INVALID, int ePColumn = -1);

    // Conversion to Z
    operator Z() const { return value_; }

    // Adds a piece. Returns the new value
    ZHash & add(Piece const & piece, Position const & position);

    // Removes a piece. Returns the new value
    ZHash & remove(Piece const & piece, Position const & position);

    // Changes the status of the ability to perform a castle. Returns the new value
    ZHash & castle(CastleId which);

    // Changes en passant status. Returns the new value
    ZHash & enPassant(Color color, int column);

    // Returns true if the value is undefined (i.e. not a legal Z value)
    bool isUndefined() const;

private:

    friend bool operator ==(ZHash const & x, ZHash const & y);

    class ZValueTable;  // declared below

    Z value_;    // The hash value

    static ZValueTable const zValueTable_;    // The hash code table
};

// Equality operator
bool operator ==(ZHash const & x, ZHash const & y);

class ZHash::ZValueTable
{
public:

    ZValueTable();

    // Returns the appropriate value for a piece on the board
    Z   pieceValue(int color, int type, int row, int column) const;

    // Returns the appropriate value for en passant
    Z   enPassantValue(int color, int column) const;

    // Returns the appropriate value for a particular castle
    Z   castleValue(int which) const;

private:

    Z pieceValues_[NUMBER_OF_COLORS][Board::SIZE][Board::SIZE][NUMBER_OF_PIECE_TYPES];
    Z enPassantValues_[NUMBER_OF_COLORS][Board::SIZE];
    Z castleValues_[NUMBER_OF_CASTLES];
};

// Inline functions

#include "ZHash.inl"
