//==============================================================================================
//	FILE      :	StreamPage.cpp
//	ABSTRACT  :	Stream tab class
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//
//	Copyright (c) 2005, Nikon Corp.  All rights reserved.
//==============================================================================================

#include "stdafx.h"
#include "Pathology.h"
#include "StreamPage.h"

#include "GetStudyUID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStreamPage Property Page

IMPLEMENT_DYNCREATE(CStreamPage, CPropertyPage)
extern CPathologyApp theApp;
//==============================================================================================
//	FUNCTION  :	CStreamPage
//	ABSTRACT  :	Constructor
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
CStreamPage::CStreamPage() : CPropertyPage(CStreamPage::IDD)
{
	//{{AFX_DATA_INIT(CStreamPage)
	m_iAppendix = -1;
	//}}AFX_DATA_INIT
	m_pCamera = theApp.GetCamera();
	m_pPreviewWnd = NULL;
	m_pEventSink = NULL;
	m_bCapturing = FALSE;
	m_pFileWriter = NULL;
}

//==============================================================================================
//	FUNCTION  :	~CStreamPage
//	ABSTRACT  :	Destructor
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
CStreamPage::~CStreamPage()
{
}

void CStreamPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStreamPage)
	DDX_Control(pDX, IDC_COMBO_QUALITY, m_cmbQuality);
	DDX_Control(pDX, IDC_COMBO_SHIFT, m_cmbShift);
	DDX_Control(pDX, IDC_COMBO_TRIGGER, m_clTrigger);
	DDX_Control(pDX, IDC_COMBO_MTLIST, m_MeteringAreaList);
	DDX_Control(pDX, IDC_COMBO_SHADINGLIST, m_ComboSaveShadingPosition);
	DDX_Control(pDX, IDC_EDIT_WINDOW, m_EditWindow);
	DDX_Control(pDX, IDC_COMBO_RENDER, m_ComboRender);
	DDX_Control(pDX, IDC_EDIT_FILEPATH, m_EditFilepath);
	DDX_Control(pDX, IDC_EDIT_BUFFERPATH, m_EditBufferpath);
	DDX_Control(pDX, IDC_EDIT_YFIELD, m_EditYField);
	DDX_Control(pDX, IDC_EDIT_XFIELD, m_EditXField);
	DDX_Control(pDX, IDC_COMBO_RESOLUTION, m_ComboResolution);
	DDX_Control(pDX, IDC_COMBO_FORMAT, m_ComboFormat);
	DDX_Control(pDX, IDC_COMBO_FIELD, m_ComboField);
	DDX_Radio(pDX, IDC_RADIO_APPADD, m_iAppendix);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStreamPage, CPropertyPage)
	//{{AFX_MSG_MAP(CStreamPage)
	ON_BN_CLICKED(IDC_BUTTON_SNAPSHOT, OnButtonSnapshot)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_WINDOWHANDLE, OnButtonWindowhandle)
	ON_BN_CLICKED(IDC_BUTTON_CONTINUOUSSHOT, OnButtonContinousshot)
	ON_BN_CLICKED(IDC_RADIO_APPADD, OnRadioAppAdd)
	ON_BN_CLICKED(IDC_RADIO_APPREMOVE, OnRadioAppRemove)
	ON_BN_CLICKED(IDC_BUTTON_FOCUSSET, OnButtonFocusSet)
	ON_BN_CLICKED(IDC_BUTTON_MTSET, OnButtonMtSet)
	ON_BN_CLICKED(IDC_BUTTON_SHADINGSET, OnButtonShadingSet)
	ON_BN_CLICKED(IDC_BUTTON_SNAPSHOTEX, OnButtonSnapshotEx)
	ON_BN_CLICKED(IDC_BUTTON_SHIFTSNAP, OnButtonShiftSnap)
	ON_BN_CLICKED(IDC_BUTTON_RAWSNAP, OnButtonRawSnapshot)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UM_CAPTURINGEVENT, OnCapturingEvent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStreamPage Message Handler

//==============================================================================================
//	FUNCTION  :	ChangeDevice
//	ABSTRACT  :	Set Camera 
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
void CStreamPage::ChangeDevice()
{
	m_ComboField.Disconnect();
	m_ComboFormat.Disconnect();
	m_ComboResolution.Disconnect();
	m_EditXField.Disconnect();
	m_EditYField.Disconnect();

	m_clTrigger.Disconnect();
}

//==============================================================================================
//	FUNCTION  :	OnSetActive
//	ABSTRACT  :	Change member conneted to camera
//	PARAMS    :	None
//	RETURN    :	TRUE
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
BOOL CStreamPage::OnSetActive() 
{
	try {
		ICameraController* pCamera = m_pCamera->GetInterface();
		IImageStreamPtr pImageStream = NULL;

		VARIANT val;
// ImageStream >>>
		if (FAILED(pCamera->get_ImageStream(&pImageStream)))
			return FALSE;

		if (SUCCEEDED(pImageStream->get_Field(&val)))
			m_ComboField.Connect(val.pdispVal);

		if (SUCCEEDED(pImageStream->get_Format(&val)))
			m_ComboFormat.Connect(val.pdispVal);

		if (SUCCEEDED(pImageStream->get_Resolution(&val)))
			m_ComboResolution.Connect(val.pdispVal);

		if (SUCCEEDED(pImageStream->get_XField(&val)))
			m_EditXField.Connect(val.pdispVal);

		if (SUCCEEDED(pImageStream->get_YField(&val)))
			m_EditYField.Connect(val.pdispVal);

		if (SUCCEEDED(pImageStream->get_TriggerMode(&val)))
			m_clTrigger.Connect(val.pdispVal);

		if (!m_pPreviewWnd)
			if (!CreatePreviewWindow(pImageStream))
				_com_raise_error(E_FAIL);

		//@ Default:Remove
		UpdateData(TRUE);
		m_iAppendix = 1;
		UpdateData(FALSE);

		SetFocusRange();
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		return FALSE;
	}
	
	return CPropertyPage::OnSetActive();
}

//==============================================================================================
//	FUNCTION  :	OnApply
//	ABSTRACT  :	Apply properties to Camera
//	PARAMS    :	None
//	RETURN    :	TRUE
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
BOOL CStreamPage::OnApply() 
{
	try {
		CWaitCursor wc;

		ICameraController* pCamera = m_pCamera->GetInterface();
		if (!m_pCamera->Lock())
			return FALSE;

		m_ComboField.PutValue();
		m_ComboFormat.PutValue();
		m_ComboResolution.PutValue();
		m_EditXField.PutValue();
		m_EditYField.PutValue();
		m_clTrigger.PutValue();

		SetFocusRange();

		m_pCamera->Unlock();
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		m_pCamera->Unlock();
		return FALSE;
	}
	
	return CPropertyPage::OnApply();
}

