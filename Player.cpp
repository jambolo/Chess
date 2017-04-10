/********************************************************************************************************************

                                                      Player.cpp

                                                                    Copyright 2004, John J. Bolton
        --------------------------------------------------------------------------------------------------------------

        $Header: //depot/Chess/Player.cpp#6 $

        $NoKeywords: $

********************************************************************************************************************/

#include "Player.h"

#if defined(PLAYER_ANALYSIS_ENABLED)

void Player::resetAnalysisData()
{
    analysisData_.reset();
}

void Player::AnalysisData::reset()
{
    time  = 0;
    value = 0;
    gameTreeAnalysisData.reset();
}

Player::AnalysisData::AnalysisData()
    : time(0)
    , value(0)
{
    reset();
}

#endif // defined(PLAYER_ANALYSIS_ENABLED)
