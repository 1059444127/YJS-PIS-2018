// SfDetailGrid.h: interface for the CSfDetailGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SFDETAILGRID_H__4A113D64_C665_4A9A_ACAE_F5BFC1952B22__INCLUDED_)
#define AFX_SFDETAILGRID_H__4A113D64_C665_4A9A_ACAE_F5BFC1952B22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSfDetailGrid : public CUGCtrl  
{
public:
	CSfDetailGrid();
	virtual ~CSfDetailGrid();
	CFont m_font;

	void GridSetup();

	virtual void OnSetup();
	virtual void OnGetCell(int col,long row,CUGCell *cell);
	virtual int  OnCellTypeNotify(long ID,int col,long row,long msg,long param);

	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	virtual void OnCharDown(UINT *vcKey,BOOL processed);
	virtual void OnKeyDown(UINT *vcKey,BOOL processed);
	virtual int  OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag);

};

#endif // !defined(AFX_SFDETAILGRID_H__4A113D64_C665_4A9A_ACAE_F5BFC1952B22__INCLUDED_)
