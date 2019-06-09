// Input.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "Input.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInput dialog


CInput::CInput(CWnd* pParent /*=NULL*/)
	: CDialog(CInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInput)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInput)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInput, CDialog)
	//{{AFX_MSG_MAP(CInput)
	ON_BN_CLICKED(ID_INPUT_OK, OnInputOk)
	ON_BN_CLICKED(ID_INPUT_CANCEL, OnInputCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInput message handlers

void CInput::OnInputOk() 
{
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_INPUT,m_blh);
	OnOK();
}

void CInput::OnInputCancel() 
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

BOOL CInput::OnInitDialog(){
	CDialog::OnInitDialog();
	//AfxMessageBox("!!!"); 
	CEdit* input = (CEdit*)GetDlgItem(IDC_INPUT);
	//CDialog::SetFocus(input->m_hWnd);
	//input->SetFocus();
	//::SendMessage(input->m_hWnd,WM_SETFOCUS,0,0);
	return TRUE;
}