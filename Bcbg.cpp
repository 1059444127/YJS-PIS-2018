// Bcbg.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "Pathologyview.h"
#include "Pathologydoc.h"
#include "Bcbg.h"
#include "NormalWord.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBcbg dialog
extern CPathologyApp theApp;

CBcbg::CBcbg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CBcbg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBcbg)
	m_bcbgys = _T("");
	m_bczd = _T("");
	//}}AFX_DATA_INIT

	CurEdit = 0;	
	pNormalWord = NULL;
}


void CBcbg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBcbg)
	DDX_Control(pDX, IDC_EDIT_BCBGYS, m_ctrl_bcbgys);
	DDX_Text(pDX, IDC_EDIT_BCBGYS, m_bcbgys);
	DDX_Text(pDX, IDC_EDIT_BCZD, m_bczd);
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNCREATE(CBcbg, CXTResizeDialog)

BEGIN_MESSAGE_MAP(CBcbg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CBcbg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_CYC, OnButtonCyc)
	ON_EN_SETFOCUS(IDC_EDIT_BCZD, OnSetfocusEditBczd)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDPRINT, OnPrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBcbg message handlers

BOOL CBcbg::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    SetResize(IDC_STATIC_BCZD, SZ_TOP_LEFT,  SZ_TOP_LEFT);
    SetResize(IDC_EDIT_BCZD, SZ_TOP_LEFT,  SZ_BOTTOM_RIGHT);
    
	SetResize(IDC_BUTTON_SAVE , SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
    SetResize(IDOK, SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
    SetResize(IDCANCEL, SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);

    SetResize(IDC_STATIC_BCBGYS, SZ_BOTTOM_RIGHT ,  SZ_BOTTOM_RIGHT);
    SetResize(IDC_EDIT_BCBGYS, SZ_BOTTOM_RIGHT ,  SZ_BOTTOM_RIGHT);
    SetResize(IDC_STATIC_BCBGRQ, SZ_BOTTOM_RIGHT ,  SZ_BOTTOM_RIGHT);
    SetResize(IDC_DATE_BCBGRQ, SZ_BOTTOM_RIGHT ,  SZ_BOTTOM_RIGHT);
	
	CRect rt;
	GetDlgItem(IDC_DATE_BCBGRQ)->GetWindowRect(&rt);
	ScreenToClient(&rt);
	if (!m_Time_Bcbgrq.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT, //,DTS_LONGDATEFORMAT
		rt, this, 112))
	{
		return FALSE;
	}
	
	m_Time_Bcbgrq.SetTime(theApp.GetOleServerDateTime());
	
	theApp.ConnectDatabase();
	
	for(int i = 0; i < theApp.m_fnamelist.GetSize(); i++)
	{
		if(theApp.m_fnamelist.GetAt(i).CompareNoCase("bcbgys") == 0)
		{
			m_ctrl_bcbgys.m_info = theApp.m_finfolist.GetAt(i);
		}
	}
	
	m_ctrl_bcbgys.SetBrowseStyle(BES_XT_POPUPMENU,0,0);

	m_bcbgys.Empty();
	m_bczd.Empty();

	COleDateTime ServerTime = theApp.GetOleServerDateTime();
	COleDateTime Bcbgrq = ServerTime;

	try
	{
		SAString cmdstr = "Select bczd,bcbgys,bcbgrq from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
		g_dbcommand.Execute();
		
		if(g_dbcommand.FetchNext())
		{
			m_bczd = g_dbcommand.Field("bczd").asString();
			m_bczd.TrimLeft();  m_bczd.TrimRight();
			m_bcbgys = g_dbcommand.Field("bcbgys").asString();
			
			if(!g_dbcommand.Field("bcbgrq").isNull())
			{
				SADateTime st = g_dbcommand.Field("bcbgrq").asDateTime();
				Bcbgrq.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
				m_Time_Bcbgrq.SetTime(Bcbgrq);
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

	UpdateData(FALSE);	

	//---------------------------------------------------
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		COleDateTimeSpan ts = ServerTime - Bcbgrq;
		if(!m_bczd.IsEmpty() && ts.GetTotalHours() > atoi(theApp.m_BcbgAutoLockMode))
		{
			GetDlgItem(IDOK)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
		}
	}
	else
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
	}	
	//---------------------------------------------------
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBcbg::OnSize(UINT nType, int cx, int cy) 
{
	CXTResizeDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_Time_Bcbgrq.GetSafeHwnd() != NULL)
	{
		CRect rt;
		GetDlgItem(IDC_DATE_BCBGRQ)->GetWindowRect(&rt);
		ScreenToClient(&rt);
		m_Time_Bcbgrq.SetWindowPos(NULL, rt.left, rt.top,rt.Width(),rt.Height(),SWP_NOZORDER);
	}
}

void CBcbg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	theApp.ConnectDatabase();

	// save other field
	try
	{
		CString fieldstr;
		fieldstr.Empty();
		fieldstr += "bczd=:bczd,";
		fieldstr += "bcbgys=:bcbgys,";
		fieldstr += "bcbgrq=:bcbgrq";
		SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set " + fieldstr + " where medicalno = :medicalno";
		
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;;
		g_dbcommand.Param("bczd").setAsString() = m_bczd;
		g_dbcommand.Param("bcbgys").setAsString() = m_bcbgys;
		
        COleDateTime m_time;
		m_Time_Bcbgrq.GetTime(m_time);
		SADateTime bcbgys_dtValue(m_time.GetYear(),m_time.GetMonth(),m_time.GetDay(),m_time.GetHour(),m_time.GetMinute(),m_time.GetSecond());
		g_dbcommand.Param("bcbgrq").setAsDateTime() = bcbgys_dtValue;
		
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
}

void CBcbg::OnOK() 
{
	// TODO: Add extra validation here
	OnButtonSave();
	
	CXTResizeDialog::OnOK();
}

BOOL CBcbg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_hWnd)
	{
		if(pMsg->message == WM_MOUSEMOVE)
		{
			CPoint pt;
			GetCursorPos(&pt);
			CRect rt;

			m_ctrl_bcbgys.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_bcbgys.did(TRUE);
			}
			else
			{
				m_ctrl_bcbgys.did(FALSE);
			}
		}
	}
	
	return CXTResizeDialog::PreTranslateMessage(pMsg);
}

