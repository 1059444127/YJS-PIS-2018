// KnlyGrid.h: interface for the CKnlyGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KNLYGRID_H__96D2785F_BA3C_4301_8F55_BF3F6F655515__INCLUDED_)
#define AFX_KNLYGRID_H__96D2785F_BA3C_4301_8F55_BF3F6F655515__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKnlyGrid : public CUGCtrl  
{
public:
	CKnlyGrid();
	~CKnlyGrid();

	virtual void OnSetup();
	virtual void OnGetCell(int col,long row,CUGCell *cell);
	virtual int  OnCanSizeRow(long row);
	virtual int  OnCanSizeTopHdg();
	virtual int  OnCanSizeSideHdg();
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	virtual void OnMenuCommand(int col,long row,int section,int item);

	CString  m_OrderBy_Cmd;
	CString  m_WhereBetween_Cmd;
	CFont m_font;

	void ResetGridSize();
};

#endif // !defined(AFX_KNLYGRID_H__96D2785F_BA3C_4301_8F55_BF3F6F655515__INCLUDED_)
