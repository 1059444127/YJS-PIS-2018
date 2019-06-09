// CImageDockWnd : header file
/////////////////////////////////////////////////////////////////////////////

#ifndef __IMAGEDOCKWND_H
#define __IMAGEDOCKWND_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CImageDockWnd class
#include "ExtDataModel.h"
#include "amsEdit.h"

class CImageDockWnd : public CXTDockWindow
{
	DECLARE_DYNCREATE(CImageDockWnd)

// Construction / destruction
public:
	CExtDataModel *m_pDataModel;

	// Constructs a CImageDockWnd object.
	CImageDockWnd();
	virtual void GetInsideRect(CRect &rect);

	// Destroys a CImageDockWnd object, handles cleanup and de-allocation.
	virtual ~CImageDockWnd();
	CAMSNumericEdit  m_wndEditGrow;

// Member variables
protected:
	CXTToolBar       m_wndToolBar;
	CWnd *GetSubView();

// Member functions
public:

	//{{AFX_VIRTUAL(CImageDockWnd)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CImageDockWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnFoDrawSelect();
	afx_msg void OnFoCmdAutopan();
	afx_msg void OnUpdateFoDrawSelect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFoCmdAutopan(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnFoEditSpots();
	afx_msg void OnUpdateFoEditSpots(CCmdUI* pCmdUI);
	afx_msg void OnFoDrawLine();
	afx_msg void OnUpdateFoDrawLine(CCmdUI* pCmdUI);
	afx_msg void OnFoDrawArrowline();
	afx_msg void OnUpdateFoDrawArrowline(CCmdUI* pCmdUI);
	afx_msg void OnFoDrawCrossline();
	afx_msg void OnUpdateFoDrawCrossline(CCmdUI* pCmdUI);
	afx_msg void OnFoDrawPolyline();
	afx_msg void OnUpdateFoDrawPolyline(CCmdUI* pCmdUI);
	afx_msg void OnFoDrawFreeline();
	afx_msg void OnUpdateFoDrawFreeline(CCmdUI* pCmdUI);
	afx_msg void OnFoDrawPolygon();
	afx_msg void OnUpdateFoDrawPolygon(CCmdUI* pCmdUI);
	afx_msg void OnFoDrawEllipse();
	afx_msg void OnUpdateFoDrawEllipse(CCmdUI* pCmdUI);
	afx_msg void OnFoDrawUprightCircle();
	afx_msg void OnUpdateFoDrawUprightCircle(CCmdUI* pCmdUI);
	afx_msg void OnFoDrawRectangle();
	afx_msg void OnUpdateFoDrawRectangle(CCmdUI* pCmdUI);
	afx_msg void OnFoUprightRect();
	afx_msg void OnUpdateFoUprightRect(CCmdUI* pCmdUI);
	afx_msg void OnFoDrawFreecloseline();
	afx_msg void OnUpdateFoDrawFreecloseline(CCmdUI* pCmdUI);
	afx_msg void OnFoDrawStatic();
	afx_msg void OnUpdateFoDrawStatic(CCmdUI* pCmdUI);
	afx_msg void OnFoDrawImage();
	afx_msg void OnUpdateFoDrawImage(CCmdUI* pCmdUI);
	afx_msg void OnFoViewScaleadd25();
	afx_msg void OnUpdateFoViewScaleadd25(CCmdUI* pCmdUI);
	afx_msg void OnFoViewScalesub25();
	afx_msg void OnUpdateFoViewScalesub25(CCmdUI* pCmdUI);
	afx_msg void OnFoViewZoom100();
	afx_msg void OnUpdateFoViewZoom100(CCmdUI* pCmdUI);
	afx_msg void OnFoZoomwithinrect();
	afx_msg void OnUpdateFoZoomwithinrect(CCmdUI* pCmdUI);
	afx_msg void OnFoZoomfitpage();
	afx_msg void OnUpdateFoZoomfitpage(CCmdUI* pCmdUI);
	afx_msg void OnFoZoomfitheight();
	afx_msg void OnUpdateFoZoomfitheight(CCmdUI* pCmdUI);
	afx_msg void OnFoZoomfitwidth();
	afx_msg void OnUpdateFoZoomfitwidth(CCmdUI* pCmdUI);
	afx_msg void OnFoZoomFitselect();
	afx_msg void OnUpdateFoZoomFitselect(CCmdUI* pCmdUI);
	afx_msg void OnImagepsColorrectify();
	afx_msg void OnUpdateImagepsColorrectify(CCmdUI* pCmdUI);
	afx_msg void OnEditMagic();
	afx_msg void OnUpdateEditMagic(CCmdUI* pCmdUI);
	afx_msg void OnValueAdd();
	afx_msg void OnValueDelete();
	afx_msg void OnAoiRefer();
	afx_msg void OnAoiCut();
	afx_msg void OnUpdateAoiCut(CCmdUI* pCmdUI);
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __IMAGEDOCKWND_H

