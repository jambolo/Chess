#pragma once

#if !defined(GameState_h__)
#define GameState_h__

#include "Board.h"
#include "Move.h"
#include "ZHash/ZHash.h"

#if defined(FEATURE_GAME_STATE_ANALYSIS)
#include "ComputerPlayer/Sequence.h"
#endif // defined(FEATURE_GAME_STATE_ANALYSIS)

#include <queue>
#include <vector>

class Move;
class Piece;

class GameState
{
public:

    using CastleStatus = uint32_t;

    GameState() = default;
    GameState(Board const & board,
              Color         whoseTurn_,
              CastleStatus  castleStatus,
              int           fiftyMoveTimer,
              Move const &  move,
              bool          inCheck,
              int           moveNumber);

    // Resets the game
    void initialize();

    bool initializeFromFen(char const * fen);

    // Returns true if a castle is allowed
    bool castleIsAllowed(Color c) const;

    // Returns true if a king-side castle is allowed
    bool kingSideCastleIsAllowed(Color c) const;

    // Returns true if a queen-side castle is allowed
    bool queenSideCastleIsAllowed(Color c) const;

    // Returns true if a piece of the specified color can occupy the position
    bool canBeOccupied(Position const & p, Color myColor) const;

    // Returns the Z hash for this state
    ZHash zhash() const;

    // Updates the game state with the specified move
    void makeMove(Color color, Move const & move, int depth = 0);

    // Returns the FEN string for the state
    std::string fen() const;

#if defined(FEATURE_GAME_STATE_ANALYSIS)

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

#endif // defined( FEATURE_GAME_STATE_ANALYSIS )

    Board board_;               // The board
    CastleStatus castleStatus_; // Which side has castled and which castles are still possible
    Color whoseTurn_;           // Whose turn
    int fiftyMoveTimer_;        // Fifty move rule countdown
    Position enPassant_;        // En passant target if any
    Move move_;                 // The move that resulted in this state
    bool inCheck_;              // True if the king is in check
    int moveNumber_;            // Move number
    ZHash zhash_;               // Zobrist hash for this state

private:

    friend bool operator ==(GameState const & x, GameState const & y);

    // Updates the game state with a move (but not a castle)
    void makeNormalMove(Color color, Move const & move);

    // Updates the game state with a castle move
    void makeCastleMove(Color color, Move const & move);

    // Updates the game state with a pawn promotion (after moving). Returns the new piece.
    Piece const * promote(Color color, Position const & position);

    bool        whoseTurnFromFen(char const * start, char const * end);
    bool        castleStatusFromFen(char const * start, char const * end);
    bool        fiftyMoveTimerFromFen(char const * start, char const * end);
    bool        moveNumberFromFen(char const * start, char const * end);
    std::string castleStatusToFen() const;
};

// Equality operator
bool operator ==(GameState const & x, GameState const & y);

// List of game states
using GameStateList = std::vector<GameState>;

#endif // !defined(GameState_h__)
