// DCCache.h: interface for the CDCCache class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCCACHE_H__310F5F48_8A8A_45A0_BDB2_D71981F7C51C__INCLUDED_)
#define AFX_DCCACHE_H__310F5F48_8A8A_45A0_BDB2_D71981F7C51C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int CIMAGE_DC_CACHE_SIZE = 4;

class CDCCache
{
public:
	CDCCache() throw();
	~CDCCache() throw();
	
	HDC GetDC() const throw();
	void ReleaseDC( HDC ) const throw();
	
private:
	HDC m_ahDCs[CIMAGE_DC_CACHE_SIZE];
};

#endif // !defined(AFX_DCCACHE_H__310F5F48_8A8A_45A0_BDB2_D71981F7C51C__INCLUDED_)
