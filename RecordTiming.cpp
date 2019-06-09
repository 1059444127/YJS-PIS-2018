// RecordTiming.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "MainFrm.h"
#include "RecordTiming.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecordTiming dialog

CRecordTiming::CRecordTiming(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CRecordTiming::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecordTiming)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_nID = CRecordTiming::IDD;
}


void CRecordTiming::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecordTiming)
	DDX_Control(pDX, IDC_LEDTEXT, m_LedText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecordTiming, CXTResizeDialog)
	//{{AFX_MSG_MAP(CRecordTiming)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CLOCK_TIMEEND,ClockEnded)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordTiming message handlers

BOOL CRecordTiming::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    SetResize(IDC_LEDTEXT, SZ_TOP_LEFT,  SZ_BOTTOM_RIGHT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CRecordTiming::Create()
{
	return CDialog::Create(m_nID, m_pParent);
}

void CRecordTiming::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CXTResizeDialog::OnLButtonDown(nFlags, point);

	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
}

LRESULT CRecordTiming::ClockEnded(WPARAM wparam, LPARAM lparam)
{
	((CMainFrame *)AfxGetMainWnd())->Sound_StopPlaying();
	return 0L;
}
