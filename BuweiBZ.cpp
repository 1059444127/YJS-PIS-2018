// BuweiBZ.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "BuweiBZ.h"
#include <fstream.h>

#include "StrEdit.h"
#include "EditReport.h"
#include "CreateReport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// BuweiBZ dialog
extern CPathologyApp theApp;

BuweiBZ::BuweiBZ(CWnd* pParent /*=NULL*/)
	: CDialog(BuweiBZ::IDD, pParent)
{
	//{{AFX_DATA_INIT(BuweiBZ)
	m_Buwei = -1;
	//}}AFX_DATA_INIT
}

void BuweiBZ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(BuweiBZ)
	DDX_LBIndex(pDX, IDC_LIST1, m_Buwei);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(BuweiBZ, CDialog)
	//{{AFX_MSG_MAP(BuweiBZ)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// BuweiBZ message handlers

BOOL BuweiBZ::OnInitDialog()
{   
	// TODO: Add your command handler code here
	WORD len,i,j,site;
	CFile BuweiBZFile;
	CString filenm;CFileStatus status;
	BuweiBZName.RemoveAll();BuweiBZLen.RemoveAll();
	filenm=theApp.m_Exe_Working_Dir;filenm+="system\\部位种类.dat";
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
		CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST1));                         
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
	m_Buwei=0;
	CDialog::OnInitDialog();
	return TRUE;
}

void BuweiBZ::OnOK()
{   
	// TODO: Add extra validation here
	WORD i,j,site;
	CFile BuweiBZFile;
	CString filenm;
	if (BuweiBZName.GetSize()<1) CDialog::OnOK();
	filenm=theApp.m_Exe_Working_Dir;filenm+="system\\部位种类.dat";
	if (BuweiBZFile.Open(filenm,CFile::modeWrite|CFile::modeCreate))
	{
		CArchive archive(&BuweiBZFile,CArchive::store);
		archive<<BuweiBZLen[0];
		for (i=0;i<BuweiBZLen[0];i++) {
			archive<<BuweiBZName[i];
		}
		site=BuweiBZLen[0];
		for (i=0;i<BuweiBZLen[0];i++) {
			archive<<BuweiBZLen[i+1];
			for (j=0;j<BuweiBZLen[i+1];j++)
			{
				archive<<BuweiBZName[site];
				site++;
			}
		}
		archive.Close();BuweiBZFile.Close();
	}
	CDialog::OnOK();
}

void BuweiBZ::OnButton1()
{
	// TODO: Add your control notification handler code here
	int BuweiNum;
	StrEdit Dlg;     
	CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST1));
	BuweiNum=pList->GetCurSel();
	if (Dlg.DoModal()==IDOK) {	
		if (BuweiNum<0)	
		{
			pList->InsertString(0,Dlg.m_EditStr);
			BuweiBZName.InsertAt(0,Dlg.m_EditStr);
			BuweiBZLen.InsertAt((int)1,(WORD)0);
			m_Buwei=0;
		} else {
			pList->InsertString(BuweiNum+1,Dlg.m_EditStr);
			BuweiBZName.InsertAt(BuweiNum+1,Dlg.m_EditStr);
			BuweiBZLen.InsertAt((BuweiNum+2),(WORD)0);
			m_Buwei=BuweiNum+1;
		}
		BuweiBZLen[0]+=1;		
		pList->SetCurSel(m_Buwei);
		pList=(CListBox *)(GetDlgItem(IDC_LIST2));
		pList->ResetContent();
	}
}

void BuweiBZ::OnButton2()
{
	// TODO: Add your control notification handler code here
	int BuweiNum;
	StrEdit Dlg;
	CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST1));                         
	BuweiNum=pList->GetCurSel();if (BuweiNum<0) return;
	m_Buwei=BuweiNum;
	Dlg.m_EditStr=BuweiBZName[BuweiNum];
	if (Dlg.DoModal()==IDOK) {	                     
		pList->DeleteString(BuweiNum);
		pList->InsertString(BuweiNum,Dlg.m_EditStr);
		BuweiBZName.SetAt(BuweiNum,Dlg.m_EditStr);
		pList->SetCurSel(m_Buwei);
	}
}

