#if !defined(AFX_PROGRESS_H__37088960_1CAB_11D6_9CA2_002018A1F394__INCLUDED_)
#define AFX_PROGRESS_H__37088960_1CAB_11D6_9CA2_002018A1F394__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Progress.h : header file
//

#include "prType.h"
#include "Resource.h"


/////////////////////////////////////////////////////////////////////////////
// CProgress dialog

class CProgress : public CDialog
{
// Construction
public:
	CProgress(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProgress)
	enum { IDD = IDD_PROGRESS };
	CProgressCtrl	m_CProgress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProgress)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

public:
	char			m_szSavePath[MAX_PATH];
	char			m_szFileName[MAX_PATH];

	CString			GetFileName() { return m_szFileName; }

	//int             year,month,day,hour,minute,second;
public:
//	void			SetProgressPos( int	iPos );

private:
	static prResponse prSTDCALL ReleaseProgressFunc(	prHandle       CameraHandle,
														prObjectHandle ObjectHandle,
														prContext      Context,
														prProgress*    pProgress	);

	static UINT AFX_CDECL	ReleaseThreadProc( LPVOID	lpParameter );

public:
	UINT					m_ThreadEndMessage;
	prResponse				m_LastErr;
	prHandle        		m_CameraHandle;
	prObjectHandle  		m_ObjectHandle;
	prptpEventCode  		m_EventCode;
	prptpObjectFormatCode	m_SavedFileFormat;
	CFile*					m_pCFile;
	CWinThread*				m_ReleaseThread;


	BOOL					GetReleaseData(	prHandle        		CameraHandle,
											prObjectHandle  		ObjectHandle,
											prptpEventCode  		EventCode,
											prptpObjectFormatCode	SavedFileFormat,
											char					*szSavePath,
											char					*szFileName
											);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESS_H__37088960_1CAB_11D6_9CA2_002018A1F394__INCLUDED_)
