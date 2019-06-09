// CMedTbs : implementation file
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "Pathology.h"
#include "MainFrm.h"
#include "PathologyView.h"
#include "PathologyDoc.h"
#include "MedTbs.h"
#include "StrEdit.h"
#include "NormalWord.h"
#include "KSFileDialog.h"
#include "Figure.h"
#include "INI.h"
#include "Qppj.h"
#include "SelectBZ.h"
#include "MedImage.h"
#include "Tjyz.h"
#include "MedPerson.h"

#include "CannedTextTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMedTbs
extern CPathologyApp theApp;

CMedTbs::CMedTbs()
	: CXTResizeFormView(CMedTbs::IDD)
{
	//{{AFX_DATA_INIT(CMedTbs)
	m_bggs = _T("请选择报告格式");
	m_zdyj = _T("");
	m_shys = _T("");
	m_zzys = _T("");
	m_bczd = _T("");
	m_rysj = _T("");
	//}}AFX_DATA_INIT

	CurEdit = 0;	
	pNormalWord = NULL;
	m_bIsFirstInit = true;
}

CMedTbs::~CMedTbs()
{
	// TODO: add destruction code here.

}

IMPLEMENT_DYNCREATE(CMedTbs, CXTResizeFormView)

BEGIN_MESSAGE_MAP(CMedTbs, CXTResizeFormView)
	//{{AFX_MSG_MAP(CMedTbs)
	ON_WM_DESTROY()
	ON_EN_SETFOCUS(IDC_EDIT_ZDYJ, OnSetfocusEditZdyj)
	ON_BN_CLICKED(IDC_BUTTON_CYC, OnButtonCyc)
	ON_BN_CLICKED(IDC_BUTTON_BC, OnButtonBc)
	ON_BN_CLICKED(IDC_CHECK_JBMYD1, OnCheckJbmyd1)
	ON_BN_CLICKED(IDC_CHECK_JBMYD2, OnCheckJbmyd2)
	ON_BN_CLICKED(IDC_CHECK_JBMYD3, OnCheckJbmyd3)
	ON_BN_CLICKED(IDC_CHECK_XBL1, OnCheckXbl1)
	ON_BN_CLICKED(IDC_CHECK_XBL2, OnCheckXbl2)
	ON_BN_CLICKED(IDC_CHECK_YZFY1, OnCheckYzfy1)
	ON_BN_CLICKED(IDC_CHECK_YZFY2, OnCheckYzfy2)
	ON_BN_CLICKED(IDC_CHECK_YZFY3, OnCheckYzfy3)
	ON_EN_SETFOCUS(IDC_EDIT_RYSJ, OnSetfocusEditRysj)
	ON_EN_CHANGE(IDC_EDIT_RYSJ, OnContentModified)
	ON_BN_CLICKED(IDC_BUTTON_QPPJ, OnButtonQppj)
	ON_BN_CLICKED(IDC_BUTTON_ZDMB, OnButtonZdmb)
	ON_BN_CLICKED(IDC_BUTTON_TJYZ, OnButtonTjyz)
	ON_CBN_CLOSEUP(IDC_COMBO_BGGS, OnCloseupComboBggs)
	ON_CBN_SETFOCUS(IDC_COMBO_BGGS, OnSetfocusComboBggs)
	ON_CBN_SELCHANGE(IDC_COMBO_BGGS, OnSelchangeComboBggs)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	ON_EN_CHANGE(IDC_EDIT_ZDYJ, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_BCZD, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_ZZYS, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_SHYS, OnContentModified)
	ON_NOTIFY(DTN_DATETIMECHANGE, 111,  OnDateTimeChange)
	ON_BN_CLICKED(IDC_CHECK_LZXB, OnContentModified)
	ON_BN_CLICKED(IDC_CHECK_JGXB, OnContentModified)
	ON_BN_CLICKED(IDC_CHECK_HSXB, OnContentModified)
	ON_BN_CLICKED(IDC_CHECK_DCGR, OnContentModified)
	ON_BN_CLICKED(IDC_CHECK_MJGR, OnContentModified)
	ON_BN_CLICKED(IDC_CHECK_LZJGR, OnContentModified)
	ON_BN_CLICKED(IDC_CHECK_QGJGR, OnContentModified)
	ON_BN_CLICKED(IDC_CHECK_FXJGR, OnContentModified)
	ON_BN_CLICKED(IDC_CHECK_HPVGR, OnContentModified)
	ON_BN_CLICKED(IDC_CHECK_BZBDGR, OnContentModified)
	ON_BN_CLICKED(IDC_CHECK_LSRTZBDGR, OnContentModified)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CONTENTSAVE, OnContentSave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMedTbs message handlers

