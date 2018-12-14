#pragma once

#if !defined(Player_h__)
#define Player_h__

#include "GameState/ChessTypes.h"

//#if defined(FEATURE_PLAYER_ANALYSIS)
//#include "ComputerPlayer/GameTree.h" // for AnalysisData
//#endif // defined(FEATURE_PLAYER_ANALYSIS)

class GameState;

class Player
{
public:
    Player(Color color);

    // Returns my next move
    virtual GameState myTurn(GameState const & s0) = 0;

protected:

    Color myColor_;
};

#endif // !defined(Player_h__)
