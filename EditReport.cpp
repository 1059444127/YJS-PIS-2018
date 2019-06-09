// EditReport.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "EditReport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EditReport dialog

EditReport::EditReport(CWnd* pParent /*=NULL*/)
	: CDialog(EditReport::IDD, pParent)
{
	//{{AFX_DATA_INIT(EditReport)
	m_Report = "";
	m_Result = "";
	//}}AFX_DATA_INIT
}

void EditReport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EditReport)
	DDX_Text(pDX, IDC_EDIT1, m_Report);
	DDX_Text(pDX, IDC_EDIT2, m_Result);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(EditReport, CDialog)
	//{{AFX_MSG_MAP(EditReport)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// EditReport message handlers
