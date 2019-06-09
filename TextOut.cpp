// TextOut.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "TextOut.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextOut dialog


CTextOut::CTextOut(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CTextOut::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTextOut)
	m_Text = _T("");
	//}}AFX_DATA_INIT
	m_Title.Empty();
	m_isFirst = true;
	m_handle = 0;
}


void CTextOut::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextOut)
	DDX_Text(pDX, IDC_EDIT_INFO, m_Text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextOut, CXTResizeDialog)
	//{{AFX_MSG_MAP(CTextOut)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextOut message handlers

BOOL CTextOut::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetResize(IDC_EDIT_INFO, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	
	if(!m_Title.IsEmpty())
	{
		SetWindowText(m_Title);
	}

	m_handle = SetTimer(1, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTextOut::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_isFirst)
	{
		m_isFirst = false;
		CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_INFO);
		pEdit->SetFocus();
		pEdit->SetSel(-1,0,TRUE);
	}
}

void CTextOut::OnDestroy() 
{
	if(m_handle)
	{
		KillTimer(m_handle);
		m_handle = 0;
	}

	CXTResizeDialog::OnDestroy();
}

void CTextOut::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == m_handle)
	{
		POINT pt;
		GetCursorPos(&pt);
		CRect ct;
		GetWindowRect(ct);
		if(!ct.PtInRect(pt))
		{
			EndDialog(IDOK);
		}
	}
	
	CXTResizeDialog::OnTimer(nIDEvent);
}
