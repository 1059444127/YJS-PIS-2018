// BookingTip.cpp : implementation file
//

#include "stdafx.h" 
#include "Pathology.h"
#include "BookingTip.h"
#include "MainFrm.h"
#include "TabDockWnd.h"
#include "PathologyView.h"
#include "DBList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBookingTip dialog

extern CPathologyApp theApp;
CBookingTip::CBookingTip(CWnd* pParent /*=NULL*/)
	: CDialog(CBookingTip::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBookingTip)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBookingTip::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBookingTip)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX,IDC_LINK_BDYY,m_link_bdyy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBookingTip, CDialog)
	//{{AFX_MSG_MAP(CBookingTip)
	ON_BN_CLICKED(IDC_BUTTON_BOOKING, OnButtonBooking)
	ON_BN_CLICKED(IDC_LINK_BDYY, OnLinkBdyy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBookingTip message handlers

//DEL void CBookingTip::OnButtonBook() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	DestroyWindow();
//DEL 	//	dlg.SetParent(AfxGetMainWnd());
//DEL 	CMainFrame* main = (CMainFrame*)(theApp.GetMainWnd());
//DEL 	//CTabDockWnd dock = main->m_wndTabDockBar;
//DEL 	CWnd* win = main->m_wndTabDockBar.GetView(0);
//DEL 	if(win->IsKindOf(RUNTIME_CLASS(CDBList))){
//DEL 		((CDBList*)win)->GetBooking();
//DEL 	} 
//DEL }

void CBookingTip::OnButtonBooking() 
{
	// TODO: Add your control notification handler code here
	DestroyWindow();
	CMainFrame* main = (CMainFrame*)(theApp.GetMainWnd());
	CWnd* win = main->m_wndTabDockBar.GetView(0);
	if(win->IsKindOf(RUNTIME_CLASS(CDBList))){
		((CDBList*)win)->GetBooking();
 	} 
}

void CBookingTip::OnLinkBdyy() 
{
	// TODO: Add your control notification handler code here
	OnButtonBooking();
}

BOOL CBookingTip::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_link_bdyy.SetURL("");
	m_link_bdyy.SetUnderline(TRUE);
	m_link_bdyy.EnableShellExecute(FALSE);

	SetDlgItemText(IDC_STATIC_BDXXTIP,"ÓÐ"+count+"Ìõ¼ÇÂ¼");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
