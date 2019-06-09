//==============================================================================================
//	FILE      :	PropertyBag.cpp
//	ABSTRACT  :	Property bag for camera settings 
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//
//	Copyright (c) 2005, Nikon Corp.  All rights reserved.
//==============================================================================================

#include "stdafx.h"
#include "PropertyBag.h"

//////////////////////////////////////////////////////////////////////
// Constructor/Destructor
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC( CPropertyBagItem, CObject );

//==============================================================================================
//	FUNCTION  :	CPropertyBag
//	ABSTRACT  :	Constructor
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
CPropertyBag::CPropertyBag()
	: m_nRefCount(0)
{

}

//==============================================================================================
//	FUNCTION  :	~CPropertyBag
//	ABSTRACT  :	Destructor
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	Releasees the contents of the property bag
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
CPropertyBag::~CPropertyBag()
{
   CPropertyBagItem* pItem;

   while( !m_listItem.IsEmpty() )
   {
	  pItem = m_listItem.RemoveHead();
	  delete pItem;
   }
}

//==============================================================================================
//	FUNCTION  :	AddRef
//	ABSTRACT  :	increments the reference count
//	PARAMS    :	None
//	RETURN    :	reference count
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
STDMETHODIMP_( ULONG ) CPropertyBag::AddRef()
{
   m_nRefCount++;

   return( m_nRefCount );
}

//==============================================================================================
//	FUNCTION  :	Release
//	ABSTRACT  :	decrements the reference count
//	PARAMS    :	None
//	RETURN    :	reference count
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
STDMETHODIMP_( ULONG ) CPropertyBag::Release()
{
   m_nRefCount--;

   if( m_nRefCount == 0 )
   {
	  delete this;
	  return( 0 );
   }
   else
   {
	  return( m_nRefCount );
   }
}
 
//==============================================================================================
//	FUNCTION  :	QueryInterface
//	ABSTRACT  :	Retrieves a pointer to the requested interface. 
//	PARAMS    :	iid: identifier of the desired interface
//			ppvObject: double pointer to return the requested interface pointer
//							
//	RETURN    :	S_OK
//				E_NOINTERFACE
//	NOTE      :	If the object does not support this interface, ppvObject is set to NULL. 
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
STDMETHODIMP CPropertyBag::QueryInterface( REFIID iid, void** ppInterface )
{
   ASSERT( ppInterface != NULL );
   *ppInterface = NULL;

   if( iid == IID_IUnknown )
   {
	  *ppInterface = (IUnknown*)this;
   }
   else if( iid == IID_IPropertyBag )
   {
	  *ppInterface = (IPropertyBag*)this;
   }

   if( *ppInterface != NULL )
   {
	  AddRef();
	  return( S_OK );
   }
   else
   {
	  return( E_NOINTERFACE );
   }
}

//==============================================================================================
//	FUNCTION  :	Read
//	ABSTRACT  :	Retrieves an item from the property bag.
//	PARAMS    :	pszPropName: property name of the property to retrieve
//			pvarValue: pointer to a variant that stores the property value
//			pErrorLog: pointer to the error log
//	RETURN    :	S_OK
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
STDMETHODIMP CPropertyBag::Read( LPCOLESTR pszPropName, VARIANT* pvarValue,
   IErrorLog* pErrorLog )
{
   USES_CONVERSION;

   POSITION posItem;
   CPropertyBagItem* pItem;
   CString strPropName;
   HRESULT hResult;
   EXCEPINFO error;

   if( pszPropName == NULL )
   {
	  return( E_POINTER );
   }
   if( pvarValue == NULL )
   {
	  return( E_POINTER );
   }

   memset( &error, 0, sizeof( error ) );

   strPropName = OLE2CT( pszPropName );

   posItem = m_listItem.GetHeadPosition();
   while( posItem != NULL )
   {
	  pItem = m_listItem.GetNext( posItem );
	  if( pItem->m_strName == strPropName )
	  {
		 // We found the property.
		 if( pvarValue->vt == VT_EMPTY )
		 {
			hResult = VariantCopy( pvarValue, &pItem->m_varValue );
			if( FAILED( hResult ) )
			{
			   if( pErrorLog != NULL )
			   {
				  error.scode = hResult;
				  pErrorLog->AddError( pszPropName, &error );
			   }
			   return( E_FAIL );
			}
		 }
		 else
		 {
			hResult = VariantChangeType( pvarValue, &pItem->m_varValue, 0,
			   pvarValue->vt );
			if( FAILED( hResult ) )
			{
			   if( pErrorLog != NULL )
			   {
				  error.scode = hResult;
				  pErrorLog->AddError( pszPropName, &error );
			   }
			   return( E_FAIL );
			}
		 }
		 return( S_OK );
	  }
   }

   return( E_INVALIDARG );
}

//==============================================================================================
//	FUNCTION  :	Write
//	ABSTRACT  :	Adds an item to the property bag
//	PARAMS    :	pszPropName: property name of the property to add
//			pvarValue: pointer to a variant that stores the property value
//	RETURN    :	S_OK
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
STDMETHODIMP CPropertyBag::Write( LPCOLESTR pszPropName, VARIANT* pvarValue )
{
	if( (pszPropName == NULL) || (pvarValue == NULL) )
	{
	  return( E_POINTER );
	}

   CPropertyBagItem* pItem;
   pItem = new CPropertyBagItem( pszPropName, pvarValue );
   if( pItem == NULL )
   {
	  return( E_OUTOFMEMORY );
   }

   m_listItem.AddTail( pItem );

	return( S_OK );
}
