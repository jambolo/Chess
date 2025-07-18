#if !defined(CHESS_TYPES_H)
#define CHESS_TYPES_H

#pragma once

enum class Color
{
    INVALID = -1,
    WHITE   = 0,
    BLACK
};
static int constexpr NUMBER_OF_COLORS = (int)Color::BLACK - (int)Color::WHITE + 1;

enum class Direction
{
    UP    = -1,
    DOWN  = 1,
    LEFT  = -1,
    RIGHT = 1
};

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
static int constexpr NUMBER_OF_PIECE_TYPES = (int)PieceTypeId::PAWN - (int)PieceTypeId::KING + 1;

enum class Notation
{
    STANDARD,
    LONG,
    FIGURINE,
    UCI,
    ICCF,
    PGN
};

// Castle moves
static int constexpr CASTLE_MOVE_MASK       = 0x0f;
static int constexpr NO_CASTLE              = 0x00;
static int constexpr WHITE_QUEENSIDE_CASTLE = 0x01;
static int constexpr WHITE_KINGSIDE_CASTLE  = 0x02;
static int constexpr BLACK_QUEENSIDE_CASTLE = 0x04;
static int constexpr BLACK_KINGSIDE_CASTLE  = 0x08;
static int constexpr WHITE_CASTLE           = WHITE_QUEENSIDE_CASTLE | WHITE_KINGSIDE_CASTLE;
static int constexpr BLACK_CASTLE           = BLACK_QUEENSIDE_CASTLE | BLACK_KINGSIDE_CASTLE;

// Castle availability
static int constexpr CASTLE_AVAILABILITY_MASK           = CASTLE_MOVE_MASK << 4;
static int constexpr ALL_CASTLES_AVAILABLE              = 0x00;
static int constexpr WHITE_QUEENSIDE_CASTLE_UNAVAILABLE = WHITE_QUEENSIDE_CASTLE << 4;
static int constexpr WHITE_KINGSIDE_CASTLE_UNAVAILABLE  = WHITE_KINGSIDE_CASTLE << 4;
static int constexpr BLACK_QUEENSIDE_CASTLE_UNAVAILABLE = BLACK_QUEENSIDE_CASTLE << 4;
static int constexpr BLACK_KINGSIDE_CASTLE_UNAVAILABLE  = BLACK_KINGSIDE_CASTLE << 4;
static int constexpr WHITE_CASTLE_UNAVAILABLE           = WHITE_QUEENSIDE_CASTLE_UNAVAILABLE | WHITE_KINGSIDE_CASTLE_UNAVAILABLE;
static int constexpr BLACK_CASTLE_UNAVAILABLE           = BLACK_QUEENSIDE_CASTLE_UNAVAILABLE | BLACK_KINGSIDE_CASTLE_UNAVAILABLE;

static int const NUMBER_OF_CASTLE_BITS = 8;

#endif // !defined(CHESS_TYPES_H)
