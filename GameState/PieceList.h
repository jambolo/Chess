#pragma once

#if !defined(PieceList_h__)
#define PieceList_h__

#include "GameState/ChessTypes.h"

class Piece;

class PieceList
{
public:
    explicit PieceList(Color color = Color::INVALID);
    virtual ~PieceList();

    Piece const * operator [] (PieceTypeId i) const
    {
        return pieces_[(size_t)i];
    }

    // Delete all the pieces
    void clear();

    // Recreate the pieces
    void initialize(Color color);

private:

    Piece * pieces_[NUMBER_OF_PIECE_TYPES];
};

#endif // !defined(PieceList_h__)
