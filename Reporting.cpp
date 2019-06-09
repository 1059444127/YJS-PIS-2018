// Reporting.cpp: implementation of the CReporting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "MainFrm.h"
#include "Reporting.h"
#include "PathologyView.h"
#include "PathologyDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern CPathologyApp theApp;

CReporting::CReporting(CString MedicalNo)
{
	CWaitCursor WC;

	m_pFormDataModel = new CFormDataModel;
	m_pFormDataModel->SetFormMode(FALSE);
	m_pFormDataModel->SetPagePosition(CRect(0,0,0,0));

	m_pReportingFrame = new CReportingFrame;
	m_pReportingView  = new CReportingView;
	m_pReportingView->SetCurrentModel( m_pFormDataModel );


	CMainFrame * pFrame=(CMainFrame *) AfxGetMainWnd();
		
	CString title = theApp.GetView()->GetDocument()->GenTitleString();
	try{
		//SAString cmdstr = "select xm,xb,nl,nldw,zyh,czys,zzys,shys,bgzt from cmipsmain where medicalno=:medicalno";
		SAString cmdstr = "select bgzt from cmipsmain where medicalno=:medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = MedicalNo;
		g_dbcommand.Execute();
			while(g_dbcommand.FetchNext()){
		/*		title += g_dbcommand.Field("xm").asString() + "  ";
				title += g_dbcommand.Field("xb").asString() + "  ";
				if(!g_dbcommand.Field("nl").asString().IsEmpty())
				{
					title += g_dbcommand.Field("nl").asString();
					title += g_dbcommand.Field("nldw").asString() + "  ";
				}	
				title += g_dbcommand.Field("zyh").asString() + "  ";
				if(g_dbcommand.Field("czys").isNull() && g_dbcommand.Field("zzys").isNull() && g_dbcommand.Field("shys").isNull())
				{
					break;
				}
				title += "诊断医师：";
				if(!g_dbcommand.Field("czys").asString().IsEmpty())
				{
					title += "(初)" + g_dbcommand.Field("czys").asString() + ",";
				}
				if(!g_dbcommand.Field("zzys").asString().IsEmpty())
				{
					title += "(主)" +g_dbcommand.Field("zzys").asString() + ",";
				}
				if(!g_dbcommand.Field("shys").asString().IsEmpty())
				{
					title += "(审)" +g_dbcommand.Field("shys").asString() + ",";
				}
				if(title.GetAt(title.GetLength() - 1) == ',')
				{
					title = title.Left(title.GetLength() - 1);
				}
				*/
				title += " - [报告";
				int bgzt = 0;
				bgzt = g_dbcommand.Field("bgzt").asLong();
				if(bgzt < 1)
				{
					title += "已登记";
				}
				else if(bgzt < 2)
				{
					title += "已取材";
				}
				else if(bgzt < 4)
				{
					title += "已延期";
				}
				else if(bgzt < 8)
				{
					title += "已诊断";
				}
				else if(bgzt < 16)
				{
					title += "已审核";
				}
				else if(bgzt < 32)
				{
					title += "已打印";
				}
				else
				{
					title += "已发送";
				}
			}
			title += "]"; 
		g_dbconnection.Commit();
	}catch(SAException &e){
		AfxMessageBox((const char*)e.ErrText());
	}
	//CRect rect;
	//GetClientRect(pFrame->m_hWnd,&rect);
	if (!m_pReportingFrame->Create(NULL,title,WS_OVERLAPPEDWINDOW,CRect(0,0,0,0),pFrame))
	{
		AfxMessageBox("Can't create window frame");
		return ;
	}

	// Don't care about size -- RecalcLayout will resize anyway.
	if (!m_pReportingView->Create(NULL,NULL,AFX_WS_DEFAULT_VIEW,CRect(0,0,0,0),m_pReportingFrame,AFX_IDW_PANE_FIRST))
	{
		m_pReportingFrame->DestroyWindow();
	}

	m_pReportingView->pMainFrame = pFrame;
	m_pReportingView->IsFrameZoomed = pFrame->IsZoomed();
	m_pReportingView->m_PreBlh   = MedicalNo;
	//pFrame->ShowWindow(SW_HIDE);

	// Make sure OnInitialUpdate fires
	m_pReportingFrame->InitialUpdateFrame(NULL,TRUE);
	

	// Layout frame
	m_pReportingFrame->RecalcLayout();

	///m_pReportingFrame->ShowWindow(SW_SHOWMAXIMIZED);
	m_pReportingFrame->m_wndEditBlh.SetFocus();

	//m_pReportingView->SetScrollSizes(MM_TEXT, CSize(10, 10) );
	if(!MedicalNo.IsEmpty())
	{
		m_pReportingFrame->m_wndEditBlh.SetWindowText(MedicalNo);
		m_pReportingView->PostMessage(WM_COMMAND, ID_REPORT_VIEW);
	}

	WC.Restore();
}

CReporting::~CReporting()
{
/*
	if(m_pFormDataModel != NULL)
	{
		delete m_pFormDataModel;
		m_pFormDataModel = NULL;
	}
*/
}
