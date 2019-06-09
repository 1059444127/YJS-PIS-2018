h//==============================================================================================
//	FILE      :	ControllerComboBox.cpp
//	ABSTRACT  :	ComboBox class
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//
//	Copyright (c) 2005, Nikon Corp.  All rights reserved.
//==============================================================================================

#include "stdafx.h"
#include "Pathology.h"
#include "ControllerComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControllerComboBox

//==============================================================================================
//	FUNCTION  :	CControllerComboBox
//	ABSTRACT  :	Constructor
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
CControllerComboBox::CControllerComboBox()
{
	::memset( m_lRawValue, 0, sizeof( m_lRawValue ) );
	m_bDirty = FALSE;
	m_bAutoChange = FALSE;
	m_Source.m_pProperty = NULL;
}

//==============================================================================================
//	FUNCTION  :	~CControllerComboBox
//	ABSTRACT  :	Destructor
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
CControllerComboBox::~CControllerComboBox()
{
}


BEGIN_MESSAGE_MAP(CControllerComboBox, CComboBox)
	//{{AFX_MSG_MAP(CControllerComboBox)
	ON_WM_DESTROY()
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PAREVENT,OnParamEvent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControllerComboBox message Handler

//==============================================================================================
//	FUNCTION  :	OnParamEvent
//	ABSTRACT  :	receive event
//	PARAMS    :	wParam: not used
//			lParam: event number
//	RETURN    :	0
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
LRESULT CControllerComboBox::OnParamEvent(WPARAM wParam, LPARAM lParam)
{
	switch(lParam)
	{
	case EMIPPAR_VALUECHANGED:
		OutputDebugString("CControllerComboBox::OnParamEvent EMIPPAR_VALUECHANGED\n");
		ValueChanged();
		break;
	case EMIPPAR_INFOCHANGED:
		OutputDebugString("CControllerComboBox::OnParamEvent EMIPPAR_INFOCHANGED\n");
		InfoChanged();
		ValueChanged();
		break;
	}
	return 0L;
}

//==============================================================================================
//	FUNCTION  :	Connect
//	ABSTRACT  :	Binds a MIP parameter to a combo box
//	PARAMS    :	piParameter: parameter to connect to the combo box
//	RETURN    :	TRUE
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
BOOL CControllerComboBox::Connect(IDispatch *piParameter)
{
	HRESULT hr;
	try {
		if (m_Source.m_pProperty)
			return FALSE;

		if(piParameter==NULL)
			return FALSE;

		Disconnect();

		CComPtr<IMipParameter> spiParam;
		hr = piParameter->QueryInterface(&spiParam);

		if(FAILED(hr))
			return FALSE;

		if(SUCCEEDED(hr)){
			m_Source.m_pProperty = spiParam;
			hr = m_Source.AdviseAsync((long)m_hWnd,WM_PAREVENT,0);

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
//	ABSTRACT  :	Disconnects a MIP parameter from a combo box
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
void CControllerComboBox::Disconnect()
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
//	ABSTRACT  :	Updates a parameter when parameter metadata changes
//	PARAMS    :	None
//	RETURN    :	TRUE
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
BOOL CControllerComboBox::InfoChanged()
{
	HRESULT hr;
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

		struct IDisplayRawPairCollection* pEnumSet;
		hr = m_Source->get_EnumSet( &pEnumSet );
		if (FAILED(hr)) 
			_com_raise_error(hr);

		long lCount = 0;
		if( pEnumSet != NULL )
		{
			hr = pEnumSet->get_Count( &lCount );
			if (FAILED(hr)) 
				_com_raise_error(hr);
		}

		long nAdd = 0;
		ResetContent();
		DisplayRawPair stDisplayRawPair;
		for( int iItem = 0; iItem < lCount; iItem++ )
		{
			hr = pEnumSet->get_Item( iItem + 1, &stDisplayRawPair );
			if (FAILED(hr)) 
				_com_raise_error(hr);

			_variant_t vtTemp = stDisplayRawPair.RawValue;
			switch( ( long )vtTemp )
			{
			case ExposureUnknown:
				break;
			default:
				AddString( CString( stDisplayRawPair.DisplayString ) );
//				m_lRawValue[iItem-1] = vtTemp.lVal;
				m_lRawValue[nAdd] = vtTemp.lVal;
				nAdd++;
				break;
			}

			::SysFreeString(stDisplayRawPair.DisplayString);
			::VariantClear(&stDisplayRawPair.RawValue);
		}

		pEnumSet->Release();
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		return FALSE;
	}
	return TRUE;
}

//==============================================================================================
//	FUNCTION  :	ValueChanged
//	ABSTRACT  :	Updates a control when the underlying MIP parameter changes
//	PARAMS    :	None
//	RETURN    :	TRUE
//	NOTE      :	The Apply button is disabled during this event
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
BOOL CControllerComboBox::ValueChanged()
{
	HRESULT hr;
	try {
		m_Source.ResetValueChangedFlag();
		if(m_Source.m_pProperty!=NULL){
			VARIANT result;
			::VariantInit( &result );

			hr = m_Source->get_RawValue( &result );
			if (FAILED(hr)) {
				if (hr == DISP_E_EXCEPTION)
					_com_raise_error(result.lVal);
				else
					_com_raise_error(hr);
			}
			int nCount = GetCount();
			for( int i = 0; i < nCount; i++ )
			{
				if( m_lRawValue[i] == result.lVal )
				{
					if (!m_bDirty) {
						m_bAutoChange = TRUE;
						CString cszTest;
						GetLBText(i,cszTest);
						SetCurSel( i );
						m_bAutoChange = FALSE;
					}
					break;
				}
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
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
void CControllerComboBox::OnDestroy() 
{
	CComboBox::OnDestroy();
	
	Disconnect();
}

//==============================================================================================
//	FUNCTION  :	PutValue
//	ABSTRACT  :	Apply current settings to the camera
//	PARAMS    :	None
//	RETURN    :	TRUE
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
BOOL CControllerComboBox::PutValue()
{
	HRESULT hr;

	if (!m_bDirty) return TRUE;

	try {
		if(m_Source.m_pProperty!=NULL){
			int nSelPos = GetCurSel();
			struct IDisplayRawPairCollection* pEnumSet;
			hr = m_Source->get_EnumSet( &pEnumSet );
			if (FAILED(hr)) 
				_com_raise_error(hr);

			long lCount = 0;
			if( pEnumSet != NULL )
			{
				hr = pEnumSet->get_Count( &lCount );
				if (FAILED(hr)) 
					_com_raise_error(hr);
			}

			for (long i = 0; i < lCount; i++) {
				DisplayRawPair stDisplayRawPair;
				hr = pEnumSet->get_Item( i + 1, &stDisplayRawPair );
				if (FAILED(hr)) 
					_com_raise_error(hr);
				_variant_t vtTemp = stDisplayRawPair.RawValue;
				if (vtTemp.lVal == m_lRawValue[nSelPos]) {
					hr = m_Source.m_pProperty->put_RawValue(vtTemp);
					if (FAILED(hr))
						_com_raise_error(hr);
					m_bDirty = FALSE;
					break;
				}
			}

			if (m_bDirty) return FALSE;
			return TRUE;
		}
		return FALSE;
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		return FALSE;
	}
}

//==============================================================================================
//	FUNCTION  :	OnSelchange
//	ABSTRACT  :	The currently selected combo box item has changed
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
void CControllerComboBox::OnSelchange() 
{
	if (!m_bAutoChange) {
		m_bDirty = TRUE;
		((CPropertyPage*)GetParent())->SetModified(TRUE);
	}
}
