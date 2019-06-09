// MovieShow.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "MovieShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMovieShow dialog

CMovieShow::CMovieShow(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CMovieShow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMovieShow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_nID = CMovieShow::IDD;
}

BOOL CMovieShow::Create()
{
	return CXTResizeDialog::Create(m_nID, m_pParent);
}

void CMovieShow::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMovieShow)
	DDX_Control(pDX, IDC_ACTIVEMOVIECONTROL1, m_ActMovie);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMovieShow, CXTResizeDialog)
	//{{AFX_MSG_MAP(CMovieShow)
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMovieShow message handlers

BOOL CMovieShow::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMovieShow::OnSize(UINT nType, int cx, int cy) 
{
	CXTResizeDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	ResizeMovieWnd();
}

void CMovieShow::ResizeMovieWnd()
{
	if(m_ActMovie.GetSafeHwnd())
	{
		CRect rt;
		this->GetClientRect(&rt);
		m_ActMovie.SetWindowPos(NULL,0, 0,  rt.Width() , rt.Height(), SWP_SHOWWINDOW);
	}
}

void CMovieShow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if(m_ActMovie.GetSafeHwnd())
	{
		CRect rt, rt1;
		this->GetClientRect(&rt);
		m_ActMovie.GetWindowRect(&rt1);

		if(rt.Width() != rt1.Width() || rt.Height() != rt1.Height())
		{
			ResizeMovieWnd();
		}
	}
	
	// Do not call CXTResizeDialog::OnPaint() for painting messages
}

void CMovieShow::CaptureImg(CString fname)
{
	CDC *pDC = m_ActMovie.GetDC();
	WndToBmp(pDC, fname);
	ReleaseDC(pDC);
}

