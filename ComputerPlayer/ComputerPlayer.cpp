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
    , transpositionTable_(new TranspositionTable)
{
}

GameState ComputerPlayer::myTurn(GameState const & s0)
{
#if defined(ANALYSIS_PLAYER)
    time_t startTime = time(NULL);
#endif // defined(ANALYSIS_PLAYER)

    // Calculate the best move from here

    GameTree tree(transpositionTable_, maxDepth_);

    GameState new_state = tree.myBestMove(s0, myColor_);
    transpositionTable_->age();

#if defined(ANALYSIS_PLAYER)

    // Update analysis data

    analysisData_.elapsedTime = (int)(time(NULL) - startTime);
#if defined(ANALYSIS_GAME_TREE)
    analysisData_.gameTreeAnalysisData = tree.analysisData_;
    tree.analysisData_.reset();
#endif

#endif // defined( ANALYSIS_PLAYER )

    return new_state;
}

#if defined(ANALYSIS_PLAYER)

ComputerPlayer::AnalysisData::AnalysisData()
    : elapsedTime(0)
{
}

void ComputerPlayer::AnalysisData::reset()
{
    elapsedTime = 0;
#if defined(ANALYSIS_GAME_TREE)
    gameTreeAnalysisData.reset();
#endif // defined(ANALYSIS_GAME_TREE)
}

nlohmann::json ComputerPlayer::AnalysisData::toJson() const
{
    json out =
    {
        { "elapsedTime", elapsedTime }
#if defined(ANALYSIS_GAME_TREE)
        , { "gameTree", gameTreeAnalysisData.toJson() }
#endif
    };
    return out;
}
#endif // defined(ANALYSIS_PLAYER)
