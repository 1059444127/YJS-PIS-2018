// Sfjg.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "Pathologyview.h"
#include "Pathologydoc.h"
#include "Sfjg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSfjg dialog
extern CPathologyApp theApp;

CSfjg::CSfjg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CSfjg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSfjg)
	m_sfjg = _T("");
	m_sfys = _T("");
	//}}AFX_DATA_INIT
}


void CSfjg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSfjg)
	DDX_Control(pDX, IDC_EDIT_SFYS, m_ctrl_sfys);
	DDX_Text(pDX, IDC_EDIT_SFJG, m_sfjg);
	DDX_Text(pDX, IDC_EDIT_SFYS, m_sfys);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSfjg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CSfjg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSfjg message handlers

BOOL CSfjg::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    SetResize(IDC_STATIC_SFJG, SZ_TOP_LEFT,  SZ_TOP_LEFT);
    SetResize(IDC_EDIT_SFJG, SZ_TOP_LEFT,  SZ_BOTTOM_RIGHT);
    
	SetResize(IDC_BUTTON_SAVE , SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
    SetResize(IDOK, SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
    SetResize(IDCANCEL, SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);

    SetResize(IDC_STATIC_JHSFSJ, SZ_TOP_LEFT ,  SZ_TOP_LEFT);
    SetResize(IDC_DATE_JHSFSJ, SZ_TOP_LEFT ,  SZ_TOP_LEFT);
    SetResize(IDC_STATIC_SFYS, SZ_TOP_LEFT ,  SZ_TOP_LEFT);
    SetResize(IDC_EDIT_SFYS, SZ_TOP_LEFT ,  SZ_TOP_LEFT);
	
	CRect rt;
	GetDlgItem(IDC_DATE_JHSFSJ)->GetWindowRect(&rt);
	ScreenToClient(&rt);
	if (!m_Time_Jhsfsj.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT, //,DTS_LONGDATEFORMAT
		rt, this, 112))
	{
		return FALSE;
	}
	
	m_Time_Jhsfsj.SetTime(theApp.GetOleServerDateTime());
	
	theApp.ConnectDatabase();
	
	for(int i = 0; i < theApp.m_fnamelist.GetSize(); i++)
	{
		if(theApp.m_fnamelist.GetAt(i).CompareNoCase("sfys") == 0)
		{
			m_ctrl_sfys.m_info = theApp.m_finfolist.GetAt(i);
		}
	}
	
	m_ctrl_sfys.SetBrowseStyle(BES_XT_POPUPMENU,0,0);

	m_sfjg.Empty();
	m_sfys.Empty();

	COleDateTime ServerTime = theApp.GetOleServerDateTime();
	try
	{
		SAString cmdstr = "Select sfjg,sfys,jhsfsj from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
		g_dbcommand.Execute();
		
		if(g_dbcommand.FetchNext())
		{
			m_sfjg = g_dbcommand.Field("sfjg").asString();
			m_sfys = g_dbcommand.Field("sfys").asString();
			
			COleDateTime curt = ServerTime;
			SADateTime st = g_dbcommand.Field("jhsfsj").asDateTime();
			curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
			m_Time_Jhsfsj.SetTime(curt);
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
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSfjg::OnSize(UINT nType, int cx, int cy) 
{
	CXTResizeDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_Time_Jhsfsj.GetSafeHwnd() != NULL)
	{
		CRect rt;
		GetDlgItem(IDC_DATE_JHSFSJ)->GetWindowRect(&rt);
		ScreenToClient(&rt);
		m_Time_Jhsfsj.SetWindowPos(NULL, rt.left, rt.top,rt.Width(),rt.Height(),SWP_NOZORDER);
	}
}

BOOL CSfjg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_hWnd)
	{
		if(pMsg->message == WM_MOUSEMOVE)
		{
			CPoint pt;
			GetCursorPos(&pt);
			CRect rt;

			m_ctrl_sfys.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_sfys.did(TRUE);
			}
			else
			{
				m_ctrl_sfys.did(FALSE);
			}
		}
	}
	
	return CXTResizeDialog::PreTranslateMessage(pMsg);
}

void CSfjg::OnOK() 
{
	// TODO: Add extra validation here
	OnButtonSave();
	CXTResizeDialog::OnOK();
}

void CSfjg::OnCancel() 
{
	// TODO: Add extra cleanup here
	COleDateTime curt;
	curt.SetDateTime(1910, 1, 1, 0, 0, 0);
	m_Time_Jhsfsj.SetTime(curt);

    OnButtonSave();	
	CXTResizeDialog::OnCancel();
}

void CSfjg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	theApp.ConnectDatabase();

	// save other field
	try
	{
		CString fieldstr;
		fieldstr.Empty();
		fieldstr += "sfjg=:sfjg,";
		fieldstr += "sfys=:sfys,";
		fieldstr += "jhsfsj=:jhsfsj";
		SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set " + fieldstr + " where medicalno = :medicalno";
		
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;;
		g_dbcommand.Param("sfjg").setAsString() = m_sfjg;
		g_dbcommand.Param("sfys").setAsString() = m_sfys;
		
        COleDateTime m_time;
		m_Time_Jhsfsj.GetTime(m_time);
		SADateTime jhsfsj_dtValue(m_time.GetYear(),m_time.GetMonth(),m_time.GetDay(),m_time.GetHour(),m_time.GetMinute(),m_time.GetSecond());
		g_dbcommand.Param("jhsfsj").setAsDateTime() = jhsfsj_dtValue;
		
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
