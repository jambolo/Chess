#include "ComputerPlayer.h"

#include "GameState/Board.h"
#include "GameState/GameState.h"
#include "GameTree.h"
#include "StaticEvaluator.h"
#include "TranspositionTable.h"

ComputerPlayer::ComputerPlayer(Color color, int maxDepth)
    : Player(color)
    , maxDepth_(maxDepth)
#if defined(FEATURE_TRANSPOSITION_TABLE)
    , transpositionTable_(new TranspositionTable)
#endif // defined(FEATURE_TRANSPOSITION_TABLE)
{
}

GameState ComputerPlayer::myTurn(GameState const & s0)
{

    // Calculate the best move from here

#if defined(FEATURE_TRANSPOSITION_TABLE)
    GameTree  tree(transpositionTable_, maxDepth_);
#else // defined(FEATURE_TRANSPOSITION_TABLE)
    GameTree  tree(maxDepth_);
#endif // defined(FEATURE_TRANSPOSITION_TABLE)

    GameState new_state = tree.myBestMove(s0, myColor_);
#if defined(FEATURE_TRANSPOSITION_TABLE)
    transpositionTable_->age();
#endif
    
#if defined(FEATURE_PLAYER_ANALYSIS)

    // Update analysis data

    analysisData_.time  = 0; // elapsed_time;
    analysisData_.value = new_state.value_;
#if defined(FEATURE_GAME_TREE_ANALYSIS)
    analysisData_.gameTreeAnalysisData = game_tree.analysisData_;
#endif
#endif // defined( FEATURE_PLAYER_ANALYSIS )

    return new_state;
}
