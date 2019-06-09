// HzlbShow.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "HzlbShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHzlbShow dialog


CHzlbShow::CHzlbShow(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CHzlbShow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHzlbShow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHzlbShow::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHzlbShow)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHzlbShow, CXTResizeDialog)
	//{{AFX_MSG_MAP(CHzlbShow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHzlbShow message handlers

BOOL CHzlbShow::OnInitDialog() 
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
