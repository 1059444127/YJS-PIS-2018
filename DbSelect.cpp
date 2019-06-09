// DbSelect.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "DbSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDbSelect dialog
extern CPathologyApp theApp;

CDbSelect::CDbSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CDbSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDbSelect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDbSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDbSelect)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDbSelect, CDialog)
	//{{AFX_MSG_MAP(CDbSelect)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDbSelect message handlers

BOOL CDbSelect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	for(int i = 0; i < theApp.m_DBName_list.GetSize(); i++)
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_DB))->AddString(theApp.m_DBName_list.GetAt(i));
	}

	int rt = ((CComboBox *)GetDlgItem(IDC_COMBO_DB))->FindStringExact(-1, m_DBName);
	if(rt != CB_ERR)
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_DB))->SetCurSel(rt);
	}
	else
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_DB))->SetCurSel(0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDbSelect::OnOK() 
{
	// TODO: Add extra validation here
	m_DBName = theApp.m_DBName_list.GetAt(((CComboBox *)GetDlgItem(IDC_COMBO_DB))->GetCurSel());
	m_DBNo   = theApp.m_DBNo_list.GetAt(((CComboBox *)GetDlgItem(IDC_COMBO_DB))->GetCurSel());

	CDialog::OnOK();
}

void CDbSelect::OnDestroy() 
{
	CDialog::OnDestroy();
}
