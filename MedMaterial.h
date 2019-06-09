// CMedMaterial : header file
/////////////////////////////////////////////////////////////////////////////

#ifndef __MEDMATERIAL_H
#define __MEDMATERIAL_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CMedMaterial class
#include "MenuEdit.h"
#include "MaterialGrid.h"
#include "MyEdit.h"
#include "MedWax.h"

#include "CannedTextTree.h"

class CNormalWord;
class CMedMaterial : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CMedMaterial)

// Construction / destruction
public:
	CSingleDocTemplate* pDocTemplate;
	CMedWax* pView;
	CStringArray writerCommandList;

	UINT CurEdit;
	CNormalWord *pNormalWord;

	CMaterialGrid  m_MaterialList;

	// Constructs a CMedMaterial object.
	CMedMaterial();

	//int i;

	// Destroys a CMedMaterial object, handles cleanup and de-allocation.
	virtual ~CMedMaterial();

	//{{AFX_DATA(CMedMaterial)
	enum { IDD = IDD_DIALOG_MEDMATERIAL };
	CXTHyperLink	m_HypLink_Note;
	CMenuEdit	m_ctrl_qcbw;
	CMenuEdit	m_ctrl_qcys;
	CXTHyperLink	m_HypLink_Bqxx;
	CMenuEdit		m_ctrl_qcfy;
	CCannedTextTree	m_TreeCtrl_RYSJ;
	CCannedTextTree	m_TreeCtrl_BWMX;
	CMenuEdit	m_ctrl_qcjlr;
	CMenuEdit	m_ctrl_bbwz;
	CMyEdit		m_ctrl_rysj;
	int		m_lkzs;
	int		m_ckzs;
	int		m_bgzt;
	CString	m_qcfy;
	CString	m_bbwz;
	CString	m_qcjlr;
	CString	m_rysj;
	//}}AFX_DATA

// Member variables
protected:
	CImageList	m_ImageList;
	CXTCoolMenu m_coolMenu;
	CXTTreeCtrl *m_pCurrentTreeCtrl;
	int  m_CurrentTreeMode;

	CXTDateTimeCtrl m_Time_Qcrq;

	void RefreshBqInfo();

	bool m_isModified;
	
    CToolTipCtrl  m_ToolTip_RYSJ;
    CToolTipCtrl  m_ToolTip_BWMX;

	char m_Fanben[1005];
private:
	bool	m_bIsFirstInit;

// Member functions
public:
	static CString GenNewSliceNo(CString blh);
	void UpdateTreeCtrl(int mode, BOOL bRebuildTree = TRUE);
	bool GetModifiedFlag() { return  m_isModified; };
	void SetModifiedFlag();

	//{{AFX_VIRTUAL(CMedMaterial)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMedMaterial)
	afx_msg void OnSetfocusEditRysj();
	afx_msg void OnButtonCyc();
	afx_msg void OnButtonBc();
	afx_msg void OnDestroy();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	afx_msg void OnContentModified();
	afx_msg void OnButtonZdmb();
	afx_msg void OnButtonPrint();
	afx_msg void OnButtonZc();
	afx_msg void OnMatearialYz();
	afx_msg void OnButtonQppj();
	afx_msg void OnMATEARIALWriterPrint();
	afx_msg void OnEmbedBoxPrint();
	afx_msg void OnMATEARIALSelAll();
	afx_msg void OnButtonWax();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	LRESULT OnContentSave(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __MEDMATERIAL_H

