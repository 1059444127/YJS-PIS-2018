// CDBList : implementation file
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Pathology.h"
#include "Pathologyview.h"
#include "Pathologydoc.h"
#include "Resource.h"
#include "DBList.h"
#include "HistoryShow.h"
#include "BqlbShow.h"
#include "HzlbShow.h"
#include "SflbShow.h"
#include "KnlyShow.h"
#include "BdyyShow.h"
#include "INI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBList
extern CPathologyApp theApp;

CDBList::CDBList(CWnd* pParent /*=NULL*/)
	: CXTCBarDialog(CDBList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDBList)
	m_wfyy = _T("");
	//}}AFX_DATA_INIT
	m_state0 = m_state1 = m_state2 = m_state3 = m_state4 = m_state5 = false;
	m_state6 = true;
}

IMPLEMENT_DYNCREATE(CDBList, CXTCBarDialog)

BEGIN_MESSAGE_MAP(CDBList, CXTCBarDialog)
	//{{AFX_MSG_MAP(CDBList)
	ON_COMMAND(ID_DBLIST_STATE1, OnDblistState1)
	ON_UPDATE_COMMAND_UI(ID_DBLIST_STATE1, OnUpdateDblistState1)
	ON_COMMAND(ID_DBLIST_STATE2, OnDblistState2)
	ON_UPDATE_COMMAND_UI(ID_DBLIST_STATE2, OnUpdateDblistState2)
	ON_COMMAND(ID_DBLIST_STATE3, OnDblistState3)
	ON_UPDATE_COMMAND_UI(ID_DBLIST_STATE3, OnUpdateDblistState3)
	ON_COMMAND(ID_DBLIST_STATE4, OnDblistState4)
	ON_UPDATE_COMMAND_UI(ID_DBLIST_STATE4, OnUpdateDblistState4)
	ON_COMMAND(ID_DBLIST_STATE0, OnDblistState0)
	ON_UPDATE_COMMAND_UI(ID_DBLIST_STATE0, OnUpdateDblistState0)
	ON_BN_CLICKED(IDC_BUTTON_BLH, OnButtonBlh)
	ON_BN_CLICKED(IDC_HYP_LSJC, OnHypLsjc)
	ON_BN_CLICKED(IDC_HYP_BQLB, OnHypBqlb)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	ON_BN_CLICKED(IDC_HYP_HZBL, OnHypHzbl)
	ON_BN_CLICKED(IDC_HYP_SFBL, OnHypSfbl)
	ON_COMMAND(ID_DBLIST_STATE5, OnDblistState5)
	ON_UPDATE_COMMAND_UI(ID_DBLIST_STATE5, OnUpdateDblistState5)
	ON_COMMAND(ID_DBLIST_STATE6, OnDblistState6)
	ON_UPDATE_COMMAND_UI(ID_DBLIST_STATE6, OnUpdateDblistState6)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_HYP_KNLY, OnHypKnly)
	ON_EN_CHANGE(IDC_EDIT_WFYY, OnContentModified)
	ON_UPDATE_COMMAND_UI(ID_DATESEL, OnUpdateDatesel)
	ON_BN_CLICKED(IDC_HYP_BDYY, OnHypBdyy)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CONTENTSAVE, OnContentSave)
	ON_MESSAGE(WM_CONTENTREFRESH, OnContentRefresh)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CDBList::DoDataExchange(CDataExchange* pDX) 
{
	CXTCBarDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDBList)
	DDX_Control(pDX, IDC_HYP_BDYY, m_HypLink_Bdyy);
	DDX_Control(pDX, IDC_HYP_KNLY, m_HypLink_Knly);
	DDX_Control(pDX, IDC_STATIC_WFYY, m_HypLink_Wfyy);
	DDX_Control(pDX, IDC_HYP_SFBL, m_HypLink_Sfbl);
	DDX_Control(pDX, IDC_HYP_HZBL, m_HypLink_Hzbl);
	DDX_Control(pDX, IDC_HYP_BQLB, m_HypLink_Bqlb);
	DDX_Control(pDX, IDC_HYP_LSJC, m_HypLink_Lsjc);
	DDX_Control(pDX, IDC_EDIT_WFYY, m_ctrl_wfyy);
	DDX_Text(pDX, IDC_EDIT_WFYY, m_wfyy);
	//}}AFX_DATA_MAP
}

