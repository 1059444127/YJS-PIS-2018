// Zpqk.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "Zpqk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZpqk dialog
extern CPathologyApp theApp;


CZpqk::CZpqk(CWnd* pParent /*=NULL*/)
	: CPrintPreviewDialog(CZpqk::IDD, pParent)
{
	//{{AFX_DATA_INIT(CZpqk)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_STATISTIC);
}


void CZpqk::DoDataExchange(CDataExchange* pDX)
{
	CPrintPreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZpqk)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CZpqk, CPrintPreviewDialog)
	//{{AFX_MSG_MAP(CZpqk)
	ON_COMMAND(ID_LOC_PRINT, OnLocPrint)
	ON_COMMAND(ID_LOC_PRINT_PREVIEW, OnLocPrintPreview)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE( TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
    ON_NOTIFY_EX_RANGE( TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	ON_CBN_SELCHANGE(ID_YEAR, OnSelchangeComboYear)
	ON_CBN_SELCHANGE(ID_TIME, OnSelchangeComboTime)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZpqk message handlers

BOOL CZpqk::OnInitDialog() 
{
	CPrintPreviewDialog::OnInitDialog();
	
	m_UsageType = 3;
	// TODO: Add extra initialization here
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),AFX_IDW_TOOLBAR+25)||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_TRANSTABLE))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;      // fail to create
	}

	if (!m_wndYear.Create( WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_CLIPCHILDREN,
		CRect(0,0,80,550), &m_wndToolBar, ID_YEAR ))
	{
		TRACE0("Failed to create combo box.\n");
		return -1;      // fail to create
	}
	if (!m_wndTime.Create( WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_CLIPCHILDREN,
		CRect(0,0,80,550), &m_wndToolBar, ID_TIME ))
	{
		TRACE0("Failed to create combo box.\n");
		return -1;      // fail to create
	}
	m_wndToolBar.InsertControl(&m_wndYear);
	m_wndToolBar.InsertControl(&m_wndTime);

	CRect rt;
	GetDlgItem(IDC_DATE_START)->GetWindowRect(&rt);
	ScreenToClient(&rt);
	if (!m_Time_Start.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT, //,DTS_LONGDATEFORMAT
		rt, this, 111))
	{
		return FALSE;
	}
	
	m_Time_Start.SetFormat(_T("yyyy-MM-dd HH':'mm"));
	m_Time_Start.SetTime(theApp.GetOleServerDateTime());

	GetDlgItem(IDC_DATE_END)->GetWindowRect(&rt);
	ScreenToClient(&rt);
	if (!m_Time_End.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT, //,DTS_LONGDATEFORMAT
		rt, this, 112))
	{
		return FALSE;
	}
	
	m_Time_End.SetFormat(_T("yyyy-MM-dd HH':'mm"));
	m_Time_End.SetTime(theApp.GetOleServerDateTime());

	// Move control bars to the correct position.
    RecalcBarLayout();

	COleDateTime m_time;
	m_Time_Start.GetTime(m_time);
	int index = 0;
	for(int i = 1980; i < 2020; i++)
	{
		CString str;
		str.Format("%d",i);
		m_wndYear.AddString(str);
		
		if(i == m_time.GetYear())
		{
			index = i - 1980;
		}
	}
	m_wndYear.SetCurSel(index);

	m_wndTime.AddString("全年");
	m_wndTime.AddString("一月份");
	m_wndTime.AddString("二月份");
	m_wndTime.AddString("三月份");
	m_wndTime.AddString("四月份");
	m_wndTime.AddString("五月份");
	m_wndTime.AddString("六月份");
	m_wndTime.AddString("七月份");
	m_wndTime.AddString("八月份");
	m_wndTime.AddString("九月份");
	m_wndTime.AddString("十月份");
	m_wndTime.AddString("十一月份");
	m_wndTime.AddString("十二月份");
	m_wndTime.AddString("一季度");
	m_wndTime.AddString("二季度");
	m_wndTime.AddString("三季度");
	m_wndTime.AddString("四季度");
	m_wndTime.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HCURSOR CZpqk::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CZpqk::OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult )
{
    ASSERT ( pNMHDR->code == TTN_NEEDTEXTA || TTN_NEEDTEXTW );

    TOOLTIPTEXTA* pTTTA = ( TOOLTIPTEXTA *)pNMHDR;
    TOOLTIPTEXTW* pTTTW = ( TOOLTIPTEXTW *)pNMHDR;

    CString strTipText;
    CString strMessage;

    if( GetToolText(pNMHDR->idFrom, strTipText, strMessage))
    {
#ifndef _UNICODE
        if(pNMHDR->code == TTN_NEEDTEXTA)
            lstrcpyn(pTTTA->szText,strTipText,_countof(pTTTA->szText));
        else
            _mbstowcsz(pTTTW->szText,strTipText,_countof(pTTTW->szText));
#else
        if(pNMHDR->code == TTN_NEEDTEXTA)
            _wcstombsz(pTTTA->szText,strTipText,_countof(pTTTA->szText));
        else
            lstrcpyn(pTTTW->szText,strTipText,_countof(pTTTW->szText));

#endif


     // SetMessageText(strMessage); // change status bar message here needs a pointer to CMainFrame
      
		// ok we handled the message, the buck stops here
		return TRUE;

    }

	// we did not handle the message, pass it on
    return CZpqk::OnToolTipText(nID,pNMHDR,pResult);
}

