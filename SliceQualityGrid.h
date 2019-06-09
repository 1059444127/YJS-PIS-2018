// SliceQualityGrid.h: interface for the CSliceQualityGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLICEQUALITYGRID_H__F34E100C_9313_4953_9BC5_48F7801D8DFE__INCLUDED_)
#define AFX_SLICEQUALITYGRID_H__F34E100C_9313_4953_9BC5_48F7801D8DFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSliceQualityGrid : public CUGCtrl  
{
public:
	CSliceQualityGrid();
	~CSliceQualityGrid();

	virtual void OnSetup();
	virtual void OnGetCell(int col,long row,CUGCell *cell);
	virtual int  OnCanSizeRow(long row);
    virtual int  OnCanSizeCol(int col);
	virtual int  OnCellTypeNotify(long ID,int col,long row,long msg,long param);
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	virtual void OnCharDown(UINT *vcKey,BOOL processed);
	virtual int  OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag);

	CFont m_font;
	void ResetGridSize();
};

#endif // !defined(AFX_SLICEQUALITYGRID_H__F34E100C_9313_4953_9BC5_48F7801D8DFE__INCLUDED_)
