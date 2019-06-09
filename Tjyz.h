#if !defined(AFX_TJYZ_H__12EF5778_4A57_4602_8ACB_3E2C5467FF41__INCLUDED_)
#define AFX_TJYZ_H__12EF5778_4A57_4602_8ACB_3E2C5467FF41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Tjyz.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTjyz dialog
#include "AdvicePreGrid.h"

class CTjyz : public CXTResizeDialog
{
// Construction
public:
	CTjyz(CWnd* pParent = NULL);   // standard constructor
    CWnd *m_pParent;
	CAdvicePreGrid  m_AdviceList;
    CString GenNewAdviceNo(CString blh);

// Dialog Data
	//{{AFX_DATA(CTjyz)
	enum { IDD = IDD_DIALOG_TJYZ };
	CXTButton	m_btBlh;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTjyz)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTjyz)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonSelbjw();
	afx_msg void OnButtonTbbjw();
	afx_msg void OnButtonYzqr();
	afx_msg void OnButtonBlh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TJYZ_H__12EF5778_4A57_4602_8ACB_3E2C5467FF41__INCLUDED_)