void CBcbg::OnButtonCyc() 
{
	if(CurEdit == 0)	return;

	if(NULL == pNormalWord)
	{
		pNormalWord=new CNormalWord(this);
		pNormalWord->Create();
		pNormalWord->SetActiveWindow();
		pNormalWord->ShowWindow(SW_SHOW);
	}
	else
	{
		pNormalWord->ShowWindow(!pNormalWord->IsWindowVisible());
	}

	if(pNormalWord->IsWindowVisible())
	{
		pNormalWord->AdjustPosition();
	}
}

void CBcbg::OnSetfocusEditBczd() 
{
	if (pNormalWord != NULL && pNormalWord->IsWindowVisible() && CurEdit != 2)
	{
		CurEdit=2;	
		pNormalWord->AdjustPosition();
	}
	else
	{
		CurEdit=2;	
	}
}

void CBcbg::OnDestroy() 
{
	CXTResizeDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (pNormalWord!=NULL) 
	{
		pNormalWord->DestroyWindow();
		delete pNormalWord;
		pNormalWord = NULL;
	}	
}

void CBcbg::OnPrint() 
{
	//---------------------------------------------------
	if(!theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		OnButtonSave();
	}
	//---------------------------------------------------

	theApp.m_PrintType = 4;
	theApp.GetView()->SendMessage(WM_COMMAND, ID_FILE_PRINT);
}
