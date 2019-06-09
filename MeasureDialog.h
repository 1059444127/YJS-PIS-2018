#if !defined(AFX_MEASUREDIALOG_H__63E7B116_8028_42F4_893E_CBB5969BE915__INCLUDED_)
#define AFX_MEASUREDIALOG_H__63E7B116_8028_42F4_893E_CBB5969BE915__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeasureDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMeasureDialog dialog

#include "MeasureGrid.h"

class CMeasureDialog : public CXTCBarDialog
{
// Construction
public:
	CMeasureDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMeasureDialog)
	enum { IDD = IDD_DIALOG_MEASURE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeasureDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	//CXTToolBar    m_wndToolBar;
	CMeasureGrid  m_MeasureCtrl;
	
	// Generated message map functions
	//{{AFX_MSG(CMeasureDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEASUREDIALOG_H__63E7B116_8028_42F4_893E_CBB5969BE915__INCLUDED_)
