// CMedSlice : header file
/////////////////////////////////////////////////////////////////////////////

#ifndef __MEDSLICE_H
#define __MEDSLICE_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CMedSlice class
#include "SliceGrid.h"

class CMedSlice : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CMedSlice)

// Construction / destruction
public:

	// Constructs a CMedSlice object.
	CMedSlice();

	// Destroys a CMedSlice object, handles cleanup and de-allocation.
	virtual ~CMedSlice();

	void Refresh();
	void PerformQuery();
	//{{AFX_DATA(CMedSlice)
	enum { IDD = IDD_DIALOG_SLICE };
	//}}AFX_DATA

	CSliceGrid  m_SliceList;

// Member variables
//protected:
	CXTDateTimeCtrl m_Time_Qprq, m_Time_Zpsj;
	
	CString m_batchType;

private:
	bool	m_bIsFirstInit;

// Member functions
public:

	//{{AFX_VIRTUAL(CMedSlice)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMedSlice)
	afx_msg void OnButtonQuery();
	afx_msg void OnCheckSelectall();
	afx_msg void OnButtonBpsmhd();
	afx_msg void OnButtonBqdy();
	afx_msg void OnDeltaposSpinBlh(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	afx_msg void OnDestroy();
	afx_msg void OnButtonTranstable();
	afx_msg void OnBatchupadate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __MEDSLICE_H

