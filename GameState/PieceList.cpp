#include "Piece.h"
#include "PieceList.h"
#include <cstdlib>

PieceList::PieceList(Color color /*= Color::INVALID*/)
{
    if (color != Color::INVALID)
    {
        initialize(color);
    }
    else
    {
        memset(pieces_, 0, sizeof pieces_);
    }
}

PieceList::~PieceList()
{
    clear();
}

void PieceList::initialize(Color color)
{
    pieces_[(size_t)PieceTypeId::KING]   = new King(color);
    pieces_[(size_t)PieceTypeId::QUEEN]  = new Queen(color);
    pieces_[(size_t)PieceTypeId::BISHOP] = new Bishop(color);
    pieces_[(size_t)PieceTypeId::KNIGHT] = new Knight(color);
    pieces_[(size_t)PieceTypeId::ROOK]   = new Rook(color);
    pieces_[(size_t)PieceTypeId::PAWN]   = new Pawn(color);
}

void PieceList::clear()
{
    for (auto p : pieces_)
    {
        delete p;
    }
    memset(pieces_, 0, sizeof pieces_);
}
