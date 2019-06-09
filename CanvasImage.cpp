// CanvasImage.cpp: implementation of the CCanvasImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CanvasImage.h"
#include "WINBASE.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCanvasImage::CCanvasImage() :
		m_hBitmap( NULL ),
		m_pBits( NULL ),
		m_hDC( NULL ),
		m_nDCRefCount( 0 ),
		m_hOldBitmap( NULL ),
		m_nWidth( 0 ),
		m_nHeight( 0 ),
		m_nPitch( 0 ),
		m_nBPP( 0 ),
		m_iTransparentColor( -1 ),
		m_bHasAlphaChannel( false ),
		m_bIsDIBSection( false )
{
}

CCanvasImage::~CCanvasImage()
{
	Destroy();
}

CCanvasImage::operator HBITMAP() const
{
	return( m_hBitmap );
}

void CCanvasImage::Attach( HBITMAP hBitmap, DIBOrientation eOrientation ) throw()
{
	ASSERT( m_hBitmap == NULL );
	ASSERT( hBitmap != NULL );
	
	m_hBitmap = hBitmap;
	
	UpdateBitmapInfo( eOrientation );
}

BOOL CCanvasImage::BitBlt( HDC hDestDC, int xDest, int yDest, DWORD dwROP ) const
{
	return( BitBlt( hDestDC, xDest, yDest, m_nWidth, m_nHeight, 0, 0, dwROP ) );
}

BOOL CCanvasImage::BitBlt( HDC hDestDC, const POINT& pointDest, DWORD dwROP ) const
{
	return( BitBlt( hDestDC, pointDest.x, pointDest.y, m_nWidth, m_nHeight,
		0, 0, dwROP ) );
}

BOOL CCanvasImage::BitBlt( HDC hDestDC, int xDest, int yDest, int nDestWidth, 
						  int nDestHeight, int xSrc, int ySrc, DWORD dwROP ) const
{
	BOOL bResult;
	
	ASSERT( m_hBitmap != NULL );
	ASSERT( hDestDC != NULL );
	
	GetDC();
	
	bResult = ::BitBlt( hDestDC, xDest, yDest, nDestWidth, nDestHeight, m_hDC, 
		xSrc, ySrc, dwROP );
	
	ReleaseDC();
	
	return( bResult );
}

BOOL CCanvasImage::BitBlt( HDC hDestDC, const RECT& rectDest, 
					const POINT& pointSrc, DWORD dwROP ) const
{
	return( BitBlt( hDestDC, rectDest.left, rectDest.top, rectDest.right-
		rectDest.left, rectDest.bottom-rectDest.top, pointSrc.x, pointSrc.y, 
		dwROP ) );
}

BOOL CCanvasImage::Create( int nWidth, int nHeight, int nBPP, DWORD dwFlags ) throw()
{
	if(nWidth <=  0) nWidth = 1;
	if(nHeight <= 0) nHeight = 1;
	
	return( CreateEx( nWidth, nHeight, nBPP, BI_RGB, NULL, dwFlags ) );
}

