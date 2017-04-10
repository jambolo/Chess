/** @file *//********************************************************************************************************

                                                  YourTurnDialog.cpp

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Chess/YourTurnDialog.cpp#7 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "StdAfx.h"

#include "YourTurnDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

CYourTurnDialog::CYourTurnDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CYourTurnDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CYourTurnDialog)
	m_Resign = FALSE;
	m_Undo = FALSE;
	m_KingSideCastle = FALSE;
	m_QueenSideCastle = FALSE;
	m_From = _T("");
	m_To = _T("");
	//}}AFX_DATA_INIT
}




/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void CYourTurnDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYourTurnDialog)
	DDX_Check(pDX, IDC_RESIGN, m_Resign);
	DDX_Check(pDX, IDC_UNDO, m_Undo);
	DDX_Check(pDX, IDC_KINGSIDECASTLE, m_KingSideCastle);
	DDX_Check(pDX, IDC_QUEENSIDECASTLE, m_QueenSideCastle);
	DDX_Text(pDX, IDC_FROM, m_From);
	DDV_MaxChars(pDX, m_From, 2);
	DDX_Text(pDX, IDC_TO, m_To);
	DDV_MaxChars(pDX, m_To, 2);
	//}}AFX_DATA_MAP
}




/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

BEGIN_MESSAGE_MAP(CYourTurnDialog, CDialog)
	//{{AFX_MSG_MAP(CYourTurnDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
