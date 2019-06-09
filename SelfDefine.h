#if !defined(AFX_SELFDEFINE_H__723063F0_7EC7_4CE9_8AB4_5A3747E78AA8__INCLUDED_)
#define AFX_SELFDEFINE_H__723063F0_7EC7_4CE9_8AB4_5A3747E78AA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelfDefine.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelfDefine dialog

class CSelfDefine : public CDialog
{
// Construction
public:
	CSelfDefine(CWnd* pParent = NULL);   // standard constructor

	int m_UsageType;

// Dialog Data
	//{{AFX_DATA(CSelfDefine)
	enum { IDD = IDD_DIALOG_SELFDEFINE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelfDefine)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelfDefine)
	afx_msg void OnButtonSel();
	afx_msg void OnButtonUnsel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPrevious();
	afx_msg void OnNext();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELFDEFINE_H__723063F0_7EC7_4CE9_8AB4_5A3747E78AA8__INCLUDED_)
