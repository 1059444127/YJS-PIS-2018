// ListInfo.cpp: implementation of the CListInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "ListInfo.h"
#include "DlgListInput.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListInfo::CListInfo()
{

}

CListInfo::~CListInfo()
{

}

void CListInfo::OnSetup()
{
	m_font.CreateFont(16,0,0,0,500,0,0,0,0,0,0,0,0,"Arial");
	SetDefFont(&m_font);

	CUGCell cell;
	GetGridDefault(&cell);
	cell.SetAlignment(UG_ALIGNLEFT);
	cell.SetBorder(UG_BDR_RECESSED);
	cell.SetBackColor(RGB(220,220,220));
	SetGridDefault(&cell);

	SetNumberRows(0);
	SetNumberCols(1);

	SetSH_Width(0);
	SetTH_Height(0);
	
	CRect rt;
	GetClientRect(&rt);
	SetColWidth(0,rt.Width() - 20);

	SetVScrollMode( UG_SCROLLTRACKING );
	SetHScrollMode( UG_SCROLLTRACKING );

	SetHighlightRow(TRUE);
	SetDoubleBufferMode(TRUE);
}

void CListInfo::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	if(!(col == 0 && row >= 0 && row < GetNumberRows() )) return;
	((CDlgListInput *)GetParent())->m_pParent->SendMessage(WM_LISTSELECT);

	GetParent()->ShowWindow(SW_HIDE);
}
