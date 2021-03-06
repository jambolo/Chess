#include "Board.h"

#include "Move.h"
#include "Piece.h"
#include "Position.h"

#include <Misc/Exceptions.h>

#include <algorithm>
#include <cassert>
#include <regex>

Position const Board::INITIAL_BLACK_ROOK_QUEENSIDE_POSITION{ 0, 0 };
Position const Board::INITIAL_BLACK_KNIGHT_QUEENSIDE_POSITION{ 0, 1 };
Position const Board::INITIAL_BLACK_BISHOP_QUEENSIDE_POSITION{ 0, 2 };
Position const Board::INITIAL_BLACK_QUEEN_POSITION{ 0, 3 };
Position const Board::INITIAL_BLACK_KING_POSITION{ 0, 4 };
Position const Board::INITIAL_BLACK_BISHOP_KINGSIDE_POSITION{ 0, 5 };
Position const Board::INITIAL_BLACK_KNIGHT_KINGSIDE_POSITION{ 0, 6 };
Position const Board::INITIAL_BLACK_ROOK_KINGSIDE_POSITION{ 0, 7 };
Position const Board::INITIAL_WHITE_ROOK_QUEENSIDE_POSITION{ 7, 0 };
Position const Board::INITIAL_WHITE_KNIGHT_QUEENSIDE_POSITION{ 7, 1 };
Position const Board::INITIAL_WHITE_BISHOP_QUEENSIDE_POSITION{ 7, 2 };
Position const Board::INITIAL_WHITE_QUEEN_POSITION{ 7, 3 };
Position const Board::INITIAL_WHITE_KING_POSITION{ 7, 4 };
Position const Board::INITIAL_WHITE_BISHOP_KINGSIDE_POSITION{ 7, 5 };
Position const Board::INITIAL_WHITE_KNIGHT_KINGSIDE_POSITION{ 7, 6 };
Position const Board::INITIAL_WHITE_ROOK_KINGSIDE_POSITION{ 7, 7 };

Board::Board()
{
    clear();
}

void Board::initialize()
{
    clear();

    putPiece(Piece::get(PieceTypeId::ROOK,   Color::BLACK), { 0, 0 });
    putPiece(Piece::get(PieceTypeId::KNIGHT, Color::BLACK), { 0, 1 });
    putPiece(Piece::get(PieceTypeId::BISHOP, Color::BLACK), { 0, 2 });
    putPiece(Piece::get(PieceTypeId::QUEEN,  Color::BLACK), { 0, 3 });
    putPiece(Piece::get(PieceTypeId::KING,   Color::BLACK), { 0, 4 });
    putPiece(Piece::get(PieceTypeId::BISHOP, Color::BLACK), { 0, 5 });
    putPiece(Piece::get(PieceTypeId::KNIGHT, Color::BLACK), { 0, 6 });
    putPiece(Piece::get(PieceTypeId::ROOK,   Color::BLACK), { 0, 7 });
    putPiece(Piece::get(PieceTypeId::PAWN,   Color::BLACK), { 1, 0 });
    putPiece(Piece::get(PieceTypeId::PAWN,   Color::BLACK), { 1, 1 });
    putPiece(Piece::get(PieceTypeId::PAWN,   Color::BLACK), { 1, 2 });
    putPiece(Piece::get(PieceTypeId::PAWN,   Color::BLACK), { 1, 3 });
    putPiece(Piece::get(PieceTypeId::PAWN,   Color::BLACK), { 1, 4 });
    putPiece(Piece::get(PieceTypeId::PAWN,   Color::BLACK), { 1, 5 });
    putPiece(Piece::get(PieceTypeId::PAWN,   Color::BLACK), { 1, 6 });
    putPiece(Piece::get(PieceTypeId::PAWN,   Color::BLACK), { 1, 7 });

    putPiece(Piece::get(PieceTypeId::ROOK,   Color::WHITE), { 7, 0 });
    putPiece(Piece::get(PieceTypeId::KNIGHT, Color::WHITE), { 7, 1 });
    putPiece(Piece::get(PieceTypeId::BISHOP, Color::WHITE), { 7, 2 });
    putPiece(Piece::get(PieceTypeId::QUEEN,  Color::WHITE), { 7, 3 });
    putPiece(Piece::get(PieceTypeId::KING,   Color::WHITE), { 7, 4 });
    putPiece(Piece::get(PieceTypeId::BISHOP, Color::WHITE), { 7, 5 });
    putPiece(Piece::get(PieceTypeId::KNIGHT, Color::WHITE), { 7, 6 });
    putPiece(Piece::get(PieceTypeId::ROOK,   Color::WHITE), { 7, 7 });
    putPiece(Piece::get(PieceTypeId::PAWN,   Color::WHITE), { 6, 0 });
    putPiece(Piece::get(PieceTypeId::PAWN,   Color::WHITE), { 6, 1 });
    putPiece(Piece::get(PieceTypeId::PAWN,   Color::WHITE), { 6, 2 });
    putPiece(Piece::get(PieceTypeId::PAWN,   Color::WHITE), { 6, 3 });
    putPiece(Piece::get(PieceTypeId::PAWN,   Color::WHITE), { 6, 4 });
    putPiece(Piece::get(PieceTypeId::PAWN,   Color::WHITE), { 6, 5 });
    putPiece(Piece::get(PieceTypeId::PAWN,   Color::WHITE), { 6, 6 });
    putPiece(Piece::get(PieceTypeId::PAWN,   Color::WHITE), { 6, 7 });
}

