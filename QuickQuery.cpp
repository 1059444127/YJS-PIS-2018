// QuickQuery.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "QuickQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQuickQuery dialog
extern CPathologyApp theApp;

CQuickQuery::CQuickQuery(CWnd* pParent /*=NULL*/)
	: CDialog(CQuickQuery::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQuickQuery)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_query.Empty();
	pListInput = NULL;
}


void CQuickQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQuickQuery)
	DDX_Control(pDX, IDC_HYP_NOTE, m_HypLink_Note);
	DDX_Control(pDX, IDC_EDIT_SHYS, m_ctrl_shys);
	DDX_Control(pDX, IDC_EDIT_ZZYS, m_ctrl_zzys);
	DDX_Control(pDX, IDC_EDIT_CZYS, m_ctrl_czys);
	DDX_Control(pDX, IDC_EDIT_BBMC, m_ctrl_bbmc);
	DDX_Control(pDX, IDC_EDIT_BBLX, m_ctrl_bblx);
	DDX_Control(pDX, IDC_EDIT_SJKS, m_ctrl_sjks);
	DDX_Control(pDX, IDC_EDIT_SJDW, m_ctrl_sjdw);
//	DDX_Control(pDX, IDC_EDIT_AGE1, m_ctrl_age1);
//	DDX_Control(pDX, IDC_EDIT_AGE2, m_ctrl_age2);
	DDX_Control(pDX, IDC_EDIT_XB,	m_ctrl_xb);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQuickQuery, CDialog)
	//{{AFX_MSG_MAP(CQuickQuery)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_MICD10, OnButtonMicd10)
	ON_BN_CLICKED(IDC_BUTTON_SICD10, OnButtonSicd10)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_LISTSELECT,ListSelect)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuickQuery message handlers

