// QcyzBZ.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "QcyzBZ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQcyzBZ dialog


CQcyzBZ::CQcyzBZ(CWnd* pParent /*=NULL*/)
	: CDialog(CQcyzBZ::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQcyzBZ)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CQcyzBZ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQcyzBZ)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQcyzBZ, CDialog)
	//{{AFX_MSG_MAP(CQcyzBZ)
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQcyzBZ message handlers

void CQcyzBZ::OnOK() 
{
	// TODO: Add extra validation here
	GetDlgItemText(IDC_EDIT_QCYZBZ,m_qcyz);
	CDialog::OnOK();
}

//DEL void CQcyzBZ::OnOk2() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	
//DEL }

void CQcyzBZ::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CQcyzBZ::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetDlgItemText(IDC_EDIT_QCYZBZ,m_qcyz);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CQcyzBZ::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}
