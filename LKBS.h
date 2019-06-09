#if !defined(AFX_LKBS_H__57D3269A_6D01_4251_8621_418C05365805__INCLUDED_)
#define AFX_LKBS_H__57D3269A_6D01_4251_8621_418C05365805__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LKBS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLKBS dialog

class CLKBS : public CDialog
{
// Construction
public:
	CLKBS(CWnd* pParent = NULL);   // standard constructor
	CString m_lkbs;
// Dialog Data
	//{{AFX_DATA(CLKBS)
	enum { IDD = IDD_DIALOG_LKBS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLKBS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLKBS)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LKBS_H__57D3269A_6D01_4251_8621_418C05365805__INCLUDED_)
