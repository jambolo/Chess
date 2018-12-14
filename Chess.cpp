#include "ComputerPlayer/ComputerPlayer.h"
#include "GameState/ChessTypes.h"
#include "GameState/GameState.h"
#include "GameState/Piece.h"

#include <nlohmann/json.hpp>

#include <cstdio>

using json = nlohmann::json;

#if !defined(NDEBUG)
static void drawBoard(Board const & b);
#endif

int main(int argc, char ** argv)
{
    GameState s0;
    if (!s0.initializeFromFen(argv[1]))
    {
        fprintf(stderr, "Unable to parse input: %s\n", argv[1]);
        exit(1);
    }
    
#if !defined(NDEBUG)
    drawBoard(s0.board_);
#endif

    ComputerPlayer computer(s0.whoseTurn_, 7);
    GameState      s1 = computer.myTurn(s0);
    printf("%s", s1.move_.notation().c_str());
    
#if !defined(NDEBUG)
    fprintf(stderr, "\n\n");

#if defined(FEATURE_PLAYER_ANALYSIS)
    json out = computer.analysisData_.toJson();
    fprintf(stderr, "%s\n", out.dump(4).c_str());
#endif
#endif
    
    return 0;
}

#if !defined(NDEBUG)
static void drawBoard(Board const & b)
{
    for (int row = 0; row < Board::SIZE; ++row)
    {
        fputs("   +---+---+---+---+---+---+---+---+\n", stderr);
        char buffer[] = "|   |   |   |   |   |   |   |   |";
        for (int column = 0; column < Board::SIZE; ++column)
        {
            Piece const * piece = b.pieceAt(row, column);
            if (piece == NO_PIECE)
                buffer[column * 4 + 2] = ' ';
            else
                buffer[column * 4 + 2] = piece->symbol()[0];
        }
        fprintf(stderr, " %1d %s\n", 8 - row, buffer);
    }
    fputs("   +---+---+---+---+---+---+---+---+\n", stderr);
    fputs("     a   b   c   d   e   f   g   h\n", stderr);
}
#endif // !defined(NDEBUG)
