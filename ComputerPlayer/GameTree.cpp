#include "GameTree.h"

#include "StaticEvaluator.h"
#include "TranspositionTable.h"

#include "GameState/Move.h"
#include "GameState/Piece.h"

#include "Misc/Random.h"

#include <functional>
#include <limits>

using json = nlohmann::json;

namespace
{
bool shouldDoQuiescentSearch(float previousValue, float thisValue)
{
#if defined(FEATURE_QUIESCENT_SEARCH)

    // In the normal case, we would not search. However, if the situation is unsettled and we haven't reached the
    // true depth limit, then we should search the next ply.

    float const QUIESCENT_THRESHOLD = 1.0f;
    return abs(previousValue - thisValue) >= QUIESCENT_THRESHOLD;

#else   // defined( FEATURE_QUIESCENT_SEARCH )

    return false;

#endif  // defined( FEATURE_QUIESCENT_SEARCH )
}
} // anonymous namespace

#if defined(FEATURE_TRANSPOSITION_TABLE)
GameTree::GameTree(std::shared_ptr<TranspositionTable> tt, int maxDepth)
#else // defined(FEATURE_TRANSPOSITION_TABLE)
GameTree::GameTree(int maxDepth)
#endif // defined(FEATURE_TRANSPOSITION_TABLE)
    : maxDepth_(maxDepth)
#if defined(FEATURE_TRANSPOSITION_TABLE)
    , transpositionTable_(tt)
#endif // defined(FEATURE_TRANSPOSITION_TABLE)
{
}

GameState GameTree::myBestMove(GameState const & s0, Color my_color)
{
    int constexpr DEPTH          = 0;                      // The depth of this ply (this is the current state, so its depth is 0)
    int constexpr RESPONSE_DEPTH = DEPTH + 1;              // Depth of my responses to this state
//    int quality              = maxDepth_ - DEPTH;          // Quality of values at this depth (this is the depth of plies searched
// to
//                                                           // get the results for this ply)
//    int responseQuality      = maxDepth_ - RESPONSE_DEPTH; // Normal quality of responses to this state

    myColor_   = my_color;
    yourColor_ = (my_color == Color::WHITE) ? Color::BLACK : Color::WHITE;

    EvaluatedGameStateList myMoves;

    // Generate a list of all my possible responses to the specified state.
    // The responses have a preliminary value and are sorted by priority and highest value.
    generateStates(s0, true, RESPONSE_DEPTH, myMoves);

    // Find the best of the responses... I want to find the state with the highest score

    EvaluatedGameState best_move;
    best_move.value_ = std::numeric_limits<float>::min();

#if defined(ANALYSIS_GAME_TREE)
    float worst_score = std::numeric_limits<float>::max();
#endif // defined( ANALYSIS_GAME_TREE )

    for (auto & myMove : myMoves)
    {
        // Revise the value of the move by searching the tree of all possible responses
        opponentsAlphaBeta(&myMove, best_move.value_, std::numeric_limits<float>::max(), RESPONSE_DEPTH);
#if defined(DEBUG_GAME_TREE_NODE_INFO)
        printStateInfo(myMove, DEPTH, best_move.value_, std::numeric_limits<int>::max());
#endif

#if defined(ANALYSIS_GAME_TREE)

        // If this is the lowest score then save that for analysis

        if (myMove.value_ < worst_score)
            worst_score = myMove.value_;

#endif  // defined( ANALYSIS_GAME_TREE )

        // If this is the highest value so far, then save it (if checkmate, then there is no need to continue looking for better).
        if (myMove.value_ > best_move.value_)
        {
            best_move = myMove;
            if (best_move.value_ == MY_CHECKMATE_VALUE)
                break;
        }
    }

    // If the value of my best move is really bad, then just resign
    if (best_move.value_ < -StaticEvaluator::RESIGNATION_THRESHOLD)
        best_move.state_.move_ = Move::resign(my_color);

#if defined(ANALYSIS_GAME_TREE)
    // Update analysis data

    analysisData_.value      = best_move.value_;
    analysisData_.worstValue = worst_score;

#if defined(ANALYSIS_TRANSPOSITION_TABLE)
    analysisData_.ttAnalysisData = transpositionTable_->analysisData_;
    transpositionTable_->analysisData_.reset();
#endif // defined(ANALYSIS_TRANSPOSITION_TABLE)
#endif // defined(ANALYSIS_GAME_TREE)

    // Return the best move
    return best_move.state_;
}

// Evaluate the state based on all my possible responses. The response I will make is the one with the highest value.

