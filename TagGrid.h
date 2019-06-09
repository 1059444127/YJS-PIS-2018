// TagGrid.h: interface for the CTagGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAGGRID_H__58866B64_1567_4AC7_8EC4_4076621F43DD__INCLUDED_)
#define AFX_TAGGRID_H__58866B64_1567_4AC7_8EC4_4076621F43DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTagGrid : public CUGCtrl  
{
public:
	CTagGrid();
	~CTagGrid();

	void ResetGridSize();

	virtual void OnSetup();
	virtual int  OnCanSizeRow(long row);
	virtual int  OnCanSizeTopHdg();
	virtual int  OnCanSizeSideHdg();

	CFont m_font;
};

#endif // !defined(AFX_TAGGRID_H__58866B64_1567_4AC7_8EC4_4076621F43DD__INCLUDED_)
