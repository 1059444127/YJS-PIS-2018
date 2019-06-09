// CMedPerson : implementation file
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "Pathology.h"
#include "PathologyView.h"
#include "PathologyDoc.h"
#include "MainFrm.h"
#include "MedPerson.h"
#include "ChargeDetail.h"
#include "BdSel.h"
#include "FieldOut.h"
#include "TextOut.h"
#include "BbmxDetail.h"
#include "tinyxml.h"
#include "TimeQuery.h"

#include "HisconnectData.h"
#include "GetStudyUID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMedPerson
extern CPathologyApp theApp;

CMedPerson::CMedPerson()
	: CXTResizeFormView(CMedPerson::IDD)
{
	//{{AFX_DATA_INIT(CMedPerson)
	m_sOrderType = _T("");
	m_sqh = _T("");
	m_brid = _T("");
	m_bdh = _T("");
	m_blh = _T("");
	m_ybh = _T("");
	m_mzh = _T("");
	m_zyh = _T("");
	m_nldw = _T("岁");
	m_csrq = _T("");
	m_mz = _T("");
	m_hf = _T("");
	m_zy = _T("");
	m_sfzh = _T("");
	m_lxdz = _T("");
	m_lxdh = _T("");
	m_mcyj = _T("");
	m_jj = _T("");
	m_sjdw = _T("");
	m_sjks = _T("");
	m_sjys = _T("");
	m_bq = _T("");
	m_ch = _T("");
	m_bblx = _T("");
	m_bbqk = _T("");
	m_bbmc = _T("");
	m_bhgyy = _T("");
	m_bbqs = _T("");
	m_fb = _T("");
	m_sf = 0.0f;
	m_qcys = _T("");
	m_lczd = _T("");
	m_bszy = _T("");
	m_sssj = _T("");
	m_xm = _T("");
	m_xb = _T("");
	m_nl = _T("");
	//}}AFX_DATA_INIT
	m_sOrderId = _T("");
	m_bIsFirstInit = true;
}

CMedPerson::~CMedPerson()
{
	// TODO: add destruction code here.

}



IMPLEMENT_DYNCREATE(CMedPerson, CXTResizeFormView)

BEGIN_MESSAGE_MAP(CMedPerson, CXTResizeFormView)
	//{{AFX_MSG_MAP(CMedPerson)
	ON_BN_CLICKED(IDC_SF_DETAIL, OnSfDetail)
	ON_BN_CLICKED(IDC_BUTTON_BC, OnButtonBc)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	ON_BN_CLICKED(IDC_HISID_EXTRACT, OnHisidExtract)
	ON_BN_CLICKED(IDC_HISID1_EXTRACT, OnHisid1Extract)
	ON_EN_CHANGE(IDC_EDIT_ORDERTYPE, OnContentModified)
	ON_BN_CLICKED(IDC_MAX_BDH, OnMaxBdh)
	ON_BN_CLICKED(IDC_BUTTON_ZLXX, OnButtonZlxx)
	ON_BN_CLICKED(IDC_BUTTON_FKXX, OnButtonFkxx)
	ON_BN_CLICKED(IDC_BUTTON_JCXX, OnButtonJcxx)
	ON_BN_CLICKED(IDC_BBMC_DETAIL, OnBbmcDetail)
	ON_BN_CLICKED(IDC_BUTTON_SQDDY, OnButtonSqddy)
	ON_BN_CLICKED(IDC_BUTTON_TD, OnButtonTd)
	ON_NOTIFY(DTN_DATETIMECHANGE, 109,  OnDateTimeChange)
	ON_NOTIFY(DTN_DATETIMECHANGE, 110,  OnDateTimeChange)
	ON_NOTIFY(DTN_DATETIMECHANGE, 111,  OnDateTimeChange)
	ON_NOTIFY(DTN_DATETIMECHANGE, 112,  OnDateTimeChange)
	ON_EN_CHANGE(IDC_EDIT_SQH, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_BRID, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_BDH, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_BLH, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_YBH, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_MZH, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_ZYH, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_XM, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_XB, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_NL, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_NLDW, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_CSRQ, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_MZ, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_HF, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_ZY, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_SFZH, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_LXDZ, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_LXDH, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_MCYJ, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_JJ, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_SJDW, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_SJKS, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_SJYS, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_BQ, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_CH, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_BBLX, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_BBQK, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_BBMC, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_BHGYY, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_BBQS, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_FB, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_SF, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_QCYS, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_LCZD, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_BSZY, OnContentModified)
	ON_EN_CHANGE(IDC_EDIT_SSSJ, OnContentModified)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CONTENTSAVE, OnContentSave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMedPerson message handlers
int CMedPerson::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTResizeFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
		
	return 0;
}

void CMedPerson::DoDataExchange(CDataExchange* pDX) 
{
	CXTResizeFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMedPerson)
	DDX_Control(pDX, IDC_BUTTON_TD, m_Button_Td);
	DDX_Control(pDX, IDC_EDIT_JJ, m_ctrl_jj);
	DDX_Control(pDX, IDC_BUTTON_SQDDY, m_Button_Sqddy);
	DDX_Control(pDX, IDC_BBMC_DETAIL, m_btBbmcDetail);
	DDX_Control(pDX, IDC_BUTTON_PRINT, m_Button_Print);
	DDX_Control(pDX, IDC_BUTTON_NEW, m_Button_New);
	DDX_Control(pDX, IDC_BUTTON_BC, m_Button_Bc);
	DDX_Control(pDX, IDC_BUTTON_JCXX, m_Button_Jcxx);
	DDX_Control(pDX, IDC_BUTTON_ZLXX, m_Button_Zlxx);
	DDX_Control(pDX, IDC_BUTTON_FKXX, m_Button_Fkxx);
	DDX_Control(pDX, IDC_MAX_BDH, m_btMaxBdh);
	DDX_Control(pDX, IDC_EDIT_ORDERTYPE, m_ctrl_orderType);
	DDX_Control(pDX, IDC_EDIT_LCZD, m_ctrl_lczd);
	DDX_Control(pDX, IDC_EDIT_QCYS, m_ctrl_qcys);
	DDX_Control(pDX, IDC_EDIT_FB, m_ctrl_fb);
	DDX_Control(pDX, IDC_EDIT_BBQS, m_ctrl_bbqs);
	DDX_Control(pDX, IDC_EDIT_BHGYY, m_ctrl_bhgyy);
	DDX_Control(pDX, IDC_EDIT_BBMC, m_ctrl_bbmc);
	DDX_Control(pDX, IDC_EDIT_BBQK, m_ctrl_bbqk);
	DDX_Control(pDX, IDC_EDIT_BBLX, m_ctrl_bblx);
	DDX_Control(pDX, IDC_EDIT_BQ, m_ctrl_bq);
	DDX_Control(pDX, IDC_EDIT_SJYS, m_ctrl_sjys);
	DDX_Control(pDX, IDC_EDIT_SJKS, m_ctrl_sjks);
	DDX_Control(pDX, IDC_EDIT_SJDW, m_ctrl_sjdw);
	DDX_Control(pDX, IDC_EDIT_ZY, m_ctrl_zy);
	DDX_Control(pDX, IDC_EDIT_HF, m_ctrl_hf);
	DDX_Control(pDX, IDC_EDIT_MZ, m_ctrl_mz);
	DDX_Control(pDX, IDC_EDIT_NLDW, m_ctrl_nldw);
	DDX_Control(pDX, IDC_EDIT_XB, m_ctrl_xb);
	DDX_Control(pDX, IDC_SF_DETAIL, m_btSfDetail);
	DDX_Control(pDX, IDC_HISID_EXTRACT, m_btHisidExtract);
	DDX_Control(pDX, IDC_HISID1_EXTRACT, m_btHisid1Extract);
	DDX_Control(pDX, IDC_EDIT_BSZY, m_ctrl_bszy);
	DDX_Text(pDX, IDC_EDIT_ORDERTYPE, m_sOrderType);
	DDX_Text(pDX, IDC_EDIT_SQH, m_sqh);
	DDX_Text(pDX, IDC_EDIT_BRID, m_brid);
	DDX_Text(pDX, IDC_EDIT_BDH, m_bdh);
	DDX_Text(pDX, IDC_EDIT_BLH, m_blh);
	DDX_Text(pDX, IDC_EDIT_YBH, m_ybh);
	DDX_Text(pDX, IDC_EDIT_MZH, m_mzh);
	DDX_Text(pDX, IDC_EDIT_ZYH, m_zyh);
	DDX_Text(pDX, IDC_EDIT_NLDW, m_nldw);
	DDX_Text(pDX, IDC_EDIT_CSRQ, m_csrq);
	DDX_Text(pDX, IDC_EDIT_MZ, m_mz);
	DDX_Text(pDX, IDC_EDIT_HF, m_hf);
	DDX_Text(pDX, IDC_EDIT_ZY, m_zy);
	DDX_Text(pDX, IDC_EDIT_SFZH, m_sfzh);
	DDX_Text(pDX, IDC_EDIT_LXDZ, m_lxdz);
	DDX_Text(pDX, IDC_EDIT_LXDH, m_lxdh);
	DDX_Text(pDX, IDC_EDIT_MCYJ, m_mcyj);
	DDX_Text(pDX, IDC_EDIT_JJ, m_jj);
	DDX_Text(pDX, IDC_EDIT_SJDW, m_sjdw);
	DDX_Text(pDX, IDC_EDIT_SJKS, m_sjks);
	DDX_Text(pDX, IDC_EDIT_SJYS, m_sjys);
	DDX_Text(pDX, IDC_EDIT_BQ, m_bq);
	DDX_Text(pDX, IDC_EDIT_CH, m_ch);
	DDX_Text(pDX, IDC_EDIT_BBLX, m_bblx);
	DDX_Text(pDX, IDC_EDIT_BBQK, m_bbqk);
	DDX_Text(pDX, IDC_EDIT_BBMC, m_bbmc);
	DDX_Text(pDX, IDC_EDIT_BHGYY, m_bhgyy);
	DDX_Text(pDX, IDC_EDIT_BBQS, m_bbqs);
	DDX_Text(pDX, IDC_EDIT_FB, m_fb);
	DDX_Text(pDX, IDC_EDIT_SF, m_sf);
	DDX_Text(pDX, IDC_EDIT_QCYS, m_qcys);
	DDX_Text(pDX, IDC_EDIT_LCZD, m_lczd);
	DDX_Text(pDX, IDC_EDIT_BSZY, m_bszy);
	DDX_Text(pDX, IDC_EDIT_SSSJ, m_sssj);
	DDX_Text(pDX, IDC_EDIT_XM, m_xm);
	DDX_Text(pDX, IDC_EDIT_XB, m_xb);
	DDX_Text(pDX, IDC_EDIT_NL, m_nl);
	//}}AFX_DATA_MAP
}

