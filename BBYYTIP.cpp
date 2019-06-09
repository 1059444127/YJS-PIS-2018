// BBYYTIP.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "BBYYTIP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBBYYTIP dialog


CBBYYTIP::CBBYYTIP(CWnd* pParent /*=NULL*/)
	: CDialog(CBBYYTIP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBBYYTIP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBBYYTIP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBBYYTIP)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBBYYTIP, CDialog)
	//{{AFX_MSG_MAP(CBBYYTIP)
	ON_BN_CLICKED(ID_BUTTON_BBYYTIP, OnButtonBbyytip) 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBBYYTIP message handlers

void CBBYYTIP::OnButtonBbyytip() 
{
	// TODO: Add your control notification handler code here
	//AfxMessageBox("~~~");
}
