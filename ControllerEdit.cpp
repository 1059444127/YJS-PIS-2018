//==============================================================================================
//	FILE      :	ControllerEdit.cpp
//	ABSTRACT  :	Binds a MipParameter to an edit box
//	HISTORY   :	2005.11.04 Nikon Corp. - Created
//
//	Copyright (c) 2005, Nikon Corp.  All rights reserved.
//==============================================================================================

#include "stdafx.h"
#include "Pathology.h"
#include "ControllerEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControllerEdit

//==============================================================================================
//	FUNCTION  :	CControllerEdit
//	ABSTRACT  :	Constructor
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.04 Nikon Corp. - Created
//==============================================================================================
CControllerEdit::CControllerEdit()
{
	m_bDirty = FALSE;
	m_bAutoChange = FALSE;
	m_Source.m_pProperty = NULL;
	m_vt = VT_NULL;
}

//==============================================================================================
//	FUNCTION  :	~CControllerEdit
//	ABSTRACT  :	destructor
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.04 Nikon Corp. - Created
//==============================================================================================
CControllerEdit::~CControllerEdit()
{
}


BEGIN_MESSAGE_MAP(CControllerEdit, CEdit)
	//{{AFX_MSG_MAP(CControllerEdit)
	ON_WM_DESTROY()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PAREVENT,OnParamEvent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControllerEdit Message handler

//==============================================================================================
//	FUNCTION  :	OnParamEvent
//	ABSTRACT  :	Event sink
//	PARAMS    :	wParam: not used
//			lParam: event number
//	RETURN    :	0
//	NOTE      :	
//	HISTORY   :	2005.11.04 Nikon Corp. - Created
//==============================================================================================
LRESULT CControllerEdit::OnParamEvent(WPARAM wParam, LPARAM lParam)
{
	switch(lParam)
	{
	case EMIPPAR_VALUECHANGED:
		OutputDebugString("CControllerEdit::OnParamEvent EMIPPAR_VALUECHANGED\n");
		ValueChanged();
		break;
	case EMIPPAR_INFOCHANGED:
		OutputDebugString("CControllerEdit::OnParamEvent EMIPPAR_INFOCHANGED\n");
		InfoChanged();
		break;
	}
	return 0L;
}

//==============================================================================================
//	FUNCTION  :	Connect
//	ABSTRACT  :	Connects a MIP parameter to an edit box
//	PARAMS    :	piParameter: parameter to connect to the edit box
//	RETURN    :	TRUE
//	NOTE      :	
//	HISTORY   :	2005.11.04 Nikon Corp. - Created
//==============================================================================================
BOOL CControllerEdit::Connect(IDispatch *piParameter)
{
	try {
		if (m_Source.m_pProperty)
			return FALSE;

		if(piParameter==NULL)
			return FALSE;

		Disconnect();

		CComPtr<IMipParameter> spiParam;
		HRESULT hr = piParameter->QueryInterface(&spiParam);

		if(FAILED(hr))
			return FALSE;

		if(SUCCEEDED(hr)){
			VARIANT var;
			m_Source.m_pProperty = spiParam;
			hr = m_Source.m_pProperty->get_RawValue(&var);
			if (SUCCEEDED(hr)) {
				m_vt = var.vt;
				hr = m_Source.AdviseAsync((long)m_hWnd,WM_PAREVENT,0);
				if (m_vt != VT_BSTR && m_vt != VT_R8) {
					hr = m_Source->put_DisplayCFormat(CComBSTR("%.0f"));
					if (!CreateSpinCtrl())
						return FALSE;
				}
			}
			InfoChanged();
			ValueChanged();
		}
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		return FALSE;
	}
	return TRUE;
}

//==============================================================================================
//	FUNCTION  :	Disconnect
//	ABSTRACT  :	Disconnects a MIP parameter from an edit box
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.04 Nikon Corp. - Created
//==============================================================================================
void CControllerEdit::Disconnect()
{
	HRESULT hr;
	try {
		hr = m_Source.UnAdviseAsync();
		if (m_Source.m_pProperty) {
			m_Source.m_pProperty.Release();
			m_Source.m_pProperty = NULL;
		}
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		return;
	}
}

