#if !defined(AFX_DLGROTATION_H__3B6EE97D_70CA_4E8A_9023_7A11ABAD502A__INCLUDED_)
#define AFX_DLGROTATION_H__3B6EE97D_70CA_4E8A_9023_7A11ABAD502A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRotation.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRotation dialog
#include "BitmapPicture.h"
#include "amsEdit.h"

class CDlgRotation : public CDialog
{
// Construction
public:
	void BeforeRotate(int nWidth, int nHeight, int &nNewWidth, int &nNewHeight, float fRotateAngle);
	CDlgRotation(CWnd* pParent = NULL);   // standard constructor
	~CDlgRotation();

	CStringArray m_File_List;
// Dialog Data
	//{{AFX_DATA(CDlgRotation)
	enum { IDD = IDD_DIALOG_ROTATION };
	CAMSNumericEdit	m_edtRotationAngle;
	CBitmapPicture	m_staticImage2;
	CBitmapPicture	m_staticImage1;
	CSpinButtonCtrl	m_spinAngle;
	int		m_nRotateStyle;
	float	m_editAngle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRotation)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRotation)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioRotateStyle();
	virtual void OnOK();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGROTATION_H__3B6EE97D_70CA_4E8A_9023_7A11ABAD502A__INCLUDED_)
