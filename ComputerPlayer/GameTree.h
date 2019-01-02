#pragma once

#if !defined(GameTree_h__)
#define GameTree_h__

#include "GameState/GameState.h"
#include "GameState/Move.h"
#include "StaticEvaluator.h"
#include "TranspositionTable.h"
#include <memory>
#include <nlohmann/json.hpp>

class GameTree
{
public:

    static int const MAX_DEPTH = 10;

#if defined(FEATURE_TRANSPOSITION_TABLE)
    GameTree(std::shared_ptr<TranspositionTable> tt, int maxDepth);
#else // defined(FEATURE_TRANSPOSITION_TABLE)
    GameTree(int maxDepth);
#endif // defined(FEATURE_TRANSPOSITION_TABLE)

    // Search for my best move and return it
    GameState myBestMove(GameState const & state, Color my_color);

    static float constexpr MY_CHECKMATE_VALUE       = StaticEvaluator::CHECKMATE_VALUE;
    static float constexpr OPPONENT_CHECKMATE_VALUE = -StaticEvaluator::CHECKMATE_VALUE;

#if defined(ANALYSIS_GAME_TREE)

    // Analysis data for the last move

    struct AnalysisData
    {
        int aGeneratedStateCounts[MAX_DEPTH + 1];
        int aEvaluationCounts[MAX_DEPTH + 1];
        float value;
        float worstValue;
        int alphaHitCount;
        int betaHitCount;
#if defined(FEATURE_TRANSPOSITION_TABLE) && defined(ANALYSIS_TRANSPOSITION_TABLE)
        TranspositionTable::AnalysisData ttAnalysisData;
#endif
#if defined(ANALYSIS_GAME_STATE)
        GameState::AnalysisData gsAnalysisData;
#endif  //

        AnalysisData();
        void           reset();
        nlohmann::json toJson() const;
    };

    mutable AnalysisData analysisData_;

#endif // defined( ANALYSIS_GAME_TREE )

private:

    struct EvaluatedGameState
    {
        GameState state_;
        float value_;  // Value of the game state
        int quality_;  // Quality of the value
#if defined(FEATURE_PRIORITIZED_MOVE_ORDERING)
        int priority_; // Higher priority states should be searched first
#endif
    };

    using EvaluatedGameStateList = std::vector<EvaluatedGameState>;

    static int constexpr SEF_QUALITY = 0;

    // Sets the value of the given state to my best response
    void myAlphaBeta(EvaluatedGameState * s0, float alpha, float beta, int depth);

    // Sets the value of the given state to my opponent's best response
    void opponentsAlphaBeta(EvaluatedGameState * s0, float alpha, float beta, int depth);

    // Static evaluation function
    void evaluate(GameState const & state, int depth, float * pValue, int * pQuality);

#if defined(FEATURE_PRIORITIZED_MOVE_ORDERING)

    // Computes a search priority for the state
    void prioritize(GameState * pstate, int depth);

#endif

    // Generate a list of all possible states in response to the specified one
    void generateStates(GameState const & state, bool my_move, int depth, EvaluatedGameStateList & states);

#if defined(DEBUG_GAME_TREE_NODE_INFO)
    void printStateInfo(GameState const & state, int depth, float alpha, float beta);
#endif

    Color myColor_;                                          // My color
    Color yourColor_;                                        // My opponent's color
    int maxDepth_;                                           // How deep to seach
#if defined(FEATURE_TRANSPOSITION_TABLE)
    std::shared_ptr<TranspositionTable> transpositionTable_; // Transposition table (persistant)
#endif
};

#endif // !defined(GameTree_h__)
