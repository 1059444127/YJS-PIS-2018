#if !defined(AFX_PASSMANAGE_H__D034F7DB_49C8_4E47_8FBE_CC3E9183C5EC__INCLUDED_)
#define AFX_PASSMANAGE_H__D034F7DB_49C8_4E47_8FBE_CC3E9183C5EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PassManage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPassManage dialog
#include "UserGrid.h"
#include "RightGrid.h"

class CPassManage : public CXTCBarDialog
{
// Construction
public:
	CPassManage(CWnd* pParent = NULL);   // standard constructor

	virtual afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult );

    CUserGrid   m_UserCtrl;
	CRightGrid  m_RightCtrl;

    int m_UserIn_Right;

// Dialog Data
	//{{AFX_DATA(CPassManage)
	enum { IDD = IDD_DIALOG_PASSMANAGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPassManage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CXTToolBar    m_wndToolBar;

	void PreSetValue(int value);

	BOOL GetToolText( UINT nID, CString& strTipText, CString& /*strMessage*/);

	// Generated message map functions
	//{{AFX_MSG(CPassManage)
	virtual BOOL OnInitDialog();
	afx_msg void OnUserGen();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnUserDelete();
	afx_msg void OnUserModify();
	afx_msg void OnUser1();
	afx_msg void OnUser2();
	afx_msg void OnUser3();
	afx_msg void OnUser4();
	//}}AFX_MSG
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSMANAGE_H__D034F7DB_49C8_4E47_8FBE_CC3E9183C5EC__INCLUDED_)
