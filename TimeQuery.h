#if !defined(AFX_TIMEQUERY_H__B9CADBBF_FE77_4F92_A5C9_19CE564C3F9E__INCLUDED_)
#define AFX_TIMEQUERY_H__B9CADBBF_FE77_4F92_A5C9_19CE564C3F9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimeQuery.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTimeQuery dialog

class CTimeQuery : public CDialog
{
// Construction
public:
	CTimeQuery(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTimeQuery)
	enum { IDD = IDD_DIALOG_TIMEQUERY };
	COleDateTime	m_enddate;
	COleDateTime	m_endtime;
	COleDateTime	m_startdate;
	COleDateTime	m_starttime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimeQuery)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTimeQuery)
	afx_msg void OnButtonToday();
	afx_msg void OnButtonYesterday();
	afx_msg void OnButtonThree();
	afx_msg void OnButtonWeek();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMEQUERY_H__B9CADBBF_FE77_4F92_A5C9_19CE564C3F9E__INCLUDED_)
