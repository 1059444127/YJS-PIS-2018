#if !defined(AFX_QUICKQUERY_H__94AF94F3_EDD7_4D29_91B0_8D173514802F__INCLUDED_)
#define AFX_QUICKQUERY_H__94AF94F3_EDD7_4D29_91B0_8D173514802F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QuickQuery.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CQuickQuery dialog
#include "MenuEdit.h"
#include "DlgListInput.h"

class CQuickQuery : public CDialog
{
// Construction
public:
	CQuickQuery(CWnd* pParent = NULL);   // standard constructor

	CString	m_query;
    CDlgListInput  *pListInput;

// Dialog Data
	//{{AFX_DATA(CQuickQuery)
	enum { IDD = IDD_DIALOG_QUICKQUERY };
	CXTHyperLink	m_HypLink_Note;
	CMenuEdit	m_ctrl_shys;
	CMenuEdit	m_ctrl_zzys;
	CMenuEdit	m_ctrl_czys;
	CMenuEdit	m_ctrl_bbmc;
	CMenuEdit	m_ctrl_bblx;
	CMenuEdit	m_ctrl_sjks;
	CMenuEdit	m_ctrl_sjdw;
	CEdit		m_ctrl_age1;
	CEdit		m_ctrl_age2;
	CMenuEdit   m_ctrl_xb;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQuickQuery)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CXTDateTimeCtrl m_Time_Sjrq_From, m_Time_Sjrq_To, m_Time_Bgrq_From, m_Time_Bgrq_To;

	// Generated message map functions
	//{{AFX_MSG(CQuickQuery)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnButtonMicd10();
	afx_msg void OnButtonSicd10();
	//}}AFX_MSG
	afx_msg LRESULT ListSelect(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUICKQUERY_H__94AF94F3_EDD7_4D29_91B0_8D173514802F__INCLUDED_)
