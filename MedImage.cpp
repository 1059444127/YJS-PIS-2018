// CMedImage : implementation file
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "Pathology.h"
#include "Pathologyview.h"
#include "Pathologydoc.h"
#include "MedImage.h"
#include "MainFrm.h"
#include "Figure.h"
#include "CanvasImage.h"
#include "KSFileDialog.h"
#include "DlgRotation.h"
#include "DlgReflection.h"
#include "Resize.h"
#include "Picnote.h"

#include "GetStudyUID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// bicubic interpolation macro define
#define CUBE(x) ((x)*(x)*(x))
#define P(x) ((x) > 0.0f ? (x) : 0.0f)
#define R(x) ((CUBE(P((x)+2)) - 4.0f * CUBE(P((x)+1)) + 6.0f * CUBE(P(x)) - 4.0f * CUBE(P((x)-1))) / 6.0f)

CRect SizeRectWithConstantAspectRatio(CRect* rcScreen,  CSize  sizePicture,  BOOL   bCenter)
{
	CRect rect(rcScreen);
	double dWidth = rcScreen->Width();
	double dHeight = rcScreen->Height();
	double dAspectRatio = dWidth/dHeight;
	
	double dPictureWidth = sizePicture.cx;
	double dPictureHeight = sizePicture.cy;
	double dPictureAspectRatio = dPictureWidth/dPictureHeight;
	
	//If the aspect ratios are the same then the screen rectangle
	// will do, otherwise we need to calculate the new rectangle
	
	if (dPictureAspectRatio > dAspectRatio)
	{
		int nNewHeight = (int)(dWidth/dPictureWidth*dPictureHeight);
		int nCenteringFactor = 0;
		
		if (bCenter)
			nCenteringFactor = (rcScreen->Height() - nNewHeight) / 2;
		
		rect.SetRect( 0, nCenteringFactor, (int)dWidth,	nNewHeight + nCenteringFactor);
		
	}
	else if (dPictureAspectRatio < dAspectRatio)
	{
		int nNewWidth =  (int)(dHeight/dPictureHeight*dPictureWidth);
		int nCenteringFactor = 0;
		
		if (bCenter)
			nCenteringFactor = (rcScreen->Width() - nNewWidth) / 2;
		
		rect.SetRect( nCenteringFactor, 0, nNewWidth + nCenteringFactor, (int)(dHeight));
	}
	
	return rect;
}

/////////////////////////////////////////////////////////////////////////////
// CMedImage
extern CPathologyApp theApp;

CMedImage::CMedImage()
	: CXTResizeFormView(CMedImage::IDD)
{
	//{{AFX_DATA_INIT(CMedImage)
	//}}AFX_DATA_INIT
	index = 0;
	
	m_bIsFirstInit = true;
}

CMedImage::~CMedImage()
{
	// TODO: add destruction code here.
	m_quickInfo.RemoveAll();
}

IMPLEMENT_DYNCREATE(CMedImage, CXTResizeFormView)

BEGIN_MESSAGE_MAP(CMedImage, CXTResizeFormView)
	//{{AFX_MSG_MAP(CMedImage)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_THUMBNAIL, OnBegindragListThumbnail)
	ON_NOTIFY(NM_CLICK, IDC_LIST_THUMBNAIL, OnClickListThumbnail)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_PHOTO, OnButtonPhoto)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_THUMBNAIL, OnDblclkListThumbnail)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_THUMBNAIL, OnRclickListThumbnail)
	ON_COMMAND(ID_PIC_ROTATE, OnPicRotate)
	ON_COMMAND(ID_PIC_MIRROR, OnPicMirror)
	ON_COMMAND(ID_PIC_FUSION, OnPicFusion)
	ON_COMMAND(ID_PIC_RESAMPLE, OnPicResample)
	ON_COMMAND(ID_PIC_NOTE, OnPicNote)
	ON_COMMAND(ID_PIC_OUTPUT, OnPicOutput)
	ON_COMMAND(ID_PIC_EXCHANGE, OnPicExchange)
	ON_COMMAND(ID_PIC_PRINT, OnPicPrint)
	ON_COMMAND(ID_PIC_UNPRINT, OnPicUnprint)
	ON_COMMAND(ID_PIC_SAVEAS, OnPicSaveas)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_THUMBNAIL, OnKeydownListThumbnail)
	ON_BN_CLICKED(IDC_BUTTON_PRINTER, OnButtonPrinter)
	ON_COMMAND(ID_FULLSCREEN, OnFullscreen)
	ON_COMMAND(ID_NOTE_QCMX, OnNoteQcmx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMedImage message handlers

void CMedImage::DoDataExchange(CDataExchange* pDX) 
{
	CXTResizeFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMedImage)
	DDX_Control(pDX, IDC_BUTTON_PRINTER, m_btPrinter);
	DDX_Control(pDX, IDC_BUTTON_PHOTO, m_btPhoto);
	//}}AFX_DATA_MAP
}

void CMedImage::OnBegindragListThumbnail(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	m_pDragImage = new CImageList;
	if( m_pListThumbnail->GetSelectedCount() > 1 )
	{
		m_pDragImage->Create(IDB_MULTI_DOC, 32, 1, RGB(0,128,128));
	}
	else
	{
		m_pDragImage->Create(IDB_SINGLE_DOC, 32, 1, RGB(0,128,128));
	}

	m_pDragImage->BeginDrag(0, CPoint(4, 0));
	m_pDragImage->DragEnter(GetDesktopWindow(), pNMListView->ptAction);
	
	m_bDragging = TRUE;
	SetCapture ();

	*pResult = 0;
}

void CMedImage::OnInitialUpdate() 
{
	CXTResizeFormView::OnInitialUpdate();
	
	if (m_bIsFirstInit)
	{
		m_bIsFirstInit = false;

		SetResize(IDC_LIST_THUMBNAIL, SZ_TOP_LEFT,  SZ_BOTTOM_RIGHT);
		SetResize(IDC_RECYCLE, SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
		SetResize(IDC_BUTTON_PHOTO, SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
		SetResize(IDC_BUTTON_PRINTER, SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
		
		m_ImageListThumb.Create(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, ILC_COLOR24, 0, 1);
		((CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL))->SetImageList(&m_ImageListThumb, LVSIL_NORMAL);
		((CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL))->SetIconSpacing(110,100); // 2006.08.05 设置影像缩略栏图像纵/横排列间距
		
		m_bDragging = FALSE;
		m_pDragImage = NULL;

		const DWORD STYLE_ALL = BS_LEFT | BS_RIGHT | BS_CENTER;
		m_btPhoto.SetBitmap(CSize(32,32),IDB_BITMAP_PHOTO);
		m_btPhoto.SetXButtonStyle(BS_XT_XPFLAT); 
		m_btPhoto.ModifyStyle(STYLE_ALL & ~BS_CENTER, BS_CENTER);

		m_btPrinter.SetBitmap(CSize(32,32),IDB_BITMAP_PRINTER);
		m_btPrinter.SetXButtonStyle(BS_XT_XPFLAT); 
		m_btPrinter.ModifyStyle(STYLE_ALL & ~BS_CENTER, BS_CENTER);

		m_ToolTip.Create(this);
		m_ToolTip.Activate(TRUE);
		m_ToolTip.AddTool(&m_btPhoto,   "导入图像");
		
		m_PrinterSelMode = false;
		if(m_PrinterSelMode)
		{
			m_btPrinter.SetBitmap(CSize(32,32),IDB_BITMAP_PRINTER);
			m_ToolTip.AddTool(&m_btPrinter, "设置打印");
		}
		else
		{
			m_btPrinter.SetBitmap(CSize(32,32),IDB_BITMAP_VIDEO);
			m_ToolTip.AddTool(&m_btPrinter, "图像浏览");
		}
	}	
}

void CMedImage::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bDragging)
	{
		// Release mouse capture, so that other controls can get control/messages
		ReleaseCapture ();

		// Note that we are NOT in a drag operation
		m_bDragging = FALSE;

		// End dragging image
		m_pDragImage->DragLeave (GetDesktopWindow ());
		m_pDragImage->EndDrag ();

		delete m_pDragImage; //must delete it because it was created at the beginning of the drag
	
		CPoint pt(point);	//get our current mouse coordinates
		ClientToScreen(&pt); //convert to screen coordinates
		CRect Recycle_Area;
		GetDlgItem(IDC_RECYCLE)->GetWindowRect(&Recycle_Area);
		if(Recycle_Area.PtInRect(pt))
		{
			DeleteListItem();
		}
	}
	
	CXTResizeFormView::OnLButtonUp(nFlags, point);
}

void CMedImage::LoadNewThumb(HDIB hDIB, int SerialNo)
{
	if(m_ImageListThumb.GetSafeHandle() == 0)  return;
	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
	m_pListThumbnail->SetRedraw(FALSE);   // hold the window update to avoid flicking

	CString ExtInfo;
	LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	BYTE *   lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
	int cxDIB = ::DIBWidth(lpDIBHdr);
	int cyDIB = ::DIBHeight(lpDIBHdr);

	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)lpDIBHdr;
	LPBITMAPCOREHEADER lpbmc = (LPBITMAPCOREHEADER)lpDIBHdr;
	
	WORD m_BitCount;
	if (IS_WIN30_DIB(lpDIBHdr))
		m_BitCount = lpbmi->biBitCount;
	else
		m_BitCount = lpbmc->bcBitCount;

	ExtInfo.Format("(%d:%d:%d)",cxDIB,cyDIB, m_BitCount);
		
	::GlobalUnlock((HGLOBAL) hDIB);
	
	CCanvasImage* m_pAtlImage = new CCanvasImage();
	
	m_pAtlImage->Create(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT,24);
	HDC hDC = m_pAtlImage->GetDC();

	CRect ScreenWnd;
	ScreenWnd.SetRect( 0 , 0 ,  THUMBNAIL_WIDTH  , THUMBNAIL_HEIGHT );
	//-----------------------------------------
	CDC* pDstDC = CDC::FromHandle(hDC);
	CPen   * n_old_pen   = (CPen   *)pDstDC->SelectStockObject(WHITE_PEN);
	CBrush * n_old_brush = (CBrush *)pDstDC->SelectStockObject(WHITE_BRUSH);
	pDstDC->Rectangle(&ScreenWnd);
	pDstDC->SelectObject(n_old_pen);
	pDstDC->SelectObject(n_old_brush);
	//-----------------------------------------
	ScreenWnd = SizeRectWithConstantAspectRatio(&ScreenWnd,  CSize(cxDIB, cyDIB),  TRUE);

	::PaintDIB(hDC,	ScreenWnd, hDIB, CRect(0, 0, cxDIB, cyDIB), NULL);
	
	// ---------- add file type tag ----------------
	if(theApp.m_SavingMode.CompareNoCase("BMP") == 0 || theApp.m_SavingMode.CompareNoCase("JPEG") == 0 || theApp.m_SavingMode.CompareNoCase("GTiff") == 0) 
	{
		HDC	hMemDC = NULL;	
		CBitmap m_bitmap;
		if(theApp.m_SavingMode.CompareNoCase("BMP") == 0)   m_bitmap.LoadBitmap(IDB_FILETYPE_BMP);
		if(theApp.m_SavingMode.CompareNoCase("JPEG") == 0)  m_bitmap.LoadBitmap(IDB_FILETYPE_JPG);
		if(theApp.m_SavingMode.CompareNoCase("GTiff") == 0) m_bitmap.LoadBitmap(IDB_FILETYPE_TIF);
		hMemDC = ::CreateCompatibleDC(NULL);
		HGDIOBJ	hOldObj = ::SelectObject(hMemDC, HBITMAP(m_bitmap) );
		
		CDC* pSrcDC = CDC::FromHandle(hMemDC);
		
		pDstDC->StretchBlt(THUMBNAIL_WIDTH - 16, 0, 16, 16, pSrcDC, 0, 0, 16,16, SRCCOPY);
		
		::SelectObject(hMemDC, hOldObj);
		::DeleteObject(hMemDC);
		m_bitmap.DeleteObject();
	}
	
	m_pAtlImage->ReleaseDC();
	
	// attach the thumbnail bitmap handle to an CBitmap object
	CBitmap * pImage = new CBitmap();		 
	pImage->Attach( m_pAtlImage->m_hBitmap );
	
	// add bitmap to our image list
	int ImgItemIndex = m_ImageListThumb.Add(pImage, (CBitmap*)NULL);
	
	CString DispStr;
	DispStr.Format("%d", SerialNo);
	DispStr += ExtInfo;
	
	m_pListThumbnail->InsertItem( m_pListThumbnail->GetItemCount(), DispStr, ImgItemIndex );
	
	SAFE_DELETE(pImage);
	SAFE_DELETE(m_pAtlImage);
	
	m_pListThumbnail->SetRedraw(TRUE);  // let's show the new thumbnails

	//m_pListThumbnail->EnsureVisible(NewID,FALSE);
	RefreshTabText();
}

