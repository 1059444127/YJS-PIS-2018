// AdviceGrid.h: interface for the CAdviceGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADVICEGRID_H__C27DFFA3_8E83_460A_A844_5C4248BBBF9E__INCLUDED_)
#define AFX_ADVICEGRID_H__C27DFFA3_8E83_460A_A844_5C4248BBBF9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAdviceGrid : public CUGCtrl  
{
public:
	CAdviceGrid();
	~CAdviceGrid();

	virtual void OnSetup();
	virtual void OnGetCell(int col,long row,CUGCell *cell);
	virtual int  OnCanSizeRow(long row);
    virtual int  OnCanSizeCol(int col);
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	virtual int  OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag);

	CFont m_font;
	void ResetGridSize();
};

#endif // !defined(AFX_ADVICEGRID_H__C27DFFA3_8E83_460A_A844_5C4248BBBF9E__INCLUDED_)
