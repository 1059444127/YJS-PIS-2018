#if !defined(AFX_NORMALWORD_H__F41BB35C_C28B_46B0_A7E0_3595244ACFBA__INCLUDED_)
#define AFX_NORMALWORD_H__F41BB35C_C28B_46B0_A7E0_3595244ACFBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NormalWord.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNormalWord dialog

class CNormalWord : public CXTResizeDialog
{
// Construction
public:
	CNormalWord(CWnd* pParent = NULL);   // standard constructor

    BOOL Create();

	void AdjustPosition(void);
	CString m_Buwei;
	
// Dialog Data
	//{{AFX_DATA(CNormalWord)
	enum { IDD = IDD_NORMALWORD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNormalWord)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd *m_pParent;
	int m_nID;

	BOOL LoadList(void);

	// Generated message map functions
	//{{AFX_MSG(CNormalWord)
	afx_msg void OnDblclkList1();
	afx_msg void OnDblclkList2();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSwitch();
	afx_msg void OnSelchangeList4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NORMALWORD_H__F41BB35C_C28B_46B0_A7E0_3595244ACFBA__INCLUDED_)
