#if !defined(CHESS_BOARD)
#define CHESS_BOARD

#pragma once

#include "Chess/Types.h"
#include <string>

class Piece;
struct Position;

class Board
{
public:
    static int constexpr SIZE = 8; // Number of rows and columns

    Board();

    // Initialize the board to the standard starting position
    void initialize();

    // Initialize the board using FEN description
    bool initializeFromFen(char const * start, char const * end);

    // Returns true if p is a valid location
    static bool isValidPosition(Position const & p);

    // Returns the piece at the given position or NO_PIECE, if none
    Piece const * pieceAt(Position const & p) const;
    Piece const * pieceAt(int r, int c) const;

    // Places the piece on the board at the given position
    void putPiece(Piece const * piece, Position const & to);
    void putPiece(Piece const * piece, int r, int c);

    // Removes the piece at the given position from the board
    void removePiece(Position const & p);
    void removePiece(int r, int c);

    // Moves the piece at 'from' to 'to' (does not check legality)
    void movePiece(Position const & from, Position const & to);

    // Returns true if the span from 'from' to 'to' (excluding 'from' and 'to') contains no pieces
    bool spanIsEmpty(Position const & from, Position const & to) const;

    // Return the board as a FEN string
    std::string fen() const;

    static Position const INITIAL_BLACK_ROOK_QUEENSIDE_POSITION;
    static Position const INITIAL_BLACK_KNIGHT_QUEENSIDE_POSITION;
    static Position const INITIAL_BLACK_BISHOP_QUEENSIDE_POSITION;
    static Position const INITIAL_BLACK_QUEEN_POSITION;
    static Position const INITIAL_BLACK_KING_POSITION;
    static Position const INITIAL_BLACK_BISHOP_KINGSIDE_POSITION;
    static Position const INITIAL_BLACK_KNIGHT_KINGSIDE_POSITION;
    static Position const INITIAL_BLACK_ROOK_KINGSIDE_POSITION;
    static Position const INITIAL_WHITE_ROOK_QUEENSIDE_POSITION;
    static Position const INITIAL_WHITE_KNIGHT_QUEENSIDE_POSITION;
    static Position const INITIAL_WHITE_BISHOP_QUEENSIDE_POSITION;
    static Position const INITIAL_WHITE_QUEEN_POSITION;
    static Position const INITIAL_WHITE_KING_POSITION;
    static Position const INITIAL_WHITE_BISHOP_KINGSIDE_POSITION;
    static Position const INITIAL_WHITE_KNIGHT_KINGSIDE_POSITION;
    static Position const INITIAL_WHITE_ROOK_KINGSIDE_POSITION;

private:

    friend bool operator ==(Board const & x, Board const & y);

    // Remove all pieces from the board
    void clear();

    Piece const * board_[SIZE][SIZE]; // The board
};

// Returns true if the other board is the same as this one
bool operator ==(Board const & x, Board const & y);

#endif // !defined(CHESS_BOARD)
