// CMedDiagnose : header file
/////////////////////////////////////////////////////////////////////////////

#ifndef __MEDDIAGNOSE_H
#define __MEDDIAGNOSE_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CMedDiagnose class
#include "MenuEdit.h"
#include "DlgListInput.h"
#include "MyEdit.h"
#include "CannedTextTree.h"

class CZdex;
class CNormalWord;
class CMedDiagnose : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CMedDiagnose)

// Construction / destruction
public:
	UINT CurEdit;
	CNormalWord *pNormalWord;
	CZdex *pZdex;
	void SwitchNormalWnd();

	// Constructs a CMedDiagnose object.
	CMedDiagnose();

	// Destroys a CMedDiagnose object, handles cleanup and de-allocation.
	virtual ~CMedDiagnose();

	void RefreshBggs();
    
	CDlgListInput  *pListInput;
	
	//{{AFX_DATA(CMedDiagnose)
	enum { IDD = IDD_DIALOG_MEDDIAGNOSE };
	CXTButton	m_btBlh;
	CXTButton	m_Button_Zdmb;
	CXTButton	m_Button_Qppj;
	CXTButton	m_Button_Cyc;
	CXTButton	m_Button_Bc;
	CXTButton 	m_Button_Qcmx;
	CXTButton	m_Button_Bdbg;
	CXTButton	m_Button_Xgyj;
	CXTButton	m_Button_Knhz;
	CXTButton	m_Button_Bcbg;
	CXTButton	m_Button_Sfjg;
	CXTButton	m_Button_Tjyz;
	CXTButton	m_Button_Jsyz;
	CXTButton	m_Button_Print;

	CCannedTextTree	m_TreeCtrl_RYSJ;
	CCannedTextTree	m_TreeCtrl_GJSJ;	
	CCannedTextTree m_TreeCtrl_ZDYJ;

	CMenuEdit	m_ctrl_shys;
	CMenuEdit	m_ctrl_zzys;
	CMenuEdit	m_ctrl_czys;
	CMenuEdit	m_ctrl_zdfh;
	CMenuEdit	m_ctrl_sfyx;
	CMyEdit		m_ctrl_zdyj;
	CMyEdit		m_ctrl_gjsj;
	CMyEdit		m_ctrl_rysj;
	CString	m_rysj;
	CString	m_gjsj;
	CString	m_zdyj;
	CString	m_zdbm;
	CString	m_zdgjc;
	CString	m_sfyx;
	CString	m_zdfh;
	CString	m_czys;
	CString	m_zzys;
	CString	m_shys;
	CString	m_bggs;
	//}}AFX_DATA

// Member variables
protected:
	CXTDateTimeCtrl m_Time_Bgrq;
	CImageList	m_ImageList;
	CXTCoolMenu m_coolMenu;
	int  m_CurrentTreeMode;

	UINT m_nCurrentTreeCtrlID;
	CXTTreeCtrl *m_pCurrentTreeCtrl;

	void ResetButtonTextColor(bool);
	void RefreshBgzt();

	bool m_isModified;

    CToolTipCtrl  m_ToolTip_RYSJ;
    CToolTipCtrl  m_ToolTip_GJSJ;
    CToolTipCtrl  m_ToolTip_ZDYJ;

	char m_Fanben[1005];
private:
	bool	m_bIsFirstInit;

// Member functions
public:
	void UpdateTreeCtrl(int nMode, BOOL bRebuildTree = TRUE);
	bool GetModifiedFlag() { return  m_isModified; };
	void SetModifiedFlag();

	//{{AFX_VIRTUAL(CMedDiagnose)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMedDiagnose)
	afx_msg void OnButtonCyc();
	afx_msg void OnDestroy();
	afx_msg void OnSetfocusEditRysj();
	afx_msg void OnSetfocusEditGjsj();
	afx_msg void OnSetfocusEditZdyj();
	afx_msg void OnButtonZdmb();
	afx_msg void OnButtonBc();
	afx_msg void OnButtonXgyj();
	afx_msg void OnButtonBdbg();
	afx_msg void OnButtonBcbg();
	afx_msg void OnButtonSfjg();
	afx_msg void OnButtonKnhz();
	afx_msg void OnButtonQcmx();
	afx_msg void OnButtonJsyz();
	afx_msg void OnButtonTjyz();
	afx_msg void OnButtonQppj();
	afx_msg void OnButtonMicd10();
	afx_msg void OnButtonSicd10();
	afx_msg void OnContentModified();
	afx_msg void OnButtonBlh();
	afx_msg void OnSetfocusComboBggs();
	afx_msg void OnSelchangeComboBggs();
	afx_msg void OnCloseupComboBggs();
	afx_msg void OnButtonPrint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	LRESULT OnContentSave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ListSelect(WPARAM wparam, LPARAM lparam);
	afx_msg void OnDateTimeChange(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __MEDDIAGNOSE_H

