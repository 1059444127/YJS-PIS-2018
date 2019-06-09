// CImageDockWnd : implementation file
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "ImageDockWnd.h"
#include "ImageView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageDockWnd

CImageDockWnd::CImageDockWnd()
{
	// TODO: add construction code here.
	m_pDataModel = new CExtDataModel;

	m_pDataModel->SetPagePosition(CRect(0,0,10,10));
	m_pDataModel->SetFormSize(CSize(10,10));
	m_pDataModel->SetFormMode(FALSE);
}

CImageDockWnd::~CImageDockWnd()
{
	// TODO: add destruction code here.
	if(m_pDataModel != NULL)
	{
		delete m_pDataModel;
		m_pDataModel = NULL;
	}
}

IMPLEMENT_DYNCREATE(CImageDockWnd, CXTDockWindow)

BEGIN_MESSAGE_MAP(CImageDockWnd, CXTDockWindow)
	//{{AFX_MSG_MAP(CImageDockWnd)
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	ON_COMMAND(ID_FO_DRAW_SELECT, OnFoDrawSelect)
	ON_COMMAND(ID_FO_CMD_AUTOPAN, OnFoCmdAutopan)
	ON_UPDATE_COMMAND_UI(ID_FO_DRAW_SELECT, OnUpdateFoDrawSelect)
	ON_UPDATE_COMMAND_UI(ID_FO_CMD_AUTOPAN, OnUpdateFoCmdAutopan)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_FO_EDIT_SPOTS, OnFoEditSpots)
	ON_UPDATE_COMMAND_UI(ID_FO_EDIT_SPOTS, OnUpdateFoEditSpots)
	ON_COMMAND(ID_FO_DRAW_LINE, OnFoDrawLine)
	ON_UPDATE_COMMAND_UI(ID_FO_DRAW_LINE, OnUpdateFoDrawLine)
	ON_COMMAND(ID_FO_DRAW_ARROWLINE, OnFoDrawArrowline)
	ON_UPDATE_COMMAND_UI(ID_FO_DRAW_ARROWLINE, OnUpdateFoDrawArrowline)
	ON_COMMAND(ID_FO_DRAW_CROSSLINE, OnFoDrawCrossline)
	ON_UPDATE_COMMAND_UI(ID_FO_DRAW_CROSSLINE, OnUpdateFoDrawCrossline)
	ON_COMMAND(ID_FO_DRAW_POLYLINE, OnFoDrawPolyline)
	ON_UPDATE_COMMAND_UI(ID_FO_DRAW_POLYLINE, OnUpdateFoDrawPolyline)
	ON_COMMAND(ID_FO_DRAW_FREELINE, OnFoDrawFreeline)
	ON_UPDATE_COMMAND_UI(ID_FO_DRAW_FREELINE, OnUpdateFoDrawFreeline)
	ON_COMMAND(ID_FO_DRAW_POLYGON, OnFoDrawPolygon)
	ON_UPDATE_COMMAND_UI(ID_FO_DRAW_POLYGON, OnUpdateFoDrawPolygon)
	ON_COMMAND(ID_FO_DRAW_ELLIPSE, OnFoDrawEllipse)
	ON_UPDATE_COMMAND_UI(ID_FO_DRAW_ELLIPSE, OnUpdateFoDrawEllipse)
	ON_COMMAND(ID_FO_DRAW_UPRIGHT_CIRCLE, OnFoDrawUprightCircle)
	ON_UPDATE_COMMAND_UI(ID_FO_DRAW_UPRIGHT_CIRCLE, OnUpdateFoDrawUprightCircle)
	ON_COMMAND(ID_FO_DRAW_RECTANGLE, OnFoDrawRectangle)
	ON_UPDATE_COMMAND_UI(ID_FO_DRAW_RECTANGLE, OnUpdateFoDrawRectangle)
	ON_COMMAND(ID_FO_UPRIGHT_RECT, OnFoUprightRect)
	ON_UPDATE_COMMAND_UI(ID_FO_UPRIGHT_RECT, OnUpdateFoUprightRect)
	ON_COMMAND(ID_FO_DRAW_FREECLOSELINE, OnFoDrawFreecloseline)
	ON_UPDATE_COMMAND_UI(ID_FO_DRAW_FREECLOSELINE, OnUpdateFoDrawFreecloseline)
	ON_COMMAND(ID_FO_DRAW_STATIC, OnFoDrawStatic)
	ON_UPDATE_COMMAND_UI(ID_FO_DRAW_STATIC, OnUpdateFoDrawStatic)
	ON_COMMAND(ID_FO_DRAW_IMAGE, OnFoDrawImage)
	ON_UPDATE_COMMAND_UI(ID_FO_DRAW_IMAGE, OnUpdateFoDrawImage)
	ON_COMMAND(ID_FO_VIEW_SCALEADD25, OnFoViewScaleadd25)
	ON_UPDATE_COMMAND_UI(ID_FO_VIEW_SCALEADD25, OnUpdateFoViewScaleadd25)
	ON_COMMAND(ID_FO_VIEW_SCALESUB25, OnFoViewScalesub25)
	ON_UPDATE_COMMAND_UI(ID_FO_VIEW_SCALESUB25, OnUpdateFoViewScalesub25)
	ON_COMMAND(ID_FO_VIEW_ZOOM100, OnFoViewZoom100)
	ON_UPDATE_COMMAND_UI(ID_FO_VIEW_ZOOM100, OnUpdateFoViewZoom100)
	ON_COMMAND(ID_FO_ZOOMWITHINRECT, OnFoZoomwithinrect)
	ON_UPDATE_COMMAND_UI(ID_FO_ZOOMWITHINRECT, OnUpdateFoZoomwithinrect)
	ON_COMMAND(ID_FO_ZOOMFITPAGE, OnFoZoomfitpage)
	ON_UPDATE_COMMAND_UI(ID_FO_ZOOMFITPAGE, OnUpdateFoZoomfitpage)
	ON_COMMAND(ID_FO_ZOOMFITHEIGHT, OnFoZoomfitheight)
	ON_UPDATE_COMMAND_UI(ID_FO_ZOOMFITHEIGHT, OnUpdateFoZoomfitheight)
	ON_COMMAND(ID_FO_ZOOMFITWIDTH, OnFoZoomfitwidth)
	ON_UPDATE_COMMAND_UI(ID_FO_ZOOMFITWIDTH, OnUpdateFoZoomfitwidth)
	ON_COMMAND(ID_FO_ZOOM_FITSELECT, OnFoZoomFitselect)
	ON_UPDATE_COMMAND_UI(ID_FO_ZOOM_FITSELECT, OnUpdateFoZoomFitselect)
	ON_COMMAND(ID_IMAGEPS_COLORRECTIFY, OnImagepsColorrectify)
	ON_UPDATE_COMMAND_UI(ID_IMAGEPS_COLORRECTIFY, OnUpdateImagepsColorrectify)
	ON_COMMAND(ID_EDIT_MAGIC, OnEditMagic)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MAGIC, OnUpdateEditMagic)
	ON_COMMAND(ID_VALUE_ADD, OnValueAdd)
	ON_COMMAND(ID_VALUE_DELETE, OnValueDelete)
	ON_COMMAND(ID_AOI_REFER, OnAoiRefer)
	ON_COMMAND(ID_AOI_CUT, OnAoiCut)
	ON_UPDATE_COMMAND_UI(ID_AOI_CUT, OnUpdateAoiCut)
	ON_COMMAND(ID_IMAGE_MARK, OnImageMark)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_MARK, OnUpdateImageMark)
	ON_COMMAND(ID_IMAGE_CUT, OnImageCut)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_CUT, OnUpdateImageCut)
	ON_COMMAND(ID_IMAGE_COPY, OnImageCopy)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_COPY, OnUpdateImageCopy)
	ON_COMMAND(ID_IMAGE_PASTE, OnImagePaste)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_PASTE, OnUpdateImagePaste)
	ON_COMMAND(ID_IMAGE_SAVE, OnImageSave)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_SAVE, OnUpdateImageSave)
	ON_COMMAND(ID_IMAGE_RECTIFY, OnImageRectify)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_RECTIFY, OnUpdateImageRectify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageDockWnd message handlers

int CImageDockWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTDockWindow::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
    // Add the tool bar to the docking window
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE |
		CBRS_ALIGN_TOP | CBRS_TOOLTIPS | CBRS_FLYBY, CRect(0,0,0,0) ) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_DRAWBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndEditGrow.Create( WS_CHILD|WS_VISIBLE|ES_LEFT|WS_CLIPCHILDREN | WS_BORDER,
		CRect(0,0,30,19), &m_wndToolBar, ID_AREAGROW ))
	{
		TRACE0("Failed to create combo box.\n");
		return -1;      // fail to create
	}
	m_wndEditGrow.SetWindowText("20");
	m_wndToolBar.InsertControl(&m_wndEditGrow);
	m_wndToolBar.AutoSizeToolbar();
    
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
    
	SetChild(CreateView(RUNTIME_CLASS(CImageView), NULL, NULL));
	return 0;
}

const int CY_TOOLBAR = 22;

void CImageDockWnd::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CXTDockWindow::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	// Reposition the toolbar.
	if (::IsWindow(m_wndToolBar.m_hWnd))
	{
		if (IsFloating()) {
			m_wndToolBar.MoveWindow(3, 3, lpwndpos->cx, CY_TOOLBAR);
		}
		else
		{
			if (IsVertDocked()) {
				m_wndToolBar.MoveWindow(0, GetMinExt(), lpwndpos->cx, CY_TOOLBAR);
			}
			else {
				m_wndToolBar.MoveWindow(GetMinExt(), 0, lpwndpos->cx, CY_TOOLBAR);
			}
		}
	}
}

