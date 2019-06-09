#if !defined(AFX_PICNOTE_H__6168F840_3109_45EB_BB0F_163512955752__INCLUDED_)
#define AFX_PICNOTE_H__6168F840_3109_45EB_BB0F_163512955752__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Picnote.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPicnote dialog

class CPicnote : public CDialog
{
// Construction
public:
	CPicnote(CWnd* pParent = NULL);   // standard constructor
	CString title;

// Dialog Data
	//{{AFX_DATA(CPicnote)
	enum { IDD = IDD_DIALOG_PICNOTE };
	CString	m_note;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPicnote)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPicnote)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonQcmx();
	afx_msg void OnSelendokComboWord();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICNOTE_H__6168F840_3109_45EB_BB0F_163512955752__INCLUDED_)
