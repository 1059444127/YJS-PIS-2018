#if !defined(AFX_DLGPROPERTYLINE_H__CC1090A2_B365_4316_B172_244A104FAE90__INCLUDED_)
#define AFX_DLGPROPERTYLINE_H__CC1090A2_B365_4316_B172_244A104FAE90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPropertyLine.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPropertyLine dialog

class CDlgPropertyLine : public CDialog
{
// Construction
public:
	CDlgPropertyLine(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPropertyLine)
	enum { IDD = IDD_PROPERTY_LINE };
	CFODropLineWidthButton	m_btnLineWidth;
	CFODropLineTypeButton	m_btnLineType;
	CFODropColorButton	m_btnLineColor;
	//}}AFX_DATA

	int m_nLineWidth;
	int m_nLineType;
	COLORREF m_crLineColor;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPropertyLine)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPropertyLine)
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROPERTYLINE_H__CC1090A2_B365_4316_B172_244A104FAE90__INCLUDED_)
