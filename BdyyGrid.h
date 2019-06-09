// BdyyGrid.h: interface for the CBdyyGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BDYYGRID_H__8B8ABA41_CE2C_4231_A88D_7026653498DF__INCLUDED_)
#define AFX_BDYYGRID_H__8B8ABA41_CE2C_4231_A88D_7026653498DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBdyyGrid : public CUGCtrl  
{
public:
	virtual int OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag);
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	CBdyyGrid();
	~CBdyyGrid();

	virtual void OnSetup();
	virtual void OnGetCell(int col,long row,CUGCell *cell);
	virtual int  OnCanSizeRow(long row);
	virtual int  OnCanSizeTopHdg();
	virtual int  OnCanSizeSideHdg();
	
	CString  m_OrderBy_Cmd;
	CString  m_WhereBetween_Cmd;
	CFont m_font;
	
	void ResetGridSize();
};

#endif // !defined(AFX_BDYYGRID_H__8B8ABA41_CE2C_4231_A88D_7026653498DF__INCLUDED_)
