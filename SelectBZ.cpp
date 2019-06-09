// SelectBZ.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "SelectBZ.h"
#include "CreateReport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectBZ dialog
extern CPathologyApp theApp;


CSelectBZ::CSelectBZ(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectBZ::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectBZ)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_See=m_Result=NULL;
	m_pWnd=pParent;
	ASSERT(::IsWindow(m_pWnd->m_hWnd));
}

CSelectBZ::CSelectBZ(CWnd* pParent /*=NULL*/,CString *See,CString *Result)
	: CDialog(CSelectBZ::IDD, pParent)
{
	m_See=See,m_Result=Result;
	m_pWnd=pParent;
	ASSERT(::IsWindow(m_pWnd->m_hWnd));
}
void CSelectBZ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectBZ)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectBZ, CDialog)
	//{{AFX_MSG_MAP(CSelectBZ)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_LBN_DBLCLK(IDC_LIST2, OnDblclkList2)
	ON_LBN_DBLCLK(IDC_LIST3, OnDblclkList3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectBZ message handlers

void CSelectBZ::OnSelchangeList1() 
{
	// TODO: Add your control notification handler code here
	int BuweiNum,site,i;
	CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST1));                         
	BuweiNum=pList->GetCurSel();if (BuweiNum<0) return;
	pList=(CListBox *)(GetDlgItem(IDC_LIST2));                         
	pList->ResetContent();		
	site=BuweiBZLen[0];
	for (i=0;i<BuweiNum;i++) site+=BuweiBZLen[i+1];
	for (i=0;i<(int)BuweiBZLen[BuweiNum+1];i++)
	{
		pList->AddString(BuweiBZName[site]);
		site++;
	}
	pList->SetCurSel(0);
	
}

void CSelectBZ::OnDblclkList2() 
{
	// TODO: Add your control notification handler code here
	int Sele;
	CString strbuf,strbuft;
	CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST1));  
	Sele=pList->GetCurSel();
	if (Sele<0) return;
	pList->GetText(Sele,strbuf);
	pList=(CListBox *)(GetDlgItem(IDC_LIST2));   
	Sele=pList->GetCurSel();                    
	if (Sele<0) return;
	pList->GetText(Sele,strbuft);
	strbuf=strbuf+' '+strbuft;
	pList=(CListBox *)(GetDlgItem(IDC_LIST3));   
	pList->AddString(strbuf);	
}

void CSelectBZ::OnDblclkList3() 
{
	// TODO: Add your control notification handler code here
	int Sele;
	CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST3));  
	Sele=pList->GetCurSel();
	if (Sele<0) return;
	pList->DeleteString(Sele);	
}

BOOL CSelectBZ::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	WORD len,i,j,site;
	CFile BuweiBZFile;
	CString filenm;CFileStatus status;
	CListBox *pList;
	BuweiBZName.RemoveAll();BuweiBZLen.RemoveAll();
	filenm=theApp.m_Exe_Working_Dir;filenm+="system/部位种类.dat";
	if (CFile::GetStatus(filenm,status))
	{
		BuweiBZFile.Open(filenm,CFile::modeRead);
		CArchive archive(&BuweiBZFile,CArchive::load);
		TRY {
			archive>>len;
			BuweiBZLen.Add(len);
			CString strbuf;
			for (i=0;i<BuweiBZLen[0];i++) {
				archive>>strbuf;
				BuweiBZName.Add(strbuf);
			}                       
			for (i=0;i<BuweiBZLen[0];i++) {
				archive>>len;
				BuweiBZLen.Add(len);
				for (j=0;j<len;j++)
				{
					archive>>strbuf;
					BuweiBZName.Add(strbuf);
				}
			}
		} 
		CATCH (CArchiveException,e)
		{
			BuweiBZName.RemoveAll();BuweiBZLen.RemoveAll();
		}
		END_CATCH		
		archive.Close();BuweiBZFile.Close();
	} 
	if (BuweiBZName.GetSize()>0)
	{
		pList=(CListBox *)(GetDlgItem(IDC_LIST1));                         
		for (i=0;i<BuweiBZLen[0];i++)
		{
			pList->AddString(BuweiBZName[i]);
		}
		pList=(CListBox *)(GetDlgItem(IDC_LIST2));                         
		site=BuweiBZLen[0];
		for (i=0;i<BuweiBZLen[1];i++)
		{
			pList->AddString(BuweiBZName[site]);
			site++;
		}
	}	
	pList=(CListBox *)(GetDlgItem(IDC_LIST1));  
	pList->SetCurSel(0);	
	pList=(CListBox *)(GetDlgItem(IDC_LIST2));                         
	pList->SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectBZ::OnOK() 
{
	// TODO: Add extra validation here
	CFile DestFile;
	CString filenm,strbuf;int i;WORD len;
	filenm=theApp.m_Exe_Working_Dir;filenm+="system/bwbz.dat";
	DestFile.Open(filenm,CFile::modeCreate|CFile::modeWrite);
	CArchive archive(&DestFile,CArchive::store);
	CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST3));  
	len=pList->GetCount();
	archive<<len;
	for (i=0;i<(int)len;i++) {
		pList->GetText(i,strbuf);
		archive<<strbuf;
	}
	pList->ResetContent();	
	archive.Close();DestFile.Close();
	int Width,Height;
	Width=GetSystemMetrics(SM_CXSCREEN);Height=GetSystemMetrics(SM_CYSCREEN);
	CWnd *pWnd=new CCreateReport(this,m_pWnd,m_See,m_Result);	
	HMENU SumMenu=LoadMenu(AfxGetInstanceHandle(),"IDR_MENU1");
	pWnd->CreateEx(WS_EX_WINDOWEDGE,AfxRegisterWndClass(CS_GLOBALCLASS|CS_NOCLOSE),"文本报告选择",WS_VISIBLE|WS_CAPTION|WS_POPUP,0,0,Width,Height, m_hWnd, SumMenu);
	pWnd->ShowWindow(SW_SHOW);
}
