#if !defined(AFX_KNLYSHOW_H__A443AFE1_8874_459A_A162_1C84E45515C9__INCLUDED_)
#define AFX_KNLYSHOW_H__A443AFE1_8874_459A_A162_1C84E45515C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KnlyShow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKnlyShow dialog
#include "KnlyGrid.h"

class CKnlyShow : public CXTResizeDialog
{
// Construction
public:
	CKnlyShow(CWnd* pParent = NULL);   // standard constructor

	CKnlyGrid m_grid;
// Dialog Data
	//{{AFX_DATA(CKnlyShow)
	enum { IDD = IDD_DIALOG_KNLY };
	CXTHyperLink	m_HypLink_Note;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKnlyShow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKnlyShow)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KNLYSHOW_H__A443AFE1_8874_459A_A162_1C84E45515C9__INCLUDED_)
