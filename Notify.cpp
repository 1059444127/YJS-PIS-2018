// Notify.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "Notify.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNotify dialog


CNotify::CNotify(CWnd* pParent /*=NULL*/)
	: CDialog(CNotify::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNotify)
	m_info = _T("");
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_nID = CNotify::IDD;
	m_TimeDelay = 2000;
	m_Title.Empty();
}


void CNotify::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNotify)
	DDX_Text(pDX, IDC_STATIC_INFO, m_info);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNotify, CDialog)
	//{{AFX_MSG_MAP(CNotify)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNotify message handlers

BOOL CNotify::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(!m_Title.IsEmpty())
	{
		SetWindowText(m_Title);
	}
	
	UpdateData(FALSE);

	m_TimerHandle  = SetTimer(1, m_TimeDelay, NULL);
	m_TimerHandle1 = SetTimer(2, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CNotify::Create()
{
	return CDialog::Create(m_nID, m_pParent);
}

void CNotify::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == m_TimerHandle1)
	{
		BringWindowToTop();	
	}

	if(nIDEvent == m_TimerHandle)
	{
		DestroyWindow();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CNotify::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if(m_TimerHandle1)
	{
		KillTimer(m_TimerHandle1);
		m_TimerHandle1 = 0;
	}
	if(m_TimerHandle)
	{
		KillTimer(m_TimerHandle);
		m_TimerHandle = 0;
	}
}
