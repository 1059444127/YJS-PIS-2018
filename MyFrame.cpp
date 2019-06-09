// MyFrame.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "MyFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyFrame

IMPLEMENT_DYNCREATE(CMyFrame, CFrameWnd)
extern CPathologyApp theApp;
CMyFrame::CMyFrame()
{
	old = NULL;
}

CMyFrame::~CMyFrame()
{
}


BEGIN_MESSAGE_MAP(CMyFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMyFrame)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFrame message handlers

void CMyFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(old != NULL)
	{
		theApp.m_pMainWnd = old;
		old->EnableWindow(TRUE);
	}
	CFrameWnd::OnClose();
}

//DEL void CMyFrame::OnLButtonDblClk(UINT nFlags, CPoint point) 
//DEL {
//DEL 	// TODO: Add your message handler code here and/or call default
//DEL 	CRect re;
//DEL 	GetWindowRect(&re);
//DEL 	if(!re.PtInRect(point))
//DEL 	{
//DEL 		AfxMessageBox("out");
//DEL 		CFrameWnd::OnLButtonDblClk(nFlags, re.CenterPoint());
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		AfxMessageBox("in");
//DEL 		CFrameWnd::OnLButtonDblClk(nFlags, point);
//DEL 	}
//DEL 	
//DEL }

//DEL void CMyFrame::OnLButtonDown(UINT nFlags, CPoint point) 
//DEL {
//DEL 	// TODO: Add your message handler code here and/or call default
//DEL 	AfxMessageBox("down");
//DEL 	CFrameWnd::OnLButtonDown(nFlags, point);
//DEL }

//DEL void CMyFrame::OnMouseMove(UINT nFlags, CPoint point) 
//DEL {
//DEL 	// TODO: Add your message handler code here and/or call default
//DEL 	AfxMessageBox("move");
//DEL 	CFrameWnd::OnMouseMove(nFlags, point);
//DEL }
