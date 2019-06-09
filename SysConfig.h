#if !defined(AFX_SYSCONFIG_H__BAA7E938_8E3B_4B79_84EE_575F20EB0636__INCLUDED_)
#define AFX_SYSCONFIG_H__BAA7E938_8E3B_4B79_84EE_575F20EB0636__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysConfig.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSysConfig dialog
#include "INI.h"

class CSysConfig : public CXTResizeDialog
{
// Construction
public:
	CSysConfig(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSysConfig)
	enum { IDD = IDD_DIALOG_SYSCONFIG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CXTPropertyGrid m_wndPropertyGrid;
	CIniReader m_IniReader;

	// Generated message map functions
	//{{AFX_MSG(CSysConfig)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSCONFIG_H__BAA7E938_8E3B_4B79_84EE_575F20EB0636__INCLUDED_)
