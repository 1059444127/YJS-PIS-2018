//==============================================================================================
//	FILE      :	CameraControl.h
//	ABSTRACT  :	Helper class to work with the CameraController object
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//
//	Copyright (c) 2005, Nikon Corp.  All rights reserved.
//==============================================================================================

#if !defined(AFX_CAMERACONTROL_H__E7C8A7F8_0185_49EA_ABEE_FFC88083BB68__INCLUDED_)
#define AFX_CAMERACONTROL_H__E7C8A7F8_0185_49EA_ABEE_FFC88083BB68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCameraControl  
{
public:
	HRESULT SetDevice(long nIndex);
	HRESULT GetDevices(CStringList& list);
	HRESULT RefreshDevices(CStringList& list);
	HRESULT Uninitialize();
	HRESULT Initialize();
	CCameraControl();
	virtual ~CCameraControl();
	inline ICameraController* GetInterface() {return m_pU2Camera;}
	ICameraController* m_pU2Camera;

protected:
	BOOL U2Error(HRESULT hr);
			// reference to a COM object for camera control from DsCamera.dll
	int m_nLockCount;		// counter to match calls to Lock with calls to Unlock

public:
	BOOL Unlock();
	BOOL Lock(DWORD dwInterval = 1000);
};

#endif // !defined(AFX_CAMERACONTROL_H__E7C8A7F8_0185_49EA_ABEE_FFC88083BB68__INCLUDED_)
