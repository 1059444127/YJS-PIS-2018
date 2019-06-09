#if !defined(AFX_MOVIESHOW_H__2A001F33_6E9D_4B03_B847_6E4F484627A9__INCLUDED_)
#define AFX_MOVIESHOW_H__2A001F33_6E9D_4B03_B847_6E4F484627A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MovieShow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMovieShow dialog
#include "activemovie.h"

class CMovieShow : public CXTResizeDialog
{
// Construction
public:
	CMovieShow(CWnd* pParent = NULL);   // standard constructor
    BOOL Create();

	void CaptureImg(CString);

// Dialog Data
	//{{AFX_DATA(CMovieShow)
	enum { IDD = IDD_DIALOG_MOVIEWND };
	CActiveMovie	m_ActMovie;
	//}}AFX_DATA
    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMovieShow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd* m_pParent;
	int m_nID;

	void ResizeMovieWnd();
	BOOL WndToBmp(CDC *pDC, CString szFile);
	HANDLE DDBToDIB(CBitmap &bitmap, DWORD dwCompression, CPalette *pPal);

	// Generated message map functions
	//{{AFX_MSG(CMovieShow)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOVIESHOW_H__2A001F33_6E9D_4B03_B847_6E4F484627A9__INCLUDED_)
