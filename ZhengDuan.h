#if !defined(AFX_ZHENGDUAN_H__0293721E_BCE9_47D8_8A50_4B3A1DA1AAC0__INCLUDED_)
#define AFX_ZHENGDUAN_H__0293721E_BCE9_47D8_8A50_4B3A1DA1AAC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZhengDuan.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CZhengDuan dialog

class CZhengDuan : public CDialog
{
// Construction
public:
	CZhengDuan(CWnd* pParent = NULL);	// standard constructor
	CZhengDuan(CString filenm,CWnd* pParent =NULL);

// Dialog Data
	//{{AFX_DATA(CZhengDuan)
	enum { IDD = IDD_ZHENDUAN };
	int		m_Buwei;
	CString	m_Tishi2;
	//}}AFX_DATA

// Implementation
protected:
	CString m_filenm;
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void Change(int Buwei,CString Strbuf);	 
	void Get(int Buwei,CString &Strbuf);
	CStringArray ZhengDuanName;
	CWordArray ZhengDuanLen;
	
	// Generated message map functions
	//{{AFX_MSG(CZhengDuan)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	virtual void OnOK();
	afx_msg void OnSelchangeList1();
	virtual BOOL OnInitDialog();
	afx_msg void OnUp();
	afx_msg void OnDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZHENGDUAN_H__0293721E_BCE9_47D8_8A50_4B3A1DA1AAC0__INCLUDED_)
