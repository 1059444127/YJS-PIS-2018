//==============================================================================================
//	FILE      :	CameraControl.cpp
//	ABSTRACT  :	Class for Management of Camera
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//
//	Copyright (c) 2005, Nikon Corp.  All rights reserved.
//==============================================================================================

#include "stdafx.h"
#include "Pathology.h"
#include "CameraControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Constructor/Destructor
//////////////////////////////////////////////////////////////////////

//==============================================================================================
//	FUNCTION  :	CCameraControl
//	ABSTRACT  :	Constructor
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
CCameraControl::CCameraControl()
{
	m_pU2Camera = NULL;
	m_nLockCount = 0;
}

//==============================================================================================
//	FUNCTION  :	~CCameraControl
//	ABSTRACT  :	Destructor
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	Wrapper for Uninitialize()
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
CCameraControl::~CCameraControl()
{
	if (m_pU2Camera)
		Uninitialize();
}

//==============================================================================================
//	FUNCTION  :	Initialize
//	ABSTRACT  :	Camera initialization
//	PARAMS    :	None
//	RETURN    :	S_OK
//	NOTE      :	Get a reference to a CameraController object from the DsCamera.dll COM server
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
HRESULT CCameraControl::Initialize()
{
	HRESULT hr;
	try {
		hr = CoCreateInstance(CLSID_CameraController, NULL, CLSCTX_ALL, IID_ICameraController, (void**)&m_pU2Camera);
		if (FAILED(hr))
			_com_raise_error(hr);
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		hr = e.Error();
	}

	return hr;
}

//==============================================================================================
//	FUNCTION  :	Uninitialize
//	ABSTRACT  :	Uninitialization of Camera
//	PARAMS    :	None
//	RETURN    :	S_OK
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
HRESULT CCameraControl::Uninitialize()
{
	HRESULT hr = S_OK;

	if (m_pU2Camera) {
		try {
			CComPtr<IControllerDevice> pU2Device = NULL;
			Unlock();
			if (U2Error(hr = m_pU2Camera->get_Device(&pU2Device)))
				return hr;
			if (pU2Device) {
				if (U2Error(hr = pU2Device->Disconnect()))
					return hr;
			}

			m_pU2Camera->Release();
			m_pU2Camera = NULL;
		}
		catch (_com_error &e) {
			_CONTROLLER_ERRORMSG(e);
			hr = e.Error();
		}
	}

	return hr;
}

//==============================================================================================
//	FUNCTION  :	GetDevices
//	ABSTRACT  :	Enumerate the list of available cameras from the Devices collection
//	PARAMS    :	CStringList: list object for camera addresses
//	RETURN    :	S_OK
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
HRESULT CCameraControl::GetDevices(CStringList &list)
{
	HRESULT hr = S_OK;

	list.RemoveAll();

	try {
		CComPtr<IControllerDevices> pU2Devices = NULL;
		if (U2Error(hr = m_pU2Camera->get_Devices(&pU2Devices)))
			return hr;

		long nCount;
		if (U2Error(hr = pU2Devices->get_Count(&nCount)))
			return hr;

		for (long i = 0; i < nCount; i++) {
			CComPtr<IControllerDevice> pU2Device = NULL;
			if (U2Error(hr = pU2Devices->get_Item(i, &pU2Device)))
				return hr;
			BSTR Address;
			if (U2Error(hr = pU2Device->get_Address(&Address)))
				return hr;
			list.AddTail(Address);
		}
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		hr = e.Error();
	}

	return hr;
}

//==============================================================================================
//	FUNCTION  :	U2Error
//	ABSTRACT  :	throw an exception using _com_raise_error
//	PARAMS    :	HRESULT hr to determine if an error has occurred
//	RETURN    :	TRUE
//	NOTE      :
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
BOOL CCameraControl::U2Error(HRESULT hr)
{
	if (SUCCEEDED(hr)) return FALSE;


	_com_raise_error(hr);

	return TRUE;
}

