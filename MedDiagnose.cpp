// CMedDiagnose : implementation file
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "Pathology.h"
#include "MainFrm.h"
#include "PathologyView.h"
#include "PathologyDoc.h"
#include "MedDiagnose.h"
#include "StrEdit.h"
#include "Figure.h"
#include "KSFileDialog.h"
#include "NormalWord.h"
#include "SelectBZ.h"
#include "Xgyj.h"
#include "Bdbg.h"
#include "Bcbg.h"
#include "Sfjg.h"
#include "Knhz.h"
#include "Qcmx.h"
#include "Jsyz.h"
#include "Tjyz.h"
#include "Qppj.h"
#include "INI.h"
#include "MedImage.h"
#include "Zdex.h"
#include "MedPerson.h"

#include "CannedTextTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMedDiagnose
extern CPathologyApp theApp;

CMedDiagnose::CMedDiagnose()
	: CXTResizeFormView(CMedDiagnose::IDD)
{
	//{{AFX_DATA_INIT(CMedDiagnose)
	m_rysj = _T("");
	m_gjsj = _T("");
	//m_zdyj = _T("");
	m_zdbm = _T("");
	m_zdgjc = _T("");
	m_sfyx = _T("");
	m_zdfh = _T("");
	m_czys = _T("");
	m_zzys = _T("");
	m_shys = _T("");
	m_bggs = _T("请选择报告格式");
	//}}AFX_DATA_INIT

	CurEdit = 0;	
	pNormalWord = NULL;
	pListInput = NULL;
	pZdex = NULL;

	m_bIsFirstInit = true;

}

CMedDiagnose::~CMedDiagnose()
{
	// TODO: add destruction code here.

}

IMPLEMENT_DYNCREATE(CMedDiagnose, CXTResizeFormView)

BEGIN_MESSAGE_MAP(CMedDiagnose, CXTResizeFormView)
	//{{AFX_MSG_MAP(CMedDiagnose)
	ON_BN_CLICKED(IDC_BUTTON_CYC, OnButtonCyc)
	ON_WM_DESTROY()
	ON_EN_SETFOCUS(IDC_EDIT_RYSJ, OnSetfocusEditRysj)
	ON_EN_SETFOCUS(IDC_EDIT_GJSJ, OnSetfocusEditGjsj)
	ON_EN_SETFOCUS(IDC_EDIT_ZDYJ, OnSetfocusEditZdyj)
	ON_BN_CLICKED(IDC_BUTTON_ZDMB, OnButtonZdmb)
	ON_BN_CLICKED(IDC_BUTTON_BC, OnButtonBc)
	ON_BN_CLICKED(IDC_BUTTON_XGYJ, OnButtonXgyj)
	ON_BN_CLICKED(IDC_BUTTON_BDBG, OnButtonBdbg)
	ON_BN_CLICKED(IDC_BUTTON_BCBG, OnButtonBcbg)
	ON_BN_CLICKED(IDC_BUTTON_SFJG, OnButtonSfjg)
	ON_BN_CLICKED(IDC_BUTTON_KNHZ, OnButtonKnhz)
	ON_BN_CLICKED(IDC_BUTTON_QCMX, OnButtonQcmx)
	ON_BN_CLICKED(IDC_BUTTON_JSYZ, OnButtonJsyz)
	ON_BN_CLICKED(IDC_BUTTON_TJYZ, OnButtonTjyz)
	ON_BN_CLICKED(IDC_BUTTON_QPPJ, OnButtonQppj)
	ON_BN_CLICKED(IDC_BUTTON_MICD10, OnButtonMicd10)
	ON_BN_CLICKED(IDC_BUTTON_SICD10, OnButtonSicd10)
	ON_EN_CHANGE(IDC_EDIT_RYSJ, OnContentModified)
	ON_BN_CLICKED(IDC_BUTTON_BLH, OnButtonBlh)
	ON_CBN_SETFOCUS(IDC_COMBO_BGGS, OnSetfocusComboBggs)
	ON_CBN_SELCHANGE(IDC_COMBO_BGGS, OnSelchangeComboBggs)
	ON_CBN_CLOSEUP(IDC_COMBO_BGGS, OnCloseupComboBggs)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	ON_NOTIFY(DTN_DATETIMECHANGE, 111,  OnDateTimeChange)
	ON_EN_CHANGE(IDC_EDIT_GJSJ, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_ZDYJ, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_ZDBM, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_ZDGJC, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_SFYX, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_ZDFH, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_CZYS, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_ZZYS, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_SHYS, OnContentModified)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CONTENTSAVE, OnContentSave)
	ON_MESSAGE(WM_LISTSELECT,ListSelect)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMedDiagnose message handlers

void CMedDiagnose::DoDataExchange(CDataExchange* pDX) 
{
	CXTResizeFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMedDiagnose)
	DDX_Control(pDX, IDC_BUTTON_BLH, m_btBlh);
	DDX_Control(pDX, IDC_BUTTON_ZDMB, m_Button_Zdmb);
	DDX_Control(pDX, IDC_BUTTON_QPPJ, m_Button_Qppj);
	DDX_Control(pDX, IDC_BUTTON_CYC, m_Button_Cyc);
	DDX_Control(pDX, IDC_BUTTON_BC, m_Button_Bc);
	DDX_Control(pDX, IDC_BUTTON_QCMX, m_Button_Qcmx);
	DDX_Control(pDX, IDC_BUTTON_BDBG, m_Button_Bdbg);
	DDX_Control(pDX, IDC_BUTTON_XGYJ, m_Button_Xgyj);
	DDX_Control(pDX, IDC_BUTTON_KNHZ, m_Button_Knhz);
	DDX_Control(pDX, IDC_BUTTON_BCBG, m_Button_Bcbg);
	DDX_Control(pDX, IDC_BUTTON_SFJG, m_Button_Sfjg);
	DDX_Control(pDX, IDC_BUTTON_TJYZ, m_Button_Tjyz);
	DDX_Control(pDX, IDC_BUTTON_JSYZ, m_Button_Jsyz);
	DDX_Control(pDX, IDC_BUTTON_PRINT,m_Button_Print);

	DDX_Control(pDX, IDC_TREE_RYSJ, m_TreeCtrl_RYSJ);
	DDX_Control(pDX, IDC_TREE_GJSJ, m_TreeCtrl_GJSJ);
	DDX_Control(pDX, IDC_TREE_ZDYJ, m_TreeCtrl_ZDYJ);

	DDX_Control(pDX, IDC_EDIT_SHYS, m_ctrl_shys);
	DDX_Control(pDX, IDC_EDIT_ZZYS, m_ctrl_zzys);
	DDX_Control(pDX, IDC_EDIT_CZYS, m_ctrl_czys);
	DDX_Control(pDX, IDC_EDIT_ZDFH, m_ctrl_zdfh);
	DDX_Control(pDX, IDC_EDIT_SFYX, m_ctrl_sfyx);
	DDX_Control(pDX, IDC_EDIT_ZDYJ, m_ctrl_zdyj);
	DDX_Control(pDX, IDC_EDIT_GJSJ, m_ctrl_gjsj);
	DDX_Control(pDX, IDC_EDIT_RYSJ, m_ctrl_rysj); 
	DDX_Text(pDX, IDC_EDIT_RYSJ, m_rysj);
	DDX_Text(pDX, IDC_EDIT_GJSJ, m_gjsj);
	DDX_Text(pDX, IDC_EDIT_ZDYJ, m_zdyj);
	DDX_Text(pDX, IDC_EDIT_ZDBM, m_zdbm);
	DDX_Text(pDX, IDC_EDIT_ZDGJC, m_zdgjc);
	DDX_Text(pDX, IDC_EDIT_SFYX, m_sfyx);
	DDX_Text(pDX, IDC_EDIT_ZDFH, m_zdfh);
	DDX_Text(pDX, IDC_EDIT_CZYS, m_czys);
	DDX_Text(pDX, IDC_EDIT_ZZYS, m_zzys);
	DDX_Text(pDX, IDC_EDIT_SHYS, m_shys);
	DDX_CBString(pDX, IDC_COMBO_BGGS, m_bggs);
	//}}AFX_DATA_MAP
}

