#if !defined(AFX_ADVICEDELREASON_H__E98B8FAE_37A4_4E0D_841B_CACD8A6E5E58__INCLUDED_)
#define AFX_ADVICEDELREASON_H__E98B8FAE_37A4_4E0D_841B_CACD8A6E5E58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdviceDelReason.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdviceDelReason dialog

class CAdviceDelReason : public CDialog
{
// Construction
public:
	CAdviceDelReason(CWnd* pParent = NULL);   // standard constructor
	
	CString m_reason;
// Dialog Data
	//{{AFX_DATA(CAdviceDelReason)
	enum { IDD = IDD_DIALOG_DELETEREASON };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdviceDelReason)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdviceDelReason)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVICEDELREASON_H__E98B8FAE_37A4_4E0D_841B_CACD8A6E5E58__INCLUDED_)
