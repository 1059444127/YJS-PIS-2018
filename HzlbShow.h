#if !defined(AFX_HZLBSHOW_H__9F0FA564_1BA8_4861_90A9_E079C932B4E2__INCLUDED_)
#define AFX_HZLBSHOW_H__9F0FA564_1BA8_4861_90A9_E079C932B4E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HzlbShow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHzlbShow dialog
#include "HzlbGrid.h"

class CHzlbShow : public CXTResizeDialog
{
// Construction
public:
	CHzlbShow(CWnd* pParent = NULL);   // standard constructor

	CHzlbGrid m_grid;

// Dialog Data
	//{{AFX_DATA(CHzlbShow)
	enum { IDD = IDD_DIALOG_HZLB };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHzlbShow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHzlbShow)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HZLBSHOW_H__9F0FA564_1BA8_4861_90A9_E079C932B4E2__INCLUDED_)
