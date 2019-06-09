// Gzddy.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "Gzddy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGzddy dialog
extern CPathologyApp theApp;

CGzddy::CGzddy(CWnd* pParent /*=NULL*/)
	: CPrintPreviewDialog(CGzddy::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGzddy)
	m_zxzt = 0;
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_STATISTIC);
}


void CGzddy::DoDataExchange(CDataExchange* pDX)
{
	CPrintPreviewDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGzddy)
	DDX_Radio(pDX, IDC_RADIO_ZXZT1, m_zxzt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGzddy, CPrintPreviewDialog)
	//{{AFX_MSG_MAP(CGzddy)
	ON_COMMAND(ID_LOC_PRINT, OnLocPrint)
	ON_COMMAND(ID_LOC_PRINT_PREVIEW, OnLocPrintPreview)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_CBN_SELCHANGE(IDC_COMBO_TIMEMODE, OnSelchangeComboTimemode)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE( TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
    ON_NOTIFY_EX_RANGE( TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGzddy message handlers

BOOL CGzddy::OnInitDialog() 
{
	CPrintPreviewDialog::OnInitDialog();
	
	m_UsageType = 2;
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

	((CComboBox *)GetDlgItem(IDC_COMBO_TIMEMODE))->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HCURSOR CGzddy::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CGzddy::OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult )
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
    return CGzddy::OnToolTipText(nID,pNMHDR,pResult);
}

BOOL CGzddy::GetToolText( UINT nID, CString& strTipText, CString& /*strMessage*/)
{
    switch( nID )
    {
		case ID_LOC_PRINT_PREVIEW:
			strTipText.Format("医嘱工作单打印预览");
			break;
		case ID_LOC_PRINT:
			strTipText.Format("打印医嘱工作单");
			break;
    }
       
    return TRUE;
}

void CGzddy::GenList() 
{
}

void CGzddy::OnLocPrint() 
{
	UpdateData(TRUE);
	
	CString yzlxstr; yzlxstr.Empty();
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX1))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '重切'";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX2))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '深切'";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX3))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '补取'";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX4))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '免疫组化'";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX5))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '特殊染色'";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX6))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '分子病理'";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX7))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '电镜'";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX8))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '薄切'";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX9))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '连切'";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX10))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '常规'";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX11))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '切全'";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX12))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '不削连切'";
	}

	if(yzlxstr.IsEmpty())  
	{
		AfxMessageBox("没有指定医嘱类型!");
		return;
	}

	theApp.ConnectDatabase();

	int qps_sum = 0;

	COleDateTime m_time;
	m_Time_Start.GetTime(m_time);
	CString start = m_time.Format("%Y-%m-%d %H:%M");
	
	m_Time_End.GetTime(m_time);
	CString end   = m_time.Format("%Y-%m-%d %H:%M");
	
	CString m_WhereBetween_Cmd;

	if(0 == ((CComboBox *)GetDlgItem(IDC_COMBO_TIMEMODE))->GetCurSel())
	{
		if(yzlxstr.IsEmpty())
			m_WhereBetween_Cmd.Format(" where sqrq >= '%s' and sqrq <= '%s'", start, end);
		else
			m_WhereBetween_Cmd.Format(" where sqrq >= '%s' and sqrq <= '%s' and (%s)", start, end, yzlxstr);

		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			if(yzlxstr.IsEmpty())
				m_WhereBetween_Cmd.Format(" where sqrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and sqrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", start, end);
			else
				m_WhereBetween_Cmd.Format(" where sqrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and sqrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss') and (%s)", start, end, yzlxstr);
		}
	}
	else
	{
		if(yzlxstr.IsEmpty())
			m_WhereBetween_Cmd.Format(" where zxrq >= '%s' and zxrq <= '%s'", start, end);
		else
			m_WhereBetween_Cmd.Format(" where zxrq >= '%s' and zxrq <= '%s' and (%s)", start, end, yzlxstr);

		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			if(yzlxstr.IsEmpty())
				m_WhereBetween_Cmd.Format(" where zxrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and zxrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", start, end);
			else
				m_WhereBetween_Cmd.Format(" where zxrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and zxrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss') and (%s)", start, end, yzlxstr);
		}
	}

	switch(m_zxzt) {
	case 0:
		m_WhereBetween_Cmd += " and yzzt = 0";
		break;
	case 1:
		m_WhereBetween_Cmd += " and yzzt = 1";
		break;
	case 2:
		break;
	}

	try
	{
		SAString cmdstr = "Select sum(qps) from " + theApp.TABLE_ADVICE + m_WhereBetween_Cmd;
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

	CStringArray sqys_list;        sqys_list.RemoveAll();
	CStringArray medicalno_list;   medicalno_list.RemoveAll();
	CStringArray yzlx_list;        yzlx_list.RemoveAll();
	CStringArray lkh_list;         lkh_list.RemoveAll();

	try
	{
		SAString cmdstr = "Select sqys, medicalno, yzlx, lkh from " + theApp.TABLE_ADVICE + m_WhereBetween_Cmd + " group by sqys,medicalno,yzlx,lkh order by sqys asc, medicalno asc";
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();

		while ( g_dbcommand.FetchNext() )
		{
			sqys_list.Add(g_dbcommand.Field("sqys").asString());
			medicalno_list.Add(g_dbcommand.Field("medicalno").asString());
			yzlx_list.Add(g_dbcommand.Field("yzlx").asString());
			lkh_list.Add(g_dbcommand.Field("lkh").asString());
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

	CStringArray medicalno_Array;   medicalno_Array.RemoveAll();
	CStringArray yzlx_Array;        yzlx_Array.RemoveAll();
	CStringArray lkh_Array;         lkh_Array.RemoveAll();
	CStringArray bjw_Array;         bjw_Array.RemoveAll();
	CUIntArray   qps_Array;         qps_Array.RemoveAll();
	CStringArray sqys_Array;        sqys_Array.RemoveAll();
	CStringArray sqrq_Array;        sqrq_Array.RemoveAll();
	CStringArray bz_Array;          bz_Array.RemoveAll();
	CStringArray xm_Array;          xm_Array.RemoveAll();
	CStringArray brid_Array;        brid_Array.RemoveAll();
	CStringArray zyh_Array;         zyh_Array.RemoveAll();
	CStringArray mzh_Array;         mzh_Array.RemoveAll();

	for(int index = 0; index < sqys_list.GetSize(); index++)
	{
		try
		{
			SAString cmdstr = "Select bjw,qps,sqrq,bz from " + theApp.TABLE_ADVICE + m_WhereBetween_Cmd + " and sqys=:sqys and medicalno=:medicalno and yzlx=:yzlx and lkh=:lkh";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("sqys").setAsString()      = sqys_list.GetAt(index);
			g_dbcommand.Param("medicalno").setAsString() = medicalno_list.GetAt(index);
			g_dbcommand.Param("yzlx").setAsString()      = yzlx_list.GetAt(index);
			g_dbcommand.Param("lkh").setAsString()       = lkh_list.GetAt(index);
			
			g_dbcommand.Execute();

			if(yzlx_list.GetAt(index).CompareNoCase("免疫组化") == 0 ||
			   yzlx_list.GetAt(index).CompareNoCase("特殊染色") == 0 ||
			   yzlx_list.GetAt(index).CompareNoCase("分子病理") == 0 ||
			   yzlx_list.GetAt(index).CompareNoCase("电镜")     == 0 )
			{
				CString bjwstr, bzstr;  bjwstr.Empty();  bzstr.Empty();
				int qps = 0;
				CString sqrqstr;
				while ( g_dbcommand.FetchNext() )
				{
					if(!bjwstr.IsEmpty())  bjwstr += ",";
					bjwstr += g_dbcommand.Field("bjw").asString();
					qps    += g_dbcommand.Field("qps").asLong();

/*
					if(!g_dbcommand.Field("bz").asString().IsEmpty())
					{
						if(!bzstr.IsEmpty())   bzstr += ";";
						bzstr += g_dbcommand.Field("bz").asString();
					}
*/
					if(bzstr.IsEmpty() && !g_dbcommand.Field("bz").asString().IsEmpty())
					{
						bzstr = g_dbcommand.Field("bz").asString();
					}
					
					sqrqstr.Format("%d-%02d-%02d %02d:%02d", g_dbcommand.Field("sqrq").asDateTime().GetYear(), g_dbcommand.Field("sqrq").asDateTime().GetMonth(), g_dbcommand.Field("sqrq").asDateTime().GetDay(), g_dbcommand.Field("sqrq").asDateTime().GetHour(), g_dbcommand.Field("sqrq").asDateTime().GetMinute());
				}

				medicalno_Array.Add(medicalno_list.GetAt(index));
				yzlx_Array.Add(yzlx_list.GetAt(index));
				lkh_Array.Add(lkh_list.GetAt(index));
				bjw_Array.Add(bjwstr);
				qps_Array.Add(qps);
				sqys_Array.Add(sqys_list.GetAt(index));
				sqrq_Array.Add(sqrqstr);
				bz_Array.Add(bzstr);
			}
			else
			{
				while ( g_dbcommand.FetchNext() )
				{
					medicalno_Array.Add(medicalno_list.GetAt(index));
					yzlx_Array.Add(yzlx_list.GetAt(index));
					lkh_Array.Add(lkh_list.GetAt(index));
					bjw_Array.Add(g_dbcommand.Field("bjw").asString());
					qps_Array.Add(g_dbcommand.Field("qps").asLong());
					sqys_Array.Add(sqys_list.GetAt(index));
					
					CString FieldStr;
					FieldStr.Format("%d-%02d-%02d %02d:%02d", g_dbcommand.Field("sqrq").asDateTime().GetYear(), g_dbcommand.Field("sqrq").asDateTime().GetMonth(), g_dbcommand.Field("sqrq").asDateTime().GetDay(), g_dbcommand.Field("sqrq").asDateTime().GetHour(), g_dbcommand.Field("sqrq").asDateTime().GetMinute());
					sqrq_Array.Add(FieldStr);
					bz_Array.Add(g_dbcommand.Field("bz").asString());
				}
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
	}

	sqys_list.RemoveAll();
	medicalno_list.RemoveAll();
	yzlx_list.RemoveAll();
	lkh_list.RemoveAll();

	CString TjFmtName;
    TjFmtName.Format("医嘱工作单");

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
		yzlx_Array.RemoveAll();
		lkh_Array.RemoveAll();
		bjw_Array.RemoveAll();
		qps_Array.RemoveAll();
		sqys_Array.RemoveAll();
		sqrq_Array.RemoveAll();
		bz_Array.RemoveAll();
		xm_Array.RemoveAll();
		brid_Array.RemoveAll();
		zyh_Array.RemoveAll();
		mzh_Array.RemoveAll();
		return;
	}

	//---------------------------------------------------------
	for(index = 0; index < medicalno_Array.GetSize(); index++)
	{
		try
		{
			SAString cmdstr = "Select *  from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("medicalno").setAsString() = medicalno_Array.GetAt(index);
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				xm_Array.Add(g_dbcommand.Field("xm").asString());
				brid_Array.Add(g_dbcommand.Field("brid").asString());
				zyh_Array.Add(g_dbcommand.Field("zyh").asString());
				mzh_Array.Add(g_dbcommand.Field("mzh").asString());
			}
			else
			{
				xm_Array.Add("");
				brid_Array.Add("");
				zyh_Array.Add("");
				mzh_Array.Add("");
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
	//---------------------------------------------------------

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

	for(index = 0; index < totalnum; index += RecNum)
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
			m_strlist.Add(yzlx_Array.GetAt(index +i));

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(lkh_Array.GetAt(index +i));
			
			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(bjw_Array.GetAt(index +i));
			
			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			str.Format("%d", qps_Array.GetAt(index +i));
			m_strlist.Add(str);

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(sqys_Array.GetAt(index +i));

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(sqrq_Array.GetAt(index +i));

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(bz_Array.GetAt(index +i));

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(xm_Array.GetAt(index +i));

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(brid_Array.GetAt(index +i));

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(zyh_Array.GetAt(index +i));

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(mzh_Array.GetAt(index +i));
		}

		m_titlelist.Add("起始时间");
		m_strlist.Add(start);

		m_titlelist.Add("终止时间");
		m_strlist.Add(end);

		CString str;
		m_titlelist.Add("医嘱片数");
		str.Format("%d", qps_sum);
		m_strlist.Add(str);

		m_titlelist.Add("页码");
		str.Format("%d", index/RecNum + 1);
		m_strlist.Add(str);

		m_titlelist.Add("时间类型");
		if(0 == ((CComboBox *)GetDlgItem(IDC_COMBO_TIMEMODE))->GetCurSel())
			m_strlist.Add("申清时间");
		else
			m_strlist.Add("执行时间");

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
	yzlx_Array.RemoveAll();
	lkh_Array.RemoveAll();
	bjw_Array.RemoveAll();
	qps_Array.RemoveAll();
	sqys_Array.RemoveAll();
	sqrq_Array.RemoveAll();
	bz_Array.RemoveAll();
	xm_Array.RemoveAll();
	brid_Array.RemoveAll();
	zyh_Array.RemoveAll();
	mzh_Array.RemoveAll();

	m_strlist.RemoveAll();
	m_titlelist.RemoveAll();
}

void CGzddy::OnLocPrintPreview() 
{
	UpdateData(TRUE);
	
	CString yzlxstr; yzlxstr.Empty();
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX1))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '重切'";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX2))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '深切'";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX3))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '补取'";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX4))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '免疫组化'";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX5))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '特殊染色'";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX6))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '分子病理'";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX7))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '电镜'";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX8))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '薄切'";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX9))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '连切'";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX10))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '常规'";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX11))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '切全'";
	}
	if(((CButton *)GetDlgItem(IDC_CHECK_YZLX12))->GetState() & 0x0003)
	{
		if(!yzlxstr.IsEmpty()) yzlxstr += " or ";
		yzlxstr += "yzlx = '不削连切'";
	}

	if(yzlxstr.IsEmpty())  
	{
		AfxMessageBox("没有指定医嘱类型!");
		return;
	}

	theApp.ConnectDatabase();

	int qps_sum = 0;

	COleDateTime m_time;
	m_Time_Start.GetTime(m_time);
	CString start = m_time.Format("%Y-%m-%d %H:%M");
	
	m_Time_End.GetTime(m_time);
	CString end   = m_time.Format("%Y-%m-%d %H:%M");
	
	CString m_WhereBetween_Cmd;

	if(0 == ((CComboBox *)GetDlgItem(IDC_COMBO_TIMEMODE))->GetCurSel())
	{
		if(yzlxstr.IsEmpty())
			m_WhereBetween_Cmd.Format(" where sqrq >= '%s' and sqrq <= '%s'", start, end);
		else
			m_WhereBetween_Cmd.Format(" where sqrq >= '%s' and sqrq <= '%s' and (%s)", start, end, yzlxstr);

		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			if(yzlxstr.IsEmpty())
				m_WhereBetween_Cmd.Format(" where sqrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and sqrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", start, end);
			else
				m_WhereBetween_Cmd.Format(" where sqrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and sqrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss') and (%s)", start, end, yzlxstr);
		}
	}
	else
	{
		if(yzlxstr.IsEmpty())
			m_WhereBetween_Cmd.Format(" where zxrq >= '%s' and zxrq <= '%s'", start, end);
		else
			m_WhereBetween_Cmd.Format(" where zxrq >= '%s' and zxrq <= '%s' and (%s)", start, end, yzlxstr);

		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			if(yzlxstr.IsEmpty())
				m_WhereBetween_Cmd.Format(" where zxrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and zxrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", start, end);
			else
				m_WhereBetween_Cmd.Format(" where zxrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and zxrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss') and (%s)", start, end, yzlxstr);
		}
	}

	switch(m_zxzt) {
	case 0:
		m_WhereBetween_Cmd += " and yzzt = 0";
		break;
	case 1:
		m_WhereBetween_Cmd += " and yzzt = 1";
		break;
	case 2:
		break;
	}

	try
	{
		SAString cmdstr = "Select sum(qps) from " + theApp.TABLE_ADVICE + m_WhereBetween_Cmd;
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

	CStringArray sqys_list;        sqys_list.RemoveAll();
	CStringArray medicalno_list;   medicalno_list.RemoveAll();
	CStringArray yzlx_list;        yzlx_list.RemoveAll();
	CStringArray lkh_list;         lkh_list.RemoveAll();

	try
	{
		SAString cmdstr = "Select sqys, medicalno, yzlx, lkh from " + theApp.TABLE_ADVICE + m_WhereBetween_Cmd + " group by sqys,medicalno,yzlx,lkh order by sqys asc, medicalno asc";
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();

		while ( g_dbcommand.FetchNext() )
		{
			sqys_list.Add(g_dbcommand.Field("sqys").asString());
			medicalno_list.Add(g_dbcommand.Field("medicalno").asString());
			yzlx_list.Add(g_dbcommand.Field("yzlx").asString());
			lkh_list.Add(g_dbcommand.Field("lkh").asString());
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

	CStringArray medicalno_Array;   medicalno_Array.RemoveAll();
	CStringArray yzlx_Array;        yzlx_Array.RemoveAll();
	CStringArray lkh_Array;         lkh_Array.RemoveAll();
	CStringArray bjw_Array;         bjw_Array.RemoveAll();
	CUIntArray   qps_Array;         qps_Array.RemoveAll();
	CStringArray sqys_Array;        sqys_Array.RemoveAll();
	CStringArray sqrq_Array;        sqrq_Array.RemoveAll();
	CStringArray bz_Array;          bz_Array.RemoveAll();
	CStringArray xm_Array;          xm_Array.RemoveAll();
	CStringArray brid_Array;        brid_Array.RemoveAll();
	CStringArray zyh_Array;         zyh_Array.RemoveAll();
	CStringArray mzh_Array;         mzh_Array.RemoveAll();
	CStringArray sjks_Array;		sjks_Array.RemoveAll();
	CStringArray xb_Array;			xb_Array.RemoveAll();
	CStringArray nl_Array;			nl_Array.RemoveAll();

	for(int index = 0; index < sqys_list.GetSize(); index++)
	{
		try
		{
			SAString cmdstr = "Select bjw,qps,sqrq,bz from " + theApp.TABLE_ADVICE + m_WhereBetween_Cmd + " and sqys=:sqys and medicalno=:medicalno and yzlx=:yzlx and lkh=:lkh";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("sqys").setAsString()      = sqys_list.GetAt(index);
			g_dbcommand.Param("medicalno").setAsString() = medicalno_list.GetAt(index);
			g_dbcommand.Param("yzlx").setAsString()      = yzlx_list.GetAt(index);
			g_dbcommand.Param("lkh").setAsString()       = lkh_list.GetAt(index);
			
			g_dbcommand.Execute();

			if(yzlx_list.GetAt(index).CompareNoCase("免疫组化") == 0 ||
			   yzlx_list.GetAt(index).CompareNoCase("特殊染色") == 0 ||
			   yzlx_list.GetAt(index).CompareNoCase("分子病理") == 0 ||
			   yzlx_list.GetAt(index).CompareNoCase("电镜")     == 0 )
			{
				CString bjwstr, bzstr;  bjwstr.Empty();  bzstr.Empty();
				int qps = 0;
				CString sqrqstr;
				while ( g_dbcommand.FetchNext() )
				{
					if(!bjwstr.IsEmpty())  bjwstr += ",";
					bjwstr += g_dbcommand.Field("bjw").asString();
					qps    += g_dbcommand.Field("qps").asLong();

/*
					if(!g_dbcommand.Field("bz").asString().IsEmpty())
					{
						if(!bzstr.IsEmpty())   bzstr += ";";
						bzstr += g_dbcommand.Field("bz").asString();
					}
*/
					if(bzstr.IsEmpty() && !g_dbcommand.Field("bz").asString().IsEmpty())
					{
						bzstr = g_dbcommand.Field("bz").asString();
					}
					
					sqrqstr.Format("%d-%02d-%02d %02d:%02d", g_dbcommand.Field("sqrq").asDateTime().GetYear(), g_dbcommand.Field("sqrq").asDateTime().GetMonth(), g_dbcommand.Field("sqrq").asDateTime().GetDay(), g_dbcommand.Field("sqrq").asDateTime().GetHour(), g_dbcommand.Field("sqrq").asDateTime().GetMinute());
				}

				medicalno_Array.Add(medicalno_list.GetAt(index));
				yzlx_Array.Add(yzlx_list.GetAt(index));
				lkh_Array.Add(lkh_list.GetAt(index));
				bjw_Array.Add(bjwstr);
				qps_Array.Add(qps);
				sqys_Array.Add(sqys_list.GetAt(index));
				sqrq_Array.Add(sqrqstr);
				bz_Array.Add(bzstr);
			}
			else
			{
				while ( g_dbcommand.FetchNext() )
				{
					medicalno_Array.Add(medicalno_list.GetAt(index));
					yzlx_Array.Add(yzlx_list.GetAt(index));
					lkh_Array.Add(lkh_list.GetAt(index));
					bjw_Array.Add(g_dbcommand.Field("bjw").asString());
					qps_Array.Add(g_dbcommand.Field("qps").asLong());
					sqys_Array.Add(sqys_list.GetAt(index));
					
					CString FieldStr;
					FieldStr.Format("%d-%02d-%02d %02d:%02d", g_dbcommand.Field("sqrq").asDateTime().GetYear(), g_dbcommand.Field("sqrq").asDateTime().GetMonth(), g_dbcommand.Field("sqrq").asDateTime().GetDay(), g_dbcommand.Field("sqrq").asDateTime().GetHour(), g_dbcommand.Field("sqrq").asDateTime().GetMinute());
					sqrq_Array.Add(FieldStr);
					bz_Array.Add(g_dbcommand.Field("bz").asString());
				}
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
	}

	sqys_list.RemoveAll();
	medicalno_list.RemoveAll();
	yzlx_list.RemoveAll();
	lkh_list.RemoveAll();

	CString TjFmtName;
    TjFmtName.Format("医嘱工作单");

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
		yzlx_Array.RemoveAll();
		lkh_Array.RemoveAll();
		bjw_Array.RemoveAll();
		qps_Array.RemoveAll();
		sqys_Array.RemoveAll();
		sqrq_Array.RemoveAll();
		bz_Array.RemoveAll();
		xm_Array.RemoveAll();
		brid_Array.RemoveAll();
		zyh_Array.RemoveAll();
		mzh_Array.RemoveAll();
		return;
	}

	//---------------------------------------------------------
	for(index = 0; index < medicalno_Array.GetSize(); index++)
	{
		try
		{
			SAString cmdstr = "Select *  from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("medicalno").setAsString() = medicalno_Array.GetAt(index);
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				xm_Array.Add(g_dbcommand.Field("xm").asString());
				brid_Array.Add(g_dbcommand.Field("brid").asString());
				zyh_Array.Add(g_dbcommand.Field("zyh").asString());
				mzh_Array.Add(g_dbcommand.Field("mzh").asString());
				sjks_Array.Add(g_dbcommand.Field("sjks").asString());
				xb_Array.Add(g_dbcommand.Field("xb").asString());
				nl_Array.Add(g_dbcommand.Field("nl").asString());
			}
			else
			{
				xm_Array.Add("");
				brid_Array.Add("");
				zyh_Array.Add("");
				mzh_Array.Add("");
				sjks_Array.Add("");
				xb_Array.Add("");
				nl_Array.Add("");
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
	//---------------------------------------------------------

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

	for(index = 0; index < totalnum; index += RecNum)
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
			m_strlist.Add(yzlx_Array.GetAt(index +i));

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(lkh_Array.GetAt(index +i));
			
			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(bjw_Array.GetAt(index +i));
			
			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			str.Format("%d", qps_Array.GetAt(index +i));
			m_strlist.Add(str);

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(sqys_Array.GetAt(index +i));

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(sqrq_Array.GetAt(index +i));

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(bz_Array.GetAt(index +i));

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(xm_Array.GetAt(index +i));

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(brid_Array.GetAt(index +i));

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(zyh_Array.GetAt(index +i));
			
			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(mzh_Array.GetAt(index +i));

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(sjks_Array.GetAt(index +i));

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(xb_Array.GetAt(index +i));

			str.Format("TABLE_%d_%d",i+1,m++);
			m_titlelist.Add(str);
			m_strlist.Add(nl_Array.GetAt(index +i));
		}

		m_titlelist.Add("起始时间");
		m_strlist.Add(start);

		m_titlelist.Add("终止时间");
		m_strlist.Add(end);

		CString str;
		m_titlelist.Add("医嘱片数");
		str.Format("%d", qps_sum);
		m_strlist.Add(str);

		m_titlelist.Add("页码");
		str.Format("%d", index/RecNum + 1);
		m_strlist.Add(str);

		m_titlelist.Add("时间类型");
		if(0 == ((CComboBox *)GetDlgItem(IDC_COMBO_TIMEMODE))->GetCurSel())
			m_strlist.Add("申清时间");
		else
			m_strlist.Add("执行时间");

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
	yzlx_Array.RemoveAll();
	lkh_Array.RemoveAll();
	bjw_Array.RemoveAll();
	qps_Array.RemoveAll();
	sqys_Array.RemoveAll();
	sqrq_Array.RemoveAll();
	bz_Array.RemoveAll();
	xm_Array.RemoveAll();
	brid_Array.RemoveAll();
	zyh_Array.RemoveAll();
	mzh_Array.RemoveAll();

	m_strlist.RemoveAll();
	m_titlelist.RemoveAll();
}

void CGzddy::OnButtonAll() 
{
	((CButton *)GetDlgItem(IDC_CHECK_YZLX1))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_YZLX2))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_YZLX3))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_YZLX4))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_YZLX5))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_YZLX6))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_YZLX7))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_YZLX8))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_YZLX9))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_YZLX10))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_YZLX11))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_CHECK_YZLX12))->SetCheck(1);
}

void CGzddy::OnSelchangeComboTimemode() 
{
	if(1 == ((CComboBox *)GetDlgItem(IDC_COMBO_TIMEMODE))->GetCurSel())
	{
		m_zxzt = 1;
		UpdateData(FALSE);
	}
}
