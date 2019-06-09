// Pathology.cpp : Defines the class behaviors for the application.
//  
#include "stdafx.h"
#include <vector>
#include <string>

#include "Pathology.h"
#include "io.h"
#include "psapi.h"

#include "MainFrm.h"
#include "PathologyDoc.h"
#include "PathologyView.h"
#include "winsock2.h"
#include "htmlhelp.h"
#include "SysConfig.h"
#include "INI.h"
#include "TagShow.h"
#include "MedImage.h"
#include "KSFileDialog.h"
#include "DbSelect.h"
#include "MedPerson.h"
#include "Security.h"
#include "PassManage.h"
#include "MedWax.h"
#include "MedPreslice.h"
#include "MedAdvice.h"
#include "MedSlice.h"
#include "MedDiagnose.h"
#include "MedTbs.h"
#include "StrEdit.h"
#include "MyProgressBar.h"
#include "LTDIC.H"
#include "NotePad.h"
#include "AdviceShow.h"
#include "ImageView.h"
#include "Zdex.h"
#include "tinyxml.h"
#include "DsControllerMainSheet.h"
#include "YQJL.h"
#include "MedMaterial.h"
#include "NormalWord.h"
#include "AdviceTip.h"
#include "MicroWriterConfig.h"
#include "NoUse.h"

#include "GetStudyUID.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CHISConnectClient  g_HISConnectClient;
CHISConnectClient1  g_HISConnectClient1;

SAConnection g_dbconnection;
SACommand    g_dbcommand;



/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//CAboutBox 


class CAboutBoxDlg : public CDialog
{
public:
	CAboutBoxDlg();

// Dialog Data
	//{{AFX_DATA(CAboutBoxDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutBoxDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutBoxDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutBoxDlg::CAboutBoxDlg() : CDialog(CAboutBoxDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutBoxDlg)
	//}}AFX_DATA_INIT
}

void CAboutBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutBoxDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutBoxDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutBoxDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutBoxDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}




/////////////////////////////////////////////////////////////////////////////
// CPathologyApp

BEGIN_MESSAGE_MAP(CPathologyApp, CWinApp)
	//{{AFX_MSG_MAP(CPathologyApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_APP_CAMERA, OnAppCamera)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_FILE_SYSSETUP, OnFileSyssetup)
	ON_COMMAND(ID_SETUP_TAG, OnSetupTag)
	ON_UPDATE_COMMAND_UI(ID_APP_CAMERA, OnUpdateAppCamera)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_APP_LAST, OnAppLast)
	ON_COMMAND(ID_APP_NEXT, OnAppNext)
	ON_COMMAND(ID_APP_CHECK, OnAppCheck)
	ON_UPDATE_COMMAND_UI(ID_APP_CHECK, OnUpdateAppCheck)
	ON_COMMAND(ID_APP_COLLECT, OnAppCollect)
	ON_UPDATE_COMMAND_UI(ID_APP_COLLECT, OnUpdateAppCollect)
	ON_COMMAND(ID_DOC_CLOSE, OnDocClose)
	ON_UPDATE_COMMAND_UI(ID_DOC_CLOSE, OnUpdateDocClose)
	ON_COMMAND(ID_FILE_INPUT, OnFileInput)
	ON_COMMAND(ID_FILE_MAIL, OnFileMail)
	ON_COMMAND(ID_FILE_OUTPUT, OnFileOutput)
	ON_COMMAND(ID_APP_DBSELECT, OnAppDbselect)
	ON_COMMAND(ID_DOC_SAVEAS, OnDocSaveas)
	ON_UPDATE_COMMAND_UI(ID_DOC_SAVEAS, OnUpdateDocSaveas)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_COMMAND(ID_FILE_SECURITYSETUP, OnFileSecuritysetup)
	ON_UPDATE_COMMAND_UI(ID_FILE_SECURITYSETUP, OnUpdateFileSecuritysetup)
	ON_UPDATE_COMMAND_UI(ID_FILE_SYSSETUP, OnUpdateFileSyssetup)
	ON_COMMAND(ID_APP_REFRESH, OnAppRefresh)
	ON_COMMAND(ID_APP_LOGOUT, OnAppLogout)
	ON_UPDATE_COMMAND_UI(ID_APP_LAST, OnUpdateAppLast)
	ON_UPDATE_COMMAND_UI(ID_APP_NEXT, OnUpdateAppNext)
	ON_COMMAND(ID_APP_SEND, OnAppSend)
	ON_UPDATE_COMMAND_UI(ID_APP_SEND, OnUpdateAppSend)
	ON_COMMAND(ID_APP_UNCHECK, OnAppUncheck)
	ON_UPDATE_COMMAND_UI(ID_APP_UNCHECK, OnUpdateAppUncheck)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_COMMAND(ID_REFERENCE, OnReference)
	ON_COMMAND(ID_DOC_QUICKINPUT, OnDocQuickinput)
	ON_UPDATE_COMMAND_UI(ID_DOC_QUICKINPUT, OnUpdateDocQuickinput)

	ON_COMMAND(ID_DOC_NEWORDER, OnDocNewOrder)
	ON_UPDATE_COMMAND_UI(ID_DOC_NEWORDER, OnUpdateDocNewOrder)

	ON_COMMAND(ID_DOC_NEWORDER1, OnDocNewOrder1)
	ON_UPDATE_COMMAND_UI(ID_DOC_NEWORDER1, OnUpdateDocNewOrder1)


	ON_COMMAND(ID_APP_NOTEPAD, OnAppNotepad)
	ON_COMMAND(ID_FILE_RPTCOPY, OnFileRptcopy)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
	ON_COMMAND(ID_FILE_RPTCOPYALL, OnFileRptcopyall)
	ON_COMMAND(ID_FILE_RPTCOPYIMG, OnFileRptcopyimg)
	ON_COMMAND(ID_REPORT_OUTPUT, OnReportOutput)
	ON_COMMAND(ID_APP_DZBL,OnAppDZBL)
	ON_COMMAND(ID_YQBG,OnAppYQBG)
	ON_COMMAND(ID_WRITER_PRINT_SETUP, OnWriterPrintSetup)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPathologyApp construction

CPathologyApp::CPathologyApp()
{
	m_nSensorHandle = 0;

	m_bHideTreeEmptyNode = TRUE;

	m_PrinterSeting_Restored = false;
    m_Refresh_Flag_Check = m_Refresh_Flag_Collect = true;

	m_DB_Type_No = 1;
	m_DB_Type_Name.Format("活检库");

	Is_Batch_Printing = false;
	InPrinting = false;
	m_PrintType = 0;

	IsLogOut = false;

	m_UnRead_Knly_Notified.Empty();

	hisStatus = 0; //不是退单

	m_sStainingMachineInterfaceFileName = "";		// 自动染色机接口文件名
	m_sStainingMachineInterfaceModuleName = "";	    // 自动染色机接口模块名

	m_hStainingMachineInterfaceDll = NULL;

	InitDBConnectionParams_Dll = NULL;
	SendSlideIDs2StainingMachine_Dll = NULL;


}


void CPathologyApp::GetParam(CString sKeyName, CString &sKeyValue)
{
	BOOL bFound;
	int ni, nKeyLen, nLen;
	CString sValue(""), sParam("");

	bFound = FALSE;
	ni = 1;
	sValue = "";
	nKeyLen = sKeyName.GetLength();

    while (ni < __argc && !bFound)
    {
        sParam = __argv[ni];
        sParam.MakeUpper();
		
		if (sParam.Find(sKeyName) >= 0)
		{
			bFound = TRUE;		
	        nLen = sParam.GetLength();

			if (nLen > nKeyLen)
			{
				sValue = sParam.Mid(nKeyLen, nLen - nKeyLen);
			}
			else
			{
				if (__argc >= (ni + 1))
				{
					sValue = __argv[ni + 1];
				}
			}
  
            bFound = true;
		}

        ni ++;
    }

	sKeyValue = sValue;

	return;
}


/////////////////////////////////////////////////////////////////////////////
// The one and only CPathologyApp object

CPathologyApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPathologyApp initialization

