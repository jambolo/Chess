#if !defined(CHESS_PIECE_H)
#define CHESS_PIECE_H

#pragma once

#include "Chess/Types.h"
#include <vector>

class CBitmap;
class Board;
class Move;
class GameState;
struct Position;
class BitBoard;

class Piece
{
public:
    using MoveList = std::vector<Move>;

    Piece(PieceTypeId t, Color c);
    virtual ~Piece()     = default;
    Piece(const Piece &) = delete;              // Non-copyable
    Piece & operator =(const Piece &) = delete; // Non-copyable

    // Generates all legal moves for this piece
    virtual void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const = 0;

    // Counts the number of legal moves for this piece
    virtual int countPossibleMoves(GameState const & state, Position const & from) const = 0;

    // Counts the number of legal moves for this piece
    virtual int countThreats(GameState const & state, Position const & from) const = 0;

    // Returns true if the move is valid
    virtual bool isValidMove(GameState const & state, Move const & move) const = 0;

    PieceTypeId  type() const     { return type_;   }
    Color        color() const    { return color_;  }
    char const * symbol() const   { return symbol_; }
    char const * figurine() const { return figurine_; }

    // Returns the text symbol for a piece type
    static char const * symbol(PieceTypeId id, Color color = Color::WHITE);

    // Returns the figurine symbol for a piece type (as UTF8)
    static char const * figurine(PieceTypeId id, Color color);

    // Returns the piece corresponding to the specified type and color
    static Piece const * get(PieceTypeId id, Color color);

protected:

    PieceTypeId type_;          // Type of piece
    Color color_;               // Color of piece
    char const * symbol_;       // Text symbol for documentation
    char const * figurine_;  // Figurine symbol for documentation

    void generateSpanMoves(Board const &     board,
                           Position const &  from,
                           int               dr,
                           int               dc,
                           Piece::MoveList & moves) const;

    int countSpanMoves(Board const &    board,
                       Position const & from,
                       int              dr,
                       int              dc) const;

    int countSpanThreats(Board const &    board,
                         Position const & from,
                         int              dr,
                         int              dc) const;

#if defined(FEATURE_BITBOARD_MOVE_GENERATION)
    static void extractMoves(BitBoard b, MoveList & list);
#endif // defined(FEATURE_BITBOARD_MOVE_GENERATION)

private:

    // NO_PIECE followed by white pieces followed by black pieces
    static Piece const * const pieces_[1 + NUMBER_OF_COLORS * NUMBER_OF_PIECE_TYPES];
};

Piece const * const NO_PIECE = nullptr;

inline Piece const * Piece::get(PieceTypeId id, Color color)
{
    return (id != PieceTypeId::INVALID) ? pieces_[1 + (int)color * NUMBER_OF_PIECE_TYPES + (int)id] : NO_PIECE;
}

#endif // !defined(CHESS_PIECE_H)
