// MyProgressBar.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "MyProgressBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyProgressBar dialog

CMyProgressBar::CMyProgressBar(CWnd* pParent /*=NULL*/)
	: CDialog(CMyProgressBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyProgressBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_nID = CMyProgressBar::IDD;
}


void CMyProgressBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyProgressBar)
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_bar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyProgressBar, CDialog)
	//{{AFX_MSG_MAP(CMyProgressBar)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyProgressBar message handlers
BOOL CMyProgressBar::Create()
{
	return CDialog::Create(m_nID, m_pParent);
}
