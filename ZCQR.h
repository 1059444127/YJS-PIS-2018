#if !defined(AFX_ZCQR_H__A7270E40_4E40_4376_9E85_E535CD6EE447__INCLUDED_)
#define AFX_ZCQR_H__A7270E40_4E40_4376_9E85_E535CD6EE447__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZCQR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CZCQR dialog
#include "ZCGrid.h"

class CZCQR : public CXTResizeDialog
{
	DECLARE_DYNCREATE(CZCQR)
// Construction
public:
	CZCQR(CWnd* pParent = NULL);   // standard constructor
	ZCGrid m_grid;
	CString GenNewAdviceNo(CString blh);
	// Dialog Data
	//{{AFX_DATA(CZCQR)
	enum { IDD = IDD_DIALOG_ZCSFQR };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZCQR)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CZCQR)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSearch();
	afx_msg void OnButtonDefaultSearch();
	afx_msg void OnButtonConfirm();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonYzqr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZCQR_H__A7270E40_4E40_4376_9E85_E535CD6EE447__INCLUDED_)