BOOL CDBList::OnInitDialog() 
{
	CXTCBarDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),AFX_IDW_TOOLBAR+5)||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_DBLIST))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;      // fail to create
	}

	if (!m_monthCal.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT,
		CRect(0,0,20,20), &m_wndToolBar, ID_DATESEL))
	{
		return FALSE;
	}
	m_wndToolBar.InsertControl(&m_monthCal);

    m_wndToolBar.AutoSizeToolbar();

	m_RecordList.AttachGrid(this,IDC_STATIC_RECORDLIST);
	m_RecordList.SetParent(this);
	m_RecordList.ResetGridSize();
	m_RecordList.RedrawAll();

	((CComboBox *)GetDlgItem(IDC_COMBO_BLH))->AddString("病理号");
	((CComboBox *)GetDlgItem(IDC_COMBO_BLH))->AddString("住院号");
	((CComboBox *)GetDlgItem(IDC_COMBO_BLH))->AddString("门诊号");
	((CComboBox *)GetDlgItem(IDC_COMBO_BLH))->AddString("冰冻号");
	((CComboBox *)GetDlgItem(IDC_COMBO_BLH))->AddString("姓名");
	((CComboBox *)GetDlgItem(IDC_COMBO_BLH))->AddString("申请号");
	((CComboBox *)GetDlgItem(IDC_COMBO_BLH))->AddString("病人ID");
	((CComboBox *)GetDlgItem(IDC_COMBO_BLH))->SetCurSel(0);

	SetResize(IDC_COMBO_BLH, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_BLH, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_BLH, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_CHECK_CURYEAR, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_CHECK_BLUR, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_RECORDLIST, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC_WFYY, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_WFYY, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_HYP_HZBL, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_HYP_SFBL, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_HYP_LSJC, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_HYP_BQLB, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_HYP_KNLY, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_HYP_BDYY, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_REFRESH, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_ALLDB, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	theApp.ConnectDatabase();
	
	for(int i = 0; i < theApp.m_fnamelist.GetSize(); i++)
	{
		if(theApp.m_fnamelist.GetAt(i).CompareNoCase("wfyy") == 0)
		{
			m_ctrl_wfyy.m_info = theApp.m_finfolist.GetAt(i);
		}
	}
	
	m_ctrl_wfyy.SetBrowseStyle(BES_XT_POPUPMENU,0,0);

	m_HypLink_Lsjc.SetURL("显示当前病人历次检查信息");
	m_HypLink_Lsjc.SetColors(RGB(0x00, 0x00, 0xFF), RGB(0x00, 0x00, 0xFF), RGB(0x00, 0x00, 0xFF));
	m_HypLink_Lsjc.SetUnderline(true);
	m_HypLink_Lsjc.EnableShellExecute(false);

	m_HypLink_Bqlb.SetURL("显示待执行的补取医嘱");
	m_HypLink_Bqlb.SetColors(RGB(0xFF, 0x00, 0x00), RGB(0xFF, 0x00, 0x00), RGB(0xFF, 0x00, 0x00));
	m_HypLink_Bqlb.SetUnderline(true);
	m_HypLink_Bqlb.EnableShellExecute(false);

	m_HypLink_Hzbl.SetURL("显示会诊中的病例");
	m_HypLink_Hzbl.SetColors(RGB(0xFF, 0x00, 0x00), RGB(0xFF, 0x00, 0x00), RGB(0xFF, 0x00, 0x00));
	m_HypLink_Hzbl.SetUnderline(true);
	m_HypLink_Hzbl.EnableShellExecute(false);

	m_HypLink_Sfbl.SetURL("显示随访标识的病例");
	m_HypLink_Sfbl.SetColors(RGB(0xFF, 0x00, 0x00), RGB(0xFF, 0x00, 0x00), RGB(0xFF, 0x00, 0x00));
	m_HypLink_Sfbl.SetUnderline(true);
	m_HypLink_Sfbl.EnableShellExecute(false);

	m_HypLink_Wfyy.SetURL("延期原因");
	m_HypLink_Wfyy.SetColors(RGB(0x00, 0x00, 0xFF), RGB(0x00, 0x00, 0xFF), RGB(0x00, 0x00, 0xFF));
	m_HypLink_Wfyy.SetUnderline(false);
	m_HypLink_Wfyy.EnableShellExecute(false);

	m_HypLink_Knly.SetURL("短信");
	m_HypLink_Knly.SetColors(RGB(0xFF, 0x00, 0x00), RGB(0xFF, 0x00, 0x00), RGB(0xFF, 0x00, 0x00));
	m_HypLink_Knly.SetUnderline(true);
	m_HypLink_Knly.EnableShellExecute(false);

	m_HypLink_Bdyy.SetURL("显示冰冻预约信息");
	m_HypLink_Bdyy.SetColors(RGB(0xFF, 0x00, 0x00), RGB(0xFF, 0x00, 0x00), RGB(0xFF, 0x00, 0x00));
	m_HypLink_Bdyy.SetUnderline(true);
	m_HypLink_Bdyy.EnableShellExecute(false);

	if(theApp.m_StationType.CompareNoCase("取材工作站") && theApp.m_StationType.CompareNoCase("诊断工作站"))
	{
		GetDlgItem(IDC_HYP_BQLB)->ShowWindow(SW_HIDE);
	}

	RefreshHyperLink();

    if(theApp.m_MyRptMode.CompareNoCase("是") == 0)
	{
		m_RecordList.OnMenuCommand(0, 0, 0, 1038);  // "我的未打印报告" 检索
	}

	((CButton *)GetDlgItem(IDC_CHECK_BLUR))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_ALLDB))->SetCheck(0);

	CIniReader m_IniReader;
	m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");
	CString m_ThisYearMode = m_IniReader.getKeyValue("本年检索条件有效","Settings");
	if(m_ThisYearMode.IsEmpty())  m_ThisYearMode.Format("是");

	if(m_ThisYearMode.CompareNoCase("是") == 0)
		((CButton *)GetDlgItem(IDC_CHECK_CURYEAR))->SetCheck(1);
	else
		((CButton *)GetDlgItem(IDC_CHECK_CURYEAR))->SetCheck(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDBList::OnDblistState0() 
{
	// TODO: Add your command handler code here
	m_state0 = !m_state0;

	if(m_state0)
	{
		m_state1 = m_state2 = m_state3 = m_state4 = m_state5 = m_state6 = false;

		m_RecordList.m_WhereBetween_Cmd.Format(" where (bgzt IS NULL or bgzt < 1)");
	}
	else
	{
		if(!m_RecordList.m_WhereBetween_Cmd.IsEmpty())
		{
			m_RecordList.m_WhereBetween_Cmd.Format(" where medicalno='tianjinwen'");
		}
	}

	m_RecordList.ResetGridSize();
	m_RecordList.RedrawAll();
}

void CDBList::OnUpdateDblistState0(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
   pCmdUI->SetCheck(m_state0);	
}

void CDBList::OnDblistState1() 
{
	// TODO: Add your command handler code here
	m_state1 = !m_state1;

	if(m_state1)
	{
		m_state0 = m_state2 = m_state3 = m_state4 = m_state5 = m_state6 = false;

		m_RecordList.m_WhereBetween_Cmd.Format(" where bgzt IS NOT NULL and bgzt >= 1 and bgzt < 2");
	}
	else
	{
		if(!m_RecordList.m_WhereBetween_Cmd.IsEmpty())
		{
			m_RecordList.m_WhereBetween_Cmd.Format(" where medicalno='tianjinwen'");
		}
	}

	m_RecordList.ResetGridSize();
	m_RecordList.RedrawAll();
}

void CDBList::OnUpdateDblistState1(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
   pCmdUI->SetCheck(m_state1);	
}

void CDBList::OnDblistState2() 
{
	// TODO: Add your command handler code here
	m_state2 = !m_state2;

	if(m_state2)
	{
		m_state0 = m_state1 = m_state3 = m_state4 = m_state5 = m_state6 = false;

		m_RecordList.m_WhereBetween_Cmd.Format(" where bgzt IS NOT NULL and bgzt >= 2 and bgzt < 4");
	}
	else
	{
		if(!m_RecordList.m_WhereBetween_Cmd.IsEmpty())
		{
			m_RecordList.m_WhereBetween_Cmd.Format(" where medicalno='tianjinwen'");
		}
	}

	m_RecordList.ResetGridSize();
	m_RecordList.RedrawAll();
}

void CDBList::OnUpdateDblistState2(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
   pCmdUI->SetCheck(m_state2);	
}

void CDBList::OnDblistState3() 
{
	// TODO: Add your command handler code here
	m_state3 = !m_state3;

	if(m_state3)
	{
		m_state0 = m_state1 = m_state2 = m_state4 = m_state5 = m_state6 = false;

		m_RecordList.m_WhereBetween_Cmd.Format(" where bgzt IS NOT NULL and bgzt >= 4 and bgzt < 8");
	}
	else
	{
		if(!m_RecordList.m_WhereBetween_Cmd.IsEmpty())
		{
			m_RecordList.m_WhereBetween_Cmd.Format(" where medicalno='tianjinwen'");
		}
	}

	m_RecordList.ResetGridSize();
	m_RecordList.RedrawAll();
}

void CDBList::OnUpdateDblistState3(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
   pCmdUI->SetCheck(m_state3);	
}

void CDBList::OnDblistState4() 
{
	// TODO: Add your command handler code here
	m_state4 = !m_state4;

	if(m_state4)
	{
		m_state0 = m_state1 = m_state2 = m_state3 = m_state5 = m_state6 = false;

		m_RecordList.m_WhereBetween_Cmd.Format(" where bgzt IS NOT NULL and bgzt >= 8 and bgzt < 16");
	}
	else
	{
		if(!m_RecordList.m_WhereBetween_Cmd.IsEmpty())
		{
			m_RecordList.m_WhereBetween_Cmd.Format(" where medicalno='tianjinwen'");
		}
	}

	m_RecordList.ResetGridSize();
	m_RecordList.RedrawAll();
}

void CDBList::OnUpdateDblistState4(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
   pCmdUI->SetCheck(m_state4);	
}

void CDBList::OnDblistState5() 
{
	// TODO: Add your command handler code here
	m_state5 = !m_state5;

	if(m_state5)
	{
		m_state0 = m_state1 = m_state2 = m_state3 = m_state4 = m_state6 = false;

		m_RecordList.m_WhereBetween_Cmd.Format(" where bgzt IS NOT NULL and bgzt >= 16");
	}
	else
	{
		if(!m_RecordList.m_WhereBetween_Cmd.IsEmpty())
		{
			m_RecordList.m_WhereBetween_Cmd.Format(" where medicalno='tianjinwen'");
		}
	}

	m_RecordList.ResetGridSize();
	m_RecordList.RedrawAll();
}

void CDBList::OnUpdateDblistState5(CCmdUI* pCmdUI) 
{
   pCmdUI->SetCheck(m_state5);	
}

void CDBList::OnDblistState6() 
{
	m_state6 = !m_state6;

	if(m_state6)
	{
		m_state0 = m_state1 = m_state2 = m_state3 = m_state4 = m_state5 = false;

		COleDateTime curt = theApp.GetOleServerDateTime();
		COleDateTimeSpan tspan;
		tspan.SetDateTimeSpan(7, 0, 0, 0);
		curt -= tspan;
		m_RecordList.m_WhereBetween_Cmd.Format(" where sjrq >= '%s'", curt.Format("%Y-%m-%d 00:00:00"));
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			m_RecordList.m_WhereBetween_Cmd.Format(" where sjrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss')", curt.Format("%Y-%m-%d 00:00:00"));
		}
	}
	else
	{
		if(!m_RecordList.m_WhereBetween_Cmd.IsEmpty())
		{
			m_RecordList.m_WhereBetween_Cmd.Format(" where medicalno='tianjinwen'");
		}
	}

	m_RecordList.ResetGridSize();
	m_RecordList.RedrawAll();
}

