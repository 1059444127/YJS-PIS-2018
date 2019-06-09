#if !defined(AFX_FIGUREVIEW_H__192FCE26_969E_11D4_AAE5_004005524B5F__INCLUDED_)
#define AFX_FIGUREVIEW_H__192FCE26_969E_11D4_AAE5_004005524B5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FigureView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFigureView view
class CFigureView : public CScrollView
{
// Attributes
public:
	CFigureView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFigureView)
	
	BOOL m_bStretch;
	HDIB m_hDIB;
    BOOL LoadImage (CString);

	int m_raster_xsize, m_raster_ysize;

	CString m_blh;
	int m_SerialNo, m_MaxSerialNo;
	bool TryNew(int);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFigureView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFigureView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFigureView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIGUREVIEW_H__192FCE26_969E_11D4_AAE5_004005524B5F__INCLUDED_)
