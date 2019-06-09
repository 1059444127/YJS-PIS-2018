#if !defined(AFX_SELFINPUT_H__E2AE56BD_E05F_47F9_9BB7_7B2D75360424__INCLUDED_)
#define AFX_SELFINPUT_H__E2AE56BD_E05F_47F9_9BB7_7B2D75360424__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelfInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelfInput dialog
#include "SelfGrid.h"
#include "SelfPrompt.h"

class CSelfInput : public CXTResizeDialog
{
// Construction
public:
	CSelfInput(CWnd* pParent = NULL);   // standard constructor
	bool m_IsFirstTimePaint;
	CString m_InputBlh, m_Operation_Blh;

	CStringArray m_FieldName;
    CStringArray m_NameList;
	CStringArray m_FieldValue;
	CStringArray m_FieldList;
	CUIntArray   m_TypeList;

    CSelfGrid   m_RecordInfo_Ctrl;
	CSelfPrompt m_PromptInfo_Ctrl_First;
	CSelfPrompt m_PromptInfo_Ctrl_Second;

	int m_ExitCode;

// Dialog Data
	//{{AFX_DATA(CSelfInput)
	enum { IDD = IDD_RECORD_SELF };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelfInput)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void SaveRecord(bool IsInsertOnly);

	// Generated message map functions
	//{{AFX_MSG(CSelfInput)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnNew();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnPrevious();
	afx_msg void OnNext();
	afx_msg void OnCapture();
	afx_msg void OnPrint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELFINPUT_H__E2AE56BD_E05F_47F9_9BB7_7B2D75360424__INCLUDED_)
