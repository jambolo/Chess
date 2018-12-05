#include "Player.h"

Player::Player(Color color)
    : myColor_(color)
{
}

#if defined(FEATURE_PLAYER_ANALYSIS)

void Player::resetAnalysisData()
{
    analysisData_.reset();
}

void Player::AnalysisData::reset()
{
    time  = 0;
    value = 0;
#if defined(FEATURE_GAME_TREE_ANALYSIS)
    gameTreeAnalysisData.reset();
#endif
}

Player::AnalysisData::AnalysisData()
    : time(0)
    , value(0)
{
    reset();
}

#endif // defined(FEATURE_PLAYER_ANALYSIS)
