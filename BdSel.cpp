// BdSel.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "BdSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBdSel dialog
extern CPathologyApp theApp;

CBdSel::CBdSel(CWnd* pParent /*=NULL*/)
	: CDialog(CBdSel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBdSel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_TableName.Empty();
}


void CBdSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBdSel)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBdSel, CDialog)
	//{{AFX_MSG_MAP(CBdSel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBdSel message handlers

BOOL CBdSel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CStdioFile File;
	if ( File.Open(theApp.m_Exe_Working_Dir + "system\\±íµ¥.txt", CFile::modeRead | CFile::typeText) )
	{
		CString tmp;
		while(1)
		{
			if( File.ReadString(tmp) == FALSE ) break;
			tmp.TrimLeft(); tmp.TrimRight();
			if(!tmp.IsEmpty())
			{
				((CComboBox *)GetDlgItem(IDC_COMBO_DB))->AddString(tmp);
			}
		}
		File.Close();
	}

	if(((CComboBox *)GetDlgItem(IDC_COMBO_DB))->GetCount() < 1)
	{
		EndDialog(IDOK);
		return FALSE;
	}

	((CComboBox *)GetDlgItem(IDC_COMBO_DB))->SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBdSel::OnOK() 
{
	((CComboBox *)GetDlgItem(IDC_COMBO_DB))->GetLBText(((CComboBox *)GetDlgItem(IDC_COMBO_DB))->GetCurSel(), m_TableName);
	
	CDialog::OnOK();
}