/*
mode
肉眼所见 1;
光镜所见 2;
诊断所见 3;
*/

void CMedDiagnose::UpdateTreeCtrl(int mode, BOOL bRebuildTree)
{
	switch(mode)
	{
	case 1:
		m_TreeCtrl_ZDYJ.ShowWindow(SW_HIDE);
		m_TreeCtrl_GJSJ.ShowWindow(SW_HIDE);
		m_TreeCtrl_RYSJ.ShowWindow(SW_SHOW);
		m_pCurrentTreeCtrl = &m_TreeCtrl_RYSJ;
		SetDlgItemText(IDC_STATIC_BWBZ, "词典－肉眼所见");
		break;
	case 2:
		m_TreeCtrl_RYSJ.ShowWindow(SW_HIDE);
		m_TreeCtrl_ZDYJ.ShowWindow(SW_HIDE);
		m_TreeCtrl_GJSJ.ShowWindow(SW_SHOW);
		m_pCurrentTreeCtrl = &m_TreeCtrl_GJSJ;
		SetDlgItemText(IDC_STATIC_BWBZ, "词典－光镜所见");
		break;
	case 3:
		m_TreeCtrl_RYSJ.ShowWindow(SW_HIDE);
		m_TreeCtrl_GJSJ.ShowWindow(SW_HIDE);
		m_TreeCtrl_ZDYJ.ShowWindow(SW_SHOW);
		m_pCurrentTreeCtrl = &m_TreeCtrl_ZDYJ;
		SetDlgItemText(IDC_STATIC_BWBZ, "词典－诊断意见");
		break;
	}

	if (bRebuildTree)
	{
		m_TreeCtrl_RYSJ.BuildTree();
		m_TreeCtrl_GJSJ.BuildTree();
		m_TreeCtrl_ZDYJ.BuildTree();
	}

	m_CurrentTreeMode = mode;
}


