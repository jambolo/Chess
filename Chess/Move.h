#pragma once

#if !defined(Move_h__)
#define Move_h__

#include "Chess/Position.h"
#include "Chess/Types.h"
#include <vector>

class Piece;

class Move
{
public:
    // Values for the special moves. The values are shifted to leave room for board locations when needed
    enum Special
    {
        NORMAL = 0,
        KINGSIDE_CASTLE,
        QUEENSIDE_CASTLE,
        PROMOTION,
        ENPASSANT,
        RESIGN,
        UNDO,
        RESET
    };
    Move() = default;
    Move(Piece const * piece, Position const & from, Position const & to, bool capture = false);
    Move(Special          special,
         Color            color   = Color::INVALID,
         Position const & from    = Position(),
         Position const & to      = Position(),
         bool             capture = false);

    // Returns the piece being moved
    Piece const * piece() const { return piece_; }

    // Returns the from position
    Position from() const { return from_; }

    // Returns the to position
    Position to() const { return to_; }

    // Returns the kind of special move (or NORMAL)
    Special special() const { return special_; }

    // Returns true if this is a special move
    bool isSpecial() const { return special_ != NORMAL;  }

    // Returns true if this move is a resignation
    bool isResignation() const { return special_ == RESIGN;  }

    // Returns true if this is an undo
    bool isUndo() const { return special_ == UNDO;  }

    // Returns true if this is the starting position
    bool isStartingPosition() const { return special_ == RESET;  }

    // Returns true if this is a king-side castle
    bool isKingSideCastle() const { return special_ == KINGSIDE_CASTLE;  }

    // Returns true if this is a queen-side castle
    bool isQueenSideCastle() const { return special_ == QUEENSIDE_CASTLE;  }

    // Returns true if the pawn is promoted
    bool isPromotion() const { return special_ == PROMOTION;  }

    // Returns true if this is en passant
    bool isEnPassant() const { return special_ == ENPASSANT;  }

    // Returns true if the move is marked as a capture
    bool isCapture() const { return capture_; }

    // Returns true if the difference is non-zero
    static bool isMoved(int dr, int dc);

    // Returns true if the difference is diagonal
    static bool isDiagonal(int dr, int dc);

    // Returns true if the difference is with the given range
    static bool inRange(int dr, int dc, int range);

    // Returns true if the difference is along a row or a column
    static bool isSquare(int dr, int dc);

    // Returns the move in notation form
    std::string notation(Notation notation = Notation::STANDARD) const;

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
    static Move promotion(Color color, Position const & from, Position const & to, bool capture = false)
    {
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

    std::string standardNotation() const;
    std::string longNotation() const;
    std::string pgnNotation() const;
    std::string figurineNotation() const;
    std::string iccfNotation() const;
    std::string uciNotation() const;

    Piece const * piece_;   // The piece that moved
    Position from_;         // Beginning location
    Position to_;           // Ending location
    bool capture_;          // True if this is a capture
    Special special_;       // Kind of move
};

#endif // !defined(Move_h__)
