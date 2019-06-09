#if !defined(AFX_QCMX_H__65F63219_F61C_4C12_A9C0_BB7E90349609__INCLUDED_)
#define AFX_QCMX_H__65F63219_F61C_4C12_A9C0_BB7E90349609__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Qcmx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CQcmx dialog
#include "MaterialGrid.h"

class CQcmx : public CXTResizeDialog
{
	DECLARE_DYNCREATE(CQcmx)
// Construction
public:
	CQcmx(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CQcmx)
	enum { IDD = IDD_DIALOG_QCMX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQcmx)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CMaterialGrid  m_MaterialList;

	// Generated message map functions
	//{{AFX_MSG(CQcmx)
	virtual BOOL OnInitDialog();
	afx_msg void OnMatearialLkbs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QCMX_H__65F63219_F61C_4C12_A9C0_BB7E90349609__INCLUDED_)
