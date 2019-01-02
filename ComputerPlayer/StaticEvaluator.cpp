#include "StaticEvaluator.h"

#include "GameState/GameState.h"
#include "GameState/Move.h"
#include "GameState/Piece.h"

#include <cassert>

namespace
{
float constexpr KING_PROPERTY_VALUE    = 4.0f;
float constexpr QUEEN_PROPERTY_VALUE   = 9.0f;
float constexpr BISHOP_PROPERTY_VALUE  = 3.0f;
float constexpr KNIGHT_PROPERTY_VALUE  = 3.0f;
float constexpr ROOK_PROPERTY_VALUE    = 5.0f;
float constexpr PAWN_PROPERTY_VALUE    = 1.0f;
float constexpr CASTLED_VALUE          = 2.0f;
float constexpr CASTLE_AVAILABLE_VALUE = 1.0f;

// Values for each piece
struct PieceValues
{
    float property;   // Standard property value
    float mobility;   // Average number moves on an empty board
    float position;   // Factor is standard property value
};

// Values of each square for each color

float constexpr s_PositionValues[Board::SIZE][Board::SIZE] =
{
    { 0.0f, 1.0f, 2.0f, 3.0f, 3.0f, 2.0f, 1.0f, 0.0f },
    { 1.0f, 2.0f, 3.0f, 4.0f, 4.0f, 3.0f, 2.0f, 1.0f },
    { 2.0f, 3.0f, 4.0f, 5.0f, 5.0f, 4.0f, 3.0f, 2.0f },
    { 3.0f, 4.0f, 5.0f, 6.0f, 6.0f, 5.0f, 4.0f, 3.0f },
    { 3.0f, 4.0f, 5.0f, 6.0f, 6.0f, 5.0f, 4.0f, 3.0f },
    { 2.0f, 3.0f, 4.0f, 5.0f, 5.0f, 4.0f, 3.0f, 2.0f },
    { 1.0f, 2.0f, 3.0f, 4.0f, 4.0f, 3.0f, 2.0f, 1.0f },
    { 0.0f, 1.0f, 2.0f, 3.0f, 3.0f, 2.0f, 1.0f, 0.0f },
};

PieceValues constexpr s_ValuesByPiece[NUMBER_OF_PIECE_TYPES] =
{
    { KING_PROPERTY_VALUE,    6.560f,  KING_PROPERTY_VALUE   }, // PieceTypeId::KING
    { QUEEN_PROPERTY_VALUE,  22.750f,  QUEEN_PROPERTY_VALUE  }, // PieceTypeId::QUEEN
    { BISHOP_PROPERTY_VALUE,  8.750f,  BISHOP_PROPERTY_VALUE }, // PieceTypeId::BISHOP
    { KNIGHT_PROPERTY_VALUE,  5.250f,  KNIGHT_PROPERTY_VALUE }, // PieceTypeId::KNIGHT
    { ROOK_PROPERTY_VALUE,   14.000f,  ROOK_PROPERTY_VALUE   }, // PieceTypeId::ROOK
    { PAWN_PROPERTY_VALUE,    1.667f,  PAWN_PROPERTY_VALUE   }  // PieceTypeId::PAWN
};

// The property factor is the reference. All other factors are relative to it.
float constexpr PROPERTY_FACTOR = 1.0f;

// Max castling value is 3 (one side has castled and the other cannot), which is worth about 3/4 a pawn
float constexpr CASTLE_FACTOR = PAWN_PROPERTY_VALUE * 0.75f / 3.0f;

// Maximum is about 144 which is worth a queen
float constexpr MOBILITY_FACTOR = QUEEN_PROPERTY_VALUE / 144.0f;

// Reasonable maximum is 100,000 which is worth a pawn
float constexpr POSITION_FACTOR = PAWN_PROPERTY_VALUE / 100000.0f;

// Maximum is 60 which is worth a pawn
float constexpr THREAT_FACTOR = PAWN_PROPERTY_VALUE / 60.0f;

float evaluate(GameState::CastleStatus castleStatus)
{
    // A bonus is given for having castled. Likewise, a penalty is given for losing the ability to castle.

    float value = 0.0f;
    if (castleStatus & BLACK_CASTLE)
        value += -CASTLED_VALUE;
    else if ((castleStatus & BLACK_CASTLE_UNAVAILABLE) == BLACK_CASTLE_UNAVAILABLE)
        value -= -CASTLE_AVAILABLE_VALUE;
    if (castleStatus & WHITE_CASTLE)
        value += CASTLED_VALUE;
    else if ((castleStatus & WHITE_CASTLE_UNAVAILABLE) == WHITE_CASTLE_UNAVAILABLE)
        value -= CASTLE_AVAILABLE_VALUE;

    return value;
}
} // anonymous namespace

// This function returns the value of the board *** BY COLOR ***.

