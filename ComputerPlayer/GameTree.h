#pragma once

#if !defined(GameTree_h__)
#define GameTree_h__


#include "GameState/GameState.h"
#include "GameState/Move.h"
#include "StaticEvaluator.h"
#include "TranspositionTable.h"
#include <memory>

class GameTree
{
public:

    static int const MAX_DEPTH = 10;

    GameTree(std::shared_ptr<TranspositionTable> tt, int maxDepth);

    // Search for my best move and return it
    GameState myBestMove(GameState const & state, Color my_color);

    static int constexpr MY_CHECKMATE_VALUE       = StaticEvaluator::CHECKMATE_VALUE;
    static int constexpr OPPONENT_CHECKMATE_VALUE = -StaticEvaluator::CHECKMATE_VALUE;

#if defined(FEATURE_GAME_TREE_ANALYSIS)

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
#if defined(FEATURE_TRANSPOSITION_TABLE) && defined(FEATURE_TRANSPOSITION_TABLE_ANALYSIS)
        TranspositionTable::AnalysisData tTableAnalysisData;
#endif
#if defined(FEATURE_GAME_STATE_ANALYSIS)
        GameState::AnalysisData gameStateAnalysisData;
#endif
        AnalysisData();
        void reset();
    };

    mutable AnalysisData analysisData_;

#endif // defined( FEATURE_GAME_TREE_ANALYSIS )

private:

    struct EvaluatedGameState
    {
        GameState state_;
        int value_;       // Value of the game state
        int quality_;     // Quality of the value
#if defined(FEATURE_PRIORITIZED_MOVE_ORDERING)
        int priority_;    // Higher priority states should be searched first
#endif
    };
    
    using EvaluatedGameStateList = std::vector<EvaluatedGameState>;

    static int constexpr SEF_QUALITY = 0;

    // Sets the value of the given state to my best response
    void myAlphaBeta(EvaluatedGameState * s0, int alpha, int beta, int depth);

    // Sets the value of the given state to my opponent's best response
    void opponentsAlphaBeta(EvaluatedGameState * s0, int alpha, int beta, int depth);

    // Static evaluation function
    void evaluate(GameState const & state, int depth, int * pValue, int * pQuality);

#if defined(FEATURE_PRIORITIZED_MOVE_ORDERING)

    // Computes a search priority for the state
    void prioritize(GameState * pstate, int depth);

#endif

    // Generate a list of all possible states in response to the specified one
    void generateStates(GameState const & state, bool my_move, int depth, EvaluatedGameStateList & states);

#if defined(FEATURE_DEBUG_GAME_TREE_NODE_INFO)
    void printStateInfo(GameState const & state, int depth, int alpha, int beta);
#endif

    Color myColor_;                           // My color
    Color yourColor_;                         // My opponent's color
#if defined(FEATURE_TRANSPOSITION_TABLE)
    std::shared_ptr<TranspositionTable> transpositionTable_; // Transposition table (persistant)
#endif
    int maxDepth_;                            // How deep to seach
};

#endif // !defined(GameTree_h__)
