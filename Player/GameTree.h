/** @file *//********************************************************************************************************

                                                      GameTree.h

                                            Copyright 2004, John J. Bolton
    --------------------------------------------------------------------------------------------------------------

    $Header: //depot/Chess/GameTree.h#11 $

    $NoKeywords: $

********************************************************************************************************************/

#pragma once

#include "GameState/GameState.h"
#include "GameState/Move.h"
#include "StaticEvaluator/StaticEvaluator.h"

#include "Player/TranspositionTable.h"

#define GAME_TREE_ANALYSIS_ENABLED

class GameTree
{
public:

    static int const MAX_DEPTH = 10;

    GameTree(TranspositionTable * pTTable, int maxDepth);

    // Search for my best move and return it
    GameState myBestMove(GameState const & state, Color my_color);

    static int constexpr MY_CHECKMATE_VALUE       = StaticEvaluator::CHECKMATE_VALUE;
    static int constexpr OPPONENT_CHECKMATE_VALUE = -StaticEvaluator::CHECKMATE_VALUE;

#if defined(GAME_TREE_ANALYSIS_ENABLED)

    // Resets all analysis data
    void resetAnalysisData();

    // Analysis data for the last move

    struct AnalysisData
    {
        int aGeneratedStateCounts[MAX_DEPTH + 1];
        int aEvaluationCounts[MAX_DEPTH + 1];
        int worstValue;
        int alphaHitCount;
        int betaHitCount;
        TranspositionTable::AnalysisData tTableAnalysisData;
        GameState::AnalysisData gameStateAnalysisData;

        AnalysisData();
        void reset();
    };

    mutable AnalysisData analysisData_;

#endif // defined( GAME_TREE_ANALYSIS_ENABLED )

private:

    static int constexpr SEF_QUALITY = -std::numeric_limits<int>::max() + 1;

    // Sets the value of the given state to my best response
    void myAlphaBeta(GameState * s0, int alpha, int beta, int depth);

    // Sets the value of the given state to my opponent's best response
    void opponentsAlphaBeta(GameState * s0, int alpha, int beta, int depth);

    // Static evaluation function
    void evaluate(GameState * pstate, int depth);

#if defined(USING_PRIORITIZED_MOVE_ORDERING)

    // Computes a search priority for the state
    void prioritize(GameState * pstate, int depth);

#endif

    // Generate a list of all possible states in response to the specified one
    void generateStates(GameState const & state, bool my_move, int depth, GameStateList &states);

    Color myColor_;                                // My color
    Color yourColor_;                              // My opponent's color
    TranspositionTable * transpositionTable_;      // Transposition table
    int maxDepth_;                                 // How deep to seach
};
