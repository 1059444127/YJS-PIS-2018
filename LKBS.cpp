// LKBS.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "LKBS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLKBS dialog


CLKBS::CLKBS(CWnd* pParent /*=NULL*/)
	: CDialog(CLKBS::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLKBS)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLKBS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLKBS)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLKBS, CDialog)
	//{{AFX_MSG_MAP(CLKBS)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLKBS message handlers

void CLKBS::OnOK() 
{
	// TODO: Add extra validation here
	GetDlgItemText(IDC_EDIT_LKBS,m_lkbs);
	CDialog::OnOK();
}
