#if !defined(CHESS_GAMESTATE_H)
#define CHESS_GAMESTATE_H

#pragma once

#include "Board.h"
#include "GamePlayer/GameState.h"
#include "Move.h"
#include "ZHash/ZHash.h"
#include <nlohmann/json.hpp>
#include <queue>
#include <vector>

class Move;
class Piece;

class GameState : public GamePlayer::GameState
{
public:

    using CastleStatus = uint32_t;

    GameState() = default;
    GameState(Board const & board,
              Color         who,
              CastleStatus  castleStatus,
              int           fiftyMoveTimer,
              Move const &  move,
              bool          inCheck,
              int           moveNumber);

    //! Resets the game
    void initialize();

    //! Resets the game to the state specified by the FEN string
    bool initializeFromFen(char const * fen);

    //!@{
    //! @name Overrides Gameplayer::GameState

    uint64_t fingerprint() const override { return zhash_.value(); }
    PlayerId whoseTurn() const override   { return (whoseTurn_ == Color::WHITE) ? PlayerId::FIRST : PlayerId::SECOND; }

    //!@}

    //! Returns true if a castle is allowed
    bool castleIsAllowed(Color c) const;

    //! Returns true if a king-side castle is allowed
    bool kingSideCastleIsAllowed(Color c) const;

    //! Returns true if a queen-side castle is allowed
    bool queenSideCastleIsAllowed(Color c) const;

    //! Returns true if a piece of the specified color can occupy the position
    bool canBeOccupied(Position const & p, Color myColor) const;

    //! Returns the Z hash for this state
    ZHash zhash() const;

    //! Updates the game state with the specified move
    void makeMove(Move const & move);

    //! Returns the FEN string for the state
    std::string fen() const;

    Board board_;               //!< The board
    Color whoseTurn_;           //!< Whose turn
    CastleStatus castleStatus_; //!< Which side has castled and which castles are still possible
    int fiftyMoveTimer_;        //!< Fifty move rule countdown
    Position enPassant_;        //!< En passant target if any
    Move move_;                 //!< The move that resulted in this state
    bool inCheck_;              //!< True if the king is in check
    int moveNumber_;            //!< Move number
    ZHash zhash_;               //!< Zobrist hash for this state

#if defined(ANALYSIS_GAME_STATE)
    struct AnalysisData
    {
        std::vector<Move> expectedLine_;

        void           reset();
        nlohmann::json toJson() const;
    };

    mutable AnalysisData analysisData_;
#endif // defined(ANALYSIS_GAME_STATE)

private:

    friend bool operator ==(GameState const & x, GameState const & y);

    // Updates the game state with a move (but not a castle)
    void makeNormalMove(Move const & move);

    // Updates the game state with a castle move
    void makeCastleMove(Move const & move);

    // Updates the game state with a pawn promotion (after moving). Returns the new piece.
    Piece const * promote(Position const & position);

    bool        whoseTurnFromFen(char const * start, char const * end);
    bool        castleStatusFromFen(char const * start, char const * end);
    bool        fiftyMoveTimerFromFen(char const * start, char const * end);
    bool        moveNumberFromFen(char const * start, char const * end);
    std::string castleStatusToFen() const;
};

bool operator ==(GameState const & x, GameState const & y);

#endif // !defined(CHESS_GAMESTATE_H)
