
// SameGameView.cpp : implementation of the CSameGameView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SameGame.h"
#endif

#include "SameGameDoc.h"
#include "SameGameView.h"
#include "OptionsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSameGameView

IMPLEMENT_DYNCREATE(CSameGameView, CView)

BEGIN_MESSAGE_MAP(CSameGameView, CView)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_LEVEL_3COLORS, &CSameGameView::OnLevel3colors)
	ON_UPDATE_COMMAND_UI(ID_LEVEL_3COLORS, &CSameGameView::OnUpdateLevel3colors)
	ON_COMMAND(ID_LEVEL_4COLORS, &CSameGameView::OnLevel4colors)
	ON_UPDATE_COMMAND_UI(ID_LEVEL_4COLORS, &CSameGameView::OnUpdateLevel4colors)
	ON_COMMAND(ID_LEVEL_5COLORS, &CSameGameView::OnLevel5colors)
	ON_UPDATE_COMMAND_UI(ID_LEVEL_5COLORS, &CSameGameView::OnUpdateLevel5colors)
	ON_COMMAND(ID_LEVEL_6COLORS, &CSameGameView::OnLevel6colors)
	ON_UPDATE_COMMAND_UI(ID_LEVEL_6COLORS, &CSameGameView::OnUpdateLevel6colors)
	ON_COMMAND(ID_LEVEL_7COLORS, &CSameGameView::OnLevel7colors)
	ON_UPDATE_COMMAND_UI(ID_LEVEL_7COLORS, &CSameGameView::OnUpdateLevel7colors)
	ON_COMMAND(ID_SETUP_BLOCKCOUNT, &CSameGameView::OnSetupBlockcount)
	ON_COMMAND(ID_SETUP_BLOCKSIZE, &CSameGameView::OnSetupBlocksize)
	ON_COMMAND(ID_EDIT_REDO32780, &CSameGameView::OnEditRedo32780)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO32780, &CSameGameView::OnUpdateEditRedo32780)
	ON_COMMAND(ID_EDIT_UNDO, &CSameGameView::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CSameGameView::OnUpdateEditUndo)
END_MESSAGE_MAP()

// CSameGameView construction/destruction

CSameGameView::CSameGameView()
{
	// TODO: add construction code here

}

CSameGameView::~CSameGameView()
{
}

BOOL CSameGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSameGameView drawing

void CSameGameView::OnDraw(CDC* pDC)
{
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: add draw code for native data here

	//  Save the current state of the device context
  int nDCSave = pDC->SaveDC();
  //  Get the client rectangle
  CRect rcClient;
  GetClientRect(&rcClient);
  //  Get the background color of the board
  COLORREF clr = pDoc->GetBoardSpace(-1, -1);
  //	Draw the background first
  pDC->FillSolidRect(&rcClient, clr);
  //  Create the brush for drawing
  CBrush br;
  br.CreateStockObject(HOLLOW_BRUSH);
  CBrush* pbrOld = pDC->SelectObject(&br);
  //	Draw the squares
  for(int row = 0; row < pDoc->GetRows(); row++)
  {
    for(int col = 0; col < pDoc->GetColumns(); col++)
    {
      //  Get the color for this board space
      clr = pDoc->GetBoardSpace(row, col);
      //  Calculate the size and position of this space
      CRect rcBlock;
      rcBlock.top = row * pDoc->GetHeight();
      rcBlock.left = col * pDoc->GetWidth();
      rcBlock.right = rcBlock.left + pDoc->GetWidth();
      rcBlock.bottom = rcBlock.top + pDoc->GetHeight();
      //  Fill in the block with the correct color
      pDC->FillSolidRect(&rcBlock, clr);
      //  Draw the block outline
      pDC->Rectangle(&rcBlock);
    }
  }
  //  Restore the device context settings
  pDC->RestoreDC(nDCSave);
  br.DeleteObject();
}


// CSameGameView diagnostics

#ifdef _DEBUG
void CSameGameView::AssertValid() const
{
	CView::AssertValid();
}

void CSameGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSameGameDoc* CSameGameView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSameGameDoc)));
	return (CSameGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CSameGameView message handlers


void CSameGameView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class

	//  Resize the window
	ResizeWindow();
}

void CSameGameView::ResizeWindow()
{
	//  First get a pointer to the document
	 CSameGameDoc* pDoc = GetDocument();
	 ASSERT_VALID(pDoc);
	if(!pDoc)
		return;
	 //  Get the size of the client area and the window
	CRect rcClient, rcWindow;
	GetClientRect(&rcClient);
	GetParentFrame()->GetWindowRect(&rcWindow);
	//  Calculate the difference
	int nWidthDiff = rcWindow.Width() - rcClient.Width();
	int nHeightDiff = rcWindow.Height() - rcClient.Height();
	//  Change the window size based on the size of the game board
	rcWindow.right = rcWindow.left + pDoc->GetWidth() * pDoc->GetColumns() + nWidthDiff;
	rcWindow.bottom = rcWindow.top + pDoc->GetHeight() * pDoc->GetRows() + nHeightDiff;
	//  The MoveWindow function resizes the frame window
	GetParentFrame()->MoveWindow(&rcWindow);
}

void CSameGameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//  First get a pointer to the document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return;
	//  Get the row and column of the block that was clicked on
	int row=point.y/pDoc->GetHeight();
	int col=point.x/pDoc->GetWidth();
	//  Delete the block from the document
	int count=pDoc->DeleteBlocks(row,col);
	//  Check if there were any blocks deleted
	if(count>0)
	{
		//  Force the view to redraw
		Invalidate();
		UpdateWindow();
		//  Check if the game is over
		if(pDoc->IsGameOver())
		{
			//  Get the count remaining
			int remaining =pDoc->GetRemainingCount();
			CString message;
			message.Format(_T("No more moves left\nBlocks is remaining: %d"),remaining);
			// Display the results to the user
			MessageBox(message,_T("Game over"),MB_OK|MB_ICONINFORMATION);
		}
	}

	// TODO: Add your message handler code here and/or call default

	CView::OnLButtonDown(nFlags, point);
}


void CSameGameView::SetColorCount(int nColors)
{
	//  First get a pointer to the document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return;
	//  Set the number of colors
	pDoc->SetNumColors(nColors);
	//  Force the view to redraw
	Invalidate();
	UpdateWindow();
}

void CSameGameView::OnLevel3colors()
{
	SetColorCount(3);
}


void CSameGameView::OnUpdateLevel3colors(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//  First get a pointer to the document
  CSameGameDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if(!pDoc)
    return;
  //  Set the check if this is the right level
  pCmdUI->SetCheck(pDoc->GetNumColors() == 3);
}


void CSameGameView::OnLevel4colors()
{
	// TODO: Add your command handler code here
	SetColorCount(4);
}


void CSameGameView::OnUpdateLevel4colors(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//  First get a pointer to the document
  CSameGameDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if(!pDoc)
    return;
  //  Set the check if this is the right level
  pCmdUI->SetCheck(pDoc->GetNumColors() == 4);
}


void CSameGameView::OnLevel5colors()
{
	// TODO: Add your command handler code here
	SetColorCount(5);
}


void CSameGameView::OnUpdateLevel5colors(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//  First get a pointer to the document
  CSameGameDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if(!pDoc)
    return;
  //  Set the check if this is the right level
  pCmdUI->SetCheck(pDoc->GetNumColors() == 5);
}


void CSameGameView::OnLevel6colors()
{
	// TODO: Add your command handler code here
	SetColorCount(6);
}


void CSameGameView::OnUpdateLevel6colors(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//  First get a pointer to the document
  CSameGameDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if(!pDoc)
    return;
  //  Set the check if this is the right level
  pCmdUI->SetCheck(pDoc->GetNumColors() == 6);
}


void CSameGameView::OnLevel7colors()
{
	// TODO: Add your command handler code here
	SetColorCount(7);
}


void CSameGameView::OnUpdateLevel7colors(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//  First get a pointer to the document
  CSameGameDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if(!pDoc)
    return;
  //  Set the check if this is the right level
  pCmdUI->SetCheck(pDoc->GetNumColors() == 7);
}


void CSameGameView::OnSetupBlockcount()
{
	// TODO: Add your command handler code here
	//  First get a pointer to the document
  CSameGameDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if(!pDoc)
    return;
  //  Create the options dialog
  COptionsDialog dlg(true, this);
  //  Set the row and column values
  dlg.m_nValue1 = pDoc->GetRows();
  dlg.m_nValue2 = pDoc->GetColumns();
  //  Display the dialog
  if(dlg.DoModal() == IDOK)
  {
    //  First delete the board
    pDoc->DeleteBoard();
    //  Get the user selected values
    pDoc->SetRows(dlg.m_nValue1);
    pDoc->SetColumns(dlg.m_nValue2);
    //  Update the board
    pDoc->SetupBoard();
    //  Resize the view
    ResizeWindow();
  }
}


void CSameGameView::OnSetupBlocksize()
{
	// TODO: Add your command handler code here
	//  First get a pointer to the document
  CSameGameDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if(!pDoc)
    return;
  //  Create the options dialog
  COptionsDialog dlg(false, this);
  //  Set the width and height values
  dlg.m_nValue1 = pDoc->GetWidth();
  dlg.m_nValue2 = pDoc->GetHeight();
  //  Display the dialog
  if(dlg.DoModal() == IDOK)
  {
    //  First delete the board
    pDoc->DeleteBoard();
    //  Get the user selected values
    pDoc->SetWidth(dlg.m_nValue1);
    pDoc->SetHeight(dlg.m_nValue2);
    //  Update the board
    pDoc->SetupBoard();
    //  Resize the view
    ResizeWindow();
  }
}


void CSameGameView::OnEditRedo32780()
{
	// TODO: Add your command handler code here
	//  First get a pointer to the document
  CSameGameDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if(!pDoc)
    return;
  //  Call redo on the document
  pDoc->RedoLast();
  //  Force the view to redraw
  Invalidate();
  UpdateWindow();
}


void CSameGameView::OnUpdateEditRedo32780(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//  First get a pointer to the document
  CSameGameDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if(!pDoc)
    return;
  //  Enable option if it is available
  pCmdUI->Enable(pDoc->CanRedo());
}


void CSameGameView::OnEditUndo()
{
	// TODO: Add your command handler code here
	//  First get a pointer to the document
  CSameGameDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if(!pDoc)
    return;
  //  Call undo on the document
  pDoc->UndoLast();
  //  Force the view to redraw
  Invalidate();
  UpdateWindow();
}


void CSameGameView::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//  First get a pointer to the document
  CSameGameDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if(!pDoc)
    return;
  //  Enable option if it is available
  pCmdUI->Enable(pDoc->CanUndo());
}
