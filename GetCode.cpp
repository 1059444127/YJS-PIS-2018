// GetCode.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "GetCode.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetCode dialog


CGetCode::CGetCode(CWnd* pParent /*=NULL*/)
	: CDialog(CGetCode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGetCode)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGetCode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetCode)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGetCode, CDialog)
	//{{AFX_MSG_MAP(CGetCode)
	ON_BN_CLICKED(IDCOPY, OnCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetCode message handlers

BOOL CGetCode::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetDlgItemText(IDC_EDIT_CODE, ((CMainFrame*)AfxGetMainWnd())->RandomCode());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGetCode::OnCopy() 
{
	if (OpenClipboard())
	{
		CString source = ((CMainFrame*)AfxGetMainWnd())->RandomCode();
		EmptyClipboard();
		HGLOBAL clipbuffer;
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, source.GetLength()+1);
		char *buffer = (char*)GlobalLock(clipbuffer);
		strcpy(buffer, LPCSTR(source));
		GlobalUnlock(clipbuffer);
		SetClipboardData (CF_TEXT, clipbuffer );
		CloseClipboard();
	}

	EndDialog(IDOK);
}
