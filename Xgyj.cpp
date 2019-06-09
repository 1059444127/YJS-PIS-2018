// Xgyj.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "Pathologyview.h"
#include "Pathologydoc.h"
#include "Xgyj.h"
#include "MedDiagnose.h"
#include "NormalWord.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
/////////////////////////////////////////////////////////////////////////////
// CXgyj dialog
extern CPathologyApp theApp;

CXgyj::CXgyj(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CXgyj::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXgyj)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;

	CurEdit = 0;	
	pNormalWord = NULL;
}


void CXgyj::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXgyj)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_EDIT_CZYJ, m_ctrl_czyj);
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNCREATE(CXgyj, CXTResizeDialog)

BEGIN_MESSAGE_MAP(CXgyj, CXTResizeDialog)
	//{{AFX_MSG_MAP(CXgyj)
	ON_BN_CLICKED(IDC_BUTTON_TQZD, OnButtonTqzd)
	ON_BN_CLICKED(IDC_BUTTON_HXZD, OnButtonHxzd)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_CYC, OnButtonCyc)
	ON_EN_SETFOCUS(IDC_EDIT_XGYJ, OnSetfocusEditXgyj)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_COPYCZ, OnButtonCopycz)
	ON_BN_CLICKED(IDC_BUTTON_COPYZZ, OnButtonCopyzz)
	ON_EN_SETFOCUS(IDC_EDIT_CZYJ, OnSetfocusEditCzyj)
	ON_EN_SETFOCUS(IDC_EDIT_HZYJ, OnSetfocusEditHzyj)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXgyj message handlers

