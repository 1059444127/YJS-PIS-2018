// Ckyj.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "Ckyj.h"
#include "INI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCkyj dialog
extern CPathologyApp theApp;

CCkyj::CCkyj(CWnd* pParent /*=NULL*/)
	: CPrintPreviewDialog(CCkyj::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCkyj)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_STATISTIC);
}


void CCkyj::DoDataExchange(CDataExchange* pDX)
{
	CPrintPreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCkyj)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCkyj, CPrintPreviewDialog)
	//{{AFX_MSG_MAP(CCkyj)
	ON_COMMAND(ID_LOC_PRINT_PREVIEW, OnLocPrintPreview)
	ON_COMMAND(ID_LOC_PRINT, OnLocPrint)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE( TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
    ON_NOTIFY_EX_RANGE( TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCkyj message handlers

BOOL CCkyj::OnInitDialog() 
{
	CPrintPreviewDialog::OnInitDialog();
	
	m_UsageType = 0;
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

	for(int i = 0; i < theApp.m_DBName_list.GetSize(); i++)
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_DB))->AddString(theApp.m_DBName_list.GetAt(i));
	}
	((CComboBox *)GetDlgItem(IDC_COMBO_DB))->AddString("全部");
	((CComboBox *)GetDlgItem(IDC_COMBO_DB))->SetCurSel(((CComboBox *)GetDlgItem(IDC_COMBO_DB))->GetCount()-1);

	// Move control bars to the correct position.
    RecalcBarLayout();
	
	m_wndToolBar.GetToolBarCtrl().DeleteButton(0);
	m_wndToolBar.GetToolBarCtrl().DeleteButton(0);

	CIniReader m_IniReader;
	m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");
	CString startno = m_IniReader.getKeyValue("材块移交起始编号","Settings");
	CString endno   = m_IniReader.getKeyValue("材块移交终止编号","Settings");
	SetDlgItemText(IDC_EDIT_START, startno);
	SetDlgItemText(IDC_EDIT_END,   endno);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HCURSOR CCkyj::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CCkyj::OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult )
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
    return CCkyj::OnToolTipText(nID,pNMHDR,pResult);
}

BOOL CCkyj::GetToolText( UINT nID, CString& strTipText, CString& /*strMessage*/)
{
    switch( nID )
    {
		case ID_LOC_PRINT_PREVIEW:
			strTipText.Format("移交表打印预览");
			break;
		case ID_LOC_PRINT:
			strTipText.Format("打印移交表");
			break;
    }
       
    return TRUE;
}

void CCkyj::GenList() 
{
}