void CMedImage::LoadThumbnail()
{
	if(m_ImageListThumb.GetSafeHandle() == 0)  return;
	
	theApp.ConnectDatabase();
	//----------------------------------------------------
	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
	// hold the window update to avoid flicking
	m_pListThumbnail->SetRedraw(FALSE);
	
	// reset our image list
	for(int i=0; i< m_ImageListThumb.GetImageCount(); i++)
		m_ImageListThumb.Remove(i);	

	// remove all items from list view
	if( m_pListThumbnail->GetItemCount() != 0 )
		m_pListThumbnail->DeleteAllItems();
	
	// set the size of the image list
	int ImageCount = 0;
	if(!theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		try
		{
			SAString cmdstr = "Select count(*) from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			
			g_dbcommand.Execute();
			g_dbcommand.FetchNext();
			ImageCount = g_dbcommand.Field(1).asLong();
			
			g_dbconnection.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
			return;
		}
	}

	m_ImageListThumb.SetImageCount(ImageCount);

	if(ImageCount > 0)
	{
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno order by serialno asc";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			
			g_dbcommand.Execute();
			int index = 0;
			while(g_dbcommand.FetchNext())
			{
				SAString Bstr = g_dbcommand.Field("imgdataset").asBLob();
				if( !Bstr.IsEmpty() )
				{
					const void * binbuf = (const void *)Bstr;
					BYTE * dataptr = (BYTE *)binbuf;

					CString transname;
					transname.Format("%s_cmipstrans.dat", theApp.GetView()->GetDocument()->m_blh);

					CFile file;
					CFileException fe;
					
					CString fstr = theApp.m_SysTmpPath;
					fstr += transname;
					remove(fstr);
					if (file.Open( fstr , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
					{
						file.WriteHuge( dataptr , Bstr.GetLength());
						file.Close();
					}

					HDIB hDIB = ::CreateDIBFile( THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, 24 );
					LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
					BYTE *   lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
					int LineByteWidth = WIDTHBYTES( THUMBNAIL_WIDTH * 24 );
					memset( lpDIBBits, 255, LineByteWidth * THUMBNAIL_HEIGHT );
					
					CString ExtInfo;
					
					GDALDataset * m_pDataset = (GDALDataset *) GDALOpen ( fstr , GA_ReadOnly );
					CString m_file_type;
					m_file_type.Empty();
					
					if( m_pDataset != NULL )
					{
						ExtInfo.Format("(%d:%d:%d)",m_pDataset->GetRasterXSize(),m_pDataset->GetRasterYSize(), 8 * m_pDataset->GetRasterCount());
						m_file_type = GDALGetDriverShortName( m_pDataset->GetDriver() );
						
						BYTE *pRBits = lpDIBBits + 2;
						BYTE *pGBits = lpDIBBits + 1;
						BYTE *pBBits = lpDIBBits + 0;
						
						int BandNo[3];
						int BandNum = m_pDataset->GetRasterCount();
						if( BandNum == 1)
						{
							BandNo[0] = 1;	BandNo[1] = 1;	BandNo[2] = 1;
						}
						else
						{
							BandNo[0] = 1;	BandNo[1] = 2;	BandNo[2] = 3;
						}
						
						CRect ScreenWnd;
						ScreenWnd.SetRect( 0 , 0 ,  THUMBNAIL_WIDTH  , THUMBNAIL_HEIGHT );
						CSize sizePicture;
						sizePicture.cx = m_pDataset->GetRasterXSize();
						sizePicture.cy = m_pDataset->GetRasterYSize();
						ScreenWnd = SizeRectWithConstantAspectRatio(&ScreenWnd,  sizePicture,  TRUE);
						
						m_pDataset->GetRasterBand(BandNo[0])->RasterIO( GF_Read, 0, 0, 
							                                            m_pDataset->GetRasterXSize(), 
							                                            m_pDataset->GetRasterYSize(), 
							                                            (void *)(pRBits + ScreenWnd.top * LineByteWidth + ScreenWnd.left * 3), 
							                                            ScreenWnd.Width() , ScreenWnd.Height() , 
							                                            GDT_Byte, 
							                                            3 , LineByteWidth );
						m_pDataset->GetRasterBand(BandNo[1])->RasterIO( GF_Read, 0, 0, 
							                                            m_pDataset->GetRasterXSize(), 
							                                            m_pDataset->GetRasterYSize(), 
							                                            (void *)(pGBits + ScreenWnd.top * LineByteWidth + ScreenWnd.left * 3), 
							                                            ScreenWnd.Width() , ScreenWnd.Height() , 
							                                            GDT_Byte, 
							                                            3 , LineByteWidth );
						m_pDataset->GetRasterBand(BandNo[2])->RasterIO( GF_Read, 0, 0, 
							                                            m_pDataset->GetRasterXSize(), 
							                                            m_pDataset->GetRasterYSize(), 
							                                            (void *)(pBBits + ScreenWnd.top * LineByteWidth + ScreenWnd.left * 3), 
							                                            ScreenWnd.Width() , ScreenWnd.Height() , 
							                                            GDT_Byte, 
							                                            3 , LineByteWidth );
						GDALClose(m_pDataset);
					}
					
					BYTE *buf = new BYTE[LineByteWidth];
					BYTE *u1,*u2;
					for(int j = 0; j < (THUMBNAIL_HEIGHT/2); j++ )
					{
						u1 = lpDIBBits + j * LineByteWidth;
						u2 = lpDIBBits + (THUMBNAIL_HEIGHT - j - 1) * LineByteWidth;
						memcpy(buf, u1,  LineByteWidth);
						memcpy(u1,  u2,  LineByteWidth);
						memcpy(u2,  buf, LineByteWidth);
					}
					delete buf;
					
					::GlobalUnlock((HGLOBAL) hDIB);
					
					CCanvasImage* m_pAtlImage = new CCanvasImage();
					
					m_pAtlImage->Create(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT,24);
					HDC hDC = m_pAtlImage->GetDC();
					
					CRect rect;
					rect.SetRect(0,0,THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT);
					::PaintDIB(hDC,	rect, hDIB, rect, NULL);
					
					::GlobalFree((HGLOBAL) hDIB);
					
					// ---------- add file type tag ----------------
					if(m_file_type == "BMP" || m_file_type == "JPEG" || m_file_type == "GTiff") 
					{
						HDC	hMemDC = NULL;	
						CBitmap m_bitmap;
						if(m_file_type == "BMP")   m_bitmap.LoadBitmap(IDB_FILETYPE_BMP);
						if(m_file_type == "JPEG")  m_bitmap.LoadBitmap(IDB_FILETYPE_JPG);
						if(m_file_type == "GTiff") m_bitmap.LoadBitmap(IDB_FILETYPE_TIF);
						hMemDC = ::CreateCompatibleDC(NULL);
						HGDIOBJ	hOldObj = ::SelectObject(hMemDC, HBITMAP(m_bitmap) );
						
						CDC* pSrcDC = CDC::FromHandle(hMemDC);
						CDC* pDstDC = CDC::FromHandle(hDC);
						
						pDstDC->StretchBlt(THUMBNAIL_WIDTH - 16, 0, 16, 16, pSrcDC, 0, 0, 16,16, SRCCOPY);
						
						::SelectObject(hMemDC, hOldObj);
						::DeleteObject(hMemDC);
						m_bitmap.DeleteObject();
					}

					// ---------- add print tag --------------------
					if(g_dbcommand.Field("printflag").asBool() == true && !g_dbcommand.Field("printflag").isNull())
					{
						HDC	hMemDC = NULL;	
						CBitmap m_bitmap;
						m_bitmap.LoadBitmap(IDB_BITMAP_PRINT);
						hMemDC = ::CreateCompatibleDC(NULL);
						HGDIOBJ	hOldObj = ::SelectObject(hMemDC, HBITMAP(m_bitmap) );
						
						CDC* pSrcDC = CDC::FromHandle(hMemDC);
						CDC* pDstDC = CDC::FromHandle(hDC);
						
						pDstDC->StretchBlt(0, 0, 16, 16, pSrcDC, 0, 0, 16,16, SRCCOPY);
						
						::SelectObject(hMemDC, hOldObj);
						::DeleteObject(hMemDC);
						m_bitmap.DeleteObject();
					}
					// ---------------------------------------------
					
					m_pAtlImage->ReleaseDC();
					
					// attach the thumbnail bitmap handle to an CBitmap object
					CBitmap * pImage = new CBitmap();		 
					pImage->Attach( m_pAtlImage->m_hBitmap );
					
					// add bitmap to our image list
					m_ImageListThumb.Replace( index, pImage, NULL );
					
					// put item to display
					// set the image file name as item text
					int SerialNo = g_dbcommand.Field("serialno").asLong();
					CString DispStr;
					DispStr.Format("%d", SerialNo);
					DispStr += ExtInfo;
					
					m_pListThumbnail->InsertItem( index, DispStr, index );
					index++;
					
					delete pImage;
					
					if( m_pAtlImage )
					{
						delete m_pAtlImage;
						m_pAtlImage =NULL;
					}
					//*********************************************************
					
					remove(fstr);
				}
			}
			
			g_dbconnection.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
			return;
		}
	}

	// let's show the new thumbnails
	m_pListThumbnail->SetRedraw(TRUE); 

	//m_pListThumbnail->EnsureVisible(NewID,FALSE);
	RefreshTabText();
}

void CMedImage::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bDragging)
	{
		CPoint pt(point);	//get our current mouse coordinates
		ClientToScreen(&pt); //convert to screen coordinates
		m_pDragImage->DragMove(pt); //move the drag image to those coordinates
		// Unlock window updates (this allows the dragging image to be shown smoothly)
		m_pDragImage->DragShowNolock(false);

		//If we are hovering over a CListCtrl we need to adjust the highlights
		CRect Recycle_Area;
		GetDlgItem(IDC_RECYCLE)->GetWindowRect(&Recycle_Area);
		if(Recycle_Area.PtInRect(pt))
		{			
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		}
		else
		{
			SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_HANDON));
		}
		// Lock window updates
		m_pDragImage->DragShowNolock(true);
	}
	
	CXTResizeFormView::OnMouseMove(nFlags, point);
}


