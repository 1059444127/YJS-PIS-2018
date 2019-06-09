// CMedAdvice : header file
/////////////////////////////////////////////////////////////////////////////

#ifndef __MEDADVICE_H
#define __MEDADVICE_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CMedAdvice class
#include "YzGrid.h"

class CMedAdvice : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CMedAdvice)

// Construction / destruction
public:

	// Constructs a CMedAdvice object.
	CMedAdvice();

	// Destroys a CMedAdvice object, handles cleanup and de-allocation.
	virtual ~CMedAdvice();

	void Refresh();
	void PerformQuery();
	//{{AFX_DATA(CMedAdvice)
	enum { IDD = IDD_DIALOG_ADVICE };
	//}}AFX_DATA

// Member variables
protected:
	CXTDateTimeCtrl m_Time_Sqrq;
	CYzGrid  m_AdviceList;
private:
	bool	m_bIsFirstInit;

// Member functions
public:

	//{{AFX_VIRTUAL(CMedAdvice)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMedAdvice)
	afx_msg void OnButtonQuery();
	afx_msg void OnCheckSelectall();
	afx_msg void OnButtonYzzh();
	afx_msg void OnButtonTranstable();
	afx_msg void OnButtonYzqr();
	afx_msg void OnButtonDefaultquery();
	afx_msg void OnButtonAdvicedel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __MEDADVICE_H

