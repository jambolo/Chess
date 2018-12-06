#pragma once

#if !defined(ComputerPlayer_h__)
#define ComputerPlayer_h__

#include "Player/Player.h"
#include <memory>

enum class Color;
class GameState;
class TranspositionTable;

class ComputerPlayer : public Player
{
public:

    ComputerPlayer(Color color, int maxDepth);
    virtual ~ComputerPlayer() = default;

    virtual GameState myTurn(GameState const & s0) override;

private:

    std::shared_ptr<TranspositionTable> transpositionTable_;
    int maxDepth_;
};

#endif // !defined(ComputerPlayer_h__)
