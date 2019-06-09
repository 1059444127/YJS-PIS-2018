#include "stdafx.h"
#include "DlgPreviewView.h"
#include "DlgDummyView.h"
#include "PrintPreviewDialog.h"

IMPLEMENT_DYNCREATE(CDlgPreviewView, CPreviewView)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPreviewView

CDlgPreviewView::CDlgPreviewView()
{
	m_pDialog = NULL;
}

CDlgPreviewView::~CDlgPreviewView()
{
}

BEGIN_MESSAGE_MAP(CDlgPreviewView, CPreviewView)
	//{{AFX_MSG_MAP(CDlgPreviewView)
	//}}AFX_MSG_MAP
	ON_COMMAND(AFX_ID_PREVIEW_PRINT, OnPreviewPrint)
	ON_COMMAND(AFX_ID_PREVIEW_NEXT, OnNextPage)
 	ON_COMMAND(AFX_ID_PREVIEW_PREV, OnPrevPage)
	ON_COMMAND(AFX_ID_PREVIEW_ZOOMIN, OnZoomIn)
	ON_COMMAND(AFX_ID_PREVIEW_ZOOMOUT, OnZoomOut)
	ON_COMMAND(AFX_ID_PREVIEW_NUMPAGE, OnNumPageChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPreviewView drawing

/////////////////////////////////////////////////////////////////////////////
// CDlgPreviewView diagnostics
#ifdef _DEBUG
void CDlgPreviewView::AssertValid() const
{
	// Bug nearby Article ID: Q192853 
	//	CPreviewView::AssertValid();
}

void CDlgPreviewView::Dump(CDumpContext& dc) const
{
	CPreviewView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDlgPreviewView message handlers

void CDlgPreviewView::OnPreviewPrint()
{
	if (m_pDialog != NULL)
	{
		m_pDialog->PostMessage(WM_COMMAND, ID_FILE_PRINT);
	}
	else
	{
		// the view created for the dialog needs to set the m_pDialog pointer
		ASSERT(FALSE);
	}
	OnPreviewClose();               // force close of Preview
}

void CDlgPreviewView::SetDialogPointer(CPrintPreviewDialog* pDialog)
{
	m_pDialog = pDialog;
}

void CDlgPreviewView::OnNextPage()
{
	CPreviewView::OnNextPage();
	m_pToolBar->OnUpdateCmdUI((CFrameWnd*)AfxGetApp()->m_pMainWnd, TRUE);
}

void CDlgPreviewView::OnPrevPage()
{
	CPreviewView::OnPrevPage();
	m_pToolBar->OnUpdateCmdUI((CFrameWnd*)AfxGetApp()->m_pMainWnd, TRUE);
}

void CDlgPreviewView::OnZoomIn()
{
	CPreviewView::OnZoomIn();
	m_pToolBar->OnUpdateCmdUI((CFrameWnd*)AfxGetApp()->m_pMainWnd, TRUE);
}

void CDlgPreviewView::OnZoomOut()
{
	CPreviewView::OnZoomOut();
	m_pToolBar->OnUpdateCmdUI((CFrameWnd*)AfxGetApp()->m_pMainWnd, TRUE);
}

void CDlgPreviewView::OnNumPageChange()
{
	CPreviewView::OnNumPageChange();
	m_pToolBar->OnUpdateCmdUI((CFrameWnd*)AfxGetApp()->m_pMainWnd, TRUE);
}