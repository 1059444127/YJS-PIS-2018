// StrEdit.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "StrEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// StrEdit dialog

StrEdit::StrEdit(CWnd* pParent /*=NULL*/)
	: CDialog(StrEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(StrEdit)
	m_EditStr = "";
	//}}AFX_DATA_INIT
	m_FieldInfo.Empty();
	m_FieldName.Empty();
	pListInput = NULL;
	m_type = 0;
}

StrEdit::StrEdit(CString Title,CWnd* pParent /*=NULL*/)
	: CDialog(StrEdit::IDD, pParent)
{
	m_Title=Title; 
	m_EditStr = "";
	m_FieldInfo.Empty();
	m_FieldName.Empty();
	pListInput = NULL;
	m_type = 0;
}

void StrEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(StrEdit)
	DDX_Text(pDX, IDC_EDIT1, m_EditStr);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(StrEdit, CDialog)
	//{{AFX_MSG_MAP(StrEdit)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDLISTVIEW, OnListview)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_LISTSELECT,ListSelect)
END_MESSAGE_MAP()

BOOL StrEdit::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_Title.GetLength()>0) SetWindowText(m_Title);

	if(m_Title.Find("ÃÜÂë") >= 0)
	{
		((CEdit *)GetDlgItem(IDC_EDIT1))->SetPasswordChar('*');
	}

	bool rt = false;

	if( m_type )
	{
		pListInput = new CDlgListInput(this);
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
	}

	if(m_Title.CompareNoCase("ÇëÊäÈëËæ»úÃÜÂë") != 0 && rt == false)
	{
		((CButton *)GetDlgItem(IDLISTVIEW))->ShowWindow(SW_HIDE);

		CRect t1,t2;
        ((CButton *)GetDlgItem(IDOK))->GetWindowRect(&t1);
		ScreenToClient(&t1);
		GetClientRect(&t2);
		((CButton *)GetDlgItem(IDOK))->SetWindowPos(NULL,(t2.Width() - 2*t1.Width() - 30)/2,t1.top,t1.Width(),t1.Height(),SWP_NOZORDER );
		((CButton *)GetDlgItem(IDCANCEL))->SetWindowPos(NULL,(t2.Width() - 2*t1.Width() - 30)/2 + t1.Width() + 30,t1.top,t1.Width(),t1.Height(),SWP_NOZORDER );
	}

	if(m_Title.CompareNoCase("ÇëÊäÈëËæ»úÃÜÂë") == 0)
	{
		SetDlgItemText(IDLISTVIEW, "Õ³Ìù");
		if(::IsClipboardFormatAvailable(CF_TEXT))
		{
			((CButton *)GetDlgItem(IDLISTVIEW))->EnableWindow(TRUE);
		}
		else
		{
			((CButton *)GetDlgItem(IDLISTVIEW))->EnableWindow(FALSE);
		}
	}

	CenterWindow();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

/////////////////////////////////////////////////////////////////////////////
// StrEdit message handlers

void StrEdit::OnDestroy() 
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

LRESULT StrEdit::ListSelect(WPARAM wparam, LPARAM lparam)
{
	if (pListInput == NULL)  return 0L;
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

	m_EditStr = firstname;

	UpdateData(FALSE);
	return 0L;
}

void StrEdit::OnListview() 
{
	if(m_Title.CompareNoCase("ÇëÊäÈëËæ»úÃÜÂë") == 0)
	{
		((CEdit *)GetDlgItem(IDC_EDIT1))->Paste();
		return;
	}

	if(pListInput != NULL)
		pListInput->ShowWindow(SW_SHOW);
}
