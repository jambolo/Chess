/** @file *//********************************************************************************************************

                                                    GameState.inl

                                            Copyright 2004, John J. Bolton
    --------------------------------------------------------------------------------------------------------------

    $Header: //depot/Chess/GameState.inl#12 $

    $NoKeywords: $

********************************************************************************************************************/

#pragma once

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
    unsigned mask = (c == Color::WHITE) ?
                    (int)CastleId::WHITE_QUEENSIDE | (int)CastleId::WHITE_KINGSIDE :
                    (int)CastleId::BLACK_QUEENSIDE | (int)CastleId::BLACK_KINGSIDE;
    return castleIsAllowed(mask);
}

inline bool GameState::kingSideCastleIsAllowed(Color c) const
{
    unsigned mask = (c == Color::WHITE) ? (int)CastleId::WHITE_KINGSIDE : (int)CastleId::BLACK_KINGSIDE;
    return castleIsAllowed(mask);
}

inline bool GameState::queenSideCastleIsAllowed(Color c) const
{
    unsigned mask = (c == Color::WHITE) ? (int)CastleId::WHITE_QUEENSIDE : (int)CastleId::BLACK_QUEENSIDE;
    return castleIsAllowed(mask);
}

inline bool GameState::castleIsAllowed(unsigned mask) const
{
    return (castleStatus_.unavailable & mask) == 0;
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
           x.castleStatus_.unavailable == y.castleStatus_.unavailable;
}
