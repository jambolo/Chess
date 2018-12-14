#include "ComputerPlayer.h"

#include "GameState/Board.h"
#include "GameState/GameState.h"
#include "GameTree.h"
#include "StaticEvaluator.h"
#include "TranspositionTable.h"

#include <nlohmann/json.hpp>

#include <time.h>

using json = nlohmann::json;

ComputerPlayer::ComputerPlayer(Color color, int maxDepth)
    : Player(color)
    , maxDepth_(maxDepth)
#if defined(FEATURE_TRANSPOSITION_TABLE)
    , transpositionTable_(new TranspositionTable)
#endif // defined(FEATURE_TRANSPOSITION_TABLE)
{
}

GameState ComputerPlayer::myTurn(GameState const & s0)
{
#if defined(FEATURE_PLAYER_ANALYSIS)
    time_t startTime = time(NULL);
#endif // defined(FEATURE_PLAYER_ANALYSIS)

    // Calculate the best move from here

#if defined(FEATURE_TRANSPOSITION_TABLE)
    GameTree  tree(transpositionTable_, maxDepth_);
#else // defined(FEATURE_TRANSPOSITION_TABLE)
    GameTree  tree(maxDepth_);
#endif // defined(FEATURE_TRANSPOSITION_TABLE)

    GameState new_state = tree.myBestMove(s0, myColor_);
#if defined(FEATURE_TRANSPOSITION_TABLE)
    transpositionTable_->age();
#endif
    
#if defined(FEATURE_PLAYER_ANALYSIS)

    // Update analysis data

    analysisData_.elapsedTime  = time(NULL) - startTime;
#if defined(FEATURE_GAME_TREE_ANALYSIS)
    analysisData_.gameTreeAnalysisData = tree.analysisData_;
    tree.analysisData_.reset();
#endif
    
#endif // defined( FEATURE_PLAYER_ANALYSIS )

    return new_state;
}

#if defined(FEATURE_PLAYER_ANALYSIS)

ComputerPlayer::AnalysisData::AnalysisData()
    : elapsedTime(0)
{
}

void ComputerPlayer::AnalysisData::reset()
{
    elapsedTime = 0;
#if defined(FEATURE_GAME_TREE_ANALYSIS)
    gameTreeAnalysisData.reset();
#endif // defined(FEATURE_GAME_TREE_ANALYSIS)
}

nlohmann::json ComputerPlayer::AnalysisData::toJson() const
{
    json out =
    {
        {"elapsedTime", elapsedTime}
#if defined(FEATURE_GAME_TREE_ANALYSIS)
        , {"gameTree", gameTreeAnalysisData.toJson()}
#endif
    };
    return out;
}
#endif // defined(FEATURE_PLAYER_ANALYSIS)
