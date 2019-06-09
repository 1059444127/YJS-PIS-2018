#if !defined(AFX_BATCHPRINTCTRL_H__7B183AC9_BCFA_4145_9D41_3FA6603AB539__INCLUDED_)
#define AFX_BATCHPRINTCTRL_H__7B183AC9_BCFA_4145_9D41_3FA6603AB539__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BatchPrintCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBatchPrintCtrl dialog

class CBatchPrintCtrl : public CDialog
{
// Construction
public:
	CBatchPrintCtrl(CWnd* pParent = NULL);   // standard constructor
    BOOL Create();

	bool * pStopPreview;
	bool * pStopPrint;

	UINT m_TimerHandle;

// Dialog Data
	//{{AFX_DATA(CBatchPrintCtrl)
	enum { IDD = IDD_DIALOG_BATCHPRINT };
	CXTButton	m_StopPrint;
	CXTButton	m_StopPreview;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBatchPrintCtrl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd* m_pParent;
	int m_nID;

	// Generated message map functions
	//{{AFX_MSG(CBatchPrintCtrl)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonStopprint();
	afx_msg void OnButtonStoppreview();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BATCHPRINTCTRL_H__7B183AC9_BCFA_4145_9D41_3FA6603AB539__INCLUDED_)