void CMedImage::LoadImageFromFile(CString strPathName)
{
	char instanceUID[100];

	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))  return;

	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  return;
	

	CString transname;
	transname.Format("%s_cmipstrans.dat", theApp.GetView()->GetDocument()->m_blh);
	CString fstr = theApp.m_SysTmpPath;
	fstr += transname;

	int maxno = 0;
	theApp.ConnectDatabase();
	try
	{
		SAString cmdstr = "Select serialno from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno order by serialno desc";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
		g_dbcommand.Execute();
		if(g_dbcommand.FetchNext())
		{
			maxno = g_dbcommand.Field("serialno").asLong();
		}
		
		g_dbconnection.Commit();
	}
	catch(SAException &x)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
		AfxMessageBox((const char*)x.ErrText());
		return;
	}

	BeginWaitCursor();

	remove(fstr);

	GDALDataset * m_pSrcDataset = (GDALDataset *) GDALOpen ( strPathName, GA_ReadOnly );
	GDALDataset * m_pDstDataset = GS_CreateCopy( theApp.m_SavingMode, fstr, m_pSrcDataset);
	GDALClose(m_pSrcDataset);
	GDALClose(m_pDstDataset);

	SAString  imgdataset;
	imgdataset.Empty();
	CFile file;
	CFileException fe;
	if (file.Open(fstr, CFile::modeRead | CFile::shareDenyNone, &fe))
	{
		char sBuf[32*1024];
		while( 1 )
		{
			if( file.GetPosition() >= file.GetLength() )  break;
				DWORD nRead = file.ReadHuge(sBuf, sizeof(sBuf) );
				imgdataset += SAString( sBuf, nRead );
		}
		file.Close();
	}

	dcmGenerateUniqueIdentifier(instanceUID, SITE_INSTANCE_UID_ROOT);

	try
	{
		SAString cmdstr = "Insert into " + theApp.TABLE_IMAGE + "(medicalno, instanceUID, serialno, imgdataset) values(:medicalno, :instanceUID, :serialno,:imgdataset)";
			
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Param("instanceUID").setAsString() = instanceUID;
		g_dbcommand.Param("serialno").setAsLong()    = ++maxno;
		g_dbcommand.Param("imgdataset").setAsBLob()  = imgdataset;
			
		g_dbcommand.Execute();
		g_dbconnection.Commit();
	}
	catch(SAException &x)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
		AfxMessageBox((const char*)x.ErrText());
	}

	imgdataset.Empty();

	remove(fstr);

	LoadThumbnail();
}


void CMedImage::OnButtonPhoto() 
{
	char instanceUID[100];

	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))  return;

	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  return;
	
	static char Filter[]="位图文件 (*.bmp)|*.bmp|JPEG文件格式 (*.jpg)|*.jpg|TIFF文件格式 (*.tif)|*.tif|所有图像文件|*.bmp; *.jpg; *.tif||";

	CKSFileDialog MyDlg( TRUE,NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER ,	Filter , this );
	MyDlg.m_ofn.lpstrTitle = _T("读入图像");

	TCHAR * tcharFile = new TCHAR[ 32767 ];
	ZeroMemory( tcharFile, 32767 );  
	MyDlg.m_ofn.lpstrFile    = tcharFile;
	MyDlg.m_ofn.nMaxFile     = 32767;
	MyDlg.m_ofn.nFileOffset  = 0;
	MyDlg.m_ofn.nFilterIndex = 4;
	
	if( MyDlg.DoModal() == IDCANCEL )	 return;

	CString transname;
	transname.Format("%s_cmipstrans.dat", theApp.GetView()->GetDocument()->m_blh);
	CString fstr = theApp.m_SysTmpPath;
	fstr += transname;

	int maxno = 0;
	theApp.ConnectDatabase();
	try
	{
		SAString cmdstr = "Select serialno from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno order by serialno desc";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
		g_dbcommand.Execute();
		if(g_dbcommand.FetchNext())
		{
			maxno = g_dbcommand.Field("serialno").asLong();
		}
		
		g_dbconnection.Commit();
	}
	catch(SAException &x)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
		AfxMessageBox((const char*)x.ErrText());
		return;
	}

	BeginWaitCursor();
	POSITION pos = MyDlg.GetStartPosition();
	while(pos)
	{
		CString strPathName = MyDlg.GetNextPathName(pos);

		remove(fstr);

		GDALDataset * m_pSrcDataset = (GDALDataset *) GDALOpen ( strPathName, GA_ReadOnly );
		GDALDataset * m_pDstDataset = GS_CreateCopy( theApp.m_SavingMode, fstr, m_pSrcDataset);
		GDALClose(m_pSrcDataset);
		GDALClose(m_pDstDataset);

		SAString  imgdataset;
		imgdataset.Empty();
		CFile file;
		CFileException fe;
		if (file.Open(fstr, CFile::modeRead | CFile::shareDenyNone, &fe))
		{
			char sBuf[32*1024];
			while( 1 )
			{
				if( file.GetPosition() >= file.GetLength() )  break;
				DWORD nRead = file.ReadHuge(sBuf, sizeof(sBuf) );
				imgdataset += SAString( sBuf, nRead );
			}
			file.Close();
		}

		dcmGenerateUniqueIdentifier(instanceUID, SITE_INSTANCE_UID_ROOT);

		try
		{
			SAString cmdstr = "Insert into " + theApp.TABLE_IMAGE + "(medicalno, instanceUID, serialno, imgdataset) values(:medicalno, :instanceUID, :serialno, :imgdataset)";
			
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Param("instanceUID").setAsString() = instanceUID;
			g_dbcommand.Param("serialno").setAsLong()    = ++maxno;
			g_dbcommand.Param("imgdataset").setAsBLob()  = imgdataset;
			
			g_dbcommand.Execute();
			g_dbconnection.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
		}

		imgdataset.Empty();

		remove(fstr);
	}
	EndWaitCursor();

    delete tcharFile;

	LoadThumbnail();
}

void CMedImage::OnDblclkListThumbnail(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	*pResult = 0;
}

void CMedImage::SaveModifiedImg(CString fname)
{
	theApp.ConnectDatabase();

	try
	{
		SAString cmdstr = "Update " + theApp.TABLE_IMAGE + " set imgdataset=:imgdataset where medicalno = :medicalno and serialno = :serialno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Param("serialno").setAsLong() = m_Current_Img_Serialno;
		
		SAString  imgdataset;
		imgdataset.Empty();
		CFile file;
		CFileException fe;
		if (file.Open(fname, CFile::modeRead | CFile::shareDenyNone, &fe))
		{
			char sBuf[32*1024];
			while( 1 )
			{
				if( file.GetPosition() >= file.GetLength() )  break;
				DWORD nRead = file.ReadHuge(sBuf, sizeof(sBuf) );
				imgdataset += SAString( sBuf, nRead );
			}
			file.Close();
		}
		g_dbcommand.Param("imgdataset").setAsBLob()  = imgdataset;
		
		g_dbcommand.Execute();
		g_dbconnection.Commit();
		
		imgdataset.Empty();
	}
	catch(SAException &x)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
		AfxMessageBox((const char*)x.ErrText());
	}

	LoadThumbnail();
}

