/** @file *//********************************************************************************************************

                                                      Sequence.h

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Chess/Sequence.h#7 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once


#include "Move.h"
#include <vector>


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

class SequenceEntry
{
public:

	SequenceEntry();
	SequenceEntry( Color color, PieceTypeId piece, Move const & move );

	Move			GetMove() const;
	Color			GetColor() const;
	PieceTypeId		GetPieceTypeId() const;

	uint16_t	m_Color			: 1;
	uint16_t	m_PieceTypeId	: 3;	// Note: 7 is PieceTypeId::INVALID
	uint16_t	m_FromRow		: 3;
	uint16_t	m_FromColumn	: 3;
	uint16_t	m_ToRow			: 3;
	uint16_t	m_ToColumn		: 3;
};


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

class Sequence : public std::vector< SequenceEntry >
{
};


#include "Sequence.inl"
