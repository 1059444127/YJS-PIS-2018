// Security.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "Security.h"
#include "PassModify.h"
#include "PassManage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSecurity dialog
extern CPathologyApp theApp;

CSecurity::CSecurity(CWnd* pParent /*=NULL*/)
	: CDialog(CSecurity::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSecurity)
	m_password = _T("");
	m_user = _T("");
	//}}AFX_DATA_INIT

	m_TimerHandle_VeriDicom = 0;
	m_pDisplayImageData = NULL;
}


void CSecurity::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSecurity)
	DDX_Control(pDX, IDC_ENROLL, m_enroll);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
	DDV_MaxChars(pDX, m_password, 255);
	DDX_CBString(pDX, IDC_COMBO_USER, m_user);
	DDV_MaxChars(pDX, m_user, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSecurity, CDialog)
	//{{AFX_MSG_MAP(CSecurity)
	ON_BN_CLICKED(IDPASS, OnPass)
	ON_BN_CLICKED(IDMANAGE, OnManage)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_ENROLL, OnEnroll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSecurity message handlers

BOOL CSecurity::OnInitDialog() 
{
	CDialog::OnInitDialog();

	theApp.m_Cpp_Right = 0;

	theApp.IsLogOut = true;
	
	const DWORD STYLE_ALL = BS_LEFT | BS_RIGHT | BS_CENTER;
	m_enroll.SetIcon(CSize(16,16),IDI_ICON_ENROLL);
	m_enroll.SetXButtonStyle(BS_XT_XPFLAT); 
    m_enroll.ModifyStyle(STYLE_ALL & ~BS_CENTER, BS_CENTER);

	//----------------------------------------------------
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP_SECURITY);
	
	CImageList imageList;
	imageList.Create(32, 32, ILC_COLOR24|ILC_MASK, 3, 1);
	imageList.Add(&bitmap, RGB(236,233,216));
	
	((CStatic *)GetDlgItem(IDC_STATIC_DATABASE))->SetIcon(imageList.ExtractIcon(0));
	///((CStatic *)GetDlgItem(IDC_STATIC_PERSON))->SetIcon(imageList.ExtractIcon(1));
	///((CStatic *)GetDlgItem(IDC_STATIC_KEY))->SetIcon(imageList.ExtractIcon(2));
	//----------------------------------------------------
	if(false == theApp.ConnectDatabase())
	{
		AfxMessageBox("数据库连接错误！");
		EndDialog(IDCANCEL);
		return FALSE;
	}

	theApp.m_DBName_list.RemoveAll();
	theApp.m_DBNo_list.RemoveAll();

	try
	{
		SAString cmdstr = "Select * from " + theApp.TABLE_DBTYPE + " order by dbno";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Execute();
		
		while( g_dbcommand.FetchNext() )
		{
			theApp.m_DBName_list.Add(g_dbcommand.Field("dbname").asString());
			theApp.m_DBNo_list.Add(g_dbcommand.Field("dbno").asLong());
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

	((CComboBox *)GetDlgItem(IDC_COMBO_DB))->Clear();

	for(int i = 0; i < theApp.m_DBName_list.GetSize(); i++)
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_DB))->AddString(theApp.m_DBName_list.GetAt(i));
	}

	int rt = ((CComboBox *)GetDlgItem(IDC_COMBO_DB))->FindStringExact(-1, m_DBName);
	if(rt != CB_ERR)
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_DB))->SetCurSel(rt);
	}
	else
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_DB))->SetCurSel(0);
	}
	//----------------------------------------------------

	CStdioFile File;
	if ( File.Open(theApp.m_Exe_Working_Dir + "system\\UserRecent.txt", CFile::modeRead | CFile::typeText) )
	{
		CString tmp;
		while(1)
		{
			if( File.ReadString(tmp) == FALSE ) break;
			tmp.TrimLeft();
			tmp.TrimRight();
			if( tmp.IsEmpty() )   continue;
            m_UserNameList.Add(tmp);
		}
		File.Close();
	}

	//-----------------------------------------------------
	try
	{
		g_dbcommand.setCommandText("Select username from " + (SAString)theApp.TABLE_SECURITY);
		g_dbcommand.Execute();
		
		while( g_dbcommand.FetchNext() )
		{
			CString name = g_dbcommand.Field("username").asString();

			for(int i = 0; i < m_UserNameList.GetSize(); i++ )
			{
				if(m_UserNameList.GetAt(i).Compare(name) == 0)  break;
			}

			if(i >= m_UserNameList.GetSize())
			{
				m_UserNameList.Add(name);
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
	}
	//-----------------------------------------------------

	for(i = 0; i < m_UserNameList.GetSize(); i++ )
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_USER))->AddString(m_UserNameList.GetAt(i));
	}

	if(m_UserNameList.GetSize() > 0)
	{
		m_user = m_UserNameList.GetAt(0);
	}

	//----------   VeriDicom Setup   --------------
	if( theApp.m_nSensorHandle > 0 )
	{
		m_pDisplayImageData = (unsigned char *)new char[theApp.m_nNumRows * theApp.m_nNumCols];
		m_TimerHandle_VeriDicom = SetTimer(1,200,NULL);
	}
	else
	{
		((CStatic *)GetDlgItem(IDC_LIVE_FINGER))->ShowWindow(FALSE);
		((CStatic *)GetDlgItem(IDC_MSG))->ShowWindow(FALSE);
		((CButton *)GetDlgItem(IDC_ENROLL))->ShowWindow(FALSE);

		CRect fgrect;
		((CStatic *)GetDlgItem(IDC_LIVE_FINGER))->GetWindowRect(&fgrect);

		CRect rt;
		GetWindowRect(&rt);	                       
		ScreenToClient(&rt);
		SetWindowPos(NULL, rt.left,rt.top,rt.Width()-fgrect.Width(),rt.Height(),SWP_NOZORDER);
		CenterWindow();
	}
	//---------------------------------------------

	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);
	m_ToolTip.AddTool(&m_enroll, "指纹登记");

	if(IsConnectable())
	{
		ActiveItem(TRUE);
	}
	else
	{
		ActiveItem(FALSE);
	}


	m_Time = theApp.GetOleServerDateTime();

	UpdateData(FALSE);
	CString title;
	CWnd::GetWindowText(title);
	title += "--";
	title += theApp.m_StationType;
	SetWindowText(title);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSecurity::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	m_user.TrimLeft();
	m_user.TrimRight();
	m_password.TrimLeft();
	m_password.TrimRight();

	theApp.ConnectDatabase();
	
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_SECURITY + " where username = :username";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("username").setAsString() = m_user;
		
		g_dbcommand.Execute();
		
		g_dbcommand.FetchNext();
		int recordnum = g_dbcommand.Field(1).asLong();
		
		g_dbconnection.Commit();
		
		if( recordnum < 1 )
		{
			AfxMessageBox("用户帐户不存在。请重新输入用户名。", MB_OK | MB_ICONSTOP);
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
		SAString cmdstr = "Select userright from " + theApp.TABLE_SECURITY + " where username = :username and password = :password";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("username").setAsString() = m_user;
		g_dbcommand.Param("password").setAsString() = m_password;
		
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			theApp.m_Cpp_Right = g_dbcommand.Field("userright").asLong();
		}
		else
		{
			AfxMessageBox("密码不正确。请重新输入密码。", MB_OK | MB_ICONSTOP);
            SetDlgItemText(IDC_EDIT_PASSWORD, "");  m_password.Empty(); GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
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

	//-----------------------------------------------------
	if(theApp.m_StationType.CompareNoCase("登记工作站") == 0 && !RIGHT_DLDJGZZ(theApp.m_Cpp_Right))
	{
		AfxMessageBox("当前用户没有登录登记工作站的权限！");
		theApp.m_Cpp_Right = 0;
        SetDlgItemText(IDC_EDIT_PASSWORD, "");  m_password.Empty(); GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
		return;
	}

	if(theApp.m_StationType.CompareNoCase("取材工作站") == 0 && !RIGHT_DLQCGZZ(theApp.m_Cpp_Right))
	{
		AfxMessageBox("当前用户没有登录取材工作站的权限！");
		theApp.m_Cpp_Right = 0;
        SetDlgItemText(IDC_EDIT_PASSWORD, "");  m_password.Empty(); GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
		return;
	}

	if(theApp.m_StationType.CompareNoCase("诊断工作站") == 0 && !RIGHT_DLZDGZZ(theApp.m_Cpp_Right))
	{
		AfxMessageBox("当前用户没有登录诊断工作站的权限！");
		theApp.m_Cpp_Right = 0;
        SetDlgItemText(IDC_EDIT_PASSWORD, "");  m_password.Empty(); GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
		return;
	}

	if(theApp.m_StationType.CompareNoCase("制片工作站") == 0 && !RIGHT_DLZPGZZ(theApp.m_Cpp_Right))
	{
		AfxMessageBox("当前用户没有登录制片工作站的权限！");
		theApp.m_Cpp_Right = 0;
        SetDlgItemText(IDC_EDIT_PASSWORD, "");  m_password.Empty(); GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
		return;
	}

	if(theApp.m_StationType.CompareNoCase("细胞学工作站") == 0 && !RIGHT_DLTBSGZZ(theApp.m_Cpp_Right))
	{
		AfxMessageBox("当前用户没有登录细胞学工作站的权限！");
		theApp.m_Cpp_Right = 0;
        SetDlgItemText(IDC_EDIT_PASSWORD, "");  m_password.Empty(); GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
		return;
	}

	if(theApp.m_StationType.CompareNoCase("常规切片工作站") == 0 && !RIGHT_DLCGQPGZZ(theApp.m_Cpp_Right))
	{
		AfxMessageBox("当前用户没有登录常规切片工作站的权限！");
		theApp.m_Cpp_Right = 0;
        SetDlgItemText(IDC_EDIT_PASSWORD, "");  m_password.Empty(); GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
		return;
	}

	if(theApp.m_StationType.CompareNoCase("特检工作站") == 0 && !RIGHT_DLTJGZZ(theApp.m_Cpp_Right))
	{
		AfxMessageBox("当前用户没有登录特检工作站的权限！");
		theApp.m_Cpp_Right = 0;
        SetDlgItemText(IDC_EDIT_PASSWORD, "");  m_password.Empty(); GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
		return;
	}

	if(theApp.m_StationType.CompareNoCase("细胞学制片工作站") == 0 && !RIGHT_DLXBXZPGZZ(theApp.m_Cpp_Right))
	{
		AfxMessageBox("当前用户没有登录细胞学制片工作站的权限！");
		theApp.m_Cpp_Right = 0;
        SetDlgItemText(IDC_EDIT_PASSWORD, "");  m_password.Empty(); GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
		return;
	}
	//-----------------------------------------------------

	theApp.m_Cpp_User     = m_user;
	theApp.m_Cpp_Password = m_password;
    theApp.m_Login_Time   = m_Time;

	//----------------------------------------------
	for(int i = 0; i < m_UserNameList.GetSize(); i++ )
	{
		if( m_UserNameList.GetAt(i).Compare( m_user ) == 0 )  break;
	}

	if( i < m_UserNameList.GetSize() )
	{
		m_UserNameList.RemoveAt( i );
	}

	m_UserNameList.InsertAt(0, m_user);

	while( m_UserNameList.GetSize() > 20 )
	{
		m_UserNameList.RemoveAt( 20 );
	}

	CStdioFile File;
	if ( File.Open(theApp.m_Exe_Working_Dir + "system\\UserRecent.txt", CFile::modeWrite |CFile::modeCreate | CFile::typeText) )
	{
		for(int i = 0; i < m_UserNameList.GetSize(); i++ )
		{
			File.WriteString(m_UserNameList.GetAt(i));
			File.WriteString("\n");
		}
		File.Close();
	}
	//----------------------------------------------

	m_DBName = theApp.m_DBName_list.GetAt(((CComboBox *)GetDlgItem(IDC_COMBO_DB))->GetCurSel());
	m_DBNo   = theApp.m_DBNo_list.GetAt(((CComboBox *)GetDlgItem(IDC_COMBO_DB))->GetCurSel());
	
	CDialog::OnOK();
}

