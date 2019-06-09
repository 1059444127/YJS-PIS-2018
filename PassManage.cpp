// PassManage.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "PassManage.h"
#include "NewUser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPassManage dialog
extern CPathologyApp theApp;

CPassManage::CPassManage(CWnd* pParent /*=NULL*/)
	: CXTCBarDialog(CPassManage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPassManage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_SECURITY);
}


void CPassManage::DoDataExchange(CDataExchange* pDX)
{
	CXTCBarDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPassManage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPassManage, CXTCBarDialog)
	//{{AFX_MSG_MAP(CPassManage)
	ON_COMMAND(ID_USER_GEN, OnUserGen)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_USER_DELETE, OnUserDelete)
	ON_COMMAND(ID_USER_MODIFY, OnUserModify)
	ON_COMMAND(ID_USER_1, OnUser1)
	ON_COMMAND(ID_USER_2, OnUser2)
	ON_COMMAND(ID_USER_3, OnUser3)
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_USER_4, OnUser4)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE( TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
    ON_NOTIFY_EX_RANGE( TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPassManage message handlers

BOOL CPassManage::OnInitDialog() 
{
	CXTCBarDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),AFX_IDW_TOOLBAR+24)||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_PSMANAGE))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;      // fail to create
	}
	
	CRect rect;
	GetClientRect(&rect);
	CRect rt = rect;
	rt.bottom -= 30;
	rt.right  -= (rect.Width() * 1.0 / 3 + 5);
    if(!m_UserCtrl.CreateGrid(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rt,this,111))
	{
		TRACE0( "Failed to create m_UserCtrl.\n" );
		return FALSE;
	}

	rt = rect;
	rt.bottom -= 30;
	rt.left   += (rect.Width() * 2.0 / 3 + 5);
    if(!m_RightCtrl.CreateGrid(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rt,this,112))
	{
		TRACE0( "Failed to create m_RightCtrl.\n" );
		return FALSE;
	}

    SetResize(IDOK, SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);

	// Move control bars to the correct position.
    RecalcBarLayout();

	m_UserCtrl.GridSetup();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPassManage::OnSize(UINT nType, int cx, int cy) 
{
	CXTCBarDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	int leftwidth = cx * 2.0 / 3-5;
	::MoveWindow(m_UserCtrl.m_hWnd,0,30,leftwidth,cy-60,TRUE);
	leftwidth -= 45;
	leftwidth /= 3;
    m_UserCtrl.SetColWidth(0,leftwidth);
    m_UserCtrl.SetColWidth(1,leftwidth);
    m_UserCtrl.SetColWidth(2,0);
    m_UserCtrl.SetColWidth(3,leftwidth);

	::MoveWindow(m_RightCtrl.m_hWnd,cx * 2.0 / 3 + 5,30,cx * 1.0 / 3 - 5,cy-60,TRUE);
	m_RightCtrl.SetColWidth(0,cx * 1.0 / 3 - 5 - 20);
}

void CPassManage::OnUserGen() 
{
	// TODO: Add your command handler code here
	CNewUser mydlg(this);
	if( mydlg.DoModal() == IDCANCEL )  return;
	mydlg.m_name.TrimLeft();
    mydlg.m_name.TrimRight();
	mydlg.m_info.TrimLeft();
    mydlg.m_info.TrimRight();

	theApp.ConnectDatabase();
	
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_SECURITY + " where username = :username";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("username").setAsString() = mydlg.m_name;
		
		g_dbcommand.Execute();
		
		g_dbcommand.FetchNext();
		int recordnum = g_dbcommand.Field(1).asLong();
		
		g_dbconnection.Commit();
		
		if( recordnum > 0 )
		{
			AfxMessageBox("该用户名的帐户已经存在!", MB_OK | MB_ICONSTOP);
			return;
		}
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
	
	try
	{
		SAString cmdstr = "insert into " + theApp.TABLE_SECURITY + " (username, password, userright,userinfo) values (:username, :password, :userright,:userinfo)";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("username").setAsString() = mydlg.m_name;
		g_dbcommand.Param("password").setAsString() = "123456";
		g_dbcommand.Param("userright").setAsLong()  = 0;
		g_dbcommand.Param("userinfo").setAsString() = mydlg.m_info;
		
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
		AfxMessageBox("新用户帐号创建失败！");
		return;
	}

	m_UserCtrl.GridSetup();
	
	if(m_UserCtrl.GetNumberRows())
		m_UserCtrl.GotoRow(m_UserCtrl.GetNumberRows() - 1);
}

