#if !defined(AFX_NOUSE_H__EE3B0DFE_3311_47E0_A6D2_FB25F949559F__INCLUDED_)
#define AFX_NOUSE_H__EE3B0DFE_3311_47E0_A6D2_FB25F949559F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NoUse.h : header file
//
#include "Wax2.h"
/////////////////////////////////////////////////////////////////////////////
// CNoUse dialog


class CNoUse : public CDialog
{
// Construction
public:
	CNoUse(CWnd* pParent = NULL);   // standard constructor
	CWax2 wax;
// Dialog Data
	//{{AFX_DATA(CNoUse)
	enum { IDD = IDD_DIALOG_NOUSE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNoUse)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNoUse)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOUSE_H__EE3B0DFE_3311_47E0_A6D2_FB25F949559F__INCLUDED_)
