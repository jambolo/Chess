/** @file *//********************************************************************************************************

                                                 StaticEvaluator.cpp

                                            Copyright 2004, John J. Bolton
    --------------------------------------------------------------------------------------------------------------

    $Header: //depot/Chess/StaticEvaluationFunction.cpp#12 $

    $NoKeywords: $

********************************************************************************************************************/

#include "StaticEvaluator.h"

#include "GameState/GameState.h"
#include "GameState/Move.h"
#include "GameState/Piece.h"

#include <cassert>

namespace
{

struct PieceValues
{
    int property;           // Values are the standard values times 1000
    int mobility;           // Values are the average number moves on an empty board times 1000
    int position;           // Values are the standard values times 1000
};

// Values of each square for each color

int const s_PositionValues[NUMBER_OF_COLORS][Board::SIZE][Board::SIZE] =
{
    // White
    {
        0, 1, 2, 3, 3, 2, 1, 0,
        1, 2, 3, 4, 4, 3, 2, 1,
        2, 3, 4, 5, 5, 4, 3, 2,
        3, 4, 5, 6, 6, 5, 4, 3,
        3, 4, 5, 6, 6, 5, 4, 3,
        2, 3, 4, 5, 5, 4, 3, 2,
        1, 2, 3, 4, 4, 3, 2, 1,
        0, 1, 2, 3, 3, 2, 1, 0,
    },

    // Black
    {
        -0, -1, -2, -3, -3, -2, -1, -0,
        -1, -2, -3, -4, -4, -3, -2, -1,
        -2, -3, -4, -5, -5, -4, -3, -2,
        -3, -4, -5, -6, -6, -5, -4, -3,
        -3, -4, -5, -6, -6, -5, -4, -3,
        -2, -3, -4, -5, -5, -4, -3, -2,
        -1, -2, -3, -4, -4, -3, -2, -1,
        -0, -1, -2, -3, -3, -2, -1, -0,
    },
};

PieceValues const s_PieceValues[NUMBER_OF_COLORS][NUMBER_OF_PIECE_TYPES] =
{
    // White
    {
        { 4000,  6560,   4000,    }, //	PieceTypeId::KING
        { 9000,  22750,  9000,    }, //	PieceTypeId::QUEEN
        { 3000,  8750,   3000,    }, //	PieceTypeId::BISHOP
        { 3000,  5250,   3000,    }, //	PieceTypeId::KNIGHT
        { 5000,  14000,  5000,    }, //	PieceTypeId::ROOK
        { 1000,  1667,   1000,    } //	PieceTypeId::PAWN
    },

    // Black
    {
        { -4000, -6560,  -4000,   }, //	PieceTypeId::KING
        { -9000, -22750, -9000,   }, //	PieceTypeId::QUEEN
        { -3000, -8750,  -3000,   }, //	PieceTypeId::BISHOP
        { -3000, -5250,  -3000,   }, //	PieceTypeId::KNIGHT
        { -5000, -14000, -5000,   }, //	PieceTypeId::ROOK
        { -1000, -1667,  -1000,   } //	PieceTypeId::PAWN
    }
};

int const PROPERTY_FACTOR = 1;      // The property factor is the reference. All other factors are relative to it.
int const CASTLE_FACTOR   = s_PieceValues[0][(size_t)PieceTypeId::PAWN].property * 3 / 4 / 3;       // Max is 3, which is worth
                                                                                                    // about 3/4 a pawn
int const MOBILITY_FACTOR = s_PieceValues[0][(size_t)PieceTypeId::QUEEN].property / 144;        // Maximum is about 144 which is
                                                                                                // worth a queen
int const POSITION_FACTOR = s_PieceValues[0][(size_t)PieceTypeId::PAWN].property / 100;     // Reasonable maximum is 100,000 which
                                                                                            // is worth a pawn
int const THREAT_FACTOR   = 0;

int _Evaluate(GameState::CastleStatus castleStatus)
{
    // A bonus is given for having castled. Likewise, a penalty is given for losing the ability to castle.

    int value = 0;
    int const BLACK_CASTLE_MASK = (int)CastleId::BLACK_QUEENSIDE | (int)CastleId::BLACK_KINGSIDE;
    if (castleStatus.castled & BLACK_CASTLE_MASK)
    {
        value += -2;
    }
    else if ((castleStatus.unavailable & BLACK_CASTLE_MASK) == BLACK_CASTLE_MASK)
    {
        value -= -1;
    }
    int const WHITE_CASTLE_MASK = (int)CastleId::WHITE_QUEENSIDE | (int)CastleId::WHITE_KINGSIDE;
    if (castleStatus.castled & WHITE_CASTLE_MASK)
    {
        value += 2;
    }
    else if ((castleStatus.unavailable & WHITE_CASTLE_MASK) == WHITE_CASTLE_MASK)
    {
        value -= 1;
    }

    return value;
}

} // anonymous namespace

// This function returns the value of the board *** BY COLOR ***.