BOOL CPathologyApp::InitInstance()
{
	int ni, nRet = -1, nLicHostPort;
	CString sParam;
	CString sLicHostName(""), sLicHostPort("");
	BOOL bFound = FALSE, bTestMode = FALSE;

	CoInitialize(NULL);

	AfxEnableControlContainer();

	// XD++ Library Add line.
	FOInit();
	AfxOleInit();

	BCLicenseMe("Mem:Anyone", (e_licKind)3, 1, "75D33E1C", (e_licProduct)9);//(e_licProduct)14);		

	// Get the EXE File dir
    char szFullPath[_MAX_PATH], szDir[_MAX_PATH], szDrive[_MAX_PATH];
	::GetModuleFileName(NULL,szFullPath,_MAX_PATH);
	_splitpath(szFullPath,szDrive,szDir,NULL,NULL);
	strcat(szDrive,szDir);
	m_Exe_Working_Dir.Format("%s",szDrive);

	GetTempPath( 500, m_SysTmpPath );
	//-------------------------------------------
	WORD wVersionRequested;
	WSADATA wsaData;
	char name[255];
	PHOSTENT hostinfo;
	wVersionRequested = MAKEWORD( 2, 0 );
	
	if ( WSAStartup( wVersionRequested, &wsaData ) == 0 )
	{
		if( gethostname ( name, sizeof(name)) == 0)
		{
			if((hostinfo = gethostbyname(name)) != NULL)
			{
				m_LocalIp = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
			}
		}
		WSACleanup( );
	} 
	while(m_LocalIp.Find('.') >= 0)
	{
		m_LocalIp = m_LocalIp.Left(m_LocalIp.Find('.')) + m_LocalIp.Right(m_LocalIp.GetLength() - m_LocalIp.Find('.') - 1);
	}
	//-------------------------------------------
	
	IniLoading();
	MicroWriterIniLoading();

	//------------------------------------------------

	///////////////////////////////////////////////////////////////////

	bFound = FALSE;
    ni = 1;
    while (ni < __argc && !bFound)
    {
        sParam = __argv[ni];
        sParam.MakeUpper();

		if (sParam.Find("-TEST=B041303861") >= 0)
		{
			bFound = TRUE;
			bTestMode = TRUE;
	    }

        ni ++;
    }

	GetParam("-HOST=", sLicHostName);
	GetParam("-PORT=", sLicHostPort);

	nLicHostPort = atoi(sLicHostPort);

	if (sLicHostName.IsEmpty()) sLicHostName = m_sLicHostName;
	if (nLicHostPort == 0) nLicHostPort = m_nLicHostPort;

	nRet = g_objLicense.Check("PIS", sLicHostName, nLicHostPort, FALSE);		
	if (nRet == -1)
	{
		::MessageBox(NULL,"通信错误！","错误",MB_OK | MB_ICONEXCLAMATION);
        //ReleaseMutex(hMutex);

		return FALSE;
	}

	if (nRet == 0)
	{
		::MessageBox(NULL,"该工作站没有使用许可证!","错误",MB_OK | MB_ICONEXCLAMATION);
        //ReleaseMutex(hMutex);

		return FALSE;
	}

    if (nRet == 2)
	{
		::MessageBox(NULL,"找不到服务器配置信息!","错误",MB_OK | MB_ICONEXCLAMATION);
        //ReleaseMutex(hMutex);

		return FALSE;
	}


	if (bTestMode)
		AfxMessageBox(g_objLicense.GetReceivedText());

	////////////////////////////////////////////////////
	
	m_sLicHostName	= sLicHostName;
	m_nLicHostPort	= nLicHostPort;

	m_sDBType		= g_objLicense.m_sDBType;
	m_sSchemaName	= g_objLicense.m_sSchemaName;

	strcpy(ServerName, g_objLicense.m_sDBHostName);
	strcpy(DATABASENAME, g_objLicense.m_sDBName);
	strcpy(DATABASE_USERNAME, g_objLicense.m_sDBUserId);
	strcpy(DATABASE_PASSWORD, g_objLicense.m_sDBPswd);

	////////////////////////////////////////////////////////////////////

    TABLE_MAIN.Format("cmipsmain");
    TABLE_DEFINE.Format("cmipsmain_dbdefine");
	TABLE_CHARGE.Format("cmipsmain_charge");
	TABLE_IMAGE.Format("cmipsmain_image");
	TABLE_SOUND.Format("cmipsmain_sound");
	TABLE_HZ.Format("cmipsmain_hz");
	TABLE_MATERIAL.Format("cmipsmain_material");
	TABLE_SLICE.Format("cmipsmain_slice");
	TABLE_ADVICE.Format("cmipsmain_advice");
	TABLE_TAG.Format("cmipsmain_tag");
	TABLE_TC.Format("cmipsmain_tc");
	TABLE_DBTYPE.Format("cmipsmain_dbtype");
	TABLE_LOGO.Format("cmipsmain_logo");
	TABLE_NOTEPAD.Format("cmipsmain_notepad");
	TABLE_SECURITY.Format("cmipssecurity");
	TABLE_ICD10.Format("icd10");
	TABLE_ZHANGQI.Format("zhangqi");
	TABLE_BINGZHONG.Format("bingzhong");
	TABLE_STANDTEXT.Format("standtext");
	TABLE_BOOKING.Format("booking");
	TABLE_BOOKINGSAMPLE.Format("Booking_Sample");
	TABLE_ZC.Format("cmipsmain_zc");

	if(m_DBType.CompareNoCase("ORACLE") == 0 && !m_OwnUser.IsEmpty())
	{
		TABLE_MAIN     = m_OwnUser + "." + TABLE_MAIN;
		TABLE_DEFINE   = m_OwnUser + "." + TABLE_DEFINE;
		TABLE_CHARGE   = m_OwnUser + "." + TABLE_CHARGE;
		TABLE_IMAGE    = m_OwnUser + "." + TABLE_IMAGE;
		TABLE_SOUND    = m_OwnUser + "." + TABLE_SOUND;
		TABLE_HZ       = m_OwnUser + "." + TABLE_HZ;
		TABLE_MATERIAL = m_OwnUser + "." + TABLE_MATERIAL;
		TABLE_SLICE    = m_OwnUser + "." + TABLE_SLICE;
		TABLE_ADVICE   = m_OwnUser + "." + TABLE_ADVICE;
		TABLE_TAG      = m_OwnUser + "." + TABLE_TAG;
		TABLE_TC       = m_OwnUser + "." + TABLE_TC;
		TABLE_DBTYPE   = m_OwnUser + "." + TABLE_DBTYPE;
		TABLE_LOGO     = m_OwnUser + "." + TABLE_LOGO;
		TABLE_NOTEPAD  = m_OwnUser + "." + TABLE_NOTEPAD;
		TABLE_SECURITY = m_OwnUser + "." + TABLE_SECURITY;
		TABLE_BOOKING  = m_OwnUser + "." + TABLE_BOOKING;
		TABLE_BOOKINGSAMPLE = m_OwnUser + "." + TABLE_BOOKINGSAMPLE;
		TABLE_ZC = m_OwnUser + "." + TABLE_ZC;
	}
	//------------------------------------------------

	// VeriDicom Functions
	VeriInit();
		
    CSecurity dlg;
	dlg.m_DBName = m_DB_Type_Name;
	dlg.m_DBNo   = m_DB_Type_No;
	if( dlg.DoModal() == IDCANCEL ) 
	{
        if (RIGHT_XGXTSZ(theApp.m_Cpp_Right) && AfxMessageBox("设置运行参数？",MB_OKCANCEL|MB_ICONQUESTION) == IDOK)
		{
			CSysConfig mydlg;
			mydlg.DoModal();
			AfxMessageBox("系统需要重新启动！");
		}

		return FALSE;
	}
	else
	{
		if(m_DB_Type_No != dlg.m_DBNo)
		{
			m_DB_Type_Name = dlg.m_DBName;
			m_DB_Type_No   = dlg.m_DBNo;
			
			CIniReader m_IniReader;
			m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");
			
			m_IniReader.setKey(m_DB_Type_Name,"dbname","StudyTable");
			CString tmp;
			tmp.Format("%d", m_DB_Type_No);
			m_IniReader.setKey(tmp,"dbno","StudyTable");
		}
	}

	
	////////////////////////////////////////////////////
	// Load HISConnectClient

	g_HISConnectClient.LoadClient(m_sHISConnectFileName);
	
	if (g_HISConnectClient.Connected())
	{
		g_HISConnectClient.Initialize(m_nHISConnectCode, m_sHISConnectModality, m_sHISConnectEndPoint, "PISX");
		g_HISConnectClient.SetSenderInfo(m_Cpp_User, m_Cpp_User);
	}


	////////////////////////////////////////////////////
	// Load HISConnectClient1

	g_HISConnectClient1.LoadClient(m_sHISConnect1FileName);
	
	if (g_HISConnectClient1.Connected())
	{
		g_HISConnectClient1.Initialize(m_nHISConnect1Code, (char *) (const char *) m_sHISConnect1Modality, (char *)(const char *) m_sHISConnect1EndPoint, "PIS","病理质控管理信息系统");
		g_HISConnectClient1.SetSenderInfo(m_Cpp_User, m_Cpp_User);
	}


	RemoveOldNotepad();
	//------------------------------------------
	isLogoTableExist = false;
	ConnectDatabase();
	try
	{
		SAString cmdstr = "select count(*) from " + theApp.TABLE_LOGO;
		g_dbcommand.setCommandText( cmdstr );
		
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
		{
			isLogoTableExist = true;
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
	//------------------------------------------
	try
	{
		SAString cmdstr = "Select FieldName, FieldDescription, FieldInfo from " + theApp.TABLE_DEFINE;
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();
		
		while( g_dbcommand.FetchNext() )
		{
			m_fnamelist.Add(g_dbcommand.Field("FieldName").asString());
			m_fdescriptionlist.Add(g_dbcommand.Field("FieldDescription").asString());
			m_finfolist.Add(g_dbcommand.Field("FieldInfo").asString());
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
		return FALSE;
	}

	for(int i = 0; i < m_fdescriptionlist.GetSize(); i++)
	{
		SADataType_t dt = SA_dtString;
		try
		{
			SAString cmdstr = "Select TOP 1 " + m_fnamelist.GetAt(i)+ " from " + theApp.TABLE_MAIN;
			if(m_DBType.CompareNoCase("ORACLE") == 0)
			{
				cmdstr = "Select " + m_fnamelist.GetAt(i)+ " from " + theApp.TABLE_MAIN + " where ROWNUM<=1";
			}
			g_dbcommand.setCommandText(cmdstr);
			
			g_dbcommand.Execute();
			
			if(g_dbcommand.FetchNext())
			{
				dt = g_dbcommand.Field(1).FieldType();
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

		m_ftypeList.Add(dt);
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#if _MSC_VER <= 1200 // MFC 6.0 or earlier
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif // MFC 6.0 or earlier

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Pathology"));
 
	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)
  
	m_PrinterControl.RestorePrinterSelection(m_hDevMode, m_hDevNames) ;
	m_PrinterSeting_Restored = true;

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CPathologyDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CPathologyView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	((CMainFrame*)m_pMainWnd)->ShowWindowEx(m_nCmdShow);
	m_pMainWnd->UpdateWindow();

	m_MainThread_FrameWnd = m_pMainWnd;

	if (g_HISConnectClient.Connected())
		g_HISConnectClient.SetOwnerWnd(m_pMainWnd->m_hWnd);

	if (g_HISConnectClient1.Connected())
		g_HISConnectClient1.SetOwnerWnd(m_pMainWnd->m_hWnd);


	//AfxMessageBox(m_PrintTime);
	m_pMainWnd->SetTimer(8888,5000,NULL);
	m_pMainWnd->SetTimer(8889,5000,NULL);

	if(theApp.m_StationType.CompareNoCase("常规切片工作站") == 0 || theApp.m_StationType.CompareNoCase("特检工作站") == 0 || theApp.m_StationType.CompareNoCase("制片工作站") == 0)
	{
		m_pMainWnd->SendMessage(WM_TIMER,9000,NULL);
		m_pMainWnd->SetTimer(9000,10*60*1000,NULL);
	}
	
	if(theApp.m_StationType.CompareNoCase("诊断工作站") == 0 || theApp.m_StationType.CompareNoCase("取材工作站")==0 || theApp.m_StationType.CompareNoCase("细胞学工作站")==0 )
	{
		GetView()->m_tabCtrl.SetActiveView(1);
		CWnd *pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CDBList));
		if(pWnd != NULL)
		{
			((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.SetActiveView(pWnd);
			((CEdit *)pWnd->GetDlgItem(IDC_EDIT_BLH))->SetFocus();
		}
	}
	
	if(theApp.m_BgyqAntoOpenMode.CompareNoCase("是") == 0 && m_PrintTime.CompareNoCase("0")){
		int count;
		try
		{
			SAString cmdstr = "select count(*) from "+theApp.TABLE_MAIN+" where bgzt>=2 and bgzt<4 and wfyy<> '' and wfyy is not null";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Execute();
			while(g_dbcommand.FetchNext())
			{
				count = g_dbcommand.Field(1).asLong();
			}
			g_dbconnection.Commit();
		}
		catch(SAException &e)
		{
			AfxMessageBox((const char*)e.ErrText());
		}
		
		if(count > 0){
			CYQJL dlg1;
			dlg1.DoModal();
		}
		
	}

	if(theApp.m_IsUsingCard.CompareNoCase("是") == 0)
	{
		if(theApp.m_StationType.CompareNoCase("登记工作站") == 0 || theApp.m_StationType.CompareNoCase("取材工作站") == 0 || theApp.m_StationType.CompareNoCase("诊断工作站") == 0)
		{
			if(theApp.m_HisCfg.CompareNoCase("通用接口") == 0)
			{
				CWinApp::OnFileNew();
				//icdev = dc_init(100,115200);
			}		
		}
	}	


	// 自动染色机接口
	if (!m_sStainingMachineInterfaceFileName.IsEmpty()) {
		m_hStainingMachineInterfaceDll = LoadLibrary(m_sStainingMachineInterfaceFileName);
		if (m_hStainingMachineInterfaceDll) {
			
			GetStainingMachineModuleCount_Dll = (int (*)(void)) GetProcAddress(m_hStainingMachineInterfaceDll, "GetStainingMachineModuleCount");
			InitDBConnectionParams_Dll = (void (*)(const char *, const char *, const char *, const char *, const char *)) GetProcAddress(m_hStainingMachineInterfaceDll,  "InitDBConnectionParams");
			if (InitDBConnectionParams_Dll) {
				// Init DBConnection
				InitDBConnectionParams_Dll(g_objLicense.m_sDBType, g_objLicense.m_sDBHostName, g_objLicense.m_sDBName, g_objLicense.m_sDBUserId, g_objLicense.m_sDBPswd);
				SendSlideIDs2StainingMachine_Dll = (BOOL (*) (const char *)) GetProcAddress(m_hStainingMachineInterfaceDll, "SendSlideIDs2StainingMachine");
			}
		}
	}

	
//	if(theApp.m_CaptureMode.CompareNoCase("佳能设备支持") == 0)
//	{
//		m_pMainWnd->SendMessage(WM_TIMER,8111,NULL);
//		m_pMainWnd->SetTimer(8111,1000,NULL);
//	}

	return TRUE;
}


BOOL  CPathologyApp::IsStainingMachineModuleAvailable()
{
	if (GetStainingMachineModuleCount_Dll) 
		return GetStainingMachineModuleCount_Dll();

	return FALSE;
}


BOOL  CPathologyApp::SendSlideIDs2StainingMachine(const char *slideIDs2Machine)
{
	if (SendSlideIDs2StainingMachine_Dll) 
		return SendSlideIDs2StainingMachine_Dll(slideIDs2Machine);

	return FALSE;
}

// App command to run the dialog
void CPathologyApp::OnAppAbout()
{
	CAboutBoxDlg dlgAbout;

	dlgAbout.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CPathologyApp message handlers
bool CPathologyApp::SetNewPrinter(int index)
{
	return m_PrinterControl.SetNewPrinter(m_hDevMode, m_hDevNames, index) ;
}

CString CPathologyApp::GetDefaultPrinter()
{
	PRINTDLG	pd ;
	CString		printer("Failed") ;
	
	pd.lStructSize = (DWORD)sizeof(PRINTDLG) ;
	BOOL bRet = GetPrinterDeviceDefaults(&pd) ;
	if (bRet)
		{
		// protect memory handle with ::GlobalLock and ::GlobalUnlock
		DEVMODE *pDevMode = (DEVMODE*)::GlobalLock(m_hDevMode) ;
		printer = pDevMode->dmDeviceName ;
		::GlobalUnlock(m_hDevMode) ;
		}
	return printer ;
}


int CPathologyApp::GB2U_Char(wchar_t * pOut, char *pText) 
{
	int ret = 0;
	if ((unsigned) pText[0] < 0x80) {
		pOut[0] = (wchar_t) pText[0];
	}
	else {
		::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pText, 2, pOut, 1);
		ret = 1;
	}
	return ret;
}

int CPathologyApp::GB2U_Str(wchar_t *pOut, char *pText, int len) 
{
	int i, j;
	for (i = 0, j = 0; i < len; i++, j++) {
		i+= GB2U_Char(&pOut[j], &pText[i]);
	}
	return j;
}


// 病理号, 序号, 标记
// 54303    A1   常规 
void CPathologyApp::MakeEmbedBoxFile(const char *medicalNo, const char *serialNo, const char *mark)
{
	// 文件名用病理号_XXX.TXT XXX 为1-999的随机数
	CString sFileName;
	SYSTEMTIME lpsystime;
	FILE *fp;
	
	
	if (m_IsEmbedBoxValid != "是")
		return;
	
	
	if (m_SlidePrintFilePath.IsEmpty())
		return;
	

	GetLocalTime(&lpsystime);

	sFileName.Format("%s\\E02%02d%02d%03d_%03d.txt",
		m_SlidePrintFilePath,
		lpsystime.wHour,
		lpsystime.wMinute,
		lpsystime.wSecond,
		lpsystime.wMilliseconds,
		GetTickCount() % 1000);

		
	if (m_SlidePrintFmtCode.Compare("1000") == 0 ) {
		wchar_t szFmtStrW[1024];
		char szFmtStr[1024];
		char szUnicode_little_file_header[2] = {(char)0xFF, (char) 0xFE};
		memset(szFmtStr, 0, 1024);
		memset(szFmtStrW, 0, 1024 * sizeof(szFmtStrW[0]));

		//sprintf(szFmtStr, "$%s$%s$%s$", medicalNo, serialNo, mark);	
		sprintf(szFmtStr, "$%s$%s$", medicalNo, serialNo);	

		GB2U_Str(szFmtStrW, szFmtStr, strlen(szFmtStr));

		fp = fopen(sFileName, "wb");
		if (fp) {
			fwrite(szUnicode_little_file_header, sizeof(char), 2, fp);
			fwrite(szFmtStrW, wcslen(szFmtStrW) * sizeof(wchar_t), 1, fp);
			fclose(fp);
		}
	}
	else {

		char szFmtStr[1024];
		memset(szFmtStr, 0, 1024);
		//sprintf(szFmtStr, "$%s$%s$%s$", medicalNo, serialNo, mark);
		sprintf(szFmtStr, "$%s$%s$", medicalNo, serialNo);

		fp = fopen(sFileName, "wb");
		
		if (fp) {
			fwrite(szFmtStr, strlen(szFmtStr), 1, fp);
			fclose(fp);
		}

	}

}


// 病理号， 序号， 制片子号， 备注， 条码号,  标记
// 54303    A1     54303-A1   HE     5430302, 常规
void CPathologyApp::MakeSlidePrintFile(const char *medicalNo, const char *serialNo, const char *slideNo, const char *memo, const char *barCodeNo, const char *mark)
{
	CString sFileName;
	SYSTEMTIME lpsystime;
	FILE *fp;
	
	if (m_IsSlidePrintValid != "是") 
		return;

	if (m_SlidePrintFilePath.IsEmpty())
		return;

	GetLocalTime(&lpsystime);

	sFileName.Format("%s\\S02%02d%02d%03d_%03d.txt",
		m_SlidePrintFilePath,
		lpsystime.wHour,
		lpsystime.wMinute,
		lpsystime.wSecond,
		lpsystime.wMilliseconds,
		GetTickCount() % 1000);

		
	if (m_SlidePrintFmtCode.Compare("1000") == 0) {

		wchar_t szFmtStrW[1024];
		char szFmtStr[1024];
		char szUnicode_little_file_header[2] = {(char)0xFF, (char) 0xFE};
		memset(szFmtStr, 0, 1024);
		memset(szFmtStrW, 0, 1024 * sizeof(szFmtStrW[0]));

		sprintf(szFmtStr, "$%s$%s$%s", medicalNo, serialNo, memo);

		GB2U_Str(szFmtStrW, szFmtStr, strlen(szFmtStr));

		fp = fopen(sFileName, "wb");
		if (fp) {
			fwrite(szUnicode_little_file_header, sizeof(char), 2, fp);
			fwrite(szFmtStrW, wcslen(szFmtStrW) * sizeof(wchar_t), 1, fp);
			fclose(fp);
		}
	}
	else {

		char szFmtStr[1024];
		memset(szFmtStr, 0, 1024);
		sprintf(szFmtStr, "$%s$%s$%s", medicalNo, serialNo, memo);

		fp = fopen(sFileName, "wb");
		
		if (fp) {
			fwrite(szFmtStr, strlen(szFmtStr), 1, fp);
			fclose(fp);
		}
	}
}

int CPathologyApp::ExitInstance() 
{
	
	if (m_hStainingMachineInterfaceDll != NULL) {
		FreeLibrary(m_hStainingMachineInterfaceDll);
		m_hStainingMachineInterfaceDll = NULL;
	}


	if( m_PrinterSeting_Restored && m_PrinterControl.GetPrinterCount() > 0)
		m_PrinterControl.SavePrinterSelection(m_hDevMode, m_hDevNames) ;

	// VeriDicom Functions
	//free sensor
	if(m_nSensorHandle > 0)
		VFPSclose(m_nSensorHandle, m_nFlags);
	
	DisconnectDatabase();

	CoUninitialize();
	
	m_fnamelist.RemoveAll();
	m_fdescriptionlist.RemoveAll();
	m_finfolist.RemoveAll();
	m_ftypeList.RemoveAll();

	m_DBName_list.RemoveAll();
    m_DBNo_list.RemoveAll();

	///////////////////////////////////////
	//ExitProcess(0);
	///////////////////////////////////////

	return CWinApp::ExitInstance();
}

bool CPathologyApp::ConnectDatabase()
{
	if(g_dbconnection.isConnected())  return true;

	SAClient_t SA_DBType = SA_SQLServer_Client;

	if(m_DBType.CompareNoCase("SQLSERVER") == 0)
	{
		SA_DBType = SA_SQLServer_Client;
	}
	if(m_DBType.CompareNoCase("ORACLE") == 0)
	{
		SA_DBType = SA_Oracle_Client;
	}

	bool IsConnected = false;
	try
	{
		switch(SA_DBType)
		{
		case SA_SQLServer_Client:
			g_dbconnection.Connect( (SAString)ServerName + "@" + DATABASENAME, DATABASE_USERNAME, DATABASE_PASSWORD, SA_SQLServer_Client );
			break;
		case SA_Oracle_Client:
			g_dbconnection.Connect( (SAString)DATABASENAME, DATABASE_USERNAME, DATABASE_PASSWORD, SA_Oracle_Client );
			break;
		}
		g_dbcommand.setConnection( &g_dbconnection );

		IsConnected = true;
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

	return IsConnected;
}

void CPathologyApp::DisconnectDatabase()
{
	if(g_dbconnection.isConnected())
	{
		g_dbconnection.Disconnect();
	}
}

void CPathologyApp::OnFilePrintPreview() 
{
	// TODO: Add your command handler code here
	CPathologyView *pView = GetView();

	if(pView != NULL)
	{
		pView->PostMessage(WM_COMMAND, ID_FILE_PRINT_PREVIEW);
	}
}

void CPathologyApp::OnFilePrint() 
{
	// TODO: Add your command handler code here
	CPathologyView *pView = GetView();

	if(pView != NULL)
	{
		pView->PostMessage(WM_COMMAND, ID_FILE_PRINT);
	}
}
DWORD FindProcess(char *strProcessName)
{
    DWORD aProcesses[1024], cbNeeded, cbMNeeded;
    HMODULE hMods[1024];
    HANDLE hProcess;
    char szProcessName[MAX_PATH];
	
    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) ) return 0;
    for(int i=0; i< (int) (cbNeeded / sizeof(DWORD)); i++)
    {
        //_tprintf(_T("%d\t"), aProcesses[i]);
        hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);
        EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbMNeeded);
        GetModuleFileNameEx( hProcess, hMods[0], szProcessName,sizeof(szProcessName));
		
        if(strstr(szProcessName, strProcessName))
        {
            //_tprintf(_T("%s;"), szProcessName);
			//AfxMessageBox("findit");
            return(aProcesses[i]);
        }
        //_tprintf(_T("\n"));
    }
	
	//AfxMessageBox("not findit");
    return 0;
}


//
// Function: ErrorForce
// 此函数中用上面的 FindProcess 函数获得你的目标进程的ID
// 用WIN API OpenPorcess 获得此进程的句柄，再以TerminateProcess
// 强制结束这个进程
//

VOID KillProcess()
{
    // When the all operation fail this function terminate the "winlogon" Process for force exit the system.
    HANDLE hYourTargetProcess = OpenProcess(PROCESS_QUERY_INFORMATION |   // Required by Alpha
		PROCESS_CREATE_THREAD     |   // For CreateRemoteThread
		PROCESS_VM_OPERATION      |   // For VirtualAllocEx/VirtualFreeEx
		PROCESS_VM_WRITE		  |
		PROCESS_TERMINATE,             // For WriteProcessMemory
		FALSE, FindProcess("Dsu2.exe"));
	
    if(hYourTargetProcess == NULL)
    {
        
    }else{
		//AfxMessageBox("teminate");
		TerminateProcess(hYourTargetProcess, 0);
	}
    
}

BOOL GetDebugPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;
	
	if ( ! OpenProcessToken( GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
    {
		return FALSE;
    }
	
	
	if ( ! LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &sedebugnameValue ) )
	{
		CloseHandle( hToken );
		return FALSE;
	}
	
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	
	if (!AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof tkp, NULL, NULL ) )
    {
        CloseHandle( hToken );
        return FALSE;
    }
	
    return TRUE;
}

void CPathologyApp::OnAppCamera() 
{
	//((CMedMaterial*)(theApp.GetView->m_tabCtrl.GetView(RUNTIME_CLASS(CMedMaterial))))->pNormalWord->ShowWindow(SW_HIDE);
	if(theApp.m_StationType.CompareNoCase("取材工作站")==0)
	{
		CMedMaterial* med = (CMedMaterial*)theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedMaterial));
		if(med != NULL)
		{
			CNormalWord* nw = med->pNormalWord;
			if(nw != NULL)
			{
				nw->ShowWindow(SW_HIDE);
			}
		}
	}
	
	if(theApp.m_StationType.CompareNoCase("诊断工作站")==0)
	{
		CMedDiagnose* mdg = (CMedDiagnose*)theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedDiagnose));
		if(mdg != NULL)
		{
			CNormalWord* nw = mdg->pNormalWord;
			if(nw != NULL)
			{
				nw->ShowWindow(SW_HIDE);
			}
		}
	}
	
	if(theApp.m_StationType.CompareNoCase("细胞学工作站") == 0)
	{
		CMedTbs* tbs = (CMedTbs*)theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedTbs));
		if(tbs != NULL)
		{
			CNormalWord* nw = tbs->pNormalWord;
			if(nw != NULL)
			{
				nw->ShowWindow(SW_HIDE);
			}
		}
	}
	
	if(m_CaptureMode.CompareNoCase("Nikon设备支持") == 0)
	{
		
		GetDebugPriv();
		KillProcess();
		
		AfxEnableControlContainer();
			
		CoInitialize(NULL);
		CDsControllerMainSheet dlg("采图");
		//m_pMainWnd = &dlg;
		((CStreamPage* )dlg.GetPage(2))->m_blh = theApp.GetView()->GetDocument()->m_blh;
		int nResponse = dlg.DoModal();

		CoUninitialize();

		return;
		
	}

	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		AfxMessageBox("您未打开病例，请打开后再操作！");
		return;
	}

	//if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))  return;
	
	CPathologyView *pView = GetView();
	if(pView != NULL)
	{
		CWnd *pWnd = pView->m_tabCtrl.GetView(RUNTIME_CLASS(CImageDockWnd));
		if(pWnd != NULL)  pView->m_tabCtrl.SetActiveView(pWnd);

		pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CMedImage));
		if(pWnd != NULL)  ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.SetActiveView(pWnd);

		pWnd = pView->m_tabCtrl.GetView(RUNTIME_CLASS(CMedDiagnose));
		if(pWnd && ((CMedDiagnose *)pWnd)->pZdex != NULL && ((CMedDiagnose *)pWnd)->pZdex->IsWindowVisible())
		{
			((CMedDiagnose *)pWnd)->pZdex->ShowWindow(SW_HIDE);
		}
	}

	if(theApp.GetView()->m_ImageDockWnd.GetSafeHwnd())
	{
 		CImageView *pImageView = NULL;
		CWnd *pWnd = theApp.GetView()->m_ImageDockWnd.GetWindow(GW_CHILD);
		while( pWnd != NULL )
		{
			if( pWnd->IsKindOf(RUNTIME_CLASS(CImageView)) )
			{
				pImageView = (CImageView *)pWnd;
				break;
			}
			pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
		}
		if( pImageView )
		{
			pImageView->PostMessage(WM_CONTENTREFRESH);
		}
	}

	if(m_CaptureMode.CompareNoCase("TWAIN采集设备支持") == 0)
	{
		((CMainFrame*)m_pMainWnd)->PostMessage(WM_COMMAND, ID_ACQUIRE);
	}
	else if(m_CaptureMode.CompareNoCase("Ok图像采集卡") == 0)
	{
		((CMainFrame*)m_pMainWnd)->PostMessage(WM_OKCAPTURE);
	}
	else if(m_CaptureMode.CompareNoCase("VFW采集设备支持") == 0)
	{
		((CMainFrame*)m_pMainWnd)->PostMessage(WM_VFWCAPTURE);
	}
	else if(m_CaptureMode.CompareNoCase("DH-HVxxxxU系列采集卡") == 0)
	{
		((CMainFrame*)m_pMainWnd)->PostMessage(WM_HVCAPTURE);
	}
	else if(m_CaptureMode.CompareNoCase("DirectShow采集设备支持") == 0)
	{
		((CMainFrame*)m_pMainWnd)->PostMessage(WM_DSHOWCAPTURE);
	}
	else if(m_CaptureMode.CompareNoCase("MVC3000采集卡") == 0)
	{
		((CMainFrame*)m_pMainWnd)->PostMessage(WM_MVC3000);
	}
	else if(m_CaptureMode.CompareNoCase("微软DSHOW设备") == 0)
	{
		((CMainFrame*)m_pMainWnd)->PostMessage(WM_MYDSHOW);
	}
	else if (m_CaptureMode.CompareNoCase("Canon S3/S5 IS") == 0)
	{
		((CMainFrame*)m_pMainWnd)->PostMessage(WM_CANONS3S5);
	}
/*	else if (m_CaptureMode.CompareNoCase("佳能设备支持") == 0)
	{
		CIniReader iniReader;
		CString sStatus;
		HANDLE hCanonProcess;

		iniReader.setINIFileName(theApp.m_Exe_Working_Dir + "qsa.ini");
		
		sStatus = iniReader.getKeyValue("CanonStatu","运行参数设置");
        
		//检查状态字
		if (!sStatus.CompareNoCase("0")==0)
		{
			iniReader.setKey("0", "CanonStatu","运行参数设置");
		}
		
		//传递病理号
		iniReader.setKey(theApp.GetView()->GetDocument()->m_blh,"当前病理号","运行参数设置");
		
		//检查采集进程
		hCanonProcess = OpenProcess(PROCESS_QUERY_INFORMATION |   // Required by Alpha
			PROCESS_CREATE_THREAD     |   // For CreateRemoteThread
			PROCESS_VM_OPERATION      |   // For VirtualAllocEx/VirtualFreeEx
			PROCESS_VM_WRITE		  |
			PROCESS_TERMINATE,             // For WriteProcessMemory
			FALSE, FindProcess("Canons.exe"));
		
		if (hCanonProcess == NULL)
		{
			::ShellExecute(NULL,_T("Open"),"Canons.exe",0,0,SW_SHOW);
		}
		else
		{
			AfxMessageBox("佳能相机采集程序已在运行中.");
		}
    }
	*/
}

