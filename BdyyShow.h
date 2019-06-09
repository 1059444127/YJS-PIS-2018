#if !defined(AFX_BDYYSHOW_H__F0A9C5CA_7CC5_42CB_9F4F_23252653255D__INCLUDED_)
#define AFX_BDYYSHOW_H__F0A9C5CA_7CC5_42CB_9F4F_23252653255D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BdyyShow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBdyyShow dialog
#include "BdyyGrid.h"

class CBdyyShow : public CXTResizeDialog
{
// Construction
public:
	CBdyyShow(CWnd* pParent = NULL);   // standard constructor
	CBdyyGrid m_grid;

// Dialog Data
	//{{AFX_DATA(CBdyyShow)
	enum { IDD = IDD_DIALOG_BDYY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBdyyShow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBdyyShow)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonHzqr();
	afx_msg void OnButtonBbqs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BDYYSHOW_H__F0A9C5CA_7CC5_42CB_9F4F_23252653255D__INCLUDED_)
