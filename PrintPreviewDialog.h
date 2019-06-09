#if !defined(StatisticPrintPreviewDialog__INCLUDED_)
#define StatisticPrintPreviewDialog__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticPrintPreviewDialog.h : header file
//
// Note that this is a base class, not meant to be built itself.
// You should inherit from it

/////////////////////////////////////////////////////////////////////////////
// CPrintPreviewDialog dialog
class CDlgDummyView;
#include "FormDataModel.h"

class __declspec(dllexport) CPrintPreviewDialog : public CXTCBarDialog
{
// Construction
public:
	CPrintPreviewDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL) : CXTCBarDialog(nIDTemplate, pParentWnd)
	{
		m_UsageType = 0;
		m_pTemplate = NULL;
		m_bPrintPreview = false;
		m_pView = NULL;
		m_pFormDataModel = new CFormDataModel;
	};
	virtual ~CPrintPreviewDialog()
	{
		if(m_pTemplate)
		{
			delete m_pTemplate;
			m_pTemplate = NULL;
		}
		if(m_pFormDataModel != NULL)
		{
			delete m_pFormDataModel;
			m_pFormDataModel = NULL;
		}
		m_strlist.RemoveAll();
		m_titlelist.RemoveAll();
	};
	void EndPreviewMode();

	CStringArray m_strlist;
	CStringArray m_titlelist;
	CFormDataModel  *m_pFormDataModel;

	bool		m_bPrintPreview;
	int m_UsageType;

// Dialog Data
	//{{AFX_DATA(CPrintPreviewDialog)
	//}}AFX_DATA
private:
	CDlgDummyView* m_pView;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintPreviewDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void GenList();
	//}}AFX_VIRTUAL
public:

// Implementation
protected:
	CSingleDocTemplate*	m_pTemplate;
	// Generated message map functions
	//{{AFX_MSG(CPrintPreviewDialog)
	afx_msg void OnDialogPrint();
	afx_msg void OnDialogPrintPreview();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(StatisticPrintPreviewDialog__INCLUDED_)
