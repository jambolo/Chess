#pragma once

#if !defined(COMPUTERPLAYER_H)
#define COMPUTERPLAYER_H

#include "Player.h"
#include <memory>

#if defined(ANALYSIS_GAME_TREE)
#include "GamePlayer/GameTree.h"
#endif
#if defined(ANALYSIS_TRANSPOSITION_TABLE)
#include "GamePlayer/TranspositionTable.h"
#endif
#if defined(ANALYSIS_PLAYER)
#include <nlohmann/json.hpp>
#endif

enum class Color;
class GameState;
class StaticEvaluator;
namespace GamePlayer { class TranspositionTable; }

class ComputerPlayer : public Player
{
public:

    ComputerPlayer(Color color, int maxDepth);
    virtual ~ComputerPlayer() = default;

    virtual GameState myTurn(GameState const & s0) override;

#if defined(ANALYSIS_PLAYER)

    struct AnalysisData
    {
        int elapsedTime;
#if defined(ANALYSIS_GAME_TREE)
        GamePlayer::GameTree::AnalysisData gameTreeAnalysisData;
#endif
#if defined(ANALYSIS_TRANSPOSITION_TABLE)
        GamePlayer::TranspositionTable::AnalysisData ttAnalysisData;
#endif
        AnalysisData();
        void           reset();
        nlohmann::json toJson() const;
    };

    mutable AnalysisData analysisData_;
#endif // defined(ANALYSIS_PLAYER)

private:

    int maxDepth_;
    std::shared_ptr<GamePlayer::TranspositionTable> transpositionTable_;
    std::shared_ptr<StaticEvaluator> staticEvaluator_;
};

#endif // !defined(COMPUTERPLAYER_H)
