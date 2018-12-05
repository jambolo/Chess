#include "GameTree.h"

#include "GameState/GameState.h"
#include "GameState/Move.h"
#include "GameState/Piece.h"
#include "StaticEvaluator.h"
#include "TranspositionTable.h"

#include "Misc/Random.h"

#include <functional>
#include <limits>

namespace
{
bool shouldDoQuiescentSearch(int previousValue, int thisValue)
{
#if defined(QUIESCENT_SEARCH_ENABLED)

    int const QUIESCENT_THRESHOLD = 1000;

    // In the normal case, we would not search. However, if the situation is unsettled and we haven't reached the
    // true depth limit, then we should search the next ply.

    return abs(previousValue - thisValue) >= QUIESCENT_THRESHOLD;

#else   // defined( QUIESCENT_SEARCH_ENABLED )

    return false;

#endif  // defined( QUIESCENT_SEARCH_ENABLED )
}
} // anonymous namespace

GameTree::GameTree(TranspositionTable * pTTable, int maxDepth)
#if defined(USING_TRANSPOSITION_TABLE)
    : transpositionTable_(pTTable)
    , maxDepth_(maxDepth)
#else
    : maxDepth_(maxDepth)
#endif
{
}

GameState GameTree::myBestMove(GameState const & s0, Color my_color)
{
    int depth         = 0;                           // The depth of this ply (this is the current state,  so its depth is 0)
    int responseDepth = depth + 1;                   // Depth of responses to this state
//    int quality         = maxDepth_ - depth;         // Quality of values at this depth (this is the depth of plies searched to
// get
//                                                     // the results for this ply )
//    int responseQuality = maxDepth_ - responseDepth; // Normal quality of responses to this state

    myColor_   = my_color;
    yourColor_ = (my_color == Color::WHITE) ? Color::BLACK : Color::WHITE;

    GameStateList myMoves;

    // Generate a list of all my possible responses to the specified state.
    // The responses have a preliminary value and are sorted by priority and highest value.
    generateStates(s0, true, responseDepth, myMoves);

    // Find the best of the responses... I want to find the state with the highest score

    GameState best_move;
    int       best_score = std::numeric_limits<int>::min();
#if defined(GAME_TREE_ANALYSIS_ENABLED)
    int worst_score = std::numeric_limits<int>::max();
#endif // defined( GAME_TREE_ANALYSIS_ENABLED )

    for (auto & myMove : myMoves)
    {
        // Revise the value of the move by searching the tree of all possible responses
        opponentsAlphaBeta(&myMove, best_score, std::numeric_limits<int>::max(), responseDepth);
#if defined(PRINTING_GAME_TREE_NODE_INFO)
        printStateInfo(myMove, depth, best_score, std::numeric_limits<int>::max());
#endif

#if defined(GAME_TREE_ANALYSIS_ENABLED)

        // If this is the lowest score then save that for analysis

        if (myMove.value_ < worst_score)
            worst_score = myMove.value_;

#endif  // defined( GAME_TREE_ANALYSIS_ENABLED )

        // If this is the highest value so far, then save it (if checkmate, then there is no need to continue looking for better).
        if (myMove.value_ > best_score)
        {
            best_score = myMove.value_;
            best_move  = myMove;
            if (myMove.value_ == MY_CHECKMATE_VALUE)
                break;
        }
    }

    // If the value of my best move is really bad, then just resign
    if (best_score < -StaticEvaluator::RESIGNATION_THRESHOLD)
        best_move.move_ = Move::resign(my_color);

#if defined(GAME_TREE_ANALYSIS_ENABLED)

    // Update analysis data
    analysisData_.worstValue = worst_score;
#if defined(USING_TRANSPOSITION_TABLE)
    analysisData_.tTableAnalysisData = transpositionTable_->analysisData_;
#endif
    analysisData_.gameStateAnalysisData = best_move.analysisData_;

#endif // defined( GAME_TREE_ANALYSIS_ENABLED )

#if defined(USING_TRANSPOSITION_TABLE)
#if defined(TRANSPOSITION_TABLE_ANALYSIS_ENABLED)
    transpositionTable_->resetAnalysisData();
#endif // defined( GAME_TREE_ANALYSIS_ENABLED )
#endif

#if defined(GAME_STATE_ANALYSIS_ENABLED)
    best_move.resetAnalysisData();
#endif // defined( GAME_STATE_ANALYSIS_ENABLED )

    // Return the best move
    return best_move;
}

