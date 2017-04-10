/** @file *//********************************************************************************************************

                                                   YourTurnDialog.h

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Chess/YourTurnDialog.h#7 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once


#include "resource.h"

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

class CYourTurnDialog : public CDialog
{
// Construction
public:
	CYourTurnDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CYourTurnDialog)
	enum { IDD = IDD_YOURTURN };
	BOOL	m_Resign;
	BOOL	m_Undo;
	BOOL	m_KingSideCastle;
	BOOL	m_QueenSideCastle;
	CString	m_From;
	CString	m_To;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYourTurnDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CYourTurnDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
//}}AFX_INSERT_LOCATION

