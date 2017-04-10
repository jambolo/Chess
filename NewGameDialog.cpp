/** @file *//********************************************************************************************************

                                                  NewGameDialog.cpp

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Chess/NewGameDialog.cpp#5 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "StdAfx.h"

#include "NewGameDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/


CNewGameDialog::CNewGameDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNewGameDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewGameDialog)
	m_HumanColor = -1;
	//}}AFX_DATA_INIT
}




/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void CNewGameDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewGameDialog)
	DDX_Radio(pDX, IDC_HUMAN_WHITE, m_HumanColor);
	//}}AFX_DATA_MAP
}




/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

BEGIN_MESSAGE_MAP(CNewGameDialog, CDialog)
	//{{AFX_MSG_MAP(CNewGameDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