void GameTree::myAlphaBeta(EvaluatedGameState * state, float alpha, float beta, int depth)
{
    int responseDepth = depth + 1;                      // Depth of responses to this state
    int quality       = maxDepth_ - depth;              // Quality of values at this depth (this is the depth of plies searched to
                                                        // get the results for this ply)
    int minResponseQuality = maxDepth_ - responseDepth; // Minimum acceptable quality of responses to this state

    // Generate a list of my possible responses to this state sorted descending by preliminary value. They are sorted in
    // descending order hoping that a beta cutoff will occur early.
    // Note: Preliminary values of the generated states are retrieved from the transposition table or computed by
    // the static evaluation function.
    EvaluatedGameStateList responses;
    generateStates(state->state_, true, responseDepth, responses);

    // Evaluate each of my responses and choose the one with the highest score
    float best_value = std::numeric_limits<float>::min();  // Initialize to worst
    bool  pruned     = false;

    for (auto & response : responses)
    {
        // If the game is not over, then let's see what my opponent can do
        if (response.value_ != MY_CHECKMATE_VALUE)
        {
            // The quality of a value is basically the depth of the search tree below it. The reason for checking the quality is
            // that
            // some of the responses have not been fully searched.
            // If the quality of the preliminary value is not as good as the minimum quality and we haven't
            // reached the maximum depth, then do a search. Otherwise, the response's quality is as good as the quality of a search,
            // so use the response as is.
            if ((response.quality_ < minResponseQuality) &&
                ((responseDepth < maxDepth_) ||
                 (shouldDoQuiescentSearch(state->value_, response.value_) && (responseDepth < MAX_DEPTH))))
            {
                opponentsAlphaBeta(&response, alpha, beta, responseDepth);
            }
        }
#if defined(DEBUG_GAME_TREE_NODE_INFO)
        printStateInfo(response, depth, alpha, beta);
#endif

        // Determine if this response's value is the best so far. If so, then save value and do alpha-beta pruning
        if (response.value_ > best_value)
        {
            // Save it
            best_value = response.value_;

            // If this is a checkmate, then there is no reason to look for anything better
            if (best_value == MY_CHECKMATE_VALUE)
                break;

            // alpha-beta pruning (beta cutoff check) Here's how it works:
            //
            // I am looking for the highest value. The 'beta' is the value of my opponent's best move found so far. If the value
            // of this response is higher than the beta, then my opponent will not make the move that allows me to choose this
            // response because it has already found a move with the value of the beta that is better.

            if (best_value > beta)
            {
                // Beta cutoff
                pruned = true;

#if defined(ANALYSIS_GAME_TREE)
                ++analysisData_.betaHitCount;
#endif          // defined( ANALYSIS_GAME_TREE )

                break;
            }

            // alpha-beta pruning (alpha) Here's how it works.
            //
            // I am looking for the highest value. The 'alpha' passed in to this function is the value of
            // my best move found so far. If the value of this response is higher than the alpha, then obviously
            // this is a better move for me. The alpha value is passed to my opponent's move evaluator so that if
            // it finds a response with a lower value than the alpha, it won't bother continuing because it knows
            // that I already have a better move and will choose it instead of allowing him to make a move with
            // worse value for me.

            if (best_value > alpha)
                alpha = best_value;
        }
    }

    // Return the value of this move

    state->value_   = best_value;
    state->quality_ = quality;

#if defined(FEATURE_TRANSPOSITION_TABLE)
    // Save the value of the state in the T-table if the ply was not pruned. Pruning results in an incorrect value
    // because the search is not complete. Also, the value is stored only if its quality is better than the quality
    // of the value in the table.
    if (!pruned)
        transpositionTable_->update(state->state_, state->value_, state->quality_);
#endif // defined(FEATURE_TRANSPOSITION_TABLE)
}

// Evaluate the state based on the values of all my opponent's possible responses. The move my opponent will
// make is the one with the lowest value, so that is the value of S0.

