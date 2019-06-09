#if !defined(AFX_DLGLISTINPUT_H__17C135C3_B2B5_4F64_ADCE_B92F3BE09E72__INCLUDED_)
#define AFX_DLGLISTINPUT_H__17C135C3_B2B5_4F64_ADCE_B92F3BE09E72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgListInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgListInput dialog

#include "ListInfo.h"

class CDlgListInput : public CXTResizeDialog
{
// Construction
public:
	CDlgListInput(CWnd* pParent = NULL);   // standard constructor
    BOOL Create();

    CListInfo m_listinfo;

	int m_type;
	CWnd* m_pParent;
	
// Dialog Data
	//{{AFX_DATA(CDlgListInput)
	enum { IDD = IDD_DIALOG_LISTINPUT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgListInput)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nID;
	
	// Generated message map functions
	//{{AFX_MSG(CDlgListInput)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnIndex();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLISTINPUT_H__17C135C3_B2B5_4F64_ADCE_B92F3BE09E72__INCLUDED_)
