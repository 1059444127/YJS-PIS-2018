// MantenancePicNote.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "MantenancePicNote.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMantenancePicNote dialog


CMantenancePicNote::CMantenancePicNote(CWnd* pParent /*=NULL*/)
	: CDialog(CMantenancePicNote::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMantenancePicNote)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMantenancePicNote::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMantenancePicNote)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMantenancePicNote, CDialog)
	//{{AFX_MSG_MAP(CMantenancePicNote)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMantenancePicNote message handlers

void CMantenancePicNote::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