// Evaluate the state based on all my possible responses. The response I will make is the one with the highest
// value, so that is the value of the state.

void GameTree::myAlphaBeta(GameState * state, int alpha, int beta, int depth)
{
    int responseDepth = depth + 1;                      // Depth of responses to this state
    int quality       = maxDepth_ - depth;              // Quality of values at this depth (this is the depth of plies searched to
                                                        // get
                                                        // the results for this ply )
    int minResponseQuality = maxDepth_ - responseDepth; // Minimum acceptable quality of responses to this state

    // Generate a list of my possible responses to this state sorted descending by value. They are sorted in
    // descending order hoping that a beta cutoff will occur early.
    // Note: Preliminary values of the generated states are retrieved from the transposition table or computed by
    // the static evaluation function.
    GameStateList responses;
    generateStates(*state, true, responseDepth, responses);

    // Evaluate each of my responses and choose the one with the highest score
    int  best_value = std::numeric_limits<int>::min(); // Initialize to worst
    bool pruned     = false;

#if defined(GAME_STATE_ANALYSIS_ENABLED)
    GameState * pBestResponse = 0;
#endif // defined( GAME_STATE_ANALYSIS_ENABLED )

    for (auto & response : responses)
    {
        // The quality of a value is basically the depth of the search tree below it. The reason for checking the quality is that
        // some
        // of the responses have not been fully searched.
        // If the quality of the preliminary value is not as good as the minimum quality and we haven't
        // reached the maximum depth, then do a search. Otherwise, the response's quality is as good as the quality of a search,
        // so use the response as is.
        if ((response.quality_ < minResponseQuality) &&
            ((responseDepth < maxDepth_) ||
             (shouldDoQuiescentSearch(state->value_, response.value_) && (responseDepth < MAX_DEPTH))))
        {
            opponentsAlphaBeta(&response, alpha, beta, responseDepth);
#if defined(PRINTING_GAME_TREE_NODE_INFO)
            printStateInfo(response, depth, alpha, beta);
#endif
        }
#if defined(PRINTING_GAME_TREE_NODE_INFO)
        else
        {
            printStateInfo(response, depth, alpha, beta);
        }
#endif

        // Determine if this response's value is the best so far. If so, then save value and do alpha-beta pruning
        if (response.value_ > best_value)
        {
            // Save it
            best_value = response.value_;

#if defined(GAME_STATE_ANALYSIS_ENABLED)
            pBestResponse = &response;
#endif      // defined( GAME_STATE_ANALYSIS_ENABLED )

            // If this is a checkmate, then there is no reason to look for anything better
            if (best_value == MY_CHECKMATE_VALUE)
                break;

            // alpha-beta pruning (beta cutoff check) Here's how it works:
            //
            // I am looking for the highest value. The 'beta' passed into this function is the value of my
            // opponent's best move found so far. If the value of this response is higher than the beta, then my opponent will not
            // make
            // the move that allows me to choose this response (and its higher value). And since the rest of the responses can only
            // result in a higher value (because I choose the highest value), there is no reason to continue considering any more
            // responses.

            if (best_value > beta)
            {
                // Beta cutoff
                pruned = true;

#if defined(GAME_TREE_ANALYSIS_ENABLED)
                ++analysisData_.betaHitCount;
#endif          // defined( GAME_TREE_ANALYSIS_ENABLED )

                break;
            }

            // alpha-beta pruning (alpha) Here's how it works.
            //
            // I am looking for the highest value. The 'alpha' passed in to this function is the value of
            // my best move found so far. If the value of this response is higher than the alpha, then obviously
            // this is a better move for me. The alpha value is passed to my opponent's move evaluator so that if
            // it finds a response with a lower value than the alpha, it won't bother continuing because it knows
            // that I already have a better move and will choose it instead of allowing him to make a move with
            // worse value for me. So, keep track of the value of my best move so far.

            if (best_value > alpha)
                alpha = best_value;
        }
    }

    // Return the value of this move

    state->value_   = best_value;
    state->quality_ = quality;

#if defined(GAME_STATE_ANALYSIS_ENABLED)

    // Save the expected sequence
    memcpy(&state->analysisData_.expected,
           &pBestResponse->analysisData_.expected,
           sizeof(state->analysisData_.expected));

#endif // defined( GAME_STATE_ANALYSIS_ENABLED )

#if defined(USING_TRANSPOSITION_TABLE)
    // Save the value of the state in the T-table if the ply was not pruned. Pruning results in an incorrect value
    // because the search is not complete. Also, the value is stored only if its quality is better than the quality
    // of the value in the table.
    if (!pruned)
        transpositionTable_->update(*state);

#endif
}