BOOL CXgyj::OnInitDialog() 
{
	
	CXTResizeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    

	SetResize(IDC_BUTTON_TQZD, SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
    SetResize(IDC_BUTTON_HXZD, SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
    SetResize(IDC_BUTTON_SAVE, SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
    SetResize(IDOK, SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
    SetResize(IDCANCEL, SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);

    SetResize(IDC_STATIC_CZYJ, SZ_TOP_LEFT,  SZ_TOP_LEFT);
    SetResize(IDC_EDIT_CZYJ, SZ_TOP_LEFT,  CXTResizePoint( 1, 0.5));
    SetResize(IDC_STATIC_XGYJ, CXTResizePoint( 0, 0.5),  CXTResizePoint( 0, 0.5));
    SetResize(IDC_EDIT_XGYJ, CXTResizePoint( 0, 0.5),  SZ_BOTTOM_RIGHT);

	m_czyj.Empty();  m_xgyj.Empty(); m_hzyj.Empty();

	theApp.ConnectDatabase();

	try
	{
		SAString cmdstr = "Select czyj,xgyj,wyyj from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
		g_dbcommand.Execute();
		
		if(g_dbcommand.FetchNext())
		{
			m_czyj = g_dbcommand.Field("czyj").asString();
			m_xgyj = g_dbcommand.Field("xgyj").asString();
			m_hzyj = g_dbcommand.Field("wyyj").asString();
			m_czyj.TrimLeft();  m_czyj.TrimRight();
			m_xgyj.TrimLeft();  m_xgyj.TrimRight();
			m_hzyj.TrimLeft();  m_hzyj.TrimRight();
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

	CString str;
	((CMedDiagnose *)m_pParent)->GetDlgItemText(IDC_EDIT_ZDYJ, str);
	str.TrimLeft(); str.TrimRight();

	CString firstDoctor; CString mainDoctor; CString checkDoctor;
	((CMedDiagnose *)m_pParent)->GetDlgItemText(IDC_EDIT_CZYS, firstDoctor);
	((CMedDiagnose *)m_pParent)->GetDlgItemText(IDC_EDIT_ZZYS, mainDoctor);
	((CMedDiagnose *)m_pParent)->GetDlgItemText(IDC_EDIT_SHYS, checkDoctor);
	firstDoctor.TrimLeft();firstDoctor.TrimRight();
	mainDoctor.TrimLeft();mainDoctor.TrimRight();
	CString user = theApp.m_Cpp_User;
	SetDlgItemText(IDC_EDIT_CZYJ,m_czyj);
	SetDlgItemText(IDC_EDIT_XGYJ,m_xgyj);
	SetDlgItemText(IDC_EDIT_HZYJ,m_hzyj);
	if(checkDoctor.Find(user) != -1)
	{
		
	} 
	else if(mainDoctor.Find(user) != -1)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_HZYJ))->SetReadOnly(TRUE);
	} 
	else if(firstDoctor.Find(user) != -1)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_XGYJ))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_HZYJ))->SetReadOnly(TRUE);
	} 
	else 
	{
		((CEdit*)GetDlgItem(IDC_EDIT_CZYJ))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_XGYJ))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_HZYJ))->SetReadOnly(TRUE);
		/*
		if(m_czyj.CompareNoCase("") == 0 && m_xgyj.CompareNoCase("") == 0)
				{
					
				} 
				else if(m_xgyj.CompareNoCase("") == 0 && m_czyj.CompareNoCase("") !=0)
				{
					SetDlgItemText(IDC_EDIT_XGYJ,m_xgyj);
				    ((CEdit*)(CWnd::GetDlgItem(IDC_EDIT_XGYJ)))->SetReadOnly(TRUE);
				} 
				else if(m_xgyj.CompareNoCase("")!=0 && m_czyj.CompareNoCase("") == 0)
				{
					SetDlgItemText(IDC_EDIT_CZYJ,m_czyj);
					((CEdit*)(CWnd::GetDlgItem(IDC_EDIT_CZYJ)))->SetReadOnly(TRUE);
				} 
				else 
				{
					SetDlgItemText(IDC_EDIT_CZYJ,m_czyj);
					SetDlgItemText(IDC_EDIT_XGYJ,m_xgyj);
					((CEdit*)(CWnd::GetDlgItem(IDC_EDIT_XGYJ)))->SetReadOnly(TRUE);
					((CEdit*)(CWnd::GetDlgItem(IDC_EDIT_CZYJ)))->SetReadOnly(TRUE);
				}*/
		
	}

	//if(!m_czyj.IsEmpty())
	//	SetDlgItemText(IDC_EDIT_CZYJ, m_czyj);
	//else
	//	SetDlgItemText(IDC_EDIT_CZYJ, str);
	//
	//SetDlgItemText(IDC_EDIT_XGYJ, m_xgyj);

	//---------------------------------------------------
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_TQZD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_HXZD)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_TQZD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_HXZD)->EnableWindow(TRUE);
	}
	//---------------------------------------------------

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXgyj::OnButtonTqzd() 
{
	// TODO: Add your control notification handler code here
/*	CString m_xgyj;
	try
	{		
		SAString cmdstr = "Select xgyj from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;		
		g_dbcommand.Execute();
		
		if(g_dbcommand.FetchNext())
		{
			m_xgyj = g_dbcommand.Field("xgyj").asString();
			m_xgyj.TrimLeft();  m_xgyj.TrimRight();
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
	
	if(!m_xgyj.IsEmpty())
	{
		AfxMessageBox("因报告权限控制，您当前操作无效!");
		return;
	}

	GetDlgItemText(IDC_EDIT_CZYJ, m_czyj);
	SetDlgItemText(IDC_EDIT_XGYJ, m_czyj);
	*/
	CString zzys,zzyj;
	m_pParent->GetDlgItemText(IDC_EDIT_ZZYS,zzys);
	GetDlgItemText(IDC_EDIT_XGYJ,zzyj);
	if(theApp.m_Cpp_User.CompareNoCase(zzys) == 0)
	{
			if(AfxMessageBox("您是否确认将本病例的主诊意见作为最终诊断结果?",MB_OKCANCEL) == IDOK)
		m_pParent->SetDlgItemText(IDC_EDIT_ZDYJ,zzyj);
	}
	else
	{
		AfxMessageBox("您不是主诊医师，不能执行回写操作!");
	}
}

