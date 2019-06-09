// HistoryGrid.h: interface for the CHistoryGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISTORYGRID_H__0F55853E_7501_4AF5_8EC4_9DC5520A02F5__INCLUDED_)
#define AFX_HISTORYGRID_H__0F55853E_7501_4AF5_8EC4_9DC5520A02F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHistoryGrid : public CUGCtrl  
{
public:
	CHistoryGrid();
	~CHistoryGrid();

	virtual void OnSetup();
	virtual void OnGetCell(int col,long row,CUGCell *cell);
	virtual int  OnCanSizeRow(long row);
	virtual int  OnCanSizeTopHdg();
	virtual int  OnCanSizeSideHdg();
	virtual void OnRowChange(long oldrow,long newrow);
	virtual void OnMenuCommand(int col,long row,int section,int item);

	CString  m_OrderBy_Cmd;
	CString  m_WhereBetween_Cmd;
	CFont m_font;

	void ResetGridSize();
};

#endif // !defined(AFX_HISTORYGRID_H__0F55853E_7501_4AF5_8EC4_9DC5520A02F5__INCLUDED_)
