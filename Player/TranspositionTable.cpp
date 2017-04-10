/********************************************************************************************************************

                                                TranspositionTable.cpp

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Chess/TranspositionTable.cpp#11 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "StdAfx.h"

#include "TranspositionTable.h"

#include "GameState.h"

#include "Misc/Etc.h"
#include "Misc/Types.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

TranspositionTable::TranspositionTable()
{
	// Invalidate all entries in the table

	for ( int i = 0; i < elementsof( table_ ); ++i )
	{
		table_[ i ].clear();
	}
}

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

bool TranspositionTable::check( GameState const & state, int * pReturnedValue, int8_t * pReturnedQuality ) const
{
#if defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )
	++analysisData_.checkCount;
#endif // defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )

	ZHash 				hashCode		= state.zhash();
	uint32				upperHashCode	= (uint32)( hashCode >> 32 );
	uint32				lowerHashCode	= (uint32)( hashCode & 0xffffffff );
	int					index			= lowerHashCode % SIZE;
	TableEntry const &	entry			= table_[ index ];

	ASSERT( upperHashCode != TableEntry::UNUSED_ENTRY );

	// A hit occurs if the states are the same.

	bool hit = false;

	if ( entry.hashCode_ == upperHashCode )
	{
#if defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )
		++analysisData_.hitCount;
#endif // defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )

		entry.age_ = 0;	// Reset age
		hit = true;
		*pReturnedValue		= entry.value_;
		*pReturnedQuality	= entry.q_;
	}
	else
	{
#if defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )
		if ( entry.hashCode_ != TableEntry::UNUSED_ENTRY )
		{
			++analysisData_.collisionCount;
		}
#endif // defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )
	}

	return hit;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

bool TranspositionTable::check( GameState const & state, int minQ, int * pReturnedValue ) const
{
#if defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )
	++analysisData_.checkCount;
#endif // defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )

	ZHash 				hashCode		= state.zhash();
	uint32				upperHashCode	= (uint32)( hashCode >> 32 );
	uint32				lowerHashCode	= (uint32)( hashCode & 0xffffffff );
	int					index			= lowerHashCode % SIZE;
	TableEntry const &	entry			= table_[ index ];

	ASSERT( upperHashCode != TableEntry::UNUSED_ENTRY );

	// A hit occurs if the states are the same, and the minimum quality is <= the quality of the stored state. The
	// reason for the quality check is that if the stored quality is less, then we aren't going to want the value
	// of the stored state anyway.

	bool hit	= false;

	if ( entry.hashCode_ == upperHashCode )
	{
#if defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )
		++analysisData_.hitCount;
#endif // defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )

		entry.age_ = 0;	// Reset age

		if ( entry.q_ >= minQ )
		{
			hit = true;
			*pReturnedValue = entry.value_;
		}
	}
	else
	{
#if defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )
		if ( entry.hashCode_ != TableEntry::UNUSED_ENTRY )
		{
			++analysisData_.collisionCount;
		}
#endif // defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )
	}

	return hit;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void TranspositionTable::forceUpdate( GameState const & state )
{
#if defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )
	++analysisData_.updateCount;
#endif // defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )

	ZHash 			hashCode		= state.zhash();
	uint32			upperHashCode	= (uint32)( hashCode >> 32 );
	uint32			lowerHashCode	= (uint32)( hashCode & 0xffffffff );
	int				index			= lowerHashCode % SIZE;
	TableEntry &	entry			= table_[ index ];

	ASSERT( upperHashCode != TableEntry::UNUSED_ENTRY );

	// If the entry is unused or if the new quality >= the stored quality, then store the new state. It is better
	// to replace values of equal quality in order to dispose of old entries that may no longer be relevant.

#if defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )

	// For tracking the number of used entries

	if ( entry.hashCode_ == TableEntry::UNUSED_ENTRY )
	{
		++analysisData_.usage;
	}
	else if ( entry.hashCode_ == upperHashCode )
	{
		++analysisData_.refreshed;
	}
	else
	{
		++analysisData_.overwritten;
	}

#endif // defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )

	// Store the state, value and quality

	ASSERT( state.quality_ < 256 );
	entry.hashCode_	= upperHashCode;
	entry.value_		= state.value_;
	entry.q_			= state.quality_;
	entry.age_			= 0;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void TranspositionTable::update( GameState const & state )
{
#if defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )
	++analysisData_.updateCount;
#endif // defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )

	ZHash 			hashCode		= state.zhash();
	uint32			upperHashCode	= (uint32)( hashCode >> 32 );
	uint32			lowerHashCode	= (uint32)( hashCode & 0xffffffff );
	int				index			= lowerHashCode % SIZE;
	TableEntry &	entry			= table_[ index ];

	ASSERT( upperHashCode != TableEntry::UNUSED_ENTRY );

	bool	isUnused	= ( entry.hashCode_ == TableEntry::UNUSED_ENTRY );

	// If the entry is unused or if the new quality >= the stored quality, then store the new state. It is better
	// to replace values of equal quality in order to dispose of old entries that may no longer be relevant.

	if ( isUnused || state.quality_ >= entry.q_ )
	{
#if defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )

		// For tracking the number of used entries

		if ( isUnused )
		{
			++analysisData_.usage;
		}
		else if ( entry.hashCode_ == upperHashCode )
		{
			++analysisData_.refreshed;
		}
		else
		{
			++analysisData_.overwritten;
		}

#endif // defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )

		// Store the state, value and quality

		entry.hashCode_	= upperHashCode;
		entry.value_		= state.value_;
		entry.q_			= state.quality_;
		entry.age_			= 0;
	}
	else
	{
#if defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )
		++analysisData_.rejected;
#endif // defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )
	}
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

// The T-table is persistent. So in order to gradually dispose of entries that are no longer relevant, entries
// that have not been reference for a while are removed.

void TranspositionTable::age()
{
	for ( int i = 0; i < SIZE; ++i )
	{
		TableEntry &	entry		= table_[ i ];

		if ( entry.hashCode_ != TableEntry::UNUSED_ENTRY )
		{
			++entry.age_;
			if ( entry.age_ > MAX_AGE )
			{
				entry.hashCode_ = TableEntry::UNUSED_ENTRY;

#if defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )
				--analysisData_.usage;
#endif // defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )
			}
		}
	}
}


#if defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void TranspositionTable::resetAnalysisData()
{
	analysisData_.reset();
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void TranspositionTable::AnalysisData::reset()
{

	checkCount		= 0;
	updateCount		= 0;
	hitCount		= 0;
//	usage			= 0;	// never reset
	collisionCount	= 0;
	rejected		= 0;
	overwritten		= 0;
	refreshed		= 0;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

TranspositionTable::AnalysisData::AnalysisData()
	: usage( 0 )
{
	reset();
}

#endif // defined( TRANSPOSITION_TABLE_ANALYSIS_ENABLED )
