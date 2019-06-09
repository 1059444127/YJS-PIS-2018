// ZhengDuan.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "ZhengDuan.h"

#include "StrEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZhengDuan dialog

CZhengDuan::CZhengDuan(CWnd* pParent /*=NULL*/)
	: CDialog(CZhengDuan::IDD, pParent)
{
	//{{AFX_DATA_INIT(CZhengDuan)
	m_Buwei = -1;
	m_Tishi2 = "";
	//}}AFX_DATA_INIT
}

CZhengDuan::CZhengDuan(CString filenm,CWnd* pParent /*=NULL*/)
	: CDialog(CZhengDuan::IDD, pParent)
{
	m_Buwei = -1;
	m_filenm=filenm;
}

void CZhengDuan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZhengDuan)
	DDX_LBIndex(pDX, IDC_LIST1, m_Buwei);
	DDX_Text(pDX, IDC_EDIT1, m_Tishi2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CZhengDuan, CDialog)
	//{{AFX_MSG_MAP(CZhengDuan)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_BN_CLICKED(IDUP, OnUp)
	ON_BN_CLICKED(IDDOWN, OnDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CZhengDuan message handlers

BOOL CZhengDuan::OnInitDialog()
{   
	// TODO: Add your command handler code here
	WORD len,i,j,site;
	CFile ZhengDuanFile;
	CFileStatus status;
	if (m_filenm.Find("用词")>-1) {
		CStatic *pStatic=(CStatic *)(GetDlgItem(IDC_STATIC1));
		pStatic->SetWindowText("部位");
		pStatic=(CStatic *)(GetDlgItem(IDC_STATIC2));
		pStatic->SetWindowText("常用词");
		CButton *pButton=(CButton *)(GetDlgItem(IDC_BUTTON1));
		pButton->SetWindowText("增加脏器");
		pButton=(CButton *)(GetDlgItem(IDC_BUTTON2));
		pButton->SetWindowText("修改脏器");
		pButton=(CButton *)(GetDlgItem(IDC_BUTTON3));
		pButton->SetWindowText("删除脏器");
	}   
	site=m_filenm.ReverseFind('\\');
	SetWindowText(m_filenm.Right(m_filenm.GetLength()-site-1));
	ZhengDuanName.RemoveAll();ZhengDuanLen.RemoveAll();
	if (CFile::GetStatus(m_filenm,status))
	{
		ZhengDuanFile.Open(m_filenm,CFile::modeRead);
		CArchive archive(&ZhengDuanFile,CArchive::load);
		TRY {
			archive>>len;
			ZhengDuanLen.Add(len);
			CString strbuf;
			for (i=0;i<ZhengDuanLen[0];i++) {
				archive>>strbuf;
				ZhengDuanName.Add(strbuf);
			}                       
			for (i=0;i<ZhengDuanLen[0];i++) {
				archive>>len;
				ZhengDuanLen.Add(len);
				for (j=0;j<len;j++)
				{
					archive>>strbuf;
					ZhengDuanName.Add(strbuf);
				}
			}
		} 
		CATCH (CArchiveException,e)
		{
			ZhengDuanName.RemoveAll();ZhengDuanLen.RemoveAll();
		}
		END_CATCH		
		archive.Close();ZhengDuanFile.Close();
	} else ZhengDuanLen.Add(0);
	
	if (ZhengDuanName.GetSize()>0)
	{
		CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST1));                         
		for (i=0;i<ZhengDuanLen[0];i++)
		{
			pList->AddString(ZhengDuanName[i]);
		}
		
		site=ZhengDuanLen[0];m_Tishi2="";
		for (i=0;i<ZhengDuanLen[1];i++)
		{
			m_Tishi2+=ZhengDuanName[site]+"\r\n";
			site++;
		}
	}
	m_Buwei=0;

	CenterWindow();
	CDialog::OnInitDialog();
	return TRUE;
}