void CMedDiagnose::OnInitialUpdate() 
{
	CXTResizeFormView::OnInitialUpdate();
	

	/// TODO 打印报告底单，

	m_Button_Print.EnableWindow(FALSE);

	if (m_bIsFirstInit)
	{
		m_bIsFirstInit = false;

		pListInput = new CDlgListInput(this);
		pListInput->m_type = 1;  // 1 - MICD10, 2 - SICD10
		pListInput->Create();
		pListInput->SetActiveWindow();
		pListInput->ShowWindow(SW_HIDE);

		DWORD m_dwXTBtnStyle;
		m_dwXTBtnStyle = BS_XT_SHOWFOCUS;
		m_dwXTBtnStyle &= ~(BS_XT_SEMIFLAT | BS_XT_FLAT | BS_XT_XPFLAT);

		m_Button_Qcmx.SetXButtonStyle(m_dwXTBtnStyle);
		m_Button_Bdbg.SetXButtonStyle(m_dwXTBtnStyle);
		m_Button_Xgyj.SetXButtonStyle(m_dwXTBtnStyle);
		m_Button_Knhz.SetXButtonStyle(m_dwXTBtnStyle);
		m_Button_Bcbg.SetXButtonStyle(m_dwXTBtnStyle);
		m_Button_Sfjg.SetXButtonStyle(m_dwXTBtnStyle);
		m_Button_Tjyz.SetXButtonStyle(m_dwXTBtnStyle);
		m_Button_Jsyz.SetXButtonStyle(m_dwXTBtnStyle);
		m_Button_Print.SetXButtonStyle(m_dwXTBtnStyle);
		
		m_Button_Zdmb.SetXButtonStyle(m_dwXTBtnStyle);
		m_Button_Qppj.SetXButtonStyle(m_dwXTBtnStyle);
		m_Button_Cyc.SetXButtonStyle(m_dwXTBtnStyle);
		m_Button_Bc.SetXButtonStyle(m_dwXTBtnStyle);

		m_btBlh.SetXButtonStyle(m_dwXTBtnStyle);
		//--------------------------------------------------
		m_TreeCtrl_RYSJ.EnableMultiSelect(FALSE);
		m_TreeCtrl_GJSJ.EnableMultiSelect(FALSE);
		m_TreeCtrl_ZDYJ.EnableMultiSelect(FALSE);
	
		m_CurrentTreeMode = 0;
		UpdateTreeCtrl(m_CurrentTreeMode);

		m_ToolTip_RYSJ.Create(this);
		m_ToolTip_RYSJ.Activate(TRUE);
		m_ToolTip_RYSJ.AddTool(&m_TreeCtrl_RYSJ);
		m_ToolTip_RYSJ.SetMaxTipWidth(300);
		m_TreeCtrl_RYSJ.SetToolTips(&m_ToolTip_RYSJ);

		m_ToolTip_GJSJ.Create(this);
		m_ToolTip_GJSJ.Activate(TRUE);
		m_ToolTip_GJSJ.AddTool(&m_TreeCtrl_GJSJ);
		m_ToolTip_GJSJ.SetMaxTipWidth(300);
		m_TreeCtrl_GJSJ.SetToolTips(&m_ToolTip_GJSJ);

		m_ToolTip_ZDYJ.Create(this);
		m_ToolTip_ZDYJ.Activate(TRUE);
		m_ToolTip_ZDYJ.AddTool(&m_TreeCtrl_ZDYJ);
		m_ToolTip_ZDYJ.SetMaxTipWidth(300);
		m_TreeCtrl_ZDYJ.SetToolTips(&m_ToolTip_ZDYJ);


		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_IMGLIST_VIEW);
		
		m_ImageList.Create( 16, 16, ILC_COLOR24 | ILC_MASK, 1, 1 );
		m_ImageList.Add( &bitmap, RGB( 0x00,0xff,0x00 ) );

		m_TreeCtrl_RYSJ.Initialize(1, IDC_EDIT_RYSJ, this);
		m_TreeCtrl_GJSJ.Initialize(2, IDC_EDIT_GJSJ, this);
		m_TreeCtrl_ZDYJ.Initialize(3, IDC_EDIT_ZDYJ, this);	

		m_TreeCtrl_RYSJ.SetImageList(&m_ImageList, TVSIL_NORMAL);
		m_TreeCtrl_GJSJ.SetImageList(&m_ImageList, TVSIL_NORMAL);
		m_TreeCtrl_ZDYJ.SetImageList(&m_ImageList, TVSIL_NORMAL);
		
		m_CurrentTreeMode = 1;
		UpdateTreeCtrl(1);//

		// initialize cool menus
		m_coolMenu.HookWindow(m_hWnd);
		m_coolMenu.LoadToolbar(IDR_POPUP_TREE);

		//--------------------------------------------------
		
		CRect rt;
		GetDlgItem(IDC_DATE_BGRQ)->GetWindowRect(&rt);
		ScreenToClient(&rt);
		if (!m_Time_Bgrq.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_UPDOWN,
			rt, this, 111))
		{
			return;
		}

		m_Time_Bgrq.SetFormat(_T("yyyy-MM-dd HH':'mm"));
		m_Time_Bgrq.SetTime(theApp.GetOleServerDateTime());

		theApp.ConnectDatabase();
		
		for(int i = 0; i < theApp.m_fnamelist.GetSize(); i++)
		{
			if(theApp.m_fnamelist.GetAt(i).CompareNoCase("shys") == 0)
			{
				m_ctrl_shys.m_info = theApp.m_finfolist.GetAt(i);
			}
			if(theApp.m_fnamelist.GetAt(i).CompareNoCase("zzys") == 0)
			{
				m_ctrl_zzys.m_info = theApp.m_finfolist.GetAt(i);
			}
			if(theApp.m_fnamelist.GetAt(i).CompareNoCase("czys") == 0)
			{
				m_ctrl_czys.m_info = theApp.m_finfolist.GetAt(i);
			}
			if(theApp.m_fnamelist.GetAt(i).CompareNoCase("zdfh") == 0)
			{
				m_ctrl_zdfh.m_info = theApp.m_finfolist.GetAt(i);
			}
			if(theApp.m_fnamelist.GetAt(i).CompareNoCase("sfyx") == 0)
			{
				m_ctrl_sfyx.m_info = theApp.m_finfolist.GetAt(i);
			}
		}
		
		m_ctrl_shys.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_zzys.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_czys.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_zdfh.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_sfyx.SetBrowseStyle(BES_XT_POPUPMENU,0,0);

		RefreshBggs();
			
		return;
	}	

	//---------------------------------------------------
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		GetDlgItem(IDC_BUTTON_BC)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_BC)->EnableWindow(TRUE);
	}
	//---------------------------------------------------

	CPathologyView *pView = theApp.GetView();
	ASSERT (pView != NULL);

	if(pView->GetDocument()->m_IsNewRecord)
	{
		m_rysj = _T("");
		m_gjsj = _T("");
		m_zdyj = _T("");
		m_zdbm = _T("");
		m_zdgjc = _T("");
		m_sfyx = _T("");
		m_zdfh = _T("");
		m_czys = _T("");
		m_zzys = _T("");
		m_shys = _T("");
		m_bggs = _T("请选择报告格式");

		//--------------------------------------------
		COleDateTime curt = theApp.GetOleServerDateTime();

		SADateTime nBgrq;
		nBgrq = theApp.GetServerDateTime();
		try
		{
			SAString cmdstr = "Select TOP 1 bgrq from " + theApp.TABLE_MAIN + " where dbtype = :dbtype and dlrq IS NOT NULL order by dlrq desc";
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				cmdstr = "Select * from (Select bgrq from " + theApp.TABLE_MAIN + " where dbtype = :dbtype and dlrq IS NOT NULL order by dlrq desc) where ROWNUM<=1";
			}
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
			
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				if(!g_dbcommand.Field("bgrq").isNull())
				{
					nBgrq = g_dbcommand.Field("bgrq").asDateTime();
				}
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
		}

		if(theApp.m_BgrqTimeMode.CompareNoCase("服务器当前时间") == 0)
		{
			m_Time_Bgrq.SetTime(curt);
		}
		else
		{
			COleDateTime ct;
			ct.SetDateTime(nBgrq.GetYear(), nBgrq.GetMonth(), nBgrq.GetDay(), nBgrq.GetHour(), nBgrq.GetMinute(), nBgrq.GetSecond());
			m_Time_Bgrq.SetTime(ct);
		}
		//--------------------------------------------

		ResetButtonTextColor(true);
	}
	else
	{
		COleDateTime ServerTime = theApp.GetOleServerDateTime();
		try
		{
			CString fieldstr;
			fieldstr.Format("rysj,gjsj,zdyj,zdbm,zdgjc,sfyx,zdfh,czys,zzys,shys,bggs,bgrq");
			//SAString cmdstr = "Select * from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
			SAString cmdstr = "Select TOP 1 " + fieldstr + " from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				cmdstr = "Select " + fieldstr + " from " + theApp.TABLE_MAIN + " where medicalno = :medicalno and ROWNUM<=1";
			}
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = pView->GetDocument()->m_blh;
			
			g_dbcommand.Execute();
			
			if(g_dbcommand.FetchNext())
			{
				m_rysj = g_dbcommand.Field("rysj").asString();
				m_gjsj = g_dbcommand.Field("gjsj").asString();
				m_zdyj = g_dbcommand.Field("zdyj").asString();
				m_zdbm = g_dbcommand.Field("zdbm").asString();
				m_zdgjc = g_dbcommand.Field("zdgjc").asString();
				m_sfyx = g_dbcommand.Field("sfyx").asString();
				m_zdfh = g_dbcommand.Field("zdfh").asString();
				m_czys = g_dbcommand.Field("czys").asString();
				m_zzys = g_dbcommand.Field("zzys").asString();
				m_shys = g_dbcommand.Field("shys").asString();
				m_bggs = g_dbcommand.Field("bggs").asString();

				if(g_dbcommand.Field("bggs").isNull() || m_bggs.IsEmpty())
				{
					m_bggs.Format("请选择报告格式");
				}

				COleDateTime curt = ServerTime;
				SADateTime st = g_dbcommand.Field("bgrq").asDateTime();
				curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
				m_Time_Bgrq.SetTime(curt);
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
	
		ResetButtonTextColor(false);
	}

	UpdateData(FALSE);

	m_isModified = false;

	if(pView->GetDocument()->m_IsNewRecord)
	{
		m_isModified = true;
	}

	if(pZdex && pZdex->IsWindowVisible())
	{
		pZdex->AdjustPosition();
	}
}

