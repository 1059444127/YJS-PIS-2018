// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Pathology.h"
#include "PathologyView.h"
#include "PathologyDoc.h"
#include "MedSound.h"
#include "MedImage.h"

#include "MainFrm.h"
#include "CWAVInfo.h"
#include "RecordInput.h"

#include "BuweiBZ.h"
#include "ZhengDuan.h"

#include "ImageView.h"
#include "FieldOut.h"

// MVC3000采集卡
#include "MVCSetup.h"

// OK CARD Properties
#include "okapi32.h"
#include "GDIThread.h"

#include "DbfStandard.h"
#include "StandInput.h"
#include "MedDiagnose.h"
#include "MedTbs.h"
#include "GetCode.h"
#include "SelfDefine.h"
#include "INI.h"
#include "DShowSetup.h"
#include "DbType.h"

#include "md5.h"

// ****    DH-HVDevice ****
#include "AdjustDlg.h"
#include "WBDlg.h"
#include "DlgGamma1.h"
// ************************

#include "LogoShow.h"
#include "Splash.h"

#include "Qpyj.h"
#include "Ckyj.h"
#include "Gzddy.h"
#include "Zpqk.h"
#include "TextOut.h"
#include "ChargeConfirm.h"
#include "NormalSfqr.h"
#include "ZCQR.h"
#include "LeaveMessage.h"
#include "AdviceTip.h"

#include "GetStudyUID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct BTN_TEXT_MAP
{
	UINT nCmdID;
	LPCTSTR lpszText;
};

static BTN_TEXT_MAP tbTextMap[] =
{
	//{ ID_FILE_NEW,			 _T("新病历")  },
	{ ID_DOC_NEWORDER,		 _T("新建.杭") },
	{ ID_DOC_NEWORDER1,		 _T("新建.联") },
	{ ID_DOC_QUICKINPUT,	 _T("自定义") },
	{ ID_APP_CAMERA,	     _T("摄图")  },
	{ ID_FILE_SAVE,		     _T("保存")  },
	{ ID_APP_REFRESH,	     _T("刷新")  },
	{ ID_APP_LAST,		     _T("上一例")  },
	{ ID_APP_NEXT,		     _T("下一例")  },
	{ ID_APP_CHECK,		     _T("审核")  },
	{ ID_APP_UNCHECK,	     _T("修改")  },
	{ ID_FILE_PRINT_PREVIEW, _T("预览")  },
	{ ID_FILE_PRINT,		 _T("打印")  },
	{ ID_APP_SEND,		     _T("发送")  },
	{ ID_APP_COLLECT,		 _T("收藏")  },
	{ ID_APP_NOTEPAD,		 _T("短信")  },
	{ ID_APP_DZBL,			 _T("EMR")	 },
	{ ID_APP_LOGOUT,		 _T("注销")  },	
	{ ID_APP_EXIT,		     _T("退出")  },
};

static UINT tbResources[] =
{
	IDR_MAINFRAME_SMALL
};

static UINT tbResourcesGray[] =
{
	IDR_MAINFRAME_SMALL_GRAY
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame
// add eventsink map to hook OnComm event
BEGIN_EVENTSINK_MAP(CMainFrame, CXTFrameWnd)
ON_EVENT(CMainFrame, ID_COMMCTRL, 1 /* OnComm */, OnCommEvent, VTS_NONE)
END_EVENTSINK_MAP()

IMPLEMENT_DYNCREATE(CMainFrame, CXTFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CXTFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateViewToolbar)
	ON_COMMAND(ID_VIEW_MEDLIST, OnViewMedlist)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MEDLIST, OnUpdateViewMedlist)
	ON_COMMAND(ID_FIELD_NOTIFICATION, OnFieldNotification)
	ON_COMMAND(ID_MAINTANCE_REPORT, OnMaintanceReport)
	ON_COMMAND(ID_REPORT_DISCRIBE, OnReportDiscribe)
	ON_COMMAND(ID_REPORT_REGION, OnReportRegion)
	ON_COMMAND(ID_REPORT_DIA, OnReportDia)
	ON_COMMAND(ID_REPORT_RYSJ, OnReportRysj)
	ON_COMMAND(ID_ACQUIRE, OnAcquire)
	ON_UPDATE_COMMAND_UI(ID_ACQUIRE, OnUpdateAcquire)
	ON_COMMAND(ID_SELECT_SOURCE, OnSelectSource)
	ON_UPDATE_COMMAND_UI(ID_SELECT_SOURCE, OnUpdateSelectSource)
	ON_COMMAND(ID_IMG_CAPTURESTP, OnImgCapturestp)
	ON_WM_TIMER()
	ON_COMMAND(ID_IMG_CAPTURE_FORMAT, OnImgCaptureFormat)
	ON_COMMAND(ID_VIEW_OUTPUT, OnViewOutput)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateViewOutput)
	ON_COMMAND(ID_STANDARD_VIEW, OnStandardView)
	ON_COMMAND(ID_STANDARD_ADD, OnStandardAdd)
	ON_UPDATE_COMMAND_UI(ID_DOC_MANAGER, OnUpdateDocManager)
	ON_COMMAND(ID_DOC_MANAGER, OnDocManager)
	ON_COMMAND(ID_FILE_RPTMODIFY, OnFileRptmodify)
	ON_COMMAND(ID_APP_GETCODE, OnAppGetcode)
	ON_UPDATE_COMMAND_UI(ID_APP_GETCODE, OnUpdateAppGetcode)
	ON_COMMAND(ID_DH_SETUP, OnDhSetup)
	ON_COMMAND(ID_DH_WB, OnDhWb)
	ON_COMMAND(ID_DH_GAMMA, OnDhGamma)
	ON_COMMAND(ID_DH_CANCELGAMMA, OnDhCancelgamma)
	ON_COMMAND(ID_DH_MODE0, OnDhMode0)
	ON_COMMAND(ID_DH_MODE1, OnDhMode1)
	ON_COMMAND(ID_DH_MODE2, OnDhMode2)
	ON_COMMAND(ID_DH_CONVERSION_GB, OnDhConversionGb)
	ON_COMMAND(ID_DH_CONVERSION_GR, OnDhConversionGr)
	ON_COMMAND(ID_DH_CONVERSION_BG, OnDhConversionBg)
	ON_COMMAND(ID_DH_CONVERSION_RG, OnDhConversionRg)
	ON_UPDATE_COMMAND_UI(ID_DH_MODE0, OnUpdateDhMode0)
	ON_UPDATE_COMMAND_UI(ID_DH_MODE1, OnUpdateDhMode1)
	ON_UPDATE_COMMAND_UI(ID_DH_MODE2, OnUpdateDhMode2)
	ON_UPDATE_COMMAND_UI(ID_DH_CONVERSION_BG, OnUpdateDhConversionBg)
	ON_UPDATE_COMMAND_UI(ID_DH_CONVERSION_GB, OnUpdateDhConversionGb)
	ON_UPDATE_COMMAND_UI(ID_DH_CONVERSION_GR, OnUpdateDhConversionGr)
	ON_UPDATE_COMMAND_UI(ID_DH_CONVERSION_RG, OnUpdateDhConversionRg)
	ON_COMMAND(ID_MAINTANCE_SELFFIELD, OnMaintanceSelffield)
	ON_COMMAND(ID_DOC_LOGO, OnDocLogo)
	ON_WM_SIZE()
	ON_WM_ACTIVATEAPP()
	ON_COMMAND(ID_FILE_DBSETUP, OnFileDbsetup)
	ON_UPDATE_COMMAND_UI(ID_FILE_DBSETUP, OnUpdateFileDbsetup)
	ON_COMMAND(ID_DOC_QUERY, OnDocQuery)
	ON_COMMAND(ID_ZPGL_CKYJ, OnZpglCkyj)
	ON_COMMAND(ID_ZPGL_YZDY, OnZpglYzdy)
	ON_COMMAND(ID_ZPGL_QPYJ, OnZpglQpyj)
	ON_COMMAND(ID_ZPGL_ZPQKTJ, OnZpglZpqktj)
	ON_UPDATE_COMMAND_UI(ID_IMG_CAPTURESTP, OnUpdateImgCapturestp)
	ON_UPDATE_COMMAND_UI(ID_MAINTANCE_SELFFIELD, OnUpdateMaintanceSelffield)
	ON_COMMAND(ID_MAINTANCE_SELFSHOW, OnMaintanceSelfshow)
	ON_UPDATE_COMMAND_UI(ID_MAINTANCE_SELFSHOW, OnUpdateMaintanceSelfshow)
	ON_COMMAND(ID_WDM_DEVICE, OnWdmDevice)
	ON_COMMAND(ID_IMG_OLYMPUS, OnImgOlympus)
	ON_COMMAND(ID_ZPGL_SFQR, OnZpglSfqr)
	ON_COMMAND(ID_ZPGL_ZCSFQR,OnZpglZCSfqr)
	ON_COMMAND(ID_ZPGL_CGSFQR, OnZpglCgsfqr)
	ON_COMMAND(ID_FILE_RPTVIEW, OnFileRptview)
	ON_MESSAGE(WM_CAPTURE_BITMAP, OnCaptureBitmap)
	ON_MESSAGE(WM_CHANGEDEVICE, OnDeviceChanged)
	ON_MESSAGE(WM_RESIZEVIDEOWINDOW, ResizeVideoWindow)
	ON_COMMAND(ID_MAINTENANCE_PICNOTE, OnMaintenancePicnote)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SWITCHON,SwitchOn)
	ON_MESSAGE(WM_OKCAPTURE,OkCapture)
	ON_MESSAGE(WM_VFWCAPTURE,VfwCapture)
	ON_MESSAGE(WM_HVCAPTURE,DhhvCapture)
	ON_MESSAGE(WM_DSHOWCAPTURE,DShowCapture)
	ON_MESSAGE(WM_MVC3000,MVC3000Preview)
	ON_MESSAGE(WM_MYDSHOW,MYDSHOWPreview)
	ON_MESSAGE(WM_CANONS3S5, CanonS3S5Capture)

	ON_CBN_SELCHANGE(ID_RULERMARK, OnSelchangeComboRulerMark)
	ON_CBN_SELCHANGE(ID_SELPRINTER, OnSelchangeComboSelPrinter)
	ON_MESSAGE(WM_CHANGE_SNAP, OnSnapMessage)
	ON_MESSAGE(WM_GET_RATIO, OnGetRatioMessage) 

	ON_UPDATE_COMMAND_UI_RANGE(ID_BLK_BEGIN, ID_BLK_BEGIN + 50, OnUpdateChangeMedicalTable)
	ON_COMMAND_RANGE(ID_BLK_BEGIN, ID_BLK_BEGIN + 50, OnChangeMedicalTable)


END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_BGZT,
	ID_INDICATOR_BGDY
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction
extern CPathologyApp theApp;
    
CMainFrame::CMainFrame()
: m_nRes(0), m_nType(-1), m_nXfer(0)  //  TWAIN  Properties
{
	// TODO: add member initialization code here
	m_blkCount = 0;
	pRecordTiming = NULL;
	m_frame_created = false;

	count_old = 0; count_bd_old=0;
	user_old.Empty();
	
	pLeaveMessage == NULL;
	pBookingTip == NULL;

	// MVC3000采集卡
	m_hMVC3000 = NULL;
	m_pRGBData = NULL;
	m_pRawData = NULL;

	// OK CARD Properties
	hBoard = NULL;
	pExtThread = NULL;
	m_realtime = false;
	InputWnd_Width  = 512;
	InputWnd_Height = 512;

	// VFW Properties
	m_pFrameGrabber = NULL;
	m_TimerHandle = 0;

	m_pWndCanonS5Capture = NULL;

	// ****  DH-HVDevice ****
	m_hhv			= NULL;
	m_SnapMode		= CONTINUATION;	
	m_Resolution	= RES_MODE0;
	m_lADCLevel		= ADC_LEVEL2;
	for (int i = 0; i < 4; i++)
	{
		m_lGain[i]	= 8;
	}
	m_ImageMode		= HV_COLOR;
	m_Layout	    = BAYER_GB;
	m_dRatioR		= 1.0;
	m_dRatioG		= 1.0;
	m_dRatioB		= 1.0;

	//颜色查值表
	for(i=0;i<256;i++)
	{
		m_pLutGamma[i] = i;		

		m_pLutR[i] = i;
		m_pLutG[i] = i;
		m_pLutB[i] = i;		
	}
	m_bIsGammaCorrect = FALSE;
	m_dGammRatio = 1.0;

	m_lTintUpper     = 60;
	m_lTintLower     = 1000;
	m_ConversionType = BAYER2RGB_NEIGHBOUR;

	m_SnapSpeed = HIGH_SPEED;
	m_nHBlanking = 0;
	m_nVBlanking = 0;

	m_pRawBuffer   = NULL;
	m_pImageBuffer = NULL;

	m_bIsSnapSpeedSprted = FALSE;   //本摄像机是否支持设置采集速度
	m_bIsBlkSizeSprted = FALSE;     //本摄像机是否支持设置取得消隐取值范围	

	nMaxWidMd0 = 800;
	nMaxHeiMd0 = 600;
	// **********************

	m_DigitalOut_NeedUpdateWnd = false;

	m_Close_TimerHandle = 0;
}

CMainFrame::~CMainFrame()
{
	GDALDestroyDriverManager();
	GS_DestroyDriverChk();
	//delete dlg;
	// ****    DH-HVDevice ****
	if (m_pImageBuffer) {
		delete []m_pImageBuffer;
	}
	if (m_pRawBuffer) {
		delete []m_pRawBuffer;
	}
	// ************************
}

HANDLE launchViaCreateProcess(LPCTSTR program, LPCTSTR args)
{
    HANDLE hProcess = NULL;
	PROCESS_INFORMATION processInfo;
    STARTUPINFO startupInfo;
    ::ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    if(::CreateProcess(program, (LPTSTR)args, 
		NULL,  // process security
		NULL,  // thread security
		FALSE, // no inheritance
		CREATE_NO_WINDOW,     // no startup flags
		NULL,  // no special environment
		NULL,  // default startup directory
		&startupInfo,
		&processInfo))
    { /* success */
        hProcess = processInfo.hProcess;
    } /* success */
    return hProcess;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CSplashWnd::EnableSplashScreen(TRUE);
	CSplashWnd::ShowSplashScreen(this);

    // Enable/Disable XP GUI Mode
    xtAfxData.bXPMode = FALSE;

    // Enable/Disable Menu Shadows
    xtAfxData.bMenuShadows = FALSE;

	if (!m_wndMenuBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndMenuBar.LoadMenuBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}
		
	CMenu blk;

	blk.CreatePopupMenu();
	
	for (int i1 = 0; i1 < theApp.m_DBName_list.GetSize(); i1++)
	{		
		blk.AppendMenu( MF_STRING, ID_BLK_BEGIN + i1 ,theApp.m_DBName_list.GetAt(i1) );
		m_blkCount++;
	}

	blk.AppendMenu( MF_SEPARATOR);
	blk.AppendMenu( MF_STRING, ID_APP_EXIT, "退出(&x)");
	 
	m_wndMenuBar.GetMenu()->ModifyMenu(0,MF_BYPOSITION | MF_POPUP ,(UINT)blk.m_hMenu,"病历库");
	
	blk.Detach();   

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
		CRect(0, 0, 0, 0), AFX_IDW_TOOLBAR+2) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	
	m_wndToolBar.SetBitmap(LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)));

	// set the title of the high color toolbars
	m_wndToolBar.SetWindowText(_T("工具栏"));
	// buttons created during toolbar customization shall be create with text
	///m_wndToolBar.SetDropCommandWithText(true);
	// make the high color toolbars customizable...
	m_wndToolBar.SetCustomBar(false);
	
	if ( !m_ilToolBarGray.Create( 32, 32, ILC_MASK|ILC_COLOR24, 14, 1 ) )
	{
		TRACE0( "Failed to create image list.\n" );
		return false;
	}

//	m_bmpToolBarGray.LoadBitmap( IDB_BITMAP_TOOLBARGRAY );
//	m_ilToolBarGray.Add( &m_bmpToolBarGray, RGB( 212,208,200 ) );

