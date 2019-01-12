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

#if defined(ANALYSIS_PLAYER)

    struct AnalysisData
    {
        int elapsedTime;
#if defined(ANALYSIS_GAME_TREE)
        GameTree::AnalysisData gameTreeAnalysisData;
#endif
        AnalysisData();
        void reset();
        nlohmann::json toJson() const;
    };

    mutable AnalysisData analysisData_;
#endif // defined(ANALYSIS_PLAYER)

private:

    int maxDepth_;
    std::shared_ptr<TranspositionTable> transpositionTable_;
};

#endif // !defined(ComputerPlayer_h__)
