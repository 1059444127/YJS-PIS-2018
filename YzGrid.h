// YzGrid.h: interface for the CYzGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YZGRID_H__B37CE9BF_490E_4145_A433_F6E9542900AA__INCLUDED_)
#define AFX_YZGRID_H__B37CE9BF_490E_4145_A433_F6E9542900AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CYzGrid : public CUGCtrl  
{
public:
	CYzGrid();
	~CYzGrid();

	virtual void OnSetup();
	virtual void OnGetCell(int col,long row,CUGCell *cell);
	virtual int  OnCanSizeRow(long row);
	virtual int  OnCanSizeTopHdg();
	virtual int  OnCanSizeSideHdg();
	virtual void OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	virtual void OnCharDown(UINT *vcKey,BOOL processed);
	virtual int  OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag);

	CString  m_OrderBy_Cmd;
	CString  m_WhereBetween_Cmd;
	CFont m_font;

	void ResetGridSize();
};

#endif // !defined(AFX_YZGRID_H__B37CE9BF_490E_4145_A433_F6E9542900AA__INCLUDED_)