void BuweiBZ::OnButton3()
{
	// TODO: Add your control notification handler code here
	int BuweiNum,site,i;
	StrEdit Dlg;
	CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST1));                         
	BuweiNum=pList->GetCurSel();if (BuweiNum<0) return;
	m_Buwei=BuweiNum;
	pList->DeleteString(BuweiNum);
	site=BuweiBZLen[0];
	for (i=0;i<BuweiNum;i++) site+=BuweiBZLen[i+1];
	for (i=0;i<(int)BuweiBZLen[BuweiNum+1];i++)
		BuweiBZName.RemoveAt(site); 
	BuweiBZName.RemoveAt(BuweiNum);
	BuweiBZLen.RemoveAt(BuweiNum+1);
	BuweiBZLen[0]-=1;
	pList=(CListBox *)(GetDlgItem(IDC_LIST2));
	pList->ResetContent();
}

void BuweiBZ::OnButton4()
{
	// TODO: Add your control notification handler code here
	int BZsel,site,i,BuweiNum;
	StrEdit Dlg;
	if (Dlg.DoModal()==IDOK) {	
		CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST1));  
		BuweiNum=pList->GetCurSel();
		if (BuweiNum<0) return;
		pList=(CListBox *)(GetDlgItem(IDC_LIST2));  
		BZsel=pList->GetCurSel();
		if (BZsel<0) {
			pList->InsertString(0,Dlg.m_EditStr);
			site=BuweiBZLen[0];
			for (i=0;i<BuweiNum;i++) site+=BuweiBZLen[i+1];
			BuweiBZName.InsertAt(site,Dlg.m_EditStr);
			BZsel=0;
		} else {    
			BZsel+=1;
			pList->InsertString(BZsel,Dlg.m_EditStr);
			site=BuweiBZLen[0];
			for (i=0;i<BuweiNum;i++) site+=BuweiBZLen[i+1];
			BuweiBZName.InsertAt(site+BZsel,Dlg.m_EditStr);
		}
		BuweiBZLen[BuweiNum+1]+=1;
		pList->SetCurSel(BZsel);
	}
}

void BuweiBZ::OnButton5()
{
	// TODO: Add your control notification handler code here
	int BZsel,site,i,BuweiNum;
	StrEdit Dlg;
	CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST1));  
	BuweiNum=pList->GetCurSel();
	pList=(CListBox *)(GetDlgItem(IDC_LIST2));   
	BZsel=pList->GetCurSel();if (BZsel<0) return;
	site=BuweiBZLen[0];
	for (i=0;i<BuweiNum;i++) site+=BuweiBZLen[i+1];
	site+=BZsel;
	Dlg.m_EditStr=BuweiBZName[site];
	if (Dlg.DoModal()==IDOK) {	                     
		pList->DeleteString(BZsel);
		pList->InsertString(BZsel,Dlg.m_EditStr);
		BuweiBZName.SetAt(site,Dlg.m_EditStr);
		pList->SetCurSel(BZsel);
	}
}

void BuweiBZ::OnButton6()
{
	// TODO: Add your control notification handler code here
	int BZsel,site,i,BuweiNum;
	StrEdit Dlg;
	CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST1));  
	BuweiNum=pList->GetCurSel();
	pList=(CListBox *)(GetDlgItem(IDC_LIST2));   
	BZsel=pList->GetCurSel();if (BZsel<0) return;
	site=BuweiBZLen[0];
	for (i=0;i<BuweiNum;i++) site+=BuweiBZLen[i+1];
	pList->DeleteString(BZsel);
	BuweiBZName.RemoveAt(site+BZsel);
	BuweiBZLen[BuweiNum+1]-=1;  
	BZsel-=1;if (BZsel<0) BZsel=0;
	pList->SetCurSel(BZsel);
}

