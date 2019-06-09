// DbfStandard.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "DbfStandard.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDbfStandard dialog
extern CPathologyApp theApp;


CDbfStandard::CDbfStandard(CWnd* pParent /*=NULL*/)
	: CDialog(CDbfStandard::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDbfStandard)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDbfStandard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDbfStandard)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDbfStandard, CDialog)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CDbfStandard)
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_DELETE_ALL, OnDeleteAll)
	ON_BN_CLICKED(ID_DELETE, OnDelete)
	ON_BN_CLICKED(ID_GET, OnGet)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DBF, OnDblclkListDbf)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDbfStandard message handlers

BOOL CDbfStandard::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	DWORD dwExStyle = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | 
						  LVS_EX_HEADERDRAGDROP;// | LVS_EX_TRACKSELECT;
	(CListCtrl *)GetDlgItem(IDC_LIST_DBF)->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(dwExStyle));	
	
	LV_COLUMN lvColumn;
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;	lvColumn.cx = 100;
	for(int i = 0; i < 3; i++) // set up columns
	{
		switch(i) {
		case 0:
			lvColumn.pszText = "物镜倍数";
			break;
		case 1:
			lvColumn.pszText = "定标量";
			break;
		case 2:
			lvColumn.pszText = "单位";
			break;
		}
		((CListCtrl *)GetDlgItem(IDC_LIST_DBF))->InsertColumn(i, &lvColumn);//insert column		
	}

	// ==============================================================
	CFile file;
	CFileException fe;
	if (file.Open(theApp.m_Exe_Working_Dir + "system/rulermark.dat", CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		STANDARD_UNIT m_unit;
		int count = file.GetLength() / sizeof(STANDARD_UNIT);

		(CListCtrl *)GetDlgItem(IDC_LIST_DBF)->SendMessage(LVM_SETITEMCOUNT, (WPARAM)count, (LPARAM)LVSICF_NOINVALIDATEALL);

		int index, subItem, iActualItem;
		CString str;
		COleVariant varValue;
		LV_ITEM	    lvitem;
		for (index = 0; index < count; index++)
		{
			file.Read(&m_unit, sizeof(STANDARD_UNIT));
			for (subItem = 0; subItem < 3; subItem++)
			{
				lvitem.mask = LVIF_TEXT;
				lvitem.iItem = (subItem == 0)? index : iActualItem;
				lvitem.iSubItem = subItem;
				
				switch(subItem) {
				case 0:
					str.Format("%d", m_unit.zoomlen);
					break;
				case 1:
					str.Format("%.4f", m_unit.value);
					break;
				case 2:
					str.Format("%s", m_unit.unitname);
					break;
				}
				char szvalue[1000];
				strcpy(szvalue , str);
				lvitem.pszText = szvalue;
				if (subItem == 0)
					iActualItem = ((CListCtrl *)GetDlgItem(IDC_LIST_DBF))->InsertItem(&lvitem);
				else
					((CListCtrl *)GetDlgItem(IDC_LIST_DBF))->SetItem(&lvitem);
			}
		}

		file.Close();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDbfStandard::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	int NumCols = 0;
	LV_COLUMN lvColumn;
	memset(&lvColumn, 0, sizeof(LV_COLUMN));
	lvColumn.mask = LVCF_FMT;
	while(((CListCtrl *)GetDlgItem(IDC_LIST_DBF))->GetColumn(NumCols, &lvColumn))
		NumCols++;
	((CListCtrl *)GetDlgItem(IDC_LIST_DBF))->SendMessage(LVM_SETITEMCOUNT, (WPARAM)0, (LPARAM)LVSICF_NOINVALIDATEALL);
	while(NumCols > 0)
	{
		((CListCtrl *)GetDlgItem(IDC_LIST_DBF))->DeleteColumn(NumCols - 1);
		NumCols--;
	}
}

void CDbfStandard::OnDeleteAll() 
{
	// TODO: Add your control notification handler code here
	int total = ((CListCtrl *)GetDlgItem(IDC_LIST_DBF))->GetItemCount();
	if(total<1)  return;
	CString msg;
	msg.Format("确认删除全部定标量记录吗？");
	if(AfxMessageBox(msg,MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
	{
		((CListCtrl *)GetDlgItem(IDC_LIST_DBF))->DeleteAllItems();
		CFile file;
		CFileException fe;
		if (file.Open(theApp.m_Exe_Working_Dir + "system/rulermark.dat", CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
		{
			file.Close();
		}
		((CMainFrame*)AfxGetMainWnd())->m_wndComboRulerMark.ResetContent();
	}
}

void CDbfStandard::OnDelete() 
{
	// TODO: Add your control notification handler code here
	int i;
	int total = ((CListCtrl *)GetDlgItem(IDC_LIST_DBF))->GetItemCount();
	if(total<1)  return;
	for(i=0;i<total;i++)
	{
        if(((CListCtrl *)GetDlgItem(IDC_LIST_DBF))->GetItemState(i, LVIS_FOCUSED))  break; 
	}
	if(i<total)
	{
		((CListCtrl *)GetDlgItem(IDC_LIST_DBF))->DeleteItem(i);

		STANDARD_UNIT * pUnitList = NULL;
		
		CFile file;
		CFileException fe;
		if (file.Open(theApp.m_Exe_Working_Dir + "system/rulermark.dat", CFile::modeRead | CFile::shareDenyWrite, &fe))
		{
			pUnitList = new STANDARD_UNIT[total];
			for(int k = 0; k < total; k++ )
			{
				file.Read(&pUnitList[k], sizeof(STANDARD_UNIT));
			}
			file.Close();
		}
		
		if (file.Open(theApp.m_Exe_Working_Dir + "system/rulermark.dat", CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
		{
			for(int k = 0; k < total; k++ )
			{
				if( k == i )  continue;
				file.Write(&pUnitList[k], sizeof(STANDARD_UNIT));
			}
			file.Close();
		}
		
		if( pUnitList )
		{
			delete pUnitList;
			pUnitList = NULL;
		}

		((CMainFrame*)AfxGetMainWnd())->m_wndComboRulerMark.DeleteString(i);
	}
}

void CDbfStandard::OnGet() 
{
	// TODO: Add your control notification handler code here
	CMainFrame *pMainFrame=(CMainFrame *)AfxGetMainWnd();
	
	int i;
	int total = ((CListCtrl *)GetDlgItem(IDC_LIST_DBF))->GetItemCount();
	if(total<1)  return;
	for(i=0;i<total;i++)
	{
        if(((CListCtrl *)GetDlgItem(IDC_LIST_DBF))->GetItemState(i, LVIS_FOCUSED))  break; 
	}
	if(i<total)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndComboRulerMark.SetCurSel(i);

		EndDialog(IDOK);
	}
}

void CDbfStandard::OnDblclkListDbf(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    OnGet(); 	
	*pResult = 0;
}

void CDbfStandard::OnContextMenu(CWnd*, CPoint point)
{

	// CG: This block was added by the Pop-up Menu component
	{
		if (point.x == -1 && point.y == -1){
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

		CMenu menu;
		VERIFY(menu.LoadMenu(CG_IDR_POPUP_DBF_STANDARD));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
	}
}
