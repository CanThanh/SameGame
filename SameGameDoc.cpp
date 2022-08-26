
// SameGameDoc.cpp : implementation of the CSameGameDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SameGame.h"
#endif

#include "SameGameDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSameGameDoc

IMPLEMENT_DYNCREATE(CSameGameDoc, CDocument)

BEGIN_MESSAGE_MAP(CSameGameDoc, CDocument)
END_MESSAGE_MAP()


// CSameGameDoc construction/destruction

CSameGameDoc::CSameGameDoc()
{
	// TODO: add one-time construction code here
	//  There should always be a game board
	m_board = new CSameGameBoard();
}

CSameGameDoc::~CSameGameDoc()
{
	//  Delete the current game board
	delete m_board;
	//  Delete everything from the undo stack
	ClearUndo();
	//  Delete everything from the redo stack
	ClearRedo();
}

BOOL CSameGameDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	//  Set (or reset) the game board
	 //  Set (or reset) the game board
	m_board->SetupBoard();
	//  Clear the undo/redo stacks
	ClearUndo();
	ClearRedo();
	return TRUE;
}

int CSameGameDoc::DeleteBlocks(int row, int col)
{
  //  Save the current board in the undo stack
  m_undo.push(new CSameGameBoard(*m_board));
  //  Empty out the redo stack
  ClearRedo();
  //  Then delete the blocks
  int blocks = m_board->DeleteBlocks(row, col);
  //  Clear the undo stack at the end of a game
  if(m_board->IsGameOver())
    ClearUndo();
  //  Return the number of blocks
  return blocks;
}

void CSameGameDoc::UndoLast()
{
  //  First make sure that there is a move to undo
  if(m_undo.empty())
    return;
  //  Take the current board and put it on the redo
  m_redo.push(m_board);
  //  Take the top undo and make it the current
  m_board = m_undo.top();
  m_undo.pop();
}

bool CSameGameDoc::CanUndo()
{
  //  Can undo if the undo stack isn't empty
  return !m_undo.empty();
}

void CSameGameDoc::RedoLast()
{
  //  First make sure that there is a move to redo
  if(m_redo.empty())
    return;
  //  Take the current board and put it on the undo
  m_undo.push(m_board);
  //  Take the top redo and make it the current
  m_board = m_redo.top();
  m_redo.pop();
}

bool CSameGameDoc::CanRedo()
{
  //  Can redo if the redo stack isn't empty
  return !m_redo.empty();
}

void CSameGameDoc::ClearUndo()
{
  //  Delete everything from the undo stack
  while(!m_undo.empty())
  {
    delete m_undo.top();
    m_undo.pop();
  }
}

void CSameGameDoc::ClearRedo()
{
  //  Delete everything from the redo stack
  while(!m_redo.empty())
  {
    delete m_redo.top();
    m_redo.pop();
  }
}

// CSameGameDoc serialization

void CSameGameDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CSameGameDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CSameGameDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CSameGameDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CSameGameDoc diagnostics

#ifdef _DEBUG
void CSameGameDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSameGameDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSameGameDoc commands
void CSameGameDoc::SetNumColors(int nColors)
{
  //  Set the number of colors first
  m_board->SetNumColors(nColors);
  //  Then reset the game board
  m_board->SetupBoard();
}