BOOL CQuickQuery::OnInitDialog() 
{
	CDialog::OnInitDialog();

    pListInput = new CDlgListInput(this);
	pListInput->m_type = 1;  // 1 - MICD10, 2 - SICD10
	pListInput->Create();
	pListInput->SetActiveWindow();
	pListInput->ShowWindow(SW_HIDE);

	m_HypLink_Note.SetURL("说明:临床诊断、病理诊断、诊断关键词中输入多个关键词时用\"|\"隔开");
	m_HypLink_Note.SetColors(RGB(0x00, 0x00, 0xFF), RGB(0x00, 0x00, 0xFF), RGB(0x00, 0x00, 0xFF));
	m_HypLink_Note.SetUnderline(true);
	m_HypLink_Note.EnableShellExecute(false);
	
	CRect rt;
	GetDlgItem(IDC_DATE_SJRQ_FROM)->GetWindowRect(&rt);
	ScreenToClient(&rt);
	if (!m_Time_Sjrq_From.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT, //,DTS_LONGDATEFORMAT
		rt, this, 112))
	{
		return FALSE;
	}
	
	m_Time_Sjrq_From.SetTime(theApp.GetOleServerDateTime());

	GetDlgItem(IDC_DATE_SJRQ_TO)->GetWindowRect(&rt);
	ScreenToClient(&rt);
	if (!m_Time_Sjrq_To.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT, //,DTS_LONGDATEFORMAT
		rt, this, 113))
	{
		return FALSE;
	}
	
	m_Time_Sjrq_To.SetTime(theApp.GetOleServerDateTime());

	GetDlgItem(IDC_DATE_BGRQ_FROM)->GetWindowRect(&rt);
	ScreenToClient(&rt);
	if (!m_Time_Bgrq_From.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT, //,DTS_LONGDATEFORMAT
		rt, this, 114))
	{
		return FALSE;
	}
	
	m_Time_Bgrq_From.SetTime(theApp.GetOleServerDateTime());

	GetDlgItem(IDC_DATE_BGRQ_TO)->GetWindowRect(&rt);
	ScreenToClient(&rt);
	if (!m_Time_Bgrq_To.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT, //,DTS_LONGDATEFORMAT
		rt, this, 115))
	{
		return FALSE;
	}
	
	m_Time_Bgrq_To.SetTime(theApp.GetOleServerDateTime());

	theApp.ConnectDatabase();
	
	for(int i = 0; i < theApp.m_fnamelist.GetSize(); i++)
	{
		if(theApp.m_fnamelist.GetAt(i).CompareNoCase("sjdw") == 0)
		{
			m_ctrl_sjdw.m_info = theApp.m_finfolist.GetAt(i);
		}
		if(theApp.m_fnamelist.GetAt(i).CompareNoCase("sjks") == 0)
		{
			m_ctrl_sjks.m_info = theApp.m_finfolist.GetAt(i);
		}
		if(theApp.m_fnamelist.GetAt(i).CompareNoCase("bblx") == 0)
		{
			m_ctrl_bblx.m_info = theApp.m_finfolist.GetAt(i);
		}
		if(theApp.m_fnamelist.GetAt(i).CompareNoCase("bbmc") == 0)
		{
			m_ctrl_bbmc.m_info = theApp.m_finfolist.GetAt(i);
		}
		if(theApp.m_fnamelist.GetAt(i).CompareNoCase("czys") == 0)
		{
			m_ctrl_czys.m_info = theApp.m_finfolist.GetAt(i);
		}
		if(theApp.m_fnamelist.GetAt(i).CompareNoCase("zzys") == 0)
		{
			m_ctrl_zzys.m_info = theApp.m_finfolist.GetAt(i);
		}
		if(theApp.m_fnamelist.GetAt(i).CompareNoCase("shys") == 0)
		{
			m_ctrl_shys.m_info = theApp.m_finfolist.GetAt(i);
		}
		if(theApp.m_fnamelist.GetAt(i).CompareNoCase("xb") == 0)
		{
			m_ctrl_xb.m_info = theApp.m_finfolist.GetAt(i);
		}
	}
	
	m_ctrl_sjdw.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
	m_ctrl_sjks.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
	m_ctrl_bblx.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
	m_ctrl_bbmc.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
	m_ctrl_czys.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
	m_ctrl_zzys.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
	m_ctrl_shys.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
	m_ctrl_xb.SetBrowseStyle(BES_XT_POPUPMENU,0,0);

	((CComboBox *)GetDlgItem(IDC_COMBO_BGZT))->AddString("已登记");
	((CComboBox *)GetDlgItem(IDC_COMBO_BGZT))->AddString("已取材");
	((CComboBox *)GetDlgItem(IDC_COMBO_BGZT))->AddString("已延期");
	((CComboBox *)GetDlgItem(IDC_COMBO_BGZT))->AddString("已诊断");
	((CComboBox *)GetDlgItem(IDC_COMBO_BGZT))->AddString("已审核");
	((CComboBox *)GetDlgItem(IDC_COMBO_BGZT))->AddString("已打印");
	((CComboBox *)GetDlgItem(IDC_COMBO_BGZT))->AddString("已发送");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CQuickQuery::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_hWnd)
	{
		if(pMsg->message == WM_MOUSEMOVE)
		{
			CPoint pt;
			GetCursorPos(&pt);
			CRect rt;

			m_ctrl_sjdw.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_sjdw.did(TRUE);
			}
			else
			{
				m_ctrl_sjdw.did(FALSE);
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

			m_ctrl_bblx.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_bblx.did(TRUE);
			}
			else
			{
				m_ctrl_bblx.did(FALSE);
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
			
			m_ctrl_czys.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_czys.did(TRUE);
			}
			else
			{
				m_ctrl_czys.did(FALSE);
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

			m_ctrl_shys.GetWindowRect(&rt);
			if(rt.PtInRect(pt))
			{
				m_ctrl_shys.did(TRUE);
			}
			else
			{
				m_ctrl_shys.did(FALSE);
			}
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CQuickQuery::OnOK() 
{
	m_query.Empty();

	bool IsBlurMode = false;
	if(((CButton *)GetDlgItem(IDC_CHECK_BLUR))->GetState() & 0x0003)
	{
		IsBlurMode = true;
	}

	CString str, tmp;
	
	if(((CButton *)GetDlgItem(IDC_CHECK_SJRQ))->GetState() & 0x0003)
	{
		COleDateTime m_time;

		m_Time_Sjrq_From.GetTime(m_time);
		CString start = m_time.Format("%Y-%m-%d 00:00:00");

		m_Time_Sjrq_To.GetTime(m_time);
		CString end   = m_time.Format("%Y-%m-%d 23:59:59");

		str.Format("sjrq >= '%s' and sjrq <= '%s'", start, end);
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			str.Format("sjrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and sjrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", start, end);
		}
		if(!m_query.IsEmpty())  m_query += " and ";
		m_query += str;
	}

	if(((CButton *)GetDlgItem(IDC_CHECK_BGRQ))->GetState() & 0x0003)
	{
		COleDateTime m_time;

		m_Time_Bgrq_From.GetTime(m_time);
		CString start = m_time.Format("%Y-%m-%d 00:00:00");

		m_Time_Bgrq_To.GetTime(m_time);
		CString end   = m_time.Format("%Y-%m-%d 23:59:59");

		str.Format("bgrq >= '%s' and bgrq <= '%s'", start, end);
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			str.Format("bgrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and bgrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", start, end);
		}
		if(!m_query.IsEmpty())  m_query += " and ";
		m_query += str;
	}

	GetDlgItemText(IDC_EDIT_XB,tmp);
	tmp.TrimLeft();tmp.TrimRight();
	if(!tmp.IsEmpty()){
		str.Format("xb = '%s'",tmp);
		if(!m_query.IsEmpty()) m_query += " and ";
		m_query += str;
	}

	GetDlgItemText(IDC_EDIT_AGE1,tmp);
	tmp.TrimLeft();tmp.TrimRight();
	if(!tmp.IsEmpty()){
		str.Format("nl >= '%s'",tmp);
		if(!m_query.IsEmpty()) m_query += " and ";
		m_query += str;
	}

	GetDlgItemText(IDC_EDIT_AGE2,tmp);
	tmp.TrimLeft();tmp.TrimRight();
	if(!tmp.IsEmpty()){
		str.Format("nl <= '%s'",tmp);
		if(!tmp.IsEmpty()) m_query += " and ";
		m_query += str;
	}

	GetDlgItemText(IDC_EDIT_SJDW, tmp);
	tmp.TrimLeft(); tmp.TrimRight();
	if(!tmp.IsEmpty())
	{
		str.Format("sjdw = '%s'", tmp);
		if(!m_query.IsEmpty())  m_query += " and ";
		m_query += str;
	}

	GetDlgItemText(IDC_EDIT_SJKS, tmp);
	tmp.TrimLeft(); tmp.TrimRight();
	if(!tmp.IsEmpty())
	{
		str.Format("sjks = '%s'", tmp);
		if(!m_query.IsEmpty())  m_query += " and ";
		m_query += str;
	}

	GetDlgItemText(IDC_EDIT_BBLX, tmp);
	tmp.TrimLeft(); tmp.TrimRight();
	if(!tmp.IsEmpty())
	{
		str.Format("bblx = '%s'", tmp);
		if(!m_query.IsEmpty())  m_query += " and ";
		m_query += str;
	}

	GetDlgItemText(IDC_EDIT_BBMC, tmp);
	tmp.TrimLeft(); tmp.TrimRight();
	if(!tmp.IsEmpty())
	{
		if(IsBlurMode)
			str.Format("bbmc like \'%%%s%%\'", tmp);
		else
			str.Format("bbmc = '%s'", tmp);
		if(!m_query.IsEmpty())  m_query += " and ";
		m_query += str;
	}

	GetDlgItemText(IDC_COMBO_BGZT, tmp);
	tmp.TrimLeft(); tmp.TrimRight();
	if(!tmp.IsEmpty())
	{
		str.Empty();

		if(tmp.CompareNoCase("已登记") == 0)
		{
			str.Format("(bgzt IS NULL or bgzt < 1)");
		}
		if(tmp.CompareNoCase("已取材") == 0)
		{
			str.Format("bgzt IS NOT NULL and bgzt >= 1 and bgzt < 2");
		}
		if(tmp.CompareNoCase("已延期") == 0)
		{
			str.Format("bgzt IS NOT NULL and bgzt >= 2 and bgzt < 4");
		}
		if(tmp.CompareNoCase("已诊断") == 0)
		{
			str.Format("bgzt IS NOT NULL and bgzt >= 4 and bgzt < 8");
		}
		if(tmp.CompareNoCase("已审核") == 0)
		{
			str.Format("bgzt IS NOT NULL and bgzt >= 8 and bgzt < 16");
		}
		if(tmp.CompareNoCase("已打印") == 0)
		{
			str.Format("bgzt IS NOT NULL and bgzt >= 16 and bgzt < 32");
		}
		if(tmp.CompareNoCase("已发送") == 0)
		{
			str.Format("bgzt IS NOT NULL and bgzt >= 32");
		}

        if(!str.IsEmpty())
		{
			if(!m_query.IsEmpty())  m_query += " and ";
			m_query += str;
		}
	}

	GetDlgItemText(IDC_EDIT_CZYS, tmp);
	tmp.TrimLeft(); tmp.TrimRight();
	if(!tmp.IsEmpty())
	{
		if(IsBlurMode)
			str.Format("czys like \'%%%s%%\'", tmp);
		else
			str.Format("czys = '%s'", tmp);
		if(!m_query.IsEmpty())  m_query += " and ";
		m_query += str;
	}

	GetDlgItemText(IDC_EDIT_ZZYS, tmp);
	tmp.TrimLeft(); tmp.TrimRight();
	if(!tmp.IsEmpty())
	{
		if(IsBlurMode)
			str.Format("zzys like \'%%%s%%\'", tmp);
		else
			str.Format("zzys = '%s'", tmp);
		if(!m_query.IsEmpty())  m_query += " and ";
		m_query += str;
	}

	GetDlgItemText(IDC_EDIT_SHYS, tmp);
	tmp.TrimLeft(); tmp.TrimRight();
	if(!tmp.IsEmpty())
	{
		if(IsBlurMode)
			str.Format("shys like \'%%%s%%\'", tmp);
		else
			str.Format("shys = '%s'", tmp);
		if(!m_query.IsEmpty())  m_query += " and ";
		m_query += str;
	}

	GetDlgItemText(IDC_EDIT_ZDBM, tmp);
	tmp.TrimLeft(); tmp.TrimRight();
	if(!tmp.IsEmpty())
	{
		str.Format("zdbm = '%s'", tmp);
		if(!m_query.IsEmpty())  m_query += " and ";
		m_query += str;
	}

	GetDlgItemText(IDC_EDIT_ZDGJC, tmp);
	tmp.TrimLeft(); tmp.TrimRight();
	if(!tmp.IsEmpty())
	{ 
		str.Empty();
		while(tmp.Find('|') >= 0)
		{
			CString t1 = tmp.Left(tmp.Find('|'));
			tmp = tmp.Right(tmp.GetLength() - tmp.Find('|') - 1);
			tmp.TrimLeft();  tmp.TrimRight();

			t1.TrimLeft();  t1.TrimRight();
			if(!t1.IsEmpty())
			{
				t1 = "zdgjc LIKE '%" + t1 + "%'";
				if(str.IsEmpty())
				{
					str = t1;
				}
				else
				{
					str += " and ";
					str += t1;
				}
			}
		}

		tmp.TrimLeft();  tmp.TrimRight();
		if(!tmp.IsEmpty())
		{
			tmp = "zdgjc LIKE '%" + tmp + "%'";
			if(str.IsEmpty())
			{
				str = tmp;
			}
			else
			{
				str += " and ";
				str += tmp;
			}
		}
		
		if(!m_query.IsEmpty())  m_query += " and ";
		m_query += str;
	}
	
	GetDlgItemText(IDC_EDIT_LCZD, tmp);
	tmp.TrimLeft(); tmp.TrimRight();
	if(!tmp.IsEmpty())
	{ 
		str.Empty();
		while(tmp.Find('|') >= 0)
		{
			CString t1 = tmp.Left(tmp.Find('|'));
			tmp = tmp.Right(tmp.GetLength() - tmp.Find('|') - 1);
			tmp.TrimLeft();  tmp.TrimRight();

			t1.TrimLeft();  t1.TrimRight();
			if(!t1.IsEmpty())
			{
				t1 = "lczd LIKE '%" + t1 + "%'";
				if(str.IsEmpty())
				{
					str = t1;
				}
				else
				{
					str += " and ";
					str += t1;
				}
			}
		}

		tmp.TrimLeft();  tmp.TrimRight();
		if(!tmp.IsEmpty())
		{
			tmp = "lczd LIKE '%" + tmp + "%'";
			if(str.IsEmpty())
			{
				str = tmp;
			}
			else
			{
				str += " and ";
				str += tmp;
			}
		}
		
		if(!m_query.IsEmpty())  m_query += " and ";
		m_query += str;
	}

	GetDlgItemText(IDC_EDIT_BLZD, tmp);
	tmp.TrimLeft(); tmp.TrimRight();
	if(!tmp.IsEmpty())
	{ 
		str.Empty();
		while(tmp.Find('|') >= 0)
		{
			CString t1 = tmp.Left(tmp.Find('|'));
			tmp = tmp.Right(tmp.GetLength() - tmp.Find('|') - 1);
			tmp.TrimLeft();  tmp.TrimRight();

			t1.TrimLeft();  t1.TrimRight();
			if(!t1.IsEmpty())
			{
				t1 = "zdyj LIKE '%" + t1 + "%'";
				if(str.IsEmpty())
				{
					str = t1;
				}
				else
				{
					str += " and ";
					str += t1;
				}
			}
		}

		tmp.TrimLeft();  tmp.TrimRight();
		if(!tmp.IsEmpty())
		{
			tmp = "zdyj LIKE '%" + tmp + "%'";
			if(str.IsEmpty())
			{
				str = tmp;
			}
			else
			{
				str += " and ";
				str += tmp;
			}
		}
		
		if(!m_query.IsEmpty())  m_query += " and ";
		m_query += str;
	}

	if(((CButton *)GetDlgItem(IDC_CHECK_YQBG))->GetState() & 0x0003)
	{
		str.Format("bgzt&2=2");
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			str.Format("bitand(bgzt,2)=2");
		}
		if(!m_query.IsEmpty())  m_query += " and ";
		m_query += str;
	}

	if(((CButton *)GetDlgItem(IDC_CHECK_BDBG))->GetState() & 0x0003)
	{
		str.Format("((bdh IS NOT NULL and bdh != '') or (bdjg IS NOT NULL and bdjg != ''))");
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			str.Format("(bdh is not null) or (bdjg is not null)");
		}
		if(!m_query.IsEmpty())  m_query += " and ";
		m_query += str;
	}

	if(((CButton *)GetDlgItem(IDC_CHECK_BCBG))->GetState() & 0x0003)
	{
		str.Format("(bczd IS NOT NULL and bczd != '')");
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			str.Format("(bczd is not null)");
		}
		if(!m_query.IsEmpty())  m_query += " and ";
		m_query += str;
	}

	if(((CButton *)GetDlgItem(IDC_CHECK_HZJL))->GetState() & 0x0003)
	{
		str.Format("(zjyj IS NOT NULL and zjyj != '')");
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			str.Format("(zjyj IS NOT NULL)");
		}
		if(!m_query.IsEmpty())  m_query += " and ";
		m_query += str;
	}

	CDialog::OnOK();
}

void CQuickQuery::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (pListInput != NULL)
	{
		pListInput->DestroyWindow();
		delete pListInput;
		pListInput = NULL;
	}
}

LRESULT CQuickQuery::ListSelect(WPARAM wparam, LPARAM lparam)
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

void CQuickQuery::OnButtonMicd10() 
{
	if(pListInput != NULL)
	{
		pListInput->m_listinfo.SetNumberRows(0);
		pListInput->m_listinfo.RedrawAll();

		pListInput->m_type = 1;  // 1 - MICD10, 2 - SICD10
		pListInput->ShowWindow(SW_SHOW);
	}
}

void CQuickQuery::OnButtonSicd10() 
{
	if(pListInput != NULL)
	{
		pListInput->m_listinfo.SetNumberRows(0);
		pListInput->m_listinfo.RedrawAll();

		pListInput->m_type = 2;  // 1 - MICD10, 2 - SICD10
		pListInput->ShowWindow(SW_SHOW);
	}
}
