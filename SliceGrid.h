// SliceGrid.h: interface for the CSliceGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLICEGRID_H__06F7E4EC_F94E_4914_99EE_7793BAD3887C__INCLUDED_)
#define AFX_SLICEGRID_H__06F7E4EC_F94E_4914_99EE_7793BAD3887C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ugctsarw.h"

class CSliceGrid : public CUGCtrl  
{
public:
	CSliceGrid();
	~CSliceGrid();

	virtual void OnSetup();
	virtual void OnGetCell(int col,long row,CUGCell *cell);
	virtual int  OnCanSizeRow(long row);
	virtual int  OnCanSizeTopHdg();
	virtual int  OnCanSizeSideHdg();
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	virtual int  OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag);
	virtual void OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);
	virtual void OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);

	CString  m_OrderBy_Cmd;
	CString  m_WhereBetween_Cmd;
	CFont m_font;

	CUGSortArrowType	m_sortArrow;
	int					m_sArrowIndex;
	BOOL m_sortedAssending;

	void ResetGridSize();
};

#endif // !defined(AFX_SLICEGRID_H__06F7E4EC_F94E_4914_99EE_7793BAD3887C__INCLUDED_)