// Evaluate the state based on the values of all my opponent's possible responses. The move my opponent will
// make is the one with the lowest value, so that is the value of S0.

void GameTree::opponentsAlphaBeta(GameState * state, int alpha, int beta, int depth)
{
    int responseDepth = depth + 1;                      // Depth of responses to this state
    int quality       = maxDepth_ - depth;              // Quality of values at this depth (this is the depth of plies searched to
                                                        // get
                                                        // the results for this ply )
    int minResponseQuality = maxDepth_ - responseDepth; // Minimum acceptable quality of responses to this state

    // Generate a list of my opponent's possible responses to this state sorted ascending by value. They are sorted
    // in ascending order hoping that a alpha cutoff will occur early.
    // Note: Preliminary values of the generated states are retrieved from the transposition table or computed by
    // the static evaluation function.
    GameStateList responses;
    generateStates(*state, false, responseDepth, responses);

    // Evaluate each of his responses and choose the one with the lowest score
    int  best_value = std::numeric_limits<int>::max(); // Initialize to worst
    bool pruned     = false;

#if defined(GAME_STATE_ANALYSIS_ENABLED)
    GameState * pBestResponse = 0;
#endif // defined( GAME_STATE_ANALYSIS_ENABLED )

    for (auto & response : responses)
    {
        // The quality of a value is basically the depth of the search tree below it. The reason for checking the quality is that
        // some
        // of the responses have not been fully searched.
        // If the quality of the preliminary value is not as good as the minimum quality and we haven't
        // reached the maximum depth, then do a search. Otherwise, the response's quality is as good as the quality of a search,
        // so use the response as is.
        if ((response.quality_ < minResponseQuality) &&
            ((responseDepth < maxDepth_) ||
             (shouldDoQuiescentSearch(state->value_, response.value_) && (responseDepth < MAX_DEPTH))))
        {
            myAlphaBeta(&response, alpha, beta, responseDepth);
#if defined(PRINTING_GAME_TREE_NODE_INFO)
            printStateInfo(response, depth, alpha, beta);
#endif
        }
#if defined(PRINTING_GAME_TREE_NODE_INFO)
        else
        {
            printStateInfo(response, depth, alpha, beta);
        }
#endif

        // Determine if this response's value is the best so far. If so, then save value and do alpha-beta pruning
        if (response.value_ < best_value)
        {
            // Save it
            best_value = response.value_;

#if defined(GAME_STATE_ANALYSIS_ENABLED)
            pBestResponse = &response;
#endif      // defined( GAME_STATE_ANALYSIS_ENABLED )

            // If this is a checkmate, then there is no reason to look for anything better
            if (best_value == OPPONENT_CHECKMATE_VALUE)
                break;

            // alpha-beta pruning (alpha cutoff check) Here's how it works:
            //
            // My opponent is looking for the lowest value. The 'alpha' passed into this function is the value of
            // my best move found so far. If the value of this response is lower than the alpha, then my opponent
            // obviously prefers it and will choose it. However, because I already can make a move with the value
            // of the alpha, I will choose that move, not the move that allows my opponent to choose this response
            // (and its lower value). Since the rest of the responses can only result in a lower value (because my
            // opponent chooses the lowest value), there is no reason to continue considering these responses.

            if (best_value < alpha)
            {
                // Alpha cutoff
                pruned = true;

#if defined(GAME_TREE_ANALYSIS_ENABLED)
                ++analysisData_.alphaHitCount;
#endif          // defined( GAME_TREE_ANALYSIS_ENABLED )

                break;
            }

            // alpha-beta pruning (beta) Here's how it works.
            //
            // My opponent is looking for the lowest value. The 'beta' passed in to this function is the value of
            // my opponent's best move found so far. If the value of this response is lower than the beta, then
            // obviously this is a better move for my opponent. The beta value is passed to my move evaluator so
            // that if it finds a response with a higher value than the beta, it won't bother continuing because
            // it knows that my opponent already has a better move and will choose it instead of allowing me to
            // make a move with worse value for him. So, keep track of the value of my opponent's best move so far.

            if (best_value < beta)
                beta = best_value;
        }
    }

    // Return the value of this move

    state->value_   = best_value;
    state->quality_ = quality;

#if defined(GAME_STATE_ANALYSIS_ENABLED)

    // Save the expected sequence
    memcpy(&state->analysisData_.expected,
           &pBestResponse->analysisData_.expected,
           sizeof(state->analysisData_.expected));

#endif // defined( GAME_STATE_ANALYSIS_ENABLED )

#if defined(USING_TRANSPOSITION_TABLE)
    // Save the value of the state in the T-table if the ply was not pruned. Pruning results in an incorrect value
    // because the search is not complete. Also, the value is stored only if its quality is better than the quality
    // of the value in the table.
    if (!pruned)
        transpositionTable_->update(*state);

#endif
}