BOOL CZpqk::GetToolText( UINT nID, CString& strTipText, CString& /*strMessage*/)
{
    switch( nID )
    {
		case ID_LOC_PRINT_PREVIEW:
			strTipText.Format("制片情况统计表打印预览");
			break;
		case ID_LOC_PRINT:
			strTipText.Format("打印制片情况统计表");
			break;
    }
       
    return TRUE;
}

void CZpqk::GenList() 
{
}

void CZpqk::OnSelchangeComboYear() 
{
	ChangeTimeStr();
}

void CZpqk::OnSelchangeComboTime() 
{
	ChangeTimeStr();
}

void CZpqk::ChangeTimeStr()
{
	CString year;
	m_wndYear.GetWindowText(year);
	year.TrimLeft();  year.TrimRight();

	COleDateTime start, end;

	switch(m_wndTime.GetCurSel()) {
	case 0:
		start.SetDateTime(atoi(year), 1, 1, 0, 0, 0);
		end.SetDateTime(atoi(year), 12, 31, 23, 59, 59);
		break;
	case 1:
		start.SetDateTime(atoi(year), 1, 1, 0, 0, 0);
		end.SetDateTime(atoi(year), 1, 31, 23, 59, 59);
		break;
	case 2:
		start.SetDateTime(atoi(year), 2, 1, 0, 0, 0);
		end.SetDateTime(atoi(year), 2, 28, 23, 59, 59);
		break;
	case 3:
		start.SetDateTime(atoi(year), 3, 1, 0, 0, 0);
		end.SetDateTime(atoi(year), 3, 31, 23, 59, 59);
		break;
	case 4:
		start.SetDateTime(atoi(year), 4, 1, 0, 0, 0);
		end.SetDateTime(atoi(year), 4, 30, 23, 59, 59);
		break;
	case 5:
		start.SetDateTime(atoi(year), 5, 1, 0, 0, 0);
		end.SetDateTime(atoi(year), 5, 31, 23, 59, 59);
		break;
	case 6:
		start.SetDateTime(atoi(year), 6, 1, 0, 0, 0);
		end.SetDateTime(atoi(year), 6, 30, 23, 59, 59);
		break;
	case 7:
		start.SetDateTime(atoi(year), 7, 1, 0, 0, 0);
		end.SetDateTime(atoi(year), 7, 31, 23, 59, 59);
		break;
	case 8:
		start.SetDateTime(atoi(year), 8, 1, 0, 0, 0);
		end.SetDateTime(atoi(year), 8, 31, 23, 59, 59);
		break;
	case 9:
		start.SetDateTime(atoi(year), 9, 1, 0, 0, 0);
		end.SetDateTime(atoi(year), 9, 30, 23, 59, 59);
		break;
	case 10:
		start.SetDateTime(atoi(year), 10, 1, 0, 0, 0);
		end.SetDateTime(atoi(year), 10, 31, 23, 59, 59);
		break;
	case 11:
		start.SetDateTime(atoi(year), 11, 1, 0, 0, 0);
		end.SetDateTime(atoi(year), 11, 30, 23, 59, 59);
		break;
	case 12:
		start.SetDateTime(atoi(year), 12, 1, 0, 0, 0);
		end.SetDateTime(atoi(year), 12, 31, 23, 59, 59);
		break;
	case 13:
		start.SetDateTime(atoi(year), 1, 1, 0, 0, 0);
		end.SetDateTime(atoi(year), 3, 31, 23, 59, 59);
		break;
	case 14:
		start.SetDateTime(atoi(year), 4, 1, 0, 0, 0);
		end.SetDateTime(atoi(year), 6, 30, 23, 59, 59);
		break;
	case 15:
		start.SetDateTime(atoi(year), 7, 1, 0, 0, 0);
		end.SetDateTime(atoi(year), 9, 30, 23, 59, 59);
		break;
	case 16:
		start.SetDateTime(atoi(year), 10, 1, 0, 0, 0);
		end.SetDateTime(atoi(year), 12, 31, 23, 59, 59);
		break;
	default:;
	}

	m_Time_Start.SetTime(start);
	m_Time_End.SetTime(end);
}

