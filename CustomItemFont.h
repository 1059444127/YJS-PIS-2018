// CustomItemFont.h: interface for the CCustomItemFont class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMITEMFONT_H__79B14592_9703_4DDE_8FE0_D2C1F3D483E1__INCLUDED_)
#define AFX_CUSTOMITEMFONT_H__79B14592_9703_4DDE_8FE0_D2C1F3D483E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCustomItemFont : public CXTPropertyGridItemFont  
{
public:
	CCustomItemFont(CString strCaption,LOGFONT& font,COLORREF color);

	COLORREF m_Color;
protected:
	virtual void OnInplaceButtonDown();
};

#endif // !defined(AFX_CUSTOMITEMFONT_H__79B14592_9703_4DDE_8FE0_D2C1F3D483E1__INCLUDED_)
