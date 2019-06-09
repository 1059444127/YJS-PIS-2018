// RightGrid.h: interface for the CRightGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIGHTGRID_H__21F92A82_3D29_4A6A_95F3_7541287FE9CB__INCLUDED_)
#define AFX_RIGHTGRID_H__21F92A82_3D29_4A6A_95F3_7541287FE9CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRightGrid : public CUGCtrl  
{
public:
	CRightGrid();
	~CRightGrid();

	virtual void OnSetup();
	virtual int  OnCanSizeRow(long row);
	virtual int  OnCanSizeTopHdg();
	virtual int  OnCanSizeSideHdg();
	virtual int  OnCellTypeNotify(long ID,int col,long row,long msg,long param);

private:

public:
	CFont m_font;
};

#endif // !defined(AFX_RIGHTGRID_H__21F92A82_3D29_4A6A_95F3_7541287FE9CB__INCLUDED_)
