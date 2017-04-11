#include <cstdio>
#include "Player/ComputerPlayer.h"
#include "GameState/ChessTypes.h"

int main(int argc, char ** argv)
{
    GameState s0(argv[1]);
    ComputerPlayer computer(Color::BLACK, 5);
    GameState s1 = computer.myTurn(s0);
    puts(s1.fen().c_str());
}
