#if !defined(AFX_SELECTBZ_H__4098815E_6D3C_4064_96D9_E120195D80C4__INCLUDED_)
#define AFX_SELECTBZ_H__4098815E_6D3C_4064_96D9_E120195D80C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectBZ.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectBZ dialog

class CSelectBZ : public CDialog
{
// Construction
public:
	CSelectBZ(CWnd* pParent = NULL);   // standard constructor
	CSelectBZ(CWnd* pParent ,CString *See,CString *Result);

// Dialog Data
	//{{AFX_DATA(CSelectBZ)
	enum { IDD = IDD_SECECTBZ };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectBZ)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString *m_See,*m_Result;
	CWnd *m_pWnd;
	CStringArray BuweiBZName;
	CWordArray BuweiBZLen;

	// Generated message map functions
	//{{AFX_MSG(CSelectBZ)
	afx_msg void OnSelchangeList1();
	afx_msg void OnDblclkList2();
	afx_msg void OnDblclkList3();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTBZ_H__4098815E_6D3C_4064_96D9_E120195D80C4__INCLUDED_)
