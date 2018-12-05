#include "Piece.h"

#include "Bishop.h"
#include "Board.h"
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"

// #include "GameState/GameState.h"
#include "GameState/Move.h"
//
// #include "misc/Etc.h"

namespace
{
struct PieceTraits
{
//	UINT blackResourceId;
//	UINT whiteResourceId;
    char symbol[2][2];
};

PieceTraits const PIECE_TRAITS[NUMBER_OF_PIECE_TYPES] =
{
    { /*IDB_BKING,	IDB_WKING,		*/{ "K", "k" } },
    { /*IDB_BQUEEN,	IDB_WQUEEN,		*/{ "Q", "q" } },
    { /*IDB_BBISHOP,	IDB_WBISHOP,*/{ "B", "b" } },
    { /*IDB_BKNIGHT,	IDB_WKNIGHT,*/{ "N", "n" } },
    { /*IDB_BROOK,	IDB_WROOK,		*/{ "R", "r" } },
    { /*IDB_BPAWN,	IDB_WPAWN,		*/{ "P", "p" } },
};

#if 0

bool squareCanBeOccupied(Board const & board, Position const & p, Color myColor)
{
    Piece const * piece = board.pieceAt(p);
    return (piece == NO_PIECE) || (piece->color() != myColor);
}

void generateSpanMoves(Board const &     board,
                       Position const &  from,
                       int               dr,
                       int               dc,
                       Piece const *     myPiece,
                       Piece::MoveList & moves)
{
    // Generate moves until the edge of the board is reached or there is a collision

    Position to(from.row + dr, from.column + dc);
    while (board.isValidPosition(to))
    {
        // If this square has a piece in it then the span is finished.
        Piece const * piece = board.pieceAt(to);
        if (piece != NO_PIECE)
        {
            // If it is an opponent's piece, it is also a valid move, so add the capture before quitting.
            if (piece->color() != myPiece->color())
                moves.emplace_back(myPiece, from, to, true);
            return;
        }

        // Add this move
        moves.emplace_back(myPiece, from, to);

        // Next square in span
        to.row    += dr;
        to.column += dc;
    }
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

Piece const * Piece::pieces_[1 + NUMBER_OF_COLORS * NUMBER_OF_PIECE_TYPES] =
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
    :  type_(t), color_(c)
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

Piece::~Piece()
{
//	if ( image_ )
//	{
//		image_->DeleteObject();
//		delete image_;
//	}
}

char const * Piece::symbol(PieceTypeId piece)
{
    return (piece != PieceTypeId::INVALID) ? PIECE_TRAITS[(size_t)piece].symbol[0] : "?";
}

void Piece::generateSpanMoves(Board const &     board,
                              Position const &  from,
                              int               dr,
                              int               dc,
                              Piece const *     myPiece,
                              Piece::MoveList & moves) const
{
    // Generate moves until the edge of the board is reached or there is a collision

    Position to(from.row + dr, from.column + dc);
    while (board.isValidPosition(to))
    {
        // If this square has a piece in it then the span is finished.
        Piece const * piece = board.pieceAt(to);
        if (piece != NO_PIECE)
        {
            // If it is an opponent's piece, it is also a valid move, so add the capture before quitting.
            if (piece->color() != myPiece->color())
                moves.emplace_back(myPiece, from, to, true);
            return;
        }

        // Add this move
        moves.emplace_back(myPiece, from, to);

        // Next square in span
        to.row    += dr;
        to.column += dc;
    }
}
