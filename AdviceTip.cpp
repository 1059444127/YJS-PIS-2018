// AdviceTip.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "AdviceTip.h"
#include "PathologyView.h"
#include "MedAdvice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdviceTip dialog
extern CPathologyApp theApp;

CAdviceTip::CAdviceTip(CWnd* pParent /*=NULL*/)
	: CDialog(CAdviceTip::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdviceTip)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAdviceTip::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdviceTip)
	DDX_Control(pDX,IDC_LINK_ADVICE,m_link_advice);
	DDX_Control(pDX,IDC_STATIC_TGTip,m_link_tg);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdviceTip, CDialog)
	//{{AFX_MSG_MAP(CAdviceTip)
	ON_BN_CLICKED(IDC_LINK_ADVICE, OnLinkAdvice)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdviceTip message handlers

void CAdviceTip::OnLinkAdvice() 
{
	// TODO: Add your control notification handler code here
	if(theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedAdvice)) != NULL)
	{
		theApp.GetView()->m_tabCtrl.SetActiveView(RUNTIME_CLASS(CMedAdvice));
		DestroyWindow();
	}
	
}

BOOL CAdviceTip::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// TODO: Add extra initialization here
	m_link_advice.SetURL("");
	m_link_advice.SetUnderline(TRUE);
	m_link_advice.EnableShellExecute(FALSE);
	
	m_link_tg.SetWindowText("在脱钙记录" + countTG + "条");
	m_link_tg.SetColors(RGB(255,0,0),RGB(255,0,0),RGB(255,0,0));
	m_link_tg.SetURL("");
	m_link_tg.SetUnderline(TRUE);
	m_link_tg.EnableShellExecute(FALSE);
	
	SetDlgItemText(IDC_STATIC_ADVICETIP,"共" + count + "条记录");
//	SetDlgItemText(IDC_STATIC_TGTip,"脱钙" + countTG + "条记录");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
