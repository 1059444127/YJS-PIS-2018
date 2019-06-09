#if !defined(AFX_QPPJ_H__12A746D4_E086_4105_BA23_6031E6BB874C__INCLUDED_)
#define AFX_QPPJ_H__12A746D4_E086_4105_BA23_6031E6BB874C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Qppj.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CQppj dialog
#include "SliceQualityGrid.h"
#include "MenuEdit.h"

class CQppj : public CXTResizeDialog
{
// Construction
public:
	CQppj(CWnd* pParent = NULL);   // standard constructor

	CSliceQualityGrid  m_SliceList;
// Dialog Data
	//{{AFX_DATA(CQppj)
	enum { IDD = IDD_DIALOG_QPPJ };
	int		m_Radio_State;
	//}}AFX_DATA
	CMenuEdit m_ctrl_qcbw;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQppj)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CXTDateTimeCtrl m_Time_Zpsj;

	// Generated message map functions
	//{{AFX_MSG(CQppj)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckSelectall();
	afx_msg void OnButtonPd();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonBpsmhd();
	afx_msg void OnButtonBqdy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QPPJ_H__12A746D4_E086_4105_BA23_6031E6BB874C__INCLUDED_)
