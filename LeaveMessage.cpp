// LeaveMessage.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "LeaveMessage.h"
#include "KnlyShow.h"
#include "MainFrm.h"
#include "TabDockWnd.h"
#include "Pathology.h"
#include "DBList.h"
#include "AdviceShow.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeaveMessage dialog

extern CPathologyApp theApp;
CLeaveMessage::CLeaveMessage(CWnd* pParent /*=NULL*/)
	: CDialog(CLeaveMessage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLeaveMessage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	pParent = theApp.GetMainWnd();
	count = "0";
}


void CLeaveMessage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLeaveMessage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_LINK_LY, m_link_ly);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLeaveMessage, CDialog)
	//{{AFX_MSG_MAP(CLeaveMessage)
	ON_BN_CLICKED(IDC_BUTTON_LY, OnButtonLy)
	ON_BN_CLICKED(IDC_LINK_LY, OnLinkLy)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeaveMessage message handlers

//DEL void CLeaveMessage::OnStaticText() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	CKnlyShow dlg;
//DEL 	dlg.DoModal();
//DEL 	OnOK();
//DEL }

void CLeaveMessage::OnButtonLy() 
{
	// TODO: Add your control notification handler code here
	OnDestroy();
	DestroyWindow();
//	CKnlyShow dlg;
//	dlg.SetParent(AfxGetMainWnd());
	CMainFrame* main = (CMainFrame*)(theApp.GetMainWnd());
	//CTabDockWnd dock = main->m_wndTabDockBar;
	CWnd* win = main->m_wndTabDockBar.GetView(0);
	if(win->IsKindOf(RUNTIME_CLASS(CDBList))){
		((CDBList*)win)->GetLeaveMessage();
	} else if(win->IsKindOf(RUNTIME_CLASS(CAdviceShow))){
		((CAdviceShow*)win)->GetLeaveMessage();
	}

}

void CLeaveMessage::OnLinkLy() 
{
	// TODO: Add your control notification handler code here
	OnButtonLy();
}

BOOL CLeaveMessage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_link_ly.SetURL("");
	m_link_ly.SetUnderline(TRUE);
	m_link_ly.EnableShellExecute(FALSE);
	
	SetDlgItemText(IDC_STATIC_COUNTLY, "ÓÐ"+count+"Ìõ¼ÇÂ¼");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLeaveMessage::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CLeaveMessage::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
}
