#pragma once

#if !defined(GameState_h__)
#define GameState_h__

#include "Board.h"
#include "Move.h"
#include "Player/Sequence.h"
#include "ZHash/ZHash.h"

#include <queue>
#include <vector>

class Move;
class Piece;

#define GAME_STATE_ANALYSIS_ENABLED
#define USING_PRIORITIZED_MOVE_ORDERING
#define INCREMENTAL_STATIC_EVALUATION_ENABLED

class GameState
{
public:

    typedef uint32_t CastleStatus;

    GameState() {}
    GameState(Board const & board, CastleStatus castleStatus, Move const & move, int value, bool inCheck);

    bool initializeFromFen(char const * fen);


    // Resets the game
    void initialize();

    // Returns true if a castle is allowed
    bool castleIsAllowed(Color c) const;

    // Returns true if a king-side castle is allowed
    bool kingSideCastleIsAllowed(Color c) const;

    // Returns true if a queen-side castle is allowed
    bool queenSideCastleIsAllowed(Color c) const;

    // Updates the game state with the specified move
    void makeMove(Color color, Move const & move, int depth = 0);

    // Returns the FEN string for the state
    std::string fen() const;

#if defined(GAME_STATE_ANALYSIS_ENABLED)

    static int const EXPECTED_SEQUENCE_SIZE = 10;

    // Analysis data for the last move

    struct AnalysisData
    {
        SequenceEntry expected[EXPECTED_SEQUENCE_SIZE]; // Sequence expected to follow this state

        void reset();
    };

    // Resets all analysis data
    void resetAnalysisData();

    mutable AnalysisData analysisData_;

#endif // defined( GAME_STATE_ANALYSIS_ENABLED )

    Board board_;               // The board
    CastleStatus castleStatus_; // Which side has castled and which castles are still possible

    Move move_;                 // The move that resulted in this state
    int value_;                 // Value of the game state
    int quality_;               // Quality of the value
#if defined(USING_PRIORITIZED_MOVE_ORDERING)
    int priority_;              // Priority of this state (determines sorting order)
#endif // defined( USING_PRIORITIZED_MOVE_ORDERING )
    bool inCheck_;              // True if the king is in check
    ZHash zhash_;               // Hash code for this state

private:

    friend bool operator ==(GameState const & x, GameState const & y);

    // Updates the game state with a move (but not a castle)
    void makeNormalMove(Color color, Move const & move);

    // Updates the game state with a castle move
    void makeCastleMove(Color color, Move const & move);

    // Updates the game state with a pawn promotion (after moving). Returns the new piece.
    Piece const * promote(Color color, Position const & position);

    bool extractColorFromFen(char const * start, char const * end);
    bool extractCastleAvailabilityFromFen(char const * start, char const * end);
    bool extractFiftyMoveTimerFromFen(char const * start, char const * end);
    bool extractMoveNumberFromFen(char const * start, char const * end);
};

// Equality operator
bool operator ==(GameState const & x, GameState const & y);

typedef std::vector<GameState> GameStateList;

template <typename _Cmp>
class GameStateListSorter
{
public:
    bool operator ()(GameState const & g0, GameState const & g1) const
    {
        _Cmp cmp;

        // Sort the elements in descending order, first by priority, then by value.
#if defined(USING_PRIORITIZED_MOVE_ORDERING)

        if (g0.priority_ > g1.priority_)
        {
            return true;
        }

        if (g0.priority_ < g1.priority_)
        {
            return false;
        }

#endif  //defined( USING_PRIORITIZED_MOVE_ORDERING )

        return cmp(g0.value_, g1.value_);
    }
};

#include "GameState/GameState.inl"

#endif // !defined(GameState_h__)