void CImageDockWnd::GetInsideRect(CRect& rect)
{
	CXTDockWindow::GetInsideRect(rect);
    rect.top += (CY_TOOLBAR + 2);
}

CWnd * CImageDockWnd::GetSubView()
{
	CImageView *pImageView = NULL;
	CWnd *pWnd = GetWindow(GW_CHILD);
	while( pWnd != NULL )
	{
		if( pWnd->IsKindOf(RUNTIME_CLASS(CImageView)) )
		{
			pImageView = (CImageView *)pWnd;
			break;
		}
		pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
	}
	return pImageView;
}

void CImageDockWnd::OnFoDrawSelect() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_DRAW_SELECT);
	}
}

void CImageDockWnd::OnUpdateFoDrawSelect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoDrawSelect(pCmdUI);
	}
}

void CImageDockWnd::OnFoCmdAutopan() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_CMD_AUTOPAN);
	}
}

void CImageDockWnd::OnUpdateFoCmdAutopan(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoCmdAutoPan(pCmdUI);
	}
}

void CImageDockWnd::OnEditUndo() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_EDIT_UNDO);
	}
}

void CImageDockWnd::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateEditUndo(pCmdUI);
	}
}

void CImageDockWnd::OnEditRedo() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_EDIT_REDO);
	}
}

void CImageDockWnd::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateEditRedo(pCmdUI);
	}
}

void CImageDockWnd::OnFoEditSpots() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_EDIT_SPOTS);
	}
}

void CImageDockWnd::OnUpdateFoEditSpots(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoEditSpot(pCmdUI);
	}
}


void CImageDockWnd::OnFoDrawLine() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_DRAW_LINE);
	}
}

void CImageDockWnd::OnUpdateFoDrawLine(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoDrawLine(pCmdUI);
	}
}

void CImageDockWnd::OnFoDrawArrowline() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_DRAW_ARROWLINE);
	}
}

void CImageDockWnd::OnUpdateFoDrawArrowline(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoDrawArrowLine(pCmdUI);
	}
}

void CImageDockWnd::OnFoDrawCrossline() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_DRAW_CROSSLINE);
	}
}

void CImageDockWnd::OnUpdateFoDrawCrossline(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoDrawCrossLine(pCmdUI);
	}
}

void CImageDockWnd::OnFoDrawPolyline() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_DRAW_POLYLINE);
	}
}

void CImageDockWnd::OnUpdateFoDrawPolyline(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoDrawPolyLine(pCmdUI);
	}
}

void CImageDockWnd::OnFoDrawFreeline() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_DRAW_FREELINE);
	}
}

void CImageDockWnd::OnUpdateFoDrawFreeline(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoDrawFreeLine(pCmdUI);
	}
}

void CImageDockWnd::OnFoDrawPolygon() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_DRAW_POLYGON);
	}
}

void CImageDockWnd::OnUpdateFoDrawPolygon(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoDrawPolygon(pCmdUI);
	}
}

void CImageDockWnd::OnFoDrawEllipse() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_DRAW_ELLIPSE);
	}
}

void CImageDockWnd::OnUpdateFoDrawEllipse(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoDrawEllipse(pCmdUI);
	}
}

void CImageDockWnd::OnFoDrawUprightCircle() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_DRAW_UPRIGHT_CIRCLE);
	}
}

void CImageDockWnd::OnUpdateFoDrawUprightCircle(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoDrawUprightCircle(pCmdUI);
	}
}

void CImageDockWnd::OnFoDrawRectangle() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_DRAW_RECTANGLE);
	}
}

void CImageDockWnd::OnUpdateFoDrawRectangle(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoDrawRectangle(pCmdUI);
	}
}

void CImageDockWnd::OnFoUprightRect() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_UPRIGHT_RECT);
	}
}

void CImageDockWnd::OnUpdateFoUprightRect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoUprightRect(pCmdUI);
	}
}

void CImageDockWnd::OnFoDrawFreecloseline() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_DRAW_FREECLOSELINE);
	}
}

void CImageDockWnd::OnUpdateFoDrawFreecloseline(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoDrawFreeCloseLine(pCmdUI);
	}
}

