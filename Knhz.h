#if !defined(AFX_KNHZ_H__F05CB0CA_BC31_424A_8C31_E18BBF0DE495__INCLUDED_)
#define AFX_KNHZ_H__F05CB0CA_BC31_424A_8C31_E18BBF0DE495__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Knhz.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKnhz dialog
#include "DalsuRichEdit.h"
#include "MenuEdit.h"

class CKnhz : public CDialog
{
// Construction
public:
	CKnhz(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CKnhz)
	enum { IDD = IDD_DIALOG_KLHZ };
	CMenuEdit	m_ctrl_hzfqr;
	CXTTabCtrl	m_tabCtrl;
	CString	m_hzfqr;
	CString	m_cyys;
	CString	m_zjyj;
	//}}AFX_DATA

	CXTDateTimeCtrl m_Time_Hzrq;
	CArray<CString,CString> PeronArray;
	CArray<SADateTime,SADateTime> DateArray;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKnhz)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDalsuRichEdit* m_pDalRich;
	CXTListBox  m_listBox;

	void RefreshTabInfo();
	UINT m_TimerHandle;

	bool IsButtonEnabled;
	// Generated message map functions
	//{{AFX_MSG(CKnhz)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButtonSave();
	virtual void OnOK();
	afx_msg void OnButtonCancel();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonModify();
	afx_msg void OnButtonDelete();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonStart();
	afx_msg void OnClickTabCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KNHZ_H__F05CB0CA_BC31_424A_8C31_E18BBF0DE495__INCLUDED_)
