#if !defined(AFX_EDITREPORT_H__1BC45FA5_0DDD_461F_85EC_2C5A73676F4F__INCLUDED_)
#define AFX_EDITREPORT_H__1BC45FA5_0DDD_461F_85EC_2C5A73676F4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditReport.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// EditReport dialog

class EditReport : public CDialog
{
// Construction
public:
	EditReport(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(EditReport)
	enum { IDD = IDD_Edit_Report };
	CString	m_Report;
	CString	m_Result;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(EditReport)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITREPORT_H__1BC45FA5_0DDD_461F_85EC_2C5A73676F4F__INCLUDED_)
