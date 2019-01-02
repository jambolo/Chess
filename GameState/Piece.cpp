#include "Piece.h"

#include "Bishop.h"
#include "Board.h"
#include "King.h"
#include "Knight.h"
#include "Move.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"

namespace
{
struct PieceTraits
{
    char symbol[2][2];
};

PieceTraits const PIECE_TRAITS[NUMBER_OF_PIECE_TYPES] =
{
    {{ "K", "k" } },
    {{ "Q", "q" } },
    {{ "B", "b" } },
    {{ "N", "n" } },
    {{ "R", "r" } },
    {{ "P", "p" } },
};

#if 0

bool squareCanBeOccupied(Board const & board, Position const & p, Color myColor)
{
    Piece const * piece = board.pieceAt(p);
    return (piece == NO_PIECE) || (piece->color() != myColor);
}

bool isMoved(int dr, int dc)
{
    return dr != 0 || dc != 0;
}

bool isDiagonal(int dr, int dc)
{
    return dr == dc || dr == -dc;
}

bool inRange(int dr, int dc, int range)
{
    return dr > range || dr < -range || dc > range || dc < -range;
}

bool isSquare(int dr, int dc)
{
    return dr == 0 || dc == 0;
}
#endif // if 0
} // anonymous namespace

Piece const * const Piece::pieces_[1 + NUMBER_OF_COLORS * NUMBER_OF_PIECE_TYPES] =
{
    NO_PIECE,
    new King(Color::WHITE),
    new Queen(Color::WHITE),
    new Bishop(Color::WHITE),
    new Knight(Color::WHITE),
    new Rook(Color::WHITE),
    new Pawn(Color::WHITE),
    new King(Color::BLACK),
    new Queen(Color::BLACK),
    new Bishop(Color::BLACK),
    new Knight(Color::BLACK),
    new Rook(Color::BLACK),
    new Pawn(Color::BLACK)
};

Piece::Piece(PieceTypeId t, Color c)
    :  type_(t)
    , color_(c)
{
    // Assign image by type and color

//	image_ = new CBitmap;
//
//	UINT image_resid = ( c == WHITE ) ?
//							s_PieceTraits[ t ].whiteResourceId :
//							s_PieceTraits[ t ].blackResourceId;
//	int	result = image_->LoadBitmap( image_resid );
//	ASSERT( result != 0 );

    symbol_ = PIECE_TRAITS[(size_t)t].symbol[(size_t)c];
}

char const * Piece::symbol(PieceTypeId piece)
{
    return (piece != PieceTypeId::INVALID) ? PIECE_TRAITS[(size_t)piece].symbol[0] : "?";
}

void Piece::generateSpanMoves(Board const &     board,
                              Position const &  from,
                              int               dr,
                              int               dc,
                              Piece::MoveList & moves) const
{
    // Generate moves until the edge of the board is reached or there is a collision

    Position to(from.row + dr, from.column + dc);
    while (board.isValidPosition(to))
    {
        // If this square has a piece in it then the span is finished.
        Piece const * piece = board.pieceAt(to);
        if (piece)
        {
            // If it is an opponent's piece, it is also a valid move, so add the capture before quitting.
            if (piece->color() != color_)
                moves.emplace_back(this, from, to, true);
            return;
        }

        // Add this move
        moves.emplace_back(this, from, to);

        // Next square in span
        to.row    += dr;
        to.column += dc;
    }
}

int Piece::countSpanMoves(Board const &    board,
                          Position const & from,
                          int              dr,
                          int              dc) const
{
    // Generate moves until the edge of the board is reached or there is a collision

    int count = 0;

    Position to(from.row + dr, from.column + dc);
    while (board.isValidPosition(to))
    {
        // If this square has a piece in it then the span is finished.
        Piece const * piece = board.pieceAt(to);
        if (piece)
        {
            // If it is an opponent's piece, it is also a valid move, so add the capture before quitting.
            if (piece->color() != color_)
                ++count;
            break;
        }

        // Add this move
        ++count;

        // Next square in span
        to.row    += dr;
        to.column += dc;
    }

    return count;
}

int Piece::countSpanThreats(Board const & board, Position const & from, int dr, int dc) const
{
    // Generate moves until the edge of the board is reached or there is a collision

    Position to(from.row + dr, from.column + dc);
    while (board.isValidPosition(to))
    {
        // If this square has a piece in it then the span is finished.
        Piece const * piece = board.pieceAt(to);
        if (piece)
            return 1;

        // Next square in span
        to.row    += dr;
        to.column += dc;
    }

    return 0;
}
