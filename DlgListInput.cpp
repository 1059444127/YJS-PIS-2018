// DlgListInput.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "DlgListInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgListInput dialog
extern CPathologyApp theApp;


CDlgListInput::CDlgListInput(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CDlgListInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgListInput)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_nID = CDlgListInput::IDD;
	m_type = 0;
}


void CDlgListInput::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgListInput)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgListInput, CXTResizeDialog)
	//{{AFX_MSG_MAP(CDlgListInput)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDINDEX, OnIndex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgListInput message handlers
BOOL CDlgListInput::Create()
{
	return CXTResizeDialog::Create(m_nID, m_pParent);
}

BOOL CDlgListInput::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect;
	GetClientRect(&rect);
	rect.bottom -= 30;
	if(m_listinfo.m_hWnd == NULL)
	{
		if(!m_listinfo.CreateGrid(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rect,this,111))
		{
			TRACE0( "Failed to create m_listinfo.\n" );
			return FALSE;
		}
	}

	if(!m_type)
	{
		((CButton *)GetDlgItem(IDINDEX))->ShowWindow(SW_HIDE);
		((CEdit *)GetDlgItem(IDC_EDIT_INDEXWORD))->ShowWindow(SW_HIDE);
	}
	else
	{
		((CButton *)GetDlgItem(IDINDEX))->ShowWindow(SW_SHOW);
		((CEdit *)GetDlgItem(IDC_EDIT_INDEXWORD))->ShowWindow(SW_SHOW);
	}

	SetResize(IDC_EDIT_INDEXWORD, SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDINDEX, SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDCANCEL,SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgListInput::OnSize(UINT nType, int cx, int cy) 
{
	CXTResizeDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rect;
	rect.left = 0;  rect.right = cx;  rect.top = 0;  rect.bottom = cy;
	rect.bottom -= 30;
	::MoveWindow(m_listinfo.m_hWnd,rect.left,rect.top,rect.Width(),rect.Height(),TRUE);
	m_listinfo.SetColWidth(0,rect.Width() - 20);
}

void CDlgListInput::OnIndex() 
{
	// TODO: Add your control notification handler code here
	if(!m_type)  return;
	
	CString m_EditStr;  
	m_EditStr.Empty();
	GetDlgItemText(IDC_EDIT_INDEXWORD, m_EditStr);
	m_EditStr.TrimLeft();   m_EditStr.TrimRight();
	CString m_like;
	m_EditStr.MakeUpper(); // for oracle
	m_like.Format("%%%s%%",m_EditStr);

	theApp.ConnectDatabase();
	try
	{
		BYTE ch;
		for(int i = 0; i < m_EditStr.GetLength(); i++)
		{
			ch = m_EditStr.GetAt(i);
			if((ch&0x80) != 0 )   break;
		}
		SAString m_WhereBetween_Cmd, FilterField;
		if( i < m_EditStr.GetLength() )
		{
			FilterField.Format("CDISE");
		}
		else
		{
			FilterField.Format("PINYIN");
		}
		m_WhereBetween_Cmd.Format(" where %s like :%s",FilterField,FilterField);
		
		
		SAString cmdstr;
		if( m_type == 1 )
			cmdstr = "select count(*) from " + (SAString)theApp.TABLE_ICD10 + m_WhereBetween_Cmd + " and MICD10 is not null and MICD10 <> '！'  and MICD10 <> '---' ";
		else
			cmdstr = "select count(*) from " + (SAString)theApp.TABLE_ICD10 + m_WhereBetween_Cmd + " and SICD10 is not null and SICD10 <> '！'  and SICD10 <> '---' ";
		g_dbcommand.setCommandText(cmdstr);

		//AfxMessageBox(cmdstr);

		g_dbcommand.Param(( SAString )FilterField).setAsString() = m_like;
		g_dbcommand.Execute();
		g_dbcommand.FetchNext();
		int numrow = g_dbcommand.Field(1).asLong();
		m_listinfo.SetNumberRows(numrow);
		
		if( m_type == 1 )
			cmdstr = "select MICD10, CDISE from " + (SAString)theApp.TABLE_ICD10 + m_WhereBetween_Cmd + " and MICD10 is not null and MICD10 <> '！'  and MICD10 <> '---' ";
		else
			cmdstr = "select SICD10, CDISE from " + (SAString)theApp.TABLE_ICD10 + m_WhereBetween_Cmd + " and SICD10 is not null and SICD10 <> '！'  and SICD10 <> '---' ";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param(( SAString )FilterField).setAsString() = m_like;
		g_dbcommand.Execute();
		
		CString str;
		i = 0;
		while( g_dbcommand.FetchNext() )
		{
			str.Format("%s [%s]",g_dbcommand.Field(1).asString(), g_dbcommand.Field(2).asString());
			m_listinfo.QuickSetText(0 , i , str);
			i++;
		}
		if( i > 0 ) 	m_listinfo.GotoRow(0);
		m_listinfo.RedrawAll();
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

BOOL CDlgListInput::PreTranslateMessage(MSG* pMsg) 
{
	if(m_type && pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnIndex();
		return true;
	}
	
	return CXTResizeDialog::PreTranslateMessage(pMsg);
}
