// Bdbg.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "Pathologyview.h"
#include "Pathologydoc.h"
#include "Bdbg.h"
#include "NormalWord.h"
#include "DBGrid.h"
#include "MedDiagnose.h"

extern char* MD5String(char* szString);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBdbg dialog
extern CPathologyApp theApp;

CBdbg::CBdbg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CBdbg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBdbg)
	m_bdbgys = _T("");
	m_bdfh = _T("");
	m_bdshys = _T("");
	m_bdjg = _T("");
	//}}AFX_DATA_INIT

	CurEdit = 0;	
	pNormalWord = NULL;
}


void CBdbg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBdbg)
	DDX_Control(pDX, IDC_EDIT_BDSHYS, m_ctrl_bdshys);
	DDX_Control(pDX, IDC_EDIT_BDFH, m_ctrl_bdfh);
	DDX_Control(pDX, IDC_EDIT_BDBGYS, m_ctrl_bdbgys);
	DDX_Control(pDX, IDC_EDIT_BDJG, m_ctrl_bdjg);
	DDX_Text(pDX, IDC_EDIT_BDBGYS, m_bdbgys);
	DDX_Text(pDX, IDC_EDIT_BDFH, m_bdfh);
	DDX_Text(pDX, IDC_EDIT_BDSHYS, m_bdshys);
	DDX_Text(pDX, IDC_EDIT_BDJG, m_bdjg);
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNCREATE(CBdbg, CXTResizeDialog)

BEGIN_MESSAGE_MAP(CBdbg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CBdbg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDPRINT, OnPrint)
	ON_BN_CLICKED(IDC_BUTTON_CYC, OnButtonCyc)
	ON_EN_SETFOCUS(IDC_EDIT_BDJG, OnSetfocusEditBdjg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_SENDBDYY, OnSendbdyy)
	ON_BN_CLICKED(IDC_BUTTON_BGCHECK, OnButtonBgcheck)
	ON_BN_CLICKED(IDC_BUTTON_NEWSAMPLE, OnButtonNewSample)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBdbg message handlers

BOOL CBdbg::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    SetResize(IDC_STATIC_BDJG, SZ_TOP_LEFT,  SZ_TOP_LEFT);
    SetResize(IDC_EDIT_BDJG, SZ_TOP_LEFT,  SZ_BOTTOM_RIGHT);
    
	SetResize(IDC_BUTTON_CYC , SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_SAVE , SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);

    SetResize(IDC_BUTTON_BGCHECK,SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
    SetResize(IDC_BUTTON_NEWSAMPLE,SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);

	SetResize(IDC_SENDBDYY, SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	SetResize(IDPRINT, SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
    SetResize(IDCANCEL, SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	
    SetResize(IDC_STATIC_BDBGYS, SZ_BOTTOM_RIGHT ,  SZ_BOTTOM_RIGHT);
    SetResize(IDC_EDIT_BDBGYS, SZ_BOTTOM_RIGHT ,  SZ_BOTTOM_RIGHT);
    SetResize(IDC_STATIC_BDSHYS, SZ_BOTTOM_RIGHT ,  SZ_BOTTOM_RIGHT);
    SetResize(IDC_EDIT_BDSHYS, SZ_BOTTOM_RIGHT ,  SZ_BOTTOM_RIGHT);
    SetResize(IDC_STATIC_BDBGRQ, SZ_BOTTOM_RIGHT ,  SZ_BOTTOM_RIGHT);
    SetResize(IDC_DATE_BDBGRQ, SZ_BOTTOM_RIGHT ,  SZ_BOTTOM_RIGHT);
    SetResize(IDC_STATIC_BDFH, SZ_BOTTOM_RIGHT ,  SZ_BOTTOM_RIGHT);
    SetResize(IDC_EDIT_BDFH, SZ_BOTTOM_RIGHT ,  SZ_BOTTOM_RIGHT);

	this->SetWindowText("冰冻报告-未审核");

	int nStatus;
	try
	{
		SAString cmdstr = "select statusword from " + theApp.TABLE_MAIN + " where medicalno=:medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		while(g_dbcommand.FetchNext())
		{
			nStatus = g_dbcommand.Field("statusword").asLong();
		}
		g_dbconnection.Commit();
	}
	catch (SAException& e)
	{
		AfxMessageBox((const char*)e.ErrText());
	}

	if(nStatus & 0x02)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_BDJG))->SetReadOnly(TRUE);
		this->SetWindowText("冰冻报告-已审核");
	}
	
	CRect rt;
	GetDlgItem(IDC_DATE_BDBGRQ)->GetWindowRect(&rt);
	ScreenToClient(&rt);
	if (!m_Time_Bdbgrq.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT, //,DTS_LONGDATEFORMAT
		rt, this, 112))
	{
		return FALSE;
	}
	
	m_Time_Bdbgrq.SetFormat(_T("yyyy-MM-dd HH':'mm"));
	m_Time_Bdbgrq.SetTime(theApp.GetOleServerDateTime());
	
	theApp.ConnectDatabase();
	
	for(int i = 0; i < theApp.m_fnamelist.GetSize(); i++)
	{
		if(theApp.m_fnamelist.GetAt(i).CompareNoCase("bdshys") == 0)
		{
			m_ctrl_bdshys.m_info = theApp.m_finfolist.GetAt(i);
		}
		if(theApp.m_fnamelist.GetAt(i).CompareNoCase("bdfh") == 0)
		{
			m_ctrl_bdfh.m_info   = theApp.m_finfolist.GetAt(i);
		}
		if(theApp.m_fnamelist.GetAt(i).CompareNoCase("bdbgys") == 0)
		{
			m_ctrl_bdbgys.m_info = theApp.m_finfolist.GetAt(i);
		}
	}
	
	m_ctrl_bdshys.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
	m_ctrl_bdfh.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
	m_ctrl_bdbgys.SetBrowseStyle(BES_XT_POPUPMENU,0,0);

	m_bdbgys.Empty();
	m_bdfh.Empty();
	m_bdshys.Empty();
	m_bdjg.Empty();

	COleDateTime ServerTime = theApp.GetOleServerDateTime();
	
	
	try
	{
		SAString cmdstr = "Select bdjg,bdbgys,bdshys,bdfh,bdbgrq from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
		g_dbcommand.Execute();
		
		if(g_dbcommand.FetchNext())
		{
			m_bdjg = g_dbcommand.Field("bdjg").asString();
			m_bdbgys = g_dbcommand.Field("bdbgys").asString();
			m_bdshys = g_dbcommand.Field("bdshys").asString();
			m_bdfh = g_dbcommand.Field("bdfh").asString();
			
			if(!g_dbcommand.Field("bdbgrq").isNull())
			{
				COleDateTime curt = ServerTime;
				SADateTime st = g_dbcommand.Field("bdbgrq").asDateTime();
				curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
				m_Time_Bdbgrq.SetTime(curt);
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

	if(m_bdbgys.Find(theApp.m_Cpp_User) != -1 || m_bdshys.Find(theApp.m_Cpp_User) != -1)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_BDJG))->SetReadOnly(FALSE);
	}

	//---------------------------------------------------
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		GetDlgItem(IDC_SENDBDYY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_SENDBDYY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
	}
	//---------------------------------------------------
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBdbg::OnSize(UINT nType, int cx, int cy) 
{
	CXTResizeDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_Time_Bdbgrq.GetSafeHwnd() != NULL)
	{
		CRect rt;
		GetDlgItem(IDC_DATE_BDBGRQ)->GetWindowRect(&rt);
		ScreenToClient(&rt);
		m_Time_Bdbgrq.SetWindowPos(NULL, rt.left, rt.top,rt.Width(),rt.Height(),SWP_NOZORDER);
	}
}

