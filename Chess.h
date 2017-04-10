/** @file *//********************************************************************************************************

                                                       Chess.h

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Chess/Chess.h#7 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once


#include "resource.h"

class CChessDoc;
class Player;
class ComputerPlayer;

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

class CChessApp : public CWinApp
{
public:
	CChessApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChessApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CChessApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	// Handle the next turn
	LRESULT OnNextTurn( WPARAM wParam, LPARAM lParam );

	Player *	m_pWhitePlayer;		// The white player
	Player *	m_pBlackPlayer;		// The black player
	CChessDoc *	m_pCurrentGame;		// The current game

private:

	ComputerPlayer const *	m_pComputerPlayer;	// Keep track of the computer player for analysis
	int						m_maxDepth;
};
