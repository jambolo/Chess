/** @file *//********************************************************************************************************

                                                     ChessView.h

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Chess/ChessView.h#5 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once


#include "ChessDoc.h"

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

class CChessView : public CView
{
protected: // create from serialization only
	CChessView();
	DECLARE_DYNCREATE( CChessView )

// Attributes
public:
	CChessDoc * GetDocument();
	CChessDoc const * GetDocument() const;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChessView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChessView();

#if defined( _DEBUG )
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext & dc ) const;
#endif // defined( _DEBUG )

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CChessView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	void DrawBoard( CDC * dc, CPoint const & xy ) const;
	void DrawStats( CDC * dc, CPoint const & xy ) const;
	void DrawRecord( CDC * dc, CPoint const & xy ) const;

};



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

#if !defined( _DEBUG )  // debug version in ChessView.cpp

inline CChessDoc * CChessView::GetDocument()
{
	return static_cast< CChessDoc * >( m_pDocument );
}

inline CChessDoc const * CChessView::GetDocument() const
{
	return static_cast< CChessDoc * >( m_pDocument );
}

#endif // !defined( _DEBUG )



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//}}AFX_INSERT_LOCATION