void CDBList::OnUpdateDblistState6(CCmdUI* pCmdUI) 
{
   pCmdUI->SetCheck(m_state6);	
}

void CDBList::OnButtonBlh() 
{
	int index = ((CComboBox *)GetDlgItem(IDC_COMBO_BLH))->GetCurSel();
	CString nContent;
	GetDlgItemText(IDC_EDIT_BLH, nContent);
	nContent.TrimLeft(); nContent.TrimRight();
	if(nContent.IsEmpty())  return;

	bool IsBlurMode = false;
	if(((CButton *)GetDlgItem(IDC_CHECK_BLUR))->GetState() & 0x0003)
	{
		IsBlurMode = true;
	}

	CString dlrq_Confine; dlrq_Confine.Empty();
	if(((CButton *)GetDlgItem(IDC_CHECK_CURYEAR))->GetState() & 0x0003)
	{
		COleDateTime curt = theApp.GetOleServerDateTime();
		COleDateTime uptime;
		uptime.SetDateTime(curt.GetYear(), 1, 1, 0,0,0);
		CString upstr;
		upstr.Format("%04d-%02d-%02d %02d:%02d:%02d",uptime.GetYear(),uptime.GetMonth(),uptime.GetDay(),uptime.GetHour(),uptime.GetMinute(),uptime.GetSecond());

		dlrq_Confine.Format(" and dlrq IS NOT NULL and dlrq>=\'%s\'",upstr);
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			dlrq_Confine.Format(" and dlrq IS NOT NULL and dlrq>=to_date('%s','yyyy-mm-dd hh24:mi:ss')",upstr);
		}
	}

	SetDlgItemText(IDC_EDIT_BLH, "");

	//------------------------------------
	CString nDBType_Confine_Str;  nDBType_Confine_Str.Empty();
	if(!(((CButton *)GetDlgItem(IDC_CHECK_ALLDB))->GetState() & 0x0003))
	{
		nDBType_Confine_Str.Format(" and dbtype=%d", theApp.m_DB_Type_No);
	}
	CString nCombine_DBType_dlrq = nDBType_Confine_Str + dlrq_Confine;
	//------------------------------------

	int num = 0;
	try
	{
		SAString cmdstr;
		switch(index) {
		case 0:
			if(IsBlurMode)
				cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where medicalno like '%" + nContent + "%'" + nCombine_DBType_dlrq;
			else
				cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where medicalno = '" + nContent + "'" + nCombine_DBType_dlrq;
			break;
		case 1:
			if(IsBlurMode)
				cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where zyh like '%" + nContent + "%'" + nCombine_DBType_dlrq;
			else
				cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where zyh = '" + nContent + "'" + nCombine_DBType_dlrq;
			break;
		case 2:
			if(IsBlurMode)
				cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where mzh like '%" + nContent + "%'" + nCombine_DBType_dlrq;
			else
				cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where mzh = '" + nContent + "'" + nCombine_DBType_dlrq;
			break;
		case 3:
			if(IsBlurMode)
				cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where bdh like '%" + nContent + "%'" + nCombine_DBType_dlrq;
			else
				cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where bdh = '" + nContent + "'" + nCombine_DBType_dlrq;
			break;
		case 4:
			if(IsBlurMode)
				cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where xm like '%" + nContent + "%'" + nCombine_DBType_dlrq;
			else
				cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where xm = '" + nContent + "'" + nCombine_DBType_dlrq;
			break;
		case 5:
			if(IsBlurMode)
				cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where sqh like '%" + nContent + "%'" + nCombine_DBType_dlrq;
			else
				cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where sqh = '" + nContent + "'" + nCombine_DBType_dlrq;
			break;
		case 6:
			if(IsBlurMode)
				cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where brid like '%" + nContent + "%'" + nCombine_DBType_dlrq;
			else
				cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where brid = '" + nContent + "'" + nCombine_DBType_dlrq;
			break;
		}
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			num = g_dbcommand.Field(1).asLong();
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

	//------------------------------------------------------------
	if(num <= 0 && index == 0 && nContent.GetLength() > 2)
	{
		nContent = nContent.Left(nContent.GetLength() - 2);
		try
		{
			SAString cmdstr;
			if(IsBlurMode)
				cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where medicalno like '%" + nContent + "%'" + nCombine_DBType_dlrq;
			else
				cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where medicalno = '" + nContent + "'" + nCombine_DBType_dlrq;

			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				num = g_dbcommand.Field(1).asLong();
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
	//------------------------------------------------------------
	
	if(num <= 0)
	{
		switch(index) {
		case 0:
			AfxMessageBox("数据库中不存在指定活检编号的登陆记录");
			break;
		case 1:
			AfxMessageBox("数据库中不存在指定住院号的登陆记录");
			break;
		case 2:
			AfxMessageBox("数据库中不存在指定门诊号的登陆记录");
			break;
		case 3:
			AfxMessageBox("数据库中不存在指定冰冻号的登陆记录");
			break;
		case 4:
			AfxMessageBox("数据库中不存在指定姓名的登陆记录");
			break;
		case 5:
			AfxMessageBox("数据库中不存在指定申请号的登陆记录");
			break;
		case 6:
			AfxMessageBox("数据库中不存在指定病人ID号的登陆记录");
			break;
		}
		
		this->SetFocus();
		GetDlgItem(IDC_EDIT_BLH)->SetFocus();
		return;
	}

	if(num > 1)
	{
		switch(index) {
		case 0:
			if(IsBlurMode)
				m_RecordList.m_WhereBetween_Cmd.Format(" where medicalno like \'%%%s%%\'", nContent);
			else
				m_RecordList.m_WhereBetween_Cmd.Format(" where medicalno = \'%s\'", nContent);
			break;
		case 1:
			if(IsBlurMode)
				m_RecordList.m_WhereBetween_Cmd.Format(" where zyh like \'%%%s%%\'", nContent);
			else
				m_RecordList.m_WhereBetween_Cmd.Format(" where zyh = \'%s\'", nContent);
			break;
		case 2:
			if(IsBlurMode)
				m_RecordList.m_WhereBetween_Cmd.Format(" where mzh like \'%%%s%%\'", nContent);
			else
				m_RecordList.m_WhereBetween_Cmd.Format(" where mzh = \'%s\'", nContent);
			break;
		case 3:
			if(IsBlurMode)
				m_RecordList.m_WhereBetween_Cmd.Format(" where bdh like \'%%%s%%\'", nContent);
			else
				m_RecordList.m_WhereBetween_Cmd.Format(" where bdh = \'%s\'", nContent);
			break;
		case 4:
			if(IsBlurMode)
				m_RecordList.m_WhereBetween_Cmd.Format(" where xm like \'%%%s%%\'", nContent);
			else
				m_RecordList.m_WhereBetween_Cmd.Format(" where xm = \'%s\'", nContent);
			break;
		case 5:
			if(IsBlurMode)
				m_RecordList.m_WhereBetween_Cmd.Format(" where hisid like \'%%%s%%\'", nContent);
			else
				m_RecordList.m_WhereBetween_Cmd.Format(" where hisid = \'%s\'", nContent);
			break;
		case 6:
			if(IsBlurMode)
				m_RecordList.m_WhereBetween_Cmd.Format(" where brid like \'%%%s%%\'", nContent);
			else
				m_RecordList.m_WhereBetween_Cmd.Format(" where brid = \'%s\'", nContent);
			break;
		}

		m_RecordList.m_WhereBetween_Cmd += dlrq_Confine;

		m_state0 = false;
		m_state1 = false;
		m_state2 = false;
		m_state3 = false;
		m_state4 = false;
		m_state5 = false;
		m_state6 = false;

		m_RecordList.ResetGridSize();
		m_RecordList.RedrawAll();
		m_RecordList.RedrawAll();
		
		return;
	}

	ASSERT(num == 1);

	CString blh; blh.Empty();
	try
	{
		SAString cmdstr;

		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			switch(index) {
			case 0:
				if(IsBlurMode)
					cmdstr = "Select medicalno from " + theApp.TABLE_MAIN + " where ROWNUM<=1 and medicalno like '%" + nContent + "%'" + nCombine_DBType_dlrq;
				else
					cmdstr = "Select medicalno from " + theApp.TABLE_MAIN + " where ROWNUM<=1 and medicalno = '" + nContent + "'" + nCombine_DBType_dlrq;
				break;
			case 1:
				if(IsBlurMode)
					cmdstr = "Select medicalno from " + theApp.TABLE_MAIN + " where ROWNUM<=1 and zyh like '%" + nContent + "%'" + nCombine_DBType_dlrq;
				else
					cmdstr = "Select medicalno from " + theApp.TABLE_MAIN + " where ROWNUM<=1 and zyh = '" + nContent + "'" + nCombine_DBType_dlrq;
				break;
			case 2:
				if(IsBlurMode)
					cmdstr = "Select medicalno from " + theApp.TABLE_MAIN + " where ROWNUM<=1 and mzh like '%" + nContent + "%'" + nCombine_DBType_dlrq;
				else
					cmdstr = "Select medicalno from " + theApp.TABLE_MAIN + " where ROWNUM<=1 and mzh = '" + nContent + "'" + nCombine_DBType_dlrq;
				break;
			case 3:
				if(IsBlurMode)
					cmdstr = "Select medicalno from " + theApp.TABLE_MAIN + " where ROWNUM<=1 and bdh like '%" + nContent + "%'" + nCombine_DBType_dlrq;
				else
					cmdstr = "Select medicalno from " + theApp.TABLE_MAIN + " where ROWNUM<=1 and bdh = '" + nContent + "'" + nCombine_DBType_dlrq;
				break;
			case 4:
				if(IsBlurMode)
					cmdstr = "Select medicalno from " + theApp.TABLE_MAIN + " where ROWNUM<=1 and xm like '%" + nContent + "%'" + nCombine_DBType_dlrq;
				else
					cmdstr = "Select medicalno from " + theApp.TABLE_MAIN + " where ROWNUM<=1 and xm = '" + nContent + "'" + nCombine_DBType_dlrq;
				break;
			case 5:
				if(IsBlurMode)
					cmdstr = "Select medicalno from " + theApp.TABLE_MAIN + " where ROWNUM<=1 and hisid like '%" + nContent + "%'" + nCombine_DBType_dlrq;
				else
					cmdstr = "Select medicalno from " + theApp.TABLE_MAIN + " where ROWNUM<=1 and hisid = '" + nContent + "'" + nCombine_DBType_dlrq;
				break;
			case 6:
				if(IsBlurMode)
					cmdstr = "Select medicalno from " + theApp.TABLE_MAIN + " where ROWNUM<=1 and brid like '%" + nContent + "%'" + nCombine_DBType_dlrq;
				else
					cmdstr = "Select medicalno from " + theApp.TABLE_MAIN + " where ROWNUM<=1 and brid = '" + nContent + "'" + nCombine_DBType_dlrq;
				break;
			}
		}
		else
		{
			switch(index) {
			case 0:
				if(IsBlurMode)
					cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where medicalno like '%" + nContent + "%'" + nCombine_DBType_dlrq;
				else
					cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where medicalno = '" + nContent + "'" + nCombine_DBType_dlrq;
				break;
			case 1:
				if(IsBlurMode)
					cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where zyh like '%" + nContent + "%'" + nCombine_DBType_dlrq;
				else
					cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where zyh = '" + nContent + "'" + nCombine_DBType_dlrq;
				break;
			case 2:
				if(IsBlurMode)
					cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where mzh like '%" + nContent + "%'" + nCombine_DBType_dlrq;
				else
					cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where mzh = '" + nContent + "'" + nCombine_DBType_dlrq;
				break;
			case 3:
				if(IsBlurMode)
					cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where bdh like '%" + nContent + "%'" + nCombine_DBType_dlrq;
				else
					cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where bdh = '" + nContent + "'" + nCombine_DBType_dlrq;
				break;
			case 4:
				if(IsBlurMode)
					cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where xm like '%" + nContent + "%'" + nCombine_DBType_dlrq;
				else
					cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where xm = '" + nContent + "'" + nCombine_DBType_dlrq;
				break;
			case 5:
				if(IsBlurMode)
					cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where hisid like '%" + nContent + "%'" + nCombine_DBType_dlrq;
				else
					cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where hisid = '" + nContent + "'" + nCombine_DBType_dlrq;
				break;
			case 6:
				if(IsBlurMode)
					cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where brid like '%" + nContent + "%'" + nCombine_DBType_dlrq;
				else
					cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where brid = '" + nContent + "'" + nCombine_DBType_dlrq;
				break;
			}
		}
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			blh = g_dbcommand.Field("medicalno").asString();
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

	theApp.OpenDocumentFile(blh);

	((CEdit *)GetDlgItem(IDC_EDIT_BLH))->SetFocus();
}

BOOL CDBList::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_hWnd)
	{
		if(pMsg->message == WM_MOUSEMOVE)
		{
			CPoint pt;
			GetCursorPos(&pt);
			CRect rt;

			m_ctrl_wfyy.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_wfyy.did(TRUE);
			}
			else
			{
				m_ctrl_wfyy.did(FALSE);
			}
		}


		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		{
			OnButtonBlh();
			return FALSE;
		}
	}
	
	return CXTCBarDialog::PreTranslateMessage(pMsg);
}

