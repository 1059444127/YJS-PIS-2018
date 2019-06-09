#if !defined(AFX_TABDOCKWND_H__6B8156DC_C19A_441E_AE59_91E48431D1FA__INCLUDED_)
#define AFX_TABDOCKWND_H__6B8156DC_C19A_441E_AE59_91E48431D1FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabDockWnd.h : header file
//

#include "DBList.h"
#include "MyTabCtrlBar.h"

/////////////////////////////////////////////////////////////////////////////
// CTabDockWnd window

class CTabDockWnd : public CMyTabCtrlBar
{
// Construction
public:
	CTabDockWnd();

// Attributes
protected:

// Operations
public:
    CDBList          m_DBList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabDockWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTabDockWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTabDockWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	afx_msg LRESULT OnTabSelChange(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABDOCKWND_H__6B8156DC_C19A_441E_AE59_91E48431D1FA__INCLUDED_)