void CMedTbs::DoDataExchange(CDataExchange* pDX) 
{
	CXTResizeFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMedTbs)
	DDX_Control(pDX, IDC_EDIT_BCZD, m_ctrl_bczd);
	DDX_Control(pDX, IDC_EDIT_ZZYS, m_ctrl_zzys);
	DDX_Control(pDX, IDC_EDIT_SHYS, m_ctrl_shys);
	
	DDX_Control(pDX, IDC_TREE_RYSJ, m_TreeCtrl_RYSJ);
	DDX_Control(pDX, IDC_TREE_ZDYJ, m_TreeCtrl_ZDYJ);

	DDX_Control(pDX, IDC_EDIT_SFYX, m_ctrl_sfyx);
	DDX_Control(pDX, IDC_EDIT_ZDFH, m_ctrl_zdfh);
	DDX_CBString(pDX, IDC_COMBO_BGGS, m_bggs);
	DDX_Text(pDX, IDC_EDIT_ZDYJ, m_zdyj);
	DDX_Text(pDX, IDC_EDIT_SHYS, m_shys);
	DDX_Text(pDX, IDC_EDIT_ZZYS, m_zzys);
	DDX_Text(pDX, IDC_EDIT_BCZD, m_bczd);
	DDX_Text(pDX, IDC_EDIT_RYSJ, m_rysj);
	DDX_Text(pDX, IDC_EDIT_ZDGJC, m_zdgjc);
	DDX_Text(pDX, IDC_EDIT_SFYX, m_sfyx);
	DDX_Text(pDX, IDC_EDIT_ZDFH, m_zdfh);
	//}}AFX_DATA_MAP
}


int CMedTbs::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CBitmap bitmap;

	if (CXTResizeFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
		
	if (!m_ToolTip_RYSJ.Create(this))
		return -1;

	if (!m_ToolTip_ZDYJ.Create(this))
		return -1;
	
	m_ToolTip_RYSJ.Activate(TRUE);
	m_ToolTip_RYSJ.AddTool(&m_TreeCtrl_RYSJ);
	m_ToolTip_RYSJ.SetMaxTipWidth(300);
	m_TreeCtrl_RYSJ.SetToolTips(&m_ToolTip_RYSJ);
		
	m_ToolTip_ZDYJ.Activate(TRUE);
	m_ToolTip_ZDYJ.AddTool(&m_TreeCtrl_ZDYJ);
	m_ToolTip_ZDYJ.SetMaxTipWidth(300);
	m_TreeCtrl_ZDYJ.SetToolTips(&m_ToolTip_RYSJ);

	bitmap.LoadBitmap(IDB_IMGLIST_VIEW);
		
	m_ImageList.Create( 16, 16, ILC_COLOR24 | ILC_MASK, 1, 1 );
	m_ImageList.Add( &bitmap, RGB( 0x00,0xff,0x00 ) );
		
	m_TreeCtrl_RYSJ.Initialize(1, IDC_EDIT_RYSJ, this);
	m_TreeCtrl_ZDYJ.Initialize(3, IDC_EDIT_ZDYJ, this);

	m_TreeCtrl_RYSJ.SetImageList(&m_ImageList, TVSIL_NORMAL);
	m_TreeCtrl_ZDYJ.SetImageList(&m_ImageList, TVSIL_NORMAL);

	// initialize cool menus
	m_coolMenu.HookWindow(m_hWnd);
	m_coolMenu.LoadToolbar(IDR_POPUP_TREE);


	return 0;
}

/*
mode
肉眼所见 1;
光镜所见 2;
诊断所见 3;
*/
void CMedTbs::UpdateTreeCtrl(int mode, BOOL bRebuildTree)
{
	switch(mode)
	{
	case 1:
		m_TreeCtrl_ZDYJ.ShowWindow(SW_HIDE);
		m_TreeCtrl_RYSJ.ShowWindow(SW_SHOW);
		m_pCurrentTreeCtrl = &m_TreeCtrl_RYSJ;

		SetDlgItemText(IDC_STATIC_BWBZ, "词典－肉眼所见");
		break;
	case 3:
		m_TreeCtrl_RYSJ.ShowWindow(SW_HIDE);
		m_TreeCtrl_ZDYJ.ShowWindow(SW_SHOW);
		m_pCurrentTreeCtrl = &m_TreeCtrl_ZDYJ;
		SetDlgItemText(IDC_STATIC_BWBZ, "词典－诊断意见");
		break;
	}

	if (bRebuildTree)
	{
		m_TreeCtrl_RYSJ.BuildTree();
		m_TreeCtrl_ZDYJ.BuildTree();
	}

}