void CMedImage::ReLoading()
{
	theApp.ConnectDatabase();
	try
	{
		SAString cmdstr = "Select * from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno and serialno = :serialno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Param("serialno").setAsLong()    = m_Current_Img_Serialno;
		
		g_dbcommand.Execute();
		if(g_dbcommand.FetchNext())
		{
			SAString Bstr = g_dbcommand.Field("imgdataset").asBLob();
			if( !Bstr.IsEmpty() )
			{
				const void * binbuf = (const void *)Bstr;
				BYTE * dataptr = (BYTE *)binbuf;
				
				CString transname;
				transname.Format("%s_cmipstrans.dat", theApp.GetView()->GetDocument()->m_blh);
				
				CFile file;
				CFileException fe;
				
				CString fstr = theApp.m_SysTmpPath;
				fstr += transname;
				remove(fstr);
				if (file.Open( fstr , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
				{
					file.WriteHuge( dataptr , Bstr.GetLength());
					file.Close();
				}
				
				theApp.GetView()->LoadImageFile(fstr, true);
				
				remove(fstr);
			}
		}
		
		g_dbconnection.Commit();
	}
	catch(SAException &x)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
		AfxMessageBox((const char*)x.ErrText());
		return;
	}
}

void CMedImage::OnClickListThumbnail(NMHDR* pNMHDR, LRESULT* pResult) 
{
	char instanceUID[100];

	//-----------------------------------
	if(m_PrinterSelMode)
	{
		CUIntArray   m_SerialList;
		CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
		int nItem = -1;
		for(int i = 0; i < m_pListThumbnail->GetSelectedCount(); i ++ )
		{
			nItem = m_pListThumbnail->GetNextItem(nItem, LVNI_SELECTED);
			
			CString str = m_pListThumbnail->GetItemText(nItem, 0);
			str = str.Left(str.Find('('));
			
			m_SerialList.Add(atoi(str));
		}
		
		if( m_SerialList.GetSize() < 1 )
		{
			return;
		}
		
		SAString imgdataset[100];
		SAString imgnote[100];
		bool     printflag[100];
		int      serialno[100];
		int count = 0;
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno order by serialno asc";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			
			g_dbcommand.Execute();
			while(g_dbcommand.FetchNext())
			{
				imgdataset[count] = g_dbcommand.Field("imgdataset").asBLob();
				imgnote[count]    = g_dbcommand.Field("imgnote").asString();
				printflag[count]  = g_dbcommand.Field("printflag").asBool();
				serialno[count]   = g_dbcommand.Field("serialno").asLong();
				count++;
				if(count >= 99) break;
			}
			
			g_dbconnection.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
		}
		
		try
		{
			SAString cmdstr = "Delete from " + theApp.TABLE_IMAGE + " where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			
			g_dbcommand.Execute();
			g_dbconnection.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
		}
		
		for(i = 0; i < count; i++)
		{
			for(int index = 0; index < m_SerialList.GetSize(); index++)
			{
				if(m_SerialList.GetAt(index) == serialno[i])  break;
			}
			if(index < m_SerialList.GetSize())
			{
				printflag[i] = false;
			}
		}
		
		int maxno = 0;
		
		int k = 0;
		while(k < count && printflag[k]==true)
		{
			if(!imgdataset[k].IsEmpty())
			{

				dcmGenerateUniqueIdentifier(instanceUID, SITE_INSTANCE_UID_ROOT);

				try
				{
					SAString cmdstr = "Insert into " + theApp.TABLE_IMAGE + "(medicalno, instanceUID, serialno, imgdataset, imgnote, printflag) values(:medicalno,:instanceUID, :serialno,:imgdataset,:imgnote, 1)";
					
					g_dbcommand.setCommandText( cmdstr );
					g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
					g_dbcommand.Param("instanceUID").setAsString() = instanceUID;
					g_dbcommand.Param("serialno").setAsLong()    = ++maxno;
					g_dbcommand.Param("imgdataset").setAsBLob()  = imgdataset[k];
					g_dbcommand.Param("imgnote").setAsString()   = imgnote[k];
					
					g_dbcommand.Execute();
					g_dbconnection.Commit();
				}
				catch(SAException &x)
				{
					try
					{
						g_dbconnection.Rollback();
					}
					catch(SAException &)
					{
					}
					AfxMessageBox((const char*)x.ErrText());
				}
			}
			k++;
		}
		
		for(int index = 0; index < m_SerialList.GetSize(); index++)
		{
			for(i = k; i < count; i++)
			{
				if(m_SerialList.GetAt(index) == serialno[i])  break;
			}
			if(i < count)
			{

				dcmGenerateUniqueIdentifier(instanceUID, SITE_INSTANCE_UID_ROOT);

				try
				{
					SAString cmdstr = "Insert into " + theApp.TABLE_IMAGE + "(medicalno, instanceUID, serialno, imgdataset, imgnote, printflag) values(:medicalno,:instanceUID, :serialno,:imgdataset,:imgnote, 1)";
					
					g_dbcommand.setCommandText( cmdstr );
					g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
					g_dbcommand.Param("instanceUID").setAsString() = instanceUID;
					g_dbcommand.Param("serialno").setAsLong()    = ++maxno;
					g_dbcommand.Param("imgdataset").setAsBLob()  = imgdataset[i];
					g_dbcommand.Param("imgnote").setAsString()   = imgnote[i];
					
					g_dbcommand.Execute();
					g_dbconnection.Commit();
				}
				catch(SAException &x)
				{
					try
					{
						g_dbconnection.Rollback();
					}
					catch(SAException &)
					{
					}
					AfxMessageBox((const char*)x.ErrText());
				}
				
				imgdataset[i].Empty();
			}
		}
		
		while(k < count)
		{
			if(!imgdataset[k].IsEmpty())
			{
				dcmGenerateUniqueIdentifier(instanceUID, SITE_INSTANCE_UID_ROOT);

				try
				{
					SAString cmdstr = "Insert into " + theApp.TABLE_IMAGE + "(medicalno, instanceUID, serialno, imgdataset, imgnote, printflag) values(:medicalno,:instanceUID, :serialno,:imgdataset,:imgnote,:printflag)";
					
					g_dbcommand.setCommandText( cmdstr );
					g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
					g_dbcommand.Param("instanceUID").setAsString() = instanceUID;
					g_dbcommand.Param("serialno").setAsLong()    = ++maxno;
					g_dbcommand.Param("imgdataset").setAsBLob()  = imgdataset[k];
					g_dbcommand.Param("imgnote").setAsString()   = imgnote[k];
					g_dbcommand.Param("printflag").setAsBool()   = printflag[k];
					
					g_dbcommand.Execute();
					g_dbconnection.Commit();
				}
				catch(SAException &x)
				{
					try
					{
						g_dbconnection.Rollback();
					}
					catch(SAException &)
					{
					}
					AfxMessageBox((const char*)x.ErrText());
				}
			}
			k++;
		}
		
		m_SerialList.RemoveAll();
		
		for(i = 0; i < count; i++)
		{
			imgdataset[i].Empty();
			imgnote[i].Empty();
		}
		
		LoadThumbnail();

		return;
	}
	//-----------------------------------

	ShowSelectedItem();

	*pResult = 0;
}

void CMedImage::OnRclickListThumbnail(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
	if( m_pListThumbnail->GetSelectedCount() < 1 )   return;

	CPoint point;
	GetCursorPos(&point);
	
	CMenu* menu;
	menu = GetMenu();
	CMenu* pPopup;
	CWnd* pWndPopupOwner = this;

	menu->LoadMenu( IDR_MENU_PICPROCESS );
	
	pPopup = menu->GetSubMenu(0);
	ASSERT(pPopup != NULL);
	
	CMenu* pPop2 = pPopup->GetSubMenu(4);

	CStdioFile File;
	if ( File.Open(theApp.m_Exe_Working_Dir + "system\\picword.txt", CFile::modeRead | CFile::typeText) )
	{
		CString tmp;
		//int index = 0;
		while(1)
		{
			if( File.ReadString(tmp) == FALSE ) break;
			tmp.TrimLeft(); tmp.TrimRight();
			if(!tmp.IsEmpty())
			{
				pPop2->AppendMenu(MF_STRING , ID_NOTE_1 + index , tmp);
				m_quickInfo.Add(tmp);
			}
			index++;
		}
		File.Close();
	}

	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		pPopup->DeleteMenu(ID_PIC_ROTATE, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_PIC_MIRROR, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_PIC_FUSION, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_PIC_RESAMPLE, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_PIC_EXCHANGE, MF_BYCOMMAND);
		//pPopup->DeleteMenu(ID_PIC_PRINT, MF_BYCOMMAND);
		//pPopup->DeleteMenu(ID_PIC_UNPRINT, MF_BYCOMMAND);
		pPopup->DeleteMenu(ID_PIC_SAVEAS, MF_BYCOMMAND);
	}

	if(! RIGHT_TXDC(theApp.m_Cpp_Right) )
	{
		pPopup->DeleteMenu(ID_PIC_OUTPUT, MF_BYCOMMAND);
	}
	
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();
	
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		this);

	menu->DestroyMenu();
	//Detach(pPopup);
	
	*pResult = 0;
}