BOOL CMovieShow::WndToBmp(CDC *pDC, CString szFile)
{
//*****This Function is wriiten by Shailesh Kanzariya....***********
//it will capture wnd and save into bmp file
	CString fname=szFile;
	CBitmap bmp,*pOldBmp;
	CRect rect;
	CWnd *pWnd;
	BOOL flg=0;
	CPalette pal;
	LOGPALETTE *pLp;

	if(pDC==NULL)
	{
		AfxMessageBox("DC Not Found");
		return FALSE;
	}
	pWnd=pDC->GetWindow();

	pWnd->GetClientRect(&rect);

	if(fname.IsEmpty())
		return FALSE;

	CDC memdc;
	
	memdc.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	pOldBmp=memdc.SelectObject(&bmp);
	memdc.BitBlt(0,0,rect.Width(),rect.Height(),pDC,0,0,SRCCOPY);

	if(pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
	{
		int nSize;
		nSize=sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * 256;
		pLp=(LOGPALETTE*) new BYTE[nSize];
		pLp->palVersion=0x300;
		pLp->palNumEntries=256;
		GetSystemPaletteEntries(pDC->m_hDC,0,256,pLp->palPalEntry);
		pal.CreatePalette(pLp);
		delete [] pLp;
	}
	
	/////////////////////////***
	memdc.SelectObject(pOldBmp);
	HANDLE hDIB=DDBToDIB(bmp,BI_RGB,&pal);
	if(hDIB==NULL)
		return FALSE;
//*************************************
	CFile m_file;
	if(!m_file.Open(fname,CFile::modeWrite | CFile::modeCreate,NULL))
		return FALSE;
	else
		flg=1;
	BITMAPFILEHEADER hdr;
	LPBITMAPINFOHEADER lpbi;
	
	lpbi=(LPBITMAPINFOHEADER ) hDIB;
	
	int nColors= 1 << lpbi->biBitCount;
	
	hdr.bfType= ((WORD) ('M' << 8) | 'B');
	hdr.bfSize=GlobalSize(hDIB) + sizeof(hdr);
	hdr.bfReserved1=0;
	hdr.bfReserved2=0;
	hdr.bfOffBits=(DWORD) (sizeof(hdr) + lpbi->biSize + nColors * sizeof(RGBQUAD));
	m_file.Write(&hdr,sizeof(hdr));
	m_file.Write(lpbi,GlobalSize(hDIB));
	m_file.Close();
//**************************************
	CFile  m_tempFile;
	BYTE dummy=0;//14
	BYTE pBuf[14];

	BOOL fres=m_tempFile.Open(fname,CFile::modeReadWrite | CFile::typeBinary);

	if(fres==0)
		return FALSE;

	UINT tt=m_tempFile.Read(pBuf,14);
	pBuf[13]=dummy;//will replace from 04 to 00
	m_tempFile.SeekToBegin();
	m_tempFile.Write(pBuf,14);

	m_tempFile.Close();

	return flg;
//*****This Function is wriiten by Shailesh Kanzariya....***********
//it will capture wnd and save into bmp file

}

HANDLE CMovieShow::DDBToDIB(CBitmap &bitmap, DWORD dwCompression, CPalette *pPal)
{

	BITMAP			bm;
	BITMAPINFOHEADER	bi;
	LPBITMAPINFOHEADER 	lpbi;
	DWORD			dwLen;
	HANDLE			hDIB;
	HANDLE			handle;
	HDC 			hDC;
	HPALETTE		hPal;


	ASSERT( bitmap.GetSafeHandle() );

	// The function has no arg for bitfields
	if( dwCompression == BI_BITFIELDS )
		return NULL;

	// If a palette has not been supplied use defaul palette
	hPal = (HPALETTE) pPal->GetSafeHandle();
	if (hPal==NULL)
		hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

	// Get bitmap information
	bitmap.GetObject(sizeof(bm),(LPSTR)&bm);

	// Initialize the bitmapinfoheader
	bi.biSize		= sizeof(BITMAPINFOHEADER);
	bi.biWidth		= bm.bmWidth;
	bi.biHeight 		= bm.bmHeight;
	bi.biPlanes 		= 1;
	bi.biBitCount		= bm.bmPlanes * bm.bmBitsPixel;			//bm.bmPlanes * bm.bmBitsPixel;
	bi.biCompression	= dwCompression;
	bi.biSizeImage		= 0;
	bi.biXPelsPerMeter	= 0;
	bi.biYPelsPerMeter	= 0;
	bi.biClrUsed		= 0;
	bi.biClrImportant	= 0;

	// Compute the size of the  infoheader and the color table
	int nColors = (1 << bi.biBitCount);
	if( nColors > 256 ) 
		nColors = 0;
	dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

	// We need a device context to get the DIB from
	hDC = ::GetDC(NULL);
	hPal = SelectPalette(hDC,hPal,FALSE);
	RealizePalette(hDC);

	// Allocate enough memory to hold bitmapinfoheader and color table
	hDIB = GlobalAlloc(GMEM_FIXED,dwLen);

	if (!hDIB){
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	*lpbi = bi;

	// Call GetDIBits with a NULL lpBits param, so the device driver 
	// will calculate the biSizeImage field 
	GetDIBits(hDC, (HBITMAP)bitmap.GetSafeHandle(), 0L, (DWORD)bi.biHeight,
			(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;

	// If the driver did not fill in the biSizeImage field, then compute it
	// Each scan line of the image is aligned on a DWORD (32bit) boundary
	if (bi.biSizeImage == 0){
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) 
						* bi.biHeight;

		// If a compression scheme is used the result may infact be larger
		// Increase the size to account for this.
		if (dwCompression != BI_RGB)
			bi.biSizeImage = (bi.biSizeImage * 3) / 2;
	}

	// Realloc the buffer so that it can hold all the bits
	dwLen += bi.biSizeImage;
	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
		hDIB = handle;
	else{
		GlobalFree(hDIB);

		// Reselect the original palette
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	// Get the bitmap bits
	lpbi = (LPBITMAPINFOHEADER)hDIB;

	// FINALLY get the DIB
	BOOL bGotBits = GetDIBits( hDC, (HBITMAP)bitmap.GetSafeHandle(),
				0L,				// Start scan line
				(DWORD)bi.biHeight,		// # of scan lines
				(LPBYTE)lpbi 			// address for bitmap bits
				+ (bi.biSize + nColors * sizeof(RGBQUAD)),
				(LPBITMAPINFO)lpbi,		// address of bitmapinfo
				(DWORD)DIB_RGB_COLORS);		// Use RGB for color table

	if( !bGotBits )
	{
		GlobalFree(hDIB);
		
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	SelectPalette(hDC,hPal,FALSE);
	::ReleaseDC(NULL,hDC);

	return hDIB;

}
