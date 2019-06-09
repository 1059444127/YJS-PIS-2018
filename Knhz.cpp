// Knhz.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "Pathologyview.h"
#include "Pathologydoc.h"
#include "Knhz.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKnhz dialog
extern CPathologyApp theApp;

CKnhz::CKnhz(CWnd* pParent /*=NULL*/)
	: CDialog(CKnhz::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKnhz)
	m_hzfqr = _T("");
	m_cyys = _T("");
	m_zjyj = _T("");
	//}}AFX_DATA_INIT
	PeronArray.RemoveAll();
	DateArray.RemoveAll();
}


void CKnhz::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKnhz)
	DDX_Control(pDX, IDC_EDIT_HZFQR, m_ctrl_hzfqr);
	DDX_Control(pDX, IDC_TAB_CTRL, m_tabCtrl);
	DDX_Text(pDX, IDC_EDIT_HZFQR, m_hzfqr);
	DDX_Text(pDX, IDC_EDIT_CYYS, m_cyys);
	DDX_Text(pDX, IDC_EDIT_ZJYJ, m_zjyj);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKnhz, CDialog)
	//{{AFX_MSG_MAP(CKnhz)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, OnButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_NOTIFY(NM_CLICK, IDC_TAB_CTRL, OnClickTabCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKnhz message handlers

BOOL CKnhz::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_tabCtrl.SendInitialUpdate(TRUE);
	m_tabCtrl.SetPadding(CSize(6, 3));

	// Set the tab to auto condense (shrink) its tabs when sized.
	m_tabCtrl.SetAutoCondense(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_tabCtrl.ModifyStyle(0L, WS_CLIPCHILDREN|WS_CLIPSIBLINGS);

	::SetWindowPos(m_tabCtrl.m_hWnd, NULL, 0,0,0,0,	SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
	m_tabCtrl.Invalidate(FALSE);

	if (!m_listBox.CreateEx(WS_EX_STATICEDGE, _T("LISTBOX"), NULL,
		WS_CHILD|WS_VISIBLE|LBS_NOINTEGRALHEIGHT|WS_VSCROLL,
		CRect(0,0,0,0), this, 110))
	{
		TRACE0("Failed to create listbox control.\n");
		return -1;
	}

	// Set the font for the listbox control.
	m_listBox.SetFont (&xtAfxData.font);

	m_pDalRich = new CDalsuRichEdit;
	m_pDalRich->Create(WS_VISIBLE | WS_CHILD |WS_BORDER | WS_HSCROLL | 
					WS_CLIPCHILDREN | WS_VSCROLL | ES_MULTILINE | ES_AUTOHSCROLL | ES_READONLY |
					ES_AUTOVSCROLL |ES_LEFT | ES_WANTRETURN, CRect(0,0,0,0), this, 111);

	if (!m_tabCtrl.AddControl(_T("信息维护"), &m_listBox, 0, -1))
	{
		TRACE0("Failed to create edit list.\n");
		return -1;
	}

	if (!m_tabCtrl.AddControl(_T("会诊记录"), m_pDalRich, 1, -1))
	{
		TRACE0("Failed to create edit list.\n");
		return -1;
	}

	
	CRect rt;
	GetDlgItem(IDC_DATE_HZRQ)->GetWindowRect(&rt);
	ScreenToClient(&rt);
	if (!m_Time_Hzrq.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT, //,DTS_LONGDATEFORMAT
		rt, this, 112))
	{
		return FALSE;
	}
	
	m_Time_Hzrq.SetTime(theApp.GetOleServerDateTime());
	
	theApp.ConnectDatabase();
	
	for(int i = 0; i < theApp.m_fnamelist.GetSize(); i++)
	{
		if(theApp.m_fnamelist.GetAt(i).CompareNoCase("hzfqr") == 0)
		{
			m_ctrl_hzfqr.m_info = theApp.m_finfolist.GetAt(i);
		}
	}
	
	m_ctrl_hzfqr.SetBrowseStyle(BES_XT_POPUPMENU,0,0);

	m_hzfqr.Empty();
	m_cyys.Empty();
	m_zjyj.Empty();

	COleDateTime ServerTime = theApp.GetOleServerDateTime();

	try
	{
		SAString cmdstr = "Select hzfqr,cyys,zjyj,hzrq from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
		g_dbcommand.Execute();
		
		if(g_dbcommand.FetchNext())
		{
			m_hzfqr = g_dbcommand.Field("hzfqr").asString();
			m_cyys = g_dbcommand.Field("cyys").asString();
			m_zjyj = g_dbcommand.Field("zjyj").asString();
			
			COleDateTime curt = ServerTime;
			SADateTime st = g_dbcommand.Field("hzrq").asDateTime();
			curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
			m_Time_Hzrq.SetTime(curt);
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

	RefreshTabInfo();

	m_TimerHandle = SetTimer(1, 5000, NULL);

	//---------------------------------------------------
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CANCEL)->EnableWindow(FALSE);

		IsButtonEnabled	= false;
	}
	else
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CANCEL)->EnableWindow(TRUE);

		IsButtonEnabled	= true;
	}
	//---------------------------------------------------

	m_tabCtrl.SetActiveView(1);
	if(IsButtonEnabled)
	{
		GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
	}
	CString joiner; joiner.Empty();
	try{
		SAString cmdstr = "select distinct person from "+ theApp.TABLE_HZ + " where medicalno =:medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		while(g_dbcommand.FetchNext()){
			CString person = g_dbcommand.Field("person").asString();
			if(!person.IsEmpty()){
				joiner += person;
				joiner += "、";
			}
		}
		g_dbconnection.Commit();
	} catch (SAException &e){
		AfxMessageBox((const char*)e.ErrText());
	}

	SetDlgItemText(IDC_EDIT_CYYS,joiner);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKnhz::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_TimerHandle)
	{
		KillTimer(m_TimerHandle);
		m_TimerHandle = 0;
	}

	SAFE_DELETE(m_pDalRich);

	PeronArray.RemoveAll();
	DateArray.RemoveAll();
}