void BuweiBZ::OnButton7()
{
	// TODO: Add your control notification handler code here
	EditReport Dlg;	  
	char Bingzhong[60],Bingzhong0[40],Bingzhong1[40],Bingzhong2[40],Bingzhong3[40],Strbuf[2048];
	CString buwei,filenm;
	fstream f,fd;
	CFileStatus status;
	int i;
	
	CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST2));  
	i=pList->GetCurSel();if (i<0) return;
	pList->GetText(i,Bingzhong);
	filenm=theApp.m_Exe_Working_Dir+"system\\tem.dat";
	fd.open(filenm,ios::out);
/*	strcpy(Bingzhong2,Bingzhong);strcat(Bingzhong2,"、");
	strcpy(Bingzhong3,"、");strcat(Bingzhong3,Bingzhong);
	strcpy(Bingzhong0,Bingzhong);strcat(Bingzhong0,"\r");
	strcpy(Bingzhong1,Bingzhong);strcat(Bingzhong1,"\r\n");*/
	strcpy(Bingzhong2,Bingzhong);strcat(Bingzhong2,"、");
	strcpy(Bingzhong3,"、");strcat(Bingzhong3,Bingzhong);
	strcpy(Bingzhong0,Bingzhong);strcat(Bingzhong0,"\r");
	strcpy(Bingzhong1,Bingzhong);strcat(Bingzhong1,"\n");
	pList=(CListBox *)(GetDlgItem(IDC_LIST1));  
	i=pList->GetCurSel();if (i<0) return;
	pList->GetText(i,Strbuf);
	buwei=theApp.m_Exe_Working_Dir+"system\\";buwei+=Strbuf;buwei+=".dat";
	Dlg.m_Report="";Dlg.m_Result="";strcpy(Strbuf,"");
	if (CFile::GetStatus(buwei,status))
	{
		f.open(buwei,ios::in|ios::nocreate);
		if (f.good()) {
			while(!f.eof() && (strcmp(Strbuf,Bingzhong)!=0) && (strstr(Strbuf,Bingzhong0)==NULL) && (strstr(Strbuf,Bingzhong1)==NULL) && (strstr(Strbuf,Bingzhong2)==NULL) && (strstr(Strbuf,Bingzhong3)==NULL)) {		
				if (strlen(Strbuf)>0) fd.write(Strbuf,strlen(Strbuf));
				while(!f.eof() && (strstr(strlwr(Strbuf),"end")==NULL)) {
					f.getline(Strbuf,2040);
					if (Strbuf[strlen(Strbuf)-1]!='\r') strcat(Strbuf,"\n");else Strbuf[strlen(Strbuf)-1]='\n';
					fd.write(Strbuf,strlen(Strbuf));
				}
				if (!f.eof()) f.getline(Strbuf,2040);
				if (Strbuf[strlen(Strbuf)-1]!='\r') strcat(Strbuf,"\n");else Strbuf[strlen(Strbuf)-1]='\n';
			}

			if (!f.eof()) {
				strcpy(Bingzhong,Strbuf);
				f.getline(Strbuf,2040);
				if (Strbuf[strlen(Strbuf)-1]!='\r') strcat(Strbuf,"\r\n");else strcat(Strbuf,"\n");
				while(!f.eof() && (strstr(Strbuf,"end")==NULL) && (strstr(Strbuf,"诊断意见：")==NULL)) {		
					Dlg.m_Report+=Strbuf;
					f.getline(Strbuf,2040);
					if (Strbuf[strlen(Strbuf)-1]!='\r') strcat(Strbuf,"\r\n");else strcat(Strbuf,"\n");
				}
				if (strstr(Strbuf,"诊断意见：")!=NULL) f.getline(Strbuf,2040);
				if (Strbuf[strlen(Strbuf)-1]!='\r') strcat(Strbuf,"\r\n");else strcat(Strbuf,"\n");
				while(!f.eof() && (strstr(Strbuf,"end")==NULL)) {		
					Dlg.m_Result+=Strbuf;
					f.getline(Strbuf,2040);
					if (Strbuf[strlen(Strbuf)-1]!='\r') strcat(Strbuf,"\r\n");else strcat(Strbuf,"\n");
				}

				while(!f.eof()) {		
					f.getline(Strbuf,2040);
					if (Strbuf[strlen(Strbuf)-1]!='\r') strcat(Strbuf,"\n");else Strbuf[strlen(Strbuf)-1]='\n';
					fd.write(Strbuf,strlen(Strbuf));
				}
			}
		}
		f.close();
	}
    if (Dlg.DoModal()==IDOK) {
    	if (strstr(Strbuf,"end")==NULL) {
	    	strcpy(Strbuf,"end\n");
    		fd.write(Strbuf,strlen(Strbuf));
    	}
		if ((Bingzhong[strlen(Bingzhong)-1]!='\r') && (Bingzhong[strlen(Bingzhong)-1]!='\n')) strcat(Bingzhong,"\n");else Bingzhong[strlen(Bingzhong)-1]='\n';
    	fd.write(Bingzhong,strlen(Bingzhong));

		CString Mystr,temstr;int site;
		Mystr=Dlg.m_Report;
		while (Mystr.GetLength()>0) {
			site=Mystr.Find('\r');
			if (site>-1) {
				temstr=Mystr.Left(site);
				Mystr=Mystr.Right(Mystr.GetLength()-site-2);
			} else {
				temstr=Mystr;
				Mystr="";
			}
			strcpy(Strbuf,temstr);strcat(Strbuf,"\n");
			fd.write(Strbuf,strlen(Strbuf));
		}
		fd.write("诊断意见：\n",11);

		Mystr=Dlg.m_Result;
		while (Mystr.GetLength()>0) {
			site=Mystr.Find('\r');
			if (site>-1) {
				temstr=Mystr.Left(site);
				Mystr=Mystr.Right(Mystr.GetLength()-site-2);
			} else {
				temstr=Mystr;
				Mystr="";
			}
			strcpy(Strbuf,temstr);strcat(Strbuf,"\n");
			fd.write(Strbuf,strlen(Strbuf));
		}
    	fd.write("end",3);
    	fd.close();
    	if (CFile::GetStatus(buwei,status)) CFile::Remove(buwei);
    	filenm=theApp.m_Exe_Working_Dir+"system\\tem.dat";
    	CFile::Rename(filenm,buwei);
    } else {
    	fd.close();
    	filenm=theApp.m_Exe_Working_Dir+"system\\tem.dat";
    	CFile::Remove(filenm);
	}
}

