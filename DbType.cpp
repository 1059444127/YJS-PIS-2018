// DbType.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "DbType.h"
#include "AreaOut.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDbType dialog
extern CPathologyApp theApp;

CDbType::CDbType(CWnd* pParent /*=NULL*/)
	: CDialog(CDbType::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDbType)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDbType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDbType)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDbType, CDialog)
	//{{AFX_MSG_MAP(CDbType)
	ON_BN_CLICKED(IDDELETE, OnDelete)
	ON_BN_CLICKED(IDINSERT, OnInsert)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDbType message handlers

void CDbType::OnDelete() 
{
	int i;
	int total = ((CListCtrl *)GetDlgItem(IDC_LIST_DBF))->GetItemCount();
	if(total<1)  return;
	for(i=0;i<total;i++)
	{
        if(((CListCtrl *)GetDlgItem(IDC_LIST_DBF))->GetItemState(i, LVIS_FOCUSED))  break; 
	}
	if(i<total)
	{
		int dbno = atoi(((CListCtrl *)GetDlgItem(IDC_LIST_DBF))->GetItemText(i, 0));
		CString dbname = ((CListCtrl *)GetDlgItem(IDC_LIST_DBF))->GetItemText(i, 1);

		try
		{
			SAString cmdstr = "Delete from " + theApp.TABLE_DBTYPE + " where dbno=:dbno and dbname=:dbname";
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("dbno").setAsLong() = dbno;
			g_dbcommand.Param("dbname").setAsString() = dbname;
			
			g_dbcommand.Execute();
			g_dbconnection.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
		}

		Refresh();
	}
}

void CDbType::OnInsert() 
{
	CAreaOut mydlg(this);
	mydlg.m_type = 1000;
	
	mydlg.m_title.Format("指定库名");
	
	if( mydlg.DoModal() == IDCANCEL )  return;
	
	mydlg.m_from.TrimLeft();   mydlg.m_from.TrimRight();
	mydlg.m_to.TrimLeft();     mydlg.m_to.TrimRight();
	
	CString dbname = mydlg.m_from;
	if(dbname.IsEmpty())  return;
	int dbno = 0;

	try
	{
		SAString cmdstr = "Select dbno from " + theApp.TABLE_DBTYPE + " order by dbno desc";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			dbno = g_dbcommand.Field("dbno").asLong();
		}
		
		g_dbconnection.Commit();
	}
	catch(SAException &)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
	}

	dbno++;

	try
	{
		SAString cmdstr = "Insert into " + theApp.TABLE_DBTYPE + " (dbno, dbname) values(:dbno, :dbname)";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("dbno").setAsLong() = dbno;
		g_dbcommand.Param("dbname").setAsString() = dbname;
		
		g_dbcommand.Execute();
		g_dbconnection.Commit();
	}
	catch(SAException &x)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
		AfxMessageBox((const char*)x.ErrText());
	}

	Refresh();
}

void CDbType::Refresh()
{
	((CListCtrl *)GetDlgItem(IDC_LIST_DBF))->DeleteAllItems();

	// ==============================================================
	theApp.ConnectDatabase();

	int count = 0;
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_DBTYPE;
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			count = g_dbcommand.Field(1).asLong();
		}
		
		g_dbconnection.Commit();
	}
	catch(SAException &)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
	}

	(CListCtrl *)GetDlgItem(IDC_LIST_DBF)->SendMessage(LVM_SETITEMCOUNT, (WPARAM)count, (LPARAM)LVSICF_NOINVALIDATEALL);
	int index = 0, subItem, iActualItem;
	CString str;
	COleVariant varValue;
	LV_ITEM	    lvitem;

	try
	{
		SAString cmdstr = "Select * from " + theApp.TABLE_DBTYPE + " order by dbno asc";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Execute();
		
		while( g_dbcommand.FetchNext() )
		{
			for (subItem = 0; subItem < 2; subItem++)
			{
				lvitem.mask = LVIF_TEXT;
				lvitem.iItem = (subItem == 0)? index : iActualItem;
				lvitem.iSubItem = subItem;
				
				switch(subItem) {
				case 0:
					str.Format("%d", g_dbcommand.Field("dbno").asLong());
					break;
				case 1:
					str.Format("%s", g_dbcommand.Field("dbname").asString());
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

			index++;
		}
		
		g_dbconnection.Commit();
	}
	catch(SAException &)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
	}
}

BOOL CDbType::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	DWORD dwExStyle = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | 
						  LVS_EX_HEADERDRAGDROP;// | LVS_EX_TRACKSELECT;
	(CListCtrl *)GetDlgItem(IDC_LIST_DBF)->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(dwExStyle));	
	
	LV_COLUMN lvColumn;
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;	lvColumn.cx = 100;
	for(int i = 0; i < 2; i++) // set up columns
	{
		switch(i) {
		case 0:
			lvColumn.pszText = "编号";
			break;
		case 1:
			lvColumn.pszText = "名称";
			break;
		}
		((CListCtrl *)GetDlgItem(IDC_LIST_DBF))->InsertColumn(i, &lvColumn);//insert column		
	}

    Refresh();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDbType::OnDestroy() 
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
