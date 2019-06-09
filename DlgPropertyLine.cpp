// DlgPropertyLine.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "DlgPropertyLine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPropertyLine dialog


CDlgPropertyLine::CDlgPropertyLine(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPropertyLine::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPropertyLine)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nLineType = PS_SOLID;
	m_nLineWidth = 1;
	m_crLineColor = RGB(0,0,0);
}


void CDlgPropertyLine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPropertyLine)
	DDX_Control(pDX, IDC_BUTTON_LINE_WIDTH, m_btnLineWidth);
	DDX_Control(pDX, IDC_BUTTON_LINE_TYPE, m_btnLineType);
	DDX_Control(pDX, IDC_BUTTON_LINE_COLOR, m_btnLineColor);
	//}}AFX_DATA_MAP
	DDX_FODropColorButton(pDX,IDC_BUTTON_LINE_COLOR,m_crLineColor);
	DDX_FODropLineTypeButton(pDX,IDC_BUTTON_LINE_TYPE,m_nLineType);
	DDX_FODropLineWidthButton(pDX,IDC_BUTTON_LINE_WIDTH,m_nLineWidth);
}


BEGIN_MESSAGE_MAP(CDlgPropertyLine, CDialog)
	//{{AFX_MSG_MAP(CDlgPropertyLine)
	ON_BN_CLICKED(IDHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPropertyLine message handlers

void CDlgPropertyLine::OnHelp() 
{
	//DWORD n = (DWORD) "/文件视窗操作/设置AOI线形.htm";
	//HWND res = HtmlHelp(m_hWnd, AfxGetApp()->m_pszHelpFilePath, HH_DISPLAY_TOPIC,n);
	
}
