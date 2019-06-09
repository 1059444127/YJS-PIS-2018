#if !defined(AFX_ADJUSTDLG_H__E4A8F77C_0EAE_4D6D_A1BF_D3BF60AB4C1A__INCLUDED_)
#define AFX_ADJUSTDLG_H__E4A8F77C_0EAE_4D6D_A1BF_D3BF60AB4C1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdjustDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdjustDlg dialog

class CAdjustDlg : public CDialog
{
	friend class CMainFrame;
// Construction
public:
	CAdjustDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CAdjustDlg)
	enum { IDD = IDD_DLG_ADJUST };
	CComboBox	m_ctlBayer;
	CScrollBar	m_ctrVBlk;
	CScrollBar	m_ctrHBlk;
	CComboBox	m_ctlCMADC;
	CComboBox	m_ctlCMGain;
	CScrollBar	m_ctlGain;
	CString	m_strGain;
	long	m_lTintLower;
	long	m_lTintUpper;
	int		m_nHBlanking;
	int		m_nVBlanking;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdjustDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdjustDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnAdjustDefault();
	afx_msg void OnAdjustReset();
	afx_msg void OnSelchangeCmGain();
	afx_msg void OnSelchangeCmAdc();
	afx_msg void OnChangeEditTintUpper();
	afx_msg void OnChangeEditTintLower();
	afx_msg void OnChangeEditHblanking();
	afx_msg void OnChangeEditVblanking();
	afx_msg void OnSelchangeCmBayer();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	long m_lRstTintUpper;
	long m_lRstTintLower;

	long m_lADCLevel;
	long m_lGain[5];	
	int m_nGainChannel;

	long m_bayerdata;

	int m_nRstVBlanking;
	int m_nRstHBlanking ;	
private:
	void ResetData();
	void SetDefaultData();
	void ResetControl();
	void ResetHVDevice();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADJUSTDLG_H__E4A8F77C_0EAE_4D6D_A1BF_D3BF60AB4C1A__INCLUDED_)
