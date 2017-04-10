/** @file *//********************************************************************************************************

                                                      Chess.cpp

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Chess/Chess.cpp#10 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "StdAfx.h"

#include "Chess.h"

#include "MainFrm.h"
#include "ChessDoc.h"
#include "ChessView.h"
#include "AboutDialog.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if defined( _DEBUG )
static int const	MAX_DEPTH	= 5;
#elif defined( PROFILING )
static int const	MAX_DEPTH	= 6;
#else
static int const	MAX_DEPTH	= 6;
#endif

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

CChessApp theApp;



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/


CChessApp::CChessApp()
	: m_pBlackPlayer( 0 ),
	m_pWhitePlayer( 0 ),
	m_pCurrentGame( 0 ),
	m_pComputerPlayer( 0 ),
	m_maxDepth( MAX_DEPTH )
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}





/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

BOOL CChessApp::InitInstance()
{
	// Standard initialization

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CChessDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CChessView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// Initialize the random number generator

	srand( (unsigned int)time( NULL ) );

	return TRUE;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

int CChessApp::ExitInstance() 
{
	// Delete the players

	delete m_pWhitePlayer;
	delete m_pBlackPlayer;

	return CWinApp::ExitInstance();
}



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/


BEGIN_MESSAGE_MAP(CChessApp, CWinApp)
	//{{AFX_MSG_MAP(CChessApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	// Standard file based document commands
//	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

// App command to run the dialog
void CChessApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void CChessApp::OnFileNew()
{
	// Load the new game

	CWinApp::OnFileNew();

	// Save a pointer to the new document

	POSITION template_position = GetFirstDocTemplatePosition();
	CDocTemplate * const doc_template = GetNextDocTemplate( template_position );
	POSITION doc_position = doc_template->GetFirstDocPosition();
	m_pCurrentGame = static_cast< CChessDoc * >( doc_template->GetNextDoc( doc_position ) );

	// Set up the players

	delete m_pWhitePlayer;
	delete m_pBlackPlayer;

#if !defined( PROFILING )

	if ( m_pCurrentGame->GetHumanColor() == WHITE )
	{
		m_pWhitePlayer = new HumanPlayer( WHITE );
		m_pBlackPlayer = new ComputerPlayer( BLACK, m_maxDepth );
		m_pComputerPlayer = static_cast< ComputerPlayer const * >( m_pBlackPlayer );
	}
	else
	{
		m_pBlackPlayer = new HumanPlayer( BLACK );
		m_pWhitePlayer = new ComputerPlayer( WHITE, m_maxDepth );
		m_pComputerPlayer = static_cast< ComputerPlayer const * >( m_pWhitePlayer );
	}

#else // !defined( PROFILING )

	m_pWhitePlayer = new ComputerPlayer( WHITE, m_maxDepth );
	m_pBlackPlayer = new ComputerPlayer( BLACK, m_maxDepth );
	m_pComputerPlayer = static_cast< ComputerPlayer const * >( m_pBlackPlayer );

#endif // !defined( PROFILING )

	// Start the game

	BOOL ok = m_pMainWnd->PostMessage( WM_NEXTTURN, WHITE, 0 );
	ASSERT( ok );
}



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void CChessApp::OnFileOpen() 
{
	// Open a file

	CWinApp::OnFileOpen();

//	if ( m_pCurrentGame->m_WhoseTurn != INVALID )
//		m_pMainWnd->PostMessage( WM_NEXTTURN, m_pCurrentGame->m_WhoseTurn );
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

LRESULT CChessApp::OnNextTurn( WPARAM wParam, LPARAM lParam )
{
	ASSERT_LIMITS( INVALID, static_cast< int >( wParam ), BLACK );
	ASSERT( lParam >= 0 );

	Color const	whose_turn	= static_cast< Color >( wParam );
	int const		turn		= lParam;

#if defined( PROFILING )

	// If it is past turn 100, then that's long enough, so don't continue

	if ( turn > 50 )
	{
		m_pMainWnd->PostMessage( WM_QUIT );
		return 0;
	}

#endif // defined( PROFILING )

	// If it's nobody's turn, the just return

	if ( whose_turn == INVALID )
		return 0;


	Player * const this_player = ( whose_turn == WHITE ) ? m_pWhitePlayer : m_pBlackPlayer;

	// Get this player's move

	GameState new_state = this_player->myTurn( m_pCurrentGame->GetGameState() );

#if defined( CHESS_DOC_ANALYSIS_ENABLED )

	// Update analysis data

	m_pCurrentGame->m_analysisData.playerAnalysisData = this_player->m_analysisData;
	this_player->ResetAnalysisData();

#endif // defined( CHESS_DOC_ANALYSIS_ENABLED )

	// If it is undo, then back up two turns, otherwise play it
	// The game is backed up two turns because only a human player can undo and
	// only after the computer player has gone.

	if ( new_state.move_.isUndo() )
	{
		if ( m_pCurrentGame->GetGameRecord().size() >= 2 )
		{
			// Undo the last two moves

			m_pCurrentGame->UndoMove();
			m_pCurrentGame->UndoMove();

			// Display the current state of the game

			m_pMainWnd->InvalidateRect( NULL );
			m_pMainWnd->UpdateWindow();
		}
		else
		{
			AfxMessageBox( "The beginning of the game has been reached.", MB_OK|MB_ICONEXCLAMATION );
		}

		// Its the this player's turn again

		BOOL const ok = m_pMainWnd->PostMessage( WM_NEXTTURN, whose_turn, turn-2 );
		ASSERT( ok );
	}
	else
	{
		// Play the move

		m_pCurrentGame->PlayMove( new_state );

		// Display the current state of the game

		m_pMainWnd->InvalidateRect( NULL );
		m_pMainWnd->UpdateWindow();

		// If this player didn't resign, then it's the other player's move

		if ( !new_state.move_.isResignation() )
		{
			BOOL const ok = m_pMainWnd->PostMessage( WM_NEXTTURN, 1 - whose_turn, turn+1 );
			ASSERT( ok );
		}
	}

	return 0;
}