void CImageDockWnd::OnFoDrawStatic() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_DRAW_STATIC);
	}
}

void CImageDockWnd::OnUpdateFoDrawStatic(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoDrawStatic(pCmdUI);
	}
}

void CImageDockWnd::OnFoDrawImage() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_DRAW_IMAGE);
	}
}

void CImageDockWnd::OnUpdateFoDrawImage(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoDoDrawImage(pCmdUI);
	}
}

void CImageDockWnd::OnFoViewScaleadd25() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_VIEW_SCALEADD25);
	}
}

void CImageDockWnd::OnUpdateFoViewScaleadd25(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoViewScaleadd25(pCmdUI);
	}
}

void CImageDockWnd::OnFoViewScalesub25() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_VIEW_SCALESUB25);
	}
}

void CImageDockWnd::OnUpdateFoViewScalesub25(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoViewScalesub25(pCmdUI);
	}
}

void CImageDockWnd::OnFoViewZoom100() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_VIEW_ZOOM100);
	}
}

void CImageDockWnd::OnUpdateFoViewZoom100(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoViewZoom100(pCmdUI);
	}
}

void CImageDockWnd::OnFoZoomwithinrect() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_ZOOMWITHINRECT);
	}
}

void CImageDockWnd::OnUpdateFoZoomwithinrect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoZoomWithinRect(pCmdUI);
	}
}

void CImageDockWnd::OnFoZoomfitpage() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_ZOOMFITPAGE);
	}
}

void CImageDockWnd::OnUpdateFoZoomfitpage(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoZoomfitpage(pCmdUI);
	}
}

void CImageDockWnd::OnFoZoomfitheight() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_ZOOMFITHEIGHT);
	}
}

void CImageDockWnd::OnUpdateFoZoomfitheight(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoZoomfitheight(pCmdUI);
	}
}

void CImageDockWnd::OnFoZoomfitwidth() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_ZOOMFITWIDTH);
	}
}

void CImageDockWnd::OnUpdateFoZoomfitwidth(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoZoomfitwidth(pCmdUI);
	}
}

void CImageDockWnd::OnFoZoomFitselect() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_FO_ZOOM_FITSELECT);
	}
}

void CImageDockWnd::OnUpdateFoZoomFitselect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateFoZoomFitselect(pCmdUI);
	}
}

void CImageDockWnd::OnImagepsColorrectify() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_IMAGEPS_COLORRECTIFY);
	}
}

void CImageDockWnd::OnUpdateImagepsColorrectify(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateImagepsColorrectify(pCmdUI);
	}
}

void CImageDockWnd::OnEditMagic() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_EDIT_MAGIC);
	}
}

void CImageDockWnd::OnUpdateEditMagic(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateEditMagic(pCmdUI);
	}
}

void CImageDockWnd::OnValueAdd() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_VALUE_ADD);
	}
}

void CImageDockWnd::OnValueDelete() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_VALUE_DELETE);
	}
}

void CImageDockWnd::OnAoiRefer() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_AOI_REFER);
	}
}

void CImageDockWnd::OnAoiCut() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_AOI_CUT);
	}
}

void CImageDockWnd::OnUpdateAoiCut(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateAoiCut(pCmdUI);
	}
}

void CImageDockWnd::OnImageMark() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_IMAGE_MARK);
	}
}

void CImageDockWnd::OnUpdateImageMark(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateImageMark(pCmdUI);
	}
}

void CImageDockWnd::OnImageCut() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_IMAGE_CUT);
	}
}

void CImageDockWnd::OnUpdateImageCut(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateImageCut(pCmdUI);
	}
}

void CImageDockWnd::OnImageCopy() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_IMAGE_COPY);
	}
}

void CImageDockWnd::OnUpdateImageCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateImageCopy(pCmdUI);
	}
}

void CImageDockWnd::OnImagePaste() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_IMAGE_PASTE);
	}
}

void CImageDockWnd::OnUpdateImagePaste(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateImagePaste(pCmdUI);
	}
}

void CImageDockWnd::OnImageSave() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_IMAGE_SAVE);
	}
}

void CImageDockWnd::OnUpdateImageSave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateImageSave(pCmdUI);
	}
}

void CImageDockWnd::OnImageRectify() 
{
	// TODO: Add your command handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PostMessage(WM_COMMAND,ID_IMAGE_RECTIFY);
	}
}

void CImageDockWnd::OnUpdateImageRectify(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CImageView *pImageView = (CImageView *)GetSubView();
	if( pImageView )
	{
		pImageView->PerformOnUpdateImageRectify(pCmdUI);
	}
}