void CMedImage::OnPicRotate() 
{
	// TODO: Add your command handler code here
	theApp.ConnectDatabase();

    CDlgRotation mydlg;
	CStringArray m_FileList;
	CUIntArray   m_SerialList;

	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
	if(m_pListThumbnail->GetItemCount() < 1)  return;

	int nItem = -1;
	for(int i = 0; i < m_pListThumbnail->GetSelectedCount(); i ++ )
	{
		nItem = m_pListThumbnail->GetNextItem(nItem, LVNI_SELECTED);

		CString str = m_pListThumbnail->GetItemText(nItem, 0);
		str = str.Left(str.Find('('));
		
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno and serialno = :serialno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Param("serialno").setAsLong()    = atoi(str);
			
			g_dbcommand.Execute();
			if(g_dbcommand.FetchNext())
			{
				SAString Bstr = g_dbcommand.Field("imgdataset").asBLob();
				if( !Bstr.IsEmpty() )
				{
					const void * binbuf = (const void *)Bstr;
					BYTE * dataptr = (BYTE *)binbuf;
					
					CString transname;
					transname.Format("%s_%d_cmipstrans.dat", theApp.GetView()->GetDocument()->m_blh, atoi(str));
					
					CFile file;
					CFileException fe;
					
					CString fstr = theApp.m_SysTmpPath;
					fstr += transname;
					remove(fstr);
					if (file.Open( fstr , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
					{
						file.WriteHuge( dataptr , Bstr.GetLength());
						file.Close();
					}
					
					mydlg.m_File_List.Add(fstr);
					m_FileList.Add(fstr);
					m_SerialList.Add(atoi(str));
				}
			}
			
			g_dbconnection.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
			return;
		}
	}
	if (m_FileList.GetSize() > 0 && mydlg.DoModal() == IDOK )
	{
		for(i = 0; i < m_FileList.GetSize(); i++)
		{
			try
			{
				SAString cmdstr = "Update " + theApp.TABLE_IMAGE + " set imgdataset=:imgdataset where medicalno = :medicalno and serialno = :serialno";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
				g_dbcommand.Param("serialno").setAsLong() = m_SerialList.GetAt(i);

				SAString  imgdataset;
				imgdataset.Empty();
				CFile file;
				CFileException fe;
				if (file.Open(m_FileList.GetAt(i), CFile::modeRead | CFile::shareDenyNone, &fe))
				{
					char sBuf[32*1024];
					while( 1 )
					{
						if( file.GetPosition() >= file.GetLength() )  break;
						DWORD nRead = file.ReadHuge(sBuf, sizeof(sBuf) );
						imgdataset += SAString( sBuf, nRead );
					}
					file.Close();
				}
				g_dbcommand.Param("imgdataset").setAsBLob()  = imgdataset;
				
				g_dbcommand.Execute();
				g_dbconnection.Commit();

				imgdataset.Empty();
			}
			catch(SAException &x)
			{
				try
				{
					g_dbconnection.Rollback();
				}
				catch(SAException &)
				{
				}
				AfxMessageBox((const char*)x.ErrText());
			}
		}

		for(i = 0; i < m_FileList.GetSize(); i++)
		{
			remove(m_FileList.GetAt(i));
		}

		m_FileList.RemoveAll();
		m_SerialList.RemoveAll();

		LoadThumbnail();
	}
}

void CMedImage::OnPicMirror() 
{
	// TODO: Add your command handler code here
	theApp.ConnectDatabase();

    CDlgReflection mydlg;
	CStringArray m_FileList;
	CUIntArray   m_SerialList;

	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
	int nItem = -1;
	for(int i = 0; i < m_pListThumbnail->GetSelectedCount(); i ++ )
	{
		nItem = m_pListThumbnail->GetNextItem(nItem, LVNI_SELECTED);

		CString str = m_pListThumbnail->GetItemText(nItem, 0);
		str = str.Left(str.Find('('));
		
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno and serialno = :serialno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Param("serialno").setAsLong()    = atoi(str);
			
			g_dbcommand.Execute();
			if(g_dbcommand.FetchNext())
			{
				SAString Bstr = g_dbcommand.Field("imgdataset").asBLob();
				if( !Bstr.IsEmpty() )
				{
					const void * binbuf = (const void *)Bstr;
					BYTE * dataptr = (BYTE *)binbuf;
					
					CString transname;
					transname.Format("%s_%d_cmipstrans.dat", theApp.GetView()->GetDocument()->m_blh, atoi(str));
					
					CFile file;
					CFileException fe;
					
					CString fstr = theApp.m_SysTmpPath;
					fstr += transname;
					remove(fstr);
					if (file.Open( fstr , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
					{
						file.WriteHuge( dataptr , Bstr.GetLength());
						file.Close();
					}
					
					mydlg.m_File_List.Add(fstr);
					m_FileList.Add(fstr);
					m_SerialList.Add(atoi(str));
				}
			}
			
			g_dbconnection.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
			return;
		}
	}

	if (m_FileList.GetSize() > 0 && mydlg.DoModal() == IDOK )
	{
		for(i = 0; i < m_FileList.GetSize(); i++)
		{
			try
			{
				SAString cmdstr = "Update " + theApp.TABLE_IMAGE + " set imgdataset=:imgdataset where medicalno = :medicalno and serialno = :serialno";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
				g_dbcommand.Param("serialno").setAsLong() = m_SerialList.GetAt(i);

				SAString  imgdataset;
				imgdataset.Empty();
				CFile file;
				CFileException fe;
				if (file.Open(m_FileList.GetAt(i), CFile::modeRead | CFile::shareDenyNone, &fe))
				{
					char sBuf[32*1024];
					while( 1 )
					{
						if( file.GetPosition() >= file.GetLength() )  break;
						DWORD nRead = file.ReadHuge(sBuf, sizeof(sBuf) );
						imgdataset += SAString( sBuf, nRead );
					}
					file.Close();
				}
				g_dbcommand.Param("imgdataset").setAsBLob()  = imgdataset;
				
				g_dbcommand.Execute();
				g_dbconnection.Commit();

				imgdataset.Empty();
			}
			catch(SAException &x)
			{
				try
				{
					g_dbconnection.Rollback();
				}
				catch(SAException &)
				{
				}
				AfxMessageBox((const char*)x.ErrText());
			}
		}

		for(i = 0; i < m_FileList.GetSize(); i++)
		{
			remove(m_FileList.GetAt(i));
		}

		m_FileList.RemoveAll();
		m_SerialList.RemoveAll();

		LoadThumbnail();
	}
}

void CMedImage::OnPicFusion() 
{
	// TODO: Add your command handler code here
	CStringArray m_File_List;
	CUIntArray   m_SerialList;
	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
	int nItem = -1;
	for(int i = 0; i < m_pListThumbnail->GetSelectedCount(); i ++ )
	{
		nItem = m_pListThumbnail->GetNextItem(nItem, LVNI_SELECTED);

		CString str = m_pListThumbnail->GetItemText(nItem, 0);
		str = str.Left(str.Find('('));
		
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno and serialno = :serialno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Param("serialno").setAsLong()    = atoi(str);
			
			g_dbcommand.Execute();
			if(g_dbcommand.FetchNext())
			{
				SAString Bstr = g_dbcommand.Field("imgdataset").asBLob();
				if( !Bstr.IsEmpty() )
				{
					const void * binbuf = (const void *)Bstr;
					BYTE * dataptr = (BYTE *)binbuf;
					
					CString transname;
					transname.Format("%s_%d_cmipstrans.dat", theApp.GetView()->GetDocument()->m_blh, atoi(str));
					
					CFile file;
					CFileException fe;
					
					CString fstr = theApp.m_SysTmpPath;
					fstr += transname;
					remove(fstr);
					if (file.Open( fstr , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
					{
						file.WriteHuge( dataptr , Bstr.GetLength());
						file.Close();
					}
					
					m_File_List.Add(fstr);
					m_SerialList.Add(atoi(str));
				}
			}
			
			g_dbconnection.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
			return;
		}
	}

	if( m_File_List.GetSize() < 2 )
	{
		for(i = 0; i < m_File_List.GetSize(); i++)
		{
			remove(m_File_List.GetAt(i));
		}

		m_File_List.RemoveAll();
		m_SerialList.RemoveAll();
		return;
	}

	BeginWaitCursor();

	CString tmpimg = theApp.m_SysTmpPath;
	tmpimg += "00000001.bmp";
	remove(tmpimg);

	CStringArray m_TmpArray;
	m_TmpArray.Copy(m_File_List);

	ImageFusion( m_File_List, tmpimg );

	m_File_List.RemoveAll();
	m_File_List.Copy(m_TmpArray);
	m_TmpArray.RemoveAll();

	((CMainFrame*)AfxGetMainWnd())->AddNewImage(tmpimg);

	remove(tmpimg);

	EndWaitCursor();

	for(i = 0; i < m_File_List.GetSize(); i++)
	{
		remove(m_File_List.GetAt(i));
	}

	m_File_List.RemoveAll();
	m_SerialList.RemoveAll();
}

void CMedImage::OnPicResample() 
{
	// TODO: Add your command handler code here
	CStringArray m_File_List;
	CUIntArray   m_SerialList;
	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
	int nItem = -1;
	for(int i = 0; i < m_pListThumbnail->GetSelectedCount(); i ++ )
	{
		nItem = m_pListThumbnail->GetNextItem(nItem, LVNI_SELECTED);

		CString str = m_pListThumbnail->GetItemText(nItem, 0);
		str = str.Left(str.Find('('));
		
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno and serialno = :serialno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Param("serialno").setAsLong()    = atoi(str);
			
			g_dbcommand.Execute();
			if(g_dbcommand.FetchNext())
			{
				SAString Bstr = g_dbcommand.Field("imgdataset").asBLob();
				if( !Bstr.IsEmpty() )
				{
					const void * binbuf = (const void *)Bstr;
					BYTE * dataptr = (BYTE *)binbuf;
					
					CString transname;
					transname.Format("%s_%d_cmipstrans.dat", theApp.GetView()->GetDocument()->m_blh, atoi(str));
					
					CFile file;
					CFileException fe;
					
					CString fstr = theApp.m_SysTmpPath;
					fstr += transname;
					remove(fstr);
					if (file.Open( fstr , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
					{
						file.WriteHuge( dataptr , Bstr.GetLength());
						file.Close();
					}
					
					m_File_List.Add(fstr);
					m_SerialList.Add(atoi(str));
				}
			}
			
			g_dbconnection.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
			return;
		}
	}

	if( m_File_List.GetSize() < 1 )
	{
		return;
	}

	GDALDataset * m_pSrcDataset = (GDALDataset *) GDALOpen ( m_File_List.GetAt(0), GA_ReadOnly );
	int imgwidth  = m_pSrcDataset->GetRasterXSize();
	int imgheight = m_pSrcDataset->GetRasterYSize();
	GDALClose(m_pSrcDataset);	m_pSrcDataset = NULL;

	CResize mydlg;

	if(m_File_List.GetSize() > 1)
	{
		for(int i = 1; i < m_File_List.GetSize(); i++)
		{
			GDALDataset * m_pSrcDataset = (GDALDataset *) GDALOpen ( m_File_List.GetAt(i), GA_ReadOnly );
			if( m_pSrcDataset->GetRasterXSize() != imgwidth || m_pSrcDataset->GetRasterYSize() != imgheight )
			{
				GDALClose(m_pSrcDataset);	m_pSrcDataset = NULL;
				break;
			}
			GDALClose(m_pSrcDataset);	m_pSrcDataset = NULL;
		}
		if( i < m_File_List.GetSize() )
		{
			imgwidth = imgheight = 0;
			mydlg.m_flag = true;
		}
	}

	mydlg.m_width  = imgwidth;
	mydlg.m_height = imgheight;

	if(mydlg.DoModal() == IDCANCEL)
	{
		for(i = 0; i < m_File_List.GetSize(); i++)
		{
			remove(m_File_List.GetAt(i));
		}

		m_File_List.RemoveAll();
		m_SerialList.RemoveAll();
		return;
	}

	CString tmpimg = theApp.m_SysTmpPath;
	tmpimg += "00000001.bmp";

	GDALInterpolateMethod  samplemethod;
	switch(mydlg.m_SampleMethod) {
	case 0:
		samplemethod = GIM_Nearest;
		break;
	case 1:
		samplemethod = GIM_Bilinear;
		break;
	case 2:
		samplemethod = GIM_CubicBSpline;
		break;
	}

	BeginWaitCursor();

	mkdir("C:\\MingDL");
	char *pBmpFile = "C:\\MingDL\\resample.bmp";
	char *pJpgFile = "C:\\MingDL\\resample.jpg";
	char *pTifFile = "C:\\MingDL\\resample.tif";

	CString NewImg;
	NewImg.Format("C:\\MingDL\\%s_newgen.bmp", theApp.GetView()->GetDocument()->m_blh);
	
	for( i = 0; i < m_File_List.GetSize(); i++ )
	{
		remove(NewImg);

		GDALDataset * m_pSrcDataset,* m_pDstDataset;
		m_pSrcDataset = (GDALDataset *) GDALOpen ( m_File_List.GetAt(i), GA_ReadOnly );
		int bandnum = m_pSrcDataset->GetRasterCount();
		int OrigenWidth  = m_pSrcDataset->GetRasterXSize();
		int OrigenHeight = m_pSrcDataset->GetRasterYSize();
		GDALClose(m_pSrcDataset);	m_pSrcDataset = NULL;

		int NewWidth  = mydlg.m_width;
		int NewHeight = mydlg.m_height;

		if( mydlg.m_flag )
		{
			NewWidth  = (int)( OrigenWidth  * mydlg.m_widthrate  / 100.0 );
			NewHeight = (int)( OrigenHeight * mydlg.m_heightrate / 100.0 );
		}

		if( mydlg.m_SampleMethod <= 2 )
		{
			HDIB hDIB = ::CreateDIBFile(NewWidth,NewHeight,bandnum * 8);
			remove(tmpimg);
			
			CFile file;
			CFileException fe;
			
			if (!file.Open(tmpimg, CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
			{
				::GlobalFree((HGLOBAL) hDIB);
				hDIB = NULL;
				continue;
			}
			
			::SaveDIB( hDIB, file );
			file.Close();
			::GlobalFree((HGLOBAL) hDIB);
			hDIB = NULL;
			
			m_pSrcDataset = (GDALDataset *) GDALOpen ( m_File_List.GetAt(i), GA_ReadOnly );
			
			m_pDstDataset = (GDALDataset *) GDALOpen ( tmpimg, GA_Update );
			
			double xrate = OrigenWidth  * 1.0 / NewWidth;
			double yrate = OrigenHeight * 1.0 / NewHeight;
			
			if(mydlg.m_SampleMethod == 1)
			{
				if( xrate < 1.0 )
				{
					xrate = OrigenWidth  * 1.0 / (NewWidth  + (int)(NewWidth  * 1.0 / OrigenWidth  + 0.5));
				}
				if( yrate < 1.0 )
				{
					yrate = OrigenHeight * 1.0 / (NewHeight + (int)(NewHeight * 1.0 / OrigenHeight + 0.5));
				}
			}
			
			double ox,oy,value;
			for(int bandno = 1; bandno <= bandnum; bandno ++ )
			{
				GDALRasterBand * pSrcBand = m_pSrcDataset->GetRasterBand(bandno);
				GDALRasterBand * pDstBand = m_pDstDataset->GetRasterBand(bandno);
				for(int x = 0; x < NewWidth; x++ )
				{
					for(int y = 0; y < NewHeight; y++ )
					{
						ox = x * xrate;
						oy = y * yrate;
						switch(mydlg.m_SampleMethod) {
						case 0:
						case 1:
							pSrcBand->PixelResample(ox,oy,&value,samplemethod);
							break;
						case 2:
							{
								int orix = (int)ox; // src coords
								int oriy = (int)oy;
								
								double dx = ox - orix;
								double dy = oy - oriy;
								
								value = 0;
								
								for(int n = -1; n <= 2; n++) {
									for(int m = -1; m <= 2; m++) {
										int srcy = oriy + n;
										int srcx = orix + m;
										
										if (srcy < 0) srcy = 0; 
										if (srcy >= OrigenHeight) srcy = OrigenHeight - 1;
										
										if (srcx < 0) srcx = 0;
										if (srcx >= OrigenWidth)  srcx = OrigenWidth - 1;
										
										double tmp;
										pSrcBand->PixelIO(GF_Read,srcx,srcy,&tmp);
										
										double fac = R(m - dx) * R(dy - n);
										
										value += tmp * fac;
									}
								}
							}
							break;
						}
						if(value < 0)   value = 0;
						if(value > 255) value = 255;
						pDstBand->PixelIO(GF_Write,x,y,&value);
					}
				}
			}
			
			GDALClose(m_pSrcDataset);	m_pSrcDataset = NULL;
			GDALClose(m_pDstDataset);	m_pDstDataset = NULL;
			
			m_pSrcDataset = (GDALDataset *) GDALOpen ( tmpimg, GA_ReadOnly );
			CString NewImgName = NewImg;
			m_pDstDataset = GS_CreateCopy( theApp.m_SavingMode, NewImgName, m_pSrcDataset);
			
			GDALClose(m_pSrcDataset);	m_pSrcDataset = NULL;
			GDALClose(m_pDstDataset);	m_pDstDataset = NULL;
		}
		else
		{
			HINSTANCE	hLib;
			if( (hLib=LoadLibrary("LibResample")) == 0 )  continue;
			typedef VOID (*MYPROC)(int argc, char ** argv); 
			MYPROC ProcInDLL;
			ProcInDLL = (MYPROC) GetProcAddress(hLib, "ImgResample");
			
			if( ProcInDLL != NULL )
			{
				int argc = 13;
				char *argv[20];

				argv[0] = "zoom";
				argv[1] = "-src";

				char FileIn[1000];
				GetShortPathName(m_File_List.GetAt(i),FileIn,1000); 
				//argv[2] = FileIn;
				GDALDataset * m_pSrcDataset = (GDALDataset *) GDALOpen ( m_File_List.GetAt(i), GA_ReadOnly );
				CString m_FileType = GDALGetDriverShortName( m_pSrcDataset->GetDriver() );
				GDALClose(m_pSrcDataset);	m_pSrcDataset = NULL;

				if( m_FileType.Compare("BMP") == 0 )
				{
					remove(pBmpFile);
					CopyFile(FileIn,pBmpFile,FALSE);
					argv[2] = pBmpFile;
				}

				if( m_FileType.Compare("JPEG") == 0 )
				{
					remove(pJpgFile);
					CopyFile(FileIn,pJpgFile,FALSE);
					argv[2] = pJpgFile;
				}

				if( m_FileType.Compare("GTiff") == 0 )
				{
					remove(pTifFile);
					CopyFile(FileIn,pTifFile,FALSE);
					argv[2] = pTifFile;
				}

				argv[3] = "-dst";

				char FileOut[1000];
				strcpy(FileOut,NewImg);
				argv[4] = FileOut;

				argv[5] = "-d";
				argv[6] = "0";
				argv[7] = "0";

				char widthstr[1000];
				sprintf(widthstr,"%d",NewWidth);
				argv[8] = widthstr;

				char heightstr[1000];
				sprintf(heightstr,"%d",NewHeight);
				argv[9] = heightstr;

				argv[10] = "-square";
				argv[11] = "-filt";

				switch(mydlg.m_SampleMethod) {
				case 3:
					argv[12] = "triangle";
					break;
				case 4:
					argv[12] = "quadratic";
					break;
				case 5:
					argv[12] = "cubic";
					break;
				case 6:
					argv[12] = "catrom";
					break;
				case 7:
					argv[12] = "mitchell";
					break;
				case 8:
					argv[12] = "gaussian";
					break;
				case 9:
					argv[12] = "sinc";
					break;
				case 10:
					argv[12] = "bessel";
					break;
				case 11:
					argv[12] = "hanning";
					break;
				case 12:
					argv[12] = "hamming";
					break;
				case 13:
					argv[12] = "blackman";
					break;
				case 14:
					argv[12] = "kaiser";
					break;
				}

				char blurstr[1000];

				if(mydlg.m_SampleMethod > 7)
				{
					argv[13] = "-blur";
					sprintf(blurstr,"%f", mydlg.m_factor);
					argv[14] = blurstr;
					argc += 2;
				}

				(ProcInDLL) (argc,argv); 

				remove(pBmpFile);
				remove(pJpgFile);
			}
			
			BOOL rt = FreeLibrary(hLib);
			hLib = 0;
		}

		((CMainFrame*)AfxGetMainWnd())->AddNewImage(NewImg);
		remove(NewImg);
	}

	remove(tmpimg);

	for(i = 0; i < m_File_List.GetSize(); i++)
	{
		remove(m_File_List.GetAt(i));
	}

	m_File_List.RemoveAll();
	m_SerialList.RemoveAll();

	EndWaitCursor();
}

void CMedImage::OnPicNote() 
{
	// TODO: Add your command handler code here
	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
	if( m_pListThumbnail->GetSelectedCount() > 0 )
	{
		int nItem = -1;
		nItem = m_pListThumbnail->GetNextItem(nItem, LVNI_SELECTED);
		if( nItem >= 0 )
		{
			if(m_pListThumbnail->GetItemCount() > 0 && nItem < m_pListThumbnail->GetItemCount())
			{
				CString str = m_pListThumbnail->GetItemText(nItem, 0);
				str = str.Left(str.Find('('));

				CString imgnote; imgnote.Empty();
				theApp.ConnectDatabase();
				try
				{
					SAString cmdstr = "Select imgnote from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno and serialno = :serialno";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
					g_dbcommand.Param("serialno").setAsLong()    = atoi(str);
					
					g_dbcommand.Execute();
					if(g_dbcommand.FetchNext())
					{
						imgnote = g_dbcommand.Field("imgnote").asString();
					}
					
					g_dbconnection.Commit();
				}
				catch(SAException &x)
				{
					try
					{
						g_dbconnection.Rollback();
					}
					catch(SAException &)
					{
					}
					AfxMessageBox((const char*)x.ErrText());
					return;
				}

				CString title;
				title.Format("图像注释<%s - %d>", theApp.GetView()->GetDocument()->m_blh, atoi(str));
				CPicnote Dlg(this);
				Dlg.title = title;
				Dlg.m_note = imgnote;

				if(Dlg.DoModal() == IDOK)
				{
					try
					{
						SAString cmdstr = "Update " + theApp.TABLE_IMAGE + " set imgnote = :imgnote where medicalno = :medicalno and serialno = :serialno";
						g_dbcommand.setCommandText(cmdstr);
						g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
						g_dbcommand.Param("serialno").setAsLong()    = atoi(str);
						g_dbcommand.Param("imgnote").setAsString()   = Dlg.m_note;
						
						g_dbcommand.Execute();
						
						g_dbconnection.Commit();
					}
					catch(SAException &x)
					{
						try
						{
							g_dbconnection.Rollback();
						}
						catch(SAException &)
						{
						}
						AfxMessageBox((const char*)x.ErrText());
					}
				}
			}
		}
	}
}

void CMedImage::OnPicOutput() 
{
	// TODO: Add your command handler code here
	CXTBrowseDialog browseDlg;
	browseDlg.SetTitle("选择图像导出目录");
	if( IDCANCEL == browseDlg.DoModal() )	return;
	CString strFileName = browseDlg.GetSelPath();
    strFileName += "\\";
	char		m_szDrive[_MAX_DRIVE];
	char		m_szDir[_MAX_DIR];
	char		m_szFname[_MAX_FNAME];
	char		m_szExt[_MAX_EXT];
	
	_splitpath(strFileName, m_szDrive, m_szDir, m_szFname, m_szExt);
	CString WorkingPath = browseDlg.GetSelPath();
	if(strcmp(m_szDir,"\\\\") != 0 )
	{
		WorkingPath += "\\";
	}

	theApp.ConnectDatabase();

	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
	int nItem = -1;
	for(int i = 0; i < m_pListThumbnail->GetSelectedCount(); i ++ )
	{
		nItem = m_pListThumbnail->GetNextItem(nItem, LVNI_SELECTED);

		CString str = m_pListThumbnail->GetItemText(nItem, 0);
		str = str.Left(str.Find('('));
		
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno and serialno = :serialno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Param("serialno").setAsLong()    = atoi(str);
			
			g_dbcommand.Execute();
			if(g_dbcommand.FetchNext())
			{
				SAString Bstr = g_dbcommand.Field("imgdataset").asBLob();
				if( !Bstr.IsEmpty() )
				{
					const void * binbuf = (const void *)Bstr;
					BYTE * dataptr = (BYTE *)binbuf;
					
					CString transname;
					transname.Format("%s_%d_cmipstrans.dat", theApp.GetView()->GetDocument()->m_blh, atoi(str));
					
					CFile file;
					CFileException fe;
					
					CString fstr = theApp.m_SysTmpPath;
					fstr += transname;
					remove(fstr);
					if (file.Open( fstr , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
					{
						file.WriteHuge( dataptr , Bstr.GetLength());
						file.Close();
					}
					
					GDALDataset * m_pDataset = (GDALDataset *) GDALOpen ( fstr , GA_ReadOnly );
					CString m_file_type = GDALGetDriverShortName( m_pDataset->GetDriver() );
					GDALClose(m_pDataset);

					CString targetFname; targetFname.Empty();
					if(m_file_type == "BMP")
					{
						targetFname.Format("%s_%d.bmp", theApp.GetView()->GetDocument()->m_blh, atoi(str));
					}
					if(m_file_type == "JPEG")
					{
						targetFname.Format("%s_%d.jpg", theApp.GetView()->GetDocument()->m_blh, atoi(str));
					}
					if(m_file_type == "GTiff") 
					{
						targetFname.Format("%s_%d.tif", theApp.GetView()->GetDocument()->m_blh, atoi(str));
					}

					if(!targetFname.IsEmpty())
					{
						CopyFile(fstr,WorkingPath + targetFname,FALSE);
					}
					remove(fstr);
				}
			}
			
			g_dbconnection.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
			return;
		}
	}
}

void CMedImage::OnPicExchange() 
{
	CUIntArray   m_SerialList;
	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
	int nItem = -1;
	for(int i = 0; i < m_pListThumbnail->GetSelectedCount(); i ++ )
	{
		nItem = m_pListThumbnail->GetNextItem(nItem, LVNI_SELECTED);

		CString str = m_pListThumbnail->GetItemText(nItem, 0);
		str = str.Left(str.Find('('));
		
		m_SerialList.Add(atoi(str));
	}

	if( m_SerialList.GetSize() != 2 )
	{
		AfxMessageBox("图像置换在两幅图像之间进行!");
		m_SerialList.RemoveAll();
		return;
	}

	int tmp = 1000;
	try
	{
		SAString cmdstr = "Update " + theApp.TABLE_IMAGE + " set serialno = :serialno1 where medicalno = :medicalno and serialno = :serialno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Param("serialno").setAsLong() = m_SerialList.GetAt(0);
		g_dbcommand.Param("serialno1").setAsLong() = tmp;
		
		g_dbcommand.Execute();
		g_dbconnection.Commit();
	}
	catch(SAException &x)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
		AfxMessageBox((const char*)x.ErrText());
	}

	try
	{
		SAString cmdstr = "Update " + theApp.TABLE_IMAGE + " set serialno = :serialno1 where medicalno = :medicalno and serialno = :serialno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Param("serialno").setAsLong() = m_SerialList.GetAt(1);
		g_dbcommand.Param("serialno1").setAsLong() = m_SerialList.GetAt(0);
		
		g_dbcommand.Execute();
		g_dbconnection.Commit();
	}
	catch(SAException &x)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
		AfxMessageBox((const char*)x.ErrText());
	}

	try
	{
		SAString cmdstr = "Update " + theApp.TABLE_IMAGE + " set serialno = :serialno1 where medicalno = :medicalno and serialno = :serialno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Param("serialno").setAsLong() = tmp;
		g_dbcommand.Param("serialno1").setAsLong() = m_SerialList.GetAt(1);
		
		g_dbcommand.Execute();
		g_dbconnection.Commit();
	}
	catch(SAException &x)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
		AfxMessageBox((const char*)x.ErrText());
	}

	m_SerialList.RemoveAll();

	LoadThumbnail();
}

void CMedImage::OnPicPrint() 
{
	CUIntArray   m_SerialList;
	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
	for(int i = 0; i < m_pListThumbnail->GetItemCount(); i ++ )
	{
		CString str = m_pListThumbnail->GetItemText(i, 0);
		str = str.Left(str.Find('('));
		
		m_SerialList.Add(atoi(str));
	}

	if( m_SerialList.GetSize() < 1 )
	{
		m_SerialList.RemoveAll();
		return;
	}

	for(int index = 0; index < m_SerialList.GetSize(); index++)
	{
		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_IMAGE + " set printflag = 0 where medicalno = :medicalno and serialno = :serialno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Param("serialno").setAsLong() = m_SerialList.GetAt(index);
			
			g_dbcommand.Execute();
			g_dbconnection.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
		}
	}

	m_SerialList.RemoveAll();

	LoadThumbnail();
}

void CMedImage::OnPicUnprint() 
{
	CUIntArray   m_SerialList;
	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
	int nItem = -1;
	for(int i = 0; i < m_pListThumbnail->GetSelectedCount(); i ++ )
	{
		nItem = m_pListThumbnail->GetNextItem(nItem, LVNI_SELECTED);

		CString str = m_pListThumbnail->GetItemText(nItem, 0);
		str = str.Left(str.Find('('));
		
		m_SerialList.Add(atoi(str));
	}

	if( m_SerialList.GetSize() < 1 )
	{
		m_SerialList.RemoveAll();
		return;
	}

	for(int index = 0; index < m_SerialList.GetSize(); index++)
	{
		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_IMAGE + " set printflag = 0 where medicalno = :medicalno and serialno = :serialno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Param("serialno").setAsLong() = m_SerialList.GetAt(index);
			
			g_dbcommand.Execute();
			g_dbconnection.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
		}
	}

	m_SerialList.RemoveAll();

	LoadThumbnail();
}

void CMedImage::OnPicSaveas() 
{
	char instanceUID[100];

	theApp.ConnectDatabase();

	//--------------------------------------------
	int maxno = 0;
	try
	{
		SAString cmdstr = "Select serialno from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno order by serialno desc";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
		g_dbcommand.Execute();
		if(g_dbcommand.FetchNext())
		{
			maxno = g_dbcommand.Field("serialno").asLong();
		}
		
		g_dbconnection.Commit();
	}
	catch(SAException &x)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
		AfxMessageBox((const char*)x.ErrText());
		return;
	}
	//--------------------------------------------

	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
	int nItem = -1;
	for(int i = 0; i < m_pListThumbnail->GetSelectedCount(); i ++ )
	{
		nItem = m_pListThumbnail->GetNextItem(nItem, LVNI_SELECTED);

		CString str = m_pListThumbnail->GetItemText(nItem, 0);
		str = str.Left(str.Find('('));
		
		SAString imgdataset; imgdataset.Empty();
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno and serialno = :serialno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Param("serialno").setAsLong()    = atoi(str);
			
			g_dbcommand.Execute();
			if(g_dbcommand.FetchNext())
			{
				imgdataset = g_dbcommand.Field("imgdataset").asBLob();
			}
			
			g_dbconnection.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
			return;
		}

		if(!imgdataset.IsEmpty())
		{
			dcmGenerateUniqueIdentifier(instanceUID, SITE_INSTANCE_UID_ROOT);

			try
			{
				SAString cmdstr = "Insert into " + theApp.TABLE_IMAGE + "(medicalno, instanceUID,serialno, imgdataset) values(:medicalno,:instanceUID,:serialno,:imgdataset)";
				
				g_dbcommand.setCommandText( cmdstr );
				g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
				g_dbcommand.Param("instanceUID").setAsString() = instanceUID;
				g_dbcommand.Param("serialno").setAsLong()    = ++maxno;
				g_dbcommand.Param("imgdataset").setAsBLob()  = imgdataset;
				
				g_dbcommand.Execute();
				g_dbconnection.Commit();
			}
			catch(SAException &x)
			{
				try
				{
					g_dbconnection.Rollback();
				}
				catch(SAException &)
				{
				}
				AfxMessageBox((const char*)x.ErrText());
			}
			
			imgdataset.Empty();
		}
	}

	LoadThumbnail();
}

