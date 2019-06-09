#if !defined(AFX_AREAOUT_H__B5F9757D_EAF4_4C7C_8FDC_C3FE1BD16C83__INCLUDED_)
#define AFX_AREAOUT_H__B5F9757D_EAF4_4C7C_8FDC_C3FE1BD16C83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AreaOut.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAreaOut dialog
#include "DlgListInput.h"

class CAreaOut : public CDialog
{
// Construction
public:
	CAreaOut(CWnd* pParent = NULL);   // standard constructor

	CString m_title;
	int m_type;

	CString  m_FieldName, m_FieldInfo;

    CDlgListInput  *pListInput;
	int m_focus_type;

// Dialog Data
	//{{AFX_DATA(CAreaOut)
	enum { IDD = IDD_DIALOG_AREAOUT };
	CString	m_from;
	CString	m_to;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAreaOut)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAreaOut)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnListview();
	afx_msg void OnSetfocusEditFrom();
	afx_msg void OnSetfocusEditTo();
	//}}AFX_MSG
	afx_msg LRESULT ListSelect(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AREAOUT_H__B5F9757D_EAF4_4C7C_8FDC_C3FE1BD16C83__INCLUDED_)
