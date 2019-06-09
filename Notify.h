#if !defined(AFX_NOTIFY_H__8F94039E_3AA9_4B93_BD9D_C229FE4A30EA__INCLUDED_)
#define AFX_NOTIFY_H__8F94039E_3AA9_4B93_BD9D_C229FE4A30EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Notify.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNotify dialog

class CNotify : public CDialog
{
// Construction
public:
	CNotify(CWnd* pParent = NULL);   // standard constructor
    BOOL Create();

	UINT m_TimerHandle, m_TimerHandle1;
	int  m_TimeDelay;
	CString m_Title;

// Dialog Data
	//{{AFX_DATA(CNotify)
	enum { IDD = IDD_DIALOG_NOTIFY };
	CString	m_info;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNotify)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd* m_pParent;
	int m_nID;

	// Generated message map functions
	//{{AFX_MSG(CNotify)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOTIFY_H__8F94039E_3AA9_4B93_BD9D_C229FE4A30EA__INCLUDED_)
