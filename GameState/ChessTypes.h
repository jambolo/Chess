#pragma once

#if !defined(ChessTypes_h__)
#define ChessTypes_h__

#include <vector>

struct Position
{
    int8_t column;
    int8_t row;

    Position() {}
    Position(int r, int c)
        : column(c)
        , row(r)
    {
    }

    explicit Position(int id)
        : column(id & 0xff)
        , row((id >> 8) & 0xff)
    {
    }

    int id() const
    {
        return (row << 8) + column;
    }
};

typedef std::vector<Position> PositionVector;

inline bool operator ==(Position const & a, Position const & b)
{
    return a.column == b.column && a.row == b.row;
}

enum class Color
{
    INVALID = -1,
    WHITE   = 0,
    BLACK
};
static int const NUMBER_OF_COLORS = (int)Color::BLACK - (int)Color::WHITE + 1;

enum class PieceTypeId
{
    INVALID = -1,
    KING    = 0,
    QUEEN,
    BISHOP,
    KNIGHT,
    ROOK,
    PAWN
};
static int const NUMBER_OF_PIECE_TYPES = (int)PieceTypeId::PAWN - (int)PieceTypeId::KING + 1;

// Castle values
static int const WHITE_QUEENSIDE_CASTLE = 0x01;
static int const WHITE_KINGSIDE_CASTLE = 0x02;
static int const BLACK_QUEENSIDE_CASTLE = 0x04;
static int const BLACK_KINGSIDE_CASTLE = 0x08;
static int const WHITE_CASTLE = WHITE_QUEENSIDE_CASTLE | WHITE_KINGSIDE_CASTLE;
static int const BLACK_CASTLE = BLACK_QUEENSIDE_CASTLE | BLACK_KINGSIDE_CASTLE;

static int const WHITE_QUEENSIDE_CASTLE_UNAVAILABLE = 0x10;
static int const WHITE_KINGSIDE_CASTLE_UNAVAILABLE  = 0x20;
static int const BLACK_QUEENSIDE_CASTLE_UNAVAILABLE = 0x40;
static int const BLACK_KINGSIDE_CASTLE_UNAVAILABLE  = 0x80;
static int const WHITE_CASTLE_UNAVAILABLE           = WHITE_QUEENSIDE_CASTLE_UNAVAILABLE | WHITE_KINGSIDE_CASTLE_UNAVAILABLE;
static int const BLACK_CASTLE_UNAVAILABLE           = BLACK_QUEENSIDE_CASTLE_UNAVAILABLE | BLACK_KINGSIDE_CASTLE_UNAVAILABLE;

static int const NUMBER_OF_CASTLE_BITS = 8;

#endif // !defined(ChessTypes_h__)
