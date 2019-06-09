// ImageView.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "mainfrm.h"
#include "Pathologyview.h"
#include "Pathologydoc.h"
#include "ImageView.h"
#include "ImageDockWnd.h"
#include "MedImage.h"
#include "Equal.h"
#include "dlgpropertyline.h"
#include "dlgpropertytext.h"
#include "INI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageView
extern CPathologyApp theApp;

IMPLEMENT_DYNCREATE(CImageView, CFODrawView)

CImageView::CImageView()
{
	m_hDIB = NULL;
	m_Target_Threshold = 255;
}

CImageView::~CImageView()
{
}


BEGIN_MESSAGE_MAP(CImageView, CFODrawView)
	//{{AFX_MSG_MAP(CImageView)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_IMAGEPS_COLORRECTIFY, OnImagepsColorrectify)
	ON_UPDATE_COMMAND_UI(ID_IMAGEPS_COLORRECTIFY, OnUpdateImagepsColorrectify)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_EDIT_MAGIC, OnEditMagic)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MAGIC, OnUpdateEditMagic)
	ON_COMMAND(ID_VALUE_ADD, OnValueAdd)
	ON_COMMAND(ID_VALUE_DELETE, OnValueDelete)
	ON_COMMAND(ID_AOI_REFER, OnAoiRefer)
	ON_COMMAND(ID_AOI_CUT, OnAoiCut)
	ON_UPDATE_COMMAND_UI(ID_AOI_CUT, OnUpdateAoiCut)
	ON_WM_LBUTTONDBLCLK()
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
	ON_COMMAND(ID_LINETYPE_SELECT, OnLinetypeSelect)
	ON_COMMAND(ID_TEXTTYPE_SELECT, OnTexttypeSelect)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CONTENTREFRESH, OnContentRefresh)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageView drawing

