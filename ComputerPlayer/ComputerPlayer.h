#pragma once

#if !defined(ComputerPlayer_h__)
#define ComputerPlayer_h__

#include "ComputerPlayer/GameTree.h"
#include "Player/Player.h"
#include <memory>
#include <nlohmann/json.hpp>

enum class Color;
class GameState;
class TranspositionTable;
//namespace nlohmann { class json; }

class ComputerPlayer : public Player
{
public:

    ComputerPlayer(Color color, int maxDepth);
    virtual ~ComputerPlayer() = default;

    virtual GameState myTurn(GameState const & s0) override;

#if defined(FEATURE_PLAYER_ANALYSIS)

    struct AnalysisData
    {
        int elapsedTime;
#if defined(FEATURE_GAME_TREE_ANALYSIS)
        GameTree::AnalysisData gameTreeAnalysisData;
#endif
        AnalysisData();
        void reset();
        nlohmann::json toJson() const;
    };

    mutable AnalysisData analysisData_;
#endif // defined(FEATURE_PLAYER_ANALYSIS)

private:

    int maxDepth_;
#if defined(FEATURE_TRANSPOSITION_TABLE)
    std::shared_ptr<TranspositionTable> transpositionTable_;
#endif // defined(FEATURE_TRANSPOSITION_TABLE)
};

#endif // !defined(ComputerPlayer_h__)
