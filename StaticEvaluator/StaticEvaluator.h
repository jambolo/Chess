#pragma once

#if !defined(StaticEvaluator_h__)
#define StaticEvaluator_h__

#include "GameState/Gamestate.h"
#include <limits>

class Move;
class Piece;
struct Position;

class StaticEvaluator
{
public:

    // Returns a value for the board
    static int evaluate(GameState const & state);

    // Returns a value for the game state based on the move and its current value
    static int incremental(Move const &            move,
                           GameState::CastleStatus castleStatus,
                           Piece const *           pMoved                  = nullptr,
                           Position const *        pRemovedPosition     = nullptr,
                           Piece const *           pRemoved                = nullptr,
                           Piece const *           pAdded                  = nullptr);

    // The absolute value of checkmate
    static int constexpr CHECKMATE_VALUE = std::numeric_limits<int>::max() - 1;

    // The absolute value of threshold at which the evaluator recommends resignation
    static int constexpr RESIGNATION_THRESHOLD = 16500;
};

#endif // !defined(StaticEvaluator_h__)
