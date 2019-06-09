// FalseNoQuery.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "FalseNoQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFalseNoQuery dialog
extern CPathologyApp theApp;

CFalseNoQuery::CFalseNoQuery(CWnd* pParent /*=NULL*/)
	: CDialog(CFalseNoQuery::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFalseNoQuery)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFalseNoQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFalseNoQuery)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFalseNoQuery, CDialog)
	//{{AFX_MSG_MAP(CFalseNoQuery)
	ON_BN_CLICKED(IDQUERY, OnQuery)
	ON_BN_CLICKED(IDEXPORT, OnExport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFalseNoQuery message handlers

void CFalseNoQuery::OnQuery() 
{
	SetDlgItemText(IDC_EDIT_QUERYRESULT, "");

	CString start, end;
	GetDlgItemText(IDC_EDIT_START, start);
	GetDlgItemText(IDC_EDIT_END, end);
	start.TrimLeft();  start.TrimRight();
	end.TrimLeft();    end.TrimRight();
	
	if(start.GetLength() != end.GetLength() || start.GetLength() < 1 || start > end)
	{
		AfxMessageBox("起止编号不规范，请重新录入！");
		return;
	}

	CString textstr; textstr.Empty();
	char enter[3];
	enter[0] = 0x0d; enter[1]= 0x0a; enter[2] = 0x00;

	BeginWaitCursor();

	while(start <= end)
	{
		int num = 0;
		try
		{
			SAString cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("medicalno").setAsString() = start;
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				num = g_dbcommand.Field(1).asLong();
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

		if(num == 0)
		{
			if(!textstr.IsEmpty())  textstr += enter;
			textstr += start;
		}

		CString str,str1;
		char strbuf[300];
		int Len=start.GetLength(),k;
		BYTE ch;
		for(k=(Len-1);k>=0;k--)
		{
			ch = start.GetAt(k); 
			if(!(ch >= 0x30 && ch <= 0x39))  break;
		}
		k = Len-1-k;
		ltoa(atol(start.Right(k))+1,strbuf,10);
		str1.Format("%s",strbuf);
		str = start.Left(Len-k);
		Len = k - str1.GetLength();
		for(k=0;k<Len;k++)  str += "0";
		str += str1;
		start = str;
	}

	EndWaitCursor();
	SetDlgItemText(IDC_EDIT_QUERYRESULT, textstr);
}

#include "KSFileDialog.h"

void CFalseNoQuery::OnExport() 
{
	static char Filter[]="漏编号列表文件 (*.txt)|*.txt||";
	CKSFileDialog MyDlg( FALSE,"txt", NULL, OFN_EXTENSIONDIFFERENT| OFN_OVERWRITEPROMPT |	OFN_PATHMUSTEXIST | OFN_LONGNAMES | OFN_EXPLORER ,	Filter , this );
	MyDlg.m_ofn.lpstrTitle = _T("漏编号列表导出");
	if( MyDlg.DoModal() == IDCANCEL )	 return;
	POSITION pos = MyDlg.GetStartPosition();
	CString strPathName = MyDlg.GetNextPathName(pos);

	CString textstr;
	GetDlgItemText(IDC_EDIT_QUERYRESULT, textstr);
	textstr.TrimLeft();  textstr.TrimRight();

	CStdioFile File;
	if ( File.Open(strPathName, CFile::modeWrite |CFile::modeCreate | CFile::typeText) )
	{
		File.WriteString(textstr);

		File.Close();
	}
}
