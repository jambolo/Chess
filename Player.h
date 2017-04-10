/********************************************************************************************************************

                                                       Player.h

                                                                    Copyright 2004, John J. Bolton
        --------------------------------------------------------------------------------------------------------------

        $Header: //depot/Chess/Player.h#8 $

        $NoKeywords: $

********************************************************************************************************************/

#pragma once

#include "Player/GameTree.h" // for AnalysisData

class GameState;

#define PLAYER_ANALYSIS_ENABLED

class Player
{
public:
    Player(Color color);

    // Returns my next move
    virtual GameState myTurn(GameState const & s0) = 0;

#if defined( PLAYER_ANALYSIS_ENABLED )

    // Resets all analysis data
    void resetAnalysisData();

    // Analysis data for the last move

    struct AnalysisData
    {
        int time;
        int value;
        GameTree::AnalysisData gameTreeAnalysisData;

        AnalysisData();
        void reset();
    };

    mutable AnalysisData analysisData_;

#endif // defined( PLAYER_ANALYSIS_ENABLED )

protected:

    Color myColor_;
};

#include "Player.inl"
