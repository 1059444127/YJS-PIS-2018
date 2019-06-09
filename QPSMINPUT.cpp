// QPSMINPUT.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "QPSMINPUT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQPSMINPUT dialog


CQPSMINPUT::CQPSMINPUT(CWnd* pParent /*=NULL*/)
	: CDialog(CQPSMINPUT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQPSMINPUT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CQPSMINPUT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQPSMINPUT)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQPSMINPUT, CDialog)
	//{{AFX_MSG_MAP(CQPSMINPUT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQPSMINPUT message handlers

void CQPSMINPUT::OnOK() 
{
	// TODO: Add extra validation here
	GetDlgItemText(IDC_EDIT_QPSMBATCH,m_qpsm);
	CDialog::OnOK();
}

void CQPSMINPUT::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CQPSMINPUT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
