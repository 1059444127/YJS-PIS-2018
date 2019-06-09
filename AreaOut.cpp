// AreaOut.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "AreaOut.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAreaOut dialog


CAreaOut::CAreaOut(CWnd* pParent /*=NULL*/)
	: CDialog(CAreaOut::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAreaOut)
	m_from = _T("");
	m_to = _T("");
	//}}AFX_DATA_INIT
	pListInput = NULL;
	m_focus_type = 0;
	m_FieldInfo.Empty();
	m_FieldName.Empty();
}


void CAreaOut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAreaOut)
	DDX_Text(pDX, IDC_EDIT_FROM, m_from);
	DDV_MaxChars(pDX, m_from, 1000);
	DDX_Text(pDX, IDC_EDIT_TO, m_to);
	DDV_MaxChars(pDX, m_to, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAreaOut, CDialog)
	//{{AFX_MSG_MAP(CAreaOut)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDLISTVIEW, OnListview)
	ON_EN_SETFOCUS(IDC_EDIT_FROM, OnSetfocusEditFrom)
	ON_EN_SETFOCUS(IDC_EDIT_TO, OnSetfocusEditTo)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_LISTSELECT,ListSelect)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAreaOut message handlers

BOOL CAreaOut::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetWindowText(m_title);

    pListInput = new CDlgListInput(this);

	bool rt = false;
	CString str = m_FieldName;
	str.MakeUpper();
	if( str.Find("MICD10") >=0 || str.Find("SICD10") >=0 )
	{
		if(str.Find("MICD10") >=0)
			pListInput->m_type = 1;
		else
			pListInput->m_type = 2;
		
		rt = true;
	}

	pListInput->Create();
	pListInput->SetActiveWindow();
	pListInput->ShowWindow(SW_HIDE);

	if(m_FieldInfo.GetLength() > 0)
	{
		char ch = m_FieldInfo.GetAt(0);
		if(ch != '{')
		{
	        CStringArray   firstlist;
			CString str = m_FieldInfo;
			while(1)
			{
				if(str.Find('&') == -1)  break;
				CString tmp = str.Left(str.Find('&'));
				firstlist.Add(tmp);
				str = str.Right(str.GetLength() - str.Find('&') - 1);
			}
			pListInput->m_listinfo.SetNumberRows(firstlist.GetSize());
			for(int i = 0; i < firstlist.GetSize(); i++ )
			{
				pListInput->m_listinfo.QuickSetText(0,i,firstlist.GetAt(i));
			}
			firstlist.RemoveAll();
			pListInput->m_listinfo.RedrawAll();
			
			rt = true;
		}
	}

	if(!rt)
	{
		((CButton *)GetDlgItem(IDLISTVIEW))->ShowWindow(SW_HIDE);

		CRect t1,t2;
        ((CButton *)GetDlgItem(IDOK))->GetWindowRect(&t1);
		ScreenToClient(&t1);
		GetClientRect(&t2);
		((CButton *)GetDlgItem(IDOK))->SetWindowPos(NULL,(t2.Width() - 2*t1.Width() - 30)/2,t1.top,t1.Width(),t1.Height(),SWP_NOZORDER );
		((CButton *)GetDlgItem(IDCANCEL))->SetWindowPos(NULL,(t2.Width() - 2*t1.Width() - 30)/2 + t1.Width() + 30,t1.top,t1.Width(),t1.Height(),SWP_NOZORDER );
	}

	if( m_type == 1000 )
	{
		CRect ct1;
		((CStatic *)GetDlgItem(IDC_STATIC_FROM))->GetWindowRect(&ct1);
		ScreenToClient(&ct1);

		CRect ct2;
		((CEdit *)GetDlgItem(IDC_EDIT_FROM))->GetWindowRect(&ct2);
		ScreenToClient(&ct2);

		((CEdit *)GetDlgItem(IDC_EDIT_FROM))->SetWindowPos(NULL,ct1.left,ct2.top,ct2.right - ct1.left,ct2.Height(),SWP_NOZORDER);

		((CButton *)GetDlgItem(IDOK))->GetWindowRect(&ct1);
		ScreenToClient(&ct1);
		((CButton *)GetDlgItem(IDOK))->SetWindowPos(NULL,ct1.left, ct1.top - ct2.Height(),ct1.Width(),ct1.Height(),SWP_NOZORDER);

		((CButton *)GetDlgItem(IDCANCEL))->GetWindowRect(&ct1);
		ScreenToClient(&ct1);
		((CButton *)GetDlgItem(IDCANCEL))->SetWindowPos(NULL,ct1.left, ct1.top - ct2.Height(),ct1.Width(),ct1.Height(),SWP_NOZORDER);

		((CButton *)GetDlgItem(IDLISTVIEW))->GetWindowRect(&ct1);
		ScreenToClient(&ct1);
		((CButton *)GetDlgItem(IDLISTVIEW))->SetWindowPos(NULL,ct1.left, ct1.top - ct2.Height(),ct1.Width(),ct1.Height(),SWP_NOZORDER);

		((CStatic *)GetDlgItem(IDC_STATIC_TO))->ShowWindow(SW_HIDE);
		((CEdit *)GetDlgItem(IDC_EDIT_TO))->ShowWindow(SW_HIDE);
		((CStatic *)GetDlgItem(IDC_STATIC_FROM))->ShowWindow(SW_HIDE);

		GetWindowRect(&ct1);	                       
		ScreenToClient(&ct1);
		SetWindowPos(NULL, ct1.left,ct1.top,ct1.Width(),ct1.Height() - ct2.Height(),SWP_NOZORDER);
		CenterWindow();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAreaOut::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if( m_type == 1000 )
	{
		m_to = m_from;
		UpdateData(FALSE);
	}
	
	CDialog::OnOK();
}

void CAreaOut::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (pListInput != NULL)
	{
		pListInput->DestroyWindow();
		delete pListInput;
		pListInput = NULL;
	}
}

void CAreaOut::OnListview() 
{
	// TODO: Add your control notification handler code here
	if(pListInput != NULL && ((CButton *)GetDlgItem(IDLISTVIEW))->IsWindowVisible())
		pListInput->ShowWindow(SW_SHOW);
}

LRESULT CAreaOut::ListSelect(WPARAM wparam, LPARAM lparam)
{
	int index = pListInput->m_listinfo.GetCurrentRow();
	if( index < 0 || index >=pListInput->m_listinfo.GetNumberRows() )  return 0L;
	CString firstname;
	CUGCell  cell;
	pListInput->m_listinfo.GetCellIndirect(0 , index, &cell);
	cell.GetText(&firstname);

	if( pListInput->m_type )
	{
		firstname = firstname.Left(firstname.Find('['));
	}

	switch(m_focus_type) {
	case 1:
		m_from = firstname;
		break;
	case 2:
		m_to   = firstname;
		break;
	}

	UpdateData(FALSE);
	return 0L;
}

void CAreaOut::OnSetfocusEditFrom() 
{
	// TODO: Add your control notification handler code here
	m_focus_type = 1;
}

void CAreaOut::OnSetfocusEditTo() 
{
	// TODO: Add your control notification handler code here
	m_focus_type = 2;
}

BOOL CAreaOut::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
    if (pMsg->wParam == VK_RETURN)
    {
		// don't allow dialog to process return or escape keys.
		OnOK();
        return TRUE;
    }
	
	return CDialog::PreTranslateMessage(pMsg);
}
