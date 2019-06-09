// ReportingFrame.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "ReportingFrame.h"
#include "PathologyView.h"
#include "PathologyDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportingFrame

IMPLEMENT_DYNCREATE(CReportingFrame, CFrameWnd)

CReportingFrame::CReportingFrame()
{
}

CReportingFrame::~CReportingFrame()
{
}


BEGIN_MESSAGE_MAP(CReportingFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CReportingFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportingFrame message handlers

extern CPathologyApp theApp;

int CReportingFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_REPORT))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndEditBlh.Create( WS_CHILD|WS_VISIBLE|ES_LEFT|WS_CLIPCHILDREN | WS_BORDER,
		CRect(0,0,100,19), &m_wndToolBar, ID_REPORT_BLH ))
	{
		TRACE0("Failed to create edit box.\n");
		return -1;      // fail to create
	}
	m_wndEditBlh.SetWindowText("");
	m_wndToolBar.InsertControl(&m_wndEditBlh);

	if (!m_wndComboFormat.Create( WS_CHILD|WS_VISIBLE|CBS_DROPDOWN|WS_CLIPCHILDREN,
		CRect(0,0,200,150), &m_wndToolBar, ID_REPORT_FORMAT ))
	{
		TRACE0("Failed to create combo box.\n");
		return false;      // fail to create
	}
    m_wndToolBar.InsertControl(&m_wndComboFormat, true);
	
	if(!m_selPic.Create("选图", WS_CHILD|WS_VISIBLE,CRect(0,0,48,23),&m_wndToolBar,ID_REPORT_SELPIC)){
		TRACE0("Failed to create button.\n");
		return false;
	}
	m_wndToolBar.InsertControl(&m_selPic);
	m_wndToolBar.AutoSizeToolbar();

	if(!m_CloseWnd.Create("关闭", WS_CHILD|WS_VISIBLE,CRect(0,0,48,23),this,ID_REPORT_CLOSE)){
		TRACE0("Failed to create button.\n");
		return false;
	} 
//	m_wndToolBar.InsertControl(&m_CloseWnd);
	m_CloseWnd.SetWindowPos(&wndTop,780,3,50,23,NULL);
	m_CloseWnd.ShowWindow(TRUE);
	CFont font;
	font.CreateFont(16,0,0,0,400,0,0,0,0,0,0,0,0,"MS Sans Serif");
	m_CloseWnd.SetFont(&font);
	m_selPic.SetFont(&font);
//	m_wndToolBar.AutoSizeToolbar();

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
	return 0;
}

BOOL CReportingFrame::PreTranslateMessage(MSG* pMsg) 
{
	if(this->GetSafeHwnd() && pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN && GetFocus()->m_hWnd == m_wndEditBlh.m_hWnd)
	{
		PostMessage(WM_COMMAND, ID_REPORT_VIEW);
	}
	
	return CFrameWnd::PreTranslateMessage(pMsg);
}

//DEL void CReportingFrame::OnUpdateReportCheck(CCmdUI* pCmdUI) 
//DEL {
//DEL 	// TODO: Add your command update UI handler code here
//DEL 	pCmdUI->Enable( RIGHT_SH(theApp.m_Cpp_Right) && (theApp.m_StationType.CompareNoCase("诊断工作站") == 0 || theApp.m_StationType.CompareNoCase("细胞学工作站") == 0));
//DEL 	
//DEL     if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())
//DEL 	{
//DEL 		pCmdUI->SetCheck(0);
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	theApp.ConnectDatabase();
//DEL 	
//DEL 	int m_bgzt = 0;
//DEL 	try
//DEL 	{
//DEL 		SAString cmdstr = "Select bgzt from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
//DEL 		g_dbcommand.setCommandText( cmdstr );
//DEL 		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
//DEL 		g_dbcommand.Execute();
//DEL 		
//DEL 		if( g_dbcommand.FetchNext() )
//DEL 		{
//DEL 			m_bgzt = g_dbcommand.Field("bgzt").asLong();
//DEL 		}
//DEL 		
//DEL 		g_dbconnection.Commit();
//DEL 	}
//DEL 	catch(SAException &x)
//DEL 	{
//DEL 		try
//DEL 		{
//DEL 			g_dbconnection.Rollback();
//DEL 		}
//DEL 		catch(SAException &)
//DEL 		{
//DEL 		}
//DEL 		AfxMessageBox((const char*)x.ErrText());
//DEL 	}
//DEL 	
//DEL 	if(m_bgzt & 0x08)   
//DEL 		m_bgzt = 1;
//DEL 	else
//DEL 		m_bgzt = 0;
//DEL 	pCmdUI->SetCheck(m_bgzt);
//DEL }

BOOL CReportingFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.x = 0;
	cs.y = 0;

	cs.cx = ::GetSystemMetrics(SM_CXSCREEN);
	cs.cy = ::GetSystemMetrics(SM_CYSCREEN);
	
	return CFrameWnd::PreCreateWindow(cs);
}
