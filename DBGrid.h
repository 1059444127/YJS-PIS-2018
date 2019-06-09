// DBGrid.h: interface for the CDBGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBGRID_H__7EF56B91_17E9_46AA_9172_97904D7F4FC5__INCLUDED_)
#define AFX_DBGRID_H__7EF56B91_17E9_46AA_9172_97904D7F4FC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ugctsarw.h"

class CDBGrid : public CUGCtrl  
{
public:
	CDBGrid();
	~CDBGrid();
	virtual void OnSetup();
	virtual void OnGetCell(int col,long row,CUGCell *cell);
	virtual int  OnCanSizeRow(long row);
    virtual int  OnCanSizeCol(int col);
	virtual int  OnCanSizeTopHdg();
	virtual int  OnCanSizeSideHdg();
	virtual void OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);
	virtual int  OnVScrollHint(long row,CString *string);
	virtual void OnMenuCommand(int col,long row,int section,int item);
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	virtual void OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);

	void RefreshMenuItem();

	void ReflectSelection();

	CUGSortArrowType	m_sortArrow;
	int					m_sArrowIndex;
	int  m_sortCol;
	BOOL m_sortedAssending;

	CString  m_WhereBetween_Cmd;

	CString  m_OrderBy_Cmd;
	CString  m_CompositeQueryString;

	CStringArray  m_FieldName_List;
	CStringArray  m_FieldDescription_List;
	CStringArray  m_FieldInfo_List;
	CUIntArray    m_FieldType_List;

	int  m_MainRecordnum, m_DefineRecordnum;

	CFont m_font;
	void ResetGridSize();
};

#endif // !defined(AFX_DBGRID_H__7EF56B91_17E9_46AA_9172_97904D7F4FC5__INCLUDED_)