BOOL CMedDiagnose::PreTranslateMessage(MSG* pMsg) 
{

	//?
	/*

	if(m_hWnd)
	{
		if(pMsg->message == WM_MOUSEMOVE)
		{
			CPoint pt;
			GetCursorPos(&pt);
			CRect rt;


			m_ctrl_shys.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_shys.did(TRUE);
			}
			else
			{
				m_ctrl_shys.did(FALSE);
			}
			m_ctrl_zzys.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_zzys.did(TRUE);
			}
			else
			{
				m_ctrl_zzys.did(FALSE);
			}
			m_ctrl_czys.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_czys.did(TRUE);
			}
			else
			{
				m_ctrl_czys.did(FALSE);
			}
			m_ctrl_zdfh.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_zdfh.did(TRUE);
			}
			else
			{
				m_ctrl_zdfh.did(FALSE);
			}
			m_ctrl_sfyx.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_sfyx.did(TRUE);
			}
			else
			{
				m_ctrl_sfyx.did(FALSE);
			}

			//---------------------------------
		//	((CComboBox *)GetDlgItem(IDC_COMBO_BGGS))->GetWindowRect(&rt);
		//	if(rt.PtInRect(pt))
		//	{
		//		CWnd *pActiveWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetActiveView();
		//		CWnd *pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CMedImage));
		//		if(pWnd != NULL && pWnd != pActiveWnd)  ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.SetActiveView(pWnd);
		//	}
			//---------------------------------
		}

		//--------------------------------
		int WndID[11] = {
			IDC_EDIT_RYSJ,
			IDC_EDIT_GJSJ,
			IDC_EDIT_ZDYJ,
			IDC_EDIT_ZDBM,
			IDC_EDIT_ZDGJC,
			IDC_EDIT_SFYX,
			IDC_EDIT_ZDFH,
			IDC_EDIT_CZYS,
			IDC_EDIT_ZZYS,
			IDC_EDIT_SHYS,
			IDC_DATE_BGRQ
		};
		int WndCount = 11;
		
		if(pMsg->message == WM_KEYDOWN)
		{
			if(pMsg->wParam == VK_TAB || pMsg->wParam == VK_RETURN)
			{
				bool IsSet = false;
				for(int i = 0; i < WndCount; i++ )
				{
					if( !GetDlgItem(WndID[i])->IsWindowVisible() ) continue;
					if( GetDlgItem(WndID[i]) == GetFocus() )  break;
				}
				if( i < (WndCount-1) )
				{
					if(!(pMsg->wParam == VK_RETURN && i < 3))
					{
						if( i < WndCount )
						{
							for(int j = i+1; j < WndCount; j++ )
							{
								if( GetDlgItem(WndID[j])->IsWindowVisible() ) break;
							}
							if( j < WndCount )
							{
								GetDlgItem(WndID[j])->SetFocus();
								IsSet = true;
							}
						}
						if(!IsSet)
						{
							int js = 0;
							for(int j = js; j < WndCount; j++)
							{
								if( GetDlgItem(WndID[j])->IsWindowVisible() ) break;
							}
							if( j < WndCount )
							{
								GetDlgItem(WndID[j])->SetFocus();
								IsSet = true;
							}
						}
					}
				}
				else
				{
					if( i == (WndCount-1) && pMsg->wParam == VK_RETURN )
					{
						pMsg->wParam = VK_TAB;
						pMsg->lParam = 0;
					}
				}
				if(IsSet)
					return TRUE;
			}
		}
		//--------------------------------

		m_ToolTip_RYSJ.RelayEvent(pMsg);
		m_ToolTip_GJSJ.RelayEvent(pMsg);
		m_ToolTip_ZDYJ.RelayEvent(pMsg);
	}

	*/
	//?
	
return CXTResizeFormView::PreTranslateMessage(pMsg);
}

