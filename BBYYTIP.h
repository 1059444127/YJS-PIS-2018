#if !defined(AFX_BBYYTIP_H__B34F9439_6A60_4E5F_A49D_0B9A4A99E2C2__INCLUDED_)
#define AFX_BBYYTIP_H__B34F9439_6A60_4E5F_A49D_0B9A4A99E2C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BBYYTIP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBBYYTIP dialog

class CBBYYTIP : public CDialog
{
// Construction
public:
	CBBYYTIP(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBBYYTIP)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBBYYTIP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBBYYTIP)
	afx_msg void OnButtonBbyytip();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BBYYTIP_H__B34F9439_6A60_4E5F_A49D_0B9A4A99E2C2__INCLUDED_)