BOOL CCanvasImage::CreateEx( int nWidth, int nHeight, int nBPP, DWORD eCompression, 
					  const DWORD* pdwBitfields, DWORD dwFlags ) throw()
{
	LPBITMAPINFO pbmi;
	HBITMAP hBitmap;
	
	ASSERT( (eCompression == BI_RGB) || (eCompression == BI_BITFIELDS) );
	/*if( dwFlags&createAlphaChannel )
	{
		ASSERT( (nBPP == 32) && (eCompression == BI_RGB) );
	}*/
	
	pbmi = LPBITMAPINFO( _alloca( sizeof( BITMAPINFO )+256*sizeof( 
		RGBQUAD ) ) );
	
	memset( &pbmi->bmiHeader, 0, sizeof( pbmi->bmiHeader ) );
	pbmi->bmiHeader.biSize = sizeof( pbmi->bmiHeader );
	pbmi->bmiHeader.biWidth = nWidth;
	pbmi->bmiHeader.biHeight = nHeight;
	pbmi->bmiHeader.biPlanes = 1;
	pbmi->bmiHeader.biBitCount = USHORT( nBPP );
	pbmi->bmiHeader.biCompression = eCompression;
	if( nBPP <= 8 )
	{
		ASSERT( eCompression == BI_RGB );
		memset( pbmi->bmiColors, 0, 256*sizeof( RGBQUAD ) );
	}
	else 
	{
		if( eCompression == BI_BITFIELDS )
		{
			ASSERT( pdwBitfields != NULL );
			memcpy( pbmi->bmiColors, pdwBitfields, 3*sizeof( DWORD ) );
		}
	}
	
	hBitmap = ::CreateDIBSection( NULL, pbmi, DIB_RGB_COLORS, &m_pBits, NULL,
		0 );
	if( hBitmap == NULL )
	{
		return( FALSE );
	}
	
	Attach( hBitmap, (nHeight < 0) ? DIBOR_TOPDOWN : DIBOR_BOTTOMUP );
	
	/*if( dwFlags&createAlphaChannel )
	{
		m_bHasAlphaChannel = true;
	}*/
	
	return( TRUE );
}

void CCanvasImage::Destroy()
{
	HBITMAP hBitmap;
	
	if( m_hBitmap != NULL )
	{
		hBitmap = Detach();
		::DeleteObject( hBitmap );
	}
}

HBITMAP CCanvasImage::Detach()
{
	HBITMAP hBitmap;
	
	ASSERT( m_hBitmap != NULL );
	ASSERT( m_hDC == NULL );
	
	hBitmap = m_hBitmap;
	m_hBitmap = NULL;
	m_pBits = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nBPP = 0;
	m_nPitch = 0;
	m_iTransparentColor = -1;
	m_bHasAlphaChannel = false;
	m_bIsDIBSection = false;
	
	return( hBitmap );
}


const void* CCanvasImage::GetBits() const
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( IsDIBSection() );
	
	return( m_pBits );
}

void* CCanvasImage::GetBits()
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( IsDIBSection() );
	
	return( m_pBits );
}

int CCanvasImage::GetBPP() const
{
	ASSERT( m_hBitmap != NULL );
	
	return( m_nBPP );
}

void CCanvasImage::GetColorTable( UINT iFirstColor, UINT nColors, 
						   RGBQUAD* prgbColors ) const
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( m_pBits != NULL );
	ASSERT( IsIndexed() );
	
	GetDC();
	
	::GetDIBColorTable( m_hDC, iFirstColor, nColors, prgbColors );
	
	ReleaseDC();
}

HDC CCanvasImage::GetDC() const
{
	ASSERT( m_hBitmap != NULL );
	
	m_nDCRefCount++;
	if( m_hDC == NULL )
	{
		m_hDC = s_cache.GetDC();
		m_hOldBitmap = HBITMAP( ::SelectObject( m_hDC, m_hBitmap ) );
	}
	
	return( m_hDC );
}


int CCanvasImage::GetHeight() const
{
	ASSERT( m_hBitmap != NULL );
	
	return( m_nHeight );
}

int CCanvasImage::GetMaxColorTableEntries() const
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( IsDIBSection() );
	
	if( IsIndexed() )
	{
		return( 1<<m_nBPP );
	}
	else
	{
		return( 0 );
	}
}

int CCanvasImage::GetPitch() const
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( IsDIBSection() );
	
	return( m_nPitch );
}

COLORREF CCanvasImage::GetPixel( int x, int y ) const
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( (x >= 0) && (x < m_nWidth) );
	ASSERT( (y >= 0) && (y < m_nHeight) );
	
	GetDC();
	
	COLORREF clr = ::GetPixel( m_hDC, x, y );
	
	ReleaseDC();
	
	return( clr );
}

