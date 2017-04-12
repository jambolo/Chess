#pragma once

#if !defined(Piece_h__)
#define Piece_h__

#include "gamestate/ChessTypes.h"
#include <memory>
#include <vector>

class CBitmap;
class Move;
class GameState;

class Piece
{
public:

    static int const MAX_POSSIBLE_MOVES = 28;           // The maximum number of possible moves for any piece

    typedef std::vector<Move> MoveList;

    Piece(PieceTypeId t, Color c);
    virtual ~Piece();

    PieceTypeId     type() const   { return type_;   }
    Color           color() const  { return color_;  }
    char const *    symbol() const { return symbol_; }
    CBitmap const * image() const  { return image_;  }

    // Returns the symbol for a piece type
    static char const * symbol(PieceTypeId id);

    // Returns the piece corresponding to the specified type and color
    static Piece const * piece(PieceTypeId id,
                               Color       color) { return (id !=
                                                            PieceTypeId::INVALID) ? pieces_[1 + (int)color * NUMBER_OF_PIECE_TYPES +
                                                                                            (int)id] : NO_PIECE; }

    // Generates all legal moves for this piece
    virtual void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const = 0;

    // Returns true if the move is valid
    virtual bool isValidMove(GameState const & state, Move const & move) const = 0;

protected:

    PieceTypeId type_;          // Type of piece
    Color color_;               // Color of piece
    char const * symbol_;       // Symbol for documentation
    CBitmap * image_;           // Image of piece

private:
    static Piece const * pieces_[1 + NUMBER_OF_COLORS * NUMBER_OF_PIECE_TYPES];
};

Piece const * const NO_PIECE = nullptr;

class King : public Piece
{
public:

    King(Color c) : Piece(PieceTypeId::KING, c) {}

    // Generates all legal moves for this piece
    void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const override;

    // Returns true if the move is valid
    bool isValidMove(GameState const & state, Move const & move) const override;
};

class Queen : public Piece
{
public:

    Queen(Color c) : Piece(PieceTypeId::QUEEN, c) {}

    // Generates all legal moves for this piece
    void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const override;

    // Returns true if the move is valid
    bool isValidMove(GameState const & state, Move const & move) const override;
};

class Rook : public Piece
{
public:

    Rook(Color c) : Piece(PieceTypeId::ROOK, c) {}

    // Generates all legal moves for this piece
    void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const override;

    // Returns true if the move is valid
    bool isValidMove(GameState const & state, Move const & move) const override;
};

class Bishop : public Piece
{
public:

    Bishop(Color c) : Piece(PieceTypeId::BISHOP, c) {}

    // Generates all legal moves for this piece
    void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const override;

    // Returns true if the move is valid
    bool isValidMove(GameState const & state, Move const & move) const override;
};

class Knight : public Piece
{
public:

    Knight(Color c) : Piece(PieceTypeId::KNIGHT, c) {}

    // Generates all legal moves for this piece
    void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const override;

    // Returns true if the move is valid
    bool isValidMove(GameState const & state, Move const & move) const override;
};

class Pawn : public Piece
{
public:

    static int const STARTING_ROW_BLACK = 1;
    static int const STARTING_ROW_WHITE = 6;

    Pawn(Color c) : Piece(PieceTypeId::PAWN, c) {}

    // Generates all legal moves for this piece
    void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const override;

    // Returns true if the move is valid
    bool isValidMove(GameState const & state, Move const & move) const override;
};

#endif // !defined(Piece_h__)