//==============================================================================================
//	FUNCTION  :	OnButtonSnapshot
//	ABSTRACT  :	SnapShot
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
void CStreamPage::OnButtonSnapshot() 
{
	char instanceUID[100];

	OnButtonStop();
	HRESULT hr;

	UpdateData(TRUE);
	UpdateData(FALSE);

	try {
		CWaitCursor wc;
		ICameraController* pCamera = m_pCamera->GetInterface();
		IImageStreamPtr pImageStream = NULL;

#ifndef _NO_ENABLE_CONTROL
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SNAPSHOTEX)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONTINUOUSSHOT)->EnableWindow(FALSE);
#endif //_NO_ENABLE_CONTROL
		if (!m_pCamera->Lock())
			goto fin;

		if (FAILED(hr = pCamera->get_ImageStream(&pImageStream)))
			_com_raise_error(hr);
	
		{
			CString str;
			BSTR bstr;
			m_EditFilepath.GetWindowText(str);
			bstr = str.AllocSysString();
			if (FAILED(hr = pImageStream->put_Path(bstr)))
				_com_raise_error(hr);
			SysFreeString(bstr);
		}

		if (!m_pEventSink) {
			IControllerDevice* pU2Device = NULL;
			if (FAILED(hr = pCamera->get_Device(&pU2Device)))
				_com_raise_error(hr);
			m_pEventSink = new CDeviceEventSink(pU2Device, this);
		}

	//	int nRender = m_ComboRender.GetCurSel();
		int nRender = 0;
		if (FAILED(hr = pImageStream->Snapshot((EnumRender)(nRender + 1))))
			_com_raise_error(hr);

		switch (nRender) {
		case 1:
		case 2:
		case 5:
		case 6:
			CString str;
			m_EditBufferpath.GetWindowText(str);
			m_pFileWriter = new CFileWriter(str);
			if (!m_pFileWriter)
				_com_raise_error(E_FAIL);

			BSTR Buffer = NULL;
			long nLength = 0;

			if (FAILED(hr = pImageStream->ReadBuffer(&nLength, &Buffer)))
				_com_raise_error(hr);

			if (!m_pFileWriter->Write((LPBYTE)Buffer, (DWORD)nLength))
				_com_raise_error(E_FAIL);

			//@ example of AddAppendix. 
			if(m_iAppendix==0){
				DWORD dwWidth = 0, dwHeight = 0;
				GetImageSize(dwWidth, dwHeight);

				long nPos = nLength - (nLength/(dwHeight+1));
				BYTE* pAppList = (LPBYTE)Buffer;

				DSCONTROLLERLib::TDSAppendixInformation* ptApp = (DSCONTROLLERLib::TDSAppendixInformation*)(pAppList+nPos);
				long item = ptApp->ExposeTime;
			}

			::SysFreeString(Buffer);

			delete m_pFileWriter;
			m_pFileWriter = NULL;
		}

		delete m_pEventSink;
		m_pEventSink = NULL;
		m_pCamera->Unlock();

	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		m_pCamera->Unlock();
	}
fin:
#ifndef _NO_ENABLE_CONTROL
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(!m_bCapturing);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(m_bCapturing);
//	GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(!m_bCapturing);
	GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SNAPSHOTEX)->EnableWindow(!m_bCapturing);
	GetDlgItem(IDC_BUTTON_CONTINUOUSSHOT)->EnableWindow(!m_bCapturing);
#else
	int n = 0;
#endif //_NO_ENABLE_CONTROL
	
	int maxno = 0;
	theApp.ConnectDatabase();
	try
	{
		SAString cmdstr = "Select serialno from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno order by serialno desc";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = m_blh;
		
		g_dbcommand.Execute();
		if(g_dbcommand.FetchNext())
		{
			maxno = g_dbcommand.Field("serialno").asLong();
		}
		
		g_dbconnection.Commit();
	}
	catch(SAException &x)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
		AfxMessageBox((const char*)x.ErrText());
		return;
	}
	CFile file; SAString imgdataset;
	file.Open("c:\\temp.bmp",CFile::modeRead | CFile::shareDenyNone,NULL);
	char sBuf[32*1024];
	while( 1 )
	{
		if( file.GetPosition() >= file.GetLength() )  break;
		DWORD nRead = file.ReadHuge(sBuf, sizeof(sBuf) );
		imgdataset += SAString( sBuf, nRead );
	}
	file.Close();

	dcmGenerateUniqueIdentifier(instanceUID, SITE_INSTANCE_UID_ROOT);

	try
	{
		SAString cmdstr = "Insert into " + theApp.TABLE_IMAGE + "(medicalno, instanceUID, serialno, imgdataset) values(:medicalno, :instanceUID, :serialno, :imgdataset)";
		
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = m_blh;
		g_dbcommand.Param("instanceUID").setAsString() = instanceUID;
		g_dbcommand.Param("serialno").setAsLong()    = ++maxno;
		g_dbcommand.Param("imgdataset").setAsBLob()  = imgdataset;
		
		g_dbcommand.Execute();
		g_dbconnection.Commit();
	}
	catch(SAException &x)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
		AfxMessageBox((const char*)x.ErrText());
	}
	
	imgdataset.Empty();
	OnButtonStart();
}

