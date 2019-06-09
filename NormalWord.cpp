// NormalWord.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "NormalWord.h"
#include "MedDiagnose.h"
#include "MedMaterial.h"
#include "Bdbg.h"
#include "Bcbg.h"
#include "Xgyj.h"
#include "NormalWord.h"
#include "MedTbs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNormalWord dialog
extern CPathologyApp theApp;

CNormalWord::CNormalWord(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CNormalWord::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNormalWord)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_nID = CNormalWord::IDD;

	m_Buwei.Empty();
}

BOOL CNormalWord::Create()
{
	return CDialog::Create(m_nID, m_pParent);
}

void CNormalWord::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNormalWord)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNormalWord, CXTResizeDialog)
	//{{AFX_MSG_MAP(CNormalWord)
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
	ON_LBN_DBLCLK(IDC_LIST2, OnDblclkList2)
	ON_BN_CLICKED(IDSWITCH, OnSwitch)
	ON_LBN_SELCHANGE(IDC_LIST4, OnSelchangeList4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNormalWord message handlers
void CNormalWord::AdjustPosition(void)
{    
	CRect rect;int height;
	CStatic *pStatic;
	GetWindowRect(&rect); 
	
	UINT ct;
	if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedDiagnose)))
	{
		ct = ((CMedDiagnose *)m_pParent)->CurEdit;
	}

	if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedMaterial)))
	{
		ct = ((CMedMaterial *)m_pParent)->CurEdit;
	}

	if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedTbs)))
	{
		ct = ((CMedTbs *)m_pParent)->CurEdit;
	}

	if(m_pParent->IsKindOf(RUNTIME_CLASS(CBdbg)))
	{
		ct = ((CBdbg *)m_pParent)->CurEdit;
	}

	if(m_pParent->IsKindOf(RUNTIME_CLASS(CBcbg)))
	{
		ct = ((CBcbg *)m_pParent)->CurEdit;
	}

	if(m_pParent->IsKindOf(RUNTIME_CLASS(CXgyj)))
	{
		//ct = ((CXgyj *)m_pParent)->CurEdit;
		ct = 2;
	}

	if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedDiagnose)))
	{
		CRect nParentRect;
		m_pParent->GetWindowRect(&nParentRect);

		height=rect.Height();
		if(ct == 2)
			rect.top = nParentRect.top;
		else
			rect.top = nParentRect.bottom - height;
		SetWindowPos(NULL, nParentRect.left, rect.top, nParentRect.Width(), height, SWP_NOZORDER);		
	}

	if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedMaterial)))
	{
		CRect nParentRect;
		m_pParent->GetWindowRect(&nParentRect);

		height=rect.Height();
		rect.top = nParentRect.top;
		SetWindowPos(NULL, nParentRect.left, rect.top, nParentRect.Width(), height, SWP_NOZORDER);		
	}

	if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedTbs)))
	{
		CRect nParentRect;
		m_pParent->GetWindowRect(&nParentRect);

		height=rect.Height();
		if(ct == 2)
			rect.top = nParentRect.top;
		else
			rect.top = nParentRect.bottom - height;
		SetWindowPos(NULL, nParentRect.left, rect.top, nParentRect.Width(), height, SWP_NOZORDER);		
	}

	if(m_pParent->IsKindOf(RUNTIME_CLASS(CBdbg)))
	{
		CRect nParentRect;
		((CEdit *)(((CBdbg *)m_pParent)->GetDlgItem(IDC_EDIT_BDJG)))->GetWindowRect(&nParentRect);

		height=rect.Height();
		rect.top = nParentRect.bottom;
		SetWindowPos(NULL, nParentRect.left, rect.top, nParentRect.Width(), height, SWP_NOZORDER);		
	}

	if(m_pParent->IsKindOf(RUNTIME_CLASS(CBcbg)))
	{
		CRect nParentRect;
		((CEdit *)(((CBcbg *)m_pParent)->GetDlgItem(IDC_EDIT_BCZD)))->GetWindowRect(&nParentRect);

		height=rect.Height();
		rect.top = nParentRect.bottom;
		SetWindowPos(NULL, nParentRect.left, rect.top, nParentRect.Width(), height, SWP_NOZORDER);		
	}

	if(m_pParent->IsKindOf(RUNTIME_CLASS(CXgyj)))
	{
		CRect nParentRect;
		((CEdit *)(((CXgyj *)m_pParent)->GetDlgItem(IDC_EDIT_XGYJ)))->GetWindowRect(&nParentRect);

		SetWindowPos(NULL, nParentRect.left, rect.top, nParentRect.Width(), rect.Height()-20, SWP_NOZORDER);		
	}


	switch (ct) {
		case 1:
			pStatic=(CStatic *)GetDlgItem(IDC_FUNCTION);
			pStatic->SetWindowText("光镜所见常用词"); // ("描述常用词");
			break;
		case 2:
			pStatic=(CStatic *)GetDlgItem(IDC_FUNCTION);
			pStatic->SetWindowText("诊断常用词");
			break;
		case 3:
			pStatic=(CStatic *)GetDlgItem(IDC_FUNCTION);
			pStatic->SetWindowText("肉眼所见常用词");
			break;
	}

	LoadList();
}