//	m_wndToolBar.SetImageList(m_ilToolBarGray, FALSE);
//	m_wndToolBar.SetImageList(m_ilToolBarGray, TRUE);

	// add text to the first high color toolbar...
	
	for (int iButton = 0; iButton < _countof(tbTextMap); ++iButton)
	{
		m_wndToolBar.SetButtonText(m_wndToolBar.CommandToIndex(tbTextMap[iButton].nCmdID), tbTextMap[iButton].lpszText);
	}


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Create the workspace bar.
	if( !m_wndTabDockBar.Create(this, AFX_IDW_TOOLBAR + 4, _T("病历信息管理"),
		CSize(200, 150), CBRS_RIGHT))
	{
		TRACE0("Failed to create workspace dock window\n");
		return -1;		// fail to create
	}

	m_wndTabDockBar.SetAutoCondense(FALSE);

	// Create the output bar.
	if (!m_wndOutputBar.Create(this, AFX_IDW_TOOLBAR + 8, _T("输出栏"),
		CSize(150, 150), CBRS_BOTTOM))
	{
		TRACE0("Failed to create output dock window\n");
		return -1;		// fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndMenuBar.EnableDockingEx(CBRS_ALIGN_ANY, CBRS_XT_SEMIFLAT);
	m_wndToolBar.EnableDockingEx(CBRS_ALIGN_ANY, CBRS_XT_SEMIFLAT);
	m_wndTabDockBar.EnableDockingEx(CBRS_ALIGN_RIGHT, CBRS_XT_ALL_FLAT|CBRS_XT_GRIPPER_GRAD);
	m_wndOutputBar.EnableDockingEx(CBRS_ALIGN_BOTTOM, CBRS_XT_ALL_FLAT|CBRS_XT_GRIPPER_GRAD);
	EnableDockingEx(CBRS_ALIGN_ANY, CBRS_XT_SEMIFLAT);

	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndTabDockBar);
	DockControlBar(&m_wndOutputBar);
	
	// Insert company name into the status bar.
	AddLogo();

	//--------------------------------------------------------
	int iIndex = 4;
	if (!m_wndComboSelPrinter.Create( WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_CLIPCHILDREN,
		CRect(0,0,200,200), &m_wndStatusBar, ID_SELPRINTER ))
	{
		TRACE0("Failed to create combo box.\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.AddIndicator(AFX_IDW_TOOLBAR + 3, iIndex);
	int nIndex = m_wndStatusBar.CommandToIndex(AFX_IDW_TOOLBAR + 3);
	m_wndStatusBar.SetPaneWidth(nIndex, 150);
	m_wndStatusBar.SetPaneStyle(nIndex, m_wndStatusBar.GetPaneStyle(nIndex) | SBPS_NOBORDERS);
	m_wndStatusBar.AddControl(&m_wndComboSelPrinter, AFX_IDW_TOOLBAR + 3, FALSE);

	for (int i = 0 ; i < theApp.m_PrinterControl.GetPrinterCount() ; i++)
	{
		m_wndComboSelPrinter.AddString(theApp.m_PrinterControl.GetPrinterName(i)) ;
	}
	// select the current default printer
	CString	current ;
	current = theApp.GetDefaultPrinter() ;
	m_wndComboSelPrinter.SetCurSel(m_wndComboSelPrinter.FindString(0, current)) ;
	m_wndComboSelPrinter.SetFont(&xtAfxData.font);

	iIndex = 5;
	if (!m_wndComboRulerMark.Create( WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_CLIPCHILDREN,
		CRect(0,0,200,200), &m_wndStatusBar, ID_RULERMARK ))
	{
		TRACE0("Failed to create combo box.\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.AddIndicator(AFX_IDW_TOOLBAR + 7, iIndex);
	nIndex = m_wndStatusBar.CommandToIndex(AFX_IDW_TOOLBAR + 7);
	m_wndStatusBar.SetPaneWidth(nIndex, 160);
	m_wndStatusBar.SetPaneStyle(nIndex, m_wndStatusBar.GetPaneStyle(nIndex) | SBPS_NOBORDERS);
	m_wndStatusBar.AddControl(&m_wndComboRulerMark, AFX_IDW_TOOLBAR + 7, FALSE);

	CFile file;
	CFileException fe;
	if (file.Open(theApp.m_Exe_Working_Dir + "system/rulermark.dat", CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		int num = file.GetLength() / sizeof(STANDARD_UNIT);
		STANDARD_UNIT  m_unit;
		CString str;
		for(int i = 0; i < num; i++ )
		{
			file.Read(&m_unit, sizeof(STANDARD_UNIT));
			str.Format("%3d倍镜: %8.4f ",m_unit.zoomlen,m_unit.value);
			str += m_unit.unitname;
			m_wndComboRulerMark.AddString(str);
		}
		file.Close();
	}

	m_wndComboRulerMark.SetCurSel(0);
	m_wndComboRulerMark.SetFont(&xtAfxData.font);
	//--------------------------------------------------------

	// TODO: Remove this line if you don't want cool menus.
	///InstallCoolMenus( tbResources, _countof( tbResources ) );
	// Initialize a disabled image list for cool menus.
	GetCoolMenu()->LoadToolbars( tbResourcesGray, _countof( tbResourcesGray ), false );

	// gdal setup
	GDALAllRegister();
	GS_RegisteDriverChk();

	if(theApp.m_CaptureMode.CompareNoCase("Ok图像采集卡") == 0)
	{
		// OK CARD Properties
		bool IsBoardExist = true;
		short iNoBoards = okGetSlotBoard(NULL);
		//check if reg device driver for nt/2k/xp 
		if (!okSetNTDriver(0)) 
		{ // TRUE:registered already, -1:not nt system
			//AfxMessageBox("采集设备没有找到!");
			IsBoardExist = false;
		}
		else if (okSetNTDriver(0) == -1) { //win95/98
			if( !iNoBoards && (!okSetStaticVxD(0)) ) 
			{
				//AfxMessageBox("采集设备没有找到!");
				IsBoardExist = false;
			}
		}
		
		if( IsBoardExist )
		{
			long iCurrUsedNo = -1;
			hBoard = okOpenBoard(&iCurrUsedNo);
			if( hBoard )
			{
				long lg = okSetCaptureParam(hBoard, CAPTURE_BUFBLOCKSIZE, -1);
				InputWnd_Width  = LOWORD(lg);
				InputWnd_Height = HIWORD(lg);
				
				m_Ok_Unmatch = 0;
			}
		}
	}

	if(theApp.m_CaptureMode.CompareNoCase("TWAIN采集设备支持") == 0)
	{
		//  TWAIN  Properties
		if (TwainAvailable()) {
			SetDefWindow(this);
			if (!OpenSourceManager()) {
				// OpenSourceManager does not report errors
				TwainError(TWERR_OPEN_DSM);
			}
		}
	}
	
	if(theApp.m_CaptureMode.CompareNoCase("DH-HVxxxxU系列采集卡") == 0)
	{
		// ****    DH-HVDevice ****
		InitParamFromINI();
		
		HVSTATUS HVstatus = BeginHVDevice(1, &m_hhv);
		//HV_MESSAGE(HVstatus);
		if (HV_SUCCESS(HVstatus)){
			InitHVDevice();
		}
		// ************************
	}

	if(theApp.m_CaptureMode.CompareNoCase("微软DSHOW设备") == 0)
	{
		// create over entire window
		if(false == DSHOW_CreateCaptureWindow(theApp.m_hInstance,this->m_hWnd,WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,0,0,1024,768,IDC_CAPTURE))
		{
			AfxMessageBox("DSHOW驱动装载错误！");
		}

		CIniReader m_IniReader;
		m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");
		CStringArray nDeviceNameList;
		if(false == DSHOW_GetDeviceNameList(nDeviceNameList))
		{
			AfxMessageBox("No devices are present！");
		}

		CString str;
		str.Empty();
		for(int i = 0; i < nDeviceNameList.GetSize(); i++)
		{
			str += nDeviceNameList.GetAt(i);
			str += ",";
		}
		m_IniReader.setKey(str,"DeviceNameList","微软DSHOW设备参数设置");
		nDeviceNameList.RemoveAll();

		// now load our persistent settings
		DSHOW_LoadSettings(this->m_hWnd);

		i = DSHOW_GetCurrentInputSetting();
		str.Format("%d", i);
		m_IniReader.setKey(str,"CurrentInputSetting","微软DSHOW设备参数设置");

		DSHOW_ChangePreviewState(FALSE);
		DSHOW_ShowWindow(SW_HIDE);
	}

	if(theApp.m_CaptureMode.CompareNoCase("MVC3000采集卡") == 0)
	{
		DWORD RGBDataSize = MAXWIDTH*MAXHEIGHT*3;
		m_pRGBData = (BYTE*)malloc(RGBDataSize*sizeof(BYTE));
		memset(m_pRGBData, 0, RGBDataSize);	
		DWORD RawDataSize = MAXWIDTH*MAXHEIGHT+512;
		m_pRawData=(BYTE*)malloc(RawDataSize*sizeof(BYTE));
		memset(m_pRawData,0,RawDataSize);

		//-------------------------------------
		memset(&m_CapInfo, 0, sizeof(struct CapInfoStruct));
		m_CapInfo.Buffer   = m_pRawData;
		m_CapInfo.Width    = 320;
		m_CapInfo.Height   = 240;
		m_CapInfo.HorizontalOffset = 0;
		m_CapInfo.VerticalOffset   = 0;
		m_CapInfo.Exposure = 50;
		m_CapInfo.Gain[0]  = 40;
		m_CapInfo.Gain[1]  = 30;
		m_CapInfo.Gain[2]  = 40;

		m_AE = FALSE;

		CIniReader m_IniReader;
		m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");
		CString str;
		str = m_IniReader.getKeyValue("捕获窗口宽度","MVC3000采集卡");
		str.TrimLeft();  str.TrimRight();
		if(!str.IsEmpty())  m_CapInfo.Width  = atoi(str);

		str = m_IniReader.getKeyValue("捕获窗口高度","MVC3000采集卡");
		str.TrimLeft();  str.TrimRight();
		if(!str.IsEmpty())  m_CapInfo.Height = atoi(str);

		str = m_IniReader.getKeyValue("捕获窗口水平偏移","MVC3000采集卡");
		str.TrimLeft();  str.TrimRight();
		if(!str.IsEmpty())  m_CapInfo.HorizontalOffset = atoi(str);

		str = m_IniReader.getKeyValue("捕获窗口垂直偏移","MVC3000采集卡");
		str.TrimLeft();  str.TrimRight();
		if(!str.IsEmpty())  m_CapInfo.VerticalOffset   = atoi(str);

		str = m_IniReader.getKeyValue("曝光时间","MVC3000采集卡");
		str.TrimLeft();  str.TrimRight();
		if(!str.IsEmpty())  m_CapInfo.Exposure = atoi(str);

		str = m_IniReader.getKeyValue("红色增益","MVC3000采集卡");
		str.TrimLeft();  str.TrimRight();
		if(!str.IsEmpty())  m_CapInfo.Gain[0] = atoi(str);

		str = m_IniReader.getKeyValue("绿色增益","MVC3000采集卡");
		str.TrimLeft();  str.TrimRight();
		if(!str.IsEmpty())  m_CapInfo.Gain[1] = atoi(str);

		str = m_IniReader.getKeyValue("蓝色增益","MVC3000采集卡");
		str.TrimLeft();  str.TrimRight();
		if(!str.IsEmpty())  m_CapInfo.Gain[2] = atoi(str);

		str = m_IniReader.getKeyValue("自动曝光","MVC3000采集卡");
		str.TrimLeft();  str.TrimRight();
		if(!str.IsEmpty())  m_AE = atoi(str);

		str = m_IniReader.getKeyValue("控制参数","MVC3000采集卡");
		str.TrimLeft();  str.TrimRight();
		if(!str.IsEmpty())  m_CapInfo.Control = atoi(str);
		//-------------------------------------
	
		int nIndex;
		if(MV_Usb2Init("MVC1000", &nIndex, &m_CapInfo, &m_hMVC3000))
		{
			AfxMessageBox("Can not open MVC3000 USB camera! ");
			MV_Usb2Uninit(&m_hMVC3000);
			m_hMVC3000 = NULL;
		}
	}

	if(theApp.m_CaptureMode.CompareNoCase("DirectShow采集设备支持") == 0)
	{
		if (!m_VideoCap.Create(NULL,WS_POPUP & (~WS_VISIBLE),CRect(0, 0, 0, 0),this,11))
		{
			AfxMessageBox("Can't create m_VideoCap activex window");
			return -1;
		}

		m_VideoCap.SetLicenseKey("8521 single developer license");
		
		m_DeviceIndex = m_VideoFormatIndex = m_VideoInputIndex = -1;

		CIniReader m_IniReader;
		m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");
		CString str;
		str = m_IniReader.getKeyValue("Device","DirectShow采集设备支持");
		str.TrimLeft();  str.TrimRight();
		if(!str.IsEmpty())  m_DeviceIndex = atoi(str);

		str = m_IniReader.getKeyValue("VideoFormat","DirectShow采集设备支持");
		str.TrimLeft();  str.TrimRight();
		if(!str.IsEmpty())  m_VideoFormatIndex = atoi(str);

		str = m_IniReader.getKeyValue("VideoInput","DirectShow采集设备支持");
		str.TrimLeft();  str.TrimRight();
		if(!str.IsEmpty())  m_VideoInputIndex = atoi(str);

		if(m_DeviceIndex < 0 || m_DeviceIndex >= m_VideoCap.GetDevices().GetCount())
		{
			m_DeviceIndex = -1;
		}

		if(m_VideoFormatIndex < 0 || m_VideoFormatIndex >= m_VideoCap.GetVideoFormats().GetCount())
		{
			m_VideoFormatIndex = -1;
		}

		if(m_VideoInputIndex < 0 || m_VideoInputIndex >= m_VideoCap.GetVideoInputs().GetCount())
		{
			m_VideoInputIndex = -1;
		}

		if(m_DeviceIndex == -1 && m_VideoCap.GetDevices().GetCount() > 0)
		{
			m_DeviceIndex = 0;
		}

		if(m_VideoFormatIndex == -1 && m_VideoCap.GetVideoFormats().GetCount() > 0)
		{
			m_VideoFormatIndex = 0;
		}

		if(m_VideoInputIndex == -1 && m_VideoCap.GetVideoInputs().GetCount() > 0)
		{
			m_VideoInputIndex = 0;
		}

		str.Format("%d", m_DeviceIndex);
		m_IniReader.setKey(str,"Device","DirectShow采集设备支持");
		str.Format("%d", m_VideoFormatIndex);
		m_IniReader.setKey(str,"VideoFormat","DirectShow采集设备支持");
		str.Format("%d", m_VideoInputIndex);
		m_IniReader.setKey(str,"VideoInput","DirectShow采集设备支持");
	}


	if(theApp.m_CaptureMode.CompareNoCase("Canon S3/S5 IS") == 0)
	{
		/* PS-Rec SDK START */
		if (PR_StartSDK() == prOK)
		{
			m_pWndCanonS5Capture = new CCaptureCanonS5Dlg(this);
			m_pWndCanonS5Capture->SetParent(this);
			m_pWndCanonS5Capture->Create(this);
		}
	}

	//----------------------------------
	m_LastTime = COleDateTime::GetCurrentTime();
	m_LastComPortTime = COleDateTime::GetCurrentTime();

	theApp.m_Waiting.TrimLeft(); theApp.m_Waiting.TrimRight();
	if(!theApp.m_Waiting.IsEmpty())
	{
		int m_WaitTime = atoi(theApp.m_Waiting);
		if( m_WaitTime > 0 )
		{
			m_Close_TimerHandle = SetTimer(3, 30000, NULL);
		}
	}
	//----------------------------------
    if( theApp.m_ComPort )
	{
		// ****  COM PORT CONFIGURATION  ****
		// Create the MSCOMM32 OLE Control.
		if (!m_commctrl.Create(NULL,0,CRect(0,0,0,0),this,ID_COMMCTRL))
		{
			TRACE0("Failed to create OLE Communications Control\n");
			return -1;      // fail to create 
		}
		// Set RThreshold property to 1. The default RThreshold value (0)
		// causes the OnComm event to NOT fire when a character is detected
		// in the serial port. 
		m_commctrl.SetRThreshold(1);
		
		// ====  set parameter ====
		int baudrate = 600;
		TCHAR parity;
		CString str;
		parity = (TCHAR)'N';
		str.Format(_T("%d,%C,%d,%d"),baudrate,parity,8,1);	
		m_commctrl.SetSettings(str);
		
		// set echo, handshaking (flow) and comm port settings here!!!
		m_commctrl.SetHandshaking(0);
		m_commctrl.SetCommPort( theApp.m_ComPort ); // Port No. range from 1 -- 4 depending on the system setup
		
		bool IsComPortValid = false;
		CRegKey m_RegKey;
		LPCTSTR m_KeyName = "hardware\\devicemap\\serialcomm";
		if(m_RegKey.Open(HKEY_LOCAL_MACHINE,m_KeyName) == ERROR_SUCCESS)
		{
			DWORD pCount = 1024;
			char szValue[1024];
			CString ComPortStr;
			ComPortStr.Format("COM%d", theApp.m_ComPort);
			for(int i = 0; i <= 100; i++)
			{
				CString m_ValueName, m_KeyValue;
				m_ValueName.Format("\\Device\\Serial%d", i);
				if(m_RegKey.QueryValue(szValue, m_ValueName, &pCount) == ERROR_SUCCESS)
				{
					m_KeyValue = szValue;
					if(m_KeyValue.CompareNoCase(ComPortStr) == 0)
					{
						IsComPortValid = true;
						break;
					}
				}
			}
			
			m_RegKey.Close();
		}
		
		if(IsComPortValid)
		{
			m_commctrl.SetPortOpen(TRUE);
		}
		// **********************************
	}
	
	m_frame_created = true;

	Sound_Open();
	Sound_Play("welcome.wav");
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CXTFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | FWS_PREFIXTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	// Helps to reduce screen flicker.
	cs.lpszClass = AfxRegisterWndClass(0, NULL, NULL,
		AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	cs.x = 0;
	cs.y = 0;
	cs.cx = ::GetSystemMetrics(SM_CXSCREEN);
	cs.cy = ::GetSystemMetrics(SM_CYSCREEN);
	

	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CXTFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CXTFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::RefreshStatusBar()
{
	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		m_wndStatusBar.SetPaneText(2, "");
		m_wndStatusBar.SetPaneText(3, "");
		return;
	}

	theApp.ConnectDatabase();

	int m_bgzt = 0;
	CString m_bgdy;  m_bgdy.Empty();
	try
	{
		SAString cmdstr = "Select bgzt, bgdy from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			m_bgzt = g_dbcommand.Field("bgzt").asLong();
			m_bgdy = g_dbcommand.Field("bgdy").asString();
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

	m_wndStatusBar.SetPaneText(3, m_bgdy);

	CString ztinfo; ztinfo.Empty();

	if(g_dbcommand.Field("bgzt").isNull() || m_bgzt < 1)  ztinfo.Format("已登记");
	if(m_bgzt >= 1  && m_bgzt < 2)   ztinfo.Format("已取材");
	if(m_bgzt >= 2  && m_bgzt < 4)   ztinfo.Format("已延期");
	if(m_bgzt >= 4  && m_bgzt < 8)   ztinfo.Format("诊断中");
	if(m_bgzt >= 8  && m_bgzt < 16)  ztinfo.Format("已审核");
	if(m_bgzt >= 16 && m_bgzt < 32)  ztinfo.Format("已打印");
	if(m_bgzt >= 32)                 ztinfo.Format("已发送");

	m_wndStatusBar.SetPaneText(2, ztinfo);
}

void CMainFrame::RefreshLogo()
{
	CString str;

	str  = "库别: ";
	str += theApp.m_DB_Type_Name;
	str += "  用户: ";
	str += theApp.m_Cpp_User;
	str += "  登录时间：";
	str += theApp.m_Login_Time.Format("%Y-%m-%d %H:%M:%S");

	m_wndLogoPane.SetLogoText(str);
	int nWidth = m_wndLogoPane.GetTextSize().cx;
	int nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_LOGO);
	ASSERT (nIndex != -1);
	m_wndStatusBar.SetPaneWidth(nIndex, nWidth);

	m_wndStatusBar.RedrawWindow();
}

void CMainFrame::AddLogo()
{
	CString str;

	str  = "库别: ";
	str += theApp.m_DB_Type_Name;
	str += "  用户: ";
	str += theApp.m_Cpp_User;
	str += "  登录时间：";
	str += theApp.m_Login_Time.Format("%Y-%m-%d %H:%M:%S");

	if (!m_wndLogoPane.Create(_T(str), &m_wndStatusBar))
	{
		TRACE0("Failed to create logo control.\n");
		return;
	}

	int nWidth = m_wndLogoPane.GetTextSize().cx;

	// add the indicator to the status bar.
	m_wndStatusBar.AddIndicator(ID_INDICATOR_LOGO, 0);
	
	// Initialize the pane info and add the control.
	int nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_LOGO);
	ASSERT (nIndex != -1);
	
	m_wndStatusBar.SetPaneWidth(nIndex, nWidth);
	m_wndStatusBar.SetPaneStyle(nIndex, m_wndStatusBar.GetPaneStyle(nIndex) | SBPS_POPOUT);
	m_wndStatusBar.AddControl(&m_wndLogoPane, ID_INDICATOR_LOGO, FALSE);
}

void CMainFrame::RemoveLogo()
{
	m_wndStatusBar.RemoveIndicator(ID_INDICATOR_LOGO);
}

void CMainFrame::SaveModifiedRecord(bool IsEchoOn)
{
	if(!theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		int num = 0;
		try
		{
			SAString cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				num = g_dbcommand.Field(1).asLong();
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

		if(num < 1)  return;

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
	
		if(!m_bgzt)
		{
			if(IsEchoOn)
			{
				if( m_realtime )
				{
					TerminateTimer();
				}
			}

			if (IsEchoOn && AfxMessageBox("是否保存当前打开病例？",MB_YESNO|MB_ICONQUESTION) == IDYES)
				theApp.GetView()->ForceSave();
		}
	}
}

void CMainFrame::OnClose() 
{
	CString sText;

	sText.Format(_T("你是否真要退出?"));

	if (AfxMessageBox(sText, MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) != IDYES)
		return ;

	SaveModifiedRecord();

	// Save control bar postion.
	SaveBarState(_T("Control Bar State"));

	// Save frame window size and position.
	m_wndPosition.SaveWindowPos(this);

	// ****    DH-HVDevice ****
	SaveParamToINI();
	if(m_hhv)
	{
		EndHVDevice(m_hhv);
	}
	// ************************

	CXTFrameWnd::OnClose();
}

BOOL CMainFrame::ShowWindowEx(int nCmdShow)
{
	ASSERT_VALID(this);

	// Restore control bar postion.
	LoadBarState(_T("Control Bar State"));

	// Restore frame window size and position.
	m_wndPosition.LoadWindowPos(this);
	nCmdShow = m_wndPosition.showCmd;

	return ShowWindow(nCmdShow);
}

void CMainFrame::OnSelchangeComboSelPrinter() 
{
	int	sel = m_wndComboSelPrinter.GetCurSel() ;
	if (sel != CB_ERR)
	{
		theApp.SetNewPrinter(sel) ;
	}
}

void CMainFrame::Sound_Play(CString Song)
{
	Sound_StopPlaying();

	Song = theApp.m_Exe_Working_Dir + "sound\\" + Song;
	char fname[1000];
	strcpy(fname,Song);

	CFileStatus status;
	if (CFile::GetStatus(Song,status))
	{
		i_Sound.PlaySoundFile(fname);
	}
}

void CMainFrame::Sound_Open()
{
	m_pRecordSound = new CRecordSound();
	m_pRecordSound->CreateThread();

	m_pPlaySound = new CPlaySound();
	m_pPlaySound->CreateThread();

	m_pWriteSound = new CWriteSoundFile();
	m_pWriteSound->CreateThread();

	m_pPlayMMSound = new CPlayMMSound();
	m_pPlayMMSound->CreateThread();

	if(m_pRecordSound)
	{
		m_pRecordSound->PostThreadMessage(WM_RECORDSOUND_SOUNDPLAYER,(WPARAM)0, (LPARAM)
			m_pPlaySound);
		m_pRecordSound->PostThreadMessage(WM_RECORDSOUND_WRITERTHREAD ,(WPARAM)0, (LPARAM)
			m_pWriteSound);
	}

	m_bRecording = false;
}

void CMainFrame::Sound_Close()
{
	if(m_pPlayMMSound)
	{
		m_pPlayMMSound->PostThreadMessage(WM_PLAYMMSOUND_ENDTHREAD,0,0);
		::WaitForSingleObject(m_pPlayMMSound->m_hThread, 5000);
		m_pPlayMMSound = NULL;
	}
	if(m_pPlaySound)
	{
		m_pPlaySound->PostThreadMessage(WM_PLAYSOUND_ENDTHREAD,0,0);
		::WaitForSingleObject(m_pPlaySound->m_hThread, 5000);
		m_pPlaySound = NULL;
	}
	if(m_pRecordSound)
	{
		m_pRecordSound->PostThreadMessage(WM_RECORDSOUND_ENDTHREAD,0,0);
		::WaitForSingleObject(m_pRecordSound->m_hThread, 5000);
		m_pRecordSound = NULL;
	}
	if(m_pWriteSound)
	{
		m_pWriteSound->PostThreadMessage(WM_WRITESOUNDFILE_ENDTHREAD,0,0);
		::WaitForSingleObject(m_pWriteSound->m_hThread, 5000);
		m_pWriteSound = NULL;
	}
}

void CMainFrame::OnDestroy() 
{

	if (g_HISConnectClient.Connected())
		g_HISConnectClient.NotifyExit();

	if (g_HISConnectClient1.Connected())
		g_HISConnectClient1.NotifyExit();


    //  TWAIN  Properties
	CloseSourceManager();
	UnloadSourceManager();
	
	//***  close COM port ***
	if (theApp.m_ComPort && m_commctrl.GetPortOpen())
	{
		// Set timeout period for 10 seconds from current time.
		BOOL bTimedOut = FALSE;
		CTime timeOut = CTime::GetCurrentTime() + CTimeSpan(0,0,0,10); 
		
		while (m_commctrl.GetOutBufferCount())
		{
			// Process all pending messages.
			DoEvents();
			
			if ( (CTime::GetCurrentTime() > timeOut) || bTimedOut)
			{
				int ret = AfxMessageBox("IDS_DATASENDFAILED",MB_ABORTRETRYIGNORE);
				
				switch (ret)
				{
					// Attempt to send data for another 10 seconds.
				case IDRETRY:
					timeOut = CTime::GetCurrentTime() + CTimeSpan(0,0,0,10);
					break;
					// Ignore timeout and exit. 
				case IDIGNORE:
					bTimedOut = TRUE;
					break;
					// Abort attempt to exit application.
				case IDABORT:
					return;
				}
			}
		}
		m_commctrl.SetPortOpen(FALSE);
	}
	//***********************

	CXTFrameWnd::OnDestroy();
	
	// 微软DSHOW设备
	if(theApp.m_CaptureMode.CompareNoCase("微软DSHOW设备") == 0)
	{
		DSHOW_SaveSettings();
		DSHOW_ReleaseCaptureWindow();

		SAFE_DELETE(cb.pBuffer);
		cb.lBufferSize = 0;
	}

	// MVC3000采集卡
	if(theApp.m_CaptureMode.CompareNoCase("MVC3000采集卡") == 0)
	{
		CIniReader m_IniReader;
		m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");
		CString str;
		str.Format("%d", m_CapInfo.Width);
		m_IniReader.setKey(str,"捕获窗口宽度","MVC3000采集卡");
		str.Format("%d", m_CapInfo.Height);
		m_IniReader.setKey(str,"捕获窗口高度","MVC3000采集卡");
		str.Format("%d", m_CapInfo.HorizontalOffset);
		m_IniReader.setKey(str,"捕获窗口水平偏移","MVC3000采集卡");
		str.Format("%d", m_CapInfo.VerticalOffset);
		m_IniReader.setKey(str,"捕获窗口垂直偏移","MVC3000采集卡");
		str.Format("%d", m_CapInfo.Exposure);
		m_IniReader.setKey(str,"曝光时间","MVC3000采集卡");
		str.Format("%d", m_CapInfo.Gain[0]);
		m_IniReader.setKey(str,"红色增益","MVC3000采集卡");
		str.Format("%d", m_CapInfo.Gain[1]);
		m_IniReader.setKey(str,"绿色增益","MVC3000采集卡");
		str.Format("%d", m_CapInfo.Gain[2]);
		m_IniReader.setKey(str,"蓝色增益","MVC3000采集卡");
		str.Format("%d", m_AE);
		m_IniReader.setKey(str,"自动曝光","MVC3000采集卡");
		str.Format("%d", m_CapInfo.Control);
		m_IniReader.setKey(str,"控制参数","MVC3000采集卡");
	}

	if(m_hMVC3000 != NULL)
	{
		MV_Usb2Uninit(&m_hMVC3000);
	}
	if(m_pRawData){
		free(m_pRawData);
		m_pRawData = NULL;
	}
	if(m_pRGBData){
		free(m_pRGBData);
		m_pRGBData = NULL;
	}

	// OK CARD Properties
	if( pExtThread != NULL )
	{
		pExtThread->m_Stop_Flag = true;
		DWORD dwExitCode;
		pExtThread->Stop(dwExitCode, 0);
		pExtThread->Kill(CThread::DW_OK,false);
		pExtThread->Stop(dwExitCode);
		delete pExtThread;
		pExtThread = NULL;
	}

	if(hBoard != NULL)
	{
		okStopCapture(hBoard);
		okCloseBoard(hBoard); 
	}

	if(m_TimerHandle)
	{
		KillTimer(m_TimerHandle);
		m_TimerHandle = 0;
	}
	if(m_pFrameGrabber != NULL)
	{
		m_pFrameGrabber->DestroyWindow();
		delete m_pFrameGrabber;
		m_pFrameGrabber = NULL;
	}

	if(m_Close_TimerHandle)
	{
		KillTimer(m_Close_TimerHandle);
		m_Close_TimerHandle = 0;
	}

	if (m_pWndCanonS5Capture != NULL)
	{

		m_pWndCanonS5Capture->DestroyWindow();

		delete m_pWndCanonS5Capture;
		m_pWndCanonS5Capture = NULL;

		(prVoid)PR_FinishSDK();
	}

	Sound_Play("quit.wav");
	Sound_Close();

  	if (pRecordTiming != NULL)
  	{
		pRecordTiming->DestroyWindow();
		SAFE_DELETE(pRecordTiming);
  	}
}

void CMainFrame::Sound_StopPlaying()
{
	if( m_bRecording )
	{
		m_pRecordSound->PostThreadMessage(WM_RECORDSOUND_STOPRECORDING, 0, 0L);
		m_bRecording = FALSE;

		// (1) Send Message to notice CMedSound to save the new generated wave file to database
		// (2) Delete corresponding temporary wave file in exchange directory

		Sleep(1000);
		CString transname;
		transname.Format("%s_cmipstrans.wav", theApp.GetView()->GetDocument()->m_blh);
		
		CString fstr = theApp.m_SysTmpPath;
		fstr += transname;

		//-------------------------------------------------------
		int maxno = 0;
		theApp.ConnectDatabase();
		try
		{
			SAString cmdstr = "Select serialno from " + theApp.TABLE_SOUND + "  where medicalno = :medicalno order by serialno desc";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			
			g_dbcommand.Execute();
			if(g_dbcommand.FetchNext())
			{
				maxno = g_dbcommand.Field("serialno").asLong();
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
		
		SAString  snddataset;
		snddataset.Empty();
		
		CFile file;
		CFileException fe;
		if (file.Open(fstr, CFile::modeRead | CFile::shareDenyNone, &fe))
		{
			char sBuf[32*1024];
			while( 1 )
			{
				if( file.GetPosition() >= file.GetLength() )  break;
				DWORD nRead = file.ReadHuge(sBuf, sizeof(sBuf) );
				snddataset += SAString( sBuf, nRead );
			}
			file.Close();
		}
		
		try
		{
			SAString cmdstr = "Insert into " + theApp.TABLE_SOUND + "(medicalno, serialno, snddataset) values(:medicalno,:serialno,:snddataset)";
			
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Param("serialno").setAsLong()    = ++maxno;
			g_dbcommand.Param("snddataset").setAsBLob()  = snddataset;
			
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
		
		snddataset.Empty();
	
		CWnd *pWnd = m_wndTabDockBar.GetView(RUNTIME_CLASS(CMedSound));
		if(pWnd != NULL)  ((CMedSound *)pWnd)->LoadThumbnail();
		//-------------------------------------------------------

		remove(fstr);
	}
    if( m_pPlayMMSound )	
	{
		m_pPlayMMSound->PostThreadMessage( WM_PLAYMMSOUND_CLOSEFILE,0,0);

		// (1) Delete corresponding temporary wave file in exchange directory
		Sleep(1000);
		CString transname;
		transname.Format("%s_cmipstrans.wav", theApp.GetView()->GetDocument()->m_blh);
		
		CString fstr = theApp.m_SysTmpPath;
		fstr += transname;
		remove(fstr);
	}

  	if (pRecordTiming != NULL)
  	{
		pRecordTiming->DestroyWindow();
		SAFE_DELETE(pRecordTiming);
  	}
}

void CMainFrame::Sound_Startplay(CString Song)
{
	if( m_bRecording )   return;

    if( m_pPlayMMSound )	
	{
		m_pPlayMMSound->PostThreadMessage( WM_PLAYMMSOUND_CLOSEFILE,0,0);
	}

    if( m_pPlayMMSound )	
	{
		char fname[1000];
		strcpy(fname,Song);

		CFileStatus status;
		if (CFile::GetStatus(fname,status))
		{
			m_pPlayMMSound->PostThreadMessage( WM_PLAYMMSOUND_PLAYSOUNDPTR ,0,(LPARAM)m_pPlaySound);
			m_pPlayMMSound->PostThreadMessage( WM_PLAYMMSOUND_PLAYFILE,0,(LPARAM)fname);

			//=================================================================
			CWAVInfo wavinfo;
			wavinfo.loadInfo( Song );
			int seconds = wavinfo.getLengthInSeconds();
			if( seconds > 3 )
			{
				if (pRecordTiming != NULL)
				{
					pRecordTiming->DestroyWindow();
					delete pRecordTiming;
					pRecordTiming = NULL;
				}
				
				pRecordTiming = new CRecordTiming(AfxGetMainWnd());
				pRecordTiming->m_LedText.m_TotalSeconds = seconds ;
				pRecordTiming->Create();
				pRecordTiming->SetActiveWindow();
				pRecordTiming->ShowWindow(SW_SHOW);
				
	            UINT ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
				CRect WndRect;
				pRecordTiming->GetWindowRect(&WndRect);
				pRecordTiming->SetWindowPos(NULL,ScreenWidth - WndRect.Width(), 0 ,WndRect.Width(),WndRect.Height(),SWP_NOZORDER);	
			}
			//=================================================================
		}
	}
}

void CMainFrame::Sound_Startrecord(CString Song)
{
	if( m_bRecording )   return;
	
	Sound_StopPlaying();

    if( m_pPlayMMSound )	
	{
		m_pPlayMMSound->PostThreadMessage( WM_PLAYMMSOUND_CLOSEFILE,0,0);
	}

	char fname[1000];
	strcpy(fname,Song);

	strcpy( m_pRecordSound->m_SoundFileName, fname );

	m_pRecordSound->PostThreadMessage(WM_RECORDSOUND_STARTRECORDING, 0, 0L);
	m_bRecording = TRUE;

  	if (pRecordTiming != NULL)
  	{
		pRecordTiming->DestroyWindow();
  		delete pRecordTiming;
  		pRecordTiming = NULL;
  	}

    pRecordTiming = new CRecordTiming(AfxGetMainWnd());
	pRecordTiming->Create();
	pRecordTiming->SetActiveWindow();
	pRecordTiming->ShowWindow(SW_SHOW);

	UINT ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	CRect WndRect;
	pRecordTiming->GetWindowRect(&WndRect);
	pRecordTiming->SetWindowPos(NULL,ScreenWidth - WndRect.Width(), 0 ,WndRect.Width(),WndRect.Height(),SWP_NOZORDER);	
}

void CMainFrame::OnViewToolbar() 
{
	// TODO: Add your command handler code here
	ShowControlBar(&m_wndToolBar, !m_wndToolBar.IsWindowVisible(), FALSE);
}

void CMainFrame::OnUpdateViewToolbar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck((m_wndToolBar.GetStyle() & WS_VISIBLE) != 0);
}

void CMainFrame::OnViewMedlist() 
{
	// TODO: Add your command handler code here
	ShowControlBar(&m_wndTabDockBar, !m_wndTabDockBar.IsWindowVisible(), FALSE);
}

void CMainFrame::OnUpdateViewMedlist(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_wndTabDockBar.IsWindowVisible());
}

void CMainFrame::OnFieldNotification() 
{
	// TODO: Add your command handler code here
    CRecordInput mydlg(this);
	
	for(int i = 0; i < theApp.m_fnamelist.GetSize(); i++)
	{
		mydlg.m_NameList.Add(theApp.m_fnamelist.GetAt(i));
		mydlg.m_FieldName.Add(theApp.m_fdescriptionlist.GetAt(i));
		mydlg.m_FieldList.Add(theApp.m_finfolist.GetAt(i));
	}

	mydlg.DoModal();
}

void CMainFrame::OnMaintanceReport() 
{
	// TODO: Add your command handler code here
	BuweiBZ Dlg(this);
	Dlg.DoModal();
}

void CMainFrame::OnReportDiscribe() 
{
	// TODO: Add your command handler code here
	CString filenm;
	filenm=theApp.m_Exe_Working_Dir;filenm+="system/描述用词.dat";
	CZhengDuan Dlg(filenm,this);
	Dlg.DoModal();
}

void CMainFrame::OnReportRegion() 
{
	// TODO: Add your command handler code here
	CString filenm;
	filenm=theApp.m_Exe_Working_Dir;filenm+="system/位置用词.dat";
	CZhengDuan Dlg(filenm,this);
	Dlg.DoModal();
}

void CMainFrame::OnReportDia() 
{
	// TODO: Add your command handler code here
	CString filenm;
	filenm=theApp.m_Exe_Working_Dir;filenm+="system/诊断用词.dat";
	CZhengDuan Dlg(filenm,this);
	Dlg.DoModal();
}

void CMainFrame::OnReportRysj() 
{
	// TODO: Add your command handler code here
	CString filenm;
	filenm=theApp.m_Exe_Working_Dir;filenm+="system/所见用词.dat";
	CZhengDuan Dlg(filenm,this);
	Dlg.DoModal();	
}

void CMainFrame::OnAcquire() 
{
	BeginAcquire();
}

void CMainFrame::OnUpdateAcquire(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(State() == SOURCE_MANAGER_OPEN);
}

void CMainFrame::OnSelectSource() 
{
	SelectSource();
}

void CMainFrame::OnUpdateSelectSource(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(State() == SOURCE_MANAGER_OPEN);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	CPoint pt;
	CRect rect;

	if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (m_pWndCanonS5Capture != NULL)
		{
			if (m_pWndCanonS5Capture->IsCreated() && m_pWndCanonS5Capture->IsWindowVisible())
			{
				::GetCursorPos(&pt);
			
				::GetWindowRect(m_pWndCanonS5Capture->m_hWnd, &rect);

				///if (!rect.PtInRect(pt))
				///	::SendMessage(m_pWndCanonS5Capture->m_hWnd, WM_COMMAND, IDCANCEL, 0);
			}
		}
	}
	//  TWAIN  Properties
	/*
	if( pMsg->message == WM_KEYDOWN)
		{
			UINT nChar = pMsg->wParam;
			if(nChar == VK_F2)
			{
						
			}
		}*/
	
	return TwainMessageHook(pMsg) ||
		   CXTFrameWnd::PreTranslateMessage(pMsg);
	
	//return CXTFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::TwainError(TW_ERR e)
{
	AfxMessageBox("A TWAIN error occurred.", MB_ICONINFORMATION | MB_OK);
} // TwainError

int CMainFrame::NegotiateCapabilities(void)
{
   if (m_nXfer != 0) {
      if (!SetXferCount(m_nXfer)) {
         if (IDYES != AfxMessageBox("Unable to set xfer count.\nContinue with Acquire?", MB_ICONINFORMATION | MB_YESNO)) {
            return FALSE;
         }
      }
   }
   if (m_nType != -1) {
      if (!SetCurrentPixelType(m_nType)) {
         if (IDYES != AfxMessageBox("Unable to set pixel type.\nContinue with Acquire?", MB_ICONINFORMATION | MB_YESNO)) {
            return FALSE;
         }
      }
   }
   if (m_nRes != 0) {
      if (!SetCurrentResolution(m_nRes)) {
         if (IDYES != AfxMessageBox("Unable to set resolution.\nContinue with Acquire?", MB_ICONINFORMATION | MB_YESNO)) {
            return FALSE;
         }
      }
   }
   return TRUE;
}

void CMainFrame::DibReceived(HGLOBAL hdib)
{
	if( hdib == NULL )   return;
	
	AddNewImage( (HDIB) hdib );

	::GlobalFree((HGLOBAL) hdib);
	hdib = NULL;
	return;
}

void CMainFrame::AddNewImage (CString fname)
{
	char instanceUID[100];

	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  return;

	CFileStatus state;
	if(CFile::GetStatus(fname,state) == FALSE)  return;
	
	BeginWaitCursor();
	
	CString transname;
	transname.Format("%s_cmipstrans.dat", theApp.GetView()->GetDocument()->m_blh);
	CString fstr = theApp.m_SysTmpPath;
	fstr += transname;

	int maxno = 0;
	theApp.ConnectDatabase();
	try
	{
		SAString cmdstr = "Select serialno from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno order by serialno desc";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
		g_dbcommand.Execute();
		if(g_dbcommand.FetchNext())
		{
			maxno = g_dbcommand.Field("serialno").asLong();
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
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Param("instanceUID").setAsString() = instanceUID;
		g_dbcommand.Param("serialno").setAsLong()    = ++maxno;
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
	
	remove(fname);
	remove(fstr);
	
	EndWaitCursor();

	CWnd *pWnd = m_wndTabDockBar.GetView(RUNTIME_CLASS(CMedImage));
	if(pWnd != NULL)  ((CMedImage *)pWnd)->LoadThumbnail();
}

void CMainFrame::AddNewImage(HDIB NewDib)
{
	char instanceUID[100];

	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  return;

	int maxno = 0;
	theApp.ConnectDatabase();
	try
	{
		SAString cmdstr = "Select serialno from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno order by serialno desc";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		
		g_dbcommand.Execute();
		if(g_dbcommand.FetchNext())
		{
			maxno = g_dbcommand.Field("serialno").asLong();
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
	
	CWnd *pWnd = m_wndTabDockBar.GetView(RUNTIME_CLASS(CMedImage));
	if(pWnd != NULL)  ((CMedImage *)pWnd)->LoadNewThumb(NewDib, maxno + 1);

	CFile file;
	CFileException fe;
	CString tmpimg = theApp.m_SysTmpPath;
	tmpimg += "00000001.bmp";
	remove(tmpimg);
	
	if (!file.Open( tmpimg, CFile::modeCreate |
		CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		return;
	}

	BeginWaitCursor();
	
	TRY
	{
		::SaveDIB( NewDib, file );
		file.Close();
	}
	CATCH (CException, eSave)
	{
		file.Abort(); // will not throw an exception
		return;
	}
	END_CATCH

	CString transname;
	transname.Format("%s_cmipstrans.dat", theApp.GetView()->GetDocument()->m_blh);
	CString fstr = theApp.m_SysTmpPath;
	fstr += transname;
	remove(fstr);
	
	GDALDataset * m_pSrcDataset = (GDALDataset *) GDALOpen ( tmpimg, GA_ReadOnly );
	GDALDataset * m_pDstDataset = GS_CreateCopy( theApp.m_SavingMode, fstr, m_pSrcDataset);
	GDALClose(m_pSrcDataset);
	GDALClose(m_pDstDataset);
	
	SAString  imgdataset;
	imgdataset.Empty();
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
		SAString cmdstr = "Insert into " + theApp.TABLE_IMAGE + "(medicalno, instanceUID, serialno, imgdataset) values(:medicalno,:instanceUID, :serialno,:imgdataset)";
		
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Param("instanceUID").setAsString() = instanceUID;
		g_dbcommand.Param("serialno").setAsLong()    = ++maxno;
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

	remove(tmpimg);
	remove(fstr);
	
	EndWaitCursor();
}

LRESULT CMainFrame::SwitchOn(WPARAM wparam, LPARAM lparam)
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

	if( m_realtime && pImageView )
	{
		pImageView->SendMessage(WM_LBUTTONDOWN);
	}
	
	return 0L;
}

LRESULT CMainFrame::OkCapture(WPARAM wparam, LPARAM lparam)
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

	if(m_realtime)
	{
		if( pExtThread != NULL )
		{
			pExtThread->m_Stop_Flag = true;
			DWORD dwExitCode;
			pExtThread->Stop(dwExitCode, 0);
			pExtThread->Kill(CThread::DW_OK,false);
			pExtThread->Stop(dwExitCode);
			delete pExtThread;
			pExtThread = NULL;
		}
		
		if(hBoard)
		{
			okStopCapture(hBoard);
		}
		
		m_realtime = false;
		
		if( pImageView != NULL)	 pImageView->RedrawWindow();
	}
	else
	{
		//=======================================================
		if( pImageView )
		{
			CDC * pDC = pImageView->GetDC();
			
			CRect ClientRect;
			pImageView->GetClientRect(&ClientRect);
			
			CPen   * n_old_pen   = (CPen   *)pDC->SelectStockObject(NULL_PEN);
			CBrush * n_old_brush = (CBrush *)pDC->SelectStockObject(GRAY_BRUSH);
			
			pDC->Rectangle(&ClientRect);
			
			pDC->SelectObject(n_old_pen);
			pDC->SelectObject(n_old_brush);
			pImageView->ReleaseDC(pDC);
		}
		//=======================================================
		if( pImageView )
		{
			extern CRect SizeRectWithConstantAspectRatio(CRect* rcScreen,  CSize  sizePicture,  BOOL   bCenter);
			CSize  sizePicture;
			sizePicture.cx = InputWnd_Width;
			sizePicture.cy = InputWnd_Height;
			CRect rcScreen;
			pImageView->GetClientRect(&rcScreen);
			
			if( rcScreen.Width() < InputWnd_Width || rcScreen.Height() < InputWnd_Height )
			{
				rcScreen = SizeRectWithConstantAspectRatio(&rcScreen, sizePicture, TRUE);
			}
			else
			{
				int w = rcScreen.Width();
				int h = rcScreen.Height();
				rcScreen.left   = ( w - InputWnd_Width ) / 2;
				rcScreen.right  = rcScreen.left + InputWnd_Width;
				rcScreen.top    = ( h - InputWnd_Height) / 2;
				rcScreen.bottom = rcScreen.top  + InputWnd_Height;
			}
			
			if( hBoard )
			{
				RECT rct;
				rct.left   = rcScreen.left;
				rct.right  = rcScreen.right;
				rct.top    = rcScreen.top;
				rct.bottom = rcScreen.bottom;
				::MapWindowPoints(pImageView->m_hWnd, HWND_DESKTOP, (LPPOINT)&rct, 2);
				
				okSetTargetRect(hBoard,SCREEN,&rct);
				
				m_realtime = true;
				okCaptureToScreen(hBoard);
			}
		}
		//=======================================================
		
		if( hBoard )
		{
			if( pExtThread != NULL )
			{
				pExtThread->m_Stop_Flag = true;
				DWORD dwExitCode;
				pExtThread->Stop(dwExitCode, 0);
				pExtThread->Kill(CThread::DW_OK,false);
				pExtThread->Stop(dwExitCode);
				delete pExtThread;
				pExtThread = NULL;
			}
			
			if(okGetSignalParam(hBoard, SIGNAL_EXTTRIGGER) >= 0) 
			{
				pExtThread = new CGDIThread();
			}
			
			if (pExtThread != NULL)
			{
				// Create Thread in a suspended state so we can set the Priority
				// before it starts getting away from us
				try
				{
					pExtThread->Start();
					pExtThread->SetPriority(THREAD_PRIORITY_IDLE);// THREAD_PRIORITY_NORMAL
					pExtThread->m_pFrame = this;
				}
				catch (CException* pe)
				{
					pe->ReportError();
					pe->Delete();
					delete pExtThread;
					pExtThread = NULL;
					return 0L;
				}
			}
		}
	}
	
	return 0L;
}

HDIB  CMainFrame::CaptureOneImage()
{
	if(theApp.m_CaptureMode.CompareNoCase("VFW采集设备支持") == 0)
	{
		if( m_pFrameGrabber == NULL )  return NULL;
		
		CSize imgsize = m_pFrameGrabber->GetImageSize();
		HDIB hDIB = ::CreateDIBFile(imgsize.cx,imgsize.cy,24);
		LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
		BYTE *   lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
		int LineByteWidth = WIDTHBYTES( imgsize.cx * 24 );
		
		LPBITMAPINFO lpBi = m_pFrameGrabber->GetDIB();
		memcpy(lpDIBBits, (BYTE *)::FindDIBBits((char *)lpBi), LineByteWidth * imgsize.cy);
		
		//----------------------------------------
		if(hBoard && m_Ok_Unmatch)
		{
			//memset(lpDIBBits, 0, LineByteWidth * imgsize.cy);
		}
		//----------------------------------------
		
		::GlobalUnlock((HGLOBAL) hDIB);
		
		return hDIB;
	}

	if(theApp.m_CaptureMode.CompareNoCase("Ok图像采集卡") == 0)
	{
		if( hBoard == NULL )  return NULL;
		
		HDIB hDIB = ::CreateDIBFile(InputWnd_Width,InputWnd_Height,24);
		LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
		BYTE *   lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
		int LineByteWidth = WIDTHBYTES( InputWnd_Width * 24 );
		
		okCaptureSingle(hBoard,BUFFER,0);
		Sleep(200);                           // added for 陆军总院  第一帧图像异常
		okCaptureSingle(hBoard,BUFFER,0);
		
		okReadRect(hBoard,BUFFER,0,lpDIBBits);
		BYTE *buf = new BYTE[LineByteWidth]; 
		for(int i = 0; i < InputWnd_Height / 2; i++ )
		{
			memcpy(buf, lpDIBBits + i * LineByteWidth, LineByteWidth);
			memcpy(lpDIBBits + i * LineByteWidth, lpDIBBits + (InputWnd_Height - i - 1) * LineByteWidth,LineByteWidth);
			memcpy(lpDIBBits + (InputWnd_Height - i - 1) * LineByteWidth, buf, LineByteWidth);
		}
		delete buf;
		
		//----------------------------------------
		if(hBoard && m_Ok_Unmatch)
		{
			memset(lpDIBBits, 0, LineByteWidth * InputWnd_Height);
		}
		//----------------------------------------
		
		::GlobalUnlock((HGLOBAL) hDIB);
		
		if(m_realtime)
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
				extern CRect SizeRectWithConstantAspectRatio(CRect* rcScreen,  CSize  sizePicture,  BOOL   bCenter);
				CSize  sizePicture;
				sizePicture.cx = InputWnd_Width;
				sizePicture.cy = InputWnd_Height;
				CRect rcScreen;
				pImageView->GetClientRect(&rcScreen);
				
				if( rcScreen.Width() < InputWnd_Width || rcScreen.Height() < InputWnd_Height )
				{
					rcScreen = SizeRectWithConstantAspectRatio(&rcScreen, sizePicture, TRUE);
				}
				else
				{
					int w = rcScreen.Width();
					int h = rcScreen.Height();
					rcScreen.left   = ( w - InputWnd_Width ) / 2;
					rcScreen.right  = rcScreen.left + InputWnd_Width;
					rcScreen.top    = ( h - InputWnd_Height) / 2;
					rcScreen.bottom = rcScreen.top  + InputWnd_Height;
				}
				
				if( hBoard )
				{
					RECT rct;
					rct.left   = rcScreen.left;
					rct.right  = rcScreen.right;
					rct.top    = rcScreen.top;
					rct.bottom = rcScreen.bottom;
					::MapWindowPoints(pImageView->m_hWnd, HWND_DESKTOP, (LPPOINT)&rct, 2);
					
					okSetTargetRect(hBoard,SCREEN,&rct);
					
					okCaptureToScreen(hBoard);
				}
			}
			//=======================================================
		}
		
		return hDIB;
	}

	if(theApp.m_CaptureMode.CompareNoCase("DH-HVxxxxU系列采集卡") == 0)
	{
		if( m_hhv == NULL )  return NULL;
		
		ConvertBayer2Rgb(m_pImageBuffer, m_pRawBuffer,nMaxWidMd0,nMaxHeiMd0, m_ConversionType,m_pLutR,m_pLutG,m_pLutB,true,m_Layout);
		
		HDIB hDIB = ::CreateDIBFile( nMaxWidMd0, nMaxHeiMd0, 24 );
		LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
		BYTE *   lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
		memcpy( lpDIBBits, m_pImageBuffer, nMaxWidMd0 * nMaxHeiMd0 * 3 );
		::GlobalUnlock((HGLOBAL) hDIB);
		
		return hDIB;
	}

	if(theApp.m_CaptureMode.CompareNoCase("微软DSHOW设备") == 0)
	{
		DSHOW_Switch_CaptureOn(this->m_hWnd);
		return NULL;
	}

	if(theApp.m_CaptureMode.CompareNoCase("MVC3000采集卡") == 0)
	{
		CString fstr = theApp.m_SysTmpPath;
		fstr += "MVC3000.bmp";
		remove(fstr);

		if (m_hMVC3000)
		{
			if(MV_Usb2SaveFrameAsBmp(m_hMVC3000, &m_CapInfo, m_pRGBData, fstr) == 0)
			{
				AddNewImage(fstr);
				remove(fstr);
			}

			return NULL;
		}
	}

	if(theApp.m_CaptureMode.CompareNoCase("DirectShow采集设备支持") == 0)
	{
		if( m_VideoCap.GetSafeHwnd() == NULL)  return NULL;
		
		CString fstr = theApp.m_SysTmpPath;
		fstr += "dshow.bmp";
		remove(fstr);

		if(m_TimerHandle)
		{
			KillTimer(m_TimerHandle);
			m_TimerHandle = 0;
		}

		HDIB hDIB = NULL;
		
		if( m_VideoCap.SnapShot(fstr) )
		{
			GDALDataset * m_pDataset = (GDALDataset *) GDALOpen ( fstr , GA_ReadOnly );

			if( m_pDataset != NULL )
			{
				hDIB = ::CreateDIBFile( m_pDataset->GetRasterXSize(),m_pDataset->GetRasterYSize(), 24 );
				LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
				BYTE *   lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
				int LineByteWidth = WIDTHBYTES( m_pDataset->GetRasterXSize() * 24 );
				memset( lpDIBBits, 255, LineByteWidth * m_pDataset->GetRasterYSize() );

				BYTE *pRBits = lpDIBBits + 2;
				BYTE *pGBits = lpDIBBits + 1;
				BYTE *pBBits = lpDIBBits + 0;
				
				int BandNo[3];
				int BandNum = m_pDataset->GetRasterCount();
				if( BandNum == 1)
				{
					BandNo[0] = 1;	BandNo[1] = 1;	BandNo[2] = 1;
				}
				else
				{
					BandNo[0] = 1;	BandNo[1] = 2;	BandNo[2] = 3;
				}
				
				m_pDataset->GetRasterBand(BandNo[0])->RasterIO( GF_Read, 0, 0, 
					                                            m_pDataset->GetRasterXSize(), 
					                                            m_pDataset->GetRasterYSize(), 
					                                            (void *)(pRBits), 
					                                            m_pDataset->GetRasterXSize(), 
					                                            m_pDataset->GetRasterYSize(), 
					                                            GDT_Byte, 
					                                            3 , LineByteWidth );
				m_pDataset->GetRasterBand(BandNo[1])->RasterIO( GF_Read, 0, 0, 
					                                            m_pDataset->GetRasterXSize(), 
					                                            m_pDataset->GetRasterYSize(), 
					                                            (void *)(pGBits), 
					                                            m_pDataset->GetRasterXSize(), 
					                                            m_pDataset->GetRasterYSize(), 
					                                            GDT_Byte, 
					                                            3 , LineByteWidth );
				m_pDataset->GetRasterBand(BandNo[2])->RasterIO( GF_Read, 0, 0, 
					                                            m_pDataset->GetRasterXSize(), 
					                                            m_pDataset->GetRasterYSize(), 
					                                            (void *)(pBBits), 
					                                            m_pDataset->GetRasterXSize(), 
					                                            m_pDataset->GetRasterYSize(), 
					                                            GDT_Byte, 
					                                            3 , LineByteWidth );

				BYTE *buf = new BYTE[LineByteWidth];
				BYTE *u1,*u2;
				for(int j = 0; j < (m_pDataset->GetRasterYSize()/2); j++ )
				{
					u1 = lpDIBBits + j * LineByteWidth;
					u2 = lpDIBBits + (m_pDataset->GetRasterYSize() - j - 1) * LineByteWidth;
					memcpy(buf, u1,  LineByteWidth);
					memcpy(u1,  u2,  LineByteWidth);
					memcpy(u2,  buf, LineByteWidth);
				}
				delete buf;

				GDALClose(m_pDataset);
				
				::GlobalUnlock((HGLOBAL) hDIB);
			}
		}
        
		remove(fstr);

		m_TimerHandle = SetTimer(1, 100, NULL);

		return hDIB;
	}

	return NULL;
}

void  CMainFrame::TerminateTimer(bool BeFreshWnd)
{
	if(theApp.m_MainThread_FrameWnd != theApp.m_pMainWnd)  
	{
		// 不是主线程调用（例如工作在打印预览模式）
		return;
	}

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

	if(theApp.m_CaptureMode.CompareNoCase("VFW采集设备支持") == 0)
	{
		if(m_realtime && m_TimerHandle)
		{
			if(m_TimerHandle)
			{
				KillTimer(m_TimerHandle);
				m_TimerHandle = 0;
			}
			
			if(m_pFrameGrabber != NULL)
			{
				m_pFrameGrabber->DestroyWindow();
				delete m_pFrameGrabber;
				m_pFrameGrabber = NULL;
			}
			m_realtime = false;
			
			if( pImageView != NULL && BeFreshWnd )	pImageView->RedrawWindow();
		}
	}

	if(theApp.m_CaptureMode.CompareNoCase("Ok图像采集卡") == 0)
	{
		if( pExtThread != NULL )
		{
			pExtThread->m_Stop_Flag = true;
			DWORD dwExitCode;
			pExtThread->Stop(dwExitCode, 0);
			pExtThread->Kill(CThread::DW_OK,false);
			pExtThread->Stop(dwExitCode);
			delete pExtThread;
			pExtThread = NULL;
		}
		
		if(m_realtime)
		{
			if(hBoard)
			{
				okStopCapture(hBoard);
				m_realtime = false;
			}
			
			if( pImageView != NULL && BeFreshWnd )	pImageView->RedrawWindow();
		}
	}

	if(theApp.m_CaptureMode.CompareNoCase("DH-HVxxxxU系列采集卡") == 0)
	{
		HVSTATUS status = STATUS_OK;
		if(m_realtime)
		{
			if( m_hhv )
			{
				status = HVCloseSnap(m_hhv);
				m_realtime = false;
			}
			
			if( pImageView != NULL && BeFreshWnd )	pImageView->RedrawWindow();
		}
	}

	if(theApp.m_CaptureMode.CompareNoCase("DirectShow采集设备支持") == 0)
	{ 
		if(m_realtime && m_TimerHandle)
		{
			if(m_TimerHandle)
			{
				KillTimer(m_TimerHandle);
				m_TimerHandle = 0;
			}
			
			m_VideoCap.Stop();
			m_realtime = false;
			
			if( pImageView != NULL && BeFreshWnd )	pImageView->RedrawWindow();
		}
	}

	if(theApp.m_CaptureMode.CompareNoCase("微软DSHOW设备") == 0)
	{ 
		if(m_realtime)
		{
			DSHOW_ChangePreviewState(FALSE);
			DSHOW_ShowWindow(SW_HIDE);
			m_realtime = false;
		
			if( pImageView != NULL && BeFreshWnd )	pImageView->RedrawWindow();
		}
	}

	if(theApp.m_CaptureMode.CompareNoCase("MVC3000采集卡") == 0)
	{
		if (m_hMVC3000)
		{
			MV_Usb2Stop(m_hMVC3000);
			m_realtime = false;
			if( pImageView != NULL && BeFreshWnd )	pImageView->RedrawWindow();
		}
	}
}

void CMainFrame::OnImgCapturestp() 
{
	CPathologyView *pView = theApp.GetView();
	if(pView != NULL)
	{
		CWnd *pWnd = pView->m_tabCtrl.GetView(RUNTIME_CLASS(CImageDockWnd));
		if(pWnd != NULL)  pView->m_tabCtrl.SetActiveView(pWnd);

		pWnd = m_wndTabDockBar.GetView(RUNTIME_CLASS(CMedImage));
		if(pWnd != NULL)  m_wndTabDockBar.SetActiveView(pWnd);
	}

	if(theApp.m_CaptureMode.CompareNoCase("VFW采集设备支持") == 0)
	{
		if(m_pFrameGrabber != NULL)
		{
			m_pFrameGrabber->VideoSourceDialog();
		}
	}

	if(theApp.m_CaptureMode.CompareNoCase("Ok图像采集卡") == 0)
	{
		if( !hBoard )  return;
		
		okOpenSetParamDlg(hBoard, NULL);
		
		okStopCapture(hBoard);
		okCloseBoard(hBoard); 
		
		long iCurrUsedNo = -1;
		hBoard = okOpenBoard(&iCurrUsedNo);
		if( hBoard )
		{
			long lg = okSetCaptureParam(hBoard, CAPTURE_BUFBLOCKSIZE, -1);
			InputWnd_Width  = LOWORD(lg);
			InputWnd_Height = HIWORD(lg);
		}
	}

	if(theApp.m_CaptureMode.CompareNoCase("DH-HVxxxxU系列采集卡") == 0)
	{
		OnDhSetup();
	}

	if(theApp.m_CaptureMode.CompareNoCase("DirectShow采集设备支持") == 0)
	{
		if(m_VideoCap.GetSafeHwnd())
		{
			if(!m_realtime)
			{
				SendMessage(WM_DSHOWCAPTURE);
			}
			m_VideoCap.ShowVideoCapturePropertyPage();
		}
	}

	if(theApp.m_CaptureMode.CompareNoCase("MVC3000采集卡") == 0)
	{
		if (m_hMVC3000 != NULL)
		{
			CMVCSetup mydlg(this);
			mydlg.DoModal();
		}
	}

	if(theApp.m_CaptureMode.CompareNoCase("微软DSHOW设备") == 0)
	{
		DSHOW_VideoSettings(this->m_hWnd);
	}
}

void CMainFrame::OnUpdateImgCapturestp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( RIGHT_XGXTSZ(theApp.m_Cpp_Right) );
}

LRESULT CMainFrame::DhhvCapture(WPARAM wparam, LPARAM lparam)
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

	HVSTATUS status = STATUS_OK;
	if(m_realtime)
	{
		if( m_hhv )
		{
			status = HVCloseSnap(m_hhv);
		}
		
		m_realtime = false;
		
		if( pImageView != NULL)	pImageView->RedrawWindow();
	}
	else
	{
		//=======================================================
		if( pImageView )
		{
			CDC * pDC = pImageView->GetDC();
			
			CRect ClientRect;
			pImageView->GetClientRect(&ClientRect);
			
			CPen   * n_old_pen   = (CPen   *)pDC->SelectStockObject(NULL_PEN);
			CBrush * n_old_brush = (CBrush *)pDC->SelectStockObject(GRAY_BRUSH);
			
			pDC->Rectangle(&ClientRect);
			
			pDC->SelectObject(n_old_pen);
			pDC->SelectObject(n_old_brush);
			pImageView->ReleaseDC(pDC);
		}
		//=======================================================
		if( pImageView && m_hhv)
		{
			if(m_pImageBuffer && m_pRawBuffer){
				HVSTATUS status = HVOpenSnap(m_hhv, SnapCallback, this);
				if (HV_SUCCESS(status)) {
					
					//只定义一个缓冲区,
					BYTE * ppBuffer[1];
					ppBuffer[0] = m_pRawBuffer;
					status = HVStartSnap(m_hhv,ppBuffer, 1);
					
					if ( ! HV_SUCCESS(status))
					{
						HVCloseSnap(m_hhv);
					}
					else{
						m_realtime = true;
						
						m_DigitalOut_NeedUpdateWnd = true;
					}
				}
				HV_MESSAGE(status);
			}
		}
	}

	return 0L;
}

LRESULT CMainFrame::VfwCapture(WPARAM wparam, LPARAM lparam)
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

	if(m_realtime)
	{
		if(m_TimerHandle)
		{
			KillTimer(m_TimerHandle);
			m_TimerHandle = 0;
		}
		
		if(m_pFrameGrabber != NULL)
		{
			m_pFrameGrabber->DestroyWindow();
			delete m_pFrameGrabber;
			m_pFrameGrabber = NULL;
		}
		
		m_realtime = false;
		
		if( pImageView != NULL)	pImageView->RedrawWindow();
	}
	else
	{
		//=======================================================
		if( pImageView )
		{
			CDC * pDC = pImageView->GetDC();
			
			CRect ClientRect;
			pImageView->GetClientRect(&ClientRect);
			
			CPen   * n_old_pen   = (CPen   *)pDC->SelectStockObject(NULL_PEN);
			CBrush * n_old_brush = (CBrush *)pDC->SelectStockObject(GRAY_BRUSH);
			
			pDC->Rectangle(&ClientRect);
			
			pDC->SelectObject(n_old_pen);
			pDC->SelectObject(n_old_brush);
			pImageView->ReleaseDC(pDC);
		}
		//=======================================================
		if( pImageView )
		{
			if(m_pFrameGrabber == NULL)
			{
				m_pFrameGrabber = new CFrameGrabber;
				BOOL rt = m_pFrameGrabber->Create(0,0,this);
				if(rt == FALSE)
				{
					if(m_pFrameGrabber != NULL)
					{
						m_pFrameGrabber->DestroyWindow();
						delete m_pFrameGrabber;
						m_pFrameGrabber = NULL;
					}
					return 0L;
				}
				m_TimerHandle = SetTimer(1, 100, NULL);
				m_realtime = true;
			}
		}
		//=======================================================
	}

	return 0L;
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	
	// TODO: Add your message handler code here and/or call default
	static bool mute = false;

	if(nIDEvent == 8888){ //科内留言
		long count;
		try{
			SAString cmdstr = "select count(*) from "+theApp.TABLE_NOTEPAD+" where jsr=:jsr and zt='0'";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("jsr").setAsString() = theApp.m_Cpp_User;
			g_dbcommand.Execute();
			while(g_dbcommand.FetchNext()){
				count = g_dbcommand.Field(1).asLong();
			}
			g_dbconnection.Commit();
		} catch (SAException &e){
			AfxMessageBox((const char*)e.ErrText());
		}

		if(user_old.CompareNoCase(theApp.m_Cpp_User) != 0){
			count_old = 0;
		} 
		
		if(count > 0 && (count > count_old)){
			//CLeaveMessage dlg;
			pLeaveMessage = new CLeaveMessage;
			pLeaveMessage->count.Format("%d",count);
			pLeaveMessage->Create(IDD_DIALOG_LeaveMessage,AfxGetMainWnd());
			
			//dlg.SetWindowPos();
			CRect rect;
			GetClientRect(&rect);
			pLeaveMessage->SetWindowPos(NULL,rect.Width()-170,rect.Height()-100,170,100,NULL);
			pLeaveMessage->ShowWindow(SW_SHOW);
			
		}
		count_old = count; 	
		user_old = theApp.m_Cpp_User;

	}

	if(nIDEvent == 8889){ //冰冻预约
		long count;
		try
		{
			SAString cmdstr = "select count(*) from "+theApp.TABLE_BOOKING+" where state='1' and bdjg is null";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Execute();
			while(g_dbcommand.FetchNext())
			{
				count = g_dbcommand.Field(1).asLong();
			}
			g_dbconnection.Commit();
		} catch (SAException &e)
		{
			AfxMessageBox((const char*)e.ErrText());
		}

		if(user_old.CompareNoCase(theApp.m_Cpp_User) != 0)
		{
			count_bd_old = 0;
		} 

		if(count > 0 && (count > count_bd_old))
		{
			pBookingTip = new CBookingTip;
			pBookingTip->count.Format("%d",count);
			pBookingTip->Create(IDD_DIALOG_BookingTip,AfxGetMainWnd());
			//dlg.SetWindowPos();
			CRect rect;
			GetClientRect(&rect);
			pBookingTip->SetWindowPos(NULL,rect.Width()-170,rect.Height()-100,170,100,NULL);
			pBookingTip->ShowWindow(SW_SHOW);
		}
		count_bd_old = count; 	
		user_old = theApp.m_Cpp_User;
	}

	if(nIDEvent == 9000) //医嘱tip
	{
		long count;
		SAString cmdstr;
		try
		{
			if(theApp.m_StationType.CompareNoCase("制片工作站") == 0)
			{
				cmdstr = "select count(*) from "+ theApp.TABLE_ADVICE + " where yzzt = 0";
			}
			else if(theApp.m_StationType.CompareNoCase("常规切片工作站") == 0)
			{
				if(theApp.m_CantainTsrs.CompareNoCase("是") == 0)
				{
					cmdstr = "select count(*) from " + theApp.TABLE_ADVICE + " where yzzt = 0 and yzlx <> '免疫组化' and yzlx <> '分子病理' and yzlx <> '电镜' and yzlx <> '其它' and yzlx <> '补取'";
				}
				else
				{
					cmdstr = "select count(*) from " + theApp.TABLE_ADVICE + " where yzzt = 0 and yzlx <> '免疫组化' and yzlx <> '特殊染色' and yzlx <> '分子病理' and yzlx <> '电镜' and yzlx <> '其它' and yzlx <> '补取'";
				}				
			}
			else if(theApp.m_StationType.CompareNoCase("特检工作站") == 0)
			{
				if(theApp.m_CantainTsrs.CompareNoCase("否") == 0)
				{
					cmdstr = "select count(*) from " + theApp.TABLE_ADVICE + " where yzzt = 0 and (yzlx = '免疫组化' or yzlx = '特殊染色' or yzlx = '分子病理' or yzlx = '电镜' or yzlx = '其它')";
				}
				else
				{
					cmdstr = "select count(*) from " + theApp.TABLE_ADVICE + " where yzzt = 0 and (yzlx = '免疫组化' or yzlx = '分子病理' or yzlx = '电镜' or yzlx = '其它')";
				}
				
			}
			
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Execute();
			while(g_dbcommand.FetchNext())
			{
				count = g_dbcommand.Field(1).asLong();
			}
			g_dbconnection.Commit();
		}
		catch (SAException &e)
		{
			AfxMessageBox((const char*)e.ErrText());
		}
		
		if(count == 0)
		{
			return;
		}
		
		long countTg = 0;
		if(theApp.m_StationType.CompareNoCase("常规切片工作站") == 0)
		{
			try
			{
				SAString cmdstr = "select count(*) from " + theApp.TABLE_MATERIAL + " where zpzt <= 1 and rwly = '脱钙'";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Execute();
				while(g_dbcommand.FetchNext())
				{
					countTg = g_dbcommand.Field(1).asLong();
				}
				g_dbconnection.Commit();
			}
			catch (SAException& e)
			{
				AfxMessageBox((const char*)e.ErrText());
			}
		}	

		pAdviceTip = new CAdviceTip;
		pAdviceTip->count.Format("%d",count);
		pAdviceTip->countTG.Format("%d",countTg);
		pAdviceTip->Create(IDD_DIALOG_AdviceTip,AfxGetMainWnd());
		CRect rect;
		GetClientRect(&rect);
		if(theApp.m_StationType.CompareNoCase("常规切片工作站") == 0)
		{
			pAdviceTip->SetWindowPos(NULL,rect.Width()-170,rect.Height()-120,170,120,NULL);
		}
		else
		{
			pAdviceTip->SetWindowPos(NULL,rect.Width()-170,rect.Height()-100,170,100,NULL);
		}
		pAdviceTip->ShowWindow(SW_SHOW);
	}

	if(nIDEvent == m_Close_TimerHandle)
	{
		COleDateTime cur = COleDateTime::GetCurrentTime();
		COleDateTimeSpan span = cur - m_LastTime;
		int m_WaitTime = atoi(theApp.m_Waiting);
		if(span.GetTotalMinutes() >= m_WaitTime && !theApp.IsLogOut)
		{
			PostMessage(WM_COMMAND, ID_APP_LOGOUT);
		}
	}
	
	if(m_realtime && nIDEvent == m_TimerHandle && !mute )
	{
		mute = true;
		//=======================================================
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

		if( pImageView && m_VideoCap.GetSafeHwnd())
		{
			HBITMAP bitmaphandle = (HBITMAP)m_VideoCap.SnapShot2HBITMAP();

			CDC * pDC = pImageView->GetDC();
	
			CDC MemDC;
			MemDC.CreateCompatibleDC(pDC);
			HBITMAP OldBitmap=(HBITMAP)MemDC.SelectObject(bitmaphandle);
			BITMAP  bmp; 
			GetObject(bitmaphandle, sizeof(BITMAP), (LPSTR)&bmp);

			extern CRect SizeRectWithConstantAspectRatio(CRect* rcScreen,  CSize  sizePicture,  BOOL   bCenter);
			CSize  sizePicture;
			sizePicture.cx = bmp.bmWidth;
			sizePicture.cy = bmp.bmHeight;
			CRect rcScreen;
			pImageView->GetClientRect(&rcScreen);
			
			if( rcScreen.Width() < bmp.bmWidth || rcScreen.Height() < bmp.bmHeight )
			{
				rcScreen = SizeRectWithConstantAspectRatio(&rcScreen, sizePicture, TRUE);
			}
			else
			{
				int w = rcScreen.Width();
				int h = rcScreen.Height();
				rcScreen.left   = ( w - bmp.bmWidth ) / 2;
				rcScreen.right  = rcScreen.left + bmp.bmWidth;
				rcScreen.top    = ( h - bmp.bmHeight) / 2;
				rcScreen.bottom = rcScreen.top  + bmp.bmHeight;
			}
			
			int OldStretchMode = pDC->SetStretchBltMode(COLORONCOLOR);
			pDC->StretchBlt(rcScreen.left,rcScreen.top,rcScreen.Width(),rcScreen.Height(),&MemDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			pDC->SetStretchBltMode(OldStretchMode);
			MemDC.SelectObject(OldBitmap);

			if(bitmaphandle) ::DeleteObject(bitmaphandle);

			MemDC.DeleteDC();
			pImageView->ReleaseDC(pDC);
		}

		if( pImageView && m_pFrameGrabber != NULL && m_pFrameGrabber->GetSafeHwnd())
		{
			CSize imgsize = m_pFrameGrabber->GetImageSize();
			if(imgsize.cx > 0 && imgsize.cy > 0)
			{
				HDIB hDIB = ::CreateDIBFile(imgsize.cx,imgsize.cy,24);
				LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
				BYTE *   lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
				int LineByteWidth = WIDTHBYTES( imgsize.cx * 24 );
				
				
				LPBITMAPINFO lpBi = m_pFrameGrabber->GetDIB();
				memcpy(lpDIBBits, (BYTE *)::FindDIBBits((char *)lpBi), LineByteWidth * imgsize.cy);
				
				::GlobalUnlock((HGLOBAL) hDIB);
				
				extern CRect SizeRectWithConstantAspectRatio(CRect* rcScreen,  CSize  sizePicture,  BOOL   bCenter);
				CSize  sizePicture;
				sizePicture.cx = imgsize.cx;
				sizePicture.cy = imgsize.cy;
				CRect rcScreen;
				pImageView->GetClientRect(&rcScreen);
				
				if( rcScreen.Width() < imgsize.cx || rcScreen.Height() < imgsize.cy )
				{
					rcScreen = SizeRectWithConstantAspectRatio(&rcScreen, sizePicture, TRUE);
				}
				else
				{
					int w = rcScreen.Width();
					int h = rcScreen.Height();
					rcScreen.left   = ( w - imgsize.cx ) / 2;
					rcScreen.right  = rcScreen.left + imgsize.cx;
					rcScreen.top    = ( h - imgsize.cy) / 2;
					rcScreen.bottom = rcScreen.top  + imgsize.cy;
				}
				
				CDC * pDC = pImageView->GetDC();
				::PaintDIB(pDC->m_hDC, &rcScreen, hDIB, &CRect(0,0,imgsize.cx,imgsize.cy), NULL);
				pImageView->ReleaseDC(pDC);
				
				::GlobalFree((HGLOBAL) hDIB);
			}
		}
		//=======================================================
		mute = false;
	}

	CXTFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnImgCaptureFormat() 
{
	if(theApp.m_CaptureMode.CompareNoCase("微软DSHOW设备") == 0)
	{
		DSHOW_Capture_Setting(this->m_hWnd);
	}

	if(theApp.m_CaptureMode.CompareNoCase("VFW采集设备支持") == 0 && m_pFrameGrabber != NULL)
	{
		m_pFrameGrabber->VideoFormatDialog();

		if(m_realtime)
		{
			PostMessage(WM_VFWCAPTURE);
			PostMessage(WM_VFWCAPTURE);
		}
	}

	if(theApp.m_CaptureMode.CompareNoCase("DirectShow采集设备支持") == 0 && m_VideoCap.GetSafeHwnd())
	{
		TerminateTimer();

		CDShowSetup dlg(this);
		dlg.m_device  = m_DeviceIndex;
		dlg.m_format  = m_VideoFormatIndex;
		dlg.m_input   = m_VideoInputIndex;
		if(dlg.DoModal() == IDOK)
		{
			m_DeviceIndex        = dlg.m_device;
			m_VideoFormatIndex   = dlg.m_format;
			m_VideoInputIndex    = dlg.m_input;

			CIniReader m_IniReader;
			m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");
			CString str;
			str.Format("%d", m_DeviceIndex);
			m_IniReader.setKey(str,"Device","DirectShow采集设备支持");
			str.Format("%d", m_VideoFormatIndex);
			m_IniReader.setKey(str,"VideoFormat","DirectShow采集设备支持");
			str.Format("%d", m_VideoInputIndex);
			m_IniReader.setKey(str,"VideoInput","DirectShow采集设备支持");
		}
	}
}

void CMainFrame::OnViewOutput() 
{
	// TODO: Add your command handler code here
	ShowControlBar(&m_wndOutputBar, !m_wndOutputBar.IsWindowVisible(), FALSE);

	if(m_wndOutputBar.IsWindowVisible())
	{
		CImageView *pView = NULL;
		if(theApp.GetView()->m_ImageDockWnd.GetSafeHwnd())
		{
			CWnd *pWnd = theApp.GetView()->m_ImageDockWnd.GetWindow(GW_CHILD);
			while( pWnd != NULL )
			{
				if( pWnd->IsKindOf(RUNTIME_CLASS(CImageView)) )
				{
					pView = (CImageView *)pWnd;
					break;
				}
				pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
			}
		}

		if( pView )
		{
			int n_count = pView->GetCurrentModel()->GetFormObjects()->GetCount();
			m_wndOutputBar.m_MeasureDialog.m_MeasureCtrl.SetNumberRows(n_count);
			m_wndOutputBar.m_MeasureDialog.m_MeasureCtrl.RedrawAll();
		}
	}
}

void CMainFrame::OnUpdateViewOutput(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_wndOutputBar.IsWindowVisible());
}

void CMainFrame::OnSelchangeComboRulerMark() 
{
	if(m_wndOutputBar.IsWindowVisible())
	{
		CImageView *pView = NULL;
		if(theApp.GetView()->m_ImageDockWnd.GetSafeHwnd())
		{
			CWnd *pWnd = theApp.GetView()->m_ImageDockWnd.GetWindow(GW_CHILD);
			while( pWnd != NULL )
			{
				if( pWnd->IsKindOf(RUNTIME_CLASS(CImageView)) )
				{
					pView = (CImageView *)pWnd;
					break;
				}
				pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
			}
		}
		if( pView )
		{
			int n_count = pView->GetCurrentModel()->GetFormObjects()->GetCount();
			m_wndOutputBar.m_MeasureDialog.m_MeasureCtrl.SetNumberRows(n_count);
			m_wndOutputBar.m_MeasureDialog.m_MeasureCtrl.RedrawAll();
		}
	}
}

void CMainFrame::OnStandardView() 
{
	// TODO: Add your command handler code here
	CDbfStandard  mydlg(this);
	mydlg.DoModal();
}

void CMainFrame::OnStandardAdd() 
{
	// TODO: Add your command handler code here
	CImageView *pView = NULL;
	if(theApp.GetView()->m_ImageDockWnd.GetSafeHwnd())
	{
		CWnd *pWnd = theApp.GetView()->m_ImageDockWnd.GetWindow(GW_CHILD);
		while( pWnd != NULL )
		{
			if( pWnd->IsKindOf(RUNTIME_CLASS(CImageView)) )
			{
				pView = (CImageView *)pWnd;
				break;
			}
			pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
		}
	}

	if(pView == NULL) return;
	
	CFODrawShape *pShape = NULL;
	POSITION pos = pView->GetSelectShapes()->GetHeadPosition();
	if(pos != NULL)	pShape = (CFODrawShape *)pView->GetSelectShapes()->GetNext(pos);
	if(!(pShape != NULL && (pShape->GetType() == FO_COMP_LINE || pShape->GetType() == FO_COMP_ARROWLINE || pShape->GetType() == FO_COMP_CROSSLINE)))
		return;

    CStandInput mydlg(this);

	float n_Lengh = 0.0, startx,starty,endx,endy;
	int spotcount = pShape->GetSpotCount();
	LPPOINT pt = pShape->GetPoints();
	for(int i = 0; i < ( spotcount - 1); i++)
	{
		startx = pt[i].x;   	starty = pt[i].y;
		endx   = pt[i+1].x;		endy   = pt[i+1].y;
		n_Lengh = (float)(n_Lengh + sqrt((endx - startx) * (endx - startx) + (endy - starty) * (endy - starty)));
	}
	
	mydlg.m_PixelLength = n_Lengh;
	mydlg.DoModal();
}

void CMainFrame::OnUpdateDocManager(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnDocManager() 
{
	if( !RIGHT_DAGL(theApp.m_Cpp_Right) )
	{
		AfxMessageBox("当前用户没有档案管理的权限!");
		return;
	}

	CString  runstr;
	runstr = theApp.m_Exe_Working_Dir + "QSR.exe";
	CString  args;
	args.Empty();
	//-----------------------------
	launchViaCreateProcess(runstr, args);
}


void CMainFrame::OnFileRptmodify() 
{
    if(!RIGHT_BJBGGS(theApp.m_Cpp_Right))
	{
		AfxMessageBox("当前用户没有编辑报告格式的权限！");
		return;
	}

	CString Rptname; 
	
	Rptname.Empty();
	
	//--------------------------------------------------
	if(Rptname.IsEmpty())
	{
		CWnd *pWnd = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedDiagnose));
	
		if(pWnd != NULL)  ((CMedDiagnose *)pWnd)->GetDlgItemText(IDC_COMBO_BGGS, Rptname);

		Rptname.TrimLeft(); 
		Rptname.TrimRight();

		if (Rptname.CompareNoCase("请选择报告格式") == 0)  Rptname.Empty();
	}

	if(Rptname.IsEmpty())
	{
		CWnd *pWnd = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedTbs));
		if(pWnd != NULL)  ((CMedTbs *)pWnd)->GetDlgItemText(IDC_COMBO_BGGS, Rptname);

		Rptname.TrimLeft(); Rptname.TrimRight();
		if(Rptname.CompareNoCase("请选择报告格式") == 0)  Rptname.Empty();
	}
	
	if (Rptname.IsEmpty())
	{
		AfxMessageBox("请选择报告格式");
		return;
	}
	//--------------------------------------------------

	Rptname = theApp.m_SysTmpPath + Rptname + ".rpt";

	CString  runstr;
	runstr = theApp.m_Exe_Working_Dir + "ReportBuilder.exe";
	CString  args;
	args.Format(" %s", theApp.TABLE_MAIN);
	args =  " " + Rptname + args;
	launchViaCreateProcess(runstr, args);
}

void CMainFrame::OnAppGetcode() 
{
	CGetCode mydlg(this);
	mydlg.DoModal();
}

CString CMainFrame::RandomCode()
{
	COleDateTime curt = theApp.GetOleServerDateTime();

	return MD5String(curt.Format("%Y-%m-%d").GetBuffer(0));
}

void CMainFrame::OnUpdateAppGetcode(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(RIGHT_HQSJMM(theApp.m_Cpp_Right));
}

//根据卡的其它参数设置曝光时间
//其它的参数如摄像机时钟频率，消隐值都取默认值，
//参数：
//nWindWidth:当前图像宽度
//lTintUpper:曝光时间的分子, lTintUpper/lTintLower 组成实际的曝光时间
//lTintLower:曝光时间的分母，lTintUpper/lTintLower 组成实际的曝光时间
void CMainFrame::SetExposureTime(int nWindWidth,long lTintUpper,long lTintLower)
{
	int size = sizeof(HVTYPE);
	HVTYPE type;
	HVGetDeviceInfo(m_hhv,DESC_DEVICE_TYPE, &type, &size);	
	
	//When outputwindow changes, change the exposure 
	//请参考曝光系数转换公式
	long lClockFreq = (m_SnapSpeed == HIGH_SPEED)? 24000000:12000000; 
	int nOutputWid = nWindWidth;
	double dExposure = 0.0;
	double dTint = max((double)lTintUpper/(double)lTintLower,MY_ZERO);
	if(type == HV1300UCTYPE || type == HV1301UCTYPE)	
	{
		if(type == HV1300UCTYPE) 
			lClockFreq = 24000000;   //HV1300UCTYPE 暂不支持更改摄像机时钟频率
		long lTb = m_nHBlanking;
		lTb -= 10;	
		if(lTb <= 0) lTb =0;
		dExposure = (dTint* lClockFreq + 180.0)/((double)nOutputWid + 244.0 + lTb);
	}
	else
	{
		if(type == HV2000UCTYPE) 
			lClockFreq = 24000000;   //HV1300UCTYPE 暂不支持更改摄像机时钟频率	
		
		long lTb = m_nHBlanking;
		lTb += 36;
		if(lTb <= 0) lTb =0;
		dExposure = (dTint* lClockFreq + 180.0)/((double)nOutputWid + 305.0 + lTb) + 1;
	}
	
	if (dExposure > 16383) 
		dExposure = 16383;
	HVAECControl(m_hhv, AEC_EXPOSURE_TIME, (long)dExposure);
	
}

void CMainFrame::InitParamFromINI()
{
	int n = 0;

	m_Resolution	= (HV_RESOLUTION)GetPrivateProfileInt("Video Parameter", "Resolution", RES_MODE0, "HVViewer.ini" );

	m_SnapMode	= (HV_SNAP_MODE)GetPrivateProfileInt("Video Parameter", "SnapMode", CONTINUATION, "HVViewer.ini" );
	
	m_ImageMode		= (HV_IMAGE_MODE)GetPrivateProfileInt("Video Parameter", "ImageMode", HV_COLOR, "HVViewer.ini" );
	
	m_Layout	= (HV_BAYER_LAYOUT)GetPrivateProfileInt("Video Parameter", "BayerLayout", BAYER_GR, "HVViewer.ini" );

	m_lTintUpper	= (long)GetPrivateProfileInt("Video Parameter", "TintUpper", 60, "HVViewer.ini" );
	m_lTintLower	= (long)GetPrivateProfileInt("Video Parameter", "TintLower", 1000, "HVViewer.ini" );

	m_lADCLevel	= (long)GetPrivateProfileInt("Video Parameter", "ADCLevel", ADC_LEVEL1, "HVViewer.ini" );
	
	m_lGain[0]	= (long)GetPrivateProfileInt("Video Parameter", "GainRCh", 8, "HVViewer.ini" );
	m_lGain[1]	= (long)GetPrivateProfileInt("Video Parameter", "GainRChG", 8, "HVViewer.ini" );
	m_lGain[2]	= (long)GetPrivateProfileInt("Video Parameter", "GainBChG", 8, "HVViewer.ini" );
	m_lGain[3]	= (long)GetPrivateProfileInt("Video Parameter", "GainBCh", 8, "HVViewer.ini" );
	
	n = GetPrivateProfileInt("Video Parameter", "RatioRL", 0, "HVViewer.ini" );
	CopyMemory(&m_dRatioR, &n, sizeof(int));
	n = GetPrivateProfileInt("Video Parameter", "RatioRH", 0, "HVViewer.ini" );
	CopyMemory((char *)&m_dRatioR + sizeof(int), &n, sizeof(int));

	n = GetPrivateProfileInt("Video Parameter", "RatioGL", 0, "HVViewer.ini" );
	CopyMemory(&m_dRatioG, &n, sizeof(int));
	n = GetPrivateProfileInt("Video Parameter", "RatioGH", 0, "HVViewer.ini" );
	CopyMemory((char *)&m_dRatioG + sizeof(int), &n, sizeof(int));

	n = GetPrivateProfileInt("Video Parameter", "RatioBL", 0, "HVViewer.ini" );
	CopyMemory(&m_dRatioB, &n, sizeof(int));
	n = GetPrivateProfileInt("Video Parameter", "RatioBH", 0, "HVViewer.ini" );
	CopyMemory((char *)&m_dRatioB + sizeof(int), &n, sizeof(int));

	m_nHBlanking	= (long)GetPrivateProfileInt("Video Parameter", "HBlanking", 0, "HVViewer.ini" );
	m_nVBlanking	= (long)GetPrivateProfileInt("Video Parameter", "VBlanking", 0, "HVViewer.ini" );
	m_SnapSpeed =  (HV_SNAP_SPEED )GetPrivateProfileInt("Video Parameter", "SnapSpeed", HIGH_SPEED, "HVViewer.ini" );
		
}

void CMainFrame::SaveParamToINI()
{
	CString strValue;

	strValue.Format("%d", m_Resolution);
	WritePrivateProfileString("Video Parameter", "Resolution", strValue, "HVViewer.ini");

	strValue.Format("%d", m_SnapMode);
	WritePrivateProfileString("Video Parameter", "SnapMode", strValue, "HVViewer.ini");

	strValue.Format("%d", m_ImageMode);
	WritePrivateProfileString("Video Parameter", "ImageMode", strValue, "HVViewer.ini");
	strValue.Format("%d", m_Layout);
	WritePrivateProfileString("Video Parameter", "BayerLayout", strValue, "HVViewer.ini");
	
	strValue.Format("%d", m_lTintLower);
	WritePrivateProfileString("Video Parameter", "TintLower", strValue, "HVViewer.ini");

	strValue.Format("%d", m_lTintUpper);
	WritePrivateProfileString("Video Parameter", "TintUpper", strValue, "HVViewer.ini");

	strValue.Format("%d", m_lADCLevel);
	WritePrivateProfileString("Video Parameter", "ADCLevel", strValue, "HVViewer.ini");

	strValue.Format("%d", m_lGain[0]);
	WritePrivateProfileString("Video Parameter", "GainRCh", strValue, "HVViewer.ini");
	strValue.Format("%d", m_lGain[1]);
	WritePrivateProfileString("Video Parameter", "GainRChG", strValue, "HVViewer.ini");
	strValue.Format("%d", m_lGain[2]);
	WritePrivateProfileString("Video Parameter", "GainBChG", strValue, "HVViewer.ini");
	strValue.Format("%d", m_lGain[3]);
	WritePrivateProfileString("Video Parameter", "GainBCh", strValue, "HVViewer.ini");

	strValue.Format("%d", *((int *)&m_dRatioR));
	WritePrivateProfileString("Video Parameter", "RatioRL", strValue, "HVViewer.ini");
	strValue.Format("%d", *((int *)&m_dRatioR + 1));
	WritePrivateProfileString("Video Parameter", "RatioRH", strValue, "HVViewer.ini");

	strValue.Format("%d", *((int *)&m_dRatioG));
	WritePrivateProfileString("Video Parameter", "RatioGL", strValue, "HVViewer.ini");
	strValue.Format("%d", *((int *)&m_dRatioG + 1));
	WritePrivateProfileString("Video Parameter", "RatioGH", strValue, "HVViewer.ini");

	strValue.Format("%d", *((int *)&m_dRatioB));
	WritePrivateProfileString("Video Parameter", "RatioBL", strValue, "HVViewer.ini");
	strValue.Format("%d", *((int *)&m_dRatioB + 1));
	WritePrivateProfileString("Video Parameter", "RatioBH", strValue, "HVViewer.ini");

	strValue.Format("%d", m_nHBlanking);
	WritePrivateProfileString("Video Parameter", "HBlanking", strValue, "HVViewer.ini");
	strValue.Format("%d", m_nVBlanking);
	WritePrivateProfileString("Video Parameter", "VBlanking", strValue, "HVViewer.ini");

	strValue.Format("%d", m_SnapSpeed);
	WritePrivateProfileString("Video Parameter", "SnapSpeed", strValue, "HVViewer.ini");	
	
}

void CMainFrame::InitHVDevice()
{
	if(m_hhv == NULL) return;

	//get the max size of the output window
	int nBuffSize = 0;
	HVSTATUS status = HVGetDeviceInfo(m_hhv, DESC_RESOLUTION, NULL,&nBuffSize);	
	if( !HV_SUCCESS(status))
		return;

	BYTE *pbContext = new BYTE[nBuffSize];
	DWORD *pdContext = (DWORD *)(pbContext);
	status = HVGetDeviceInfo(m_hhv, DESC_RESOLUTION, pdContext,&nBuffSize);	

	nMaxWidMd0 = *(pdContext + 2*m_Resolution);
	nMaxHeiMd0 = *(pdContext + 2*m_Resolution +1);
	delete []pbContext;

	int size = sizeof(HVTYPE);
	HVTYPE type;
	HVGetDeviceInfo(m_hhv,DESC_DEVICE_TYPE, &type, &size);	
	if(type == HV2001UCTYPE || type == HV1301UCTYPE)
	{
		m_bIsSnapSpeedSprted = TRUE;   //本摄像机是否支持设置采集速度
		m_bIsBlkSizeSprted = TRUE;   //本摄像机是否支持设置取得消隐取值范围	
	}

	
	HVSetResolution(m_hhv, m_Resolution);
	HVSetSnapMode(m_hhv, m_SnapMode);
	HVSetOutputWindow(m_hhv, 0, 0, nMaxWidMd0, nMaxHeiMd0);
	if(m_bIsSnapSpeedSprted)
		HVSetSnapSpeed(m_hhv,m_SnapSpeed);

	HVSetBlanking(m_hhv,m_nHBlanking,m_nVBlanking);
	
	for (int i = 0; i < 4; i++){
		HVAGCControl(m_hhv, RED_CHANNEL + i, m_lGain[i]);
	}

	//设置曝光时间
	SetExposureTime(nMaxWidMd0,m_lTintUpper,m_lTintLower);

	HVADCControl(m_hhv, ADC_BITS, m_lADCLevel);

	//初始化查找表
	SetLutTable(m_dRatioR,m_dRatioG,m_dRatioB);	

	
	//allocate the memeor accord to the resolution fo RES_MODE0
	if (m_pImageBuffer) {
		delete []m_pImageBuffer;
	}
	if (m_pRawBuffer) {
		delete []m_pRawBuffer;
	}

	m_pImageBuffer = new BYTE[nMaxWidMd0 * nMaxHeiMd0 * 3];

	if (m_pImageBuffer) {
		FillMemory(m_pImageBuffer, nMaxWidMd0 * nMaxHeiMd0  * 3, 0xff);
	}
	
	m_pRawBuffer = new BYTE[nMaxWidMd0 * nMaxHeiMd0 ];
	if (m_pRawBuffer) {
		FillMemory(m_pRawBuffer, nMaxWidMd0 * nMaxHeiMd0 , 0xff);
	}
}

BOOL CMainFrame::SetLutTable(double dRatioR,double dRatioG,double dRatioB)
{
	if(dRatioR <=0)
		return FALSE;
	if(dRatioG <=0)
		return FALSE;
	if(dRatioB <=0)
		return FALSE;

	for(int i=0;i<256;i++)
	{
		if(m_bIsGammaCorrect)
		{
			m_pLutR[i] = min((int)(m_pLutGamma[i]*dRatioR),255);
			m_pLutG[i] = min((int)(m_pLutGamma[i]*dRatioG),255);
			m_pLutB[i] = min((int)(m_pLutGamma[i]*dRatioB),255);

		}
		else
		{
			m_pLutR[i] = min((int)(i*dRatioR),255);
			m_pLutG[i] = min((int)(i*dRatioG),255);
			m_pLutB[i] = min((int)(i*dRatioB),255);
		}
	}

	return TRUE;
}

int CALLBACK CMainFrame::SnapCallback(HV_SNAP_INFO *pInfo)
{
	CMainFrame *pFrame = (CMainFrame *)(pInfo->pParam);
	ASSERT(pFrame);


	pFrame->SendMessage(WM_CHANGE_SNAP, 0, 0);
		
	return 1;
}

LRESULT CMainFrame::OnSnapMessage(WPARAM wParam, LPARAM lParam)
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
		if( m_DigitalOut_NeedUpdateWnd )
		{
			m_DigitalOut_NeedUpdateWnd = false;

			CDC * pDC = pImageView->GetDC();
			
			CRect ClientRect;
			pImageView->GetClientRect(&ClientRect);
			
			CPen   * n_old_pen   = (CPen   *)pDC->SelectStockObject(BLACK_PEN);
			CBrush * n_old_brush = (CBrush *)pDC->SelectStockObject(BLACK_BRUSH);
			
			pDC->Rectangle(&ClientRect);
			
			pDC->SelectObject(n_old_pen);
			pDC->SelectObject(n_old_brush);
			pImageView->ReleaseDC(pDC);
		}

		CSize  sizePicture;
		sizePicture.cx = nMaxWidMd0;
		sizePicture.cy = nMaxHeiMd0;
		CRect rcScreen;
		pImageView->GetClientRect(&rcScreen);

		if(rcScreen.Width() >= sizePicture.cx && rcScreen.Height() >= sizePicture.cy)
		{
			rcScreen.left = (rcScreen.Width()  - sizePicture.cx) / 2;
			rcScreen.top  = (rcScreen.Height() - sizePicture.cy) / 2;
			rcScreen.right  = rcScreen.left + sizePicture.cx;
			rcScreen.bottom = rcScreen.top  + sizePicture.cy;
		}
		else
		{
			CRect SizeRectWithConstantAspectRatio(CRect* rcScreen,  CSize  sizePicture,  BOOL   bCenter);
			rcScreen = SizeRectWithConstantAspectRatio(&rcScreen, sizePicture, TRUE);
		}

		CDC * pDC = pImageView->GetDC();

		ConvertBayer2Rgb(m_pImageBuffer, m_pRawBuffer,nMaxWidMd0,nMaxHeiMd0, m_ConversionType,m_pLutR,m_pLutG,m_pLutB,true,m_Layout);

		HDIB lDIB = ::CreateDIBFile( nMaxWidMd0, nMaxHeiMd0, 24 );
		LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) lDIB);
		BYTE *   lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
		memcpy( lpDIBBits, m_pImageBuffer, nMaxWidMd0 * nMaxHeiMd0 * 3 );
		::GlobalUnlock((HGLOBAL) lDIB);

		::PaintDIB(pDC->m_hDC, &rcScreen, lDIB, &CRect(0,0,nMaxWidMd0,nMaxHeiMd0), NULL);
		
		::GlobalFree((HGLOBAL)lDIB);
		pImageView->ReleaseDC(pDC);
	}

	return 1;
}