//==============================================================================================
//	FUNCTION  :	OnButtonStart
//	ABSTRACT  :	Start
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
void CStreamPage::OnButtonStart() 
{
	HRESULT hr;

	if (m_bCapturing)
		OnButtonStop();

	try {
		//int nRender = m_ComboRender.GetCurSel();
		int nRender = 3;
		ICameraController* pCamera = m_pCamera->GetInterface();
		IImageStreamPtr pImageStream = NULL;

#ifndef _NO_ENABLE_CONTROL
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SNAPSHOTEX)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONTINUOUSSHOT)->EnableWindow(FALSE);
#endif //_NO_ENABLE_CONTROL

		if (!m_pCamera->Lock())
			goto fin;

		if (FAILED(hr = pCamera->get_ImageStream(&pImageStream)))
			_com_raise_error(hr);
	
		{
			CString str;
			BSTR bstr;
			m_EditFilepath.GetWindowText(str);
			bstr = str.AllocSysString();
			if (FAILED(hr = pImageStream->put_Path(bstr)))
				_com_raise_error(hr);
			SysFreeString(bstr);
		}

		// enable buffer
		switch (nRender) {
		case 1:
		case 2:
		case 5:
		case 6:
			CString str;
			m_EditBufferpath.GetWindowText(str);
			m_pFileWriter = new CFileWriter(str);
			if (!m_pFileWriter)
				_com_raise_error(E_FAIL);
		}

		// Set _IDeviceEvents for receive OnNewImage Event and OnCaptureEvent
		if (!m_pEventSink) {
			IControllerDevice* pU2Device = NULL;
			if (FAILED(hr = pCamera->get_Device(&pU2Device)))
				_com_raise_error(hr);
			m_pEventSink = new CDeviceEventSink(pU2Device, this);
		}

		if(m_clTrigger.GetCurSel()!=0){
			//@ Trigger mode
			//@ In the case of a trigger, it is necessary to set RenderMode of the live to RenderBuffer.
			if (FAILED(hr = pImageStream->PrepareTriggerBuffer(1)))
				_com_raise_error(hr);
		}

		if (FAILED(hr = pImageStream->Start((EnumRender)(nRender + 1))))
			_com_raise_error(hr);

		m_bCapturing = TRUE;
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		m_pCamera->Unlock();
		if (m_pFileWriter) {
			delete m_pFileWriter;
			m_pFileWriter = NULL;
		}
	}
	catch (CFileException* e) {
		e->ReportError(MB_ICONEXCLAMATION);
		e->Delete();
		if (m_pFileWriter) {
			delete m_pFileWriter;
			m_pFileWriter = NULL;
		}
		m_pCamera->Unlock();
	}
fin:
#ifndef _NO_ENABLE_CONTROL
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(!m_bCapturing);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(m_bCapturing);
//	GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(!m_bCapturing);
	GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SNAPSHOTEX)->EnableWindow(!m_bCapturing);
#endif //_NO_ENABLE_CONTROL
	return;
}

//==============================================================================================
//	FUNCTION  :	OnButtonStop
//	ABSTRACT  :	Stop
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
void CStreamPage::OnButtonStop() 
{
	HRESULT hr;

	try {
		ICameraController* pCamera = m_pCamera->GetInterface();
		IImageStreamPtr pImageStream = NULL;

#ifndef _NO_ENABLE_CONTROL
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SNAPSHOTEX)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONTINUOUSSHOT)->EnableWindow(FALSE);
#endif //_NO_ENABLE_CONTROL

		if (FAILED(hr = pCamera->get_ImageStream(&pImageStream)))
			_com_raise_error(hr);

		if (FAILED(hr = pImageStream->Stop()))
			_com_raise_error(hr);

		m_bCapturing = FALSE;

		m_pCamera->Unlock();
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		m_pCamera->Unlock();
	}

	if (m_pEventSink) {
		delete m_pEventSink;
		m_pEventSink = NULL;
	}

	if (m_pFileWriter) {
		delete m_pFileWriter;
		m_pFileWriter = NULL;
	}

#ifndef _NO_ENABLE_CONTROL
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(!m_bCapturing);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(m_bCapturing);
	GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(!m_bCapturing);
	GetDlgItem(IDC_BUTTON_SNAPSHOTEX)->EnableWindow(!m_bCapturing);
#endif //_NO_ENABLE_CONTROL
	return;
}

//==============================================================================================
//	FUNCTION  :	CreatePreviewWindow
//	ABSTRACT  :	create preview window
//	PARAMS    :	pImageStream	a pointer to ImageStream interface
//	RETURN    :	TRUE
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
BOOL CStreamPage::CreatePreviewWindow(IImageStream* pImageStream)
{
    LPCTSTR             lpszClassName;
	DWORD           dwStyle, dwExStyle;
	HCURSOR         hCursor = 0;
	CRect           rectWindow,rectFrame;
	
	dwExStyle = 0;
	dwStyle = WS_CHILD | WS_VISIBLE;
	
	if (m_pPreviewWnd)
	{
		m_pPreviewWnd->DestroyWindow();
		delete m_pPreviewWnd;
		m_pPreviewWnd = NULL;
	}

	lpszClassName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, hCursor);
	
	m_pPreviewWnd = new CWnd;
	if (!m_pPreviewWnd) return FALSE;
	
	GetDlgItem(IDC_PICTURE_PREVIEW)->GetWindowRect(rectFrame);
	rectWindow.SetRect (2,2,rectFrame.Width()-4,rectFrame.Height()-4);

	if (!m_pPreviewWnd->CreateEx(dwExStyle, lpszClassName, TEXT("Preview"),dwStyle,
		rectWindow, GetDlgItem(IDC_PICTURE_PREVIEW),IDC_PICTURE_PREVIEW,NULL)) 
	{
		delete m_pPreviewWnd;
		m_pPreviewWnd = NULL;
		return FALSE;
	}

	if (pImageStream) {
		HRESULT hr;
		try
		{
			if (FAILED(hr = SetWindowHandle(pImageStream)))
				_com_raise_error(hr);
		}
		catch (_com_error &e) 
		{
			_CONTROLLER_ERRORMSG(e);
			return FALSE;
		}
	}

	m_pPreviewWnd->UpdateWindow();
	
	return TRUE;
}

//==============================================================================================
//	FUNCTION  :	OnDestroy
//	ABSTRACT  :	
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
void CStreamPage::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	if (m_pPreviewWnd) {
		m_pPreviewWnd->DestroyWindow();
		delete m_pPreviewWnd;
		m_pPreviewWnd = NULL;
	}
	if (m_bCapturing)
		OnButtonStop();
	if (m_pEventSink)
		delete m_pEventSink;
	if (m_pFileWriter)
		delete m_pFileWriter;
}

