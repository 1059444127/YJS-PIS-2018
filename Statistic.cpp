// Statistic.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "Statistic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatistic dialog
extern CPathologyApp theApp;

CStatistic::CStatistic(CWnd* pParent /*=NULL*/)
	: CXTCBarDialog(CStatistic::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatistic)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_STATISTIC);
	m_Title.Empty();
}


void CStatistic::DoDataExchange(CDataExchange* pDX)
{
	CXTCBarDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatistic)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatistic, CXTCBarDialog)
	//{{AFX_MSG_MAP(CStatistic)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(ID_YEAR, OnSelchangeComboYear)
	ON_CBN_SELCHANGE(ID_TIME, OnSelchangeComboTime)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatistic message handlers

BOOL CStatistic::OnInitDialog() 
{
	CXTCBarDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetWindowText(m_Title);

	// TODO: Add extra initialization here
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),AFX_IDW_TOOLBAR+25)||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_STATISTIC))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;      // fail to create
	}
	if (!m_wndYear.Create( WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_CLIPCHILDREN,
		CRect(0,0,80,550), &m_wndToolBar, ID_YEAR ))
	{
		TRACE0("Failed to create combo box.\n");
		return -1;      // fail to create
	}
	if (!m_wndTime.Create( WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_CLIPCHILDREN,
		CRect(0,0,80,550), &m_wndToolBar, ID_TIME ))
	{
		TRACE0("Failed to create combo box.\n");
		return -1;      // fail to create
	}
	m_wndToolBar.InsertControl(&m_wndYear);
	m_wndToolBar.InsertControl(&m_wndTime);

	COleDateTime curt = theApp.GetOleServerDateTime();

	int index = 0;
	for(int i = 1980; i < 2020; i++)
	{
		CString str;
		str.Format("%d",i);
		m_wndYear.AddString(str);
		
		if(i == curt.GetYear())
		{
			index = i - 1980;
		}
	}
	m_wndYear.SetCurSel(index);

	m_wndTime.AddString("全年");
	m_wndTime.AddString("一月份");
	m_wndTime.AddString("二月份");
	m_wndTime.AddString("三月份");
	m_wndTime.AddString("四月份");
	m_wndTime.AddString("五月份");
	m_wndTime.AddString("六月份");
	m_wndTime.AddString("七月份");
	m_wndTime.AddString("八月份");
	m_wndTime.AddString("九月份");
	m_wndTime.AddString("十月份");
	m_wndTime.AddString("十一月份");
	m_wndTime.AddString("十二月份");
	m_wndTime.AddString("一季度");
	m_wndTime.AddString("二季度");
	m_wndTime.AddString("三季度");
	m_wndTime.AddString("四季度");
	m_wndTime.SetCurSel(0);

	((CEdit *)GetDlgItem(IDC_EDIT_START))->SetWindowText(curt.Format("%Y-01-01 00:00:00"));
	((CEdit *)GetDlgItem(IDC_EDIT_END))->SetWindowText(curt.Format("%Y-12-31 23:59:59"));

	SetDlgItemText(IDOK, "查询");

	// Move control bars to the correct position.
    RecalcBarLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HCURSOR CStatistic::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CStatistic::OnSelchangeComboYear() 
{
	ChangeTimeStr();
}

void CStatistic::OnSelchangeComboTime() 
{
	ChangeTimeStr();
}

void CStatistic::ChangeTimeStr()
{
	CString year, start, end;
	m_wndYear.GetWindowText(year);
	year.TrimLeft();  year.TrimRight();

	switch(m_wndTime.GetCurSel()) {
	case 0:
		start = year + "-01-01 00:00:00";
		end   = year + "-12-31 23:59:59";
		break;
	case 1:
		start = year + "-01-01 00:00:00";
		end   = year + "-01-31 23:59:59";
		break;
	case 2:
		start = year + "-02-01 00:00:00";
		end   = year + "-02-28 23:59:59";
		break;
	case 3:
		start = year + "-03-01 00:00:00";
		end   = year + "-03-31 23:59:59";
		break;
	case 4:
		start = year + "-04-01 00:00:00";
		end   = year + "-04-30 23:59:59";
		break;
	case 5:
		start = year + "-05-01 00:00:00";
		end   = year + "-05-31 23:59:59";
		break;
	case 6:
		start = year + "-06-01 00:00:00";
		end   = year + "-06-30 23:59:59";
		break;
	case 7:
		start = year + "-07-01 00:00:00";
		end   = year + "-07-31 23:59:59";
		break;
	case 8:
		start = year + "-08-01 00:00:00";
		end   = year + "-08-31 23:59:59";
		break;
	case 9:
		start = year + "-09-01 00:00:00";
		end   = year + "-09-30 23:59:59";
		break;
	case 10:
		start = year + "-10-01 00:00:00";
		end   = year + "-10-31 23:59:59";
		break;
	case 11:
		start = year + "-11-01 00:00:00";
		end   = year + "-11-30 23:59:59";
		break;
	case 12:
		start = year + "-12-01 00:00:00";
		end   = year + "-12-31 23:59:59";
		break;
	case 13:
		start = year + "-01-01 00:00:00";
		end   = year + "-03-31 23:59:59";
		break;
	case 14:
		start = year + "-04-01 00:00:00";
		end   = year + "-06-30 23:59:59";
		break;
	case 15:
		start = year + "-07-01 00:00:00";
		end   = year + "-09-30 23:59:59";
		break;
	case 16:
		start = year + "-10-01 00:00:00";
		end   = year + "-12-31 23:59:59";
		break;
	default:;
	}

	((CEdit *)GetDlgItem(IDC_EDIT_START))->SetWindowText(start);
	((CEdit *)GetDlgItem(IDC_EDIT_END))->SetWindowText(end);
}

void CStatistic::OnOK() 
{
	// TODO: Add extra validation here
	GetDlgItemText(IDC_EDIT_START, m_from);
	GetDlgItemText(IDC_EDIT_END,   m_to);
	
	CXTCBarDialog::OnOK();
}