void GameTree::generateStates(GameState const & s0, bool my_move, int depth, GameStateList & states)
{
    int const MAX_POSSIBLE_STATES = 147;

    Board const & board         = s0.board_;                            // Convenience
    Color         current_color = my_move ? myColor_ : yourColor_;

    // Preallocate enough space to hold all the generated states
    states.reserve(MAX_POSSIBLE_STATES);

    // For each square on the board, if it contains a piece on the side whose turn it is, generate all the possible
    // moves for it and add the resulting game states to game state list.
    Position p(0, 0);
    for (p.row = 0; p.row < Board::SIZE; p.row++)
    {
        for (p.column = 0; p.column < Board::SIZE; p.column++)
        {
            Piece const * piece = board.pieceAt(p);

            if ((piece != NO_PIECE) && (piece->color() == current_color))
            {
                Piece::MoveList moves;
                piece->generatePossibleMoves(s0, p, moves);

                // Convert the moves into new states and put the new states into the state list
                for (auto const & move : moves)
                {
                    GameState new_state(s0);
                    new_state.makeMove(current_color, move, depth);

                    // Compute a preliminary value for the state
                    evaluate(&new_state, depth);

#if defined(USING_PRIORITIZED_MOVE_ORDERING)

                    // Determine the new state's priority
                    prioritize(&new_state, depth);
#endif

#if defined(GAME_TREE_ANALYSIS_ENABLED)
                    ++analysisData_.aGeneratedStateCounts[depth];
#endif

                    // Save the new state
                    states.push_back(new_state);
                }
            }
        }
    }

    // Now sort the moves in descending (my moves) or ascending (opponent's moves) value
    if (my_move)
    {
        std::sort(states.begin(), states.end(), [] (GameState const & g0, GameState const & g1)
                  {
                      // Sort the elements in descending order, first by priority, then by value.
#if defined(USING_PRIORITIZED_MOVE_ORDERING)

                      if (g0.priority_ > g1.priority_)
                          return true;

                      if (g0.priority_ < g1.priority_)
                          return false;

#endif      // defined( USING_PRIORITIZED_MOVE_ORDERING )

                      return g0.value_ > g1.value_;
                  }
        );
    }
    else
    {
        std::sort(states.begin(), states.end(), [](GameState const & g0, GameState const & g1)
                  {
                      // Sort the elements in descending order, first by priority, then by value.
#if defined(USING_PRIORITIZED_MOVE_ORDERING)

                      if (g0.priority_ > g1.priority_)
                          return true;

                      if (g0.priority_ < g1.priority_)
                          return false;

#endif      // defined( USING_PRIORITIZED_MOVE_ORDERING )

                      return g0.value_ < g1.value_;
                  }
        );
    }
}

