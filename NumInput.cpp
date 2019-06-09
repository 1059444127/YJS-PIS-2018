// NumInput.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "NumInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NumInput dialog

NumInput::NumInput(CWnd* pParent /*=NULL*/)
	: CDialog(NumInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(NumInput)
	m_Data = "";
	//}}AFX_DATA_INIT
}

NumInput::NumInput(int left,int top,CWnd* pParent /*=NULL*/)
	: CDialog(NumInput::IDD, pParent)
{
	m_Data = "";
	m_Left=left;m_top=top;
}

void NumInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NumInput)
	DDX_Text(pDX, IDC_EDIT1, m_Data);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(NumInput, CDialog)
	//{{AFX_MSG_MAP(NumInput)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// NumInput message handlers

BOOL NumInput::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect,rect1;
	GetWindowRect(&rect);	                       
	rect1.left=m_Left;rect1.top=m_top-2;
	rect1.right=rect1.left+rect.Width();
	rect1.bottom=rect1.top+rect.Height();
	GetParent()->ClientToScreen(&rect1);
	SetWindowPos(NULL, rect1.left, rect1.top ,rect1.Width(),rect1.Height(),SWP_NOZORDER);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void NumInput::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDown(nFlags, point);
}

void NumInput::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}
