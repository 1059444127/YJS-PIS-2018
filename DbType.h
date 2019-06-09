#if !defined(AFX_DBTYPE_H__4914A89D_F05F_4A28_BAD0_FDA4D9BB1561__INCLUDED_)
#define AFX_DBTYPE_H__4914A89D_F05F_4A28_BAD0_FDA4D9BB1561__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DbType.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDbType dialog

class CDbType : public CDialog
{
// Construction
public:
	CDbType(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDbType)
	enum { IDD = IDD_DIALOG_DBTYPE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDbType)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void Refresh();

	// Generated message map functions
	//{{AFX_MSG(CDbType)
	afx_msg void OnDelete();
	afx_msg void OnInsert();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBTYPE_H__4914A89D_F05F_4A28_BAD0_FDA4D9BB1561__INCLUDED_)