LRESULT CMainFrame::OnGetRatioMessage(WPARAM wParam, LPARAM lParam)
{
	if (m_pImageBuffer) {
		if (m_ImageMode	== HV_COLOR)
		{


			SetLutTable(1.0,1.0,1.0);

			CSize Size;
			Size.cx  = nMaxWidMd0;
			Size.cy  = nMaxHeiMd0;

			ConvertBayer2Rgb(m_pImageBuffer, m_pRawBuffer,Size.cx,Size.cy, m_ConversionType,m_pLutR,m_pLutG,m_pLutB,true,m_Layout);


			GetWhiteBalanceRatio(m_pImageBuffer, 
				(int)nMaxWidMd0, (int)nMaxHeiMd0, 
				&m_dRatioR, &m_dRatioG, &m_dRatioB);
			

		} 
		else {
				return 1;
		}

	} 
	
	return 1;
}

//设置行消隐和场消隐,
//因为这个时候会影响曝光时间,所以同时要设置曝光时间
void CMainFrame::SetBlanking(int nHBlanking,int nVBlanking)
{

	DWORD pBlankSize[4];
	int nBufSize = 0;
	int nHmin = -9;
	int nVmin = -9;
	int nHmax = 1930;
	int nVmax = 1930;		

	if(m_bIsBlkSizeSprted)
	{
		HVGetDeviceInfo(m_hhv,DESC_DEVICE_BLANKSIZE,NULL,&nBufSize);	
		HVGetDeviceInfo(m_hhv,DESC_DEVICE_BLANKSIZE,pBlankSize,&nBufSize);	
		
		//得到消隐的边界值
		nHmin = (int)pBlankSize[0];
		nVmin = (int)pBlankSize[1];
		nHmax = (int)pBlankSize[2];
		nVmax = (int)pBlankSize[3];	
	}

	nHBlanking = max(min(nHmax,nHBlanking),nHmin);
	nVBlanking = max(min(nVmax,nVBlanking),nVmin);

	m_nHBlanking = nHBlanking;
	m_nVBlanking = nVBlanking;

	HVSetBlanking(m_hhv,nHBlanking,nVBlanking);
	SetExposureTime(nMaxWidMd0,m_lTintUpper,m_lTintLower);	
}

