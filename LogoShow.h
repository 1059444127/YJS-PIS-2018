#if !defined(AFX_LOGOSHOW_H__EEAB9B28_E97E_445D_A8EA_E8D8FE35E8EF__INCLUDED_)
#define AFX_LOGOSHOW_H__EEAB9B28_E97E_445D_A8EA_E8D8FE35E8EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogoShow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogoShow dialog
#include "LogoGrid.h"

class CLogoShow : public CXTResizeDialog
{
// Construction
public:
	CLogoShow(CWnd* pParent = NULL);   // standard constructor

	CLogoGrid m_grid;
// Dialog Data
	//{{AFX_DATA(CLogoShow)
	enum { IDD = IDD_DIALOG_LOGO };
	CXTHyperLink	m_HypLink_Note;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogoShow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CXTDateTimeCtrl m_Time_Start, m_Time_End;

	// Generated message map functions
	//{{AFX_MSG(CLogoShow)
	virtual BOOL OnInitDialog();
	afx_msg void OnDelete();
	afx_msg void OnButtonQuery();
	afx_msg void OnCheckSelectall();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGOSHOW_H__EEAB9B28_E97E_445D_A8EA_E8D8FE35E8EF__INCLUDED_)
