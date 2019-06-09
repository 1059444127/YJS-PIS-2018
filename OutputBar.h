// OutputBar.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPUTBAR_H__0E6F7E9B_9E93_46C8_A749_717C1D5FE308__INCLUDED_)
#define AFX_OUTPUTBAR_H__0E6F7E9B_9E93_46C8_A749_717C1D5FE308__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// COutputBar dock window class
#include "MeasureDialog.h"

class COutputBar : public CXTDockWindow
{
	DECLARE_DYNAMIC(COutputBar)

// Construction / destruction
public:
	COutputBar();
	virtual ~COutputBar();
    CMeasureDialog  m_MeasureDialog;

// Attributes
protected:

	CXTFlatTabCtrl	m_flatTabCtrl;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputBar)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(COutputBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTBAR_H__0E6F7E9B_9E93_46C8_A749_717C1D5FE308__INCLUDED_)