//==============================================================================================
//	FUNCTION  :	SetDevice
//	ABSTRACT  :	Connect to the specified camera.
//	PARAMS    :	long nIndex: index of the camera corresponding to the Devices collection
//	RETURN    :	S_OK
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
HRESULT CCameraControl::SetDevice(long nIndex)
{
	HRESULT hr;

	CComPtr<IControllerDevices> pU2Devices = NULL;
	CComPtr<IControllerDevice> pU2Device = NULL;

	if (U2Error(hr = m_pU2Camera->get_Device(&pU2Device)))
		return hr;

	if (U2Error(hr = pU2Device->Disconnect()))
		return hr;

	if (U2Error(hr = m_pU2Camera->get_Devices(&pU2Devices)))
		return hr;

	pU2Device = NULL;
	if (U2Error(hr = pU2Devices->get_Item(nIndex, &pU2Device)))
		return hr;

	if (U2Error(hr = m_pU2Camera->put_Device(pU2Device)))
		return hr;

	if (U2Error(hr = pU2Device->Connect()))
		return hr;

	return hr;
}

//==============================================================================================
//	FUNCTION  :	Lock
//	ABSTRACT  :	Locks the camera for exclusive write access
//	PARAMS    :	DWORD dwInterval: Reserved for future use
//	RETURN    :	TRUE
//	NOTE      : 
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
BOOL CCameraControl::Lock(DWORD dwInterval)
{
	HRESULT hr;
	if (m_pU2Camera && !m_nLockCount) {
		try {
			hr = m_pU2Camera->LockDevice((long)dwInterval);
			if (FAILED(hr))
				_com_raise_error(hr);
		}
		catch (_com_error &e) {
			_CONTROLLER_ERRORMSG(e);
			return FALSE;
		}
	}
	m_nLockCount++;
	return TRUE;
}

//==============================================================================================
//	FUNCTION  :	Unlock
//	ABSTRACT  :	Releases the camera for shared write access
//	PARAMS    :	None
//	RETURN    :	TRUE
//	NOTE      :	if Counter == 0 then call UnlockDevice
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
BOOL CCameraControl::Unlock()
{
	HRESULT hr;
	if (!m_nLockCount)
		return TRUE;
	m_nLockCount--;
	if (!m_nLockCount) {
		try {
			OutputDebugString("UnlockDevice Start\n");
			hr = m_pU2Camera->UnlockDevice();
			OutputDebugString("UnlockDevice End\n");
			if (FAILED(hr))
				_com_raise_error(hr);
		}
		catch (_com_error &e) {
			_CONTROLLER_ERRORMSG(e);
			return FALSE;
		}
	}
	return TRUE;
}

//==============================================================================================
//	FUNCTION  :	RefreshDevices
//	ABSTRACT  :	Refreshes the list of available cameras
//	PARAMS    :	CStringList: list object for camera addresses
//	RETURN    :	S_OK
//	NOTE      :	The list of USB devices can change at runtime
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
HRESULT CCameraControl::RefreshDevices(CStringList &list)
{
	HRESULT hr = S_OK;

	list.RemoveAll();

	try {
		CComPtr<IControllerDevices> pU2Devices = NULL;
		if (U2Error(hr = m_pU2Camera->get_Devices(&pU2Devices)))
			return hr;

		if (U2Error(hr = pU2Devices->_UpdateDevices()))
			return hr;
		long nCount;
		if (U2Error(hr = pU2Devices->get_Count(&nCount)))
			return hr;

		for (long i = 0; i < nCount; i++) {
			CComPtr<IControllerDevice> pU2Device = NULL;
			if (U2Error(hr = pU2Devices->get_Item(i, &pU2Device)))
				return hr;
			BSTR Address;
			if (U2Error(hr = pU2Device->get_Address(&Address)))
				return hr;
			list.AddTail(Address);
		}
	}
	catch (_com_error &e) {
		_CONTROLLER_ERRORMSG(e);
		hr = e.Error();
	}

	return hr;
}
