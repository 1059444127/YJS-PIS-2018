// MicroWriterConfig.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "MicroWriterConfig.h"
#include "PathologyDoc.h"
#include "PathologyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMicroWriterConfig dialog


CMicroWriterConfig::CMicroWriterConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CMicroWriterConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMicroWriterConfig)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INI
}


void CMicroWriterConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMicroWriterConfig)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Text(pDX, IDC_COMBO_FontOneLine, m_FontOneLine);
	DDX_Text(pDX, IDC_COMBO_FontFirstLine, m_FontFirstLine);
	DDX_Text(pDX, IDC_COMBO_FontSecondLine, m_FontSecondLine);
	DDX_Text(pDX, IDC_EDIT_BeginIndex, m_BeginIndex);
	DDX_Text(pDX, IDC_EDIT_FirstLineText, m_FirstLineText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMicroWriterConfig, CDialog)
	//{{AFX_MSG_MAP(CMicroWriterConfig)
	ON_BN_CLICKED(IDC_RADIO_TwoLineMode, OnRADIOTwoLineMode)
	ON_BN_CLICKED(IDC_RADIO_OneLineMode, OnRADIOOneLineMode)
	ON_BN_CLICKED(IDC_BUTTON_MicroWriterPrint, OnBUTTONMicroWriterPrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMicroWriterConfig message handlers

extern CPathologyApp theApp;
void CMicroWriterConfig::OnRADIOTwoLineMode() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_COMBO_FontOneLine)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_FontFirstLine)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_FontSecondLine)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_FirstLineText)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_BeginIndex)->EnableWindow(TRUE);
}

void CMicroWriterConfig::OnRADIOOneLineMode() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_COMBO_FontOneLine)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_FontFirstLine)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_FontSecondLine)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_FirstLineText)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_BeginIndex)->EnableWindow(FALSE);
}

//DEL void CMicroWriterConfig::OnOK() 
//DEL {
//DEL 	// TODO: Add extra validation here
//DEL 	CString mno;
//DEL 	mno = theApp.GetView()->GetDocument()->m_blh;
//DEL 	if(mno.IsEmpty())
//DEL 	{
//DEL 		return;
//DEL 	}
//DEL 
//DEL 	if(((CButton*)GetDlgItem(IDC_RADIO_OneLineMode))->GetCheck() == 1)
//DEL 	{
//DEL 		m_IsOneLineMode = TRUE;
//DEL 		m_IsTwoLineMode = FALSE;
//DEL 	}
//DEL 
//DEL 	if(((CButton*)GetDlgItem(IDC_RADIO_TwoLineMode))->GetCheck() == 1)
//DEL 	{
//DEL 		m_IsOneLineMode = FALSE;
//DEL 		m_IsTwoLineMode = TRUE;
//DEL 	}
//DEL 	
//DEL 	CDialog::OnOK();
//DEL }

void CMicroWriterConfig::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CMicroWriterConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	((CComboBox*)GetDlgItem(IDC_COMBO_FontOneLine))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_FontFirstLine))->SetCurSel(2);
	((CComboBox*)GetDlgItem(IDC_COMBO_FontSecondLine))->SetCurSel(0);

	m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "MicroWriter.ini");
	CString mode = m_IniReader.getKeyValue("MicroWriterMode","parameters");
	if(mode.CompareNoCase("一行") == 0)
	{
		((CButton*)GetDlgItem(IDC_RADIO_OneLineMode))->SetCheck(1);
		OnRADIOOneLineMode();
	}
	else if(mode.CompareNoCase("两行") == 0)
	{
		((CButton*)GetDlgItem(IDC_RADIO_TwoLineMode))->SetCheck(1);
		OnRADIOTwoLineMode();
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_OneLineMode))->SetCheck(1);
		OnRADIOOneLineMode();	
	}

	m_FontOneLine = m_IniReader.getKeyValue("FontOneLine","parameters");
	m_FontFirstLine = m_IniReader.getKeyValue("FontFirstLine","parameters");
	m_FontSecondLine = m_IniReader.getKeyValue("FontSecondLine","parameters");
	m_BeginIndex = m_IniReader.getKeyValue("BeginIndex","parameters");
	m_FirstLineText = m_IniReader.getKeyValue("FirstLineText","parameters");
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CMicroWriterConfig::OnBUTTONMicroWriterPrint() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString mno;
	mno = theApp.GetView()->GetDocument()->m_blh;
	if(mno.IsEmpty())
	{
		return;
	}
	
	if(((CButton*)GetDlgItem(IDC_RADIO_OneLineMode))->GetCheck() == 1)
	{
		theApp.m_MicroWriterMode = "一行";
		theApp.m_FontOneLine = m_FontOneLine;
		m_IniReader.setKey(theApp.m_MicroWriterMode,"MicroWriterMode","parameters");
		m_IniReader.setKey(theApp.m_FontOneLine,"FontOneLine","parameters");
	}
	
	if(((CButton*)GetDlgItem(IDC_RADIO_TwoLineMode))->GetCheck() == 1)
	{
		theApp.m_MicroWriterMode = "两行";
		theApp.m_FontFirstLine = m_FontFirstLine;
		theApp.m_FontSecondLine = m_FontSecondLine;
		theApp.m_BeginIndex = m_BeginIndex;
		theApp.m_FirstLineText = m_FirstLineText;

		m_IniReader.setKey(theApp.m_MicroWriterMode,"MicroWriterMode","parameters");
		m_IniReader.setKey(theApp.m_FontFirstLine,"FontFirstLine","parameters");
		m_IniReader.setKey(theApp.m_FontSecondLine,"FontSecondLine","parameters");
		m_IniReader.setKey(theApp.m_BeginIndex,"BeginIndex","parameters");
		m_IniReader.setKey(theApp.m_FirstLineText,"FirstLineText","parameters");
	}
	OnOK();
}
