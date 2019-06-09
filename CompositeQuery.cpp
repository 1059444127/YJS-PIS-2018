// CompositeQuery.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "CompositeQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCompositeQuery dialog
extern CPathologyApp theApp;

CCompositeQuery::CCompositeQuery(CWnd* pParent /*=NULL*/)
	: CDialog(CCompositeQuery::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCompositeQuery)
	m_query = _T("");
	m_content = _T("");
	//}}AFX_DATA_INIT
	pListInput = NULL;
}


void CCompositeQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompositeQuery)
	DDX_Text(pDX, IDC_EDIT_QUERY, m_query);
	DDV_MaxChars(pDX, m_query, 2000);
	DDX_CBString(pDX, IDC_COMBO_CONTENT, m_content);
	DDV_MaxChars(pDX, m_content, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCompositeQuery, CDialog)
	//{{AFX_MSG_MAP(CCompositeQuery)
	ON_BN_CLICKED(IDC_BUTTON_OR, OnButtonOr)
	ON_BN_CLICKED(IDC_BUTTON_AND, OnButtonAnd)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD, OnSelchangeComboField)
	ON_BN_CLICKED(IDLISTVIEW, OnListview)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_LISTSELECT,ListSelect)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompositeQuery message handlers

BOOL CCompositeQuery::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	((CComboBox *)GetDlgItem(IDC_COMBO_OPERATOR))->AddString("=");
	((CComboBox *)GetDlgItem(IDC_COMBO_OPERATOR))->AddString(">");
	((CComboBox *)GetDlgItem(IDC_COMBO_OPERATOR))->AddString(">=");
	((CComboBox *)GetDlgItem(IDC_COMBO_OPERATOR))->AddString("<");
	((CComboBox *)GetDlgItem(IDC_COMBO_OPERATOR))->AddString("<=");
	((CComboBox *)GetDlgItem(IDC_COMBO_OPERATOR))->AddString("<>");
	((CComboBox *)GetDlgItem(IDC_COMBO_OPERATOR))->AddString("包含");
	((CComboBox *)GetDlgItem(IDC_COMBO_OPERATOR))->SetCurSel(0);

	for(int i = 0; i < m_DescriptionList.GetSize(); i++ )
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_FIELD))->AddString(m_DescriptionList.GetAt(i));
	}
    ((CComboBox *)GetDlgItem(IDC_COMBO_FIELD))->SetCurSel(0);
	
	//----------------------------------------------
	CStdioFile File;
	if ( File.Open(theApp.m_Exe_Working_Dir + "system\\ContentRecent.txt", CFile::modeRead | CFile::typeText) )
	{
		CString tmp;
		while(1)
		{
			if( File.ReadString(tmp) == FALSE ) break;
			tmp.TrimLeft();
			tmp.TrimRight();
			if( tmp.IsEmpty() )   continue;
            m_ContentList.Add(tmp);
		}
		File.Close();
	}

	for( i = 0; i < m_ContentList.GetSize(); i++ )
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_CONTENT))->AddString(m_ContentList.GetAt(i));
	}

	//----------------------------------------------

	UpdateData(FALSE);

	//------------------------------------------------
	int index = ((CComboBox *)GetDlgItem(IDC_COMBO_FIELD))->GetCurSel();

    pListInput = new CDlgListInput(this);

	bool rt = false;
	CString str = m_NameList.GetAt(index);
	str.MakeUpper();
	if( str.Find("MICD10") >=0 || str.Find("SICD10") >=0 )
	{
		if(str.Find("MICD10") >=0)
			pListInput->m_type = 1;
		else
			pListInput->m_type = 2;
		
		rt = true;
	}
	else
	{
		pListInput->m_type = 0;
	}

	pListInput->Create();
	pListInput->SetActiveWindow();
	pListInput->ShowWindow(SW_HIDE);

	if(m_InfoList.GetAt(index).GetLength() > 0)
	{
		char ch = m_InfoList.GetAt(index).GetAt(0);
		if(ch != '{')
		{
	        CStringArray   firstlist;
			CString str = m_InfoList.GetAt(index);
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
	//------------------------------------------------

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCompositeQuery::OnButtonOr() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	m_query += " or";
	
	char enter[3];
	enter[0] = 0x0d;	enter[1] = 0x0a;	enter[2] = '\0';
	m_query += enter;

	UpdateData(FALSE);
}

void CCompositeQuery::OnButtonAnd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	m_query += " and";
	
	char enter[3];
	enter[0] = 0x0d;	enter[1] = 0x0a;	enter[2] = '\0';
	m_query += enter;

	UpdateData(FALSE);
}