void CPathologyApp::OnFileSave() 
{
	if(theApp.m_StationType.CompareNoCase("取材工作站")==0){
		CMedMaterial* med = (CMedMaterial*)theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedMaterial));
		if(med != NULL){
			CNormalWord* nw = med->pNormalWord;
			if(nw != NULL){
				nw->ShowWindow(SW_HIDE);
			}
		}
	}

	if(theApp.m_StationType.CompareNoCase("诊断工作站")==0){
		CMedDiagnose* mdg = (CMedDiagnose*)theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedDiagnose));
		if(mdg != NULL){
			CNormalWord* nw = mdg->pNormalWord;
			if(nw != NULL){
				nw->ShowWindow(SW_HIDE);
			}
		}
	}
	
	if(theApp.m_StationType.CompareNoCase("细胞学工作站") == 0){
		CMedTbs* tbs = (CMedTbs*)theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedTbs));
		if(tbs != NULL){
			CNormalWord* nw = tbs->pNormalWord;
			if(nw != NULL){
				nw->ShowWindow(SW_HIDE);
			}
		}
	}


	

/*	CWnd *pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CDBList));
	if(pWnd != NULL)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.SetActiveView(pWnd);
		((CEdit *)pWnd->GetDlgItem(IDC_EDIT_BLH))->SetFocus();
	}   */

	CPathologyView *pView = GetView();

	if(pView != NULL)
	{
		pView->SendMessage(WM_COMMAND, ID_FILE_SAVE);
	}
}

CPathologyView * CPathologyApp::GetView()
{
    POSITION pos = GetFirstDocTemplatePosition();
	if(pos == NULL)  return NULL;
	CSingleDocTemplate* pDocTemplate = (CSingleDocTemplate*)AfxGetApp()->GetNextDocTemplate( pos );
    if(pDocTemplate == NULL) return NULL;
    pos = pDocTemplate->GetFirstDocPosition( );
	if(pos == NULL)  return NULL;
	
	CPathologyDoc *pDoc = (CPathologyDoc *)pDocTemplate->GetNextDoc(pos);
	if(pDoc == NULL) return  NULL;

	CPathologyView *pMyview = NULL;
	pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if( pView->IsKindOf(RUNTIME_CLASS(CPathologyView)) )
		{
			pMyview = (CPathologyView *)pView;
			break;
		}
	}  

	return pMyview;
}

void CPathologyApp::OnHelp() 
{
	char fname[_MAX_PATH];
    char szFullPath[_MAX_PATH];
	char szDir[_MAX_PATH];
	char szDrive[_MAX_PATH];
	
	::GetModuleFileName(NULL,szFullPath,_MAX_PATH);
	_splitpath(szFullPath,szDrive,szDir,NULL,NULL);
	strcpy(fname,szDrive);
	strcat(fname,szDir);
	strcat(fname,"Pathology.chm");
	
    HtmlHelp(NULL, fname, HH_DISPLAY_TOPIC, 0);
}

void CPathologyApp::OnFileSyssetup() 
{
    CSysConfig mydlg;
	mydlg.DoModal();
}

void  CPathologyApp::IniLoading()
{
	CString sValue;
	CString sText;
	CIniReader m_IniReader;
	m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");

	m_DB_Type_Name = m_IniReader.getKeyValue("dbname","StudyTable");
	if(m_DB_Type_Name.IsEmpty()) m_DB_Type_Name.Format("活检库");
    CString tmp;
	tmp = m_IniReader.getKeyValue("dbno","StudyTable");
	if(tmp.IsEmpty()) tmp.Format("1");
	m_DB_Type_No = atoi(tmp);

	//////////////////////////////////////////////////////////////////////////////
	// 首先从[网络设置]查找参数
	// 许可证服务器
	// 通信端口号
	// 
	// 如果[网络设置]中没有,则从[Settings]中查找
	//
	// LicHostName
	// LicHostPort

	sText = m_IniReader.getKeyValue("许可证服务器", "网络设置");
	sText.TrimLeft();
	sText.TrimRight();

	if (!sText.IsEmpty())
	{
		m_sLicHostName = sText;
			
		sText = m_IniReader.getKeyValue("通信端口号", "网络设置");
		m_nLicHostPort = atoi(sText);
	}
	else
	{
		sText = m_IniReader.getKeyValue("LicHostName", "Settings");
		sText.TrimLeft();
		sText.TrimRight();
		m_sLicHostName = sText;

		sText   = m_IniReader.getKeyValue("LicHostPort","Settings");
		m_nLicHostPort = atoi(sText);
	}

	if (m_sLicHostName.IsEmpty())
		m_sLicHostName = "localhost";

	if (m_nLicHostPort <= 0 || m_nLicHostPort >= 65535) 
		m_nLicHostPort = 6100;
	//////////////////////////////////////////////////////////////////////////////
		
	sText = m_IniReader.getKeyValue("StainingMachineInterfaceFileName", "");
	sText.TrimLeft();
	sText.TrimRight();

	m_sStainingMachineInterfaceFileName = sText;
	if (!sText.IsEmpty()) { 
		m_sStainingMachineInterfaceModuleName = m_IniReader.getKeyValue("StainingMachineInterfaceModuleName", "");
		m_sStainingMachineInterfaceModuleName.TrimLeft();
		m_sStainingMachineInterfaceModuleName.TrimRight();
	}
	//////////////////////////////////////////////////////////////////////////////
	
	//CString m_Ip = m_IniReader.getKeyValue("DBHost","Database");
	//if(m_Ip.IsEmpty())  m_Ip.Format("127.0.0.1");
	//strcpy(ServerName, m_Ip);

	//CString m_Datebase = m_IniReader.getKeyValue("DBName","Database");
	//if(m_Datebase.IsEmpty())  m_Datebase.Format("cmips");
	//strcpy(DATABASENAME, m_Datebase);

	//CString m_User = m_IniReader.getKeyValue("DBUserId","Database");
	//if(m_User.IsEmpty())  m_User.Format("sa");
	//strcpy(DATABASE_USERNAME, m_User);

	//CString m_Pass = m_IniReader.getKeyValue("DBPassword","Database");
	//if(m_Pass.IsEmpty())  m_Pass.Format("sa");
	//strcpy(DATABASE_PASSWORD, m_Pass);

	/////////////////////////////////////////////////////////////////////

	sValue = m_IniReader.getKeyValue("HISConnectCode", "Settings");
	m_nHISConnectCode = atoi(sValue);

	m_sHISConnectFileName = m_IniReader.getKeyValue("HISConnectFileName", "Settings");
	if (m_sHISConnectFileName.IsEmpty()) m_sHISConnectFileName = "HISConnectClient.dll";

	m_sHISConnectModality = m_IniReader.getKeyValue("HISConnectModality", "Settings");

	m_sHISConnectEndPoint = m_IniReader.getKeyValue("HISConnectEndPoint", "Settings");


	// HISConnectClient1 
	sValue = m_IniReader.getKeyValue("HISConnect1Code", "Settings");
	m_nHISConnect1Code = atoi(sValue);

	m_sHISConnect1FileName = m_IniReader.getKeyValue("HISConnect1FileName", "Settings");

	m_sHISConnect1Modality = m_IniReader.getKeyValue("HISConnect1Modality", "Settings");

	m_sHISConnect1EndPoint = m_IniReader.getKeyValue("HISConnect1EndPoint", "Settings");


	m_CaptureMode = m_IniReader.getKeyValue("采集模式","Settings");
	if(m_CaptureMode.IsEmpty())  m_CaptureMode.Format("VFW采集设备支持");

	m_PrintingMode = m_IniReader.getKeyValue("打印图像反差","Settings");
	if(m_PrintingMode.IsEmpty())  m_PrintingMode.Format("高反差");

	m_PrintingMode1 = m_IniReader.getKeyValue("打印图像比例","Settings");
	if(m_PrintingMode1.IsEmpty())  m_PrintingMode1.Format("等比例输出");

	m_SavingMode = m_IniReader.getKeyValue("内部存储格式","Settings");
	if(m_SavingMode.IsEmpty())  m_SavingMode.Format("BMP");

	m_EditingMode = m_IniReader.getKeyValue("字符窗口编辑模式","Settings");
	if(m_EditingMode.IsEmpty())  m_EditingMode.Format("覆盖");

	m_SjrqTimeMode = m_IniReader.getKeyValue("送检日期设置","Settings");
	if(m_SjrqTimeMode.IsEmpty())  m_SjrqTimeMode.Format("服务器当前时间");

	m_QcrqTimeMode = m_IniReader.getKeyValue("取材日期设置","Settings");
	if(m_QcrqTimeMode.IsEmpty())  m_QcrqTimeMode.Format("服务器当前时间");

	m_BgrqTimeMode = m_IniReader.getKeyValue("报告日期设置","Settings");
	if(m_BgrqTimeMode.IsEmpty())  m_BgrqTimeMode.Format("服务器当前时间");

	m_CzysMode = m_IniReader.getKeyValue("初诊医生自动记忆","Settings");
	if(m_CzysMode.IsEmpty())  m_CzysMode.Format("否");

	m_ZzysMode = m_IniReader.getKeyValue("主诊医生自动记忆","Settings");
	if(m_ZzysMode.IsEmpty())  m_ZzysMode.Format("否");

	m_ShysMode = m_IniReader.getKeyValue("审核医生自动取当前登录者","Settings");
	if(m_ShysMode.IsEmpty())  m_ShysMode.Format("否");

	m_QcjlrMode = m_IniReader.getKeyValue("取材记录人自动取当前登录者","Settings");
	if(m_QcjlrMode.IsEmpty())  m_QcjlrMode.Format("否");

	m_SjdwMode = m_IniReader.getKeyValue("送检单位自动记忆","Settings");
	if(m_SjdwMode.IsEmpty())  m_SjdwMode.Format("否");

	m_BgrqRefreshMode = m_IniReader.getKeyValue("自动更新报告日期","Settings");
	if(m_BgrqRefreshMode.IsEmpty())  m_BgrqRefreshMode.Format("否");

	m_Waiting = m_IniReader.getKeyValue("无操作系统等待时间(分钟)","Settings");
	if(m_Waiting.IsEmpty())  m_Waiting.Format("5");

	tmp = m_IniReader.getKeyValue("lfHeight","图像文字标注字体");
	m_lfFont.lfHeight = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfWidth","图像文字标注字体");
	m_lfFont.lfWidth = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfEscapement","图像文字标注字体");
	m_lfFont.lfEscapement = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfOrientation","图像文字标注字体");
	m_lfFont.lfOrientation = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfWeight","图像文字标注字体");
	m_lfFont.lfWeight = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfItalic","图像文字标注字体");
	m_lfFont.lfItalic = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfUnderline","图像文字标注字体");
	m_lfFont.lfUnderline = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfStrikeOut","图像文字标注字体");
	m_lfFont.lfStrikeOut = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfCharSet","图像文字标注字体");
	m_lfFont.lfCharSet = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfOutPrecision","图像文字标注字体");
	m_lfFont.lfOutPrecision = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfClipPrecision","图像文字标注字体");
	m_lfFont.lfClipPrecision = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfQuality","图像文字标注字体");
	m_lfFont.lfQuality = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfPitchAndFamily","图像文字标注字体");
	m_lfFont.lfPitchAndFamily = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfFaceName","图像文字标注字体");
	strcpy(m_lfFont.lfFaceName,tmp);
	tmp = m_IniReader.getKeyValue("红色分量","图像文字标注字体");
	int r = atoi(tmp);
	tmp = m_IniReader.getKeyValue("绿色分量","图像文字标注字体");
	int g = atoi(tmp);
	tmp = m_IniReader.getKeyValue("蓝色分量","图像文字标注字体");
	int b = atoi(tmp);
	m_lfColor = RGB(r,g,b);

	m_Tag_LeftBottom  = m_IniReader.getKeyValue("图像左下角标注文字","Settings");
	m_Tag_RightBottom = m_IniReader.getKeyValue("图像右下角标注文字","Settings");
	CString m_AoiInfo = m_IniReader.getKeyValue("图像裁切区尺寸","Settings");
	m_Aoi_Width = atoi(m_AoiInfo.Left(m_AoiInfo.Find(',')));;
	m_AoiInfo = m_AoiInfo.Right(m_AoiInfo.GetLength() - m_AoiInfo.Find(',') - 1);
	m_Aoi_Height = atoi(m_AoiInfo);

	m_StationType = m_IniReader.getKeyValue("系统工作站点","Settings");
	if(m_StationType.IsEmpty())  m_StationType.Format("登记工作站");

	m_NewBlhGenMode = m_IniReader.getKeyValue("新病理号产生方式","Settings");
	if(m_NewBlhGenMode.IsEmpty())  m_NewBlhGenMode.Format("最大病理号加1");

	m_PrintRestrict = m_IniReader.getKeyValue("未审核报告允许打印","Settings");
	if(m_PrintRestrict.IsEmpty())  m_PrintRestrict.Format("是");

	m_PrintTime = m_IniReader.getKeyValue("常规报告打印限定时间(分钟)","Settings");
	if(m_PrintTime.IsEmpty()) m_PrintTime.Format("0");
 
	m_PrintAndSendMode = m_IniReader.getKeyValue("打印即发送病理报告","Settings");
	if(m_PrintAndSendMode.IsEmpty())  m_PrintAndSendMode.Format("否");

	///m_HisCfg = m_IniReader.getKeyValue("HIS连接设置","Settings");
	///if(m_HisCfg.IsEmpty())  m_HisCfg.Format("无");

	m_BggsGenMode = m_IniReader.getKeyValue("单记录打印报告选择模式","Settings");
	if(m_BggsGenMode.IsEmpty())  m_BggsGenMode.Format("自动记忆上一记录报告格式");

	m_SelfDefineWndAutoOpenMode = m_IniReader.getKeyValue("系统启动自动打开自定义窗口","Settings");
	if(m_SelfDefineWndAutoOpenMode.IsEmpty())  m_SelfDefineWndAutoOpenMode.Format("否");

	m_BgyqAntoOpenMode = m_IniReader.getKeyValue("系统启动自动打开报告延期列表","Settings");
	if(m_BgyqAntoOpenMode.IsEmpty())  m_BgyqAntoOpenMode.Format("否");

	tmp = m_IniReader.getKeyValue("外触发COM端口号","Settings");
	if(tmp.IsEmpty())
		m_ComPort = 0;
	else
		m_ComPort = atoi(tmp);

	tmp = m_IniReader.getKeyValue("图像采集放缩比例","Settings");
	if(tmp.IsEmpty())
		m_ImgZoomRate = 1.0;
	else
		m_ImgZoomRate = atof(tmp);

	m_TimeResMode = m_IniReader.getKeyValue("报告单详细时间模式","Settings");
	if(m_TimeResMode.IsEmpty())  m_TimeResMode.Format("否");

	m_MyRptMode = m_IniReader.getKeyValue("系统自动检索我的报告","Settings");
	if(m_MyRptMode.IsEmpty())  m_MyRptMode.Format("否");

	m_LoadSoundTabMode = m_IniReader.getKeyValue("装载语音管理","Settings");
	if(m_LoadSoundTabMode.IsEmpty())  m_LoadSoundTabMode.Format("否");

	m_LoadVideoTabMode = m_IniReader.getKeyValue("装载视频管理","Settings");
	if(m_LoadVideoTabMode.IsEmpty())  m_LoadVideoTabMode.Format("否");

	m_MedPersonFocusMode = m_IniReader.getKeyValue("新建记录病人信息窗口录入焦点","Settings");
	if(m_MedPersonFocusMode.IsEmpty())  m_MedPersonFocusMode.Format("姓名");

	m_StringFormatMode = m_IniReader.getKeyValue("报告单诊断信息首行缩进","Settings");
	if(m_StringFormatMode.IsEmpty())  m_StringFormatMode.Format("否");

	m_AutoRefreshDBListMode = m_IniReader.getKeyValue("自动刷新记录列表","Settings");
	if(m_AutoRefreshDBListMode.IsEmpty())  m_AutoRefreshDBListMode.Format("否");

	m_NormalSliceMode = m_IniReader.getKeyValue("常规切片说明","Settings");
	if(m_NormalSliceMode.IsEmpty())  m_NormalSliceMode.Format("无");

	m_BdSliceMode = m_IniReader.getKeyValue("冰冻切片说明","Settings");
	if(m_BdSliceMode.IsEmpty())  m_BdSliceMode.Format("冰冻");

	m_CurrentTimeMode = m_IniReader.getKeyValue("本站点当前时间模式","Settings");
	if(m_CurrentTimeMode.IsEmpty())  m_CurrentTimeMode.Format("服务器时间");

	m_TmhMode = m_IniReader.getKeyValue("玻片条码号打印紧缩模式","Settings");
	if(m_TmhMode.IsEmpty())  m_TmhMode.Format("否");

	m_BlkNumber = m_IniReader.getKeyValue("病理科科室代码","Settings");
	if(m_BlkNumber.IsEmpty())  m_BlkNumber.Format("");

	m_ImgNumShowMode = m_IniReader.getKeyValue("病例浏览显示记录对应图像数","Settings");
	if(m_ImgNumShowMode.IsEmpty())  m_ImgNumShowMode.Format("是");

	m_TransImgMode = m_IniReader.getKeyValue("提交时回传病理图像数据","Settings");
	if(m_TransImgMode.IsEmpty())  m_TransImgMode.Format("是");

	m_InitSubCode = m_IniReader.getKeyValue("蜡块初始序号","Settings");
	if(m_InitSubCode.IsEmpty())  m_InitSubCode.Format("");

	m_Default_Sex = m_IniReader.getKeyValue("性别默认值","Settings");
	if(m_Default_Sex.IsEmpty())  m_Default_Sex.Format("");

	m_BarcodePaperType = m_IniReader.getKeyValue("标签打印纸类型","Settings");
	if(m_BarcodePaperType.IsEmpty())  m_BarcodePaperType.Format("连续纸");

	m_QcysTbXr = m_IniReader.getKeyValue("取材医生同步写入","Settings");
	if(m_QcysTbXr.IsEmpty())  m_QcysTbXr.Format("无");

	m_QpxxMode = m_IniReader.getKeyValue("切片信息显示未核对切片记录","Settings");
	if(m_QpxxMode.IsEmpty())  m_QpxxMode.Format("是");

	m_CantainTsrs = m_IniReader.getKeyValue("常规制片工作站含特殊染色","Settings");
	if(m_CantainTsrs.IsEmpty())  m_CantainTsrs.Format("否");

	m_AntiNullRecordMode = m_IniReader.getKeyValue("空病例写入防护","Settings");
	if(m_AntiNullRecordMode.IsEmpty())  m_AntiNullRecordMode.Format("打开");

	m_BcbgAutoLockMode = m_IniReader.getKeyValue("补充报告自动锁定时间","Settings");
	if(m_BcbgAutoLockMode.IsEmpty())  m_BcbgAutoLockMode.Format("72");

	m_ZdDyMode = m_IniReader.getKeyValue("未诊断病例不能预览和打印","Settings");
	if(m_ZdDyMode.IsEmpty())  m_ZdDyMode.Format("是");

	m_RedValue = m_IniReader.getKeyValue("打印图像红色偏差","Settings");
	if(m_RedValue.IsEmpty())  m_RedValue.Format("0");

	m_GreenValue = m_IniReader.getKeyValue("打印图像绿色偏差","Settings");
	if(m_GreenValue.IsEmpty())  m_GreenValue.Format("0");

	m_BlueValue = m_IniReader.getKeyValue("打印图像蓝色偏差","Settings");
	if(m_BlueValue.IsEmpty())  m_BlueValue.Format("0");

	m_LsjcMode = m_IniReader.getKeyValue("历次检查记录按姓名性别检索","Settings");
	if(m_LsjcMode.IsEmpty())  m_LsjcMode.Format("是");

	m_RecyclePath = m_IniReader.getKeyValue("图像回收站文件夹","Settings");
	if(m_RecyclePath.IsEmpty())  m_RecyclePath.Format("");

	m_TjyzMode = m_IniReader.getKeyValue("特检工作站医嘱列表显示已收费未执行的记录","Settings");
	if(m_TjyzMode.IsEmpty())  m_TjyzMode.Format("否");

	m_MyReportQueryMode = m_IniReader.getKeyValue("我的报告检索方式","Settings");
	if(m_MyReportQueryMode.IsEmpty())  m_MyReportQueryMode.Format("模糊");

	m_AuthorPrintOnlyMode = m_IniReader.getKeyValue("报告打印专属权限","Settings");
	if(m_AuthorPrintOnlyMode.IsEmpty())  m_AuthorPrintOnlyMode.Format("关闭");

	m_AuthorCheckOnlyMode = m_IniReader.getKeyValue("报告审核专属权限","Settings");
	if(m_AuthorCheckOnlyMode.IsEmpty()) m_AuthorCheckOnlyMode.Format("关闭");

	m_CheckAndSendMode = m_IniReader.getKeyValue("审核并发送","Settings");
	if(m_CheckAndSendMode.IsEmpty())  m_CheckAndSendMode.Format("否");

	m_Export2ImgMode = m_IniReader.getKeyValue("报告单图像输出","Settings");
	if(m_Export2ImgMode.IsEmpty())  m_Export2ImgMode.Format("否");

	m_Export2PdfMode = m_IniReader.getKeyValue("报告单PDF输出","Settings");
	if(m_Export2PdfMode.IsEmpty())  m_Export2PdfMode.Format("否");

	m_DBType = m_IniReader.getKeyValue("DBType","Database");
	if(m_DBType.IsEmpty())  m_DBType.Format("SQLSERVER");

	m_OwnUser = m_IniReader.getKeyValue("DBOwnder","Database");

	m_PrintviewType = m_IniReader.getKeyValue("预览显示","Settings");

	m_IsUsingCard = m_IniReader.getKeyValue("是否启用读卡器","Settings");

	m_IsEmbedBoxValid = m_IniReader.getKeyValue("是否启用包埋打印", "Settings");

	m_EmbedBoxFilePath = m_IniReader.getKeyValue("包埋打印文件路径","Settings");

	m_EmbedBoxFmtCode = m_IniReader.getKeyValue("包埋打印格式代码","Settings");

	m_IsSlidePrintValid = m_IniReader.getKeyValue("是否启用玻片打印", "Settings");

	m_SlidePrintFilePath = m_IniReader.getKeyValue("玻片打印文件路径","Settings");

	m_SlidePrintFmtCode = m_IniReader.getKeyValue("玻片打印格式代码","Settings");

	m_MaterialMode = m_IniReader.getKeyValue("取材记录只有取材医师和记录人可以修改","Settings");
	if (m_MaterialMode.IsEmpty()) m_MaterialMode.Format("否");

	m_BlhMode = m_IniReader.getKeyValue("连续编号病例切换去掉病理号最后一位","Settings");
	if (m_BlhMode.IsEmpty()) m_BlhMode.Format("否");
}

