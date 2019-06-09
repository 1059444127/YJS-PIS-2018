// CMedWax : header file
/////////////////////////////////////////////////////////////////////////////

#ifndef __MEDWAX_H
#define __MEDWAX_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CMedWax class
#include "WaxGrid.h"

class CMedWax : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CMedWax)

// Construction / destruction
public:

	// Constructs a CMedWax object.
	CMedWax();

	// Destroys a CMedWax object, handles cleanup and de-allocation.
	virtual ~CMedWax();

	void Refresh();
	void PerformQuery();

	
	//{{AFX_DATA(CMedWax)
	enum { IDD = IDD_DIALOG_WAX };
	CXTHyperLink	m_HypLink_Note;
	//}}AFX_DATA

// Member variables
protected:

	CXTDateTimeCtrl m_Time_Qcrq;
	CWaxGrid  m_WaxList;

private:
	bool	m_bIsFirstInit;

// Member functions
public:

	//{{AFX_VIRTUAL(CMedWax)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMedWax)
	afx_msg void OnCheckSelectall();
	afx_msg void OnButtonQuery();
	afx_msg void OnButtonCkhd();
	afx_msg void OnButtonBmqr();
	afx_msg void OnButtonTranstable();
	afx_msg void OnButtonDefaultquery();
	afx_msg void OnButtonBdquery();
	afx_msg void OnButtonBqdy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __MEDWAX_H