void CMedTbs::OnDestroy() 
{
	CXTResizeFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	if (pNormalWord!=NULL) 
	{
		pNormalWord->DestroyWindow();
		delete pNormalWord;
		pNormalWord = NULL;
	}	
}

void CMedTbs::OnSetfocusEditRysj() 
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

void CMedTbs::OnSetfocusEditZdyj() 
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


void CMedTbs::OnInitialUpdate() 
{
	CXTResizeFormView::OnInitialUpdate();

	m_nCurrentTreeCtrlID = 0;

	/////////////////////////////////////////////////////////////////

	if (m_bIsFirstInit)
	{
		m_bIsFirstInit = false;

		//--------------------------------------------------
		m_TreeCtrl_RYSJ.EnableMultiSelect(FALSE);
		m_TreeCtrl_ZDYJ.EnableMultiSelect(FALSE);
	
		m_CurrentTreeMode = 3;
		UpdateTreeCtrl(m_CurrentTreeMode);
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
			if(theApp.m_fnamelist.GetAt(i).CompareNoCase("bczd") == 0)
			{
				m_ctrl_bczd.m_info = theApp.m_finfolist.GetAt(i);
			}
			if(theApp.m_fnamelist.GetAt(i).CompareNoCase("sfyx") == 0)
			{
				m_ctrl_sfyx.m_info = theApp.m_finfolist.GetAt(i);
			}
			if(theApp.m_fnamelist.GetAt(i).CompareNoCase("zdfh") == 0)
			{
				m_ctrl_zdfh.m_info = theApp.m_finfolist.GetAt(i);
			}
		}
		
		m_ctrl_shys.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_zzys.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_bczd.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_sfyx.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_zdfh.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
	
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

    ((CButton *)GetDlgItem(IDC_CHECK_JBMYD1))->SetCheck(0);	//满意		
    ((CButton *)GetDlgItem(IDC_CHECK_JBMYD2))->SetCheck(0); //基本满意
    ((CButton *)GetDlgItem(IDC_CHECK_JBMYD3))->SetCheck(0); //需重采样
    ((CButton *)GetDlgItem(IDC_CHECK_XBL1))->SetCheck(0);	// 细胞量 >40%  
    ((CButton *)GetDlgItem(IDC_CHECK_XBL2))->SetCheck(0);   // 细胞量 <40%
    ((CButton *)GetDlgItem(IDC_CHECK_YZFY1))->SetCheck(0);
    ((CButton *)GetDlgItem(IDC_CHECK_YZFY2))->SetCheck(0);
    ((CButton *)GetDlgItem(IDC_CHECK_YZFY3))->SetCheck(0);
    ((CButton *)GetDlgItem(IDC_CHECK_LZXB))->SetCheck(0);
    ((CButton *)GetDlgItem(IDC_CHECK_JGXB))->SetCheck(0);
    ((CButton *)GetDlgItem(IDC_CHECK_HSXB))->SetCheck(0);
    ((CButton *)GetDlgItem(IDC_CHECK_DCGR))->SetCheck(0);
    ((CButton *)GetDlgItem(IDC_CHECK_MJGR))->SetCheck(0);
    ((CButton *)GetDlgItem(IDC_CHECK_LZJGR))->SetCheck(0);
    ((CButton *)GetDlgItem(IDC_CHECK_QGJGR))->SetCheck(0);
    ((CButton *)GetDlgItem(IDC_CHECK_FXJGR))->SetCheck(0);
    ((CButton *)GetDlgItem(IDC_CHECK_HPVGR))->SetCheck(0);
    ((CButton *)GetDlgItem(IDC_CHECK_BZBDGR))->SetCheck(0);
    ((CButton *)GetDlgItem(IDC_CHECK_LSRTZBDGR))->SetCheck(0);

	if (pView->GetDocument()->m_IsNewRecord)
	{
		m_rysj = _T("");
		m_zdyj = _T("");
		m_bczd = _T("");
		m_zzys = _T("");
		m_shys = _T("");
		m_bggs = _T("请选择报告格式");

		//--------------------------------------------
		COleDateTime curt = theApp.GetOleServerDateTime();
		SADateTime nBgrq;
		nBgrq = theApp.GetServerDateTime();
		try
		{
			SAString cmdstr = "Select TOP 1 bgrq from " + theApp.TABLE_MAIN + " where dbtype = :dbtype order by medicalno desc";
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				cmdstr = "Select * from (Select bgrq from " + theApp.TABLE_MAIN + " where dbtype = :dbtype order by medicalno desc) where ROWNUM<=1";
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


		((CButton *)GetDlgItem(IDC_CHECK_JBMYD1))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_CHECK_XBL1))->SetCheck(1);

	}
	else
	{
		COleDateTime ServerTime = theApp.GetOleServerDateTime();
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = pView->GetDocument()->m_blh;
			
			g_dbcommand.Execute();
			
			if(g_dbcommand.FetchNext())
			{
				m_rysj = g_dbcommand.Field("rysj").asString();
				m_zdyj = g_dbcommand.Field("zdyj").asString();
				m_bczd = g_dbcommand.Field("bczd").asString();
				m_zzys = g_dbcommand.Field("zzys").asString();
				m_shys = g_dbcommand.Field("shys").asString();
				m_bggs = g_dbcommand.Field("bggs").asString();
				m_sfyx = g_dbcommand.Field("sfyx").asString();
				m_zdgjc = g_dbcommand.Field("zdgjc").asString();
				m_zdfh = g_dbcommand.Field("zdfh").asString();

				if(g_dbcommand.Field("bggs").isNull() || m_bggs.IsEmpty())
				{
					m_bggs.Format("请选择报告格式");
				}

				COleDateTime curt = ServerTime;
				SADateTime st = g_dbcommand.Field("bgrq").asDateTime();
				curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
				m_Time_Bgrq.SetTime(curt);

				//--------------------------------------------
				CString str = g_dbcommand.Field("jbmyd").asString();
				if(str.CompareNoCase("满意") == 0)
					((CButton *)GetDlgItem(IDC_CHECK_JBMYD1))->SetCheck(1);
				if(str.CompareNoCase("基本满意") == 0)
					((CButton *)GetDlgItem(IDC_CHECK_JBMYD2))->SetCheck(1);
				if(str.CompareNoCase("需重采样") == 0)
					((CButton *)GetDlgItem(IDC_CHECK_JBMYD3))->SetCheck(1);

				str = g_dbcommand.Field("xbl").asString();
				if(str.CompareNoCase("＞40％") == 0)
					((CButton *)GetDlgItem(IDC_CHECK_XBL1))->SetCheck(1);
				if(str.CompareNoCase("＜40％") == 0)
					((CButton *)GetDlgItem(IDC_CHECK_XBL2))->SetCheck(1);

				str = g_dbcommand.Field("yzfy").asString();
				if(str.CompareNoCase("轻度") == 0)
					((CButton *)GetDlgItem(IDC_CHECK_YZFY1))->SetCheck(1);
				if(str.CompareNoCase("中度") == 0)
					((CButton *)GetDlgItem(IDC_CHECK_YZFY2))->SetCheck(1);
				if(str.CompareNoCase("重度") == 0)
					((CButton *)GetDlgItem(IDC_CHECK_YZFY3))->SetCheck(1);
				
				str = g_dbcommand.Field("lzxb").asString();
				if(str.CompareNoCase("有") == 0)
					((CButton *)GetDlgItem(IDC_CHECK_LZXB))->SetCheck(1);
				str = g_dbcommand.Field("jgxb").asString();
				if(str.CompareNoCase("有") == 0)
					((CButton *)GetDlgItem(IDC_CHECK_JGXB))->SetCheck(1);
				str = g_dbcommand.Field("hsxb").asString();
				if(str.CompareNoCase("有") == 0)
					((CButton *)GetDlgItem(IDC_CHECK_HSXB))->SetCheck(1);
				str = g_dbcommand.Field("dcgr").asString();
				if(str.CompareNoCase("有") == 0)
					((CButton *)GetDlgItem(IDC_CHECK_DCGR))->SetCheck(1);
				str = g_dbcommand.Field("mjgr").asString();
				if(str.CompareNoCase("有") == 0)
					((CButton *)GetDlgItem(IDC_CHECK_MJGR))->SetCheck(1);
				str = g_dbcommand.Field("lzjgr").asString();
				if(str.CompareNoCase("有") == 0)
					((CButton *)GetDlgItem(IDC_CHECK_LZJGR))->SetCheck(1);
				str = g_dbcommand.Field("qgjgr").asString();
				if(str.CompareNoCase("有") == 0)
					((CButton *)GetDlgItem(IDC_CHECK_QGJGR))->SetCheck(1);
				str = g_dbcommand.Field("fxjgr").asString();
				if(str.CompareNoCase("有") == 0)
					((CButton *)GetDlgItem(IDC_CHECK_FXJGR))->SetCheck(1);
				str = g_dbcommand.Field("hpvgr").asString();
				if(str.CompareNoCase("有") == 0)
					((CButton *)GetDlgItem(IDC_CHECK_HPVGR))->SetCheck(1);
				str = g_dbcommand.Field("bzbdgr").asString();
				if(str.CompareNoCase("有") == 0)
					((CButton *)GetDlgItem(IDC_CHECK_BZBDGR))->SetCheck(1);
				str = g_dbcommand.Field("lsrtzbdgr").asString();
				if(str.CompareNoCase("有") == 0)
					((CButton *)GetDlgItem(IDC_CHECK_LSRTZBDGR))->SetCheck(1);
				//--------------------------------------------
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
}

LRESULT CMedTbs::OnContentSave(WPARAM wParam, LPARAM lParam)
{
	//if(!m_isModified)  return 0;

	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  return 0;
	CPathologyView *pView = theApp.GetView();
	ASSERT (pView != NULL);

	theApp.ConnectDatabase();
	
	int m_bgzt = 0;
	CString xRysj, xZdyj, xZzys; 
	xRysj.Empty(); xZdyj.Empty(); xZzys.Empty();
	
	try
	{
		SAString cmdstr = "Select bgzt, rysj, zdyj, zzys from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
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

			if(!g_dbcommand.Field("zdyj").isNull())
			{
				xZdyj = g_dbcommand.Field("zdyj").asString();
			}

			if(!g_dbcommand.Field("zzys").isNull())
			{
				xZzys = g_dbcommand.Field("zzys").asString();
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
	//--------------------------------------------------------
	CIniReader m_IniReader;
	m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");
	
	if(theApp.m_ZzysMode.CompareNoCase("是") == 0 && m_zzys.IsEmpty() && !m_zdyj.IsEmpty())
	{
		m_zzys = m_IniReader.getKeyValue("主诊医生","Settings");
		SetDlgItemText(IDC_EDIT_ZZYS, m_zzys);
	}

	if(m_zzys.CompareNoCase(xZzys) && !m_zzys.IsEmpty())
	{
		m_IniReader.setKey(m_zzys,"主诊医生","Settings");
	}

	bool Rysj_Suspicious = false;
	bool Zdyj_Suspicious = false;

	m_zdyj.TrimLeft(); m_zdyj.TrimRight();
	m_rysj.TrimLeft(); m_rysj.TrimRight();

	if(m_rysj.IsEmpty() && !xRysj.IsEmpty())
	{
		Rysj_Suspicious = true;
	}

	if(m_zdyj.IsEmpty() && !xZdyj.IsEmpty())
	{
		Zdyj_Suspicious = true;
	}

	if(Rysj_Suspicious || Zdyj_Suspicious)
	{
		CString value, nContent;
		value.Empty(); nContent.Empty();
		if(Rysj_Suspicious)
		{
			if(!nContent.IsEmpty()) nContent += "、";
			nContent += "“肉眼所见”";
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
				Rysj_Suspicious = Zdyj_Suspicious = false;
			}
		}
	}
	// save other field
	try
	{
		CString fieldstr;
		fieldstr.Empty();
		if(!Rysj_Suspicious)
		{
			fieldstr += "rysj=:rysj,";
		}
		if(!Zdyj_Suspicious)
		{
			fieldstr += "zdyj=:zdyj,";
		}
		fieldstr += "bczd=:bczd,";
		fieldstr += "zzys=:zzys,";
		fieldstr += "shys=:shys,";
		fieldstr += "bgrq=:bgrq,";
		fieldstr += "bggs=:bggs,";

		fieldstr += "jbmyd=:jbmyd,";
		fieldstr += "xbl=:xbl,";
		fieldstr += "yzfy=:yzfy,";

		fieldstr += "lzxb=:lzxb,";
		fieldstr += "jgxb=:jgxb,";
		fieldstr += "hsxb=:hsxb,";
		fieldstr += "dcgr=:dcgr,";
		fieldstr += "mjgr=:mjgr,";
		fieldstr += "lzjgr=:lzjgr,";
		fieldstr += "qgjgr=:qgjgr,";
		fieldstr += "fxjgr=:fxjgr,";
		fieldstr += "hpvgr=:hpvgr,";
		fieldstr += "bzbdgr=:bzbdgr,";
		fieldstr += "lsrtzbdgr=:lsrtzbdgr,";
		fieldstr += "zdgjc=:zdgjc,";
		fieldstr += "sfyx =:sfyx,";
		fieldstr += "zdfh =:zdfh";

		SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set " + fieldstr + " where medicalno = :medicalno";

		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = pView->GetDocument()->m_blh;
		if(!Rysj_Suspicious)
		{
			g_dbcommand.Param("rysj").setAsString() = m_rysj;
		}
		if(!Zdyj_Suspicious)
		{
			g_dbcommand.Param("zdyj").setAsString() = m_zdyj;
			xZdyj = m_zdyj;
		}
		g_dbcommand.Param("bczd").setAsString() = m_bczd;
		g_dbcommand.Param("zzys").setAsString() = m_zzys;
		g_dbcommand.Param("shys").setAsString() = m_shys;
		g_dbcommand.Param("zdgjc").setAsString() = m_zdgjc;
		g_dbcommand.Param("sfyx").setAsString() = m_sfyx;
		g_dbcommand.Param("zdfh").setAsString() = m_zdfh;
		
		m_bggs.TrimLeft(); m_bggs.TrimRight();
		if(m_bggs.CompareNoCase("请选择报告格式") == 0)  m_bggs.Empty();
		g_dbcommand.Param("bggs").setAsString() = m_bggs;
        
		COleDateTime m_time;
		m_Time_Bgrq.GetTime(m_time);
		SADateTime bgrq_dtValue(m_time.GetYear(),m_time.GetMonth(),m_time.GetDay(),m_time.GetHour(),m_time.GetMinute(),m_time.GetSecond());
		g_dbcommand.Param("bgrq").setAsDateTime() = bgrq_dtValue;

		//----------------------------------------------------------
		CString str; str.Empty();
		if(((CButton *)GetDlgItem(IDC_CHECK_JBMYD1))->GetState() & 0x0003)	str.Format("满意");
		if(((CButton *)GetDlgItem(IDC_CHECK_JBMYD2))->GetState() & 0x0003)	str.Format("基本满意");
		if(((CButton *)GetDlgItem(IDC_CHECK_JBMYD3))->GetState() & 0x0003)	str.Format("需重采样");
		g_dbcommand.Param("jbmyd").setAsString() = str;

		str.Empty();
		if(((CButton *)GetDlgItem(IDC_CHECK_XBL1))->GetState() & 0x0003)	str.Format("＞40％");
		if(((CButton *)GetDlgItem(IDC_CHECK_XBL2))->GetState() & 0x0003)	str.Format("＜40％");
		g_dbcommand.Param("xbl").setAsString() = str;

		//str.Empty();
		str.Format("无");
		if(((CButton *)GetDlgItem(IDC_CHECK_YZFY1))->GetState() & 0x0003)	str.Format("轻度");
		if(((CButton *)GetDlgItem(IDC_CHECK_YZFY2))->GetState() & 0x0003)	str.Format("中度");
		if(((CButton *)GetDlgItem(IDC_CHECK_YZFY3))->GetState() & 0x0003)	str.Format("重度");
		g_dbcommand.Param("yzfy").setAsString() = str;

		str = m_IniReader.getKeyValue("鳞状细胞","Settings");
		if(str.IsEmpty()) str.Format("无");
		if(((CButton *)GetDlgItem(IDC_CHECK_LZXB))->GetState() & 0x0003)	str.Format("有");
		g_dbcommand.Param("lzxb").setAsString() = str;

		str = m_IniReader.getKeyValue("颈管细胞","Settings");
		if(str.IsEmpty()) str.Format("无");
		if(((CButton *)GetDlgItem(IDC_CHECK_JGXB))->GetState() & 0x0003)	str.Format("有");
		g_dbcommand.Param("jgxb").setAsString() = str;

		str = m_IniReader.getKeyValue("化生细胞","Settings");
		if(str.IsEmpty()) str.Format("无");
		if(((CButton *)GetDlgItem(IDC_CHECK_HSXB))->GetState() & 0x0003)	str.Format("有");
		g_dbcommand.Param("hsxb").setAsString() = str;

		str = m_IniReader.getKeyValue("滴虫感染","Settings");
		if(str.IsEmpty()) str.Format("无");
		if(((CButton *)GetDlgItem(IDC_CHECK_DCGR))->GetState() & 0x0003)	str.Format("有");
		g_dbcommand.Param("dcgr").setAsString() = str;

		str = m_IniReader.getKeyValue("霉菌感染","Settings");
		if(str.IsEmpty()) str.Format("无");
		if(((CButton *)GetDlgItem(IDC_CHECK_MJGR))->GetState() & 0x0003)	str.Format("有");
		g_dbcommand.Param("mjgr").setAsString() = str;

		str = m_IniReader.getKeyValue("念珠菌感染","Settings");
		if(str.IsEmpty()) str.Format("无");
		if(((CButton *)GetDlgItem(IDC_CHECK_LZJGR))->GetState() & 0x0003)	str.Format("有");
		g_dbcommand.Param("lzjgr").setAsString() = str;

		str = m_IniReader.getKeyValue("球杆菌感染","Settings");
		if(str.IsEmpty()) str.Format("无");
		if(((CButton *)GetDlgItem(IDC_CHECK_QGJGR))->GetState() & 0x0003)	str.Format("有");
		g_dbcommand.Param("qgjgr").setAsString() = str;

		str = m_IniReader.getKeyValue("放线菌感染","Settings");
		if(str.IsEmpty()) str.Format("无");
		if(((CButton *)GetDlgItem(IDC_CHECK_FXJGR))->GetState() & 0x0003)	str.Format("有");
		g_dbcommand.Param("fxjgr").setAsString() = str;

		str = m_IniReader.getKeyValue("HPV感染","Settings");
		if(str.IsEmpty()) str.Format("无");
		if(((CButton *)GetDlgItem(IDC_CHECK_HPVGR))->GetState() & 0x0003)	str.Format("有");
		g_dbcommand.Param("hpvgr").setAsString() = str;

		str = m_IniReader.getKeyValue("疱疹病毒感染","Settings");
		if(str.IsEmpty()) str.Format("无");
		if(((CButton *)GetDlgItem(IDC_CHECK_BZBDGR))->GetState() & 0x0003)	str.Format("有");
		g_dbcommand.Param("bzbdgr").setAsString() = str;

		str = m_IniReader.getKeyValue("类似乳头状病毒感染","Settings");
		if(str.IsEmpty()) str.Format("无");
		if(((CButton *)GetDlgItem(IDC_CHECK_LSRTZBDGR))->GetState() & 0x0003)	str.Format("有");
		g_dbcommand.Param("lsrtzbdgr").setAsString() = str;
		//----------------------------------------------------------
		
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




BOOL CMedTbs::PreTranslateMessage(MSG* pMsg) 
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
			m_ctrl_bczd.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_bczd.did(TRUE);
			}
			else
			{
				m_ctrl_bczd.did(FALSE);
			}
	
			//---------------------------------
			//((CComboBox *)GetDlgItem(IDC_COMBO_BGGS))->GetWindowRect(&rt);
			//if(rt.PtInRect(pt))
			//{
			//	CWnd *pActiveWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetActiveView();
			//	CWnd *pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CMedImage));
			//	if(pWnd != NULL && pWnd != pActiveWnd)  ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.SetActiveView(pWnd);
			//}

			//---------------------------------
		}

		//--------------------------------
		int WndID[6] = {
			IDC_EDIT_RYSJ,
			IDC_EDIT_ZDYJ,
			IDC_EDIT_BCZD,
			IDC_EDIT_ZZYS,
			IDC_EDIT_SHYS,
			IDC_DATE_BGRQ
		};
		int WndCount = 6;
		
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
					if(!(pMsg->wParam == VK_RETURN && i <= 1))
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
		m_ToolTip_ZDYJ.RelayEvent(pMsg);

	}
	*/
	//?

	return CXTResizeFormView::PreTranslateMessage(pMsg);
}

