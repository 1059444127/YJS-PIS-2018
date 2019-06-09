// BbmxGrid.h: interface for the CBbmxGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BBMXGRID_H__E6F663CF_2A3D_4673_9243_1E786C8625B3__INCLUDED_)
#define AFX_BBMXGRID_H__E6F663CF_2A3D_4673_9243_1E786C8625B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBbmxGrid : public CUGCtrl  
{
public:
	CBbmxGrid();
	virtual ~CBbmxGrid();
	CFont m_font;

	virtual void OnSetup();
	virtual void OnGetCell(int col,long row,CUGCell *cell);
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	virtual void OnCharDown(UINT *vcKey,BOOL processed);
	virtual int  OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag);
};

#endif // !defined(AFX_BBMXGRID_H__E6F663CF_2A3D_4673_9243_1E786C8625B3__INCLUDED_)
