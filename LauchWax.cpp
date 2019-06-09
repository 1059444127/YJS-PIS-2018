// LauchWax.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "LauchWax.h"
#include "MedWax.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLauchWax dialog


CLauchWax::CLauchWax(CWnd* pParent /*=NULL*/)
	: CDialog(CLauchWax::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLauchWax)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLauchWax::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLauchWax)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLauchWax, CDialog)
	//{{AFX_MSG_MAP(CLauchWax)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLauchWax message handlers

BOOL CLauchWax::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	wax.Attach(this->m_hWnd);
	wax.SetParent(this);
	wax.ShowWindow(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
