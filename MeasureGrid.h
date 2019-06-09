// MeasureGrid.h: interface for the CMeasureGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEASUREGRID_H__EAF6715F_E783_492B_8CD6_F04305FD593E__INCLUDED_)
#define AFX_MEASUREGRID_H__EAF6715F_E783_492B_8CD6_F04305FD593E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ugctrl.h"

class CMeasureGrid : public CUGCtrl  
{
public:
	CMeasureGrid();
	virtual ~CMeasureGrid();

	CFont m_font;
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeasureGrid)
	//}}AFX_VIRTUAL
	
	
	//{{AFX_MSG(CMeasureGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual void OnSetup();
	virtual void OnGetCell(int col,long row,CUGCell *cell);
	virtual void OnMenuCommand(int col,long row,int section,int item);

	void LpToGeop(int,int, float &, float &);
	void LpToGeop(float , float &);
};

#endif // !defined(AFX_MEASUREGRID_H__EAF6715F_E783_492B_8CD6_F04305FD593E__INCLUDED_)
