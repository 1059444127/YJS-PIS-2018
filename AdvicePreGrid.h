// AdvicePreGrid.h: interface for the CAdvicePreGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADVICEPREGRID_H__1487EF41_0FDD_43F7_A66E_652C5E2E339F__INCLUDED_)
#define AFX_ADVICEPREGRID_H__1487EF41_0FDD_43F7_A66E_652C5E2E339F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAdvicePreGrid : public CUGCtrl  
{
public:
	CAdvicePreGrid();
	~CAdvicePreGrid();

	virtual void OnSetup();
	virtual void OnGetCell(int col,long row,CUGCell *cell);
	virtual int  OnCanSizeRow(long row);
    virtual int  OnCanSizeCol(int col);
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	virtual void OnCharDown(UINT *vcKey,BOOL processed);
	virtual int  OnCellTypeNotify(long ID,int col,long row,long msg,long param);
	virtual int  OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag);

	CFont m_font;
	void ResetGridSize();
};

#endif // !defined(AFX_ADVICEPREGRID_H__1487EF41_0FDD_43F7_A66E_652C5E2E339F__INCLUDED_)