LRESULT CDBList::OnContentSave(WPARAM wParam, LPARAM lParam)
{
	if(!m_isModified)  return 0;

	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  return 0;

	theApp.ConnectDatabase();
	
	int m_bgzt = 0;
	
	try
	{
		SAString cmdstr = "Select TOP 1 bgzt from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			cmdstr = "Select bgzt from " + theApp.TABLE_MAIN + " where medicalno = :medicalno and ROWNUM<=1";
		}
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			m_bgzt = g_dbcommand.Field("bgzt").asLong();
		}
		else
		{
			//  在数据库中不存在指定活检编号的病例记录
			return 0;
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

	if(m_bgzt & 0x08)	return 0;
	
	UpdateData(TRUE);

	m_wfyy.TrimLeft(); m_wfyy.TrimRight();
	
	// save other field
	bool IsPerformNotify = false;
	try
	{
		CString fieldstr;
		fieldstr.Empty();
		fieldstr += "wfyy=:wfyy";
		if((m_bgzt&2) == 0 && !m_wfyy.IsEmpty())
		{
			fieldstr += ",bgzt=:bgzt";
			IsPerformNotify = true;
		}
		SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set " + fieldstr + " where medicalno = :medicalno";
		
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Param("wfyy").setAsString() = m_wfyy;
		if((m_bgzt&2) == 0 && !m_wfyy.IsEmpty())
		{
			m_bgzt = (m_bgzt | 0x02);
			g_dbcommand.Param("bgzt").setAsLong() = m_bgzt;
		}
		
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

	if(theApp.m_HisCfg.CompareNoCase("通用接口") == 0 && IsPerformNotify)
	{
		theApp.NotifyRecordStatus(theApp.GetView()->GetDocument()->m_blh);
	}

	m_isModified = false;

	return 1;
}

LRESULT CDBList::OnContentRefresh(WPARAM wParam, LPARAM lParam)
{
	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		m_wfyy.Empty();
	}
	else
	{
		theApp.ConnectDatabase();
		try
		{
			SAString cmdstr = "Select TOP 1 wfyy from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				cmdstr = "Select wfyy from " + theApp.TABLE_MAIN + " where medicalno = :medicalno and ROWNUM<=1";
			}
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			
			g_dbcommand.Execute();
			
			if(g_dbcommand.FetchNext())
			{
				m_wfyy = g_dbcommand.Field("wfyy").asString();
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

	UpdateData(FALSE);

	m_isModified = false;

	return 1;
}

void CDBList::OnHypLsjc() 
{
	// TODO: Add your control notification handler code here
	CHistoryShow mydlg(this);

	CString id, ybh, xm, xb;
	id.Empty();
	ybh.Empty();
	xm.Empty();
	xb.Empty();

	theApp.ConnectDatabase();
	try
	{
		SAString cmdstr = "Select TOP 1 brid,ybh,xm,xb from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			cmdstr = "Select brid,ybh,xm,xb from " + theApp.TABLE_MAIN + " where medicalno = :medicalno and ROWNUM<=1";
		}
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
		g_dbcommand.Execute();
		
		if(g_dbcommand.FetchNext())
		{
			id  = g_dbcommand.Field("brid").asString();
			ybh = g_dbcommand.Field("ybh").asString();
			xm  = g_dbcommand.Field("xm").asString();
			xb  = g_dbcommand.Field("xb").asString();
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
	id.TrimLeft();   id.TrimRight();
	ybh.TrimLeft();  ybh.TrimRight();
	xm.TrimLeft();   xm.TrimRight();
	xb.TrimLeft();   xb.TrimRight();

	CString cmdstr, str;
	cmdstr.Format(" where medicalno != '%s' and (medicalno = '%s' ", theApp.GetView()->GetDocument()->m_blh, ybh);
	if(!id.IsEmpty())
	{
		str.Format(" or brid = '%s'", id);
		cmdstr += str;
	}
	
	if(theApp.m_LsjcMode.CompareNoCase("是") == 0)
	{
		if(!xm.IsEmpty())
		{
			if(!xb.IsEmpty())
			{
				str.Format(" or (xm = '%s' and xb = '%s')", xm, xb);
				cmdstr += str;
			}
			else
			{
				str.Format(" or xm = '%s'", xm);
				cmdstr += str;
			}
		}
	}
			
	cmdstr += ")";

	mydlg.m_grid.m_WhereBetween_Cmd = cmdstr;
	mydlg.DoModal();
}

void CDBList::OnHypBqlb() 
{
	// TODO: Add your control notification handler code here
	CBqlbShow mydlg(this);
	mydlg.DoModal();

	RefreshHyperLink();
}

void CDBList::RefreshHyperLink()
{
	theApp.ConnectDatabase();

	CString str;

	int bqlb_no = 0;
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_ADVICE + " where yzlx = '补取' and yzzt = 0";
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			bqlb_no = g_dbcommand.Field(1).asLong();
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
	
	str.Format("补取列表: %d 例", bqlb_no);
	
	m_HypLink_Bqlb.SetWindowText(str);
	m_HypLink_Bqlb.Invalidate();

	//-----------------------------
	COleDateTime curt = theApp.GetOleServerDateTime();
	COleDateTimeSpan tspan;
	tspan.SetDateTimeSpan(15, 0, 0, 0);
	curt -= tspan;

	CString m_WhereBetween_Cmd;
	m_WhereBetween_Cmd.Format(" where medicalno IN (select DISTINCT medicalno from %s where inputdate >= '%s')", theApp.TABLE_HZ, curt.Format("%Y-%m-%d 00:00:00"));

	if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
	{
		m_WhereBetween_Cmd.Format(" where medicalno IN (select DISTINCT medicalno from %s where inputdate >= to_date('%s','yyyy-mm-dd hh24:mi:ss'))", theApp.TABLE_HZ, curt.Format("%Y-%m-%d 00:00:00"));
	}

	int Hzlb_no = 0;
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + m_WhereBetween_Cmd;
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
		{
			Hzlb_no = g_dbcommand.Field(1).asLong();
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

	str.Format("会诊病例: %d 例", Hzlb_no);

	m_HypLink_Hzbl.SetWindowText(str);
	m_HypLink_Hzbl.Invalidate();
	//-----------------------------

	m_WhereBetween_Cmd.Format(" where jhsfsj IS NOT NULL and jhsfsj > '1949-10-01 00:00:00'");
	if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
	{
		m_WhereBetween_Cmd.Format(" where jhsfsj IS NOT NULL and jhsfsj > to_date('1949-10-01 00:00:00','yyyy-mm-dd hh24:mi:ss')");
	}

	int Sflb_no = 0;
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + m_WhereBetween_Cmd;
		g_dbcommand.setCommandText(cmdstr);
		//g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
		
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
		{
			Sflb_no = g_dbcommand.Field(1).asLong();
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

	curt = theApp.GetOleServerDateTime();
	m_WhereBetween_Cmd.Format(" where jhsfsj IS NOT NULL and jhsfsj > '1949-10-01 00:00:00' and jhsfsj <= '%s'", curt.Format("%Y-%m-%d 23:59:59"));
	if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
	{
		m_WhereBetween_Cmd.Format(" where jhsfsj IS NOT NULL and jhsfsj > to_date('1949-10-01 00:00:00','yyyy-mm-dd hh24:mi:ss') and jhsfsj <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", curt.Format("%Y-%m-%d 23:59:59"));
	}

	int Sflb_no_Reach = 0;
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + m_WhereBetween_Cmd;
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
		{
			Sflb_no_Reach = g_dbcommand.Field(1).asLong();
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

	str.Format("随访病例: %d/%d", Sflb_no_Reach, Sflb_no);

	m_HypLink_Sfbl.SetWindowText(str);
	m_HypLink_Sfbl.Invalidate();
	//-----------------------------

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

	str.Format("短信: %d/%d", UnReadCount, NoteCount);

	if(UnReadCount > 0)
	{
		theApp.m_UnRead_Knly_Notified.Format("您当前有%d条未读短信！", UnReadCount);
	}

	m_HypLink_Knly.SetWindowText(str);
	m_HypLink_Knly.Invalidate();

	//-----------------------------
	int bdyynum = 0;
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_BOOKING + " where (state = '1' or state = '2') and bdjg is null";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			bdyynum = g_dbcommand.Field(1).asLong();
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
		//AfxMessageBox((const char*)x.ErrText());
		//return;
	}
	str.Format("冰冻预约: %d 例", bdyynum);
	m_HypLink_Bdyy.SetWindowText(str);
	m_HypLink_Bdyy.Invalidate();
}

void CDBList::OnButtonRefresh() 
{
	RefreshHyperLink();

	// added on 2008.05.22  手动刷新记录列表
	m_RecordList.ResetGridSize();
	m_RecordList.RedrawAll();
	m_RecordList.RedrawAll();
}

void CDBList::OnHypHzbl() 
{
	// TODO: Add your control notification handler code here
	CHzlbShow mydlg(this);
	mydlg.DoModal();
}

void CDBList::OnHypSfbl() 
{
	// TODO: Add your control notification handler code here
	CSflbShow mydlg(this);
	mydlg.DoModal();
}

void CDBList::OnDestroy() 
{
	CStdioFile File;
	if ( File.Open(theApp.m_Exe_Working_Dir + "system\\cpi_grid.siz", CFile::modeWrite |CFile::modeCreate | CFile::typeText) )
	{
		CString tmp;
		for(int i = 0; i < m_RecordList.m_DefineRecordnum; i++ )
		{
            tmp.Format("%d",m_RecordList.GetColWidth(i));
			File.WriteString(tmp);
			File.WriteString("\n");
		}
		File.Close();
	}

	CIniReader m_IniReader;
	m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");
	if(((CButton *)GetDlgItem(IDC_CHECK_CURYEAR))->GetState() & 0x0003)
		m_IniReader.setKey("是","本年检索条件有效","Settings");
	else
		m_IniReader.setKey("否","本年检索条件有效","Settings");

	CXTCBarDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CDBList::OnHypKnly() 
{
	CKnlyShow mydlg(this);
	mydlg.DoModal();

	RefreshHyperLink();
}

void CDBList::OnContentModified() 
{
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh)) return;

	m_isModified = true;
}

void CDBList::OnUpdateDatesel(CCmdUI* pCmdUI) 
{
}

BOOL CDBList::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	static bool bOK = true;
	LPNMSELCHANGE lpNMSelChange = (LPNMSELCHANGE) lParam;
	LPNMHDR lpnmhdr = &lpNMSelChange->nmhdr;
	if(lpnmhdr->code == DTN_DATETIMECHANGE)
	{
		CXTMonthCalCtrl *pMoCalCtrl = m_monthCal.GetMonthCalCtrl();
		if(pMoCalCtrl != NULL)  bOK = !bOK;

		if(bOK)
		{
			COleDateTime m_time;
			m_monthCal.GetTime(m_time);
			
			m_state0 = m_state1 = m_state2 = m_state3 = m_state4 = m_state5 = m_state6 = false;
			
			m_RecordList.m_WhereBetween_Cmd.Format(" where sjrq >= '%s' and sjrq <= '%s'", m_time.Format("%Y-%m-%d 00:00:00"), m_time.Format("%Y-%m-%d 23:59:59"));
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				m_RecordList.m_WhereBetween_Cmd.Format(" where sjrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and sjrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", m_time.Format("%Y-%m-%d 00:00:00"), m_time.Format("%Y-%m-%d 23:59:59"));
			}
			
			m_RecordList.ResetGridSize();
			m_RecordList.RedrawAll();
		}
	}
	
	return CXTCBarDialog::OnNotify(wParam, lParam, pResult);
}

void CDBList::OnHypBdyy() 
{
	CBdyyShow mydlg(this);
	mydlg.DoModal();
}

void CDBList::GetLeaveMessage(){
	OnHypKnly();
}

void CDBList::GetBooking(){
	OnHypBdyy();
}

void CDBList::openLcjc()
{
	OnHypLsjc();
}
