#include "Piece.h"

#include "gamestate/GameState.h"
#include "gamestate/Move.h"

#include "misc/Etc.h"

namespace
{
struct PieceTraits
{
//	UINT blackResourceId;
//	UINT whiteResourceId;
    char symbol[2];
};

PieceTraits const PIECE_TRAITS[NUMBER_OF_PIECE_TYPES] =
{
    { /*IDB_BKING,	IDB_WKING,		*/"K"   },
    { /*IDB_BQUEEN,	IDB_WQUEEN,		*/"Q" },
    { /*IDB_BBISHOP,	IDB_WBISHOP,*/"B" },
    { /*IDB_BKNIGHT,	IDB_WKNIGHT,*/"N" },
    { /*IDB_BROOK,	IDB_WROOK,		*/"R"   },
    { /*IDB_BPAWN,	IDB_WPAWN,		*/"P"   },
};


bool squareCanBeOccupied(Board const & board, Position const & p, Color myColor)
{
    Piece const * piece = board.pieceAt(p);
    return (piece == NO_PIECE) || (piece->color() != myColor);
}

void generateSpanMoves(Board const &     board,
                       Position const &  from,
                       int               dr,
                       int               dc,
                       Color             myColor,
                       Piece::MoveList & moves)
{
    // Generate moves until the edge of the board is reached or there is a collision

    Position to(from.m_Row + dr, from.m_Column + dc);
    while (board.isValidPosition(to))
    {
        // If this square has a piece in it then the span is finished.
        Piece const * piece = board.pieceAt(to);
        if (piece != NO_PIECE)
        {
            Piece const * captured = piece;

            // If it is an opponent's piece, it is also a valid move, so add it before quitting.
            if (captured->color() != myColor)
            {
                moves.emplace_back(from, to);
            }
            return;
        }

        // Add this move

        moves.emplace_back(from, to);

        // Next square in span

        to.m_Row    += dr;
        to.m_Column += dc;
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

    symbol_ = PIECE_TRAITS[(size_t)t].symbol;
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
    return (piece != PieceTypeId::INVALID) ? PIECE_TRAITS[(size_t)piece].symbol : "?";
}

void King::generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const
{
    Board const & board = state.board_;

    moves.reserve(moves.size() + MAX_POSSIBLE_MOVES);

    Position to = from;

    // Up

    --to.m_Row;
    if ((to.m_Row >= 0) && squareCanBeOccupied(board, to, color_))
    {
        moves.emplace_back(from, to);
    }

    // Up-right

    ++to.m_Column;
    if ((to.m_Row >= 0) && (to.m_Column < Board::SIZE) && squareCanBeOccupied(board, to, color_))
    {
        moves.emplace_back(from, to);
    }

    // Right

    ++to.m_Row;
    if ((to.m_Column < Board::SIZE) && squareCanBeOccupied(board, to, color_))
    {
        moves.emplace_back(from, to);
    }

    // Down-right

    ++to.m_Row;
    if ((to.m_Row < Board::SIZE) && (to.m_Column < Board::SIZE) && squareCanBeOccupied(board, to, color_))
    {
        moves.emplace_back(from, to);
    }

    // Down

    --to.m_Column;
    if ((to.m_Row < Board::SIZE) && squareCanBeOccupied(board, to, color_))
    {
        moves.emplace_back(from, to);
    }

    // Down-left

    --to.m_Column;
    if ((to.m_Row < Board::SIZE) && (to.m_Column >= 0) && squareCanBeOccupied(board, to, color_))
    {
        moves.emplace_back(from, to);
    }

    // Left

    --to.m_Row;
    if ((to.m_Column >= 0) && squareCanBeOccupied(board, to, color_))
    {
        moves.emplace_back(from, to);
    }

    // Up-left

    --to.m_Row;
    if ((to.m_Row >= 0) && (to.m_Column >= 0) && squareCanBeOccupied(board, to, color_))
    {
        moves.emplace_back(from, to);
    }

    // Castles

    if (state.kingSideCastleIsAllowed(color_) && !state.inCheck_)
    {
        //! @todo Must check all squares that the kings moves through
        moves.push_back(Move::kingSideCastle());
    }

    if (state.queenSideCastleIsAllowed(color_) && !state.inCheck_)
    {
        //! @todo Must check all squares that the kings moves through
        moves.push_back(Move::queenSideCastle());
    }
}

bool King::isValidMove(GameState const & state, Move const & move) const
{
    Board const & board   = state.board_;
    Position const & from = move.from();
    Position const & to = move.to();

    // Check castles

    if (move.isKingSideCastle())
    {
        //! @todo Must check all squares that the kings moves through
        return state.kingSideCastleIsAllowed(color_) && !state.inCheck_;
    }

    if (move.isQueenSideCastle())
    {
        //! @todo Must check all squares that the kings moves through
        return state.queenSideCastleIsAllowed(color_) && !state.inCheck_;
    }

    // Check if the destination square can be occupied

    if (!squareCanBeOccupied(board, to, color_))
        return false;

    // Check movement (exactly one square in any direction)

    int dr = to.m_Row - from.m_Row;
    int dc = to.m_Column - from.m_Column;

    if (!isMoved(dr, dc) || !inRange(dr, dc, 1))
        return false;

    return true;
}

void Queen::generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const
{
    Board const & board = state.board_;

    moves.reserve(moves.size() + MAX_POSSIBLE_MOVES);

    generateSpanMoves(board, from, -1, 0, color_, moves);  // up
    generateSpanMoves(board, from, -1, 1, color_, moves);  // up-right
    generateSpanMoves(board, from, 0, 1, color_, moves);   // right
    generateSpanMoves(board, from, 1, 1, color_, moves);   // down-right
    generateSpanMoves(board, from, 1, 0, color_, moves);   // down
    generateSpanMoves(board, from, 1, -1, color_, moves);  // down-left
    generateSpanMoves(board, from, 0, -1, color_, moves);  // left
    generateSpanMoves(board, from, -1, -1, color_, moves); // up-left
}

bool Queen::isValidMove(GameState const & state, Move const & move) const
{
    Board const & board   = state.board_;
    Position const & from = move.from();
    Position const & to = move.to();

    // Check if the destination square can be occupied

    if (!squareCanBeOccupied(board, to, color_))
        return false;

    // Check movement (at least 1 square in 8 directions)

    int dr = to.m_Row - from.m_Row;
    int dc = to.m_Column - from.m_Column;

    if (!isMoved(dr, dc) || (!isDiagonal(dr, dc) && !isSquare(dr, dc)))
        return false;

    // Check for moving through a piece

    if (!board.spanIsEmpty(from, to))
        return false;

    return true;
}

void Bishop::generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const
{
    Board const & board = state.board_;

    moves.reserve(moves.size() + MAX_POSSIBLE_MOVES);

    generateSpanMoves(board, from, -1, 1, color_, moves);  // up-right
    generateSpanMoves(board, from, 1, 1, color_, moves);   // down-right
    generateSpanMoves(board, from, 1, -1, color_, moves);  // down-left
    generateSpanMoves(board, from, -1, -1, color_, moves); // up-left
}

bool Bishop::isValidMove(GameState const & state, Move const & move) const
{
    Board const & board   = state.board_;
    Position const & from = move.from();
    Position const & to = move.to();

    // Check if the destination square can be occupied

    if (!squareCanBeOccupied(board, to, color_))
        return false;

    // Check movement (at least 1 square diagonally)

    int dr = to.m_Row - from.m_Row;
    int dc = to.m_Column - from.m_Column;

    if (!isMoved(dr, dc) || !isDiagonal(dr, dc))
        return false;

    // Check for moving through a piece

    if (!board.spanIsEmpty(from, to))
        return false;

    return true;
}

void Knight::generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const
{
    Board const & board = state.board_;

    static Position const offsets[] =
    {
        Position(-2, -1), // up-left
        Position(-2, 1),  // up-right
        Position(-1, 2),  // right-up
        Position(1, 2),   // right-down
        Position(2, -1),  // down-left
        Position(2, 1),   // down-right
        Position(-1, -2), // left-up
        Position(1, -2)   // left-down
    };

    moves.reserve(moves.size() + MAX_POSSIBLE_MOVES);

    for (int i = 0; i < elementsof(offsets); ++i)
    {
        Position to(from.m_Row + offsets[i].m_Row, from.m_Column + offsets[i].m_Column);

        if (board.isValidPosition(to))
        {
            // Check if the destination square can be occupied

            if (squareCanBeOccupied(board, to, color_))
            {
                moves.emplace_back(from, to);
            }
        }
    }
}

bool Knight::isValidMove(GameState const & state, Move const & move) const
{
    Board const & board   = state.board_;
    Position const & from = move.from();
    Position const & to = move.to();

    // Check if the destination square can be occupied
    if (!squareCanBeOccupied(board, to, color_))
        return false;

    // Check movement

    int adr = abs(to.m_Row - from.m_Row);
    int adc = abs(to.m_Column - from.m_Column);

    if (!isMoved(adr, adc))
        return false;

    if (((adr != 2) || (adc != 1)) && ((adc != 2) || (adr != 1)))
        return false;

    return true;
}

void Rook::generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const
{
    Board const & board = state.board_;

    moves.reserve(moves.size() + MAX_POSSIBLE_MOVES);

    generateSpanMoves(board, from, -1, 0, color_, moves); // up
    generateSpanMoves(board, from, 0, 1, color_, moves);  // right
    generateSpanMoves(board, from, 1, 0, color_, moves);  // down
    generateSpanMoves(board, from, 0, -1, color_, moves); // left
}

bool Rook::isValidMove(GameState const & state, Move const & move) const
{
    Board const & board   = state.board_;
    Position const & from = move.from();
    Position const & to = move.to();

    // Check if the destination square can be occupied

    if (!squareCanBeOccupied(board, to, color_))
        return false;

    // Check movement (at least 1 square in 4 directions)

    int dr = to.m_Row - from.m_Row;
    int dc = to.m_Column - from.m_Column;

    if (!isMoved(dc, dc) || !isSquare(dr, dc))
        return false;

    // Check for moving through a piece (note: this doesn't check for castling)

    if (!board.spanIsEmpty(from, to))
        return false;

    return true;
}

void Pawn::generatePossibleMoves(GameState const & state, Position const & from, MoveList & moves) const
{
    Board const & board = state.board_;

    int direction       = (color_ == Color::BLACK) ? 1 : -1;

    moves.reserve(moves.size() + MAX_POSSIBLE_MOVES);

    Position to;

    // Diagonal to left (must capture)

    to.m_Row    = from.m_Row + direction;
    to.m_Column = from.m_Column - 1;

    if (board.isValidPosition(to))
    {
        Piece const * captured = board.pieceAt(to);

        if ((captured != NO_PIECE) && (captured->color() != color_))
        {
            moves.emplace_back(from, to);
        }
        else
        {
            // Note: If a piece can be captured normally, then it is impossible for a pawn to have moved two
            // spaces (and be open to capture by en passant).
            // Check en passant -- if the piece next to the destination square is a pawn and the previous move was
            // a two-space move to that square (assumed to be made by the pawn), then en passant is possible.

            captured = board.pieceAt(from.m_Row, to.m_Column);

            if ((captured != NO_PIECE) &&
                (captured->type() == PieceTypeId::PAWN) &&
                (state.move_.from() == Position(to.m_Row + direction, to.m_Column)) &&
                (state.move_.to() == Position(from.m_Row, to.m_Column)))
            {
                moves.push_back(Move::enPassant(from, to));
            }
        }
    }

    // Diagonal to right (must capture)

    to.m_Row    = from.m_Row + direction;
    to.m_Column = from.m_Column + 1;

    if (board.isValidPosition(to))
    {
        Piece const * captured = board.pieceAt(to);

        if ((captured != NO_PIECE) && (captured->color() != color_))
        {
            moves.emplace_back(from, to);
        }
        else
        {
            // Note: If a piece can be captured normally, then it is impossible for a pawn to have moved two
            // spaces (and be open to capture by en passant).
            // Check en passant -- if the piece next to the destination square is a pawn and the previous move was
            // a two-space move to that square (assumed to be made by the pawn), then en passant is possible.

            captured = board.pieceAt(from.m_Row, to.m_Column);

            if ((captured != NO_PIECE) &&
                (captured->type() == PieceTypeId::PAWN) &&
                (state.move_.from() == Position(to.m_Row + direction, to.m_Column)) &&
                (state.move_.to() == Position(from.m_Row, to.m_Column)))
            {
                moves.push_back(Move::enPassant(from, to));
            }
        }
    }

    // Ahead 1 row (must be empty) and ahead 2 rows (must be empty)

    to.m_Row    = from.m_Row + direction;
    to.m_Column = from.m_Column;

    if (board.isValidPosition(to) && (board.pieceAt(to) == NO_PIECE))
    {
        moves.emplace_back(from, to);

        // Ahead 2 rows if in its original spot (must be empty)

        if ((from.m_Row == STARTING_ROW_BLACK) && (color_ == Color::BLACK) ||
            (from.m_Row == STARTING_ROW_WHITE) && (color_ == Color::WHITE))
        {
            to.m_Row += direction;
            if (board.pieceAt(to) == NO_PIECE)
            {
                moves.emplace_back(from, to);
            }
        }
    }
}

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

bool Pawn::isValidMove(GameState const & state, Move const & move) const
{
    Board const & board   = state.board_;
    Position const & from = move.from();
    Position const & to = move.to();

    int direction       = (color_ == Color::BLACK) ? 1 : -1;

    // Ahead 1?

    if ((to.m_Row == from.m_Row + direction) &&
        (to.m_Column == from.m_Column) &&
        (board.pieceAt(to) == NO_PIECE))
    {
        return true;
    }

    // Capture diagonally?

    if ((to.m_Row == from.m_Row + direction) &&
        ((to.m_Column == from.m_Column + 1) || (to.m_Column == from.m_Column - 1)))
    {
        Piece const * captured = board.pieceAt(to);

        if ((captured != NO_PIECE) && (captured->color() != color_))
        {
            return true;
        }

        // Check en passant

        captured = board.pieceAt(from.m_Row, to.m_Column);

        if ((captured != NO_PIECE) &&
            (captured->type() == PieceTypeId::PAWN) &&
            (state.move_.from() == Position(to.m_Row + direction, to.m_Column)) &&
            (state.move_.to() == Position(from.m_Row, to.m_Column)))
        {
            return true;
        }
    }

    // Ahead 2?

    if ((to.m_Row == from.m_Row + 2 * direction) &&
        (to.m_Column == from.m_Column) &&
        (board.pieceAt(Position(from.m_Row + direction, from.m_Column)) == NO_PIECE) &&
        (board.pieceAt(to) == NO_PIECE))
    {
        return true;
    }

    return false;
}
