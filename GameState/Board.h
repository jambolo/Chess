/** @file *//********************************************************************************************************

                                                       Board.h

                                            Copyright 2004, John J. Bolton
    --------------------------------------------------------------------------------------------------------------

    $Header: //depot/Chess/Board.h#7 $

    $NoKeywords: $

********************************************************************************************************************/

#pragma once

#include "GameState/ChessTypes.h"

class PieceList;
class Piece;
class Position;

class Board
{
public:
    static int const SIZE = 8;          // Number of rows and columns

    Board();

    // Initialize the board to the standard starting position
    void initialize();

    // Initialize the board using FEN description
    void initialize(std::string const & fen)

    // Returns true if p is a valid location
    static bool isValidPosition(Position const & p);

    // Returns the piece at the given position or NO_PIECE, if none
    Piece const * pieceAt(Position const & p) const;
    Piece const * pieceAt(int r, int c) const;

    // Places the piece on the board at the given position
    void putPiece(Piece const * piece, Position const & to);

    // Removes the piece at the given position from the board
    void removePiece(Position const & p);
    void removePiece(int r, int c);

    // Moves the piece at 'from' to 'to' (does not check legality)
    void movePiece(Position const & from, Position const & to);

    // Returns true if the span from 'from' to 'to' (excluding 'to') contains no pieces
    bool spanIsEmpty(Position const & from, Position const & to) const;

private:

    friend bool operator ==(Board const & x, Board const & y);

    // Remove all pieces from the board
    void clear();

    Piece const * board_[SIZE][SIZE]; // The board
};

// Returns true if the other board is the same as this one
bool operator ==(Board const & x, Board const & y);

// Inline functions

#include "Board.inl"