void CXgyj::OnButtonHxzd() //会诊回写
{
	CString shys,hzyj;
	((CMedDiagnose *)m_pParent)->GetDlgItemText(IDC_EDIT_SHYS, shys);
	//((CMedDiagnose *)m_pParent)->GetDlgItemText(IDC_EDIT_ZZYS, zzys);
	GetDlgItemText(IDC_EDIT_HZYJ,hzyj);
	if(shys.CompareNoCase(theApp.m_Cpp_User) == 0)
	{		
		if(AfxMessageBox("您是否确认将本病例的会诊意见作为最终诊断结果?",MB_OKCANCEL) == IDOK)
		((CMedDiagnose *)m_pParent)->SetDlgItemText(IDC_EDIT_ZDYJ, hzyj);
	}
	else
	{
		AfxMessageBox("您不是审核医师，不能执行回写操作!");
	}
	
	
	// TODO: Add your control notification handler code here
//	GetDlgItemText(IDC_EDIT_XGYJ, m_xgyj);
	
	
}

void CXgyj::OnButtonSave() 
{
	// TODO: Add your control notification handler code here


	GetDlgItemText(IDC_EDIT_CZYJ, m_czyj);
	GetDlgItemText(IDC_EDIT_XGYJ, m_xgyj);
	GetDlgItemText(IDC_EDIT_HZYJ, m_hzyj);
	
	try
	{
		SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set czyj=:czyj,xgyj=:xgyj,wyyj=:wyyj where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("czyj").setAsString() = m_czyj;
		g_dbcommand.Param("xgyj").setAsString() = m_xgyj;
		g_dbcommand.Param("wyyj").setAsString() = m_hzyj;
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
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

void CXgyj::OnOK() 
{
	// TODO: Add extra validation here
	OnButtonSave();
	
	CXTResizeDialog::OnOK();
}

void CXgyj::OnButtonCyc() 
{
	if(CurEdit == 0)	return;

	if(NULL == pNormalWord)
	{
		pNormalWord=new CNormalWord(this);
		pNormalWord->Create();
		pNormalWord->SetActiveWindow();
		pNormalWord->ShowWindow(SW_SHOW);
	}
	else
	{
		pNormalWord->ShowWindow(!pNormalWord->IsWindowVisible());
	}

	if(pNormalWord->IsWindowVisible())
	{
		pNormalWord->AdjustPosition();
	}
}

void CXgyj::OnSetfocusEditXgyj() 
{
	if (pNormalWord != NULL && pNormalWord->IsWindowVisible() && CurEdit != 2)
	{
		CurEdit=2;	
		pNormalWord->AdjustPosition();
	}
	else
	{
		CurEdit=2;	
	}
}

void CXgyj::OnDestroy() 
{
	CXTResizeDialog::OnDestroy();
	
	if (pNormalWord!=NULL) 
	{
		pNormalWord->DestroyWindow();
		delete pNormalWord;
		pNormalWord = NULL;
	}	
}

void CXgyj::OnButtonCopycz() 
{
	// TODO: Add your control notification handler code here
	CString czyj;
	GetDlgItemText(IDC_EDIT_CZYJ,czyj);
	CString xgyj;
	GetDlgItemText(IDC_EDIT_XGYJ,xgyj);
	SetDlgItemText(IDC_EDIT_XGYJ,xgyj + "   " + czyj);
}

void CXgyj::OnButtonCopyzz() 
{
	// TODO: Add your control notification handler code here
	CString xgyj,hzyj;
	GetDlgItemText(IDC_EDIT_XGYJ,xgyj);
	GetDlgItemText(IDC_EDIT_HZYJ,hzyj);
	SetDlgItemText(IDC_EDIT_HZYJ,hzyj + "   " + xgyj);
}

void CXgyj::OnSetfocusEditCzyj() 
{
	// TODO: Add your control notification handler code here
	CurEdit = 1;
}

void CXgyj::OnSetfocusEditHzyj() 
{
	// TODO: Add your control notification handler code here
	CurEdit = 3;
}
