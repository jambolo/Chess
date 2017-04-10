/** @file *//********************************************************************************************************

                                                     ChessTypes.h

                                            Copyright 2004, John J. Bolton
    --------------------------------------------------------------------------------------------------------------

    $Header: //depot/Chess/ChessTypes.h#11 $

    $NoKeywords: $

********************************************************************************************************************/

#pragma once

#include <vector>

class Position
{
public:
    Position() {}
    Position(int r, int c) : m_Row(r), m_Column(c) {}
    explicit Position(unsigned _id) : id(_id) {}

    union
    {
        struct
        {
            int8_t m_Column;
            int8_t m_Row;
        };
        uint16_t id;    // Warning: Bitboards require m_Column to be the low-order byte
    };
};

typedef std::vector<Position> PositionVector;

inline bool operator ==(Position const & a, Position const & b)
{
    return a.id == b.id;
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

enum class CastleId
{
    WHITE_QUEENSIDE,
    WHITE_KINGSIDE,
    BLACK_QUEENSIDE,
    BLACK_KINGSIDE
};

static int const NUMBER_OF_CASTLES = (int)CastleId::BLACK_KINGSIDE - (int)CastleId::WHITE_QUEENSIDE + 1;
