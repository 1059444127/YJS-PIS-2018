// LogoShow.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "LogoShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogoShow dialog
extern CPathologyApp theApp;

CLogoShow::CLogoShow(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CLogoShow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogoShow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLogoShow::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogoShow)
	DDX_Control(pDX, IDC_HYP_NOTE, m_HypLink_Note);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogoShow, CXTResizeDialog)
	//{{AFX_MSG_MAP(CLogoShow)
	ON_BN_CLICKED(IDDELETE, OnDelete)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnButtonQuery)
	ON_BN_CLICKED(IDC_CHECK_SELECTALL, OnCheckSelectall)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogoShow message handlers

BOOL CLogoShow::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();

	CRect rt;
	GetDlgItem(IDC_DATE_START)->GetWindowRect(&rt);
	ScreenToClient(&rt);
	if (!m_Time_Start.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT, //,DTS_LONGDATEFORMAT
		rt, this, 111))
	{
		return FALSE;
	}
	
	m_Time_Start.SetTime(theApp.GetOleServerDateTime());

	GetDlgItem(IDC_DATE_END)->GetWindowRect(&rt);
	ScreenToClient(&rt);
	if (!m_Time_End.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT, //,DTS_LONGDATEFORMAT
		rt, this, 112))
	{
		return FALSE;
	}
	
	m_Time_End.SetTime(theApp.GetOleServerDateTime());

	m_grid.AttachGrid(this,IDC_STATIC_RECORDLIST);
	m_grid.SetParent(this);
	m_grid.ResetGridSize();
	m_grid.RedrawAll();

	SetResize(IDC_STATIC_BLH, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_BLH, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_START, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_DATE_START, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_END, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_DATE_END, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_QUERY, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_CHECK_SELECTALL, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC_RECORDLIST, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetResize(IDDELETE, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	
	SetResize(IDC_HYP_NOTE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	
	m_HypLink_Note.SetURL("说明:请在\"修改前内容\"栏双击鼠标左键可显示全部内容");
	m_HypLink_Note.SetColors(RGB(0x00, 0x00, 0xFF), RGB(0x00, 0x00, 0xFF), RGB(0x00, 0x00, 0xFF));
	m_HypLink_Note.SetUnderline(true);
	m_HypLink_Note.EnableShellExecute(false);

	if(!RIGHT_SCXTRZ(theApp.m_Cpp_Right))
	{
		((CButton *)GetDlgItem(IDDELETE))->EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLogoShow::OnDelete() 
{
	CUIntArray  rowlist;
	int nCol, nRet;
	long nRow;

	nRet = m_grid.EnumFirstSelected( &nCol, &nRow );
	while( nRet == UG_SUCCESS )
	{
		if(nRow >= 0)
		{
			int rowcount = rowlist.GetSize();
			for(int i = 0; i < rowcount; i ++ )
			{
				if( nRow == rowlist.GetAt(i) )  break;
			}
			if( i >= rowcount )
			{
				rowlist.Add(nRow);
			}
		}
		
		nRet = m_grid.EnumNextSelected( &nCol, &nRow );
	}

	int curnum = m_grid.GetCurrentRow();
	for(int i = 0; i < rowlist.GetSize(); i++)
	{
		m_grid.GotoRow(rowlist.GetAt(i));
	}
	m_grid.GotoRow(curnum);

	if( rowlist.GetSize() <= 0 )
	{
		return;
	}
	
	CStringArray optypelist, blhlist, personlist;
	CArray<SADateTime,SADateTime> optimelist;
	optypelist.RemoveAll();
	blhlist.RemoveAll();
	personlist.RemoveAll();
	optimelist.RemoveAll();

	for(i = 0; i < rowlist.GetSize(); i++)
	{
		CString str;
		CUGCell  cell;
		m_grid.GetCellIndirect(0,rowlist.GetAt(i),&cell);
		cell.GetText(&str);
		optypelist.Add(str);
		
		m_grid.GetCellIndirect(1,rowlist.GetAt(i),&cell);
		cell.GetText(&str);
		blhlist.Add(str);

		m_grid.GetCellIndirect(3,rowlist.GetAt(i),&cell);
		cell.GetText(&str);
		int year = atoi(str.Left(str.Find("-")));
		str = str.Right(str.GetLength() - str.Find("-") - 1);
		
		int month = atoi(str.Left(str.Find("-")));
		str = str.Right(str.GetLength() - str.Find("-") - 1);
		
		str.TrimLeft();
		str += " ";
		int day = atoi(str.Left(str.Find(" ")));
		str = str.Right(str.GetLength() - str.Find(" ") - 1);
		
		int hour = atoi(str.Left(str.Find(":")));
		str = str.Right(str.GetLength() - str.Find(":") - 1);
		
		int minute = atoi(str.Left(str.Find(":")));
		str = str.Right(str.GetLength() - str.Find(":") - 1);
		
		int second = atoi( str );
		
		SADateTime dtValue(year,month,day,hour,minute,second);
		optimelist.Add(dtValue);
		
		m_grid.GetCellIndirect(4,rowlist.GetAt(i),&cell);
		cell.GetText(&str);
		personlist.Add(str);
	}

	for(i = 0; i < blhlist.GetSize(); i++)
	{
		try
		{
			SAString cmdstr = "Delete from " + theApp.TABLE_LOGO + " where optype=:optype and blh=:blh and optime>=:optime1 and optime<=:optime2 and person=:person";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("optype").setAsString() = optypelist.GetAt(i);
			g_dbcommand.Param("blh").setAsString() = blhlist.GetAt(i);
			g_dbcommand.Param("person").setAsString() = personlist.GetAt(i);
			SADateTime start(optimelist.GetAt(i).GetYear(),optimelist.GetAt(i).GetMonth(),optimelist.GetAt(i).GetDay(),optimelist.GetAt(i).GetHour(),optimelist.GetAt(i).GetMinute(),optimelist.GetAt(i).GetSecond());
			COleDateTimeSpan tspan;
			tspan.SetDateTimeSpan(0, 0, 0, 1);
			SADateTime end = start + tspan;
			g_dbcommand.Param("optime1").setAsDateTime() = start;
			g_dbcommand.Param("optime2").setAsDateTime() = end;
			
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
			return;
		}
	}

	optypelist.RemoveAll();
	blhlist.RemoveAll();
	personlist.RemoveAll();
	optimelist.RemoveAll();

	m_grid.ResetGridSize();
	m_grid.RedrawAll();
	m_grid.RedrawAll();
}

void CLogoShow::OnButtonQuery() 
{
	CString mno; mno.Empty();
	GetDlgItemText(IDC_EDIT_BLH, mno);
	mno.TrimLeft(); mno.TrimRight();
	SetDlgItemText(IDC_EDIT_BLH, "");

	if(mno.IsEmpty())
	{
		//  取材日期查询
		COleDateTime m_time;
		m_Time_Start.GetTime(m_time);
		CString start = m_time.Format("%Y-%m-%d 00:00:00");

		m_Time_End.GetTime(m_time);
		CString end   = m_time.Format("%Y-%m-%d 23:59:59");

		m_grid.m_WhereBetween_Cmd.Format(" where optime >= '%s' and optime <= '%s'", start, end);
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			m_grid.m_WhereBetween_Cmd.Format(" where optime >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and optime <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", start, end);
		}
	}
	else
	{
		//  病理号查询
		m_grid.m_WhereBetween_Cmd.Format(" where blh = '%s'", mno);
	}

	m_grid.ResetGridSize();
	m_grid.RedrawAll();
}

void CLogoShow::OnCheckSelectall() 
{
	if(((CButton *)GetDlgItem(IDC_CHECK_SELECTALL))->GetState() & 0x0003)
	{
		// Select all
		m_grid.SelectRange(0, 0, m_grid.GetNumberCols()-1,m_grid.GetNumberRows()-1);
	}
	else
	{
		// Clear all selection
		m_grid.ClearSelections();
	}
}

void CLogoShow::OnSize(UINT nType, int cx, int cy) 
{
	CXTResizeDialog::OnSize(nType, cx, cy);
	
	if(m_Time_Start.GetSafeHwnd() != NULL)
	{
		CRect rt;
		GetDlgItem(IDC_DATE_START)->GetWindowRect(&rt);
		ScreenToClient(&rt);
		m_Time_Start.SetWindowPos(NULL, rt.left, rt.top,rt.Width(),rt.Height(),SWP_NOZORDER);
	}

	if(m_Time_End.GetSafeHwnd() != NULL)
	{
		CRect rt;
		GetDlgItem(IDC_DATE_END)->GetWindowRect(&rt);
		ScreenToClient(&rt);
		m_Time_End.SetWindowPos(NULL, rt.left, rt.top,rt.Width(),rt.Height(),SWP_NOZORDER);
	}
}