void CZhengDuan::OnOK()
{   
	// TODO: Add extra validation here
	WORD i,j,site;
	CFile ZhengDuanFile;
	
	UpdateData(TRUE);Change(m_Buwei,m_Tishi2);
	
	if (ZhengDuanFile.Open(m_filenm,CFile::modeWrite|CFile::modeCreate))
	{
		CArchive archive(&ZhengDuanFile,CArchive::store);
		archive<<ZhengDuanLen[0];
		for (i=0;i<ZhengDuanLen[0];i++) {
			archive<<ZhengDuanName[i];
		}
		site=ZhengDuanLen[0];
		for (i=0;i<ZhengDuanLen[0];i++) {
			archive<<ZhengDuanLen[i+1];
			for (j=0;j<ZhengDuanLen[i+1];j++)
			{
				archive<<ZhengDuanName[site];
				site++;
			}
		}
		archive.Close();ZhengDuanFile.Close();
	}
	CDialog::OnOK();
}

void CZhengDuan::OnButton1()
{
	// TODO: Add your control notification handler code here
	int BuweiNum;
	StrEdit Dlg;     
	UpdateData(TRUE);Change(m_Buwei,m_Tishi2);
	CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST1));
	BuweiNum=pList->GetCurSel();
	if (Dlg.DoModal()==IDOK) {	
		if (BuweiNum<0)	
		{
			pList->InsertString(0,Dlg.m_EditStr);
			ZhengDuanName.InsertAt(0,Dlg.m_EditStr);
			ZhengDuanLen.InsertAt((int)1,(WORD)0);
			m_Buwei=0;
		} else {
			pList->InsertString(BuweiNum+1,Dlg.m_EditStr);
			ZhengDuanName.InsertAt(BuweiNum+1,Dlg.m_EditStr);
			ZhengDuanLen.InsertAt((BuweiNum+2),(WORD)0);
			m_Buwei=BuweiNum+1;
		}
		ZhengDuanLen[0]+=1;		
		pList->SetCurSel(m_Buwei);
		m_Tishi2="";UpdateData(FALSE);
	}
}

void CZhengDuan::OnButton2()
{
	// TODO: Add your control notification handler code here
	int BuweiNum;
	StrEdit Dlg;
	UpdateData(TRUE);Change(m_Buwei,m_Tishi2);
	CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST1));                         
	BuweiNum=pList->GetCurSel();if (BuweiNum<0) return;
	m_Buwei=BuweiNum;
	Dlg.m_EditStr=ZhengDuanName[BuweiNum];
	if (Dlg.DoModal()==IDOK) {	                     
		pList->DeleteString(BuweiNum);
		pList->InsertString(BuweiNum,Dlg.m_EditStr);
		ZhengDuanName.SetAt(BuweiNum,Dlg.m_EditStr);
		pList->SetCurSel(m_Buwei);
	}
}

void CZhengDuan::OnButton3()
{
	// TODO: Add your control notification handler code here
	int BuweiNum,site,i;
	StrEdit Dlg;
	CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST1));                         
	BuweiNum=pList->GetCurSel();if (BuweiNum<0) return;
	m_Buwei=BuweiNum;
	pList->DeleteString(BuweiNum);
	site=ZhengDuanLen[0];
	for (i=0;i<BuweiNum;i++) site+=ZhengDuanLen[i+1];
	for (i=0;i<(int)ZhengDuanLen[BuweiNum+1];i++)
		ZhengDuanName.RemoveAt(site); 
	ZhengDuanName.RemoveAt(BuweiNum);
	ZhengDuanLen.RemoveAt(BuweiNum+1);
	ZhengDuanLen[0]-=1;
	if (m_Buwei>0) m_Buwei--;
	pList->SetCurSel(m_Buwei);
	Get(m_Buwei,m_Tishi2);UpdateData(FALSE);	
}

