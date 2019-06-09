#if !defined(AFX_DLGREFLECTION_H__F3DE33B6_7F7B_4C95_A2BE_9D2D050C9D1D__INCLUDED_)
#define AFX_DLGREFLECTION_H__F3DE33B6_7F7B_4C95_A2BE_9D2D050C9D1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgReflection.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgReflection dialog

#include "BitmapPicture.h"

class CDlgReflection : public CDialog
{
// Construction
public:
	CDlgReflection(CWnd* pParent = NULL);   // standard constructor
	~CDlgReflection();

	CStringArray m_File_List;

// Dialog Data
	//{{AFX_DATA(CDlgReflection)
	enum { IDD = IDD_DIALOG_REFLECTION };
	CBitmapPicture	m_staticImage2;
	CBitmapPicture	m_staticImage1;
	int		m_nReflectStyle;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgReflection)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgReflection)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioReflectStyle();
	virtual void OnOK();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGREFLECTION_H__F3DE33B6_7F7B_4C95_A2BE_9D2D050C9D1D__INCLUDED_)
