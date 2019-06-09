// Figure.h: interface for the CFigure class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIGURE_H__192FCE25_969E_11D4_AAE5_004005524B5F__INCLUDED_)
#define AFX_FIGURE_H__192FCE25_969E_11D4_AAE5_004005524B5F__INCLUDED_

#include "FigureView.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFigure  
{
public:
	CFigure(CString fname,UINT WindowLeft, UINT WindowTop,UINT ViewX,UINT ViewY,BOOL bStretch=FALSE);
	CFigure(CString fname,UINT WindowLeft, UINT WindowTop,UINT ViewX,UINT ViewY,CString blh, int serialno, BOOL bStretch=TRUE);
	virtual ~CFigure();
	CFrameWnd   * m_pFigureFrame;
	CFigureView * m_pFigureView;
};

#endif // !defined(AFX_FIGURE_H__192FCE25_969E_11D4_AAE5_004005524B5F__INCLUDED_)
