// SelfDefine.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "SelfDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelfDefine dialog
extern CPathologyApp theApp;

CSelfDefine::CSelfDefine(CWnd* pParent /*=NULL*/)
	: CDialog(CSelfDefine::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelfDefine)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_UsageType = 0;
}


void CSelfDefine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelfDefine)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelfDefine, CDialog)
	//{{AFX_MSG_MAP(CSelfDefine)
	ON_BN_CLICKED(IDC_BUTTON_SEL, OnButtonSel)
	ON_BN_CLICKED(IDC_BUTTON_UNSEL, OnButtonUnsel)
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_PREVIOUS, OnPrevious)
	ON_BN_CLICKED(ID_NEXT, OnNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelfDefine message handlers

void CSelfDefine::OnButtonSel() 
{
	CListBox *pListDbdefine=(CListBox *)(GetDlgItem(IDC_LIST_DBDEFINE));  
	CListBox *pListSelfdefine=(CListBox *)(GetDlgItem(IDC_LIST_SELFDEFINE));  
	
    if((pListDbdefine->GetCurSel() != LB_ERR) && (pListDbdefine->GetCount() > 1))
	{
		CString str;
		pListDbdefine->GetText(pListDbdefine->GetCurSel(), str);
		pListSelfdefine->AddString(str);
	}
}

void CSelfDefine::OnButtonUnsel() 
{
	CListBox *pListSelfdefine=(CListBox *)(GetDlgItem(IDC_LIST_SELFDEFINE));  

    if((pListSelfdefine->GetCurSel() != LB_ERR) && (pListSelfdefine->GetCount() > 1))
	{
		pListSelfdefine->DeleteString(pListSelfdefine->GetCurSel());
	}
}

BOOL CSelfDefine::OnInitDialog() 
{
	CDialog::OnInitDialog();

	switch(m_UsageType) {
	case 0:
		SetWindowText("自定义录入字段");
		break;
	case 1:
		SetWindowText("自定义显示字段");
		break;
	}
	
	CListBox *pListDbdefine=(CListBox *)(GetDlgItem(IDC_LIST_DBDEFINE));  
	CListBox *pListSelfdefine=(CListBox *)(GetDlgItem(IDC_LIST_SELFDEFINE));  

	// TODO: Add extra initialization here
	theApp.ConnectDatabase();

	for(int i = 0; i < theApp.m_fdescriptionlist.GetSize(); i++)
	{
		pListDbdefine->AddString(theApp.m_fdescriptionlist.GetAt(i));
	}

	CString opFname;
	switch(m_UsageType) {
	case 0:
		opFname = "system\\selffield.txt";
		break;
	case 1:
		opFname = "system\\selfshow.txt";
		break;
	}

	CStdioFile File;
	if ( File.Open(theApp.m_Exe_Working_Dir + opFname, CFile::modeRead | CFile::typeText) )
	{
		CString tmp;
		while(1)
		{
			if( File.ReadString(tmp) == FALSE ) break;
			tmp.TrimLeft(); tmp.TrimRight();
			if(!tmp.IsEmpty())
			{
				for(int i = 0; i < theApp.m_fdescriptionlist.GetSize(); i++)
				{
					if(theApp.m_fdescriptionlist.GetAt(i).CompareNoCase(tmp) == 0)
					{
						pListSelfdefine->AddString(tmp);
						break;
					}
				}
			}
		}
		File.Close();
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelfDefine::OnDestroy() 
{
	CListBox *pListSelfdefine=(CListBox *)(GetDlgItem(IDC_LIST_SELFDEFINE));  

	CString opFname;
	switch(m_UsageType) {
	case 0:
		opFname = "system\\selffield.txt";
		break;
	case 1:
		opFname = "system\\selfshow.txt";
		break;
	}

	CStdioFile File;
	if ( File.Open(theApp.m_Exe_Working_Dir + opFname, CFile::modeWrite |CFile::modeCreate | CFile::typeText) )
	{
		CString str;
		for(int i = 0; i < pListSelfdefine->GetCount(); i++)
		{
			pListSelfdefine->GetText(i, str);
			File.WriteString(str);
			File.WriteString("\n");
		}

		File.Close();
	}

	CDialog::OnDestroy();
}

void CSelfDefine::OnPrevious() 
{
	CListBox *pListSelfdefine=(CListBox *)(GetDlgItem(IDC_LIST_SELFDEFINE));  
	int CurSel = pListSelfdefine->GetCurSel();
	if(CurSel == LB_ERR)  return;

    if(pListSelfdefine->GetCount() > 1 && CurSel > 0)
	{
		CString str;
		pListSelfdefine->GetText(CurSel, str);
		pListSelfdefine->DeleteString(CurSel);
		pListSelfdefine->InsertString(CurSel-1, str);
		pListSelfdefine->SetCurSel(CurSel-1);
	}
}

void CSelfDefine::OnNext() 
{
	CListBox *pListSelfdefine=(CListBox *)(GetDlgItem(IDC_LIST_SELFDEFINE));  
	int CurSel = pListSelfdefine->GetCurSel();
	if(CurSel == LB_ERR)  return;

    if(pListSelfdefine->GetCount() > 1 && CurSel >= 0 && CurSel < (pListSelfdefine->GetCount()-1))
	{
		CString str;
		pListSelfdefine->GetText(CurSel, str);
		pListSelfdefine->DeleteString(CurSel);
		pListSelfdefine->InsertString(CurSel+1, str);		
		pListSelfdefine->SetCurSel(CurSel+1);
	}
}