int StaticEvaluator::evaluate(GameState const & state)
{
    // If both kings are present, checkmate_value will be 0, otherwise it will be -CHECKMATE_VALUE() if the WHITE
    // king is missing, or CHECKMATE_VALUE() if the BLACK king is missing.
    int checkmate_value =  0;

    // The value of property is computed by adding up the values of all the pieces on the board.
    int property_value = 0;

//	// The value of mobility is calculated by finding the difference between the total number of moves available
//	// to each side.
//	int		mobility_value	= 0;

    // The value of position is is found by adding up the values of the positions of each piece (by color)
    int position_value = 0;

//	// The value of threats is not currently computed.
//	int		threat_value	= 0;

    for (int row = 0; row < Board::SIZE; ++row)
    {
        for (int column = 0; column < Board::SIZE; ++column)
        {
            Piece * const id = state.board_.pieceAt(row, column);

            if (id != NO_PIECE)
            {
                Piece const * pPiece = Board::piece(id);
                Color color          = pPiece->color();
                PieceTypeId type     = pPiece->type();

                // Compute the checkmate value

                if (id == (int)PieceTypeId::KING)
                {
                    checkmate_value += (color == Color::WHITE) ? CHECKMATE_VALUE : -CHECKMATE_VALUE;
                }

                // Compute the property value

                property_value += s_PieceValues[(int)color][(int)type].property;

//				// Compute the mobility value
//
//				{
//					MoveList moves;
//
//					pPiece->GeneratePossibleMoves( board, p, &moves );
//
//					mobility_value += moves.size() * color_factor;
//				}

                // Compute the position value

                {
                    position_value += s_PieceValues[(int)color][(int)type].property * s_PositionValues[(int)color][row][column];
                }

//				// Compute the threat value
//
//				{
//				}
            }
        }
    }

    int const castleStatusValue = _Evaluate(state.castleStatus_);

    // Compute the overall value

    int value;

    if (checkmate_value != 0)
    {
        value = checkmate_value;
    }
    else
    {
        value =   property_value * PROPERTY_FACTOR
                + position_value * POSITION_FACTOR / 1000
                + castleStatusValue * CASTLE_FACTOR
//				+ mobility_value * MOBILITY_FACTOR
//				+ threat_value * THREAT_FACTOR
        ;
    }

//#if defined( PROFILING )
//
//	// Randomize the value slightly so that equivalent moves have different
//	// values and profiling results of ordering methods are more realistic
//
//	value += ( state.GetHashCode() & 0x7 ) - 4;
//
//#endif // defined( PROFILING )

    return value;
}

// This function returns the change in the value of the board *** BY COLOR ***.

int StaticEvaluator::incremental(Move const &            move,
                                 GameState::CastleStatus castleStatus,
                                 Piece const *           pMoved /* = 0*/,
                                 Position const *        pRemovedPosition /* = 0*/,
                                 Piece const *           pRemoved /* = 0*/,
                                 Piece const *           pAdded /* = 0*/)
{
    assert(pMoved);

    // These special moves don't do anything

    if (move.isResignation() || move.isUndo() || move.isStartingPosition())
    {
        return 0;
    }

    // Check for checkmate (note: this assumes there is at least one king on the board)

    if (pRemoved && pRemoved->type() == PieceTypeId::KING)
    {
        return (pRemoved->color() == Color::WHITE) ? -CHECKMATE_VALUE : CHECKMATE_VALUE;
    }

    int value = 0;

    // Evaluate any change in position and property values from adding or removing pieces. Note: adding a piece
    // assumes promoting a pawn, so the added piece's position is the To position.

    if (pAdded)
    {
        int color         = (int)pAdded->color();
        int propertyValue = s_PieceValues[color][(int)pAdded->type()].property;

        value +=  propertyValue * PROPERTY_FACTOR;
        value +=  propertyValue * s_PositionValues[color][move.to().m_Row][move.to().m_Column] * POSITION_FACTOR / 1000;
    }

    if (pRemoved)
    {
        int color         = (int)pRemoved->color();
        int propertyValue = s_PieceValues[color][(int)pRemoved->type()].property;

        value -=  propertyValue * PROPERTY_FACTOR;
        value -=  propertyValue * s_PositionValues[color][pRemovedPosition->m_Row][pRemovedPosition->m_Column] * POSITION_FACTOR /
                 1000;
    }

    // Evaluate the change in position value from moving the piece

    {
        int color         = (int)pMoved->color();
        int propertyValue = s_PieceValues[color][(int)pMoved->type()].property;

        value -=  propertyValue * s_PositionValues[color][move.from().m_Row][move.from().m_Column] * POSITION_FACTOR / 1000;
        value +=  propertyValue * s_PositionValues[color][move.to().m_Row][move.to().m_Column] * POSITION_FACTOR / 1000;
    }

    // Evaluate any change in castling status

    value += _Evaluate(castleStatus) * CASTLE_FACTOR;

//#if defined( PROFILING )
//
//	// Randomize the value slightly so that equivalent moves have different
//	// values and profiling results of ordering methods are more realistic
//
//	value += ( state.GetHashCode() & 0x7 ) - 4;
//
//#endif // defined( PROFILING )

    return value;
}
