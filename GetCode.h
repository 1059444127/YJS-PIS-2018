#if !defined(AFX_GETCODE_H__5C235D24_3A04_4B0E_965C_C01592D884AC__INCLUDED_)
#define AFX_GETCODE_H__5C235D24_3A04_4B0E_965C_C01592D884AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetCode.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGetCode dialog

class CGetCode : public CDialog
{
// Construction
public:
	CGetCode(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGetCode)
	enum { IDD = IDD_DIALOG_GETCODE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetCode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGetCode)
	virtual BOOL OnInitDialog();
	afx_msg void OnCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETCODE_H__5C235D24_3A04_4B0E_965C_C01592D884AC__INCLUDED_)
