// NewUser.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "NewUser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewUser dialog


CNewUser::CNewUser(CWnd* pParent /*=NULL*/)
	: CDialog(CNewUser::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewUser)
	m_name = _T("");
	m_info = _T("");
	//}}AFX_DATA_INIT
	m_status = 0;
}


void CNewUser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewUser)
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDV_MaxChars(pDX, m_name, 255);
	DDX_Text(pDX, IDC_EDIT_OTHERINFO, m_info);
	DDV_MaxChars(pDX, m_info, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewUser, CDialog)
	//{{AFX_MSG_MAP(CNewUser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewUser message handlers

BOOL CNewUser::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( m_status == 1 )
	{
		((CEdit *)GetDlgItem(IDC_EDIT_NAME))->EnableWindow(FALSE);
		SetWindowText("用户信息修改");
	}
	if( m_status == 2 )
	{
		((CStatic *)GetDlgItem(IDC_STATIC_NAME))->SetWindowText("宽度：");
		((CStatic *)GetDlgItem(IDC_STATIC_INFO))->SetWindowText("高度：");
		SetWindowText("指定裁切矩形区域");
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