void CMainFrame::OnDhSetup() 
{
	CAdjustDlg dlg;

	if (dlg.DoModal() == IDOK){
	}
	m_DigitalOut_NeedUpdateWnd = true;
}

void CMainFrame::OnDhWb() 
{
	CWBDlg dlg;

	if (dlg.DoModal() == IDOK) {
	} 
	m_DigitalOut_NeedUpdateWnd = true;
}

void CMainFrame::OnDhGamma() 
{
	m_bIsGammaCorrect = TRUE;	
	CDlgGamma1 dlg;
	dlg.m_dGammRatio = m_dGammRatio;
	if(dlg.DoModal() == IDOK)
	{
		m_dGammRatio = dlg.m_dGammRatio;
		//设置Gamma查值表的值
		SetGammaLut(m_pLutGamma,m_dGammRatio);
		//更新颜色查值表
		SetLutTable(m_dRatioR,m_dRatioG,m_dRatioB);	
	}	
	m_DigitalOut_NeedUpdateWnd = true;
}

void CMainFrame::OnDhCancelgamma() 
{
	if( m_bIsGammaCorrect )
	{
		m_bIsGammaCorrect = FALSE;		
		SetLutTable(m_dRatioR,m_dRatioG,m_dRatioB);	
	}
}

void CMainFrame::OnDhMode0() 
{
	BOOL rt = m_realtime;
	if (m_realtime)
	{
		SendMessage(WM_HVCAPTURE);
	}
	
	m_Resolution = RES_MODE0;
	
	InitHVDevice();
	if(rt)
	{
		SendMessage(WM_HVCAPTURE);
	}
	m_DigitalOut_NeedUpdateWnd = true;
}

