#if !defined(AFX_MANTENANCEPICNOTE_H__7E79115D_8BA9_41EA_A749_FA2428037E3D__INCLUDED_)
#define AFX_MANTENANCEPICNOTE_H__7E79115D_8BA9_41EA_A749_FA2428037E3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MantenancePicNote.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMantenancePicNote dialog

class CMantenancePicNote : public CDialog
{
// Construction
public:
	CMantenancePicNote(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMantenancePicNote)
	enum { IDD = IDD_DIALOG_PICNOTEMAINTENANCE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMantenancePicNote)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMantenancePicNote)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANTENANCEPICNOTE_H__7E79115D_8BA9_41EA_A749_FA2428037E3D__INCLUDED_)
