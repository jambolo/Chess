/** @file *//********************************************************************************************************

                                                    ChessView.cpp

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Chess/ChessView.cpp#8 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "StdAfx.h"

#include "ChessView.h"

#include "ChessDoc.h"
#include "Board.h"
#include "Piece.h"
#include "GameRecordEntry.h"
#include "GameTree.h"
#include "TranspositionTable.h"

#include <sstream>
#include <limits>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


namespace
{


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

std::ostringstream & operator<<( std::ostringstream & stream, GameRecordEntry const & entry )
{
	stream << entry.m_Piece->symbol();
	stream << char( 'a' + entry.m_Move.from().m_Column ) << char ( '8' - entry.m_Move.from().m_Row );
	stream << '-';
	stream << char( 'a' + entry.m_Move.to().m_Column ) << char( '8' - entry.m_Move.to().m_Row );

	return stream;
}

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

std::ostringstream & operator<<( std::ostringstream & stream, SequenceEntry const & entry )
{
	stream << Piece::symbol( entry.GetPieceTypeId() );
	stream << char( 'a' + entry.m_FromColumn ) << 8 - entry.m_FromRow;
	stream << '-';
	stream << char( 'a' + entry.m_ToColumn ) << 8 - entry.m_ToRow;

	return stream;
}


} // anonymous namespace

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

CChessView::CChessView()
{
	// TODO: add construction code here

}



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

CChessView::~CChessView()
{
}



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

BOOL CChessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

#if defined( _DEBUG )


void CChessView::AssertValid() const
{
	CView::AssertValid();
}


void CChessView::Dump( CDumpContext & dc ) const
{
	CView::Dump( dc );
}


CChessDoc * CChessView::GetDocument() // non-debug version is inline
{
	ASSERT( m_pDocument->IsKindOf( RUNTIME_CLASS( CChessDoc ) ) );
	return static_cast< CChessDoc * >( m_pDocument );
}

CChessDoc const * CChessView::GetDocument() const // non-debug version is inline
{
	ASSERT( m_pDocument->IsKindOf( RUNTIME_CLASS( CChessDoc ) ) );
	return static_cast< CChessDoc * >( m_pDocument );
}


#endif // defined( _DEBUG )



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

IMPLEMENT_DYNCREATE( CChessView, CView )

BEGIN_MESSAGE_MAP( CChessView, CView )
	//{{AFX_MSG_MAP(CChessView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND( ID_FILE_PRINT, CView::OnFilePrint )
	ON_COMMAND( ID_FILE_PRINT_DIRECT, CView::OnFilePrint )
	ON_COMMAND( ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview )
END_MESSAGE_MAP()



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void CChessView::OnDraw( CDC * pDC)
{
	CChessDoc* pDoc = GetDocument();
	ASSERT_VALID( pDoc );

	DrawBoard( pDC, CPoint( 20, 20 ) );
	DrawStats( pDC, CPoint( 20, 400 ) );
	DrawRecord( pDC, CPoint( 400, 20 ) );
}



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

BOOL CChessView::OnPreparePrinting( CPrintInfo * pInfo )
{
	// default preparation
	return DoPreparePrinting( pInfo );
}



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void CChessView::OnBeginPrinting( CDC * /*pDC*/, CPrintInfo * /*pInfo*/ )
{
	// TODO: add extra initialization before printing
}



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void CChessView::OnEndPrinting( CDC * pDC, CPrintInfo * pInfo )
{
	// TODO: add cleanup after printing
}



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void CChessView::DrawBoard( CDC * dc, CPoint const & xy ) const
{
	int const		board_size	= 320;
	int const		square_size	= board_size / Board::SIZE;
	Board const &	board		= GetDocument()->m_GameState.board_;
	int				i, j;
	char			label[2];

	// Label the squares

	for ( i = 0; i < Board::SIZE; ++i )
	{
		label[0] = 'a' + i;
		label[1] = 0;
		dc->TextOut( xy.x + i * square_size + square_size/2, xy.y + board_size + 10, label );
	}

	for ( i = 0; i < Board::SIZE; ++i )
	{
		label[0] = '8' - i;
		label[1] = 0;
		dc->TextOut( xy.x + board_size + 10, xy.y + i * square_size + square_size/2, label );
	}

	// Do the board outline

	CBrush	outline_brush( RGB( 0, 0, 0 ) );
	CRect	outline_rect( xy, CSize( board_size+2, board_size+2 ) );

	dc->FrameRect( &outline_rect, &outline_brush );

	// Draw the squares

	CBrush white_brush( RGB( 192, 192, 192 ) );
	CBrush black_brush( RGB( 128, 128, 128 ) );
	dc->SelectStockObject( NULL_PEN );
	int y = outline_rect.top + 1;
	for ( i = 0; i < Board::SIZE; ++i )
	{
		int x = outline_rect.left + 1;
		for ( j = 0; j < Board::SIZE; j++ )
		{
			if ( ( ( i ^ j ) & 1 ) == 0 )			// Note: generates a checkerboard pattern
				dc->SelectObject( &white_brush );
			else
				dc->SelectObject( &black_brush );

			dc->Rectangle( x, y, x+square_size+1, y+square_size+1 );

			x += square_size;
		}
		y += square_size;
	}

	dc->SelectStockObject( BLACK_BRUSH );	// Release the last selected brush so it can be deleted

	// Draw the pieces

	CDC bitmap_dc;
	bitmap_dc.CreateCompatibleDC( NULL );

	CDC image_dc;
	image_dc.CreateCompatibleDC( NULL );
	CBitmap image_bitmap;
	image_bitmap.CreateCompatibleBitmap( dc, square_size, square_size );
	CBitmap* old_image_bitmap = image_dc.SelectObject( &image_bitmap );

	CDC mask_dc;
	mask_dc.CreateCompatibleDC( NULL );
	CBitmap mask_bitmap;
	mask_bitmap.CreateBitmap( square_size, square_size, 1, 1, NULL );
	CBitmap* old_mask_bitmap = mask_dc.SelectObject( &mask_bitmap );

	Position p;
	for ( p.m_Row = 0; p.m_Row < Board::SIZE; p.m_Row++ )
	{
		for ( p.m_Column = 0; p.m_Column < Board::SIZE; p.m_Column++ )
		{
			Piece const * const piece	= board.pieceAt( p );

			if ( piece != NO_PIECE )
			{
				CBitmap const * const bitmap = piece->image();

				BITMAP	bm;
				const_cast< CBitmap * >( bitmap )->GetBitmap( &bm );
				ASSERT( bm.bmWidth <= square_size );
				ASSERT( bm.bmHeight <= square_size );

				CBitmap* old_bitmap = bitmap_dc.SelectObject( const_cast< CBitmap * >( bitmap ) );
				image_dc.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &bitmap_dc, 0, 0, SRCCOPY );
				bitmap_dc.SelectObject( old_bitmap );


				int const x = outline_rect.left+1 + p.m_Column*square_size + ( square_size - bm.bmWidth ) / 2;
				int const y = outline_rect.top+1 + p.m_Row*square_size + ( square_size - bm.bmHeight ) / 2;

				image_dc.SetBkColor( RGB( 255, 0, 255 ) );
				mask_dc.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &image_dc, 0, 0, SRCCOPY );
				dc->BitBlt( x, y, bm.bmWidth, bm.bmHeight, &mask_dc, 0, 0, SRCAND );
				mask_dc.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, NULL, 0, 0, DSTINVERT );
				image_dc.SetBkColor( RGB( 255, 255, 255 ) );
				image_dc.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &mask_dc, 0, 0, SRCAND );
				dc->BitBlt( x, y, bm.bmWidth, bm.bmHeight, &image_dc, 0, 0, SRCPAINT );
			}
		}
	}

	mask_dc.SelectObject( old_mask_bitmap );
	mask_bitmap.DeleteObject();

	image_dc.SelectObject( old_image_bitmap );
	image_bitmap.DeleteObject();
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void CChessView::DrawStats( CDC * dc, CPoint const & xy ) const
{
	CChessDoc const * const pdoc = GetDocument();

	if ( pdoc->m_GameState.move_.isStartingPosition() )
	{
		return;
	}

#if defined( CHESS_DOC_ANALYSIS_ENABLED )

	Player::AnalysisData const &				playerAnalysis		= pdoc->m_analysisData.playerAnalysisData;
	GameTree::AnalysisData const &				gameTreeAnalysis	= playerAnalysis.gameTreeAnalysisData;
	GameState::AnalysisData const &				gameStateAnalysis	= gameTreeAnalysis.gameStateAnalysisData;
	TranspositionTable::AnalysisData const &	tTableAnalysis		= gameTreeAnalysis.tTableAnalysisData;

	int x = xy.x;
	int y = xy.y;

	y += 20;

	if ( playerAnalysis.time >= 0 )
	{
		std::ostringstream buffer;
		buffer << "Elapsed Time: " << playerAnalysis.time / 1000.f << std::ends;
		dc->TextOut( x, y, buffer.str().c_str() );
	}

	y += 20;

	if ( gameTreeAnalysis.aGeneratedStateCounts[0] >= 0 )
	{
		int					total	= 0;
		std::ostringstream	buffer;

		buffer << "Generated States: ";
		for ( int i = 0; i < elementsof( gameTreeAnalysis.aGeneratedStateCounts ); ++i )
		{
			if ( gameTreeAnalysis.aGeneratedStateCounts[i] > 0 )
			{
				total += gameTreeAnalysis.aGeneratedStateCounts[i];
				buffer << i << ": " << gameTreeAnalysis.aGeneratedStateCounts[i] << " ";
			}
		}

		buffer << "Total: " << total;
		buffer << std::ends;
		dc->TextOut( x, y, buffer.str().c_str() );
	}

	y += 20;

	if ( gameTreeAnalysis.aEvaluationCounts[0] >= 0 )
	{
		int					total	= 0;
		std::ostringstream	buffer;

		buffer << "Static Evaluations: ";
		for ( int i = 0; i < elementsof( gameTreeAnalysis.aEvaluationCounts ); ++i )
		{
			if ( gameTreeAnalysis.aEvaluationCounts[i] > 0 )
			{
				total += gameTreeAnalysis.aEvaluationCounts[i];
				buffer << i << ": " << gameTreeAnalysis.aEvaluationCounts[i] << " ";
			}
		}

		buffer << "Total: " << total;
		buffer << std::ends;
		dc->TextOut( x, y, buffer.str().c_str() );
	}

	y += 20;

	if ( gameTreeAnalysis.alphaHitCount >= 0 )
	{
		std::ostringstream buffer;
		buffer << "Alpha hits: " << gameTreeAnalysis.alphaHitCount << "  ";
		buffer << "Beta hits: " << gameTreeAnalysis.betaHitCount;
		buffer << std::ends;
		dc->TextOut( x, y, buffer.str().c_str() );
	}

	y += 20;

	if ( tTableAnalysis.usage >= 0 )
	{
		{
			std::ostringstream buffer;
			buffer << "Table usage: " << tTableAnalysis.usage << " (" << ( tTableAnalysis.usage * 100 ) / TranspositionTable::SIZE << "%)";
			buffer << std::ends;
			dc->TextOut( x, y, buffer.str().c_str() );
		}
		y += 20;
		{
			std::ostringstream buffer;
			buffer << "    Checks: " << tTableAnalysis.checkCount << "  ";
			buffer << "Hits: " << tTableAnalysis.hitCount << " (" << ( tTableAnalysis.hitCount * 100 ) / tTableAnalysis.checkCount << "%)  ";
			buffer << "Collisions: " << tTableAnalysis.collisionCount << " (" << ( tTableAnalysis.collisionCount * 100 ) / tTableAnalysis.checkCount << "%)";
			buffer << std::ends;
			dc->TextOut( x, y, buffer.str().c_str() );
		}
		y += 20;
		{
			std::ostringstream buffer;
			buffer << "    Updates: " << tTableAnalysis.updateCount << "  ";
			buffer << "Overwritten: " << tTableAnalysis.overwritten << " (" << ( tTableAnalysis.overwritten * 100 ) / tTableAnalysis.updateCount << "%)  ";
			buffer << "Refreshed: " << tTableAnalysis.refreshed << " (" << ( tTableAnalysis.refreshed * 100 ) / tTableAnalysis.updateCount << "%)  ";
			buffer << "Rejected: " << tTableAnalysis.rejected << " (" << ( tTableAnalysis.rejected * 100 ) / tTableAnalysis.updateCount << "%)";
			buffer << std::ends;
			dc->TextOut( x, y, buffer.str().c_str() );
		}
	}

	y += 20;

	if ( playerAnalysis.value != std::numeric_limits<int>::min() )
	{
		std::ostringstream buffer;
		buffer.setf( std::ios::fixed, std::ios::floatfield );
		buffer.precision( 2 );

		buffer << "Value: ";
		if ( playerAnalysis.value == GameTree::MY_CHECKMATE_VALUE() )
			buffer << "I win";
		else if ( playerAnalysis.value == GameTree::OPPONENT_CHECKMATE_VALUE() )
			buffer << "I lose";
		else
			buffer << float( playerAnalysis.value ) / 1000.f;

		buffer << " (worst value: ";
		if ( gameTreeAnalysis.worstValue == GameTree::MY_CHECKMATE_VALUE() )
			buffer << "I win";
		else if ( gameTreeAnalysis.worstValue == GameTree::OPPONENT_CHECKMATE_VALUE() )
			buffer << "I lose";
		else
			buffer << float( gameTreeAnalysis.worstValue ) / 1000.f;
		
		buffer << ")" << std::ends;

		dc->TextOut( x, y, buffer.str().c_str() );
	}

	y += 20;

	if ( gameStateAnalysis.expected[ 0 ].GetPieceTypeId() != PieceTypeId::INVALID )
	{
		std::ostringstream buffer;

		buffer << "Expected sequence: ";

		for ( int i = 1;
			  i < GameState::EXPECTED_SEQUENCE_SIZE && gameStateAnalysis.expected[ i ].GetPieceTypeId() != PieceTypeId::INVALID;
			  ++i )
		{
			buffer << gameStateAnalysis.expected[ i ] << ' ';
		}
		buffer << std::ends;

		dc->TextOut( x, y, buffer.str().c_str() );
	}

#endif // defined( CHESS_DOC_ANALYSIS_ENABLED )
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void CChessView::DrawRecord( CDC * dc, CPoint const & xy ) const
{
	int const RECORD_ROWS = 10;
	GameRecord const & game_record = GetDocument()->m_GameRecord;

	int x = xy.x;
	int y = xy.y;

	GameRecord::const_iterator e;
	if ( game_record.size() < RECORD_ROWS*2 )
		e = game_record.begin();
	else
		e = game_record.begin() + ( ( game_record.size()+1 )/2 - RECORD_ROWS ) * 2;

	while ( e != game_record.end() )
	{
		// Display move number

		{
			std::ostringstream move_number;
			move_number << ( e-game_record.begin() ) / 2 + 1 << '.' << std::ends;
			dc->TextOut( x, y, move_number.str().c_str() );
		}

		// Display white's move

		GameRecordEntry const & whites_move = *e++;

		if ( whites_move.m_Piece != NO_PIECE )
		{
			std::ostringstream buffer;

			buffer << whites_move << std::ends;
			dc->TextOut( x+30, y, buffer.str().c_str() );
		}
		else
		{
			dc->TextOut( x+30, y, "White resigns" );
			break;
		}

		if ( e == game_record.end() )
			break;

		// Display black's move

		GameRecordEntry const & blacks_move = *e++;

		if ( blacks_move.m_Piece != NO_PIECE )
		{
			std::ostringstream buffer;

			buffer << blacks_move << std::ends;
			dc->TextOut( x+120, y, buffer.str().c_str() );
		}
		else
		{
			dc->TextOut( x+120, y, "Black resigns" );
			break;
		}

		// Next row

		y += 20;
	}
}
