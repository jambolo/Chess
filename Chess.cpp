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

static constexpr char OPTION_NOTATION_KEY[] = "--notation=";
static constexpr size_t OPTION_NOTATION_KEY_LENGTH = sizeof(OPTION_NOTATION_KEY) - 1;
static constexpr char OPTION_DEPTH_KEY[] = "--depth=";
static constexpr size_t OPTION_DEPTH_KEY_LENGTH = sizeof(OPTION_DEPTH_KEY) - 1;


static Notation notation = Notation::PGN;
static int depth = 7;

int main(int argc, char ** argv)
{
    GameState s0;

    --argc;
    ++argv;
    while (argc > 0)
    {
        if (strncmp(*argv, OPTION_NOTATION_KEY, OPTION_NOTATION_KEY_LENGTH) == 0)
        {
            if (strcmp(*argv + OPTION_NOTATION_KEY_LENGTH, "standard") == 0)
                notation = Notation::STANDARD;
            else if (strcmp(*argv + OPTION_NOTATION_KEY_LENGTH, "long") == 0)
                notation = Notation::LONG;
            else if (strcmp(*argv + OPTION_NOTATION_KEY_LENGTH, "figurine") == 0)
                notation = Notation::FIGURINE;
            else if (strcmp(*argv + OPTION_NOTATION_KEY_LENGTH, "uci") == 0)
                notation = Notation::UCI;
            else if (strcmp(*argv + OPTION_NOTATION_KEY_LENGTH, "iccf") == 0)
                notation = Notation::ICCF;
            else if (strcmp(*argv + OPTION_NOTATION_KEY_LENGTH, "pgn") == 0)
                notation = Notation::PGN;
            else 
                notation = Notation::PGN;
        }
        else if (strncmp(*argv, OPTION_DEPTH_KEY, OPTION_DEPTH_KEY_LENGTH) == 0)
        {
            sscanf(*argv + OPTION_DEPTH_KEY_LENGTH, "%d", &depth);
        }
        else if (!s0.initializeFromFen(*argv))
        {
            fprintf(stderr, "Unable to parse input: %s\n", *argv);
            exit(1);
        }
        --argc;
        ++argv;
    }

#if !defined(NDEBUG)
    drawBoard(s0.board_);
#endif

    ComputerPlayer computer(s0.whoseTurn_, depth);
    GameState      s1 = computer.myTurn(s0);
    printf("%s", s1.move_.notation(notation).c_str());

#if !defined(NDEBUG)
    fprintf(stderr, "\n\n");

#if defined(ANALYSIS_PLAYER)
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
