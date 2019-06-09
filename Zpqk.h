#if !defined(AFX_ZPQK_H__1F4F57EB_9A43_40B5_8813_DCD4A3258188__INCLUDED_)
#define AFX_ZPQK_H__1F4F57EB_9A43_40B5_8813_DCD4A3258188__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Zpqk.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CZpqk dialog
#include "PrintPreviewDialog.h"

class CZpqk : public CPrintPreviewDialog
{
// Construction
public:
	CZpqk(CWnd* pParent = NULL);   // standard constructor
	virtual afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult );

// Dialog Data
	//{{AFX_DATA(CZpqk)
	enum { IDD = IDD_DIALOG_ZPQK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CXTFlatComboBox     m_wndYear;
	CXTFlatComboBox     m_wndTime;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZpqk)
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

	void ChangeTimeStr();

	// Generated message map functions
	//{{AFX_MSG(CZpqk)
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLocPrint();
	afx_msg void OnLocPrintPreview();
	//}}AFX_MSG
	afx_msg void OnSelchangeComboYear();
	afx_msg void OnSelchangeComboTime();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZPQK_H__1F4F57EB_9A43_40B5_8813_DCD4A3258188__INCLUDED_)