void CMedTbs::OnButtonCyc() 
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

void CMedTbs::OnButtonBc() 
{
	SendMessage(WM_CONTENTSAVE);

	CWnd *pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CDBList));
	if(pWnd != NULL)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.SetActiveView(pWnd);
		((CEdit *)pWnd->GetDlgItem(IDC_EDIT_BLH))->SetFocus();
	}
}

void CMedTbs::OnCheckJbmyd1() 
{
	if(((CButton *)GetDlgItem(IDC_CHECK_JBMYD1))->GetState() & 0x0003)
	{
		((CButton *)GetDlgItem(IDC_CHECK_JBMYD2))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_CHECK_JBMYD3))->SetCheck(0);
	}

	OnContentModified();
}

void CMedTbs::OnCheckJbmyd2() 
{
	if(((CButton *)GetDlgItem(IDC_CHECK_JBMYD2))->GetState() & 0x0003)
	{
		((CButton *)GetDlgItem(IDC_CHECK_JBMYD1))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_CHECK_JBMYD3))->SetCheck(0);
	}

	OnContentModified();
}

void CMedTbs::OnCheckJbmyd3() 
{
	if(((CButton *)GetDlgItem(IDC_CHECK_JBMYD3))->GetState() & 0x0003)
	{
		((CButton *)GetDlgItem(IDC_CHECK_JBMYD1))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_CHECK_JBMYD2))->SetCheck(0);
	}

	OnContentModified();
}