BOOL CNormalWord::LoadList(void)
{   
	CFile DisFile;
	CFileStatus status;
	WORD len,i,j,Select;
	CString strbuf,filenm;
	CListBox *pList;
	filenm = theApp.m_Exe_Working_Dir;

	UINT ct;
	if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedDiagnose)))
	{
		ct = ((CMedDiagnose *)m_pParent)->CurEdit;
	}
	if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedMaterial)))
	{
		ct = ((CMedMaterial *)m_pParent)->CurEdit;
	}
	if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedTbs)))
	{
		ct = ((CMedTbs *)m_pParent)->CurEdit;
	}
	if(m_pParent->IsKindOf(RUNTIME_CLASS(CBdbg)))
	{
		ct = ((CBdbg *)m_pParent)->CurEdit;
	}
	if(m_pParent->IsKindOf(RUNTIME_CLASS(CBcbg)))
	{
		ct = ((CBcbg *)m_pParent)->CurEdit;
	}
	if(m_pParent->IsKindOf(RUNTIME_CLASS(CXgyj)))
	{
		//ct = ((CXgyj *)m_pParent)->CurEdit;
		ct = 2;
	}

	switch (ct) {
		default:filenm+="system/描述用词.dat";break;
		case 2:filenm+="system/诊断用词.dat";break;
		case 3:filenm+="system/所见用词.dat";break;
	}
	if (CFile::GetStatus(filenm,status))
	{
		DisFile.Open(filenm,CFile::modeRead);
		CArchive archive(&DisFile,CArchive::load);
		TRY {
			archive>>len;
			pList=(CListBox *)(GetDlgItem(IDC_LIST4));  
			pList->ResetContent();Select=0;
			for (i=0;i<len;i++) {
				archive>>strbuf;     
				pList->AddString(strbuf);
				if (m_Buwei.Compare(strbuf)==0) Select=i;
			}
			for (i=0;i<Select;i++) {
				archive>>len;
				for (j=0;j<len;j++)	archive>>strbuf;
			}
			pList->SetCurSel(Select);
			pList=(CListBox *)(GetDlgItem(IDC_LIST1));  
			pList->ResetContent();archive>>len;
			for (j=0;j<len;j++)	{
				archive>>strbuf;
				pList->AddString(strbuf);
			}
		} 
		CATCH (CArchiveException,e)
		{
		}
		END_CATCH		
		archive.Close();DisFile.Close();
	} 
	
	filenm =theApp.m_Exe_Working_Dir;filenm+="system/位置用词.dat";
	if (CFile::GetStatus(filenm,status))
	{
		DisFile.Open(filenm,CFile::modeRead);
		CArchive archive(&DisFile,CArchive::load);
		TRY {
			archive>>len;
			Select=0;
			for (i=0;i<len;i++) {
				archive>>strbuf;     
				if (m_Buwei.Compare(strbuf)==0) Select=i;
			}
			for (i=0;i<Select;i++) {
				archive>>len;
				for (j=0;j<len;j++)	archive>>strbuf;
			}
			pList=(CListBox *)(GetDlgItem(IDC_LIST2));  
			pList->ResetContent();archive>>len;
			for (j=0;j<len;j++)	{
				archive>>strbuf;
				pList->AddString(strbuf);
			}
		} 
		CATCH (CArchiveException,e)
		{
		}
		END_CATCH		
		archive.Close();DisFile.Close();
	}
	return TRUE;
}
void CNormalWord::OnDblclkList1() 
{
	// TODO: Add your control notification handler code here
	int Sele;
	HANDLE hData;LPSTR lpData;
	CString strbuf;
	CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST1));  
	Sele=pList->GetCurSel();
	if (Sele<0) return;
	pList->GetText(Sele,strbuf);
	if (!(hData=GlobalAlloc(GMEM_MOVEABLE,strbuf.GetLength()+1))) {AfxMessageBox("内存不够！");return ;}
	if (!(lpData=(LPSTR)GlobalLock(hData))) {AfxMessageBox("内存不够！");return ;}
	lstrcpy(lpData,strbuf);
	GlobalUnlock(hData);
	if (OpenClipboard()) {
		EmptyClipboard();
		SetClipboardData(CF_TEXT,hData);
		CloseClipboard();
	}

	if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedDiagnose)))
	{
		switch(((CMedDiagnose *)m_pParent)->CurEdit) {
		case 1:
			((CEdit *)(((CMedDiagnose *)m_pParent)->GetDlgItem(IDC_EDIT_GJSJ)))->Paste();
			break;
		case 2:
			((CEdit *)(((CMedDiagnose *)m_pParent)->GetDlgItem(IDC_EDIT_ZDYJ)))->Paste();
			break;
		case 3:
			((CEdit *)(((CMedDiagnose *)m_pParent)->GetDlgItem(IDC_EDIT_RYSJ)))->Paste();
			break;
		}
	}
	if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedMaterial)))
	{
		((CEdit *)(((CMedMaterial *)m_pParent)->GetDlgItem(IDC_EDIT_RYSJ)))->Paste();
	}

	if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedTbs)))
	{
		switch(((CMedTbs *)m_pParent)->CurEdit) {
		case 2:
			((CEdit *)(((CMedTbs *)m_pParent)->GetDlgItem(IDC_EDIT_ZDYJ)))->Paste();
			break;
		case 3:
			((CEdit *)(((CMedTbs *)m_pParent)->GetDlgItem(IDC_EDIT_RYSJ)))->Paste();
			break;
		}
	}

	if(m_pParent->IsKindOf(RUNTIME_CLASS(CBdbg)))
	{
		((CEdit *)(((CBdbg *)m_pParent)->GetDlgItem(IDC_EDIT_BDJG)))->Paste();
	}
	if(m_pParent->IsKindOf(RUNTIME_CLASS(CBcbg)))
	{
		((CEdit *)(((CBcbg *)m_pParent)->GetDlgItem(IDC_EDIT_BCZD)))->Paste();
	}
	if(m_pParent->IsKindOf(RUNTIME_CLASS(CXgyj)))
	{
		if(((CXgyj*)m_pParent)->CurEdit == 1)
		{
			((CEdit *)(((CXgyj *)m_pParent)->GetDlgItem(IDC_EDIT_CZYJ)))->Paste();
		}
		else if(((CXgyj*)m_pParent)->CurEdit == 2)
		{
			((CEdit *)(((CXgyj *)m_pParent)->GetDlgItem(IDC_EDIT_XGYJ)))->Paste();
		}
		else
		{
			((CEdit *)(((CXgyj *)m_pParent)->GetDlgItem(IDC_EDIT_HZYJ)))->Paste();
		}
		
	}
}

