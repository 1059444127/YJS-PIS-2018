#if !defined(AFX_RECORDINPUT_H__4CF2F5C9_2E92_4C91_828F_4C1A52993D2C__INCLUDED_)
#define AFX_RECORDINPUT_H__4CF2F5C9_2E92_4C91_828F_4C1A52993D2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecordInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRecordInput dialog
#include "RecordInfo.h"
#include "PromptInfo.h"

class CRecordInput : public CXTResizeDialog
{
// Construction
public:
	CRecordInput(CWnd* pParent = NULL);   // standard constructor
	bool m_IsFirstTimePaint;

	CStringArray m_FieldName;
	CStringArray m_FieldList;
    CStringArray m_NameList;

    CRecordInfo m_RecordInfo_Ctrl;
	CPromptInfo m_PromptInfo_Ctrl_First;
	CPromptInfo m_PromptInfo_Ctrl_Second;


// Dialog Data
	//{{AFX_DATA(CRecordInput)
	enum { IDD = IDD_RECORD_INFO };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordInput)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRecordInput)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDINPUT_H__4CF2F5C9_2E92_4C91_828F_4C1A52993D2C__INCLUDED_)