void CMainFrame::OnUpdateDhMode0(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_Resolution == RES_MODE0);	
}

void CMainFrame::OnDhMode1() 
{
	BOOL rt = m_realtime;
	if (m_realtime)
	{
		SendMessage(WM_HVCAPTURE);
	}
	
	m_Resolution = RES_MODE1;
	
	InitHVDevice();
	if(rt)
	{
		SendMessage(WM_HVCAPTURE);
	}
	m_DigitalOut_NeedUpdateWnd = true;
}

void CMainFrame::OnUpdateDhMode1(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_Resolution == RES_MODE1);	
}

void CMainFrame::OnDhMode2() 
{
	BOOL rt = m_realtime;
	if (m_realtime)
	{
		SendMessage(WM_HVCAPTURE);
	}
	
	m_Resolution = RES_MODE2;
	
	InitHVDevice();
	if(rt)
	{
		SendMessage(WM_HVCAPTURE);
	}
	m_DigitalOut_NeedUpdateWnd = true;
}

void CMainFrame::OnUpdateDhMode2(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_Resolution == RES_MODE2);	
}

void CMainFrame::OnDhConversionGb() 
{
	m_Layout = BAYER_GB;
}

void CMainFrame::OnUpdateDhConversionGb(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_Layout == BAYER_GB);
}

