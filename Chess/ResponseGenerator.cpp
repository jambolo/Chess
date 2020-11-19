#include "ResponseGenerator.h"

#include "GameState.h"
#include "Piece.h"
#include <vector>

std::vector<GamePlayer::GameState *> ResponseGenerator::operator ()(GamePlayer::GameState const & state, int depth)
{
    int constexpr MAX_POSSIBLE_STATES = 147;

    std::vector<GamePlayer::GameState *> rv;
    rv.reserve(MAX_POSSIBLE_STATES);

    // For each square on the board, if it contains a piece on the side whose turn it is, generate all the possible
    // moves for it and add the resulting game states to responses list.
    Position p(0, 0);
    for (p.row = 0; p.row < Board::SIZE; p.row++)
    {
        for (p.column = 0; p.column < Board::SIZE; p.column++)
        {
            Piece const * piece = static_cast<GameState const &>(state).board_.pieceAt(p);

            if (piece && (piece->color() == static_cast<GameState const &>(state).whoseTurn_))
            {
                Piece::MoveList moves;
                piece->generatePossibleMoves(static_cast<GameState const &>(state), p, moves);

                // Convert the moves into new states and put the new states into the state list
                for (auto const & move : moves)
                {
                    GameState * newState = new GameState(static_cast<GameState const &>(state));
                    newState->makeMove(move);

#if defined(FEATURE_PRIORITIZED_MOVE_ORDERING)
                    // Determine the new state's priority
                    newState.priority_ = prioritize(newState, depth);
#endif

                    // Save the new state
                    rv.push_back(newState);
                }
            }
        }
    }
    return rv;
}