void CCompositeQuery::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	
	//--------------------------------------------
	for(int i = 0; i < m_ContentList.GetSize(); i++ )
	{
		if( m_ContentList.GetAt(i).Compare( m_content ) == 0 )  break;
	}
	if( i < m_ContentList.GetSize() )
	{
		m_ContentList.RemoveAt( i );
	}

	m_ContentList.InsertAt(0, m_content);

	while( m_ContentList.GetSize() > 20 )
	{
		m_ContentList.RemoveAt( 20 );
	}

	((CComboBox *)GetDlgItem(IDC_COMBO_CONTENT))->ResetContent();

	for( i = 0; i < m_ContentList.GetSize(); i++ )
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_CONTENT))->AddString(m_ContentList.GetAt(i));
	}
	//--------------------------------------------

	int index = ((CComboBox *)GetDlgItem(IDC_COMBO_FIELD))->GetCurSel();
	CString str;
	((CComboBox *)GetDlgItem(IDC_COMBO_OPERATOR))->GetWindowText(str);
	str.TrimLeft();  str.TrimRight();
	if(str.CompareNoCase("包含") == 0)
	{
		str.Format("like");
	}

	m_query += " ";
	m_query += m_NameList.GetAt(index);
	m_query += " ";
	m_query += str;
	m_query += " ";


	switch((SADataType_t)m_TypeList.GetAt(index)) {
	case SA_dtShort:
	case SA_dtLong:
	case SA_dtDouble:
		m_query += m_content;
		break;
	case SA_dtBool:
		if(m_content.CompareNoCase("是") == 0)
			m_query += "1";
		else
			m_query += "0";
		break;
	case SA_dtDateTime:
        if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			m_query += "to_date('";
			m_query += m_content;
			m_query += "','yyyy-mm-dd hh24:mi:ss')";
		}
		else
		{
			m_query += "'";
			m_query += m_content;
			m_query += "'";
		}
        break;
	case SA_dtString:
	case SA_dtLongChar:
	case SA_dtCLob:
		if(str.CompareNoCase("like") == 0)
		{
			m_query += "'%";
			m_query += m_content;
			m_query += "%'";
		}
		else
		{
			m_query += "'";
			if(m_content.IsEmpty())
			{
				m_content = " ";
			}
			m_query += m_content;
			m_query += "'";
		}
		break;
	}

	UpdateData(FALSE);
}

void CCompositeQuery::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	char enter[3];
	enter[0] = 0x0d;	enter[1] = 0x0a;	enter[2] = '\0';
	m_query += enter;
	int k;
	CString lstr,rstr;
	while(1)
	{
		if( (k = m_query.Find(enter)) == -1 )  break;
		lstr = m_query.Left(k);
		rstr = m_query.Right(m_query.GetLength() - k - 2);
		m_query = lstr + rstr;
	}

	m_query.TrimLeft();  m_query.TrimRight();
	
	UpdateData(FALSE);
	
	CDialog::OnOK();
}

void CCompositeQuery::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (pListInput != NULL)
	{
		pListInput->DestroyWindow();
		delete pListInput;
		pListInput = NULL;
	}

	m_NameList.RemoveAll();
	m_DescriptionList.RemoveAll();
	m_TypeList.RemoveAll();
	m_InfoList.RemoveAll();

	CStdioFile File;
	if ( File.Open(theApp.m_Exe_Working_Dir + "system\\ContentRecent.txt", CFile::modeWrite |CFile::modeCreate | CFile::typeText) )
	{
		for(int i = 0; i < m_ContentList.GetSize(); i++ )
		{
			File.WriteString(m_ContentList.GetAt(i));
			File.WriteString("\n");
		}
		File.Close();
	}

	m_ContentList.RemoveAll();
}

