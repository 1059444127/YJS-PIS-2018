// CAdviceShow : implementation file
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "Pathology.h"
#include "Pathologyview.h"
#include "Pathologydoc.h"
#include "AdviceShow.h"
#include "KnlyShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdviceShow

CAdviceShow::CAdviceShow()
	: CXTResizeFormView(CAdviceShow::IDD)
{
	//{{AFX_DATA_INIT(CAdviceShow)
	//}}AFX_DATA_INIT
	m_TimerHandle = 0;

	m_bIsFirstInit = true;
}

CAdviceShow::~CAdviceShow()
{
	// TODO: add destruction code here.
	if(m_TimerHandle)
	{
		KillTimer(m_TimerHandle);
		m_TimerHandle = 0;
	}
}

IMPLEMENT_DYNCREATE(CAdviceShow, CXTResizeFormView)

BEGIN_MESSAGE_MAP(CAdviceShow, CXTResizeFormView)
	//{{AFX_MSG_MAP(CAdviceShow)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_HYP_KNLY, OnHypKnly)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdviceShow message handlers
extern CPathologyApp theApp;

void CAdviceShow::DoDataExchange(CDataExchange* pDX) 
{
	CXTResizeFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdviceShow)
	DDX_Control(pDX, IDC_HYP_KNLY, m_HypLink_Knly);
	//}}AFX_DATA_MAP
}

void CAdviceShow::OnInitialUpdate() 
{
	CXTResizeFormView::OnInitialUpdate();
	
	if (m_bIsFirstInit)
	{
		m_bIsFirstInit = false;
		
		m_HypLink_Knly.SetURL("短信");
		m_HypLink_Knly.SetColors(RGB(0xFF, 0x00, 0x00), RGB(0xFF, 0x00, 0x00), RGB(0xFF, 0x00, 0x00));
		m_HypLink_Knly.SetUnderline(true);
		m_HypLink_Knly.EnableShellExecute(false);

		m_RecordList.AttachGrid(this,IDC_STATIC_RECORDLIST);
		m_RecordList.SetParent(this);
		m_RecordList.ResetGridSize();
		m_RecordList.RedrawAll();

		SetResize(IDC_STATIC_RECORDLIST, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
		SetResize(IDC_HYP_KNLY, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_BUTTON_REFRESH, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

		m_TimerHandle = SetTimer(2, 1000, NULL);
	}

	RefreshHyperLink();
}

void CAdviceShow::OnDestroy() 
{
	CXTResizeFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_TimerHandle)
	{
		KillTimer(m_TimerHandle);
		m_TimerHandle = 0;
	}
}

void CAdviceShow::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	static bool mute = false;
	if(nIDEvent == m_TimerHandle && !mute)
	{
		mute = true;
		m_RecordList.ResetGridSize();
		m_RecordList.RedrawAll();
		mute = false;
	}

	CXTResizeFormView::OnTimer(nIDEvent);
}

void CAdviceShow::OnHypKnly() 
{
	CKnlyShow mydlg(this);
	mydlg.DoModal();

	RefreshHyperLink();
}

void CAdviceShow::RefreshHyperLink()
{
	theApp.ConnectDatabase();

	int NoteCount = 0;
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_NOTEPAD + " where jsr=:jsr or fbr=:fbr";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("jsr").setAsString() = theApp.m_Cpp_User;
		g_dbcommand.Param("fbr").setAsString() = theApp.m_Cpp_User;
		
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
		{
			NoteCount = g_dbcommand.Field(1).asLong();
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

	int UnReadCount = 0;
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_NOTEPAD + " where jsr=:jsr and zt=0";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("jsr").setAsString() = theApp.m_Cpp_User;
		
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
		{
			UnReadCount = g_dbcommand.Field(1).asLong();
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

	CString str;
	str.Format("短    信: %d/%d", UnReadCount, NoteCount);

	if(UnReadCount > 0)
	{
		theApp.m_UnRead_Knly_Notified.Format("您当前有%d条未读留言！", UnReadCount);
	}

	m_HypLink_Knly.SetWindowText(str);
	m_HypLink_Knly.Invalidate();
}

void CAdviceShow::OnButtonRefresh() 
{
	RefreshHyperLink();
}

void CAdviceShow::GetLeaveMessage(){
	OnHypKnly();
}