void CImageView::OnDraw(CDC* pDC)
{
	static double m_PreZoomScale = -1.0;
	// TODO: add draw code for native data here
	if( m_hDIB != NULL )
	{
		LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
		int cxDIB = ::DIBWidth(lpDIBHdr);
		int cyDIB = ::DIBHeight(lpDIBHdr);
		::GlobalUnlock((HGLOBAL) m_hDIB);

		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right  = cxDIB;
		rcDIB.bottom = cyDIB;
		CRect rcDest;
		rcDest = rcDIB;

		rcDest.right  = rcDIB.Width()  * GetZoomScale();
		rcDest.bottom = rcDIB.Height() * GetZoomScale();
		
   	    int oldMapMode = pDC->SetMapMode(MM_TEXT);
		::PaintDIB(pDC->m_hDC, &rcDest, m_hDIB, &rcDIB, NULL);
		pDC->SetMapMode(oldMapMode);
		OnPrepareDC(pDC);

		if( m_PreZoomScale != GetZoomScale() )
		{
			m_PreZoomScale = GetZoomScale();
			Invalidate();
		}
	}
	Draw(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CImageView diagnostics

#ifdef _DEBUG
void CImageView::AssertValid() const
{
	CFODrawView::AssertValid();
}

void CImageView::Dump(CDumpContext& dc) const
{
	CFODrawView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageView message handlers

void CImageView::OnDestroy() 
{
	CFODrawView::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_hDIB != NULL)
	{
		::GlobalFree((HGLOBAL) m_hDIB);
		m_hDIB = NULL;
	}
}

void CImageView::OnInitialUpdate() 
{
	SetCurrentModel(m_pParent->m_pDataModel);
	CFODrawView::OnInitialUpdate();

	CIniReader m_IniReader;
	m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");

	CString LineColor = m_IniReader.getKeyValue("LineColor","线型设置");
	if(!LineColor.IsEmpty())
	{
		int r = atoi(LineColor.Left(LineColor.Find(',')));
		LineColor = LineColor.Right(LineColor.GetLength() - LineColor.Find(',') - 1);
		int g = atoi(LineColor.Left(LineColor.Find(',')));
		LineColor = LineColor.Right(LineColor.GetLength() - LineColor.Find(',') - 1);
		int b = atoi(LineColor);

		((CExtDataModel *)GetCurrentModel())->qLineColor = RGB(r,g,b);
	}

	CString LineWidth = m_IniReader.getKeyValue("LineWidth","线型设置");
	if(!LineWidth.IsEmpty())
	{
		((CExtDataModel *)GetCurrentModel())->qLineWidth = atoi(LineWidth);
	}

	CString LineType = m_IniReader.getKeyValue("LineType","线型设置");
	if(!LineType.IsEmpty())
	{
		((CExtDataModel *)GetCurrentModel())->qLineType = atoi(LineType);
	}

	CString FontType = m_IniReader.getKeyValue("FontType","文字标注格式设置");
	if(!FontType.IsEmpty())
	{
		FO_FONT Font;

		Font.m_strFaceName = FontType.Left(FontType.Find(','));
		FontType = FontType.Right(FontType.GetLength() - FontType.Find(',') - 1);

		int r = atoi(FontType.Left(FontType.Find(',')));
		FontType = FontType.Right(FontType.GetLength() - FontType.Find(',') - 1);
		int g = atoi(FontType.Left(FontType.Find(',')));
		FontType = FontType.Right(FontType.GetLength() - FontType.Find(',') - 1);
		int b = atoi(FontType.Left(FontType.Find(',')));
		FontType = FontType.Right(FontType.GetLength() - FontType.Find(',') - 1);
		Font.m_crColor = RGB(r,g,b);

		Font.m_nPointSize = atoi(FontType.Left(FontType.Find(',')));
		FontType = FontType.Right(FontType.GetLength() - FontType.Find(',') - 1);

		Font.m_nHeight = atoi(FontType.Left(FontType.Find(',')));
		FontType = FontType.Right(FontType.GetLength() - FontType.Find(',') - 1);

		Font.m_bItalic = atoi(FontType.Left(FontType.Find(',')));
		FontType = FontType.Right(FontType.GetLength() - FontType.Find(',') - 1);

		Font.m_bUnderline = atoi(FontType.Left(FontType.Find(',')));
		FontType = FontType.Right(FontType.GetLength() - FontType.Find(',') - 1);

		Font.m_nWeight = atoi(FontType.Left(FontType.Find(',')));
		FontType = FontType.Right(FontType.GetLength() - FontType.Find(',') - 1);

		((CExtDataModel *)GetCurrentModel())->StaticFont = Font;

		((CExtDataModel *)GetCurrentModel())->m_Static_bMultiLine = atoi(FontType.Left(FontType.Find(',')));
		FontType = FontType.Right(FontType.GetLength() - FontType.Find(',') - 1);
		((CExtDataModel *)GetCurrentModel())->m_Static_bHorzAlign = atoi(FontType.Left(FontType.Find(',')));
		FontType = FontType.Right(FontType.GetLength() - FontType.Find(',') - 1);
		((CExtDataModel *)GetCurrentModel())->m_Static_bVertAlign = atoi(FontType);
	}
	
	// TODO: Add your specialized code here and/or call the base class
	SendMessage(WM_COMMAND, ID_EDIT_CLEAR_ALL);

	//=========================================================================
	CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	if( pAppFrame->m_wndOutputBar.IsWindowVisible() )
	{
		int n_count = GetCurrentModel()->GetFormObjects()->GetCount();
		pAppFrame->m_wndOutputBar.m_MeasureDialog.m_MeasureCtrl.SetNumberRows(n_count);
		pAppFrame->m_wndOutputBar.m_MeasureDialog.m_MeasureCtrl.RedrawAll();
	}
	//=========================================================================
	
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 10;

	if( m_hDIB != NULL )
	{
		LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
		sizeTotal.cx = ::DIBWidth(lpDIBHdr);
		sizeTotal.cy = ::DIBHeight(lpDIBHdr);
		::GlobalUnlock((HGLOBAL) m_hDIB);
	}

	SetScrollSizes(MM_TEXT, sizeTotal);

	CRect rcPos;
	rcPos = CRect(0,0,sizeTotal.cx, sizeTotal.cy);
	GetCurrentModel()->SetPagePosition(rcPos);

	SetZoomScale( 100 );

	if( m_hDIB != NULL )
	{
		CRect rt;
		GetClientRect(&rt);
		if( sizeTotal.cx > rt.Width() || sizeTotal.cy > rt.Height() )
		{
			ZoomToFitPage();
			return;
		}
	}

	RedrawWindow();
}

void CImageView::Draw(CDC* pDC)
{
	if(m_pCurDataModel == NULL)
	{
		TRACE(_T("You must call SetCurrentModel(CFODataModel *pModel) at OnInitialUpdate first."));
		return;
	}
	
	// m_pDataModel == GetCurrentModel()
	// Draw shapes.
	BOOL bDesign = m_pParent->m_pDataModel->IsDesignMode();
	m_pParent->m_pDataModel->OnDraw(pDC);
	
	// Draw tab order.
	if(bTabOrder)
	{
		if(bDesign)
		{
			m_pParent->m_pDataModel->OnDrawTabOrder(pDC);
		}
	}
	
	if(!pDC->IsPrinting())
	{
		if(bDesign && m_bShowSelectHandle)
		{
			DoDrawSelectShapes(pDC);
		}
	}
}


BOOL CImageView::OnEraseBkgnd(CDC* pDC) 
{
	CRect rt;
	GetClientRect(&rt);
	rt.InflateRect(1,1);

	CPen   * n_old_pen   = (CPen   *)pDC->SelectStockObject(NULL_PEN);
	CBrush * n_old_brush = (CBrush *)pDC->SelectStockObject(GRAY_BRUSH);

	if( m_hDIB != NULL )
	{
		LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
		int cxDIB = ::DIBWidth(lpDIBHdr);
		int cyDIB = ::DIBHeight(lpDIBHdr);
		::GlobalUnlock((HGLOBAL) m_hDIB);

		CRect MemWnd;
		MemWnd.top = MemWnd.left = 0;
		MemWnd.right  = cxDIB * GetZoomScale();
		MemWnd.bottom = cyDIB * GetZoomScale();

		pDC->Rectangle(CRect(0,0,rt.right,(rt.Height() - MemWnd.Height())/2));
		pDC->Rectangle(CRect(0,rt.Height() - (rt.Height() - MemWnd.Height())/2 - 1 ,rt.right, rt.bottom));

		pDC->Rectangle(CRect(0,0, (rt.Width() - MemWnd.Width())/2,rt.bottom));
		pDC->Rectangle(CRect(rt.right - (rt.Width() - MemWnd.Width())/2 - 1,0, rt.right,rt.bottom));

	}
	else
	{
		pDC->Rectangle(&rt);
	}

	pDC->SelectObject(n_old_pen);
	pDC->SelectObject(n_old_brush);
	return true;
}

int CImageView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFODrawView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_pParent = (CImageDockWnd *)GetParent();
	
	return 0;
}

void CImageView::RefreshDIB(CString fname, bool IstheSame)
{
	GDALDataset * m_pGDALDataset = (GDALDataset *) GDALOpen ( fname, GA_ReadOnly );
	if( m_pGDALDataset == NULL)
	{
		return;
	}

	int m_raster_xsize = m_pGDALDataset->GetRasterXSize();
	int m_raster_ysize = m_pGDALDataset->GetRasterYSize();
	int m_raster_count = m_pGDALDataset->GetRasterCount();

	if (m_hDIB != NULL)
	{
		::GlobalFree((HGLOBAL) m_hDIB);
		m_hDIB = NULL;
	}
	m_hDIB = ::CreateDIBFile( m_raster_xsize, m_raster_ysize , 24 );
	LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	BYTE *   lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
	int LineByteWidth = WIDTHBYTES( m_raster_xsize * 24 );
	memset( lpDIBBits, 255, LineByteWidth * m_raster_ysize );
	
	BYTE *pRBits = lpDIBBits + 2;
	BYTE *pGBits = lpDIBBits + 1;
	BYTE *pBBits = lpDIBBits + 0;
	
	int BandNo[3];
	if( m_raster_count == 1)
	{
		BandNo[0] = 1;	BandNo[1] = 1;	BandNo[2] = 1;
	}
	else
	{
		BandNo[0] = 1;	BandNo[1] = 2;	BandNo[2] = 3;
	}

	m_pGDALDataset->GetRasterBand(BandNo[0])->RasterIO( GF_Read, 0, 0, 
		                                                m_raster_xsize, m_raster_ysize, 
		                                                (void *)(pRBits), 
		                                                m_raster_xsize, m_raster_ysize, 
		                                                GDT_Byte, 
		                                                3 , LineByteWidth );
	m_pGDALDataset->GetRasterBand(BandNo[1])->RasterIO( GF_Read, 0, 0, 
		                                                m_raster_xsize, m_raster_ysize, 
		                                                (void *)(pGBits), 
		                                                m_raster_xsize, m_raster_ysize, 
		                                                GDT_Byte, 
		                                                3 , LineByteWidth );
	m_pGDALDataset->GetRasterBand(BandNo[2])->RasterIO( GF_Read, 0, 0, 
		                                                m_raster_xsize, m_raster_ysize, 
		                                                (void *)(pBBits), 
		                                                m_raster_xsize, m_raster_ysize, 
		                                                GDT_Byte, 
		                                                3 , LineByteWidth );
	GDALClose(m_pGDALDataset);

	BYTE *buf = new BYTE[LineByteWidth];
	BYTE *u1,*u2;
	for(int j = 0; j < (m_raster_ysize/2); j++ )
	{
		u1 = lpDIBBits + j * LineByteWidth;
		u2 = lpDIBBits + (m_raster_ysize - j - 1) * LineByteWidth;
		memcpy(buf, u1,  LineByteWidth);
		memcpy(u1,  u2,  LineByteWidth);
		memcpy(u2,  buf, LineByteWidth);
	}
	delete buf;
	::GlobalUnlock((HGLOBAL) m_hDIB);

	if(!IstheSame)
	{
		SendMessage(WM_INITIALUPDATE,0,0);
	}
}

void CImageView::PerformOnUpdateFoDrawSelect(CCmdUI* pCmdUI)
{
	OnUpdateFoDrawSelect(pCmdUI);
}

void CImageView::PerformOnUpdateFoCmdAutoPan(CCmdUI* pCmdUI)
{
	OnUpdateFoCmdAutoPan(pCmdUI);
}

void CImageView::PerformOnUpdateEditUndo(CCmdUI* pCmdUI)
{
	OnUpdateEditUndo(pCmdUI);
}

void CImageView::PerformOnUpdateEditRedo(CCmdUI* pCmdUI)
{
	OnUpdateEditRedo(pCmdUI);
}

void CImageView::PerformOnUpdateFoEditSpot(CCmdUI* pCmdUI)
{
	OnUpdateFoEditSpot(pCmdUI);
}

void CImageView::PerformOnUpdateFoDrawLine(CCmdUI* pCmdUI)
{
	OnUpdateFoDrawLine(pCmdUI);
}

void CImageView::PerformOnUpdateFoDrawArrowLine(CCmdUI* pCmdUI)
{
	OnUpdateFoDrawArrowLine(pCmdUI);
}

void CImageView::PerformOnUpdateFoDrawCrossLine(CCmdUI* pCmdUI)
{
	OnUpdateFoDrawCrossLine(pCmdUI);
}

void CImageView::PerformOnUpdateFoDrawPolyLine(CCmdUI* pCmdUI)
{
	OnUpdateFoDrawPolyLine(pCmdUI);
}

void CImageView::PerformOnUpdateFoDrawFreeLine(CCmdUI* pCmdUI)
{
	OnUpdateFoDrawFreeLine(pCmdUI);
}

void CImageView::PerformOnUpdateFoDrawPolygon(CCmdUI* pCmdUI)
{
	OnUpdateFoDrawPolygon(pCmdUI);
}

void CImageView::PerformOnUpdateFoDrawEllipse(CCmdUI* pCmdUI)
{
	OnUpdateFoDrawEllipse(pCmdUI);
}

void CImageView::PerformOnUpdateFoDrawUprightCircle(CCmdUI* pCmdUI)
{
	OnUpdateFoDrawUprightCircle(pCmdUI);
}

void CImageView::PerformOnUpdateFoDrawRectangle(CCmdUI* pCmdUI)
{
	OnUpdateFoDrawRectangle(pCmdUI);
}

void CImageView::PerformOnUpdateFoUprightRect(CCmdUI* pCmdUI)
{
	OnUpdateFoUprightRect(pCmdUI);
}

void CImageView::PerformOnUpdateFoDrawFreeCloseLine(CCmdUI* pCmdUI)
{
	OnUpdateFoDrawFreeCloseLine(pCmdUI);
}

void CImageView::PerformOnUpdateFoDrawStatic(CCmdUI* pCmdUI)
{
	OnUpdateFoDrawStatic(pCmdUI);
}

void CImageView::PerformOnUpdateFoDoDrawImage(CCmdUI* pCmdUI)
{
	OnUpdateFoDoDrawImage(pCmdUI);
}

void CImageView::PerformOnUpdateFoViewScaleadd25(CCmdUI* pCmdUI)
{
	OnUpdateFoViewScaleadd25(pCmdUI);
}

void CImageView::PerformOnUpdateFoViewScalesub25(CCmdUI* pCmdUI)
{
	OnUpdateFoViewScalesub25(pCmdUI);
}

void CImageView::PerformOnUpdateFoViewZoom100(CCmdUI* pCmdUI)
{
	OnUpdateFoViewZoom100(pCmdUI);
}

void CImageView::PerformOnUpdateFoZoomWithinRect(CCmdUI* pCmdUI)
{
	OnUpdateFoZoomWithinRect(pCmdUI);
}

void CImageView::PerformOnUpdateFoZoomfitpage(CCmdUI* pCmdUI)
{
	OnUpdateFoZoomfitpage(pCmdUI);
}

void CImageView::PerformOnUpdateFoZoomfitheight(CCmdUI* pCmdUI)
{
	OnUpdateFoZoomfitheight(pCmdUI);
}

void CImageView::PerformOnUpdateFoZoomfitwidth(CCmdUI* pCmdUI)
{
	OnUpdateFoZoomfitwidth(pCmdUI);
}

void CImageView::PerformOnUpdateFoZoomFitselect(CCmdUI* pCmdUI)
{
	OnUpdateFoZoomFitselect(pCmdUI);
}

LRESULT CImageView::OnContentRefresh(WPARAM wParam, LPARAM lParam)
{
	if (m_hDIB != NULL)
	{
		::GlobalFree((HGLOBAL) m_hDIB);
		m_hDIB = NULL;
	}

	SendMessage(WM_INITIALUPDATE,0,0);
	return 1;
}

void CImageView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CString sTempPath, sTempFile;
CString tempPath; ::GetTempPath(1024, tempPath.GetBufferSetLength(1024)); tempPath.ReleaseBuffer(); 
	// TODO: Add your message handler code here and/or call default
	if( ((CMainFrame*)AfxGetMainWnd())->m_realtime )
	{
		HDIB hDIB = ((CMainFrame*)AfxGetMainWnd())->CaptureOneImage();
		if( hDIB != NULL )
		{
			//-----------------------------------------
			if(fabs(theApp.m_ImgZoomRate - 1.0) > 0.00001)
			{
				::GetTempPath(1024, sTempPath.GetBufferSetLength(1024));
				sTempPath.ReleaseBuffer();
				sTempFile = sTempPath + "\\resample.bmp";

				remove(sTempFile);

				CFile file;
				CFileException fe;
				
				if (file.Open(sTempFile, CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
				{
					::SaveDIB( hDIB, file );
					file.Close();

					GDALDataset * m_pSrcDataset;
					m_pSrcDataset = (GDALDataset *) GDALOpen ( sTempFile, GA_ReadOnly );
					int OrigenWidth  = m_pSrcDataset->GetRasterXSize();
					int OrigenHeight = m_pSrcDataset->GetRasterYSize();
					int NewWidth  = (int)( OrigenWidth  * theApp.m_ImgZoomRate );
					int NewHeight = (int)( OrigenHeight * theApp.m_ImgZoomRate );

					if (hDIB != NULL)
					{
						::GlobalFree((HGLOBAL) hDIB);
						hDIB = NULL;
					}

					hDIB = ::CreateDIBFile(NewWidth,NewHeight,24);
					LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
					BYTE *   lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
					int LineByteWidth = WIDTHBYTES( NewWidth * 24 );
					BYTE *pRBits = lpDIBBits + 2;
					BYTE *pGBits = lpDIBBits + 1;
					BYTE *pBBits = lpDIBBits + 0;
					
					m_pSrcDataset->GetRasterBand(1)->RasterIO( GF_Read, 0, 0, 
						                                       OrigenWidth, 
						                                       OrigenHeight, 
						                                       (void *)(pRBits), 
						                                       NewWidth, 
						                                       NewHeight, 
						                                       GDT_Byte, 
						                                       3 , LineByteWidth );
					m_pSrcDataset->GetRasterBand(2)->RasterIO( GF_Read, 0, 0, 
						                                       OrigenWidth, 
						                                       OrigenHeight, 
						                                       (void *)(pGBits), 
						                                       NewWidth, 
						                                       NewHeight, 
						                                       GDT_Byte, 
						                                       3 , LineByteWidth );
					m_pSrcDataset->GetRasterBand(3)->RasterIO( GF_Read, 0, 0, 
						                                       OrigenWidth, 
						                                       OrigenHeight, 
						                                       (void *)(pBBits), 
						                                       NewWidth, 
						                                       NewHeight, 
						                                       GDT_Byte, 
						                                       3 , LineByteWidth );
					
					BYTE *buf = new BYTE[LineByteWidth];
					BYTE *u1,*u2;
					for(int j = 0; j < (NewHeight/2); j++ )
					{
						u1 = lpDIBBits + j * LineByteWidth;
						u2 = lpDIBBits + (NewHeight - j - 1) * LineByteWidth;
						memcpy(buf, u1,  LineByteWidth);
						memcpy(u1,  u2,  LineByteWidth);
						memcpy(u2,  buf, LineByteWidth);
					}
					SAFE_DELETE(buf)
					
					::GlobalUnlock((HGLOBAL) hDIB);
					
					GDALClose(m_pSrcDataset);	m_pSrcDataset = NULL;
					remove(sTempFile);
				}
			}
			//------------图像文字标注 ----------------
			if(!(theApp.m_Tag_LeftBottom.IsEmpty() && theApp.m_Tag_RightBottom.IsEmpty()))
			{
				CString LeftBottomStr  = theApp.m_Tag_LeftBottom;
				CString RightBottomStr = theApp.m_Tag_RightBottom;
				LeftBottomStr.TrimLeft();   LeftBottomStr.TrimRight();
				RightBottomStr.TrimLeft();  RightBottomStr.TrimRight();
				
				if(LeftBottomStr.CompareNoCase("medicalno") == 0 )
				{
					LeftBottomStr = theApp.GetView()->GetDocument()->m_blh;
					
				}
				else if(LeftBottomStr.CompareNoCase("bdh") == 0) 
				{
					try
					{
						SAString cmdstr = "select bdh from " + theApp.TABLE_MAIN + " where medicalno=:medicalno";
						g_dbcommand.setCommandText(cmdstr);
						g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
						g_dbcommand.Execute();
						while(g_dbcommand.FetchNext()){
							LeftBottomStr = g_dbcommand.Field("bdh").asString();
						}
					} 
					catch(SAException &e)
					{
						AfxMessageBox((const char*)e.ErrText());
					}
				}

				if(RightBottomStr.CompareNoCase("medicalno") == 0)
				{
					RightBottomStr = theApp.GetView()->GetDocument()->m_blh;
				}
				else if(RightBottomStr.CompareNoCase("bdh") == 0)
				{
					try
					{
						SAString cmdstr = "select bdh from " + theApp.TABLE_MAIN + " where medicalno=:medicalno";
						g_dbcommand.setCommandText(cmdstr);
						g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
						g_dbcommand.Execute();
						while(g_dbcommand.FetchNext()){
							RightBottomStr = g_dbcommand.Field("bdh").asString();
						}
					} 
					catch(SAException &e)
					{
						AfxMessageBox((const char*)e.ErrText());
					}
				}

				//--------------------------------------

				LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
				BYTE *   lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
				int cxdib = ::DIBWidth(lpDIBHdr);
				int cydib = ::DIBHeight(lpDIBHdr);
				CCanvasImage* m_pAtlImage = new CCanvasImage();
				
				m_pAtlImage->Create(cxdib, cydib, 24);
				HDC hDC = m_pAtlImage->GetDC();
				
				CRect rect;
				rect.SetRect(0,0,cxdib, cydib);
				::PaintDIB(hDC,	rect, hDIB, rect, NULL);
				
				CDC* pDstDC = CDC::FromHandle(hDC);
				
				CFont* pFont = new CFont();
				pFont->CreateFontIndirect(&theApp.m_lfFont);
				CFont* pPrevFont = NULL;
				pPrevFont = (CFont*) pDstDC->SelectObject(pFont);
				
				COLORREF PreColor = pDstDC->SetTextColor(theApp.m_lfColor);
				int PreBkMode = pDstDC->SetBkMode(TRANSPARENT);
				
				CSize sizeLine(0,0);
				if(!LeftBottomStr.IsEmpty())
				{
					sizeLine = pDstDC->GetTextExtent(LeftBottomStr);
					pDstDC->TextOut(5, cydib - (sizeLine.cy + 5), LeftBottomStr);
				}
				if(!RightBottomStr.IsEmpty())
				{
					sizeLine = pDstDC->GetTextExtent(RightBottomStr);
					pDstDC->TextOut(cxdib - (sizeLine.cx + 5), cydib - (sizeLine.cy + 5), RightBottomStr);
				}
				
				pDstDC->SelectObject(pPrevFont);
				SAFE_DELETE(pFont);
				
				pDstDC->SetBkMode(PreBkMode);
				pDstDC->SetTextColor(PreColor);
				
				if(!(LeftBottomStr.IsEmpty() && RightBottomStr.IsEmpty()))
				{
					BYTE *pDIBBits = m_pAtlImage->GetBitmapBits();
					int LineWidth = m_pAtlImage->m_widthbyte;
					
					int LineByteWidth = WIDTHBYTES( cxdib * 24 );
					BYTE *ptr, *ptr1;
					for(int i = 0; i < cydib; i ++ )
					{
						ptr  = lpDIBBits + i * LineByteWidth;
						ptr1 = pDIBBits  + (cydib - i - 1) * LineWidth;
						memcpy(ptr, ptr1, cxdib * 3);
					}
					
					if( pDIBBits != NULL )
					{
						delete pDIBBits;
						pDIBBits = NULL;
					}
				}
				
				::GlobalUnlock((HGLOBAL) hDIB);
				
				m_pAtlImage->ReleaseDC();
				
				if( m_pAtlImage )
				{
					delete m_pAtlImage;
					m_pAtlImage =NULL;
				}
				//--------------------------------------
			}
			//-----------------------------------------

			((CMainFrame*)AfxGetMainWnd())->AddNewImage(hDIB);
			::GlobalFree((HGLOBAL) hDIB);
			hDIB = NULL;
			
			((CMainFrame*)AfxGetMainWnd())->Sound_Play("camera.wav");
		}
		return;
	}
	
	CFODrawView::OnLButtonDown(nFlags, point);
}

void CImageView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( ((CMainFrame*)AfxGetMainWnd())->m_realtime )
	{
		((CMainFrame*)AfxGetMainWnd())->TerminateTimer();
		return;
	}
	
	CFODrawView::OnRButtonUp(nFlags, point);
}

void CImageView::OnImagepsColorrectify() 
{
	// TODO: Add your command handler code here
	if( m_action_state != 1002 )
	{
		hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR_RECTIFY);	
		m_action_state = 1002;  // Magic Bar
		m_drawshape = FO_SHAPE_BASE + 1001;
	}
	else
	{
		SendMessage(WM_COMMAND,ID_FO_DRAW_SELECT);
	}
}

