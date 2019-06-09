#if !defined(AFX_HISTORYSHOW_H__389F64CB_AF2E_4086_BCB2_3A98909A060C__INCLUDED_)
#define AFX_HISTORYSHOW_H__389F64CB_AF2E_4086_BCB2_3A98909A060C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistoryShow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHistoryShow dialog
#include "HistoryGrid.h"
#include "MyEdit.h"

class CMyEdit;
class CHistoryShow : public CXTResizeDialog
{
	DECLARE_DYNCREATE(CHistoryShow)
// Construction
public:
	CHistoryShow(CWnd* pParent = NULL);   // standard constructor

	CHistoryGrid m_grid;
// Dialog Data
	//{{AFX_DATA(CHistoryShow)
	enum { IDD = IDD_DIALOG_HISTORY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CMyEdit m_rysj;
	CMyEdit m_zdyj;
	CMyEdit m_bdbg;
	CMyEdit m_bcyj;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistoryShow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHistoryShow)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTORYSHOW_H__389F64CB_AF2E_4086_BCB2_3A98909A060C__INCLUDED_)