void CSecurity::OnPass() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_user.TrimLeft();
	m_user.TrimRight();
	m_password.TrimLeft();
	m_password.TrimRight();

	theApp.ConnectDatabase();
	
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_SECURITY + " where username = :username";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("username").setAsString() = m_user;
		
		g_dbcommand.Execute();
		
		g_dbcommand.FetchNext();
		int recordnum = g_dbcommand.Field(1).asLong();
		
		g_dbconnection.Commit();
		
		if( recordnum < 1 )
		{
			AfxMessageBox("用户帐户不存在。请重新输入用户名。", MB_OK | MB_ICONSTOP);
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
		SAString cmdstr = "Select userright from " + theApp.TABLE_SECURITY + " where username = :username and password = :password";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("username").setAsString() = m_user;
		g_dbcommand.Param("password").setAsString() = m_password;
		
		g_dbcommand.Execute();
		
		if( !g_dbcommand.FetchNext() )
		{
			AfxMessageBox("密码不正确。请重新输入密码。", MB_OK | MB_ICONSTOP);
            SetDlgItemText(IDC_EDIT_PASSWORD, "");  m_password.Empty(); GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
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

	CPassModify  mydlg(this);
	mydlg.m_title.Format("更改密码 (%s)", m_user );
	if( mydlg.DoModal() == IDOK )
	{
		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_SECURITY + " set password = :password where username = :username";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("username").setAsString() = m_user;
			g_dbcommand.Param("password").setAsString() = mydlg.m_ps1;
			
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
}

void CSecurity::OnManage() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_user.TrimLeft();
	m_user.TrimRight();
	m_password.TrimLeft();
	m_password.TrimRight();

	theApp.ConnectDatabase();
	
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_SECURITY + " where username = :username";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("username").setAsString() = m_user;
		
		g_dbcommand.Execute();
		
		g_dbcommand.FetchNext();
		int recordnum = g_dbcommand.Field(1).asLong();
		
		g_dbconnection.Commit();
		
		if( recordnum < 1 )
		{
			AfxMessageBox("用户帐户不存在。请重新输入用户名。", MB_OK | MB_ICONSTOP);
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

	int m_user_right = 0;

	try
	{
		SAString cmdstr = "Select userright from " + theApp.TABLE_SECURITY + " where username = :username and password = :password";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("username").setAsString() = m_user;
		g_dbcommand.Param("password").setAsString() = m_password;
		
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			m_user_right = g_dbcommand.Field("userright").asLong();
		}
		else
		{
			AfxMessageBox("密码不正确。请重新输入密码。", MB_OK | MB_ICONSTOP);
            SetDlgItemText(IDC_EDIT_PASSWORD, "");  m_password.Empty(); GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
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

	if( RIGHT_QXGL(m_user_right) )
	{
		CPassManage mydlg(this);
		mydlg.m_UserIn_Right = m_user_right;
		mydlg.DoModal();
	}
	else
	{
		AfxMessageBox("只有具有权限管理权限的用户才能进入!");
	}
}

void CSecurity::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_UserNameList.RemoveAll();

	if( m_TimerHandle_VeriDicom )
	{
		KillTimer( m_TimerHandle_VeriDicom );
		m_TimerHandle_VeriDicom = 0;
	}

	if( m_pDisplayImageData != NULL )
	{
		delete [] m_pDisplayImageData;
		m_pDisplayImageData = NULL;
	}

	theApp.IsLogOut = false;
}

BOOL CSecurity::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
    if (pMsg->wParam == VK_RETURN)
    {
		// don't allow dialog to process return or escape keys.
		OnOK();
        return TRUE;
    }
	
	if( m_hWnd )
    {
        m_ToolTip.RelayEvent(pMsg);
		return CDialog::PreTranslateMessage(pMsg);
    }
    return (FALSE);

	//return CDialog::PreTranslateMessage(pMsg);
}

void CSecurity::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	static bool m_mute = false;
	
	if( nIDEvent == m_TimerHandle_VeriDicom && !m_mute)
	{
		m_mute = true;

		VFPSgetImage(theApp.m_nSensorHandle, theApp.m_nMode, m_pDisplayImageData, theApp.m_nFlags);
		
		// displayImage(m_pDisplayImageData);
		CWnd *pDisplayWnd = GetDlgItem(IDC_LIVE_FINGER);
		CRect displayRect;
		
		pDisplayWnd->GetClientRect(&displayRect);
		CDC *pDC = pDisplayWnd->GetDC();
		
		//display image, keep the image ratio 
		displayGrayImage(pDC->m_hDC, m_pDisplayImageData, 0, 0, 
			displayRect.Width() * theApp.m_nNumCols / theApp.m_nNumRows, displayRect.Height(),
			theApp.m_nNumCols, theApp.m_nNumRows);
		
		//release DC
		pDisplayWnd->ReleaseDC(pDC);

		CString str;
		str.Empty();

		long passFailCode;		/* result code of quality assessment */
		long qualScore;			/* overall quality score */
	    long confidence = Quality( m_pDisplayImageData, theApp.m_nNumRows, theApp.m_nNumCols, &passFailCode, &qualScore, NULL, NULL );
		
		if(strlen(theApp.DATABASENAME) < 1)
		{
			passFailCode = Q_FAIL;
		}
		
		if(passFailCode == Q_PASS && confidence > 20)
		{
			char fileMinu[MAX_SIZE_STANDARD_TEMPLATE];	/* file of minutiae */
			long nFileMinu;			/* number of file minutiae */
			confidence = fpProcess (m_pDisplayImageData, theApp.m_nNumRows, theApp.m_nNumCols, fileMinu, &nFileMinu, NULL, 0, 0);
			if(confidence > 20)
			{
				//str.Format("图像质量参数:%d-%d",confidence,nFileMinu);

				bool findmatch = false;

				theApp.ConnectDatabase();

				try
				{
					g_dbcommand.setCommandText("Select * from " + (SAString)theApp.TABLE_SECURITY);
					g_dbcommand.Execute();
					
					SAString nUser,nPass,nFingerPrint;

					CString str;
					while( g_dbcommand.FetchNext() )
					{
						nUser = g_dbcommand.Field("username").asString();
						nPass = g_dbcommand.Field("password").asString();
						nFingerPrint = g_dbcommand.Field("fingerprint").asBytes();
						if(nFingerPrint.GetLength() == MAX_SIZE_STANDARD_TEMPLATE)
						{
							char DbMinu[MAX_SIZE_STANDARD_TEMPLATE];	/* file of minutiae */
					        const void * binbuf = (const void *)nFingerPrint;
							char * dataptr = (char *)binbuf;
							memcpy(DbMinu,(char *)dataptr,MAX_SIZE_STANDARD_TEMPLATE);
	                        long score = matchprints( fileMinu, DbMinu, V_MATCH_MEDIUM_SEC );
							if(score > 0)
							{
								findmatch = true;
								m_user = nUser;
								m_password = nPass;
								UpdateData(FALSE);
								break;
							}
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
				}

				if(findmatch)
				{
					str.Format("匹配成功!");
				}
				else
				{
					str.Format("匹配失败!");
				}

				((CComboBox *)GetDlgItem(IDC_COMBO_USER))->RedrawWindow();
				((CEdit *)GetDlgItem(IDC_EDIT_PASSWORD))->RedrawWindow();

				((CStatic *)GetDlgItem(IDC_MSG))->SetWindowText(str);

				_sleep(2000);

				if(findmatch)
				{
					m_mute = false;
					SendMessage(WM_COMMAND,IDOK);
					return;
				}
			}
		}

		((CStatic *)GetDlgItem(IDC_MSG))->SetWindowText(str);

		m_mute = false;
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CSecurity::OnEnroll() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_user.TrimLeft();
	m_user.TrimRight();
	m_password.TrimLeft();
	m_password.TrimRight();

	theApp.ConnectDatabase();
	
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_SECURITY + " where username = :username";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("username").setAsString() = m_user;
		
		g_dbcommand.Execute();
		
		g_dbcommand.FetchNext();
		int recordnum = g_dbcommand.Field(1).asLong();
		
		g_dbconnection.Commit();
		
		if( recordnum < 1 )
		{
			AfxMessageBox("用户帐户不存在。请重新输入用户名。", MB_OK | MB_ICONSTOP);
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

	int m_user_right = 0;

	try
	{
		SAString cmdstr = "Select userright from " + theApp.TABLE_SECURITY + " where username = :username and password = :password";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("username").setAsString() = m_user;
		g_dbcommand.Param("password").setAsString() = m_password;
		
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			m_user_right = g_dbcommand.Field("userright").asLong();
		}
		else
		{
			AfxMessageBox("密码不正确。请重新输入密码。", MB_OK | MB_ICONSTOP);
            SetDlgItemText(IDC_EDIT_PASSWORD, "");  m_password.Empty(); GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
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


	if( m_TimerHandle_VeriDicom )
	{
		KillTimer( m_TimerHandle_VeriDicom );
		m_TimerHandle_VeriDicom = 0;
	}
	//-------------------------------------
	unsigned char tempBuf[3][MAX_SIZE_STANDARD_TEMPLATE];
	long tempNum[3], nQuality;
	long nReturn;
	long nNumEnroll, nTemplateUsed;
	CString strMsg;

	//it could be 2
	long nSuggestEnrollCapture = 3;

	for(int i=0; i<nSuggestEnrollCapture; i++)
	{
		//prompt help message
		strMsg.Format("指纹注册第 %d/%d 次", i+1,nSuggestEnrollCapture);
		((CStatic *)GetDlgItem(IDC_MSG))->SetWindowText(strMsg);

		nReturn = VFPSautosnap(theApp.m_nSensorHandle, m_pDisplayImageData, 300, 60, 2, &nQuality, theApp.m_nFlags);
		if(nReturn >= 0)
		{
//			displayImage(m_pDisplayImageData);
			CWnd *pDisplayWnd = GetDlgItem(IDC_LIVE_FINGER);
			CRect displayRect;
			
			pDisplayWnd->GetClientRect(&displayRect);
			CDC *pDC = pDisplayWnd->GetDC();
			
			//display image, keep the image ratio 
			displayGrayImage(pDC->m_hDC, m_pDisplayImageData, 0, 0, 
				displayRect.Width() * theApp.m_nNumCols / theApp.m_nNumRows, displayRect.Height(),
				theApp.m_nNumCols, theApp.m_nNumRows);
			
			//release DC
			pDisplayWnd->ReleaseDC(pDC);
			
			fpProcess(m_pDisplayImageData, theApp.m_nNumRows, theApp.m_nNumCols, &tempBuf[i], &tempNum[i], NULL, 0, 0);
		}
		else
			break;

		//delay for 1 second to allow user to move off fingerprint and place it again
		_sleep(1000);
	}

	if(i < nSuggestEnrollCapture)
	{
		((CStatic *)GetDlgItem(IDC_MSG))->SetWindowText("指纹注册失败!");

		_sleep(1000);
		if( theApp.m_nSensorHandle > 0 )
		{
			m_TimerHandle_VeriDicom = SetTimer(1,200,NULL);
		}

		return;
	}

	//using multiEnroll() for a successful enrollment
	char m_pEnrollTemplate[MAX_SIZE_STANDARD_TEMPLATE];
	nReturn = multiEnroll(&tempBuf[0], &tempBuf[1], &tempBuf[2], m_pEnrollTemplate, &nNumEnroll, &nTemplateUsed, 0);
	if(nReturn > 0)
	{
		((CStatic *)GetDlgItem(IDC_MSG))->SetWindowText("指纹注册成功!");
	}
	else
	{
		((CStatic *)GetDlgItem(IDC_MSG))->SetWindowText("指纹注册失败!");
	}
	//-------------------------------------
    SAString  fingerset;
	fingerset.Empty();
	fingerset += SAString( (BYTE *)m_pEnrollTemplate, MAX_SIZE_STANDARD_TEMPLATE );

	try
	{
		SAString cmdstr = "Update " + theApp.TABLE_SECURITY + " set fingerprint = :fingerprint where username = :username";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("username").setAsString() = m_user;
		g_dbcommand.Param("fingerprint").setAsBytes() = fingerset;
		
		g_dbcommand.Execute();
		
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
		//AfxMessageBox((const char*)x.ErrText());
		return;
	}

    fingerset.Empty();

	_sleep(1000);

	if( theApp.m_nSensorHandle > 0 )
	{
		m_TimerHandle_VeriDicom = SetTimer(1,200,NULL);
	}
	
}

void CSecurity::ActiveItem(BOOL value)
{
	((CComboBox *)GetDlgItem(IDC_COMBO_USER))->EnableWindow(value);
	((CEdit *)GetDlgItem(IDC_EDIT_PASSWORD))->EnableWindow(value);
	((CButton *)GetDlgItem(IDMANAGE))->EnableWindow(value);
	((CButton *)GetDlgItem(IDPASS))->EnableWindow(value);
	((CButton *)GetDlgItem(IDOK))->EnableWindow(value);

	if(value)
	{
		((CEdit *)GetDlgItem(IDC_EDIT_PASSWORD))->SetFocus();
	}
}

bool CSecurity::IsConnectable()
{
	bool Connection_Available = theApp.ConnectDatabase();

	if( Connection_Available )
	{
		try
		{
			g_dbcommand.setCommandText("Select count(*) from " + (SAString)theApp.TABLE_SECURITY);
			
			g_dbcommand.Execute();
			
			g_dbcommand.FetchNext();
			int usernum = g_dbcommand.Field(1).asLong();
			
			g_dbconnection.Commit();
			
			if( usernum < 1 )
			{
				try
				{
					SAString cmdstr = "insert into " + theApp.TABLE_SECURITY + " (username, password, userright,userinfo) values (:username, :password, :userright,:userinfo)";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("username").setAsString() = "administrator";
					g_dbcommand.Param("password").setAsString() = "888888";
					g_dbcommand.Param("userright").setAsLong()  = 0xFFFFFFFF;
					g_dbcommand.Param("userinfo").setAsString() = "系统管理员";
					
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
					AfxMessageBox("管理员初始信息录入失败，系统无法正常启动！");
					Connection_Available = false;
				}
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
			AfxMessageBox("登录信息文件丢失，系统无法正常启动！");
			Connection_Available = false;
		}

	}

	return Connection_Available;
}


void CSecurity::OnCancel() 
{
	UpdateData(TRUE);
	m_user.TrimLeft();
	m_user.TrimRight();
	m_password.TrimLeft();
	m_password.TrimRight();

	theApp.ConnectDatabase();
	
	try
	{
		SAString cmdstr = "Select userright from " + theApp.TABLE_SECURITY + " where username = :username and password = :password";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("username").setAsString() = m_user;
		g_dbcommand.Param("password").setAsString() = m_password;
		
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			theApp.m_Cpp_Right = g_dbcommand.Field("userright").asLong();
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
	
	CDialog::OnCancel();
}
