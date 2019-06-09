// FmtSelect.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "PathologyView.h"
#include "PathologyDoc.h"
#include "FmtSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFmtSelect dialog
extern CPathologyApp theApp;

CFmtSelect::CFmtSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CFmtSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFmtSelect)
	m_method = 1;
	//}}AFX_DATA_INIT
	m_Mode = 0;
}


void CFmtSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFmtSelect)
	DDX_Radio(pDX, IDC_RADIO1, m_method);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFmtSelect, CDialog)
	//{{AFX_MSG_MAP(CFmtSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFmtSelect message handlers

BOOL CFmtSelect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	theApp.ConnectDatabase();

	CStringArray bdlist; bdlist.RemoveAll();
	CStdioFile File;
	if ( File.Open(theApp.m_Exe_Working_Dir + "system\\表单.txt", CFile::modeRead | CFile::typeText) )
	{
		CString tmp;
		while(1)
		{
			if( File.ReadString(tmp) == FALSE ) break;
			tmp.TrimLeft(); tmp.TrimRight();
			if(!tmp.IsEmpty())
			{
				bdlist.Add(tmp);
			}
		}
		File.Close();
	}
	
	try
	{
		g_dbcommand.setCommandText("Select rpt_name from REPORTFORMAT where dbtype = :dbtype");
		g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
		g_dbcommand.Execute();
		
		while( g_dbcommand.FetchNext() )
		{
			CString str = g_dbcommand.Field("rpt_name").asString();
			if(str.CompareNoCase("制片条码") == 0 || str.CompareNoCase("回执单") == 0 || str.CompareNoCase("冰冻报告") == 0 || str.CompareNoCase("材块移交表") == 0 || str.CompareNoCase("切片移交表") == 0 || str.CompareNoCase("医嘱工作单") == 0 || str.CompareNoCase("制片情况统计表") == 0)  continue;

			if(bdlist.GetSize() > 0)
			{
				for(int nx = 0; nx < bdlist.GetSize(); nx++)
				{
					if(bdlist.GetAt(nx).CompareNoCase(str) == 0)  break;
				}
				if(nx < bdlist.GetSize())  continue;
			}

			((CComboBox *)GetDlgItem(IDC_COMBO_FMT))->AddString( g_dbcommand.Field("rpt_name").asString() );
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

	bdlist.RemoveAll();

	if(((CComboBox *)GetDlgItem(IDC_COMBO_FMT))->GetCount() > 0)
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_FMT))->SetCurSel(0);
	}
	
	fmtstr.Empty();

	((CButton *)GetDlgItem(IDC_CHECK_RECORDFMT))->SetCheck(1);

	if(m_Mode)
	{
		m_method = 0;
		((CButton *)GetDlgItem(IDC_RADIO2))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_RADIO3))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_CHECK_RECORDFMT))->EnableWindow(FALSE);
		SetWindowText("还未指定报告格式，请选择...");
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFmtSelect::OnOK() 
{
	// TODO: Add extra validation here
	if(((CComboBox *)GetDlgItem(IDC_COMBO_FMT))->GetCount() > 0)
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_FMT))->GetLBText(((CComboBox *)GetDlgItem(IDC_COMBO_FMT))->GetCurSel(), fmtstr);
	}

	if(((CButton *)GetDlgItem(IDC_CHECK_RECORDFMT))->GetState() & 0x0003)
	{
		theApp.m_IsRecordFmtInBatchPrinting = true;
	}
	else
	{
		theApp.m_IsRecordFmtInBatchPrinting = false;
	}
	
	CDialog::OnOK();
}
