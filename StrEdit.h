#if !defined(AFX_STREDIT_H__9B83EF72_5D6D_4481_A9F9_2E440D2701D5__INCLUDED_)
#define AFX_STREDIT_H__9B83EF72_5D6D_4481_A9F9_2E440D2701D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StrEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// StrEdit dialog

#include "DlgListInput.h"

class StrEdit : public CDialog
{
// Construction
public:
	StrEdit(CWnd* pParent = NULL);	// standard constructor
	StrEdit(CString Title,CWnd* pParent = NULL);

	CString  m_FieldName, m_FieldInfo;
    CDlgListInput  *pListInput;

	int m_type;

// Dialog Data
	//{{AFX_DATA(StrEdit)
	enum { IDD = IDD_StrEdit };
	CString	m_EditStr;
	//}}AFX_DATA

// Implementation
protected:
	CString m_Title;
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(StrEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnListview();
	//}}AFX_MSG
	afx_msg LRESULT ListSelect(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STREDIT_H__9B83EF72_5D6D_4481_A9F9_2E440D2701D5__INCLUDED_)
