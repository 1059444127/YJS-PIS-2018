#if !defined(AFX_STANDINPUT_H__AE45116A_D540_471E_A83B_A6670D8CC487__INCLUDED_)
#define AFX_STANDINPUT_H__AE45116A_D540_471E_A83B_A6670D8CC487__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StandInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStandInput dialog

class CStandInput : public CDialog
{
// Construction
public:
	CStandInput(CWnd* pParent = NULL);   // standard constructor

	double m_PixelLength;

// Dialog Data
	//{{AFX_DATA(CStandInput)
	enum { IDD = IDD_DIALOG_DINGBIAO };
	int		m_zoom;
	double	m_length;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStandInput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStandInput)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STANDINPUT_H__AE45116A_D540_471E_A83B_A6670D8CC487__INCLUDED_)
