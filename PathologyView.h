// PathologyView.h : interface of the CPathologyView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATHOLOGYVIEW_H__307B2C9A_2A33_427D_8133_92408CBE4A39__INCLUDED_)
#define AFX_CPI_PATHOLOGYVIEW_H__307B2C9A_2A33_427D_8133_92408CBE4A39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FormDataModel.h"
#include "resource.h"
#include "ImageDockWnd.h"
class CDummyView;
class CPathologyDoc;

class CPathologyView : public CXTResizeFormView
{
protected: // create from serialization only
	CPathologyView();
	DECLARE_DYNCREATE(CPathologyView)

public:
	//{{AFX_DATA(CPathologyView)
	enum { IDD = IDD_PATHOLOGY_FORM };
	CXTTabCtrl	m_tabCtrl;
	//}}AFX_DATA

// Attributes
public:
	CPathologyDoc* GetDocument();
	void LoadImageFile(CString, bool);

	void ForceSave();

	BOOL m_OnPreparePrinting_Return;

// Operations
public:
	CFormDataModel  *m_pFormDataModel;
	CSingleDocTemplate*	m_pTemplate;
	bool		m_bPrintPreview;
	CDummyView* m_pView;
	void EndPreviewMode();

    CImageDockWnd m_ImageDockWnd;
	void CloseNormalWordWnd();
	bool LoadReportFormat(CString &);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPathologyView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPathologyView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CImageList	m_TabImageList;
	int InitTabCtrl();
	int AddTabViews();

	
	void DuplicateRecord(bool IsAllInfo, bool IsOnlyImg);

private:
	bool	m_bIsFirstInit;

// Generated message map functions
protected:
	//{{AFX_MSG(CPathologyView)
	afx_msg void OnFileSave();
	afx_msg void OnClickTabCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDocQuickinput();
	afx_msg void OnUpdateDocQuickinput(CCmdUI* pCmdUI);
	afx_msg void OnFileRptcopy();
	afx_msg void OnFileRptcopyall();
	afx_msg void OnFileRptcopyimg();
	//}}AFX_MSG
	afx_msg void OnViewPrint();
	afx_msg void OnViewPrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PathologyView.cpp
inline CPathologyDoc* CPathologyView::GetDocument()
   { return (CPathologyDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATHOLOGYVIEW_H__307B2C9A_2A33_427D_8133_92408CBE4A39__INCLUDED_)
