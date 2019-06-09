#if !defined(AFX_CHARGECONFIRM_H__AB0EBD77_A4F9_4E1F_84ED_615D5AFC23D7__INCLUDED_)
#define AFX_CHARGECONFIRM_H__AB0EBD77_A4F9_4E1F_84ED_615D5AFC23D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChargeConfirm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChargeConfirm dialog
#include "YzGrid.h"

class CChargeConfirm : public CXTResizeDialog
{
	DECLARE_DYNCREATE(CChargeConfirm)

// Construction
public:
	CChargeConfirm(CWnd* pParent = NULL);   // standard constructor
	CYzGrid m_grid;
	void PerformQuery();

// Dialog Data
	//{{AFX_DATA(CChargeConfirm)
	enum { IDD = IDD_DIALOG_CHARGECONFORM };
	CXTHyperLink	m_HypLink_Note;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChargeConfirm)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CXTDateTimeCtrl m_Time_Sqrq;

	// Generated message map functions
	//{{AFX_MSG(CChargeConfirm)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonQuery();
	afx_msg void OnButtonYzqr();
	afx_msg void OnCheckSelectall();
	afx_msg void OnButtonDefaultquery();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARGECONFIRM_H__AB0EBD77_A4F9_4E1F_84ED_615D5AFC23D7__INCLUDED_)
