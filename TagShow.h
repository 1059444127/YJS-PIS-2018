#if !defined(AFX_TAGSHOW_H__5C442F4D_DF47_488D_A32E_DE8BB56DF5A0__INCLUDED_)
#define AFX_TAGSHOW_H__5C442F4D_DF47_488D_A32E_DE8BB56DF5A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TagShow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTagShow dialog
#include "TagGrid.h"

class CTagShow : public CXTResizeDialog
{
// Construction
public:
	CTagShow(CWnd* pParent = NULL);   // standard constructor

	CTagGrid    m_TagList;

	int m_UsageType;
	CString m_TagString;

// Dialog Data
	//{{AFX_DATA(CTagShow)
	enum { IDD = IDD_DIALOG_TAG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTagShow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void RefreshTcList();

	// Generated message map functions
	//{{AFX_MSG(CTagShow)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAddbjw();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonAddtc();
	afx_msg void OnSelchangeComboTc();
	virtual void OnOK();
	afx_msg void OnButtonDeletetc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TAGSHOW_H__5C442F4D_DF47_488D_A32E_DE8BB56DF5A0__INCLUDED_)
