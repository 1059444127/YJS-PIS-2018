// FieldOut.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "FieldOut.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFieldOut dialog


CFieldOut::CFieldOut(CWnd* pParent /*=NULL*/)
	: CDialog(CFieldOut::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFieldOut)
	m_Index = -1;
	//}}AFX_DATA_INIT
	title.Empty();
}


void CFieldOut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFieldOut)
	DDX_CBIndex(pDX, IDC_COMBO_FIELD, m_Index);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFieldOut, CDialog)
	//{{AFX_MSG_MAP(CFieldOut)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFieldOut message handlers

BOOL CFieldOut::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	for(int i = 0; i < strlist.GetSize(); i++ )
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_FIELD))->AddString(strlist.GetAt(i));
	}
	((CComboBox *)GetDlgItem(IDC_COMBO_FIELD))->SetCurSel(0);

	if(!title.IsEmpty())
	{
		SetWindowText(title);
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFieldOut::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	strlist.RemoveAll();
}
