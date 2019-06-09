#if !defined(AFX_SFJG_H__EE5EFB84_9161_4F48_ADC7_9A9CE0FBAA18__INCLUDED_)
#define AFX_SFJG_H__EE5EFB84_9161_4F48_ADC7_9A9CE0FBAA18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Sfjg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSfjg dialog
#include "MenuEdit.h"

class CSfjg : public CXTResizeDialog
{
// Construction
public:
	CSfjg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSfjg)
	enum { IDD = IDD_DIALOG_SFJG };
	CMenuEdit	m_ctrl_sfys;
	CString	m_sfjg;
	CString	m_sfys;
	//}}AFX_DATA

	CXTDateTimeCtrl m_Time_Jhsfsj;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSfjg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSfjg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SFJG_H__EE5EFB84_9161_4F48_ADC7_9A9CE0FBAA18__INCLUDED_)
