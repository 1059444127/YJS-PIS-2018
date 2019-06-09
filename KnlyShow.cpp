// KnlyShow.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "KnlyShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKnlyShow dialog
extern CPathologyApp theApp;

CKnlyShow::CKnlyShow(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CKnlyShow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKnlyShow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//pParent = theApp.GetMainWnd();
}


void CKnlyShow::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKnlyShow)
	DDX_Control(pDX, IDC_HYP_NOTE, m_HypLink_Note);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKnlyShow, CXTResizeDialog)
	//{{AFX_MSG_MAP(CKnlyShow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKnlyShow message handlers

BOOL CKnlyShow::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_HypLink_Note.SetURL("说明: 请在\"事项\"栏双击鼠标左键查看详细事项");
	m_HypLink_Note.SetColors(RGB(0x00, 0x00, 0xFF), RGB(0x00, 0x00, 0xFF), RGB(0x00, 0x00, 0xFF));
	m_HypLink_Note.SetUnderline(true);
	m_HypLink_Note.EnableShellExecute(false);

	m_grid.AttachGrid(this,IDC_STATIC_RECORDLIST);
	m_grid.SetParent(this);
	m_grid.ResetGridSize();
	m_grid.RedrawAll();
	
	SetResize(IDC_STATIC_RECORDLIST, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_HYP_NOTE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