void CCompositeQuery::OnSelchangeComboField() 
{
	// TODO: Add your control notification handler code here
	int index = ((CComboBox *)GetDlgItem(IDC_COMBO_FIELD))->GetCurSel();
	switch((SADataType_t)m_TypeList.GetAt(index)) {
	case SA_dtShort:
	case SA_dtLong:
		((CComboBox *)GetDlgItem(IDC_COMBO_CONTENT))->SetWindowText("0");
		break;
	case SA_dtDouble:
		((CComboBox *)GetDlgItem(IDC_COMBO_CONTENT))->SetWindowText("0.0");
		break;
	case SA_dtBool:
		((CComboBox *)GetDlgItem(IDC_COMBO_CONTENT))->SetWindowText("是");
		break;
	case SA_dtDateTime:
		((CComboBox *)GetDlgItem(IDC_COMBO_CONTENT))->SetWindowText("1999-01-01 00:00:00");
        break;
	case SA_dtString:
	case SA_dtLongChar:
	case SA_dtCLob:
		((CComboBox *)GetDlgItem(IDC_COMBO_CONTENT))->SetWindowText("");
		break;
	}

	//------------------------------------------------
	bool rt = false;
	CString str = m_NameList.GetAt(index);
	str.MakeUpper();
	if( str.Find("MICD10") >=0 || str.Find("SICD10") >=0 )
	{
		if(str.Find("MICD10") >=0)
			pListInput->m_type = 1;
		else
			pListInput->m_type = 2;

		pListInput->m_listinfo.SetNumberRows(0);
		pListInput->m_listinfo.RedrawAll();
		
		rt = true;
	}
	else
	{
		pListInput->m_type = 0;
	}

	pListInput->SendMessage(WM_INITDIALOG);

	if(m_InfoList.GetAt(index).GetLength() > 0)
	{
		char ch = m_InfoList.GetAt(index).GetAt(0);
		if(ch != '{')
		{
	        CStringArray   firstlist;
			CString str = m_InfoList.GetAt(index);
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
		pListInput->m_listinfo.SetNumberRows(0);
		pListInput->m_listinfo.RedrawAll();

		//if(pListInput != NULL)	pListInput->ShowWindow(SW_HIDE);
		((CButton *)GetDlgItem(IDLISTVIEW))->ShowWindow(SW_HIDE);

		CRect t1,t2;
        ((CButton *)GetDlgItem(IDOK))->GetWindowRect(&t1);
		ScreenToClient(&t1);
		GetClientRect(&t2);
		((CButton *)GetDlgItem(IDOK))->SetWindowPos(NULL,(t2.Width() - 2*t1.Width() - 30)/2,t1.top,t1.Width(),t1.Height(),SWP_NOZORDER );
		((CButton *)GetDlgItem(IDCANCEL))->SetWindowPos(NULL,(t2.Width() - 2*t1.Width() - 30)/2 + t1.Width() + 30,t1.top,t1.Width(),t1.Height(),SWP_NOZORDER );
	}
	else
	{
		((CButton *)GetDlgItem(IDLISTVIEW))->ShowWindow(SW_SHOW);

		CRect t1,t2;
        ((CButton *)GetDlgItem(IDOK))->GetWindowRect(&t1);
		ScreenToClient(&t1);
		GetClientRect(&t2);
		((CButton *)GetDlgItem(IDLISTVIEW))->SetWindowPos(NULL,(t2.Width() - 3*t1.Width() - 60)/2,t1.top,t1.Width(),t1.Height(),SWP_NOZORDER );
		((CButton *)GetDlgItem(IDOK))->SetWindowPos(NULL,(t2.Width() - 3*t1.Width() - 60)/2+(t1.Width() + 30),t1.top,t1.Width(),t1.Height(),SWP_NOZORDER );
		((CButton *)GetDlgItem(IDCANCEL))->SetWindowPos(NULL,(t2.Width() - 3*t1.Width() - 60)/2 + 2*(t1.Width() + 30),t1.top,t1.Width(),t1.Height(),SWP_NOZORDER );
	}
	
	RedrawWindow();
	if(pListInput->IsWindowVisible())
		pListInput->RedrawWindow();
	//------------------------------------------------
}

void CCompositeQuery::OnListview() 
{
	// TODO: Add your control notification handler code here
	if(pListInput != NULL)
		pListInput->ShowWindow(SW_SHOW);
}

LRESULT CCompositeQuery::ListSelect(WPARAM wparam, LPARAM lparam)
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

	m_content = firstname;

	UpdateData(FALSE);
	return 0L;
}


void CCompositeQuery::OnButtonClear() 
{
	// TODO: Add your control notification handler code here
	m_query.Empty();
	UpdateData(FALSE);
}
