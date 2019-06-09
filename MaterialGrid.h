// MaterialGrid.h: interface for the CMaterialGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATERIALGRID_H__67A48900_FB74_4AB7_BF18_08314869B202__INCLUDED_)
#define AFX_MATERIALGRID_H__67A48900_FB74_4AB7_BF18_08314869B202__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMaterialGrid : public CUGCtrl  
{
public:
	bool isSingleSel;
	CMaterialGrid();
	~CMaterialGrid();
	virtual void OnSetup();
	virtual void OnGetCell(int col,long row,CUGCell *cell);
	virtual int  OnCanSizeRow(long row);
    virtual int  OnCanSizeCol(int col);
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	virtual int  OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag);
	virtual int  OnCellTypeNotify(long ID,int col,long row,long msg,long param);
	virtual void OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);

	CFont m_font;
	void ResetGridSize();
};

#endif // !defined(AFX_MATERIALGRID_H__67A48900_FB74_4AB7_BF18_08314869B202__INCLUDED_)