void CMedTbs::OnCheckXbl1() 
{
	if(((CButton *)GetDlgItem(IDC_CHECK_XBL1))->GetState() & 0x0003)
	{
		((CButton *)GetDlgItem(IDC_CHECK_XBL2))->SetCheck(0);
	}

	OnContentModified();
}

void CMedTbs::OnCheckXbl2() 
{
	if(((CButton *)GetDlgItem(IDC_CHECK_XBL2))->GetState() & 0x0003)
	{
		((CButton *)GetDlgItem(IDC_CHECK_XBL1))->SetCheck(0);
	}

	OnContentModified();
}

void CMedTbs::OnCheckYzfy1() 
{
	if(((CButton *)GetDlgItem(IDC_CHECK_YZFY1))->GetState() & 0x0003)
	{
		((CButton *)GetDlgItem(IDC_CHECK_YZFY2))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_CHECK_YZFY3))->SetCheck(0);
	}

	OnContentModified();
}

void CMedTbs::OnCheckYzfy2() 
{
	if(((CButton *)GetDlgItem(IDC_CHECK_YZFY2))->GetState() & 0x0003)
	{
		((CButton *)GetDlgItem(IDC_CHECK_YZFY1))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_CHECK_YZFY3))->SetCheck(0);
	}

	OnContentModified();
}

