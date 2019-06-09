// SflbGrid.h: interface for the CSflbGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SFLBGRID_H__F5967BFA_F4AD_4A07_B375_CB73BDC479D6__INCLUDED_)
#define AFX_SFLBGRID_H__F5967BFA_F4AD_4A07_B375_CB73BDC479D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSflbGrid : public CUGCtrl  
{
public:
	CSflbGrid();
	~CSflbGrid();

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

#endif // !defined(AFX_SFLBGRID_H__F5967BFA_F4AD_4A07_B375_CB73BDC479D6__INCLUDED_)
