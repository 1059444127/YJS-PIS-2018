// BatchPrintCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "BatchPrintCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBatchPrintCtrl dialog
CBatchPrintCtrl::CBatchPrintCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CBatchPrintCtrl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBatchPrintCtrl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_nID = CBatchPrintCtrl::IDD;
}


void CBatchPrintCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBatchPrintCtrl)
	DDX_Control(pDX, IDC_BUTTON_STOPPRINT, m_StopPrint);
	DDX_Control(pDX, IDC_BUTTON_STOPPREVIEW, m_StopPreview);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBatchPrintCtrl, CDialog)
	//{{AFX_MSG_MAP(CBatchPrintCtrl)
	ON_BN_CLICKED(IDC_BUTTON_STOPPRINT, OnButtonStopprint)
	ON_BN_CLICKED(IDC_BUTTON_STOPPREVIEW, OnButtonStoppreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBatchPrintCtrl message handlers

BOOL CBatchPrintCtrl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	const DWORD STYLE_ALL = BS_LEFT | BS_RIGHT | BS_CENTER;
	m_StopPreview.SetIcon(CSize(16,16),IDI_ICON_STOPPREVIEW );
	m_StopPreview.SetXButtonStyle(BS_XT_XPFLAT); 
    m_StopPreview.ModifyStyle(STYLE_ALL & ~BS_CENTER, BS_CENTER);
	
	m_StopPrint.SetIcon(CSize(16,16),IDI_ICON_STOPPRINT );
	m_StopPrint.SetXButtonStyle(BS_XT_XPFLAT); 
    m_StopPrint.ModifyStyle(STYLE_ALL & ~BS_CENTER, BS_CENTER);

	m_StopPreview.ShowWindow(FALSE);

	m_TimerHandle = SetTimer(1, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBatchPrintCtrl::Create()
{
	return CDialog::Create(m_nID, m_pParent);
}

void CBatchPrintCtrl::OnButtonStopprint() 
{
	// TODO: Add your control notification handler code here
	*pStopPrint = true;
}

void CBatchPrintCtrl::OnButtonStoppreview() 
{
	// TODO: Add your control notification handler code here
	*pStopPreview = true;
}

void CBatchPrintCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDown(nFlags, point);

	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
}

void CBatchPrintCtrl::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent == m_TimerHandle )
	{
		BringWindowToTop();	
		ShowWindow(SW_SHOW );
	}

	CDialog::OnTimer(nIDEvent);
}

void CBatchPrintCtrl::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_TimerHandle)
	{
		KillTimer(m_TimerHandle);
		m_TimerHandle = 0;
	}
}