LRESULT CMedDiagnose::OnContentSave(WPARAM wParam, LPARAM lParam)
{
	if(!m_isModified)  return 0;

	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  return 0;

	CPathologyView *pView = theApp.GetView();
	ASSERT (pView != NULL);

	theApp.ConnectDatabase();
	
	int m_bgzt = 0;
	CString xRysj, xGjsj, xZdyj, xCzys, xZzys, xCzyj, xXgyj,xWyyj;  
	xRysj.Empty(); xGjsj.Empty(); xZdyj.Empty(); xCzys.Empty(); xZzys.Empty(); xCzyj.Empty(); xXgyj.Empty();  
	
	try
	{
		SAString cmdstr = "Select TOP 1 bgzt, rysj, gjsj, zdyj, czys, zzys, czyj, xgyj,wyyj from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			cmdstr = "Select bgzt, rysj, gjsj, zdyj, czys, zzys, czyj, xgyj,wyyj from " + theApp.TABLE_MAIN + " where medicalno = :medicalno and ROWNUM<=1";
		}
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			m_bgzt = g_dbcommand.Field("bgzt").asLong();

			if(!g_dbcommand.Field("rysj").isNull())
			{
				xRysj = g_dbcommand.Field("rysj").asString();
			}

			if(!g_dbcommand.Field("gjsj").isNull())
			{
				xGjsj = g_dbcommand.Field("gjsj").asString();
			}

			if(!g_dbcommand.Field("zdyj").isNull())
			{
				xZdyj = g_dbcommand.Field("zdyj").asString();
			}
		
			if(!g_dbcommand.Field("czys").isNull())
			{
				xCzys = g_dbcommand.Field("czys").asString();
			}

			if(!g_dbcommand.Field("zzys").isNull())
			{
				xZzys = g_dbcommand.Field("zzys").asString();
			}

			if(!g_dbcommand.Field("czyj").isNull())
			{
				xCzyj = g_dbcommand.Field("czyj").asString();
			}

			if(!g_dbcommand.Field("xgyj").isNull())
			{
				xXgyj = g_dbcommand.Field("xgyj").asString();
			}

			if(!g_dbcommand.Field("wyyj").isNull())
			{
				xWyyj = g_dbcommand.Field("wyyj").asString();
			}
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

	//--------------------------------------------------------

	//-----------自动更新报告日期-----------------------------
	if(theApp.m_BgrqRefreshMode.CompareNoCase("是") == 0)
	{
		m_zdyj.TrimLeft();    m_zdyj.TrimRight();
		if(m_zdyj.CompareNoCase(xZdyj) != 0)
		{
			COleDateTime curt = theApp.GetOleServerDateTime();
			m_Time_Bgrq.SetTime(curt);
		}
	}
	else
	{
		xZdyj.TrimLeft();    xZdyj.TrimRight();
		if(xZdyj.IsEmpty() && !m_zdyj.IsEmpty())
		{
			COleDateTime curt = theApp.GetOleServerDateTime();
			m_Time_Bgrq.SetTime(curt);
		}
	}
	//--------------------------------------------------------
	CIniReader m_IniReader;
	m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");

	if(theApp.m_CzysMode.CompareNoCase("是") == 0 && m_czys.IsEmpty() && !m_zdyj.IsEmpty())
	{
		m_czys = m_IniReader.getKeyValue("初诊医生","Settings");
		SetDlgItemText(IDC_EDIT_CZYS, m_czys);
	}
	
	if(theApp.m_ZzysMode.CompareNoCase("是") == 0 && m_zzys.IsEmpty() && !m_zdyj.IsEmpty())
	{
		m_zzys = m_IniReader.getKeyValue("主诊医生","Settings");
		SetDlgItemText(IDC_EDIT_ZZYS, m_zzys);
	}

	if(m_czys.CompareNoCase(xCzys) && !m_czys.IsEmpty())
	{
		m_IniReader.setKey(m_czys,"初诊医生","Settings");
	}

	if(m_zzys.CompareNoCase(xZzys) && !m_zzys.IsEmpty())
	{
		m_IniReader.setKey(m_zzys,"主诊医生","Settings");
	}

	bool Rysj_Suspicious = false;
	bool Gjsj_Suspicious = false;
	bool Zdyj_Suspicious = false;

	m_rysj.TrimLeft(); m_rysj.TrimRight();
	m_gjsj.TrimLeft(); m_gjsj.TrimRight();
	m_zdyj.TrimLeft(); m_zdyj.TrimRight();

	if(m_rysj.IsEmpty() && !xRysj.IsEmpty())
	{
		Rysj_Suspicious = true;
	}

	if(m_gjsj.IsEmpty() && !xGjsj.IsEmpty())
	{
		Gjsj_Suspicious = true;
	}

	if(m_zdyj.IsEmpty() && !xZdyj.IsEmpty())
	{
		Zdyj_Suspicious = true;
	}

	if(Rysj_Suspicious || Gjsj_Suspicious || Zdyj_Suspicious)
	{
		CString value, nContent;
		value.Empty(); nContent.Empty();
		if(Rysj_Suspicious)
		{
			if(!nContent.IsEmpty()) nContent += "、";
			nContent += "“肉眼所见”";
		}
		if(Gjsj_Suspicious)
		{
			if(!nContent.IsEmpty()) nContent += "、";
			nContent += "“光镜所见”";
		}
		if(Zdyj_Suspicious)
		{
			if(!nContent.IsEmpty()) nContent += "、";
			nContent += "“诊断意见”";
		}
		value = "当前记录的" + nContent + "已在其它站点录入保存，是否覆盖录入内容？";
		value += "\n";
		value += "建议按“取消”键保护内容。";

		if( ((CMainFrame*)AfxGetMainWnd())->m_realtime )
		{
			((CMainFrame*)AfxGetMainWnd())->TerminateTimer();
		}

		if(AfxMessageBox(value, MB_OKCANCEL|MB_ICONQUESTION) == IDOK)
		{
			value = "当前记录的" + nContent + "将被清空，是否进行下一步操作？";

			if(AfxMessageBox(value, MB_OKCANCEL|MB_ICONQUESTION) == IDOK)
			{
				Rysj_Suspicious = Gjsj_Suspicious = Zdyj_Suspicious = false;
			}
		}
	}

	//----------------------------------------
	bool IsRefresh_Czyj = false;
	
	if(!m_czys.IsEmpty() && m_czys.Find(theApp.m_Cpp_User) !=-1 && xXgyj.IsEmpty()/*&& xCzyj.IsEmpty() && !m_zdyj.IsEmpty()*/)
	{
		IsRefresh_Czyj = true;
	}
	bool IsRefresh_Xgyj = false;
	
//	if(!m_zzys.IsEmpty() && m_zzys.Find(theApp.m_Cpp_User) !=-1 && m_shys.IsEmpty()/*&& xXgyj.IsEmpty() && !m_zdyj.IsEmpty()*/)
//	{
//		IsRefresh_Xgyj = true;
//	}
	// save other field
	try
	{
		CString fieldstr;
		fieldstr.Empty();
		if(!Rysj_Suspicious)
		{
			fieldstr += "rysj=:rysj,";
		}
		if(!Gjsj_Suspicious)
		{
			fieldstr += "gjsj=:gjsj,";
		}
		if(!Zdyj_Suspicious)
		{
			fieldstr += "zdyj=:zdyj,";
		/*	if(xZdyj.IsEmpty() || xXgyj.IsEmpty()){
				fieldstr += "zdyj=:zdyj,";
			} else {
			if(!m_shys.IsEmpty() && theApp.m_Cpp_User.Find(m_shys) != -1 ){
				fieldstr += "zdyj=:zdyj,";
			} else if(m_shys.IsEmpty() && theApp.m_Cpp_User.Find(m_zzys) != -1){
				fieldstr += "zdyj=:zdyj,";
			} else {
				
			}
			}*/
			
		}

		if(IsRefresh_Czyj)
		{
			fieldstr += "czyj=:czyj,";
		}

		if(IsRefresh_Xgyj)
		{
			fieldstr += "xgyj=:xgyj,";
		}

		if(theApp.m_Cpp_User.Find(m_shys) != -1 && !m_shys.IsEmpty())
		{
			fieldstr += "wyyj=:wyyj,";
		}

		if(theApp.m_Cpp_User.Find(m_zzys) >= 0 && xWyyj.IsEmpty() && !m_zzys.IsEmpty())
		{
			fieldstr += "xgyj=:xgyj,";
		}

		fieldstr += "zdbm=:zdbm,";
		fieldstr += "zdgjc=:zdgjc,";
		fieldstr += "sfyx=:sfyx,";
		fieldstr += "zdfh=:zdfh,";
		fieldstr += "czys=:czys,";
		fieldstr += "zzys=:zzys,";
		fieldstr += "shys=:shys,";
		fieldstr += "bgrq=:bgrq,";
		fieldstr += "bggs=:bggs";

		SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set " + fieldstr + " where medicalno = :medicalno";

		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = pView->GetDocument()->m_blh;
		if(!Rysj_Suspicious)
		{
			g_dbcommand.Param("rysj").setAsString() = m_rysj;
		}
		if(!Gjsj_Suspicious)
		{
			g_dbcommand.Param("gjsj").setAsString() = m_gjsj;
		}
		if(!Zdyj_Suspicious)
		{
		/*	if(xZdyj.IsEmpty() || xXgyj.IsEmpty()){
				g_dbcommand.Param("zdyj").setAsString() = m_zdyj;
				xZdyj = m_zdyj;
			} else {
			if(!m_shys.IsEmpty() && theApp.m_Cpp_User.Find(m_shys) != -1 ){
				g_dbcommand.Param("zdyj").setAsString() = m_zdyj;
				xZdyj = m_zdyj;
			} else if(m_shys.IsEmpty() && theApp.m_Cpp_User.Find(m_zzys) != -1){
				g_dbcommand.Param("zdyj").setAsString() = m_zdyj;
				xZdyj = m_zdyj;
			} else {

			}
			}*/
			g_dbcommand.Param("zdyj").setAsString() = m_zdyj;
			xZdyj = m_zdyj;
		}
		
		if(IsRefresh_Czyj)
		{
			g_dbcommand.Param("czyj").setAsString() = m_zdyj;
		}

//		if(IsRefresh_Xgyj)
//		{
//			g_dbcommand.Param("xgyj").setAsString() = m_zdyj;
//		}

		if(theApp.m_Cpp_User.Find(m_shys) >= 0 && !m_shys.IsEmpty())
		{
			g_dbcommand.Param("wyyj").setAsString() = m_zdyj;
		}

		if(theApp.m_Cpp_User.Find(m_zzys) >= 0 && xWyyj.IsEmpty() && !m_zzys.IsEmpty())
		{
			g_dbcommand.Param("xgyj").setAsString() = m_zdyj;
		}

		g_dbcommand.Param("zdbm").setAsString() = m_zdbm;
		g_dbcommand.Param("zdgjc").setAsString() = m_zdgjc;
		g_dbcommand.Param("sfyx").setAsString() = m_sfyx;
		g_dbcommand.Param("zdfh").setAsString() = m_zdfh;
		g_dbcommand.Param("czys").setAsString() = m_czys;
		g_dbcommand.Param("zzys").setAsString() = m_zzys;
		g_dbcommand.Param("shys").setAsString() = m_shys;
		
		m_bggs.TrimLeft(); m_bggs.TrimRight();
		if(m_bggs.CompareNoCase("请选择报告格式") == 0)  m_bggs.Empty();
		g_dbcommand.Param("bggs").setAsString() = m_bggs;
        
		COleDateTime m_time;
		m_Time_Bgrq.GetTime(m_time);
		SADateTime bgrq_dtValue(m_time.GetYear(),m_time.GetMonth(),m_time.GetDay(),m_time.GetHour(),m_time.GetMinute(),m_time.GetSecond());
		g_dbcommand.Param("bgrq").setAsDateTime() = bgrq_dtValue;
		
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

	//-------------------------------------------------------
	if(xZdyj.IsEmpty())
	{
		int i = 0x04;  i = ~i;
		m_bgzt = (m_bgzt & i);
	}
	else
	{
		m_bgzt = (m_bgzt | 0x04);
	}

	try
	{
		SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set bgzt=:bgzt where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("bgzt").setAsLong() = m_bgzt;
		g_dbcommand.Param("medicalno").setAsString() = pView->GetDocument()->m_blh;
		
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
		theApp.NotifyRecordStatus(pView->GetDocument()->m_blh);
	}

	((CMainFrame *)AfxGetMainWnd())->RefreshStatusBar();

	if(theApp.m_AutoRefreshDBListMode.CompareNoCase("是") == 0 && ((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
	{
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.ResetGridSize();
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
	}
	//-------------------------------------------------------

	m_isModified = false;

	return 1;
}




void CMedDiagnose::OnButtonCyc() 
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

void CMedDiagnose::OnDestroy() 
{
	CXTResizeFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	if(pZdex != NULL)
	{
		pZdex->DestroyWindow();
		delete pZdex;
		pZdex = NULL;
	}
	if (pNormalWord!=NULL) 
	{
		pNormalWord->DestroyWindow();
		delete pNormalWord;
		pNormalWord = NULL;
	}	

	if (pListInput != NULL)
	{
		pListInput->DestroyWindow();
		delete pListInput;
		pListInput = NULL;
	}
}

void CMedDiagnose::OnSetfocusEditRysj() 
{
	if (pNormalWord != NULL && pNormalWord->IsWindowVisible() && CurEdit != 3)
	{
		CurEdit=3;	
		pNormalWord->AdjustPosition();
	}
	else
	{
		CurEdit=3;	
	}

	m_CurrentTreeMode = 1;
	UpdateTreeCtrl(m_CurrentTreeMode, FALSE);
}

void CMedDiagnose::OnSetfocusEditGjsj() 
{
	if (pNormalWord != NULL && pNormalWord->IsWindowVisible() && CurEdit != 1)
	{
		CurEdit=1;	
		pNormalWord->AdjustPosition();
	}
	else
	{
		CurEdit=1;	
	}

	m_CurrentTreeMode = 2;
	UpdateTreeCtrl(m_CurrentTreeMode, FALSE);
}

void CMedDiagnose::OnSetfocusEditZdyj() 
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

	m_CurrentTreeMode = 3;
	UpdateTreeCtrl(m_CurrentTreeMode, FALSE);
}

void CMedDiagnose::OnButtonZdmb() 
{
	UpdateData(TRUE);

	CString m_old_gjsj = m_gjsj;
	CString m_old_zdyj = m_zdyj;

	CSelectBZ Dlg(this,&m_gjsj,&m_zdyj);
	Dlg.DoModal();

	if(m_old_gjsj.CompareNoCase(m_gjsj) || m_old_zdyj.CompareNoCase(m_zdyj))
	{
		OnContentModified();
	}

	UpdateData(FALSE);
}

void CMedDiagnose::OnButtonBc() 
{
	PostMessage(WM_CONTENTSAVE);
	CWnd *pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CDBList));
	if(pWnd != NULL)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.SetActiveView(pWnd);
		((CEdit *)pWnd->GetDlgItem(IDC_EDIT_BLH))->SetFocus();
	}
}