const void* CCanvasImage::GetPixelAddress( int x, int y ) const
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( IsDIBSection() );
	ASSERT( (x >= 0) && (x < m_nWidth) );
	ASSERT( (y >= 0) && (y < m_nHeight) );
	
	return( LPBYTE( m_pBits )+(y*m_nPitch)+((x*m_nBPP)/8) );
}

void* CCanvasImage::GetPixelAddress( int x, int y )
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( IsDIBSection() );
	ASSERT( (x >= 0) && (x < m_nWidth) );
	ASSERT( (y >= 0) && (y < m_nHeight) );
	
	return( LPBYTE( m_pBits )+(y*m_nPitch)+((x*m_nBPP)/8) );
}


int CCanvasImage::GetWidth() const
{
	ASSERT( m_hBitmap != NULL );
	
	return( m_nWidth );
}

bool CCanvasImage::IsDIBSection() const
{
	return( m_bIsDIBSection );
}

bool CCanvasImage::IsIndexed() const
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( IsDIBSection() );
	
	return( m_nBPP <= 8 );
}

bool CCanvasImage::IsNull() const throw()
{
	return( m_hBitmap == NULL );
}


BOOL CCanvasImage::MaskBlt( HDC hDestDC, int xDest, int yDest, int nWidth, 
					 int nHeight, int xSrc, int ySrc, HBITMAP hbmMask, int xMask, int yMask,
					 DWORD dwROP ) const
{
	BOOL bResult;
	
	ASSERT( m_hBitmap != NULL );
	ASSERT( hDestDC != NULL );
	
	GetDC();
	
	bResult = ::MaskBlt( hDestDC, xDest, yDest, nWidth, nHeight, m_hDC, xSrc, 
		ySrc, hbmMask, xMask, yMask, dwROP );
	
	ReleaseDC();
	
	return( bResult );
}

BOOL CCanvasImage::MaskBlt( HDC hDestDC, const RECT& rectDest, 
					 const POINT& pointSrc, HBITMAP hbmMask, const POINT& pointMask, 
					 DWORD dwROP ) const
{
	return( MaskBlt( hDestDC, rectDest.left, rectDest.top, rectDest.right-
		rectDest.left, rectDest.bottom-rectDest.top, pointSrc.x, pointSrc.y, 
		hbmMask, pointMask.x, pointMask.y, dwROP ) );
}

BOOL CCanvasImage::MaskBlt( HDC hDestDC, int xDest, int yDest, HBITMAP hbmMask, 
					 DWORD dwROP ) const
{
	return( MaskBlt( hDestDC, xDest, yDest, m_nWidth, m_nHeight, 0, 0, hbmMask, 
		0, 0, dwROP ) );
}

BOOL CCanvasImage::MaskBlt( HDC hDestDC, const POINT& pointDest, HBITMAP hbmMask,
					 DWORD dwROP ) const
{
	return( MaskBlt( hDestDC, pointDest.x, pointDest.y, m_nWidth, m_nHeight, 0, 
		0, hbmMask, 0, 0, dwROP ) );
}

BOOL CCanvasImage::PlgBlt( HDC hDestDC, const POINT* pPoints, int xSrc, 
					int ySrc, int nSrcWidth, int nSrcHeight, HBITMAP hbmMask, int xMask, 
					int yMask ) const
{
	BOOL bResult;
	
	ASSERT( m_hBitmap != NULL );
	ASSERT( hDestDC != NULL );
	
	GetDC();
	
	bResult = ::PlgBlt( hDestDC, pPoints, m_hDC, xSrc, ySrc, nSrcWidth, 
		nSrcHeight, hbmMask, xMask, yMask );
	
	ReleaseDC();
	
	return( bResult );
}

BOOL CCanvasImage::PlgBlt( HDC hDestDC, const POINT* pPoints, 
					const RECT& rectSrc, HBITMAP hbmMask, const POINT& pointMask ) const
{
	return( PlgBlt( hDestDC, pPoints, rectSrc.left, rectSrc.top, rectSrc.right-
		rectSrc.left, rectSrc.bottom-rectSrc.top, hbmMask, pointMask.x, 
		pointMask.y ) );
}

