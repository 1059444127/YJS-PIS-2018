// CAdviceShow : header file
/////////////////////////////////////////////////////////////////////////////

#ifndef __ADVICESHOW_H
#define __ADVICESHOW_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CAdviceShow class
#include "YzRealTimeGrid.h"

class CAdviceShow : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CAdviceShow)

// Construction / destruction
public:

	// Constructs a CAdviceShow object.
	CAdviceShow();
	void RefreshHyperLink();
	void GetLeaveMessage();
	CYzRealTimeGrid    m_RecordList;

	// Destroys a CAdviceShow object, handles cleanup and de-allocation.
	virtual ~CAdviceShow();

	//{{AFX_DATA(CAdviceShow)
	enum { IDD = IDD_DIALOG_REALTIMEADVICE };
	CXTHyperLink	m_HypLink_Knly;
	//}}AFX_DATA

// Member variables
protected:
	UINT  m_TimerHandle;
private:
	bool	m_bIsFirstInit;
// Member functions
public:

	//{{AFX_VIRTUAL(CAdviceShow)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAdviceShow)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnHypKnly();
	afx_msg void OnButtonRefresh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __ADVICESHOW_H

