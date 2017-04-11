#pragma once

#if !defined(Piece_h__)
#define Piece_h__

#include <vector>
#include <memory>
#include "GameState/ChessTypes.h"

class CBitmap;
class Move;
class GameState;

class Piece
{
public:

    static int const MAX_POSSIBLE_MOVES      = 28;                      // The maximum number of possible moves for any piece

    typedef std::vector<Move> MoveList;

    Piece(PieceTypeId t, Color c);
    virtual ~Piece();

    PieceTypeId type() const { return type_; }
    Color color() const { return color_;        }
    char const * symbol() const { return symbol_;       }
    CBitmap const * image() const { return image_;        }

    static char const * symbol(PieceTypeId id);

    // Create a clone of this piece
    virtual std::unique_ptr<Piece> clone() const = 0;

    // Generate all legal moves for this piece
    virtual void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const = 0;

    // Returns true if the move is valid
    virtual bool isValidMove(GameState const & state, Move const & move) const = 0;

protected:

    PieceTypeId type_;                  // Type of piece
    Color color_;                       // Color of piece
    char const * symbol_;               // Symbol for documentation
    CBitmap * image_;                   // Image of piece
};

Piece const * const NO_PIECE = nullptr;

class King : public Piece
{
public:

    King(Color c) : Piece(PieceTypeId::KING, c) {}
//	~King();

    // Create a clone of this piece
    std::unique_ptr<Piece> clone() const { return std::unique_ptr<Piece>(new King(*this) ); }

    // Generate all legal moves for this piece
    void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const;

    // Returns true if the move is valid
    bool isValidMove(GameState const & state, Move const & move) const;
};

class Queen : public Piece
{
public:

    Queen(Color c) : Piece(PieceTypeId::QUEEN, c) {}
//	~Queen();

    // Create a clone of this piece
    std::unique_ptr<Piece> clone() const { return std::unique_ptr<Piece>(new Queen(*this) ); }

    // Generate all legal moves for this piece
    void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const;

    // Returns true if the move is valid
    bool isValidMove(GameState const & state, Move const & move) const;
};

class Rook : public Piece
{
public:

    Rook(Color c) : Piece(PieceTypeId::ROOK, c) {}
//	~Rook();

    // Create a clone of this piece
    std::unique_ptr<Piece> clone() const { return std::unique_ptr<Piece>(new Rook(*this) ); }

    // Generate all legal moves for this piece
    void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const;

    // Returns true if the move is valid
    bool isValidMove(GameState const & state, Move const & move) const;
};

class Bishop : public Piece
{
public:

    Bishop(Color c) : Piece(PieceTypeId::BISHOP, c) {}
//	~Bishop();

    // Create a clone of this piece
    std::unique_ptr<Piece> clone() const { return std::unique_ptr<Piece>(new Bishop(*this) ); }

    // Generate all legal moves for this piece
    void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const;

    // Returns true if the move is valid
    bool isValidMove(GameState const & state, Move const & move) const;
};

class Knight : public Piece
{
public:

    Knight(Color c) : Piece(PieceTypeId::KNIGHT, c) {}
//	~Knight();

    // Create a clone of this piece
    std::unique_ptr<Piece> clone() const { return std::unique_ptr<Piece>(new Knight(*this) ); }

    // Generate all legal moves for this piece
    void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const;

    // Returns true if the move is valid
    bool isValidMove(GameState const & state, Move const & move) const;
};

class Pawn : public Piece
{
public:

    static int const STARTING_ROW_BLACK      = 1;
    static int const STARTING_ROW_WHITE      = 6;

    Pawn(Color c) : Piece(PieceTypeId::PAWN, c) {}
//	~Pawn();

    // Create a clone of this piece
    std::unique_ptr<Piece> clone() const { return std::unique_ptr<Piece>(new Pawn(*this) ); }

    // Generate all legal moves for this piece
    void generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const;

    // Returns true if the move is valid
    bool isValidMove(GameState const & state, Move const & move) const;
};

#endif // !defined(Piece_h__)
