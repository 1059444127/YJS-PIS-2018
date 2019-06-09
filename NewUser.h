#if !defined(AFX_NEWUSER_H__4C350A7F_AD10_4150_9D0C_02F5E846A4B2__INCLUDED_)
#define AFX_NEWUSER_H__4C350A7F_AD10_4150_9D0C_02F5E846A4B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewUser.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewUser dialog

class CNewUser : public CDialog
{
// Construction
public:
	CNewUser(CWnd* pParent = NULL);   // standard constructor
	int m_status;

// Dialog Data
	//{{AFX_DATA(CNewUser)
	enum { IDD = IDD_DIALOG_NEWUSER };
	CString	m_name;
	CString	m_info;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewUser)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewUser)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWUSER_H__4C350A7F_AD10_4150_9D0C_02F5E846A4B2__INCLUDED_)
