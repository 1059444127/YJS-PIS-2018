// SelfGrid.h: interface for the CSelfGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELFGRID_H__FBF24DDF_AA1B_40DC_8BCD_DE06B7C88066__INCLUDED_)
#define AFX_SELFGRID_H__FBF24DDF_AA1B_40DC_8BCD_DE06B7C88066__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSelfGrid : public CUGCtrl  
{
public:
	CSelfGrid();
	~CSelfGrid();

	virtual void OnSetup();
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	virtual void OnCharDown(UINT *vcKey,BOOL processed);
	virtual void OnRowChange(long oldrow,long newrow);
	virtual int  OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag);
	virtual void OnKeyDown(UINT *vcKey,BOOL processed);
	virtual int  OnEditVerify(int col,long row,CWnd *edit, UINT *vcKey);
	virtual void OnSetCell(int col,long row,CUGCell *cell);
	virtual void OnMenuCommand(int col,long row,int section,int item);

public:
	CFont m_font;
};

#endif // !defined(AFX_SELFGRID_H__FBF24DDF_AA1B_40DC_8BCD_DE06B7C88066__INCLUDED_)
