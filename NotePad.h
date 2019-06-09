#if !defined(AFX_NOTEPAD_H__46F58707_8DE7_4AA2_8D7D_822B9B0F563D__INCLUDED_)
#define AFX_NOTEPAD_H__46F58707_8DE7_4AA2_8D7D_822B9B0F563D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotePad.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNotePad dialog

class CNotePad : public CDialog
{
// Construction
public:
	CNotePad(CWnd* pParent = NULL);   // standard constructor
	CString status;
// Dialog Data
	//{{AFX_DATA(CNotePad)
	enum { IDD = IDD_DIALOG_NOTEPAD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNotePad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNotePad)
	virtual BOOL OnInitDialog();
	afx_msg void OnClear();
	afx_msg void OnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOTEPAD_H__46F58707_8DE7_4AA2_8D7D_822B9B0F563D__INCLUDED_)
