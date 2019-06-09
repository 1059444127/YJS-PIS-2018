// MeasureDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "MeasureDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeasureDialog dialog


CMeasureDialog::CMeasureDialog(CWnd* pParent /*=NULL*/)
	: CXTCBarDialog(CMeasureDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMeasureDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMeasureDialog::DoDataExchange(CDataExchange* pDX)
{
	CXTCBarDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasureDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMeasureDialog, CXTCBarDialog)
	//{{AFX_MSG_MAP(CMeasureDialog)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeasureDialog message handlers

BOOL CMeasureDialog::OnInitDialog() 
{
	CXTCBarDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect;
	GetClientRect(&rect);
    if(!m_MeasureCtrl.CreateGrid(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rect,this,111))
	{
		TRACE0( "Failed to create m_MeasureCtrl.\n" );
		return FALSE;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMeasureDialog::OnSize(UINT nType, int cx, int cy) 
{
	CXTCBarDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	//::MoveWindow(m_MeasureCtrl.m_hWnd,0,25,cx,cy-25,FALSE);
	::MoveWindow(m_MeasureCtrl.m_hWnd,0,0,cx,cy,FALSE);
}
