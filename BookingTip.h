#if !defined(AFX_BOOKINGTIP_H__613C62BD_BD20_497C_B682_AEB21FFF74B2__INCLUDED_)
#define AFX_BOOKINGTIP_H__613C62BD_BD20_497C_B682_AEB21FFF74B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BookingTip.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBookingTip dialog

class CBookingTip : public CDialog
{
// Construction
public:
	CBookingTip(CWnd* pParent = NULL);   // standard constructor
	CString count;
// Dialog Data
	//{{AFX_DATA(CBookingTip)
	enum { IDD = IDD_DIALOG_BookingTip };
	CXTHyperLink m_link_bdyy;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBookingTip)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBookingTip)
	afx_msg void OnButtonBooking();
	afx_msg void OnLinkBdyy();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOOKINGTIP_H__613C62BD_BD20_497C_B682_AEB21FFF74B2__INCLUDED_)
