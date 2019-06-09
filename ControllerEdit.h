//==============================================================================================
//	FILE      :	ControllerEdit.h
//	ABSTRACT  :	Binds a MipParameter to an edit box
//	HISTORY   :	2005.11.04 Nikon Corp. - Created
//
//	Copyright (c) 2005, Nikon Corp.  All rights reserved.
//==============================================================================================

#if !defined(AFX_CONTROLLEREDIT_H__A9F5E72D_C5DE_46CC_96AF_252AC05FA352__INCLUDED_)
#define AFX_CONTROLLEREDIT_H__A9F5E72D_C5DE_46CC_96AF_252AC05FA352__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mipproplink.h"

#define WM_PAREVENT (WM_USER + 1)

/////////////////////////////////////////////////////////////////////////////
// CControllerEdit Window

class CControllerEdit : public CEdit
{
public:
	CControllerEdit();

public:

public:

protected:
	CMipPropLink     m_Source;
	CSpinButtonCtrl	m_Spin;
	BOOL			m_bDirty;
	BOOL			m_bAutoChange;
	VARTYPE			m_vt;

	//{{AFX_VIRTUAL(CControllerEdit)
	//}}AFX_VIRTUAL

public:
	BOOL PutValue();
	void Disconnect();
	BOOL Connect(IDispatch *piParameter);
	virtual ~CControllerEdit();

protected:
	BOOL ValueChanged();
	BOOL InfoChanged();
	BOOL CreateSpinCtrl();
	//{{AFX_MSG(CControllerEdit)
	afx_msg void OnDestroy();
	afx_msg void OnChange();
	//}}AFX_MSG
	afx_msg LRESULT OnParamEvent(WPARAM wParam = 0, LPARAM lParam = 0);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_CONTROLLEREDIT_H__A9F5E72D_C5DE_46CC_96AF_252AC05FA352__INCLUDED_)
