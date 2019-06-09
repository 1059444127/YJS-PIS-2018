// ListInfo.h: interface for the CListInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTINFO_H__D259F394_4F69_48ED_8E61_20064CAF5B8D__INCLUDED_)
#define AFX_LISTINFO_H__D259F394_4F69_48ED_8E61_20064CAF5B8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CListInfo : public CUGCtrl  
{
public:
	CListInfo();
	~CListInfo();

	virtual void OnSetup();
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	
public:
	CFont m_font;
};

#endif // !defined(AFX_LISTINFO_H__D259F394_4F69_48ED_8E61_20064CAF5B8D__INCLUDED_)