void GameTree::opponentsAlphaBeta(EvaluatedGameState * state, float alpha, float beta, int depth)
{
    int responseDepth = depth + 1;                      // Depth of responses to this state
    int quality       = maxDepth_ - depth;              // Quality of values at this depth (this is the depth of plies searched to
                                                        // get the results for this ply )
    int minResponseQuality = maxDepth_ - responseDepth; // Minimum acceptable quality of responses to this state

    // Generate a list of my opponent's possible responses to this state sorted ascending by value. They are sorted
    // in ascending order hoping that a alpha cutoff will occur early.
    // Note: Preliminary values of the generated states are retrieved from the transposition table or computed by
    // the static evaluation function.
    EvaluatedGameStateList responses;
    generateStates(state->state_, false, responseDepth, responses);

    // Evaluate each of his responses and choose the one with the lowest score
    float best_value = std::numeric_limits<float>::max();  // Initialize to worst
    bool  pruned     = false;

    for (auto & response : responses)
    {
        // If the the game is not over, let's see what I can do
        if (response.value_ != OPPONENT_CHECKMATE_VALUE)
        {
            // The quality of a value is basically the depth of the search tree below it. The reason for checking the quality is
            // that
            // some of the responses have not been fully searched. If the quality of the preliminary value is not as good as the
            // minimum quality and we haven't reached the maximum depth, then do a search. Otherwise, the response's quality is as
            // good as the quality of a search, so use the response as is.
            if ((response.quality_ < minResponseQuality) &&
                ((responseDepth < maxDepth_) ||
                 (shouldDoQuiescentSearch(state->value_, response.value_) && (responseDepth < MAX_DEPTH))))
            {
                myAlphaBeta(&response, alpha, beta, responseDepth);
#if defined(DEBUG_GAME_TREE_NODE_INFO)
                printStateInfo(response, depth, alpha, beta);
#endif
            }
#if defined(DEBUG_GAME_TREE_NODE_INFO)
            else
            {
                printStateInfo(response, depth, alpha, beta);
            }
#endif
        }

        // Determine if this response's value is the best so far. If so, then save value and do alpha-beta pruning
        if (response.value_ < best_value)
        {
            // Save it
            best_value = response.value_;

            // If this is a checkmate, then there is no reason to look for anything better
            if (best_value == OPPONENT_CHECKMATE_VALUE)
                break;

            // alpha-beta pruning (alpha cutoff check) Here's how it works:
            //
            // My opponent is looking for the lowest value. The 'alpha' is the value of my best move found so far.
            // If the value of this response is lower than the alpha, then I should abandon this move because it
            // allows my opponent to make a move that is worse for me than the move I have already found with a
            // value of the alpha.

            if (best_value < alpha)
            {
                // Alpha cutoff
                pruned = true;

#if defined(ANALYSIS_GAME_TREE)
                ++analysisData_.alphaHitCount;
#endif          // defined( ANALYSIS_GAME_TREE )

                break;
            }

            // alpha-beta pruning (beta) Here's how it works.
            //
            // My opponent is looking for the lowest value. The 'beta' passed in to this function is the value of
            // my opponent's best move found so far. If the value of this response is lower than the beta, then
            // obviously this is a better move for my opponent. The beta value is passed to my move evaluator so
            // that if it finds a response with a higher value than the beta, it won't bother continuing because
            // it knows that my opponent already has a better move and will choose it instead of allowing me to
            // make a move with worse value for him.

            if (best_value < beta)
                beta = best_value;
        }
    }

    // Return the value of this move

    state->value_   = best_value;
    state->quality_ = quality;

#if defined(FEATURE_TRANSPOSITION_TABLE)
    // Save the value of the state in the T-table if the ply was not pruned. Pruning results in an incorrect value
    // because the search is not complete. Also, the value is stored only if its quality is better than the quality
    // of the value in the table.
    if (!pruned)
        transpositionTable_->update(state->state_, state->value_, state->quality_);

#endif
}

void GameTree::generateStates(GameState const & s0, bool my_move, int depth, EvaluatedGameStateList & states)
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

            if (piece && (piece->color() == current_color))
            {
                Piece::MoveList moves;
                piece->generatePossibleMoves(s0, p, moves);

                // Convert the moves into new states and put the new states into the state list
                for (auto const & move : moves)
                {
                    EvaluatedGameState new_state{ s0, 0, 0 };
                    new_state.state_.makeMove(current_color, move);

                    // Compute a preliminary value for the state
                    evaluate(new_state.state_, depth, &new_state.value_, &new_state.quality_);

#if defined(FEATURE_PRIORITIZED_MOVE_ORDERING)

                    // Determine the new state's priority
                    new_state.priority_ = prioritize(new_state, depth);
#endif

#if defined(ANALYSIS_GAME_TREE)
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
        std::sort(states.begin(), states.end(), [] (EvaluatedGameState const & g0, EvaluatedGameState const & g1)
                  {
                      // Sort the elements in descending order, first by priority, then by value.
#if defined(FEATURE_PRIORITIZED_MOVE_ORDERING)

                      if (g0.priority_ > g1.priority_)
                          return true;

                      if (g0.priority_ < g1.priority_)
                          return false;

#endif      // defined( FEATURE_PRIORITIZED_MOVE_ORDERING )

                      return g0.value_ > g1.value_;
                  }
        );
    }
    else
    {
        std::sort(states.begin(), states.end(), [](EvaluatedGameState const & g0, EvaluatedGameState const & g1)
                  {
                      // Sort the elements in descending order, first by priority, then by value.
#if defined(FEATURE_PRIORITIZED_MOVE_ORDERING)

                      if (g0.priority_ > g1.priority_)
                          return true;

                      if (g0.priority_ < g1.priority_)
                          return false;

#endif      // defined( FEATURE_PRIORITIZED_MOVE_ORDERING )

                      return g0.value_ < g1.value_;
                  }
        );
    }
}