void CMedTbs::OnCheckYzfy3() 
{
	if(((CButton *)GetDlgItem(IDC_CHECK_YZFY3))->GetState() & 0x0003)
	{
		((CButton *)GetDlgItem(IDC_CHECK_YZFY1))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_CHECK_YZFY2))->SetCheck(0);
	}

	OnContentModified();
}

void CMedTbs::RefreshBggs()
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

void CMedTbs::SwitchNormalWnd()
{
	switch(CurEdit) {
	case 2:
		((CWnd *)GetDlgItem(IDC_EDIT_RYSJ))->SetFocus();
		OnSetfocusEditRysj();
		break;
	case 3:
		((CWnd *)GetDlgItem(IDC_EDIT_ZDYJ))->SetFocus();
		OnSetfocusEditZdyj();
		break;
	}
}

void CMedTbs::OnContentModified() 
{
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh)) return;

	m_isModified = true;
}

void CMedTbs::OnDateTimeChange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnContentModified();
	
	*pResult = 0;
}

void CMedTbs::SetModifiedFlag()
{
	OnContentModified();
}

BOOL CMedTbs::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
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


void CMedTbs::OnButtonQppj() 
{
	CQppj mydlg(this);
	mydlg.DoModal();
}

void CMedTbs::OnButtonZdmb() 
{
	UpdateData(TRUE);

	CString m_old_rysj = m_rysj;
	CString m_old_zdyj = m_zdyj;

	CSelectBZ Dlg(this,&m_rysj,&m_zdyj);
	Dlg.DoModal();

	if(m_old_rysj.CompareNoCase(m_rysj) || m_old_zdyj.CompareNoCase(m_zdyj))
	{
		OnContentModified();
	}

	UpdateData(FALSE);
}

void CMedTbs::OnButtonTjyz() 
{
	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  return;

	CTjyz mydlg(this);
	mydlg.DoModal();

	RefreshBgzt();
}

void CMedTbs::RefreshBgzt()
{
	int m_bgzt = 0;
	try
	{
		SAString cmdstr = "Select bgzt from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
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

void CMedTbs::OnCloseupComboBggs() 
{
	// TODO: Add your control notification handler code here
	SetFocus();
}

void CMedTbs::OnSetfocusComboBggs() 
{
	// TODO: Add your control notification handler code here
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

void CMedTbs::OnSelchangeComboBggs() 
{
	// TODO: Add your control notification handler code here
	this->SetFocus();
}

void CMedTbs::OnButtonPrint() 
{
	// TODO: Add your control notification handler code here
	CWnd* person = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedPerson));
	person->PostMessage(WM_COMMAND,IDC_BUTTON_PRINT);
}


void CMedTbs::OnSize(UINT nType, int cx, int cy) 
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
