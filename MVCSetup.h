#if !defined(AFX_MVCSETUP_H__28511374_00DB_4125_A740_2A53B52DDB0A__INCLUDED_)
#define AFX_MVCSETUP_H__28511374_00DB_4125_A740_2A53B52DDB0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MVCSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMVCSetup dialog
#include "mvcapi.h"

class CMVCSetup : public CDialog
{
// Construction
public:
	CMVCSetup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMVCSetup)
	enum { IDD = IDD_DIALOG_MVC300STP };
	int		m_Size;
	BOOL	m_AE;
	int		m_Sensitive;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMVCSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CapInfoStruct m_TempCapInfo;

	// Generated message map functions
	//{{AFX_MSG(CMVCSetup)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnCancel();
	afx_msg void OnCheckAe();
	afx_msg void OnRadio9();
	afx_msg void OnRadio10();
	afx_msg void OnRadio11();
	afx_msg void OnCheckRgbbgr();
	afx_msg void OnCheckFlip();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MVCSETUP_H__28511374_00DB_4125_A740_2A53B52DDB0A__INCLUDED_)
