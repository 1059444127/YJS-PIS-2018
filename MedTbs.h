// CMedTbs : header file
/////////////////////////////////////////////////////////////////////////////

#ifndef __MEDTBS_H
#define __MEDTBS_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CMedTbs class
#include "MenuEdit.h"

#include "CannedTextTree.h"

class CNormalWord;
class CMedTbs : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CMedTbs)

// Construction / destruction
public:
	UINT CurEdit;
	CNormalWord *pNormalWord;
	void SwitchNormalWnd();

	// Constructs a CMedTbs object.
	CMedTbs();

	// Destroys a CMedTbs object, handles cleanup and de-allocation.
	virtual ~CMedTbs();

    void RefreshBggs();

	//{{AFX_DATA(CMedTbs)
	enum { IDD = IDD_DIALOG_TBS };
	CMenuEdit	m_ctrl_bczd;
	CMenuEdit	m_ctrl_zzys;
	CMenuEdit	m_ctrl_shys;
	CMenuEdit	m_ctrl_sfyx;
	CMenuEdit	m_ctrl_zdfh;

	CCannedTextTree m_TreeCtrl_RYSJ;
	CCannedTextTree m_TreeCtrl_ZDYJ;
	
	CString	m_bggs;
	CString	m_zdyj;
	CString	m_shys;
	CString	m_zzys;
	CString	m_bczd;
	CString	m_rysj;
	CString	m_zdgjc;
	CString	m_zdfh;
	CString m_sfyx;
	//}}AFX_DATA

// Member variables
protected:
	CXTDateTimeCtrl m_Time_Bgrq;
	CImageList	m_ImageList;
	CXTCoolMenu m_coolMenu;
	int  m_CurrentTreeMode;

	UINT m_nCurrentTreeCtrlID;
	CXTTreeCtrl *m_pCurrentTreeCtrl;

	bool m_isModified;

    CToolTipCtrl  m_ToolTip_RYSJ, m_ToolTip_ZDYJ;

	char m_Fanben[1005];

	void RefreshBgzt();

private:
	bool	m_bIsFirstInit;

// Member functions
public:
	void UpdateTreeCtrl(int nMode, BOOL bRebuildTree = TRUE);
	bool GetModifiedFlag() { return  m_isModified; };
	void SetModifiedFlag();

	//{{AFX_VIRTUAL(CMedTbs)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMedTbs)
	afx_msg void OnDestroy();
	afx_msg void OnSetfocusEditZdyj();
	afx_msg void OnButtonCyc();
	afx_msg void OnButtonBc();
	afx_msg void OnCheckJbmyd1();
	afx_msg void OnCheckJbmyd2();
	afx_msg void OnCheckJbmyd3();
	afx_msg void OnCheckXbl1();
	afx_msg void OnCheckXbl2();
	afx_msg void OnCheckYzfy1();
	afx_msg void OnCheckYzfy2();
	afx_msg void OnCheckYzfy3();
	afx_msg void OnSetfocusEditRysj();
	afx_msg void OnContentModified();
	afx_msg void OnButtonQppj();
	afx_msg void OnButtonZdmb();
	afx_msg void OnButtonTjyz();
	afx_msg void OnCloseupComboBggs();
	afx_msg void OnSetfocusComboBggs();
	afx_msg void OnSelchangeComboBggs();
	afx_msg void OnButtonPrint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	LRESULT OnContentSave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDateTimeChange(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __MEDTBS_H

