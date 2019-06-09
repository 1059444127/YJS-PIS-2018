#if !defined(AFX_FMTSELECT_H__02B7B2F5_F02F_464F_93DF_0C2D19F76C79__INCLUDED_)
#define AFX_FMTSELECT_H__02B7B2F5_F02F_464F_93DF_0C2D19F76C79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FmtSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFmtSelect dialog

class CFmtSelect : public CDialog
{
// Construction
public:
	CFmtSelect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFmtSelect)
	enum { IDD = IDD_DIALOG_FMTSELECT };
	int		m_method;
	//}}AFX_DATA

	CString fmtstr;
	int m_Mode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFmtSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFmtSelect)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMTSELECT_H__02B7B2F5_F02F_464F_93DF_0C2D19F76C79__INCLUDED_)
