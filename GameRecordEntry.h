/** @file *//********************************************************************************************************

                                                  GameRecordEntry.h

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Chess/GameRecordEntry.h#9 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once


#include "Board.h"
#include "Move.h"
#include "GameState.h"
#include <vector>

class Piece;


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

// This is just a condensed version of the game state though I'm not sure what its full purpose is yet

class GameRecordEntry
{
public:
	GameRecordEntry() {}
	GameRecordEntry( GameState const & new_state );

	Piece const *			m_Piece;			// Piece moved
	Move					m_Move;				// The move
	Board					m_Board;			// The resulting board
	int						m_Value;			// Value of the game after this move
	GameState::CastleStatus	m_castleStatus;		// Status of castling
};


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

class GameRecord : public std::vector<GameRecordEntry>
{
};
