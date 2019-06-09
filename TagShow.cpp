// TagShow.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "TagShow.h"
#include "Tjyz.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTagShow dialog
extern CPathologyApp theApp;

CTagShow::CTagShow(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CTagShow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTagShow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_UsageType = 0;
	m_TagString.Empty();
}


void CTagShow::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTagShow)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTagShow, CXTResizeDialog)
	//{{AFX_MSG_MAP(CTagShow)
	ON_BN_CLICKED(IDC_BUTTON_ADDBJW, OnButtonAddbjw)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_ADDTC, OnButtonAddtc)
	ON_CBN_SELCHANGE(IDC_COMBO_TC, OnSelchangeComboTc)
	ON_BN_CLICKED(IDC_BUTTON_DELETETC, OnButtonDeletetc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTagShow message handlers

BOOL CTagShow::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_TagList.AttachGrid(this,IDC_STATIC_TAGLIST);
	m_TagList.SetParent(this);
	m_TagList.ResetGridSize();
	m_TagList.RedrawAll();

	SetResize(IDC_STATIC_TAGLIST, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC_TJXM, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_ADDTC, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_COMBO_TC, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_DELETETC, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_EDIT_BJW, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_ADDBJW, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_DELETE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	
	SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	RefreshTcList();
	
	if(m_UsageType)
	{
		GetDlgItem(IDC_BUTTON_ADDBJW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_BJW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_DELETE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ADDTC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_DELETETC)->ShowWindow(SW_HIDE);

		CRect nParentRect;
		((CTjyz *)GetParent())->GetWindowRect(&nParentRect);

		CRect nItemRect;
		((CTjyz *)GetParent())->GetDlgItem(IDC_STATIC_JSYZ)->GetWindowRect(&nItemRect);

		SetWindowPos(NULL, nParentRect.left, nParentRect.top, nParentRect.Width(), (nItemRect.bottom + 5 - nParentRect.top), SWP_NOZORDER);
	}
	else
	{
		GetDlgItem(IDC_STATIC_TJXM)->ShowWindow(SW_HIDE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTagShow::RefreshTcList()
{
	((CComboBox *)GetDlgItem(IDC_COMBO_TC))->ResetContent();

	theApp.ConnectDatabase();
	try
	{
		SAString cmdstr = "Select * from " + theApp.TABLE_TC + " order by name asc";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Execute();
		
		while( g_dbcommand.FetchNext() )
		{
			((CComboBox *)GetDlgItem(IDC_COMBO_TC))->AddString(g_dbcommand.Field("name").asString());
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
}

void CTagShow::OnButtonAddbjw() 
{
	// TODO: Add your control notification handler code here
	CString tagname;
	GetDlgItemText(IDC_EDIT_BJW, tagname);
	tagname.TrimLeft(); tagname.TrimRight();

	if(tagname.IsEmpty())  return;

	theApp.ConnectDatabase();

	int ItemCount = 0;
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_TAG + " where tagname=:tagname";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("tagname").setAsString() = tagname;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			ItemCount = g_dbcommand.Field(1).asLong();
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

	if(ItemCount > 0)
	{
		AfxMessageBox("该标记物已经存在!");
		return;
	}

	try
	{
		SAString cmdstr = "Insert into " + theApp.TABLE_TAG + "(tagname) values(:tagname)";
		
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("tagname").setAsString() = tagname;
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

	m_TagList.ResetGridSize();
	m_TagList.RedrawAll();
}

void CTagShow::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	theApp.ConnectDatabase();

	for(int i = 0; i < m_TagList.GetNumberCols(); i++)
	{
		for(int j = 0; j < m_TagList.GetNumberRows(); j++)
		{
			CString str;
			CUGCell  cell;
			m_TagList.GetCellIndirect(i , j, &cell);
			cell.GetText(&str);
			int	k = atoi(str);
           
			if( k )
			{
				str = cell.GetLabelText();

				try
				{
					SAString cmdstr = "Delete from " + theApp.TABLE_TAG + " where tagname=:tagname";
					g_dbcommand.setCommandText( cmdstr );
					g_dbcommand.Param("tagname").setAsString() = str;
					
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
		}
	}

	m_TagList.ResetGridSize();
	m_TagList.RedrawAll();
}

void CTagShow::OnButtonAddtc() 
{
	// TODO: Add your control notification handler code here
	CString taglist; taglist.Empty();
	for(int i = 0; i < m_TagList.GetNumberCols(); i++)
	{
		for(int j = 0; j < m_TagList.GetNumberRows(); j++)
		{
			CString str;
			CUGCell  cell;
			m_TagList.GetCellIndirect(i , j, &cell);
			cell.GetText(&str);
			int	k = atoi(str);
            str = cell.GetLabelText();
			str.TrimLeft(); str.TrimRight();
			if( k && !str.IsEmpty())
			{	
				taglist += str;
				taglist += ',';
			}
		}
	}

	CString name;
	GetDlgItemText(IDC_COMBO_TC, name);
	name.TrimLeft();  name.TrimRight();
	if(name.IsEmpty())  return;

	int ItemCount = 0;
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_TC + " where name=:name";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("name").setAsString() = name;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			ItemCount = g_dbcommand.Field(1).asLong();
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

	if(ItemCount > 0 && taglist.IsEmpty())
	{
		try
		{
			SAString cmdstr = "Delete from " + theApp.TABLE_TC + " where name=:name";
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("name").setAsString() = name;
			
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
        RefreshTcList();
		return;
	}

	SAString cmdstr;
	if(ItemCount > 0)
	{
		cmdstr = "Update " + theApp.TABLE_TC + " set taglist=:taglist where name = :name";
	}
	else
	{
		cmdstr = "Insert into " + theApp.TABLE_TC + "(name, taglist) values(:name,:taglist)";
	}

	try
	{
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("name").setAsString() = name;
		g_dbcommand.Param("taglist").setAsString() = taglist;
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

	m_TagList.ResetGridSize();
	m_TagList.RedrawAll();

	RefreshTcList();
}

void CTagShow::OnSelchangeComboTc() 
{
	// TODO: Add your control notification handler code here
	CString name;
	((CComboBox *)GetDlgItem(IDC_COMBO_TC))->GetLBText(((CComboBox *)GetDlgItem(IDC_COMBO_TC))->GetCurSel(), name);

	try
	{
		SAString cmdstr = "Select * from " + theApp.TABLE_TC + " where name=:name";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("name").setAsString() = name;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			CStringArray taglist; 
			CString str = g_dbcommand.Field("taglist").asString();
			str.TrimLeft();  str.TrimRight();
			while(!str.IsEmpty())
			{
				taglist.Add(str.Left(str.Find(',')));
				str = str.Right(str.GetLength() - str.Find(',') - 1);
				str.TrimLeft();  str.TrimRight();
			}

			int i,j,k;
			for(i = 0; i < m_TagList.GetNumberCols(); i++)
			{
				for(j = 0; j < m_TagList.GetBottomRow(); j++)
				{
					CUGCell  cell;
					m_TagList.GetCellIndirect(i , j, &cell);
					str = cell.GetLabelText();
					for(k = 0; k < taglist.GetSize(); k++)
					{
						if(taglist.GetAt(k).CompareNoCase(str) == 0) break;
					}
					if(k < taglist.GetSize())
						m_TagList.QuickSetText(i, j, "1");
					else
						m_TagList.QuickSetText(i, j, "0");
				}
			}

			taglist.RemoveAll();
			m_TagList.RedrawAll();
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
}

void CTagShow::OnOK() 
{
	// TODO: Add extra validation here
	m_TagString.Empty();
	for(int i = 0; i < m_TagList.GetNumberCols(); i++)
	{
		for(int j = 0; j < m_TagList.GetNumberRows(); j++)
		{
			CString str;
			CUGCell  cell;
			m_TagList.GetCellIndirect(i , j, &cell);
			cell.GetText(&str);
			int	k = atoi(str);
            str = cell.GetLabelText();
			str.TrimLeft(); str.TrimRight();
			if( k && !str.IsEmpty())
			{	
				m_TagString += str;
				m_TagString += ',';
			}
		}
	}
	
	CXTResizeDialog::OnOK();
}

void CTagShow::OnButtonDeletetc() 
{
	CString name;
	GetDlgItemText(IDC_COMBO_TC, name);
	name.TrimLeft();  name.TrimRight();
	if(name.IsEmpty())  return;

	try
	{
		SAString cmdstr = "Delete from " + theApp.TABLE_TC + " where name=:name";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("name").setAsString() = name;
		
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
	RefreshTcList();
}
