// StandInput.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "StandInput.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStandInput dialog
extern CPathologyApp theApp;


CStandInput::CStandInput(CWnd* pParent /*=NULL*/)
	: CDialog(CStandInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStandInput)
	m_zoom = 0;
	m_length = 0.0;
	//}}AFX_DATA_INIT
	m_PixelLength = 0.0;
}


void CStandInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStandInput)
	DDX_Text(pDX, IDC_EDIT_ZOOM, m_zoom);
	DDV_MinMaxInt(pDX, m_zoom, 1, 10000);
	DDX_Text(pDX, IDC_EDIT_LENGTH, m_length);
	DDV_MinMaxDouble(pDX, m_length, 0., 9999999.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStandInput, CDialog)
	//{{AFX_MSG_MAP(CStandInput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStandInput message handlers

BOOL CStandInput::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	((CComboBox *)GetDlgItem(IDC_COMBO_UNIT))->AddString("微米");
	((CComboBox *)GetDlgItem(IDC_COMBO_UNIT))->AddString("毫米");
	((CComboBox *)GetDlgItem(IDC_COMBO_UNIT))->AddString("厘米");
	((CComboBox *)GetDlgItem(IDC_COMBO_UNIT))->AddString("米");

	((CComboBox *)GetDlgItem(IDC_COMBO_UNIT))->SetCurSel(0);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStandInput::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if( m_PixelLength < 1 )
	{
		AfxMessageBox("像素定标长度过小！");
		return;
	}

	STANDARD_UNIT  m_newunit;
	m_newunit.value   = m_length / m_PixelLength;
	m_newunit.zoomlen = m_zoom;

	CString str;
	((CComboBox *)GetDlgItem(IDC_COMBO_UNIT))->GetWindowText(str);
	str.TrimLeft();  str.TrimRight();
	strcpy( m_newunit.unitname, str);

	STANDARD_UNIT * pUnitList = NULL;
	int num;

	CFile file;
	CFileException fe;
	if (file.Open(theApp.m_Exe_Working_Dir + "system/rulermark.dat", CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		num = file.GetLength() / sizeof(STANDARD_UNIT);
		pUnitList = new STANDARD_UNIT[num];
		for(int i = 0; i < num; i++ )
		{
			file.Read(&pUnitList[i], sizeof(STANDARD_UNIT));
		}
		file.Close();
	}

	if (file.Open(theApp.m_Exe_Working_Dir + "system/rulermark.dat", CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		for(int i = 0; i < num; i++ )
		{
			file.Write(&pUnitList[i], sizeof(STANDARD_UNIT));
		}
		file.Write(&m_newunit, sizeof(STANDARD_UNIT));
		file.Close();
	}

	if( pUnitList )
	{
		delete pUnitList;
		pUnitList = NULL;
	}

	str.Format("%3d倍镜: %8.4f ",m_newunit.zoomlen,m_newunit.value);
	str += m_newunit.unitname;
	((CMainFrame*)AfxGetMainWnd())->m_wndComboRulerMark.AddString(str);

	CDialog::OnOK();
}
