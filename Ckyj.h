#if !defined(AFX_CKYJ_H__F2AE9410_F6EF_43BF_8FC4_FE3118C7DF21__INCLUDED_)
#define AFX_CKYJ_H__F2AE9410_F6EF_43BF_8FC4_FE3118C7DF21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Ckyj.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCkyj dialog
#include "PrintPreviewDialog.h"

class CCkyj : public CPrintPreviewDialog
{
// Construction
public:
	CCkyj(CWnd* pParent = NULL);   // standard constructor
	virtual afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult );

// Dialog Data
	//{{AFX_DATA(CCkyj)
	enum { IDD = IDD_DIALOG_CKYJ };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCkyj)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void GenList();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CXTToolBar    m_wndToolBar;

	BOOL GetToolText( UINT nID, CString& strTipText, CString& /*strMessage*/);

	CXTDateTimeCtrl m_Time_Start, m_Time_End;

	// Generated message map functions
	//{{AFX_MSG(CCkyj)
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLocPrintPreview();
	afx_msg void OnLocPrint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CKYJ_H__F2AE9410_F6EF_43BF_8FC4_FE3118C7DF21__INCLUDED_)
