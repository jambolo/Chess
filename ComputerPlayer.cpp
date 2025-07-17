#include "ComputerPlayer.h"

#include "Chess/Board.h"
#include "Chess/GameState.h"
#include "Chess/ResponseGenerator.h"
#include "Chess/StaticEvaluator.h"
#include "GamePlayer/GameTree.h"
#include "GamePlayer/TranspositionTable.h"

#include <nlohmann/json.hpp>

#include <ctime>

using json = nlohmann::json;

ComputerPlayer::ComputerPlayer(Color color, int maxDepth)
    : Player(color)
    , maxDepth_(maxDepth)
    , transpositionTable_(new GamePlayer::TranspositionTable(1<<19, 1))
    , staticEvaluator_(new StaticEvaluator)
{
}

GameState ComputerPlayer::myTurn(GameState const & s0)
{
#if defined(ANALYSIS_PLAYER)
    time_t startTime = time(nullptr);
#endif

    // Calculate the best move from here

    std::shared_ptr<GamePlayer::GameState> copy(new GameState(s0));
    GamePlayer::GameTree tree(transpositionTable_, staticEvaluator_, ResponseGenerator(), maxDepth_);
    tree.findBestResponse(copy);
    std::shared_ptr<GameState> response = std::static_pointer_cast<GameState>(copy->response_);
    transpositionTable_->age();

#if defined(ANALYSIS_PLAYER)

    // Update analysis data

    analysisData_.elapsedTime = (int)(time(nullptr) - startTime);
#if defined(ANALYSIS_GAME_TREE)
    analysisData_.gameTreeAnalysisData = tree.analysisData_;
    tree.analysisData_.reset();
#endif
#if defined(ANALYSIS_TRANSPOSITION_TABLE)
    analysisData_.ttAnalysisData = transpositionTable_->analysisData_;
#endif
#endif // defined(ANALYSIS_PLAYER)

    return *response;
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
#endif
#if defined(ANALYSIS_TRANSPOSITION_TABLE)
    ttAnalysisData.reset();
#endif
}

nlohmann::json ComputerPlayer::AnalysisData::toJson() const
{
    json out =
    {
        { "elapsedTime", elapsedTime }
#if defined(ANALYSIS_GAME_TREE)
        , { "gameTree", gameTreeAnalysisData.toJson() }
#endif
#if defined(ANALYSIS_TRANSPOSITION_TABLE)
        , { "transpositionTable", ttAnalysisData.toJson() }
#endif
    };
    return out;
}
#endif // defined(ANALYSIS_PLAYER)
