#if !defined(AFX_XGYJ_H__7D6FEEE3_39AD_4B61_8A31_CFB1FF87DCDE__INCLUDED_)
#define AFX_XGYJ_H__7D6FEEE3_39AD_4B61_8A31_CFB1FF87DCDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Xgyj.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXgyj dialog
#include "MyEdit.h"

class CNormalWord;
class CXgyj : public CXTResizeDialog
{
	DECLARE_DYNCREATE(CXgyj)

// Construction
public:
	CXgyj(CWnd* pParent = NULL);   // standard constructor

	UINT CurEdit;
	CNormalWord *pNormalWord;

// Dialog Data
	//{{AFX_DATA(CXgyj)
	enum { IDD = IDD_DIALOG_XGYJ };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CMyEdit m_ctrl_czyj;
	CString m_czyj, m_xgyj,m_hzyj;
	CWnd *m_pParent;
	CBitmapButton bitButton;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXgyj)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXgyj)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonTqzd();
	afx_msg void OnButtonHxzd();
	afx_msg void OnButtonSave();
	virtual void OnOK();
	afx_msg void OnButtonCyc();
	afx_msg void OnSetfocusEditXgyj();
	afx_msg void OnDestroy();
	afx_msg void OnButtonCopycz();
	afx_msg void OnButtonCopyzz();
	afx_msg void OnSetfocusEditCzyj();
	afx_msg void OnSetfocusEditHzyj();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XGYJ_H__7D6FEEE3_39AD_4B61_8A31_CFB1FF87DCDE__INCLUDED_)
