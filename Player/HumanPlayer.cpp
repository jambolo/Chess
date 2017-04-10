/** @file *//********************************************************************************************************

                                                   HumanPlayer.cpp

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Chess/HumanPlayer.cpp#9 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "StdAfx.h"

#include "HumanPlayer.h"

#include "Board.h"
#include "GameState.h"
#include "Piece.h"
#include "YourTurnDialog.h"
#include "Move.h"

#include <limits>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

HumanPlayer::HumanPlayer( Color color )
	: Player( color )
{
}




/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

HumanPlayer::~HumanPlayer()
{
}



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

GameState HumanPlayer::MyTurn( GameState const & s0 )
{
	Board const & board = s0.board_;
	Move move;

	// Get a valid move

	CYourTurnDialog dlg;

	while ( true )
	{
		int result = dlg.DoModal();

		if ( result == IDOK )
		{
			if ( dlg.m_Resign )
			{
				move = Move::resign();
				break;
			}
			else if ( dlg.m_Undo )
			{
				move = Move::undo();
				break;
			}
			else if ( dlg.m_KingSideCastle  )
			{
				if ( s0.kingSideCastleIsAllowed( m_MyColor ) )
				{
					move = Move::kingSideCastle();
					break;
				}
			}
			else if ( dlg.m_QueenSideCastle )
			{
				if ( s0.queenSideCastleIsAllowed( m_MyColor ) )
				{
					move = Move::queenSideCastle();
					break;
				}
			}
			else
			{
				if ( dlg.m_From.GetLength() == 2 && dlg.m_To.GetLength() == 2 )
				{
					Position const from( 8 - (dlg.m_From[1]-'0'), dlg.m_From[0]-'a' );
					Position const to( 8 - (dlg.m_To[1]-'0'), dlg.m_To[0]-'a' );

					if ( board.IsValidPosition( to ) && board.IsValidPosition( from ) )
					{
						Piece const * piece = board.pieceAt( from );

						if ( piece != NO_PIECE )
						{

							if ( ( piece->color() == m_MyColor ) && piece->isValidMove( s0, Move( from, to ) ) )
							{
								move = Move( from, to );
								break;
							}
						}
					}
				}
			}
		}
	}

#if defined( PLAYER_ANALYSIS_ENABLED )

	memset( &m_analysisData, -1, sizeof( m_analysisData ) );
	m_analysisData.time		= 0;
	m_analysisData.value	= 0;

#endif // defined( PLAYER_ANALYSIS_ENABLED )

#if defined( GAME_STATE_ANALYSIS_ENABLED )
	return GameState( s0, m_MyColor, move, 1 );
#else // defined( GAME_STATE_ANALYSIS_ENABLED )
	return GameState( s0, m_MyColor, move );
#endif // defined( GAME_STATE_ANALYSIS_ENABLED )
}
