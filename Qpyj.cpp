// Qpyj.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "Qpyj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQpyj dialog
extern CPathologyApp theApp;

CQpyj::CQpyj(CWnd* pParent /*=NULL*/)
	: CPrintPreviewDialog(CQpyj::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQpyj)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_STATISTIC);
}


void CQpyj::DoDataExchange(CDataExchange* pDX)
{
	CPrintPreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQpyj)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQpyj, CPrintPreviewDialog)
	//{{AFX_MSG_MAP(CQpyj)
	ON_COMMAND(ID_LOC_PRINT, OnLocPrint)
	ON_COMMAND(ID_LOC_PRINT_PREVIEW, OnLocPrintPreview)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE( TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
    ON_NOTIFY_EX_RANGE( TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQpyj message handlers

BOOL CQpyj::OnInitDialog() 
{
	CPrintPreviewDialog::OnInitDialog();
	
	m_UsageType = 1;
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

	// Move control bars to the correct position.
    RecalcBarLayout();

	m_wndToolBar.GetToolBarCtrl().DeleteButton(0);
	m_wndToolBar.GetToolBarCtrl().DeleteButton(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HCURSOR CQpyj::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CQpyj::OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult )
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
    return CQpyj::OnToolTipText(nID,pNMHDR,pResult);
}

BOOL CQpyj::GetToolText( UINT nID, CString& strTipText, CString& /*strMessage*/)
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

void CQpyj::GenList() 
{
}

void CQpyj::OnLocPrint() 
{
	CString rwlystr; rwlystr.Empty();
	CString rwly;    rwly.Empty();
	int kind = 0;
	if(((CButton *)GetDlgItem(IDC_CHECK_ZC))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '常规'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "常规";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_BD))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '冰冻'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "冰冻";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_TG))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '脱钙'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "脱钙";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_CQ))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '重切'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "重切";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_SQ))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '深切'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "深切";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_BQ))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '补取'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "补取";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_MYZH))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '免疫组化'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "免疫组化";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_FZBL))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '分子病理'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "分子病理";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_DJ))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '电镜'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "电镜";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_TSRS))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '特殊染色'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "特殊染色";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_BAOQIE))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '薄切'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "薄切";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_LQ))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '连切'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "连切";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_BS))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '冰石'";
		kind++;
		
		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "冰石";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_BY))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '冰余'";
		kind++;
		
		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "冰余";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_QQ))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '切全'";
		kind++;
		
		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "切全";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_BXLQ))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '不削连切'";
		kind++;
		
		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "不削连切";
	}

	if(rwlystr.IsEmpty())  
	{
		AfxMessageBox("没有指定任务来源!");
		return;
	}

	if(kind == 12)
	{
		rwlystr.Empty();
		rwly.Format("全部");
	}

	theApp.ConnectDatabase();

	int qps_sum = 0;

	COleDateTime m_time;
	m_Time_Start.GetTime(m_time);
	CString start = m_time.Format("%Y-%m-%d %H:%M");
	
	m_Time_End.GetTime(m_time);
	CString end   = m_time.Format("%Y-%m-%d %H:%M");
	
	CString startno, endno;
	GetDlgItemText(IDC_EDIT_START, startno);
	GetDlgItemText(IDC_EDIT_END, endno);

	CString group;
	GetDlgItemText(IDC_COMBO_GROUP,group);
	group.TrimLeft();group.TrimRight();
	
	CString m_WhereBetween_Cmd;
	if(group.IsEmpty())
	{
		if(((CButton *)GetDlgItem(IDC_CHECK_MODE))->GetState() & 0x0003)
		{
			if(rwlystr.IsEmpty())
				m_WhereBetween_Cmd.Format(" where medicalno >= '%s' and medicalno <= '%s'", startno, endno);
			else
				m_WhereBetween_Cmd.Format(" where medicalno >= '%s' and medicalno <= '%s' and (%s)", startno, endno, rwlystr);
		}
		else
		{
			if(rwlystr.IsEmpty())
				m_WhereBetween_Cmd.Format(" where qprq >= '%s' and qprq <= '%s'", start, end);
			else
				m_WhereBetween_Cmd.Format(" where qprq >= '%s' and qprq <= '%s' and (%s)", start, end, rwlystr);
			
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				if(rwlystr.IsEmpty())
					m_WhereBetween_Cmd.Format(" where qprq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and qprq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", start, end);
				else
					m_WhereBetween_Cmd.Format(" where qprq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and qprq <= to_date('%s','yyyy-mm-dd hh24:mi:ss') and (%s)", start, end, rwlystr);
			}
		}
	}
	else
	{
		m_WhereBetween_Cmd.Format(" where medicalno like '%%%s' and qprq >= '%s' and qprq <= '%s' and (%s)", group, start, end, rwlystr);
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			m_WhereBetween_Cmd.Format(" where medicalno like '%%%s' and qprq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and qprq <= to_date('%s','yyyy-mm-dd hh24:mi:ss') and (%s)", group, start, end, rwlystr);
		}
	}
	
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_SLICE + m_WhereBetween_Cmd;
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
		{
			qps_sum = g_dbcommand.Field(1).asLong();
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

	m_WhereBetween_Cmd += " order by medicalno asc, qptmh asc";

	CStringArray medicalno_Array;   medicalno_Array.RemoveAll();
	CStringArray lkh_Array;         lkh_Array.RemoveAll();
	CStringArray qpsm_Array;        qpsm_Array.RemoveAll();
	CStringArray qptmh_Array;       qptmh_Array.RemoveAll();
	CStringArray qpczy_Array;       qpczy_Array.RemoveAll();
	CStringArray bphd_Array;        bphd_Array.RemoveAll();

	SAString  medicalno_previous, lkh_previous;
	medicalno_previous.Empty(); lkh_previous.Empty();
	try
	{
		SAString cmdstr = "Select medicalno, lkh, qpsm, qptmh, qpczy, bphd from " + theApp.TABLE_SLICE + m_WhereBetween_Cmd;
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();

		while ( g_dbcommand.FetchNext() )
		{
			if(g_dbcommand.Field("medicalno").asString() == medicalno_previous)
			{
				medicalno_Array.Add("");
			}
			else
			{
				medicalno_Array.Add(g_dbcommand.Field("medicalno").asString());
				medicalno_previous = g_dbcommand.Field("medicalno").asString();
			}
			if(g_dbcommand.Field("lkh").asString() == lkh_previous)
			{
				lkh_Array.Add("");
			}
			else
			{
				lkh_Array.Add(g_dbcommand.Field("lkh").asString());
				lkh_previous = g_dbcommand.Field("lkh").asString();
			}
			qpsm_Array.Add(g_dbcommand.Field("qpsm").asString());
			qptmh_Array.Add(g_dbcommand.Field("qptmh").asString());
			qpczy_Array.Add(g_dbcommand.Field("qpczy").asString());
			bphd_Array.Add(g_dbcommand.Field("bphd").asString());
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
    TjFmtName.Format("切片移交表");

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
		lkh_Array.RemoveAll();
		qpsm_Array.RemoveAll();
		qptmh_Array.RemoveAll();
		qpczy_Array.RemoveAll();
		bphd_Array.RemoveAll();
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
			str.Format("TABLE_%d_%d",i+1,1);
			m_titlelist.Add(str);
			m_strlist.Add(medicalno_Array.GetAt(index +i));
			
			m++;
			str.Format("TABLE_%d_%d",i+1,m);
			m_titlelist.Add(str);
			m_strlist.Add(lkh_Array.GetAt(index +i));

			m++;
			str.Format("TABLE_%d_%d",i+1,m);
			m_titlelist.Add(str);
			m_strlist.Add(qpsm_Array.GetAt(index +i));
			
			m++;
			str.Format("TABLE_%d_%d",i+1,m);
			m_titlelist.Add(str);
			m_strlist.Add(qptmh_Array.GetAt(index +i));
			
			m++;
			str.Format("TABLE_%d_%d",i+1,m);
			m_titlelist.Add(str);
			m_strlist.Add(qpczy_Array.GetAt(index +i));

			m++;
			str.Format("TABLE_%d_%d",i+1,m);
			m_titlelist.Add(str);
			m_strlist.Add(bphd_Array.GetAt(index +i));
		}

		m_titlelist.Add("起始时间");
		m_strlist.Add(start);

		m_titlelist.Add("终止时间");
		m_strlist.Add(end);

		CString str;
		m_titlelist.Add("切片总数");
		str.Format("%d", qps_sum);
		m_strlist.Add(str);

		m_titlelist.Add("任务来源");
		m_strlist.Add(rwly);

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
	lkh_Array.RemoveAll();
	qpsm_Array.RemoveAll();
	qptmh_Array.RemoveAll();
	qpczy_Array.RemoveAll();
	bphd_Array.RemoveAll();

	m_strlist.RemoveAll();
	m_titlelist.RemoveAll();
}

void CQpyj::OnLocPrintPreview() 
{
	CString rwlystr; rwlystr.Empty();
	CString rwly;    rwly.Empty();
	int kind = 0;
	if(((CButton *)GetDlgItem(IDC_CHECK_ZC))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '常规'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "常规";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_BD))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '冰冻'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "冰冻";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_TG))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '脱钙'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "脱钙";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_CQ))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '重切'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "重切";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_SQ))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '深切'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "深切";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_BQ))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '补取'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "补取";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_MYZH))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '免疫组化'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "免疫组化";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_FZBL))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '分子病理'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "分子病理";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_DJ))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '电镜'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "电镜";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_TSRS))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '特殊染色'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "特殊染色";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_BAOQIE))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '薄切'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "薄切";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_LQ))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '连切'";
		kind++;

		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "连切";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_BS))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '冰石'";
		kind++;
		
		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "冰石";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_BY))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '冰余'";
		kind++;
		
		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "冰余";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_QQ))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '切全'";
		kind++;
		
		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "切全";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_BXLQ))->GetState() & 0x0003)
	{
		if(!rwlystr.IsEmpty()) rwlystr += " or ";
		rwlystr += "rwly = '不削连切'";
		kind++;
		
		if(!rwly.IsEmpty())  rwly += ",";
		rwly += "不削连切";
	}

	if(rwlystr.IsEmpty())  
	{
		AfxMessageBox("没有指定任务来源!");
		return;
	}

	if(kind == 12)
	{
		rwlystr.Empty();
		rwly.Format("全部");
	}

	theApp.ConnectDatabase();

	int qps_sum = 0;

	COleDateTime m_time;
	m_Time_Start.GetTime(m_time);
	CString start = m_time.Format("%Y-%m-%d %H:%M");
	
	m_Time_End.GetTime(m_time);
	CString end   = m_time.Format("%Y-%m-%d %H:%M");
	
	CString startno, endno;
	GetDlgItemText(IDC_EDIT_START, startno);
	GetDlgItemText(IDC_EDIT_END, endno);

	CString group;
	GetDlgItemText(IDC_COMBO_GROUP,group);
	group.TrimLeft();group.TrimRight();

	CString m_WhereBetween_Cmd;
	if(group.IsEmpty())
	{
		if(((CButton *)GetDlgItem(IDC_CHECK_MODE))->GetState() & 0x0003)
		{
			if(rwlystr.IsEmpty())
				m_WhereBetween_Cmd.Format(" where medicalno >= '%s' and medicalno <= '%s'", startno, endno);
			else
				m_WhereBetween_Cmd.Format(" where medicalno >= '%s' and medicalno <= '%s' and (%s)", startno, endno, rwlystr);
		}
		else
		{
			if(rwlystr.IsEmpty())
				m_WhereBetween_Cmd.Format(" where qprq >= '%s' and qprq <= '%s'", start, end);
			else
				m_WhereBetween_Cmd.Format(" where qprq >= '%s' and qprq <= '%s' and (%s)", start, end, rwlystr);
			
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				if(rwlystr.IsEmpty())
					m_WhereBetween_Cmd.Format(" where qprq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and qprq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", start, end);
				else
					m_WhereBetween_Cmd.Format(" where qprq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and qprq <= to_date('%s','yyyy-mm-dd hh24:mi:ss') and (%s)", start, end, rwlystr);
			}
		}
	}
	else
	{
		m_WhereBetween_Cmd.Format(" where medicalno like '%%%s' and qprq >= '%s' and qprq <= '%s' and (%s)", group, start, end, rwlystr);
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			m_WhereBetween_Cmd.Format(" where medicalno like '%%%s' and qprq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and qprq <= to_date('%s','yyyy-mm-dd hh24:mi:ss') and (%s)", group, start, end, rwlystr);
		}
	}


	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_SLICE + m_WhereBetween_Cmd;
		g_dbcommand.setCommandText(cmdstr);
	
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
		{
			qps_sum = g_dbcommand.Field(1).asLong();
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

	m_WhereBetween_Cmd += " order by medicalno asc, qptmh asc";

	CStringArray medicalno_Array;   medicalno_Array.RemoveAll();
	CStringArray lkh_Array;         lkh_Array.RemoveAll();
	CStringArray qpsm_Array;        qpsm_Array.RemoveAll();
	CStringArray qptmh_Array;       qptmh_Array.RemoveAll();
	CStringArray qpczy_Array;       qpczy_Array.RemoveAll();
	CStringArray bphd_Array;        bphd_Array.RemoveAll();

	SAString  medicalno_previous, lkh_previous;
	medicalno_previous.Empty(); lkh_previous.Empty();
	try
	{
		SAString cmdstr = "Select medicalno, lkh, qpsm, qptmh, qpczy, bphd from " + theApp.TABLE_SLICE + m_WhereBetween_Cmd;
		g_dbcommand.setCommandText(cmdstr);
	
		g_dbcommand.Execute();

		while ( g_dbcommand.FetchNext() )
		{
			if(g_dbcommand.Field("medicalno").asString() == medicalno_previous)
			{
				medicalno_Array.Add("");
			}
			else
			{
				medicalno_Array.Add(g_dbcommand.Field("medicalno").asString());
				medicalno_previous = g_dbcommand.Field("medicalno").asString();
			}
			if(g_dbcommand.Field("lkh").asString() == lkh_previous)
			{
				lkh_Array.Add(g_dbcommand.Field("lkh").asString());
			}
			else
			{
				lkh_Array.Add(g_dbcommand.Field("lkh").asString());
				lkh_previous = g_dbcommand.Field("lkh").asString();
			}
			qpsm_Array.Add(g_dbcommand.Field("qpsm").asString());
			qptmh_Array.Add(g_dbcommand.Field("qptmh").asString());
			qpczy_Array.Add(g_dbcommand.Field("qpczy").asString());
			bphd_Array.Add(g_dbcommand.Field("bphd").asString());
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
    TjFmtName.Format("切片移交表");

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
		lkh_Array.RemoveAll();
		qpsm_Array.RemoveAll();
		qptmh_Array.RemoveAll();
		qpczy_Array.RemoveAll();
		bphd_Array.RemoveAll();
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
			str.Format("TABLE_%d_%d",i+1,1);
			m_titlelist.Add(str);
			m_strlist.Add(medicalno_Array.GetAt(index +i));
			
			m++;
			str.Format("TABLE_%d_%d",i+1,m);
			m_titlelist.Add(str);
			m_strlist.Add(lkh_Array.GetAt(index +i));

			m++;
			str.Format("TABLE_%d_%d",i+1,m);
			m_titlelist.Add(str);
			m_strlist.Add(qpsm_Array.GetAt(index +i));
			
			m++;
			str.Format("TABLE_%d_%d",i+1,m);
			m_titlelist.Add(str);
			m_strlist.Add(qptmh_Array.GetAt(index +i));
			
			m++;
			str.Format("TABLE_%d_%d",i+1,m);
			m_titlelist.Add(str);
			m_strlist.Add(qpczy_Array.GetAt(index +i));

			m++;
			str.Format("TABLE_%d_%d",i+1,m);
			m_titlelist.Add(str);
			m_strlist.Add(bphd_Array.GetAt(index +i));
		}

		m_titlelist.Add("起始时间");
		m_strlist.Add(start);

		m_titlelist.Add("终止时间");
		m_strlist.Add(end);

		CString str;
		m_titlelist.Add("切片总数");
		str.Format("%d", qps_sum);
		m_strlist.Add(str);

		m_titlelist.Add("任务来源");
		m_strlist.Add(rwly);

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
	lkh_Array.RemoveAll();
	qpsm_Array.RemoveAll();
	qptmh_Array.RemoveAll();
	qpczy_Array.RemoveAll();
	bphd_Array.RemoveAll();

	m_strlist.RemoveAll();
	m_titlelist.RemoveAll();
}

void CQpyj::OnButtonAll() 
{
	((CButton *)GetDlgItem(IDC_CHECK_ZC))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_BD))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_TG))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_CQ))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_SQ))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_BQ))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_MYZH))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_FZBL))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_DJ))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_TSRS))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_BAOQIE))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_LQ))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_BS))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_BY))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_QQ))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_BXLQ))->SetCheck(1);
}
