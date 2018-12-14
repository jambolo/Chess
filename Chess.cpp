#include "ComputerPlayer/ComputerPlayer.h"
#include "GameState/ChessTypes.h"
#include "GameState/GameState.h"
#include "GameState/Piece.h"

#include <cstdio>

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
    ComputerPlayer::AnalysisData const & computerPlayerData = computer.analysisData_;
    fprintf(stderr, "ComputerPlayer Analysis:\n");
    fprintf(stderr, "  elapsedTime = %d\n", computerPlayerData.elapsedTime);
#if defined(FEATURE_GAME_TREE_ANALYSIS)
    GameTree::AnalysisData const & gameTreeData = computerPlayerData.gameTreeAnalysisData;
    fprintf(stderr, "  GameTree Analysis:\n");
    fprintf(stderr, "    aGeneratedStateCounts = [ %d", gameTreeData.aGeneratedStateCounts[0]);
    for (int i = 1; i < GameTree::MAX_DEPTH + 1; ++i)
    {
        fprintf(stderr, ", %d", gameTreeData.aGeneratedStateCounts[i]);
    }
    fprintf(stderr, " ]\n");
    fprintf(stderr, "    aEvaluationCounts     = [ %d", gameTreeData.aEvaluationCounts[0]);
    for (int i = 1; i < GameTree::MAX_DEPTH + 1; ++i)
    {
        fprintf(stderr, ", %d", gameTreeData.aEvaluationCounts[i]);
    }
    fprintf(stderr, " ]\n");
    fprintf(stderr, "    value                 = %d\n", gameTreeData.value);
    fprintf(stderr, "    worstValue            = %d\n", gameTreeData.worstValue);
    fprintf(stderr, "    alphaHitCount         = %d\n", gameTreeData.alphaHitCount);
    fprintf(stderr, "    betaHitCount          = %d\n", gameTreeData.betaHitCount);
#if defined(FEATURE_TRANSPOSITION_TABLE) && defined(FEATURE_TRANSPOSITION_TABLE_ANALYSIS)
    TranspositionTable::AnalysisData const & ttData = gameTreeData.ttAnalysisData;
    fprintf(stderr, "    TTable Analysis:\n");
    fprintf(stderr, "      checkCount     = %d\n", ttData.checkCount);
    fprintf(stderr, "      updateCount    = %d\n", ttData.updateCount);
    fprintf(stderr, "      hitCount       = %d\n", ttData.hitCount);
    fprintf(stderr, "      collisionCount = %d\n", ttData.collisionCount);
    fprintf(stderr, "      rejected       = %d\n", ttData.rejected);
    fprintf(stderr, "      overwritten    = %d\n", ttData.overwritten);
    fprintf(stderr, "      refreshed      = %d\n", ttData.refreshed);
    fprintf(stderr, "      usage          = %d\n", ttData.usage);
#endif
//#if defined(FEATURE_GAME_STATE_ANALYSIS)
//    GameState::AnalysisData gameStateAnalysisData;
//#endif
#endif
#endif // defined(FEATURE_PLAYER_ANALYSIS)
#endif // !defined(NDEBUG)
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
