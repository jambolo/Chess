#pragma once

#if !defined(Player_h__)
#define Player_h__

#include "GameState/ChessTypes.h"

#if defined(PLAYER_ANALYSIS_ENABLED)
#include "ComputerPlayer/GameTree.h" // for AnalysisData
#endif // defined(PLAYER_ANALYSIS_ENABLED)

class GameState;

//#define PLAYER_ANALYSIS_ENABLED

class Player
{
public:
    Player(Color color);

    // Returns my next move
    virtual GameState myTurn(GameState const & s0) = 0;

#if defined(PLAYER_ANALYSIS_ENABLED)

    // Resets all analysis data
    void resetAnalysisData();

    // Analysis data for the last move

    struct AnalysisData
    {
        int time;
        int value;
#if defined(GAME_TREE_ANALYSIS_ENABLED)
        GameTree::AnalysisData gameTreeAnalysisData;
#endif
        AnalysisData();
        void reset();
    };

    mutable AnalysisData analysisData_;

#endif // defined( PLAYER_ANALYSIS_ENABLED )

protected:

    Color myColor_;
};

#endif // !defined(Player_h__)
