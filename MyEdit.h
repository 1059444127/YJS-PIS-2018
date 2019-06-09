#if !defined(AFX_MYEDIT_H__B81B8264_C214_43E0_BB25_ADCB4FD9D700__INCLUDED_)
#define AFX_MYEDIT_H__B81B8264_C214_43E0_BB25_ADCB4FD9D700__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyEdit window

class CMyEdit : public CEdit
{
// Construction
public:
	CMyEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyEdit();

	// Generated message map functions
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnCopy();
	afx_msg void OnCut();
	//{{AFX_MSG(CMyEdit)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCopyGjsj();
	afx_msg void OnEditPaste();
	afx_msg void OnBdjg();
	afx_msg void OnBbmc();
	afx_msg void OnCopyRysj();
	afx_msg void OnCopyZdyj();
	afx_msg void OnInsertBszy();
	afx_msg void OnInsertRysj();
	afx_msg void OnInsertZdyj();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYEDIT_H__B81B8264_C214_43E0_BB25_ADCB4FD9D700__INCLUDED_)