void CImageView::OnUpdateImagepsColorrectify(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_action_state == 1002);	
	pCmdUI->Enable(m_hDIB != NULL);
}

BOOL CImageView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_action_state > 1000 )
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);
		
		CRect rt;
		GetClientRect(rt);
		
		if(rt.PtInRect(pt))
		{
			if(GetCursor() != hCursor)
				SetCursor(hCursor);
		}
		else
		{
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		}
		return 1;
	}
	
	return CFODrawView::OnSetCursor(pWnd, nHitTest, message);
}

void CImageView::PerformOnUpdateImagepsColorrectify(CCmdUI* pCmdUI)
{
	OnUpdateImagepsColorrectify(pCmdUI);
}

void CImageView::Perform_UI_tools(int index, CPoint point)
{
	switch( index )
	{
	case 1001:  // 区域增长
		{
			CPoint pt = point;
			FODPtoLP(&pt);
			CRect ImageArea;
			int cxDIB,cyDIB;
			if( m_hDIB != NULL )
			{
				LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
				cxDIB = ::DIBWidth(lpDIBHdr);
				cyDIB = ::DIBHeight(lpDIBHdr);
				::GlobalUnlock((HGLOBAL) m_hDIB);

				ImageArea.SetRect(0,0,cxDIB,cyDIB);
			}
	        if( m_hDIB != NULL && ImageArea.PtInRect(pt))
			{
				m_hot_point = pt;

				HDIB m_cur_dib = (HDIB)CopyHandle(m_hDIB);
				
				LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_cur_dib);
				BYTE *   lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
				int LineByteWidth = WIDTHBYTES( cxDIB * 24 );

				BYTE *buf = new BYTE[LineByteWidth];
				BYTE *u1,*u2;
				for(int j = 0; j < (cyDIB/2); j++ )
				{
					u1 = lpDIBBits + j * LineByteWidth;
					u2 = lpDIBBits + (cyDIB - j - 1) * LineByteWidth;
					memcpy(buf, u1,  LineByteWidth);
					memcpy(u1,  u2,  LineByteWidth);
					memcpy(u2,  buf, LineByteWidth);
				}
				delete buf;

				CString str;
				m_pParent->m_wndEditGrow.GetWindowText(str);
				int n_threshold = (int)atoi(str);

				CPointList n_pointlist;
				n_pointlist.RemoveAll();
				
				grow( lpDIBBits, cxDIB , cyDIB,
					  LineByteWidth,
					  pt,
					  n_threshold,
					  n_pointlist);
				
				::GlobalUnlock((HGLOBAL) m_cur_dib);
				::GlobalFree((HGLOBAL) m_cur_dib);

				CArray<CPoint,CPoint> ptArray;
				CPoint sb;
				POSITION Pos = n_pointlist.GetHeadPosition();
				while (Pos != NULL)
				{
					sb = (CPoint)n_pointlist.GetNext(Pos);
					ptArray.Add(sb);
				}

				CFOPolygonShape* pPolygonShape =new CFOPolygonShape;
				pPolygonShape->Create(&ptArray);

				CRect AreaRect = pPolygonShape->GetRect();
				if( AreaRect.Width() < 3 || AreaRect.Height() < 3 )
				{
					if( pPolygonShape != NULL )
					{
						delete pPolygonShape;
						pPolygonShape = NULL;
					}
				}
				else
				{
					pPolygonShape->SetLineColor(RGB(255,255,255));
					pPolygonShape->SetBrushType(PS_SOLID);
					GetCurrentModel()->AddShape(pPolygonShape);			

					m_listSelectComp.RemoveAll();
					m_listSelectComp.AddTail(pPolygonShape);

					RedrawWindow();
				}
				
				ptArray.RemoveAll();
				n_pointlist.RemoveAll();
			}
		}
		break;
	case 1002:  // 背底校正
		{
			CPoint pt = point;
			FODPtoLP(&pt);
			CRect ImageArea;
			int cxDIB,cyDIB;
			if( m_hDIB != NULL )
			{
				LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
				cxDIB = ::DIBWidth(lpDIBHdr);
				cyDIB = ::DIBHeight(lpDIBHdr);
				::GlobalUnlock((HGLOBAL) m_hDIB);

				ImageArea.SetRect(0,0,cxDIB,cyDIB);
			}

	        if( m_hDIB != NULL && ImageArea.PtInRect(pt))
			{
				m_hot_point = pt;

				LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock(m_hDIB);
				BYTE *   lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
				int LineByteWidth = WIDTHBYTES( cxDIB * 24 );

				CRect rt;
				rt.SetRect(pt.x, pt.y, pt.x, pt.y);
				rt.InflateRect(3, 3);
	
				m_Target_Threshold = 255;

				BOOL result = ImageBkRectification( lpDIBBits, 
					                                cxDIB , 
													cyDIB,
					                                LineByteWidth,
					                                rt, m_Target_Threshold);
				
				::GlobalUnlock((HGLOBAL) m_hDIB);

				if( result ) RedrawWindow();

				if(AfxMessageBox("是否保存修改后图像？",MB_YESNO|MB_ICONQUESTION) == IDYES)
				{
					OnImageSave();
				}
			}
		}
		break;
	}
}

