#include "ComputerPlayer.h"

#include "GameState/Board.h"
#include "GameState/GameState.h"
#include "GameTree.h"
#include "StaticEvaluator/StaticEvaluator.h"
#include "TranspositionTable.h"

//#include <mmsystem.h>

ComputerPlayer::ComputerPlayer(Color color, int maxDepth)
    : Player(color)
    , maxDepth_(maxDepth)
{
    transpositionTable_ = new TranspositionTable;
}

ComputerPlayer::~ComputerPlayer()
{
    delete transpositionTable_;
}

GameState ComputerPlayer::myTurn(GameState const & s0)
{
//	CWaitCursor wait_cursor;

//    uint32_t start_time = timeGetTime();

    // Calculate the best move from here

    GameTree game_tree(transpositionTable_, maxDepth_);
    GameState new_state = game_tree.myBestMove(s0, myColor_);
    transpositionTable_->age();

//    uint32_t elapsed_time = timeGetTime() - start_time;

#if defined(PLAYER_ANALYSIS_ENABLED)

    // Update analysis data

    analysisData_.time  = 0; // elapsed_time;
    analysisData_.value = new_state.value_;
#if defined(GAME_TREE_ANALYSIS_ENABLED)
    analysisData_.gameTreeAnalysisData = game_tree.analysisData_;
#endif
#endif // defined( PLAYER_ANALYSIS_ENABLED )

    return new_state;
}
