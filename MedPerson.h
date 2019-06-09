// CMedPerson : header file
/////////////////////////////////////////////////////////////////////////////

#ifndef __MEDPERSON_H
#define __MEDPERSON_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CMedPerson class
#include "MenuEdit.h"
#include "MyEdit.h"

class CMedPerson : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CMedPerson)

// Construction / destruction
public:
	// Constructs a CMedPerson object.
	CMedPerson();

	// Destroys a CMedPerson object, handles cleanup and de-allocation.
	virtual ~CMedPerson();

	//{{AFX_DATA(CMedPerson)
	enum { IDD = IDD_DIALOG_MEDPERSON };
	CXTButton	m_Button_Td;
	CMenuEdit	m_ctrl_jj;
	CXTButton	m_Button_Sqddy;
	CXTButton	m_btBbmcDetail;
	CXTButton	m_Button_Print;
	CXTButton	m_Button_New;
	CXTButton	m_Button_Bc;
	CXTButton	m_Button_Jcxx;
	CXTButton	m_Button_Zlxx;
	CXTButton	m_Button_Fkxx;
	CXTButton	m_btMaxBdh;
	CMenuEdit	m_ctrl_orderType;
	CMenuEdit	m_ctrl_lczd;
	CMenuEdit	m_ctrl_qcys;
	CMenuEdit	m_ctrl_fb;
	CMenuEdit	m_ctrl_bbqs;
	CMenuEdit	m_ctrl_bhgyy;
	CMenuEdit	m_ctrl_bbmc;
	CMenuEdit	m_ctrl_bbqk;
	CMenuEdit	m_ctrl_bblx;
	CMenuEdit	m_ctrl_bq;
	CMenuEdit	m_ctrl_sjys;
	CMenuEdit	m_ctrl_sjks;
	CMenuEdit	m_ctrl_sjdw;
	CMenuEdit	m_ctrl_zy;
	CMenuEdit	m_ctrl_hf;
	CMenuEdit	m_ctrl_mz;
	CMenuEdit	m_ctrl_nldw;
	CMenuEdit	m_ctrl_xb;
	CXTButton	m_btSfDetail;
	CXTButton	m_btHisidExtract;
	CXTButton	m_btHisid1Extract;
	CMyEdit		m_ctrl_bszy;

	CString	m_sqh;
	CString	m_brid;
	CString	m_bdh;
	CString	m_blh;
	CString	m_ybh;
	CString	m_mzh;
	CString	m_zyh;
	CString	m_nldw;
	CString m_csrq;
	CString	m_mz;
	CString	m_hf;
	CString	m_zy;
	CString	m_sfzh;
	CString	m_lxdz;
	CString m_lxdh;
	CString	m_mcyj;
	CString	m_jj;
	CString	m_sjdw;
	CString	m_sjks;
	CString	m_sjys;
	CString	m_bq;
	CString	m_ch;
	CString	m_bblx;
	CString	m_bbqk;
	CString	m_bbmc;
	CString	m_bhgyy;
	CString	m_bbqs;
	CString	m_fb;
	float	m_sf;
	CString	m_qcys;
	CString	m_lczd;
	CString	m_bszy;
	CString	m_sssj;
	CString	m_xm;
	CString	m_xb;
	CString	m_nl;
	//}}AFX_DATA
	CString m_sOrderType;
	CString m_sOrderId;
// Member variables
protected:
	void RefreshHistoryInfo(CString medicalno,CString id, CString ybh, CString xm, CString xb);

	void ResetButtonTextColor(bool);

	bool m_isModified;
private:
	bool	m_bIsFirstInit;

	void ReadMedicalOrder(bool bFromLX);

// Member functions
public:
	CXTDateTimeCtrl m_Time_Csrq, m_Time_Mcyj, m_Time_Sjrq, m_Time_Qcrq;
	static CString GenNewBlh();
	bool   GetModifiedFlag() { return  m_isModified; };
	void   SetModifiedFlag(bool flag) { m_isModified = flag; };

	//{{AFX_VIRTUAL(CMedPerson)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMedPerson)
	afx_msg void OnSfDetail();
	afx_msg void OnButtonBc();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonNew1();
	afx_msg void OnButtonPrint();
	afx_msg void OnHisidExtract();
	afx_msg void OnHisid1Extract();
	afx_msg void OnContentModified();
	afx_msg void OnMaxBdh();
	afx_msg void OnButtonZlxx();
	afx_msg void OnButtonFkxx();
	afx_msg void OnButtonJcxx();
	afx_msg void OnBbmcDetail();
	afx_msg void OnButtonSqddy();
	afx_msg void OnButtonTd();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	LRESULT OnContentSave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDateTimeChange(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __MEDPERSON_H

