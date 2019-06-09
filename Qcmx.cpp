// Qcmx.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "Pathologyview.h"
#include "Pathologydoc.h"
#include "Qcmx.h"
#include "LKBS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQcmx dialog
extern CPathologyApp theApp;

CQcmx::CQcmx(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CQcmx::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQcmx)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CQcmx::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQcmx)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
IMPLEMENT_DYNCREATE(CQcmx, CXTResizeDialog)

BEGIN_MESSAGE_MAP(CQcmx, CXTResizeDialog)
	//{{AFX_MSG_MAP(CQcmx)
	ON_COMMAND(IDM_MATEARIAL_LKBS, OnMatearialLkbs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQcmx message handlers

BOOL CQcmx::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_MaterialList.AttachGrid(this,IDC_STATIC_QCXX);
	m_MaterialList.SetParent(this);
	m_MaterialList.ResetGridSize();
	m_MaterialList.RedrawAll();

	SetResize(IDC_STATIC_QCFY, SZ_TOP_LEFT ,  SZ_TOP_LEFT);
	SetResize(IDC_STATIC_BBWZ, SZ_TOP_LEFT ,  SZ_TOP_LEFT);
	SetResize(IDC_STATIC_LKZS, SZ_BOTTOM_LEFT ,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_CKZS, SZ_BOTTOM_LEFT ,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_QCJLR, SZ_TOP_RIGHT ,  SZ_TOP_RIGHT);

	SetResize(IDC_TEXT_QCFY, SZ_TOP_LEFT ,  SZ_TOP_LEFT);
	SetResize(IDC_TEXT_BBWZ, SZ_TOP_LEFT ,  SZ_TOP_LEFT);
	SetResize(IDC_TEXT_LKZS, SZ_BOTTOM_LEFT ,  SZ_BOTTOM_LEFT);
	SetResize(IDC_TEXT_CKZS, SZ_BOTTOM_LEFT ,  SZ_BOTTOM_LEFT);
	SetResize(IDC_TEXT_QCJLR, SZ_TOP_RIGHT ,  SZ_TOP_RIGHT);

	SetResize(IDC_STATIC_QCXX, SZ_TOP_LEFT ,  SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT ,  SZ_BOTTOM_RIGHT);

	SetDlgItemText(IDC_TEXT_QCFY, "");
	SetDlgItemText(IDC_TEXT_BBWZ, "");
	SetDlgItemText(IDC_TEXT_LKZS, "");
	SetDlgItemText(IDC_TEXT_CKZS, "");
	SetDlgItemText(IDC_TEXT_QCJLR, "");
	
	try
	{
		SAString cmdstr = "Select * from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
		g_dbcommand.Execute();
		
		if(g_dbcommand.FetchNext())
		{
			SetDlgItemText(IDC_TEXT_QCFY, g_dbcommand.Field("qcfy").asString());
			SetDlgItemText(IDC_TEXT_BBWZ, g_dbcommand.Field("bbwz").asString());
			SetDlgItemInt(IDC_TEXT_LKZS, g_dbcommand.Field("lkzs").asLong());
			SetDlgItemInt(IDC_TEXT_CKZS, g_dbcommand.Field("ckzs").asLong());
			SetDlgItemText(IDC_TEXT_QCJLR, g_dbcommand.Field("qcjlr").asString());
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
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CQcmx::OnMatearialLkbs() 
{
	// TODO: Add your command handler code here
	CLKBS dlg;
	if(dlg.DoModal() == IDCANCEL)
	{
		return;
	}
	CStringArray mnoList,xhList;
	int col; long row;
	int ret = m_MaterialList.EnumFirstSelected(&col,&row);
	while(ret == UG_SUCCESS)
	{
		xhList.Add(m_MaterialList.QuickGetText(1,row));
		ret = m_MaterialList.EnumNextSelected(&col,&row);
	}
	for(int i = 0;i < xhList.GetSize();i++)
	{
		try
		{
			SAString cmdstr = "update " + theApp.TABLE_MATERIAL + " set bz=:bz where medicalno=:medicalno and xh=:xh";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("bz").setAsString() = dlg.m_lkbs;
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Param("xh").setAsString() = xhList.GetAt(i);
			g_dbcommand.Execute();
			g_dbconnection.Commit();
		}
		catch (SAException& e)
		{
			AfxMessageBox((const char*)e.ErrText());
		}
	}
	m_MaterialList.ResetGridSize();
	m_MaterialList.RedrawAll();
}
