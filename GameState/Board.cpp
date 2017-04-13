#include "Board.h"

#include "Move.h"
#include "Piece.h"

#include "misc/Etc.h"
#include "misc/exceptions.h"

#include <algorithm>
#include <regex>

Board::Board()
{
    clear();
}

void Board::initialize()
{
    clear();

    putPiece(Piece::piece(PieceTypeId::ROOK,   Color::BLACK), Position(0, 0));
    putPiece(Piece::piece(PieceTypeId::KNIGHT, Color::BLACK), Position(0, 1));
    putPiece(Piece::piece(PieceTypeId::BISHOP, Color::BLACK), Position(0, 2));
    putPiece(Piece::piece(PieceTypeId::QUEEN,  Color::BLACK), Position(0, 3));
    putPiece(Piece::piece(PieceTypeId::KING,   Color::BLACK), Position(0, 4));
    putPiece(Piece::piece(PieceTypeId::BISHOP, Color::BLACK), Position(0, 5));
    putPiece(Piece::piece(PieceTypeId::KNIGHT, Color::BLACK), Position(0, 6));
    putPiece(Piece::piece(PieceTypeId::ROOK,   Color::BLACK), Position(0, 7));
    putPiece(Piece::piece(PieceTypeId::PAWN,   Color::BLACK), Position(1, 0));
    putPiece(Piece::piece(PieceTypeId::PAWN,   Color::BLACK), Position(1, 1));
    putPiece(Piece::piece(PieceTypeId::PAWN,   Color::BLACK), Position(1, 2));
    putPiece(Piece::piece(PieceTypeId::PAWN,   Color::BLACK), Position(1, 3));
    putPiece(Piece::piece(PieceTypeId::PAWN,   Color::BLACK), Position(1, 4));
    putPiece(Piece::piece(PieceTypeId::PAWN,   Color::BLACK), Position(1, 5));
    putPiece(Piece::piece(PieceTypeId::PAWN,   Color::BLACK), Position(1, 6));
    putPiece(Piece::piece(PieceTypeId::PAWN,   Color::BLACK), Position(1, 7));

    putPiece(Piece::piece(PieceTypeId::ROOK,   Color::WHITE), Position(7, 0));
    putPiece(Piece::piece(PieceTypeId::KNIGHT, Color::WHITE), Position(7, 1));
    putPiece(Piece::piece(PieceTypeId::BISHOP, Color::WHITE), Position(7, 2));
    putPiece(Piece::piece(PieceTypeId::QUEEN,  Color::WHITE), Position(7, 3));
    putPiece(Piece::piece(PieceTypeId::KING,   Color::WHITE), Position(7, 4));
    putPiece(Piece::piece(PieceTypeId::BISHOP, Color::WHITE), Position(7, 5));
    putPiece(Piece::piece(PieceTypeId::KNIGHT, Color::WHITE), Position(7, 6));
    putPiece(Piece::piece(PieceTypeId::ROOK,   Color::WHITE), Position(7, 7));
    putPiece(Piece::piece(PieceTypeId::PAWN,   Color::WHITE), Position(6, 0));
    putPiece(Piece::piece(PieceTypeId::PAWN,   Color::WHITE), Position(6, 1));
    putPiece(Piece::piece(PieceTypeId::PAWN,   Color::WHITE), Position(6, 2));
    putPiece(Piece::piece(PieceTypeId::PAWN,   Color::WHITE), Position(6, 3));
    putPiece(Piece::piece(PieceTypeId::PAWN,   Color::WHITE), Position(6, 4));
    putPiece(Piece::piece(PieceTypeId::PAWN,   Color::WHITE), Position(6, 5));
    putPiece(Piece::piece(PieceTypeId::PAWN,   Color::WHITE), Position(6, 6));
    putPiece(Piece::piece(PieceTypeId::PAWN,   Color::WHITE), Position(6, 7));
}

bool Board::initializeFromFen(char const * start, char const * end)
{
    std::string fen(start, end);
    std::smatch match;
    try
    {
        std::regex re("([pnbrqkPNBRQK1-8]+)/([pnbrqkPNBRQK1-8]+)/([pnbrqkPNBRQK1-8]+)/([pnbrqkPNBRQK1-8]+)/([pnbrqkPNBRQK1-8]+)/([pnbrqkPNBRQK1-8]+)/([pnbrqkPNBRQK1-8]+)/([pnbrqkPNBRQK1-8]+)");
        if (!std::regex_search(fen, match, re) || match.size() != SIZE + 1)
            return false;
    }
    catch (std::regex_error &)
    {
        return false;
    }

    int column = 0;
    for (int row = 0; row < SIZE; ++row)
    {
        for (auto c : match.str(row + 1))
        {
            if (c >= '1' && c <= '8')
            {
                // Skip columns
                column += (c - '0');
            }
            else
            {
                Piece const * piece = NO_PIECE;
                switch (c)
                {
                    case 'B': piece = Piece::piece(PieceTypeId::BISHOP, Color::WHITE);   break;
                    case 'K': piece = Piece::piece(PieceTypeId::KING, Color::WHITE);     break;
                    case 'N': piece = Piece::piece(PieceTypeId::KNIGHT, Color::WHITE);   break;
                    case 'P': piece = Piece::piece(PieceTypeId::PAWN, Color::WHITE);     break;
                    case 'Q': piece = Piece::piece(PieceTypeId::QUEEN, Color::WHITE);    break;
                    case 'R': piece = Piece::piece(PieceTypeId::ROOK, Color::WHITE);     break;
                    case 'b': piece = Piece::piece(PieceTypeId::BISHOP, Color::BLACK);   break;
                    case 'k': piece = Piece::piece(PieceTypeId::KING, Color::BLACK);     break;
                    case 'n': piece = Piece::piece(PieceTypeId::KNIGHT, Color::BLACK);   break;
                    case 'p': piece = Piece::piece(PieceTypeId::PAWN, Color::BLACK);     break;
                    case 'q': piece = Piece::piece(PieceTypeId::QUEEN, Color::BLACK);    break;
                    case 'r': piece = Piece::piece(PieceTypeId::ROOK, Color::BLACK);     break;
                    default: throw ConstructorFailedException();
                }
                putPiece(piece, Position(row, column));
                ++column;
            }
            if (column >= SIZE)
                return false;
        }
    }
    return true;
}

bool Board::spanIsEmpty(Position const & from, Position const & to) const
{
    // Get the distance in rows and columns of the move (sign determines direction)
    int row_move    = to.row - from.row;
    int column_move = to.column - from.column;

    // Determine the row and column increment for each square tested. If the distance moved in a direction is < 0,
    // the increment is -1, if the distance is > 0, then the increment is 1, otherwise the increment is 0.
    int row_delta    = (row_move < 0) ? -1 : (row_move == 0) ? 0 : 1;
    int column_delta = (column_move < 0) ? -1 : (column_move == 0) ? 0 : 1;

    // Determine the number of squares moved (excluding the last square)
    int distance = std::max(abs(row_move), abs(column_move)) - 1;

    // Check every square except the first one. If there is a piece occupying a square, then return false.
    Position p = from;
    for (int i = 0; i < distance; ++i)
    {
        p.row    += row_delta;
        p.column += column_delta;

        if (pieceAt(p) != NO_PIECE)
            return false;
    }

    return true;
}
