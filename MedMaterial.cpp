// CMedMaterial : implementation file
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "Pathology.h"
#include "MainFrm.h"
#include "PathologyView.h"
#include "PathologyDoc.h"
#include "MedMaterial.h"
#include "StrEdit.h"
#include "Figure.h"
#include "KSFileDialog.h"
#include "NormalWord.h"
#include "SelectBZ.h"
#include "BdSel.h"
#include "MedPerson.h"
#include "QcyzBZ.h"
#include "MyEdit.h"
#include "Qppj.h"
#include "MicroWriterConfig.h"
#include "MicroWriterThread.h"
#include "MedWax.h"
#include "LauchWax.h"
#include "MyFrame.h"
#include "MedPreSlice.h"

#include "CannedTextTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMedMaterial
extern CPathologyApp theApp;

CMedMaterial::CMedMaterial()
	: CXTResizeFormView(CMedMaterial::IDD)
{
	//{{AFX_DATA_INIT(CMedMaterial)
	m_lkzs = 0;
	m_ckzs = 0;
	m_qcfy = _T("");
	m_bbwz = _T("");
	m_qcjlr = _T("");
	m_rysj = _T("");
	//}}AFX_DATA_INIT

	CurEdit = 0;
	pNormalWord = NULL;
	m_bIsFirstInit = true;
}

CMedMaterial::~CMedMaterial()
{
	// TODO: add destruction code here.

}

IMPLEMENT_DYNCREATE(CMedMaterial, CXTResizeFormView)

BEGIN_MESSAGE_MAP(CMedMaterial, CXTResizeFormView)
	//{{AFX_MSG_MAP(CMedMaterial)
	
	ON_EN_SETFOCUS(IDC_EDIT_RYSJ, OnSetfocusEditRysj)
	ON_BN_CLICKED(IDC_BUTTON_CYC, OnButtonCyc)
	ON_BN_CLICKED(IDC_BUTTON_BC, OnButtonBc)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_EN_CHANGE(IDC_EDIT_LKZS, OnContentModified)
	ON_BN_CLICKED(IDC_BUTTON_ZDMB, OnButtonZdmb)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_ZC, OnButtonZc)
	ON_COMMAND(IDM_MATEARIAL_YZ, OnMatearialYz)
	ON_BN_CLICKED(IDC_BUTTON_QPPJ, OnButtonQppj)
	ON_COMMAND(IDM_MATEARIAL_WriterPrint, OnMATEARIALWriterPrint)
	ON_COMMAND(IDM_MATEARIAL_EMBEDBOXPRINT, OnEmbedBoxPrint)
	ON_COMMAND(IDM_MATEARIAL_SelAll, OnMATEARIALSelAll)
	ON_EN_CHANGE(IDC_EDIT_CKZS, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_QCFY, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_BBWZ, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_QCJLR, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_RYSJ, OnContentModified)
	ON_BN_CLICKED(IDC_BUTTON_WAX, OnButtonWax)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CONTENTSAVE, OnContentSave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMedMaterial message handlers

void CMedMaterial::DoDataExchange(CDataExchange* pDX) 
{
	CXTResizeFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMedMaterial)
	DDX_Control(pDX, IDC_HYP_NOTE, m_HypLink_Note);
	DDX_Control(pDX, IDC_EDIT_QCBW, m_ctrl_qcbw);
	DDX_Control(pDX, IDC_EDIT_QCYS, m_ctrl_qcys);
	DDX_Control(pDX, IDC_HYP_BQXX, m_HypLink_Bqxx);
	DDX_Control(pDX, IDC_EDIT_QCFY, m_ctrl_qcfy);
	DDX_Control(pDX, IDC_TREE_BWMX, m_TreeCtrl_BWMX);
	DDX_Control(pDX, IDC_TREE_RYSJ, m_TreeCtrl_RYSJ);
	DDX_Control(pDX, IDC_EDIT_QCJLR, m_ctrl_qcjlr);
	DDX_Control(pDX, IDC_EDIT_BBWZ, m_ctrl_bbwz);
	DDX_Control(pDX, IDC_EDIT_RYSJ, m_ctrl_rysj);
	DDX_Text(pDX, IDC_EDIT_LKZS, m_lkzs);
	DDX_Text(pDX, IDC_EDIT_CKZS, m_ckzs);
	DDX_Text(pDX, IDC_EDIT_QCFY, m_qcfy);
	DDX_Text(pDX, IDC_EDIT_BBWZ, m_bbwz);
	DDX_Text(pDX, IDC_EDIT_QCJLR, m_qcjlr);
	DDX_Text(pDX, IDC_EDIT_RYSJ, m_rysj);
	//}}AFX_DATA_MAP
}

