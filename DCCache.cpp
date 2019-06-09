// DCCache.cpp: implementation of the CDCCache class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCCache.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDCCache::CDCCache()
{
	int iDC;
	
	for( iDC = 0; iDC < CIMAGE_DC_CACHE_SIZE; iDC++ )
	{
		m_ahDCs[iDC] = NULL;
	}
}

CDCCache::~CDCCache()
{
	int iDC;
	
	for( iDC = 0; iDC < CIMAGE_DC_CACHE_SIZE; iDC++ )
	{
		if( m_ahDCs[iDC] != NULL )
		{
			::DeleteDC( m_ahDCs[iDC] );
		}
	}
}

HDC CDCCache::GetDC() const
{
	HDC hDC;
	
	/*for( int iDC = 0; iDC < CIMAGE_DC_CACHE_SIZE; iDC++ )
	{
		//hDC = static_cast< HDC >( InterlockedExchangePointer( reinterpret_cast< void** >(&m_ahDCs[iDC]), NULL ) );
		hDC = m_ahDCs[iDC];
		if( hDC != NULL )
		{
			return( hDC );
		}
	}*/
	
	hDC = ::CreateCompatibleDC( NULL );
	
	return( hDC );
}

void CDCCache::ReleaseDC( HDC hDC ) const
{
	/*for( int iDC = 0; iDC < CIMAGE_DC_CACHE_SIZE; iDC++ )
	{
		HDC hOldDC;
		
		//hOldDC = static_cast< HDC >( InterlockedExchangePointer( reinterpret_cast< void** >(&m_ahDCs[iDC]), hDC ) );
		hOldDC = m_ahDCs[iDC];
		if( hOldDC == NULL )
		{
			return;
		}
		else
		{
			hDC = hOldDC;
		}
	}*/
	if( hDC != NULL )
	{
		::DeleteDC( hDC );
	}
}
