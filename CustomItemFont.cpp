// CustomItemFont.cpp: implementation of the CCustomItemFont class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "CustomItemFont.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomItemFont::CCustomItemFont(CString strCaption,LOGFONT& font,COLORREF color)
     : CXTPropertyGridItemFont(strCaption, font)
{
	SetFont(font);
	m_nFlags = pgiHasExpandButton;
	m_Color = color;
}

void CCustomItemFont::OnInplaceButtonDown()
{
	LOGFONT nTmpLf;
	GetFont(&nTmpLf);
	CFontDialog dlg(&nTmpLf);
    dlg.m_cf.rgbColors = m_Color;
	if (dlg.DoModal() == IDOK)
	{
		LOGFONT lf;
		dlg.GetCurrentFont(&lf);
		SetFont(lf);
		OnValueChanged(m_strValue);
		m_pGrid->Invalidate(FALSE);

		m_Color = dlg.GetColor();
	}
}