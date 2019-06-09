#if !defined(AFX_LAUCHWAX_H__B82A14F5_81B5_4A71_A9DB_2917700D4E8E__INCLUDED_)
#define AFX_LAUCHWAX_H__B82A14F5_81B5_4A71_A9DB_2917700D4E8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LauchWax.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLauchWax dialog
#include "MedWax.h"

class CLauchWax : public CDialog
{
// Construction
public:
	CLauchWax(CWnd* pParent = NULL);   // standard constructor

	CMedWax wax;
// Dialog Data
	//{{AFX_DATA(CLauchWax)
	enum { IDD = IDD_DIALOG_GOWAX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLauchWax)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLauchWax)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAUCHWAX_H__B82A14F5_81B5_4A71_A9DB_2917700D4E8E__INCLUDED_)
