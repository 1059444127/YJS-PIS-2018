#if !defined(AFX_SECURITY_H__581C376F_3306_413C_99BD_12B77EADCE5D__INCLUDED_)
#define AFX_SECURITY_H__581C376F_3306_413C_99BD_12B77EADCE5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Security.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSecurity dialog

class CSecurity : public CDialog
{
// Construction
public:
	CSecurity(CWnd* pParent = NULL);   // standard constructor

	CString m_DBName;
	int m_DBNo;

// Dialog Data
	//{{AFX_DATA(CSecurity)
	enum { IDD = IDD_DIALOG_SECURITY };
	CXTButton	m_enroll;
	CString	m_password;
	CString	m_user;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSecurity)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ActiveItem(BOOL value);
	bool IsConnectable();
	CStringArray  m_UserNameList;

    CToolTipCtrl     m_ToolTip;

	// VeriDicom Parameters
	UINT m_TimerHandle_VeriDicom;
	unsigned char *m_pDisplayImageData;

	COleDateTime m_Time;
	// Generated message map functions
	//{{AFX_MSG(CSecurity)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPass();
	afx_msg void OnManage();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEnroll();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECURITY_H__581C376F_3306_413C_99BD_12B77EADCE5D__INCLUDED_)