void CZpqk::OnLocPrint() 
{
	theApp.ConnectDatabase();

	COleDateTime m_time;
	m_Time_Start.GetTime(m_time);
	CString start = m_time.Format("%Y-%m-%d %H:%M");
	
	m_Time_End.GetTime(m_time);
	CString end   = m_time.Format("%Y-%m-%d %H:%M");

	CString startno, endno;
	GetDlgItemText(IDC_EDIT_START, startno);
	GetDlgItemText(IDC_EDIT_END, endno);
	
	CString m_WhereBetween_Cmd;
	if(((CButton *)GetDlgItem(IDC_CHECK_MODE))->GetState() & 0x0003)
	{
		m_WhereBetween_Cmd.Format(" where medicalno >= '%s' and medicalno <= '%s'", startno, endno);
	}
	else
	{
		m_WhereBetween_Cmd.Format(" where bgrq >= '%s' and bgrq <= '%s'", start, end);
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			m_WhereBetween_Cmd.Format(" where bgrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and bgrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", start, end);
		}
	}
	
	m_WhereBetween_Cmd += " order by medicalno asc";

	CStringArray medicalno_Array;   medicalno_Array.RemoveAll();
	CUIntArray   ckzs_Array;        ckzs_Array.RemoveAll();
	CUIntArray   lkzs_Array;        lkzs_Array.RemoveAll();
	CStringArray wfyy_Array;        wfyy_Array.RemoveAll();

	try
	{
		SAString cmdstr = "Select medicalno, ckzs, lkzs, wfyy from " + theApp.TABLE_MAIN + m_WhereBetween_Cmd;
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();

		while ( g_dbcommand.FetchNext() )
		{
			medicalno_Array.Add(g_dbcommand.Field("medicalno").asString());
			ckzs_Array.Add(g_dbcommand.Field("ckzs").asLong());
			lkzs_Array.Add(g_dbcommand.Field("lkzs").asLong());
			wfyy_Array.Add(g_dbcommand.Field("wfyy").asString());
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
		return;
	}

	CString TjFmtName;
    TjFmtName.Format("制片情况统计表");

	CFormDataModel  *m_pFormDataModel;
	m_pFormDataModel = new CFormDataModel;

	bool rdok = false;
	try
	{
		g_dbcommand.setCommandText("Select rpt_data from REPORTFORMAT where rpt_name = :rpt_name");
		g_dbcommand.Param("rpt_name").setAsString() = TjFmtName;
		
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			SAString ss = g_dbcommand.Field("rpt_data").asBLob();
			
			CFile file;
			CFileException fe;
			
			if (file.Open( theApp.m_SysTmpPath + TjFmtName + ".rpt" , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
			{
				const void * binbuf = (const void *)ss;
				file.WriteHuge( binbuf, ss.GetBinaryLength() );
				file.Close();
			}

			if( m_pFormDataModel->OpenDocument(theApp.m_SysTmpPath + TjFmtName + ".rpt") == TRUE )
			{
				rdok = true;
			}
			remove(theApp.m_SysTmpPath + TjFmtName + ".rpt");
			
			ss.Empty();
		}
		
		g_dbconnection.Commit();
	}
	catch(SAException &)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
	}

	if(!rdok)
	{
		if(m_pFormDataModel != NULL)
		{
			delete m_pFormDataModel;
			m_pFormDataModel = NULL;
		}

		medicalno_Array.RemoveAll();
		ckzs_Array.RemoveAll();
		lkzs_Array.RemoveAll();
		wfyy_Array.RemoveAll();
		return;
	}

	int RecNum = 1;

	CFODrawShapeList *pShapeList = m_pFormDataModel->GetFormObjects();
	POSITION pos = pShapeList->GetHeadPosition();
	while ( pos )
	{
		CFODrawShape *pShape = pShapeList->GetNext(pos);
		if( pShape == NULL )   continue;
		UINT ObjType = pShape->GetType();
		if(ObjType == FO_COMP_FIELD)
		{
			CString str = pShape->GetObjectCaption();
			str.TrimLeft(); str.TrimRight();
			if(str.Find('_') >= 0)
			{
				str = str.Right(str.GetLength() - str.Find('_') - 1);
                if(str.Find('_') >= 0)
				{
					str = str.Left(str.Find('_'));
					int k = atoi(str);
					if( k > RecNum )
					{
						RecNum = k;
					}
				}
			}
		}
	}

	if(m_pFormDataModel != NULL)
	{
		delete m_pFormDataModel;
		m_pFormDataModel = NULL;
	}

	int totalnum = medicalno_Array.GetSize();

	for(int index = 0; index < totalnum; index += RecNum)
	{
		m_strlist.RemoveAll();
		m_titlelist.RemoveAll();

		for(int i = 0; i < RecNum; i++)
		{
			if((index +i) >= medicalno_Array.GetSize())  break;
			
			int m = 1;
			
			CString str;
			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(medicalno_Array.GetAt(index +i));

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			str.Format("%d", ckzs_Array.GetAt(index +i));
			m_strlist.Add(str);

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			str.Format("%d", lkzs_Array.GetAt(index +i));
			m_strlist.Add(str);
			
			for(int sk = 0; sk < 4; sk++)
			{
				try
				{
					SAString cmdstr;
					switch(sk) {
					case 0:
						cmdstr = "Select count(*) from " + theApp.TABLE_SLICE + " where medicalno = :medicalno";
						break;
					case 1:
						cmdstr = "Select count(*) from " + theApp.TABLE_SLICE + " where medicalno = :medicalno and qppj='Ⅰ级'";
						break;
					case 2:
						cmdstr = "Select count(*) from " + theApp.TABLE_SLICE + " where medicalno = :medicalno and qppj='Ⅱ级'";
						break;
					case 3:
						cmdstr = "Select count(*) from " + theApp.TABLE_SLICE + " where medicalno = :medicalno and qppj='Ⅲ级'";
						break;
					}
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = medicalno_Array.GetAt(index +i);
					
					g_dbcommand.Execute();
					
					if ( g_dbcommand.FetchNext() )
					{
						str.Format("TABLE_%d_%d",i+1,m++);
						m_titlelist.Add(str);
						str.Format("%d", g_dbcommand.Field(1).asLong());
						m_strlist.Add(str);
					}
					
					
					g_dbconnection.Commit();
				}
				catch(SAException &)
				{
					try
					{
						g_dbconnection.Rollback();
					}
					catch(SAException &)
					{
					}
				}
			}

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(wfyy_Array.GetAt(index +i));
		}

		m_titlelist.Add("起始时间");
		m_strlist.Add(start);

		m_titlelist.Add("终止时间");
		m_strlist.Add(end);

		CString str;
		m_titlelist.Add("总记录数");
		str.Format("%d", totalnum);
		m_strlist.Add(str);

		m_titlelist.Add("起始编号");
		m_strlist.Add(startno);

		m_titlelist.Add("终止编号");
		m_strlist.Add(endno);

		m_titlelist.Add("页码");
		str.Format("%d", index/RecNum + 1);
		m_strlist.Add(str);
		
		SendMessage(WM_COMMAND,ID_DLG_PRINT);

		while (m_bPrintPreview) {
			MSG msg;
			if (::PeekMessage((LPMSG)&msg,(HWND)NULL,(WORD)NULL,(WORD)NULL,TRUE))
			{
				TranslateMessage((LPMSG)&msg);
				DispatchMessage((LPMSG)&msg);
			}	
		}
	}

	medicalno_Array.RemoveAll();
	ckzs_Array.RemoveAll();
	lkzs_Array.RemoveAll();
	wfyy_Array.RemoveAll();

	m_strlist.RemoveAll();
	m_titlelist.RemoveAll();
}

