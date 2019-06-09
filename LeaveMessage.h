#if !defined(AFX_LEAVEMESSAGE_H__4C3B4B7F_0754_4D61_A44A_4BDDDBBC7288__INCLUDED_)
#define AFX_LEAVEMESSAGE_H__4C3B4B7F_0754_4D61_A44A_4BDDDBBC7288__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LeaveMessage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLeaveMessage dialog

class CLeaveMessage : public CDialog
{
// Construction
public:
	CLeaveMessage(CWnd* pParent = NULL);   // standard constructor
	CString count;
// Dialog Data
	//{{AFX_DATA(CLeaveMessage)
	enum { IDD = IDD_DIALOG_LeaveMessage };
	CXTHyperLink m_link_ly;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeaveMessage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLeaveMessage)
	afx_msg void OnButtonLy();
	afx_msg void OnLinkLy();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEAVEMESSAGE_H__4C3B4B7F_0754_4D61_A44A_4BDDDBBC7288__INCLUDED_)