//==============================================================================================
//	FUNCTION  :	OnInitDialog
//	ABSTRACT  :	Initialize this class
//	PARAMS    :	None
//	RETURN    :	TRUE
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
BOOL CStreamPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// Metering Area
	m_MeteringAreaList.ResetContent();
	m_MeteringAreaList.AddString(TEXT("UserDataA"));
	m_MeteringAreaList.AddString(TEXT("UserDataB"));
	m_MeteringAreaList.AddString(TEXT("UserDataC"));
	m_MeteringAreaList.AddString(TEXT("UserDataD"));
	m_MeteringAreaList.AddString(TEXT("UserDataE"));
	m_MeteringAreaList.AddString(TEXT("UserDataF"));
	m_MeteringAreaList.AddString(TEXT("UserDataG"));
	m_MeteringAreaList.SetCurSel(0);

	// SaveShadingPosition
	m_ComboSaveShadingPosition.ResetContent();
	m_ComboSaveShadingPosition.AddString(TEXT("UserDataA"));
	m_ComboSaveShadingPosition.AddString(TEXT("UserDataB"));
	m_ComboSaveShadingPosition.AddString(TEXT("UserDataC"));
	m_ComboSaveShadingPosition.AddString(TEXT("UserDataD"));
	m_ComboSaveShadingPosition.AddString(TEXT("UserDataE"));
	m_ComboSaveShadingPosition.AddString(TEXT("UserDataF"));
	m_ComboSaveShadingPosition.AddString(TEXT("UserDataG"));
	m_ComboSaveShadingPosition.SetCurSel(0);
	
	// RenderCombo
	m_ComboRender.ResetContent();
	m_ComboRender.AddString(TEXT("RenderFile"));
	m_ComboRender.AddString(TEXT("RenderBuffer"));
	m_ComboRender.AddString(TEXT("RenderFileAndBuffer"));
	m_ComboRender.AddString(TEXT("RenderWindow"));
	m_ComboRender.AddString(TEXT("RenderFileAndWindow"));
	m_ComboRender.AddString(TEXT("RenderBufferAndWindow"));
	m_ComboRender.AddString(TEXT("RenderAll"));
	m_ComboRender.SetCurSel(3);

	// Ri1 CropMode
	m_cmbShift.ResetContent();
	m_cmbShift.AddString("ShiftModeFullFrame");
	m_cmbShift.AddString("ShiftModeView");
	m_cmbShift.AddString("ShiftModeRect");
	m_cmbShift.SetCurSel(0);

	// Ri1 CropMode
	m_cmbQuality.ResetContent();
	m_cmbQuality.AddString("QualityModeImage");
	m_cmbQuality.AddString("QualityModeSpeed");
	m_cmbQuality.SetCurSel(0);

	try {
		ICameraController* pCamera = m_pCamera->GetInterface();
		IControllerDevice* pU2Device = NULL;
		IImageStreamPtr pImageStream = NULL;

		if (FAILED(pCamera->get_Device(&pU2Device)))
			return FALSE;

		if (pU2Device) {
			long lVal;
// ImageStream >>>
			CComBSTR str;
			if (FAILED(pCamera->get_ImageStream(&pImageStream)))
				return FALSE;

			if (SUCCEEDED(pImageStream->get_Path(&str))) {
				m_EditFilepath.SetWindowText(CString(str));
			}

			if (SUCCEEDED(pImageStream->get_Window(&lVal))) {
				CString szTemp;
				szTemp.Format(TEXT("0x%08x"), (DWORD)lVal);
				m_EditWindow.SetWindowText(szTemp);
			}
		}
		m_EditWindow.EnableWindow(FALSE);

		{
			CSpinButtonCtrl *pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FOCUSX);
			pWnd->SetBuddy(GetDlgItem(IDC_EDIT_FOCUSX));
			pWnd->SetPos(0);

			pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FOCUSY);
			pWnd->SetBuddy(GetDlgItem(IDC_EDIT_FOCUSY));
			pWnd->SetPos(0);

			pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FOCUSW);
			pWnd->SetBuddy(GetDlgItem(IDC_EDIT_FOCUSW));
			pWnd->SetPos(0);

			pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FOCUSH);
			pWnd->SetBuddy(GetDlgItem(IDC_EDIT_FOCUSH));
			pWnd->SetPos(0);
		}

		{
			CSpinButtonCtrl *pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_MTX);
			pWnd->SetBuddy(GetDlgItem(IDC_EDIT_MTX));
			pWnd->SetPos(0);

			pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_MTY);
			pWnd->SetBuddy(GetDlgItem(IDC_EDIT_MTY));
			pWnd->SetPos(0);

			pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_MTW);
			pWnd->SetBuddy(GetDlgItem(IDC_EDIT_MTW));
			pWnd->SetPos(0);

			pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_MTH);
			pWnd->SetBuddy(GetDlgItem(IDC_EDIT_MTH));
			pWnd->SetPos(0);
		}
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		return FALSE;
	}
// 2005.2.15 debug code
m_EditFilepath.SetWindowText("c:\\temp.bmp");
m_EditBufferpath.SetWindowText("c:\\test.buf");

#ifndef _NO_ENABLE_CONTROL
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
#endif //_NO_ENABLE_CONTROL

	return TRUE;
}

//==============================================================================================
//	FUNCTION  :	OnNewImage
//	ABSTRACT  :	Save Buffer image
//	PARAMS    :	FrameNumber		Frame Number
//	RETURN    :	void
//	NOTE      :	may not call ReadBuffer to CameraController
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
void CStreamPage::OnNewImage(long FrameNumber)
{
	TRACE("Sink_OnNewImage FrameNumber=%ld\n", FrameNumber);

	if(FrameNumber==-1)
		return;	// Invalid image

	if (m_pFileWriter) {
		HRESULT hr;

		try {
			ICameraController* pCamera = m_pCamera->GetInterface();
			IImageStreamPtr pImageStream = NULL;
			BSTR Buffer = NULL;
			long nLength = 0;

			if (FAILED(hr = pCamera->get_ImageStream(&pImageStream)))
				_com_raise_error(hr);

			if(m_clTrigger.GetCurSel()==0){
				if (FAILED(hr = pImageStream->ReadBuffer(&nLength, &Buffer)))
					_com_raise_error(hr);

				//@ One line is added when AddAppendix is being called.
				//@ Refer to the OnButtonSnapshot for how to take out image information.
			}
			else{
				//@ Trigger out
				if(FrameNumber>-1){
					if (FAILED(hr = pImageStream->ReadTriggerBuffer(RenderWindow, 1, &nLength, &Buffer)))
						_com_raise_error(hr);
				}
			}

//			if (!m_pFileWriter->Write((LPBYTE)Buffer, (DWORD)nLength))
//				_com_raise_error(E_FAIL);

			::SysFreeString(Buffer);
		}
		catch (_com_error &e) {
			_CONTROLLER_ERRORMSG(e);
			return;
		}
	}
}

//==============================================================================================
//	FUNCTION  :	OnPaint
//	ABSTRACT  :	
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
void CStreamPage::OnPaint() 
{
	CPaintDC dc(this); // Device context
	
	HRESULT hr;
	try {
		if (!m_bCapturing){//Not capturing
		ICameraController* pCamera = m_pCamera->GetInterface();
		IImageStreamPtr pImageStream = NULL;

		if (FAILED(hr = pCamera->get_ImageStream(&pImageStream)))
			_com_raise_error(hr);
		if(pImageStream)
			pImageStream->RedrawImage();
		}
	}
	catch (_com_error &e) {
		OutputDebugString("OnPaint Error-->");
		OutputDebugString(e.ErrorMessage());
		OutputDebugString("<--OnPaint Error\n");
		return;
	}
}

