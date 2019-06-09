#if !defined(AFX_NUMINPUT_H__4225881B_DC20_4B89_88EF_9F0D5196ED72__INCLUDED_)
#define AFX_NUMINPUT_H__4225881B_DC20_4B89_88EF_9F0D5196ED72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NumInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NumInput dialog

class NumInput : public CDialog
{
// Construction
public:
	NumInput(CWnd* pParent = NULL);	// standard constructor
	NumInput(int left,int top,CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(NumInput)
	enum { IDD = IDD_NUMINPUT };
	CString	m_Data;
	//}}AFX_DATA

// Implementation
protected:
	int m_Left,m_top;
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(NumInput)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NUMINPUT_H__4225881B_DC20_4B89_88EF_9F0D5196ED72__INCLUDED_)
