#pragma once

#if !defined(Board_h__)
#define Board_h__

#include "GameState/ChessTypes.h"

class PieceList;
class Piece;
struct Position;

class Board
{
public:
    static int const SIZE = 8; // Number of rows and columns

    // A reference to a particular piece on the board
    typedef unsigned char PieceId;

    // An empty square has this value
    static PieceId const EMPTY_SQUARE = 0;

    // Reset the board
    void initialize(typename PieceList const & white_pieces, typename PieceList const & black_pieces);

    // Returns true if p is a valid location
    static bool isValidPosition(Position const & p);

    // Returns the piece at the given position or EMPTY_SQUARE, if none
    PieceId pieceIdAt(Position const & p) const;
    PieceId pieceIdAt(int r, int c) const;

    // Returns the piece at the given position or NO_PIECE, if none
    Piece const * pieceAt(Position const & p) const;
    Piece const * pieceAt(int r, int c) const;

    // Places the piece on the board at the given position
    void putPiece(PieceId piece, Position const & to);
    void putPiece(Piece const * piece, Position const & to);

    // Removes the piece at the given position from the board
    void removePiece(Position const & p);

    // Moves the piece at 'from' to 'to' (does not check legality)
    void movePiece(Position const & from, Position const & to);

    // Returns true if the span from 'from' to 'to' (excluding 'to') contains no pieces
    bool spanIsEmpty(Position const & from, Position const & to) const;

    // Returns the referenced piece
    static Piece const * piece(PieceId id);

    // Returns the piece id of a piece
    static PieceId pieceId(Piece const * piece);

private:

    friend bool operator == (Board const & x, Board const & y);

    // Remove all pieces from the board
    void clear();

    // Convert a PieceTypeId to a PieceId
    static PieceId pieceId(PieceTypeId type, Color color);

    PieceId board_[SIZE][SIZE]; // The board

    static Piece const * pieces_[1 + NUMBER_OF_COLORS * NUMBER_OF_PIECE_TYPES];
};

// Returns true if the other board is the same as this one
bool operator == (Board const & x, Board const & y);

// Inline functions

#include "Board.inl"

#endif // !defined(Board_h__)
