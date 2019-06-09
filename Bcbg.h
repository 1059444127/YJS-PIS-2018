#if !defined(AFX_BCBG_H__A3E0FC5D_D9A6_41D9_963D_4A99F5F8A4CE__INCLUDED_)
#define AFX_BCBG_H__A3E0FC5D_D9A6_41D9_963D_4A99F5F8A4CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Bcbg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBcbg dialog
#include "MenuEdit.h"

class CNormalWord;
class CBcbg : public CXTResizeDialog
{
	DECLARE_DYNCREATE(CBcbg)

// Construction
public:
	CBcbg(CWnd* pParent = NULL);   // standard constructor

	UINT CurEdit;
	CNormalWord *pNormalWord;

// Dialog Data
	//{{AFX_DATA(CBcbg)
	enum { IDD = IDD_DIALOG_BCBG };
	CMenuEdit	m_ctrl_bcbgys;
	CString	m_bcbgys;
	CString	m_bczd;
	//}}AFX_DATA

	CXTDateTimeCtrl m_Time_Bcbgrq;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBcbg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBcbg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonSave();
	virtual void OnOK();
	afx_msg void OnButtonCyc();
	afx_msg void OnSetfocusEditBczd();
	afx_msg void OnDestroy();
	afx_msg void OnPrint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCBG_H__A3E0FC5D_D9A6_41D9_963D_4A99F5F8A4CE__INCLUDED_)