//==============================================================================================
//	FUNCTION  :	OnButtonWindowhandle
//	ABSTRACT  :	SetWindowHandle
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
void CStreamPage::OnButtonWindowhandle() 
{
	HRESULT hr;
	try {
		ICameraController* pCamera = m_pCamera->GetInterface();
		IImageStreamPtr pImageStream = NULL;

		if (!m_pCamera->Lock())
			return;
		
		if (FAILED(hr = pCamera->get_ImageStream(&pImageStream)))
			_com_raise_error(hr);

		if ( pImageStream ) 
		{
			if (FAILED(hr = SetWindowHandle(pImageStream)))
				_com_raise_error(hr);
		}

		m_pCamera->Unlock();
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		m_pCamera->Unlock();
		return;
	}
}

//==============================================================================================
//	FUNCTION  :	SetWindowHandle
//	ABSTRACT  :	put_Window
//	PARAMS    :	None
//	RETURN    :	S_OK
//	NOTE      :	get_Window and Set Edit Box
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
HRESULT CStreamPage::SetWindowHandle(IImageStream* pImageStream)
{
	HRESULT hr = pImageStream->put_Window((long)m_pPreviewWnd->m_hWnd);
	if (FAILED(hr)) return hr;

	long lVal;
	if (SUCCEEDED(pImageStream->get_Window(&lVal))) {
		CString szTemp;
		szTemp.Format(TEXT("0x%08x"), (DWORD)lVal);
		m_EditWindow.SetWindowText(szTemp);
	}
	return hr;
}

//==============================================================================================
//	FUNCTION  :	OnCapturingEvent
//	ABSTRACT  :	Receive OnCapturing Event
//	PARAMS    :	wParam		not used
//				lParam		event number
//	RETURN    :	S_OK
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
HRESULT CStreamPage::OnCapturingEvent(WPARAM wParam, LPARAM lParam)
{
	long Occurred = (long)lParam;
	switch (Occurred)
	{
	case 0: // Image Stream Stop message called from Camera Device
		if (m_bCapturing) {
			m_bCapturing = FALSE;

			// Need Sleep
			Sleep(10);

			m_pCamera->Unlock();

			if (m_pFileWriter) {
				delete m_pFileWriter;
				m_pFileWriter = NULL;
			}
		}
#ifndef _NO_ENABLE_CONTROL
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(!m_bCapturing);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(m_bCapturing);
		GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(!m_bCapturing);
		GetDlgItem(IDC_BUTTON_SNAPSHOTEX)->EnableWindow(!m_bCapturing);
		GetDlgItem(IDC_BUTTON_CONTINUOUSSHOT)->EnableWindow(!m_bCapturing);
#endif //_NO_ENABLE_CONTROL
		break;
	}

	return 0;
}

void CStreamPage::OnButtonContinousshot() 
{
	HRESULT hr;
	BOOL bFlg;
	long lInterval = GetDlgItemInt(IDC_EDIT_INTERVAL,&bFlg,TRUE);
	long lCount = GetDlgItemInt(IDC_EDIT_COUNT,&bFlg,TRUE);
	BSTR Buffer = NULL;
	long nLength = 0;

	try {
		CWaitCursor wc;

		ICameraController* pCamera = m_pCamera->GetInterface();
		IImageStreamPtr pImageStream = NULL;

#ifndef _NO_ENABLE_CONTROL
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SNAPSHOTEX)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONTINUOUSSHOT)->EnableWindow(FALSE);
#endif //_NO_ENABLE_CONTROL
		if (!m_pCamera->Lock())
			goto fin;

		if (FAILED(hr = pCamera->get_ImageStream(&pImageStream)))
			_com_raise_error(hr);
	
		{
			CString str;
			BSTR bstr;
			m_EditFilepath.GetWindowText(str);
			bstr = str.AllocSysString();
			if (FAILED(hr = pImageStream->put_Path(bstr)))
				_com_raise_error(hr);
			SysFreeString(bstr);
		}

		if (!m_pEventSink) {
			IControllerDevice* pU2Device = NULL;
			if (FAILED(hr = pCamera->get_Device(&pU2Device)))
				_com_raise_error(hr);
			m_pEventSink = new CDeviceEventSink(pU2Device, this);
		}
		for( int i = 0; i < lCount; i++){
			if (FAILED(hr = pImageStream->Snapshot((EnumRender)(m_ComboRender.GetCurSel() + 1)))){
				_com_raise_error(hr);
/* Output temporary File
				Buffer = NULL;
				nLength = 0;
				if (FAILED(hr = pImageStream->ReadBuffer(&nLength, &Buffer)))
					_com_raise_error(hr);
				CString strName;
				strName.Format("C:\\temp\\temp_%d.raw",i);
				CFileWriter * pFileWriter = new CFileWriter((LPCTSTR)strName);
				if (pFileWriter) {
					if( pFileWriter->Write((LPBYTE)Buffer, (DWORD)nLength) != TRUE){
						OutputDebugString("Error in FileWriter\n");
				}
				delete pFileWriter;
				pFileWriter = NULL;
*/
			}
			Sleep(lInterval);
		}
//
		delete m_pEventSink;
		m_pEventSink = NULL;
		m_pCamera->Unlock();

	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		m_pCamera->Unlock();
	}
fin:
#ifndef _NO_ENABLE_CONTROL
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(!m_bCapturing);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(m_bCapturing);
	GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(!m_bCapturing);
	GetDlgItem(IDC_BUTTON_SNAPSHOTEX)->EnableWindow(!m_bCapturing);
	GetDlgItem(IDC_BUTTON_CONTINUOUSSHOT)->EnableWindow(!m_bCapturing);
#else
	int n = 0;
#endif //_NO_ENABLE_CONTROL
}

//==============================================================================================
//	FUNCTION  :	OnRadioAppAdd
//	ABSTRACT  :	AddAppendix
//	PARAMS    :	None
//	RETURN    :	None
//	NOTE      :	The appendix is added to the image. 
//	HISTORY   :	2005.11.18 Nikon Corp. - Created
//==============================================================================================
void CStreamPage::OnRadioAppAdd() 
{
	try {
		ICameraController* pCamera = m_pCamera->GetInterface();
		IImageStreamPtr pImageStream = NULL;

		if (FAILED(pCamera->get_ImageStream(&pImageStream)))
			return;

		pImageStream->AddAppendix();
		//@ The example of how to take out image information is in OnButtonSnapshot.
		//@ Please refer to.
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		return;
	}
}