void CMainFrame::OnDhConversionGr() 
{
	m_Layout = BAYER_GR;
}

void CMainFrame::OnUpdateDhConversionGr(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_Layout == BAYER_GR);
}

void CMainFrame::OnDhConversionBg() 
{
	m_Layout = BAYER_BG;
}

void CMainFrame::OnUpdateDhConversionBg(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_Layout == BAYER_BG);
}

void CMainFrame::OnDhConversionRg() 
{
	m_Layout = BAYER_RG;
}

void CMainFrame::OnUpdateDhConversionRg(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_Layout == BAYER_RG);
}

void CMainFrame::OnMaintanceSelffield() 
{
	CSelfDefine mydlg(this);
	mydlg.m_UsageType = 0;
	mydlg.DoModal();
}

void CMainFrame::OnUpdateMaintanceSelffield(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( RIGHT_XGXTSZ(theApp.m_Cpp_Right) );
}

void CMainFrame::OnMaintanceSelfshow() 
{
	CSelfDefine mydlg(this);
	mydlg.m_UsageType = 1;
	mydlg.DoModal();
}

void CMainFrame::OnUpdateMaintanceSelfshow(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( RIGHT_XGXTSZ(theApp.m_Cpp_Right) );
}

void CMainFrame::OnDocLogo() 
{
	CLogoShow mydlg(this);
	mydlg.DoModal();
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CXTFrameWnd::OnSize(nType, cx, cy);
	
	if(nType == SIZE_MINIMIZED)
	{
		if( m_realtime )
		{
			TerminateTimer();
		}
	}
}

