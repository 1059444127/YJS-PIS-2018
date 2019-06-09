#if !defined(AFX_DSHOWSETUP_H__C9BF8871_3B25_4A28_A7C7_90003E4474C9__INCLUDED_)
#define AFX_DSHOWSETUP_H__C9BF8871_3B25_4A28_A7C7_90003E4474C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DShowSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDShowSetup dialog

class CDShowSetup : public CDialog
{
// Construction
public:
	CDShowSetup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDShowSetup)
	enum { IDD = IDD_DIALOG_DSHOW };
	int		m_device;
	int		m_format;
	int		m_input;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDShowSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDShowSetup)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSHOWSETUP_H__C9BF8871_3B25_4A28_A7C7_90003E4474C9__INCLUDED_)
