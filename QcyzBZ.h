#if !defined(AFX_QCYZBZ_H__72636543_D708_4537_83DE_575D5318CF65__INCLUDED_)
#define AFX_QCYZBZ_H__72636543_D708_4537_83DE_575D5318CF65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QcyzBZ.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CQcyzBZ dialog

class CQcyzBZ : public CDialog
{
// Construction
public:
	CQcyzBZ(CWnd* pParent = NULL);   // standard constructor
	CString m_qcyz;
// Dialog Data
	//{{AFX_DATA(CQcyzBZ)
	enum { IDD = IDD_DIALOG_QCYZBZ };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQcyzBZ)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CQcyzBZ)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnCancelMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QCYZBZ_H__72636543_D708_4537_83DE_575D5318CF65__INCLUDED_)
