#if !defined(AFX_ZDEX_H__7F5E6EC1_A93A_4092_947A_B6DBD01EE8F6__INCLUDED_)
#define AFX_ZDEX_H__7F5E6EC1_A93A_4092_947A_B6DBD01EE8F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Zdex.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CZdex dialog

class CZdex : public CXTResizeDialog
{
// Construction
public:
	CZdex(CWnd* pParent = NULL);   // standard constructor
    BOOL Create();
	void AdjustPosition(void);

// Dialog Data
	//{{AFX_DATA(CZdex)
	enum { IDD = IDD_DIALOG_ZDEX };
	CXTHyperLink	m_HypLink_Note;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZdex)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd *m_pParent;
	int m_nID;

	// Generated message map functions
	//{{AFX_MSG(CZdex)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZDEX_H__7F5E6EC1_A93A_4092_947A_B6DBD01EE8F6__INCLUDED_)
