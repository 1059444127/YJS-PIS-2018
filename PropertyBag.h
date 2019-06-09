//==============================================================================================
//	FILE      :	PropertyBag.h
//	ABSTRACT  :	Property bag for camera settings 
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//
//	Copyright (c) 2005, Nikon Corp.  All rights reserved.
//==============================================================================================

#if !defined(AFX_PROPERTYBAG_H__6250A5A3_84CE_41F4_A03D_1CDD651D7ACA__INCLUDED_)
#define AFX_PROPERTYBAG_H__6250A5A3_84CE_41F4_A03D_1CDD651D7ACA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPropertyBagItem :
   public CObject
{
   DECLARE_DYNAMIC( CPropertyBagItem );

public:
   CPropertyBagItem( LPCOLESTR pszName, VARIANT* pvarValue )
   {
	   m_strName = pszName;
	   m_varValue = pvarValue;
   }

public:
   CString m_strName;			// Buffers the name of a camera parameter
   COleVariant m_varValue;		// Buffers the value of a camera parameter
};

typedef CTypedPtrList< CObList, CPropertyBagItem* > CPropertyBagItemList;

class CPropertyBag : public IPropertyBag  
{
public:
	CPropertyBag();
	virtual ~CPropertyBag();

protected:
	CPropertyBagItemList m_listItem;	// List of camera settings

// IUnknown
public:
   STDMETHOD_( ULONG, AddRef )();
   STDMETHOD_( ULONG, Release )();
   STDMETHOD( QueryInterface )( REFIID iid, void** ppInterface );
protected:
   ULONG m_nRefCount;

// IPropertyBag
public:
   STDMETHOD( Read )( LPCOLESTR pszPropName, VARIANT* pvarValue,
	  IErrorLog* pErrorLog );
   STDMETHOD( Write )( LPCOLESTR pszPropName, VARIANT* pvarValue );
};

#endif // !defined(AFX_PROPERTYBAG_H__6250A5A3_84CE_41F4_A03D_1CDD651D7ACA__INCLUDED_)
