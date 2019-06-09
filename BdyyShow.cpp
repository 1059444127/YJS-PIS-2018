// BdyyShow.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "BdyyShow.h"
#include "PathologyView.h"
#include "MedPerson.h"
#include "PathologyDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBdyyShow dialog
extern CPathologyApp theApp;

CBdyyShow::CBdyyShow(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CBdyyShow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBdyyShow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBdyyShow::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBdyyShow)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBdyyShow, CXTResizeDialog)
	//{{AFX_MSG_MAP(CBdyyShow)
	ON_BN_CLICKED(IDC_BUTTON_HZQR, OnButtonHzqr)
	ON_BN_CLICKED(IDC_BUTTON_BBQS, OnButtonBbqs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBdyyShow message handlers

BOOL CBdyyShow::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	m_grid.AttachGrid(this,IDC_STATIC_RECORDLIST);
	m_grid.SetParent(this);
	m_grid.ResetGridSize();
	m_grid.RedrawAll();
	
	SetResize(IDC_STATIC_RECORDLIST, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_HZQR, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_BBQS, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK,            SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBdyyShow::OnButtonHzqr() 
{
	CUIntArray  rowlist;
	int nCol, nRet;
	long nRow;

	nRet = m_grid.EnumFirstSelected( &nCol, &nRow );
	while( nRet == UG_SUCCESS )
	{
		if(nRow >= 0)
		{
			int rowcount = rowlist.GetSize();
			for(int i = 0; i < rowcount; i ++ )
			{
				if( nRow == rowlist.GetAt(i) )  break;
			}
			if( i >= rowcount )
			{
				rowlist.Add(nRow);
			}
		}
		
		nRet = m_grid.EnumNextSelected( &nCol, &nRow );
	}

	int curnum = m_grid.GetCurrentRow();
	for(int i = 0; i < rowlist.GetSize(); i++)
	{
		m_grid.GotoRow(rowlist.GetAt(i));
	}
	m_grid.GotoRow(curnum);

	if( rowlist.GetSize() <= 0 )
	{
		return;
	}
	
	CStringArray sqhlist;

	for(i = 0; i < rowlist.GetSize(); i++)
	{
		CString str;
		CUGCell  cell;
		m_grid.GetCellIndirect(0,rowlist.GetAt(i),&cell);
		cell.GetText(&str);
		CString yyhz;
		yyhz = m_grid.QuickGetText(7,rowlist.GetAt(i));
		if(yyhz.IsEmpty())
		{
			sqhlist.Add(str);		
		}
	}
	rowlist.RemoveAll();

	for(i = 0; i < sqhlist.GetSize(); i++)
	{
		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_BOOKING + " set yyhz	= '已做好接收准备', state = '2' where sqh = :sqh";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("sqh").setAsString() = sqhlist.GetAt(i);
			
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
			return;
		}
	}

	sqhlist.RemoveAll();

	m_grid.ResetGridSize();
	m_grid.RedrawAll();
	m_grid.RedrawAll();
}

void CBdyyShow::OnButtonBbqs() 
{
	
	CUIntArray  rowlist;
	int nCol, nRet;
	long nRow;
	
	nRet = m_grid.EnumFirstSelected( &nCol, &nRow );
	while( nRet == UG_SUCCESS )
	{
		if(nRow >= 0)
		{
			int rowcount = rowlist.GetSize();
			for(int i = 0; i < rowcount; i ++ )
			{
				if( nRow == rowlist.GetAt(i) )  break;
			}
			if( i >= rowcount )
			{
				rowlist.Add(nRow);
			}
		}
		
		nRet = m_grid.EnumNextSelected( &nCol, &nRow );
	}
	
	int curnum = m_grid.GetCurrentRow();
	for(int i = 0; i < rowlist.GetSize(); i++)
	{
		m_grid.GotoRow(rowlist.GetAt(i));
	}
	m_grid.GotoRow(curnum);
	
	if( rowlist.GetSize() <= 0 )
	{
		return;
	}
	
	CString bbmc; 
	CStringArray sqhlist;
	
	for(i = 0; i < rowlist.GetSize(); i++)
	{
		CString str;
		CUGCell  cell;
		m_grid.GetCellIndirect(0,rowlist.GetAt(i),&cell);
		cell.GetText(&str);
		sqhlist.Add(str);

		m_grid.GetCellIndirect(6,rowlist.GetAt(i),&cell);
		cell.GetText(&bbmc);
	}
	rowlist.RemoveAll();
	
	for(i = 0; i < sqhlist.GetSize(); i++)
	{
		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_BOOKING + " set yyhz	= '已接收' where sqh = :sqh";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("sqh").setAsString() = sqhlist.GetAt(i);
			
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
			return;
		}
	}
	
	
	CString sqh,brid,zyh,xm,xb,nl,csrq,lxdz,lxdh,sjks,sjys,bq,ch,bszy,lczd,sssj;
	try
	{
		SAString cmdstr = "Select * from " + theApp.TABLE_BOOKING + " where sqh = :sqh";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("sqh").setAsString() = sqhlist.GetAt(0);
		g_dbcommand.Execute();
		if( g_dbcommand.FetchNext() )
		{
			sqh  = g_dbcommand.Field("sqh").asString();
			brid = g_dbcommand.Field("brid").asString();
			zyh  = g_dbcommand.Field("zyh").asString();
			xm   = g_dbcommand.Field("xm").asString();
			xb   = g_dbcommand.Field("xb").asString();
			nl   = g_dbcommand.Field("nl").asString();
			sjks = g_dbcommand.Field("sjks").asString();
			sjys = g_dbcommand.Field("sjys").asString();
			bq   = g_dbcommand.Field("bq").asString();
			ch   = g_dbcommand.Field("ch").asString();
			bszy = g_dbcommand.Field("bszy").asString();
			lczd = g_dbcommand.Field("lczd").asString();
			sssj = g_dbcommand.Field("sssj").asString();
		}
		
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
		return;
	}
	
	sqhlist.RemoveAll();
	
	m_grid.ResetGridSize();
	m_grid.RedrawAll();
	m_grid.RedrawAll();
	((CMainFrame *)AfxGetMainWnd())->SendMessage(WM_COMMAND,ID_FILE_NEW);
	CPathologyView *pView = theApp.GetView();
	if(pView != NULL)
	{
		CWnd *pWnd = pView->m_tabCtrl.GetView(RUNTIME_CLASS(CMedPerson));
		if(pWnd != NULL)
		{
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_SQH,   sqh);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_BRID,  brid);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_ZYH,   zyh);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_XM,    xm);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_XB,    xb);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_NL,    nl);

			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_LXDZ,  lxdz);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_SJKS,  sjks);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_SJYS,  sjys);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_BQ,    bq);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_CH,    ch);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_BBMC,  bbmc);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_LCZD,  lczd);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_BSZY,  bszy);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_SSSJ,  sssj);
		}
	}
	EndDialog(IDOK);
}