void GameTree::evaluate(GameState * pState, int depth)
{
#if defined(USING_TRANSPOSITION_TABLE)
    // SEF optimization:
    //
    // Since any value of any state in the T-table has already been computed by search and/or SEF, it has a quality
    // that is at least as good as the quality of the value returned by the SEF. So, if the state being evaluated
    // is in the T-table, then the value in the T-table is used instead of running the SEF because T-table
    // lookup is so much faster than the SEF.

    // If it is in the T-table then use that value, otherwise compute the value using SEF.
    if (transpositionTable_->check(*pState, &pState->value_, &pState->quality_))
    {
        // Use the value from the T-table
    }
    else
#endif // if defined(USING_TRANSPOSITION_TABLE)
    {
#if defined(GAME_TREE_ANALYSIS_ENABLED)
        ++analysisData_.aEvaluationCounts[depth];
#endif  // defined( GAME_TREE_ANALYSIS_ENABLED )

#if defined(INCREMENTAL_STATIC_EVALUATION_ENABLED)

        // Note: The static value was already computed during move generation

#if defined(INCREMENTAL_STATIC_EVALUATION_VALIDATION_ENABLED)
        int absoluteValue = StaticEvaluator::evaluate(*pState);

        if (myColor_ == Color::BLACK)
            value = -value;

        ASSERT(pState->value_ == absoluteValue);

#endif         // defined( INCREMENTAL_STATIC_EVALUATION_VALIDATION_ENABLED )

#else   // defined( INCREMENTAL_STATIC_EVALUATION_ENABLED )

        int value = StaticEvaluator::evaluate(*pState);

        // Black has negative value so if I am black then I need to negate the value. This is to make the value in terms
        // of me or my opponent, rather than color.
        if (myColor_ == Color::BLACK)
            value = -value;

        pState->value_   = value;
        pState->quality_ = SEF_QUALITY;

#endif  // defined( INCREMENTAL_STATIC_EVALUATION_ENABLED )

#if defined(USING_TRANSPOSITION_TABLE)
        // Save the value of the state in the T-table
        transpositionTable_->update(*pState);
#endif
    }
}

#if defined(USING_PRIORITIZED_MOVE_ORDERING)

void GameTree::prioritize(GameState * pState, int depth)
{
    int const quality = maxDepth_ - depth;

    // Prioritization strategy:
    //
    // It is assumed that the tree will do a search only if the preliminary quality is lower than the result of
    // searching. So, a preliminary quality as high as the quality at this ply is given a higher priority. The
    // result is that high-priority values are considered first and could potentially hasten the occurrences of
    // alpha-beta cut-offs. The cost of a preliminary value is the cost of a T-table lookup or an SEF, which is
    // much cheaper than the cost of a search. The savings of this strategy is the saving gained by earlier
    // alpha-beta cutoffs minus the costs of the additional SEFs.

    int const PRIORITY_HIGH = 1;
    int const PRIORITY_LOW  = 0;

    pState->priority_ = (pState->quality_ > quality) ? PRIORITY_HIGH : PRIORITY_LOW;
}

#endif // defined( USING_PRIORITIZED_MOVE_ORDERING )

#if defined(GAME_TREE_ANALYSIS_ENABLED)

void GameTree::resetAnalysisData()
{
    analysisData_.reset();
}

void GameTree::AnalysisData::reset()
{
    memset(aGeneratedStateCounts, 0, sizeof(aGeneratedStateCounts));
    memset(aEvaluationCounts, 0, sizeof(aEvaluationCounts));

    alphaHitCount = 0;
    betaHitCount  = 0;
    worstValue    = 0;

    tTableAnalysisData.reset();
    gameStateAnalysisData.reset();
}

GameTree::AnalysisData::AnalysisData()
{
    memset(aGeneratedStateCounts, 0, sizeof(aGeneratedStateCounts));
    memset(aEvaluationCounts, 0, sizeof(aEvaluationCounts));

    alphaHitCount = 0;
    betaHitCount  = 0;
    worstValue    = 0;
}

#endif // defined( GAME_TREE_ANALYSIS_ENABLED )

#if defined(PRINTING_GAME_TREE_NODE_INFO)
void GameTree::printStateInfo(GameState const & state, int depth, int alpha, int beta)
{
    for (int i = 0; i < depth; ++i)
    {
        fprintf(stderr, "    ");
    }

    fprintf(stderr, "%6s, value = %11d, quality = %11d, alpha = %11d, beta = %11d\n",
            state.move_.notation().c_str(), state.value_, state.quality_, alpha, beta);
}

#endif // if defined(PRINTING_GAME_TREE_NODE_INFO)
