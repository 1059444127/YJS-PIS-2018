// PreSliceGrid.h: interface for the CPreSliceGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRESLICEGRID_H__92D74211_097C_4E78_A26E_8F436205198C__INCLUDED_)
#define AFX_PRESLICEGRID_H__92D74211_097C_4E78_A26E_8F436205198C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPreSliceGrid : public CUGCtrl  
{
public:
	CPreSliceGrid();
	~CPreSliceGrid();

	virtual void OnSetup();
	virtual void OnGetCell(int col,long row,CUGCell *cell);
	virtual int  OnCanSizeRow(long row);
	virtual int  OnCanSizeTopHdg();
	virtual int  OnCanSizeSideHdg();
	virtual int  OnCellTypeNotify(long ID,int col,long row,long msg,long param);

	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	virtual void OnCharDown(UINT *vcKey,BOOL processed);
	virtual int  OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag);
	virtual void OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);

	CString  m_OrderBy_Cmd;
	CString  m_WhereBetween_Cmd;
	CFont m_font;

	void ResetGridSize();
	
};

#endif // !defined(AFX_PRESLICEGRID_H__92D74211_097C_4E78_A26E_8F436205198C__INCLUDED_)
