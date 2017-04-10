/** @file *//********************************************************************************************************

                                                      ChessDoc.h

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Chess/ChessDoc.h#7 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once


#include "GameState.h"
#include "GameRecordEntry.h"
#include "PieceList.h"
#include "GameTree.h"	// for MAX_DEPTH
#include "Player.h"		// for AnalysisData


#define CHESS_DOC_ANALYSIS_ENABLED

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

class CChessDoc : public CDocument
{
	friend class CChessView;

protected: // create from serialization only
	CChessDoc();
	DECLARE_DYNCREATE( CChessDoc )

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChessDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChessDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext& dc ) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CChessDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	Color GetHumanColor() const					{ return m_HumanColor; }
	GameRecord const & GetGameRecord() const	{ return m_GameRecord; }
	GameState const & GetGameState() const		{ return m_GameState; }

	// Undo the last move
	void UndoMove();

	// Play a move
	void PlayMove( GameState const & new_state );

#if defined( CHESS_DOC_ANALYSIS_ENABLED )

	// Resets all analysis data
	void ResetAnalysisData();

	// Analysis data for the last move

	struct AnalysisData
	{
		Player::AnalysisData	playerAnalysisData;

		void Reset();
	};

	mutable AnalysisData	m_analysisData;

#endif // defined( CHESS_DOC_ANALYSIS_ENABLED )

private:
	void Initialize();

	Color		m_WhoseTurn;	// Whose turn it is
	Color		m_HumanColor;	// Color of the human player (here for convenience)
	GameState	m_GameState;	// The game as it currently stands
	GameRecord	m_GameRecord;	// Game record up to the current state
	PieceList	m_WhitePieces;	// White pieces
	PieceList	m_BlackPieces;	// Black pieces
};



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//}}AFX_INSERT_LOCATION
