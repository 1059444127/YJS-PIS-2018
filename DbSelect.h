#if !defined(AFX_DBSELECT_H__8173C2A1_A8D4_487D_8EE8_A8383EECDD5F__INCLUDED_)
#define AFX_DBSELECT_H__8173C2A1_A8D4_487D_8EE8_A8383EECDD5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DbSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDbSelect dialog

class CDbSelect : public CDialog
{
// Construction
public:
	CDbSelect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDbSelect)
	enum { IDD = IDD_DIALOG_DBSELECT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CStringArray m_DBName_list;
    CUIntArray   m_DBNo_list;

	CString m_DBName;
	int m_DBNo;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDbSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDbSelect)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBSELECT_H__8173C2A1_A8D4_487D_8EE8_A8383EECDD5F__INCLUDED_)
