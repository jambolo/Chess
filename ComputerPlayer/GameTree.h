#pragma once

#if !defined(GameTree_h__)
#define GameTree_h__

#include "GameState/GameState.h"
#include "GameState/Move.h"
#include "StaticEvaluator.h"

#include "TranspositionTable.h"

// #define GAME_TREE_ANALYSIS_ENABLED
// #define USING_TRANSPOSITION_TABLE
// #define PRINTING_GAME_TREE_NODE_INFO

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
#if defined(USING_TRANSPOSITION_TABLE) && defined(TRANSPOSITION_TABLE_ANALYSIS_ENABLED)
        TranspositionTable::AnalysisData tTableAnalysisData;
#endif
#if defined(GAME_STATE_ANALYSIS_ENABLED)
        GameState::AnalysisData gameStateAnalysisData;
#endif
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
    void generateStates(GameState const & state, bool my_move, int depth, GameStateList & states);

#if defined(PRINTING_GAME_TREE_NODE_INFO)
    void printStateInfo(GameState const & state, int depth, int alpha, int beta);
#endif

    Color myColor_;                           // My color
    Color yourColor_;                         // My opponent's color
#if defined(USING_TRANSPOSITION_TABLE)
    TranspositionTable * transpositionTable_; // Transposition table
#endif
    int maxDepth_;                            // How deep to seach
};

#endif // !defined(GameTree_h__)
