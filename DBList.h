// CDBList : header file
/////////////////////////////////////////////////////////////////////////////

#ifndef __DBLIST_H
#define __DBLIST_H

/////////////////////////////////////////////////////////////////////////////
// CDBList class
#include "resource.h"
#include "DBGrid.h"
#include "MenuEdit.h"

class CDBList : public CXTCBarDialog
{
	DECLARE_DYNCREATE(CDBList)

// Construction / destruction
public:
	void openLcjc();
	CDBList(CWnd* pParent = NULL);
	bool GetModifiedFlag() { return  m_isModified; };

	CDBGrid    m_RecordList;

	bool m_state0, m_state1, m_state2, m_state3, m_state4, m_state5, m_state6;
	void RefreshHyperLink();
	void GetLeaveMessage();
	void GetBooking();
// Dialog Data
	//{{AFX_DATA(CDBList)
	enum { IDD = IDD_DIALOG_DBLIST };
	CXTHyperLink	m_HypLink_Bdyy;
	CXTHyperLink	m_HypLink_Knly;
	CXTHyperLink	m_HypLink_Wfyy;
	CXTHyperLink	m_HypLink_Sfbl;
	CXTHyperLink	m_HypLink_Hzbl;
	CXTHyperLink	m_HypLink_Bqlb;
	CXTHyperLink	m_HypLink_Lsjc;
	CMenuEdit	m_ctrl_wfyy;
	CString	m_wfyy;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDBList)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CXTToolBar    m_wndToolBar;

	bool m_isModified;

    CXTDateTimeCtrl m_monthCal;

	// Generated message map functions
	//{{AFX_MSG(CDBList)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblistState1();
	afx_msg void OnUpdateDblistState1(CCmdUI* pCmdUI);
	afx_msg void OnDblistState2();
	afx_msg void OnUpdateDblistState2(CCmdUI* pCmdUI);
	afx_msg void OnDblistState3();
	afx_msg void OnUpdateDblistState3(CCmdUI* pCmdUI);
	afx_msg void OnDblistState4();
	afx_msg void OnUpdateDblistState4(CCmdUI* pCmdUI);
	afx_msg void OnDblistState0();
	afx_msg void OnUpdateDblistState0(CCmdUI* pCmdUI);
	afx_msg void OnButtonBlh();
	afx_msg void OnHypLsjc();
	afx_msg void OnHypBqlb();
	afx_msg void OnButtonRefresh();
	afx_msg void OnHypHzbl();
	afx_msg void OnHypSfbl();
	afx_msg void OnDblistState5();
	afx_msg void OnUpdateDblistState5(CCmdUI* pCmdUI);
	afx_msg void OnDblistState6();
	afx_msg void OnUpdateDblistState6(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnHypKnly();
	afx_msg void OnContentModified();
	afx_msg void OnUpdateDatesel(CCmdUI* pCmdUI);
	afx_msg void OnHypBdyy();
	//}}AFX_MSG
	LRESULT OnContentSave(WPARAM wParam, LPARAM lParam);
	LRESULT OnContentRefresh(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif // __DBLIST_H

