#pragma once

#if !defined(Move_h__)
#define Move_h__

#include <vector>
#include "GameState/ChessTypes.h"

class Move
{
public:

    typedef std::vector<Move> List;

    // Values for the special moves. The values are shifted to leave room for board locations when needed

    enum class SpecialMoveId
    {
        RESIGN              = 1 << 3,
        UNDO                = 2 << 3,
        RESET               = 3 << 3,
        KINGSIDE_CASTLE     = 4 << 3,
        QUEENSIDE_CASTLE    = 5 << 3,
        PROMOTION           = 6 << 3,
        ENPASSANT           = 7 << 3
    };

    Move() {}
    Move(Position const & f, Position const & t);
    Move(SpecialMoveId move);
    Move(SpecialMoveId move, Position const & f, Position const & t);

    // Returns the from position
    Position const from() const;

    // Returns the to position
    Position const to() const;

    // Returns true if this is a special move
    bool isSpecial() const;

    // Returns true if this move is a resignation
    bool isResignation() const { return isSpecial(SpecialMoveId::RESIGN); }

    // Returns true if this is an undo
    bool isUndo() const { return isSpecial(SpecialMoveId::UNDO); }

    // Returns true if this is the starting position
    bool isStartingPosition() const { return isSpecial(SpecialMoveId::RESET); }

    // Returns true if this is a king-side castle
    bool isKingSideCastle() const { return isSpecial(SpecialMoveId::KINGSIDE_CASTLE); }

    // Returns true if this is a queen-side castle
    bool isQueenSideCastle() const { return isSpecial(SpecialMoveId::QUEENSIDE_CASTLE); }

    // Returns true if the pawn is promoted
    bool isPromotion() const { return isSpecial(SpecialMoveId::PROMOTION); }

    // Returns true if this is en passant
    bool isEnPassant() const { return isSpecial(SpecialMoveId::ENPASSANT); }

    // Special move -- resignation
    static Move resign() { return Move(SpecialMoveId::RESIGN); }

    // Special move -- undo
    static Move undo() { return Move(SpecialMoveId::UNDO); }

    // Special move -- initial board
    static Move reset() { return Move(SpecialMoveId::RESET); }

    // Special move -- King-side castle
    static Move kingSideCastle() { return Move(SpecialMoveId::KINGSIDE_CASTLE); }

    // Special move -- Queen-side castle
    static Move queenSideCastle() { return Move(SpecialMoveId::QUEENSIDE_CASTLE); }

    // Special move -- Promotion
    static Move promotion(Position const & from, Position const & to) { return Move(SpecialMoveId::PROMOTION, from, to); }

    // Special move -- En passant
    static Move enPassant(Position const & from, Position const & to) { return Move(SpecialMoveId::ENPASSANT, from, to); }

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
    bool isSpecial(SpecialMoveId move) const;

    Position from_;
    Position to_;
};

#include "Move.inl"

#endif // !defined(Move_h__)
