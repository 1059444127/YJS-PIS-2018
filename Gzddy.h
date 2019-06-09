#if !defined(AFX_GZDDY_H__4CA4D3AB_28AD_4596_BF67_ED0B75579B13__INCLUDED_)
#define AFX_GZDDY_H__4CA4D3AB_28AD_4596_BF67_ED0B75579B13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Gzddy.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGzddy dialog
#include "PrintPreviewDialog.h"

class CGzddy : public CPrintPreviewDialog
{
// Construction
public:
	CGzddy(CWnd* pParent = NULL);   // standard constructor
	virtual afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult );

// Dialog Data
	//{{AFX_DATA(CGzddy)
	enum { IDD = IDD_DIALOG_GZDDY };
	int		m_zxzt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGzddy)
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
	//{{AFX_MSG(CGzddy)
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLocPrint();
	afx_msg void OnLocPrintPreview();
	afx_msg void OnButtonAll();
	afx_msg void OnSelchangeComboTimemode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GZDDY_H__4CA4D3AB_28AD_4596_BF67_ED0B75579B13__INCLUDED_)
