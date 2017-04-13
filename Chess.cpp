#include "gamestate/ChessTypes.h"
#include "player/ComputerPlayer.h"
#include <cstdio>

int main(int argc, char ** argv)
{
    GameState s0;
    s0.initializeFromFen(argv[1]);
    ComputerPlayer computer(Color::BLACK, 5);
    GameState s1 = computer.myTurn(s0);
    puts(s1.fen().c_str());
}
