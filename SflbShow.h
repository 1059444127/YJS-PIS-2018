#if !defined(AFX_SFLBSHOW_H__9FB922B5_451C_48EE_B56E_826F03CF93E1__INCLUDED_)
#define AFX_SFLBSHOW_H__9FB922B5_451C_48EE_B56E_826F03CF93E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SflbShow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSflbShow dialog
#include "SflbGrid.h"

class CSflbShow : public CXTResizeDialog
{
// Construction
public:
	CSflbShow(CWnd* pParent = NULL);   // standard constructor

	CSflbGrid m_grid;

// Dialog Data
	//{{AFX_DATA(CSflbShow)
	enum { IDD = IDD_DIALOG_SFLB };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSflbShow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSflbShow)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SFLBSHOW_H__9FB922B5_451C_48EE_B56E_826F03CF93E1__INCLUDED_)
