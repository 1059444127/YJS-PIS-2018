// DShowSetup.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "DShowSetup.h"
#include "MainFrm.h"
#include "INI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDShowSetup dialog
extern CPathologyApp theApp;

CDShowSetup::CDShowSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDShowSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDShowSetup)
	m_device = -1;
	m_format = -1;
	m_input = -1;
	//}}AFX_DATA_INIT
}


void CDShowSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDShowSetup)
	DDX_CBIndex(pDX, IDC_COMBO_DEVICE, m_device);
	DDX_CBIndex(pDX, IDC_COMBO_FORMAT, m_format);
	DDX_CBIndex(pDX, IDC_COMBO_VIDEOINPUT, m_input);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDShowSetup, CDialog)
	//{{AFX_MSG_MAP(CDShowSetup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDShowSetup message handlers

BOOL CDShowSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(theApp.m_CaptureMode.CompareNoCase("DirectShow采集设备支持") == 0 && ((CMainFrame *)AfxGetMainWnd())->m_VideoCap.GetSafeHwnd() )
	{
		int iDeviceCount = ((CMainFrame *)AfxGetMainWnd())->m_VideoCap.GetDevices().GetCount();
		CString str;
		
		for(int i = 0; i < iDeviceCount; i++)
		{
			str = ((CMainFrame *)AfxGetMainWnd())->m_VideoCap.GetDevices().FindDeviceName(i);
			((CComboBox *)GetDlgItem(IDC_COMBO_DEVICE))->AddString(str);
		}

		int iVideoFormatCount= ((CMainFrame *)AfxGetMainWnd())->m_VideoCap.GetVideoFormats().GetCount();
		
		for(i=0; i <iVideoFormatCount;i++)
		{
			str = ((CMainFrame *)AfxGetMainWnd())->m_VideoCap.GetVideoFormats().FindVideoFormatName(i);
			((CComboBox *)GetDlgItem(IDC_COMBO_FORMAT))->AddString(str);
		}
	
		int iVideoInputCount= ((CMainFrame *)AfxGetMainWnd())->m_VideoCap.GetVideoInputs().GetCount();
		
		for(i=0; i <iVideoInputCount;i++)
		{
			str = ((CMainFrame *)AfxGetMainWnd())->m_VideoCap.GetVideoInputs().FindVideoInputName(i);
			((CComboBox *)GetDlgItem(IDC_COMBO_VIDEOINPUT))->AddString(str);
		}
	}

	if(theApp.m_CaptureMode.CompareNoCase("微软DSHOW设备") == 0)
	{
		CIniReader m_IniReader;
		m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");

		CString str;
		str = m_IniReader.getKeyValue("DeviceNameList","微软DSHOW设备参数设置");
		while (str.Find(',') >= 0 && !str.IsEmpty()) {
            CString tmp = str.Left(str.Find(','));
			str = str.Right(str.GetLength() - str.Find(',') - 1);
			str.TrimLeft(); str.TrimRight();
			((CComboBox *)GetDlgItem(IDC_COMBO_DEVICE))->AddString(tmp);
		}

		str = m_IniReader.getKeyValue("VideoInputList","微软DSHOW设备参数设置");
		while (str.Find(',') >= 0 && !str.IsEmpty()) {
            CString tmp = str.Left(str.Find(','));
			str = str.Right(str.GetLength() - str.Find(',') - 1);
			str.TrimLeft(); str.TrimRight();
			((CComboBox *)GetDlgItem(IDC_COMBO_VIDEOINPUT))->AddString(tmp);
		}
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDShowSetup::OnOK() 
{
	UpdateData(TRUE);
	
	CDialog::OnOK();
}