//==============================================================================================
//	FUNCTION  :	OnRadioAppRemove
//	ABSTRACT  :	RemoveAppendix
//	PARAMS    :	None
//	RETURN    :	None
//	NOTE      :	The appendix is deleted from the image. 
//	HISTORY   :	2005.11.18 Nikon Corp. - Created
//==============================================================================================
void CStreamPage::OnRadioAppRemove() 
{
	try {
		ICameraController* pCamera = m_pCamera->GetInterface();
		IImageStreamPtr pImageStream = NULL;

		if (FAILED(pCamera->get_ImageStream(&pImageStream)))
			return;

		pImageStream->RemoveAppendix();
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		return;
	}
}

//==============================================================================================
//	FUNCTION  :	SetFocusRange
//	ABSTRACT  :	Initialize spin control
//	PARAMS    :	None
//	RETURN    :	None
//	NOTE      :	 
//	HISTORY   :	2005.11.18 Nikon Corp. - Created
//==============================================================================================
void CStreamPage::SetFocusRange()
{
	ICameraController* pCamera = m_pCamera->GetInterface();
	IImageStreamPtr pImageStream = NULL;

	try
	{
		if (FAILED(pCamera->get_ImageStream(&pImageStream)))
			return;

		CComVariant varParam;
		pImageStream->get_Resolution(&varParam);

		int iXLow, iXHigh;
		int iYLow, iYHigh;
		int iWLow, iWHigh;
		int iHLow, iHHigh;
		varParam.ChangeType(VT_I4);
		switch(varParam.lVal)
		{
		case SizeUnknown:
			iXLow	= 0;iXHigh	= 0;
			iYLow	= 0;iYHigh	= 0;
			iWLow	= 0;iWHigh	= 0;
			iHLow	= 0;iHHigh	= 0;
			break;
		case 	Size320x240:
			iXLow	= 0;iXHigh	= 319;
			iYLow	= 0;iYHigh	= 239;
			iWLow	= 0;iWHigh	= 320;
			iHLow	= 0;iHHigh	= 240;
			break;
		case 	Size640x480:
			iXLow	= 0;iXHigh	= 639;
			iYLow	= 0;iYHigh	= 479;
			iWLow	= 0;iWHigh	= 640;
			iHLow	= 0;iHHigh	= 480;
			break;
		case 	Size1280x960:
			iXLow	= 0;iXHigh	= 1279;
			iYLow	= 0;iYHigh	= 959;
			iWLow	= 0;iWHigh	= 1280;
			iHLow	= 0;iHHigh	= 960;
			break;
		case 	Size2560x1920:
			iXLow	= 0;iXHigh	= 2559;
			iYLow	= 0;iYHigh	= 1919;
			iWLow	= 0;iWHigh	= 2560;
			iHLow	= 0;iHHigh	= 1920;
			break;
		case 	Size1600x1200:
			iXLow	= 0;iXHigh	= 1599;
			iYLow	= 0;iYHigh	= 1199;
			iWLow	= 0;iWHigh	= 1600;
			iHLow	= 0;iHHigh	= 1200;
			break;
		case 	Size800x600:
			iXLow	= 0;iXHigh	= 799;
			iYLow	= 0;iYHigh	= 599;
			iWLow	= 0;iWHigh	= 800;
			iHLow	= 0;iHHigh	= 600;
			break;
		case 	Size800x560:
			iXLow	= 0;iXHigh	= 799;
			iYLow	= 0;iYHigh	= 559;
			iWLow	= 0;iWHigh	= 800;
			iHLow	= 0;iHHigh	= 560;
			break;
		case 	Size400x300:
			iXLow	= 0;iXHigh	= 399;
			iYLow	= 0;iYHigh	= 299;
			iWLow	= 0;iWHigh	= 400;
			iHLow	= 0;iHHigh	= 300;
			break;
		case 	Size400x280:
			iXLow	= 0;iXHigh	= 399;
			iYLow	= 0;iYHigh	= 279;
			iWLow	= 0;iWHigh	= 400;
			iHLow	= 0;iHHigh	= 280;
			break;
		}

// Focus >>>
		CSpinButtonCtrl *pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FOCUSX);
		pWnd->SetRange(iXLow,iXHigh);

		pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FOCUSY);
		pWnd->SetRange(iYLow,iYHigh);

		pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FOCUSW);
		pWnd->SetRange(iWLow,iWHigh);

		pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FOCUSH);
		pWnd->SetRange(iHLow,iHHigh);

// Metaring Area >>>
		pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_MTX);
		pWnd->SetRange(iXLow,iXHigh);

		pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_MTY);
		pWnd->SetRange(iYLow,iYHigh);

		pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_MTW);
		pWnd->SetRange(iWLow,iWHigh);

		pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_MTH);
		pWnd->SetRange(iHLow,iHHigh);
	}
	catch(_com_error &e)
	{
		_CONTROLLER_ERRORMSG(e);
		return;
	}
}

//==============================================================================================
//	FUNCTION  :	SetFocusRange
//	ABSTRACT  :	Focus Area Setting
//	PARAMS    :	None
//	RETURN    :	None
//	NOTE      :	 
//	HISTORY   :	2005.11.18 Nikon Corp. - Created
//==============================================================================================
void CStreamPage::OnButtonFocusSet() 
{
	HRESULT hr;
	try {
		CWaitCursor wc;

		ICameraController* pCamera = m_pCamera->GetInterface();
		if (!m_pCamera->Lock())
			return;

		CSpinButtonCtrl *pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FOCUSX);
		long nX = pWnd->GetPos();

		pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FOCUSY);
		long nY = pWnd->GetPos();

		pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FOCUSW);
		long nW = pWnd->GetPos();

		pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FOCUSH);
		long nH = pWnd->GetPos();

		if (FAILED(hr = pCamera->PresetFocusArea(nX,nY,nW,nH)))
			_com_raise_error(hr);

		m_pCamera->Unlock();
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		m_pCamera->Unlock();
		return;
	}
}

//==============================================================================================
//	FUNCTION  :	SetFocusRange
//	ABSTRACT  :	Metering Area Setting
//	PARAMS    :	None
//	RETURN    :	None
//	NOTE      :	 
//	HISTORY   :	2005.11.18 Nikon Corp. - Created
//==============================================================================================
void CStreamPage::OnButtonMtSet() 
{
	HRESULT hr;
	try {
		CWaitCursor wc;

		UpdateData(TRUE);
		UpdateData(FALSE);

		ICameraController* pCamera = m_pCamera->GetInterface();
		if (!m_pCamera->Lock())
			return;

		CSpinButtonCtrl *pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_MTX);
		long nX = pWnd->GetPos();

		pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_MTY);
		long nY = pWnd->GetPos();

		pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_MTW);
		long nW = pWnd->GetPos();

		pWnd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_MTH);
		long nH = pWnd->GetPos();

		CComPtr <IExposure> pExposure = NULL;
		if (FAILED(hr = pCamera->get_Exposure(&pExposure)))
			_com_raise_error(hr);
		if (FAILED(hr = pExposure->PresetMeteringArea(
								nX,
								nY,
								nW,
								nH)))
			_com_raise_error(hr);

		if (FAILED(hr = pExposure->put_MeteringArea(CComVariant(MeteringArea1))))
			_com_raise_error(hr);

		m_pCamera->Unlock();
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		m_pCamera->Unlock();
		return;
	}
}

