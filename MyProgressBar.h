#if !defined(AFX_MYPROGRESSBAR_H__C23656B2_31E9_4E7E_814A_DB9A0811CC0E__INCLUDED_)
#define AFX_MYPROGRESSBAR_H__C23656B2_31E9_4E7E_814A_DB9A0811CC0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyProgressBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyProgressBar dialog

class CMyProgressBar : public CDialog
{
// Construction
public:
	CMyProgressBar(CWnd* pParent = NULL);   // standard constructor
    BOOL Create();

// Dialog Data
	//{{AFX_DATA(CMyProgressBar)
	enum { IDD = IDD_DIALOG_PROGRESSBAR };
	CProgressCtrl	m_bar;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyProgressBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd* m_pParent;
	int m_nID;

	// Generated message map functions
	//{{AFX_MSG(CMyProgressBar)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPROGRESSBAR_H__C23656B2_31E9_4E7E_814A_DB9A0811CC0E__INCLUDED_)
