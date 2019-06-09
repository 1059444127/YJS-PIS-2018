// NoUse.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "NoUse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNoUse dialog


CNoUse::CNoUse(CWnd* pParent /*=NULL*/)
	: CDialog(CNoUse::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNoUse)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNoUse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNoUse)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNoUse, CDialog)
	//{{AFX_MSG_MAP(CNoUse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNoUse message handlers



BOOL CNoUse::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect;
	GetDlgItem(IDC_STATIC_WAXINM)->GetWindowRect(&rect);
//	wax.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rect,this,IDC_STATIC_WAXINM,NULL);

	//wax.OnInitialUpdate();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