float StaticEvaluator::evaluate(GameState const & state)
{
    // If both kings are present, checkmate_value will be 0, otherwise it will be -CHECKMATE_VALUE if the WHITE
    // king is missing, or CHECKMATE_VALUE if the BLACK king is missing.
    float totalCheckmateValue =  0.0f;

    // The difference in property value is computed by adding up the values of all the pieces on the board.
    float totalPropertyValue = 0.0f;

    // The value of mobility is calculated by finding the difference between the total number of moves available
    // to each side.
    float totalMobilityValue = 0.0f;

    // The value of position is is found by adding up the values of the positions of each piece (by color)
    float totalPositionValue = 0.0f;

    // The value of the threats is found by adding up the number of pieces attacked and protected by each piece (by color)
    float totalThreatValue = 0;

    // Compute the difference in values of the pieces on the board
    for (int row = 0; row < Board::SIZE; ++row)
    {
        for (int column = 0; column < Board::SIZE; ++column)
        {
            Piece const * p = state.board_.pieceAt(row, column);
            Position position{ row, column };

            if (p)
            {
                Color       color = p->color();
                PieceTypeId type  = p->type();

                // Compute the checkmate value
                if (type == PieceTypeId::KING)
                    totalCheckmateValue += (color == Color::WHITE) ? CHECKMATE_VALUE : -CHECKMATE_VALUE;

                // Compute the property difference
                {
                    float propertyValue = s_ValuesByPiece[(int)type].property;
                    if (color != Color::WHITE)
                        propertyValue = -propertyValue;
                    totalPropertyValue += propertyValue;
                }

                // Compute the mobility difference
                {
                    float mobilityValue = (float)p->countPossibleMoves(state, position);
                    if (color != Color::WHITE)
                        mobilityValue = -mobilityValue;
                    totalMobilityValue += mobilityValue;
                }

                // Compute the position difference
                {
                    float positionValue = s_ValuesByPiece[(int)type].position * s_PositionValues[row][column];
                    if (color != Color::WHITE)
                        positionValue = -positionValue;
                    totalPositionValue += positionValue;
                }

                // Compute threat difference
                {
                    float threatValue = (float)p->countThreats(state, position);
                    if (color != Color::WHITE)
                        threatValue = -threatValue;
                    totalThreatValue += threatValue;
                }
            }
        }
    }

    float castleStatusValue = ::evaluate(state.castleStatus_);

    // Compute the overall value

    float value;

    if (totalCheckmateValue != 0.0f)
    {
        value = totalCheckmateValue;
    }
    else
    {
        value =   totalPropertyValue * PROPERTY_FACTOR
                + totalPositionValue * POSITION_FACTOR
                + castleStatusValue * CASTLE_FACTOR
                + totalMobilityValue * MOBILITY_FACTOR
                + totalThreatValue * THREAT_FACTOR;
    }

#if defined(PROFILING)

    // Randomize the value slightly so that equivalent moves have different
    // values and profiling results of ordering methods are more realistic

    value += (state.GetHashCode() & 0x7) - 4;

#endif // defined( PROFILING )

    return value;
}

#if defined(FEATURE_INCREMENTAL_STATIC_EVALUATION)

// This function returns the change in the value of the board *** BY COLOR ***.

float StaticEvaluator::incremental(Move const &            move,
                                   GameState::CastleStatus castleStatus,
                                   Piece const *           pMoved /* = 0*/,
                                   Position const *        pRemovedPosition /* = 0*/,
                                   Piece const *           pRemoved /* = 0*/,
                                   Piece const *           pAdded /* = 0*/)
{
    assert(pMoved);

    // These special moves don't do anything

    if (move.isResignation() || move.isUndo() || move.isStartingPosition())
        return 0;

    // Check for checkmate (note: this assumes there is at least one king on the board)

    if (pRemoved && (pRemoved->type() == PieceTypeId::KING))
        return (pRemoved->color() == Color::WHITE) ? -CHECKMATE_VALUE : CHECKMATE_VALUE;

    float value = 0.0f;

    // Evaluate any change in position and property values from adding or removing pieces. Note: adding a piece
    // assumes promoting a pawn, so the added piece's position is the To position.

    if (pAdded)
    {
        int color         = (int)pAdded->color();
        int propertyValue = s_ValuesByPiece[color][(int)pAdded->type()].property;

        value +=  propertyValue * PROPERTY_FACTOR;
        value +=  propertyValue * s_PositionValues[color][move.to().row][move.to().column] * POSITION_FACTOR;
    }

    if (pRemoved)
    {
        int color         = (int)pRemoved->color();
        int propertyValue = s_ValuesByPiece[color][(int)pRemoved->type()].property;

        value -=  propertyValue * PROPERTY_FACTOR;
        value -=  propertyValue * s_PositionValues[color][pRemovedPosition->row][pRemovedPosition->column] * POSITION_FACTOR /
                 1000;
    }

    // Evaluate the change in position value from moving the piece

    {
        int color         = (int)pMoved->color();
        int propertyValue = s_ValuesByPiece[color][(int)pMoved->type()].property;

        value -=  propertyValue * s_PositionValues[color][move.from().row][move.from().column] * POSITION_FACTOR;
        value +=  propertyValue * s_PositionValues[color][move.to().row][move.to().column] * POSITION_FACTOR;
    }

    // Evaluate any change in castling status

    value += ::evaluate(castleStatus) * CASTLE_FACTOR;

#if defined(PROFILING)

    // Randomize the value slightly so that equivalent moves have different
    // values and profiling results of ordering methods are more realistic

    value += (state.GetHashCode() & 0x7) - 4;

#endif // defined( PROFILING )

    return value;
}

#endif defined(FEATURE_INCREMENTAL_STATIC_EVALUATION)
