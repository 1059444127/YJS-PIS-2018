// SflbShow.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "SflbShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSflbShow dialog


CSflbShow::CSflbShow(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CSflbShow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSflbShow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSflbShow::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSflbShow)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSflbShow, CXTResizeDialog)
	//{{AFX_MSG_MAP(CSflbShow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSflbShow message handlers

BOOL CSflbShow::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_grid.AttachGrid(this,IDC_STATIC_RECORDLIST);
	m_grid.SetParent(this);
	m_grid.ResetGridSize();
	m_grid.RedrawAll();

	SetResize(IDC_STATIC_RECORDLIST, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
