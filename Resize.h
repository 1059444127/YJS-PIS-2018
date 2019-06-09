#if !defined(AFX_RESIZE_H__CDCBDB6C_CA33_4C45_AD3D_BE4896A37138__INCLUDED_)
#define AFX_RESIZE_H__CDCBDB6C_CA33_4C45_AD3D_BE4896A37138__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Resize.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CResize dialog

class CResize : public CDialog
{
// Construction
public:
	CResize(CWnd* pParent = NULL);   // standard constructor

	bool m_flag;
// Dialog Data
	//{{AFX_DATA(CResize)
	enum { IDD = IDD_DIALOG_RESIZE };
	int		m_height;
	int		m_width;
	int		m_heightrate;
	int		m_widthrate;
	BOOL	m_distort;
	int		m_SampleMethod;
	float	m_factor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResize)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_old_width,m_old_height;

	// Generated message map functions
	//{{AFX_MSG(CResize)
	afx_msg void OnUpdateEditHeight();
	afx_msg void OnUpdateEditHeightrate();
	afx_msg void OnUpdateEditWidth();
	afx_msg void OnUpdateEditWidthrate();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESIZE_H__CDCBDB6C_CA33_4C45_AD3D_BE4896A37138__INCLUDED_)
