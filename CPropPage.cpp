//==============================================================================================
//	FILE      :	PropPage.cpp
//	ABSTRACT  :	Property Page Class
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//
//	Copyright (c) 2005, Nikon Corp.  All rights reserved.
//==============================================================================================

#include "stdafx.h"
#include "Pathology.h"
#include "PropPage.h"
#include "IO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPage Property Page
extern CPathologyApp theApp;

IMPLEMENT_DYNCREATE(CPropPage, CPropertyPage)

//==============================================================================================
//	FUNCTION  :	CPropPage
//	ABSTRACT  :	Constructor
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	Initializes m_pCamera
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
CPropPage::CPropPage() : CPropertyPage(CPropPage::IDD)
{
	//{{AFX_DATA_INIT(CPropPage)
	//}}AFX_DATA_INIT
//	m_pCamera = ((theApp*)AfxGetApp())->GetCamera();
	m_pCamera = theApp.GetCamera();
	m_pBag = NULL;
}

//==============================================================================================
//	FUNCTION  :	~CPropPage
//	ABSTRACT  :	Destructor
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	Releases the Property Bag for camera settings
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
CPropPage::~CPropPage()
{
	if (m_pBag)
		m_pBag->Release();
}

void CPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPage)
	DDX_Control(pDX, IDC_EDIT_TIME, m_EditDetailTime);
	DDX_Control(pDX, IDC_COMBO_PRESET2, m_ComboPreset);
	DDX_Control(pDX, IDC_EDIT_TONEFILE, m_ToneFilePath);
	DDX_Control(pDX, IDC_COMBO_TONELIST, m_ToneList);
	DDX_Control(pDX, IDC_EDIT_DTGAIN, m_DtGain);
	DDX_Control(pDX, IDC_EDIT_BLUEBALANCE, m_BlueBalance);
	DDX_Control(pDX, IDC_EDIT_GREENBALANCE, m_GreenBalance);
	DDX_Control(pDX, IDC_EDIT_REDBALANCE, m_RedBalance);
	DDX_Control(pDX, IDC_COMBO_ISSUSPENDED, m_ComboIsSuspended);
	DDX_Control(pDX, IDC_EDIT_CONTRAST, m_EditContrast);
	DDX_Control(pDX, IDC_EDIT_LEVELSGAIN, m_EditLevelsGain);
	DDX_Control(pDX, IDC_COMBO_SAVESHADINGPOSITION, m_ComboSaveShadingPosition);
	DDX_Control(pDX, IDC_EDIT_TONE, m_EditTone);
	DDX_Control(pDX, IDC_EDIT_HUE, m_EditHue);
	DDX_Control(pDX, IDC_EDIT_SATURATION, m_EditSaturation);
	DDX_Control(pDX, IDC_EDIT_BLUESATURATION, m_EditBlueSaturation);
	DDX_Control(pDX, IDC_EDIT_GREENSATURATION, m_EditGreenSaturation);
	DDX_Control(pDX, IDC_EDIT_REDSATURATION, m_EditRedSaturation);
	DDX_Control(pDX, IDC_EDIT_BLUEGAIN, m_EditBlueGain);
	DDX_Control(pDX, IDC_EDIT_GREENGAIN, m_EditGreenGain);
	DDX_Control(pDX, IDC_EDIT_REDGAIN, m_EditRedGain);
	DDX_Control(pDX, IDC_EDIT_SHARPNESS, m_EditSharpness);
	DDX_Control(pDX, IDC_EDIT_SHADINGCORRECTION, m_EditShadingCorrection);
	DDX_Control(pDX, IDC_COMBO_NOISEREDUCTION, m_ComboNoiseReduction);
	DDX_Control(pDX, IDC_COMBO_COLORCAST, m_ComboColorCast);
	DDX_Control(pDX, IDC_EDIT_PRESET, m_EditPreset);
	DDX_Control(pDX, IDC_COMBO_METERING2, m_ComboMetering2);
	DDX_Control(pDX, IDC_COMBO_METERING1, m_ComboMetering1);
	DDX_Control(pDX, IDC_COMBO_LOCKED, m_ComboLocked);
	DDX_Control(pDX, IDC_COMBO_MODE, m_ComboMode);
	DDX_Control(pDX, IDC_EDIT_GAIN, m_EditGain);
	DDX_Control(pDX, IDC_EDIT_BIAS, m_EditBias);
	DDX_Control(pDX, IDC_COMBO_TIME, m_ComboTime);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPage)
	ON_BN_CLICKED(IDC_BUTTON_PROPLOAD, OnButtonPropload)
	ON_BN_CLICKED(IDC_BUTTON_PROPSAVE, OnButtonPropsave)
	ON_BN_CLICKED(IDC_BUTTON_BALANCE, OnButtonBalance)
	ON_BN_CLICKED(IDC_BUTTON_SHADINGDATACAPTURE, OnButtonShadingdatacapture)
	ON_BN_CLICKED(IDC_BUTTON_TONESET, OnButtonToneSet)
	ON_BN_CLICKED(IDC_ONEPUSHAE, OnOnePushAE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPage message handler

//==============================================================================================
//	FUNCTION  :	OnSetActive
//	ABSTRACT  :	When this tab is selected, intialize controls by reading camera parameters
//	PARAMS    :	None
//	RETURN    :	TRUE
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
BOOL CPropPage::OnSetActive() 
{
	try {
		ICameraController* pCamera = m_pCamera->GetInterface();
		CComPtr <IExposure> pExposure = NULL;
		CComPtr <ILevels> pLevels = NULL;
		CComPtr <IEffects> pEffects = NULL;

		VARIANT val;
// Exposure >>>
		if (FAILED(pCamera->get_Exposure(&pExposure)))
			return FALSE;

		if (SUCCEEDED(pExposure->get_IsSuspended(&val)))
			m_ComboIsSuspended.Connect(val.pdispVal);

		if (SUCCEEDED(pExposure->get_Mode(&val)))
			m_ComboMode.Connect(val.pdispVal);

		if (SUCCEEDED(pExposure->get_MeteringMode(&val)))
			m_ComboMetering1.Connect(val.pdispVal);

		if (SUCCEEDED(pExposure->get_Bias(&val)))
			m_EditBias.Connect(val.pdispVal);

//		if (SUCCEEDED(pExposure->get_Time(&val)))
//			m_EditTime.Connect(val.pdispVal);
		if (SUCCEEDED(pExposure->get_Time(&val)))
			m_ComboTime.Connect(val.pdispVal);

		if (SUCCEEDED(pExposure->get_Gain(&val)))
			m_EditGain.Connect(val.pdispVal);

		if (SUCCEEDED(pExposure->get_IsLocked(&val)))
			m_ComboLocked.Connect(val.pdispVal);

//		if (SUCCEEDED(pExposure->get_Preset(&val)))
//			m_EditPreset.Connect(val.pdispVal);

		if (SUCCEEDED(pExposure->get_MeteringArea(&val)))
			m_ComboMetering2.Connect(val.pdispVal);

		if (SUCCEEDED(pExposure->get_DetailedGain(&val)))
			m_DtGain.Connect(val.pdispVal);

		if (SUCCEEDED(pExposure->get_DetailedTime(&val)))
			m_EditDetailTime.Connect(val.pdispVal);

		if (SUCCEEDED(pExposure->get_Preset(&val)))
			m_ComboPreset.Connect(val.pdispVal);
// Exposure <<<
// Levels >>>
		if (FAILED(pCamera->get_Levels(&pLevels)))
			return FALSE;

		if (SUCCEEDED(pLevels->get_RedGain(&val)))
			m_EditRedGain.Connect(val.pdispVal);
		
		if (SUCCEEDED(pLevels->get_GreenGain(&val)))
			m_EditGreenGain.Connect(val.pdispVal);

		if (SUCCEEDED(pLevels->get_BlueGain(&val)))
			m_EditBlueGain.Connect(val.pdispVal);

		if (SUCCEEDED(pLevels->get_Gain(&val)))
			m_EditLevelsGain.Connect(val.pdispVal);

		if (SUCCEEDED(pLevels->get_RedSaturation(&val)))
			m_EditRedSaturation.Connect(val.pdispVal);

		if (SUCCEEDED(pLevels->get_GreenSaturation(&val)))
			m_EditGreenSaturation.Connect(val.pdispVal);

		if (SUCCEEDED(pLevels->get_BlueSaturation(&val)))
			m_EditBlueSaturation.Connect(val.pdispVal);

		if (SUCCEEDED(pLevels->get_Saturation(&val)))
			m_EditSaturation.Connect(val.pdispVal);

		if (SUCCEEDED(pLevels->get_Hue(&val)))
			m_EditHue.Connect(val.pdispVal);

		if (SUCCEEDED(pLevels->get_Contrast(&val)))
			m_EditContrast.Connect(val.pdispVal);

		if (SUCCEEDED(pLevels->get_Tone(&val)))
			m_EditTone.Connect(val.pdispVal);

		if (SUCCEEDED(pLevels->get_RedBalance(&val)))
			m_RedBalance.Connect(val.pdispVal);

		if (SUCCEEDED(pLevels->get_GreenBalance(&val)))
			m_GreenBalance.Connect(val.pdispVal);

		if (SUCCEEDED(pLevels->get_BlueBalance(&val)))
			m_BlueBalance.Connect(val.pdispVal);
// Levels <<<
// Effects >>>
		if (FAILED(pCamera->get_Effects(&pEffects)))
			return FALSE;
		
		if (SUCCEEDED(pEffects->get_ColorCast(&val)))
			m_ComboColorCast.Connect(val.pdispVal);

		if (SUCCEEDED(pEffects->get_NoiseReductionEnabled(&val)))
			m_ComboNoiseReduction.Connect(val.pdispVal);

		if (SUCCEEDED(pEffects->get_Sharpness(&val)))
			m_EditSharpness.Connect(val.pdispVal);

		if (SUCCEEDED(pEffects->get_ShadingCorrection(&val)))
			m_EditShadingCorrection.Connect(val.pdispVal);
// Effects <<<
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		return FALSE;
	}

	if (!m_pBag)
		GetDlgItem(IDC_BUTTON_PROPLOAD)->EnableWindow(FALSE);

	return CPropertyPage::OnSetActive();
}

//==============================================================================================
//	FUNCTION  :	OnButtonPropload
//	ABSTRACT  :	Load an item from the Property bag.
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
void CPropPage::OnButtonPropload() 
{
	HRESULT hr;
	IPersistPropertyBag* pPersistPropertyBag;

	if (m_pBag) {
		CWaitCursor wc;

		try {
			ICameraController* pCamera = m_pCamera->GetInterface();
			CComPtr <IExposure> pExposure = NULL;
			CComPtr <ILevels> pLevels = NULL;
			CComPtr <IEffects> pEffects = NULL;

			GetDlgItem(IDC_BUTTON_PROPLOAD)->EnableWindow(FALSE);

			if (!m_pCamera->Lock())
				return;

			pPersistPropertyBag = NULL;
			if (FAILED(pCamera->get_Exposure(&pExposure)))
				return;
			if (FAILED(pExposure->QueryInterface(IID_IPersistPropertyBag, (void**)&pPersistPropertyBag)))
				return;
			hr = pPersistPropertyBag->Load(m_pBag, NULL);
			pPersistPropertyBag->Release();

			pPersistPropertyBag = NULL;
			if (FAILED(pCamera->get_Levels(&pLevels)))
				return;
			if (FAILED(pLevels->QueryInterface(IID_IPersistPropertyBag, (void**)&pPersistPropertyBag)))
				return;
			hr = pPersistPropertyBag->Load(m_pBag, NULL);
			pPersistPropertyBag->Release();

			pPersistPropertyBag = NULL;
			if (FAILED(pCamera->get_Effects(&pEffects)))
				return;
			if (FAILED(pEffects->QueryInterface(IID_IPersistPropertyBag, (void**)&pPersistPropertyBag)))
				return;
			hr = pPersistPropertyBag->Load(m_pBag, NULL);
			pPersistPropertyBag->Release();

			m_pCamera->Unlock();

			GetDlgItem(IDC_BUTTON_PROPLOAD)->EnableWindow(TRUE);
		}
		catch (_com_error &e) {
			_CONTROLLER_ERRORMSG(e);
			m_pCamera->Unlock();
			return;
		}
	}
}

//==============================================================================================
//	FUNCTION  :	OnButtonPropsave
//	ABSTRACT  :	Save items to the Property Bag
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
void CPropPage::OnButtonPropsave() 
{
	IPersistPropertyBag* pPersistPropertyBag;

	{
		if (m_pBag) {
			m_pBag->Release();
			m_pBag = NULL;
		}

		CPropertyBag* pPropertyBag = new CPropertyBag;
		pPropertyBag->QueryInterface(IID_IPropertyBag, (void**)&m_pBag);
	}

	try {
		CWaitCursor wc;

		ICameraController* pCamera = m_pCamera->GetInterface();
		CComPtr <IExposure> pExposure = NULL;
		CComPtr <ILevels> pLevels = NULL;
		CComPtr <IEffects> pEffects = NULL;
		HRESULT hr;

		pPersistPropertyBag = NULL;
		if (FAILED(pCamera->get_Exposure(&pExposure)))
			return;
		if (FAILED(pExposure->QueryInterface(IID_IPersistPropertyBag, (void**)&pPersistPropertyBag)))
			return;
		hr = pPersistPropertyBag->Save(m_pBag, TRUE, TRUE);
		pPersistPropertyBag->Release();

		pPersistPropertyBag = NULL;
		if (FAILED(pCamera->get_Levels(&pLevels)))
			return;
		if (FAILED(pLevels->QueryInterface(IID_IPersistPropertyBag, (void**)&pPersistPropertyBag)))
			return;
		hr = pPersistPropertyBag->Save(m_pBag, TRUE, TRUE);
		pPersistPropertyBag->Release();

		pPersistPropertyBag = NULL;
		if (FAILED(pCamera->get_Effects(&pEffects)))
			return;
		if (FAILED(pEffects->QueryInterface(IID_IPersistPropertyBag, (void**)&pPersistPropertyBag)))
			return;
		hr = pPersistPropertyBag->Save(m_pBag, TRUE, TRUE);
		pPersistPropertyBag->Release();
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		return;
	}

	GetDlgItem(IDC_BUTTON_PROPLOAD)->EnableWindow(TRUE);
}

//==============================================================================================
//	FUNCTION  :	OnApply
//	ABSTRACT  :	Apply current settings by setting camera parameters
//	PARAMS    :	None
//	RETURN    :	TRUE
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
BOOL CPropPage::OnApply() 
{
	try {
		CWaitCursor wc;

		ICameraController* pCamera = m_pCamera->GetInterface();
		if (!m_pCamera->Lock())
			return FALSE;
		m_ComboIsSuspended.PutValue();
		m_ComboMode.PutValue();
		m_ComboMetering1.PutValue();
		m_EditBias.PutValue();
//		m_EditTime.PutValue();
		m_EditGain.PutValue();
		m_ComboLocked.PutValue();
		m_EditPreset.PutValue();
		m_ComboMetering2.PutValue();
		m_ComboTime.PutValue();
		m_DtGain.PutValue();
		m_EditDetailTime.PutValue();
		m_ComboPreset.PutValue();

		m_EditRedGain.PutValue();
		m_EditGreenGain.PutValue();
		m_EditBlueGain.PutValue();
		m_EditLevelsGain.PutValue();
		m_EditRedSaturation.PutValue();
		m_EditGreenSaturation.PutValue();
		m_EditBlueSaturation.PutValue();
		m_EditSaturation.PutValue();
		m_EditHue.PutValue();
		m_EditContrast.PutValue();
		m_EditTone.PutValue();
		m_RedBalance.PutValue();
		m_GreenBalance.PutValue();
		m_BlueBalance.PutValue();

		m_ComboColorCast.PutValue();
		m_ComboNoiseReduction.PutValue();
		m_EditSharpness.PutValue();
		m_EditShadingCorrection.PutValue();

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
//	FUNCTION  :	ChangeDevice
//	ABSTRACT  :	Respond to changes to the currently selected camera
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
void CPropPage::ChangeDevice()
{
	m_ComboIsSuspended.Disconnect();
	m_ComboMode.Disconnect();
	m_ComboMetering1.Disconnect();
	m_EditBias.Disconnect();
//	m_EditTime.Disconnect();
	m_EditGain.Disconnect();
	m_ComboLocked.Disconnect();
	m_EditPreset.Disconnect();
	m_ComboMetering2.Disconnect();
	m_ComboTime.Disconnect();
	m_DtGain.Disconnect();
	m_EditDetailTime.Disconnect();
	m_ComboPreset.Disconnect();

	m_EditRedGain.Disconnect();
	m_EditGreenGain.Disconnect();
	m_EditBlueGain.Disconnect();
	m_EditLevelsGain.Disconnect();
	m_EditRedSaturation.Disconnect();
	m_EditGreenSaturation.Disconnect();
	m_EditBlueSaturation.Disconnect();
	m_EditSaturation.Disconnect();
	m_EditHue.Disconnect();
	m_EditContrast.Disconnect();
	m_EditTone.Disconnect();
	m_RedBalance.Disconnect();
	m_GreenBalance.Disconnect();
	m_BlueBalance.Disconnect();

	m_ComboColorCast.Disconnect();
	m_ComboNoiseReduction.Disconnect();
	m_EditSharpness.Disconnect();
	m_EditShadingCorrection.Disconnect();
}

//==============================================================================================
//	FUNCTION  :	OnButtonBalance
//	ABSTRACT  :	Perform a white balance
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
void CPropPage::OnButtonBalance() 
{
	HRESULT hr;
	try {
		CWaitCursor wc;

		ICameraController* pCamera = m_pCamera->GetInterface();
		if (!m_pCamera->Lock())
			return;

		CComPtr<ILevels> pLevels = NULL;
		if (FAILED(hr = pCamera->get_Levels(&pLevels)))
			_com_raise_error(hr);

		if (FAILED(hr = pLevels->Balance()))
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
//	FUNCTION  :	OnInitDialog
//	ABSTRACT  :	Initialize this class
//	PARAMS    :	None
//	RETURN    :	TRUE
//	NOTE      :	Intialize the ShadingPosition ComboBox
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
BOOL CPropPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
/*	
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
*/
	// Save Tone Position
	m_ToneList.ResetContent();
	m_ToneList.AddString(TEXT("UserDataA"));
	m_ToneList.AddString(TEXT("UserDataB"));
	m_ToneList.AddString(TEXT("UserDataC"));
	m_ToneList.AddString(TEXT("UserDataD"));
	m_ToneList.AddString(TEXT("UserDataE"));
	m_ToneList.AddString(TEXT("UserDataF"));
	m_ToneList.AddString(TEXT("UserDataG"));
	m_ToneList.SetCurSel(0);
	
	return TRUE;
}

//==============================================================================================
//	FUNCTION  :	OnButtonShadingdatacapture
//	ABSTRACT  :	Send Shading Command to Camera
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
void CPropPage::OnButtonShadingdatacapture() 
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
//	FUNCTION  :	OnButtonToneSet
//	ABSTRACT  :	Send Tone Command to Camera
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.16 Nikon Corp. - Created
//==============================================================================================
void CPropPage::OnButtonToneSet() 
{
	HRESULT hr;
	WORD wToneTable[4096];

	try
	{
		CWaitCursor wc;

		//@ Read file
		CString strFileName;
		m_ToneFilePath.GetWindowText(strFileName);

		CStdioFile clFile;
		if( !clFile.Open(strFileName, CFile::modeRead | CFile::typeText) ){
			MessageBox("Unable to open file.");
			return;
		}

		char	buf[100];
		int		nn = 0;
		while(clFile.ReadString(buf,99)){
			if(nn>4095)
				break;
			wToneTable[nn] = atoi(buf);
			nn++;
		}

		FILE *fp = fopen("C:\\Program Files\\Nikon\\U2-SDK\\Samples\\DsU1Sample\\Release\\Tone.bin","wb");
		if(fp!=NULL){
			fwrite(wToneTable, sizeof(WORD), 4096, fp);
			fclose(fp);
		}
		else{
			MessageBox("Unable to open file.");
			return;
		}

//		BSTR bstrTone = ::SysAllocStringByteLen((LPCSTR)wToneTable,4096*2);
		BSTR bstrToneFileName = ::SysAllocString(L"C:\\Program Files\\Nikon\\U2-SDK\\Samples\\DsU1Sample\\Release\\Tone.bin");

		ICameraController* pCamera = m_pCamera->GetInterface();
		if (!m_pCamera->Lock())
			return;

		CComPtr <ILevels> pLevels = NULL;
		if (FAILED(hr = pCamera->get_Levels(&pLevels)))
			_com_raise_error(hr);
		if (FAILED(hr = pLevels->PresetTone(bstrToneFileName)))
			_com_raise_error(hr);

		::SysFreeString(bstrToneFileName);

		m_pCamera->Unlock();
	}
	catch (_com_error &e)
	{
		_CONTROLLER_ERRORMSG(e);
		m_pCamera->Unlock();
	}
}

//------------------------------------------------------------
//  NAME        :
//    CPropPage::OnOnePushAE
//  DESCTIPTION :
//    
//  TYPE        :
//    void 
//  PARAMETERS  :
//    NONE
//  NOTE  :
//    NONE
//------------------------------------------------------------
void CPropPage::OnOnePushAE() 
{
	HRESULT hr;
	try {
		CWaitCursor wc;

		ICameraController* pCamera = m_pCamera->GetInterface();
		if (!m_pCamera->Lock())
			return;

		CComPtr <IExposure> pExposure = NULL;
		if (FAILED(hr = pCamera->get_Exposure(&pExposure)))
			_com_raise_error(hr);
		if (FAILED(hr = pExposure->OnePushAutoExposure()))
			_com_raise_error(hr);

		m_pCamera->Unlock();
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		m_pCamera->Unlock();
		return;
	}
}
