#include "stdafx.h"
#include "MyPreviewView.h"
#include "DummyView.h"
#include "PathologyView.h"

IMPLEMENT_DYNCREATE(CMyPreviewView, CPreviewView)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyPreviewView

CMyPreviewView::CMyPreviewView()
{
	m_pView = NULL;
}

CMyPreviewView::~CMyPreviewView()
{
}

BEGIN_MESSAGE_MAP(CMyPreviewView, CPreviewView)
	//{{AFX_MSG_MAP(CMyPreviewView)
	//}}AFX_MSG_MAP
	ON_COMMAND(AFX_ID_PREVIEW_PRINT, OnPreviewPrint)
	ON_COMMAND(AFX_ID_PREVIEW_NEXT, OnNextPage)
 	ON_COMMAND(AFX_ID_PREVIEW_PREV, OnPrevPage)
	ON_COMMAND(AFX_ID_PREVIEW_ZOOMIN, OnZoomIn)
	ON_COMMAND(AFX_ID_PREVIEW_ZOOMOUT, OnZoomOut)
	ON_COMMAND(AFX_ID_PREVIEW_NUMPAGE, OnNumPageChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPreviewView drawing

/////////////////////////////////////////////////////////////////////////////
// CMyPreviewView diagnostics
#ifdef _DEBUG
void CMyPreviewView::AssertValid() const
{
	// Bug nearby Article ID: Q192853 
	//	CPreviewView::AssertValid();
}

void CMyPreviewView::Dump(CDumpContext& dc) const
{
	CPreviewView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyPreviewView message handlers

void CMyPreviewView::OnPreviewPrint()
{
	if (m_pView != NULL)
	{
		m_pView->PostMessage(WM_COMMAND, ID_FILE_PRINT);
	}
	else
	{
		// the view created for the dialog needs to set the m_pDialog pointer
		ASSERT(FALSE);
	}
	OnPreviewClose();               // force close of Preview
}

void CMyPreviewView::SetViewPointer(CPathologyView* pView)
{
	m_pView = pView;
}

void CMyPreviewView::OnNextPage()
{
	CPreviewView::OnNextPage();
	m_pToolBar->OnUpdateCmdUI((CFrameWnd*)AfxGetApp()->m_pMainWnd, TRUE);
}

void CMyPreviewView::OnPrevPage()
{
	CPreviewView::OnPrevPage();
	m_pToolBar->OnUpdateCmdUI((CFrameWnd*)AfxGetApp()->m_pMainWnd, TRUE);
}

void CMyPreviewView::OnZoomIn()
{
	CPreviewView::OnZoomIn();
	m_pToolBar->OnUpdateCmdUI((CFrameWnd*)AfxGetApp()->m_pMainWnd, TRUE);
}

void CMyPreviewView::OnZoomOut()
{
	CPreviewView::OnZoomOut();
	m_pToolBar->OnUpdateCmdUI((CFrameWnd*)AfxGetApp()->m_pMainWnd, TRUE);
}

void CMyPreviewView::OnNumPageChange()
{
	CPreviewView::OnNumPageChange();
	m_pToolBar->OnUpdateCmdUI((CFrameWnd*)AfxGetApp()->m_pMainWnd, TRUE);
}