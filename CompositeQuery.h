#if !defined(AFX_COMPOSITEQUERY_H__F0DF8D18_A4CF_4C59_8D82_2309BDC1BDA2__INCLUDED_)
#define AFX_COMPOSITEQUERY_H__F0DF8D18_A4CF_4C59_8D82_2309BDC1BDA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CompositeQuery.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCompositeQuery dialog

#include "DlgListInput.h"

class CCompositeQuery : public CDialog
{
// Construction
public:
	CCompositeQuery(CWnd* pParent = NULL);   // standard constructor

	CStringArray m_NameList;
	CStringArray m_DescriptionList;
	CStringArray m_InfoList;
	CUIntArray   m_TypeList;

    CDlgListInput  *pListInput;

// Dialog Data
	//{{AFX_DATA(CCompositeQuery)
	enum { IDD = IDD_DIALOG_COMPOSITEQUERY };
	CString	m_query;
	CString	m_content;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCompositeQuery)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CStringArray  m_ContentList;

	// Generated message map functions
	//{{AFX_MSG(CCompositeQuery)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonOr();
	afx_msg void OnButtonAnd();
	afx_msg void OnButtonAdd();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeComboField();
	afx_msg void OnListview();
	afx_msg void OnButtonClear();
	//}}AFX_MSG
	afx_msg LRESULT ListSelect(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPOSITEQUERY_H__F0DF8D18_A4CF_4C59_8D82_2309BDC1BDA2__INCLUDED_)
