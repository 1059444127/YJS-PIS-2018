// DlgGamma1.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "DlgGamma1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGamma1 dialog


CDlgGamma1::CDlgGamma1(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGamma1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGamma1)
	m_dGammRatio = 1.0;
	//}}AFX_DATA_INIT
}


void CDlgGamma1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGamma1)
	DDX_Text(pDX, IDC_GAMMA_RATIO, m_dGammRatio);
	DDV_MinMaxDouble(pDX, m_dGammRatio, 0.3, 3.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGamma1, CDialog)
	//{{AFX_MSG_MAP(CDlgGamma1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGamma1 message handlers

void CDlgGamma1::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CDialog::OnOK();
}

void CDlgGamma1::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
