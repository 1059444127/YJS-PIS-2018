#if !defined(AFX_ADVICETIP_H__AE06FC54_E0AF_4E58_9B9F_960D0C73C4AE__INCLUDED_)
#define AFX_ADVICETIP_H__AE06FC54_E0AF_4E58_9B9F_960D0C73C4AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdviceTip.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdviceTip dialog

class CAdviceTip : public CDialog
{
// Construction
public:
	CAdviceTip(CWnd* pParent = NULL);   // standard constructor
	CString count;
	CString countTG;
// Dialog Data
	//{{AFX_DATA(CAdviceTip)
	enum { IDD = IDD_DIALOG_AdviceTip };
	CXTHyperLink m_link_advice;
	CXTHyperLink m_link_tg;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdviceTip)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdviceTip)
	afx_msg void OnLinkAdvice();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVICETIP_H__AE06FC54_E0AF_4E58_9B9F_960D0C73C4AE__INCLUDED_)