void CZpqk::OnLocPrintPreview() 
{
	theApp.ConnectDatabase();

	COleDateTime m_time;
	m_Time_Start.GetTime(m_time);
	CString start = m_time.Format("%Y-%m-%d %H:%M");
	
	m_Time_End.GetTime(m_time);
	CString end   = m_time.Format("%Y-%m-%d %H:%M");

	CString startno, endno;
	GetDlgItemText(IDC_EDIT_START, startno);
	GetDlgItemText(IDC_EDIT_END, endno);
	
	CString m_WhereBetween_Cmd;
	if(((CButton *)GetDlgItem(IDC_CHECK_MODE))->GetState() & 0x0003)
	{
		m_WhereBetween_Cmd.Format(" where medicalno >= '%s' and medicalno <= '%s'", startno, endno);
	}
	else
	{
		m_WhereBetween_Cmd.Format(" where bgrq >= '%s' and bgrq <= '%s'", start, end);
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			m_WhereBetween_Cmd.Format(" where bgrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and bgrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", start, end);
		}
	}
	
	m_WhereBetween_Cmd += " order by medicalno asc";

	CStringArray medicalno_Array;   medicalno_Array.RemoveAll();
	CUIntArray   ckzs_Array;        ckzs_Array.RemoveAll();
	CUIntArray   lkzs_Array;        lkzs_Array.RemoveAll();
	CStringArray wfyy_Array;        wfyy_Array.RemoveAll();

	try
	{
		SAString cmdstr = "Select medicalno, ckzs, lkzs, wfyy from " + theApp.TABLE_MAIN + m_WhereBetween_Cmd;
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();

		while ( g_dbcommand.FetchNext() )
		{
			medicalno_Array.Add(g_dbcommand.Field("medicalno").asString());
			ckzs_Array.Add(g_dbcommand.Field("ckzs").asLong());
			lkzs_Array.Add(g_dbcommand.Field("lkzs").asLong());
			wfyy_Array.Add(g_dbcommand.Field("wfyy").asString());
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
		return;
	}

	CString TjFmtName;
    TjFmtName.Format("制片情况统计表");

	CFormDataModel  *m_pFormDataModel;
	m_pFormDataModel = new CFormDataModel;

	bool rdok = false;
	try
	{
		g_dbcommand.setCommandText("Select rpt_data from REPORTFORMAT where rpt_name = :rpt_name");
		g_dbcommand.Param("rpt_name").setAsString() = TjFmtName;
		
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			SAString ss = g_dbcommand.Field("rpt_data").asBLob();
			
			CFile file;
			CFileException fe;
			
			if (file.Open( theApp.m_SysTmpPath + TjFmtName + ".rpt" , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
			{
				const void * binbuf = (const void *)ss;
				file.WriteHuge( binbuf, ss.GetBinaryLength() );
				file.Close();
			}

			if( m_pFormDataModel->OpenDocument(theApp.m_SysTmpPath + TjFmtName + ".rpt") == TRUE )
			{
				rdok = true;
			}
			remove(theApp.m_SysTmpPath + TjFmtName + ".rpt");
			
			ss.Empty();
		}
		
		g_dbconnection.Commit();
	}
	catch(SAException &)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
	}

	if(!rdok)
	{
		if(m_pFormDataModel != NULL)
		{
			delete m_pFormDataModel;
			m_pFormDataModel = NULL;
		}

		medicalno_Array.RemoveAll();
		ckzs_Array.RemoveAll();
		lkzs_Array.RemoveAll();
		wfyy_Array.RemoveAll();
		return;
	}

	int RecNum = 1;

	CFODrawShapeList *pShapeList = m_pFormDataModel->GetFormObjects();
	POSITION pos = pShapeList->GetHeadPosition();
	while ( pos )
	{
		CFODrawShape *pShape = pShapeList->GetNext(pos);
		if( pShape == NULL )   continue;
		UINT ObjType = pShape->GetType();
		if(ObjType == FO_COMP_FIELD)
		{
			CString str = pShape->GetObjectCaption();
			str.TrimLeft(); str.TrimRight();
			if(str.Find('_') >= 0)
			{
				str = str.Right(str.GetLength() - str.Find('_') - 1);
                if(str.Find('_') >= 0)
				{
					str = str.Left(str.Find('_'));
					int k = atoi(str);
					if( k > RecNum )
					{
						RecNum = k;
					}
				}
			}
		}
	}

	if(m_pFormDataModel != NULL)
	{
		delete m_pFormDataModel;
		m_pFormDataModel = NULL;
	}

	int totalnum = medicalno_Array.GetSize();

	for(int index = 0; index < totalnum; index += RecNum)
	{
		m_strlist.RemoveAll();
		m_titlelist.RemoveAll();

		for(int i = 0; i < RecNum; i++)
		{
			if((index +i) >= medicalno_Array.GetSize())  break;
			
			int m = 1;
			
			CString str;
			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(medicalno_Array.GetAt(index +i));

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			str.Format("%d", ckzs_Array.GetAt(index +i));
			m_strlist.Add(str);

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			str.Format("%d", lkzs_Array.GetAt(index +i));
			m_strlist.Add(str);
			
			for(int sk = 0; sk < 4; sk++)
			{
				try
				{
					SAString cmdstr;
					switch(sk) {
					case 0:
						cmdstr = "Select count(*) from " + theApp.TABLE_SLICE + " where medicalno = :medicalno";
						break;
					case 1:
						cmdstr = "Select count(*) from " + theApp.TABLE_SLICE + " where medicalno = :medicalno and qppj='Ⅰ级'";
						break;
					case 2:
						cmdstr = "Select count(*) from " + theApp.TABLE_SLICE + " where medicalno = :medicalno and qppj='Ⅱ级'";
						break;
					case 3:
						cmdstr = "Select count(*) from " + theApp.TABLE_SLICE + " where medicalno = :medicalno and qppj='Ⅲ级'";
						break;
					}
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = medicalno_Array.GetAt(index +i);
					
					g_dbcommand.Execute();
					
					if ( g_dbcommand.FetchNext() )
					{
						str.Format("TABLE_%d_%d",i+1,m++);
						m_titlelist.Add(str);
						str.Format("%d", g_dbcommand.Field(1).asLong());
						m_strlist.Add(str);
					}
					
					
					g_dbconnection.Commit();
				}
				catch(SAException &)
				{
					try
					{
						g_dbconnection.Rollback();
					}
					catch(SAException &)
					{
					}
				}
			}
		
			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(wfyy_Array.GetAt(index +i));
		}

		m_titlelist.Add("起始时间");
		m_strlist.Add(start);

		m_titlelist.Add("终止时间");
		m_strlist.Add(end);

		CString str;
		m_titlelist.Add("总记录数");
		str.Format("%d", totalnum);
		m_strlist.Add(str);

		m_titlelist.Add("起始编号");
		m_strlist.Add(startno);

		m_titlelist.Add("终止编号");
		m_strlist.Add(endno);

		m_titlelist.Add("页码");
		str.Format("%d", index/RecNum + 1);
		m_strlist.Add(str);
		
		SendMessage(WM_COMMAND,ID_DLG_PRINTPREVIEW);

		while (m_bPrintPreview) {
			MSG msg;
			if (::PeekMessage((LPMSG)&msg,(HWND)NULL,(WORD)NULL,(WORD)NULL,TRUE))
			{
				TranslateMessage((LPMSG)&msg);
				DispatchMessage((LPMSG)&msg);
			}	
		}
	}

	medicalno_Array.RemoveAll();
	ckzs_Array.RemoveAll();
	lkzs_Array.RemoveAll();
	wfyy_Array.RemoveAll();

	m_strlist.RemoveAll();
	m_titlelist.RemoveAll();
}