void CZhengDuan::Change(int OldBuwei,CString Strbuf)
{
	CString Rs;int i,site,num;
	site=ZhengDuanLen[0];
	for (i=0;i<OldBuwei;i++) site+=ZhengDuanLen[i+1];
	for (i=0;i<(int)ZhengDuanLen[OldBuwei+1];i++)
	{
		ZhengDuanName.RemoveAt(site);
	}
	num=0;
	Rs.Empty();
	for (i=0;i<(int)Strbuf.GetLength();i++)
	{
		if (Strbuf.GetAt(i)!='\r') 
		{                 
			if ((Rs.GetLength()>0) || (Strbuf.GetAt(i)!=' ')) Rs+=Strbuf.GetAt(i);
		} else {
			if (Rs.GetLength()>0) {ZhengDuanName.InsertAt(site,Rs);site++;num++;}
			i++;	//skip \n 
			Rs.Empty();   
		}
	}
	if (Rs.GetLength()>0) {ZhengDuanName.InsertAt(site,Rs);site++;num++;}
	ZhengDuanLen[OldBuwei+1]=num;
}

void CZhengDuan::Get(int NewBuwei,CString &Strbuf)
{
	int site,i;
	site=ZhengDuanLen[0];
	for (i=0;i<NewBuwei;i++) site+=ZhengDuanLen[i+1];
	Strbuf="";
	for (i=0;i<(int)ZhengDuanLen[NewBuwei+1];i++)
	{
		Strbuf+=ZhengDuanName[site]+"\r\n";
		site++;
	}
}

void CZhengDuan::OnSelchangeList1()
{
	// TODO: Add your control notification handler code here
	int BuweiNum,OldBuwei;
	CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST1));
	BuweiNum=pList->GetCurSel();if (BuweiNum<0) return;
	if (BuweiNum!=m_Buwei) {
		OldBuwei=m_Buwei;UpdateData(TRUE);Change(OldBuwei,m_Tishi2);
		Get(BuweiNum,m_Tishi2);UpdateData(FALSE);
		m_Buwei=BuweiNum;
	}	
}

void CZhengDuan::OnUp() 
{
	CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST1));                         
	int BuweiNum = pList->GetCurSel();
	if(!(ZhengDuanLen[0] > 1 && BuweiNum > 0))  return;

	UpdateData(TRUE);
	Change(BuweiNum,m_Tishi2);
	CString m_OpTishi2 = m_Tishi2;
	CString m_BwName   = ZhengDuanName[BuweiNum];

	pList->SetCurSel(BuweiNum);
	OnButton3();

	BuweiNum--;

	pList->InsertString(BuweiNum,m_BwName);
	ZhengDuanName.InsertAt(BuweiNum,m_BwName);
	ZhengDuanLen.InsertAt((BuweiNum+1),(WORD)0);
	m_Buwei = BuweiNum;
	ZhengDuanLen[0]+=1;		
	pList->SetCurSel(m_Buwei);
	Change(BuweiNum,m_OpTishi2);
	m_Tishi2 = m_OpTishi2;
	UpdateData(FALSE);
}

void CZhengDuan::OnDown() 
{
	CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST1));                         
	int BuweiNum = pList->GetCurSel();
	if(!(ZhengDuanLen[0] > 1 && BuweiNum < (ZhengDuanLen[0]-1)))  return;

	UpdateData(TRUE);
	Change(BuweiNum,m_Tishi2);
	CString m_OpTishi2 = m_Tishi2;
	CString m_BwName   = ZhengDuanName[BuweiNum];

	pList->SetCurSel(BuweiNum);
	OnButton3();

	BuweiNum++;

	pList->InsertString(BuweiNum,m_BwName);
	ZhengDuanName.InsertAt(BuweiNum,m_BwName);
	ZhengDuanLen.InsertAt((BuweiNum+1),(WORD)0);
	m_Buwei = BuweiNum;
	ZhengDuanLen[0]+=1;		
	pList->SetCurSel(m_Buwei);
	Change(BuweiNum,m_OpTishi2);
	m_Tishi2 = m_OpTishi2;
	UpdateData(FALSE);
}
