// TabDockWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "TabDockWnd.h"
#include "MainFrm.h"
#include "MedImage.h"
#include "MedSound.h"
#include "AdviceShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabDockWnd
extern CPathologyApp theApp;

CTabDockWnd::CTabDockWnd()
{
}

CTabDockWnd::~CTabDockWnd()
{
}

BEGIN_MESSAGE_MAP(CTabDockWnd, CMyTabCtrlBar)
	//{{AFX_MSG_MAP(CTabDockWnd)
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
	ON_MESSAGE(TCN_XT_SELCHANGE, OnTabSelChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabDockWnd message handlers

int CTabDockWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMyTabCtrlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// add the tree control to the tab docking bar.
	int iIndex = 0;

	if(theApp.m_StationType.CompareNoCase("�Ǽǹ���վ") == 0 || theApp.m_StationType.CompareNoCase("ȡ�Ĺ���վ") == 0 || theApp.m_StationType.CompareNoCase("��Ϲ���վ") == 0 || theApp.m_StationType.CompareNoCase("ϸ��ѧ����վ") == 0)
	{
		if (!m_DBList.Create(IDD_DIALOG_DBLIST,this))
		{
			TRACE0( "Failed to create Dialog.\n" );
			return -1;	
		}
		
		AddControl( _T( "�б�" ), &m_DBList, iIndex);
		iIndex++;
	}

	if(theApp.m_StationType.CompareNoCase("ȡ�Ĺ���վ") == 0 || theApp.m_StationType.CompareNoCase("��Ϲ���վ") == 0 || theApp.m_StationType.CompareNoCase("ϸ��ѧ����վ") == 0)
	{
		AddView(_T( "ͼ��" ),  RUNTIME_CLASS(CMedImage), NULL, NULL, iIndex);
		iIndex++;
		
		if(theApp.m_LoadSoundTabMode.CompareNoCase("��") == 0)
		{
			AddView(_T( "����" ),  RUNTIME_CLASS(CMedSound), NULL, NULL, iIndex);
			iIndex++;
		}
	}


	if(theApp.m_StationType.CompareNoCase("��Ƭ����վ")     == 0 || 
	   theApp.m_StationType.CompareNoCase("������Ƭ����վ") == 0 || 
	   theApp.m_StationType.CompareNoCase("�ؼ칤��վ")     == 0)
	{
		AddView(_T( "ҽ��" ),  RUNTIME_CLASS(CAdviceShow), NULL, NULL, iIndex);
		iIndex++;
	}

	// Associate the image list with the tab control bar.
	SetActiveView(0);
	return 0;
}

void CTabDockWnd::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CMyTabCtrlBar::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	if((lpwndpos->flags & SWP_HIDEWINDOW))
	{
	}
}

LRESULT CTabDockWnd::OnTabSelChange(WPARAM wParam, LPARAM lParam)
{
	CWnd *pWnd = GetActiveView();
	if(pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CDBList)))
	{
		((CEdit *)pWnd->GetDlgItem(IDC_EDIT_BLH))->SetFocus();
	} 
	else if(pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CMedImage)))
	{
		CWnd* p = GetActiveView();
		//((CMedImage*)p)->m_PrinterSelMode = TRUE;
		((CMedImage*)p)->setDefault();
	//	((CMedImage*)p)->m_btPrinter.SetBitmap(CSize(32,32),IDB_BITMAP_VIDEO);
	//	((CMedImage*)p)->m_ToolTip.AddTool(&m_btPrinter, "ͼ�����");
	}

	
  
	return CMyTabCtrlBar::OnTabSelChange(wParam, lParam);
}
