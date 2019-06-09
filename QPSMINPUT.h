#if !defined(AFX_QPSMINPUT_H__44664559_B1B7_43A2_8BFD_26DE8D400BA8__INCLUDED_)
#define AFX_QPSMINPUT_H__44664559_B1B7_43A2_8BFD_26DE8D400BA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QPSMINPUT.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CQPSMINPUT dialog

class CQPSMINPUT : public CDialog
{
// Construction
public:
	CQPSMINPUT(CWnd* pParent = NULL);   // standard constructor
	CString m_qpsm;
// Dialog Data
	//{{AFX_DATA(CQPSMINPUT)
	enum { IDD = IDD_DIALOG_QPSMBATCH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQPSMINPUT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CQPSMINPUT)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QPSMINPUT_H__44664559_B1B7_43A2_8BFD_26DE8D400BA8__INCLUDED_)
