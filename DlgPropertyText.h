#if !defined(AFX_DLGPROPERTYTEXT_H__1F984CE2_F564_433C_84DE_058766C6D28F__INCLUDED_)
#define AFX_DLGPROPERTYTEXT_H__1F984CE2_F564_433C_84DE_058766C6D28F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPropertyText.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPropertyText dialog

class CDlgPropertyText : public CDialog
{
// Construction
public:
	CDlgPropertyText(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPropertyText)
	enum { IDD = IDD_PROPERTY_TEXT };
	CFOFontNameCombo   m_FontFaceName;
	BOOL               m_bHorzAlign;
    BOOL               m_bVertAlign;
	CString	m_strContent;
	//}}AFX_DATA

    // Multi lines
	BOOL			   m_bMultiLine;

	// font.
	FO_FONT			   m_Font;


	//Button Text Color Button
	CFODropColorButton	m_btnTextColor;

	// Get font info.
	virtual void GetFontInfo(FO_FONT* pFontSet);

	// Set font info.
	virtual void SetFontInfo(FO_FONT *pFontSet);

	// Init font size.
	virtual void InitFontSize();

	BOOL IsAllDigits(LPCTSTR lpStr);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPropertyText)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPropertyText)
    virtual BOOL OnInitDialog();
	afx_msg void OnChkMultiline();
	afx_msg void OnChkFontstyleBold();
	afx_msg void OnChkFontstyleItalic();
	afx_msg void OnChkFontstyleUnderline();
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROPERTYTEXT_H__1F984CE2_F564_433C_84DE_058766C6D28F__INCLUDED_)
