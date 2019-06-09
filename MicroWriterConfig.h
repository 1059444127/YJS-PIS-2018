#if !defined(AFX_MICROWRITERCONFIG_H__2FC4AF54_F6D0_44D1_8BB7_20C8F52B1B87__INCLUDED_)
#define AFX_MICROWRITERCONFIG_H__2FC4AF54_F6D0_44D1_8BB7_20C8F52B1B87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MicroWriterConfig.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMicroWriterConfig dialog
#include "INI.h"

class CMicroWriterConfig : public CDialog
{
// Construction
public:
	CMicroWriterConfig(CWnd* pParent = NULL);   // standard constructor
	
	CString m_FontFirstLine;
	CString m_FontSecondLine;
	CString m_FontOneLine;
	CString m_FirstLineText;
	CString m_BeginIndex;
	
// Dialog Data
	//{{AFX_DATA(CMicroWriterConfig)
	enum { IDD = IDD_DIALOG_MicroWriterConfig };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMicroWriterConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CIniReader m_IniReader;
	// Generated message map functions
	//{{AFX_MSG(CMicroWriterConfig)
	afx_msg void OnRADIOTwoLineMode();
	afx_msg void OnRADIOOneLineMode();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONMicroWriterPrint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MICROWRITERCONFIG_H__2FC4AF54_F6D0_44D1_8BB7_20C8F52B1B87__INCLUDED_)