void CMainFrame::OnActivateApp(BOOL bActive, HTASK hTask) 
{
	CXTFrameWnd::OnActivateApp(bActive, hTask);
	
	if(bActive == FALSE)
	{
		if( GetSafeHwnd() && m_realtime && theApp.m_CaptureMode.CompareNoCase("DirectShow采集设备支持"))
		{
			TerminateTimer();
		}
	}
}

LRESULT CMainFrame::DShowCapture(WPARAM wparam, LPARAM lparam)
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

	if(m_realtime)
	{
		if(m_TimerHandle)
		{
			KillTimer(m_TimerHandle);
			m_TimerHandle = 0;
		}

		m_VideoCap.Stop();
		
		m_realtime = false;
		
		if( pImageView != NULL)	pImageView->RedrawWindow();
	}
	else
	{
		//=======================================================
		if( pImageView )
		{
			CDC * pDC = pImageView->GetDC();
			
			CRect ClientRect;
			pImageView->GetClientRect(&ClientRect);
			
			CPen   * n_old_pen   = (CPen   *)pDC->SelectStockObject(NULL_PEN);
			CBrush * n_old_brush = (CBrush *)pDC->SelectStockObject(GRAY_BRUSH);
			
			pDC->Rectangle(&ClientRect);
			
			pDC->SelectObject(n_old_pen);
			pDC->SelectObject(n_old_brush);
			pImageView->ReleaseDC(pDC);
		}
		//=======================================================
		if( pImageView )
		{
			if(m_DeviceIndex != -1)
			{
				m_VideoCap.SetDevice(m_DeviceIndex);
			}

			if(m_VideoInputIndex != -1)
			{
				m_VideoCap.SetVideoInput(m_VideoInputIndex);
			}

			if(m_VideoFormatIndex != -1)
			{
				m_VideoCap.SetVideoFormat(m_VideoFormatIndex);
			}

			m_VideoCap.Start();
			m_TimerHandle = SetTimer(1, 100, NULL);
			m_realtime = true;
		}
		//=======================================================
	}

	return 0L;
}

void CMainFrame::OnFileDbsetup() 
{
	CDbType mydlg(this);
	mydlg.DoModal();
}

void CMainFrame::OnUpdateFileDbsetup(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(RIGHT_ZDYKBJ(theApp.m_Cpp_Right));
}

void CMainFrame::OnCommEvent()
{
	if(theApp.m_ComPort == 0)  return;

	int k = m_commctrl.GetCommEvent();
	switch(m_commctrl.GetCommEvent())
	{
	case 1: // vbMSCommEvSend:
		break;
	case 2: // vbMSCommEvReceive:
		{
			COleDateTime cur = COleDateTime::GetCurrentTime();
			COleDateTimeSpan span = cur - m_LastComPortTime;
			if(span.GetTotalSeconds() >= 1)
			{
				if(m_realtime)
				{
					SendMessage(WM_SWITCHON);
				}

				m_LastComPortTime = COleDateTime::GetCurrentTime();
			}
		}

		m_commctrl.GetInput();
		break;
	default:;
	}
}

void CMainFrame::DoEvents()
{
	MSG msg;
	
	// Process existing messages in the application's message queue.
	// When the queue is empty, do clean up and return.
	//	while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE) && !m_bCancel)
	while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
	{
		if (!AfxGetThread()->PumpMessage())
			return;
	}
}

#include "FalseNoQuery.h"

void CMainFrame::OnDocQuery() 
{
	CFalseNoQuery mydlg(this);
	mydlg.DoModal();
}

void CMainFrame::OnZpglCkyj() 
{
	CCkyj mydlg(this);
	mydlg.DoModal();
}

void CMainFrame::OnZpglYzdy() 
{
	CGzddy mydlg(this);
	mydlg.DoModal();
}

void CMainFrame::OnZpglQpyj() 
{
	CQpyj mydlg(this);
	mydlg.DoModal();
}

void CMainFrame::OnZpglZpqktj() 
{
	CZpqk mydlg(this);
	mydlg.DoModal();
}

void CMainFrame::OnWdmDevice() 
{
	if(theApp.m_CaptureMode.CompareNoCase("微软DSHOW设备") == 0)
	{
	    CDShowSetup dlg(this);

		CIniReader m_IniReader;
		m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");
		CString str;
		str = m_IniReader.getKeyValue("输入设备","微软DSHOW设备参数设置");
		str.TrimLeft();  str.TrimRight();
		if(!str.IsEmpty())  dlg.m_device = atoi(str);

		str = m_IniReader.getKeyValue("CurrentInputSetting","微软DSHOW设备参数设置");
		str.TrimLeft();  str.TrimRight();
		if(!str.IsEmpty())  dlg.m_input  = atoi(str);
	
		if(dlg.DoModal() == IDOK)
		{
			if(dlg.m_device >= 0)
			{
				str.Format("%d", dlg.m_device);
				m_IniReader.setKey(str,"输入设备","微软DSHOW设备参数设置");

				DSHOW_ChangeDevices(this->m_hWnd,dlg.m_device);
			}

			if(dlg.m_input >= 0)
			{
				str.Format("%d", dlg.m_input);
				m_IniReader.setKey(str,"CurrentInputSetting","微软DSHOW设备参数设置");
				
				DSHOW_ChangeInput(dlg.m_input);
			}
		}
	}
}

