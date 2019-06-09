#if !defined(AFX_FIELDOUT_H__D17D4E30_A15B_46A9_915A_690A30AC21CB__INCLUDED_)
#define AFX_FIELDOUT_H__D17D4E30_A15B_46A9_915A_690A30AC21CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FieldOut.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFieldOut dialog

class CFieldOut : public CDialog
{
// Construction
public:
	CFieldOut(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFieldOut)
	enum { IDD = IDD_DIALOG_FIELDSELECT };
	int		m_Index;
	//}}AFX_DATA
    
	CStringArray  strlist;
	CString title;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFieldOut)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFieldOut)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIELDOUT_H__D17D4E30_A15B_46A9_915A_690A30AC21CB__INCLUDED_)
