#if !defined(AFX_CHARGEDETAIL_H__F2849FE3_3F01_4DFC_ABA4_78B6C02FBFC8__INCLUDED_)
#define AFX_CHARGEDETAIL_H__F2849FE3_3F01_4DFC_ABA4_78B6C02FBFC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChargeDetail.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChargeDetail dialog
#include "SfDetailGrid.h"
#include "resource.h"

struct CHARGEITEM_RECORD
{
	CHARGEITEM_RECORD()
	{
		code[0]     = 0x00;
		item[0]     = 0x00;
		SinglePrice = 0.0;
		Type        = 0;
		Unit[0]     = 0x00;
		Num         = 1;
		price       = 0.0;
		Note[0]     = 0x00;
        Recorder[0] = 0x00;
		Reserved[0] = 0x00;
		Reserved1[0] = 0x00;
	}
	
	char   code[100];     // �շѴ���
	char   item[100];     // �շ�����
	float  SinglePrice;   // �����շѣ�Ԫ��
	int    Type;          // ������� 0-סԺ 1-����
	char   Unit[100];     // ��λ
	int    Num;           // ����
	float  price;         // �����ܷ��ã�Ԫ��
	char   Note[500];     // ������ע
	char   Recorder[100]; // �Ʒ�Ա
	char   Reserved[100]; // ����    ;ɽ������ҽ��Ժ--Item_class
	char   Reserved1[100]; // ����    ;ɽ������ҽ��Ժ--item_spec 
};

class CMedPerson;
class CChargeDetail : public CXTCBarDialog
{
// Construction / destruction
public:
	CChargeDetail(CWnd* pParent = NULL);	// standard constructor
	CMedPerson *m_pParent;

	virtual afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult );

// Dialog Data
	//{{AFX_DATA(CChargeDetail)
	enum { IDD = IDD_DIALOG_SFDETAIL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Member variables
protected:
	HICON m_hIcon;
	CXTToolBar    m_wndToolBar;

	BOOL GetToolText( UINT nID, CString& strTipText, CString& /*strMessage*/);

// Member functions
public:
	CString getTitle();
    CSfDetailGrid   m_SfDetailCtrl;
	int m_Original_Item_Num;

	CArray<CHARGEITEM_RECORD,CHARGEITEM_RECORD> SfDetailArray;
	CArray<CHARGEITEM_RECORD,CHARGEITEM_RECORD> ChargeDBArray;
	CString m_LabelText;

	CString m_blh;

	CStringArray ITEM_CODE_List;
	CArray<double,double> ParamList;

	//{{AFX_VIRTUAL(CChargeDetail)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChargeDetail)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnChargeitemNew();
	afx_msg void OnChargeitemDelete();
	afx_msg void OnChargeitemAllitem();
	//}}AFX_MSG
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARGEDETAIL_H__F2849FE3_3F01_4DFC_ABA4_78B6C02FBFC8__INCLUDED_)