LRESULT CMainFrame::MVC3000Preview(WPARAM wparam, LPARAM lparam)
{
	static CRect rcScreen;
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

	if(m_hMVC3000 == NULL)
	{
		int nIndex;
		if(MV_Usb2Init("MVC1000", &nIndex, &m_CapInfo, &m_hMVC3000))
		{
			AfxMessageBox("Can not open MVC3000 USB camera! ");
			MV_Usb2Uninit(&m_hMVC3000);
			m_hMVC3000 = NULL;
		}
	}

	if(m_hMVC3000 == NULL || pImageView == NULL)  return 1;

	if(m_realtime)
	{
		MV_Usb2Stop(m_hMVC3000);

		m_realtime = false;
		if( pImageView != NULL)	pImageView->RedrawWindow();
	}
	else
	{
		//=======================================================
		if( pImageView )
		{
			CDC * pDC = pImageView->GetDC();
			
			CRect ClientRect;
			pImageView->GetClientRect(&ClientRect);
			
			CPen   * n_old_pen   = (CPen   *)pDC->SelectStockObject(NULL_PEN);
			CBrush * n_old_brush = (CBrush *)pDC->SelectStockObject(GRAY_BRUSH);
			
			pDC->Rectangle(&ClientRect);
			
			pDC->SelectObject(n_old_pen);
			pDC->SelectObject(n_old_brush);
			pImageView->ReleaseDC(pDC);
		}
		//=======================================================
		if( pImageView )
		{
			extern CRect SizeRectWithConstantAspectRatio(CRect* rcScreen,  CSize  sizePicture,  BOOL   bCenter);
			CSize  sizePicture;
			sizePicture.cx = m_CapInfo.Width;
			sizePicture.cy = m_CapInfo.Height;
			pImageView->GetClientRect(&rcScreen);
			
			if( rcScreen.Width() < sizePicture.cx || rcScreen.Height() < sizePicture.cy )
			{
				rcScreen = SizeRectWithConstantAspectRatio(&rcScreen, sizePicture, TRUE);
			}
			else
			{
				int w = rcScreen.Width();
				int h = rcScreen.Height();
				rcScreen.left   = ( w - sizePicture.cx ) / 2;
				rcScreen.right  = rcScreen.left + sizePicture.cx;
				rcScreen.top    = ( h - sizePicture.cy) / 2;
				rcScreen.bottom = rcScreen.top  + sizePicture.cy;
			}

			//打开视频预览
			MV_Usb2Start(m_hMVC3000,
				         "Microview Preview",	
				         WS_CHILD|WS_VISIBLE,
				         rcScreen.left,					
				         rcScreen.top,				
				         rcScreen.Width(),			
				         rcScreen.Height(),			
						 pImageView->GetSafeHwnd(),	
				         NULL,
				         THREAD_PRIORITY_NORMAL,
				         THREAD_PRIORITY_NORMAL
				         );			
			
			LONG gExposure;
			MV_Usb2AutoExposure(m_hMVC3000, m_AE, 180, NULL,&gExposure);

			m_realtime = true;
		}
		//=======================================================
	}

	return 0L;
}

void CMainFrame::GetSfExt(CString str_MedicalNo, int &lknum, int &cgnum, int &bdnum, int &tjnum)
{
	str_MedicalNo.TrimLeft();  str_MedicalNo.TrimRight();
	if(str_MedicalNo.IsEmpty())  return;

	//---------------------------------------------------
	// 蜡块数（cmipsmain_material表中当前记录总数）
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_MATERIAL + " where medicalno = '" + str_MedicalNo + "'";
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
		{
			lknum = g_dbcommand.Field(1).asLong();
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

	// 常规数（cmipsmain_material表中当前记录任务来源为常规和脱钙的记录数）
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_MATERIAL + " where medicalno = '" + str_MedicalNo + "' and (rwly = '常规' or rwly = '脱钙')";
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
		{
			cgnum = g_dbcommand.Field(1).asLong();
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

	// 冰冻数（cmipsmain_material表中当前记录任务来源为冰冻的记录数）
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_MATERIAL + " where medicalno = '" + str_MedicalNo + "' and rwly = '冰冻'";
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
		{
			bdnum = g_dbcommand.Field(1).asLong();
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

	// 特检数（cmipsmain_advice表中当前记录医嘱类型为免疫组化或分子病理或特殊染色或电镜的记录数）
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_ADVICE + " where medicalno = '" + str_MedicalNo + "' and (yzlx = '免疫组化' or yzlx = '特殊染色' or yzlx = '分子病理' or yzlx = '电镜')";
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
		{
			tjnum = g_dbcommand.Field(1).asLong();
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

void CMainFrame::ShowRecordInfo(CString str_MedicalNo)
{
	str_MedicalNo.TrimLeft();  str_MedicalNo.TrimRight();
	if(str_MedicalNo.IsEmpty())  return;

	int lknum = 0, cgnum = 0, bdnum = 0, tjnum = 0;
	GetSfExt(str_MedicalNo, lknum, cgnum, bdnum, tjnum);
	int bgzt;
	CString content; content.Empty();
	//-------------------------------------------------
	// output basic information
	content += "【病理号】: ";
	content += str_MedicalNo;
	char enter[3];
	enter[0] = 0x0d; enter[1]= 0x0a; enter[2] = 0x00;
	
	try
	{
		SAString cmdstr = "Select * from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = str_MedicalNo;
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
				if(FieldName.CompareNoCase("bgzt") == 0)
				{
					bgzt = g_dbcommand.Field(j).asLong();
				}
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
					content += ("【" + FieldName + "】: " + FieldStr);
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

	int numNotDo = 0,numNotPay = 0;
	CString yzlxstr; yzlxstr.Empty();
	if(theApp.m_StationType.CompareNoCase("特检工作站") == 0)
	{
		yzlxstr = " and yzlx in('免疫组化','特殊染色','分子病理','电镜')";
	}

	try
	{
		SAString cmdstr = "select count(*) from " + theApp.TABLE_ADVICE + " where yzzt = 0 and medicalno=:medicalno" + yzlxstr;
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = str_MedicalNo;
		g_dbcommand.Execute();
		while(g_dbcommand.FetchNext())
		{
			numNotDo = g_dbcommand.Field(1).asLong();
		}
		g_dbconnection.Commit();

		cmdstr = "select count(*) from " + theApp.TABLE_ADVICE + " where fxjg = '待收费' and medicalno=:medicalno" + yzlxstr;
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = str_MedicalNo;
		g_dbcommand.Execute();
		while(g_dbcommand.FetchNext())
		{
			numNotPay = g_dbcommand.Field(1).asLong();
		}
		g_dbconnection.Commit();
	}
	catch (SAException& e)
	{
		AfxMessageBox((const char*)e.ErrText());	
	}
	//-------------------------------------------------
	
	CTextOut mydlg(this);	
	mydlg.m_Title = "病例信息—【";
	CString s;
	if(lknum != 0)
	{
		s.Format("%d、",lknum);
		mydlg.m_Title += "蜡块";
		mydlg.m_Title += s;
	}
	
	if(cgnum != 0)
	{
		s.Format("%d、",lknum);
		mydlg.m_Title += "常规";
		mydlg.m_Title += s;
	}

	if(cgnum != 0)
	{
		s.Format("%d、",bdnum);
		mydlg.m_Title += "冰冻";
		mydlg.m_Title += s;
	}
	mydlg.m_Title.Format("%s特检%d(未执行%d、未收费%d)】", mydlg.m_Title, tjnum, numNotDo,numNotPay);
	mydlg.m_Title += " - [报告";
	if(bgzt < 1)
	{
		mydlg.m_Title += "已登记";
	}
	else if(bgzt < 2)
	{
		mydlg.m_Title += "已取材";
	}
	else if(bgzt < 4)
	{
		mydlg.m_Title += "已延期";
	}
	else if(bgzt < 8)
	{
		mydlg.m_Title += "诊断中";
	}
	else if(bgzt < 16)
	{
		mydlg.m_Title += "已审核";
	}
	else if(bgzt < 32)
	{
		mydlg.m_Title += "已打印";
	}
	else
	{
		mydlg.m_Title += "已发送";
	}

	mydlg.m_Title += "]"; 
	mydlg.m_Text = content;
	mydlg.DoModal();
}

void CMainFrame::OnImgOlympus() 
{
	CString  runstr;
	runstr = theApp.m_Exe_Working_Dir + "OlympusCam.exe";
	CString  args;  args.Empty();
	if(!theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		args.Format(" %s", theApp.GetView()->GetDocument()->m_blh);
	}
	//-----------------------------
	launchViaCreateProcess(runstr, args);
}

void CMainFrame::OnZpglSfqr() 
{
	CChargeConfirm mydlg(this);
	mydlg.DoModal();
}

void CMainFrame::OnZpglZCSfqr(){
	CZCQR mydlg(this);
	mydlg.DoModal();
}

void CMainFrame::OnZpglCgsfqr() 
{
	CNormalSfqr mydlg(this);
	mydlg.DoModal();
}

LRESULT CMainFrame::MYDSHOWPreview(WPARAM wparam, LPARAM lparam)
{
	static CRect rcScreen;
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

	// ================================================================
	int m_DShow_Device = 0;
	CIniReader m_IniReader;
	m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");
	CString str;
	str = m_IniReader.getKeyValue("输入设备","微软DSHOW设备参数设置");
	str.TrimLeft();  str.TrimRight();
	if(!str.IsEmpty())  m_DShow_Device = atoi(str);

	int m_DShow_PreviewWnd_Width  = 1024;
	int m_DShow_PreviewWnd_Height = 768;
	str = m_IniReader.getKeyValue("图像宽度","微软DSHOW设备参数设置");
	str.TrimLeft();  str.TrimRight();
	if(!str.IsEmpty())  m_DShow_PreviewWnd_Width  = atoi(str);
	str = m_IniReader.getKeyValue("图像高度","微软DSHOW设备参数设置");
	str.TrimLeft();  str.TrimRight();
	if(!str.IsEmpty())  m_DShow_PreviewWnd_Height = atoi(str);
	// ================================================================

	if(m_realtime)
	{
		DSHOW_ChangePreviewState(FALSE);
		DSHOW_ShowWindow(SW_HIDE);
		m_realtime = false;

		if( pImageView != NULL)	pImageView->RedrawWindow();
	}
	else
	{
		//=======================================================
		if( pImageView )
		{
			CDC * pDC = pImageView->GetDC();
			
			CRect ClientRect;
			pImageView->GetClientRect(&ClientRect);
			
			CPen   * n_old_pen   = (CPen   *)pDC->SelectStockObject(NULL_PEN);
			CBrush * n_old_brush = (CBrush *)pDC->SelectStockObject(GRAY_BRUSH);
			
			pDC->Rectangle(&ClientRect);
			
			pDC->SelectObject(n_old_pen);
			pDC->SelectObject(n_old_brush);
			pImageView->ReleaseDC(pDC);
		}
		//=======================================================
		if( pImageView )
		{
			extern CRect SizeRectWithConstantAspectRatio(CRect* rcScreen,  CSize  sizePicture,  BOOL   bCenter);
			CSize  sizePicture;
			sizePicture.cx = m_DShow_PreviewWnd_Width;
			sizePicture.cy = m_DShow_PreviewWnd_Height;
			pImageView->GetClientRect(&rcScreen);
			
			if( rcScreen.Width() < sizePicture.cx || rcScreen.Height() < sizePicture.cy )
			{
				rcScreen = SizeRectWithConstantAspectRatio(&rcScreen, sizePicture, TRUE);
			}
			else
			{
				int w = rcScreen.Width();
				int h = rcScreen.Height();
				rcScreen.left   = ( w - sizePicture.cx ) / 2;
				rcScreen.right  = rcScreen.left + sizePicture.cx;
				rcScreen.top    = ( h - sizePicture.cy) / 2;
				rcScreen.bottom = rcScreen.top  + sizePicture.cy;
			}
			
			pImageView->ClientToScreen(rcScreen);
			this->ScreenToClient(rcScreen);

			DSHOW_ShowWindow(SW_SHOW);
			DSHOW_ChangePreviewState(TRUE);

			::SetWindowPos(::GetDlgItem(this->m_hWnd,IDC_CAPTURE),NULL,rcScreen.left,rcScreen.top,rcScreen.Width(),rcScreen.Height(),SWP_NOZORDER);

			DSHOW_ChangeDevices(this->m_hWnd,m_DShow_Device);

			CWnd *pWnd = CWnd::FromHandle(::GetDlgItem(this->m_hWnd,IDC_CAPTURE));
			if(pWnd)
			{
				pWnd->BringWindowToTop();
			}
			
			pImageView->SetFocus();
			m_realtime = true;
		}
		//=======================================================
	}

	return 0L;
}



LRESULT CMainFrame::CanonS3S5Capture(WPARAM wParam, LPARAM lParam)
{
	if (m_pWndCanonS5Capture != NULL)
	{
		if (m_pWndCanonS5Capture->IsWindowVisible())
		{
			m_pWndCanonS5Capture->Disconnect();
		}
		else
		{
			m_pWndCanonS5Capture->Connect();
		}
	}


	return 0L;
}


LRESULT CMainFrame::OnDeviceChanged(WPARAM wParam, LPARAM lParam)
{
  long nStatus = wParam;
  long index   = lParam;

  CIniReader m_IniReader;
  m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");
  CString str;
  str.Format("%d", index);
  m_IniReader.setKey(str,"输入设备","微软DSHOW设备参数设置");
  str.Format("%d", nStatus);
  m_IniReader.setKey(str,"设备状态字","微软DSHOW设备参数设置");

  str.Empty();
  for(int i = 0; i < gVideoInputList.GetSize(); i++)
  {
	  str += gVideoInputList.GetAt(i);
	  str += ",";
  }
  m_IniReader.setKey(str,"VideoInputList","微软DSHOW设备参数设置");

  return 0L;
}

LRESULT CMainFrame::ResizeVideoWindow(WPARAM wParam, LPARAM lParam)
{
  ULONG cx = wParam;
  ULONG cy = lParam;
  CIniReader m_IniReader;
  m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");
  CString str;
  str.Format("%d", cx);
  m_IniReader.setKey(str,"图像宽度","微软DSHOW设备参数设置");
  str.Format("%d", cy);
  m_IniReader.setKey(str,"图像高度","微软DSHOW设备参数设置");
  return 0L;
}

LRESULT CMainFrame::OnCaptureBitmap(WPARAM wParam, LPARAM lParam)
{
	CString sTempPath,sImgFileName; 
	
	::GetTempPath(1024, sTempPath.GetBufferSetLength(1024)); 
	sTempPath.ReleaseBuffer(); 
	
	sImgFileName = sTempPath + "\\imgdshow.bmp";

	remove(sTempPath);

	// write out a BMP file
	//
	HANDLE hf = CreateFile(
		sImgFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL,
		  CREATE_ALWAYS, NULL, NULL );
  
	if ( hf == INVALID_HANDLE_VALUE )
		return 0;
  
	// write out the file header
	//
	BITMAPFILEHEADER bfh;
	memset( &bfh, 0, sizeof( bfh ) );
	bfh.bfType = 'MB';
	bfh.bfSize = sizeof( bfh ) + cb.lBufferSize + sizeof( BITMAPINFOHEADER );
	bfh.bfOffBits = sizeof( BITMAPINFOHEADER ) + sizeof( BITMAPFILEHEADER );
  
	DWORD dwWritten = 0;
	WriteFile( hf, &bfh, sizeof( bfh ), &dwWritten, NULL );
  
	dwWritten = 0;
	WriteFile( hf, &cb.bih, sizeof( cb.bih ), &dwWritten, NULL );
  
	// and the bits themselves
	//
	dwWritten = 0;
	WriteFile( hf, cb.pBuffer, cb.lBufferSize, &dwWritten, NULL );
	CloseHandle( hf );

	((CMainFrame*)AfxGetMainWnd())->AddNewImage(sImgFileName);
	remove(sImgFileName);

	((CMainFrame*)AfxGetMainWnd())->Sound_Play("camera.wav");
	return 0L;
}


#include "Reporting.h"

void CMainFrame::OnFileRptview() 
{
	CString m_InputBlh;  m_InputBlh.Empty();

	if(!theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		theApp.GetView()->ForceSave();
		m_InputBlh = theApp.GetView()->GetDocument()->m_blh;
		
		//PostMessage(WM_COMMAND,ID_FILE_NEW);
	}
	((CMainFrame*)theApp.GetMainWnd())->m_wndTabDockBar.SetActiveView(RUNTIME_CLASS(CDBList));

	CReporting ReportView(m_InputBlh);
}

void CMainFrame::OnMaintenancePicnote() 
{
	// TODO: Add your command handler code here
	ShellExecute(NULL,"open",theApp.m_Exe_Working_Dir + "SYSTEM\\picword.txt",NULL,NULL,SW_SHOW);
}


void CMainFrame::OnUpdateChangeMedicalTable(CCmdUI *pCmdUI)
{
	int nIdx, nNo;

	nIdx = pCmdUI->m_nID - (int) (ID_BLK_BEGIN);
	
	if (nIdx >= 0 && nIdx < theApp.m_DBName_list.GetSize())
	{
		nNo = theApp.m_DBNo_list.GetAt(nIdx);
		pCmdUI->SetCheck(nNo == theApp.m_DB_Type_No);
	}
}


void CMainFrame::OnChangeMedicalTable(UINT nID)
{
	int nIdx;
	
	nIdx = nID - (int) (ID_BLK_BEGIN);

	if (nIdx >= 0 && nIdx < theApp.m_DBName_list.GetSize())
	{
		theApp.m_DB_Type_Name = theApp.m_DBName_list.GetAt(nIdx);
 		theApp.m_DB_Type_No = theApp.m_DBNo_list.GetAt(nIdx);

 		theApp.ReflectDBChange(theApp.m_DB_Type_No,theApp.m_DB_Type_Name);
 	}
}



