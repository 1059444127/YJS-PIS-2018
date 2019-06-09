#if !defined(AFX_BPSMHD_H__5312763B_6110_4723_9D0F_D15B19E7AC6D__INCLUDED_)
#define AFX_BPSMHD_H__5312763B_6110_4723_9D0F_D15B19E7AC6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Bpsmhd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBpsmhd dialog

class CMedSlice;


class CBpsmhd : public CDialog
{
// Construction
public:
	CBpsmhd(CWnd* pParent = NULL);   // standard constructor

	//CMedSlice* m_pParent;
	CMedSlice* m_pParent;
	

// Dialog Data
	//{{AFX_DATA(CBpsmhd)
	enum { IDD = IDD_DIALOG_BPSMHD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBpsmhd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString   m_CodeString;

	// Generated message map functions
	//{{AFX_MSG(CBpsmhd)
	afx_msg void OnConform();
	afx_msg void OnChangeEditCode();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BPSMHD_H__5312763B_6110_4723_9D0F_D15B19E7AC6D__INCLUDED_)
