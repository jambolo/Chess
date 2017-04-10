/** @file *//********************************************************************************************************

                                                       Move.inl

                                            Copyright 2004, John J. Bolton
    --------------------------------------------------------------------------------------------------------------

    $Header: //depot/Chess/Move.inl#8 $

    $NoKeywords: $

********************************************************************************************************************/

#pragma once

#include "GameState/ChessTypes.h"
#include "GameState/Move.h"

inline Move::Move(Position const & f, Position const & t)
    : from_(f)
    , to_(t)
{
}

inline Move::Move(SpecialMoveId move)
{
    from_.m_Row = (int)move;
}

inline Move::Move(SpecialMoveId move, Position const & from, Position const & to)
    :   from_(from)
    ,   to_(to)
{
    from_.m_Row |= (int)move;
}

inline Position const Move::from() const
{
    return Position(from_.m_Row & NORMAL_MOVE_MASK, from_.m_Column);
}

inline Position const Move::to() const
{
    return Position(to_.m_Row & NORMAL_MOVE_MASK, to_.m_Column & NORMAL_MOVE_MASK);
}

inline bool Move::isSpecial() const
{
    return (from_.m_Row & ~NORMAL_MOVE_MASK) != 0;
}

inline bool Move::isSpecial(SpecialMoveId move) const
{
    return (from_.m_Row & ~NORMAL_MOVE_MASK) == (int)move;
}

inline Move Move::kingSideCastleKing(Color c)
{
    if (c == Color::WHITE)
    {
        return Move(Position(7, 4), Position(7, 6));
    }
    else
    {
        return Move(Position(0, 4), Position(0, 6));
    }
}

inline Move Move::kingSideCastleRook(Color c)
{
    if (c == Color::WHITE)
    {
        return Move(Position(7, 7), Position(7, 5));
    }
    else
    {
        return Move(Position(0, 7), Position(0, 5));
    }
}

inline Move Move::queenSideCastleKing(Color c)
{
    if (c == Color::WHITE)
    {
        return Move(Position(7, 4), Position(7, 2));
    }
    else
    {
        return Move(Position(0, 4), Position(0, 2));
    }
}

inline Move Move::queenSideCastleRook(Color c)
{
    if (c == Color::WHITE)
    {
        return Move(Position(7, 0), Position(7, 3));
    }
    else
    {
        return Move(Position(0, 0), Position(0, 3));
    }
}