void CMedImage::RefreshTabText()
{
	CWnd * pMainWnd = AfxGetMainWnd();
	if(pMainWnd == NULL || !pMainWnd->IsKindOf(RUNTIME_CLASS(CMainFrame)))  return;

	CWnd *pWnd = ((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CMedImage));
	if(pWnd != NULL)
	{
		CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
		CString str;
		if(m_pListThumbnail->GetItemCount() > 0)
		{
			str.Format("图像(%d)", m_pListThumbnail->GetItemCount());
		}
		else
		{
			str.Format("图像");
		}
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.GetTabCtrl().SetTabText(pWnd, str);
	}
}

BOOL CMedImage::PreTranslateMessage(MSG* pMsg) 
{
	//?
	/*

	if( m_hWnd )
    {
        m_ToolTip.RelayEvent(pMsg);
		return CXTResizeFormView::PreTranslateMessage(pMsg);
    }

	if(pMsg->message == WM_COMMAND){
		AfxMessageBox("command");
		return CXTResizeFormView::PreTranslateMessage(pMsg);
	}

	if(pMsg->message == WM_NOTIFY){
		AfxMessageBox("notify");
		return CXTResizeFormView::PreTranslateMessage(pMsg);
	}
    return (FALSE);
	
	//return CXTResizeFormView::PreTranslateMessage(pMsg);
	*/
	//?
	return CXTResizeFormView::PreTranslateMessage(pMsg);
	//?
}