void CMedPerson::OnInitialUpdate() 
{
	CPathologyView *pView;
	
	CXTResizeFormView::OnInitialUpdate();
	
	if (m_bIsFirstInit)
	{
		m_bIsFirstInit = false;

		DWORD m_dwXTBtnStyle;
		m_dwXTBtnStyle = BS_XT_SHOWFOCUS;
		m_dwXTBtnStyle &= ~(BS_XT_SEMIFLAT | BS_XT_FLAT | BS_XT_XPFLAT);

		m_Button_Td.SetXButtonStyle(m_dwXTBtnStyle);
		m_Button_Zlxx.SetXButtonStyle(m_dwXTBtnStyle);
		m_Button_Fkxx.SetXButtonStyle(m_dwXTBtnStyle);
		m_Button_Print.SetXButtonStyle(m_dwXTBtnStyle);
		m_Button_New.SetXButtonStyle(m_dwXTBtnStyle);
		m_Button_Bc.SetXButtonStyle(m_dwXTBtnStyle);
		m_Button_Jcxx.SetXButtonStyle(m_dwXTBtnStyle);
		m_Button_Sqddy.SetXButtonStyle(m_dwXTBtnStyle);

		const DWORD STYLE_ALL = BS_LEFT | BS_RIGHT | BS_CENTER;

		m_btHisidExtract.SetBitmap(CSize(16,16),IDB_BITMAP_HISIDIN);
		m_btHisidExtract.SetXButtonStyle(BS_XT_XPFLAT); 
		m_btHisidExtract.ModifyStyle(STYLE_ALL & ~BS_CENTER, BS_CENTER);
		
		m_btHisidExtract.EnableWindow(g_HISConnectClient.Connected());
		m_btHisid1Extract.EnableWindow(g_HISConnectClient1.Connected());


		m_btSfDetail.SetIcon(CSize(16,16),IDB_ICON_CHARGEDETAIL);
		m_btSfDetail.SetXButtonStyle(BS_XT_XPFLAT); 
		m_btSfDetail.ModifyStyle(STYLE_ALL & ~BS_CENTER, BS_CENTER);
		
		m_btBbmcDetail.SetIcon(CSize(16,16),IDB_ICON_CHARGEDETAIL);
		m_btBbmcDetail.SetXButtonStyle(BS_XT_XPFLAT); 
		m_btBbmcDetail.ModifyStyle(STYLE_ALL & ~BS_CENTER, BS_CENTER);

		m_btMaxBdh.SetBitmap(CSize(16,16),IDB_BITMAP_HISIDIN);
		m_btMaxBdh.SetXButtonStyle(BS_XT_XPFLAT); 
		m_btMaxBdh.ModifyStyle(STYLE_ALL & ~BS_CENTER, BS_CENTER);

		CRect rt;
		

		GetDlgItem(IDC_EDIT_CSRQ)->GetWindowRect(&rt);
		ScreenToClient(&rt);
		rt.left  = rt.right + 1;
		rt.right = rt.left + 20;

		if (!m_Time_Csrq.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT,
			rt, this, 109))
		{
			return;
		}

		
		GetDlgItem(IDC_EDIT_MCYJ)->GetWindowRect(&rt);
		ScreenToClient(&rt);
		rt.left  = rt.right + 1;
		rt.right = rt.left + 20;

		if (!m_Time_Mcyj.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT,
			rt, this, 110))
		{
			return;
		}

		GetDlgItem(IDC_DATE_SJRQ)->GetWindowRect(&rt);
		ScreenToClient(&rt);
		if (!m_Time_Sjrq.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_UPDOWN,
			rt, this, 111))
		{
			return;
		}

		m_Time_Sjrq.SetFormat(_T("yyyy-MM-dd HH':'mm"));
		m_Time_Sjrq.SetTime(theApp.GetOleServerDateTime());

		GetDlgItem(IDC_DATE_QCRQ)->GetWindowRect(&rt);
		ScreenToClient(&rt);
		if (!m_Time_Qcrq.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_UPDOWN,
			rt, this, 112))
		{
			return;
		}

		m_Time_Qcrq.SetFormat(_T("yyyy-MM-dd HH':'mm"));
		m_Time_Qcrq.SetTime(theApp.GetOleServerDateTime());

		theApp.ConnectDatabase();
		
		for(int i = 0; i < theApp.m_fnamelist.GetSize(); i++)
		{
			if (theApp.m_fnamelist.GetAt(i).CompareNoCase("orderType") == 0)
			{
				m_ctrl_orderType.m_info = theApp.m_finfolist.GetAt(i);
			}
			else if(theApp.m_fnamelist.GetAt(i).CompareNoCase("lczd") == 0)
			{
				m_ctrl_lczd.m_info = theApp.m_finfolist.GetAt(i);
			}
			else if(theApp.m_fnamelist.GetAt(i).CompareNoCase("qcys") == 0)
			{
				m_ctrl_qcys.m_info = theApp.m_finfolist.GetAt(i);
			}
			else if(theApp.m_fnamelist.GetAt(i).CompareNoCase("fb") == 0)
			{
				m_ctrl_fb.m_info = theApp.m_finfolist.GetAt(i);
			}
			else if(theApp.m_fnamelist.GetAt(i).CompareNoCase("bbqs") == 0)
			{
				m_ctrl_bbqs.m_info = theApp.m_finfolist.GetAt(i);
			}
			else if(theApp.m_fnamelist.GetAt(i).CompareNoCase("bhgyy") == 0)
			{
				m_ctrl_bhgyy.m_info = theApp.m_finfolist.GetAt(i);
			}
			else if(theApp.m_fnamelist.GetAt(i).CompareNoCase("bbmc") == 0)
			{
				m_ctrl_bbmc.m_info = theApp.m_finfolist.GetAt(i);
			}
			else if(theApp.m_fnamelist.GetAt(i).CompareNoCase("bbqk") == 0)
			{
				m_ctrl_bbqk.m_info = theApp.m_finfolist.GetAt(i);
			}
			else if(theApp.m_fnamelist.GetAt(i).CompareNoCase("bblx") == 0)
			{
				m_ctrl_bblx.m_info = theApp.m_finfolist.GetAt(i);
			}
			else if(theApp.m_fnamelist.GetAt(i).CompareNoCase("bq") == 0)
			{
				m_ctrl_bq.m_info = theApp.m_finfolist.GetAt(i);
			}
			else if(theApp.m_fnamelist.GetAt(i).CompareNoCase("sjys") == 0)
			{
				m_ctrl_sjys.m_info = theApp.m_finfolist.GetAt(i);
			}
			else if(theApp.m_fnamelist.GetAt(i).CompareNoCase("sjks") == 0)
			{
				m_ctrl_sjks.m_info = theApp.m_finfolist.GetAt(i);
			}
			else if(theApp.m_fnamelist.GetAt(i).CompareNoCase("sjdw") == 0)
			{
				m_ctrl_sjdw.m_info = theApp.m_finfolist.GetAt(i);
			}
			else if(theApp.m_fnamelist.GetAt(i).CompareNoCase("zy") == 0)
			{
				m_ctrl_zy.m_info = theApp.m_finfolist.GetAt(i);
			}
			else if(theApp.m_fnamelist.GetAt(i).CompareNoCase("hf") == 0)
			{
				m_ctrl_hf.m_info = theApp.m_finfolist.GetAt(i);
			}
			else if(theApp.m_fnamelist.GetAt(i).CompareNoCase("mz") == 0)
			{
				m_ctrl_mz.m_info = theApp.m_finfolist.GetAt(i);
			}
			else if(theApp.m_fnamelist.GetAt(i).CompareNoCase("nldw") == 0)
			{
				m_ctrl_nldw.m_info = theApp.m_finfolist.GetAt(i);
			}
			else if(theApp.m_fnamelist.GetAt(i).CompareNoCase("xb") == 0)
			{
				m_ctrl_xb.m_info = theApp.m_finfolist.GetAt(i);
			}
			else if(theApp.m_fnamelist.GetAt(i).CompareNoCase("jj") == 0)
			{
				m_ctrl_jj.m_info = theApp.m_finfolist.GetAt(i);
			}
		}
		
		m_ctrl_orderType.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_lczd.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_qcys.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_fb.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_bbqs.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_bhgyy.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_bbmc.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_bbqk.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_bblx.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_bq.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_sjys.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_sjks.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_sjdw.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_zy.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_hf.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_mz.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_nldw.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_xb.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
		m_ctrl_jj.SetBrowseStyle(BES_XT_POPUPMENU,0,0);

		//---------------------------------------------------
		m_ctrl_sjys.m_TableName.Format("sjys");
		m_ctrl_sjks.m_TableName.Format("sjks");
		//---------------------------------------------------

		((CButton *)GetDlgItem(IDC_BUTTON_JCXX))->ShowWindow(SW_HIDE);
		((CButton *)GetDlgItem(IDC_BUTTON_SQDDY))->ShowWindow(SW_HIDE);



		if(theApp.m_HisCfg.CompareNoCase("山东济宁医附院") == 0)
		{
			((CButton *)GetDlgItem(IDC_BUTTON_JCXX))->ShowWindow(SW_SHOW);
			((CButton *)GetDlgItem(IDC_BUTTON_SQDDY))->ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_STATIC_MZ,     "住院门诊:");
			SetDlgItemText(IDC_STATIC_SFZH,   "检查方法:");
		}

		if(theApp.m_HisCfg.CompareNoCase("北京肿瘤医院") == 0)
		{
			SetDlgItemText(IDC_STATIC_BDH,    "是否冰冻:");
			//SetDlgItemText(IDC_STATIC_QCYS,   "检查类别:");
		}





		if(!RIGHT_DLDJGZZ(theApp.m_Cpp_Right))
		{
			GetDlgItem(IDC_BUTTON_BC)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_NEW)->EnableWindow(FALSE);
		}

		return;
	}	

	//---------------------------------------------------
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh) || !RIGHT_DLDJGZZ(theApp.m_Cpp_Right))
	{
		GetDlgItem(IDC_BUTTON_BC)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_NEW)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_BC)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_NEW)->EnableWindow(TRUE);
	}
	//---------------------------------------------------
	int nStatusWord = 0;

	pView = theApp.GetView();
	ASSERT (pView != NULL);

	if(pView->GetDocument()->m_IsNewRecord)
	{
		m_sOrderId = _T("");
		m_sOrderType =_T("");
		m_sqh = _T("");
		m_brid = _T("");
		m_bdh = _T("");
		m_blh = GenNewBlh();
		m_ybh = _T("");
		m_mzh = _T("");
		m_zyh = _T("");
		m_xm = _T("");
		m_xb = theApp.m_Default_Sex;
		m_nl = _T("");
		m_nldw = _T("岁");
		m_csrq = _T("");
		m_mz = _T("");
		m_hf = _T("");
		m_zy = _T("");
		m_sfzh = _T("");
		m_lxdz = _T("");
		m_lxdh = _T("");
		m_mcyj = _T("");
		m_jj = _T("");
		m_sjdw = _T("");
		m_sjks = _T("");
		m_sjys = _T("");
		m_bq = _T("");
		m_ch = _T("");
		m_bblx = _T("");
		m_bbqk = _T("");
		m_bbmc = _T("");
		m_bhgyy = _T("");
		m_bbqs = _T("");
		m_fb = _T("");
		m_sf = 0.0f;
		m_qcys = _T("");
		m_lczd = _T("");
		m_bszy = _T("");
		m_sssj = _T("");

		//--------------------------------------------
		COleDateTime curt = theApp.GetOleServerDateTime();
		
		CString nSjdw;  nSjdw.Empty();
		SADateTime nSjrq, nQcrq;
		nSjrq = nQcrq = theApp.GetServerDateTime();
		try
		{
			SAString cmdstr = "Select TOP 1 sjrq, qcrq, sjdw from " + theApp.TABLE_MAIN + " where dbtype = :dbtype and dlrq IS NOT NULL order by dlrq desc";
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				cmdstr = "Select * from (Select sjrq, qcrq, sjdw from " + theApp.TABLE_MAIN + " where dbtype = :dbtype and dlrq IS NOT NULL order by dlrq desc) where ROWNUM<=1";
			}
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
			
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				if(!g_dbcommand.Field("sjrq").isNull())
				{
					nSjrq = g_dbcommand.Field("sjrq").asDateTime();
				}
				if(!g_dbcommand.Field("qcrq").isNull())
				{
					nQcrq = g_dbcommand.Field("qcrq").asDateTime();
				}

				if(!g_dbcommand.Field("sjdw").isNull())
				{
					nSjdw = g_dbcommand.Field("sjdw").asString();
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

		if(theApp.m_SjrqTimeMode.CompareNoCase("服务器当前时间") == 0)
		{
			m_Time_Sjrq.SetTime(curt);
		}
		else
		{
			COleDateTime ct;
			ct.SetDateTime(nSjrq.GetYear(), nSjrq.GetMonth(), nSjrq.GetDay(), nSjrq.GetHour(), nSjrq.GetMinute(), nSjrq.GetSecond());
			m_Time_Sjrq.SetTime(ct);
		}

		if(theApp.m_QcrqTimeMode.CompareNoCase("服务器当前时间") == 0)
		{
			m_Time_Qcrq.SetTime(curt);
		}
		else
		{
			COleDateTime ct;
			ct.SetDateTime(nQcrq.GetYear(), nQcrq.GetMonth(), nQcrq.GetDay(), nQcrq.GetHour(), nQcrq.GetMinute(), nQcrq.GetSecond());
			m_Time_Qcrq.SetTime(ct);
		}

		if(theApp.m_SjdwMode.CompareNoCase("是") == 0)
		{
			m_sjdw = nSjdw;
		}
		//--------------------------------------------

		((CEdit *)GetDlgItem(IDC_EDIT_BLH))->EnableWindow(TRUE);

		ResetButtonTextColor(true);
	}
	else
	{
		COleDateTime ServerTime = theApp.GetOleServerDateTime();
		try
		{
			CString fieldstr;
			fieldstr.Format("hisid,orderType,sqh,brid,bdh,ybh,mzh,zyh,xm,xb,nl,nldw, csrq, mz,hf,zy,sfzh,lxdz,lxdh, mcyj,jj,sjdw,sjks,sjys,bq,ch,bblx,bbqk,bbmc,bhgyy,bbqs,fb,sf,qcys,lczd,bszy,sssj,sjrq,qcrq,statusword");
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
				m_sOrderId = g_dbcommand.Field("hisid").asString();
				m_sOrderType = g_dbcommand.Field("OrderType").asString();
				m_sqh = g_dbcommand.Field("sqh").asString();
				m_brid = g_dbcommand.Field("brid").asString();
				m_bdh = g_dbcommand.Field("bdh").asString();
				m_blh = pView->GetDocument()->m_blh;
				m_ybh = g_dbcommand.Field("ybh").asString();
				m_mzh = g_dbcommand.Field("mzh").asString();
				m_zyh = g_dbcommand.Field("zyh").asString();
				m_xm = g_dbcommand.Field("xm").asString();
				m_xb = g_dbcommand.Field("xb").asString();
				m_nl = g_dbcommand.Field("nl").asString();
				m_nldw = g_dbcommand.Field("nldw").asString();
				m_csrq = g_dbcommand.Field("csrq").asString();
				m_mz = g_dbcommand.Field("mz").asString();
				m_hf = g_dbcommand.Field("hf").asString();
				m_zy = g_dbcommand.Field("zy").asString();
				m_sfzh = g_dbcommand.Field("sfzh").asString();
				m_lxdz = g_dbcommand.Field("lxdz").asString();
				m_lxdh = g_dbcommand.Field("lxdh").asString();
				m_mcyj = g_dbcommand.Field("mcyj").asString();
				m_jj = g_dbcommand.Field("jj").asString();
				m_sjdw = g_dbcommand.Field("sjdw").asString();
				m_sjks = g_dbcommand.Field("sjks").asString();
				m_sjys = g_dbcommand.Field("sjys").asString();
				m_bq = g_dbcommand.Field("bq").asString();
				m_ch = g_dbcommand.Field("ch").asString();
				m_bblx = g_dbcommand.Field("bblx").asString();
				m_bbqk = g_dbcommand.Field("bbqk").asString();
				m_bbmc = g_dbcommand.Field("bbmc").asString();
				m_bhgyy = g_dbcommand.Field("bhgyy").asString();
				m_bbqs = g_dbcommand.Field("bbqs").asString();
				m_fb = g_dbcommand.Field("fb").asString();
				m_sf = g_dbcommand.Field("sf").asDouble();
				m_qcys = g_dbcommand.Field("qcys").asString();
				m_lczd = g_dbcommand.Field("lczd").asString();
				m_bszy = g_dbcommand.Field("bszy").asString();
				m_sssj = g_dbcommand.Field("sssj").asString();

				COleDateTime curt = ServerTime;
				SADateTime st = g_dbcommand.Field("sjrq").asDateTime();
				curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
				m_Time_Sjrq.SetTime(curt);

				st = g_dbcommand.Field("qcrq").asDateTime();
				curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
				m_Time_Qcrq.SetTime(curt);

				try
				{
					if(!g_dbcommand.Field("statusword").isNull())
					{
						nStatusWord = g_dbcommand.Field("statusword").asLong();
					}
				}
				catch(SAException &)
				{
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
	
		((CEdit *)GetDlgItem(IDC_EDIT_BLH))->EnableWindow(FALSE);

		ResetButtonTextColor(false);
	}

	if(nStatusWord & 0x01)
	{
		((CEdit *)GetDlgItem(IDC_EDIT_ORDERTYPE))->EnableWindow(FALSE);
		((CEdit *)GetDlgItem(IDC_EDIT_SQH))->EnableWindow(FALSE);
		((CEdit *)GetDlgItem(IDC_EDIT_BRID))->EnableWindow(FALSE);
		((CEdit *)GetDlgItem(IDC_EDIT_XM))->EnableWindow(FALSE);
		((CEdit *)GetDlgItem(IDC_EDIT_XB))->EnableWindow(FALSE);
	}
	else
	{
		((CEdit *)GetDlgItem(IDC_EDIT_ORDERTYPE))->EnableWindow(TRUE);
		((CEdit *)GetDlgItem(IDC_EDIT_SQH))->EnableWindow(TRUE);
		((CEdit *)GetDlgItem(IDC_EDIT_BRID))->EnableWindow(TRUE);
		((CEdit *)GetDlgItem(IDC_EDIT_XM))->EnableWindow(TRUE);
		((CEdit *)GetDlgItem(IDC_EDIT_XB))->EnableWindow(TRUE);
	}

	if(theApp.m_MedPersonFocusMode.CompareNoCase("姓名") == 0)
	{
		((CEdit *)GetDlgItem(IDC_EDIT_XM))->SetFocus();
	}
	
	if(theApp.m_MedPersonFocusMode.CompareNoCase("患者来源") == 0)
	{
		((CEdit *)GetDlgItem(IDC_EDIT_ORDERTYPE))->SetFocus();
	}
	if(theApp.m_MedPersonFocusMode.CompareNoCase("病人ID") == 0)
	{
		((CEdit *)GetDlgItem(IDC_EDIT_BRID))->SetFocus();
	}
	if(theApp.m_MedPersonFocusMode.CompareNoCase("住院号") == 0)
	{
		((CEdit *)GetDlgItem(IDC_EDIT_ZYH))->SetFocus();
	}
	if(theApp.m_MedPersonFocusMode.CompareNoCase("申请号") == 0)
	{
		((CEdit *)GetDlgItem(IDC_EDIT_SQH))->SetFocus();
	}
	
	//------更新历史检查------
	RefreshHistoryInfo(m_blh, m_brid, m_ybh, m_xm, m_xb);

	UpdateData(FALSE);

	m_isModified = false;
}


CString CMedPerson::GenNewBlh()
{
	theApp.ConnectDatabase();

	CString nBlh;  nBlh.Empty();

	if(theApp.m_NewBlhGenMode.CompareNoCase("最新病理号加1") == 0)
	{
		try
		{
			SAString cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where dbtype = :dbtype and dlrq IS NOT NULL order by dlrq desc";
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				cmdstr = "Select * from (Select medicalno from " + theApp.TABLE_MAIN + " where dbtype = :dbtype and dlrq IS NOT NULL order by dlrq desc) where ROWNUM<=1";
			}
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
			
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				CString curno = g_dbcommand.Field("medicalno").asString();
				
				CString str,str1;
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
				nBlh = str;
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
	else if(theApp.m_NewBlhGenMode.CompareNoCase("最大病理号加1") == 0)
	{
		try
		{
			SAString cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where dbtype = :dbtype order by medicalno desc";
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				cmdstr = "Select * from (Select medicalno from " + theApp.TABLE_MAIN + " where dbtype = :dbtype order by medicalno desc) where ROWNUM<=1";
			}
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
			
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				CString curno = g_dbcommand.Field("medicalno").asString();
				
				CString str,str1;
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
				nBlh = str;
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
	else if(theApp.m_NewBlhGenMode.CompareNoCase("最新病理号去掉最后一位加1") == 0)
	{
		try
		{
			SAString cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where dbtype = :dbtype and dlrq IS NOT NULL order by dlrq desc";
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				cmdstr = "Select * from (Select medicalno from " + theApp.TABLE_MAIN + " where dbtype = :dbtype and dlrq IS NOT NULL order by dlrq desc) where ROWNUM<=1";
			}
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
			
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				CString curno = g_dbcommand.Field("medicalno").asString();
				CString lastChar = curno.Right(1);
				curno = curno.Left(curno.GetLength() - 1);
				CString str,str1;
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
				nBlh = str;
				nBlh += lastChar;
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

	return nBlh;
}

LRESULT CMedPerson::OnContentSave(WPARAM wParam, LPARAM lParam)
{
	CPathologyView *pView;
	BOOL bCommited = FALSE;
	CHISConnectData  resultObj;
	char studyUID[100], seriesUID[100];
	CString sXMLDoc("");
	
	if (!RIGHT_DLDJGZZ(theApp.m_Cpp_Right)) return 0;
	if (!m_isModified)  return 0;

	/////////////////////////////////////////////////////////////////
	theApp.ConnectDatabase();

	UpdateData(TRUE);

	

	int m_bgzt = 0, num = 0;
	
	try
	{
		SAString cmdstr = "Select TOP 1 bgzt from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		if (theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			cmdstr = "Select bgzt from " + theApp.TABLE_MAIN + " where medicalno = :medicalno and ROWNUM<=1";
		}
		g_dbcommand.setCommandText( cmdstr );
		if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())
		{
			m_blh.TrimLeft();  m_blh.TrimRight();
			g_dbcommand.Param("medicalno").setAsString() = m_blh;
		}
		else
		{
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		}
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			m_bgzt = g_dbcommand.Field("bgzt").asLong();
			num = 1;
		}
		else
		{
			//  在数据库中不存在指定活检编号的病例记录
			num = 0;
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

	if (theApp.GetView()->GetDocument()->m_blh.IsEmpty()==FALSE && (m_bgzt & 0x08))	return 0;

	if (((CEdit *)GetDlgItem(IDC_EDIT_BLH))->IsWindowEnabled())
	{
		// insert new generated record
		if(m_blh.IsEmpty())
		{
			AfxMessageBox("病理编号不能为空!");
			return 0;
		}

		if(num > 0)
		{
			//------------------------------------------------------
			CString newblh = GenNewBlh();
			try
			{
				SAString cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
				g_dbcommand.setCommandText( cmdstr );
				g_dbcommand.Param("medicalno").setAsString() = newblh;
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
			//------------------------------------------------------
			if(num > 0)
			{
				AfxMessageBox("数据库中已经存在指定病理编号<"+m_blh+">的登录记录");
				return 0;
			}
			else
			{
				AfxMessageBox("数据库中已经存在指定病理编号<"+m_blh+">的登录记录，当前病例的病理编号自动替换为<"+newblh+">");
				m_blh = newblh;
				SetDlgItemText(IDC_EDIT_BLH, m_blh);
			}
		}

		//---------------------------------------------------------
		if(theApp.m_AntiNullRecordMode.CompareNoCase("打开") == 0)
		{
			if(m_sOrderId.IsEmpty() && m_sqh.IsEmpty() && m_brid.IsEmpty() && 
			   m_bdh.IsEmpty() && m_mzh.IsEmpty() && m_zyh.IsEmpty() && m_xm.IsEmpty())
			{
				//  空病例写入防护
				return 0;
			}
		}
		//---------------------------------------------------------


		///////////////////////////////////////////////////////////
		//QZ
		//
		
		memset(studyUID, 0 ,100);
		memset(seriesUID, 0, 100);

		dcmGenerateUniqueIdentifier(studyUID, SITE_STUDY_UID_ROOT);
 		dcmGenerateUniqueIdentifier(seriesUID, SITE_SERIES_UID_ROOT);

		//////////////////////////////////////////////////////////

		SADateTime ServerTime = theApp.GetServerDateTime();
		try
		{
			SAString cmdstr = "Insert into " + theApp.TABLE_MAIN + "(medicalno,StudyUID, SeriesUID, dbtype,dlrq,bgzt) values(:medicalno, :StudyUID, :SeriesUID, :dbtype, :dlrq, 0)";

			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("medicalno").setAsString() = m_blh;
			g_dbcommand.Param("StudyUID").setAsString() = studyUID;
			g_dbcommand.Param("SeriesUID").setAsString() = seriesUID;
			g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
			g_dbcommand.Param("dlrq").setAsDateTime() = ServerTime;
			g_dbcommand.Execute();
			g_dbconnection.Commit();

			bCommited = TRUE;
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

		theApp.GetView()->GetDocument()->m_blh = m_blh;

		//-------------------------------------------------
		// 新建病历保存信息的同时系统将当前登录者写入"标本接收"字段
		m_bbqs = theApp.m_Cpp_User;
		SetDlgItemText(IDC_EDIT_BBQS, m_bbqs);
		//-------------------------------------------------

		/////////////////////////////////////////////////////////////
		//QZ 调用HISCONNECTCLIENT

		if (bCommited)
		{
			if (!m_sqh.IsEmpty())
			{			
				resultObj.Clear();
				resultObj.m_sStudyGUID = "PIS:";
				resultObj.m_sStudyGUID += m_blh;

				resultObj.m_sStudyID =  m_blh;
				resultObj.m_sStudyUID = studyUID;
				resultObj.m_sModality = seriesUID;
				
				resultObj.m_nAge	  = atoi(m_nl);

				if (strcmp(m_nldw,"岁") == 0)
					resultObj.m_sAgeUnit = "Y";
				else if (strcmp(m_nldw, "月") == 0)
					resultObj.m_sAgeUnit = "M";
				else if (strcmp(m_nldw, "天") == 0)
					resultObj.m_sAgeUnit = "D";
				else if (strcmp(m_nldw, "时") == 0)
					resultObj.m_sAgeUnit = "H";
				else 
					resultObj.m_sAgeUnit = "Y";
			
				resultObj.m_sInpatientNo = m_zyh;
				resultObj.m_sOutpatientNo = m_mzh;
    
				resultObj.m_sName = m_xm;
				resultObj.m_sSex = m_xb;
			
				if (m_sOrderType.IsEmpty())
				{
					if (!m_zyh.IsEmpty())
						resultObj.m_sOrderType = "ZY";
					else 
						resultObj.m_sOrderType = "MZ";
				}
				else
				{
					resultObj.m_sOrderType = m_sOrderType;
				}

				resultObj.m_sOrderID = m_sqh;
				resultObj.m_sOrderNo = m_sqh;
				resultObj.m_sOrderItems = m_bbmc;

				if (resultObj.m_sOrderNo.Mid(0,3) == "LX:") {
					if (g_HISConnectClient1.Connected()) {
						sXMLDoc = resultObj.EncodeToXMLDoc_LX1();			
						g_HISConnectClient1.UpdateMedicalOrder(sXMLDoc, "INSPECTED");
					}
				}
				else {
					if (g_HISConnectClient.Connected()) {
						sXMLDoc = resultObj.EncodeToXMLDoc();			
						g_HISConnectClient.UpdateMedicalOrder("4",sXMLDoc);
					}
				}
			}
		}

	}

	// save other field
	try
	{
		CString fieldstr;
		fieldstr.Empty();
		fieldstr += "orderType = :orderType,";
		fieldstr += "hisid=:hisid,";
		fieldstr += "sqh=:sqh,";
		fieldstr += "brid=:brid,";
		fieldstr += "bdh=:bdh,";
		fieldstr += "ybh=:ybh,";
		fieldstr += "mzh=:mzh,";
		fieldstr += "zyh=:zyh,";
		fieldstr += "xm=:xm,";
		fieldstr += "xb=:xb,";
		fieldstr += "nl=:nl,";
		fieldstr += "nldw=:nldw,";
		fieldstr += "csrq=:csrq,";
		fieldstr += "mz=:mz,";
		fieldstr += "hf=:hf,";
		fieldstr += "zy=:zy,";
		fieldstr += "sfzh=:sfzh,";
		fieldstr += "lxdz=:lxdz,";
		fieldstr += "lxdh=:lxdh,";
		fieldstr += "mcyj=:mcyj,";
		fieldstr += "jj=:jj,";
		fieldstr += "sjdw=:sjdw,";
		fieldstr += "sjks=:sjks,";
		fieldstr += "sjys=:sjys,";
		fieldstr += "sjrq=:sjrq,";
		fieldstr += "bq=:bq,";
		fieldstr += "ch=:ch,";
		fieldstr += "bblx=:bblx,";
		fieldstr += "bbqk=:bbqk,";
		fieldstr += "bbmc=:bbmc,";
		fieldstr += "bhgyy=:bhgyy,";
		fieldstr += "bbqs=:bbqs,";
		fieldstr += "fb=:fb,";
		fieldstr += "sf=:sf,";
		fieldstr += "qcys=:qcys,";
		fieldstr += "qcrq=:qcrq,";
		fieldstr += "lczd=:lczd,";
		fieldstr += "bszy=:bszy,";
		fieldstr += "sssj=:sssj";
		SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set " + fieldstr + " where medicalno = :medicalno";
		
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = m_blh;
		g_dbcommand.Param("orderType").setAsString() = m_sOrderType;
		g_dbcommand.Param("hisid").setAsString() = m_sOrderId;
		g_dbcommand.Param("sqh").setAsString() = m_sqh;
		g_dbcommand.Param("brid").setAsString() = m_brid;
		g_dbcommand.Param("bdh").setAsString() = m_bdh;
		g_dbcommand.Param("ybh").setAsString() = m_ybh;
		g_dbcommand.Param("mzh").setAsString() = m_mzh;
		g_dbcommand.Param("zyh").setAsString() = m_zyh;
		g_dbcommand.Param("xm").setAsString() = m_xm;
		g_dbcommand.Param("xb").setAsString() = m_xb;
		g_dbcommand.Param("nl").setAsString() = m_nl;
		g_dbcommand.Param("nldw").setAsString() = m_nldw;
		g_dbcommand.Param("csrq").setAsString() = m_csrq;
		g_dbcommand.Param("mz").setAsString() = m_mz;
		g_dbcommand.Param("hf").setAsString() = m_hf;
		g_dbcommand.Param("zy").setAsString() = m_zy;
		g_dbcommand.Param("sfzh").setAsString() = m_sfzh;
		g_dbcommand.Param("lxdz").setAsString() = m_lxdz;
		g_dbcommand.Param("lxdh").setAsString() = m_lxdh;
		g_dbcommand.Param("mcyj").setAsString() = m_mcyj;
		g_dbcommand.Param("jj").setAsString() = m_jj;
		g_dbcommand.Param("sjdw").setAsString() = m_sjdw;
		g_dbcommand.Param("sjks").setAsString() = m_sjks;
		g_dbcommand.Param("sjys").setAsString() = m_sjys;
        COleDateTime m_time;
		m_Time_Sjrq.GetTime(m_time);
		SADateTime sjrq_dtValue(m_time.GetYear(),m_time.GetMonth(),m_time.GetDay(),m_time.GetHour(),m_time.GetMinute(),m_time.GetSecond());
		g_dbcommand.Param("sjrq").setAsDateTime() = sjrq_dtValue;
		g_dbcommand.Param("bq").setAsString() = m_bq;
		g_dbcommand.Param("ch").setAsString() = m_ch;
		g_dbcommand.Param("bblx").setAsString() = m_bblx;
		g_dbcommand.Param("bbqk").setAsString() = m_bbqk;
		g_dbcommand.Param("bbmc").setAsString() = m_bbmc;
		g_dbcommand.Param("bhgyy").setAsString() = m_bhgyy;
		g_dbcommand.Param("bbqs").setAsString() = m_bbqs;
		g_dbcommand.Param("fb").setAsString() = m_fb;
		g_dbcommand.Param("sf").setAsDouble() = m_sf;
		g_dbcommand.Param("qcys").setAsString() = m_qcys;
		m_Time_Qcrq.GetTime(m_time);
		SADateTime qcrq_dtValue(m_time.GetYear(),m_time.GetMonth(),m_time.GetDay(),m_time.GetHour(),m_time.GetMinute(),m_time.GetSecond());
		g_dbcommand.Param("qcrq").setAsDateTime() = qcrq_dtValue;
		g_dbcommand.Param("lczd").setAsString() = m_lczd;
		g_dbcommand.Param("bszy").setAsString() = m_bszy;
		g_dbcommand.Param("sssj").setAsString() = m_sssj;
		
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

	((CEdit *)GetDlgItem(IDC_EDIT_BLH))->EnableWindow(FALSE);

	//------更新历史检查------
	RefreshHistoryInfo(m_blh, m_brid, m_ybh, m_xm, m_xb);

	if(theApp.m_AutoRefreshDBListMode.CompareNoCase("是") == 0 && ((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
	{
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.ResetGridSize();
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
	}

	CString AppTitle;
	AppTitle.LoadString(AFX_IDS_APP_TITLE);
	((CMainFrame *)AfxGetMainWnd())->SetWindowText(AppTitle + " - " + theApp.GetView()->GetDocument()->GenTitleString());

	m_isModified = false;

	return 1;
}


void CMedPerson::OnSfDetail() 
{
	// TODO: Add your control notification handler code here
	if(((CEdit *)GetDlgItem(IDC_EDIT_BLH))->IsWindowEnabled())
	{
		SetModifiedFlag(true);
		SendMessage(WM_CONTENTSAVE);
	}

	if(((CEdit *)GetDlgItem(IDC_EDIT_BLH))->IsWindowEnabled())  return;
	
	CChargeDetail mydlg(this);
	GetDlgItemText(IDC_EDIT_BLH, mydlg.m_blh);
	mydlg.DoModal();
}

BOOL CMedPerson::PreTranslateMessage(MSG* pMsg) 
{
	/*
	//?

	if (m_hWnd)
	{
		if(pMsg->message == WM_MOUSEMOVE)
		{
			CPoint pt;
			GetCursorPos(&pt);
			CRect rt;
			
			m_ctrl_orderType.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_orderType.did(TRUE);
			}
			else
			{
				m_ctrl_orderType.did(FALSE);
			}
			
			m_ctrl_lczd.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_lczd.did(TRUE);
			}
			else
			{
				m_ctrl_lczd.did(FALSE);
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
			m_ctrl_fb.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_fb.did(TRUE);
			}
			else
			{
				m_ctrl_fb.did(FALSE);
			}
			m_ctrl_bbqs.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_bbqs.did(TRUE);
			}
			else
			{
				m_ctrl_bbqs.did(FALSE);
			}
			m_ctrl_bhgyy.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_bhgyy.did(TRUE);
			}
			else
			{
				m_ctrl_bhgyy.did(FALSE);
			}
			m_ctrl_bbmc.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_bbmc.did(TRUE);
			}
			else
			{
				m_ctrl_bbmc.did(FALSE);
			}
			m_ctrl_bbqk.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_bbqk.did(TRUE);
			}
			else
			{
				m_ctrl_bbqk.did(FALSE);
			}
			m_ctrl_bblx.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_bblx.did(TRUE);
			}
			else
			{
				m_ctrl_bblx.did(FALSE);
			}
			m_ctrl_bq.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_bq.did(TRUE);
			}
			else
			{
				m_ctrl_bq.did(FALSE);
			}
			m_ctrl_sjys.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_sjys.did(TRUE);
			}
			else
			{
				m_ctrl_sjys.did(FALSE);
			}
			m_ctrl_sjks.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_sjks.did(TRUE);
			}
			else
			{
				m_ctrl_sjks.did(FALSE);
			}
			m_ctrl_sjdw.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_sjdw.did(TRUE);
			}
			else
			{
				m_ctrl_sjdw.did(FALSE);
			}
			m_ctrl_zy.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_zy.did(TRUE);
			}
			else
			{
				m_ctrl_zy.did(FALSE);
			}
			m_ctrl_hf.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_hf.did(TRUE);
			}
			else
			{
				m_ctrl_hf.did(FALSE);
			}
			m_ctrl_mz.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_mz.did(TRUE);
			}
			else
			{
				m_ctrl_mz.did(FALSE);
			}
			m_ctrl_nldw.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_nldw.did(TRUE);
			}
			else
			{
				m_ctrl_nldw.did(FALSE);
			}
			m_ctrl_xb.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_xb.did(TRUE);
			}
			else
			{
				m_ctrl_xb.did(FALSE);
			}
			m_ctrl_jj.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_jj.did(TRUE);
			}
			else
			{
				m_ctrl_jj.did(FALSE);
			}
		}
		//--------------------------------
		int WndID[39] = {
			IDC_EDIT_ORDERTYPE,
			IDC_EDIT_SQH,
			IDC_EDIT_BRID,
			IDC_EDIT_BDH,
			IDC_EDIT_BLH,
			IDC_EDIT_YBH,
			IDC_EDIT_MZH,
			IDC_EDIT_ZYH,
			IDC_EDIT_XM,
			IDC_EDIT_XB,
			IDC_EDIT_NL,
			IDC_EDIT_NLDW,
			IDC_EDIT_CSRQ,
			IDC_EDIT_HF,
			IDC_EDIT_ZY,
			IDC_EDIT_MZ,
			IDC_EDIT_SFZH,
			IDC_EDIT_LXDZ,
			IDC_EDIT_LXDH,
			IDC_EDIT_SJDW,
			IDC_EDIT_SJKS,
			IDC_EDIT_SJYS,
			IDC_DATE_SJRQ,
			IDC_EDIT_BQ,
			IDC_EDIT_CH,
			IDC_EDIT_BBLX,
			IDC_EDIT_BBQK,
			IDC_EDIT_BBMC,
			IDC_EDIT_BHGYY,
			IDC_EDIT_BBQS,
			IDC_EDIT_FB,
			IDC_EDIT_SF,
			IDC_EDIT_QCYS,
			IDC_DATE_QCRQ,
			IDC_EDIT_LCZD,
			IDC_EDIT_MCYJ,
			IDC_EDIT_JJ,
			IDC_EDIT_BSZY,
			IDC_EDIT_SSSJ
		};
		int WndCount = 39;
		
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
					if(!(pMsg->wParam == VK_RETURN && (i == 36 || i == 0 || i == 1  || i == 2 || i == 6 || i == 7)))
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
					else
					{
						if(i == 0 || i == 1 || i == 2  || i == 6 || i == 7)
						{
							OnHisidExtract();
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
	}
	
	*/
	//?

	return CXTResizeFormView::PreTranslateMessage(pMsg);
}

void CMedPerson::RefreshHistoryInfo(CString medicalno,CString id, CString ybh, CString xm, CString xb)
{
	int num = 0;
	try
	{
		SAString cmdstr= "Select count(*) from " + theApp.TABLE_MAIN + " where medicalno != :medicalno1 and (medicalno = :medicalno2 ";
		if(!id.IsEmpty())
		{
			cmdstr += " or brid = :brid";
		}


		if(theApp.m_LsjcMode.CompareNoCase("是") == 0)
		{
			if(!xm.IsEmpty())
			{
				if(!xb.IsEmpty())
				{
					cmdstr += " or (xm = :xm and xb = :xb)";
				}
				else
				{
					cmdstr += " or xm = :xm";
				}
			}
		}

		cmdstr += ")";

		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno1").setAsString() = medicalno;
		g_dbcommand.Param("medicalno2").setAsString() = ybh;
		if(!id.IsEmpty())
		{
			g_dbcommand.Param("brid").setAsString() = id;
		}

		if(theApp.m_LsjcMode.CompareNoCase("是") == 0)
		{
			if(!xm.IsEmpty())
			{
				g_dbcommand.Param("xm").setAsString() = xm;
				if(!xb.IsEmpty())
				{
					g_dbcommand.Param("xb").setAsString() = xb;
				}
			}
		}


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
	CString str;
	str.Format("历次检查: %d例", num);

	CWnd * pMainWnd = AfxGetMainWnd();
	if(pMainWnd && pMainWnd->IsKindOf(RUNTIME_CLASS(CMainFrame)) && ((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
	{
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_HypLink_Lsjc.SetWindowText(str);
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_HypLink_Lsjc.Invalidate();
	}
}

void CMedPerson::OnButtonBc() 
{
	SendMessage(WM_CONTENTSAVE); 
	if(theApp.m_HisCfg.CompareNoCase("通用接口") == 0)
	{
		theApp.NotifyRecordStatus(theApp.GetView()->GetDocument()->m_blh);
	}
}

void CMedPerson::OnButtonNew() 
{
	SendMessage(WM_CONTENTSAVE);

	if(theApp.m_HisCfg.CompareNoCase("通用接口") == 0)
	{
		theApp.NotifyRecordStatus(theApp.GetView()->GetDocument()->m_blh);
	}

	//QZ ((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_COMMAND,ID_FILE_NEW);
	((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_COMMAND,ID_DOC_NEWORDER);

}

void CMedPerson::OnButtonNew1() 
{
	SendMessage(WM_CONTENTSAVE);

	if(theApp.m_HisCfg.CompareNoCase("通用接口") == 0)
	{
		theApp.NotifyRecordStatus(theApp.GetView()->GetDocument()->m_blh);
	}

	//QZ ((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_COMMAND,ID_FILE_NEW);
	((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_COMMAND,ID_DOC_NEWORDER1);

}



void CMedPerson::OnButtonPrint() 
{
	OnButtonBc();

	CBdSel mydlg(this);
	if(mydlg.DoModal() == IDCANCEL)  return;

	//--------------------------------------------------
	CPathologyView *pView = theApp.GetView();
	ASSERT (pView != NULL);
	
	if(!pView->GetDocument()->m_blh.IsEmpty())
	{
		try
		{
			int status = 0;
			SAString cmdstr = "select statusword from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Execute();
			while(g_dbcommand.FetchNext())
			{
				status = g_dbcommand.Field("statusword").asLong();
			}
			g_dbconnection.Commit();

			status = status | 0x01;
			cmdstr = "Update " + theApp.TABLE_MAIN + " set statusword =:statusword where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = pView->GetDocument()->m_blh;
			g_dbcommand.Param("statusword").setAsLong() = status;
			g_dbcommand.Execute();
			g_dbconnection.Commit();

			((CEdit *)GetDlgItem(IDC_EDIT_ORDERTYPE))->EnableWindow(FALSE);
			((CEdit *)GetDlgItem(IDC_EDIT_SQH))->EnableWindow(FALSE);
			((CEdit *)GetDlgItem(IDC_EDIT_BRID))->EnableWindow(FALSE);
			((CEdit *)GetDlgItem(IDC_EDIT_XM))->EnableWindow(FALSE);
			((CEdit *)GetDlgItem(IDC_EDIT_XB))->EnableWindow(FALSE);
		}
		catch(SAException & e)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException& e)
			{
			}
			AfxMessageBox((const char*)e.ErrText());
		}
	}
	//--------------------------------------------------

	mydlg.m_TableName.TrimLeft();  mydlg.m_TableName.TrimRight();
	theApp.m_BdName = mydlg.m_TableName;
	if(theApp.m_BdName.IsEmpty())
	{
		theApp.m_BdName.Format("回执单");
	}

	theApp.m_PrintType = 2;
	theApp.GetView()->SendMessage(WM_COMMAND, ID_FILE_PRINT);
}

void CMedPerson::ReadMedicalOrder(bool bFromLX)
{
	CString sXMLDoc;
	CHISConnectData orderObj;
	CString sAge, sFee, sContact;
	CWnd *pWnd, *pWndOrderNo, *pWndOrderType;
	CString sBLH;
	CString sOrderNo(""), sOrderType("");

	if (bFromLX)
		if (!g_HISConnectClient1.Connected()) return;
	else
		if (!g_HISConnectClient.Connected()) return;

	pWnd = GetDlgItem(IDC_EDIT_BLH);
	if (pWnd == NULL) return;
	pWnd->GetWindowText(sBLH);
	if (sBLH.IsEmpty()) return;

	pWndOrderNo = GetDlgItem(IDC_EDIT_SQH);
	pWndOrderType = GetDlgItem(IDC_EDIT_ORDERTYPE);

	if (pWndOrderNo != NULL)
	{
		/*
		pWndOrderNo->GetWindowText(sOrderNo);
		
		if (pWndOrderType != NULL) pWndOrderType->GetWindowText(sOrderType);

		if (!sOrderNo.IsEmpty())
		{
			g_HISConnectClient.ShowMedicalOrder(sOrderNo, sOrderType);

			return;
		}
		*/
	}

	if (bFromLX) 
		sXMLDoc = g_HISConnectClient1.ReadMedicalOrder();
	else
		sXMLDoc = g_HISConnectClient.ReadMedicalOrder();

	if (sXMLDoc.IsEmpty()) return;

	
	if (bFromLX) 
		orderObj.DecodeFromXMLDoc_LX(sXMLDoc);
	else
		orderObj.DecodeFromXMLDoc(sXMLDoc);

	m_sOrderId = orderObj.m_sOrderID;
	
	SetDlgItemText(IDC_EDIT_ORDERTYPE, orderObj.m_sOrderType);
	SetDlgItemText(IDC_EDIT_SQH,  orderObj.m_sOrderID);
	SetDlgItemText(IDC_EDIT_BRID, orderObj.m_sPatientID);

	SetDlgItemText(IDC_EDIT_XM,   orderObj.m_sName);
	SetDlgItemText(IDC_EDIT_XB,   orderObj.m_sSex);

	if (orderObj.m_nAge > 0)
	{
		sAge.Format("%d", orderObj.m_nAge);
		SetDlgItemText(IDC_EDIT_NL, sAge);
	}

	if (stricmp(orderObj.m_sAgeUnit, "Y") == 0)
		SetDlgItemText(IDC_EDIT_NLDW, "岁");
	else if (stricmp(orderObj.m_sAgeUnit, "M") == 0)
		SetDlgItemText(IDC_EDIT_NLDW, "月");
	else if (stricmp(orderObj.m_sAgeUnit, "D") == 0)
		SetDlgItemText(IDC_EDIT_NLDW, "天");
	else if (stricmp(orderObj.m_sAgeUnit, "H") == 0)
		SetDlgItemText(IDC_EDIT_NLDW, "时");

	sContact = orderObj.m_sAddress;
	
	if (!orderObj.m_sTelNo.IsEmpty())
	{
		if (!sContact.IsEmpty()) sContact += " ";
		sContact += orderObj.m_sTelNo;
	}

	SetDlgItemText(IDC_EDIT_CSRQ, orderObj.m_sDOB);
	SetDlgItemText(IDC_EDIT_LXDZ, orderObj.m_sAddress);
	SetDlgItemText(IDC_EDIT_LXDH, orderObj.m_sTelNo);
	SetDlgItemText(IDC_EDIT_CH,   orderObj.m_sBedNo);
	SetDlgItemText(IDC_EDIT_ZYH,  orderObj.m_sInpatientNo);
	SetDlgItemText(IDC_EDIT_MZH,  orderObj.m_sOutpatientNo);
	SetDlgItemText(IDC_EDIT_BQ,   orderObj.m_sZoneName);
			
	SetDlgItemText(IDC_EDIT_LCZD, orderObj.m_sClinicDiag);
			
	SetDlgItemText(IDC_EDIT_SJKS, orderObj.m_sRefDept);
	SetDlgItemText(IDC_EDIT_SJYS, orderObj.m_sRefDoctor);

	SetDlgItemText(IDC_EDIT_BBMC, orderObj.m_sSampleName);
	SetDlgItemText(IDC_EDIT_BSZY, orderObj.m_sHistory);

	if (orderObj.m_fFee > 0.0)
	{
		sFee.Format("%7.2f", orderObj.m_fFee);
		SetDlgItemText(IDC_EDIT_SF, sFee);
	}
	
	OnContentModified();
}

void CMedPerson::OnHisidExtract() 
{
	ReadMedicalOrder(false);
}

void CMedPerson::OnHisid1Extract() 
{
	ReadMedicalOrder(true);
}

void CMedPerson::OnContentModified() 
{
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh)) return;

	m_isModified = true;
}

void CMedPerson::OnDateTimeChange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	COleDateTime dtTime;
	CString sTime;
	CWnd *pWnd;

	if (pNMHDR->idFrom == 109)
	{
		m_Time_Csrq.GetTime(dtTime);
		sTime.Format("%04d-%02d-%02d", dtTime.GetYear(), dtTime.GetMonth(), dtTime.GetDay());
		pWnd = GetDlgItem(IDC_EDIT_CSRQ);
		if (pWnd)
			pWnd->SetWindowText(sTime);
	}
	else if (pNMHDR->idFrom == 110)
	{
		m_Time_Mcyj.GetTime(dtTime);
		sTime.Format("%04d-%02d-%02d", dtTime.GetYear(), dtTime.GetMonth(), dtTime.GetDay());
		pWnd = GetDlgItem(IDC_EDIT_MCYJ);
		if (pWnd)
			pWnd->SetWindowText(sTime);
	}

	OnContentModified();
	
	*pResult = 0;
}

void CMedPerson::OnMaxBdh() 
{
	CString bdh;
	GetDlgItemText(IDC_EDIT_BDH,bdh);
	if(!bdh.IsEmpty())
	{
		return;
	}

	theApp.ConnectDatabase();

	CString nBdh;  nBdh.Empty();

	try
	{
		SAString cmdstr = "Select TOP 1 bdh from " + theApp.TABLE_MAIN + " where bdh IS NOT NULL and bdh != '' and dbtype = :dbtype and dlrq IS NOT NULL order by dlrq desc";
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			cmdstr = "Select * from (Select bdh from " + theApp.TABLE_MAIN + " where bdh IS NOT NULL and bdh != '' and dbtype = :dbtype and dlrq IS NOT NULL order by dlrq desc) where ROWNUM<=1";
		}
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
		
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			CString curno = g_dbcommand.Field("bdh").asString();
			
			CString str,str1;
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
			nBdh = str;
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

	SetDlgItemText(IDC_EDIT_BDH, nBdh);
}

void CMedPerson::OnButtonZlxx() 
{
	if(((CEdit *)GetDlgItem(IDC_EDIT_BLH))->IsWindowEnabled())  return;

	CTextOut mydlg(this);
	theApp.GetDatabaseValue(theApp.GetView()->GetDocument()->m_blh,"zlxx",mydlg.m_Text,true);
	mydlg.m_Title.Format("肿瘤信息");
	mydlg.DoModal();
}

void CMedPerson::OnButtonFkxx() 
{
	if(((CEdit *)GetDlgItem(IDC_EDIT_BLH))->IsWindowEnabled())  return;

	CTextOut mydlg(this);
	theApp.GetDatabaseValue(theApp.GetView()->GetDocument()->m_blh,"fkxx",mydlg.m_Text,true);
	mydlg.m_Title.Format("妇科信息");
	mydlg.DoModal();
}

#include "INI.h"

void CMedPerson::OnButtonJcxx() 
{

	if(theApp.m_HisCfg.CompareNoCase("山东济宁医附院") != 0)  return;
	CString nIp, nDATABASENAME,nDATABASE_USERNAME,nDATABASE_PASSWORD;


	CString his_Patientid;  his_Patientid.IsEmpty();
	
	CStdioFile File;
	if ( File.Open(theApp.m_Exe_Working_Dir + "system\\pacsconnect.cfg", CFile::modeRead | CFile::typeText) )
	{
		CString tmp;
		int index = 0;
		while(1)
		{
			if( File.ReadString(tmp) == FALSE ) break;
			tmp.TrimLeft();	tmp.TrimRight();
			if( tmp.IsEmpty() )   continue;
			switch( index ) {
			case 0:
				nIp = tmp;
				break;
			case 1:
				nDATABASENAME = tmp;
				break;
			case 2:
				nDATABASE_USERNAME = tmp;
				break;
			case 3:
				nDATABASE_PASSWORD = tmp;
				break;
			}
			index++;
		}
		File.Close();
		
		CString his_sqh;
		GetDlgItemText(IDC_EDIT_SQH,his_sqh);
		his_sqh.TrimLeft();          his_sqh.TrimRight();

		SAConnection pacscon;
		SACommand    pacscmd;
		try
		{
			pacscon.Connect( (SAString)nDATABASENAME, (SAString)nDATABASE_USERNAME, (SAString)nDATABASE_PASSWORD, SA_Oracle_Client );
			pacscmd.setConnection( &pacscon );

			pacscmd.setCommandText("Select Patientid from PathologyApp where flowId = :flowId");
			pacscmd.Param("flowId").setAsString() = his_sqh;
			pacscmd.Execute();
			
			if( pacscmd.FetchNext() )
			{
				his_Patientid = pacscmd.Field("Patientid").asString();
			}
			
			pacscon.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				pacscon.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
		}

		if(pacscon.isConnected())    pacscon.Disconnect();
	}	

	CIniReader m_IniReader;
	m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");
	CString m_strURL;
	
	CString his_zymzbs;
	GetDlgItemText(IDC_EDIT_MZ,   his_zymzbs);
	his_zymzbs.TrimLeft();  his_zymzbs.TrimRight();
	if(his_zymzbs.CompareNoCase("住院") == 0)
	{
		m_strURL = m_IniReader.getKeyValue("电子病历的临床浏览器","Settings");
		m_strURL.Replace("(patientid)", his_Patientid);
	}
	if(his_zymzbs.CompareNoCase("门诊") == 0)
	{
		m_strURL = m_IniReader.getKeyValue("电子病历的临床浏览器1","Settings");
		m_strURL.Replace("(patientid)", his_Patientid);
	}
	if(!m_strURL.IsEmpty())
	{
		::ShellExecute(m_hWnd, _T("open"), m_strURL, NULL, NULL, SW_SHOW);
	}
	
}

void CMedPerson::ResetButtonTextColor(bool IsInit)
{
	CPathologyView *pView = theApp.GetView();
	ASSERT (pView != NULL);

	if(IsInit || pView->GetDocument()->m_blh.IsEmpty())
	{
		m_Button_Zlxx.SetColorText(RGB(0,0,0));
		m_Button_Fkxx.SetColorText(RGB(0,0,0));

		m_Button_Zlxx.RedrawWindow();
		m_Button_Fkxx.RedrawWindow();
		return;
	}

	CString m_zlxx, m_fkxx;
	try
	{
		SAString cmdstr = "Select TOP 1 zlxx, fkxx from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			cmdstr = "Select zlxx, fkxx from " + theApp.TABLE_MAIN + " where medicalno = :medicalno and ROWNUM<=1";
		}
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = pView->GetDocument()->m_blh;
		
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
		{
			m_zlxx = g_dbcommand.Field("zlxx").asString();
			m_fkxx = g_dbcommand.Field("fkxx").asString();
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

	m_zlxx.TrimLeft();  m_zlxx.TrimRight();
    m_fkxx.TrimLeft();  m_fkxx.TrimRight();

	if(m_zlxx.IsEmpty())
	{
		m_Button_Zlxx.SetColorText(RGB(0,0,0));
	}
	else
	{
		m_Button_Zlxx.SetColorText(RGB(0,0,255));
	}

	if(m_fkxx.IsEmpty())
	{
		m_Button_Fkxx.SetColorText(RGB(0,0,0));
	}
	else
	{
		m_Button_Fkxx.SetColorText(RGB(0,0,255));
	}

	m_Button_Zlxx.RedrawWindow();
	m_Button_Fkxx.RedrawWindow();
}


void CMedPerson::OnBbmcDetail() 
{
	if(theApp.m_HisCfg.CompareNoCase("北京肿瘤医院") != 0)  return;

	if(((CEdit *)GetDlgItem(IDC_EDIT_BLH))->IsWindowEnabled())
	{
		SetModifiedFlag(true);
		SendMessage(WM_CONTENTSAVE);
	}

	if(((CEdit *)GetDlgItem(IDC_EDIT_BLH))->IsWindowEnabled())  return;
	
	CBbmxDetail mydlg(this);
	GetDlgItemText(IDC_EDIT_SQH, mydlg.m_Sqh);
	if(mydlg.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_EDIT_BBMC, mydlg.m_Bbmc);
		OnContentModified();
	}
}


void CMedPerson::OnButtonSqddy() 
{
	if(theApp.m_HisCfg.CompareNoCase("山东济宁医附院") != 0)  return;
	CString his_sqh;
	GetDlgItemText(IDC_EDIT_SQH,his_sqh);
	his_sqh.TrimLeft();          his_sqh.TrimRight();
	
	CIniReader m_IniReader;
	m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");
	CString m_strURL = m_IniReader.getKeyValue("电子病历的临床浏览器2","Settings");
	m_strURL.Replace("(flowid)", his_sqh);
	
	if(!m_strURL.IsEmpty())
	{
		::ShellExecute(m_hWnd, _T("open"), m_strURL, NULL, NULL, SW_SHOW);
	}
}

void CMedPerson::OnButtonTd() 
{
	
	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())	return;

	theApp.hisStatus = 1;

	if(theApp.m_HisCfg.CompareNoCase("通用接口") == 0)
	{
		theApp.NotifyRecordStatus(theApp.GetView()->GetDocument()->m_blh);
	}	

	CString m_medicalno;
	int     m_bgzt, m_dbtype;
	SADateTime m_dlrq, m_sjrq, m_qcrq;
	try
	{
		SAString cmdstr = "Select TOP 1 * from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			cmdstr = "Select * from " + theApp.TABLE_MAIN + " where medicalno = :medicalno and ROWNUM<=1";
		}
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			m_medicalno = theApp.GetView()->GetDocument()->m_blh;
			m_bgzt   = g_dbcommand.Field("bgzt").asLong();
			m_dbtype = g_dbcommand.Field("dbtype").asLong();
			m_dlrq   = g_dbcommand.Field("dlrq").asDateTime();
			m_sjrq   = g_dbcommand.Field("sjrq").asDateTime();
			m_qcrq   = g_dbcommand.Field("qcrq").asDateTime();
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

	CString ztinfo; ztinfo.Empty();
	if(m_bgzt >= 1  && m_bgzt < 2)   ztinfo.Format("已取材");
	if(m_bgzt >= 2  && m_bgzt < 4)   ztinfo.Format("已延期");
	if(m_bgzt >= 4  && m_bgzt < 8)   ztinfo.Format("已诊断");
	if(m_bgzt >= 8  && m_bgzt < 16)  ztinfo.Format("已审核");
	if(m_bgzt >= 16 && m_bgzt < 32)  ztinfo.Format("已打印");
	if(m_bgzt >= 32)                 ztinfo.Format("已发送");

	if(!ztinfo.IsEmpty())
	{
		AfxMessageBox("当前记录" + ztinfo + "，不能退单！");
		return;
	}

	if(AfxMessageBox("当前记录的信息将全部清空，是否继续？",MB_OKCANCEL|MB_ICONQUESTION) == IDCANCEL)  return;

	((CMainFrame *)AfxGetMainWnd())->SendMessage(WM_COMMAND,ID_FILE_NEW);

	try
	{
		SAString cmdstr = "Delete from " + theApp.TABLE_MAIN + "  where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = m_medicalno;
		
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
	
	try
	{
		SAString cmdstr = "Insert into " + theApp.TABLE_MAIN + "(medicalno,dbtype,bgzt,dlrq,sjrq,qcrq) values(:medicalno,:dbtype,:bgzt,:dlrq,:sjrq,:qcrq)";
		
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = m_medicalno;
		g_dbcommand.Param("dbtype").setAsLong()   = m_dbtype;
		g_dbcommand.Param("bgzt").setAsLong()     = m_bgzt;
		g_dbcommand.Param("dlrq").setAsDateTime() = m_dlrq;
		g_dbcommand.Param("sjrq").setAsDateTime() = m_sjrq;
		g_dbcommand.Param("qcrq").setAsDateTime() = m_qcrq;
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
		
	theApp.OpenDocumentFile(m_medicalno);
}
