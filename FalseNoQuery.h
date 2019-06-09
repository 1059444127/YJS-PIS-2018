#if !defined(AFX_FALSENOQUERY_H__5464B7FC_CD4B_4922_AAF0_3ED871920032__INCLUDED_)
#define AFX_FALSENOQUERY_H__5464B7FC_CD4B_4922_AAF0_3ED871920032__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FalseNoQuery.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFalseNoQuery dialog

class CFalseNoQuery : public CDialog
{
// Construction
public:
	CFalseNoQuery(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFalseNoQuery)
	enum { IDD = IDD_DIALOG_FALSENOQUERY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFalseNoQuery)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFalseNoQuery)
	afx_msg void OnQuery();
	afx_msg void OnExport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FALSENOQUERY_H__5464B7FC_CD4B_4922_AAF0_3ED871920032__INCLUDED_)
