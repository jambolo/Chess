/** @file *//********************************************************************************************************

                                                     MainFrm.cpp

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Chess/MainFrm.cpp#5 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "MainFrm.h"


#include "StdAfx.h"

#include "MainFrm.h"

#include "resource.h"
#include "Chess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

CMainFrame::~CMainFrame()
{
}



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

BOOL CMainFrame::PreCreateWindow( CREATESTRUCT& cs )
{
	if( !CFrameWnd::PreCreateWindow( cs ) )
		return FALSE;

	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

#if defined( _DEBUG )

void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump( CDumpContext & dc ) const
{
	CFrameWnd::Dump( dc );
}

#endif // defined( _DEBUG )


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_NEXTTURN, OnNextTurn)
END_MESSAGE_MAP()



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

int CMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CFrameWnd::OnCreate( lpCreateStruct ) == -1 )
		return -1;
	
	DWORD const window_style	= WS_CHILD | WS_VISIBLE |
								CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC;

	if ( !m_wndToolBar.CreateEx( this, TBSTYLE_FLAT, window_style ) ||
		 !m_wndToolBar.LoadToolBar( IDR_MAINFRAME ) )
	{
		TRACE0( "Failed to create toolbar\n" );
		return -1;      // fail to create
	}

	if ( !m_wndStatusBar.Create( this ) ||
		 !m_wndStatusBar.SetIndicators( indicators, sizeof( indicators ) / sizeof( UINT ) ) )
	{
		TRACE0( "Failed to create status bar\n" );
		return -1;      // fail to create
	}

//	// TODO: Delete these three lines if you don't want the toolbar to
//	//  be dockable
//
//	m_wndToolBar.EnableDocking( CBRS_ALIGN_ANY );
//	EnableDocking( CBRS_ALIGN_ANY );
//	DockControlBar( &m_wndToolBar );

	return 0;
}



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

LRESULT CMainFrame::OnNextTurn( WPARAM wParam, LPARAM lParam )
{
	// Just pass on to app

	return static_cast< CChessApp * >( AfxGetApp() )->OnNextTurn( wParam, lParam );
}
