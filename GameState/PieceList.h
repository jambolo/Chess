/********************************************************************************************************************

                                                     PieceList.h

                                            Copyright 2004, John J. Bolton
    --------------------------------------------------------------------------------------------------------------

    $Header: //depot/Chess/PieceList.h#6 $

    $NoKeywords: $

********************************************************************************************************************/

#pragma once

#include "GameState/ChessTypes.h"

class Piece;

class PieceList
{
public:
    explicit PieceList(Color color = Color::INVALID);
    virtual ~PieceList();

    Piece const * operator [](PieceTypeId i) const
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
