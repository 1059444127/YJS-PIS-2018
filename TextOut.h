#if !defined(AFX_TEXTOUT_H__13930916_AB9E_4DD3_AE1F_DB420D844E8C__INCLUDED_)
#define AFX_TEXTOUT_H__13930916_AB9E_4DD3_AE1F_DB420D844E8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextOut.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextOut dialog

class CTextOut : public CXTResizeDialog
{
// Construction
public:
	CTextOut(CWnd* pParent = NULL);   // standard constructor
	int m_handle;

	CString m_Title;
// Dialog Data
	//{{AFX_DATA(CTextOut)
	enum { IDD = IDD_DIALOG_TEXT };
	CString	m_Text;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextOut)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    bool m_isFirst;
	// Generated message map functions
	//{{AFX_MSG(CTextOut)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTOUT_H__13930916_AB9E_4DD3_AE1F_DB420D844E8C__INCLUDED_)