void GameTree::evaluate(GameState const & state, int depth, float * pValue, int * pQuality)
{
#if defined(FEATURE_TRANSPOSITION_TABLE)
    // SEF optimization:
    //
    // Since any value of any state in the T-table has already been computed by search and/or SEF, it has a quality
    // that is at least as good as the quality of the value returned by the SEF. So, if the state being evaluated
    // is in the T-table, then the value in the T-table is used instead of running the SEF because T-table
    // lookup is so much faster than the SEF.

    // If it is in the T-table then use that value, otherwise compute the value using SEF.
    if (transpositionTable_->check(state, pValue, pQuality))
    {
        // Use the value from the T-table
    }
    else
#endif // if defined(FEATURE_TRANSPOSITION_TABLE)
    {
#if defined(ANALYSIS_GAME_TREE)
        ++analysisData_.aEvaluationCounts[depth];
#endif  // defined( ANALYSIS_GAME_TREE )

#if defined(FEATURE_INCREMENTAL_STATIC_EVALUATION)

        // Note: The static value was already computed during move generation

#if defined(FEATURE_INCREMENTAL_STATIC_EVALUATION_VALIDATION)
        int absoluteValue = StaticEvaluator::evaluate(*pState);

        if (myColor_ == Color::BLACK)
            value = -value;

        ASSERT(state.value_ == absoluteValue);

#endif  // defined( FEATURE_INCREMENTAL_STATIC_EVALUATION_VALIDATION )

#else   // defined( FEATURE_INCREMENTAL_STATIC_EVALUATION )

        float value = StaticEvaluator::evaluate(state);

        // Black has negative value so if I am black then I need to negate the value. This is to make the value in terms
        // of me or my opponent, rather than color.
        if (myColor_ == Color::BLACK)
            value = -value;

        *pValue   = value;
        *pQuality = SEF_QUALITY;

#endif  // defined( FEATURE_INCREMENTAL_STATIC_EVALUATION )

#if defined(FEATURE_TRANSPOSITION_TABLE)
        // Save the value of the state in the T-table
        transpositionTable_->update(state, *pValue, *pQuality);
#endif
    }
}

#if defined(FEATURE_PRIORITIZED_MOVE_ORDERING)

int GameTree::prioritize(EvaluatedGameState const & state, int depth)
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

    return (state.quality_ > quality) ? PRIORITY_HIGH : PRIORITY_LOW;
}

#endif // defined( FEATURE_PRIORITIZED_MOVE_ORDERING )

#if defined(ANALYSIS_GAME_TREE)

GameTree::AnalysisData::AnalysisData()
    : value(0)
    , worstValue(0)
    , alphaHitCount(0)
    , betaHitCount(0)
{
    memset(aGeneratedStateCounts, 0, sizeof(aGeneratedStateCounts));
    memset(aEvaluationCounts, 0, sizeof(aEvaluationCounts));
}

void GameTree::AnalysisData::reset()
{
    memset(aGeneratedStateCounts, 0, sizeof(aGeneratedStateCounts));
    memset(aEvaluationCounts, 0, sizeof(aEvaluationCounts));

    value         = 0.0f;
    worstValue    = 0.0f;
    alphaHitCount = 0;
    betaHitCount  = 0;

#if defined(ANALYSIS_TRANSPOSITION_TABLE)
    ttAnalysisData.reset();
#endif // defined(ANALYSIS_TRANSPOSITION_TABLE)
}

json GameTree::AnalysisData::toJson() const
{
    json out =
    {
        { "aGeneratedStateCounts", aGeneratedStateCounts },
        { "aEvaluationCounts", aEvaluationCounts },
        { "value", value },
        { "worstValue", worstValue },
        { "alphaHitCount", alphaHitCount },
        { "betaHitCount", betaHitCount }

#if defined(ANALYSIS_TRANSPOSITION_TABLE)
        , { "transpositonTable", ttAnalysisData.toJson() }
#endif  // defined(ANALYSIS_TRANSPOSITION_TABLE)
    };
    return out;
}
#endif // defined( ANALYSIS_GAME_TREE )

#if defined(DEBUG_GAME_TREE_NODE_INFO)
void GameTree::printStateInfo(GameState const & state, int depth, int alpha, int beta)
{
    for (int i = 0; i < depth; ++i)
    {
        fprintf(stderr, "    ");
    }

    fprintf(stderr, "%6s, value = %6.2f, quality = %3d, alpha = %6.2f, beta = %6.2f\n",
            state.move_.notation().c_str(), state.value_, state.quality_, alpha, beta);
}

#endif // if defined(DEBUG_GAME_TREE_NODE_INFO)