void CMedImage::OnKeydownListThumbnail(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	switch(pLVKeyDow->wVKey) {
	case VK_DELETE:
		DeleteListItem();
		break;
	case VK_SPACE:
		ShowSelectedItem();
		break;
	}
	
	*pResult = 0;
}

void CMedImage::DeleteListItem()
{
	if(!(RIGHT_SCTXYY(theApp.m_Cpp_Right) && !theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh)))  return;

	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
	
	CString bhlist; bhlist.Empty();
	int nItem = -1;
	for(int i = 0; i < m_pListThumbnail->GetSelectedCount(); i ++ )
	{
		nItem = m_pListThumbnail->GetNextItem(nItem, LVNI_SELECTED);
		if( nItem >= 0 )
		{
			CString str = m_pListThumbnail->GetItemText(nItem, 0);
			str = str.Left(str.Find('('));
			if(!bhlist.IsEmpty()) bhlist += "，";
			bhlist += str;
		}
	}
	
/*
	if( ((CMainFrame*)AfxGetMainWnd())->m_realtime )
	{
		((CMainFrame*)AfxGetMainWnd())->TerminateTimer();
	}
*/
	
	if(((CMainFrame*)AfxGetMainWnd())->m_realtime || AfxMessageBox("您是否要删除图像编号为[" + bhlist + "]的图像？",MB_OKCANCEL|MB_ICONQUESTION) == IDOK)
	{
		nItem = -1;
		for( i = 0; i < m_pListThumbnail->GetSelectedCount(); i ++ )
		{
			nItem = m_pListThumbnail->GetNextItem(nItem, LVNI_SELECTED);
			if( nItem >= 0 )
			{
				CString str = m_pListThumbnail->GetItemText(nItem, 0);
				str = str.Left(str.Find('('));

				if(!theApp.m_RecyclePath.IsEmpty())
				{
					mkdir(theApp.m_RecyclePath);
					mkdir(theApp.m_RecyclePath + theApp.GetView()->GetDocument()->m_blh + "\\");
					try
					{
						SAString cmdstr = "Select * from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno and serialno = :serialno";
						g_dbcommand.setCommandText(cmdstr);
						g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
						g_dbcommand.Param("serialno").setAsLong()    = atoi(str);
						
						g_dbcommand.Execute();
						if(g_dbcommand.FetchNext())
						{
							SAString Bstr = g_dbcommand.Field("imgdataset").asBLob();
							if( !Bstr.IsEmpty() )
							{
								const void * binbuf = (const void *)Bstr;
								BYTE * dataptr = (BYTE *)binbuf;
								
								CString transname;
								transname.Format("%s_%d_cmipstrans.dat", theApp.GetView()->GetDocument()->m_blh, atoi(str));
								
								CFile file;
								CFileException fe;
								
								CString fstr = theApp.m_SysTmpPath;
								fstr += transname;
								remove(fstr);
								if (file.Open( fstr , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
								{
									file.WriteHuge( dataptr , Bstr.GetLength());
									file.Close();
								}
								
								GDALDataset * m_pDataset = (GDALDataset *) GDALOpen ( fstr , GA_ReadOnly );
								CString m_file_type = GDALGetDriverShortName( m_pDataset->GetDriver() );
								GDALClose(m_pDataset);
								
								CString targetFname; targetFname.Empty();
								if(m_file_type == "BMP")
								{
									targetFname.Format("%s_%d.bmp", theApp.GetView()->GetDocument()->m_blh, atoi(str));
								}
								if(m_file_type == "JPEG")
								{
									targetFname.Format("%s_%d.jpg", theApp.GetView()->GetDocument()->m_blh, atoi(str));
								}
								if(m_file_type == "GTiff") 
								{
									targetFname.Format("%s_%d.tif", theApp.GetView()->GetDocument()->m_blh, atoi(str));
								}
								
								if(!targetFname.IsEmpty())
								{
									CopyFile(fstr,theApp.m_RecyclePath + theApp.GetView()->GetDocument()->m_blh + "\\" + targetFname,FALSE);
								}
								remove(fstr);
							}
						}
						
						g_dbconnection.Commit();
					}
					catch(SAException &x)
					{
						try
						{
							g_dbconnection.Rollback();
						}
						catch(SAException &)
						{
						}
						AfxMessageBox((const char*)x.ErrText());
						return;
					}
				}
				
				try
				{
					SAString cmdstr = "Delete from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno and serialno = :serialno";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
					g_dbcommand.Param("serialno").setAsLong()    = atoi(str);
					
					g_dbcommand.Execute();
					g_dbconnection.Commit();
				}
				catch(SAException &x)
				{
					try
					{
						g_dbconnection.Rollback();
					}
					catch(SAException &)
					{
					}
					AfxMessageBox((const char*)x.ErrText());
					return;
				}
			}
		}
		
		if(m_pListThumbnail->GetSelectedCount() > 0)
		{
			((CMainFrame*)AfxGetMainWnd())->Sound_Play("recycle.wav");
			
			LoadThumbnail();
		}
	}
}

void CMedImage::ShowImgItem(int nItem)
{
	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
	if(m_pListThumbnail->GetItemCount() > 0 && nItem >= 0 && nItem < m_pListThumbnail->GetItemCount())
	{
		CString str = m_pListThumbnail->GetItemText(nItem, 0);
		str = str.Left(str.Find('('));
		
		theApp.ConnectDatabase();
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno and serialno = :serialno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Param("serialno").setAsLong()    = atoi(str);
			
			g_dbcommand.Execute();
			if(g_dbcommand.FetchNext())
			{
				// keep current loaded image's serialno for further reloading
				m_Current_Img_Serialno = atoi(str);
				
				SAString Bstr = g_dbcommand.Field("imgdataset").asBLob();
				if( !Bstr.IsEmpty() )
				{
					const void * binbuf = (const void *)Bstr;
					BYTE * dataptr = (BYTE *)binbuf;
					
					CString transname;
					transname.Format("%s_cmipstrans.dat", theApp.GetView()->GetDocument()->m_blh);
					
					CFile file;
					CFileException fe;
					
					CString fstr = theApp.m_SysTmpPath;
					fstr += transname;
					remove(fstr);
					if (file.Open( fstr , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
					{
						file.WriteHuge( dataptr , Bstr.GetLength());
						file.Close();
					}
					
					if( ((CMainFrame*)AfxGetMainWnd())->m_realtime )
					{
						((CMainFrame*)AfxGetMainWnd())->TerminateTimer();
					}
					
					theApp.GetView()->LoadImageFile(fstr, false);
					
					remove(fstr);
				}
			}
			
			g_dbconnection.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
			return;
		}
	}
}

void CMedImage::ShowSelectedItem()
{
	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
	if( m_pListThumbnail->GetSelectedCount() > 0 )
	{
		int nItem = -1;
		nItem = m_pListThumbnail->GetNextItem(nItem, LVNI_SELECTED);
		if( nItem >= 0 )
		{
			ShowImgItem(nItem);
		}
	}
	
	SetFocus();
}

void CMedImage::OnButtonPrinter() 
{
	m_PrinterSelMode = !m_PrinterSelMode;
	if(m_PrinterSelMode)
	{
		m_btPrinter.SetBitmap(CSize(32,32),IDB_BITMAP_PRINTER);
		m_ToolTip.AddTool(&m_btPrinter, "设置打印");
	}
	else
	{
		m_btPrinter.SetBitmap(CSize(32,32),IDB_BITMAP_VIDEO);
		m_ToolTip.AddTool(&m_btPrinter, "图像浏览");
	}
}

void CMedImage::OnFullscreen() 
{
	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
	if( m_pListThumbnail->GetSelectedCount() > 0 )
	{
		int nItem = -1;
		nItem = m_pListThumbnail->GetNextItem(nItem, LVNI_SELECTED);
		if( nItem >= 0 )
		{
			if(m_pListThumbnail->GetItemCount() > 0 && nItem < m_pListThumbnail->GetItemCount())
			{
				CString str = m_pListThumbnail->GetItemText(nItem, 0);
				str = str.Left(str.Find('('));

				theApp.ConnectDatabase();
				try
				{
					SAString cmdstr = "Select * from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno and serialno = :serialno";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
					g_dbcommand.Param("serialno").setAsLong()    = atoi(str);
					
					g_dbcommand.Execute();
					if(g_dbcommand.FetchNext())
					{
						SAString Bstr = g_dbcommand.Field("imgdataset").asBLob();
						if( !Bstr.IsEmpty() )
						{
							const void * binbuf = (const void *)Bstr;
							BYTE * dataptr = (BYTE *)binbuf;
							
							CString transname;
							transname.Format("%s_cmipstrans.dat", theApp.GetView()->GetDocument()->m_blh);
							
							CFile file;
							CFileException fe;
							
							CString fstr = theApp.m_SysTmpPath;
							fstr += transname;
							remove(fstr);
							if (file.Open( fstr , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
							{
								file.WriteHuge( dataptr , Bstr.GetLength());
								file.Close();
							}
							
							CFigure figure(fstr,
								           190,80,//window位置
								           640,480,TRUE);//显示大小
							figure.m_pFigureFrame->ShowWindow(SW_SHOWMAXIMIZED);
							
							remove(fstr);
						}
					}
					
					g_dbconnection.Commit();
				}
				catch(SAException &x)
				{
					try
					{
						g_dbconnection.Rollback();
					}
					catch(SAException &)
					{
					}
					AfxMessageBox((const char*)x.ErrText());
					return;
				}
			}
		}
	}
}

//DEL void CMedImage::OnPicQuicknote() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	
//DEL }

//DEL BOOL CMedImage::OnCommand(WPARAM wParam, LPARAM lParam) 
//DEL {
//DEL 	// TODO: Add your specialized code here and/or call the base class
//DEL 	AfxMessageBox("command");
//DEL 	CString s;
//DEL 	s.Format("%d",wParam);
//DEL 	AfxMessageBox(s);
//DEL 	if(wParam == ID_NOTE_1){
//DEL 		AfxMessageBox("note1");
//DEL 	}
//DEL 	return CXTResizeFormView::OnCommand(wParam, lParam);
//DEL }

//DEL BOOL CMedImage::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
//DEL {
//DEL 	// TODO: Add your specialized code here and/or call the base class
//DEL 	
//DEL 	if(nID == WM_COMMAND){
//DEL 		AfxMessageBox("command");
//DEL 	}
//DEL 	
//DEL 	return CXTResizeFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
//DEL }

BOOL CMedImage::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	int count = 0; CString s;
	
	

	while(count < index + 1){

		if(wParam == ID_NOTE_1 + count){

			CString str;
			CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
			if( m_pListThumbnail->GetSelectedCount() > 0 )
			{
				int nItem = -1;
				nItem = m_pListThumbnail->GetNextItem(nItem, LVNI_SELECTED);
				if( nItem >= 0 )
				{
					if(m_pListThumbnail->GetItemCount() > 0 && nItem < m_pListThumbnail->GetItemCount())
					{
						str = m_pListThumbnail->GetItemText(nItem, 0);
						str = str.Left(str.Find('('));
					}
				}
			}
			
			try
			{
				SAString cmdstr = "Update " + theApp.TABLE_IMAGE + " set imgnote = :imgnote where medicalno = :medicalno and serialno = :serialno";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
				g_dbcommand.Param("serialno").setAsLong()    = atoi(str);
				g_dbcommand.Param("imgnote").setAsString()   = m_quickInfo.GetAt(count);
				
				g_dbcommand.Execute();
				
				g_dbconnection.Commit();
			}
			catch(SAException &x)
			{
				try
				{
					g_dbconnection.Rollback();
				}
				catch(SAException &)
				{
				}
				AfxMessageBox((const char*)x.ErrText());
					}
		}
		count++;
	}
	return CXTResizeFormView::OnCommand(wParam, lParam);
}

void CMedImage::OnNoteQcmx() 
{
	// TODO: Add your command handler code here

	CString str;
	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
	if( m_pListThumbnail->GetSelectedCount() > 0 )
	{
		int nItem = -1;
		nItem = m_pListThumbnail->GetNextItem(nItem, LVNI_SELECTED);
		if( nItem >= 0 )
		{
			if(m_pListThumbnail->GetItemCount() > 0 && nItem < m_pListThumbnail->GetItemCount())
			{
				str = m_pListThumbnail->GetItemText(nItem, 0);
				str = str.Left(str.Find('('));
			}
		}
	}

	char enter[3];
	enter[0] = 0x0d; enter[1]= 0x0a; enter[2] = 0x00;
	
	CString info; info.Empty();
	try
	{
		SAString cmdstr = "Select * from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		while( g_dbcommand.FetchNext() )
		{
			CString str;
			str.Format("%3s  %20s  %2d", g_dbcommand.Field("xh").asString(), g_dbcommand.Field("qcbw").asString(),g_dbcommand.Field("cks").asLong());
			
			if(!info.IsEmpty()) info += enter;
			info += str;
		}

		cmdstr = "update " + theApp.TABLE_IMAGE + " set imgnote=:imgnote where medicalno=:medicalno and serialno=:serialno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("imgnote").setAsString() = info;
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Param("serialno").setAsString() = str;
		g_dbcommand.Execute();
		g_dbconnection.Commit();
	}
	catch(SAException &x)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
		AfxMessageBox((const char*)x.ErrText());
		return;
	}
}

void CMedImage::setDefault()
{
	m_PrinterSelMode = true;
	OnButtonPrinter();
}

void CMedImage::setNotDefault()
{
	m_PrinterSelMode = false;
	OnButtonPrinter();
}
