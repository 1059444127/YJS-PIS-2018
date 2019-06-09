// CMedPreslice : header file
/////////////////////////////////////////////////////////////////////////////

#ifndef __MEDPRESLICE_H
#define __MEDPRESLICE_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CMedPreslice class
#include "PreSliceGrid.h"

class CMedPreslice : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CMedPreslice)

// Construction / destruction
public:
    static CString GenNewSliceNo(CString blh, int nStartNo = 1);

	// Constructs a CMedPreslice object.
	CMedPreslice();

	// Destroys a CMedPreslice object, handles cleanup and de-allocation.
	virtual ~CMedPreslice();

	void Refresh();
	void PerformQuery();
	//{{AFX_DATA(CMedPreslice)
	enum { IDD = IDD_DIALOG_PRESLICE };
	CXTHyperLink	m_HypLink_Note;
	//}}AFX_DATA

// Member variables
protected:
	CXTDateTimeCtrl m_Time_Bmrq;
	CPreSliceGrid  m_PreSliceList;
private:
	bool	m_bIsFirstInit;

// Member functions
public:

	//{{AFX_VIRTUAL(CMedPreslice)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMedPreslice)
	afx_msg void OnButtonQuery();
	afx_msg void OnCheckSelectall();
	afx_msg void OnButtonQpqr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __MEDPRESLICE_H

