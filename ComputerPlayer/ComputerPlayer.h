#pragma once

#if !defined(ComputerPlayer_h__)
#define ComputerPlayer_h__

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

#endif // !defined(ComputerPlayer_h__)
