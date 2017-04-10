/** @file *//********************************************************************************************************

                                                     ChessDoc.cpp

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Chess/ChessDoc.cpp#8 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "StdAfx.h"

#include "ChessDoc.h"

#include "NewGameDialog.h"
#include "GameState.h"
#include "Piece.h"
#include "Misc/Types.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

CChessDoc::CChessDoc()
	: m_HumanColor( WHITE )
{
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

CChessDoc::~CChessDoc()
{
}



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void CChessDoc::Initialize()
{
	// Recreate the pieces

	m_WhitePieces.Initialize( WHITE );
	m_BlackPieces.Initialize( BLACK );

	// Reset the game

	m_GameState.initialize( m_WhitePieces, m_BlackPieces );

}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void CChessDoc::DeleteContents()
{
	// Delete the pieces

	m_WhitePieces.Clear();
	m_BlackPieces.Clear();

	// Erase the game record

	m_GameRecord.clear();

	// Do base class stuff

	CDocument::DeleteContents();
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void CChessDoc::Serialize( CArchive& ar )
{
	if ( ar.IsStoring() )
	{
		ar << static_cast< int >( m_WhoseTurn );

		// Save the game record

		ar << static_cast< int >( m_GameRecord.size() );

		for ( GameRecord::const_iterator i = m_GameRecord.begin(); i != m_GameRecord.end(); ++i )
		{
			GameRecordEntry const & entry = *i;		// Convenience

			// Save the board

			Board const & board = entry.m_Board;	// Convenience

			for ( int r = 0; r < Board::SIZE; r++ )
			{
				for ( int c = 0; c < Board::SIZE; c++ )
				{
					Piece const * const	piece	= board.pieceAt( r, c );

					if ( piece != NO_PIECE )
					{
						ar << static_cast< int >( piece->color() );
						ar << static_cast< int >( piece->type() );
					}
					else
					{
						ar << static_cast< int >( INVALID );
						ar << static_cast< int >( PieceTypeId::INVALID );
					}
				}
			}

			// Save the last move

			Move const & move = entry.m_Move;	// Convenience
			
			ar << move.from().m_Row << move.from().m_Column;
			ar << move.to().m_Row << move.to().m_Column;

			// Save the value

			ar << entry.m_Value;
		}
	}
	else
	{
		// Do standard initialization first

		Initialize();

		// Load the saved game

		int color_int;

		ar >> color_int; ASSERT_LIMITS( INVALID, color_int, BLACK ); m_WhoseTurn = static_cast< Color >( color_int );

		// Load the game record

		int	size;

		ar >> size;

		m_GameRecord.resize( size );

		for ( GameRecord::iterator i = m_GameRecord.begin(); i != m_GameRecord.end(); ++i )
		{
			GameRecordEntry & entry = *i;		// Convenience

			// Load the board

			Board & board = entry.m_Board;				// Convenience

			board.Initialize( m_WhitePieces, m_BlackPieces );

			for ( int r = 0; r < Board::SIZE; r++ )
			{
				for ( int c = 0; c < Board::SIZE; c++ )
				{
					int				piece_type_int;
					int				color_int;
					PieceTypeId	piece_type;
					Color			color;

					ar >> color_int; ASSERT_LIMITS( WHITE, color_int, BLACK ); color = static_cast< Color >( color_int );
					ar >> piece_type_int; ASSERT_LIMITS( PieceTypeId::INVALID, piece_type_int, NUMBER_OF_PIECE_TYPES-1 ); piece_type = static_cast< PieceTypeId >( piece_type_int );

					if ( piece_type != PieceTypeId::INVALID )
					{
						Piece const * const piece = ( color == WHITE ) ?
														m_WhitePieces[ piece_type ] :
														m_BlackPieces[ piece_type ];
						board.putPiece( piece, Position( r, c ) );
					}
				}
			}

			// Load the last move

			Move & move = entry.m_Move;	// Convenience

			int8_t & from_row		= const_cast< Position & >( move.from() ).m_Row;
			int8_t & from_column	= const_cast< Position & >( move.from() ).m_Column;
			int8_t & to_row		= const_cast< Position & >( move.to() ).m_Row;
			int8_t & to_column	= const_cast< Position & >( move.to() ).m_Column;
			
			ar >> reinterpret_cast< __int8 & >( from_row ) >> reinterpret_cast< __int8 & >( from_column );
			ar >> reinterpret_cast< __int8 & >( to_row )   >> reinterpret_cast< __int8 & >( to_column );

			// Load the value

			ar >> entry.m_Value;
		}

		// Set the game state from the game record
	}
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

#ifdef _DEBUG

void CChessDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CChessDoc::Dump( CDumpContext & dc ) const
{
	CDocument::Dump( dc );
}

#endif //_DEBUG


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/


IMPLEMENT_DYNCREATE( CChessDoc, CDocument )

BEGIN_MESSAGE_MAP( CChessDoc, CDocument )
	//{{AFX_MSG_MAP(CChessDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

BOOL CChessDoc::OnNewDocument()
{
	if ( !CDocument::OnNewDocument() )
		return FALSE;

#if !defined( PROFILING )

	// Bring up a dialog box to get the top color and the human color. When
	// the app first starts, for user convenience, the dialog box is skipped and
	// the values are default.

	static bool the_first_time = true;

	if ( the_first_time )
	{
		// Skip the dialog box this time but never again

		the_first_time = false;
	}
	else
	{
		// Ask for the initial setup

		CNewGameDialog dlg;

		// Set the defaults to the values used last game

		dlg.m_HumanColor	= m_HumanColor;

		// Get the setup for this game

		dlg.DoModal();

		ASSERT( dlg.m_HumanColor == WHITE || dlg.m_HumanColor == BLACK );

		m_HumanColor	= static_cast< Color >( dlg.m_HumanColor );
	}

#endif // !defined( PROFILING )

	// Do standard initialization

	Initialize();

	return TRUE;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void CChessDoc::UndoMove()
{
	ASSERT( m_GameRecord.size() > 0 );

	// Erase the last record

	m_GameRecord.erase( m_GameRecord.end()-1 );

	// Force the state to the previous value according to the game record. If
	// the game has been undone to the beginning, then reset it instead.

	if ( m_GameRecord.size() > 0 )
	{
		m_GameState = GameState( m_GameRecord.back().m_Board,
								 m_GameRecord.back().m_Move,
								 m_GameRecord.back().m_Value,
								 m_GameRecord.back().m_castleStatus );

		ASSERT( m_GameState.board_.pieceAt( m_GameState.move_.to() ) != NO_PIECE );
		Color const who_just_moved = m_GameState.board_.pieceAt( m_GameState.move_.to() )->color();
		m_WhoseTurn = ( who_just_moved == WHITE ) ? BLACK : WHITE;
	}
	else
	{
		m_GameState.initialize( m_WhitePieces, m_BlackPieces );
		m_WhoseTurn = WHITE;
	}
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void CChessDoc::PlayMove( GameState const & new_state )
{
	// Make the move

	m_GameState = new_state;

	// Document the move

	m_GameRecord.push_back( GameRecordEntry( new_state ) );

	// Figure out who went and whose turn it is now

	if ( new_state.move_.isStartingPosition() )
	{
		m_WhoseTurn = WHITE;
	}
	else if ( new_state.move_.isResignation() )
	{
		m_WhoseTurn = INVALID;
	}
	else
	{
		// Figure out who just moved and it's the other color's turn now

		ASSERT( new_state.board_.pieceAt( new_state.move_.to() ) != NO_PIECE );
		Color const who_just_moved = new_state.board_.pieceAt( new_state.move_.to() )->color();
		m_WhoseTurn = ( who_just_moved == WHITE ) ? BLACK : WHITE;
	}
}


#if defined( CHESS_DOC_ANALYSIS_ENABLED )

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void CChessDoc::ResetAnalysisData()
{
	m_analysisData.Reset();
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void CChessDoc::AnalysisData::Reset()
{
	playerAnalysisData.Reset();
}

#endif // defined( CHESS_DOC_ANALYSIS_ENABLED )