//==============================================================================================
//	FUNCTION  :	SetFocusRange
//	ABSTRACT  :	Shading Data Setting
//	PARAMS    :	None
//	RETURN    :	None
//	NOTE      :	 
//	HISTORY   :	2005.11.18 Nikon Corp. - Created
//==============================================================================================
void CStreamPage::OnButtonShadingSet() 
{
	HRESULT hr;
	try {
		CWaitCursor wc;

		ICameraController* pCamera = m_pCamera->GetInterface();
		if (!m_pCamera->Lock())
			return;

		CComPtr<IEffects> pEffects = NULL;
		if (FAILED(hr = pCamera->get_Effects(&pEffects)))
			_com_raise_error(hr);
		if (FAILED(hr = pEffects->MemorizeShadingData()))
//			(EnumSaveUserData)m_ComboSaveShadingPosition.GetCurSel())))
			_com_raise_error(hr);

		m_pCamera->Unlock();
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		m_pCamera->Unlock();
		return;
	}
}

//==============================================================================================
//	FUNCTION  :	CStreamPage
//	ABSTRACT  :	OnButtonSnapshotEx
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.12.12 Nikon Corp. - Created
//==============================================================================================
void CStreamPage::OnButtonSnapshotEx() 
{
	// Live stop and set the camera settings beforehand.

	HRESULT hr = S_OK;

	long lSnapshotCount = 10;

	try {
		CWaitCursor wc;
		ICameraController* pCamera = m_pCamera->GetInterface();
		IImageStreamPtr pImageStream = NULL;

#ifndef _NO_ENABLE_CONTROL
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SNAPSHOTEX)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONTINUOUSSHOT)->EnableWindow(FALSE);
#endif //_NO_ENABLE_CONTROL
		if (!m_pCamera->Lock())
			goto fin;

		if (FAILED(hr = pCamera->get_ImageStream(&pImageStream)))
			_com_raise_error(hr);
	
		if (FAILED(hr = pImageStream->PrepareSnapshotExBuffer(lSnapshotCount)))
			_com_raise_error(hr);

		for( int iCount = 1; iCount <= lSnapshotCount ; iCount++ )
		{
			if (FAILED(hr = pImageStream->SnapshotEx(iCount, iCount+20)))
				_com_raise_error(hr);
		}

		DWORD dwWidth = 0, dwHeight = 0;
		GetImageSize(dwWidth, dwHeight);
		for( iCount = 1; iCount <= lSnapshotCount ; iCount++ )
		{
			CString str;
			str.Format("c:\\SnapshotEx%d.bmp", iCount);
			m_pFileWriter = new CFileWriter(str);
			if (!m_pFileWriter)
				_com_raise_error(E_FAIL);

			BSTR Buffer = NULL;
			long nLength = 0;

			if (FAILED(hr = pImageStream->ReadBufferEx(iCount, &nLength, &Buffer)))
				_com_raise_error(hr);

//			if (!m_pFileWriter->Write((LPBYTE)Buffer, (DWORD)nLength))
			if (!m_pFileWriter->WriteBMP((LPBYTE)Buffer, dwWidth, dwHeight))
				_com_raise_error(E_FAIL);

			::SysFreeString(Buffer);

			delete m_pFileWriter;
			m_pFileWriter = NULL;
		}

		m_pCamera->Unlock();

	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		m_pCamera->Unlock();
	}
fin:
#ifndef _NO_ENABLE_CONTROL
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(!m_bCapturing);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(m_bCapturing);
	GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(!m_bCapturing);
	GetDlgItem(IDC_BUTTON_SNAPSHOTEX)->EnableWindow(!m_bCapturing);
	GetDlgItem(IDC_BUTTON_CONTINUOUSSHOT)->EnableWindow(!m_bCapturing);
#else
	int n = 0;
#endif //_NO_ENABLE_CONTROL
}

void CStreamPage::GetImageSize(DWORD & dwWidth, DWORD & dwHeight)
{
	ICameraController* pCamera = m_pCamera->GetInterface();
	IImageStreamPtr pImageStream = NULL;

	try
	{
		if (FAILED(pCamera->get_ImageStream(&pImageStream)))
			return;

		CComVariant varParam;
		pImageStream->get_Resolution(&varParam);

		varParam.ChangeType(VT_I4);
		switch(varParam.lVal)
		{
		case SizeUnknown:
			dwWidth	= 0;dwHeight	= 0;
			break;
		case 	Size320x240:
			dwWidth	= 320;dwHeight	= 240;
			break;
		case 	Size640x480:
			dwWidth	= 640;dwHeight	= 480;
			break;
		case 	Size1280x960:
			dwWidth	= 1280;dwHeight	= 960;
			break;
		case 	Size2560x1920:
			dwWidth	= 2560;dwHeight	= 1920;
			break;
		case 	Size1600x1200:
			dwWidth	= 1600;dwHeight	= 1200;
			break;
		case 	Size800x600:
			dwWidth	= 800;dwHeight	= 600;
			break;
		case 	Size800x560:
			dwWidth	= 800;dwHeight	= 560;
			break;
		case 	Size400x300:
			dwWidth	= 400;dwHeight	= 300;
			break;
		case 	Size400x280:
			dwWidth	= 400;dwHeight	= 280;
			break;
		}
	}
	catch(_com_error &e)
	{
		_CONTROLLER_ERRORMSG(e);
		return;
	}
}

