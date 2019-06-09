//==============================================================================================
//	FILE      :	DevicesPage.cpp
//	ABSTRACT  :	Handles interactions with the CameraController object for the property page 
//			hosted by the Devices Tab
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//
//	Copyright (c) 2005, Nikon Corp.  All rights reserved.
//==============================================================================================

#include "stdafx.h"
#include "Pathology.h"
#include "DevicesPage.h"
#include "dscontrollermainsheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDevicesPage PropertyPage

IMPLEMENT_DYNCREATE(CDevicesPage, CPropertyPage)

extern CPathologyApp theApp;
//==============================================================================================
//	FUNCTION  :	CDevicesPage
//	ABSTRACT  :	Constructor
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
CDevicesPage::CDevicesPage() : CPropertyPage(CDevicesPage::IDD)
{
	//{{AFX_DATA_INIT(CDevicesPage)
	m_cszCameraType = _T("");
	//}}AFX_DATA_INIT
	m_pCamera = theApp.GetCamera();
	m_bDirty = FALSE;
	m_ChangeFlg = FALSE;
}

//==============================================================================================
//	FUNCTION  :	~CDevicesPage
//	ABSTRACT  :	Destructor
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
CDevicesPage::~CDevicesPage()
{
}

void CDevicesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDevicesPage)
	DDX_Control(pDX, IDC_COMBO_CAMERATEMP, m_clCameraTemp);
	DDX_Control(pDX, IDC_EDIT_ROIPOSITION, m_ROIPosition);
	DDX_Control(pDX, IDC_EDIT_VERSION, m_EditVersion);
	DDX_Control(pDX, IDC_EDIT_NAME, m_EditName);
	DDX_Control(pDX, IDC_COMBO_MODEL, m_ComboModel);
	DDX_Control(pDX, IDC_COMBO_SCANMODE, m_ComboScanMode);
	DDX_Control(pDX, IDC_COMBO_DEVICES, m_ComboDevices);
	DDX_Text(pDX, IDC_EDIT_CAMERATYPE, m_cszCameraType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDevicesPage, CPropertyPage)
	//{{AFX_MSG_MAP(CDevicesPage)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICES, OnSelchangeComboDevices)
	ON_EN_CHANGE(IDC_EDIT_NAME, OnChangeEditName)
	ON_BN_CLICKED(IDC_RefreshList, OnRefreshList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDevicesPage Message Handler

//==============================================================================================
//	FUNCTION  :	OnInitDialog
//	ABSTRACT  :	Initialization of this window
//	PARAMS    :	None
//	RETURN    :	TRUE
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
BOOL CDevicesPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// get a list of available cameras
	CStringList slistDevice;
	if (FAILED(m_pCamera->GetDevices(slistDevice))) {
		EndDialog(-1);
		return TRUE;
	}
	POSITION pos = slistDevice.GetHeadPosition();
	if (!pos) {
		AfxMessageBox(TEXT("No Device."), MB_ICONSTOP);
		EndDialog(-1);
		return TRUE;
	}
	while (pos) {
		m_ComboDevices.AddString(slistDevice.GetNext(pos));
	}
	m_ComboDevices.SetCurSel(1);
	m_pCamera->SetDevice(1); // select the simulation device by default

	return TRUE;  // return TRUE if this window does not have input focus
	              // Can return FALSE if using an OCX Property Page
}
//==============================================================================================
//	FUNCTION  :	ChangeDevice
//	ABSTRACT  :	Refresh GUI controls when the connected camera changes
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	Called by PropertySheet
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
void CDevicesPage::ChangeDevice()
{
	m_ComboModel.Disconnect();
	m_ComboScanMode.Disconnect();
	m_ROIPosition.Disconnect();
	m_clCameraTemp.Disconnect();
}

//==============================================================================================
//	FUNCTION  :	OnSelchangeComboDevices
//	ABSTRACT  :	Respond to a change in the currently selected camera
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	Notify PropertySheet
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
void CDevicesPage::OnSelchangeComboDevices() 
{
	((CDsControllerMainSheet*)GetParent())->ChangeDevice();
	m_pCamera->SetDevice(m_ComboDevices.GetCurSel());
	OnSetActive();
}