void CNormalWord::OnDblclkList2() 
{
	// TODO: Add your control notification handler code here
	int Sele;
	HANDLE hData;LPSTR lpData;
	CString strbuf;
	CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST2));  
	Sele=pList->GetCurSel();
	if (Sele<0) return;
	pList->GetText(Sele,strbuf);
	if (!(hData=GlobalAlloc(GMEM_MOVEABLE,strbuf.GetLength()+1))) {AfxMessageBox("内存不够！");return ;}
	if (!(lpData=(LPSTR)GlobalLock(hData))) {AfxMessageBox("内存不够！");return ;}
	lstrcpy(lpData,strbuf);
	GlobalUnlock(hData);
	if (OpenClipboard()) {
		EmptyClipboard();
		SetClipboardData(CF_TEXT,hData);
		CloseClipboard();
	}

	if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedDiagnose)))
	{
		switch(((CMedDiagnose *)m_pParent)->CurEdit) {
		case 1:
			((CEdit *)(((CMedDiagnose *)m_pParent)->GetDlgItem(IDC_EDIT_GJSJ)))->Paste();
			break;
		case 2:
			((CEdit *)(((CMedDiagnose *)m_pParent)->GetDlgItem(IDC_EDIT_ZDYJ)))->Paste();
			break;
		case 3:
			((CEdit *)(((CMedDiagnose *)m_pParent)->GetDlgItem(IDC_EDIT_RYSJ)))->Paste();
			break;
		}
	}
	if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedMaterial)))
	{
		((CEdit *)(((CMedMaterial *)m_pParent)->GetDlgItem(IDC_EDIT_RYSJ)))->Paste();
	}
	if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedTbs)))
	{
		switch(((CMedTbs *)m_pParent)->CurEdit) {
		case 2:
			((CEdit *)(((CMedTbs *)m_pParent)->GetDlgItem(IDC_EDIT_ZDYJ)))->Paste();
			break;
		case 3:
			((CEdit *)(((CMedTbs *)m_pParent)->GetDlgItem(IDC_EDIT_RYSJ)))->Paste();
			break;
		}
	}
	if(m_pParent->IsKindOf(RUNTIME_CLASS(CBdbg)))
	{
		((CEdit *)(((CBdbg *)m_pParent)->GetDlgItem(IDC_EDIT_BDJG)))->Paste();
	}
	if(m_pParent->IsKindOf(RUNTIME_CLASS(CBcbg)))
	{
		((CEdit *)(((CBcbg *)m_pParent)->GetDlgItem(IDC_EDIT_BCZD)))->Paste();
	}
	if(m_pParent->IsKindOf(RUNTIME_CLASS(CXgyj)))
	{
		((CEdit *)(((CXgyj *)m_pParent)->GetDlgItem(IDC_EDIT_XGYJ)))->Paste();
	}
}

BOOL CNormalWord::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    SetResize(IDC_LIST1, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_LIST2, SZ_TOP_RIGHT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_LIST4, SZ_TOP_RIGHT, SZ_BOTTOM_RIGHT);

    SetResize(IDC_FUNCTION, SZ_TOP_LEFT, SZ_TOP_LEFT);
    SetResize(IDC_SITE, SZ_TOP_RIGHT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_ZQM,  SZ_TOP_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDSWITCH, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNormalWord::OnOK() 
{
	ShowWindow(FALSE);
	CXTResizeDialog::OnOK();
}

void CNormalWord::OnSwitch() 
{
	if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedTbs)))
	{
		((CMedTbs *)m_pParent)->SwitchNormalWnd();
	}

	if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedDiagnose)))
	{
		((CMedDiagnose *)m_pParent)->SwitchNormalWnd();
	}
}

void CNormalWord::OnSelchangeList4() 
{
	CListBox *pList=(CListBox *)(GetDlgItem(IDC_LIST4));  
	int Sele=pList->GetCurSel();
	if (Sele<0) return;
	pList->GetText(Sele,m_Buwei);
	LoadList();
}