BOOL CCanvasImage::PlgBlt( HDC hDestDC, const POINT* pPoints, 
					HBITMAP hbmMask ) const
{
	return( PlgBlt( hDestDC, pPoints, 0, 0, m_nWidth, m_nHeight, hbmMask, 0, 
		0 ) );
}

void CCanvasImage::ReleaseDC() const
{
	HBITMAP hBitmap;
	
	ASSERT( m_hDC != NULL );
	
	m_nDCRefCount--;
	if( m_nDCRefCount == 0 )
	{
		hBitmap = HBITMAP( ::SelectObject( m_hDC, m_hOldBitmap ) );
		ASSERT( hBitmap == m_hBitmap );
		s_cache.ReleaseDC( m_hDC );
		m_hDC = NULL;
	}
}


void CCanvasImage::SetColorTable( UINT iFirstColor, UINT nColors, 
						   const RGBQUAD* prgbColors )
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( IsDIBSection() );
	ASSERT( IsIndexed() );
	
	GetDC();
	
	::SetDIBColorTable( m_hDC, iFirstColor, nColors, prgbColors );
	
	ReleaseDC();
}

void CCanvasImage::SetPixel( int x, int y, COLORREF color )
{
	ASSERT( m_hBitmap != NULL );
	ASSERT( (x >= 0) && (x < m_nWidth) );
	ASSERT( (y >= 0) && (y < m_nHeight) );
	
	GetDC();
	
	::SetPixel( m_hDC, x, y, color );
	
	ReleaseDC();
}

void CCanvasImage::SetPixelIndexed( int x, int y, int iIndex )
{
	SetPixel( x, y, PALETTEINDEX( iIndex ) );
}

void CCanvasImage::SetPixelRGB( int x, int y, BYTE r, BYTE g, BYTE b )
{
	SetPixel( x, y, RGB( r, g, b ) );
}

LONG CCanvasImage::SetTransparentColor( LONG iTransparentColor )
{
	LONG iOldTransparentColor;
	
	ASSERT( m_hBitmap != NULL );
	ASSERT( (m_nBPP == 4) || (m_nBPP == 8) );
	ASSERT( iTransparentColor < GetMaxColorTableEntries() );
	ASSERT( iTransparentColor >= -1 );
	
	iOldTransparentColor = m_iTransparentColor;
	m_iTransparentColor = iTransparentColor;
	
	return( iOldTransparentColor );
}

BOOL CCanvasImage::StretchBlt( HDC hDestDC, int xDest, int yDest, 
						int nDestWidth, int nDestHeight, DWORD dwROP ) const
{
	return( StretchBlt( hDestDC, xDest, yDest, nDestWidth, nDestHeight, 0, 0, 
		m_nWidth, m_nHeight, dwROP ) );
}

BOOL CCanvasImage::StretchBlt( HDC hDestDC, const RECT& rectDest, 
						DWORD dwROP ) const
{
	return( StretchBlt( hDestDC, rectDest.left, rectDest.top, rectDest.right-
		rectDest.left, rectDest.bottom-rectDest.top, 0, 0, m_nWidth, m_nHeight, 
		dwROP ) );
}

BOOL CCanvasImage::StretchBlt( HDC hDestDC, int xDest, int yDest, 
						int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, 
						int nSrcHeight, DWORD dwROP ) const
{
	BOOL bResult;
	
	if(m_hBitmap== NULL||hDestDC == NULL )
		return FALSE;
	
	GetDC();
	
	bResult = ::StretchBlt( hDestDC, xDest, yDest, nDestWidth, nDestHeight, m_hDC,
		xSrc, ySrc, nSrcWidth, nSrcHeight, dwROP );
	
	ReleaseDC();
	
	return( bResult );
}

