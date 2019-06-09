//==============================================================================================
//	FILE      :	DsControllerMainSheet.h
//	ABSTRACT  :	Container class for the Devices, Property, and Stream tabs.
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//
//	Copyright (c) 2005, Nikon Corp.  All rights reserved.
//==============================================================================================

#if !defined(AFX_DSCONTROLLERMAINSHEET_H__8FCCC41C_A194_46F6_891B_30E4F4692EBE__INCLUDED_)
#define AFX_DSCONTROLLERMAINSHEET_H__8FCCC41C_A194_46F6_891B_30E4F4692EBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDsControllerMainSheet

#include "PropPage.h"
#include "streampage.h"
#include "devicespage.h"

class CDsControllerMainSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CDsControllerMainSheet)

//Constructor
public:
	CDsControllerMainSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDsControllerMainSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CString m_blh;

protected:
	CCameraControl* m_pCamera;
	CPropPage m_PropPage;
	CStreamPage m_StreamPage;
	CDevicesPage m_DevicesPage;

// override functions
	//{{AFX_VIRTUAL(CDsControllerMainSheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

public:
	void ChangeDevice();
	virtual ~CDsControllerMainSheet();

protected:
	//{{AFX_MSG(CDsControllerMainSheet)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_DSCONTROLLERMAINSHEET_H__8FCCC41C_A194_46F6_891B_30E4F4692EBE__INCLUDED_)