BOOL CBdbg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_hWnd)
	{
		if(pMsg->message == WM_MOUSEMOVE)
		{
			CPoint pt;
			GetCursorPos(&pt);
			CRect rt;

			m_ctrl_bdshys.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_bdshys.did(TRUE);
			}
			else
			{
				m_ctrl_bdshys.did(FALSE);
			}
			m_ctrl_bdfh.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_bdfh.did(TRUE);
			}
			else
			{
				m_ctrl_bdfh.did(FALSE);
			}
			m_ctrl_bdbgys.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_bdbgys.did(TRUE);
			}
			else
			{
				m_ctrl_bdbgys.did(FALSE);
			}
		}
	}
	
	return CXTResizeDialog::PreTranslateMessage(pMsg);
}

void CBdbg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	theApp.ConnectDatabase();

	// save other field
	try
	{
		CString fieldstr;
		fieldstr.Empty();
		fieldstr += "bdbgys=:bdbgys,";
		fieldstr += "bdfh=:bdfh,";
		fieldstr += "bdshys=:bdshys,";
		fieldstr += "bdjg=:bdjg,";
		fieldstr += "bdbgrq=:bdbgrq";
		SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set " + fieldstr + " where medicalno = :medicalno";
		
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Param("bdbgys").setAsString() = m_bdbgys;
		g_dbcommand.Param("bdfh").setAsString() = m_bdfh;
		g_dbcommand.Param("bdshys").setAsString() = m_bdshys;
		g_dbcommand.Param("bdjg").setAsString() = m_bdjg;
		
        COleDateTime m_time;
		m_Time_Bdbgrq.GetTime(m_time);
		SADateTime bdbgrq_dtValue(m_time.GetYear(),m_time.GetMonth(),m_time.GetDay(),m_time.GetHour(),m_time.GetMinute(),m_time.GetSecond());
		g_dbcommand.Param("bdbgrq").setAsDateTime() = bdbgrq_dtValue;
		
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

void CBdbg::OnOK() 
{
	CXTResizeDialog::OnOK();
}

void CBdbg::OnPrint() 
{
	//---------------------------------------------------
	if(!theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		OnButtonSave();
	}
	//---------------------------------------------------

	theApp.m_PrintType = 3;
	theApp.GetView()->SendMessage(WM_COMMAND, ID_FILE_PRINT);
}

void CBdbg::OnButtonCyc() 
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

void CBdbg::OnSetfocusEditBdjg() 
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

void CBdbg::OnDestroy() 
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

void CBdbg::OnSendbdyy() 
{
	OnButtonSave();
	SADateTime bdbgrq;
	CString sqh,bdjg,bdbgys,bdshys;
	try
	{
		SAString cmdstr = "Select * from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		if(g_dbcommand.FetchNext())
		{
			sqh    = g_dbcommand.Field("sqh").asString();
			bdjg   = g_dbcommand.Field("bdjg").asString();
			bdbgys = g_dbcommand.Field("bdbgys").asString();
			bdshys = g_dbcommand.Field("bdshys").asString();
			bdbgrq = g_dbcommand.Field("bdbgrq").asDateTime();
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

	if(bdjg.IsEmpty())
	{
		return;	
	}
	else
	{
		if(AfxMessageBox("您确定将结果发送手术室吗？",MB_OKCANCEL) == IDCANCEL)
		{
			return;	
		}
	}

	try
	{
		SAString cmdstr = "Update " + theApp.TABLE_BOOKING + " set bdjg	= :bdjg, zzys = :zzys, shys = :shys, bgrq = :bgrq, state = '4' where sqh = :sqh";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("sqh").setAsString()    = sqh;
		g_dbcommand.Param("zzys").setAsString()   = bdbgys;
		g_dbcommand.Param("shys").setAsString()   = bdshys;
		g_dbcommand.Param("bdjg").setAsString()   = bdjg;
		g_dbcommand.Param("bgrq").setAsDateTime() = bdbgrq;
		
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

void CBdbg::OnButtonBgcheck() 
{
	// TODO: Add your control notification handler code here
	CString bdjg;
	GetDlgItemText(IDC_EDIT_BDJG,bdjg);
	if(bdjg.IsEmpty())
	{
		return;
	}
	
	if(!RIGHT_SH(theApp.m_Cpp_Right))
	{
		AfxMessageBox("您没有审核权限!");
		return;
	}
	

	int nStatus = 0;
	CString sBDJG_MD5("");

	// TODO, If exisiting multi same medicalno, bug!   

	try
	{
		SAString cmdstr = "select statusword from " + theApp.TABLE_MAIN + " where medicalno=:medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();

		while(g_dbcommand.FetchNext())
		{
			nStatus = g_dbcommand.Field("statusword").asLong();
		}
		g_dbconnection.Commit();

		sBDJG_MD5 = MD5String( (char *) (const char *)  bdjg);
		
		nStatus = nStatus | 0x02;

		cmdstr = "Update " + theApp.TABLE_MAIN + " set statusword=:statusword, bdjgMD5=:bdjgMD5 where medicalno=:medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("statusword").setAsLong() = nStatus;
		g_dbcommand.Param("bdjgMD5").setAsString() = sBDJG_MD5;
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		g_dbconnection.Commit();
	}
	catch (SAException& e)
	{
		AfxMessageBox((const char*)e.ErrText());
		return;
	}
	
	SetDlgItemText(IDC_EDIT_BDSHYS,theApp.m_Cpp_User);

	this->SetWindowText("冰冻报告-已审核");

	/// 应该先保存， 再审核 
	 OnButtonSave();
	
}

void CBdbg::OnButtonNewSample() 
{
	CString sMsg;
	int nStatus = 0;

	try
	{
		SAString cmdstr = "select statusword from " + theApp.TABLE_MAIN + " where medicalno=:medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();

		while(g_dbcommand.FetchNext())
		{
			nStatus = g_dbcommand.Field("statusword").asLong();
		}
	
		g_dbconnection.Commit();
	
	}
	catch (SAException& e)
	{
		AfxMessageBox((const char*)e.ErrText());
		return;
	}


	if (nStatus & 0x02 == 0) {
		sMsg.Format("当前冰冻报告还处于未审核状态，不需要执行此操作!");
		AfxMessageBox(sMsg);
		return;
	}
	
	sMsg.Format("执行追加标本操作会取消当前冰冻报告的已审核状态，\n医生可追加此冰冻报告的内容, 冰冻报告需要重新审核。 操作是否要继续?");
	

	if (AfxMessageBox(sMsg, MB_YESNO | MB_ICONEXCLAMATION) != IDYES)
		return;


	try
	{
		
		nStatus = nStatus & ~0x02;

		SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set statusword=:statusword, MultiSampleFlag = 1 where medicalno=:medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("statusword").setAsLong() = nStatus;
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		g_dbconnection.Commit();
	}
	catch (SAException& e)
	{
		AfxMessageBox((const char*)e.ErrText());
		return;
	}
	
	this->SetWindowText("冰冻报告-未审核");
}