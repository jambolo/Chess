#if !defined(CHESS_STATICEVALUATOR_H)
#define CHESS_STATICEVALUATOR_H

#pragma once

#include "GamePlayer/StaticEvaluator.h"

class Move;
class Piece;
struct Position;
namespace GamePlayer { class GameState; }

class StaticEvaluator : public GamePlayer::StaticEvaluator
{
public:

    // The absolute value of checkmate
    static float constexpr CHECKMATE_VALUE = 1000000.0f;

    // The absolute value of threshold at which the evaluator recommends resignation (down by 1 of each major piece)
    static float constexpr RESIGNATION_THRESHOLD = 20000.0f;

    //!@{
    //! @name Overrides GamePlayer::StaticEvaluator

    float evaluate(GamePlayer::GameState const & state) const override;
    float firstPlayerWins() const override  { return CHECKMATE_VALUE; }
    float secondPlayerWins() const override { return -CHECKMATE_VALUE; }

    //!@}

#if defined(FEATURE_INCREMENTAL_STATIC_EVALUATION)
    // Returns a value for the game state based on the move and its current value
    static float incremental(Move const &            move,
                             GameState::CastleStatus castleStatus,
                             Piece const *           pMoved           = nullptr,
                             Position const *        pRemovedPosition = nullptr,
                             Piece const *           pRemoved         = nullptr,
                             Piece const *           pAdded           = nullptr);
#endif
};

#endif // !defined(CHESS_STATICEVALUATOR_H)
