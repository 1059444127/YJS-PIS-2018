// FigureView.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "FigureView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CRect SizeRectWithConstantAspectRatio(CRect* rcScreen,  CSize  sizePicture,  BOOL   bCenter);

/////////////////////////////////////////////////////////////////////////////
// CFigureView
extern CPathologyApp theApp;

IMPLEMENT_DYNCREATE(CFigureView, CScrollView)

CFigureView::CFigureView()
{
	m_bStretch=FALSE;
	m_hDIB = NULL;

	m_blh.Empty();
}

CFigureView::~CFigureView()
{
	if (m_hDIB != NULL)
	{
		::GlobalFree((HGLOBAL) m_hDIB);
		m_hDIB = NULL;
	}
}


BEGIN_MESSAGE_MAP(CFigureView, CScrollView)
	//{{AFX_MSG_MAP(CFigureView)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFigureView drawing

void CFigureView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 10;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CFigureView::OnDraw(CDC* pDC)
{
	if(m_hDIB != NULL)
	{
		if(	m_bStretch)
		{
			CRect r;
			GetClientRect(r);
			SetScrollSizes(MM_TEXT,CSize(r.Width(),r.Height()));
			CRect ScreenWnd;
			ScreenWnd.SetRect( 0, 0, r.Width(), r.Height() );
			CSize sizePicture;
			sizePicture.cx = m_raster_xsize;
			sizePicture.cy = m_raster_ysize;
			ScreenWnd = SizeRectWithConstantAspectRatio(&ScreenWnd,  sizePicture,  TRUE);
			
			CRect rcDIB;
			rcDIB.top = rcDIB.left = 0;
			rcDIB.right  = m_raster_xsize;
			rcDIB.bottom = m_raster_ysize;
			CRect rcDest = ScreenWnd;
			::PaintDIB(pDC->m_hDC, &ScreenWnd, m_hDIB, &rcDIB, NULL);
		}
		else
		{
			SetScrollSizes(MM_TEXT,CSize(m_raster_xsize,m_raster_ysize));
			CRect rcDIB;
			rcDIB.top = rcDIB.left = 0;
			rcDIB.right  = m_raster_xsize;
			rcDIB.bottom = m_raster_ysize;
			CRect rcDest;
			rcDest = rcDIB;
			::PaintDIB(pDC->m_hDC, &rcDest, m_hDIB, &rcDIB, NULL);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFigureView diagnostics

#ifdef _DEBUG
void CFigureView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CFigureView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFigureView message handlers

int CFigureView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CFigureView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnMouseMove(nFlags, point);
}

void CFigureView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CScrollView::OnLButtonDown(nFlags, point);
}

void CFigureView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CScrollView::OnRButtonDown(nFlags, point);
}

bool CFigureView::TryNew(int count)
{
	if(m_blh.IsEmpty())  return false;

	int TotalNum = 0;
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_IMAGE + " where medicalno = :medicalno";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = m_blh;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			TotalNum = g_dbcommand.Field(1).asLong();
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

	int num = 0;
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_IMAGE + " where medicalno = :medicalno and serialno = :serialno";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = m_blh;
		g_dbcommand.Param("serialno").setAsLong()    = count;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			num = g_dbcommand.Field(1).asLong();
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
	
	if(num > 0)
	{
		BeginWaitCursor();
		
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_IMAGE + " where medicalno = :medicalno and serialno = :serialno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = m_blh;
			g_dbcommand.Param("serialno").setAsLong()    = count;
			
			g_dbcommand.Execute();
			if(g_dbcommand.FetchNext())
			{
				SAString Bstr = g_dbcommand.Field("imgdataset").asBLob();
				if( !Bstr.IsEmpty() )
				{
					const void * binbuf = (const void *)Bstr;
					BYTE * dataptr = (BYTE *)binbuf;
					
					CFile file;
					CFileException fe;
					
					CString fstr = theApp.m_SysTmpPath;
					fstr += "pic.dat";
					remove(fstr);
					if (file.Open( fstr , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
					{
						file.WriteHuge( dataptr , Bstr.GetLength());
						file.Close();
					}
					
					BOOL rt = LoadImage(fstr);

					if(rt)
						SetScrollSizes(MM_TEXT, CSize(m_raster_xsize, m_raster_ysize) );
					else
						SetScrollSizes(MM_TEXT, CSize(100, 100) );
					
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
			return true;
		}
		
		m_SerialNo = count;
		EndWaitCursor();
		RedrawWindow();
		
		CString Title;
		Title.Format("²¡ÀýÍ¼Ïñ - %s - %d/%d  [¶à·ùÍ¼ÏñÊ±£¬Çë°´¼üÅÌ¡û¡ú(»ò¡ü¡ý)¼üÇÐ»»]", m_blh, m_SerialNo, TotalNum);
		GetParent()->SetWindowText(Title);
		return true;
	}
	else
	{
		return false;
	}
}

BOOL CFigureView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
		UINT nChar = (UINT)pMsg->wParam;
		if(nChar == VK_SPACE)
		{
			m_bStretch = !m_bStretch;
			RedrawWindow();
		}

		if(!m_blh.IsEmpty())
		{
			if(nChar == VK_UP || nChar == VK_LEFT)
			{
				int count = m_SerialNo;
				while(count > 1)
				{
					count--;
					
					if(TryNew(count) == true)  break;
				}
			}
			
			if(nChar == VK_DOWN || nChar == VK_RIGHT)
			{
				int count = m_SerialNo;
				while(count < m_MaxSerialNo)
				{
					count++;
					
					if(TryNew(count) == true)  break;
				}
			}
		}
	}
	
	return CScrollView::PreTranslateMessage(pMsg);
}

BOOL CFigureView::LoadImage (CString fname)
{
	if (m_hDIB != NULL)
	{
		::GlobalFree((HGLOBAL) m_hDIB);
		m_hDIB = NULL;
	}

	GDALDataset * m_pGDALDataset = (GDALDataset *) GDALOpen ( fname, GA_ReadOnly );
	if( m_pGDALDataset == NULL)
	{
		return FALSE;
	}

	m_raster_xsize = m_pGDALDataset->GetRasterXSize();
	m_raster_ysize = m_pGDALDataset->GetRasterYSize();

	m_hDIB = ::CreateDIBFile( m_raster_xsize, m_raster_ysize , 24 );

	// ImportData
	LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	BYTE *   lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
	int LineByteWidth = WIDTHBYTES( m_raster_xsize * 24 );
	memset( lpDIBBits, 255, LineByteWidth * m_raster_ysize );
	
	BYTE *pRBits = lpDIBBits + 2;
	BYTE *pGBits = lpDIBBits + 1;
	BYTE *pBBits = lpDIBBits + 0;
	
	int BandNo[3];
	if( m_pGDALDataset->GetRasterCount() == 1)
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
	SAFE_DELETE(buf)
	
	::GlobalUnlock((HGLOBAL) m_hDIB);

	return TRUE;
}
