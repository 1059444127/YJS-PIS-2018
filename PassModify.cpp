// PassModify.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "PassModify.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPassModify dialog


CPassModify::CPassModify(CWnd* pParent /*=NULL*/)
	: CDialog(CPassModify::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPassModify)
	m_ps1 = _T("");
	m_ps2 = _T("");
	//}}AFX_DATA_INIT
}


void CPassModify::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPassModify)
	DDX_Text(pDX, IDC_EDIT_PS1, m_ps1);
	DDV_MaxChars(pDX, m_ps1, 255);
	DDX_Text(pDX, IDC_EDIT_PS2, m_ps2);
	DDV_MaxChars(pDX, m_ps2, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPassModify, CDialog)
	//{{AFX_MSG_MAP(CPassModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPassModify message handlers

void CPassModify::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	m_ps1.TrimLeft();
	m_ps1.TrimRight();
	m_ps2.TrimLeft();
	m_ps2.TrimRight();

	if( m_ps1.Compare(m_ps2) != 0 )
	{
		AfxMessageBox("新密码与确认密码不一致！");
		return;
	}

/*
	if( m_ps1.GetLength() < 6 )
	{
		AfxMessageBox("密码长度不能少于6个字符！");
		return;
	}
*/
	
	CDialog::OnOK();
}

BOOL CPassModify::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetWindowText(m_title);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
