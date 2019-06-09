// AdviceDelReason.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "AdviceDelReason.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdviceDelReason dialog


CAdviceDelReason::CAdviceDelReason(CWnd* pParent /*=NULL*/)
	: CDialog(CAdviceDelReason::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdviceDelReason)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAdviceDelReason::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdviceDelReason)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdviceDelReason, CDialog)
	//{{AFX_MSG_MAP(CAdviceDelReason)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdviceDelReason message handlers

void CAdviceDelReason::OnOK() 
{
	// TODO: Add extra validation here
	GetDlgItemText(IDC_EDIT_DELREASON,m_reason);
	CDialog::OnOK();
}

void CAdviceDelReason::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