void CKnhz::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	theApp.ConnectDatabase();

	// save other field
	try
	{
		CString fieldstr;
		fieldstr.Empty();
		fieldstr += "hzfqr=:hzfqr,";
		fieldstr += "cyys=:cyys,";
		fieldstr += "zjyj=:zjyj,";
		fieldstr += "hzrq=:hzrq";
		SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set " + fieldstr + " where medicalno = :medicalno";
		
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;;
		g_dbcommand.Param("hzfqr").setAsString() = m_hzfqr;
		g_dbcommand.Param("cyys").setAsString() = m_cyys;
		g_dbcommand.Param("zjyj").setAsString() = m_zjyj;
		
        COleDateTime m_time;
		m_Time_Hzrq.GetTime(m_time);
		SADateTime hzrq_dtValue(m_time.GetYear(),m_time.GetMonth(),m_time.GetDay(),m_time.GetHour(),m_time.GetMinute(),m_time.GetSecond());
		g_dbcommand.Param("hzrq").setAsDateTime() = hzrq_dtValue;
		
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

void CKnhz::OnOK() 
{
	// TODO: Add extra validation here
	OnButtonSave();
	
	CDialog::OnOK();
}

void CKnhz::RefreshTabInfo()
{
	PeronArray.RemoveAll();
	DateArray.RemoveAll();

	CString CR = "\r\n";
	COLORREF crColor = RGB(255, 0, 0);
	COLORREF textColor = RGB(0, 0, 0);
	
	for(int index = m_listBox.GetCount()-1; index >=0; index--)
	{
		m_listBox.DeleteString(index);
	}

	m_pDalRich->SetWindowText("");

	theApp.ConnectDatabase();
 
	int IsFirstRecord = true;
	try
	{
		SAString cmdstr = "Select person,info,inputdate from " + theApp.TABLE_HZ + " where medicalno = :medicalno order by inputdate asc";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
		g_dbcommand.Execute();
		
		while(g_dbcommand.FetchNext())
		{
			CString m_person = g_dbcommand.Field("person").asString();
			CString m_info   = g_dbcommand.Field("info").asString();

			SADateTime st = g_dbcommand.Field("inputdate").asDateTime();
			COleDateTime m_bgrq;
			m_bgrq.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());

			if(IsFirstRecord)
			{
				IsFirstRecord = false;
				m_pDalRich->AddMsg(m_person + "  初诊于  " + m_bgrq.Format("%Y-%m-%d %H:%M:%S") + CR, crColor, TRUE, TRUE);
				m_pDalRich->AddText(m_info + CR, textColor);
				
				m_listBox.AddString(m_person + "  初诊于  " + m_bgrq.Format("%Y-%m-%d %H:%M:%S"));
			}
			else
			{
				m_pDalRich->AddMsg(m_person + "  建立于  " + m_bgrq.Format("%Y-%m-%d %H:%M:%S") + CR, crColor, TRUE, TRUE);
				m_pDalRich->AddText(m_info + CR, textColor);
				
				m_listBox.AddString(m_person + "  建立于  " + m_bgrq.Format("%Y-%m-%d %H:%M:%S"));
			}


			PeronArray.Add(m_person);
			DateArray.Add(st);
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

void CKnhz::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_YSYJ, "");
}

void CKnhz::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	if( 0 == m_listBox.GetCount())
	{
		AfxMessageBox("该病例还未发起会诊！");
		return;
	}

	CString m_ysyj; m_ysyj.Empty();
	GetDlgItemText(IDC_EDIT_YSYJ, m_ysyj);
	m_ysyj.TrimLeft();  m_ysyj.TrimRight();
	if(m_ysyj.IsEmpty())  return;

	SADateTime ServerTime = theApp.GetServerDateTime();
	theApp.ConnectDatabase();
	try
	{
		SAString cmdstr = "Insert into " + theApp.TABLE_HZ + "(medicalno,person,info,inputdate) values(:medicalno,:person,:info,:inputdate)";
		
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Param("person").setAsString() = theApp.m_Cpp_User;
		g_dbcommand.Param("info").setAsString() = m_ysyj;
		
		SADateTime st = ServerTime;
		g_dbcommand.Param("inputdate").setAsDateTime() = st;
		
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

	RefreshTabInfo();
}

void CKnhz::OnButtonModify() 
{
	// TODO: Add your control notification handler code here
	CString m_ysyj; m_ysyj.Empty();
	GetDlgItemText(IDC_EDIT_YSYJ, m_ysyj);
	m_ysyj.TrimLeft();  m_ysyj.TrimRight();
	if(m_ysyj.IsEmpty())  return;

    int nIndex = m_listBox.GetCurSel();
	int nCount = m_listBox.GetCount();
	if ((nIndex != LB_ERR) && (nCount > 0))
	{
		if(theApp.m_Cpp_User.CompareNoCase(PeronArray.GetAt(nIndex)))
		{
			AfxMessageBox("用户只能修改自己建立的科内会诊记录!");
			return;
		}

		SADateTime ServerTime = theApp.GetServerDateTime();

		theApp.ConnectDatabase();

		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_HZ + " set info=:info,inputdate=:inputdate1 where medicalno = :medicalno and person = :person and inputdate = :inputdate";
			
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Param("person").setAsString() = PeronArray.GetAt(nIndex);
			g_dbcommand.Param("inputdate").setAsDateTime() = DateArray.GetAt(nIndex);
			g_dbcommand.Param("info").setAsString() = m_ysyj;

			SADateTime st = ServerTime;
			g_dbcommand.Param("inputdate1").setAsDateTime() = st;
			
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

		RefreshTabInfo();
	}
}

void CKnhz::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
    int nIndex = m_listBox.GetCurSel();
	int nCount = m_listBox.GetCount();
	if ((nIndex != LB_ERR) && (nCount > 0))
	{
		if(theApp.m_Cpp_User.CompareNoCase(PeronArray.GetAt(nIndex)))
		{
			AfxMessageBox("用户只能删除自己建立的科内会诊记录!");
			return;
		}

		theApp.ConnectDatabase();

		try
		{
			SAString cmdstr = "Delete from " + theApp.TABLE_HZ + " where medicalno = :medicalno and person = :person and inputdate = :inputdate";
			
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Param("person").setAsString() = PeronArray.GetAt(nIndex);
			g_dbcommand.Param("inputdate").setAsDateTime() = DateArray.GetAt(nIndex);
			
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

		RefreshTabInfo();
	}
}

void CKnhz::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent == m_TimerHandle )
	{
		RefreshTabInfo();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CKnhz::OnButtonStart() 
{
	theApp.ConnectDatabase();

	int count = 0;

	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_HZ + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
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
		return;
	}

	if(count == 0)
	{
		CString m_czys, m_czyj, m_zdyj;  
		m_czys.Empty();
		m_czyj.Empty();
		m_zdyj.Empty();
		
		SADateTime m_bgrq = theApp.GetServerDateTime();
		
		try
		{
			SAString cmdstr = "Select czys,czyj,zdyj,bgrq from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			
			g_dbcommand.Execute();
			
			if(g_dbcommand.FetchNext())
			{
				m_czys = g_dbcommand.Field("czys").asString();
				m_czyj = g_dbcommand.Field("czyj").asString();
				m_zdyj = g_dbcommand.Field("zdyj").asString();
				
				if(!g_dbcommand.Field("bgrq").isNull())
				{
					m_bgrq = g_dbcommand.Field("bgrq").asDateTime();
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
		
		m_czyj.TrimLeft(); m_czyj.TrimRight();
		m_zdyj.TrimLeft(); m_zdyj.TrimRight();

		CString info;
		if(m_czyj.IsEmpty())
		{
			info = m_zdyj;
		}
		else
		{
			info = m_czyj;
		}

		try
		{
			SAString cmdstr = "Insert into " + theApp.TABLE_HZ + "(medicalno,person,info,inputdate) values(:medicalno,:person,:info,:inputdate)";
			
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Param("person").setAsString() = m_czys;
			g_dbcommand.Param("info").setAsString() = info;
			g_dbcommand.Param("inputdate").setAsDateTime() = m_bgrq;
			
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

		UpdateData(TRUE);

		m_hzfqr = theApp.m_Cpp_User;
		m_Time_Hzrq.SetTime(theApp.GetOleServerDateTime());

		UpdateData(FALSE);

		RefreshTabInfo();
	}
	else
	{
		AfxMessageBox("该病例已发起会诊！");
	}
}

void CKnhz::OnClickTabCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(!IsButtonEnabled)   return;

    CWnd *pWnd = m_tabCtrl.GetActiveView();

	if(pWnd->IsKindOf(RUNTIME_CLASS(CXTListBox)))
	{
		GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
	}
	if(pWnd->IsKindOf(RUNTIME_CLASS(CDalsuRichEdit)))
	{
		GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
	}
	
	*pResult = 0;
}
