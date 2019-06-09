// YzRealTimeGrid.h: interface for the CYzRealTimeGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YZREALTIMEGRID_H__6356FEBC_FA14_4963_ABE7_D61BC05A4AAE__INCLUDED_)
#define AFX_YZREALTIMEGRID_H__6356FEBC_FA14_4963_ABE7_D61BC05A4AAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CYzRealTimeGrid : public CUGCtrl  
{
public:
	CYzRealTimeGrid();
	~CYzRealTimeGrid();

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

#endif // !defined(AFX_YZREALTIMEGRID_H__6356FEBC_FA14_4963_ABE7_D61BC05A4AAE__INCLUDED_)
