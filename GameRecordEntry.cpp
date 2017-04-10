/** @file *//********************************************************************************************************

                                                 GameRecordEntry.cpp

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Chess/GameRecordEntry.cpp#9 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "StdAfx.h"

#include "GameRecordEntry.h"

#include "GameState.h"
#include "Piece.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

GameRecordEntry::GameRecordEntry( GameState const & new_state )
	:	m_Move( new_state.move_ ),
		m_Board( new_state.board_ ),
		m_Value( new_state.value_ ),
		m_castleStatus( new_state.castleStatus_ )
{
	if ( !new_state.move_.isResignation() )
	{
		m_Piece = m_Board.pieceAt( m_Move.to() );
	}
	else
	{
		m_Piece = NO_PIECE;
	}
}


