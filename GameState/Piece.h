#pragma once

#if !defined(Piece_h__)
#define Piece_h__

#include "GameState/ChessTypes.h"
//#include <memory>
#include <vector>

class CBitmap;
class Board;
//class CBitmap;
class Move;
class GameState;
struct Position;

class Piece
{
public:

    static int constexpr MAX_POSSIBLE_MOVES = 28;           // The maximum number of possible moves for any piece

    using MoveList = std::vector<Move>;

    Piece(PieceTypeId t, Color c);
    virtual ~Piece();

    // Generates all legal moves for this piece
    virtual void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const = 0;

    // Returns true if the move is valid
    virtual bool    isValidMove(GameState const & state, Move const & move) const = 0;

    PieceTypeId     type() const   { return type_;   }
    Color           color() const  { return color_;  }
    char const *    symbol() const { return symbol_; }
//    CBitmap const * image() const  { return image_;  }

    // Returns the symbol for a piece type
    static char const * symbol(PieceTypeId id);

    // Returns the piece corresponding to the specified type and color
    static Piece const * piece(PieceTypeId id, Color color);

protected:

    PieceTypeId type_;          // Type of piece
    Color color_;               // Color of piece
    char const * symbol_;       // Symbol for documentation
//    CBitmap * image_;           // Image of piece

void generateSpanMoves(Board const &     board,
                       Position const &  from,
                       int               dr,
                       int               dc,
                       Piece const *     myPiece,
                       Piece::MoveList & moves) const;

private:
    // NO_PIECE followed by white pieces followed by black pieces
    static Piece const * pieces_[1 + NUMBER_OF_COLORS * NUMBER_OF_PIECE_TYPES];
};

Piece const * const NO_PIECE = nullptr;

inline Piece const * Piece::piece(PieceTypeId id, Color color)
{
    return (id != PieceTypeId::INVALID) ? pieces_[1 + (int)color * NUMBER_OF_PIECE_TYPES + (int)id] : NO_PIECE;
}

#endif // !defined(Piece_h__)
