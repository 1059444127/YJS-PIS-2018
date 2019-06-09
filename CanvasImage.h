// CanvasImage.h: interface for the CCanvasImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CANVASIMAGE_H__BB862999_01EF_4357_B973_235BB4C50353__INCLUDED_)
#define AFX_CANVASIMAGE_H__BB862999_01EF_4357_B973_235BB4C50353__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "DCCache.h"
#include <malloc.h>


PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp);
void CreateBMPFile(LPTSTR pszFile,PBITMAPINFO pbi,HBITMAP hBMP,HDC hDC);

class CCanvasImage
{

private:

public:

	enum DIBOrientation
	{
		DIBOR_DEFAULT,
		DIBOR_TOPDOWN,
		DIBOR_BOTTOMUP
	};

public:
	//constructors
	CCanvasImage();
	virtual ~CCanvasImage();

	operator HBITMAP() const throw();

	void Attach( HBITMAP hBitmap, DIBOrientation eOrientation = DIBOR_DEFAULT ) throw();
	BOOL BitBlt( HDC hDestDC, int xDest, int yDest, DWORD dwROP = SRCCOPY ) const throw();
	BOOL BitBlt( HDC hDestDC, const POINT& pointDest, DWORD dwROP = SRCCOPY ) const throw();

	BOOL BitBlt( HDC hDestDC, int xDest, int yDest, int nDestWidth, 
		         int nDestHeight, int xSrc, int ySrc, 
				 DWORD dwROP = SRCCOPY ) const throw();

	BOOL BitBlt( HDC hDestDC, const RECT& rectDest, const POINT& pointSrc, 
		         DWORD dwROP = SRCCOPY ) const throw();

	BOOL Create( int nWidth, int nHeight, int nBPP, DWORD dwFlags = 0 ) throw();
	BOOL CreateEx( int nWidth, int nHeight, int nBPP, DWORD eCompression, 
		           const DWORD* pdwBitmasks = NULL, DWORD dwFlags = 0 ) throw();

	void	Destroy() throw();
	HBITMAP Detach() throw();

	const void* GetBits() const throw();
	void*       GetBits() throw();
	int			GetBPP() const throw();
	void		GetColorTable( UINT iFirstColor, UINT nColors, RGBQUAD* prgbColors ) const throw();
	HDC			GetDC() const throw();

	int			GetHeight() const throw();
	int			GetMaxColorTableEntries() const throw();
	int			GetPitch() const throw();
	const void* GetPixelAddress( int x, int y ) const throw();
	void*		GetPixelAddress( int x, int y ) throw();
	COLORREF	GetPixel( int x, int y ) const throw();
	int			GetWidth() const throw();
	bool		IsDIBSection() const throw();
	bool		IsIndexed() const throw();
	bool		IsNull() const throw();

	BOOL		MaskBlt( HDC hDestDC, int xDest, int yDest, int nDestWidth, 
						int nDestHeight, int xSrc, int ySrc, HBITMAP hbmMask, int xMask, 
						int yMask, DWORD dwROP = SRCCOPY ) const throw();

	BOOL MaskBlt( HDC hDestDC, const RECT& rectDest, const POINT& pointSrc, 
				HBITMAP hbmMask, const POINT& pointMask, DWORD dwROP = SRCCOPY ) const throw();
	BOOL MaskBlt( HDC hDestDC, int xDest, int yDest, HBITMAP hbmMask, 
				DWORD dwROP = SRCCOPY ) const throw();
	BOOL MaskBlt( HDC hDestDC, const POINT& pointDest, HBITMAP hbmMask, 
				DWORD dwROP = SRCCOPY ) const throw();
	BOOL PlgBlt( HDC hDestDC, const POINT* pPoints, HBITMAP hbmMask = NULL ) const throw();
	BOOL PlgBlt( HDC hDestDC, const POINT* pPoints, int xSrc, int ySrc, 
				int nSrcWidth, int nSrcHeight, HBITMAP hbmMask = NULL, int xMask = 0, 
				int yMask = 0 ) const throw();
	BOOL PlgBlt( HDC hDestDC, const POINT* pPoints, const RECT& rectSrc, 
				HBITMAP hbmMask = NULL, const POINT& pointMask = CPoint( 0, 0 ) ) const throw();

	void ReleaseDC() const throw();

	void SetColorTable( UINT iFirstColor, UINT nColors, 
						const RGBQUAD* prgbColors ) throw();
	void SetPixel( int x, int y, COLORREF color ) throw();
	void SetPixelIndexed( int x, int y, int iIndex ) throw();
	void SetPixelRGB( int x, int y, BYTE r, BYTE g, BYTE b ) throw();
	LONG SetTransparentColor( LONG iTransparentColor ) throw();
	BOOL StretchBlt( HDC hDestDC, int xDest, int yDest, int nDestWidth, 
					int nDestHeight, DWORD dwROP = SRCCOPY ) const throw();
	BOOL StretchBlt( HDC hDestDC, const RECT& rectDest, DWORD dwROP = SRCCOPY ) const throw();
	BOOL StretchBlt( HDC hDestDC, int xDest, int yDest, int nDestWidth, 
					int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight,
					DWORD dwROP = SRCCOPY ) const throw();
	BOOL StretchBlt( HDC hDestDC, const RECT& rectDest, const RECT& rectSrc,
					DWORD dwROP = SRCCOPY ) const throw();

	BYTE* GetBitmapBits();
		
public:
	HBITMAP m_hBitmap;
	void* m_pBits;
	int m_nWidth;
	int m_nHeight;
	int m_nPitch;
	int m_nBPP;
	bool m_bIsDIBSection;
	bool m_bHasAlphaChannel;
	LONG m_iTransparentColor;
	int m_widthbyte;


private:
	void UpdateBitmapInfo( DIBOrientation eOrientation );
	static int ComputePitch( int nWidth, int nBPP )	{return( (((nWidth*nBPP)+31)/32)*4 );}
	
private:
	mutable HDC m_hDC;
	mutable int m_nDCRefCount;
	mutable HBITMAP m_hOldBitmap;

public:
	CDCCache s_cache;

};




#endif // !defined(AFX_CANVASIMAGE_H__BB862999_01EF_4357_B973_235BB4C50353__INCLUDED_)
