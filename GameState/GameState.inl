#pragma once

#if !defined(GameState_inl__)
#define GameState_inl__

#include "GameState/GameState.h"

#include "GameState/Board.h"
#include "GameState/ChessTypes.h"

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

inline bool operator ==(GameState const & x, GameState const & y)
{
    //! @todo	Actually this is not correct because en passant validity is part of the state too
    return x.zhash_ == y.zhash_ &&
           x.board_ == y.board_ &&
           (x.castleStatus_ & CASTLE_AVAILABILITY_MASK) == (y.castleStatus_ & CASTLE_AVAILABILITY_MASK);
}

#endif // !defined(GameState_inl__)
