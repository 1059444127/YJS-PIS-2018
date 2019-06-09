#if !defined(AFX_PASSMODIFY_H__27692EDC_E6AA_48AF_B82C_B3CE41E59C78__INCLUDED_)
#define AFX_PASSMODIFY_H__27692EDC_E6AA_48AF_B82C_B3CE41E59C78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PassModify.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPassModify dialog

class CPassModify : public CDialog
{
// Construction
public:
	CPassModify(CWnd* pParent = NULL);   // standard constructor

	CString m_title;

// Dialog Data
	//{{AFX_DATA(CPassModify)
	enum { IDD = IDD_DIALOG_PASSINPUT };
	CString	m_ps1;
	CString	m_ps2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPassModify)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPassModify)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSMODIFY_H__27692EDC_E6AA_48AF_B82C_B3CE41E59C78__INCLUDED_)
