#pragma once

#if !defined(Move_inl__)
#define Move_inl__

#include "GameState/ChessTypes.h"
#include "GameState/Move.h"
#include "GameState/Piece.h"
#include <cassert>

inline Position const Move::from() const
{
    return Position(from_.row & NORMAL_MOVE_MASK, from_.column);
}

inline Position const Move::to() const
{
    return Position(to_.row, to_.column);
}

inline bool Move::isSpecial() const
{
    return (from_.row & ~NORMAL_MOVE_MASK) != 0;
}

inline bool Move::isSpecial(int move) const
{
    return (from_.row & ~NORMAL_MOVE_MASK) == move;
}

inline bool Move::isMoved(int dr, int dc)
{
    return dr != 0 || dc != 0;
}

inline bool Move::isDiagonal(int dr, int dc)
{
    return dr == dc || dr == -dc;
}

inline bool Move::inRange(int dr, int dc, int range)
{
    return dr > range || dr < -range || dc > range || dc < -range;
}

inline bool Move::isSquare(int dr, int dc)
{
    return dr == 0 || dc == 0;
}

inline Move Move::kingSideCastleKing(Color c)
{
    if (c == Color::WHITE)
        return Move(KINGSIDE_CASTLE, Color::WHITE, { 7, 4 }, { 7, 6 });
    else
        return Move(KINGSIDE_CASTLE, Color::BLACK, { 0, 4 }, { 0, 6 });
}

inline Move Move::kingSideCastleRook(Color c)
{
    if (c == Color::WHITE)
        return Move(KINGSIDE_CASTLE, Color::WHITE, { 7, 7 }, { 7, 5 });
    else
        return Move(KINGSIDE_CASTLE, Color::BLACK, { 0, 7 }, { 0, 5 });
}

inline Move Move::queenSideCastleKing(Color c)
{
    if (c == Color::WHITE)
        return Move(QUEENSIDE_CASTLE, Color::WHITE, { 7, 4 }, { 7, 2 });
    else
        return Move(QUEENSIDE_CASTLE, Color::BLACK, { 0, 4 }, { 0, 2 });
}

inline Move Move::queenSideCastleRook(Color c)
{
    if (c == Color::WHITE)
        return Move(QUEENSIDE_CASTLE, Color::WHITE, { 7, 0 }, { 7, 3 });
    else
        return Move(QUEENSIDE_CASTLE, Color::BLACK, { 0, 0 }, { 0, 3 });
}

#endif // !defined(Move_inl__)
