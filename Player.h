#pragma once

#if !defined(PLAYER_H)
#define PLAYER_H

#include "Chess/Types.h"

class GameState;

class Player
{
public:
    Player(Color color);

    // Returns the player's next move
    virtual GameState myTurn(GameState const & s0) = 0;

protected:

    Color myColor_;
};

#endif // !defined(PLAYER_H)
