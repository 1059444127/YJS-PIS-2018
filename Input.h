#if !defined(AFX_INPUT_H__73C51781_7F9E_4F68_84C0_4092A58AE53F__INCLUDED_)
#define AFX_INPUT_H__73C51781_7F9E_4F68_84C0_4092A58AE53F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Input.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInput dialog

class CInput : public CDialog
{
// Construction
public:
	CInput(CWnd* pParent = NULL);   // standard constructor
	CString m_blh;
// Dialog Data
	//{{AFX_DATA(CInput)
	enum { IDD = IDD_DIALOG_INPUT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInput)
	afx_msg void OnInputOk();
	afx_msg void OnInputCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUT_H__73C51781_7F9E_4F68_84C0_4092A58AE53F__INCLUDED_)