//==============================================================================================
//	FUNCTION  :	OnApply
//	ABSTRACT  :	Set camera parameters based on the current selections in property pages
//	PARAMS    :	None
//	RETURN    :	TRUE
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
BOOL CDevicesPage::OnApply() 
{
	HRESULT hr;

	try {
		CWaitCursor wc;

		ICameraController* pCamera = m_pCamera->GetInterface();
		if (!m_pCamera->Lock())
			return FALSE;

		if (m_bDirty) {
			CString szName;
			BSTR	bstrName;
			m_EditName.GetWindowText( szName );
			bstrName = szName.AllocSysString();
			if( FAILED( hr = pCamera->put_ControllerName( bstrName ) ) )
				_com_raise_error(hr);
			SysFreeString( bstrName );

			m_bDirty = FALSE;
			m_ChangeFlg = FALSE;
			SetModified( FALSE );
		}
		m_ComboScanMode.PutValue();
		m_ROIPosition.PutValue();
		m_clCameraTemp.PutValue();

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
//	FUNCTION  :	OnSetActive
//	ABSTRACT  :	When this tab is selected, intialize controls by reading camera parameters
//	PARAMS    :	None
//	RETURN    :	TRUE
//	NOTE      :	Binds MipParameter objects to camera parameters using the 'Connect' function
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
BOOL CDevicesPage::OnSetActive() 
{
	try {
		ICameraController* pCamera = m_pCamera->GetInterface();

		CComVariant val;
		CComBSTR	bstr;
				
		m_ChangeFlg = FALSE;

		if( SUCCEEDED( pCamera->get_Model( &val ) ) )
			m_ComboModel.Connect( val.pdispVal );

		{
			UpdateData(TRUE);
			CComPtr<IMipParameter> spiParam;
			val.pdispVal->QueryInterface(&spiParam);

			CComVariant varModel;
			spiParam->get_RawValue(&varModel);
			switch(varModel.lVal)
			{
			case ModelUnknown:
				m_cszCameraType = "Unknown";
				break;
			case ModelDS5MU2:
			case ModelDS5ML2:
			case ModelDS2MvU2:
			case ModelDS2MvL2:
			case ModelDS2MBWU2:
			case ModelDS2MBWL2:
			case ModelFi1U2:
			case ModelFi1L2:
				m_cszCameraType = "Normal";
				break;
			case ModelDS5McU2:
			case ModelDS5McL2:
			case ModelDS2MBWcU2:
			case ModelDS2MBWcL2:
			case ModelQi1McU2:
			case ModelQi1McL2:
			case ModelRi1cU2:
			case ModelRi1cL2:
				m_cszCameraType = "Cooled";
				break;
			}
			UpdateData(FALSE);
		}

		if(m_ComboDevices.GetCurSel()>0){
			if(m_cszCameraType.Compare("Unknown")==0){
				MessageBox("Invalid camera type.",NULL,MB_OK|MB_ICONERROR);
				m_ComboDevices.SetCurSel(0);
				m_pCamera->SetDevice(0);
				return FALSE;
			}
		}

		if( SUCCEEDED( pCamera->get_Version( &bstr ) ) )
			m_EditVersion.SetWindowText( ( CString )bstr );
		m_EditVersion.EnableWindow( FALSE );

		if( SUCCEEDED( pCamera->get_ControllerName( &bstr ) ) )
		{
			if (!m_bDirty)
				m_EditName.SetWindowText( ( CString )bstr ); 
		}

		val.Clear();

		if( SUCCEEDED( pCamera->get_ScanMode( &val ) ) )
			m_ComboScanMode.Connect( val.pdispVal );

		val.Clear();

		if (SUCCEEDED(pCamera->get_ROIPosition(&val)))
			m_ROIPosition.Connect(val.pdispVal);

		val.Clear();

		if (SUCCEEDED(pCamera->get_CoolingMode(&val)))
			m_clCameraTemp.Connect(val.pdispVal);

		val.Clear();
	}
	catch( _com_error &e ) {
		_CONTROLLER_ERRORMSG(e);
		m_ChangeFlg = TRUE;
		return FALSE;
	}
	m_ChangeFlg = TRUE;

	return CPropertyPage::OnSetActive();
}

//==============================================================================================
//	FUNCTION  :	OnChangeEditName
//	ABSTRACT  :	Handles changes to the user-defined camera name
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
void CDevicesPage::OnChangeEditName() 
{
	if( m_ChangeFlg )
	{
		if( !m_bDirty )
		{
			SetModified(TRUE);
			m_bDirty = TRUE;
		}
	}
}

//==============================================================================================
//	FUNCTION  :	OnRefreshList
//	ABSTRACT  :	Refreshes the list of available cameras
//	PARAMS    :	None
//	RETURN    :	S_OK
//	NOTE      :	The list of USB devices can change at runtime
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
void CDevicesPage::OnRefreshList() 
{
	CStringList slistDevice;
	if (FAILED(m_pCamera->RefreshDevices(slistDevice))) {
//		EndDialog(-1);
		return;
	}
	POSITION pos = slistDevice.GetHeadPosition();
	if (!pos) {
		AfxMessageBox(TEXT("No Device."), MB_ICONSTOP);
//		EndDialog(-1);
		return;
	}
	m_ComboDevices.ResetContent();
	while (pos) {
		m_ComboDevices.AddString(slistDevice.GetNext(pos));
	}
}
