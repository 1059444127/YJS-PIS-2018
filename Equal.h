#if !defined(AFX_EQUAL_H__A25077B4_D5CB_43CF_B03C_A73D11CB74AE__INCLUDED_)
#define AFX_EQUAL_H__A25077B4_D5CB_43CF_B03C_A73D11CB74AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Equal.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEqual dialog

class CEqual : public CDialog
{
// Construction
public:
	CEqual(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEqual)
	enum { IDD = IDD_DIALOG_EQUAL };
	CSliderCtrl	m_slider_green;
	CSliderCtrl	m_slider_blue;
	CSliderCtrl	m_slider_red;
	CSliderCtrl	m_slider_brightness;
	CSliderCtrl	m_slider_contrast;
	CSliderCtrl	m_slider_gamma;
	//}}AFX_DATA

	CWnd * m_pParent;
	UINT  m_TimerHandle;
	HDIB  hBakDIB;
    int prethreshold_brightness,prethreshold_contrast,prethreshold_gamma;
    int prethreshold_red,prethreshold_green,prethreshold_blue;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEqual)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEqual)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EQUAL_H__A25077B4_D5CB_43CF_B03C_A73D11CB74AE__INCLUDED_)
