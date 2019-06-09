// Zdex.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "PathologyView.h"
#include "PathologyDoc.h"
#include "Zdex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZdex dialog
extern CPathologyApp theApp;

CZdex::CZdex(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CZdex::IDD, pParent)
{
	//{{AFX_DATA_INIT(CZdex)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_nID = CZdex::IDD;
}

BOOL CZdex::Create()
{
	return CDialog::Create(m_nID, m_pParent);
}

void CZdex::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZdex)
	DDX_Control(pDX, IDC_HYP_NOTE, m_HypLink_Note);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CZdex, CXTResizeDialog)
	//{{AFX_MSG_MAP(CZdex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZdex message handlers

BOOL CZdex::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
    SetResize(IDC_EDIT_LCZL, SZ_TOP_LEFT,    SZ_BOTTOM_RIGHT);
    SetResize(IDC_EDIT_RYSJ, SZ_TOP_RIGHT,   SZ_BOTTOM_RIGHT);
    SetResize(IDC_EDIT_QCJL, SZ_TOP_RIGHT,   SZ_BOTTOM_RIGHT);

    SetResize(IDC_FUNCTION, SZ_TOP_LEFT,     SZ_TOP_LEFT);
    SetResize(IDC_SITE,     SZ_TOP_RIGHT,    SZ_BOTTOM_RIGHT);
    SetResize(IDC_ZQM,      SZ_TOP_RIGHT,    SZ_BOTTOM_RIGHT);
	SetResize(IDC_HYP_NOTE, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK,         SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	
	m_HypLink_Note.SetURL("");
	m_HypLink_Note.SetColors(RGB(0x00, 0x00, 0xFF), RGB(0x00, 0x00, 0xFF), RGB(0x00, 0x00, 0xFF));
	m_HypLink_Note.SetUnderline(false);
	m_HypLink_Note.EnableShellExecute(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CZdex::OnOK() 
{
	ShowWindow(FALSE);
	CXTResizeDialog::OnOK();
}

void CZdex::AdjustPosition(void)
{    
	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		SetDlgItemText(IDC_EDIT_LCZL, "");
		SetDlgItemText(IDC_EDIT_RYSJ, "");
		SetDlgItemText(IDC_EDIT_QCJL, "");
		m_HypLink_Note.SetWindowText("");
		return;
	}
	CRect rect;int height;
	GetWindowRect(&rect); 
	
	CRect nParentRect;
	m_pParent->GetWindowRect(&nParentRect);
	
	height=rect.Height();
	SetWindowPos(NULL, nParentRect.left, nParentRect.top, nParentRect.Width(), height, SWP_NOZORDER);		

	CString lczl, rysj, qcjl;
	lczl.Empty(); rysj.Empty(); qcjl.Empty();
	char enter[3];
	enter[0] = 0x0d; enter[1]= 0x0a; enter[2] = 0x00;

	int lkzs = 0;
	try
	{
		SAString cmdstr = "Select * from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
		g_dbcommand.Execute();
		
		if(g_dbcommand.FetchNext())
		{
			lczl += ("[送检科室]：" + g_dbcommand.Field("sjks").asString());
			lczl += enter;
			lczl += ("[送检医师]：" + g_dbcommand.Field("sjys").asString());
			lczl += enter;
			lczl += ("[标本名称]：" + g_dbcommand.Field("bbmc").asString());
			lczl += enter;
			if(g_dbcommand.Field("lczd").isNull() || g_dbcommand.Field("lczd").asString().IsEmpty())
				lczl += ("[临床诊断]：无");
			else
				lczl += ("[临床诊断]：" + g_dbcommand.Field("lczd").asString());
			lczl += enter;
			if(g_dbcommand.Field("bszy").isNull() || g_dbcommand.Field("bszy").asString().IsEmpty())
				lczl += ("[病史摘要]：无");
			else
				lczl += ("[病史摘要]：" + g_dbcommand.Field("bszy").asString());
			lczl += enter;
			if(g_dbcommand.Field("sssj").isNull() || g_dbcommand.Field("sssj").asString().IsEmpty())
				lczl += ("[手术所见]：无");
			else
				lczl += ("[手术所见]：" + g_dbcommand.Field("sssj").asString());
			lczl += enter;
			if(g_dbcommand.Field("zlxx").isNull() || g_dbcommand.Field("zlxx").asString().IsEmpty())
				lczl += ("[肿瘤信息]：无");
			else
				lczl += ("[肿瘤信息]：" + g_dbcommand.Field("zlxx").asString());
			lczl += enter;
			if(g_dbcommand.Field("fkxx").isNull() || g_dbcommand.Field("fkxx").asString().IsEmpty())
				lczl += ("[妇科信息]：无");
			else
				lczl += ("[妇科信息]：" + g_dbcommand.Field("fkxx").asString());
			lczl += enter;

			rysj = g_dbcommand.Field("rysj").asString();

			lkzs = g_dbcommand.Field("lkzs").asLong();
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

	try
	{
		SAString cmdstr = "Select * from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno order by qcrq asc";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();

		CString tmp;
		while( g_dbcommand.FetchNext() )
		{
			tmp.Format("%-3s %s", g_dbcommand.Field("xh").asString(), g_dbcommand.Field("qcbw").asString());
			qcjl += tmp;
			qcjl += enter;
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

	int qpzs = 0;
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_SLICE + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
		{
			qpzs = g_dbcommand.Field(1).asLong();
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

	SetDlgItemText(IDC_EDIT_LCZL, lczl);
	SetDlgItemText(IDC_EDIT_RYSJ, rysj);
	SetDlgItemText(IDC_EDIT_QCJL, qcjl);

	CString info;
	info.Format("蜡块：%d，切片：%d", lkzs, qpzs);
	m_HypLink_Note.SetWindowText(info);
	m_HypLink_Note.Invalidate();
}