void CMedMaterial::OnInitialUpdate() 
{
	CXTResizeFormView::OnInitialUpdate();
	
	SetResize(IDC_COMBO_BGGS, SZ_TOP_LEFT, SZ_TOP_RIGHT); 

	if (m_bIsFirstInit)
	{
		m_bIsFirstInit = false;

		m_HypLink_Note.SetURL("");
		m_HypLink_Note.SetColors(RGB(0xFF, 0x00, 0x00), RGB(0xFF, 0x00, 0x00), RGB(0xFF, 0x00, 0x00));
		m_HypLink_Note.SetUnderline(false);
		m_HypLink_Note.EnableShellExecute(false);

		m_HypLink_Bqxx.SetURL("待执行的补取医嘱");
		m_HypLink_Bqxx.SetColors(RGB(0x00, 0x00, 0xFF), RGB(0x00, 0x00, 0xFF), RGB(0x00, 0x00, 0xFF));
		m_HypLink_Bqxx.SetUnderline(false);
		m_HypLink_Bqxx.EnableShellExecute(false);

		m_HypLink_Bqxx.SetWindowText("");
		m_HypLink_Bqxx.Invalidate();

		m_MaterialList.AttachGrid(this,IDC_STATIC_QCXX);
		m_MaterialList.SetParent(this);
		m_MaterialList.ResetGridSize();
		m_MaterialList.RedrawAll();

		int bdnum = 0;
		for(int k = 0; k < m_MaterialList.GetNumberRows(); k++)
		{
			CString cellinfo = m_MaterialList.QuickGetText(0, k);
			if(cellinfo.CompareNoCase("冰冻") == 0)  bdnum++;
		}
		if(bdnum)
		{
			CString info;
			info.Format("【冰冻数：%d】", bdnum);
			m_HypLink_Note.SetWindowText(info);
			m_HypLink_Note.Invalidate();
		}
		else
		{
			m_HypLink_Note.SetWindowText("");
			m_HypLink_Note.Invalidate();
		}

		((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("常规");
		((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("冰冻");
		((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("冰余");
		((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("冰石");
		((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("补取");
		((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("脱钙");
		((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->SetCurSel(0);

		SetDlgItemText(IDC_EDIT_CKS, "1");

		CStdioFile File;
		if ( File.Open(theApp.m_Exe_Working_Dir + "system\\切片子号.txt", CFile::modeRead | CFile::typeText) )
		{
			CString tmp;
			while(1)
			{
				if( File.ReadString(tmp) == FALSE ) break;
				tmp.TrimLeft(); tmp.TrimRight();
				if(!tmp.IsEmpty())
				{
					((CComboBox *)GetDlgItem(IDC_COMBO_XH))->AddString(tmp);
				}
			}
			File.Close();
		}

		if(((CComboBox *)GetDlgItem(IDC_COMBO_XH))->GetCount() > 0)
		{
			((CComboBox *)GetDlgItem(IDC_COMBO_XH))->SetCurSel(0);
		}
		//--------------------------------------------------
		m_TreeCtrl_RYSJ.EnableMultiSelect(FALSE);
		m_TreeCtrl_BWMX.EnableMultiSelect(FALSE);
		
		m_ToolTip_RYSJ.Create(this);
		m_ToolTip_RYSJ.Activate(TRUE);
		m_ToolTip_RYSJ.AddTool(&m_TreeCtrl_RYSJ);
		m_ToolTip_RYSJ.SetMaxTipWidth(300);
		m_TreeCtrl_RYSJ.SetToolTips(&m_ToolTip_RYSJ);


		m_ToolTip_BWMX.Create(this);
		m_ToolTip_BWMX.Activate(TRUE);
		m_ToolTip_BWMX.AddTool(&m_TreeCtrl_BWMX);
		m_ToolTip_BWMX.SetMaxTipWidth(300);
		m_TreeCtrl_BWMX.SetToolTips(&m_ToolTip_BWMX);


		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_IMGLIST_VIEW);
		
		m_ImageList.Create( 16, 16, ILC_COLOR24 | ILC_MASK, 1, 1 );
		m_ImageList.Add( &bitmap, RGB( 0x00,0xff,0x00 ) );

		m_TreeCtrl_RYSJ.Initialize(1, IDC_EDIT_RYSJ, this);
		m_TreeCtrl_BWMX.Initialize(4, 0, this, &m_MaterialList);
		
		m_TreeCtrl_BWMX.SetImageList(&m_ImageList, TVSIL_NORMAL);
		m_TreeCtrl_RYSJ.SetImageList(&m_ImageList, TVSIL_NORMAL);
		
		// initialize cool menus
		m_coolMenu.HookWindow(m_hWnd);
		m_coolMenu.LoadToolbar(IDR_POPUP_TREE);

		m_CurrentTreeMode = 1;
		UpdateTreeCtrl(1);//

		//--------------------------------------------------
		CRect rt;
		GetDlgItem(IDC_DATE_QCRQ)->GetWindowRect(&rt);
		ScreenToClient(&rt);
		if (!m_Time_Qcrq.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT, //,DTS_LONGDATEFORMAT
			rt, this, 112))
		{
			return;
		}

		m_Time_Qcrq.SetTime(theApp.GetOleServerDateTime());

		theApp.ConnectDatabase();

		for(int i = 0; i < theApp.m_fnamelist.GetSize(); i++)
		{
			if(theApp.m_fnamelist.GetAt(i).CompareNoCase("bbwz") == 0)
			{
				m_ctrl_bbwz.m_info = theApp.m_finfolist.GetAt(i);
			}
			if(theApp.m_fnamelist.GetAt(i).CompareNoCase("qcjlr") == 0)
			{
				m_ctrl_qcjlr.m_info = theApp.m_finfolist.GetAt(i);
			}
			if(theApp.m_fnamelist.GetAt(i).CompareNoCase("qcfy") == 0)
			{
				m_ctrl_qcfy.m_info = theApp.m_finfolist.GetAt(i);
			}
			if(theApp.m_fnamelist.GetAt(i).CompareNoCase("qcbw") == 0)  //bbmc
			{
				m_ctrl_qcbw.m_info = theApp.m_finfolist.GetAt(i);
			}
			if(theApp.m_fnamelist.GetAt(i).CompareNoCase("qcys") == 0)
			{
				m_ctrl_qcys.m_info = theApp.m_finfolist.GetAt(i);
			}
		}

		m_ctrl_bbwz.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_qcjlr.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_qcfy.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_qcbw.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_qcys.SetBrowseStyle(BES_XT_POPUPMENU,0,0);

		m_ctrl_qcbw.m_TableName.Format("qcbw");
		return;
	}	

	if(((CComboBox *)GetDlgItem(IDC_COMBO_XH))->GetCount() > 0)
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_XH))->SetCurSel(0);
	}
	SetDlgItemText(IDC_COMBO_XH, theApp.m_InitSubCode);

	//---------------------------------------------------
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		GetDlgItem(IDC_BUTTON_BC)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_BC)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
	}
	//---------------------------------------------------

	CPathologyView *pView = theApp.GetView();
	ASSERT (pView != NULL);

	if(pView->GetDocument()->m_IsNewRecord)
	{
		m_lkzs = 0;
		m_ckzs = 0;
		m_qcfy = _T("");
		m_bbwz = _T("");
		m_qcjlr = _T("");
		m_rysj = _T("");
	}
	else
	{
		try
		{
			CString fieldstr;
			fieldstr.Format("lkzs,ckzs,qcfy,bbwz,qcjlr,rysj,bbmc,bgzt");
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
				m_lkzs = g_dbcommand.Field("lkzs").asLong();
				m_ckzs = g_dbcommand.Field("ckzs").asLong();
				m_qcfy = g_dbcommand.Field("qcfy").asString();
				m_bbwz = g_dbcommand.Field("bbwz").asString();
				m_qcjlr = g_dbcommand.Field("qcjlr").asString();
				m_rysj = g_dbcommand.Field("rysj").asString();
				m_bgzt = g_dbcommand.Field("bgzt").asLong();
				SetDlgItemText(IDC_EDIT_QCBW, g_dbcommand.Field("bbmc").asString());
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

	if(m_bgzt >= 1)
	{
		GetDlgItem(IDC_BUTTON_ZC)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_ZC)->EnableWindow(TRUE);
	}

	UpdateData(FALSE);

	m_MaterialList.ResetGridSize();
	m_MaterialList.RedrawAll();

	int bdnum = 0;
	for(int k = 0; k < m_MaterialList.GetNumberRows(); k++)
	{
		CString cellinfo = m_MaterialList.QuickGetText(0, k);
		if(cellinfo.CompareNoCase("冰冻") == 0)  bdnum++;
	}
	if(bdnum)
	{
		CString info;
		info.Format("[冰冻数：%d]", bdnum);
		m_HypLink_Note.SetWindowText(info);
		m_HypLink_Note.Invalidate();
	}
	else
	{
		m_HypLink_Note.SetWindowText("");
		m_HypLink_Note.Invalidate();
	}

	RefreshBqInfo();

	m_isModified = false;
}

LRESULT CMedMaterial::OnContentSave(WPARAM wParam, LPARAM lParam)
{
	if(!m_isModified)  return 0;

	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  return 0;

	CPathologyView *pView = theApp.GetView();
	ASSERT (pView != NULL);

	theApp.ConnectDatabase();
	
	int m_bgzt = 0;
	CString xRysj;  
	xRysj.Empty();
	
	try
	{
		SAString cmdstr = "Select TOP 1 bgzt, rysj from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			cmdstr = "Select bgzt, rysj from " + theApp.TABLE_MAIN + " where medicalno = :medicalno and ROWNUM<=1";
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

	//----------------------------------------------------
	if(theApp.m_QcjlrMode.CompareNoCase("是") == 0 && m_qcjlr.IsEmpty() && (/*m_MaterialList.GetNumberRows() > 0 || */!m_rysj.IsEmpty()))
	{
		m_qcjlr = theApp.m_Cpp_User;
		SetDlgItemText(IDC_EDIT_QCJLR, m_qcjlr);
	}
	//----------------------------------------------------

	bool Rysj_Suspicious = false;
	m_rysj.TrimLeft(); m_rysj.TrimRight();

	if(m_rysj.IsEmpty() && !xRysj.IsEmpty())
	{
		Rysj_Suspicious = true;
	}

	if(Rysj_Suspicious)
	{
		CString value, nContent;
		value.Empty(); nContent.Empty();
		if(Rysj_Suspicious)
		{
			if(!nContent.IsEmpty()) nContent += "、";
			nContent += "“肉眼所见”";
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
				Rysj_Suspicious = false;
			}
		}
	}

	// save other field
	try
	{
		CString fieldstr;
		fieldstr.Empty();
		fieldstr += "lkzs=:lkzs,";
		fieldstr += "ckzs=:ckzs,";
		fieldstr += "qcfy=:qcfy,";
		fieldstr += "bbwz=:bbwz,";
		fieldstr += "qcjlr=:qcjlr,";
		if(!Rysj_Suspicious)
		{
			fieldstr += "rysj=:rysj";
		}
		
		SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set " + fieldstr + " where medicalno = :medicalno";

		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = pView->GetDocument()->m_blh;
		g_dbcommand.Param("lkzs").setAsLong()    = m_lkzs;
		g_dbcommand.Param("ckzs").setAsLong()    = m_ckzs;
		g_dbcommand.Param("qcfy").setAsString()  = m_qcfy;
		g_dbcommand.Param("bbwz").setAsString()  = m_bbwz;
		g_dbcommand.Param("qcjlr").setAsString() = m_qcjlr;
		if(!Rysj_Suspicious)
		{
			g_dbcommand.Param("rysj").setAsString()  = m_rysj;
			xRysj = m_rysj;
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

	//-----------------------------------------------
	int QcCount = 0;
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = pView->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			QcCount = g_dbcommand.Field(1).asLong();
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

	if(QcCount == 0 && xRysj.IsEmpty())
	{
		int i = 0x01;  i = ~i;
		m_bgzt = (m_bgzt & i);
	}
	else
	{
		m_bgzt = (m_bgzt | 0x01);
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

	//-----------------------------------------------
	m_isModified = false;

	//-----------------------------------------------
	if(m_MaterialList.GetNumberRows() < 1)
	{
		// 取材工作站保存病历时，用户如果没有录入或忘了录入取材明细记录，系统给出提示
		AfxMessageBox("当前病历无取材明细记录？");
	}

	return 1;
}


BOOL CMedMaterial::PreTranslateMessage(MSG* pMsg) 
{
	//?
	/*

	if (m_hWnd)
	{
		if (pMsg->message == WM_LBUTTONDOWN)
		{
			CPoint pt;
			GetCursorPos(&pt);
			CRect rt;

			m_MaterialList.GetWindowRect(&rt);

			if (rt.PtInRect(pt))
			{
				UpdateTreeCtrl(4, FALSE);
			}
		}

		if(pMsg->message == WM_MOUSEMOVE)
		{
			CPoint pt;
			GetCursorPos(&pt);
			CRect rt;
			
			
			m_MaterialList.GetWindowRect(&rt);
			if (rt.PtInRect(pt))
			{	
				if (GetFocus() == GetDlgItem(IDC_EDIT_RYSJ)) {
				
					return TRUE;
				}

			///	if (m_ni != 1)
			///	{
			///		m_CurrentTreeMode = 4;
			///		UpdateTreeCtrl(m_CurrentTreeMode);
			///	}

			///	i = 1;

			} 

			m_ctrl_bbwz.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				
				m_ctrl_bbwz.did(TRUE);
			}
			else
			{
				m_ctrl_bbwz.did(FALSE);
			}
			m_ctrl_qcjlr.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_qcjlr.did(TRUE);
			}
			else
			{
				m_ctrl_qcjlr.did(FALSE);
			}
			m_ctrl_qcfy.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_qcfy.did(TRUE);
			}
			else
			{
				m_ctrl_qcfy.did(FALSE);
			}
			m_ctrl_qcbw.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_qcbw.did(TRUE);
			}
			else
			{
				m_ctrl_qcbw.did(FALSE);
			}
			m_ctrl_qcys.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_qcys.did(TRUE);
			}
			else
			{
				m_ctrl_qcys.did(FALSE);
			}
		}

		//--------------------------------
		int WndID[6] = {
			IDC_EDIT_LKZS,
			IDC_EDIT_CKZS,
			IDC_EDIT_QCFY,
			IDC_EDIT_BBWZ,
			IDC_EDIT_QCJLR,
			IDC_EDIT_RYSJ
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
					if(!(pMsg->wParam == VK_RETURN && i == 5))
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

					//if( i == (WndCount-1) && pMsg->wParam == VK_RETURN )
					//{
					//	pMsg->wParam = VK_TAB;
					//	pMsg->lParam = 0;
					//}

				}
				if(IsSet)
					return TRUE;
			}
		}
		//--------------------------------

		m_ToolTip_RYSJ.RelayEvent(pMsg);
		m_ToolTip_BWMX.RelayEvent(pMsg);
	}
	*/
	//?

	return CXTResizeFormView::PreTranslateMessage(pMsg);
}

/*
mode
肉眼所见 1;
光镜所见 2;
诊断所见 3;
*/
void CMedMaterial::UpdateTreeCtrl(int mode, BOOL bRebuildTree)
{
	switch(mode)
	{
	case 1:
		m_TreeCtrl_BWMX.ShowWindow(SW_HIDE);
		m_TreeCtrl_RYSJ.ShowWindow(SW_SHOW);
		m_pCurrentTreeCtrl = &m_TreeCtrl_RYSJ;		
		SetDlgItemText(IDC_STATIC_BWBZ, "词典－肉眼所见");			
		break;
	case 4:
		m_TreeCtrl_RYSJ.ShowWindow(SW_HIDE);
		m_TreeCtrl_BWMX.ShowWindow(SW_SHOW);
		m_pCurrentTreeCtrl = &m_TreeCtrl_BWMX;		
		SetDlgItemText(IDC_STATIC_BWBZ, "词典－部位明细");
		break;
	}

	m_CurrentTreeMode = mode;

	if (bRebuildTree)
	{
		m_TreeCtrl_RYSJ.BuildTree();
		m_TreeCtrl_BWMX.BuildTree();
	}
	
	return;
}


void CMedMaterial::OnSetfocusEditRysj() 
{
	// TODO: Add your control notification handler code here
	///i = 0;

	if (pNormalWord != NULL && pNormalWord->IsWindowVisible() && CurEdit != 3)
	{
		CurEdit=3;	
		pNormalWord->AdjustPosition();
	}
	else
	{
		CurEdit=3;	
	}

	UpdateTreeCtrl(1, FALSE);
}

void CMedMaterial::OnButtonCyc() 
{
	// TODO: Add your control notification handler code here
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

void CMedMaterial::OnButtonBc() 
{
	// TODO: Add your control notification handler code here
	PostMessage(WM_CONTENTSAVE);
	CWnd *pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CDBList));
	if(pWnd != NULL)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.SetActiveView(pWnd);
		((CEdit *)pWnd->GetDlgItem(IDC_EDIT_BLH))->SetFocus();
	}
}

void CMedMaterial::OnDestroy() 
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

void CMedMaterial::OnButtonAdd() 
{
	
	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty()) return;
	
	CString rwly,xh,qcbw,qcys;
	GetDlgItemText(IDC_COMBO_RWLY, rwly);    rwly.TrimLeft();  rwly.TrimRight();
	GetDlgItemText(IDC_COMBO_XH, xh);        xh.TrimLeft();    xh.TrimRight();
	GetDlgItemText(IDC_EDIT_QCBW, qcbw);     qcbw.TrimLeft();  qcbw.TrimRight();
	int cks = GetDlgItemInt(IDC_EDIT_CKS); 
	GetDlgItemText(IDC_EDIT_QCYS, qcys);     qcys.TrimLeft();  qcys.TrimRight();
	
	if(qcys.IsEmpty())
	{
		AfxMessageBox("请选择取材医生!");
		return;
	}

	CString nQcys="";
	try
	{
		SAString cmdstr = "select qcys from "+ theApp.TABLE_MAIN + " where medicalno=:medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
			
		while(g_dbcommand.FetchNext())
		{
			nQcys = g_dbcommand.Field(1).asString();
		}
		g_dbconnection.Commit();
	}
	catch (SAException &e)
	{
	}

	if(nQcys.CompareNoCase(qcys) != 0)
	{
		try
		{
			SAString cmdstr = "update " + theApp.TABLE_MAIN + " set qcys='"+qcys+"' where medicalno =:medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Execute();
			g_dbconnection.Commit();
			//theApp.OpenDocumentFile(theApp.GetView()->GetDocument()->m_blh);
			//theApp.GetView()->OnPaint();
			CWnd *pWnd = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedPerson));
			if(pWnd != NULL)  ((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_QCYS,qcys);
		} 
		catch (SAException &e)
		{
			try
			{
				g_dbconnection.Rollback();
			} catch (SAException &e)
			{
			}
			AfxMessageBox((const char*)e.ErrText());
		}
	}
	//if(rwly.IsEmpty() || xh.IsEmpty() || qcbw.IsEmpty() || qcys.IsEmpty() || cks <= 0)
//	if(rwly.IsEmpty() || qcbw.IsEmpty() || qcys.IsEmpty() || cks <= 0)
//	{
//		AfxMessageBox("输入了无效的数据,请检查输入值的有效性!");
//		return;
//	}
	CStringArray xhList;
	if(xh.Find("~") > 0)
	{
		CString sub_left,sub_right;
		sub_left = xh.Left(xh.Find("~"));
		sub_right = xh.Right(xh.GetLength() - xh.Find("~") - 1);

		for(int i = 0; i < sub_left.GetLength(); i++)
		{
			if(sub_left.GetAt(i) >= 0x30 && sub_left.GetAt(i) <= 0x39)
			{
				break;
			}
		}

		for(int j = 0; j < sub_right.GetLength(); j++)
		{
			if(sub_right.GetAt(j) >= 0x30 && sub_right.GetAt(j) <= 0x39)
			{
				break;
			}
		}

		if(i != j)
		{
			AfxMessageBox("序号输入格式不正确！");
			return;
		}

		if(i == sub_left.GetLength() && j == sub_left.GetLength())
		{
			if(i == 1)
			{
				char l = sub_left.GetAt(0);
				char r = sub_right.GetAt(0);
				if( l >= r)
				{
					AfxMessageBox("序号输入格式不正确！");
					return;
				}
				while(l <= r)
				{
					xhList.Add(l);
					l = l + 1;
				}
			}
			else
			{
				AfxMessageBox("序号输入格式不正确！");
				return;
			}
		}
		else
		{
			int num_left,num_right;
			num_left = atol(sub_left.Right(sub_left.GetLength() - i));
			num_right = atol(sub_right.Right(sub_right.GetLength() - j));
			if(num_left >= num_right)
			{
				AfxMessageBox("序号输入格式不正确！");
				return;
			}
			else
			{
				int count = 0;
				while(num_left <= num_right)
				{
					if(count >= 50)	//最多50个
					{
						break;
					}
					CString temp;
					temp.Format("%d",num_left);
					xhList.Add(sub_left.Left(i) + temp);
					num_left++;
					count++;
				}
			}
		}
		
	}
	else
	{
		xhList.Add(xh);
	}

	bool IsInsertionSuccessFul = false;
	for(int n = 0; n < xhList.GetSize(); n++)
	{
		COleDateTime m_time;
		m_Time_Qcrq.GetTime(m_time);
		COleDateTime m_CurTime = COleDateTime::GetCurrentTime();
		timeb timeBuffer;
		ftime(&timeBuffer);
		//SADateTime qcrq_dtValue(m_time.GetYear(),m_time.GetMonth(),m_time.GetDay(),m_CurTime.GetHour(),m_CurTime.GetMinute(),m_CurTime.GetSecond());
		
		//qcrq_dtValue.m_nFraction
		int count = m_CurTime.GetDay() - m_time.GetDay();
		if(count > 0 )
		{
			timeBuffer.time = timeBuffer.time - count*24*60*60;
		}
		else if(count < 0)
		{
			timeBuffer.time = timeBuffer.time + count*24*60*60;
		}
		SADateTime time(timeBuffer);
		 
		//time = SADateTime::currentDateTimeWithFraction();

		int row_num = 0;
		try
		{
			SAString cmdstr = "Select count(*) from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno and xh = :xh";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Param("xh").setAsString() = xhList.GetAt(n);
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
			AfxMessageBox("数据库中已经存在指定病理号和序号的取材记录!");
			return;
		}

		CString qpsm;
		if(rwly.CompareNoCase("冰冻") == 0 || rwly.CompareNoCase("脱钙") == 0 || rwly.CompareNoCase("冰石") == 0 || rwly.CompareNoCase("冰余") == 0 || rwly.CompareNoCase("补取") == 0)
		{
			if(rwly.CompareNoCase("冰冻") == 0 && theApp.m_BdSliceMode.CompareNoCase("冰冻"))
			{
				CString nBdh;
				theApp.GetDatabaseValue(theApp.GetView()->GetDocument()->m_blh, "bdh", nBdh, true);
				//g_dbcommand.Param("qpsm").setAsString() = "冰" + nBdh;
				qpsm = "冰" + nBdh;
			} 
			else
			{
				//g_dbcommand.Param("qpsm").setAsString() = rwly;
				qpsm = rwly;
			}
		}
		else
		{
			if(rwly.CompareNoCase("常规") == 0)
			{
				CString qmsmstr;  qmsmstr.Empty();
				if(theApp.m_NormalSliceMode.CompareNoCase("取材部位") == 0)
				{
					qmsmstr = qcbw;
				}
				if(theApp.m_NormalSliceMode.CompareNoCase("HE") == 0)
				{
					qmsmstr = "HE";
				}
				
				//g_dbcommand.Param("qpsm").setAsString() = qmsmstr;
				qpsm = qmsmstr;
			}
			// 				else
			// 				{
			// 					g_dbcommand.Param("qpsm").setAsString() = "";
			// 				}
		}
		
		try
		{
			SAString cmdstr = "Insert into " + theApp.TABLE_MATERIAL + "(medicalno,rwly,xh,qcbw,cks,qcys,qcrq,zpzt,lkh,qptmh,qpsm,dyzt) values(:medicalno,:rwly,:xh,:qcbw,:cks,:qcys,:qcrq,0,:lkh,'" + GenNewSliceNo(theApp.GetView()->GetDocument()->m_blh) + "','"+qpsm+"',0)";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Param("rwly").setAsString() = rwly;
			g_dbcommand.Param("xh").setAsString() = xhList.GetAt(n);
			g_dbcommand.Param("qcbw").setAsString() = qcbw;
			g_dbcommand.Param("cks").setAsLong() = cks;
			g_dbcommand.Param("qcys").setAsString() = qcys;
			g_dbcommand.Param("qcrq").setAsDateTime() = time;
			if(xhList.GetAt(n).IsEmpty())
			{
				g_dbcommand.Param("lkh").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			}
			else
			{
				g_dbcommand.Param("lkh").setAsString() = theApp.GetView()->GetDocument()->m_blh + "-" + xhList.GetAt(n);
			}
			
		//	g_dbcommand.Param("qptmh").setAsString() = ;
			
			
			g_dbcommand.Execute();
			g_dbconnection.Commit();
			
			IsInsertionSuccessFul = true;
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
	

	m_MaterialList.ResetGridSize();
	m_MaterialList.RedrawAll();
//	m_MaterialList.ClearSelections();
	m_MaterialList.GotoRow(m_MaterialList.GetNumberRows() - 1);

	int bdnum = 0;
	for(int k = 0; k < m_MaterialList.GetNumberRows(); k++)
	{
		CString cellinfo = m_MaterialList.QuickGetText(0, k);
		if(cellinfo.CompareNoCase("冰冻") == 0)  bdnum++;
	}
	if(bdnum)
	{
		CString info;
		info.Format("【冰冻数：%d】", bdnum);
		m_HypLink_Note.SetWindowText(info);
		m_HypLink_Note.Invalidate();
	}
	else
	{
		m_HypLink_Note.SetWindowText("");
		m_HypLink_Note.Invalidate();
	}
	
	if(rwly.CompareNoCase("脱钙") == 0)
	{
		SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set wfyy='脱钙' where medicalno = :medicalno";	
		
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		g_dbconnection.Commit();
	}
	//-----------------------------------
	if(IsInsertionSuccessFul && (theApp.m_QcysTbXr.CompareNoCase("初诊医生") == 0 || theApp.m_QcysTbXr.CompareNoCase("主诊医生") == 0))
	{ 
		SAString cmdstr;
		try
		{
		
			if(theApp.m_QcysTbXr.CompareNoCase("初诊医生") == 0)
				cmdstr = "Update " + theApp.TABLE_MAIN + " set czys=:czys where medicalno = :medicalno";
			else
				cmdstr = "Update " + theApp.TABLE_MAIN + " set zzys=:zzys where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			if(theApp.m_QcysTbXr.CompareNoCase("初诊医生") == 0)
				g_dbcommand.Param("czys").setAsString() = qcys;
			else
				g_dbcommand.Param("zzys").setAsString() = qcys;
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
	}
	//-----------------------------------
	if(xhList.GetSize() == 1)
	{
		CString str,str1;
		char strbuf[300];
		int Len=xh.GetLength();
		BYTE ch;
		for(k=(Len-1);k>=0;k--)
		{
			ch = xh.GetAt(k); 
			if(!(ch >= 0x30 && ch <= 0x39))  break;
		}
		k = Len-1-k;
		if(k > 0)
		{
			ltoa(atol(xh.Right(k))+1,strbuf,10);
			str1.Format("%s",strbuf);
			str = xh.Left(Len-k);
			Len = k - str1.GetLength();
			for(k=0;k<Len;k++)  str += "0";
			str += str1;
			
			SetDlgItemText(IDC_COMBO_XH, str);
		}
		else
		{
			if(xh.GetLength() > 0)
			{
				BYTE ch = xh.GetAt(xh.GetLength() - 1);
				ch += 1;
				xh.SetAt(xh.GetLength() - 1, ch);
				
				SetDlgItemText(IDC_COMBO_XH, xh);
			}
		}
	}
	
	//-----------------------------------

}

void CMedMaterial::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	if(!RIGHT_XGSCQCJL(theApp.m_Cpp_Right)){
		AfxMessageBox("您没有删除权限!");
		return;
	}

	CStringArray  XhNolist;
	CUIntArray  rowlist;
	int nCol, nRet;
	long nRow;

	nRet = m_MaterialList.EnumFirstSelected( &nCol, &nRow );

	while( nRet == UG_SUCCESS )
	{
		if(nRow >= 0)
		{
			int rowcount = rowlist.GetSize();
			for(int i = 0; i < rowcount; i ++ )
			{
				if( nRow == rowlist.GetAt(i) )  break;
			}
			if( i >= rowcount )
			{
				CString str_Xh;
				
				CUGCell  cell;
				m_MaterialList.GetCellIndirect(1,nRow,&cell);
				cell.GetText(&str_Xh);
				
				rowlist.Add(nRow);
				XhNolist.Add(str_Xh);
			}
		}
		
		nRet = m_MaterialList.EnumNextSelected( &nCol, &nRow );
	}

	rowlist.RemoveAll();

	if( XhNolist.GetSize() <= 0 )
	{
		return;
	}

	for(int index = 0; index < XhNolist.GetSize(); index++)
	{
		try
		{
			SAString cmdstr = "Delete from " + theApp.TABLE_MATERIAL + "  where medicalno = :medicalno and xh = :xh"; /*and zpzt = 0";*/
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				if(XhNolist.GetAt(index).IsEmpty())
				{
					cmdstr = "Delete from " + theApp.TABLE_MATERIAL + "  where medicalno = :medicalno and xh is null";
				}
			}
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			if(!(theApp.m_DBType.CompareNoCase("ORACLE") == 0 && XhNolist.GetAt(index).IsEmpty()))
			{
				g_dbcommand.Param("xh").setAsString()        = XhNolist.GetAt(index);
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
			return;
		}
	}

	XhNolist.RemoveAll();

	m_MaterialList.ResetGridSize();
	m_MaterialList.RedrawAll();

	int bdnum = 0;
	for(int k = 0; k < m_MaterialList.GetNumberRows(); k++)
	{
		CString cellinfo = m_MaterialList.QuickGetText(0, k);
		if(cellinfo.CompareNoCase("冰冻") == 0)  bdnum++;
	}
	if(bdnum)
	{
		CString info;
		info.Format("【冰冻数：%d】", bdnum);
		m_HypLink_Note.SetWindowText(info);
		m_HypLink_Note.Invalidate();
	}
	else
	{
		m_HypLink_Note.SetWindowText("");
		m_HypLink_Note.Invalidate();
	}
}

void CMedMaterial::RefreshBqInfo()
{
	CPathologyView *pView = theApp.GetView();
	ASSERT (pView != NULL);

	m_HypLink_Bqxx.SetWindowText("");
	m_HypLink_Bqxx.Invalidate();

    if(pView->GetDocument()->m_blh.IsEmpty())
	{
		return;
	}

	theApp.ConnectDatabase();

	try
	{
		//SAString cmdstr = "Select * from " + theApp.TABLE_ADVICE + " where medicalno = :medicalno and yzlx = '补取' and yzzt = 0 order by sqrq asc";
		SAString cmdstr = "Select * from " + theApp.TABLE_ADVICE + " where medicalno = :medicalno and yzlx = '补取' order by sqrq asc";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = pView->GetDocument()->m_blh;
		
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
		{
			CString str;
			if(g_dbcommand.Field("yzzt").asBool())
				str.Format("[补取: %s  %d 申请医生: %s]",g_dbcommand.Field("bz").asString(),g_dbcommand.Field("qps").asLong(),g_dbcommand.Field("sqys").asString());
			else
				str.Format("[补取: %s  %d 申请医生: %s]",g_dbcommand.Field("bz").asString(),g_dbcommand.Field("qps").asLong(),g_dbcommand.Field("sqys").asString());

			m_HypLink_Bqxx.SetWindowText(str);
			m_HypLink_Bqxx.Invalidate();
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

void CMedMaterial::OnContentModified() 
{
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh)) return;

	m_isModified = true;
}

void CMedMaterial::SetModifiedFlag()
{
	OnContentModified();
}

BOOL CMedMaterial::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
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


void CMedMaterial::OnButtonZdmb() 
{
	UpdateData(TRUE);

	CString m_old_rysj = m_rysj;
	CString tmp;

	CSelectBZ Dlg(this,&m_rysj,&tmp);
	Dlg.DoModal();

	if(m_old_rysj.CompareNoCase(m_rysj))
	{
		OnContentModified();
	}

	UpdateData(FALSE);
}

void CMedMaterial::OnButtonPrint() 
{
	CBdSel mydlg(this);
	if(mydlg.DoModal() == IDCANCEL)  return;
	
	mydlg.m_TableName.TrimLeft();  mydlg.m_TableName.TrimRight();
	theApp.m_BdName = mydlg.m_TableName;
	if(theApp.m_BdName.IsEmpty())
	{
		theApp.m_BdName.Format("回执单");
	}
	
	theApp.m_PrintType = 2;
	theApp.GetView()->SendMessage(WM_COMMAND, ID_FILE_PRINT);
}

void CMedMaterial::OnButtonZc() 
{
	if(AfxMessageBox("是否暂存当前录入信息？暂存不改变取材状态！",MB_OKCANCEL) == IDCANCEL){
		return;
	}
	// TODO: Add your control notification handler code here
	if(!m_isModified)  return;

	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  return;

	CPathologyView *pView = theApp.GetView();
	ASSERT (pView != NULL);

	theApp.ConnectDatabase();
	
	int m_bgzt = 0;
	CString xRysj;  
	xRysj.Empty();
	
	try
	{
		SAString cmdstr = "Select TOP 1 bgzt, rysj from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			cmdstr = "Select bgzt, rysj from " + theApp.TABLE_MAIN + " where medicalno = :medicalno and ROWNUM<=1";
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
		}
		else
		{
			//  在数据库中不存在指定活检编号的病例记录
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
	}

	if(m_bgzt & 0x08)	return ;

	UpdateData(TRUE);

	//----------------------------------------------------
	if(theApp.m_QcjlrMode.CompareNoCase("是") == 0 && m_qcjlr.IsEmpty() && (m_MaterialList.GetNumberRows() > 0 || !m_rysj.IsEmpty()))
	{
		m_qcjlr = theApp.m_Cpp_User;
		SetDlgItemText(IDC_EDIT_QCJLR, m_qcjlr);
	}
	//----------------------------------------------------

	bool Rysj_Suspicious = false;
	m_rysj.TrimLeft(); m_rysj.TrimRight();

	if(m_rysj.IsEmpty() && !xRysj.IsEmpty())
	{
		Rysj_Suspicious = true;
	}

	if(Rysj_Suspicious)
	{
		CString value, nContent;
		value.Empty(); nContent.Empty();
		if(Rysj_Suspicious)
		{
			if(!nContent.IsEmpty()) nContent += "、";
			nContent += "“肉眼所见”";
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
				Rysj_Suspicious = false;
			}
		}
	}

	// save other field
	try
	{
		CString fieldstr;
		fieldstr.Empty();
		fieldstr += "lkzs=:lkzs,";
		fieldstr += "ckzs=:ckzs,";
		fieldstr += "qcfy=:qcfy,";
		fieldstr += "bbwz=:bbwz,";
		fieldstr += "qcjlr=:qcjlr,";
		if(!Rysj_Suspicious)
		{
			fieldstr += "rysj=:rysj";
		}
		
		SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set " + fieldstr + " where medicalno = :medicalno";

		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = pView->GetDocument()->m_blh;
		g_dbcommand.Param("lkzs").setAsLong()    = m_lkzs;
		g_dbcommand.Param("ckzs").setAsLong()    = m_ckzs;
		g_dbcommand.Param("qcfy").setAsString()  = m_qcfy;
		g_dbcommand.Param("bbwz").setAsString()  = m_bbwz;
		g_dbcommand.Param("qcjlr").setAsString() = m_qcjlr;
		if(!Rysj_Suspicious)
		{
			g_dbcommand.Param("rysj").setAsString()  = m_rysj;
			xRysj = m_rysj;
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

	//-----------------------------------------------
	int QcCount = 0;
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = pView->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			QcCount = g_dbcommand.Field(1).asLong();
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

	if(QcCount == 0 && xRysj.IsEmpty())
	{
		int i = 0x01;  i = ~i;
		m_bgzt = (m_bgzt & i);
	}
	else
	{
		m_bgzt = (m_bgzt | 0x01);
	}

	try
	{
		SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set bgzt=0 where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		//g_dbcommand.Param("bgzt").setAsLong() = m_bgzt;
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

	//-----------------------------------------------
	m_isModified = false;

	//-----------------------------------------------
	if(m_MaterialList.GetNumberRows() < 1)
	{
		// 取材工作站保存病历时，用户如果没有录入或忘了录入取材明细记录，系统给出提示
		AfxMessageBox("当前病历无取材明细记录？");
	}
	
}

void CMedMaterial::OnMatearialYz() 
{
	// TODO: Add your command handler code here
	CQcyzBZ dlg;
	if(m_MaterialList.isSingleSel == TRUE)
	{
		int col;long row;
		m_MaterialList.EnumFirstSelected(&col,&row);
		dlg.m_qcyz = m_MaterialList.QuickGetText(11,row);
		if(dlg.DoModal() == IDCANCEL)
		{
			return;
		}
		CString qpzt;
		qpzt = m_MaterialList.QuickGetText(6,row);
		if(qpzt.CompareNoCase("已切片") == 0)
		{
		//	result = m_MaterialList.EnumNextSelected(&col,&row);
			return;
		}
		try
		{
			SAString cmdstr = "update "+ theApp.TABLE_MATERIAL + " set bz =:bz where rwly=:rwly and xh=:xh and qcbw=:qcbw";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("bz").setAsString() = dlg.m_qcyz;
			g_dbcommand.Param("rwly").setAsString() = m_MaterialList.QuickGetText(0,row);
			g_dbcommand.Param("xh").setAsString() = m_MaterialList.QuickGetText(1,row);
			g_dbcommand.Param("qcbw").setAsString() = m_MaterialList.QuickGetText(2,row);
			g_dbcommand.Execute();
			g_dbconnection.Commit();
		} 
		catch (SAException &e)
		{
			AfxMessageBox((const char*)e.ErrText());
		}

	} 
	else
	{
		if(dlg.DoModal() == IDCANCEL)
		{
			return;
		}
		
		int col; long row,row_old;
		int result;
		result = m_MaterialList.EnumFirstSelected(&col,&row);
		CString qpzt;
		while(result == UG_SUCCESS)
		{
			qpzt = m_MaterialList.QuickGetText(6,row);
			if(qpzt.CompareNoCase("已切片") == 0)
			{
				result = m_MaterialList.EnumNextSelected(&col,&row);
				continue;
			}
			try
			{
				SAString cmdstr = "update "+ theApp.TABLE_MATERIAL + " set bz =:bz where rwly=:rwly and xh=:xh and qcbw=:qcbw";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("bz").setAsString() = dlg.m_qcyz;
				g_dbcommand.Param("rwly").setAsString() = m_MaterialList.QuickGetText(0,row);
				g_dbcommand.Param("xh").setAsString() = m_MaterialList.QuickGetText(1,row);
				g_dbcommand.Param("qcbw").setAsString() = m_MaterialList.QuickGetText(2,row);
				g_dbcommand.Execute();
				g_dbconnection.Commit();
			} 
			catch (SAException &e)
			{
				AfxMessageBox((const char*)e.ErrText());
			}
			
			result = m_MaterialList.EnumNextSelected(&col,&row);
		}
	
	}
	m_MaterialList.ResetGridSize();
	m_MaterialList.RedrawAll();
	
}

void CMedMaterial::OnButtonQppj() 
{
	// TODO: Add your control notification handler code here
	CQppj mydlg(this);
	mydlg.DoModal();
}

void CMedMaterial::OnMATEARIALWriterPrint() 
{
	// TODO: Add your command handler code here
	CString mno = theApp.GetView()->GetDocument()->m_blh;
	if(mno.IsEmpty())
	{
		return;
	}


	int col; long row,row_old; CStringArray xhList; xhList.RemoveAll();
	int ret = m_MaterialList.EnumFirstSelected(&col,&row);
	xhList.Add(m_MaterialList.QuickGetText(1,row));
	row_old = row;
	while(ret == UG_SUCCESS)
	{
		if(row == row_old)
		{
			ret = m_MaterialList.EnumNextSelected(&col,&row);
			continue;
		}
		xhList.Add(m_MaterialList.QuickGetText(1,row));
		row_old = row;
		ret = m_MaterialList.EnumNextSelected(&col,&row);	
	}
	
	if(theApp.m_MicroWriterMode.CompareNoCase("一行") == 0)
	{
		for(int i = 0; i< xhList.GetSize(); i++)
		{
			CString temp;
			if(xhList.GetAt(i).IsEmpty())
			{
				temp = "$#" + theApp.m_FontOneLine + mno + "\r";
			}
			else
			{
				temp = "$#" + theApp.m_FontOneLine + mno + "-" + xhList.GetAt(i) + "\r";
			}			
			writerCommandList.Add(temp);
		}
	}
	else if(theApp.m_MicroWriterMode.CompareNoCase("两行") == 0)
	{
		for(int i = 0; i< xhList.GetSize(); i++)
		{
			CString temp;
			if(xhList.GetAt(i).IsEmpty())
			{
				temp.Format("$#%s%s#N#%s%s\r",theApp.m_FontFirstLine,theApp.m_FirstLineText,theApp.m_FontSecondLine,mno.Right(mno.GetLength() - atoi(theApp.m_BeginIndex) + 1));
			}
			else
			{
				temp.Format("$#%s%s#N#%s%s-%s\r",theApp.m_FontFirstLine,theApp.m_FirstLineText,theApp.m_FontSecondLine,mno.Right(mno.GetLength() - atoi(theApp.m_BeginIndex) + 1),xhList.GetAt(i));
			}			
			writerCommandList.Add(temp);
		}
	}

	CMicroWriterThread* th;
	th = new CMicroWriterThread;
	th->setCommandList(&writerCommandList);
	th->Start();
}

//包埋盒打印
void CMedMaterial::OnEmbedBoxPrint()
{

	CUIntArray  rowlist;
	long nRow, nRow_old = -1; 
	int nCol, nRet, ni;
	CString mno;
	CWaitCursor wait;
	
	if (theApp.m_IsEmbedBoxValid != "是" || theApp.m_EmbedBoxFilePath.IsEmpty())
		return;

	mno = theApp.GetView()->GetDocument()->m_blh;
	if(mno.IsEmpty())
	{
		return;
	}


	nRet = m_MaterialList.EnumFirstSelected( &nCol, &nRow );
	while( nRet == UG_SUCCESS )
	{
		if (nRow >= 0)
		{
			int rowcount = rowlist.GetSize();

			for (ni = 0; ni < rowcount; ni ++ )
			{
				if( nRow == rowlist.GetAt(ni) )  break;
			}

			if( ni >= rowcount )
			{
				rowlist.Add(nRow);
			}
		}

		nRet = m_MaterialList.EnumNextSelected( &nCol, &nRow );
	}

	for( ni = 0; ni < rowlist.GetSize(); ni++)
	{
		nRow = rowlist.GetAt(ni);
		theApp.MakeEmbedBoxFile(mno, m_MaterialList.QuickGetText(1,nRow), m_MaterialList.QuickGetText(0, nRow));				
		DWORD dwTickCount = GetTickCount();
		while ((GetTickCount() - dwTickCount) < 200)
			Sleep(10);

	}

	rowlist.RemoveAll();

}

void CMedMaterial::OnMATEARIALSelAll() 
{
	// TODO: Add your command handler code here
	m_MaterialList.SelectRange(0, 0, m_MaterialList.GetNumberCols()-1,m_MaterialList.GetNumberRows()-1);
}

void CMedMaterial::OnButtonWax() 
{
	// TODO: Add your control notification handler code here


	pDocTemplate = NULL;
	pView = NULL;

	pDocTemplate = new CSingleDocTemplate(
		NULL,
		NULL,
		RUNTIME_CLASS(CMyFrame),       
		RUNTIME_CLASS(CMedWax));

	CMyFrame* pFrameWnd = (CMyFrame*)pDocTemplate->CreateNewFrame(NULL, NULL);
	pDocTemplate->InitialUpdateFrame(pFrameWnd, NULL, FALSE);
	//pFrameWnd->SetParent(AfxGetMainWnd());
	//pFrameWnd->
	pFrameWnd->ModifyStyle(WS_MAXIMIZEBOX|WS_MINIMIZEBOX,NULL,0);
	pFrameWnd->SetWindowPos(&wndTop,50,50,900,625,NULL);
	pFrameWnd->SetWindowText(_T("待包埋列表"));
	
	theApp.m_pMainWnd->EnableWindow(FALSE);
	pFrameWnd->old = AfxGetApp()->m_pMainWnd;
	AfxGetApp()->m_pMainWnd = pFrameWnd;
	//pFrameWnd->RunModalLoop(MLF_SHOWONIDLE);
	
	pView = (CMedWax*)pFrameWnd->GetActiveView();
	
	


	pFrameWnd->ShowWindow(SW_SHOW);

	pView->OnInitialUpdate();
	
}

CString CMedMaterial::GenNewSliceNo(CString blh)
{
	theApp.ConnectDatabase();
	
	//CString nBlh = blh + "-001";
	CString nBlh = blh + "01";
	//========== Generate proper medicalno automatically ===============
	try
	{
		SAString cmdstr = "Select qptmh from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno order by qptmh desc";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = blh;
		
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			CString curno = g_dbcommand.Field("qptmh").asString();
			if(curno.IsEmpty())
			{
		//		AfxMessageBox(nBlh);
				return nBlh;
			}
			CString str,str1;
			char strbuf[300];
			int Len=curno.GetLength(),k;
			
			k = 2;
			
			ltoa(atol(curno.Right(k))+1,strbuf,10);
			str1.Format("%s",strbuf);
			str = curno.Left(Len-k);
			Len = k - str1.GetLength();
			for(k=0;k<Len;k++)  str += "0";
			str += str1;
			nBlh = str;
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
	
	return nBlh;
}


void CMedMaterial::OnSize(UINT nType, int cx, int cy) 
{
	CRect rect, rcClient;

	CXTResizeFormView::OnSize(nType, cx, cy);
	
	GetClientRect(rcClient);
	m_TreeCtrl_RYSJ.GetWindowRect(&rect);
	ScreenToClient(&rect);

	rect.right	= rcClient.right;
	rect.bottom	= rcClient.bottom;
	m_TreeCtrl_RYSJ.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOREPOSITION);
	m_TreeCtrl_BWMX.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOREPOSITION);

}
