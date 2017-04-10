/** @file *//********************************************************************************************************

                                                    GameState.cpp

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Chess/GameState.cpp#12 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "GameState.h"

#include "Move.h"
#include "Board.h"
#include "ZHash/ZHash.h"
#include "Piece.h"
#include "Piecelist.h"
#include "StaticEvaluator/StaticEvaluator.h"

#include "Misc/Etc.h"

Piece const *	GameState::whiteQueen_	= NO_PIECE;
Piece const *	GameState::blackQueen_	= NO_PIECE;


#if defined( GAME_STATE_ANALYSIS_ENABLED )

GameState::GameState( GameState const & old_state, Color color, Move const & move, int depth )
{
	analysisData_	= old_state.analysisData_;
	board_			= old_state.board_;
	castleStatus_	= old_state.castleStatus_;
	zhash_		= old_state.zhash_;

#if defined( INCREMENTAL_STATIC_EVALUATION_ENABLED )
	value_			= old_state.value_;
#endif

	// Add this move to the sequence

	int const	sequenceIndex	= depth - 1;

	if ( sequenceIndex < elementsof( analysisData_.expected ) )
	{
		if ( !move.isResignation() && !move.isUndo() )
		{
			if ( move.isKingSideCastle() || move.isQueenSideCastle() )
			{
				analysisData_.expected[ sequenceIndex ] = SequenceEntry( color, PieceTypeId::INVALID, move );
			}
			else
			{
				Piece const * const	pPiece	= board_.pieceAt( move.from() );
				analysisData_.expected[ sequenceIndex ] = SequenceEntry( color, pPiece->type(), move );
			}
		}
	}

	// Make the move
	makeMove( color, move );

	// Note: m_Value, m_Quality, and m_Priority are left unitialized
}

#else // defined( GAME_STATE_ANALYSIS_ENABLED )

GameState::GameState( GameState const & old_state, Color color, Move const & move )
{
	board_			= old_state.board_;
	castleStatus_	= old_state.castleStatus_;
	zhash_		= old_state.zhash_;

	makeMove( color, move );

	// Note: m_Value, m_Quality, and m_Priority are left unitialized
}

#endif // defined( GAME_STATE_ANALYSIS_ENABLED )


void GameState::makeMove( Color color, Move const & move )
{
	// Save the move

	move_ = move;

	// These special moves don't do anything

	if ( move.isResignation() || move.isUndo() || move.isStartingPosition() )
	{
		// Do nothing
	}

	// Castle is a special case move. Handle it separately

	else if ( move.isKingSideCastle() || move.isQueenSideCastle() )
	{
		makeCastleMove( color, move );
	}

	// Normal move

	else
	{
		makeNormalMove( color, move );
	}
}

void GameState::makeCastleMove( Color color, Move const & move )
{
	CastleStatus	oldStatus	= castleStatus_;

	Move	kingsMove;
	Move	rooksMove;

	if ( move.isKingSideCastle() )
	{
		kingsMove	= Move::kingSideCastleKing( color );
		rooksMove	= Move::kingSideCastleRook( color );
	}
	else
	{
		kingsMove	= Move::queenSideCastleKing( color );
		rooksMove	= Move::queenSideCastleRook( color );
	}

	// Move the king

	Piece const *	pKing	= board_.pieceAt( kingsMove.from() );

	zhash_.remove( *pKing, kingsMove.from() );
	zhash_.add( *pKing, kingsMove.to() );

	board_.movePiece( kingsMove.from(), kingsMove.to() );

	// Move the rook

	Piece const * const	pRook	= board_.pieceAt( rooksMove.from() );

	zhash_.remove( *pRook, rooksMove.from() );
	zhash_.add( *pRook, rooksMove.to() );

	board_.movePiece( rooksMove.from(), rooksMove.to() );

	// Update castle status

	if ( color == Color::WHITE )
	{
		castleStatus_.castled		|= ( move.isKingSideCastle() ) ? (int)CastleId::WHITE_KINGSIDE : (int)CastleId::WHITE_QUEENSIDE;
		castleStatus_.unavailable	|= (int)CastleId::WHITE_KINGSIDE| (int)CastleId::WHITE_QUEENSIDE;
	}
	else
	{
		castleStatus_.castled		|= ( move.isKingSideCastle() ) ? (int)CastleId::BLACK_KINGSIDE : (int)CastleId::BLACK_QUEENSIDE;
		castleStatus_.unavailable	|= (int)CastleId::BLACK_KINGSIDE| (int)CastleId::BLACK_QUEENSIDE;
	}

	CastleStatus castleStatusChange;
	castleStatusChange.status = oldStatus.status ^ castleStatus_.status;

	// Update check status
	// @todo check for in check

#if defined( INCREMENTAL_STATIC_EVALUATION_ENABLED )
	value_ += StaticEvaluator::incremental( move, castleStatusChange );
#endif // defined( INCREMENTAL_STATIC_EVALUATION_ENABLED )
}

void GameState::makeNormalMove( Color color, Move const & move )
{
	Piece const *	pMoved	= board_.pieceAt( move.from() );

	// Capture a piece

	Piece const *	pCaptured;
	Position		capturedPosition;

	if ( move.isEnPassant() )
	{
		capturedPosition = Position( move.from().m_Row, move.to().m_Column );
	}
	else
	{
		capturedPosition = move.to();
	}

	pCaptured = board_.pieceAt( capturedPosition );
	if ( pCaptured != NO_PIECE )
	{
		zhash_.remove( *pCaptured, capturedPosition );
		board_.removePiece( capturedPosition );
	}

	// Move the piece on the board

	zhash_.remove( *pMoved, move.from() );
	zhash_.add( *pMoved, move.to() );

	board_.movePiece( move.from(), move.to() );

	// Handle promotion

	Piece const *	pAdded;

	if ( move.isPromotion() )
	{
		pAdded = promote( color, move.to() );
	}
	else
	{
		pAdded = NO_PIECE;
	}

	// Update castle status

	CastleStatus	oldStatus	= castleStatus_;

	if ( color == Color::WHITE )
	{
		if ( pMoved->type() == PieceTypeId::KING )
		{
			castleStatus_.unavailable |= (int)CastleId::WHITE_KINGSIDE|(int)CastleId::WHITE_QUEENSIDE;
		}
		else if ( pMoved->type() == PieceTypeId::ROOK )
		{
			if ( move.from().m_Row == Board::SIZE-1 && move.from().m_Column == 0 )
			{
				castleStatus_.unavailable |= (int)CastleId::WHITE_QUEENSIDE;
			}
			else if ( move.from().m_Row == Board::SIZE-1 && move.from().m_Column == Board::SIZE-1 )
			{
				castleStatus_.unavailable |= (int)CastleId::WHITE_KINGSIDE;
			}
		}
	}
	else
	{
		if ( pMoved->type() == PieceTypeId::KING )
		{
			castleStatus_.unavailable |= (int)CastleId::BLACK_KINGSIDE| (int)CastleId::BLACK_QUEENSIDE;
		}
		else if ( pMoved->type() == PieceTypeId::ROOK )
		{
			if ( move.from().m_Row == 0 && move.from().m_Column == 0 )
			{
				castleStatus_.unavailable |= (int)CastleId::BLACK_KINGSIDE;
			}
			else if ( move.from().m_Row == 0 && move.from().m_Column == 7 )
			{
				castleStatus_.unavailable |= (int)CastleId::BLACK_QUEENSIDE;
			}
		}
	}

	CastleStatus castleStatusChange;
	castleStatusChange.status = oldStatus.status ^ castleStatus_.status;

	// Update check status
	// @todo check for in check

#if defined( INCREMENTAL_STATIC_EVALUATION_ENABLED )
	value_ += StaticEvaluator::incremental( move, castleStatusChange, pMoved, &capturedPosition, pCaptured, pAdded );
#endif // defined( INCREMENTAL_STATIC_EVALUATION_ENABLED )
}

Piece const * GameState::promote( Color color, Position const & position )
{
	Piece const *	pPiece	= board_.pieceAt( position );

	// Remove the pawn

	zhash_.remove( *pPiece, position );
	board_.removePiece( position );

	// Replace with a queen

	Piece const *	pAdded	= queen( color );

	zhash_.add( *pAdded, position );
	board_.putPiece( pAdded, position );

	return pAdded;
}

void GameState::initialize( PieceList const & white_pieces, PieceList const & black_pieces )
{
	board_.initialize( white_pieces, black_pieces );
	castleStatus_.status	= 0;

	zhash_ = ZHash( board_ );

	makeMove( Color::WHITE, Move::reset() );

	value_		= 0;
	quality_	= std::numeric_limits<int8_t>::min();
	priority_	= 0;

	// Save the queens

	whiteQueen_ = white_pieces[ PieceTypeId::QUEEN ];
	blackQueen_ = black_pieces[ PieceTypeId::QUEEN ];
}


#if defined( GAME_STATE_ANALYSIS_ENABLED )

void GameState::resetAnalysisData()
{
	analysisData_.reset();
}


void GameState::AnalysisData::reset()
{
	memset( expected, -1, sizeof( expected ) );
}

#endif // defined( GAME_STATE_ANALYSIS_ENABLED )
