#pragma once

#if !defined(Move_h__)
#define Move_h__

#include "gamestate/ChessTypes.h"
#include "gamestate/Position.h"
#include <vector>

class Piece;

class Move
{
public:
    // Values for the special moves. The values are shifted to leave room for board locations when needed
    static int const RESIGN = 1 << 3;
    static int const UNDO   = 2 << 3;
    static int const RESET  = 3 << 3;
    static int const KINGSIDE_CASTLE  = 4 << 3;
    static int const QUEENSIDE_CASTLE = 5 << 3;
    static int const PROMOTION        = 6 << 3;
    static int const ENPASSANT        = 7 << 3;

    Move() {}
    Move(Piece const * piece, Position const & from, Position const & to, bool capture = false);
    Move(int              special,
         Color            color = Color::INVALID,
         Position const & from = Position(),
         Position const & to = Position(),
         bool             capture = false);

    // Returns the from position
    Position const from() const;

    // Returns the to position
    Position const to() const;

    // Returns true if this is a special move
    bool isSpecial() const;

    // Returns true if this move is a resignation
    bool isResignation() const { return isSpecial(RESIGN); }

    // Returns true if this is an undo
    bool isUndo() const { return isSpecial(UNDO); }

    // Returns true if this is the starting position
    bool isStartingPosition() const { return isSpecial(RESET); }

    // Returns true if this is a king-side castle
    bool isKingSideCastle() const { return isSpecial(KINGSIDE_CASTLE); }

    // Returns true if this is a queen-side castle
    bool isQueenSideCastle() const { return isSpecial(QUEENSIDE_CASTLE); }

    // Returns true if the pawn is promoted
    bool isPromotion() const { return isSpecial(PROMOTION); }

    // Returns true if this is en passant
    bool isEnPassant() const { return isSpecial(ENPASSANT); }

    // Returns the move in SAN form
    std::string Move::san() const;

    // Special move -- resignation
    static Move resign(Color color) { return Move(RESIGN, color); }

    // Special move -- undo
    static Move undo() { return Move(UNDO); }

    // Special move -- initial board
    static Move reset() { return Move(RESET); }

    // Special move -- King-side castle
    static Move kingSideCastle(Color color) { return Move(KINGSIDE_CASTLE, color); }

    // Special move -- Queen-side castle
    static Move queenSideCastle(Color color) { return Move(QUEENSIDE_CASTLE, color); }

    // Special move -- Promotion
    static Move promotion(Color color, Position const & from, Position const & to, bool capture = false) {
        return Move(PROMOTION,
                    color,
                    from,
                    to,
                    capture);
    }

    // Special move -- En passant
    static Move enPassant(Color color, Position const & from, Position const & to) { return Move(ENPASSANT, color, from, to); }

    // Returns the king's move in a king-side castle
    static Move kingSideCastleKing(Color c);

    // Returns the rook's move in a king-side castle
    static Move kingSideCastleRook(Color c);

    // Returns the king's move in a queen-side castle
    static Move queenSideCastleKing(Color c);

    // Returns the rook's move in a queen-side castle
    static Move queenSideCastleRook(Color c);

private:

    static int const NORMAL_MOVE_MASK = 0x7;

    // Returns true if this is the specified special move
    bool isSpecial(int move) const;

    Piece const * piece_;   // The piece that moved
    Position from_;         // Beginning location
    Position to_;           // Ending location
    bool capture_;          // True if this is a capture

public:
};

#include "Move.inl"

#endif // !defined(Move_h__)
