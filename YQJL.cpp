// BqlbShow.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "YQJL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBqlbShow dialog
extern CPathologyApp theApp;

CYQJL::CYQJL(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CYQJL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CYQJL)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CYQJL::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYQJL)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CYQJL, CXTResizeDialog)
	//{{AFX_MSG_MAP(CYQJL)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBqlbShow message handlers

BOOL CYQJL::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	CString count;
	try
	{
		SAString cmdstr = "select count(*) from "+theApp.TABLE_MAIN+" where bgzt>=2 and bgzt<4 and wfyy is not null";
		if(theApp.m_DBType.CompareNoCase("oracle") == 0)
		{
			cmdstr = "select count(*) from "+theApp.TABLE_MAIN+" where bgzt>=2 and bgzt<4 and wfyy is not null and wfyy <> ''";
		}

		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Execute();
		while(g_dbcommand.FetchNext())
		{
			count.Format("%d",g_dbcommand.Field(1).asLong());
		}
		g_dbconnection.Commit();
	} 
	catch(SAException &e)
	{
		AfxMessageBox((const char*)e.ErrText());
	}
	SetWindowText("ÑÓÆÚ¼ÇÂ¼-"+count+"Àý");
	// TODO: Add extra initialization here
	m_grid.AttachGrid(this,IDC_STATIC_YQJL);
	m_grid.SetParent(this);
	m_grid.ResetGridSize();
	m_grid.RedrawAll();

	SetResize(IDC_STATIC_YQJL, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_CLOSE, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	
	//SetResize(IDC_BUTTON_BQZX, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	//SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CYQJL::OnButtonClose() 
{
	// TODO: Add your control notification handler code here
	/*CString blh = m_grid.Perform();
	blh.TrimLeft();  blh.TrimRight();
	if(blh.IsEmpty())  return;
	
	theApp.OpenDocumentFile(blh);*/
	EndDialog(IDOK);
}
