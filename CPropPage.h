//==============================================================================================
//	FILE      :	PropPage.h
//	ABSTRACT  :	Property Page
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//
//	Copyright (c) 2005, Nikon Corp.  All rights reserved.
//==============================================================================================

#if !defined(AFX_PROPPAGE_H__E5A9A9C5_46F8_4D16_8579_A6721D4E2A4B__INCLUDED_)
#define AFX_PROPPAGE_H__E5A9A9C5_46F8_4D16_8579_A6721D4E2A4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPropPage Dialog

#include "controlleredit.h"
#include "controllercombobox.h"
#include "propertybag.h"

class CPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropPage)

public:
	void ChangeDevice();
	CPropPage();
	~CPropPage();

protected:
	CCameraControl* m_pCamera;	// references a camera control object
	IPropertyBag* m_pBag;		// used for management of camera settings

	//{{AFX_DATA(CPropPage)
	enum { IDD = IDD_DIALOG_PROPERTY };
	CControllerEdit	m_EditDetailTime;
	CControllerComboBox	m_ComboPreset;
	CEdit	m_ToneFilePath;
	CComboBox	m_ToneList;
	CControllerEdit	m_DtGain;
	CControllerEdit	m_BlueBalance;
	CControllerEdit	m_GreenBalance;
	CControllerEdit	m_RedBalance;
	CControllerComboBox	m_ComboIsSuspended;
	CControllerComboBox	m_ComboFreezed;
	CControllerEdit	m_EditContrast;
	CControllerEdit	m_EditLevelsGain;
	CComboBox	m_ComboSaveShadingPosition;
	CControllerEdit	m_EditTone;
	CControllerEdit	m_EditHue;
	CControllerEdit	m_EditSaturation;
	CControllerEdit	m_EditBlueSaturation;
	CControllerEdit	m_EditGreenSaturation;
	CControllerEdit	m_EditRedSaturation;
	CControllerEdit	m_EditBlueGain;
	CControllerEdit	m_EditGreenGain;
	CControllerEdit	m_EditRedGain;
	CControllerEdit	m_EditSharpness;
	CControllerEdit	m_EditShadingCorrection;
	CControllerComboBox	m_ComboNoiseReduction;
	CControllerComboBox	m_ComboColorCast;
	CControllerEdit	m_EditPreset;
	CControllerComboBox	m_ComboMetering2;
	CControllerComboBox	m_ComboMetering1;
	CControllerComboBox	m_ComboLocked;
	CControllerComboBox	m_ComboMode;
	CControllerEdit	m_EditGain;
	CControllerEdit	m_EditBias;
	CControllerComboBox	m_ComboTime;
	//}}AFX_DATA


// override
	//{{AFX_VIRTUAL(CPropPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CPropPage)
	afx_msg void OnButtonPropload();
	afx_msg void OnButtonPropsave();
	afx_msg void OnButtonBalance();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonShadingdatacapture();
	afx_msg void OnButtonToneSet();
	afx_msg void OnOnePushAE();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_PROPPAGE_H__E5A9A9C5_46F8_4D16_8579_A6721D4E2A4B__INCLUDED_)
