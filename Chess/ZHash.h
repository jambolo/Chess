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

#if !defined(ZHash_h__)
#define ZHash_h__

#include "Board.h"
#include "Types.h"
#include <cstdint>

class Piece;
struct Position;
class Board;
enum class Color;

//! Zobrist Hashing Calculator.
//!
//! Zobrist Hashing is used to encode the state of a chess game in a single number. There is a risk of two unrelated states
//! having the same value. In this implementation, the value has 64 bits and the risk of collision can be assumed to be
//! very low, though not for a large number of values. The probability of at least one collision with 1 million values
//! is 2.710502×10<sup>-8</sup>, but with 1 billion values, it rises to 1 in 40.
//!
//! There are 6 components of state. This implementation of Zorbrist hashing incorporates most of them.
//!     1. Piece placement
//!     2. Active color
//!     3. Castling availability
//!     4. En-passant possibility
//!     5. Fifty-move rule
//!     6. Number of moves (this is ignored because it is not useful and it interferes with transposition optimization)
//!
//! An important characteristic of a Zorbrist hash is that it is independent of the order of the changes made to reach the state.
class ZHash
{
public:

    //! Type of a hash value
    using Z = std::uint64_t;

    //! The value of an empty board
    static Z constexpr EMPTY = 0;

    //! A value which represents an "undefined" state
    static Z constexpr UNDEFINED = ~EMPTY;

    //! Constructor
    explicit ZHash(Z z = EMPTY)
        : value_(z)
    {
    }

    //! Constructor
    ZHash(Board const & board,
          Color         turn,
          unsigned      castleStatus  = 0,
          Color         epColor       = Color::INVALID,
          int           epColumn      = -1,
          bool          fiftyMoveRule = false);

    //! Returns the current value.
    Z value() const { return value_; }

    //! Adds a piece. Returns a reference to itself
    ZHash & add(Piece const * piece, Position const & position);

    //! Removes a piece. Returns a reference to itself.
    ZHash & remove(Piece const * piece, Position const & position);

    //! Removes a piece at the 'from' position and adds it to the 'to' position
    ZHash & move(Piece const * piece, Position const & from, Position const & to);

    //! Changes whose turn. Returns a reference to itself.
    ZHash & turn();

    //! Changes the ability to perform a castle. Returns a reference to itself.
    ZHash & castleAvailability(unsigned mask);

    //! Changes en passant status. Returns a reference to itself.
    ZHash & enPassant(Color color, int column);

    //! Changes the state of the fifty-move rule (it either now applies to the next move or it doesn't)
    ZHash & fifty();

    // Returns true if the value is undefined (i.e. not a legal Z value)
    bool isUndefined() const { return value_ == UNDEFINED; }

private:

    friend bool operator ==(ZHash const & x, ZHash const & y);

    class ZValueTable; // declared below

    Z value_; // The hash value

    static ZValueTable const zValueTable_; // The hash values for each incremental state change
};

//! Equality operator
inline bool operator ==(ZHash const & x, ZHash const & y)
{
    return x.value_ == y.value_;
}


class ZHash::ZValueTable
{
public:

    // Constructor
    ZValueTable();

    // Returns the hash value for a piece on the board
    Z pieceValue(int color, int type, int row, int column) const;

    // Returns the hash value for a particular castle availability
    Z castleValue(int which) const;

    // Returns the hash value for en passant
    Z enPassantValue(int color, int column) const;

    // Returns the hash value for changing whether the fifty-move rule now or no longer applies
    Z fiftyValue() const;

    // Returns the hash value for changing the color of the next player to move
    Z turnValue() const;

private:

    Z pieceValues_[NUMBER_OF_COLORS][Board::SIZE][Board::SIZE][NUMBER_OF_PIECE_TYPES];
    Z castleValues_[4];
    Z enPassantValues_[NUMBER_OF_COLORS][Board::SIZE];
    Z fiftyValue_;
    Z turnValue_;
};

#endif // !defined(ZHash_h__)
