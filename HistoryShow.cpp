// HistoryShow.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "HistoryShow.h"
#include "MyEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistoryShow dialog

extern CPathologyApp theApp;
CHistoryShow::CHistoryShow(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CHistoryShow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHistoryShow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHistoryShow::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHistoryShow)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_EDIT_RYSJ, m_rysj);
	DDX_Control(pDX, IDC_EDIT_ZDYJ, m_zdyj);
	DDX_Control(pDX, IDC_EDIT_BDJG, m_bdbg);
	DDX_Control(pDX, IDC_EDIT_BCZD, m_bcyj);
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNCREATE(CHistoryShow, CXTResizeDialog)

BEGIN_MESSAGE_MAP(CHistoryShow, CXTResizeDialog)
	//{{AFX_MSG_MAP(CHistoryShow)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistoryShow message handlers

BOOL CHistoryShow::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_grid.AttachGrid(this,IDC_STATIC_RECORDLIST);
	m_grid.SetParent(this);
	m_grid.ResetGridSize();
	m_grid.RedrawAll();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHistoryShow::OnDestroy() 
{
	CXTResizeDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	//theApp.OnAppRefresh();
}

void CHistoryShow::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	theApp.OnAppRefresh();
	CXTResizeDialog::OnClose();
}
