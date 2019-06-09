#if !defined(AFX_STATISTIC_H__D05DE708_ADF7_4039_A7F4_B64A58B25578__INCLUDED_)
#define AFX_STATISTIC_H__D05DE708_ADF7_4039_A7F4_B64A58B25578__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Statistic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatistic dialog

class CStatistic : public CXTCBarDialog
{
// Construction
public:
	CStatistic(CWnd* pParent = NULL);   // standard constructor

	CString	m_from,	m_to;
// Dialog Data
	//{{AFX_DATA(CStatistic)
	enum { IDD = IDD_DIALOG_STATISTIC };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
    
	CString m_Title;
	CXTFlatComboBox     m_wndYear;
	CXTFlatComboBox     m_wndTime;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatistic)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CXTToolBar    m_wndToolBar;


	void ChangeTimeStr();

	// Generated message map functions
	//{{AFX_MSG(CStatistic)
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnSelchangeComboYear();
	afx_msg void OnSelchangeComboTime();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTIC_H__D05DE708_ADF7_4039_A7F4_B64A58B25578__INCLUDED_)
