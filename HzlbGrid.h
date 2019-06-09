// HzlbGrid.h: interface for the CHzlbGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HZLBGRID_H__2C813760_DED3_4022_BD98_53FC2FA205AA__INCLUDED_)
#define AFX_HZLBGRID_H__2C813760_DED3_4022_BD98_53FC2FA205AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHzlbGrid : public CUGCtrl  
{
public:
	CHzlbGrid();
	~CHzlbGrid();

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

#endif // !defined(AFX_HZLBGRID_H__2C813760_DED3_4022_BD98_53FC2FA205AA__INCLUDED_)
