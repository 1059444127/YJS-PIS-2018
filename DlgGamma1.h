#if !defined(AFX_DLGGAMMA1_H__6A313604_BB8A_44D0_B497_2FB598A9D88C__INCLUDED_)
#define AFX_DLGGAMMA1_H__6A313604_BB8A_44D0_B497_2FB598A9D88C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGamma1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGamma1 dialog

class CDlgGamma1 : public CDialog
{
// Construction
public:
	CDlgGamma1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGamma1)
	enum { IDD = IDD_DLG_GAMMA };
	double	m_dGammRatio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGamma1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGamma1)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGAMMA1_H__6A313604_BB8A_44D0_B497_2FB598A9D88C__INCLUDED_)
