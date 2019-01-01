#pragma once

#if !defined(StaticEvaluator_h__)
#define StaticEvaluator_h__

#include "GameState/GameState.h"
#include <limits>

class Move;
class Piece;
struct Position;

class StaticEvaluator
{
public:

    // Returns a value for the board
    static float evaluate(GameState const & state);

#if defined(FEATURE_INCREMENTAL_STATIC_EVALUATION)
    // Returns a value for the game state based on the move and its current value
    static float incremental(Move const &            move,
                             GameState::CastleStatus castleStatus,
                             Piece const *           pMoved           = nullptr,
                             Position const *        pRemovedPosition = nullptr,
                             Piece const *           pRemoved         = nullptr,
                             Piece const *           pAdded           = nullptr);
#endif defined(FEATURE_INCREMENTAL_STATIC_EVALUATION)

    // The absolute value of checkmate
    static float constexpr CHECKMATE_VALUE = 1000000.0f;

    // The absolute value of threshold at which the evaluator recommends resignation (down by 1 of each major piece)
    static float constexpr RESIGNATION_THRESHOLD = 20000.0f;
};

#endif // !defined(StaticEvaluator_h__)
