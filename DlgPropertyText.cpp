// DlgPropertyText.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "DlgPropertyText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPropertyText dialog

static int nFontSizes[] = 
	{8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 36, 48, 72,80,88,96,128,168,272,405,524,624};

CDlgPropertyText::CDlgPropertyText(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPropertyText::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPropertyText)
	m_bHorzAlign = 1;
	m_bVertAlign = 1;
	m_strContent = _T("");
	//}}AFX_DATA_INIT

	m_bMultiLine = TRUE;

	m_Font.m_strFaceName = "隶书";
	m_Font.m_nPointSize = 15;
	m_Font.m_nHeight = 10;
	m_Font.m_crColor = RGB(0,0,0);
	m_Font.m_bItalic = FALSE;
	m_Font.m_bUnderline = FALSE;
	m_Font.m_nWeight = FW_BOLD;
}


void CDlgPropertyText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPropertyText)
	DDX_Control(pDX, IDC_COMBO_FONTNAME, m_FontFaceName);
	DDX_Radio(pDX, IDC_LEFT, m_bHorzAlign);
	DDX_Radio(pDX, IDC_TOP, m_bVertAlign);
	DDX_Control(pDX,IDC_BUTTON_COLORTEXT,m_btnTextColor);
	DDX_Text(pDX, IDC_EDIT_CONTENT, m_strContent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPropertyText, CDialog)
	//{{AFX_MSG_MAP(CDlgPropertyText)
	ON_BN_CLICKED(IDC_CHK_MULTILINE, OnChkMultiline)
	ON_BN_CLICKED(IDC_CHK_FONTSTYLE_BOLD, OnChkFontstyleBold)
	ON_BN_CLICKED(IDC_CHK_FONTSTYLE_ITALIC, OnChkFontstyleItalic)
	ON_BN_CLICKED(IDC_CHK_FONTSTYLE_UNDERLINE, OnChkFontstyleUnderline)
	ON_BN_CLICKED(IDHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPropertyText message handlers
void CDlgPropertyText::InitFontSize()
{
	CComboBox *pBox = (CComboBox *)GetDlgItem(IDC_COMBO_FONTSIZE);
	// Clear the combobox.
	pBox->ResetContent();
	TCHAR buf[10];
	for (int i = 0; i < 25; i++)
	{
		wsprintf(buf, _T("%d"), nFontSizes[i]);
		pBox->AddString(buf);
	}
}


BOOL CDlgPropertyText::IsAllDigits(LPCTSTR lpStr)
{
	while (*lpStr != '\0')
	{
		if (!_istdigit(*lpStr++))
		{
			return FALSE;
		}
	}
	return TRUE;
}


void CDlgPropertyText::GetFontInfo(FO_FONT* pFontSet)
{
	pFontSet->m_strFaceName = m_Font.m_strFaceName;
	pFontSet->m_nPointSize = m_Font.m_nPointSize;
	pFontSet->m_crColor = m_Font.m_crColor;
	pFontSet->m_nHeight = m_Font.m_nHeight;
	pFontSet->m_bItalic = m_Font.m_bItalic;
	pFontSet->m_nWeight = m_Font.m_nWeight;
	pFontSet->m_bUnderline = m_Font.m_bUnderline;	
}
void CDlgPropertyText::SetFontInfo(FO_FONT *pFontSet)
{
	m_Font.m_strFaceName = pFontSet->m_strFaceName;
	m_Font.m_nPointSize = pFontSet->m_nPointSize;
	m_Font.m_crColor = pFontSet->m_crColor;
	m_Font.m_nHeight = pFontSet->m_nHeight;
	m_Font.m_bItalic = pFontSet->m_bItalic;
	m_Font.m_nWeight = pFontSet->m_nWeight;
	m_Font.m_bUnderline = pFontSet->m_bUnderline;	
}

/////////////////////////////////////////////////////////////////////////////
// CDlgPropertyText message handlers

BOOL CDlgPropertyText::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_FontFaceName.DoInit();
	m_FontFaceName.SelectString(-1,m_Font.m_strFaceName);

	InitFontSize();	
	CComboBox *pBox = (CComboBox *)GetDlgItem(IDC_COMBO_FONTSIZE);
	CString strTemp;
	strTemp.Format(_T("%u"),m_Font.m_nPointSize);
	pBox->SetWindowText(strTemp);

    m_btnTextColor.SetColor(m_Font.m_crColor);

	if(m_Font.m_bItalic)
	{
		((CButton*)GetDlgItem(IDC_CHK_FONTSTYLE_ITALIC))->SetCheck(TRUE);
	}
	if(m_Font.m_nWeight == FW_BOLD)
	{
		((CButton*)GetDlgItem(IDC_CHK_FONTSTYLE_BOLD))->SetCheck(TRUE);
	}
	if(m_Font.m_bUnderline)
	{
		((CButton*)GetDlgItem(IDC_CHK_FONTSTYLE_UNDERLINE))->SetCheck(TRUE);
	}
	
	if(m_bMultiLine)
	{
		GetDlgItem(IDC_TOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_MIDDLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BOTTOM)->EnableWindow(FALSE);
	}
	((CButton*)GetDlgItem(IDC_CHK_MULTILINE))->SetCheck(m_bMultiLine);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPropertyText::OnOK() 
{
    UpdateData();

	int nCur = m_FontFaceName.GetCurSel();
	if(nCur == -1)
	{
		AfxMessageBox(IDS_FO_MSG1,MB_OK|MB_ICONINFORMATION);
		m_FontFaceName.SetFocus();
		return;
	}
	CString strText;
	m_FontFaceName.GetLBText(nCur,strText);
	m_Font.m_strFaceName = strText;
	
	CComboBox *pBox = (CComboBox *)GetDlgItem(IDC_COMBO_FONTSIZE);
	strText;
	pBox->GetWindowText(strText);
	if(strText.IsEmpty())
	{
		AfxMessageBox(IDS_FO_MSG2,MB_OK|MB_ICONINFORMATION);
		pBox->SetFocus();
		return;
	}

	if(!IsAllDigits(strText))
	{
		AfxMessageBox(IDS_FO_MSG2,MB_OK|MB_ICONINFORMATION);
		pBox->SetFocus();
		return;
	}

	int nSize = _ttoi(strText);
	if(nSize < 0 || nSize > 1000)
	{
		AfxMessageBox(IDS_FO_MSG2,MB_OK|MB_ICONINFORMATION);
		pBox->SetFocus();
		return;
	}
	m_Font.m_nPointSize = nSize;

	m_Font.m_crColor = m_btnTextColor.GetColor();

	UpdateData(FALSE);
	
	CDialog::OnOK();
}

void CDlgPropertyText::OnChkMultiline() 
{
	m_bMultiLine = !m_bMultiLine;
	if(m_bMultiLine)
	{
		GetDlgItem(IDC_TOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_MIDDLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BOTTOM)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_MIDDLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BOTTOM)->EnableWindow(TRUE);
	}
	
}

void CDlgPropertyText::OnChkFontstyleBold() 
{
	if(m_Font.m_nWeight == FW_NORMAL)
	{
		m_Font.m_nWeight = FW_BOLD;
	}
	else
	{
		m_Font.m_nWeight = FW_NORMAL;
	}
	
}

void CDlgPropertyText::OnChkFontstyleItalic() 
{
	m_Font.m_bItalic = !m_Font.m_bItalic;	
}

void CDlgPropertyText::OnChkFontstyleUnderline() 
{
	m_Font.m_bUnderline = !m_Font.m_bUnderline;
}

void CDlgPropertyText::OnHelp() 
{
	//DWORD n = (DWORD) "/文件视窗操作/设置注记样式.htm";
	//HWND res = HtmlHelp(m_hWnd, AfxGetApp()->m_pszHelpFilePath, HH_DISPLAY_TOPIC,n);
	
}
