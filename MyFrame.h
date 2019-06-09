#if !defined(AFX_MYFRAME_H__14F224CA_E542_4625_A9D5_E2E738F319FD__INCLUDED_)
#define AFX_MYFRAME_H__14F224CA_E542_4625_A9D5_E2E738F319FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyFrame frame

class CMyFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CMyFrame)
protected:
	CMyFrame();           // protected constructor used by dynamic creation
	
// Attributes
public:
	CWnd* old;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFrame)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyFrame();

	// Generated message map functions
	//{{AFX_MSG(CMyFrame)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFRAME_H__14F224CA_E542_4625_A9D5_E2E738F319FD__INCLUDED_)
