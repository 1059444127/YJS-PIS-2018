#if !defined(AFX_QPYJ_H__B3533644_FBBE_437A_BCCD_E285C6B02D69__INCLUDED_)
#define AFX_QPYJ_H__B3533644_FBBE_437A_BCCD_E285C6B02D69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Qpyj.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CQpyj dialog
#include "PrintPreviewDialog.h"

class CQpyj : public CPrintPreviewDialog
{
// Construction
public:
	CQpyj(CWnd* pParent = NULL);   // standard constructor
	virtual afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult );

// Dialog Data
	//{{AFX_DATA(CQpyj)
	enum { IDD = IDD_DIALOG_QPYJ };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQpyj)
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
	//{{AFX_MSG(CQpyj)
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLocPrint();
	afx_msg void OnLocPrintPreview();
	afx_msg void OnButtonAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QPYJ_H__B3533644_FBBE_437A_BCCD_E285C6B02D69__INCLUDED_)
