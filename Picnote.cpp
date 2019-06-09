// Picnote.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "PathologyView.h"
#include "PathologyDoc.h"
#include "Picnote.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPicnote dialog
extern CPathologyApp theApp;

CPicnote::CPicnote(CWnd* pParent /*=NULL*/)
	: CDialog(CPicnote::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPicnote)
	m_note = _T("");
	//}}AFX_DATA_INIT
}


void CPicnote::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPicnote)
	DDX_Text(pDX, IDC_NOTE, m_note);
	DDV_MaxChars(pDX, m_note, 10000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPicnote, CDialog)
	//{{AFX_MSG_MAP(CPicnote)
	ON_BN_CLICKED(IDC_BUTTON_QCMX, OnButtonQcmx)
	ON_CBN_SELENDOK(IDC_COMBO_WORD, OnSelendokComboWord)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPicnote message handlers

BOOL CPicnote::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetWindowText(title);
	
	CStdioFile File;
	if ( File.Open(theApp.m_Exe_Working_Dir + "system\\picword.txt", CFile::modeRead | CFile::typeText) )
	{
		CString tmp;
		while(1)
		{
			if( File.ReadString(tmp) == FALSE ) break;
			tmp.TrimLeft(); tmp.TrimRight();
			if(!tmp.IsEmpty())
			{
				((CComboBox *)GetDlgItem(IDC_COMBO_WORD))->AddString(tmp);
			}
		}
		File.Close();
	}
	if( ((CComboBox *)GetDlgItem(IDC_COMBO_WORD))->GetCount() > 0 )
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_WORD))->SetCurSel(0);
	}

	((CEdit *)GetDlgItem(IDC_NOTE))->SetFocus();

	UpdateData(FALSE);

	//---------------------------------------------------
	/*if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}*/
	//---------------------------------------------------

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPicnote::OnButtonQcmx() 
{
	// TODO: Add your control notification handler code here
	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  return;

	char enter[3];
	enter[0] = 0x0d; enter[1]= 0x0a; enter[2] = 0x00;

	CString info; info.Empty();
	try
	{
		SAString cmdstr = "Select * from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		while( g_dbcommand.FetchNext() )
		{
			CString str;
			str.Format("%3s  %20s  %2d", g_dbcommand.Field("xh").asString(), g_dbcommand.Field("qcbw").asString(),g_dbcommand.Field("cks").asLong());
		
			if(!info.IsEmpty()) info += enter;
			info += str;
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

	if(info.IsEmpty())  return;
	UpdateData(TRUE);

	if(!m_note.IsEmpty())  m_note += enter;
	m_note += info;

	UpdateData(FALSE);
}

void CPicnote::OnSelendokComboWord() 
{
	if( ((CComboBox *)GetDlgItem(IDC_COMBO_WORD))->GetCount() < 1 )  return;

	CString tmp;
	((CComboBox *)GetDlgItem(IDC_COMBO_WORD))->GetWindowText(tmp);
	tmp.TrimLeft();  tmp.TrimRight();

	UpdateData(TRUE);

	if(!tmp.IsEmpty())
	{
		m_note += tmp;
	}

	UpdateData(FALSE);
}