void CMedDiagnose::OnButtonXgyj() 
{
	PostMessage(WM_CONTENTSAVE);

	CString m_old_zdyj;
	GetDlgItemText(IDC_EDIT_ZDYJ, m_old_zdyj);
	
	CXgyj mydlg(this);
	mydlg.DoModal();

	CString str;
	GetDlgItemText(IDC_EDIT_ZDYJ, str);
	if(m_old_zdyj.CompareNoCase(str))
	{
		OnContentModified();
	}
}

void CMedDiagnose::OnButtonBdbg() 
{
	CBdbg mydlg(this);
	mydlg.DoModal();
}

void CMedDiagnose::OnButtonBcbg() 
{
	CBcbg mydlg(this);
	mydlg.DoModal();
}

void CMedDiagnose::OnButtonSfjg() 
{
	CSfjg mydlg(this);
	mydlg.DoModal();
}

void CMedDiagnose::OnButtonKnhz() 
{
	CKnhz mydlg(this);
	mydlg.DoModal();
}

void CMedDiagnose::OnButtonQcmx() 
{
	CQcmx mydlg(this);
	mydlg.DoModal();
}

void CMedDiagnose::RefreshBgzt()
{
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
	
	int yzcount = 0;
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_ADVICE + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			yzcount = g_dbcommand.Field(1).asLong();
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

	if(yzcount > 0)
	{
		m_bgzt = (m_bgzt | 0x02);
	}
	else
	{
		int i = 0x02;  i = ~i;
		m_bgzt = (m_bgzt & i);
	}

	try
	{
		SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set bgzt=:bgzt where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("bgzt").setAsLong() = m_bgzt;
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
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
		theApp.NotifyRecordStatus(theApp.GetView()->GetDocument()->m_blh);
	}

	((CMainFrame *)AfxGetMainWnd())->RefreshStatusBar();

	//-----------------------------------------------------
	CString m_wfyy; m_wfyy.Empty();
	try
	{
		SAString cmdstr = "Select DISTINCT yzlx from " + theApp.TABLE_ADVICE + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		while( g_dbcommand.FetchNext() )
		{
			if(!m_wfyy.IsEmpty())  m_wfyy += ",";
			m_wfyy += g_dbcommand.Field("yzlx").asString();
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

	try
	{
		CString fieldstr;
		fieldstr.Empty();
		fieldstr += "wfyy=:wfyy";
		SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set " + fieldstr + " where medicalno = :medicalno";
		
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Param("wfyy").setAsString() = m_wfyy;
		
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
	
	if(((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
	{
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.PostMessage(WM_CONTENTREFRESH);
	}
	//-----------------------------------------------------

	if(theApp.m_AutoRefreshDBListMode.CompareNoCase("是") == 0 && ((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
	{
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.ResetGridSize();
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
	}
}

void CMedDiagnose::OnButtonJsyz() 
{
	// TODO: Add your control notification handler code here
	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  return;

	CJsyz mydlg(this);
	mydlg.DoModal();

	RefreshBgzt();
}

void CMedDiagnose::OnButtonTjyz() 
{
	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  return;

	CTjyz mydlg(this);
	mydlg.DoModal();

	RefreshBgzt();
}

void CMedDiagnose::OnButtonQppj() 
{
	CQppj mydlg(this);
	mydlg.DoModal();
}

void CMedDiagnose::ResetButtonTextColor(bool IsInit)
{
	CPathologyView *pView = theApp.GetView();
	ASSERT (pView != NULL);

	if(IsInit || pView->GetDocument()->m_blh.IsEmpty())
	{
		m_Button_Qcmx.SetColorText(RGB(0,0,0));
		m_Button_Bdbg.SetColorText(RGB(0,0,0));
		m_Button_Xgyj.SetColorText(RGB(0,0,0));
		m_Button_Knhz.SetColorText(RGB(0,0,0));
		m_Button_Bcbg.SetColorText(RGB(0,0,0));
		m_Button_Sfjg.SetColorText(RGB(0,0,0));
		m_Button_Tjyz.SetColorText(RGB(0,0,0));
		m_Button_Jsyz.SetColorText(RGB(0,0,0));
		m_Button_Qppj.SetColorText(RGB(0,0,0));

		m_Button_Qcmx.RedrawWindow();
		m_Button_Bdbg.RedrawWindow();
		m_Button_Xgyj.RedrawWindow();
		m_Button_Knhz.RedrawWindow();
		m_Button_Bcbg.RedrawWindow();
		m_Button_Sfjg.RedrawWindow();
		m_Button_Tjyz.RedrawWindow();
		m_Button_Jsyz.RedrawWindow();
		m_Button_Qppj.RedrawWindow();
		return;
	}

	int row_num = 0;
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = pView->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			row_num = g_dbcommand.Field(1).asLong();
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

	if(row_num > 0)
	{
		m_Button_Qcmx.SetColorText(RGB(0,0,255));
	}
	else
	{
		m_Button_Qcmx.SetColorText(RGB(0,0,0));
	}

	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_HZ + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = pView->GetDocument()->m_blh;
		
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
		{
			row_num = g_dbcommand.Field(1).asLong();
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

	// 当"专家意见"不为空时也判断该记录已会诊
	bool m_IsZjyjNull = true;
	try
	{
		SAString cmdstr = "Select TOP 1 zjyj from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			cmdstr = "Select zjyj from " + theApp.TABLE_MAIN + " where medicalno = :medicalno and ROWNUM<=1";
		}
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = pView->GetDocument()->m_blh;
		
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
		{
			if(!g_dbcommand.Field("zjyj").isNull() && g_dbcommand.Field("zjyj").asString().GetLength() > 0)
			{
				m_IsZjyjNull = false;
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
		return;
	}

	if(row_num > 1 || !m_IsZjyjNull)
	{
		m_Button_Knhz.SetColorText(RGB(0,0,255));
	}
	else
	{
		m_Button_Knhz.SetColorText(RGB(0,0,0));
	}

	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_SLICE + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = pView->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			row_num = g_dbcommand.Field(1).asLong();
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
	if(row_num >= 1)
	{
		m_Button_Qppj.SetColorText(RGB(0,0,255));
	}
	else
	{
		m_Button_Qppj.SetColorText(RGB(0,0,0));
	}

	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_ADVICE + " where medicalno = :medicalno and (yzlx = '补取' or yzlx = '重切' or yzlx = '深切' or yzlx = '薄切' or yzlx = '连切' or yzlx = '常规')";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = pView->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			row_num = g_dbcommand.Field(1).asLong();
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
	if(row_num > 0)
	{
		m_Button_Jsyz.SetColorText(RGB(0,0,255));
	}
	else
	{
		m_Button_Jsyz.SetColorText(RGB(0,0,0));
	}

	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_ADVICE + " where medicalno = :medicalno and (yzlx = '免疫组化' or yzlx = '特殊染色' or yzlx = '分子病理' or yzlx = '电镜')";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			row_num = g_dbcommand.Field(1).asLong();
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

	if(row_num > 0)
	{
		m_Button_Tjyz.SetColorText(RGB(0,0,255));
	}
	else
	{
		m_Button_Tjyz.SetColorText(RGB(0,0,0));
	}

	CString m_bdjg, m_xgyj, m_bczd, m_sfjg;
	m_bdjg.Empty();
	m_xgyj.Empty();
	m_bczd.Empty();
	m_sfjg.Empty();

	try
	{
		SAString cmdstr = "Select TOP 1 bdjg,xgyj,bczd,sfjg from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			cmdstr = "Select bdjg,xgyj,bczd,sfjg from " + theApp.TABLE_MAIN + " where medicalno = :medicalno and ROWNUM<=1";
		}
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = pView->GetDocument()->m_blh;
		
		g_dbcommand.Execute();
		
		if(g_dbcommand.FetchNext())
		{
			m_bdjg = g_dbcommand.Field("bdjg").asString();
			m_xgyj = g_dbcommand.Field("xgyj").asString();
			m_bczd = g_dbcommand.Field("bczd").asString();
			m_sfjg = g_dbcommand.Field("sfjg").asString();
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

	if(m_bdjg.IsEmpty())
	{
		m_Button_Bdbg.SetColorText(RGB(0,0,0));
	}
	else
	{
		m_Button_Bdbg.SetColorText(RGB(0,0,255));
	}

	if(m_xgyj.IsEmpty())
	{
		m_Button_Xgyj.SetColorText(RGB(0,0,0));
	}
	else
	{
		m_Button_Xgyj.SetColorText(RGB(0,0,255));
	}

	if(m_bczd.IsEmpty())
	{
		m_Button_Bcbg.SetColorText(RGB(0,0,0));
	}
	else
	{
		m_Button_Bcbg.SetColorText(RGB(0,0,255));
	}

	if(m_sfjg.IsEmpty())
	{
		m_Button_Sfjg.SetColorText(RGB(0,0,0));
	}
	else
	{
		m_Button_Sfjg.SetColorText(RGB(0,0,255));
	}

	m_Button_Qcmx.RedrawWindow();
	m_Button_Bdbg.RedrawWindow();
	m_Button_Xgyj.RedrawWindow();
	m_Button_Knhz.RedrawWindow();
	m_Button_Bcbg.RedrawWindow();
	m_Button_Sfjg.RedrawWindow();
	m_Button_Tjyz.RedrawWindow();
	m_Button_Jsyz.RedrawWindow();
	m_Button_Qppj.RedrawWindow();
}

void CMedDiagnose::RefreshBggs()
{
	((CComboBox *)GetDlgItem(IDC_COMBO_BGGS))->ResetContent();

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
		g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
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
			
			((CComboBox *)GetDlgItem(IDC_COMBO_BGGS))->AddString( g_dbcommand.Field("rpt_name").asString() );
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

void CMedDiagnose::OnButtonMicd10() 
{
	if(pListInput != NULL)
	{
		pListInput->m_type = 1;  // 1 - MICD10, 2 - SICD10
		pListInput->ShowWindow(SW_SHOW);
	}
}

void CMedDiagnose::OnButtonSicd10() 
{
	if(pListInput != NULL)
	{
		pListInput->m_type = 2;  // 1 - MICD10, 2 - SICD10
		pListInput->ShowWindow(SW_SHOW);
	}
}

LRESULT CMedDiagnose::ListSelect(WPARAM wparam, LPARAM lparam)
{
	int index = pListInput->m_listinfo.GetCurrentRow();
	if( index < 0 || index >=pListInput->m_listinfo.GetNumberRows() )  return 0L;
	CString firstname;
	CUGCell  cell;
	pListInput->m_listinfo.GetCellIndirect(0 , index, &cell);
	cell.GetText(&firstname);

	CString str;
	GetDlgItemText(IDC_EDIT_ZDBM, str);
	str.TrimLeft();  str.TrimRight();
	if(!str.IsEmpty())  str += "|";
	str += firstname.Left(firstname.Find('['));
	SetDlgItemText(IDC_EDIT_ZDBM, str);

	GetDlgItemText(IDC_EDIT_ZDGJC, str);
	str.TrimLeft();  str.TrimRight();
	if(!str.IsEmpty())  str += "|";
	firstname = firstname.Right(firstname.GetLength() - firstname.Find('[') - 1);
	firstname = firstname.Left(firstname.Find(']'));
    str += firstname;
	SetDlgItemText(IDC_EDIT_ZDGJC, str);

	return 0L;
}

void CMedDiagnose::SwitchNormalWnd()
{
	switch(CurEdit) {
	case 1:
		((CWnd *)GetDlgItem(IDC_EDIT_ZDYJ))->SetFocus();
		OnSetfocusEditZdyj();
		break;
	case 2:
		((CWnd *)GetDlgItem(IDC_EDIT_RYSJ))->SetFocus();
		OnSetfocusEditRysj();
		break;
	case 3:
		((CWnd *)GetDlgItem(IDC_EDIT_GJSJ))->SetFocus();
		OnSetfocusEditGjsj();
		break;
	}
}

void CMedDiagnose::OnContentModified() 
{
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh)) return;

	m_isModified = true;
}

void CMedDiagnose::OnDateTimeChange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnContentModified();
	
	*pResult = 0;
}

void CMedDiagnose::SetModifiedFlag()
{
	OnContentModified();
}

BOOL CMedDiagnose::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	CToolTipCtrl *pToolTipCtrl;
	CXTTreeCtrl  *pTreeCtrl;
	LPNMTTDISPINFO lpnmtdi = (LPNMTTDISPINFO)lParam;
	LPNMHDR lpnmhdr = &lpnmtdi->hdr;

	
	if (lpnmhdr->hwndFrom == m_ToolTip_RYSJ.m_hWnd)
	{
		pToolTipCtrl= &m_ToolTip_RYSJ;
		pTreeCtrl	= &m_TreeCtrl_RYSJ;
	}
	else if (lpnmhdr->hwndFrom == m_ToolTip_GJSJ.m_hWnd)
	{
		pToolTipCtrl= &m_ToolTip_GJSJ;
		pTreeCtrl	= &m_TreeCtrl_GJSJ;
	}
	else if (lpnmhdr->hwndFrom == m_ToolTip_ZDYJ.m_hWnd)
	{
		pToolTipCtrl= &m_ToolTip_ZDYJ;
		pTreeCtrl	= &m_TreeCtrl_ZDYJ;
	}
	else
	{
		pToolTipCtrl= NULL;
		pTreeCtrl	= NULL;
	}		
	
	
	if (pToolTipCtrl != NULL)
	{
		POINT myPoint;
		switch (lpnmhdr->code)       {         
		case  TTN_GETDISPINFO:     
			GetCursorPos(&myPoint);
			pTreeCtrl->ScreenToClient(&myPoint);
			// Select the item that is at the point myPoint.
			UINT uFlags;
			HTREEITEM hItem = pTreeCtrl->HitTest(myPoint, &uFlags);
			
			if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
			{
				int i,j;
				pTreeCtrl->GetItemImage(hItem, i, j);
				if(i == 2) 
				{
					HTREEITEM hti = pTreeCtrl->GetParentItem(hItem);
					CString bzname = pTreeCtrl->GetItemText(hti);
					
					hti = pTreeCtrl->GetParentItem(hti);
					CString zqname = pTreeCtrl->GetItemText(hti);
					
					int count = 0;
					hti = pTreeCtrl->GetPrevSiblingItem(hItem);
					while(hti != NULL)
					{
						count++;
						hti = pTreeCtrl->GetPrevSiblingItem(hti);
					}
					
					CString fanben;  fanben.Empty();
					
					theApp.ConnectDatabase();
					try
					{
						SAString cmdstr = "Select fanben from " + theApp.TABLE_STANDTEXT + " where zhangqi = :zhangqi and bingzhong = :bingzhong and usagetype = :usagetype";
						g_dbcommand.setCommandText(cmdstr);
						g_dbcommand.Param("zhangqi").setAsString()   = zqname;
						g_dbcommand.Param("bingzhong").setAsString() = bzname;
						g_dbcommand.Param("usagetype").setAsLong()   = pTreeCtrl->GetItemData(hItem);
						g_dbcommand.Execute();
						
						i = 0;
						while( g_dbcommand.FetchNext() )
						{
							if(count == i)
							{
								fanben = g_dbcommand.Field("fanben").asString();
								break;
							}
							i++;
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
					
					int strlen = fanben.GetLength();
					if(strlen > 1000)  strlen = 1000;
					sprintf(m_Fanben, "%s", fanben.Left(strlen));
					lpnmtdi->lpszText = m_Fanben;
				}
			}
			break;         
		}         
	}

	return CXTResizeFormView::OnNotify(wParam, lParam, pResult);
}


void CMedDiagnose::OnButtonBlh() 
{
	if(NULL == pZdex)
	{
		pZdex=new CZdex(this);
		pZdex->Create();
		pZdex->SetActiveWindow();
		pZdex->ShowWindow(SW_SHOW);
	}
	else
	{
		pZdex->ShowWindow(!pZdex->IsWindowVisible());
	}

	if(pZdex && pZdex->IsWindowVisible())
	{
		pZdex->AdjustPosition();
	}
}

void CMedDiagnose::OnSetfocusComboBggs() 
{
	// TODO: Add your control notification handler code here
	//CMedImage切换
	CWnd* pImage = ((CMainFrame*)theApp.GetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CMedImage));
	((CMedImage*)pImage)->setNotDefault(); //设置打印状态

	CWnd *pActiveWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetActiveView();
	CWnd *pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CMedImage));
	if(pWnd != NULL && pWnd != pActiveWnd) 
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.SetActiveView(pWnd);
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_BGGS))->SendMessage(CBN_DROPDOWN);

}

void CMedDiagnose::OnSelchangeComboBggs() 
{
	// TODO: Add your control notification handler code here
	this->SetFocus();
}

void CMedDiagnose::OnCloseupComboBggs() 
{
	// TODO: Add your control notification handler code here
	SetFocus();
}

void CMedDiagnose::OnButtonPrint() 
{
	// TODO: Add your control notification handler code here
//	CWnd* person = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedPerson));
//	person->PostMessage(WM_COMMAND,IDC_BUTTON_PRINT);
	SendMessage(WM_CONTENTSAVE);

	CFile file(theApp.m_Exe_Working_Dir + "MedList.txt",CFile::modeReadWrite|CFile.modeCreate);
	CString mno = theApp.GetView()->GetDocument()->m_blh;
	file.Write(mno,mno.GetLength());
	file.Flush();
	file.Close();

	HINSTANCE	hLib;
	if( (hLib = LoadLibrary(theApp.m_Exe_Working_Dir + "dpt.dll")) == 0 )
	{
		AfxMessageBox("dpt.dll装载错误！");
		return;
	}
	
	typedef int (*DetailRpt)(char *); 
	DetailRpt ProcInDLL;
	ProcInDLL = (DetailRpt) GetProcAddress(hLib, "DetailRpt");
				
	if( ProcInDLL == NULL )
	{
		AfxMessageBox("DetailRpt函数定位错误！");
		return;
	}

	//int result = ProcInDLL((LPCTSTR)theApp.m_Exe_Working_Dir);
	ProcInDLL(theApp.m_Exe_Working_Dir.GetBuffer(theApp.m_Exe_Working_Dir.GetLength() + 1));
	theApp.m_Exe_Working_Dir.ReleaseBuffer();
	FreeLibrary(hLib);
	hLib = 0;
}

void CMedDiagnose::OnSize(UINT nType, int cx, int cy) 
{
	CRect rect, rcClient;
	CWnd *pWnd;

	CXTResizeFormView::OnSize(nType, cx, cy);
	
	GetClientRect(rcClient);
	m_TreeCtrl_RYSJ.GetWindowRect(&rect);
	ScreenToClient(&rect);

	rect.right	= rcClient.right;
	rect.bottom	= rcClient.bottom;
	m_TreeCtrl_RYSJ.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOREPOSITION);
	m_TreeCtrl_GJSJ.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOREPOSITION);
	m_TreeCtrl_ZDYJ.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOREPOSITION);

	pWnd = GetDlgItem(IDC_COMBO_BGGS);
	if (pWnd != NULL)
	{
		pWnd->GetWindowRect(rect);
		ScreenToClient(&rect);
		rect.right = rcClient.right;
		pWnd->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOREPOSITION);
	}
}
