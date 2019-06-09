#if !defined(AFX_DBFSTANDARD_H__3FA7FCBA_8187_4E0F_BFD1_F78C7F490795__INCLUDED_)
#define AFX_DBFSTANDARD_H__3FA7FCBA_8187_4E0F_BFD1_F78C7F490795__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DbfStandard.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDbfStandard dialog

class CDbfStandard : public CDialog
{
// Construction
public:
	CDbfStandard(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDbfStandard)
	enum { IDD = IDD_DIALOG_STANDARD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDbfStandard)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);

	// Generated message map functions
	//{{AFX_MSG(CDbfStandard)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDeleteAll();
	afx_msg void OnDelete();
	afx_msg void OnGet();
	afx_msg void OnDblclkListDbf(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBFSTANDARD_H__3FA7FCBA_8187_4E0F_BFD1_F78C7F490795__INCLUDED_)