void CImageView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_action_state > 1000 )
	{
		Perform_UI_tools(m_action_state,point);
	}
	
	CFODrawView::OnLButtonUp(nFlags, point);

	//=========================================================================
	CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	if( pAppFrame->m_wndOutputBar.IsWindowVisible() )
	{
		int n_count = GetCurrentModel()->GetFormObjects()->GetCount();
		pAppFrame->m_wndOutputBar.m_MeasureDialog.m_MeasureCtrl.SetNumberRows(n_count);

		CFODrawShape *pSelectedObj = NULL;
		POSITION pos = m_listSelectComp.GetHeadPosition();
		if(pos != NULL)	pSelectedObj = (CFODrawShape *)m_listSelectComp.GetNext(pos);
		if( pSelectedObj != NULL )
		{
			int index = GetCurrentModel()->FindShapePosition(pSelectedObj);
			
			pAppFrame->m_wndOutputBar.m_MeasureDialog.m_MeasureCtrl.GotoRow(index);
		}

		pAppFrame->m_wndOutputBar.m_MeasureDialog.m_MeasureCtrl.RedrawAll();
	}
}

void CImageView::OnEditMagic() 
{
	// TODO: Add your command handler code here
	hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR_MAGIC);	
	m_action_state = 1001;  // Magic Bar
	m_drawshape = FO_SHAPE_BASE + 1001;
}

void CImageView::OnUpdateEditMagic(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_action_state == 1001);	
	pCmdUI->Enable(m_hDIB != NULL);
}

void CImageView::PerformOnUpdateEditMagic(CCmdUI* pCmdUI)
{
	OnUpdateEditMagic(pCmdUI);
}

void CImageView::OnValueAdd() 
{
	// TODO: Add your command handler code here
	if ( m_hDIB == NULL )  return;

	LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	int cxDIB = ::DIBWidth(lpDIBHdr);
	int cyDIB = ::DIBHeight(lpDIBHdr);
	::GlobalUnlock((HGLOBAL) m_hDIB);

	if(m_action_state == 1002)
	{
		CRect ImageArea;
		ImageArea.SetRect(0,0,cxDIB,cyDIB);
		if( !ImageArea.PtInRect(m_hot_point) )  return;

		if( m_Target_Threshold >= 255 )  return;
		m_Target_Threshold++;

		// Reloading Original Image here ......
		CWnd *pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CMedImage));
		if(pWnd != NULL)  ((CMedImage *)pWnd)->ReLoading();

		LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
		BYTE *   lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
		int LineByteWidth = WIDTHBYTES( cxDIB * 24 );
		
		CRect rt;
		rt.SetRect(m_hot_point.x, m_hot_point.y, m_hot_point.x, m_hot_point.y);
		rt.InflateRect(3, 3);
		
		BOOL result = ImageBkRectification( lpDIBBits, 
			cxDIB , 
			cyDIB,
			LineByteWidth,
			rt, m_Target_Threshold);
		
		::GlobalUnlock((HGLOBAL) m_hDIB);
		
		if( result ) RedrawWindow();
	}

	if( m_action_state == 1001 )
	{
		CFODrawShape *pShape = NULL;
		POSITION pos = m_listSelectComp.GetHeadPosition();
		if(pos != NULL)	pShape = (CFODrawShape *)m_listSelectComp.GetNext(pos);
		if ( pShape == NULL || pShape->GetType() != FO_COMP_POLYGON )  return;
		CRect ImageArea;
		ImageArea.SetRect(0,0,cxDIB,cyDIB);
		if( !ImageArea.PtInRect(m_hot_point) )  return;
	
		CString str;
		m_pParent->m_wndEditGrow.GetWindowText(str);
		int n_threshold = (int)atoi(str);
		if( n_threshold > 500 )   return;

		HDIB m_cur_dib = (HDIB)CopyHandle(m_hDIB);
		
		LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_cur_dib);
		BYTE *   lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
		int LineByteWidth = WIDTHBYTES( cxDIB * 24 );
		
		BYTE *buf = new BYTE[LineByteWidth];
		BYTE *u1,*u2;
		for(int j = 0; j < (cyDIB/2); j++ )
		{
			u1 = lpDIBBits + j * LineByteWidth;
			u2 = lpDIBBits + (cyDIB - j - 1) * LineByteWidth;
			memcpy(buf, u1,  LineByteWidth);
			memcpy(u1,  u2,  LineByteWidth);
			memcpy(u2,  buf, LineByteWidth);
		}
		delete buf;
		
		n_threshold++;                               // new lines
		str.Format("%d",n_threshold);
		m_pParent->m_wndEditGrow.SetWindowText(str);
		
		CPointList n_pointlist;
		n_pointlist.RemoveAll();
		
		grow( lpDIBBits, cxDIB , cyDIB,
			LineByteWidth,
			m_hot_point,
			n_threshold,
			n_pointlist);
		
		::GlobalUnlock((HGLOBAL) m_cur_dib);
		::GlobalFree((HGLOBAL) m_cur_dib);
		
		CArray<CPoint,CPoint> ptArray;
		CPoint sb;
		POSITION Pos = n_pointlist.GetHeadPosition();
		while (Pos != NULL)
		{
			sb = (CPoint)n_pointlist.GetNext(Pos);
			ptArray.Add(sb);
		}
		
		CFOPolygonShape* pPolygonShape =new CFOPolygonShape;
		pPolygonShape->Create(&ptArray);
		
		CRect AreaRect = pPolygonShape->GetRect();
		if( AreaRect.Width() < 3 || AreaRect.Height() < 3 )
		{
			if( pPolygonShape != NULL )
			{
				delete pPolygonShape;
				pPolygonShape = NULL;
			}
		}
		else
		{
			m_listSelectComp.RemoveAll();
			GetCurrentModel()->RemoveShape( pShape );
			
			pPolygonShape->SetLineColor(RGB(255,255,255));
			pPolygonShape->SetBrushType(PS_SOLID);
			GetCurrentModel()->AddShape(pPolygonShape);			
			
			m_listSelectComp.AddTail(pPolygonShape);
			
			RedrawWindow();
		}
		
		ptArray.RemoveAll();
		n_pointlist.RemoveAll();
	}
    
	return;
}

void CImageView::OnValueDelete() 
{
	// TODO: Add your command handler code here
	if ( m_hDIB == NULL )  return;

	LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	int cxDIB = ::DIBWidth(lpDIBHdr);
	int cyDIB = ::DIBHeight(lpDIBHdr);
	::GlobalUnlock((HGLOBAL) m_hDIB);

	if(m_action_state == 1002)
	{
		CRect ImageArea;
		ImageArea.SetRect(0,0,cxDIB,cyDIB);
		if( !ImageArea.PtInRect(m_hot_point) )  return;

		if( m_Target_Threshold < 50 )  return;
		m_Target_Threshold--;

		// Reloading Original Image here ......
		CWnd *pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CMedImage));
		if(pWnd != NULL)  ((CMedImage *)pWnd)->ReLoading();

		LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
		BYTE *   lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
		int LineByteWidth = WIDTHBYTES( cxDIB * 24 );
		
		CRect rt;
		rt.SetRect(m_hot_point.x, m_hot_point.y, m_hot_point.x, m_hot_point.y);
		rt.InflateRect(3, 3);
		
		BOOL result = ImageBkRectification( lpDIBBits, 
			cxDIB , 
			cyDIB,
			LineByteWidth,
			rt, m_Target_Threshold);
		
		::GlobalUnlock((HGLOBAL) m_hDIB);
		
		if( result ) RedrawWindow();
	}

	if( m_action_state == 1001 )
	{
		CFODrawShape *pShape = NULL;
		POSITION pos = m_listSelectComp.GetHeadPosition();
		if(pos != NULL)	pShape = (CFODrawShape *)m_listSelectComp.GetNext(pos);
		if ( pShape == NULL || pShape->GetType() != FO_COMP_POLYGON )  return;
		CRect ImageArea;
		ImageArea.SetRect(0,0,cxDIB,cyDIB);
		if( !ImageArea.PtInRect(m_hot_point) )  return;

		CString str;
		m_pParent->m_wndEditGrow.GetWindowText(str);
		int n_threshold = (int)atoi(str);
		if( n_threshold <= 1 )   return;
		
		HDIB m_cur_dib = (HDIB)CopyHandle(m_hDIB);
		
		LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_cur_dib);
		BYTE *   lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
		int LineByteWidth = WIDTHBYTES( cxDIB * 24 );
		
		BYTE *buf = new BYTE[LineByteWidth];
		BYTE *u1,*u2;
		for(int j = 0; j < (cyDIB/2); j++ )
		{
			u1 = lpDIBBits + j * LineByteWidth;
			u2 = lpDIBBits + (cyDIB - j - 1) * LineByteWidth;
			memcpy(buf, u1,  LineByteWidth);
			memcpy(u1,  u2,  LineByteWidth);
			memcpy(u2,  buf, LineByteWidth);
		}
		delete buf;
		
		n_threshold--;                               // new lines
		str.Format("%d",n_threshold);
		m_pParent->m_wndEditGrow.SetWindowText(str);
		
		CPointList n_pointlist;
		n_pointlist.RemoveAll();
		
		grow( lpDIBBits, cxDIB , cyDIB,
			LineByteWidth,
			m_hot_point,
			n_threshold,
			n_pointlist);
		
		::GlobalUnlock((HGLOBAL) m_cur_dib);
		::GlobalFree((HGLOBAL) m_cur_dib);
		
		CArray<CPoint,CPoint> ptArray;
		CPoint sb;
		POSITION Pos = n_pointlist.GetHeadPosition();
		while (Pos != NULL)
		{
			sb = (CPoint)n_pointlist.GetNext(Pos);
			ptArray.Add(sb);
		}
		
		CFOPolygonShape* pPolygonShape =new CFOPolygonShape;
		pPolygonShape->Create(&ptArray);
		
		CRect AreaRect = pPolygonShape->GetRect();
		if( AreaRect.Width() < 3 || AreaRect.Height() < 3 )
		{
			if( pPolygonShape != NULL )
			{
				delete pPolygonShape;
				pPolygonShape = NULL;
			}
		}
		else
		{
			m_listSelectComp.RemoveAll();
			GetCurrentModel()->RemoveShape( pShape );
			
			pPolygonShape->SetLineColor(RGB(255,255,255));
			pPolygonShape->SetBrushType(PS_SOLID);
			GetCurrentModel()->AddShape(pPolygonShape);			
			
			m_listSelectComp.AddTail(pPolygonShape);
			
			RedrawWindow();
		}
		
		ptArray.RemoveAll();
		n_pointlist.RemoveAll();
	}
    
	return;
}

