#if !defined(AFX_ZCGRID_H__5EC44BE3_38CB_4DB5_A776_4E343B650BD4__INCLUDED_)
#define AFX_ZCGRID_H__5EC44BE3_38CB_4DB5_A776_4E343B650BD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZCGrid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ZCGrid dialog
#include "ugctsarw.h"
class ZCGrid : public CUGCtrl
{
// Construction
public:
	ZCGrid();   // standard constructor
	~ZCGrid();

	virtual void OnSetup();
	virtual void OnGetCell(int col,long row,CUGCell *cell);
	virtual int OnCanSizeRow(long row);
	virtual int  OnCanSizeTopHdg();
	virtual int  OnCanSizeSideHdg();
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	virtual int  OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag);
	virtual void OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);
	virtual void OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);
	
	CString m_OrderBy_Cmd;
	CString m_WhereBetween_Cmd;
	CFont m_font;

	CUGSortArrowType m_sortArrow;
	int m_sArrowIndex;
	BOOL m_sortedAssending;

	void ResetGridSize();

};


#endif // !defined(AFX_ZCGRID_H__5EC44BE3_38CB_4DB5_A776_4E343B650BD4__INCLUDED_)