void CStreamPage::OnButtonShiftSnap() 
{
	HRESULT hr;

	UpdateData(TRUE);
	UpdateData(FALSE);

	try {
		CWaitCursor wc;
		if (m_bCapturing)
			OnButtonStop();

		ICameraController* pCamera = m_pCamera->GetInterface();
		IImageStreamPtr pImageStream = NULL;

#ifndef _NO_ENABLE_CONTROL
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SNAPSHOTEX)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SHIFTSNAP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONTINUOUSSHOT)->EnableWindow(FALSE);
#endif //_NO_ENABLE_CONTROL
		if (!m_pCamera->Lock())
			goto fin;

		if (FAILED(hr = pCamera->get_ImageStream(&pImageStream)))
			_com_raise_error(hr);
	
		{
			CString str;
			BSTR bstr;
			m_EditFilepath.GetWindowText(str);
			bstr = str.AllocSysString();
			if (FAILED(hr = pImageStream->put_Path(bstr)))
				_com_raise_error(hr);
			SysFreeString(bstr);
		}

		if (!m_pEventSink) {
			IControllerDevicePtr pU2Device = NULL;
			if (FAILED(hr = pCamera->get_Device(&pU2Device)))
				_com_raise_error(hr);
			m_pEventSink = new CDeviceEventSink(pU2Device, this);
		}

		int nRender = m_ComboRender.GetCurSel();

		m_bCapturing = TRUE;

		CSize size(0,0);
		CRect rect;
		rect.SetRectEmpty();
		EnumCropMode	eCropMode		= (EnumCropMode)m_cmbShift.GetCurSel();
		EnumQualityMode	eQualityMode	= (EnumQualityMode)m_cmbQuality.GetCurSel();

		// ShiftSnap
		if (FAILED(hr = pImageStream->ShiftSnap((EnumRender)(nRender + 1),eCropMode,24/*48*/,eQualityMode,StatusFalse,rect,&size))){
			_com_raise_error(hr);
		}
		HANDLE ShiftEndEvent = OpenEvent(EVENT_ALL_ACCESS,FALSE,"NIKON_RI1_SHIFTSNAP");
		::WaitForSingleObject(ShiftEndEvent, INFINITE);
		::CloseHandle(ShiftEndEvent);

		m_bCapturing = FALSE;

		switch (nRender) {
		case 1:
		case 2:
		case 5:
		case 6:
			CString str;
			m_EditBufferpath.GetWindowText(str);
			m_pFileWriter = new CFileWriter(str);
			if (!m_pFileWriter)
				_com_raise_error(E_FAIL);

			BSTR Buffer = NULL;
			long nLength = 0;

			if (FAILED(hr = pImageStream->ReadBuffer(&nLength, &Buffer)))
				_com_raise_error(hr);

			if (!m_pFileWriter->WriteBMP((LPBYTE)Buffer, size.cx, size.cy))
				_com_raise_error(E_FAIL);

			::SysFreeString(Buffer);

			delete m_pFileWriter;
			m_pFileWriter = NULL;
		}

		delete m_pEventSink;
		m_pEventSink = NULL;
		m_pCamera->Unlock();
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		m_pCamera->Unlock();
	}
fin:
#ifndef _NO_ENABLE_CONTROL
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(!m_bCapturing);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(!m_bCapturing);
	GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(!m_bCapturing);
	GetDlgItem(IDC_BUTTON_SNAPSHOTEX)->EnableWindow(!m_bCapturing);
	GetDlgItem(IDC_BUTTON_SHIFTSNAP)->EnableWindow(!m_bCapturing);
	GetDlgItem(IDC_BUTTON_CONTINUOUSSHOT)->EnableWindow(!m_bCapturing);
#endif //_NO_ENABLE_CONTROL
}

void CStreamPage::OnButtonRawSnapshot() 
{
	HRESULT hr;

	try {
		CWaitCursor wc;
		if (m_bCapturing)
			OnButtonStop();

		ICameraController* pCamera = m_pCamera->GetInterface();
		IImageStreamPtr pImageStream = NULL;

#ifndef _NO_ENABLE_CONTROL
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SNAPSHOTEX)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONTINUOUSSHOT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SHIFTSNAP)->EnableWindow(FALSE);
#endif //_NO_ENABLE_CONTROL
		if (!m_pCamera->Lock())
			goto fin;

		if (FAILED(hr = pCamera->get_ImageStream(&pImageStream)))
			_com_raise_error(hr);
	
		{
			CString str;
			BSTR bstr;
			m_EditFilepath.GetWindowText(str);
			bstr = str.AllocSysString();
			if (FAILED(hr = pImageStream->put_Path(bstr)))
				_com_raise_error(hr);
			SysFreeString(bstr);
		}

		if (!m_pEventSink) {
			IControllerDevicePtr pU2Device = NULL;
			if (FAILED(hr = pCamera->get_Device(&pU2Device)))
				_com_raise_error(hr);
			m_pEventSink = new CDeviceEventSink(pU2Device, this);
		}

		int nRender = m_ComboRender.GetCurSel();
		int nFormat = m_ComboFormat.GetCurSel();

		CSize size(0,0);
		CRect rect;
		rect.SetRectEmpty();
		EnumCropMode	eCropMode		= (EnumCropMode)m_cmbShift.GetCurSel();
		EnumQualityMode	eQualityMode	= (EnumQualityMode)m_cmbQuality.GetCurSel();

		if (FAILED(hr = pImageStream->RawSnapshot((EnumRender)(nRender + 1),eCropMode,24/*48*/,eQualityMode,StatusFalse,rect,&size)))
			_com_raise_error(hr);

		switch (nRender) {
		case 1:
		case 2:
		case 5:
		case 6:
			CString str;
			m_EditBufferpath.GetWindowText(str);
			m_pFileWriter = new CFileWriter(str);
			if (!m_pFileWriter)
				_com_raise_error(E_FAIL);

			BSTR Buffer = NULL;
			long nLength = 0;

			if (FAILED(hr = pImageStream->ReadBuffer(&nLength, &Buffer)))
				_com_raise_error(hr);

			if (!m_pFileWriter->WriteBMP((LPBYTE)Buffer, size.cx, size.cy))
				_com_raise_error(E_FAIL);

			::SysFreeString(Buffer);

			delete m_pFileWriter;
			m_pFileWriter = NULL;
		}

		delete m_pEventSink;
		m_pEventSink = NULL;
		m_pCamera->Unlock();

	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		m_pCamera->Unlock();
	}
fin:
#ifndef _NO_ENABLE_CONTROL
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(!m_bCapturing);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(m_bCapturing);
	GetDlgItem(IDC_BUTTON_SNAPSHOT)->EnableWindow(!m_bCapturing);
	GetDlgItem(IDC_BUTTON_SNAPSHOTEX)->EnableWindow(!m_bCapturing);
	GetDlgItem(IDC_BUTTON_CONTINUOUSSHOT)->EnableWindow(!m_bCapturing);
	GetDlgItem(IDC_BUTTON_SHIFTSNAP)->EnableWindow(!m_bCapturing);
#endif //_NO_ENABLE_CONTROL
}

void CStreamPage::start(){
	OnButtonStart();
}
