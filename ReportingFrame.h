#if !defined(AFX_REPORTINGFRAME_H__71C88216_2D05_491F_9BE1_105F65DC9D0F__INCLUDED_)
#define AFX_REPORTINGFRAME_H__71C88216_2D05_491F_9BE1_105F65DC9D0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportingFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReportingFrame frame

class CReportingFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CReportingFrame)
public:
	CReportingFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	CXTToolBar    m_wndToolBar;
	CEdit         m_wndEditBlh;
    CComboBox	  m_wndComboFormat;
	CButton		  m_selPic;
	CButton		  m_CloseWnd;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportingFrame)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CReportingFrame();

	// Generated message map functions
	//{{AFX_MSG(CReportingFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTINGFRAME_H__71C88216_2D05_491F_9BE1_105F65DC9D0F__INCLUDED_)