void CImageView::OnAoiRefer() 
{
	// TODO: Add your command handler code here
	CFORectShape* pFORectShape =new CFORectShape;
	CRect rt;
	rt.SetRect(0,0,theApp.m_Aoi_Width,theApp.m_Aoi_Height);
	if ( m_hDIB != NULL )
	{
		LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
		int cxDIB = ::DIBWidth(lpDIBHdr);
		int cyDIB = ::DIBHeight(lpDIBHdr);
		::GlobalUnlock((HGLOBAL) m_hDIB);

		int cx = theApp.m_Aoi_Width, cy = theApp.m_Aoi_Height;
		if(cx > cxDIB)   cx = cxDIB;
		if(cy > cyDIB)   cy = cyDIB;

		rt.SetRect((cxDIB - cx) / 2, (cyDIB - cy) / 2, (cxDIB - cx) / 2 + cx, (cyDIB - cy) / 2 + cy);
	}

	pFORectShape->Create(rt,_T(""));
	pFORectShape->SetBrushType(PS_SOLID);
	pFORectShape->SetLineWidth(1);
	pFORectShape->SetLineColor(RGB(0,0,0));
	pFORectShape->SetPenStyle(PS_SOLID);
	GetCurrentModel()->AddShape(pFORectShape);
	RedrawWindow();
}

void CImageView::OnAoiCut() 
{
	// TODO: Add your command handler code here
	if(m_hDIB == NULL)   return;

	BeginWaitCursor();

	CFile file;
	CFileException fe;
	CString OriImg = theApp.m_SysTmpPath;
	OriImg += "00000002.bmp";
	remove(OriImg);
	
	if (!file.Open( OriImg, CFile::modeCreate |
		CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		return;
	}
	
	TRY
	{
		::SaveDIB( m_hDIB, file );
		file.Close();
	}
	CATCH (CException, eSave)
	{
		file.Abort(); // will not throw an exception
		return;
	}
	END_CATCH

	CString tmpimg = theApp.m_SysTmpPath;
	tmpimg += "00000001.bmp";
	remove(tmpimg);

    if( AoiExtract(OriImg, tmpimg) == FALSE )
	{
		remove(OriImg);
		remove(tmpimg);
		EndWaitCursor();
		return;
	}

	((CMainFrame*)AfxGetMainWnd())->AddNewImage(tmpimg);

	remove(OriImg);
	remove(tmpimg);
	EndWaitCursor();
}

void CImageView::OnUpdateAoiCut(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_hDIB != NULL );
}

void CImageView::PerformOnUpdateAoiCut(CCmdUI* pCmdUI)
{
	OnUpdateAoiCut(pCmdUI);
}

BOOL CImageView::AoiExtract(CString SrcName, CString ImgName)
{
	CFODrawShape *pShape = NULL;
	POSITION pos = m_listSelectComp.GetHeadPosition();
	if(pos != NULL)	pShape = (CFODrawShape *)m_listSelectComp.GetNext(pos);
	if( pShape == NULL )  return FALSE;

	CRgn   m_Target_Rgn;

	UINT n_ShapeType = pShape->GetType();
	switch( n_ShapeType ) 
	{
	case FO_COMP_RECTANGLE:
		{
			CRect ShapeRect = pShape->GetRect();

			CPoint ptVertex[4];
			ptVertex[0].x = ShapeRect.left;
			ptVertex[0].y = ShapeRect.top;
			ptVertex[1].x = ShapeRect.right;
			ptVertex[1].y = ShapeRect.top;
			ptVertex[2].x = ShapeRect.right;
			ptVertex[2].y = ShapeRect.bottom;
			ptVertex[3].x = ShapeRect.left;
			ptVertex[3].y = ShapeRect.bottom;
			
			m_Target_Rgn.CreatePolygonRgn( ptVertex, 4, ALTERNATE);
		}
		break;
	case FO_COMP_POLYGON:
	case FO_COMP_FREECLOSELINE:
		{
			int spotcount = pShape->GetSpotCount();
			LPPOINT pt = pShape->GetPoints();
			m_Target_Rgn.CreatePolygonRgn( pt, spotcount, ALTERNATE);
		}
		break;
	case FO_COMP_ELLIPSE:
		{
			CRect ShapeRect = pShape->GetRect();
			m_Target_Rgn.CreateEllipticRgnIndirect(ShapeRect);
		}
		break;
	default:
		return FALSE;
	}
	
	CRect rectAOI;
	if(ERROR == m_Target_Rgn.GetRgnBox(&rectAOI))
	{
		m_Target_Rgn.DeleteObject();
		return FALSE;
	}

	int m_anAOIRect[4];
	m_anAOIRect[0] = rectAOI.left;
	m_anAOIRect[1] = rectAOI.top;
	m_anAOIRect[2] = rectAOI.right;
	m_anAOIRect[3] = rectAOI.bottom;
	int m_anLimitRect[4];
	memcpy(m_anLimitRect,m_anAOIRect,4*sizeof(int));

	LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	int nXSize = ::DIBWidth(lpDIBHdr);
	int nYSize = ::DIBHeight(lpDIBHdr);
	::GlobalUnlock((HGLOBAL) m_hDIB);

	//截去屏幕外的区域
	if(m_anLimitRect[0] < 0) m_anLimitRect[0] = 0;
	if(m_anLimitRect[0] > nXSize) m_anLimitRect[0] = nXSize;
	if(m_anLimitRect[1] < 0) m_anLimitRect[1] = 0;
	if(m_anLimitRect[1] > nYSize) m_anLimitRect[1] = nYSize;
	if(m_anLimitRect[2] < 0) m_anLimitRect[2] = 0;
	if(m_anLimitRect[2] > nXSize) m_anLimitRect[2] = nXSize;
	if(m_anLimitRect[3] < 0) m_anLimitRect[3] = 0;
	if(m_anLimitRect[3] > nYSize) m_anLimitRect[3] = nYSize;


	CFile file;
	CFileException fe;
	remove(ImgName);

	GDALDataset * m_pOrgDataset = (GDALDataset *) GDALOpen ( SrcName, GA_ReadOnly );
	if(NULL == m_pOrgDataset)
	{
		m_Target_Rgn.DeleteObject();
		return FALSE;
	}

	int m_nBandSel = m_pOrgDataset->GetRasterCount();

	//拷贝AOI区域数据
	rectAOI.SetRect(m_anAOIRect[0],m_anAOIRect[1],m_anAOIRect[2],m_anAOIRect[3]);
	if(rectAOI.Width() <= 0 || rectAOI.Height() <= 0)
	{
		m_Target_Rgn.DeleteObject();
		return FALSE;
	}
	
	GDALDataset *pAOIDataset = (GDALDataset *)GDALVrtOpen( m_pOrgDataset, m_nBandSel,
										      NULL, m_anAOIRect,
										      GDT_Unknown,0,0,0,0);
	if(NULL == pAOIDataset)
	{
		m_Target_Rgn.DeleteObject();
		return FALSE;
	}

	int nBandCount = pAOIDataset->GetRasterCount();//获取影像波段数目
	
	GDALRasterBand *poBand = m_pOrgDataset->GetRasterBand(1);
	GDALDataType eSrcDataType = GDALGetRasterDataType(poBand);

	GDALDataset *pOutputDataset = GS_CustomCreateWithHeader("BMP",ImgName,
					                           pAOIDataset,rectAOI.Width(),rectAOI.Height(),
					                           m_nBandSel,eSrcDataType,NULL);
	int iBand,i,j,iLimit,jLimit;
	int nRasterXSize = pOutputDataset->GetRasterXSize();
	int nRasterYSize = pOutputDataset->GetRasterYSize();
	int nXOffset = m_anLimitRect[0]-rectAOI.left;// m_anAOIRect[0];
	int nYOffset = m_anLimitRect[1]-rectAOI.top;// m_anAOIRect[1]; 
	double fTProcess = nBandCount*nRasterXSize;
	double fValue = 0.;

	BYTE *buf = NULL;
	if(n_ShapeType == FO_COMP_RECTANGLE)
	{
		buf = new BYTE[nRasterXSize * nRasterYSize];
	}

	for(iBand=0 ; iBand<nBandCount ;iBand++)
	{
		GDALRasterBand *pBand = pOutputDataset->GetRasterBand(iBand+1);
		GDALRasterBand *pSrcBand = pAOIDataset->GetRasterBand(iBand+1);
		if(buf == NULL)
		{
			for(i=0;i<nRasterXSize;i++)
			{
				iLimit = i + rectAOI.left;
				for(j=0;j<nRasterYSize;j++)
				{
					jLimit = j + rectAOI.top;
					if(m_Target_Rgn.PtInRegion(iLimit,jLimit))
					{
						pSrcBand->PixelIO(GF_Read,i-nXOffset,j-nYOffset,&fValue);
						pBand->PixelIO(GF_Write,i,j,&fValue);
					}
				}
			}
		}
		else
		{
	   		pSrcBand->RasterIO( GF_Read, 0, 0, 
		                        nRasterXSize, nRasterYSize, 
		                        (void *)(buf), 
		                        nRasterXSize, nRasterYSize, 
                                GDT_Byte, 
                                1 , nRasterXSize );
	   		pBand->RasterIO(    GF_Write, 0, 0, 
		                        nRasterXSize, nRasterYSize, 
		                        (void *)(buf), 
		                        nRasterXSize, nRasterYSize, 
                                GDT_Byte, 
                                1 , nRasterXSize );
		}
	}

	if( buf != NULL )
	{
		delete buf;
		buf = NULL;
	}

	GDALClose(pAOIDataset);
	GDALClose(pOutputDataset);
	GDALClose(m_pOrgDataset);

	m_Target_Rgn.DeleteObject();
	
	DoDeleteAction(&m_listSelectComp);
	return TRUE;
}

