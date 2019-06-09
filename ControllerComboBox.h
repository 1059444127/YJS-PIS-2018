//==============================================================================================
//	FILE      :	ControllerComboBox.h
//	ABSTRACT  :	ComboBox class For MipParameter
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//
//	Copyright (c) 2005, Nikon Corp.  All rights reserved.
//==============================================================================================

#if !defined(AFX_CONTROLLERCOMBOBOX_H__C7D01224_89AF_4CF5_870B_2A558591C1C7__INCLUDED_)
#define AFX_CONTROLLERCOMBOBOX_H__C7D01224_89AF_4CF5_870B_2A558591C1C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mipproplink.h"

#define WM_PAREVENT		(WM_USER + 1)
#define MAX_COMBO_VALUE	50

/////////////////////////////////////////////////////////////////////////////
// CControllerComboBox Window

class CControllerComboBox : public CComboBox
{
public:
	CControllerComboBox();

public:

public:

protected:
	CMipPropLink	m_Source;			// a pointer to CMipPropLink
	BOOL			m_bDirty;			// flag to determine if a property has changed
	BOOL			m_bAutoChange;		// flag when a property has changed automatically

// override
	//{{AFX_VIRTUAL(CControllerComboBox)
	//}}AFX_VIRTUAL

public:
	BOOL PutValue();
	void Disconnect();
	BOOL Connect(IDispatch *piParameter);
	virtual ~CControllerComboBox();

protected:
	BOOL ValueChanged();
	BOOL InfoChanged();
	long m_lRawValue[MAX_COMBO_VALUE];
	//{{AFX_MSG(CControllerComboBox)
	afx_msg void OnDestroy();
	afx_msg void OnSelchange();
	//}}AFX_MSG
	afx_msg LRESULT OnParamEvent(WPARAM wParam = 0, LPARAM lParam = 0);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_CONTROLLERCOMBOBOX_H__C7D01224_89AF_4CF5_870B_2A558591C1C7__INCLUDED_)
