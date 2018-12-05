#include "ComputerPlayer/ComputerPlayer.h"
#include "GameState/ChessTypes.h"
#include "GameState/GameState.h"

#include <cstdio>

// static void drawBoard(Board const & b);

int main(int argc, char ** argv)
{
    GameState s0;
    if (!s0.initializeFromFen(argv[1]))
    {
        fprintf(stderr, "Unable to parse input: %s\n", argv[1]);
        exit(1);
    }
    ComputerPlayer computer(Color::BLACK, 7);
    GameState      s1 = computer.myTurn(s0);
    printf("%s", s1.move_.notation().c_str());

    return 0;
}

#if 0

static void drawBoard(Board const & b)
{
    for (int row = 0; row < Board::SIZE; ++row)
    {
        puts("   +---+---+---+---+---+---+---+---+");
        char buffer[] = "|   |   |   |   |   |   |   |   |";
        for (int column = 0; column < Board::SIZE; ++column)
        {
            Piece const * piece = b.pieceAt(row, column);
            if (piece == NO_PIECE)
                buffer[column * 4 + 2] = ' ';
            else
                buffer[column * 4 + 2] = piece->symbol()[0];
        }
        printf(" %1d %s\n", 8 - row, buffer);
    }
    puts("   +---+---+---+---+---+---+---+---+");
    puts("     a   b   c   d   e   f   g   h");
}

#endif // if 0
