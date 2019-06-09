#if !defined(AFX_RECORDTIMING_H__5AF184BF_FC61_440C_B653_4209EC9A7FC6__INCLUDED_)
#define AFX_RECORDTIMING_H__5AF184BF_FC61_440C_B653_4209EC9A7FC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecordTiming.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRecordTiming dialog
#include "DigiUtil.h"
#include "resource.h"

class CRecordTiming : public CXTResizeDialog
{
// Construction
public:
	CRecordTiming(CWnd* pParent = NULL);   // standard constructor
    BOOL Create();

// Dialog Data
	//{{AFX_DATA(CRecordTiming)
	enum { IDD = IDD_DIALOG_RECORDING };
	CDigiClock	m_LedText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordTiming)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd* m_pParent;
	int m_nID;

	// Generated message map functions
	//{{AFX_MSG(CRecordTiming)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT ClockEnded(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDTIMING_H__5AF184BF_FC61_440C_B653_4209EC9A7FC6__INCLUDED_)