void CImageView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CFODrawShape *pShape = NULL;
	POSITION pos = m_listSelectComp.GetHeadPosition();
	if(pos != NULL)	pShape = (CFODrawShape *)m_listSelectComp.GetNext(pos);
	if( pShape != NULL )
	{
		UINT n_ShapeType = pShape->GetType();
		if( n_ShapeType == FO_COMP_RECTANGLE || 
			n_ShapeType == FO_COMP_POLYGON || 
			n_ShapeType == FO_COMP_FREECLOSELINE || 
			n_ShapeType == FO_COMP_ELLIPSE)
		{
			PostMessage(WM_COMMAND, ID_AOI_CUT);
			return;
		}
	}
	
	CFODrawView::OnLButtonDblClk(nFlags, point);
}

BOOL CImageView::MyExportAsBmpfile(CString tmpimg)
{
	CFOBitmap dib;

	CDC dcMem;
	CBitmap bmp;
	CDC dcScreen;
	
	dcScreen.Attach(::GetDC(0));
	
	CDC *pDC = &dcScreen;
	
	dcMem.CreateCompatibleDC(pDC);
	
	LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	int cxDIB = ::DIBWidth(lpDIBHdr);
	int cyDIB = ::DIBHeight(lpDIBHdr);
	::GlobalUnlock((HGLOBAL) m_hDIB);

	CSize szBitmap(cxDIB, cyDIB);
	
	BOOL bBmpCreated = bmp.CreateCompatibleBitmap(pDC, szBitmap.cx, szBitmap.cy);
	
	if (bBmpCreated)
	{
		dcMem.SelectObject(&bmp);
		
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right  = cxDIB;
		rcDIB.bottom = cyDIB;

		::PaintDIB(dcMem.m_hDC, &rcDIB, m_hDIB, &rcDIB, NULL);
		
		//GetCurrentModel()->OnDraw(&dcMem);
		CRect rcTemp;
		CFODrawShape* pObj = NULL;
		CFODrawShapeIterator i(&m_listSelectComp);
		for (pObj = i.GetFirst(); pObj != NULL; pObj = i.GetNext())
		{
			if(pObj != NULL)
			{
				rcTemp = pObj->GetRect();
				GetCurrentModel()->NormalRect(rcTemp);
				pObj->SetRect(rcTemp);
				pObj->SetPrevRect(pObj->GetRect());
				GetCurrentModel()->DoDrawPorts(pObj);
				pObj->OnDraw(&dcMem);
			}
		}
		
		dib.MakeNewBitmap(&dcMem, &bmp);
	}
	
	dcScreen.Detach();
	
	BOOL bReturn = dib.Save(tmpimg);
	return TRUE;
}

void CImageView::OnImageMark() 
{
	// TODO: Add your command handler code here
	CString tmpimg = theApp.m_SysTmpPath;
	tmpimg += "00000001.bmp";
	remove(tmpimg);

	BeginWaitCursor();
    
	if ( !MyExportAsBmpfile(tmpimg) )
	{
		remove(tmpimg);
		EndWaitCursor();
		return;
	}

	CFile file;
	CFileException fe;
	if (!file.Open(tmpimg, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		remove(tmpimg);
		EndWaitCursor();
		return;
	}

	if (m_hDIB != NULL)
	{
		::GlobalFree((HGLOBAL) m_hDIB);
		m_hDIB = NULL;
	}

	TRY
	{
		m_hDIB = ::ReadDIBFile(file);
		file.Close();
	}
	CATCH (CFileException, eLoad)
	{
		file.Abort(); // will not throw an exception
		remove(tmpimg);
		EndWaitCursor();
		return;
	}
	END_CATCH

	CString transname;
	transname.Format("%s_cmipstrans.dat", theApp.GetView()->GetDocument()->m_blh);
	CString fstr = theApp.m_SysTmpPath;
	fstr += transname;

	GDALDataset * m_pSrcDataset = (GDALDataset *) GDALOpen ( tmpimg, GA_ReadOnly );
	GDALDataset * m_pDstDataset = GS_CreateCopy( theApp.m_SavingMode, fstr, m_pSrcDataset);
	GDALClose(m_pSrcDataset);
	GDALClose(m_pDstDataset);

	CWnd *pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CMedImage));
	if(pWnd != NULL)  ((CMedImage *)pWnd)->SaveModifiedImg(fstr);

	remove(fstr);
	remove(tmpimg);

	DoDeleteAction(&m_listSelectComp);
	
	EndWaitCursor();

	RedrawWindow();

//	if(AfxMessageBox("是否保存修改后图像？",MB_OKCANCEL|MB_ICONQUESTION) == IDOK)
//	{
		OnImageSave();
//	}
}

void CImageView::OnUpdateImageMark(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( m_hDIB != NULL && m_listSelectComp.GetCount() > 0);
}

void CImageView::PerformOnUpdateImageMark(CCmdUI* pCmdUI)
{
	OnUpdateImageMark(pCmdUI);
}

