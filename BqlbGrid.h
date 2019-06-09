// BqlbGrid.h: interface for the CBqlbGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BQLBGRID_H__F1456A3C_5375_4E01_9A67_777A70F2C25D__INCLUDED_)
#define AFX_BQLBGRID_H__F1456A3C_5375_4E01_9A67_777A70F2C25D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBqlbGrid : public CUGCtrl  
{
public:
	virtual void OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);
	CBqlbGrid();
	~CBqlbGrid();

	CString Perform();

	virtual void OnSetup();
	virtual void OnGetCell(int col,long row,CUGCell *cell);
	virtual int  OnCanSizeRow(long row);
	virtual int  OnCanSizeTopHdg();
	virtual int  OnCanSizeSideHdg();
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	CString  m_OrderBy_Cmd;
	CString  m_WhereBetween_Cmd;
	CFont m_font;

	void ResetGridSize();
};

#endif // !defined(AFX_BQLBGRID_H__F1456A3C_5375_4E01_9A67_777A70F2C25D__INCLUDED_)
