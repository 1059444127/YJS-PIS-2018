#if !defined(AFX_IMAGEVIEW_H__C6138052_1803_4698_B55D_966D4BCE407F__INCLUDED_)
#define AFX_IMAGEVIEW_H__C6138052_1803_4698_B55D_966D4BCE407F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageView view
class CImageDockWnd;

class CImageView : public CFODrawView
{
protected:
	CImageView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CImageView)

	BOOL AoiExtract(CString SrcName, CString ImgName);
	BOOL MyExportAsBmpfile(CString tmpimg);
	virtual CFORemoveCompsAction*	DoDeleteAction(CFODrawShapeList* pShapeList);

// Attributes
public:
	HDIB m_hDIB;
	void RefreshDIB(CString, bool);
	CImageDockWnd *m_pParent;

// Operations
public:
	virtual void Draw(CDC* pDC);
	void Perform_UI_tools(int,CPoint);
    
	CPoint m_hot_point;
	int m_Target_Threshold;

	void PerformOnUpdateFoDrawSelect(CCmdUI*);
	void PerformOnUpdateFoCmdAutoPan(CCmdUI*);
	void PerformOnUpdateEditUndo(CCmdUI*);
	void PerformOnUpdateEditRedo(CCmdUI*);
	void PerformOnUpdateFoEditSpot(CCmdUI*);
	void PerformOnUpdateFoDrawLine(CCmdUI*);
	void PerformOnUpdateFoDrawArrowLine(CCmdUI*);
	void PerformOnUpdateFoDrawCrossLine(CCmdUI*);
	void PerformOnUpdateFoDrawPolyLine(CCmdUI*);
	void PerformOnUpdateFoDrawFreeLine(CCmdUI*);
	void PerformOnUpdateFoDrawPolygon(CCmdUI*);
	void PerformOnUpdateFoDrawEllipse(CCmdUI*);
	void PerformOnUpdateFoDrawUprightCircle(CCmdUI*);
	void PerformOnUpdateFoDrawRectangle(CCmdUI*);
	void PerformOnUpdateFoUprightRect(CCmdUI*);
	void PerformOnUpdateFoDrawFreeCloseLine(CCmdUI*);
	void PerformOnUpdateFoDrawStatic(CCmdUI*);
	void PerformOnUpdateFoDoDrawImage(CCmdUI*);
	void PerformOnUpdateFoViewScaleadd25(CCmdUI*);
	void PerformOnUpdateFoViewScalesub25(CCmdUI*);
	void PerformOnUpdateFoViewZoom100(CCmdUI*);
	void PerformOnUpdateFoZoomWithinRect(CCmdUI*);
	void PerformOnUpdateFoZoomfitpage(CCmdUI*);
	void PerformOnUpdateFoZoomfitheight(CCmdUI*);
	void PerformOnUpdateFoZoomfitwidth(CCmdUI*);
	void PerformOnUpdateFoZoomFitselect(CCmdUI*);
	void PerformOnUpdateImagepsColorrectify(CCmdUI*);
	void PerformOnUpdateEditMagic(CCmdUI*);
	void PerformOnUpdateAoiCut(CCmdUI*);
	void PerformOnUpdateImageMark(CCmdUI*);
	void PerformOnUpdateImageCut(CCmdUI*);
	void PerformOnUpdateImageCopy(CCmdUI*);
	void PerformOnUpdateImagePaste(CCmdUI*);
	void PerformOnUpdateImageSave(CCmdUI*);
	void PerformOnUpdateImageRectify(CCmdUI*);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

    //Create popup menu
	virtual CMenu* CreateContextMenu();

// Implementation
protected:
	virtual ~CImageView();
	HCURSOR hCursor; // recycled for our cursor

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CImageView)
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnImagepsColorrectify();
	afx_msg void OnUpdateImagepsColorrectify(CCmdUI* pCmdUI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEditMagic();
	afx_msg void OnUpdateEditMagic(CCmdUI* pCmdUI);
	afx_msg void OnValueAdd();
	afx_msg void OnValueDelete();
	afx_msg void OnAoiRefer();
	afx_msg void OnAoiCut();
	afx_msg void OnUpdateAoiCut(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnImageMark();
	afx_msg void OnUpdateImageMark(CCmdUI* pCmdUI);
	afx_msg void OnImageCut();
	afx_msg void OnUpdateImageCut(CCmdUI* pCmdUI);
	afx_msg void OnImageCopy();
	afx_msg void OnUpdateImageCopy(CCmdUI* pCmdUI);
	afx_msg void OnImagePaste();
	afx_msg void OnUpdateImagePaste(CCmdUI* pCmdUI);
	afx_msg void OnImageSave();
	afx_msg void OnUpdateImageSave(CCmdUI* pCmdUI);
	afx_msg void OnImageRectify();
	afx_msg void OnUpdateImageRectify(CCmdUI* pCmdUI);
	afx_msg void OnLinetypeSelect();
	afx_msg void OnTexttypeSelect();
	afx_msg void OnEditRedo();
	afx_msg void OnEditUndo();
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	//}}AFX_MSG
	LRESULT OnContentRefresh(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEVIEW_H__C6138052_1803_4698_B55D_966D4BCE407F__INCLUDED_)
