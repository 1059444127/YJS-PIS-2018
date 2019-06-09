#if !defined(StatisticDummyView__INCLUDED_)
#define StatisticDummyView__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticDummyView.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDlgDummyView view
class CPrintPreviewDialog;

class CDlgDummyView : public CFODrawView
{
public:
	CDlgDummyView();
	virtual ~CDlgDummyView();
protected:
	DECLARE_DYNCREATE(CDlgDummyView)

// Attributes
public:
	void		SetPrintDialog(CPrintPreviewDialog* pDialog);
	void		OnDoPrintPreview();
	void		OnDoFilePrint();
	CFrameWnd*	m_pOldFrame;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDummyView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
	//}}AFX_VIRTUAL
// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CPrintPreviewDialog*	m_pDialog;
	// Generated message map functions
protected:
	//{{AFX_MSG(CDlgDummyView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	friend class CStatisticMyPreviewView;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(StatisticDummyView__INCLUDED_)
