// BqlbShow.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "BqlbShow.h"
#include "PathologyView.h"
#include "MedMaterial.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBqlbShow dialog
extern CPathologyApp theApp;

CBqlbShow::CBqlbShow(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CBqlbShow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBqlbShow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBqlbShow::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBqlbShow)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBqlbShow, CXTResizeDialog)
	//{{AFX_MSG_MAP(CBqlbShow)
	ON_BN_CLICKED(IDC_BUTTON_BQZX, OnButtonBqzx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBqlbShow message handlers

BOOL CBqlbShow::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_grid.AttachGrid(this,IDC_STATIC_RECORDLIST);
	m_grid.SetParent(this);
	m_grid.ResetGridSize();
	m_grid.RedrawAll();
	
	SetResize(IDC_STATIC_RECORDLIST, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_BQZX, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	if(theApp.m_StationType.CompareNoCase("È¡²Ä¹¤×÷Õ¾"))
	{
		GetDlgItem(IDC_BUTTON_BQZX)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_BQZX)->EnableWindow(TRUE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBqlbShow::OnButtonBqzx() 
{
	// TODO: Add your control notification handler code here
	CString blh = m_grid.Perform();
	blh.TrimLeft();  blh.TrimRight();
	if(blh.IsEmpty())  return;

	theApp.OpenDocumentFile(blh);
	 
	int col;long row;
	m_grid.EnumFirstSelected(&col,&row);
	row = row + 1;
	CString yzlx,qcbw,cks,qcys;
	m_grid.QuickGetText(2,row,&yzlx);
	m_grid.QuickGetText(3,row,&qcbw);
	m_grid.QuickGetText(4,row,&cks);
	m_grid.QuickGetText(5,row,&qcys);

	CWnd* material = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedMaterial));
	((CMedMaterial*)material)->SetDlgItemText(IDC_COMBO_RWLY,yzlx);
	((CMedMaterial*)material)->SetDlgItemText(IDC_EDIT_QCBW,qcbw);
	((CMedMaterial*)material)->SetDlgItemText(IDC_EDIT_CKS,cks);
	((CMedMaterial*)material)->SetDlgItemText(IDC_EDIT_QCYS,qcys);
	EndDialog(IDOK);
}
