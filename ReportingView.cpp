// ReportingView.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "MainFrm.h"
#include "ReportingFrame.h"
#include "ReportingView.h"
#include "FOPictureShape.h"
#include "FOBarCodeShape.h"
#include "FOFieldShape.h"
#include "MedDiagnose.h"
#include "PathologyView.h"
#include "MedImage.h"
#include "PathologyDoc.h"
#include "PathologyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportingView
extern CPathologyApp theApp;

IMPLEMENT_DYNCREATE(CReportingView, CFODrawView)

CReportingView::CReportingView()
{
	pMainFrame = NULL;
	m_PreDBType = -1;
	m_IsPrinted = FALSE;
}

CReportingView::~CReportingView()
{
}


BEGIN_MESSAGE_MAP(CReportingView, CFODrawView)
	//{{AFX_MSG_MAP(CReportingView)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_REPORT_VIEW, OnReportView)
	ON_COMMAND(ID_REPORT_PREVIOUS, OnReportPrevious)
	ON_COMMAND(ID_REPORT_NEXT, OnReportNext)
	ON_CBN_SELCHANGE(ID_REPORT_FORMAT, OnSelchangeComboFormat)
	ON_COMMAND(ID_REPORT_SEND, OnReportSend)
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_REPORT_CHECK, OnReportCheck)
	ON_COMMAND(ID_REPORT_SELPIC, OnReportSelpic)
	ON_COMMAND(ID_REPORT_PRINT, OnReportPrint)
	ON_COMMAND(ID_REPORT_CLOSE, OnReportClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportingView drawing

void CReportingView::OnInitialUpdate()
{
	CFODrawView::OnInitialUpdate();

	CFOFormProperties *pProp = GetCurrentModel()->GetDefaultProperty();
	pProp->ShowMargin(FALSE);
	GetCurrentModel()->NotifyObserver(FO_HINT_UPDATE_WINDOW,NULL);

	m_action_state = State_AutoMoveScroll;
	FOSetCursor(IDC_FO_AUTO_PAN_START);

	SetZoomScale(GetZoomScale() * 100 + 0.5);

	if(theApp.m_StationType.CompareNoCase("诊断工作站") && theApp.m_StationType.CompareNoCase("细胞学工作站")){
		((CReportingFrame*)GetParent())->m_wndComboFormat.EnableWindow(FALSE);
	}
	
}


void CReportingView::OnDraw(CDC* pDC)
{
	Draw(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CReportingView diagnostics

#ifdef _DEBUG
void CReportingView::AssertValid() const
{
	CFODrawView::AssertValid();
}

void CReportingView::Dump(CDumpContext& dc) const
{
	CFODrawView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CReportingView message handlers

void CReportingView::OnDestroy() 
{
	CFODrawView::OnDestroy();

	if(pMainFrame)
	{
		if(IsFrameZoomed) 
			pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
		else
			pMainFrame->ShowWindow(SW_SHOW);

		//if(!m_PreBlh.IsEmpty())
		//{
		//	theApp.OpenDocumentFile(m_PreBlh);
		//}

		if(theApp.m_AutoRefreshDBListMode.CompareNoCase("是") == 0 && ((CMainFrame *)pMainFrame)->m_wndTabDockBar.m_DBList.GetSafeHwnd())
		{
			((CMainFrame *)pMainFrame)->m_wndTabDockBar.m_DBList.m_RecordList.ResetGridSize();
			((CMainFrame *)pMainFrame)->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
			((CMainFrame *)pMainFrame)->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
		}
	}
}

BOOL CReportingView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	////////////////////////////////////////////////////////
	//QZ
	// TRUE if bypassing Print Dialog
	//
	pInfo->m_bDirect = TRUE;
	////////////////////////////////////////////////////////

	//------------------------------------------------------
	CReportingFrame *pFrame = (CReportingFrame *)GetParent();
	CString curno;
	pFrame->m_wndEditBlh.GetWindowText(curno);
	curno.TrimLeft(); curno.TrimRight();
	if(curno.IsEmpty())  return FALSE;

	CString nBdjg, nZdyj, nczys, nzzys, nshys;
	nBdjg.Empty(); nZdyj.Empty(); nczys.Empty(); nzzys.Empty(); nshys.Empty();
	
	try
	{
		SAString cmdstr = "Select bdjg, zdyj, czys, zzys, shys from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = curno;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			if(!g_dbcommand.Field("bdjg").isNull())
			{
				nBdjg = g_dbcommand.Field("bdjg").asString();
			}
			if(!g_dbcommand.Field("zdyj").isNull())
			{
				nZdyj = g_dbcommand.Field("zdyj").asString();
			}
			if(!g_dbcommand.Field("czys").isNull())
			{
				nczys = g_dbcommand.Field("czys").asString();
			}
			if(!g_dbcommand.Field("zzys").isNull())
			{
				nzzys = g_dbcommand.Field("zzys").asString();
			}
			if(!g_dbcommand.Field("shys").isNull())
			{
				nshys = g_dbcommand.Field("shys").asString();
			}
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
	
	if(theApp.m_AuthorPrintOnlyMode.CompareNoCase("打开") == 0)
	{
		bool bFounded = false;
		if(!nczys.IsEmpty() && nczys.Find(theApp.m_Cpp_User) >= 0)   bFounded = true;
		if(!nzzys.IsEmpty() && nzzys.Find(theApp.m_Cpp_User) >= 0)   bFounded = true;
		if(!nshys.IsEmpty() && nshys.Find(theApp.m_Cpp_User) >= 0)   bFounded = true;
		
		if(!bFounded)
		{
			AfxMessageBox("当前报告只有诊断医师自已才能打印！");
			return FALSE;
		}
	}
	
	if(nBdjg.IsEmpty() && nZdyj.IsEmpty() && theApp.m_ZdDyMode.CompareNoCase("是") == 0)
	{
		AfxMessageBox("该病例还未诊断，不能打印！");
		return FALSE;
	}

	if(theApp.m_PrintRestrict.CompareNoCase("否") == 0)
	{
		for(int i = 0; i < 2; i++)
		{
			int m_bgzt = 0;
			CString nZdyj;  nZdyj.Empty();
			try
			{
				SAString cmdstr = "Select bgzt, zdyj from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
				g_dbcommand.setCommandText( cmdstr );
				g_dbcommand.Param("medicalno").setAsString() = curno;
				g_dbcommand.Execute();
				
				if( g_dbcommand.FetchNext() )
				{
					m_bgzt = g_dbcommand.Field("bgzt").asLong();
					nZdyj  = g_dbcommand.Field("zdyj").asString();
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
			
			if(m_bgzt < 8)
			{
				if( i == 0 && RIGHT_SH(theApp.m_Cpp_Right) && (theApp.m_StationType.CompareNoCase("诊断工作站") == 0 || theApp.m_StationType.CompareNoCase("细胞学工作站") == 0))
				{
					if(AfxMessageBox("该病历未审核，必须审核后才能打印，是否审核？",MB_OKCANCEL|MB_ICONQUESTION) == IDCANCEL)  return FALSE;
					//-------------------------------------------------------------------------
					if(m_bgzt < 4)
					{
						AfxMessageBox("该病例还未下诊断意见，不能审核！");
						return FALSE;
					}
					nZdyj.TrimLeft();  nZdyj.TrimRight();
					
					if(nZdyj.IsEmpty())
					{
						AfxMessageBox("诊断意见为空，不能审核！");
						return FALSE;
					}
					
					m_bgzt = (m_bgzt | 0x08);
					
					try
					{
						SAString cmdstr;
						if(theApp.m_ShysMode.CompareNoCase("是") == 0)
							cmdstr = "Update " + theApp.TABLE_MAIN + " set bgzt=:bgzt, shys=:shys where medicalno = :medicalno";
						else
							cmdstr = "Update " + theApp.TABLE_MAIN + " set bgzt=:bgzt where medicalno = :medicalno";
						g_dbcommand.setCommandText(cmdstr);
						
						if(theApp.m_ShysMode.CompareNoCase("是") == 0)
						{
							g_dbcommand.Param("shys").setAsString() = theApp.m_Cpp_User;
						}
						
						g_dbcommand.Param("bgzt").setAsLong() = m_bgzt;
						g_dbcommand.Param("medicalno").setAsString() = curno;
						
						g_dbcommand.Execute();
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
					AfxMessageBox("该病例审核成功，记录已被锁定！");
			
					if(theApp.m_HisCfg.CompareNoCase("通用接口") == 0)
					{
						theApp.NotifyRecordStatus(curno);
					}
					//-------------------------------------------------------------------------
				}
				else
				{
					AfxMessageBox("该病例未审核，不能打印！");
					return FALSE;
				}
			}
			else
			{
				break;
			}
		}
	}
	
	if(!RIGHT_BGDY(theApp.m_Cpp_Right))
	{
		AfxMessageBox("当前用户没有打印报告的权限!");
		return FALSE;
	}
	//------------------------------------------------------
	
	return CFODrawView::OnPreparePrinting(pInfo);
}

void CReportingView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	m_IsPrinted = TRUE;
	CFODrawView::OnBeginPrinting(pDC,pInfo);
}

void CReportingView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	//------------------------------------------------------
	CReportingFrame *pFrame = (CReportingFrame *)GetParent();
	CString curno;
	pFrame->m_wndEditBlh.GetWindowText(curno);
	curno.TrimLeft(); curno.TrimRight();
	if(!curno.IsEmpty())
	{
		int m_bgzt = 0;
		try
		{
			SAString cmdstr = "Select bgzt from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("medicalno").setAsString() = curno;
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				m_bgzt = g_dbcommand.Field("bgzt").asLong();
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
		
		m_bgzt = (m_bgzt | 0x10);
		
		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set bgzt=:bgzt, bgdy=:bgdy where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = curno;
			g_dbcommand.Param("bgzt").setAsLong()        = m_bgzt;
			g_dbcommand.Param("bgdy").setAsString()      = theApp.m_Cpp_User;
			
			g_dbcommand.Execute();
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

		if(theApp.m_HisCfg.CompareNoCase("通用接口") == 0)
		{
			theApp.NotifyRecordStatus(curno);
		}
	}

	if(theApp.m_PrintAndSendMode.CompareNoCase("是") == 0)
	{
		OnReportSend();
	}	
	//------------------------------------------------------

	CFODrawView::OnEndPrinting(pDC,pInfo);
}

void CReportingView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CFODrawView::OnLButtonDown(nFlags, point);
}

void CReportingView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CFODrawView::OnRButtonDown(nFlags, point);
}
  
void CReportingView::OnReportPrevious() 
{
	CString curno;
	CReportingFrame *pFrame = (CReportingFrame *)GetParent();
	pFrame->m_wndEditBlh.GetWindowText(curno);
	CString lastChar;
	if(theApp.m_BlhMode.CompareNoCase("是") == 0)
	{
		lastChar = curno.Right(1);
		curno = curno.Left(curno.GetLength() - 1);
	}
	curno.TrimLeft(); curno.TrimRight();
	CString str, str1;
	
	char strbuf[300];
	int Len=curno.GetLength(),k;
	BYTE ch;
	for(k=(Len-1);k>=0;k--)
	{
		ch = curno.GetAt(k); 
		if(!(ch >= 0x30 && ch <= 0x39))  break;
	}
	k = Len-1-k;
	ltoa(atol(curno.Right(k))-1,strbuf,10);
	str1.Format("%s",strbuf);
	str = curno.Left(Len-k);
	Len = k - str1.GetLength();
	for(k=0;k<Len;k++)  str += "0";
	str += str1;
	
	curno = str;
	//curno += lastChar;
	int num = 0;
	CString mno;mno.Empty();
	try
	{
		SAString cmdstr = "Select medicalno from " + theApp.TABLE_MAIN + " where medicalno like '" + curno + "%' and dbtype = :dbtype";
		g_dbcommand.setCommandText( cmdstr );
		//g_dbcommand.Param("medicalno").setAsString() = curno;
		g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			mno = g_dbcommand.Field(1).asString();
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
	
	if(mno.IsEmpty())
	{
		if( ((CMainFrame*)AfxGetMainWnd())->m_realtime )
		{
			((CMainFrame*)AfxGetMainWnd())->TerminateTimer();
		}
		
		AfxMessageBox("数据库中不存在指定病理编号的登录记录");
		return;
	}
	pFrame->m_wndEditBlh.SetWindowText(mno);

	CString title = "报告浏览：" + theApp.GetView()->GetDocument()->GenTitleString();
	try
	{
		//SAString cmdstr = "select xm,xb,nl,nldw,zyh,czys,zzys,shys,bgzt from cmipsmain where medicalno=:medicalno";
		SAString cmdstr = "select bgzt from cmipsmain where medicalno=:medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = curno;
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
				title += "诊断中";
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
	}
	catch(SAException &e)
	{
		AfxMessageBox((const char*)e.ErrText());
	}
	
	GetParent()->SetWindowText(title);
	theApp.OpenDocumentFile(curno);
	CString fmt;
	theApp.GetView()->LoadReportFormat(fmt);
	try
	{
		SAString cmdstr = "update "+theApp.TABLE_MAIN + " set bggs = '"+fmt+"' where medicalno = '"+curno+"'";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Execute();
		g_dbconnection.Commit();
	}
	catch(SAException &e)
	{
		AfxMessageBox((const char*)e.ErrText());
	}
//	((CMainFrame*)theApp.GetMainWnd())->m_wndTabDockBar.SetActiveView(1);
	OnReportView();
}

void CReportingView::OnReportNext() 
{
	CString curno;
	CReportingFrame *pFrame = (CReportingFrame *)GetParent();
	pFrame->m_wndEditBlh.GetWindowText(curno);

	curno.TrimLeft(); curno.TrimRight();
	CString lastChar;lastChar.Empty();
	if(theApp.m_BlhMode.CompareNoCase("是") == 0)
	{
		lastChar = curno.Right(1);
		curno = curno.Left(curno.GetLength() - 1);
	}
	CString str, str1;
	
	char strbuf[300];
	int Len=curno.GetLength(),k;
	BYTE ch;
	for(k=(Len-1);k>=0;k--)
	{
		ch = curno.GetAt(k); 
		if(!(ch >= 0x30 && ch <= 0x39))  break;
	}
	k = Len-1-k;
	ltoa(atol(curno.Right(k))+1,strbuf,10);
	str1.Format("%s",strbuf);
	str = curno.Left(Len-k);
	Len = k - str1.GetLength();
	for(k=0;k<Len;k++)  str += "0";
	str += str1;
	
	curno = str;
	//curno += lastChar;

	int num = 0;
	CString mno;mno.Empty();
	try
	{
		SAString cmdstr = "Select medicalno from " + theApp.TABLE_MAIN + " where medicalno like '" + curno + "%' and dbtype = :dbtype";
		g_dbcommand.setCommandText( cmdstr );
		//g_dbcommand.Param("medicalno").setAsString() = curno;
		g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			mno = g_dbcommand.Field(1).asString();
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
	
	if(mno.IsEmpty())
	{
		if( ((CMainFrame*)AfxGetMainWnd())->m_realtime )
		{
			((CMainFrame*)AfxGetMainWnd())->TerminateTimer();
		}
		
		AfxMessageBox("数据库中不存在指定病理编号的登录记录");
		return;
	}
	

	pFrame->m_wndEditBlh.SetWindowText(mno);
	
	CString title = theApp.GetView()->GetDocument()->GenTitleString();
	try{
		//SAString cmdstr = "select xm,xb,nl,nldw,zyh,czys,zzys,shys,bgzt from cmipsmain where medicalno=:medicalno";
		SAString cmdstr = "select bgzt from cmipsmain where medicalno=:medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = curno;
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
				title += "诊断中";
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
	
	GetParent()->SetWindowText(title);
	theApp.OpenDocumentFile(curno);
	CString fmt;
	theApp.GetView()->LoadReportFormat(fmt);
	try{
		SAString cmdstr = "update "+theApp.TABLE_MAIN + " set bggs = '"+fmt+"' where medicalno = '"+curno+"'";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Execute();
		g_dbconnection.Commit();
	}catch(SAException &e){
		AfxMessageBox((const char*)e.ErrText());
	}
	//GetParent()->GetDlgItem(ID_REPORT_FORMAT)->SetWindowText(fmt);
//	((CMainFrame*)theApp.GetMainWnd())->m_wndTabDockBar.SetActiveView(1);
	OnReportView();
}

void CReportingView::OnReportView() 
{
	GetCurrentModel()->ResetContent();

	CString curno;
	CReportingFrame *pFrame = (CReportingFrame *)GetParent();
	pFrame->m_wndEditBlh.GetWindowText(curno);
	curno.TrimLeft(); curno.TrimRight();

	CString title = theApp.GetView()->GetDocument()->GenTitleString();

	try{
		//SAString cmdstr = "select xm,xb,nl,nldw,zyh,czys,zzys,shys,bgzt from cmipsmain where medicalno=:medicalno";
		SAString cmdstr = "select bgzt from cmipsmain where medicalno=:medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = curno;
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
				title += "诊断中";
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
	}
	catch(SAException &e)
	{
		AfxMessageBox((const char*)e.ErrText());
	}
	
	GetParent()->SetWindowText(title);
	theApp.OpenDocumentFile(curno);

	CString Rptname;  Rptname.Empty();

	int pass = 0, m_DbType = -1;
	
	if(!curno.IsEmpty())
	{
		try
		{
			SAString cmdstr = "Select bggs, dbtype from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = curno;
			
			g_dbcommand.Execute();
			
			if(g_dbcommand.FetchNext())
			{
				Rptname  = g_dbcommand.Field("bggs").asString();
				m_DbType = g_dbcommand.Field("dbtype").asLong();
				pass++;
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
	}

	if(!Rptname.IsEmpty()) 
	{
		try
		{
			g_dbcommand.setCommandText("Select rpt_data from REPORTFORMAT where rpt_name = :rpt_name");
			g_dbcommand.Param("rpt_name").setAsString() = Rptname;
			
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				SAString ss = g_dbcommand.Field("rpt_data").asBLob();
				
				CFile file;
				CFileException fe;
				
				if (file.Open( theApp.m_SysTmpPath + Rptname + ".rpt" , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
				{
					const void * binbuf = (const void *)ss;
					file.WriteHuge( binbuf, ss.GetBinaryLength() );
					file.Close();
				}
				
				if( GetCurrentModel()->OpenDocument(theApp.m_SysTmpPath + Rptname + ".rpt") == FALSE )
				{
					return;
				}
				remove(theApp.m_SysTmpPath + Rptname + ".rpt");

				pass++;
			}
			else
			{
				return;
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
			return;
		}
	}

	if(pass == 2)
	{
		CDC* pDC = GetDC();
		CStringArray  m_Local_Name_List;
		CUIntArray    m_Local_Value_List;

		m_Local_Name_List.RemoveAll();
		m_Local_Value_List.RemoveAll();
		
		CFODrawShapeList *pShapeList = GetCurrentModel()->GetFormObjects();
		POSITION pos = pShapeList->GetHeadPosition();
		while ( pos )
		{
			CFODrawShape *pShape = pShapeList->GetNext(pos);
			if( pShape == NULL )   continue;
			if( pShape->GetType() != FO_COMP_PICTURE )  continue;
			m_Local_Name_List.Add( pShape->GetObjectName() );
			m_Local_Value_List.Add(0);
		}
		
		//-----------------------------------------------------------
		int count = 0;
		try
		{
			SAString cmdstr = "Select count(*) from " + theApp.TABLE_IMAGE + " where medicalno = :medicalno and printflag = 1";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = curno;
			
			g_dbcommand.Execute();
			if(g_dbcommand.FetchNext())
			{
				count = g_dbcommand.Field(1).asLong();
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
		
		SAString cmdstr;
		if(count >= m_Local_Name_List.GetSize())
			cmdstr = "Select serialno from " + theApp.TABLE_IMAGE + " where medicalno = :medicalno and printflag = 1 order by serialno asc";
		else
			cmdstr = "Select serialno from " + theApp.TABLE_IMAGE + " where medicalno = :medicalno order by serialno asc";
		
		try
		{
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = curno;
			
			g_dbcommand.Execute();
			int k = 0;
			while(g_dbcommand.FetchNext())
			{
				if(k >= m_Local_Name_List.GetSize())  break;
				m_Local_Value_List.SetAt(k, g_dbcommand.Field("serialno").asLong());
				k++;
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
		//-----------------------------------------------------------

		extern CRect SizeRectWithConstantAspectRatio(CRect* rcScreen,  CSize  sizePicture,  BOOL   bCenter);
		
		//-------------------------------------------------------------
		COleDateTime ServerTime = theApp.GetOleServerDateTime();
		CStringArray Charge_Item_List, Charge_Title_List;
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_CHARGE + " where medicalno = :medicalno";
			
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = curno;
			
			g_dbcommand.Execute();
			int i = 0;
			while( g_dbcommand.FetchNext() )
			{
				CString str;
				
				str.Format("%s", g_dbcommand.Field("item").asString());
				Charge_Item_List.Add(str);
				
				str.Format("sf_%d_1", i+1);
				Charge_Title_List.Add(str);
				
				str.Format("%.2f", g_dbcommand.Field("price").asDouble());
				Charge_Item_List.Add(str);
				
				str.Format("sf_%d_2", i+1);
				Charge_Title_List.Add(str);
				
				i++;
			}
			
			g_dbconnection.Commit();
		}
		catch(SAException &)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			//AfxMessageBox((const char*)x.ErrText());
		}

		CStringArray Qcmx_Item_List, Qcmx_Title_List;
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno";
			
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = curno;
			
			g_dbcommand.Execute();
			int i = 0;
			while( g_dbcommand.FetchNext() )
			{
				CString str;
				
				Qcmx_Item_List.Add(g_dbcommand.Field("xh").asString());
				
				str.Format("QCMX_%d_1", i+1);
				Qcmx_Title_List.Add(str);
				
				Qcmx_Item_List.Add(g_dbcommand.Field("qcbw").asString());
				
				str.Format("QCMX_%d_2", i+1);
				Qcmx_Title_List.Add(str);

				str.Format("%d", g_dbcommand.Field("cks").asLong());
				Qcmx_Item_List.Add(str);
				
				str.Format("QCMX_%d_3", i+1);
				Qcmx_Title_List.Add(str);

				Qcmx_Item_List.Add(g_dbcommand.Field("qcys").asString());
				
				str.Format("QCMX_%d_4", i+1);
				Qcmx_Title_List.Add(str);

				SADateTime st = g_dbcommand.Field("qcrq").asDateTime();
				COleDateTime st1;
				st1.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
				Qcmx_Item_List.Add(st1.Format("%Y-%m-%d %H:%M"));
				
				str.Format("QCMX_%d_5", i+1);
				Qcmx_Title_List.Add(str);
				
				i++;
			}
			
			g_dbconnection.Commit();
		}
		catch(SAException &)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			//AfxMessageBox((const char*)x.ErrText());
		}
		//-------------------------------------------------------------
		
		
		pShapeList = GetCurrentModel()->GetFormObjects();
		pos = pShapeList->GetHeadPosition();
		while ( pos )
		{
			CFODrawShape *pShape = pShapeList->GetNext(pos);
			if( pShape == NULL )   continue;
			UINT ObjType = pShape->GetType();
			switch( ObjType ) {
			case FO_COMP_FIELD:
				{
					CString value;  value.Empty();
					
					bool rt = theApp.GetDatabaseValue(curno,pShape->GetObjectName(),value,true);

					if(rt && theApp.m_StringFormatMode.CompareNoCase("是") == 0 && (pShape->GetObjectName().CompareNoCase("rysj") == 0 || pShape->GetObjectName().CompareNoCase("gjsj") == 0 || pShape->GetObjectName().CompareNoCase("zdyj") == 0))
					{
						value = "    " + value;
					}
					
					if(rt == false)
					{
						CString imgtitle = pShape->GetObjectCaption();
						int imgno = 0;
						for(int i = 0; i < m_Local_Name_List.GetSize(); i++ )		
						{
							if( imgtitle.CompareNoCase(m_Local_Name_List.GetAt(i)) == 0 )
							{
								imgno = m_Local_Value_List.GetAt(i);
								break;
							}
						}
						if(i < m_Local_Name_List.GetSize() && imgno > 0)
						{
							value.Empty();
							
							try
							{
								SAString cmdstr = "Select imgnote from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno and serialno = :serialno";
								g_dbcommand.setCommandText(cmdstr);
								g_dbcommand.Param("medicalno").setAsString() = curno;
								g_dbcommand.Param("serialno").setAsLong()    = imgno;
								
								g_dbcommand.Execute();
								if(g_dbcommand.FetchNext())
								{
									value = g_dbcommand.Field("imgnote").asString();
									rt = true;
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
						}
					}
					
					if(rt == false)
					{
						for(int i = 0; i < Charge_Title_List.GetSize(); i++)
						{
							if(Charge_Title_List.GetAt(i).CompareNoCase(pShape->GetObjectCaption()) == 0)  break;
						}
						
						if(i < Charge_Title_List.GetSize())
						{
							value = Charge_Item_List.GetAt(i);
							
							rt = true;
						}
					}

					if(rt == false)
					{
						for(int i = 0; i < Qcmx_Title_List.GetSize(); i++)
						{
							if(Qcmx_Title_List.GetAt(i).CompareNoCase(pShape->GetObjectCaption()) == 0)  break;
						}
						
						if(i < Qcmx_Title_List.GetSize())
						{
							value = Qcmx_Item_List.GetAt(i);
							
							rt = true;
						}
					}

					if(rt == false)
					{
						if(pShape->GetObjectCaption().CompareNoCase("当前时间") == 0)
						{
							COleDateTime curtime = ServerTime;
							value = curtime.Format("%Y-%m-%d %H:%M");

							rt = true;
						}
						if(pShape->GetObjectCaption().CompareNoCase("诊断医师") == 0)
						{
							value.Empty();
							CString shys, zzys, czys;
							theApp.GetDatabaseValue(curno,"shys",shys,true);
							theApp.GetDatabaseValue(curno,"zzys",zzys,true);
							theApp.GetDatabaseValue(curno,"czys",czys,true);
							//-----------------------
							if(czys == zzys || czys == shys)  czys.Empty();
							if(zzys == shys)  zzys.Empty();
							//-----------------------
							if(!shys.IsEmpty())
							{
								if(!value.IsEmpty()) value += "、";
								value += shys;
							}
							if(!zzys.IsEmpty())
							{
								if(!value.IsEmpty()) value += "、";
								value += zzys;
							}
							if(!czys.IsEmpty())
							{
								if(!value.IsEmpty()) value += "、";
								value += czys;
							}

							if(!value.IsEmpty())
							{
								rt = true;
							}
						}

						if(pShape->GetObjectCaption().CompareNoCase("切片数") == 0)
						{
							try
							{
								SAString cmdstr = "select count(*) from " + theApp.TABLE_SLICE + " where medicalno=:medicalno";
								g_dbcommand.setCommandText(cmdstr);
								g_dbcommand.Param("medicalno").setAsString() = curno;
								g_dbcommand.Execute();
								while(g_dbcommand.FetchNext())
								{
									value.Format("%d",g_dbcommand.Field(1).asLong());
								}
								g_dbconnection.Commit();
							}
							catch(SAException &e)
							{
								AfxMessageBox((const char*)e.ErrText());
							}
							if(!value.IsEmpty())
							{
								rt = true;
							}
						}

						if(pShape->GetObjectCaption().CompareNoCase("特检医嘱") == 0)
						{
							value.Empty();
							CString temp;
							try
							{
								SAString cmdstr = "select yzlx,count(*) from " + theApp.TABLE_ADVICE + " where medicalno=:medicalno and yzlx in('免疫组化','特殊染色','分子病理','电镜','其它') group by yzlx";
								g_dbcommand.setCommandText(cmdstr);
								g_dbcommand.Param("medicalno").setAsString() = curno;
								g_dbcommand.Execute();
								while(g_dbcommand.FetchNext())
								{
									value += g_dbcommand.Field(1).asString();
									temp.Format("%d项、",g_dbcommand.Field(2).asLong());
									value += temp;
								}
								g_dbconnection.Commit();
							}
							catch(SAException &e)
							{
								AfxMessageBox((const char*)e.ErrText());
							}
							if(!value.IsEmpty())
							{
								rt = true;
							}
						}

						if(pShape->GetObjectCaption().CompareNoCase("技术医嘱") == 0)
						{
							value.Empty();
							CString temp;
							try
							{
								SAString cmdstr = "select yzlx,count(*) from " + theApp.TABLE_ADVICE + " where medicalno=:medicalno and yzlx in('常规','补取','重切','深切','薄切','连切','切全','不削连切') group by yzlx";
								g_dbcommand.setCommandText(cmdstr);
								g_dbcommand.Param("medicalno").setAsString() = curno;
								g_dbcommand.Execute();
								while(g_dbcommand.FetchNext())
								{
									value += g_dbcommand.Field(1).asString();
									temp.Format("%d项、",g_dbcommand.Field(2).asLong());
									value += temp;
								}
								g_dbconnection.Commit();
							}
							catch(SAException &e)
							{
								AfxMessageBox((const char*)e.ErrText());
							}
							if(!value.IsEmpty())
							{
								rt = true;
							}
						}

						if(pShape->GetObjectCaption().CompareNoCase("冰冻制片完成时间") == 0)
						{
							value.Empty();
							SADateTime time;
							SADateTime curTime = theApp.GetServerDateTime();
							CString curTimeStr,temp;
							curTimeStr.Format("%d-%.2d-%.2d",curTime.GetYear(),curTime.GetMonth(),curTime.GetDay());

							try
							{
								SAString cmdstr;
								if(theApp.m_DBType.CompareNoCase("oracle") == 0)
								{
									cmdstr = "select distinct hdrq from " + theApp.TABLE_SLICE + " where to_char(hdrq,'yyyy-mm-dd') = '" +  curTimeStr + "' and medicalno='" + curno + "'";	
								}
								else
								{
									cmdstr = "select distinct hdrq from " + theApp.TABLE_SLICE + " where replace(convert(char,hdrq,111),'/','-') = '" + curTimeStr + "' and medicalno='" + curno + "'";
								}
								//SAString cmdstr = "select hdrq from " + theApp.TABLE_SLICE + " where medicalno=:medicalno and hdrq is not null and rwly = '冰冻' order by hdrq desc";
								g_dbcommand.setCommandText(cmdstr);
							//	AfxMessageBox(cmdstr);
							//	g_dbcommand.Param("medicalno").setAsString() = curno;
								g_dbcommand.Execute();
								while(g_dbcommand.FetchNext())
								{
									time = g_dbcommand.Field(1).asDateTime();
									temp.Format("%.2d:%.2d",time.GetHour(),time.GetMinute());
									value += temp;
									value += ",";
								}
								g_dbconnection.Commit();
							}
							catch (SAException& e)
							{
								AfxMessageBox((const char*)e.ErrText());
							}
							if(!value.IsEmpty())
							{
								rt = true;
							}
						}
					}
					
					if( rt )
					{
						CSize sz = ((CFOFieldShape *)pShape)->GenTextSize(pDC,value);
						CRect ShapeRect = pShape->GetRect();
						if(sz.cx > ShapeRect.Width())
						{
							CSize minsize = ((CFOFieldShape *)pShape)->GenTextSize(pDC,"明");
							CString newstr,newline;
							newstr.Empty();  newline.Empty();
							char enter[3];
							enter[0] = 0x0d; enter[1]= 0x0a; enter[2] = 0x00;
							
							while(!value.IsEmpty())
							{
								if((((CFOFieldShape *)pShape)->GenTextSize(pDC,newline).cx + 2*minsize.cx ) > ShapeRect.Width() )
								{
									BYTE ch = newline.GetAt(newline.GetLength() -1);
									if((ch&0x80) != 0 )
									{
										int k = 0;
										for(int i = (newline.GetLength() -1); i >=0; i-- )
										{
											ch = newline.GetAt(i);
											if((ch&0x80) == 0 )   break;
											k++;
										}
										if((k%2) != 0 )
										{
											value = newline.Right(1) + value;
											newline = newline.Left(newline.GetLength() - 1);
										}
									}
									newline += enter;
									newstr += newline;
									newline.Empty();
								}
								else
								{
									newline += value.Left(1);
									value = value.Right(value.GetLength() - 1);
								}
							}
							if(!newline.IsEmpty())
							{
								newstr += newline;
								newline.Empty();
							}
							pShape->SetObjectCaption( newstr );
						}
						else
						{
							pShape->SetObjectCaption( value );
						}
					}
					else
					{
						pShape->SetVisible(FALSE);
					}
					((CFOFieldShape *)pShape)->SetAutoSize(FALSE);
				}
				break;
		    case FO_COMP_PICTURE:
				{
					int imgno = 0;
					CString name = pShape->GetObjectName();
					for(int i = 0; i < m_Local_Name_List.GetSize(); i++ )		
					{
						if( name.CompareNoCase(m_Local_Name_List.GetAt(i)) == 0 )
						{
							imgno = m_Local_Value_List.GetAt(i);
							break;
						}
					}
					
					if(i < m_Local_Name_List.GetSize() && imgno > 0)
					{
						bool ImgLoaded = false;
						try
						{
							SAString cmdstr = "Select imgdataset from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno and serialno = :serialno";
							g_dbcommand.setCommandText(cmdstr);
							g_dbcommand.Param("medicalno").setAsString() = curno;
							g_dbcommand.Param("serialno").setAsLong()    = imgno;
							
							g_dbcommand.Execute();
							if(g_dbcommand.FetchNext())
							{
								SAString Bstr = g_dbcommand.Field("imgdataset").asBLob();
								if( !Bstr.IsEmpty() )
								{
									const void * binbuf = (const void *)Bstr;
									BYTE * dataptr = (BYTE *)binbuf;
									
									CString transname;
									transname.Format("%s_%d_cmipstrans.dat", curno, imgno);
									
									CFile file;
									CFileException fe;
									
									CString fstr = theApp.m_SysTmpPath;
									fstr += transname;
									remove(fstr);
									if (file.Open( fstr , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
									{
										file.WriteHuge( dataptr , Bstr.GetLength());
										file.Close();
									}
									
									//----------------------------------
									ImgLoaded = true;
									
									LPPOINT lpShapePoints = pShape->GetPoints();
									CRect rcTemp;
									rcTemp = CRect(lpShapePoints[0],lpShapePoints[2]);
									rcTemp.NormalizeRect();
									
									CString strText = pShape->GetObjectCaption();
									CSize  sizeText = ((CFOPictureShape *)pShape)->GetTextSize(pDC,rcTemp,strText);
									
									rcTemp.bottom -= sizeText.cy;
									int PIC_WIDTH  = rcTemp.Width();
									int PIC_HEIGHT = rcTemp.Height();
									
									HDIB hDIB = ::CreateDIBFile( PIC_WIDTH, PIC_HEIGHT, 24 );
									LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
									BYTE *   lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
									int LineByteWidth = WIDTHBYTES( PIC_WIDTH * 24 );
									memset( lpDIBBits, 255, LineByteWidth * PIC_HEIGHT );
									
									GDALDataset * m_pDataset = (GDALDataset *) GDALOpen ( fstr , GA_ReadOnly );
									
									if( m_pDataset != NULL )
									{
										BYTE *pRBits = lpDIBBits + 2;
										BYTE *pGBits = lpDIBBits + 1;
										BYTE *pBBits = lpDIBBits + 0;
										
										int BandNo[3];
										int BandNum = m_pDataset->GetRasterCount();
										if( BandNum == 1)
										{
											BandNo[0] = 1;	BandNo[1] = 1;	BandNo[2] = 1;
										}
										else
										{
											BandNo[0] = 1;	BandNo[1] = 2;	BandNo[2] = 3;
										}
										
										CRect ScreenWnd;
										ScreenWnd.SetRect( 0 , 0 ,  PIC_WIDTH  , PIC_HEIGHT );
										CSize sizePicture;
										sizePicture.cx = m_pDataset->GetRasterXSize();
										sizePicture.cy = m_pDataset->GetRasterYSize();
										
										if(theApp.m_PrintingMode1.CompareNoCase("等比例输出") == 0)
										{
											ScreenWnd = SizeRectWithConstantAspectRatio(&ScreenWnd,  sizePicture,  TRUE);
										}
										
										m_pDataset->GetRasterBand(BandNo[0])->RasterIO( GF_Read, 0, 0, 
											m_pDataset->GetRasterXSize(), 
											m_pDataset->GetRasterYSize(), 
											(void *)(pRBits + ScreenWnd.top * LineByteWidth + ScreenWnd.left * 3), 
											ScreenWnd.Width() , ScreenWnd.Height() , 
											GDT_Byte, 
											3 , LineByteWidth );
										m_pDataset->GetRasterBand(BandNo[1])->RasterIO( GF_Read, 0, 0, 
											m_pDataset->GetRasterXSize(), 
											m_pDataset->GetRasterYSize(), 
											(void *)(pGBits + ScreenWnd.top * LineByteWidth + ScreenWnd.left * 3), 
											ScreenWnd.Width() , ScreenWnd.Height() , 
											GDT_Byte, 
											3 , LineByteWidth );
										m_pDataset->GetRasterBand(BandNo[2])->RasterIO( GF_Read, 0, 0, 
											m_pDataset->GetRasterXSize(), 
											m_pDataset->GetRasterYSize(), 
											(void *)(pBBits + ScreenWnd.top * LineByteWidth + ScreenWnd.left * 3), 
											ScreenWnd.Width() , ScreenWnd.Height() , 
											GDT_Byte, 
											3 , LineByteWidth );
										GDALClose(m_pDataset);
										
										//--------------------------------------------------------------
										if(theApp.m_PrintingMode.CompareNoCase("高反差") == 0 || theApp.m_PrintingMode.CompareNoCase("普通") == 0)
										{
											long hist[257];
											for(i = 0; i < 256; i++)   hist[i] = 0;
											for (i = 10; i < (PIC_HEIGHT-10); i++)
											{       
												BYTE *ptr = lpDIBBits + i * LineByteWidth;
												for (int j = 10; j < (PIC_WIDTH-10)*3; j++) 
												{
													int grey = *ptr++;
													hist[grey]++;
												}
											}
											hist[256] = (long)(PIC_WIDTH-20) * (PIC_HEIGHT-20) * 3;
											int sum=0, low=0, high=255;
											for (i = 0; i < 256; i++)	
											{
												sum += hist[i];
												if ((sum*1000/hist[256]>1) && (low==0)) low=i;
												if (sum*1000/hist[256]<999) high=i;
											}
											int Tab[256];    
											for (i = 0; i < 256; i++)
											{
												int grey;
												if(theApp.m_PrintingMode.CompareNoCase("高反差") == 0)
												{
													grey=(int)(((long)i-(long)low)*255l/(long)(high-low));       
													if (grey<0) grey=0;if (grey>255) grey=255;
													Tab[i]=grey;
												}
												if(theApp.m_PrintingMode.CompareNoCase("普通") == 0)
												{
													grey=(int)(((long)i-(long)low)*215l/(long)(high-low))+20;       
													if (grey<0) grey=0;if (grey>255) grey=255;
													Tab[i]=grey;
												}
											}
											for (i=0;i<PIC_HEIGHT;i++)
											{  
												BYTE *ptr = lpDIBBits + i * LineByteWidth;
												for (int j=0;j<PIC_WIDTH*3;j++) *ptr++=Tab[*ptr];
											}
										}

										if(theApp.m_PrintingMode.CompareNoCase("RGB调色") == 0)
										{
											int rvalue = atoi(theApp.m_RedValue);
											int gvalue = atoi(theApp.m_GreenValue);
											int bvalue = atoi(theApp.m_BlueValue);
											for (i=ScreenWnd.top; i<ScreenWnd.bottom;i++)
											{  
												BYTE *ptr = lpDIBBits + i * LineByteWidth;
												for (int j=ScreenWnd.left;j<ScreenWnd.right;j++)
												{
													int mm = ptr[j * 3 + 2];
													mm += rvalue;
													if(mm < 0)   mm = 0;
													if(mm > 255) mm = 255;
													ptr[j * 3 + 2] = mm;

													mm = ptr[j * 3 + 1];
													mm += gvalue;
													if(mm < 0)   mm = 0;
													if(mm > 255) mm = 255;
													ptr[j * 3 + 1] = mm;

													mm = ptr[j * 3 + 0];
													mm += bvalue;
													if(mm < 0)   mm = 0;
													if(mm > 255) mm = 255;
													ptr[j * 3 + 0] = mm;
												}
											}
										}
										//--------------------------------------------------------------
									}
									
									BYTE *buf = new BYTE[LineByteWidth];
									BYTE *u1,*u2;
									for(int j = 0; j < (PIC_HEIGHT/2); j++ )
									{
										u1 = lpDIBBits + j * LineByteWidth;
										u2 = lpDIBBits + (PIC_HEIGHT - j - 1) * LineByteWidth;
										memcpy(buf, u1,  LineByteWidth);
										memcpy(u1,  u2,  LineByteWidth);
										memcpy(u2,  buf, LineByteWidth);
									}
									delete buf;
									
									::GlobalUnlock((HGLOBAL) hDIB);
									
									strText = theApp.m_SysTmpPath;
									strText += "00002.bmp";
									remove(strText);
									
									if ( file.Open( strText, CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
									{
										::SaveDIB( hDIB, file );
										file.Close();
									}
									::GlobalFree((HGLOBAL) hDIB);
									
									if( ((CFOPictureShape *)pShape)->MingLoadImage(strText) == FALSE )
									{
										((CFOPictureShape *)pShape)->SetVisible(FALSE);
									}
									remove(strText);
									//----------------------------------
									
									remove(fstr);
								} 
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
					
						if(!ImgLoaded)
						{
							((CFOPictureShape *)pShape)->SetVisible(FALSE);
						}
					}
					else
					{
						((CFOPictureShape *)pShape)->SetVisible(FALSE);
					}
				}
			    break;
		    case FO_COMP_BARCODE:
				{
					CString value;
					bool rt = theApp.GetDatabaseValue(curno,pShape->GetObjectName(),value);
					if( rt )
					{
						((CFOBarCodeShape *)pShape)->SetObjectCaption(value);
					}
				}
				break;
			default:;
			}
		}
	
		m_Local_Name_List.RemoveAll();
		m_Local_Value_List.RemoveAll();
		
		Charge_Item_List.RemoveAll();
		Charge_Title_List.RemoveAll();

		Qcmx_Item_List.RemoveAll();
		Qcmx_Title_List.RemoveAll();

		ReleaseDC(pDC);
	}
	//------------------------------------------------
    if(m_PreDBType != m_DbType)
	{
		m_PreDBType = m_DbType;
		pFrame->m_wndComboFormat.ResetContent();
		
		CStringArray bdlist; bdlist.RemoveAll();
		CStdioFile File;
		if ( File.Open(theApp.m_Exe_Working_Dir + "system\\表单.txt", CFile::modeRead | CFile::typeText) )
		{
			CString tmp;
			while(1)
			{
				if( File.ReadString(tmp) == FALSE ) break;
				tmp.TrimLeft(); tmp.TrimRight();
				if(!tmp.IsEmpty())
				{
					bdlist.Add(tmp);
				}
			}
			File.Close();
		}
		
		try
		{
			g_dbcommand.setCommandText("Select rpt_name from REPORTFORMAT where dbtype = :dbtype");
			g_dbcommand.Param("dbtype").setAsLong() = m_DbType;
			g_dbcommand.Execute();
			
			while( g_dbcommand.FetchNext() )
			{
				CString str = g_dbcommand.Field("rpt_name").asString();
				if(str.CompareNoCase("制片条码") == 0 || str.CompareNoCase("回执单") == 0 || str.CompareNoCase("冰冻报告") == 0 || str.CompareNoCase("材块移交表") == 0 || str.CompareNoCase("切片移交表") == 0 || str.CompareNoCase("医嘱工作单") == 0 || str.CompareNoCase("制片情况统计表") == 0)  continue;
				
				if(bdlist.GetSize() > 0)
				{
					for(int nx = 0; nx < bdlist.GetSize(); nx++)
					{
						if(bdlist.GetAt(nx).CompareNoCase(str) == 0)  break;
					}
					if(nx < bdlist.GetSize())  continue;
				}
				
				pFrame->m_wndComboFormat.AddString( g_dbcommand.Field("rpt_name").asString() );
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
		
		bdlist.RemoveAll();
	}

    pFrame->m_wndComboFormat.SetWindowText(Rptname);
	//------------------------------------------------

	PostMessage(WM_INITIALUPDATE);
}

void CReportingView::OnSelchangeComboFormat() 
{
	CReportingFrame *pFrame = (CReportingFrame *)GetParent();

	CString curno;
	pFrame->m_wndEditBlh.GetWindowText(curno);
	curno.TrimLeft(); curno.TrimRight();
	if(curno.IsEmpty())  return;

	CString RptName_Old, RptName_New;
	pFrame->m_wndComboFormat.GetWindowText(RptName_Old);
	pFrame->m_wndComboFormat.GetLBText(pFrame->m_wndComboFormat.GetCurSel(), RptName_New);

	if(RptName_Old == RptName_New)   return;

//	if(theApp.IsRecordLocked(curno))
//	{
//		AfxMessageBox("该病例已审核，不能修改！");
//		int index = pFrame->m_wndComboFormat.FindStringExact(-1, RptName_Old);
//		if(index != CB_ERR)
//		{
//			pFrame->m_wndComboFormat.SetCurSel(index);
//		}
//		return;
//	}

	try
	{
		SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set bggs=:bggs where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = curno;
		g_dbcommand.Param("bggs").setAsString()      = RptName_New;
		
		g_dbcommand.Execute();
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
	//theApp.OnAppRefresh();
	CMedDiagnose* pMed = (CMedDiagnose*)theApp.GetView()->m_tabCtrl.GetView(1);
	((CComboBox*)pMed->GetDlgItem(IDC_COMBO_BGGS))->SetWindowText(RptName_New);
	OnReportView();
}

void CReportingView::OnReportSend() 
{
	CStringArray blhList;
	CReportingFrame *pFrame = (CReportingFrame *)GetParent();
	CString curno;

	pFrame->m_wndEditBlh.GetWindowText(curno);
	
	curno.TrimLeft(); curno.TrimRight();
	if(curno.IsEmpty())  return;

	blhList.Add(curno);

	theApp.SubmitReport(blhList);

	blhList.RemoveAll();
}

void CReportingView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CFODrawView::OnLButtonUp(nFlags, point);

	CFODrawShapeList* pShapeList = &m_listSelectComp;
	POSITION pos = pShapeList->GetHeadPosition();
	while ( pos )
	{
		CFODrawShape *pShape = pShapeList->GetNext(pos);
		if( pShape == NULL )   continue;
		UINT ObjType = pShape->GetType();
		
		CString curno;
		CReportingFrame *pFrame = (CReportingFrame *)GetParent();
		pFrame->m_wndEditBlh.GetWindowText(curno);
		curno.TrimLeft(); curno.TrimRight();
		if(ObjType == FO_COMP_FIELD && !curno.IsEmpty())
		{
			CString value;    value.Empty();
			CString ShapeExt; ShapeExt.Empty();
			CFODrawShape *pShapeExt = NULL;
			
			bool rt = theApp.GetDatabaseValue(curno,pShape->GetObjectName(),value,true);
			//if(rt && theApp.m_StringFormatMode.CompareNoCase("是") == 0 && (pShape->GetObjectName().CompareNoCase("rysj") == 0 || pShape->GetObjectName().CompareNoCase("gjsj") == 0 || pShape->GetObjectName().CompareNoCase("zdyj") == 0))
			if(rt && (pShape->GetObjectName().CompareNoCase("rysj") == 0 || pShape->GetObjectName().CompareNoCase("gjsj") == 0 || pShape->GetObjectName().CompareNoCase("zdyj") == 0))
			{
				value = "    " + value;
				ShapeExt = pShape->GetObjectName();
				ShapeExt += "1";
				CFODrawShapeList *pList = GetCurrentModel()->GetFormObjects();
				POSITION ns = pList->GetHeadPosition();
				while ( ns )
				{
					CFODrawShape *pSh = pList->GetNext(ns);
					if( pSh == NULL )   continue;
					if(pSh->GetType() == FO_COMP_FIELD && (pSh->GetObjectName().CompareNoCase(ShapeExt) == 0 || pSh->GetObjectCaption().CompareNoCase(ShapeExt) == 0))
					{
						pSh->SetObjectName(ShapeExt);
						pShapeExt = pSh;
						break;
					}
				}
			}
			
			if(rt == false)
			{
				value = pShape->GetObjectCaption();
			}
			
			if( 1 )
			{
				CDC* pDC = GetDC();
				CSize sz = ((CFOFieldShape *)pShape)->GenTextSize(pDC,value);
				CRect ShapeRect = pShape->GetRect();
				if(sz.cx > ShapeRect.Width())
				{
					CSize minsize = ((CFOFieldShape *)pShape)->GenTextSize(pDC,"明");
					CString newstr,newline;
					newstr.Empty();  newline.Empty();
					char enter[3];
					enter[0] = 0x0d; enter[1]= 0x0a; enter[2] = 0x00;
					int TextHeight = 0;
					while(!value.IsEmpty())
					{
						if((((CFOFieldShape *)pShape)->GenTextSize(pDC,newline).cx + 2*minsize.cx ) > ShapeRect.Width() )
						{
							BYTE ch = newline.GetAt(newline.GetLength() -1);
							if((ch&0x80) != 0 )
							{
								int k = 0;
								for(int i = (newline.GetLength() -1); i >=0; i-- )
								{
									ch = newline.GetAt(i);
									if((ch&0x80) == 0 )   break;
									k++;
								}
								if((k%2) != 0 )
								{
									value = newline.Right(1) + value;
									newline = newline.Left(newline.GetLength() - 1);
								}
							}
							newline += enter;
							newstr += newline;
							newline.Empty();

							TextHeight += minsize.cy;
							if((TextHeight+minsize.cy) > ShapeRect.Height())
							{
								break;
							}
						}
						else
						{
							if(value.GetLength() >= 2 && value.GetAt(0) == 0x0d && value.GetAt(1) == 0x0a)
							{
								value = value.Right(value.GetLength() - 2);
								newline += enter;
								newstr += newline;
								newline.Empty();
								
								TextHeight += minsize.cy;
								if((TextHeight+minsize.cy) > ShapeRect.Height())
								{
									break;
								}
							}
							else
							{
								newline += value.Left(1);
								value = value.Right(value.GetLength() - 1);
							}
						}
					}
					if(!newline.IsEmpty())
					{
						newstr += newline;
						newline.Empty();
					}
					pShape->SetObjectCaption( newstr );
					if(!ShapeExt.IsEmpty())
					{
						if(!value.IsEmpty())
						{
							if(pShapeExt)
							{
								pShapeExt->SetVisible(TRUE);
								pShapeExt->SetObjectCaption(value);
							}
						}
						else
						{
							if(pShapeExt)
							{
								pShapeExt->SetVisible(FALSE);
							}
						}
					}
				}
				else
				{
					pShape->SetObjectCaption( value );
				}

				GetCurrentModel()->DoDrawPorts(pShape);
				if(pShapeExt)
				GetCurrentModel()->DoDrawPorts(pShapeExt);
				pShape->OnDraw(pDC);
				if(pShapeExt)
				pShapeExt->OnDraw(pDC);

				SelectShape(pShape);
				if(pShapeExt)  SelectShape(pShapeExt);
				SelectShape(pShape);

				ReleaseDC(pDC);
			}
			else
			{
				pShape->SetVisible(FALSE);
			}

			RedrawWindow();
		}
	}
}

void CReportingView::OnReportCheck() 
{
	// TODO: Add your command handler code here
	if(!RIGHT_SH(theApp.m_Cpp_Right)){
		AfxMessageBox("对不起,您没有审核权限!");
		return;
	}
	theApp.OnAppCheck();
}

CMenu* CReportingView::CreateContextMenu()
{
	// load the menu resource
	if (m_pContextMenu)
	{
		delete m_pContextMenu;
	}
	
	CMenu* pMenu;
	m_pContextMenu = new CMenu();
	m_pContextMenu->LoadMenu(IDR_MENU_REPORTVIEW);
	
	pMenu = m_pContextMenu->GetSubMenu(0);
	
	ASSERT_VALID(pMenu);
	return pMenu;
}

void CReportingView::OnReportSelpic() 
{
	// TODO: Add your command handler code here
	GetParent()->DestroyWindow();
	
	((CMainFrame*)theApp.GetMainWnd())->m_wndTabDockBar.SetActiveView(RUNTIME_CLASS(CMedImage));
	((CMedImage*)(((CMainFrame*)theApp.GetMainWnd())->m_wndTabDockBar.GetActiveView()))->setNotDefault();
}

void CReportingView::OnReportPrint() 
{

	CView::OnFilePrint();
	
	if(m_IsPrinted)
	{
		GetParent()->DestroyWindow();
		m_IsPrinted = FALSE;
	}

	
}

void CReportingView::OnReportClose() 
{
	// TODO: Add your command handler code here
	GetParent()->DestroyWindow();
}