void CImageView::OnImageCut() 
{
	// TODO: Add your command handler code here
	CString SrcName = theApp.m_SysTmpPath;
	SrcName += "00000002.bmp";
	remove(SrcName);
	CFile file;
	CFileException fe;
	
	if (!file.Open(SrcName, CFile::modeCreate |
		CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		return;
	}

	BeginWaitCursor();
	
	TRY
	{
		::SaveDIB( m_hDIB, file );
		file.Close();
	}
	CATCH (CException, eSave)
	{
		file.Abort(); // will not throw an exception
		EndWaitCursor();
		return;
	}
	END_CATCH

	CString tmpimg = theApp.m_SysTmpPath;
	tmpimg += "00000001.bmp";
	remove(tmpimg);

	//-------------------------------------------
	CFODrawShape *pShape = NULL;
	POSITION pos = m_listSelectComp.GetHeadPosition();
	if(pos != NULL)	pShape = (CFODrawShape *)m_listSelectComp.GetNext(pos);
	if( pShape == NULL )
	{
		EndWaitCursor();
		return;
	}

	CRgn   m_Target_Rgn;

	UINT n_ShapeType = pShape->GetType();
	switch( n_ShapeType ) 
	{
	case FO_COMP_RECTANGLE:
		{
			CRect ShapeRect = pShape->GetRect();

			CPoint ptVertex[4];
			ptVertex[0].x = ShapeRect.left;
			ptVertex[0].y = ShapeRect.top;
			ptVertex[1].x = ShapeRect.right;
			ptVertex[1].y = ShapeRect.top;
			ptVertex[2].x = ShapeRect.right;
			ptVertex[2].y = ShapeRect.bottom;
			ptVertex[3].x = ShapeRect.left;
			ptVertex[3].y = ShapeRect.bottom;
			
			m_Target_Rgn.CreatePolygonRgn( ptVertex, 4, ALTERNATE);
		}
		break;
	case FO_COMP_POLYGON:
	case FO_COMP_FREECLOSELINE:
		{
			int spotcount = pShape->GetSpotCount();
			LPPOINT pt = pShape->GetPoints();
			m_Target_Rgn.CreatePolygonRgn( pt, spotcount, ALTERNATE);
		}
		break;
	case FO_COMP_ELLIPSE:
		{
			CRect ShapeRect = pShape->GetRect();
			m_Target_Rgn.CreateEllipticRgnIndirect(ShapeRect);
		}
		break;
	default:
		EndWaitCursor();
		return;
	}
	
	//-------------------------------------------

    if( AoiExtract(SrcName, tmpimg) == FALSE )
	{
		m_Target_Rgn.DeleteObject();
		remove(tmpimg);
		remove(SrcName);
		EndWaitCursor();
		return;
	}

	//-------------------------------------------
	LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	int cxdib = ::DIBWidth(lpDIBHdr);
	int cydib = ::DIBHeight(lpDIBHdr);
	BYTE *   lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
	int LineByteWidth = WIDTHBYTES( cxdib * 24 );
	
	BYTE *ptr;
	POINT pt;
	for(int i = 0; i < cydib; i++ )
	{
		ptr = lpDIBBits + (cydib - i - 1) * LineByteWidth;
		pt.y = i;
		for(int j = 0; j < cxdib; j++)
		{
			pt.x = j;
			if( !m_Target_Rgn.PtInRegion(pt) )  continue;
			memset(ptr + j * 3, 255, 3);
		}
	}
	::GlobalUnlock((HGLOBAL) m_hDIB);
	//-------------------------------------------

	m_Target_Rgn.DeleteObject();

	if (!file.Open(tmpimg, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		remove(tmpimg);
		remove(SrcName);
		EndWaitCursor();
		return;
	}

	HDIB hDIB = NULL;
	TRY
	{
		hDIB = ::ReadDIBFile(file);
		file.Close();
	}
	CATCH (CFileException, eLoad)
	{
		file.Abort(); // will not throw an exception
		remove(tmpimg);
		remove(SrcName);
		EndWaitCursor();
		return;
	}
	END_CATCH

	remove(tmpimg);
	remove(SrcName);

	if( hDIB == NULL )
	{
		EndWaitCursor();
		return;
	}

	if (OpenClipboard())
	{
		EmptyClipboard();
		SetClipboardData (CF_DIB, CopyHandle((HANDLE) hDIB) );
		CloseClipboard();
	}
	
	::GlobalFree((HGLOBAL) hDIB);
    hDIB = NULL;

	EndWaitCursor();
}

void CImageView::OnUpdateImageCut(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CFODrawShape *pShape = NULL;
	POSITION pos = m_listSelectComp.GetHeadPosition();
	if(pos != NULL)	pShape = (CFODrawShape *)m_listSelectComp.GetNext(pos);

	pCmdUI->Enable( m_hDIB != NULL && pShape != NULL && 
					(pShape->GetType() == FO_COMP_RECTANGLE ||
					 pShape->GetType() == FO_COMP_POLYGON ||
					 pShape->GetType() == FO_COMP_FREECLOSELINE ||
					 pShape->GetType() == FO_COMP_ELLIPSE) );
}

void CImageView::OnImageCopy() 
{
	// TODO: Add your command handler code here
	CString SrcName = theApp.m_SysTmpPath;
	SrcName += "00000002.bmp";
	remove(SrcName);
	CFile file;
	CFileException fe;
	
	if (!file.Open(SrcName, CFile::modeCreate |
		CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		return;
	}

	BeginWaitCursor();
	
	TRY
	{
		::SaveDIB( m_hDIB, file );
		file.Close();
	}
	CATCH (CException, eSave)
	{
		file.Abort(); // will not throw an exception
		EndWaitCursor();
		return;
	}
	END_CATCH

	CString tmpimg = theApp.m_SysTmpPath;
	tmpimg += "00000001.bmp";
	remove(tmpimg);

    if( AoiExtract(SrcName, tmpimg) == FALSE )
	{
		remove(tmpimg);
		remove(SrcName);
		EndWaitCursor();
		return;
	}

	if (!file.Open(tmpimg, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		remove(tmpimg);
		remove(SrcName);
		EndWaitCursor();
		return;
	}

	HDIB hDIB = NULL;
	TRY
	{
		hDIB = ::ReadDIBFile(file);
		file.Close();
	}
	CATCH (CFileException, eLoad)
	{
		file.Abort(); // will not throw an exception
		remove(tmpimg);
		remove(SrcName);
		EndWaitCursor();
		return;
	}
	END_CATCH

	remove(tmpimg);
	remove(SrcName);

	if( hDIB == NULL )
	{
		EndWaitCursor();
		return;
	}

	if (OpenClipboard())
	{
		EmptyClipboard();
		SetClipboardData (CF_DIB, CopyHandle((HANDLE) hDIB) );
		CloseClipboard();
	}
	
	::GlobalFree((HGLOBAL) hDIB);
    hDIB = NULL;

	EndWaitCursor();
}

void CImageView::OnUpdateImageCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CFODrawShape *pShape = NULL;
	POSITION pos = m_listSelectComp.GetHeadPosition();
	if(pos != NULL)	pShape = (CFODrawShape *)m_listSelectComp.GetNext(pos);

	pCmdUI->Enable( m_hDIB != NULL && pShape != NULL && 
					(pShape->GetType() == FO_COMP_RECTANGLE ||
					 pShape->GetType() == FO_COMP_POLYGON ||
					 pShape->GetType() == FO_COMP_FREECLOSELINE ||
					 pShape->GetType() == FO_COMP_ELLIPSE) );
}

void CImageView::OnImagePaste() 
{
	// TODO: Add your command handler code here
	if (!OpenClipboard())  return;
	HDIB hDIB = (HDIB) CopyHandle(::GetClipboardData(CF_DIB));
	CloseClipboard();
	if (hDIB == NULL)  return;
	LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	int cxdib = ::DIBWidth(lpDIBHdr);
	int cydib = ::DIBHeight(lpDIBHdr);
	::GlobalUnlock((HGLOBAL) hDIB);

    COleDateTime dtime;
    dtime = COleDateTime::GetCurrentTime();
	CString tmpimg = dtime.Format("%Y_%B_%d_%H_%M_%S.bmp");
	remove(tmpimg);
	CFile file;
	CFileException fe;
	
	if (!file.Open(tmpimg, CFile::modeCreate |
		CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		::GlobalFree((HGLOBAL) hDIB);
		hDIB = NULL;
		return;
	}
	
	TRY
	{
		::SaveDIB( hDIB, file );
		file.Close();
	}
	CATCH (CException, eSave)
	{
		file.Abort(); // will not throw an exception
		return;
	}
	END_CATCH
	::GlobalFree((HGLOBAL) hDIB);
    hDIB = NULL;

	CRect rcScrollPos;
	rcScrollPos.SetRectEmpty();
	CFODrawShapeList listCopy;

	CRect rc = CRect(0,0,cxdib,cydib);
	CFOImageShape *pReturn = new CFOImageShape;
	pReturn->AddRef();
	pReturn->Create(rc,_T(""));
	pReturn->LoadImage(tmpimg);

	pReturn->SetObjectCaption(GetCurrentModel()->GetUniqueCaption(pReturn->GetType()));
	pReturn->SetObjectName(GetCurrentModel()->GetUniqueName(pReturn->GetType()));
	listCopy.AddTail(pReturn);
	pReturn->Release();
	pReturn = NULL;
	remove(tmpimg);

	if(listCopy.GetCount() >0)
	{
		BOOL bFirst = TRUE;
		CRect rcTop = CRect(200000,200000,-200000,-200000);
		CString strName;
		CFODrawShape *pShape = NULL;
		POSITION pos = listCopy.GetHeadPosition();
		while(pos != NULL)
		{
			pShape = (CFODrawShape *)listCopy.GetNext(pos);
			if(pShape->IsKindOf(RUNTIME_CLASS(CFODrawPortsShape)))
			{
				CFODrawPortsShape *pShapeTemp = (CFODrawPortsShape *)pShape;
				pShapeTemp->RemoveAllLinks();
			}
			
			CRect rcTemp = pShape->GetRect();
			if(bFirst)
			{
				rcScrollPos = rcTemp;
				rcScrollPos.OffsetRect(m_ptPasteOffset);
				bFirst = FALSE;
			}

			if(pShape->IsOver4ResizeHandles())
			{
				pShape->OffsetAllPoints(m_ptPasteOffset);
				rcTemp.OffsetRect(m_ptPasteOffset);
			}
			else
			{
				rcTemp.OffsetRect(m_ptPasteOffset);
				pShape->PositionShape(rcTemp);
			}

			if(rcTemp.TopLeft().x <rcTop.TopLeft().x)
			{
				rcTop.left = rcTemp.TopLeft().x;
			}

			if(rcTemp.TopLeft().y <rcTop.TopLeft().y)
			{
				rcTop.top = rcTemp.TopLeft().y;
			}

			if(rcTemp.BottomRight().x > rcTop.BottomRight().x)
			{
				rcTop.right = rcTemp.BottomRight().x;
			}

			if(rcTemp.BottomRight().y > rcTop.BottomRight().y)
			{
				rcTop.bottom = rcTemp.BottomRight().y;
			}

			pShape->UpdateComp();
			strName = _T("");
			strName = GetCurrentModel()->GetUniqueName(pShape->GetType());
			pShape->SetObjectName(strName);
		}
		
		GetCurrentModel()->CheckFullLink(listCopy);
		GetCurrentModel()->DoChangeLinkPortName(listCopy);

		CFOAddCompsAction *pAction = new CFOAddCompsAction(GetCurrentModel(),listCopy);
		GetCurrentModel()->Do(pAction,TRUE);
		
		CRect rcClient;
		GetClientRect(&rcClient);
		int nOffsetX = (int)(((double)rcClient.Width()/m_dScale - (double)rcTop.Width())/2);
		int nOffsetY = (int)(((double)rcClient.Height()/m_dScale - (double)rcTop.Height())/2);

		CPoint ptStart = rcTop.TopLeft() - CPoint(nOffsetX,nOffsetY);
		ptStart = CPoint((int)(ptStart.x * m_dScale),(int)(ptStart.y* m_dScale));

		AdjustScrollPoint(ptStart);
		SetNewScrollBarPos(ptStart);
		CPoint PasteOffset = CSize(12,12);
		m_ptPasteOffset +=	PasteOffset;
	}	
}

void CImageView::OnUpdateImagePaste(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( m_hDIB != NULL && ::IsClipboardFormatAvailable(CF_DIB));
}

void CImageView::PerformOnUpdateImageCut(CCmdUI* pCmdUI)
{
	OnUpdateImageCut(pCmdUI);
}

void CImageView::PerformOnUpdateImageCopy(CCmdUI* pCmdUI)
{
	OnUpdateImageCopy(pCmdUI);
}

void CImageView::PerformOnUpdateImagePaste(CCmdUI* pCmdUI)
{
	OnUpdateImagePaste(pCmdUI);
}

void CImageView::OnImageSave() 
{
	// TODO: Add your command handler code here
	CString tmpimg = theApp.m_SysTmpPath;
	tmpimg += "00000001.bmp";
	remove(tmpimg);
	CFile file;
	CFileException fe;
	
	if (!file.Open(tmpimg, CFile::modeCreate |
		CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		return;
	}

	BeginWaitCursor();
	
	TRY
	{
		::SaveDIB( m_hDIB, file );
		file.Close();
	}
	CATCH (CException, eSave)
	{
		file.Abort(); // will not throw an exception
		EndWaitCursor();
		return;
	}
	END_CATCH

	CString transname;
	transname.Format("%s_cmipstrans.dat", theApp.GetView()->GetDocument()->m_blh);
	CString fstr = theApp.m_SysTmpPath;
	fstr += transname;

	GDALDataset * m_pSrcDataset = (GDALDataset *) GDALOpen ( tmpimg, GA_ReadOnly );
	GDALDataset * m_pDstDataset = GS_CreateCopy( theApp.m_SavingMode, fstr, m_pSrcDataset);
	GDALClose(m_pSrcDataset);
	GDALClose(m_pDstDataset);

	CWnd *pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CMedImage));
	if(pWnd != NULL)  ((CMedImage *)pWnd)->SaveModifiedImg(fstr);

	remove(fstr);
	remove(tmpimg);

	EndWaitCursor();
}

void CImageView::OnUpdateImageSave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( m_hDIB != NULL );
}

void CImageView::PerformOnUpdateImageSave(CCmdUI* pCmdUI)
{
	OnUpdateImageSave(pCmdUI);
}

void CImageView::OnImageRectify() 
{
	// TODO: Add your command handler code here
	CEqual mydlg(this);
	if(mydlg.DoModal() == IDOK)
	{
		RedrawWindow();

		if(AfxMessageBox("是否保存修改后图像？",MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
			OnImageSave();
		}
	}
	else
	{
		RedrawWindow();
	}
}

void CImageView::OnUpdateImageRectify(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( m_hDIB != NULL );
}

void CImageView::PerformOnUpdateImageRectify(CCmdUI* pCmdUI)
{
	OnUpdateImageRectify(pCmdUI);
}

void CImageView::OnLinetypeSelect() 
{
	// TODO: Add your command handler code here
	CIniReader m_IniReader;
	m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");

	CDlgPropertyLine dlg;

	CString LineColor = m_IniReader.getKeyValue("LineColor","线型设置");
	if(LineColor.IsEmpty())
	{
		dlg.m_crLineColor = ((CExtDataModel *)GetCurrentModel())->qLineColor;
	}
	else
	{
		int r = atoi(LineColor.Left(LineColor.Find(',')));
		LineColor = LineColor.Right(LineColor.GetLength() - LineColor.Find(',') - 1);
		int g = atoi(LineColor.Left(LineColor.Find(',')));
		LineColor = LineColor.Right(LineColor.GetLength() - LineColor.Find(',') - 1);
		int b = atoi(LineColor);

		dlg.m_crLineColor = RGB(r,g,b);
	}

	CString LineWidth = m_IniReader.getKeyValue("LineWidth","线型设置");
	if(LineWidth.IsEmpty())
	{
		dlg.m_nLineWidth = ((CExtDataModel *)GetCurrentModel())->qLineWidth;
	}
	else
	{
		dlg.m_nLineWidth = atoi(LineWidth);
	}

	CString LineType = m_IniReader.getKeyValue("LineType","线型设置");
	if(LineType.IsEmpty())
	{
		dlg.m_nLineType = ((CExtDataModel *)GetCurrentModel())->qLineType;
	}
	else
	{
		dlg.m_nLineType = atoi(LineType);
	}

	if(IDOK == dlg.DoModal())
	{
		CString str;
		str.Format("%d,%d,%d", GetRValue(dlg.m_crLineColor), GetGValue(dlg.m_crLineColor), GetBValue(dlg.m_crLineColor));
		m_IniReader.setKey(str,"LineColor","线型设置");

		str.Format("%d", dlg.m_nLineWidth);
		m_IniReader.setKey(str,"LineWidth","线型设置");

		str.Format("%d", dlg.m_nLineType);
		m_IniReader.setKey(str,"LineType","线型设置");

		((CExtDataModel *)GetCurrentModel())->qLineColor = dlg.m_crLineColor;
		((CExtDataModel *)GetCurrentModel())->qLineWidth = dlg.m_nLineWidth;
		((CExtDataModel *)GetCurrentModel())->qLineType = dlg.m_nLineType;
	
		CFODrawShape *pOb;
		POSITION pos = GetCurrentModel()->GetFormObjects()->GetHeadPosition();
		while (pos != NULL) 
		{
			pOb = (CFODrawShape*)(GetCurrentModel()->GetFormObjects()->GetNext(pos));
			if( pOb->GetType() == FO_COMP_COMPOSITE || 
				pOb->GetType() == FO_COMP_STATIC ||
				pOb->GetType() == FO_COMP_HYLINK ||
				pOb->GetType() == FO_COMP_IMAGE )
			{
				continue;
			}
			pOb->SetLineColor(dlg.m_crLineColor);
			pOb->SetLineWidth(dlg.m_nLineWidth);
			pOb->SetPenStyle(dlg.m_nLineType);

			//------------------------------------
			if(pOb->GetType() == FO_COMP_ARROWLINE)
			{
				int nType = 3;
				
				CFOCompProperties *prop = (CFOCompProperties *)pOb->FindProperty(FO_DEFAULT_PROP_ID);
				CFOCompProperties *propNew = (CFOCompProperties *)prop->Copy();
				propNew->SetLineColor(dlg.m_crLineColor);
				propNew->SetLineWidth(dlg.m_nLineWidth);
				propNew->SetPenStyle(dlg.m_nLineType);
				propNew->SetBkColor(dlg.m_crLineColor);
				propNew->SetEndArrowType(nType);
				propNew->SetType(FO_COMP_ARROWLINE);
				
				CFOCompsPropAction *pAction = new CFOCompsPropAction(GetCurrentModel(),pOb);
				pAction->SetCurrentProp(propNew);
				GetCurrentModel()->Do(pAction);
				if(propNew != NULL)
				{
					delete propNew;
					propNew = NULL;
				}
			}
			//------------------------------------
		}
		RedrawWindow();
	}
}

void CImageView::OnTexttypeSelect() 
{
	// TODO: Add your command handler code here
	CFODrawShape *pObj = NULL;
	POSITION pos = m_listSelectComp.GetHeadPosition();
	if(pos != NULL)	pObj = (CFODrawShape *)m_listSelectComp.GetNext(pos);
	if( pObj == NULL || pObj->GetType() != FO_COMP_STATIC)
	{
		return;
	}

	CIniReader m_IniReader;
	m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");

	CDlgPropertyText dlg;
	FO_FONT Font;

	CString FontType = m_IniReader.getKeyValue("FontType","文字标注格式设置");
	if(FontType.IsEmpty())
	{
		Font.m_strFaceName = pObj->GetFaceName();
		Font.m_crColor     = pObj->GetFontColor();
		Font.m_nPointSize  = pObj->GetPointSize();
		Font.m_nHeight     = pObj->GetHeight();
		Font.m_bItalic     = pObj->GetItalic();
		Font.m_bUnderline  = pObj->GetUnderline();	
		Font.m_nWeight     = pObj->GetWeight();

		dlg.m_bMultiLine   = pObj->IsMultiLine();
		dlg.m_bHorzAlign   = pObj->GetTextHorzAlignment();
		dlg.m_bVertAlign   = pObj->GetTextVertAlignment();
	}
	else
	{
		Font.m_strFaceName = FontType.Left(FontType.Find(','));
		FontType = FontType.Right(FontType.GetLength() - FontType.Find(',') - 1);

		int r = atoi(FontType.Left(FontType.Find(',')));
		FontType = FontType.Right(FontType.GetLength() - FontType.Find(',') - 1);
		int g = atoi(FontType.Left(FontType.Find(',')));
		FontType = FontType.Right(FontType.GetLength() - FontType.Find(',') - 1);
		int b = atoi(FontType.Left(FontType.Find(',')));
		FontType = FontType.Right(FontType.GetLength() - FontType.Find(',') - 1);
		Font.m_crColor = RGB(r,g,b);

		Font.m_nPointSize = atoi(FontType.Left(FontType.Find(',')));
		FontType = FontType.Right(FontType.GetLength() - FontType.Find(',') - 1);

		Font.m_nHeight = atoi(FontType.Left(FontType.Find(',')));
		FontType = FontType.Right(FontType.GetLength() - FontType.Find(',') - 1);

		Font.m_bItalic = atoi(FontType.Left(FontType.Find(',')));
		FontType = FontType.Right(FontType.GetLength() - FontType.Find(',') - 1);

		Font.m_bUnderline = atoi(FontType.Left(FontType.Find(',')));
		FontType = FontType.Right(FontType.GetLength() - FontType.Find(',') - 1);

		Font.m_nWeight = atoi(FontType.Left(FontType.Find(',')));
		FontType = FontType.Right(FontType.GetLength() - FontType.Find(',') - 1);

		dlg.m_bMultiLine = atoi(FontType.Left(FontType.Find(',')));
		FontType = FontType.Right(FontType.GetLength() - FontType.Find(',') - 1);
		dlg.m_bHorzAlign = atoi(FontType.Left(FontType.Find(',')));
		FontType = FontType.Right(FontType.GetLength() - FontType.Find(',') - 1);
		dlg.m_bVertAlign = atoi(FontType);
	}
	
	
	dlg.SetFontInfo(&Font);

	dlg.m_strContent   = pObj->GetObjectCaption();
	
	if(IDOK == dlg.DoModal())
	{
		dlg.GetFontInfo(&Font);
		
		pObj->SetFaceName(Font.m_strFaceName);
		pObj->SetPointSize(Font.m_nPointSize);
		pObj->SetFontColor(Font.m_crColor);
		pObj->SetItalic(Font.m_bItalic);
		pObj->SetUnderline(Font.m_bUnderline);
		pObj->SetWeight(Font.m_nWeight);
		
		pObj->SetMultiLine(dlg.m_bMultiLine);
		pObj->SetTextHorzAlignment(dlg.m_bHorzAlign);
		pObj->SetTextVertAlignment(dlg.m_bVertAlign);
		
		pObj->SetObjectCaption(dlg.m_strContent);
//		pObj->SetObjectCaption("请输入注记!");
		
		((CFOStaticShape *)pObj)->AdjustTextBoxSize(NULL);
		RedrawWindow();

		//-------------------------------------------
		((CExtDataModel *)GetCurrentModel())->StaticFont = Font;
		((CExtDataModel *)GetCurrentModel())->m_Static_bMultiLine = dlg.m_bMultiLine;
		((CExtDataModel *)GetCurrentModel())->m_Static_bHorzAlign = dlg.m_bHorzAlign;
        ((CExtDataModel *)GetCurrentModel())->m_Static_bVertAlign = dlg.m_bVertAlign;
		//-------------------------------------------

		CString str;
		str.Format("%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", Font.m_strFaceName,GetRValue(Font.m_crColor),GetGValue(Font.m_crColor),GetBValue(Font.m_crColor),Font.m_nPointSize,Font.m_nHeight,Font.m_bItalic,Font.m_bUnderline,Font.m_nWeight,dlg.m_bMultiLine,dlg.m_bHorzAlign,dlg.m_bVertAlign);
		m_IniReader.setKey(str,"FontType","文字标注格式设置");
	}
}

CFORemoveCompsAction* CImageView::DoDeleteAction(CFODrawShapeList* pShapeList)
{
	CFODrawView::DoDeleteAction(pShapeList);

	CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	if( pAppFrame->m_wndOutputBar.IsWindowVisible() )
	{
		int n_count = GetCurrentModel()->GetFormObjects()->GetCount();
		pAppFrame->m_wndOutputBar.m_MeasureDialog.m_MeasureCtrl.SetNumberRows(n_count);
		pAppFrame->m_wndOutputBar.m_MeasureDialog.m_MeasureCtrl.RedrawAll();
	}
	
	return NULL;
}

void CImageView::OnEditRedo() 
{
	// TODO: Add your command handler code here
	GetCurrentModel()->Redo();

	//=========================================================================
	CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	if( pAppFrame->m_wndOutputBar.IsWindowVisible() )
	{
		int n_count = GetCurrentModel()->GetFormObjects()->GetCount();
		pAppFrame->m_wndOutputBar.m_MeasureDialog.m_MeasureCtrl.SetNumberRows(n_count);
		pAppFrame->m_wndOutputBar.m_MeasureDialog.m_MeasureCtrl.RedrawAll();
	}
	//=========================================================================
}

void CImageView::OnEditUndo() 
{
	// TODO: Add your command handler code here
	if (m_pCurrentEditComp != NULL && (m_pCurrentEditComp->GetEditWnd() != NULL))
	{
		CEdit* pEdit = m_pCurrentEditComp->GetEditWnd();
		
		ASSERT(pEdit != NULL);
		
		if(pEdit->CanUndo())
			pEdit->Undo();
		
		return;
	}
	
	GetCurrentModel()->Undo();
	
	if(m_pCurrentEditComp)
	{
		OnTextEditFinish(0, CPoint(0,0));
	}

	//=========================================================================
	CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	if( pAppFrame->m_wndOutputBar.IsWindowVisible() )
	{
		int n_count = GetCurrentModel()->GetFormObjects()->GetCount();
		pAppFrame->m_wndOutputBar.m_MeasureDialog.m_MeasureCtrl.SetNumberRows(n_count);
		pAppFrame->m_wndOutputBar.m_MeasureDialog.m_MeasureCtrl.RedrawAll();
	}
	//=========================================================================
}

//Create popup menu when right click mouse button
CMenu* CImageView::CreateContextMenu()
{
	if (m_pContextMenu)
	{
		delete m_pContextMenu;
	}
	
	CMenu* pMenu;
	m_pContextMenu = new CMenu();
	m_pContextMenu->LoadMenu(IDR_MENU_IMAGEPROCESSING);
	
	pMenu = m_pContextMenu->GetSubMenu(0);
	
	ASSERT_VALID(pMenu);
	return pMenu;
}

void CImageView::OnFilePrint() 
{
	theApp.GetView()->PostMessage(WM_COMMAND, ID_FILE_PRINT);
}

void CImageView::OnFilePrintPreview() 
{
	theApp.GetView()->PostMessage(WM_COMMAND, ID_FILE_PRINT_PREVIEW);
}
