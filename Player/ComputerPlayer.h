/********************************************************************************************************************

                                                   ComputerPlayer.h

                                            Copyright 2004, John J. Bolton
    --------------------------------------------------------------------------------------------------------------

    $Header: //depot/Chess/ComputerPlayer.h#8 $

    $NoKeywords: $

********************************************************************************************************************/

#pragma once

#include "Player.h"

enum class Color;
class GameState;
class TranspositionTable;

class ComputerPlayer : public Player
{
public:

    ComputerPlayer(Color color, int maxDepth);
    virtual ~ComputerPlayer();

    GameState myTurn(GameState const & s0);

private:

    TranspositionTable * transpositionTable_;
    int maxDepth_;
};