//==============================================================================================
//	FUNCTION  :	InfoChanged
//	ABSTRACT  :	Updates the edit box when the metadata of the underlying MIP parameter changes
//	PARAMS    :	None
//	RETURN    :	TRUE
//	NOTE      :	
//	HISTORY   :	2005.11.04 Nikon Corp. - Created
//==============================================================================================
BOOL CControllerEdit::InfoChanged()
{
	try {
		m_Source.ResetInfoChangedFlag();
		ATLASSERT(m_Source);
		EMIPPAR_LIMITATION lt;
		m_Source->get_LimitationType(&lt);

		VARIANT_BOOL bRead = VARIANT_FALSE;
		m_Source->get_IsReadOnly(&bRead);
		if(bRead==VARIANT_TRUE)
			this->EnableWindow(FALSE);
		else
			this->EnableWindow(TRUE);

		if (m_vt != VT_BSTR && m_vt != VT_R8) {
			// Upper limit, Lower limit
			VARIANT low, high;
			VariantInit( &low );
			VariantInit( &high );
			m_Source->get_RangeLowerLimit( &low );
			m_Source->get_RangeHigherLimit( &high );
			if (m_Spin.m_hWnd)
				m_Spin.SetRange( low.lVal, high.lVal );
		}
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		return FALSE;
	}

	return TRUE;
}

//==============================================================================================
//	FUNCTION  :	ValueChanged
//	ABSTRACT  :	Updates the edit box when the value of the underlying MIP parameter changes
//	PARAMS    :	None
//	RETURN    :	TRUE
//	NOTE      :	The Apply button is disabled during this event
//	HISTORY   :	2005.11.04 Nikon Corp. - Created
//==============================================================================================
BOOL CControllerEdit::ValueChanged()
{
	try {
		m_Source.ResetValueChangedFlag();
		if(m_Source.m_pProperty!=NULL){
			CComBSTR bstrDisplay;
			HRESULT hr = m_Source.m_pProperty->get_DisplayString(&bstrDisplay);
			if (FAILED(hr)) 
				_com_raise_error(hr);
			if (!bstrDisplay.Length()) {
				CComVariant var;
				hr = m_Source.m_pProperty->get_RawValue(&var);
				if (FAILED(hr)) {
					if (hr == DISP_E_EXCEPTION)
						_com_raise_error(var.lVal);
					else
						_com_raise_error(hr);
				}
			}

			//Update data
			if (!m_bDirty) {
				CString strValue(bstrDisplay);
				m_bAutoChange = TRUE;
				SetWindowText(strValue);
				m_bAutoChange = FALSE;
			}
		}
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG_LOG(e);
		return FALSE;
	}
	return TRUE;
}

//==============================================================================================
//	FUNCTION  :	OnDestroy
//	ABSTRACT  :	
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.04 Nikon Corp. - Created
//==============================================================================================
void CControllerEdit::OnDestroy() 
{
	CEdit::OnDestroy();
	
	Disconnect();
	if (m_Spin.m_hWnd)
		m_Spin.DestroyWindow();
}

//==============================================================================================
//	FUNCTION  :	CreateSpinCtrl
//	ABSTRACT  :	create a spin control
//	PARAMS    :	None
//	RETURN    :	TRUE
//	NOTE      :	
//	HISTORY   :	2005.11.04 Nikon Corp. - Created
//==============================================================================================
BOOL CControllerEdit::CreateSpinCtrl()
{
	if (m_Spin.m_hWnd)
		return TRUE;

	BOOL res = m_Spin.Create(UDS_SETBUDDYINT | UDS_ARROWKEYS | UDS_NOTHOUSANDS | UDS_ALIGNRIGHT,
		CRect(0, 0, 0, 0), GetParent(), GetDlgCtrlID() + 100);
	if (!res) return FALSE;

	m_Spin.SetBuddy(this);
	m_Spin.ShowWindow(SW_SHOW);
	return TRUE;
}

//==============================================================================================
//	FUNCTION  :	OnChange
//	ABSTRACT  :	
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.04 Nikon Corp. - Created
//==============================================================================================
void CControllerEdit::OnChange() 
{
	if (!m_bAutoChange) {
		m_bDirty = TRUE;
		((CPropertyPage*)GetParent())->SetModified(TRUE);
	}
}

//==============================================================================================
//	FUNCTION  :	PutValue
//	ABSTRACT  :	Apply the current settings to the camera
//	PARAMS    :	None
//	RETURN    :	TRUE
//	NOTE      :	
//	HISTORY   :	2005.11.04 Nikon Corp. - Created
//==============================================================================================
BOOL CControllerEdit::PutValue()
{
	CString str;

	if (!m_bDirty) return TRUE;
	GetWindowText(str);

	try {
		if(m_Source.m_pProperty!=NULL){
			CComBSTR bstrDisplay;
			bstrDisplay = str;
			HRESULT hr = m_Source.m_pProperty->put_DisplayString(bstrDisplay);
			if (FAILED(hr)) 
				_com_raise_error(hr);

			m_bDirty = FALSE;
			return TRUE;
		}
		return FALSE;
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		return FALSE;
	}
}
