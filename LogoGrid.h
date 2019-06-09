// LogoGrid.h: interface for the CLogoGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGOGRID_H__E04CFEF0_B398_4038_80DD_D7BC109EA9B3__INCLUDED_)
#define AFX_LOGOGRID_H__E04CFEF0_B398_4038_80DD_D7BC109EA9B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLogoGrid : public CUGCtrl  
{
public:
	CLogoGrid();
	~CLogoGrid();

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

#endif // !defined(AFX_LOGOGRID_H__E04CFEF0_B398_4038_80DD_D7BC109EA9B3__INCLUDED_)
