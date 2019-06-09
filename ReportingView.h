#if !defined(AFX_REPORTINGVIEW_H__E63F92AE_10AD_4F5E_9C21_235CF4374D31__INCLUDED_)
#define AFX_REPORTINGVIEW_H__E63F92AE_10AD_4F5E_9C21_235CF4374D31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportingView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReportingView view

class CReportingView : public CFODrawView
{
public:
	CReportingView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CReportingView)

// Attributes
public:
	CWnd *pMainFrame;
	bool IsFrameZoomed;
	int m_PreDBType;
	CString m_PreBlh;
	bool m_IsPrinted;
// Operations
public:
	// Create context menu.
	virtual CMenu* CreateContextMenu();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportingView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CReportingView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CReportingView)
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnReportView();
	afx_msg void OnReportPrevious();
	afx_msg void OnReportNext();
	afx_msg void OnSelchangeComboFormat();
	afx_msg void OnReportSend();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnReportCheck();
	afx_msg void OnReportSelpic();
	afx_msg void OnReportPrint();
	afx_msg void OnReportClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTINGVIEW_H__E63F92AE_10AD_4F5E_9C21_235CF4374D31__INCLUDED_)
