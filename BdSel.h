#if !defined(AFX_BDSEL_H__AD33EDC0_DD38_40E8_BB2E_D236B018D85D__INCLUDED_)
#define AFX_BDSEL_H__AD33EDC0_DD38_40E8_BB2E_D236B018D85D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BdSel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBdSel dialog

class CBdSel : public CDialog
{
// Construction
public:
	CBdSel(CWnd* pParent = NULL);   // standard constructor

	CString m_TableName;
// Dialog Data
	//{{AFX_DATA(CBdSel)
	enum { IDD = IDD_DIALOG_BDSEL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBdSel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBdSel)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BDSEL_H__AD33EDC0_DD38_40E8_BB2E_D236B018D85D__INCLUDED_)
