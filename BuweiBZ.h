#if !defined(AFX_BUWEIBZ_H__2B8EB81E_603E_497E_A3C4_30215991A2D6__INCLUDED_)
#define AFX_BUWEIBZ_H__2B8EB81E_603E_497E_A3C4_30215991A2D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BuweiBZ.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// BuweiBZ dialog

class BuweiBZ : public CDialog
{
// Construction
public:
	BuweiBZ(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(BuweiBZ)
	enum { IDD = IDD_BuweiBZ };
	int		m_Buwei;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();
	
	CStringArray BuweiBZName;
	CWordArray BuweiBZLen;
	
	// Generated message map functions
	//{{AFX_MSG(BuweiBZ)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	virtual void OnOK();
	afx_msg void OnSelchangeList1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUWEIBZ_H__2B8EB81E_603E_497E_A3C4_30215991A2D6__INCLUDED_)
