#pragma once

#if !defined(GameState_inl__)
#define GameState_inl__

#include "GameState/Board.h"
#include "GameState/ChessTypes.h"
#include "GameState/GameState.h"
#include "GameState/Move.h"

inline GameState::GameState(Board const & board,
                            Color         whoseTurn,
                            CastleStatus  castleStatus,
                            int           fiftyMoveTimer,
                            Move const &  move,
                            int           value,
                            bool          inCheck,
                            int           moveNumber)
    : board_(board)
    , whoseTurn_(whoseTurn)
    , castleStatus_(castleStatus)
    , fiftyMoveTimer_(fiftyMoveTimer)
    , move_(move)
    , value_(value)
    , quality_(std::numeric_limits<int>::min())
#if defined(USING_PRIORITIZED_MOVE_ORDERING)
    , priority_(0)
#endif // defined( USING_PRIORITIZED_MOVE_ORDERING )
    , inCheck_(inCheck)
    , moveNumber_(moveNumber)
    , zhash_(board)
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

inline bool GameState::canBeOccupied(Position const & p, Color myColor) const
{
    Piece const * piece = board_.pieceAt(p);
    return (piece == NO_PIECE) || (piece->color() != myColor);
}

inline ZHash GameState::zhash() const
{
    return zhash_;
}

inline bool operator ==(GameState const & x, GameState const & y)
{
    //! @todo	Actually this is not correct because en passant validity is part of the state too
    return x.zhash_ == y.zhash_ &&
           x.board_ == y.board_ &&
           (x.castleStatus_ & 0xf0) == (y.castleStatus_ & 0xf0);
}

#endif // !defined(GameState_inl__)
