// Figure.cpp: implementation of the CFigure class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "Figure.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CFigure::CFigure(CString fname,UINT WindowLeft, UINT WindowTop,UINT ViewX,UINT ViewY,BOOL bStretch)
{
	CWaitCursor WC;

	m_pFigureFrame = new CFrameWnd;
	m_pFigureView  = new CFigureView;

	CMainFrame * pFrame=(CMainFrame *) AfxGetMainWnd();
	
	RECT r;
	r.left   = WindowLeft;
	r.right  = r.left + ViewX;
	r.top    = WindowTop;
	r.bottom = r.top  + ViewY;
	
	
	if (!m_pFigureFrame->Create(NULL,"Ô­Ê¼Í¼Ïñ",WS_OVERLAPPEDWINDOW,r,pFrame))
	{
		AfxMessageBox("Can't create window frame");
		return ;
	}
	
	// Don't care about size -- RecalcLayout will resize anyway.
	if (!m_pFigureView->Create(NULL,NULL,AFX_WS_DEFAULT_VIEW,CRect(0,0,0,0),m_pFigureFrame,AFX_IDW_PANE_FIRST))
	{
		AfxMessageBox("Can't create debug window view");
		m_pFigureFrame->DestroyWindow();
	}
	// Make sure OnInitialUpdate fires
	m_pFigureFrame->InitialUpdateFrame(NULL,TRUE);
	// Layout frame
	m_pFigureFrame->RecalcLayout();

	m_pFigureView->LoadImage( fname );

	m_pFigureView->SetScrollSizes(MM_TEXT, CSize(m_pFigureView->m_raster_xsize, m_pFigureView->m_raster_ysize) );

	m_pFigureView->m_bStretch=bStretch;

	WC.Restore();
}

CFigure::CFigure(CString fname,UINT WindowLeft, UINT WindowTop,UINT ViewX,UINT ViewY,CString blh, int serialno, BOOL bStretch)
{
	CWaitCursor WC;

	m_pFigureFrame = new CFrameWnd;
	m_pFigureView  = new CFigureView;

	CMainFrame * pFrame=(CMainFrame *) AfxGetMainWnd();
	
	RECT r;
	r.left   = WindowLeft;
	r.right  = r.left + ViewX;
	r.top    = WindowTop;
	r.bottom = r.top  + ViewY;
	
	int TotalNum = 0;
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_IMAGE + " where medicalno = :medicalno";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = blh;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			TotalNum = g_dbcommand.Field(1).asLong();
		}
		
		g_dbconnection.Commit();
	}
	catch(SAException &x)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
		AfxMessageBox((const char*)x.ErrText());
	}

	CString Title;
	Title.Format("²¡ÀýÍ¼Ïñ - %s - %d/%d  [¶à·ùÍ¼ÏñÊ±£¬Çë°´¼üÅÌ¡û¡ú(»ò¡ü¡ý)¼üÇÐ»»]", blh, serialno, TotalNum);
	if (!m_pFigureFrame->Create(NULL,Title,WS_OVERLAPPEDWINDOW,r,pFrame))
	{
		AfxMessageBox("Can't create window frame");
		return ;
	}
	
	// Don't care about size -- RecalcLayout will resize anyway.
	if (!m_pFigureView->Create(NULL,NULL,AFX_WS_DEFAULT_VIEW,CRect(0,0,0,0),m_pFigureFrame,AFX_IDW_PANE_FIRST))
	{
		AfxMessageBox("Can't create debug window view");
		m_pFigureFrame->DestroyWindow();
	}
	// Make sure OnInitialUpdate fires
	m_pFigureFrame->InitialUpdateFrame(NULL,TRUE);
	// Layout frame
	m_pFigureFrame->RecalcLayout();

	m_pFigureView->m_blh = blh;
	m_pFigureView->m_SerialNo = serialno;
	m_pFigureView->m_MaxSerialNo = serialno;

	BOOL rt = m_pFigureView->LoadImage( fname );

	if(rt)
		m_pFigureView->SetScrollSizes(MM_TEXT, CSize(m_pFigureView->m_raster_xsize, m_pFigureView->m_raster_ysize) );
	else
		m_pFigureView->SetScrollSizes(MM_TEXT, CSize(100, 100) );

	m_pFigureView->m_bStretch=bStretch;

	WC.Restore();
}

CFigure::~CFigure()
{
}
