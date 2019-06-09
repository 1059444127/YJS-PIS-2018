// UserGrid.h: interface for the CUserGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERGRID_H__057511F5_1482_4B24_8C32_A0E11777C983__INCLUDED_)
#define AFX_USERGRID_H__057511F5_1482_4B24_8C32_A0E11777C983__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUserGrid : public CUGCtrl  
{
public:
	CUserGrid();
	~CUserGrid();

	void GridSetup();

	virtual void OnSetup();
	virtual void OnGetCell(int col,long row,CUGCell *cell);
	virtual int  OnCanSizeRow(long row);
	virtual int  OnCanSizeTopHdg();
	virtual int  OnCanSizeSideHdg();
	virtual void OnRowChange(long oldrow,long newrow);

private:

public:
	CFont m_font;
};

#endif // !defined(AFX_USERGRID_H__057511F5_1482_4B24_8C32_A0E11777C983__INCLUDED_)
