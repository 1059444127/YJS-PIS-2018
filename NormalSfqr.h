#if !defined(AFX_NORMALSFQR_H__7A462517_87E3_4FDB_8C05_B8F55570F715__INCLUDED_)
#define AFX_NORMALSFQR_H__7A462517_87E3_4FDB_8C05_B8F55570F715__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NormalSfqr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNormalSfqr dialog
#include "CgsfqrGrid.h"

class CNormalSfqr : public CXTResizeDialog
{
	DECLARE_DYNCREATE(CNormalSfqr)

// Construction
public:
	CNormalSfqr(CWnd* pParent = NULL);   // standard constructor
	void PerformQuery();

// Dialog Data
	//{{AFX_DATA(CNormalSfqr)
	enum { IDD = IDD_DIALOG_CGSFQR };
	CXTHyperLink	m_HypLink_Note;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNormalSfqr)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CXTDateTimeCtrl m_Time_Qcrq;
	CCgsfqrGrid  m_WaxList;

	// Generated message map functions
	//{{AFX_MSG(CNormalSfqr)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonQuery();
	afx_msg void OnButtonDefaultquery();
	afx_msg void OnCheckSelectall();
	afx_msg void OnButtonYzqr();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NORMALSFQR_H__7A462517_87E3_4FDB_8C05_B8F55570F715__INCLUDED_)