void CCkyj::OnLocPrintPreview() 
{
	theApp.ConnectDatabase();

	int bms_sum = 0, cks_sum = 0;

	COleDateTime m_time;
	m_Time_Start.GetTime(m_time);
	CString start = m_time.Format("%Y-%m-%d %H:%M");
	
	m_Time_End.GetTime(m_time);
	CString end   = m_time.Format("%Y-%m-%d %H:%M");

	CString startno, endno;
	GetDlgItemText(IDC_EDIT_START, startno);
	GetDlgItemText(IDC_EDIT_END, endno);

	CString group;
	GetDlgItemText(IDC_COMBO_GROUP, group);
	group.TrimLeft();group.TrimRight();
	
	CString m_WhereBetween_Cmd;

	if(group.IsEmpty())
	{
		if(((CButton *)GetDlgItem(IDC_CHECK_MODE))->GetState() & 0x0003)
			m_WhereBetween_Cmd.Format(" where medicalno >= '%s' and medicalno <= '%s'", startno, endno);
		else
		{
			if(((CComboBox *)GetDlgItem(IDC_COMBO_DB))->GetCurSel() >= theApp.m_DBName_list.GetSize())
			{
				m_WhereBetween_Cmd.Format(" where qcrq >= '%s' and qcrq <= '%s'", start, end);
				if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
				{
					m_WhereBetween_Cmd.Format(" where qcrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and qcrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", start, end);
				}
			}
			else
			{
				m_WhereBetween_Cmd.Format(" where qcrq >= '%s' and qcrq <= '%s' and medicalno in (select medicalno from cmipsmain where dbtype=%d)", start, end, theApp.m_DBNo_list.GetAt(((CComboBox *)GetDlgItem(IDC_COMBO_DB))->GetCurSel()));
				if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
				{
					m_WhereBetween_Cmd.Format(" where qcrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and qcrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss') and medicalno in (select medicalno from cmipsmain where dbtype=%d)", start, end, theApp.m_DBNo_list.GetAt(((CComboBox *)GetDlgItem(IDC_COMBO_DB))->GetCurSel()));
				}
			}
		}
	}
	else
	{
		m_WhereBetween_Cmd.Format(" where medicalno like '%%%s' and qcrq >= '%s' and qcrq <= '%s'", group, start, end);
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			m_WhereBetween_Cmd.Format(" where medicalno like '%%%s' and qcrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and qcrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", group, start, end);
		}
	}
	

	try
	{
		SAString cmdstr = "Select count(*), sum(cks) from " + theApp.TABLE_MATERIAL + m_WhereBetween_Cmd;
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
		{
			bms_sum = g_dbcommand.Field(1).asLong();
			cks_sum = g_dbcommand.Field(2).asLong();
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

	m_WhereBetween_Cmd += " group by medicalno order by medicalno asc";

	CStringArray medicalno_Array;   medicalno_Array.RemoveAll();
	CUIntArray   cks_Array;         cks_Array.RemoveAll();
	CUIntArray   lks_Array;         lks_Array.RemoveAll();
	CStringArray qcys_Array;        qcys_Array.RemoveAll();
	CStringArray ckhd_Array;        ckhd_Array.RemoveAll();

	try
	{
		SAString cmdstr = "Select medicalno, sum(cks), count(*) from " + theApp.TABLE_MATERIAL + m_WhereBetween_Cmd;
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();

		while ( g_dbcommand.FetchNext() )
		{
			medicalno_Array.Add(g_dbcommand.Field("medicalno").asString());
			cks_Array.Add(g_dbcommand.Field(2).asLong());
			lks_Array.Add(g_dbcommand.Field(3).asLong());
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

	for(int i = 0; i < medicalno_Array.GetSize(); i++)
	{
		try
		{
			SAString cmdstr = "Select qcys, ckhd from " + theApp.TABLE_MATERIAL + " where medicalno=:medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = medicalno_Array.GetAt(i);
			
			g_dbcommand.Execute();
			
			if ( g_dbcommand.FetchNext() )
			{
				qcys_Array.Add(g_dbcommand.Field("qcys").asString());
				ckhd_Array.Add(g_dbcommand.Field("ckhd").asString());
			}
			else
			{
				qcys_Array.Add("");
				ckhd_Array.Add("");
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

	CString TjFmtName;
    TjFmtName.Format("材块移交表");

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
		cks_Array.RemoveAll();
		lks_Array.RemoveAll();
		qcys_Array.RemoveAll();
		ckhd_Array.RemoveAll();
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
			str.Format("%d", cks_Array.GetAt(index +i));
			m_strlist.Add(str);

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			str.Format("%d", lks_Array.GetAt(index +i));
			m_strlist.Add(str);
			
			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(qcys_Array.GetAt(index +i));

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(ckhd_Array.GetAt(index +i));
		}

		m_titlelist.Add("起始时间");
		m_strlist.Add(start);

		m_titlelist.Add("终止时间");
		m_strlist.Add(end);

		CString str;
		m_titlelist.Add("待包埋数");
		str.Format("%d", bms_sum);
		m_strlist.Add(str);

		m_titlelist.Add("材块总数");
		str.Format("%d", cks_sum);
		m_strlist.Add(str);

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
	cks_Array.RemoveAll();
	lks_Array.RemoveAll();
	qcys_Array.RemoveAll();
	ckhd_Array.RemoveAll();

	m_strlist.RemoveAll();
	m_titlelist.RemoveAll();
}

void CCkyj::OnLocPrint() 
{
	theApp.ConnectDatabase();

	int bms_sum = 0, cks_sum = 0;

	COleDateTime m_time;
	m_Time_Start.GetTime(m_time);
	CString start = m_time.Format("%Y-%m-%d %H:%M");
	
	m_Time_End.GetTime(m_time);
	CString end   = m_time.Format("%Y-%m-%d %H:%M");

	CString startno, endno;
	GetDlgItemText(IDC_EDIT_START, startno);
	GetDlgItemText(IDC_EDIT_END, endno);
	
	CString group;
	GetDlgItemText(IDC_COMBO_GROUP, group);
	group.TrimLeft();group.TrimRight();
	
	CString m_WhereBetween_Cmd;
	
	if(group.IsEmpty())
	{
		if(((CButton *)GetDlgItem(IDC_CHECK_MODE))->GetState() & 0x0003)
			m_WhereBetween_Cmd.Format(" where medicalno >= '%s' and medicalno <= '%s'", startno, endno);
		else
		{
			if(((CComboBox *)GetDlgItem(IDC_COMBO_DB))->GetCurSel() >= theApp.m_DBName_list.GetSize())
			{
				m_WhereBetween_Cmd.Format(" where qcrq >= '%s' and qcrq <= '%s'", start, end);
				if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
				{
					m_WhereBetween_Cmd.Format(" where qcrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and qcrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", start, end);
				}
			}
			else
			{
				m_WhereBetween_Cmd.Format(" where qcrq >= '%s' and qcrq <= '%s' and medicalno in (select medicalno from cmipsmain where dbtype=%d)", start, end, theApp.m_DBNo_list.GetAt(((CComboBox *)GetDlgItem(IDC_COMBO_DB))->GetCurSel()));
				if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
				{
					m_WhereBetween_Cmd.Format(" where qcrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and qcrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss') and medicalno in (select medicalno from cmipsmain where dbtype=%d)", start, end, theApp.m_DBNo_list.GetAt(((CComboBox *)GetDlgItem(IDC_COMBO_DB))->GetCurSel()));
				}
			}
		}
	}
	else
	{
		m_WhereBetween_Cmd.Format(" where medicalno like '%%%s' and qcrq >= '%s' and qcrq <= '%s'", group, start, end);
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			m_WhereBetween_Cmd.Format(" where medicalno like '%%%s' and qcrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and qcrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", group, start, end);
		}
	}

	try
	{
		SAString cmdstr = "Select count(*), sum(cks) from " + theApp.TABLE_MATERIAL + m_WhereBetween_Cmd;
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
		{
			bms_sum = g_dbcommand.Field(1).asLong();
			cks_sum = g_dbcommand.Field(2).asLong();
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

	m_WhereBetween_Cmd += " group by medicalno order by medicalno asc";

	CStringArray medicalno_Array;   medicalno_Array.RemoveAll();
	CUIntArray   cks_Array;         cks_Array.RemoveAll();
	CUIntArray   lks_Array;         lks_Array.RemoveAll();
	CStringArray qcys_Array;        qcys_Array.RemoveAll();
	CStringArray ckhd_Array;        ckhd_Array.RemoveAll();

	try
	{
		SAString cmdstr = "Select medicalno, sum(cks), count(*) from " + theApp.TABLE_MATERIAL + m_WhereBetween_Cmd;
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();

		while ( g_dbcommand.FetchNext() )
		{
			medicalno_Array.Add(g_dbcommand.Field("medicalno").asString());
			cks_Array.Add(g_dbcommand.Field(2).asLong());
			lks_Array.Add(g_dbcommand.Field(3).asLong());
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

	for(int i = 0; i < medicalno_Array.GetSize(); i++)
	{
		try
		{
			SAString cmdstr = "Select qcys, ckhd from " + theApp.TABLE_MATERIAL + " where medicalno=:medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = medicalno_Array.GetAt(i);
			
			g_dbcommand.Execute();
			
			if ( g_dbcommand.FetchNext() )
			{
				qcys_Array.Add(g_dbcommand.Field("qcys").asString());
				ckhd_Array.Add(g_dbcommand.Field("ckhd").asString());
			}
			else
			{
				qcys_Array.Add("");
				ckhd_Array.Add("");
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

	CString TjFmtName;
    TjFmtName.Format("材块移交表");

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
		cks_Array.RemoveAll();
		lks_Array.RemoveAll();
		qcys_Array.RemoveAll();
		ckhd_Array.RemoveAll();
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
			str.Format("%d", cks_Array.GetAt(index +i));
			m_strlist.Add(str);

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			str.Format("%d", lks_Array.GetAt(index +i));
			m_strlist.Add(str);
			
			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(qcys_Array.GetAt(index +i));

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(ckhd_Array.GetAt(index +i));
		}

		m_titlelist.Add("起始时间");
		m_strlist.Add(start);

		m_titlelist.Add("终止时间");
		m_strlist.Add(end);

		CString str;
		m_titlelist.Add("待包埋数");
		str.Format("%d", bms_sum);
		m_strlist.Add(str);

		m_titlelist.Add("材块总数");
		str.Format("%d", cks_sum);
		m_strlist.Add(str);

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
	cks_Array.RemoveAll();
	lks_Array.RemoveAll();
	qcys_Array.RemoveAll();
	ckhd_Array.RemoveAll();

	m_strlist.RemoveAll();
	m_titlelist.RemoveAll();
}

void CCkyj::OnDestroy() 
{
	CIniReader m_IniReader;
	m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");

	CString startno, endno;
	GetDlgItemText(IDC_EDIT_START, startno);
	GetDlgItemText(IDC_EDIT_END, endno);

	m_IniReader.setKey(startno,"材块移交起始编号","Settings");
	m_IniReader.setKey(endno,  "材块移交终止编号","Settings");

	CPrintPreviewDialog::OnDestroy();
}
