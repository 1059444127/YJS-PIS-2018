//==============================================================================================
//	FILE      :	DevicesPage.h
//	ABSTRACT  :	Handles interactions with the CameraController object for the property page 
//			hosted by the Device Tab
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//
//	Copyright (c) 2005, Nikon Corp.  All rights reserved.
//==============================================================================================

#if !defined(AFX_DEVICESPAGE_H__A84493AA_88C2_4FF1_AB59_CF6A5AB89BBB__INCLUDED_)
#define AFX_DEVICESPAGE_H__A84493AA_88C2_4FF1_AB59_CF6A5AB89BBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDevicesPage Dialog

#include "controlleredit.h"
#include "controllercombobox.h"

class CDevicesPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CDevicesPage)

public:
	void ChangeDevice();
	CDevicesPage();
	~CDevicesPage();

protected:
	CCameraControl* m_pCamera;	// a pointer to a wrapper class for the CameraController object

	//{{AFX_DATA(CDevicesPage)
	enum { IDD = IDD_DIALOG_DEVICES };
	CControllerComboBox	m_clCameraTemp;
	CControllerEdit	m_ROIPosition;
	CEdit	m_EditVersion;
	CEdit	m_EditName;
	CControllerComboBox	m_ComboModel;
	CControllerComboBox	m_ComboScanMode;
	CComboBox	m_ComboDevices;
	CString	m_cszCameraType;
	//}}AFX_DATA


// Override

	//{{AFX_VIRTUAL(CDevicesPage)
	public:
	virtual BOOL OnApply();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL	m_bDirty;		// flag to determine if a property has changed
	BOOL	m_ChangeFlg;	// flag to determine if the selected camera has changed
	//{{AFX_MSG(CDevicesPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboDevices();
	afx_msg void OnChangeEditName();
	afx_msg void OnRefreshList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_DEVICESPAGE_H__A84493AA_88C2_4FF1_AB59_CF6A5AB89BBB__INCLUDED_)