void CPathologyApp::OnSetupTag() 
{
	// TODO: Add your command handler code here
    CTagShow mydlg;
	mydlg.DoModal();
}

void CPathologyApp::OnUpdateAppCamera(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(theApp.m_StationType.CompareNoCase("取材工作站") == 0 || theApp.m_StationType.CompareNoCase("诊断工作站") == 0 || theApp.m_StationType.CompareNoCase("细胞学工作站") == 0);
	if((CMainFrame*)m_pMainWnd)
		pCmdUI->SetCheck( ((CMainFrame*)m_pMainWnd)->m_realtime );
}

void CPathologyApp::OnFileOpen() 
{
	// TODO: Add your command handler code here
    if(((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
	{
		CStringArray  strlist;
		CUIntArray  rowlist;
		int nCol, nRet;
		long nRow;
		
		int rownum = ((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.GetNumberRows();
		
		nRet = ((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.EnumFirstSelected( &nCol, &nRow );
		while( nRet == UG_SUCCESS )
		{
			if( nRow >= 0 && nRow < rownum )
			{
				int rowcount = rowlist.GetSize();
				for(int i = 0; i < rowcount; i ++ )
				{
					if( nRow == rowlist.GetAt(i) )  break;
				}
				if( i >= rowcount && nRow >= 0 )
				{
					CString str_MedicalNo;
					
					CUGCell  cell;
					((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawCell(0,nRow);
					((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.GetCellIndirect(0,nRow,&cell);
					cell.GetText(&str_MedicalNo);
					
					rowlist.Add(nRow);
					strlist.Add(str_MedicalNo);
				}
			}
			
			nRet = ((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.EnumNextSelected( &nCol, &nRow );
		}
		
		if( strlist.GetSize() <= 0 )
		{
			return;
		}

		theApp.OpenDocumentFile(strlist.GetAt(0));

		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.ClearSelections();
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.Select( 0, rowlist.GetAt(0) );

		strlist.RemoveAll();	
		rowlist.RemoveAll();
	}
}

void CPathologyApp::OnAppLast() 
{
	theApp.GetView()->CloseNormalWordWnd();

	theApp.ConnectDatabase();

	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		CString curno;  curno.Empty();
		try
		{
			SAString cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where dbtype = :dbtype and dlrq IS NOT NULL order by dlrq desc";
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				cmdstr = "Select * from (Select medicalno from " + theApp.TABLE_MAIN + " where dbtype = :dbtype and dlrq IS NOT NULL order by dlrq desc) where ROWNUM<=1";
			}
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
			
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				curno = g_dbcommand.Field("medicalno").asString();
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

		if(curno.IsEmpty())
		{
			try
			{
				SAString cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where dbtype = :dbtype order by medicalno desc";
				if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
				{
					cmdstr = "Select * from (Select medicalno from " + theApp.TABLE_MAIN + " where dbtype = :dbtype order by medicalno desc) where ROWNUM<=1";
				}
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
				
				g_dbcommand.Execute();
				
				if( g_dbcommand.FetchNext() )
				{
					curno = g_dbcommand.Field("medicalno").asString();
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

		if(!curno.IsEmpty())
			theApp.OpenDocumentFile(curno);
	}
	else
	{
		CString curno = theApp.GetView()->GetDocument()->m_blh;
		curno.TrimLeft(); curno.TrimRight();
		CString lastChar;
		if(theApp.m_BlhMode.CompareNoCase("是") == 0)
		{
			lastChar = curno.Right(1);
			curno = curno.Left(curno.GetLength() - 1);			
		}
		CString str, str1;
		
		char strbuf[300];
		int Len=curno.GetLength(),k;
		BYTE ch;
		for(k=(Len-1);k>=0;k--)
		{
			ch = curno.GetAt(k); 
			if(!(ch >= 0x30 && ch <= 0x39))  break;
		}
		k = Len-1-k;
		ltoa(atol(curno.Right(k))-1,strbuf,10);
		str1.Format("%s",strbuf);
		str = curno.Left(Len-k);
		Len = k - str1.GetLength();
		for(k=0;k<Len;k++)  str += "0";
		str += str1;

		curno = str;
	//	curno += lastChar;

		int num = 0;
		CString mno;mno.Empty();
		try
		{
			SAString cmdstr = "Select medicalno from " + theApp.TABLE_MAIN + " where medicalno like '" + curno + "%' and dbtype = :dbtype";
			g_dbcommand.setCommandText( cmdstr );
			//g_dbcommand.Param("medicalno").setAsString() = curno;
			g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				mno = g_dbcommand.Field(1).asString();
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
		
		if(mno.IsEmpty())
		{
			if( ((CMainFrame*)AfxGetMainWnd())->m_realtime )
			{
				((CMainFrame*)AfxGetMainWnd())->TerminateTimer();
			}
			
			AfxMessageBox("数据库中不存在指定活检编号的登陆记录");
			return;
		}
		
		theApp.OpenDocumentFile(mno);
	}
}

void CPathologyApp::OnAppNext() 
{
	theApp.GetView()->CloseNormalWordWnd();

	theApp.ConnectDatabase();

	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		CString curno;  curno.Empty();
		try
		{
			SAString cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where dbtype = :dbtype and dlrq IS NOT NULL order by dlrq desc";
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				cmdstr = "Select * from (Select medicalno from " + theApp.TABLE_MAIN + " where dbtype = :dbtype and dlrq IS NOT NULL order by dlrq desc) where ROWNUM<=1";
			}
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
			
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				curno = g_dbcommand.Field("medicalno").asString();
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

		if(curno.IsEmpty())
		{
			try
			{
				SAString cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where dbtype = :dbtype order by medicalno desc";
				if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
				{
					cmdstr = "Select * from (Select medicalno from " + theApp.TABLE_MAIN + " where dbtype = :dbtype order by medicalno desc) where ROWNUM<=1";
				}
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
				
				g_dbcommand.Execute();
				
				if( g_dbcommand.FetchNext() )
				{
					curno = g_dbcommand.Field("medicalno").asString();
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

		if(!curno.IsEmpty())
			theApp.OpenDocumentFile(curno);
	}
	else
	{
		CString curno = theApp.GetView()->GetDocument()->m_blh;
		curno.TrimLeft(); curno.TrimRight();
		CString lastChar;
		if(theApp.m_BlhMode.CompareNoCase("是") == 0)
		{
			lastChar = curno.Right(1);
			curno = curno.Left(curno.GetLength() - 1);	
		}
		CString str, str1;

		char strbuf[300];
		int Len=curno.GetLength(),k;
		BYTE ch;
		for(k=(Len-1);k>=0;k--)
		{
			ch = curno.GetAt(k); 
			if(!(ch >= 0x30 && ch <= 0x39))  break;
		}
		k = Len-1-k;
		ltoa(atol(curno.Right(k))+1,strbuf,10);
		str1.Format("%s",strbuf);
		str = curno.Left(Len-k);
		Len = k - str1.GetLength();
		for(k=0;k<Len;k++)  str += "0";
		str += str1;

		curno = str;
		//curno += lastChar;

		int num = 0;
		CString mno;mno.Empty();
		try
		{
			SAString cmdstr = "Select medicalno from " + theApp.TABLE_MAIN + " where medicalno like '" + curno + "%' and dbtype = :dbtype";
			g_dbcommand.setCommandText( cmdstr );
			//g_dbcommand.Param("medicalno").setAsString() = curno;
			g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				mno = g_dbcommand.Field(1).asString();
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
		
		if(mno.IsEmpty())
		{
			if( ((CMainFrame*)AfxGetMainWnd())->m_realtime )
			{
				((CMainFrame*)AfxGetMainWnd())->TerminateTimer();
			}

			AfxMessageBox("数据库中不存在指定活检编号的登陆记录");
			return;
		}
		
		theApp.OpenDocumentFile(mno);
	}
}

void CPathologyApp::OnAppCheck() 
{
	if(theApp.m_ToCheck != 1)
	{
	    if(AfxMessageBox("您确认要对当前记录进行审核吗？",MB_OKCANCEL) == IDCANCEL){
		  return;
		}
	}
	
	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  
	{
		return;
	}

	CWnd* pWnd = theApp.GetView()->m_tabCtrl.GetActiveView();

	theApp.GetView()->ForceSave();
	
	theApp.ConnectDatabase();

	if(theApp.m_AuthorCheckOnlyMode.Compare("打开") == 0)
	{
		CString czys;CString zzys;CString shys;
		try
		{
			SAString cmdstr = "Select czys,zzys,shys from "+theApp.TABLE_MAIN + " where medicalno =:medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Execute();
			if(g_dbcommand.FetchNext())
			{
				czys = g_dbcommand.Field("czys").asString();
				zzys = g_dbcommand.Field("zzys").asString();
				shys = g_dbcommand.Field("shys").asString();
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
		if(!(shys.IsEmpty()) && theApp.m_Cpp_User.CompareNoCase(shys) == 0)
		{
			
		} 
		else if(shys.IsEmpty() && theApp.m_Cpp_User.CompareNoCase(zzys) == 0)
		{
			
		} 
		else 
		{
			AfxMessageBox("因报告权限控制，您不能审核此病例!");
			return;
		}
		
		
	}

	CString nZdyj;  nZdyj.Empty();
	int m_bgzt = 0;
	try
	{
		SAString cmdstr = "Select bgzt, zdyj from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			m_bgzt = g_dbcommand.Field("bgzt").asLong();
			nZdyj  = g_dbcommand.Field("zdyj").asString();
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

	if(m_bgzt & 0x08)
	{
		m_Refresh_Flag_Check = true;
		AfxMessageBox("当前报告已审核过,重复审核无效!");
		return;
	}

	if(m_bgzt < 4)
	{
		AfxMessageBox("该病例还未下诊断意见，不能审核！");
		
		m_Refresh_Flag_Check = true;
		return;
	}

	nZdyj.TrimLeft();  nZdyj.TrimRight();

	if(nZdyj.IsEmpty())
	{
		AfxMessageBox("诊断意见为空，不能审核！");
		
		m_Refresh_Flag_Check = true;
		return;
	}
	
	m_bgzt = (m_bgzt | 0x08);

	try
	{
		SAString cmdstr;
		if(theApp.m_ShysMode.CompareNoCase("是") == 0)
			cmdstr = "Update " + theApp.TABLE_MAIN + " set bgzt=:bgzt, shys=:shys where medicalno = :medicalno";
		else
			cmdstr = "Update " + theApp.TABLE_MAIN + " set bgzt=:bgzt where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);

		if(theApp.m_ShysMode.CompareNoCase("是") == 0)
		{
			g_dbcommand.Param("shys").setAsString() = theApp.m_Cpp_User;
		}

		g_dbcommand.Param("bgzt").setAsLong() = m_bgzt;
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
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

	if(theApp.m_ShysMode.CompareNoCase("是") == 0)
	{
		CWnd *pWnd = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedDiagnose));
		if(pWnd != NULL)  ((CMedDiagnose *)pWnd)->SetDlgItemText(IDC_EDIT_SHYS, theApp.m_Cpp_User);

		pWnd = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedTbs));
		if(pWnd != NULL)  ((CMedTbs *)pWnd)->SetDlgItemText(IDC_EDIT_SHYS, theApp.m_Cpp_User);
	}

	((CMainFrame *)AfxGetMainWnd())->RefreshStatusBar();

	if(theApp.m_AutoRefreshDBListMode.CompareNoCase("是") == 0 && ((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
	{
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.ResetGridSize();
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
	}

	if(theApp.m_ToCheck !=1 )
	{
		AfxMessageBox("该病例审核成功，记录已被锁定！");
	}
	theApp.m_ToCheck = 0;

	if(theApp.m_HisCfg.CompareNoCase("通用接口") == 0)
	{
		theApp.NotifyRecordStatus(theApp.GetView()->GetDocument()->m_blh);
	}

	m_Refresh_Flag_Check = true;

	// 点工具栏"修改"图标取消审核，用户输入密码成功后解锁记录的同时刷新记录
	if(	!theApp.GetView()->GetDocument()->m_blh.IsEmpty() )
	{
		CString nBlh = theApp.GetView()->GetDocument()->m_blh;
		((CMainFrame *)AfxGetMainWnd())->SendMessage(WM_COMMAND,ID_FILE_NEW);
		theApp.OpenDocumentFile(nBlh);
		theApp.GetView()->m_tabCtrl.SetActiveView(pWnd);
	}

	if(theApp.m_CheckAndSendMode.CompareNoCase("是") == 0)
	{
		((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_COMMAND, ID_APP_SEND);
	}
}

void CPathologyApp::OnUpdateAppCheck(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( RIGHT_SH(theApp.m_Cpp_Right) && (theApp.m_StationType.CompareNoCase("诊断工作站") == 0 || theApp.m_StationType.CompareNoCase("细胞学工作站") == 0));
	
    if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		pCmdUI->SetCheck(0);
		return;
	}

	if(m_Refresh_Flag_Check == false)	 return;
	m_Refresh_Flag_Check = false;

	theApp.ConnectDatabase();

	int m_bgzt = 0;
	try
	{
		SAString cmdstr = "Select bgzt from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			m_bgzt = g_dbcommand.Field("bgzt").asLong();
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

	if(m_bgzt & 0x08)   
		m_bgzt = 1;
	else
		m_bgzt = 0;
	pCmdUI->SetCheck(m_bgzt);
}

void CPathologyApp::OnAppCollect() 
{
	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  return;
	
	theApp.ConnectDatabase();

	CString  collectdoctor;  collectdoctor.Empty();
	try
	{
		SAString cmdstr = "Select collectdoctor from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			collectdoctor = g_dbcommand.Field("collectdoctor").asString();
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

	bool IsAddNewCollection = true;
	if(collectdoctor.Find(theApp.m_Cpp_User+',') >= 0)
	{
		if(AfxMessageBox("您真要取消对当前记录的收藏吗？",MB_OKCANCEL|MB_ICONQUESTION) == IDCANCEL)
		{
			m_Refresh_Flag_Collect = true;
			return;
		}

		CString t1 = collectdoctor.Left(collectdoctor.Find(theApp.m_Cpp_User+','));
		collectdoctor = collectdoctor.Right(collectdoctor.GetLength() - collectdoctor.Find(theApp.m_Cpp_User+',') - (theApp.m_Cpp_User.GetLength()+1));
		collectdoctor = t1 + collectdoctor;

		IsAddNewCollection = false;
	}
	else
	{
		collectdoctor += (theApp.m_Cpp_User+',');
	}

	try
	{
		SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set collectdoctor=:collectdoctor where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("collectdoctor").setAsString() = collectdoctor;
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
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

	if(theApp.m_AutoRefreshDBListMode.CompareNoCase("是") == 0 && ((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
	{
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.ResetGridSize();
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
	}

	if(IsAddNewCollection)
		AfxMessageBox("当前病例已成功加入您的收藏夹！");
	else
		AfxMessageBox("当前病例已成功移出您的收藏夹！");

	m_Refresh_Flag_Collect = true;
}

void CPathologyApp::OnUpdateAppCollect(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.m_StationType.CompareNoCase("制片工作站")     && 
		           theApp.m_StationType.CompareNoCase("常规切片工作站") && 
				   theApp.m_StationType.CompareNoCase("特检工作站")     &&
				   theApp.m_StationType.CompareNoCase("细胞学制片工作站"));

    if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		pCmdUI->SetCheck(0);
		return;
	}
		
	if(m_Refresh_Flag_Collect == false)	 return;
	m_Refresh_Flag_Collect = false;
	
	theApp.ConnectDatabase();
	
	CString  collectdoctor;  collectdoctor.Empty();
	try
	{
		SAString cmdstr = "Select collectdoctor from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			collectdoctor = g_dbcommand.Field("collectdoctor").asString();
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

	if(collectdoctor.Find(theApp.m_Cpp_User+',') >= 0)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CPathologyApp::OnDocClose() 
{
	((CMainFrame *)AfxGetMainWnd())->SaveModifiedRecord();
   	((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_COMMAND,ID_FILE_NEW);
}

void CPathologyApp::OnUpdateDocClose(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!theApp.GetView()->GetDocument()->m_blh.IsEmpty());
}

void CPathologyApp::ReflectDBChange(int RecordDbtype, CString RecordDbname, bool IsGenNew)
{
	m_DB_Type_Name = RecordDbname;
	m_DB_Type_No   = RecordDbtype;
	
	((CMainFrame *)AfxGetMainWnd())->RefreshLogo();
	//---------------------------------------------------------------
	CIniReader m_IniReader;
	m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");
	
	m_IniReader.setKey(m_DB_Type_Name,"dbname","StudyTable");
	CString tmp;
	tmp.Format("%d", m_DB_Type_No);
	m_IniReader.setKey(tmp,"dbno","StudyTable");
	//---------------------------------------------------------------
	CWnd *pWnd = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedDiagnose));
	if(pWnd != NULL)  ((CMedDiagnose *)pWnd)->RefreshBggs();
	
	pWnd = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedTbs));
	if(pWnd != NULL)  ((CMedTbs *)pWnd)->RefreshBggs();
	//---------------------------------------------------------------

	if(IsGenNew)
	{
		((CMainFrame *)AfxGetMainWnd())->SendMessage(WM_COMMAND,ID_FILE_NEW);
		
		pWnd = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedPerson));
		if(pWnd != NULL)
		{
			pWnd->SetDlgItemText(IDC_EDIT_BLH, "");
		}
	}
	
	if(((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
	{
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.RefreshHyperLink();
		
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.ResetGridSize();
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
		
		if(theApp.m_MyRptMode.CompareNoCase("是") == 0)
		{
			((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.OnMenuCommand(0, 0, 0, 1038);  // "我的未打印报告" 检索
		}
	}
}

void CPathologyApp::OnAppDbselect() 
{
	// TODO: Add your command handler code here
    CDbSelect mydlg;
	mydlg.m_DBName = m_DB_Type_Name;
	mydlg.m_DBNo   = m_DB_Type_No;
	
	if(mydlg.DoModal() == IDOK)
	{
		ReflectDBChange(mydlg.m_DBNo, mydlg.m_DBName);
	}
}

void CPathologyApp::LoadPhoto(CString fname, CString blh, int sno) 
{
	char instanceUID[100];

	// TODO: Add your control notification handler code here
	CString transname;
	transname.Format("%s_cmipstrans.dat", theApp.GetView()->GetDocument()->m_blh);
	CString fstr = theApp.m_SysTmpPath;
	fstr += transname;
	remove(fstr);
	
	GDALDataset * m_pSrcDataset = (GDALDataset *) GDALOpen ( fname, GA_ReadOnly );
	GDALDataset * m_pDstDataset = GS_CreateCopy( theApp.m_SavingMode, fstr, m_pSrcDataset);
	GDALClose(m_pSrcDataset);
	GDALClose(m_pDstDataset);
	
	SAString  imgdataset;
	imgdataset.Empty();
	CFile file;
	CFileException fe;
	if (file.Open(fstr, CFile::modeRead | CFile::shareDenyNone, &fe))
	{
		char sBuf[32*1024];
		while( 1 )
		{
			if( file.GetPosition() >= file.GetLength() )  break;
			DWORD nRead = file.ReadHuge(sBuf, sizeof(sBuf) );
			imgdataset += SAString( sBuf, nRead );
		}
		file.Close();
	}
	
	dcmGenerateUniqueIdentifier(instanceUID, SITE_INSTANCE_UID_ROOT);

	try
	{
		SAString cmdstr = "Insert into " + theApp.TABLE_IMAGE + "(medicalno, instanceUID, serialno, imgdataset) values(:medicalno, :instanceUID, :serialno, :imgdataset)";
		
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = blh;
		g_dbcommand.Param("instanceUID").setAsString() = instanceUID;
		g_dbcommand.Param("serialno").setAsLong()    = sno;
		g_dbcommand.Param("imgdataset").setAsBLob()  = imgdataset;
		
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
	
	imgdataset.Empty();
	
	remove(fstr);
}


void CPathologyApp::LoadSound(CString fname, CString blh, int sno) 
{
	SAString  imgdataset;
	imgdataset.Empty();
	CFile file;
	CFileException fe;
	if (file.Open(fname, CFile::modeRead | CFile::shareDenyNone, &fe))
	{
		char sBuf[32*1024];
		while( 1 )
		{
			if( file.GetPosition() >= file.GetLength() )  break;
			DWORD nRead = file.ReadHuge(sBuf, sizeof(sBuf) );
			imgdataset += SAString( sBuf, nRead );
		}
		file.Close();
	}
	
	try
	{
		SAString cmdstr = "Insert into " + theApp.TABLE_SOUND + "(medicalno, serialno, snddataset) values(:medicalno,:serialno,:snddataset)";
		
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = blh;
		g_dbcommand.Param("serialno").setAsLong()    = sno;
		g_dbcommand.Param("snddataset").setAsBLob()  = imgdataset;
		
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
	
	imgdataset.Empty();
}

void CPathologyApp::OnFileInput() 
{
	// TODO: Add your command handler code here
	static char Filter[]="病历信息文件 (*.cpi)|*.cpi||";
	CKSFileDialog MyDlg( TRUE,NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_EXPLORER ,	Filter , AfxGetMainWnd() );
	MyDlg.m_ofn.lpstrTitle = _T("病历导入");
	if( MyDlg.DoModal() == IDCANCEL )	 return;
	POSITION pos = MyDlg.GetStartPosition();
	CString strPathName = MyDlg.GetNextPathName(pos);

	CIniReader m_IniReader;
	m_IniReader.setINIFileName(strPathName);
	
	theApp.ConnectDatabase();

	CStringArray FieldNameList;
	CUIntArray   FieldTypeList;
	CStringArray FieldValuList;

	try
	{
		SAString cmdstr = "Select TOP 1 * from " + theApp.TABLE_MAIN;
		if(m_DBType.CompareNoCase("ORACLE") == 0)
		{
			cmdstr = "Select * from " + theApp.TABLE_MAIN + " where ROWNUM<=1";
		}
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();
		
		g_dbcommand.FetchNext();
		int FieldCount = g_dbcommand.FieldCount();
		for( int i = 1; i <= FieldCount; i++ )
		{
			SADataType_t  DataType = (SADataType_t) g_dbcommand.Field(i).FieldType();
			if(!( DataType == SA_dtShort   || DataType == SA_dtLong     || DataType == SA_dtBool ||
				  DataType == SA_dtDouble  || DataType == SA_dtDateTime || DataType == SA_dtString ||
				  DataType == SA_dtLongChar|| DataType == SA_dtCLob ) )
			{
				continue;
			}

			CString str;
			str = m_IniReader.getKeyValue((CString)g_dbcommand.Field(i).Name(),"病历信息");
			str.TrimLeft(); str.TrimRight();
			if(str.IsEmpty())  continue;

			FieldNameList.Add(g_dbcommand.Field(i).Name());
			FieldTypeList.Add(g_dbcommand.Field(i).FieldType());
			FieldValuList.Add(str);
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

	CString MedicalNo = CMedPerson::GenNewBlh();
    SADateTime ServerTime = theApp.GetServerDateTime();
	try
	{
		SAString cmdstr = "Insert into " + theApp.TABLE_MAIN + "(medicalno,dbtype,dlrq,bgzt) values(:medicalno,:dbtype,:dlrq,0)";
		
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = MedicalNo;
		g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
		g_dbcommand.Param("dlrq").setAsDateTime() = ServerTime;
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

	for(int i = 0; i < FieldNameList.GetSize(); i++ )
	{
		SADataType_t  DataType = (SADataType_t) FieldTypeList.GetAt(i);
		if(!( DataType == SA_dtShort   || DataType == SA_dtLong     || DataType == SA_dtBool ||
			  DataType == SA_dtDouble  || DataType == SA_dtDateTime || DataType == SA_dtString ||
		      DataType == SA_dtLongChar|| DataType == SA_dtCLob ) )
		{
			continue;
		}
		if( FieldNameList.GetAt(i).CompareNoCase("medicalno")  == 0)
		{
			continue;
		}
		
		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set " + FieldNameList.GetAt(i) + " = :" + FieldNameList.GetAt(i) + " where medicalno = :medicalno";
			
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("medicalno").setAsString() = MedicalNo;
			
			switch( DataType ) {
			case SA_dtShort:
				g_dbcommand.Param(( SAString )FieldNameList.GetAt(i)).setAsShort() = atoi(FieldValuList.GetAt(i));
				break;
			case SA_dtLong:
				g_dbcommand.Param(( SAString )FieldNameList.GetAt(i)).setAsLong() = atol(FieldValuList.GetAt(i));
				break;
			case SA_dtBool:
				if( FieldValuList.GetAt(i).Compare("是") == 0 )
				{
					g_dbcommand.Param(( SAString )FieldNameList.GetAt(i)).setAsBool() = true;
				}
				else
				{
					g_dbcommand.Param(( SAString )FieldNameList.GetAt(i)).setAsBool() = false;
				}
				break;
			case SA_dtDouble:
				g_dbcommand.Param(( SAString )FieldNameList.GetAt(i)).setAsDouble() = atof(FieldValuList.GetAt(i));
				break;
			case SA_dtDateTime:
				{	
					CString str = FieldValuList.GetAt(i);
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
					g_dbcommand.Param(( SAString )FieldNameList.GetAt(i)).setAsDateTime() = dtValue;
				}
				break;
			case SA_dtString:
				g_dbcommand.Param(( SAString )FieldNameList.GetAt(i)).setAsString() = FieldValuList.GetAt(i);
				break;
			case SA_dtLongChar:
				g_dbcommand.Param(( SAString )FieldNameList.GetAt(i)).setAsLongChar() = FieldValuList.GetAt(i);
				break;
			case SA_dtCLob:
				g_dbcommand.Param(( SAString )FieldNameList.GetAt(i)).setAsCLob() = FieldValuList.GetAt(i);
				break;
			default:;
			}				
			
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
		}
	}

	FieldNameList.RemoveAll();
	FieldTypeList.RemoveAll();
	FieldValuList.RemoveAll();

	char		m_szDrive[_MAX_DRIVE];
	char		m_szDir[_MAX_DIR];
	char		m_szFname[_MAX_FNAME];
	char		m_szExt[_MAX_EXT];
	_splitpath(strPathName, m_szDrive, m_szDir, m_szFname, m_szExt);

	CString OriginMedicalNo = m_szFname;
	CString OriginPath = m_szDrive;
	OriginPath += m_szDir;

	CString filenm;
	filenm.Format("%s_*.bmp", OriginMedicalNo);
	filenm = OriginPath + filenm;
	
	struct _finddata_t ffblk;
	long handle;
	int status;
	status = _findfirst(filenm,&ffblk);
	handle = status;
	while ( status > -1 ) 
	{
		filenm  = ffblk.name;
		CString str = filenm.Right(filenm.GetLength() - filenm.Find('_') - 1);
		str = str.Left(filenm.Find('.'));
		LoadPhoto(OriginPath + filenm, MedicalNo, atoi(str));
		status = _findnext(handle,&ffblk);
	}
	
	filenm.Format("%s_*.jpg", OriginMedicalNo);
	filenm = OriginPath + filenm;
	status = _findfirst(filenm,&ffblk);
	handle = status;
	while ( status > -1 ) 
	{
		filenm  = ffblk.name;
		CString str = filenm.Right(filenm.GetLength() - filenm.Find('_') - 1);
		str = str.Left(filenm.Find('.'));
		LoadPhoto(OriginPath + filenm, MedicalNo, atoi(str));
		status = _findnext(handle,&ffblk);
	}

	filenm.Format("%s_*.tif", OriginMedicalNo);
	filenm = OriginPath + filenm;
	status = _findfirst(filenm,&ffblk);
	handle = status;
	while ( status > -1 ) 
	{
		filenm  = ffblk.name;
		CString str = filenm.Right(filenm.GetLength() - filenm.Find('_') - 1);
		str = str.Left(filenm.Find('.'));
		LoadPhoto(OriginPath + filenm, MedicalNo, atoi(str));
		status = _findnext(handle,&ffblk);
	}

	filenm.Format("%s_*.wav", OriginMedicalNo);
	filenm = OriginPath + filenm;
	status = _findfirst(filenm,&ffblk);
	handle = status;
	while ( status > -1 ) 
	{
		filenm  = ffblk.name;
		CString str = filenm.Right(filenm.GetLength() - filenm.Find('_') - 1);
		str = str.Left(filenm.Find('.'));
		LoadSound(OriginPath + filenm, MedicalNo, atoi(str));
		status = _findnext(handle,&ffblk);
	}

	if(((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
	{
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.ResetGridSize();
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
	}
}

void CPathologyApp::OnFileMail() 
{
	// TODO: Add your command handler code here
	HINSTANCE	hLib;
	if( (hLib=LoadLibrary("SmtpSender")) == 0 )  return;
	typedef VOID (*MYPROC)(CWnd *,char *,char *); 
	MYPROC ProcInDLL;
	ProcInDLL = (MYPROC) GetProcAddress(hLib, "ShowSmtpDlg");

	if( ProcInDLL != NULL )
	{
		(ProcInDLL) (AfxGetMainWnd(),NULL,NULL); 
	}

	FreeLibrary(hLib);
	hLib = 0;
}

void CPathologyApp::OnFileOutput() 
{
	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  return;
	
	CXTBrowseDialog dlg;
	dlg.SetTitle(_T("选择输出文件目录"));
	char buf[1000];
	strcpy(buf, theApp.m_Exe_Working_Dir + "mail\\");
	dlg.SetSelPath(buf);
	if (dlg.DoModal() == IDCANCEL)   return;

	CString m_Dir = dlg.GetSelPath();
	m_Dir.TrimLeft(); 	m_Dir.TrimRight();
	if(m_Dir.GetAt(m_Dir.GetLength()-1) != '\\')
	{
		m_Dir += "\\";
	}

	CString fname = theApp.GetView()->GetDocument()->m_blh + ".cpi";
	remove(m_Dir + fname);
	
	CIniReader m_IniReader;
	m_IniReader.setINIFileName(m_Dir + fname);

	CString flist;
    flist = m_Dir + fname;

	m_IniReader.setKey(theApp.GetView()->GetDocument()->m_blh,"medicalno","病历信息");

	theApp.ConnectDatabase();

	// output basic information
	try
	{
		SAString cmdstr = "Select * from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			int FieldCount = g_dbcommand.FieldCount();
			
			for(int j = 1; j <= FieldCount; j++ )
			{
				SADataType_t  DataType = g_dbcommand.Field(j).FieldType();
				if(!( DataType == SA_dtShort   || DataType == SA_dtLong     || DataType == SA_dtBool ||
					DataType == SA_dtDouble  || DataType == SA_dtDateTime || DataType == SA_dtString ||
					DataType == SA_dtLongChar|| DataType == SA_dtCLob ) )
				{
					continue;
				}
				
				if( g_dbcommand.Field(j).Name().CompareNoCase("medicalno")  == 0)
				{
					continue;
				}
				
				CString FieldStr;	FieldStr.Empty();
				
				switch( DataType ) {
				case SA_dtShort:
					FieldStr.Format( "%d", g_dbcommand.Field(j).asShort() );
					break;
				case SA_dtLong:
					FieldStr.Format( "%d", g_dbcommand.Field(j).asLong() );
					break;
				case SA_dtBool:
					if( g_dbcommand.Field(j).asBool() )
						FieldStr.Format("是");
					else
						FieldStr.Format("否");
					break;
				case SA_dtDouble:
					FieldStr.Format( "%f", g_dbcommand.Field(j).asDouble() );
					break;
				case SA_dtDateTime:
					//FieldStr.Format("%d-%d-%d  %d:%d:%d", g_dbcommand.Field(j).asDateTime().GetYear(), g_dbcommand.Field(j).asDateTime().GetMonth(), g_dbcommand.Field(j).asDateTime().GetDay(), g_dbcommand.Field(j).asDateTime().GetHour(), g_dbcommand.Field(j).asDateTime().GetMinute(), g_dbcommand.Field(j).asDateTime().GetSecond());
					if(g_dbcommand.Field(j).asDateTime().GetYear() > 1950)
					{
						FieldStr.Format("%d-%d-%d", g_dbcommand.Field(j).asDateTime().GetYear(), g_dbcommand.Field(j).asDateTime().GetMonth(), g_dbcommand.Field(j).asDateTime().GetDay());
					}
					else
					{
						FieldStr.Empty();
					}
					break;
				case SA_dtString:
					FieldStr = g_dbcommand.Field(j).asString();
					break;
				case SA_dtBytes:
					FieldStr.Format("二进制流数据");
					break;
				case SA_dtLongBinary:
				case SA_dtBLob:
					FieldStr.Format("长二进制流数据");
					break;
				case SA_dtLongChar:
				case SA_dtCLob:
					FieldStr = g_dbcommand.Field(j).asString();
					break;
				case SA_dtUnknown:
				case SA_dtNumeric:
				case SA_dtCursor:
				case SA_dtSpecificToDBMS:
					FieldStr.Format("系统数据类型");
					break;
				default:;
				}
				
				m_IniReader.setKey(FieldStr,(CString)g_dbcommand.Field(j).Name(),"病历信息");
			}
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

	// output image

	try
	{
		SAString cmdstr = "Select * from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
		g_dbcommand.Execute();
		while(g_dbcommand.FetchNext())
		{
			SAString Bstr = g_dbcommand.Field("imgdataset").asBLob();
			if( !Bstr.IsEmpty() )
			{
				const void * binbuf = (const void *)Bstr;
				BYTE * dataptr = (BYTE *)binbuf;
				
				CString transname;
				transname.Format("%s_%d_cmipstrans.dat", theApp.GetView()->GetDocument()->m_blh, g_dbcommand.Field("serialno").asLong());
				
				CFile file;
				CFileException fe;
				
				CString fstr = theApp.m_SysTmpPath;
				fstr += transname;
				remove(fstr);
				if (file.Open( fstr , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
				{
					file.WriteHuge( dataptr , Bstr.GetLength());
					file.Close();
				}
				
				GDALDataset * m_pDataset = (GDALDataset *) GDALOpen ( fstr , GA_ReadOnly );
				CString m_file_type = GDALGetDriverShortName( m_pDataset->GetDriver() );
				GDALClose(m_pDataset);
				
				CString targetFname; targetFname.Empty();
				if(m_file_type == "BMP")
				{
					targetFname.Format("%s_%d.bmp", theApp.GetView()->GetDocument()->m_blh, g_dbcommand.Field("serialno").asLong());
				}
				if(m_file_type == "JPEG")
				{
					targetFname.Format("%s_%d.jpg", theApp.GetView()->GetDocument()->m_blh, g_dbcommand.Field("serialno").asLong());
				}
				if(m_file_type == "GTiff") 
				{
					targetFname.Format("%s_%d.tif", theApp.GetView()->GetDocument()->m_blh, g_dbcommand.Field("serialno").asLong());
				}
				
				if(!targetFname.IsEmpty())
				{
					CopyFile(fstr, m_Dir + targetFname,FALSE);

					flist += ", ";
					flist += (m_Dir + targetFname);
				}
				remove(fstr);
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

	// output sound
	try
	{
		SAString cmdstr = "Select * from " + theApp.TABLE_SOUND + "  where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
		g_dbcommand.Execute();
		while(g_dbcommand.FetchNext())
		{
			SAString Bstr = g_dbcommand.Field("snddataset").asBLob();
			if( !Bstr.IsEmpty() )
			{
				const void * binbuf = (const void *)Bstr;
				BYTE * dataptr = (BYTE *)binbuf;
				
				CString transname;
				transname.Format("%s_%d_cmipstrans.dat", theApp.GetView()->GetDocument()->m_blh, g_dbcommand.Field("serialno").asLong());
				
				CFile file;
				CFileException fe;
				
				CString fstr = theApp.m_SysTmpPath;
				fstr += transname;
				remove(fstr);
				if (file.Open( fstr , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
				{
					file.WriteHuge( dataptr , Bstr.GetLength());
					file.Close();
				}
				
				CString targetFname; targetFname.Empty();
				targetFname.Format("%s_%d.wav", theApp.GetView()->GetDocument()->m_blh, g_dbcommand.Field("serialno").asLong());

				if(!targetFname.IsEmpty())
				{
					CopyFile(fstr, m_Dir + targetFname,FALSE);

					flist += ", ";
					flist += (m_Dir + targetFname);
				}
				remove(fstr);
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


    if(AfxMessageBox("进行远程发送吗",MB_OKCANCEL|MB_ICONQUESTION) == IDOK)
	{
		HINSTANCE	hLib;
		if( (hLib=LoadLibrary("SmtpSender")) == 0 )  return;
		typedef VOID (*MYPROC)(CWnd *,char *,char *); 
		MYPROC ProcInDLL;
		ProcInDLL = (MYPROC) GetProcAddress(hLib, "ShowSmtpDlg");
		
		if( ProcInDLL != NULL )
		{
			CString title;
			title.Format("病历档案资料(病理号: %s)",theApp.GetView()->GetDocument()->m_blh);
			char *ptitle = new char[title.GetLength() + 1];
			strcpy(ptitle, title);

			flist.TrimLeft(); flist.TrimRight();
			char *pflist = new char[flist.GetLength() + 1];
			strcpy(pflist, flist);
			
			(ProcInDLL) (AfxGetMainWnd(),ptitle,pflist); 

			if(ptitle)
			{
				delete ptitle;
				ptitle = NULL;
			}
			if(pflist)
			{
				delete pflist;
				pflist = NULL;
			}
		}
		
		FreeLibrary(hLib);
		hLib = 0;
	}
}

void CPathologyApp::OnDocSaveas() 
{
	// TODO: Add your command handler code here
    CDbSelect mydlg;
	mydlg.m_DBName = m_DB_Type_Name;
	mydlg.m_DBNo   = m_DB_Type_No;
	
	if(mydlg.DoModal() == IDOK && mydlg.m_DBNo != m_DB_Type_No)
	{
		theApp.ConnectDatabase();

		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set dbtype =:dbtype where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("dbtype").setAsLong() = mydlg.m_DBNo;
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			
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

		((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_COMMAND,ID_FILE_NEW);
		
		if(((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
		{
			((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.ResetGridSize();
			((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
		}
	}
}

void CPathologyApp::OnUpdateDocSaveas(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!theApp.GetView()->GetDocument()->m_blh.IsEmpty());
}

void CPathologyApp::OnUpdateFileOpen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.m_StationType.CompareNoCase("登记工作站") == 0 || theApp.m_StationType.CompareNoCase("取材工作站") == 0 || theApp.m_StationType.CompareNoCase("诊断工作站") == 0 || theApp.m_StationType.CompareNoCase("细胞学工作站") == 0);
}

long CPathologyApp::VeriInit()
{
	//see type defination in VFPSimg.h,  You may re-order the following TYPE's according to your
	//definition of default.
	long nInterfaceType[] = { V_USB_TYPE, V_PPORT_TYPE, V_USB_SC_COMBO_TYPE, V_PPORT_SC_COMBO_TYPE, V_PCMCIA_TYPE};
	long nNumOfTypes = sizeof( nInterfaceType ) / sizeof( long );
	long i;
	long nResult;
	
	m_nFlags = VFPS_IMAGEPLUS;
	m_nSensorHandle = -1;

	//Enumerate all the possible interfaces and see which one can be
	//opened.
	//Note that testing priority can be changed by re-ordering the
	//interface types in InterfaceType[].
	for ( i = 0; i < nNumOfTypes; i++ )
	{
		nResult = VFPSselectSensor(nInterfaceType[i], 1);
		if(nResult >= 0)
		{
			m_nSensorHandle = VFPSinit(0, &m_nNumRows, &m_nNumCols, &m_nMode, NULL, m_nFlags, NULL, NULL);

			if(m_nSensorHandle > 0)
				break;
		}
	}

	if(m_nSensorHandle > 0)
	   return 0;		//initialize success
	else
	   return -1;		//initialize fail
}

void CPathologyApp::OnFileSecuritysetup() 
{
	CPassManage mydlg(AfxGetMainWnd());
	mydlg.m_UserIn_Right = theApp.m_Cpp_Right;
	mydlg.DoModal();
}

void CPathologyApp::OnUpdateFileSecuritysetup(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( RIGHT_QXGL(theApp.m_Cpp_Right) );
}

void CPathologyApp::OnUpdateFileSyssetup(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( RIGHT_XGXTSZ(theApp.m_Cpp_Right) );
}

void CPathologyApp::OnAppRefresh() 
{
	CWnd* old = theApp.GetView()->m_tabCtrl.GetActiveView();
	if(old == NULL)
	{
		old = theApp.GetView()->m_tabCtrl.GetView(0);
	}

	if(	!theApp.GetView()->GetDocument()->m_blh.IsEmpty() )
	{
		CString nBlh = theApp.GetView()->GetDocument()->m_blh;
		//QZ ((CMainFrame *)AfxGetMainWnd())->SendMessage(WM_COMMAND,ID_FILE_NEW);
		theApp.OpenDocumentFile(nBlh);
	}

	if(((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
	{
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.RefreshHyperLink();

		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.ResetGridSize();
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
	}
	
	CWnd *pWnd = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedWax));
	if(pWnd != NULL)  ((CMedWax *)pWnd)->Refresh();
	
	pWnd = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedPreslice));
	if(pWnd != NULL)  ((CMedPreslice *)pWnd)->Refresh();
	
	pWnd = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedAdvice));
	if(pWnd != NULL)  ((CMedAdvice *)pWnd)->Refresh();
	
	pWnd = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedSlice));
	if(pWnd != NULL)  ((CMedSlice *)pWnd)->Refresh();
	
	pWnd = ((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CAdviceShow));
	if(pWnd != NULL)  ((CAdviceShow *)pWnd)->RefreshHyperLink();
	
	theApp.GetView()->m_tabCtrl.SetActiveView(old);
}

void CPathologyApp::OnAppLogout() 
{
	//-----------------------------------------
	if(!theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		theApp.GetView()->ForceSave();
	}
	//-----------------------------------------

	((CMainFrame *)AfxGetMainWnd())->SendMessage(WM_COMMAND,ID_FILE_NEW);
	CWnd *pWnd = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedPerson));
	if(pWnd != NULL)
	{
		pWnd->SetDlgItemText(IDC_EDIT_BLH, "");
	}
	
    CSecurity dlg;
	dlg.m_DBName = m_DB_Type_Name;
	dlg.m_DBNo   = m_DB_Type_No;

	if( dlg.DoModal() == IDOK ) 
	{
		if(((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
		{
			((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RefreshMenuItem();
		}

		if(m_DB_Type_No != dlg.m_DBNo)
		{
			m_DB_Type_Name = dlg.m_DBName;
			m_DB_Type_No   = dlg.m_DBNo;
			
			CIniReader m_IniReader;
			m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");
			
			m_IniReader.setKey(m_DB_Type_Name,"dbname","StudyTable");
			CString tmp;
			tmp.Format("%d", m_DB_Type_No);
			m_IniReader.setKey(tmp,"dbno","StudyTable");

			CWnd *pWnd = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedDiagnose));
			if(pWnd != NULL)  ((CMedDiagnose *)pWnd)->RefreshBggs();
			
			pWnd = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedTbs));
			if(pWnd != NULL)  ((CMedTbs *)pWnd)->RefreshBggs();

			((CMainFrame *)AfxGetMainWnd())->SendMessage(WM_COMMAND,ID_FILE_NEW);

			pWnd = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedPerson));
			if(pWnd != NULL)
			{
				pWnd->SetDlgItemText(IDC_EDIT_BLH, "");
			}
		}

		((CMainFrame *)AfxGetMainWnd())->RefreshLogo();

		OnAppRefresh();

		if(theApp.m_MyRptMode.CompareNoCase("是") == 0 && ((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
		{
			((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.OnMenuCommand(0, 0, 0, 1038);  // "我的未打印报告" 检索
		}

		
		if (g_HISConnectClient.Connected())
			g_HISConnectClient.SetSenderInfo(m_Cpp_User, m_Cpp_User);			

		if (g_HISConnectClient1.Connected())
			g_HISConnectClient1.SetSenderInfo(m_Cpp_User, m_Cpp_User);			

	}
	else
	{
		((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_COMMAND, ID_APP_EXIT);
	}
}

void CPathologyApp::OnUpdateAppLast(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.m_StationType.CompareNoCase("制片工作站")     && 
		           theApp.m_StationType.CompareNoCase("常规切片工作站") && 
				   theApp.m_StationType.CompareNoCase("特检工作站")     &&
				   theApp.m_StationType.CompareNoCase("细胞学制片工作站"));
}

void CPathologyApp::OnUpdateAppNext(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.m_StationType.CompareNoCase("制片工作站")     && 
		           theApp.m_StationType.CompareNoCase("常规切片工作站") && 
				   theApp.m_StationType.CompareNoCase("特检工作站")     &&
				   theApp.m_StationType.CompareNoCase("细胞学制片工作站"));
}

void CPathologyApp::OnAppSend() 
{
	CStringArray blhList;

	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  return;

	blhList.RemoveAll();
	blhList.Add(theApp.GetView()->GetDocument()->m_blh);

	SubmitReport(blhList);

	blhList.RemoveAll();	
}

void CPathologyApp::OnUpdateAppSend(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(RIGHT_FSBG(theApp.m_Cpp_Right) && 
		           theApp.m_StationType.CompareNoCase("制片工作站")     &&
				   theApp.m_StationType.CompareNoCase("常规切片工作站") && 
				   theApp.m_StationType.CompareNoCase("特检工作站")     &&
				   theApp.m_StationType.CompareNoCase("细胞学制片工作站"));
}

void CPathologyApp::OnAppUncheck() 
{
	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  return;
	
	theApp.ConnectDatabase();

	CString nShys;  nShys.Empty();
	CString nZzys;  nZzys.Empty();
	int m_bgzt = 0;
	try
	{
		SAString cmdstr = "Select bgzt, shys, zzys from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			m_bgzt = g_dbcommand.Field("bgzt").asLong();
			nShys  = g_dbcommand.Field("shys").asString();
			nZzys  = g_dbcommand.Field("zzys").asString();
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

	if((m_bgzt & 0x08) == 0)
	{
		m_Refresh_Flag_Check = true;
		return;
	}

	nShys.TrimLeft();  nShys.TrimRight();
	nZzys.TrimLeft();  nZzys.TrimRight();

	
	if(!nShys.IsEmpty())
	{
		/*CStringArray shysList;
		while(nShys.Find("、") != -1)
		{
			shysList.Add(nShys.Left(nShys.Find("、")));
			nShys = nShys.Right(nShys.GetLength() - nShys.Find("、") - 1);
		}
		shysList.Add(nShys);
		
		for(int i1  =0; i1 < nShys.GetLength(); i1++)
		{
			AfxMessageBox(nShys.GetAt(i1));
		}*/
		
		CString nPassword;  nPassword.Empty();
		try
		{
			SAString cmdstr;
			cmdstr = "Select password from " + theApp.TABLE_SECURITY + " where username =:username";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("username").setAsString() = nShys;
			
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				nPassword = g_dbcommand.Field("password").asString();

				StrEdit mydlg("请输入审核医生 (" + nShys + ") 密码",AfxGetMainWnd());
				if(mydlg.DoModal() == IDCANCEL)  return;
				CString ps = mydlg.m_EditStr;
				ps.TrimLeft();  ps.TrimRight();
				if(ps.Compare(nPassword))
				{
					AfxMessageBox("输入密码错误!");
					m_Refresh_Flag_Check = true;
					return;
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

			m_Refresh_Flag_Check = true;
			return;
		}
	}
	else
	{
		if(!nZzys.IsEmpty())
		{
			CString nPassword;  nPassword.Empty();
			try
			{
				SAString cmdstr = "Select password from " + theApp.TABLE_SECURITY + " where username = :username";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("username").setAsString() = nZzys;
				
				g_dbcommand.Execute();
				
				if( g_dbcommand.FetchNext() )
				{
					nPassword = g_dbcommand.Field("password").asString();
					
					StrEdit mydlg("请输入主检医生 (" + nZzys + ") 密码",AfxGetMainWnd());
					if(mydlg.DoModal() == IDCANCEL)  return;
					CString ps = mydlg.m_EditStr;
					ps.TrimLeft();  ps.TrimRight();
					if(ps.Compare(nPassword))
					{
						AfxMessageBox("输入密码错误!");
						m_Refresh_Flag_Check = true;
						return;
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
				
				m_Refresh_Flag_Check = true;
				return;
			}
		}
	}

	if(!RIGHT_HQSJMM(theApp.m_Cpp_Right))
	{
		StrEdit mydlg("请输入随机密码",AfxGetMainWnd());
		if(mydlg.DoModal() == IDCANCEL)  return;
		CString ps = mydlg.m_EditStr;
		ps.TrimLeft();  ps.TrimRight();
		if(ps.Compare(((CMainFrame*)AfxGetMainWnd())->RandomCode()))
		{
			AfxMessageBox("输入密码错误!");
			m_Refresh_Flag_Check = true;
			return;
		}
	}

	int i = 0x08;  i = ~i;
//	m_bgzt = (m_bgzt & i);
	m_bgzt = 4;
	try
	{
		SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set bgzt=:bgzt where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("bgzt").setAsLong() = m_bgzt;
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
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

	if(theApp.m_HisCfg.CompareNoCase("通用接口") == 0)
	{
		theApp.NotifyRecordStatus(theApp.GetView()->GetDocument()->m_blh);
	}

	((CMainFrame *)AfxGetMainWnd())->RefreshStatusBar();

	if(theApp.m_AutoRefreshDBListMode.CompareNoCase("是") == 0 && ((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
	{
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.ResetGridSize();
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
	}

	m_Refresh_Flag_Check = true;

	// 点工具栏"修改"图标取消审核，用户输入密码成功后解锁记录的同时刷新记录
	if(	!theApp.GetView()->GetDocument()->m_blh.IsEmpty() )
	{
		CString nBlh = theApp.GetView()->GetDocument()->m_blh;
		((CMainFrame *)AfxGetMainWnd())->SendMessage(WM_COMMAND,ID_FILE_NEW);
		theApp.OpenDocumentFile(nBlh);
	}
	
	OperationLogoRecord("修改", theApp.GetView()->GetDocument()->m_blh);
}

void CPathologyApp::OnUpdateAppUncheck(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(RIGHT_SH(theApp.m_Cpp_Right) && (theApp.m_StationType.CompareNoCase("诊断工作站") == 0 || theApp.m_StationType.CompareNoCase("细胞学工作站") == 0));
}

bool CPathologyApp::IsRecordLocked(CString nBlh)
{
	if(nBlh.IsEmpty())  return false;
	
	int m_bgzt = 0;
	try
	{
		SAString cmdstr = "Select bgzt from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = nBlh;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			m_bgzt = g_dbcommand.Field("bgzt").asLong();
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

	if(m_bgzt & 0x08)
		return true;
	else
		return false;
}

void CPathologyApp::OnUpdateFileNew(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(RIGHT_DLDJGZZ(theApp.m_Cpp_Right) &&
		           theApp.m_StationType.CompareNoCase("制片工作站")     && 
		           theApp.m_StationType.CompareNoCase("常规切片工作站") && 
				   theApp.m_StationType.CompareNoCase("特检工作站")     &&
				   theApp.m_StationType.CompareNoCase("细胞学制片工作站"));
}

bool CPathologyApp::GetDatabaseValue(CString MedicalNo, CString Field, CString &Value, bool RealValue )
{
	Field.TrimLeft(); Field.TrimRight();
	if( Field.IsEmpty() )  return false;

	ConnectDatabase();
	
	try
	{
		SAString cmdstr = "Select " + Field + " from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";

		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = MedicalNo;
		
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			SAString  fn = Field;
			SADataType_t  DataType = g_dbcommand.Field(fn).FieldType();
	
			CString FieldStr;	FieldStr.Empty();

			switch( DataType ) {
			case SA_dtShort:
				FieldStr.Format( "%d", g_dbcommand.Field(fn).asShort() );
				break;
			case SA_dtLong:
				FieldStr.Format( "%d", g_dbcommand.Field(fn).asLong() );
				break;
			case SA_dtBool:
				if( g_dbcommand.Field(fn).asBool() )
					FieldStr.Format("是");
				else
					FieldStr.Format("否");
				break;
			case SA_dtDouble:
				FieldStr.Format( "%f", g_dbcommand.Field(fn).asDouble() );
				break;
			case SA_dtDateTime:
				//FieldStr.Format("%d-%d-%d  %d:%d:%d", g_dbcommand.Field(fn).asDateTime().GetYear(), g_dbcommand.Field(fn).asDateTime().GetMonth(), g_dbcommand.Field(fn).asDateTime().GetDay(), g_dbcommand.Field(fn).asDateTime().GetHour(), g_dbcommand.Field(fn).asDateTime().GetMinute(), g_dbcommand.Field(fn).asDateTime().GetSecond());
				if(g_dbcommand.Field(fn).asDateTime().GetYear() > 1950)
				{
					if(theApp.m_TimeResMode.CompareNoCase("是") == 0)
						FieldStr.Format("%d-%02d-%02d %02d:%02d", g_dbcommand.Field(fn).asDateTime().GetYear(), g_dbcommand.Field(fn).asDateTime().GetMonth(), g_dbcommand.Field(fn).asDateTime().GetDay(), g_dbcommand.Field(fn).asDateTime().GetHour(), g_dbcommand.Field(fn).asDateTime().GetMinute());
					else
						FieldStr.Format("%d-%02d-%02d", g_dbcommand.Field(fn).asDateTime().GetYear(), g_dbcommand.Field(fn).asDateTime().GetMonth(), g_dbcommand.Field(fn).asDateTime().GetDay());
				}
				else
				{
					FieldStr.Empty();
				}
				break;
			case SA_dtString:
				FieldStr = g_dbcommand.Field(fn).asString();
				break;
			case SA_dtBytes:
				FieldStr.Format("二进制流数据");
				break;
			case SA_dtLongBinary:
			case SA_dtBLob:
				FieldStr.Format("长二进制流数据");
				break;
			case SA_dtLongChar:
			case SA_dtCLob:
				if( RealValue )
					FieldStr = g_dbcommand.Field(fn).asString();
				else
					FieldStr.Format("长二进制流数据");
				break;
			case SA_dtUnknown:
			case SA_dtNumeric:
			case SA_dtCursor:
			case SA_dtSpecificToDBMS:
				FieldStr.Format("系统数据类型");
				break;
			default:;
			}
  
			Value = FieldStr;
		}
		else
		{
			return false;
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
		return false;
	}

	return true;
}

void CPathologyApp::OnReference() 
{
	CString  runstr;
	runstr = theApp.m_Exe_Working_Dir + "PDQS-R.exe";
	CString  args;
	args.Empty();
	//-----------------------------
	HANDLE launchViaCreateProcess(LPCTSTR program, LPCTSTR args);
	launchViaCreateProcess(runstr, args);
}

/////////////////////////////////////////////////////////////////////

void CPathologyApp::OnDocNewOrder()
{
	CWnd *pWnd;
	CPathologyView *pView;
	
	pView = GetView();
	if (pView == NULL) return;
	
	((CMainFrame *)AfxGetMainWnd())->SendMessage(WM_COMMAND,ID_FILE_NEW);
	
	pWnd = pView->m_tabCtrl.GetView(RUNTIME_CLASS(CMedPerson));
	if (pWnd)
	{
		pWnd->SendMessage(WM_COMMAND, MAKELONG(IDC_HISID_EXTRACT, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
	}
}


void CPathologyApp::OnUpdateDocNewOrder(CCmdUI* pCmdUI) 
{
	BOOL bEnabled;
	
	bEnabled = (theApp.m_StationType.CompareNoCase("登记工作站") == 0 || theApp.m_StationType.CompareNoCase("取材工作站") == 0 || theApp.m_StationType.CompareNoCase("诊断工作站") == 0 || theApp.m_StationType.CompareNoCase("细胞学工作站") == 0);

	pCmdUI->Enable(bEnabled);
}


void CPathologyApp::OnDocNewOrder1()
{
	CWnd *pWnd;
	CPathologyView *pView;
	
	pView = GetView();
	if (pView == NULL) return;
	
	((CMainFrame *)AfxGetMainWnd())->SendMessage(WM_COMMAND,ID_FILE_NEW);
	
	pWnd = pView->m_tabCtrl.GetView(RUNTIME_CLASS(CMedPerson));
	if (pWnd)
	{
		pWnd->SendMessage(WM_COMMAND, MAKELONG(IDC_HISID1_EXTRACT, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
	}
}


void CPathologyApp::OnUpdateDocNewOrder1(CCmdUI* pCmdUI) 
{
	BOOL bEnabled;
	
	bEnabled = (theApp.m_StationType.CompareNoCase("登记工作站") == 0 || theApp.m_StationType.CompareNoCase("取材工作站") == 0 || theApp.m_StationType.CompareNoCase("诊断工作站") == 0 || theApp.m_StationType.CompareNoCase("细胞学工作站") == 0);
	bEnabled = bEnabled && g_HISConnectClient1.Connected();
	pCmdUI->Enable(bEnabled);
}


/////////////////////////////////////////////////////////////////////

void CPathologyApp::OnDocQuickinput() 
{
//	((CMedMaterial*)theApp.GetView->m_tabCtrl.GetView(RUNTIME_CLASS(CMedMaterial)))->pNormalWord->ShowWindow(SW_HIDE);
	if(theApp.m_StationType.CompareNoCase("取材工作站")==0){
		CMedMaterial* med = (CMedMaterial*)theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedMaterial));
		if(med != NULL){
			CNormalWord* nw = med->pNormalWord;
			if(nw != NULL){
				nw->ShowWindow(SW_HIDE);
			}
		}
	}
	
	if(theApp.m_StationType.CompareNoCase("诊断工作站")==0){
		CMedDiagnose* mdg = (CMedDiagnose*)theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedDiagnose));
		if(mdg != NULL){
			CNormalWord* nw = mdg->pNormalWord;
			if(nw != NULL){
				nw->ShowWindow(SW_HIDE);
			}
		}
	}
	
	if(theApp.m_StationType.CompareNoCase("细胞学工作站") == 0){
		CMedTbs* tbs = (CMedTbs*)theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedTbs));
		if(tbs != NULL){
			CNormalWord* nw = tbs->pNormalWord;
			if(nw != NULL){
				nw->ShowWindow(SW_HIDE);
			}
		}
	}

	CPathologyView *pView = GetView();

	if(pView != NULL)
	{
		pView->PostMessage(WM_COMMAND, ID_DOC_QUICKINPUT);
	}
}

void CPathologyApp::OnUpdateDocQuickinput(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.m_StationType.CompareNoCase("登记工作站") == 0 || theApp.m_StationType.CompareNoCase("取材工作站") == 0 || theApp.m_StationType.CompareNoCase("诊断工作站") == 0 || theApp.m_StationType.CompareNoCase("细胞学工作站") == 0);
}


void CPathologyApp::SubmitReport(CStringArray &blhList)
{
	int index;
	BOOL bFound;
	CString mno;
	CString sXMLDoc("");
	CMyProgressBar *pProgressBar = NULL;
	SADateTime CurTime;
	BOOL bRetValue = FALSE;
	
	if (!g_HISConnectClient.Connected() && !g_HISConnectClient1.Connected()) return;

	CurTime= GetServerDateTime();

	pProgressBar = new CMyProgressBar(AfxGetMainWnd());
	pProgressBar->Create();
	pProgressBar->SetActiveWindow();
	pProgressBar->ShowWindow(SW_SHOW);
	pProgressBar->CenterWindow(AfxGetMainWnd());
	
	pProgressBar->m_bar.SetRange(0, blhList.GetSize());
	pProgressBar->m_bar.SetPos(0);
	pProgressBar->m_bar.SetStep(1);
	((CStatic *) pProgressBar->GetDlgItem(IDC_STATIC_MSG))->SetWindowText("请稍后...");
	

	for(index = 0; index < blhList.GetSize(); index++)
	{
		mno = blhList.GetAt(index);

		bFound = FALSE;

		try
		{
			SAString cmdstr = "Select * from " + TABLE_MAIN + " where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = mno;
			
			g_dbcommand.Execute();
			
			if( !g_dbcommand.FetchNext() )
			{
				AfxMessageBox("病理库中没有找到病理记录: " + mno);
				bFound = FALSE;
			}
			else
			{
				bFound = TRUE;
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
			continue;
		}

		if (!bFound)  continue;

		int m_bgzt = 0;
		if(!g_dbcommand.Field("bgzt").isNull())
		{
			m_bgzt = g_dbcommand.Field("bgzt").asLong();
		}

		CString nBdjg = g_dbcommand.Field("bdjg").asString();
		nBdjg.TrimLeft();  nBdjg.TrimRight();

		if(nBdjg.IsEmpty())
		{
			if(m_bgzt < 8)
			{
				AfxMessageBox("病例(" + mno + ")还未审核，不能发送！");
				continue;
			}
		}
		else
		{
			if(!RIGHT_FSBDBG(theApp.m_Cpp_Right))
			{
				AfxMessageBox("病例(" + mno + ")为冰冻报告，当前用户没有发送冰冻报告的权限！");
				continue;
			}
		}
		
		//-------------------------------------------------
		CString exam_no = g_dbcommand.Field("sqh").asString();
		exam_no.TrimLeft(); exam_no.TrimRight();
		
		if(exam_no.IsEmpty())
		{
			AfxMessageBox("申请单号为空，不能提交！");
			continue;
		}
		
		if (exam_no.Mid(0,3) == "LX:") {
			if (g_HISConnectClient1.Connected()) {
				sXMLDoc = g_HISConnectClient1.GenerateXMLDocFromDataset(g_dbcommand);
				
				bRetValue = g_HISConnectClient1.UpdateMedicalOrder(sXMLDoc, "FINISHED");
			}
		}
		else {
			if (g_HISConnectClient.Connected()) {
				sXMLDoc = g_HISConnectClient.GenerateXMLDocFromDataset(g_dbcommand);
				// status = 1 预约 4 检查中  6 完成
				bRetValue = g_HISConnectClient.UpdateMedicalOrder("6", sXMLDoc);
			}
		}
		// 生成PDF 报告文件
		if (bRetValue)
		{
			
		}
		
		/*
		try
		{
			SAString cmdstr;
			if(ExistNum == 0)
			  cmdstr = "Insert into EXAM.EXAM_REPORT (EXAM_NO,EXAM_PARA,DESCRIPTION,IMPRESSION) values(:EXAM_NO,:EXAM_PARA,:DESCRIPTION,:IMPRESSION)";
			else
			  cmdstr = "Update EXAM.EXAM_REPORT set EXAM_PARA=:EXAM_PARA,DESCRIPTION=:DESCRIPTION,IMPRESSION=:IMPRESSION where EXAM_NO=:EXAM_NO";
			pacscmd.setCommandText(cmdstr);
			pacscmd.Param("EXAM_NO").setAsString() = exam_no;
			pacscmd.Param("EXAM_PARA").setAsString() = g_dbcommand.Field("bbmc").asString();
			CString str; str.Empty();
			char enter[3];
			enter[0] = 0x0d; enter[1]= 0x0a; enter[2] = 0x00;
			if(!g_dbcommand.Field("rysj").asString().IsEmpty())
			{
				str += "[肉眼所见]";                 str += enter;
				str += g_dbcommand.Field("rysj").asString(); str += enter;
			}
			if(!g_dbcommand.Field("bdjg").asString().IsEmpty())
			{
				str += "[冰冻结果]";                  str += enter;
				str += g_dbcommand.Field("bdjg").asString();  str += enter;
				if(!g_dbcommand.Field("bdbgys").asString().IsEmpty())
				{
					str += "[冰冻报告医师]";                str += enter;
					str += g_dbcommand.Field("bdbgys").asString();  str += enter;
				}
				if(!g_dbcommand.Field("bdbgrq").isNull())
				{
					COleDateTime bdbgrq;
					SADateTime st = g_dbcommand.Field("bdbgrq").asDateTime();
					bdbgrq.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
					str += "[冰冻报告日期]";                    str += enter;
					str += bdbgrq.Format("%Y-%m-%d %H:%M:%S");  str += enter;
				}
			}
			if(!g_dbcommand.Field("gjsj").asString().IsEmpty())
			{
				str += "[光镜所见]";                  str += enter;
				str += g_dbcommand.Field("gjsj").asString();  str += enter;
			}
			pacscmd.Param("DESCRIPTION").setAsString() = str;
			pacscmd.Param("IMPRESSION").setAsString() = g_dbcommand.Field("zdyj").asString();
			
			pacscmd.Execute();
			pacscon.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				pacscon.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
			continue;
		}
		
		*/


		//-------------------------------------------------
		m_bgzt = (m_bgzt | 0x20);
		
		try
		{
			SAString cmdstr = "Update " + TABLE_MAIN + " set bgzt=:bgzt where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("bgzt").setAsLong() = m_bgzt;
			g_dbcommand.Param("medicalno").setAsString() = mno;
			
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
		//-------------------------------------------------

		// if(!IsTransferImg)  continue;

		//------------------------------------------------------------------
		CString msg;
		msg.Format("%d / %d", index + 1, blhList.GetSize());
		((CStatic *) pProgressBar->GetDlgItem(IDC_STATIC_VALUE))->SetWindowText(msg);
		pProgressBar->m_bar.StepIt();
		//------------------------------------------------------------------
	}

	if (pProgressBar != NULL)
	{
		pProgressBar->DestroyWindow();
		SAFE_DELETE(pProgressBar);
	}

	((CMainFrame *)AfxGetMainWnd())->RefreshStatusBar();

	if(theApp.m_AutoRefreshDBListMode.CompareNoCase("是") == 0 && ((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
	{
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.ResetGridSize();
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
	}
}


CString CPathologyApp::GetNewInstancdUID(void)						
{
	//MedViewer产品的Implementation Class UID
	CString strMedViewerUID;
	strMedViewerUID = "1.2.26.3680043.2.428.2.1.1";
	//时间
	CString strDateTime;
	strDateTime = COleDateTime::GetCurrentTime().Format("%Y%m%d%H%M%S");
	//随即数
	CString strRandomStr;
	strRandomStr.Format( ".%04d.%04d", rand()%9999, rand()%9999 );
	return strMedViewerUID+strDateTime+strRandomStr ;
}

COleDateTime CPathologyApp::GetOleServerDateTime()
{
	SADateTime st = GetServerDateTime();
	COleDateTime curt;
	curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());

	return curt;
}

SADateTime CPathologyApp::GetServerDateTime()
{
	SADateTime curt = SADateTime::currentDateTime();

	if(theApp.m_CurrentTimeMode.CompareNoCase("服务器时间") == 0)
	{
		// 本站点当前时间模式取：服务器时间
		//-------  Retrieve Server Machine Time  -------------
		if(m_DBType.CompareNoCase("SQLSERVER") == 0)
		{
			theApp.ConnectDatabase();
			try
			{
				SAString cmdstr = "SELECT GETDATE()";
				g_dbcommand.setCommandText( cmdstr );
				g_dbcommand.Execute();
				
				if(g_dbcommand.FetchNext())
				{
					if(g_dbcommand.Field(1).FieldType() == SA_dtDateTime)
					{
						curt = g_dbcommand.Field(1).asDateTime();
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
		}

		if(m_DBType.CompareNoCase("ORACLE") == 0)
		{
			theApp.ConnectDatabase();
			try
			{
				SAString cmdstr = "SELECT SYSDATE FROM DUAL";
				g_dbcommand.setCommandText( cmdstr );
				g_dbcommand.Execute();
				
				if(g_dbcommand.FetchNext())
				{
					if(g_dbcommand.Field(1).FieldType() == SA_dtDateTime)
					{
						curt = g_dbcommand.Field(1).asDateTime();
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
		}
	}

	return curt;
}

void CPathologyApp::OperationLogoRecord(CString optype, CString blh)
{
	if( !isLogoTableExist )  return;
	ConnectDatabase();

	CString content; content.Empty();
	//-------------------------------------------------
	// output basic information

	content += ("病理号: " + blh);
	char enter[3];
	enter[0] = 0x0d; enter[1]= 0x0a; enter[2] = 0x00;

	try
	{
		SAString cmdstr = "Select * from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = blh;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			int FieldCount = g_dbcommand.FieldCount();
			
			for(int j = 1; j <= FieldCount; j++ )
			{
				SADataType_t  DataType = g_dbcommand.Field(j).FieldType();
				if(!( DataType == SA_dtShort   || DataType == SA_dtLong     || DataType == SA_dtBool ||
					  DataType == SA_dtDouble  || DataType == SA_dtDateTime || DataType == SA_dtString ||
				      DataType == SA_dtLongChar|| DataType == SA_dtCLob ) )
				{
					continue;
				}
				
				if( g_dbcommand.Field(j).Name().CompareNoCase("medicalno")  == 0)
				{
					continue;
				}
				
				CString FieldStr;	FieldStr.Empty();
				
				switch( DataType ) {
				case SA_dtShort:
					FieldStr.Format( "%d", g_dbcommand.Field(j).asShort() );
					break;
				case SA_dtLong:
					FieldStr.Format( "%d", g_dbcommand.Field(j).asLong() );
					break;
				case SA_dtBool:
					if( g_dbcommand.Field(j).asBool() )
						FieldStr.Format("是");
					else
						FieldStr.Format("否");
					break;
				case SA_dtDouble:
					FieldStr.Format( "%f", g_dbcommand.Field(j).asDouble() );
					break;
				case SA_dtDateTime:
					//FieldStr.Format("%d-%d-%d  %d:%d:%d", g_dbcommand.Field(j).asDateTime().GetYear(), g_dbcommand.Field(j).asDateTime().GetMonth(), g_dbcommand.Field(j).asDateTime().GetDay(), g_dbcommand.Field(j).asDateTime().GetHour(), g_dbcommand.Field(j).asDateTime().GetMinute(), g_dbcommand.Field(j).asDateTime().GetSecond());
					if(g_dbcommand.Field(j).asDateTime().GetYear() > 1950)
					{
						FieldStr.Format("%d-%d-%d", g_dbcommand.Field(j).asDateTime().GetYear(), g_dbcommand.Field(j).asDateTime().GetMonth(), g_dbcommand.Field(j).asDateTime().GetDay());
					}
					else
					{
						FieldStr.Empty();
					}
					break;
				case SA_dtString:
					FieldStr = g_dbcommand.Field(j).asString();
					break;
				case SA_dtBytes:
					FieldStr.Format("二进制流数据");
					break;
				case SA_dtLongBinary:
				case SA_dtBLob:
					FieldStr.Format("长二进制流数据");
					break;
				case SA_dtLongChar:
				case SA_dtCLob:
					FieldStr = g_dbcommand.Field(j).asString();
					break;
				case SA_dtUnknown:
				case SA_dtNumeric:
				case SA_dtCursor:
				case SA_dtSpecificToDBMS:
					FieldStr.Format("系统数据类型");
					break;
				default:;
				}
				
				CString FieldName = g_dbcommand.Field(j).Name();
				for(int k = 0; k < theApp.m_fnamelist.GetSize(); k++)
				{
					if(theApp.m_fnamelist.GetAt(k).CompareNoCase(g_dbcommand.Field(j).Name()) == 0)
					{
						FieldName = theApp.m_fdescriptionlist.GetAt(k);
						break;
					}
				}

				FieldStr.TrimLeft();  FieldStr.TrimRight();
				if(!FieldStr.IsEmpty())
				{
					content += enter;
					content += (FieldName + ": " + FieldStr);
				}
			}
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
	//-------------------------------------------------

	SADateTime curt = theApp.GetServerDateTime();

	try
	{
		SAString cmdstr = "Insert into " + theApp.TABLE_LOGO + " (optype,blh,content,optime,person) values(:optype,:blh,:content,:optime,:person)";
		g_dbcommand.setCommandText( cmdstr );
		
		g_dbcommand.Param("optype").setAsString()   = optype;
		g_dbcommand.Param("blh").setAsString()      = blh;
		g_dbcommand.Param("content").setAsString()  = content;
		g_dbcommand.Param("optime").setAsDateTime() = curt;
		g_dbcommand.Param("person").setAsString()   = theApp.m_Cpp_User;
		
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

	content.Empty();
}

BOOL CPathologyApp::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN || 
	   pMsg->message == WM_MOUSEMOVE || 
	   pMsg->message == WM_LBUTTONDOWN || 
	   pMsg->message == WM_RBUTTONDOWN ||
	   pMsg->message == WM_LBUTTONDBLCLK ||
	   pMsg->message == WM_RBUTTONDBLCLK)
	{
	
		CWnd * pMainWnd = AfxGetMainWnd();
		if(pMainWnd && pMainWnd->IsKindOf(RUNTIME_CLASS(CMainFrame)))
		{
			if(((CMainFrame *)pMainWnd)->m_Close_TimerHandle)
			{
				((CMainFrame *)AfxGetMainWnd())->m_LastTime = COleDateTime::GetCurrentTime();
			}
		}

		if(pMainWnd && 
		   pMainWnd->IsKindOf(RUNTIME_CLASS(CMainFrame)) && 
		   pMsg->message == WM_LBUTTONDOWN)
		{
			if(((CMainFrame*)AfxGetMainWnd())->m_realtime)
			{
				CImageView *pImageView = NULL;
				CWnd *pWnd = theApp.GetView()->m_ImageDockWnd.GetWindow(GW_CHILD);
				while( pWnd != NULL )
				{
					if( pWnd->IsKindOf(RUNTIME_CLASS(CImageView)) )
					{
						pImageView = (CImageView *)pWnd;
						break;
					}
					pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
				}
				if( pImageView && pImageView->m_hWnd == GetFocus())
				{
					CPoint pt;
					GetCursorPos(&pt);
					pImageView->ScreenToClient(&pt);

					CRect rt;
					pImageView->GetClientRect(rt);
					if(rt.PtInRect(pt))
					{
						pImageView->SendMessage(WM_LBUTTONDOWN);
						return TRUE;
					}
				}
			}
		}

		if(pMainWnd && 
		   pMainWnd->IsKindOf(RUNTIME_CLASS(CMainFrame)) && 
		   pMsg->message == WM_RBUTTONDOWN)
		{
			if(((CMainFrame*)AfxGetMainWnd())->m_realtime)
			{
				CImageView *pImageView = NULL;
				CWnd *pWnd = theApp.GetView()->m_ImageDockWnd.GetWindow(GW_CHILD);
				while( pWnd != NULL )
				{
					if( pWnd->IsKindOf(RUNTIME_CLASS(CImageView)) )
					{
						pImageView = (CImageView *)pWnd;
						break;
					}
					pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
				}
				if( pImageView && pImageView->m_hWnd == GetFocus())
				{
					CPoint pt;
					GetCursorPos(&pt);
					pImageView->ScreenToClient(&pt);

					CRect rt;
					pImageView->GetClientRect(rt);
					if(rt.PtInRect(pt))
					{
						pImageView->SendMessage(WM_RBUTTONUP);
						return TRUE;
					}
				}
			}
		}

		if(pMainWnd && pMainWnd->IsKindOf(RUNTIME_CLASS(CMainFrame)) && pMsg->message == WM_KEYDOWN)
		{
			UINT nChar = (UINT)pMsg->wParam;
			if(nChar == VK_F2)
			{
				if(theApp.m_IsUsingCard.CompareNoCase("是") == 0)
				{
					if(theApp.m_StationType.CompareNoCase("登记工作站") == 0 || theApp.m_StationType.CompareNoCase("取材工作站") == 0 || theApp.m_StationType.CompareNoCase("诊断工作站") == 0)
					{
						((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_COMMAND,ID_FILE_NEW);
					}
					else
					{
						((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_COMMAND,ID_FILE_NEW);
					}
				}
				else
				{
					((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_COMMAND,ID_FILE_NEW);
				}
				
			}
			
			if(nChar == VK_F3)
			{
				((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_COMMAND,ID_FILE_SAVE);
			}
			
			if(nChar == VK_F4)
			{
				((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_COMMAND,ID_APP_CAMERA);
			}
			
			if(nChar == VK_F5)
			{
				((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_COMMAND,ID_APP_LAST);
			}
			
			if(nChar == VK_F6)
			{
				((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_COMMAND,ID_APP_NEXT);
			}

			if(nChar == VK_F7 && theApp.m_CaptureMode.CompareNoCase("微软DSHOW设备") == 0 && ((CMainFrame*)AfxGetMainWnd())->m_realtime)
			{
				DSHOW_FullScreen(TRUE);
			}

			if(nChar == VK_ESCAPE && theApp.m_CaptureMode.CompareNoCase("微软DSHOW设备") == 0 && ((CMainFrame*)AfxGetMainWnd())->m_realtime)
			{
				DSHOW_FullScreen(FALSE);
			}

			if(nChar == VK_SPACE && ((CMainFrame*)AfxGetMainWnd())->m_realtime)
			{
				((CMainFrame *)AfxGetMainWnd())->SendMessage(WM_SWITCHON);
			}
		}
	}
	
	return CWinApp::PreTranslateMessage(pMsg);
}

void CPathologyApp::OnAppNotepad() 
{
	CNotePad mydlg;
	mydlg.DoModal();
}

void CPathologyApp::RemoveOldNotepad()
{
	ConnectDatabase();

	COleDateTime curt = GetOleServerDateTime();

	COleDateTimeSpan tspan;
	tspan.SetDateTimeSpan(15, 0, 0, 0);
	curt -= tspan;
	CString m_WhereBetween_Cmd;
	m_WhereBetween_Cmd.Format(" where fbsj < '%s'", curt.Format("%Y-%m-%d 00:00:00"));
	if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
	{
		m_WhereBetween_Cmd.Format(" where fbsj < to_date('%s','yyyy-mm-dd hh24:mi:ss')", curt.Format("%Y-%m-%d 00:00:00"));
	}

	try
	{
		SAString cmdstr = "Delete from " + theApp.TABLE_NOTEPAD + m_WhereBetween_Cmd;
		g_dbcommand.setCommandText(cmdstr);
		
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

void CPathologyApp::OnFileRptcopy() 
{
	CPathologyView *pView = GetView();

	if(pView != NULL)
	{
		pView->PostMessage(WM_COMMAND, ID_FILE_RPTCOPY);
	}
}

void CPathologyApp::OnFileRptcopyall() 
{
	CPathologyView *pView = GetView();

	if(pView != NULL)
	{
		pView->PostMessage(WM_COMMAND, ID_FILE_RPTCOPYALL);
	}
}

void CPathologyApp::OnFileRptcopyimg() 
{
	CPathologyView *pView = GetView();

	if(pView != NULL)
	{
		pView->PostMessage(WM_COMMAND, ID_FILE_RPTCOPYIMG);
	}
}

void CPathologyApp::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.m_StationType.CompareNoCase("制片工作站")     && 
		           theApp.m_StationType.CompareNoCase("常规切片工作站") && 
				   theApp.m_StationType.CompareNoCase("特检工作站")     &&
				   theApp.m_StationType.CompareNoCase("细胞学制片工作站"));
}

void CPathologyApp::OnUpdateFilePrintPreview(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.m_StationType.CompareNoCase("制片工作站")     && 
		           theApp.m_StationType.CompareNoCase("常规切片工作站") && 
				   theApp.m_StationType.CompareNoCase("特检工作站")     &&
				   theApp.m_StationType.CompareNoCase("细胞学制片工作站"));
}

BOOL CPathologyApp::OnIdle(LONG lCount) 
{
	if(m_pMainWnd->GetSafeHwnd() != NULL && m_pMainWnd->IsKindOf(RUNTIME_CLASS(CMainFrame)))
	{
		if(((CMainFrame*)m_pMainWnd)->m_wndTabDockBar.GetSafeHwnd() != NULL && !((CMainFrame*)m_pMainWnd)->m_wndTabDockBar.IsWindowVisible())
		{
			((CMainFrame*)m_pMainWnd)->SendMessage(WM_COMMAND, ID_VIEW_MEDLIST);
		}
		if(((CMainFrame*)m_pMainWnd)->m_wndTabDockBar.GetSafeHwnd() != NULL && ((CMainFrame*)m_pMainWnd)->m_wndTabDockBar.IsFloating())
		{
			((CMainFrame*)m_pMainWnd)->DockControlBar(&((CMainFrame*)m_pMainWnd)->m_wndTabDockBar, AFX_IDW_DOCKBAR_RIGHT);
		}
	}
	
	return CWinApp::OnIdle(lCount);
}

/*
// 广西CA认证
CString CPathologyApp::GetCAUser()
{
	// 广西CA认证
	IiTrusPTA gpta;
	ICertificates m_certs;

	gpta.CreateDispatch("PTA.iTrusPTA");
	m_certs.AttachDispatch(gpta.GetMyCertificates());

	CString causer; causer.Empty();
	if(m_certs.GetCount() < 1)
	{
		//AfxMessageBox("没有找到CA认证证书！");
		m_certs.ReleaseDispatch();
		gpta.ReleaseDispatch();
		return causer;
	}
	ICertificate cert = m_certs.GetItem(0).pdispVal;//获取当前证书
	causer = cert.GetSubject();
	causer = causer.Right(causer.GetLength() - causer.Find("OU=EmployeeID -") - 15);
	causer = causer.Left(causer.Find(','));
	causer.TrimLeft(); causer.TrimRight();

	m_certs.ReleaseDispatch();
	gpta.ReleaseDispatch();
	return causer;
}

*/


bool  CPathologyApp::Export2Image(CFODataModel *pFODataModel, CString Fname)
{
	if(pFODataModel == NULL)  return false;

	if(TRUE == pFODataModel->IsDesignMode())
	{
		pFODataModel->SetDesignMode(FALSE);
		pFODataModel->NotifyObserver(FO_HINT_UPDATE_WINDOW,NULL);
	}

	AfxGetApp()->BeginWaitCursor();
	CFOBitmap dib;
	
	BOOL bSuccess = FALSE;
	CDC dcMem;
	CBitmap bmp;
	CDC dcScreen;
	
	dcScreen.Attach(::GetDC(0));
	
	CDC* pDC = &dcScreen;
	
	dcMem.CreateCompatibleDC(pDC);
	
	CRect rcForm = pFODataModel->GetPagePosition();
	CSize szBitmap(rcForm.Width(), rcForm.Height());
	
	BOOL bBmpCreated = bmp.CreateCompatibleBitmap(pDC, szBitmap.cx, szBitmap.cy);
	
	if (bBmpCreated)
	{
		dcMem.SelectObject(&bmp);
		
		CRect rect = rcForm;
		if(dcMem.IsPrinting())
		{
			// Fill background with APPWORKSPACE
			CBrush backBrush(RGB(255,255,255));
			CBrush* pOldBrush = dcMem.SelectObject(&backBrush);
			
			dcMem.PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
			dcMem.SelectObject(pOldBrush);
		}
		else
		{
			// Fill background with APPWORKSPACE
			CBrush backBrush(pFODataModel->GetPageBkColor());
			CBrush* pOldBrush = dcMem.SelectObject(&backBrush);
			
			dcMem.PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
			dcMem.SelectObject(pOldBrush);
		}
		
		pFODataModel->SelectForm(TRUE);
		
		pFODataModel->OnDrawBack(&dcMem);
	
		pFODataModel->OnDraw(&dcMem);
		
		dib.MakeNewBitmap(&dcMem, &bmp);
		
		bSuccess = TRUE;
	}
	
	dcScreen.Detach();
	
	if(FALSE == bSuccess) return FALSE;

	CString fstr = theApp.m_SysTmpPath;
	fstr += "export2bmp.bmp";
	remove(fstr);

	bSuccess = dib.Save(fstr);
	AfxGetApp()->EndWaitCursor();

	if(bSuccess)
	{
		if(theApp.m_SavingMode.CompareNoCase("BMP") == 0)
		{
			CopyFile(fstr, Fname,FALSE);
		}
		else
		{
			GDALDataset * m_pSrcDataset = (GDALDataset *) GDALOpen ( fstr, GA_ReadOnly );
			GDALDataset * m_pDstDataset = GS_CreateCopy( theApp.m_SavingMode, Fname, m_pSrcDataset);
			GDALClose(m_pSrcDataset);
			GDALClose(m_pDstDataset);
		}
	}

	remove(fstr);
	return bSuccess;
}

#include "pdflib.h"
bool CPathologyApp::Export2PDF(CString ImgFname, CString PDFFname)
{
    PDF *p;
    int image;
	
    /* This is where font/image/PDF input files live. Adjust as necessary. */
    //char *searchpath = "../data";
	
    /* create a new PDFlib object */
    if ((p = PDF_new()) == (PDF *) 0)
    {
        printf("Couldn't create PDFlib object (out of memory)!\n");
        return false;
    }
	
	PDF_set_parameter(p, "license", "X600605-009100-4673AA-96A763");
	
    PDF_TRY(p)
	{
		if (PDF_begin_document(p, PDFFname, 0, "") == -1) {
			printf("Error: %s\n", PDF_get_errmsg(p));
			return false;
		}
		
		//PDF_set_parameter(p, "SearchPath", searchpath);
		
		/* This line is required to avoid problems on Japanese systems */
		PDF_set_parameter(p, "hypertextencoding", "host");
		
		PDF_set_info(p, "Creator", "CPI");
		PDF_set_info(p, "Author",  "Ming Delie");
		PDF_set_info(p, "Title",   "JPEG2PDF");
		
		image = PDF_load_image(p, "auto", ImgFname, 0, "");
		
		if (image == -1) {
			printf("Error: %s\n", PDF_get_errmsg(p));
			return false;
		}
		
		/* dummy page size, will be adjusted by PDF_fit_image() */
		PDF_begin_page_ext(p, 10, 10, "");
		PDF_fit_image(p, image, 0.0, 0.0, "adjustpage");
		PDF_close_image(p, image);
		PDF_end_page_ext(p, "");
		
		PDF_end_document(p, "");
    }
	PDF_CATCH(p) {
        printf("PDFlib exception occurred in image sample:\n");
        printf("[%d] %s: %s\n",
			PDF_get_errnum(p), PDF_get_apiname(p), PDF_get_errmsg(p));
        PDF_delete(p);
        return false;
    }
	
    PDF_delete(p);
	
	return true;
}

void CPathologyApp::OnReportOutput() 
{
	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  return;

	static char Filter[]="PDF文件 (*.pdf)|*.pdf|JPEG文件格式 (*.jpg)|*.jpg||";
	CKSFileDialog MyDlg( FALSE,"pdf", NULL, OFN_EXTENSIONDIFFERENT| OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_LONGNAMES | OFN_EXPLORER ,	Filter , AfxGetMainWnd() );
	MyDlg.m_ofn.lpstrTitle = _T("报告单输出");
	if( MyDlg.DoModal() == IDCANCEL )	 return;
	POSITION pos = MyDlg.GetStartPosition();
	CString strPathName = MyDlg.GetNextPathName(pos);

	CString extstr = MyDlg.GetFileExt();
	extstr.MakeUpper();

	SAString blobdateset; blobdateset.Empty();
	if(extstr == "PDF")
	{
		try
		{
			SAString cmdstr = "Select export2pdf from " + theApp.TABLE_MAIN + "  where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			
			g_dbcommand.Execute();
			if(g_dbcommand.FetchNext())
			{
				blobdateset = g_dbcommand.Field("export2pdf").asBLob();
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
	else
	{
		try
		{
			SAString cmdstr = "Select export2img from " + theApp.TABLE_MAIN + "  where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			
			g_dbcommand.Execute();
			if(g_dbcommand.FetchNext())
			{
				blobdateset = g_dbcommand.Field("export2img").asBLob();
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

	if( !blobdateset.IsEmpty() )
	{
		const void * binbuf = (const void *)blobdateset;
		BYTE * dataptr = (BYTE *)binbuf;
		
		CFile file;
		CFileException fe;
		
		if (file.Open( strPathName , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
		{
			file.WriteHuge( dataptr , blobdateset.GetLength());
			file.Close();
		}
	}

	blobdateset.Empty();
}


void CPathologyApp::NotifyRecordStatus(CString medicalno)
{
	try
	{
		SAString cmdstr = "Select * from " + TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = medicalno;
		
		g_dbcommand.Execute();
		
		if( !g_dbcommand.FetchNext() )
		{
			AfxMessageBox("病理库中没有找到病理记录: " + medicalno);
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
	

	HINSTANCE	hLib;
	if( (hLib=LoadLibrary(theApp.m_Exe_Working_Dir + "commInterface.dll")) == 0 )
	{
		AfxMessageBox("commInterface.dll装载错误！");
		return;
	}
	
	typedef int (*pNotifyRecordStatus)(char *); 
	pNotifyRecordStatus ProcInDLL;
	ProcInDLL = (pNotifyRecordStatus) GetProcAddress(hLib, "NotifyRecordStatus");
				
	if( ProcInDLL == NULL )
	{
		AfxMessageBox("NotifyRecordStatus函数定位错误！");
		return;
	}
	
	////mkdir("C:\\MingDL");

	
	int m_bgzt = 0;
	if(!g_dbcommand.Field("bgzt").isNull())
	{
		m_bgzt = g_dbcommand.Field("bgzt").asLong();
	}
	
	CString ztinfo; ztinfo.Empty();
	
	if(g_dbcommand.Field("bgzt").isNull() || m_bgzt < 1)  ztinfo.Format("已登记");
	if(m_bgzt >= 1  && m_bgzt < 2)   ztinfo.Format("已取材");
	if(m_bgzt >= 2  && m_bgzt < 4)   ztinfo.Format("已延期");
	if(m_bgzt >= 4  && m_bgzt < 8)   ztinfo.Format("已诊断");
	if(m_bgzt >= 8  && m_bgzt < 16)  ztinfo.Format("已审核");
	if(m_bgzt >= 16 && m_bgzt < 32)  ztinfo.Format("已打印");
	if(m_bgzt >= 32)                 ztinfo.Format("已发送");
	
	//  generate XML disk file
	//-------------------------------------------------
	TiXmlDocument xml;//输出结果的用XML文件组织起来
	TiXmlDeclaration *declaration = new TiXmlDeclaration("1.0", "gb2312", "yes");;
	xml.LinkEndChild(declaration);
	
	TiXmlElement *pRoot = new TiXmlElement("Root");
	xml.LinkEndChild(pRoot);
	
	TiXmlElement *pExeDir = new TiXmlElement("ExeDir");
	pRoot->LinkEndChild(pExeDir);
	TiXmlText *pText = new TiXmlText(theApp.m_Exe_Working_Dir);
	pExeDir->LinkEndChild(pText);

	TiXmlElement *pStatus = new TiXmlElement("Status");
	pRoot->LinkEndChild(pStatus);
	if(theApp.hisStatus == 1)
	{
		pText = new TiXmlText("已退单");
		theApp.hisStatus = 0;
	}
	else
	{
		pText = new TiXmlText(ztinfo);
	}	
	pStatus->LinkEndChild(pText);
	
	TiXmlElement *pFieldList = new TiXmlElement("FieldList");
	pRoot->LinkEndChild(pFieldList);
	pText = new TiXmlText(medicalno);
	pFieldList->LinkEndChild(pText);
	
	TiXmlElement *pNode;
	int FieldCount = g_dbcommand.FieldCount();
	
	int ItemCount = 1;
	for(int j = 1; j <= FieldCount; j++ )
	{
		SADataType_t  DataType = g_dbcommand.Field(j).FieldType();
		if(!( DataType == SA_dtShort   || DataType == SA_dtLong     || DataType == SA_dtBool ||
			DataType == SA_dtDouble  || DataType == SA_dtDateTime || DataType == SA_dtString ||
			DataType == SA_dtLongChar|| DataType == SA_dtCLob ) )
		{
			continue;
		}
		
		CString FieldStr;	FieldStr.Empty();
		
		switch( DataType ) {
		case SA_dtShort:
			FieldStr.Format( "%d", g_dbcommand.Field(j).asShort() );
			break;
		case SA_dtLong:
			FieldStr.Format( "%d", g_dbcommand.Field(j).asLong() );
			break;
		case SA_dtBool:
			if( g_dbcommand.Field(j).asBool() )
				FieldStr.Format("是");
			else
				FieldStr.Format("否");
			break;
		case SA_dtDouble:
			FieldStr.Format( "%f", g_dbcommand.Field(j).asDouble() );
			break;
		case SA_dtDateTime:
			if(g_dbcommand.Field(j).asDateTime().GetYear() > 1950)
			{
				FieldStr.Format("%d-%d-%d", g_dbcommand.Field(j).asDateTime().GetYear(), g_dbcommand.Field(j).asDateTime().GetMonth(), g_dbcommand.Field(j).asDateTime().GetDay());
			}
			else
			{
				FieldStr.Empty();
			}
			break;
		case SA_dtString:
			FieldStr = g_dbcommand.Field(j).asString();
			break;
		case SA_dtBytes:
			FieldStr.Format("二进制流数据");
			break;
		case SA_dtLongBinary:
		case SA_dtBLob:
			FieldStr.Format("长二进制流数据");
			break;
		case SA_dtLongChar:
		case SA_dtCLob:
			FieldStr = g_dbcommand.Field(j).asString();
			break;
		case SA_dtUnknown:
		case SA_dtNumeric:
		case SA_dtCursor:
		case SA_dtSpecificToDBMS:
			FieldStr.Format("系统数据类型");
			break;
		default:;
		}
		
		CString FieldName = g_dbcommand.Field(j).Name();
		for(int k = 0; k < theApp.m_fnamelist.GetSize(); k++)
		{
			if(theApp.m_fnamelist.GetAt(k).CompareNoCase(g_dbcommand.Field(j).Name()) == 0)
			{
				FieldName = theApp.m_fdescriptionlist.GetAt(k);
				break;
			}
		}
		
		FieldStr.TrimLeft();  FieldStr.TrimRight();
		if(!FieldStr.IsEmpty())
		{
			pNode = new TiXmlElement("Field");
			pFieldList->LinkEndChild(pNode);
			pNode->SetAttribute("INDEX", ItemCount++);
			pNode->SetAttribute("FIELD", g_dbcommand.Field(j).Name());
			pNode->SetAttribute("NAME",  FieldName);
			pNode->SetAttribute("VALUE", FieldStr);
		}
	}
	
	char xmlFileName[MAX_PATH];

	memset(xmlFileName, 0, MAX_PATH);

	::GetTempPath(MAX_PATH - 1, xmlFileName);

	strcat(xmlFileName, "\\");
	strcat(xmlFileName, medicalno);
	strcat(xmlFileName, "_status.xml");
	
	xml.SaveFile(xmlFileName);
	xml.Clear();		
	
	int result = (ProcInDLL) (xmlFileName); 
	remove(xmlFileName);


	FreeLibrary(hLib);
	hLib = 0;
}

void CPathologyApp::OnAppDZBL()
{
	CString ExeDir;
	CString blh,BRID;
	char coffilepath[200];
	
    ExeDir=theApp.m_Exe_Working_Dir +"Pathology.ini";
	
	strcpy(coffilepath,ExeDir);
	
	blh=theApp.GetView()->GetDocument()->m_blh;
	
	//theApp.ConnectDatabase();
	
	try
	{
		SAString cmdstr = "Select TOP 1 brid from " + theApp.TABLE_MAIN + " where medicalno=:blh";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("blh").setAsString() =blh;
		
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			BRID = g_dbcommand.Field("brid").asString();
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
    
    CIniReader m_IniReader;
	m_IniReader.setINIFileName(ExeDir);
	
	m_IniReader.setKey(BRID,"brid","病人ID");
	
	HINSTANCE	hLib;
	if( (hLib=LoadLibrary(theApp.m_Exe_Working_Dir + "LoadEMR.dll")) == 0 )
	{
		AfxMessageBox("LoadEMR.dll装载错误！");
		return;
	}
	
	typedef int (*pInfoDownload)(char *); 
	pInfoDownload ProcInDLL;
	ProcInDLL = (pInfoDownload) GetProcAddress(hLib, "LoadEMR");
				
	if( ProcInDLL == NULL )
	{
		AfxMessageBox("LoadEMR函数定位错误！");
		return;
	}
	
	int result = (ProcInDLL) (coffilepath);

	FreeLibrary(hLib);
	hLib = 0;

}

void CPathologyApp::OnAppYQBG(){
	CYQJL dlg;
	dlg.DoModal();
}

void CPathologyApp::OnAppNew() 
{
	CWinApp::OnFileNew();
}



void CPathologyApp::OnWriterPrintSetup() 
{
	// TODO: Add your command handler code here
	CMicroWriterConfig dlg;
	if(dlg.DoModal() == IDCANCEL)
	{
		return;
	}
}

void CPathologyApp::MicroWriterIniLoading()
{
	if(theApp.m_StationType.CompareNoCase("取材工作站"))
	{
		return;
	}

	CIniReader m_IniReader;
	m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "MicroWriter.ini");
	m_MicroWriterMode = m_IniReader.getKeyValue("MicroWriterMode","parameters");
	m_FontOneLine = m_IniReader.getKeyValue("FontOneLine","parameters");
	m_FontFirstLine = m_IniReader.getKeyValue("FontFirstLine","parameters");
	m_FontSecondLine = m_IniReader.getKeyValue("FontSecondLine","parameters");
	m_BeginIndex = m_IniReader.getKeyValue("BeginIndex","parameters");
	m_FirstLineText = m_IniReader.getKeyValue("FirstLineText","parameters");
}