void BuweiBZ::OnButton8()
{
	// TODO: Add your control notification handler code here
	CFile DestFile;
	CString filenm,strbuf;int i;
	
	CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST1));  
	i=pList->GetCurSel();if (i<0) return;
	pList->GetText(i,strbuf);
	pList=(CListBox *)(GetDlgItem(IDC_LIST2));  
	i=pList->GetCurSel();if (i<0) return;
	pList->GetText(i,filenm);
	strbuf=strbuf+" "+filenm;
	
	filenm=theApp.m_Exe_Working_Dir;filenm+="system/bwbz.dat";
	DestFile.Open(filenm,CFile::modeCreate|CFile::modeWrite);
	CArchive archive(&DestFile,CArchive::store);
	archive<<(WORD)1;
	archive<<strbuf;
	archive.Close();DestFile.Close();
	int Width,Height;
	Width=GetSystemMetrics(SM_CXSCREEN);Height=GetSystemMetrics(SM_CYSCREEN);
	CWnd *pWnd=new CCreateReport();	
	HMENU SumMenu=LoadMenu(AfxGetInstanceHandle(),"IDR_MENU1");
	pWnd->CreateEx(WS_EX_WINDOWEDGE,AfxRegisterWndClass(CS_GLOBALCLASS|CS_NOCLOSE),"文本报告选择",WS_VISIBLE|WS_CAPTION|WS_POPUP,0,0,Width,Height, m_hWnd, SumMenu);
	pWnd->ShowWindow(SW_SHOW);
}

void BuweiBZ::OnSelchangeList1()
{
	// TODO: Add your control notification handler code here
	int BuweiNum,site,i;
	CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST1));
	BuweiNum=pList->GetCurSel();if (BuweiNum<0) return;
	if (BuweiNum!=m_Buwei) {
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
		m_Buwei=BuweiNum;
	}	
}
