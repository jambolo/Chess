#pragma once

#if !defined(GameState_inl__)
#define GameState_inl__

#include "GameState/Board.h"
#include "GameState/ChessTypes.h"
#include "GameState/GameState.h"
#include "GameState/Move.h"

inline GameState::GameState(Board const & board, Move const & move, int value, CastleStatus castleStatus)
    : board_(board)
    , move_(move)
    , value_(value)
    , quality_(std::numeric_limits<int>::min())
    , zhash_(board)
    , castleStatus_(castleStatus)
{
}

inline bool GameState::castleIsAllowed(Color c) const
{
    unsigned mask = (c == Color::WHITE) ? WHITE_CASTLE_UNAVAILABLE : BLACK_CASTLE_UNAVAILABLE;
    return (castleStatus_ & mask) == 0;
}

inline bool GameState::kingSideCastleIsAllowed(Color c) const
{
    unsigned mask = (c == Color::WHITE) ? WHITE_KINGSIDE_CASTLE_UNAVAILABLE : BLACK_KINGSIDE_CASTLE_UNAVAILABLE;
    return (castleStatus_ & mask) == 0;
}

inline bool GameState::queenSideCastleIsAllowed(Color c) const
{
    unsigned mask = (c == Color::WHITE) ? WHITE_QUEENSIDE_CASTLE_UNAVAILABLE : BLACK_QUEENSIDE_CASTLE_UNAVAILABLE;
    return (castleStatus_ & mask) == 0;
}

inline ZHash GameState::zhash() const
{
    return zhash_;
}

inline Piece const * GameState::queen(Color color)
{
    return (color == Color::WHITE) ? whiteQueen_ : blackQueen_;
}

inline bool operator ==(GameState const & x, GameState const & y)
{
    //! @todo	Actually this is not correct because en passant validity is part of the state too
    return x.zhash_ == y.zhash_ &&
           x.board_ == y.board_ &&
           (x.castleStatus_ & 0xf0) == (y.castleStatus_ & 0xf0);
}

#endif // !defined(GameState_inl__)
