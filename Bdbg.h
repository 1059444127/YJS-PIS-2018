#if !defined(AFX_BDBG_H__23CD4F9A_6D18_49F6_8930_B9474776F900__INCLUDED_)
#define AFX_BDBG_H__23CD4F9A_6D18_49F6_8930_B9474776F900__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Bdbg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBdbg dialog
#include "MenuEdit.h"
#include "MyEdit.h"

class CNormalWord;
class CBdbg : public CXTResizeDialog
{
	DECLARE_DYNCREATE(CBdbg)

// Construction
public:
	CBdbg(CWnd* pParent = NULL);   // standard constructor

	UINT CurEdit;
	CNormalWord *pNormalWord;
	
// Dialog Data
	//{{AFX_DATA(CBdbg)
	enum { IDD = IDD_DIALOG_BDBG };
	CMenuEdit	m_ctrl_bdshys;
	CMenuEdit	m_ctrl_bdfh;
	CMenuEdit	m_ctrl_bdbgys;
	CMyEdit		m_ctrl_bdjg;
	CString	m_bdbgys;
	CString	m_bdfh;
	CString	m_bdshys;
	CString	m_bdjg;
	//}}AFX_DATA

	CXTDateTimeCtrl m_Time_Bdbgrq;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBdbg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBdbg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonSave();
	virtual void OnOK();
	afx_msg void OnPrint();
	afx_msg void OnButtonCyc();
	afx_msg void OnSetfocusEditBdjg();
	afx_msg void OnDestroy();
	afx_msg void OnSendbdyy();
	afx_msg void OnButtonBgcheck();

	afx_msg void OnButtonNewSample();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BDBG_H__23CD4F9A_6D18_49F6_8930_B9474776F900__INCLUDED_)