void CPassManage::OnDestroy() 
{
	CXTCBarDialog::OnDestroy();
	
	// TODO: Add your message handler code here
}

void CPassManage::OnUserDelete() 
{
	// TODO: Add your command handler code here
	int row = m_UserCtrl.GetCurrentRow();
	int CurRecordNum = m_UserCtrl.GetNumberRows();
	
	if( CurRecordNum > 0 && row >= 0 && row < CurRecordNum )
	{
		CString username;
		CUGCell  cell;
		m_UserCtrl.GetCellIndirect(0 , row, &cell);
		cell.GetText(&username);

		theApp.ConnectDatabase();
		
		try
		{
			SAString cmdstr = "delete from " + theApp.TABLE_SECURITY + " where username = :username";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("username").setAsString() = username;
			
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

		m_UserCtrl.GridSetup();
	}
}

void CPassManage::OnUserModify() 
{
	// TODO: Add your command handler code here
	int row = m_UserCtrl.GetCurrentRow();
	int CurRecordNum = m_UserCtrl.GetNumberRows();
	if( CurRecordNum > 0 && row >= 0 && row < CurRecordNum )
	{
		CNewUser mydlg(this);
		mydlg.m_status = 1;

		CUGCell  cell;
		m_UserCtrl.GetCellIndirect(0 , row, &cell);
		cell.GetText(&mydlg.m_name);

		theApp.ConnectDatabase();
		
		try
		{
			SAString cmdstr = "Select userinfo from " + theApp.TABLE_SECURITY + " where username = :username";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("username").setAsString() = mydlg.m_name;
			
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				mydlg.m_info  = g_dbcommand.Field("userinfo").asString();
			}
			else
			{
				AfxMessageBox("没有找到该用户名的帐户!", MB_OK | MB_ICONSTOP);
				return;
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

		if( mydlg.DoModal() == IDCANCEL )  return;
		mydlg.m_info.TrimLeft();
		mydlg.m_info.TrimRight();

		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_SECURITY + " set userinfo = :userinfo where username = :username";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("username").setAsString() = mydlg.m_name;
			g_dbcommand.Param("userinfo").setAsString() = mydlg.m_info;
			
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

		m_UserCtrl.QuickSetText(3,row,mydlg.m_info);
		m_UserCtrl.RedrawRow(row);
	}
}

BOOL CPassManage::OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult )
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
    return CPassManage::OnToolTipText(nID,pNMHDR,pResult);

}

/////////////////////////////////////////////////////////////////////////////
// CToolTipView message handlers
/////////////////////////////////////////////////////////////////////////////////
//
BOOL CPassManage::GetToolText( UINT nID, CString& strTipText, CString& /*strMessage*/)
{
    CString strFullString;
  
	switch( nID )
	{
	case ID_USER_GEN:
		strTipText.Format("创建新用户");
		break;
	case ID_USER_DELETE:
		strTipText.Format("删除用户");
		break;
	case ID_USER_MODIFY:
		strTipText.Format("用户信息修改");
		break;
	case ID_USER_1:
		strTipText.Format("初诊权限");
		break;
	case ID_USER_2:
		strTipText.Format("主诊权限");
		break;
	case ID_USER_3:
		strTipText.Format("审核权限");
		break;
	case ID_USER_4:
		strTipText.Format("主任权限");
		break;
	}
    return TRUE;
}

void CPassManage::PreSetValue(int value)
{
	int index = m_UserCtrl.GetCurrentRow();
	if(index >= 0 && index < m_UserCtrl.GetNumberRows())
	{
		CUGCell  cell;
		CString username;
		m_UserCtrl.GetCellIndirect(0 , index, &cell);
		cell.GetText(&username);
	
		//-------------------------------------------------
		CString str = username;
		str.MakeLower();
		if(str.CompareNoCase("administrator") == 0)
		{
			return;
		}
		//-------------------------------------------------

		theApp.ConnectDatabase();
		
		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_SECURITY + " set userright = :userright where username = :username";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("username").setAsString() = username;
			g_dbcommand.Param("userright").setAsLong()  = value;
			
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
		
		//CString str;
		str.Format("%d", value);
		m_UserCtrl.QuickSetText(2,index,str);
		m_UserCtrl.RedrawCell(2,index);

		m_UserCtrl.OnRowChange(index,index);
	}
}

void CPassManage::OnUser1() 
{
	PreSetValue(0);
}

void CPassManage::OnUser2() 
{
	PreSetValue(0);
}

void CPassManage::OnUser3() 
{
	PreSetValue(0);
}

void CPassManage::OnUser4() 
{
	PreSetValue(0);
}

HCURSOR CPassManage::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

