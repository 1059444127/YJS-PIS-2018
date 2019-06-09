#if !defined(AFX_BBMXDETAIL_H__BC75661D_7F90_4AC9_B758_D244363F8CC1__INCLUDED_)
#define AFX_BBMXDETAIL_H__BC75661D_7F90_4AC9_B758_D244363F8CC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BbmxDetail.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBbmxDetail dialog
#include "BbmxGrid.h"
class CMedPerson;
class CBbmxDetail : public CXTCBarDialog
{
// Construction
public:
	CBbmxDetail(CWnd* pParent = NULL);   // standard constructor
	CMedPerson *m_pParent;
	CString m_Sqh;
	SAConnection hiscon;
	SACommand hiscmd;

	CString m_Bbmc;

// Dialog Data
	//{{AFX_DATA(CBbmxDetail)
	enum { IDD = IDD_DIALOG_BBMCDETAIL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBbmxDetail)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CBbmxGrid   m_DetailCtrl;

	// Generated message map functions
	//{{AFX_MSG(CBbmxDetail)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnCheck();
	virtual void OnOK();
	afx_msg void OnReject();
	//}}AFX_MSG
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BBMXDETAIL_H__BC75661D_7F90_4AC9_B758_D244363F8CC1__INCLUDED_)
