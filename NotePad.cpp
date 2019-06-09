// NotePad.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "NotePad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNotePad dialog
extern CPathologyApp theApp;

CNotePad::CNotePad(CWnd* pParent /*=NULL*/)
	: CDialog(CNotePad::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNotePad)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	status.Empty();
}


void CNotePad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNotePad)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNotePad, CDialog)
	//{{AFX_MSG_MAP(CNotePad)
	ON_BN_CLICKED(ID_CLEAR, OnClear)
	ON_BN_CLICKED(ID_SEND, OnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNotePad message handlers

BOOL CNotePad::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	theApp.ConnectDatabase();

	try
	{
		g_dbcommand.setCommandText("Select username from " + (SAString)theApp.TABLE_SECURITY);
		g_dbcommand.Execute();
		
		while( g_dbcommand.FetchNext() )
		{
			if(g_dbcommand.Field("username").asString().CompareNoCase("administrator"))
			{
				((CComboBox *)GetDlgItem(IDC_COMBO_PERSON))->AddString(g_dbcommand.Field("username").asString());
			}
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

	((CComboBox *)GetDlgItem(IDC_COMBO_PERSON))->AddString("全科");
	
	if(((CComboBox *)GetDlgItem(IDC_COMBO_PERSON))->GetCount() > 0)
	{
		if(status.IsEmpty())
		{
			((CComboBox *)GetDlgItem(IDC_COMBO_PERSON))->SetCurSel(0);
		}
		else
		{
			GetDlgItem(IDC_COMBO_PERSON)->SetWindowText(status);
		}
	
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNotePad::OnClear() 
{
	SetDlgItemText(IDC_EDIT_INFO, "");

	((CEdit *)GetDlgItem(IDC_EDIT_INFO))->SetFocus();
}

void CNotePad::OnSend() 
{
	CString info;
	GetDlgItemText(IDC_COMBO_PERSON, info);
	info.TrimLeft(); info.TrimRight();

	if(info.IsEmpty())
	{
		AfxMessageBox("接收人不能为空！");
		return;
	}

	CStringArray nPersonList;
	nPersonList.RemoveAll();
	if(info.CompareNoCase("全科") == 0)
	{
		for(int index = 0; index < (((CComboBox *)GetDlgItem(IDC_COMBO_PERSON))->GetCount()-1); index++)
		{
			CString tmp;
			((CComboBox *)GetDlgItem(IDC_COMBO_PERSON))->GetLBText(index, tmp);
			nPersonList.Add(tmp);
		}
	}
	else
	{
		nPersonList.Add(info);
	}

	GetDlgItemText(IDC_EDIT_INFO, info);
	info.TrimLeft(); info.TrimRight();

	if(info.IsEmpty())
	{
		AfxMessageBox("发送信息不能为空！");
		return;
	}

	theApp.ConnectDatabase();

	SADateTime ServerTime = theApp.GetServerDateTime();

	for(int index = 0; index < nPersonList.GetSize(); index ++)
	{
		try
		{
			SAString cmdstr = "Insert into " + theApp.TABLE_NOTEPAD + " (fbsj,fbr,sx,jsr,zt) values(:fbsj,:fbr,:sx,:jsr,0)";
			g_dbcommand.setCommandText(cmdstr);
			SADateTime time = ServerTime;
			g_dbcommand.Param("fbsj").setAsDateTime() = time;
			g_dbcommand.Param("fbr").setAsString() = theApp.m_Cpp_User;
			g_dbcommand.Param("sx").setAsString()  = info;
			g_dbcommand.Param("jsr").setAsString() = nPersonList.GetAt(index);
			
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
	}

	nPersonList.RemoveAll();

	AfxMessageBox("发送成功！");
}
