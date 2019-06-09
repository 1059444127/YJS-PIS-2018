#if !defined(AFX_JSYZ_H__D380B67B_9E0C_428A_B471_604D8569A799__INCLUDED_)
#define AFX_JSYZ_H__D380B67B_9E0C_428A_B471_604D8569A799__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Jsyz.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJsyz dialog
#include "AdviceGrid.h"

class CJsyz : public CXTResizeDialog
{
// Construction
public:
	CJsyz(CWnd* pParent = NULL);   // standard constructor

	CAdviceGrid  m_AdviceList;
    CString GenNewAdviceNo(CString blh);

	CStringArray m_QcbwList;

// Dialog Data
	//{{AFX_DATA(CJsyz)
	enum { IDD = IDD_DIALOG_JSYZ };
	CXTButton	m_btBlh;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJsyz)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJsyz)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	afx_msg void OnSelchangeComboYzlx();
	afx_msg void OnDestroy();
	afx_msg void OnButtonBlh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JSYZ_H__D380B67B_9E0C_428A_B471_604D8569A799__INCLUDED_)