BOOL CCanvasImage::StretchBlt( HDC hDestDC, const RECT& rectDest, 
						const RECT& rectSrc, DWORD dwROP ) const
{
	return( StretchBlt( hDestDC, rectDest.left, rectDest.top, rectDest.right-
		rectDest.left, rectDest.bottom-rectDest.top, rectSrc.left, rectSrc.top, 
		rectSrc.right-rectSrc.left, rectSrc.bottom-rectSrc.top, dwROP ) );
}


void CCanvasImage::UpdateBitmapInfo( DIBOrientation eOrientation )
{
	DIBSECTION dibsection;
	int nBytes;
	
	nBytes = ::GetObject( m_hBitmap, sizeof( DIBSECTION ), &dibsection );
	if( nBytes == sizeof( DIBSECTION ) )
	{
		m_bIsDIBSection = true;
		m_nWidth = dibsection.dsBmih.biWidth;
		m_nHeight = abs( dibsection.dsBmih.biHeight );
		m_nBPP = dibsection.dsBmih.biBitCount;
		m_nPitch = ComputePitch( m_nWidth, m_nBPP );
		m_pBits = dibsection.dsBm.bmBits;
		if( eOrientation == DIBOR_DEFAULT )
		{
			eOrientation = (dibsection.dsBmih.biHeight > 0) ? DIBOR_BOTTOMUP : DIBOR_TOPDOWN;
		}
		if( eOrientation == DIBOR_BOTTOMUP )
		{
			m_pBits = LPBYTE( m_pBits )+((m_nHeight-1)*m_nPitch);
			m_nPitch = -m_nPitch;
		}
	}
	else
	{
		// Non-DIBSection
		ASSERT( nBytes == sizeof( BITMAP ) );
		m_bIsDIBSection = false;
		m_nWidth = dibsection.dsBm.bmWidth;
		m_nHeight = dibsection.dsBm.bmHeight;
		m_nBPP = dibsection.dsBm.bmBitsPixel;
		m_nPitch = 0;
		m_pBits = 0;
	}
	m_iTransparentColor = -1;
	m_bHasAlphaChannel = false;
}

BYTE* CCanvasImage::GetBitmapBits()
{
	CDC* pDC =CDC::FromHandle(GetDC());
	CBitmap* pBitmap =pDC->GetCurrentBitmap();
	BITMAP stBitmap;
	pBitmap->GetBitmap(&stBitmap);

	m_widthbyte =((stBitmap.bmWidth * 24 +31) & ~31) /8; 
	BYTE* pBuffer =new BYTE[stBitmap.bmHeight * m_widthbyte];

	PBITMAPINFO pInfo =CreateBitmapInfoStruct(HBITMAP(*pBitmap));
	pInfo->bmiHeader.biHeight*=-1;
	GetDIBits(pDC->GetSafeHdc(),HBITMAP(*pBitmap),
		0,(WORD)stBitmap.bmHeight,
		pBuffer,pInfo, 
        DIB_RGB_COLORS);

	//PBITMAPINFO pInfo =CreateBitmapInfoStruct(HBITMAP(*pBitmap));
	//CreateBMPFile("e:\\testimg.bmp",pInfo,HBITMAP(*pBitmap),pDC->GetSafeHdc(),pBuffer);

	ReleaseDC();
	return pBuffer;
}

PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp)
{ 
    BITMAP bmp; 
    PBITMAPINFO pbmi; 
    WORD    cClrBits; 

    // Retrieve the bitmap color format, width, and height. 
    if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp)) 
        TRACE("CetBitmap failed");

    // Convert the color format to a count of bits. 
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
    if (cClrBits == 1) 
        cClrBits = 1; 
    else if (cClrBits <= 4) 
        cClrBits = 4; 
    else if (cClrBits <= 8) 
        cClrBits = 8; 
    else if (cClrBits <= 16) 
        cClrBits = 16; 
    else if (cClrBits <= 24) 
        cClrBits = 24; 
    else cClrBits = 32; 

    // Allocate memory for the BITMAPINFO structure. (This structure 
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD 
    // data structures.) 

     if (cClrBits != 24) 
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
                    sizeof(BITMAPINFOHEADER) + 
                    sizeof(RGBQUAD) * (1<< cClrBits)); 

     // There is no RGBQUAD array for the 24-bit-per-pixel format. 

     else 
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
                    sizeof(BITMAPINFOHEADER)); 

    // Initialize the fields in the BITMAPINFO structure. 

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
    pbmi->bmiHeader.biWidth = bmp.bmWidth; 
    pbmi->bmiHeader.biHeight = bmp.bmHeight; 
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
    if (cClrBits < 24) 
        pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 

    // If the bitmap is not compressed, set the BI_RGB flag. 
    pbmi->bmiHeader.biCompression = BI_RGB; 

    // Compute the number of bytes in the array of color 
    // indices and store the result in biSizeImage. 
    // For Windows NT, the width must be DWORD aligned unless 
    // the bitmap is RLE compressed. This example shows this. 
    // For Windows 95/98/Me, the width must be WORD aligned unless the 
    // bitmap is RLE compressed.
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8
                                  * pbmi->bmiHeader.biHeight; 
    // Set biClrImportant to 0, indicating that all of the 
    // device colors are important. 
     pbmi->bmiHeader.biClrImportant = 0; 
     return pbmi; 
} 


void CreateBMPFile(LPTSTR pszFile,PBITMAPINFO pbi,HBITMAP hBMP,HDC hDC) 
 { 
     HANDLE hf;                 // file handle 
    BITMAPFILEHEADER hdr;       // bitmap file-header 
    PBITMAPINFOHEADER pbih;     // bitmap info-header 
    LPBYTE lpBits;              // memory pointer 
    DWORD dwTotal;              // total count of bytes 
    DWORD cb;                   // incremental count of bytes 
    BYTE *hp;                   // byte pointer 
    DWORD dwTmp; 

    pbih = (PBITMAPINFOHEADER) pbi; 
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    if (!lpBits) 
         TRACE("");

    // Retrieve the color table (RGBQUAD array) and the bits 
    // (array of palette indices) from the DIB. 
    if (!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi, 
        DIB_RGB_COLORS)) 
    {
        TRACE("");
    }

    // Create the .BMP file. 
    hf = CreateFile(pszFile, 
                   GENERIC_READ | GENERIC_WRITE, 
                   (DWORD) 0, 
                    NULL, 
                   CREATE_ALWAYS, 
                   FILE_ATTRIBUTE_NORMAL, 
                   (HANDLE) NULL); 
    if (hf == INVALID_HANDLE_VALUE) 
        TRACE("");
    hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
    // Compute the size of the entire file. 
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + 
                 pbih->biSize + pbih->biClrUsed 
                 * sizeof(RGBQUAD) + pbih->biSizeImage); 
    hdr.bfReserved1 = 0; 
    hdr.bfReserved2 = 0; 

    // Compute the offset to the array of color indices. 
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + 
                    pbih->biSize + pbih->biClrUsed 
                    * sizeof (RGBQUAD); 

    // Copy the BITMAPFILEHEADER into the .BMP file. 
    if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), 
        (LPDWORD) &dwTmp,  NULL)) 
    {
       TRACE(""); 
    }

    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file. 
    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) 
                  + pbih->biClrUsed * sizeof (RGBQUAD), 
                  (LPDWORD) &dwTmp, ( NULL)) )
        TRACE(""); 

    // Copy the array of color indices into the .BMP file. 
    dwTotal = cb = pbih->biSizeImage; 
    hp = lpBits; 
    if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL)) 
           TRACE("");

    // Close the .BMP file. 
     if (!CloseHandle(hf)) 
           TRACE("");

    // Free memory. 
    GlobalFree((HGLOBAL)lpBits);
}