bool Board::initializeFromFen(char const * start, char const * end)
{
    clear();

    std::string fen(start, end);
    std::smatch match;
    try
    {
        std::regex re(
            "([pnbrqkPNBRQK1-8]+)/([pnbrqkPNBRQK1-8]+)/([pnbrqkPNBRQK1-8]+)/([pnbrqkPNBRQK1-8]+)/([pnbrqkPNBRQK1-8]+)/([pnbrqkPNBRQK1-8]+)/([pnbrqkPNBRQK1-8]+)/([pnbrqkPNBRQK1-8]+)");
        if (!std::regex_search(fen, match, re) || match.size() != SIZE + 1)
            return false;
    }
    catch (std::regex_error &)
    {
        return false;
    }

    for (int row = 0; row < SIZE; ++row)
    {
        int column = 0;
        for (char c : match.str(row + 1))
        {
            if (column > SIZE)
                return false;
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
                    case 'B': piece = Piece::get(PieceTypeId::BISHOP, Color::WHITE); break;
                    case 'K': piece = Piece::get(PieceTypeId::KING, Color::WHITE);   break;
                    case 'N': piece = Piece::get(PieceTypeId::KNIGHT, Color::WHITE); break;
                    case 'P': piece = Piece::get(PieceTypeId::PAWN, Color::WHITE);   break;
                    case 'Q': piece = Piece::get(PieceTypeId::QUEEN, Color::WHITE);  break;
                    case 'R': piece = Piece::get(PieceTypeId::ROOK, Color::WHITE);   break;
                    case 'b': piece = Piece::get(PieceTypeId::BISHOP, Color::BLACK); break;
                    case 'k': piece = Piece::get(PieceTypeId::KING, Color::BLACK);   break;
                    case 'n': piece = Piece::get(PieceTypeId::KNIGHT, Color::BLACK); break;
                    case 'p': piece = Piece::get(PieceTypeId::PAWN, Color::BLACK);   break;
                    case 'q': piece = Piece::get(PieceTypeId::QUEEN, Color::BLACK);  break;
                    case 'r': piece = Piece::get(PieceTypeId::ROOK, Color::BLACK);   break;
                    default: throw ConstructorFailedException();
                }
                putPiece(piece, Position(row, column));
                ++column;
            }
            if (column > SIZE)
                return false;
        }
    }
    return true;
}

bool Board::isValidPosition(Position const & p)
{
    return p.row >= 0 &&
           p.row < SIZE &&
           p.column >= 0 &&
           p.column < SIZE;
}

Piece const * Board::pieceAt(Position const & p) const
{
    return pieceAt(p.row, p.column);
}

Piece const * Board::pieceAt(int r, int c) const
{
    return board_[r][c];
}

void Board::putPiece(Piece const * piece, Position const & p)
{
    putPiece(piece, p.row, p.column);
}

void Board::putPiece(Piece const * piece, int r, int c)
{
    board_[r][c] = piece;
}

void Board::removePiece(Position const & p)
{
    removePiece(p.row, p.column);
}

void Board::removePiece(int r, int c)
{
    board_[r][c] = NO_PIECE;
}

void Board::movePiece(Position const & from, Position const & to)
{
    board_[to.row][to.column]     = board_[from.row][from.column];
    board_[from.row][from.column] = NO_PIECE;
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

    // Check every square except the first and last ones. If there is a piece occupying a square, then return false.
    Position p = from;
    for (int i = 0; i < distance; ++i)
    {
        p.row    += row_delta;
        p.column += column_delta;

        if (pieceAt(p))
            return false;
    }

    return true;
}

std::string Board::fen() const
{
    std::string result;
    for (int row = 0; row < SIZE; ++row)
    {
        if (row != 0)
            result += '/';
        int span = 0;
        for (int column = 0; column < SIZE; ++column)
        {
            Piece const * piece = board_[row][column];
            if (piece == NO_PIECE)
            {
                ++span;
            }
            else
            {
                if (span > 0)
                {
                    result += std::to_string(span);
                    span    = 0;
                }
                result += piece->symbol();
            }
        }
        if (span > 0)
            result += std::to_string(span);
    }
    return result;
}

void Board::clear()
{
    assert(NO_PIECE == 0);
    memset(board_, 0, sizeof board_);
}

bool operator ==(Board const & x, Board const & y)
{
    return memcmp(x.board_, y.board_, sizeof x.board_) == 0;
}
