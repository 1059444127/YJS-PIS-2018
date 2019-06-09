// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__1294D021_A6CE_4D17_905B_BE473EAEC302__INCLUDED_)
#define AFX_MAINFRM_H__1294D021_A6CE_4D17_905B_BE473EAEC302__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RecordSound.h"
#include "PlaySound.h"
#include "PlayMMSound.h"

#include "cSound.h"
#include "RecordTiming.h"
#include "TabDockWnd.h"
#include "FrameGrabber.h"

#include "CaptureCanonS5Dlg.h"

#include "OutputBar.h"

#include "CTwain.h"
#include "LeaveMessage.h"
#include "BookingTip.h"
#include "AdviceTip.h"


class CGDIThread;

typedef struct
{
	int    zoomlen;            // 放大镜倍数
	double value;              // 定标值
	char   unitname[255];      // 定标单位
} STANDARD_UNIT;

// ****    DH-HVDevice ****
#include "hvdailt.h"
#include "Raw2Rgb.h"
#define  MY_ZERO 0.000000001

//定义彩色图像还是黑白图像
typedef enum  tagHV_IMAGE_MODE
{
		HV_COLOR = 0,
		HV_BW = 1
}HV_IMAGE_MODE;
// ************************

// ** DirectShow采集设备支持 ****
#include "videocap.h"
#include "Devices.h"
#include "VideoFormats.h"
#include "VideoInputs.h"

#include "mscomm.h"
#define ID_COMMCTRL 2001  // Ctrl ID for comm control

// MVC3000采集卡
//----------------------------------------------
#include "mvcapi.h"
#define MVC3000

#ifdef MVC1000
const DWORD MAXWIDTH  = 1280;
const DWORD MAXHEIGHT = 1024;
#endif

#ifdef MVC2000
const DWORD MAXWIDTH  = 1600;
const DWORD MAXHEIGHT = 1200;
#endif

#ifdef MVC3000
const DWORD MAXWIDTH  = 2048;
const DWORD MAXHEIGHT = 1536;
#endif
//----------------------------------------------
// 微软DSHOW设备
//----------------------------------------------
#include "mydshow.h"
// this is the ID of the child control.  
#define IDC_CAPTURE 5000
//----------------------------------------------

class CMainFrame : public CXTFrameWnd, CTwain
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CXTFlatComboBox     m_wndComboRulerMark;
	CXTFlatComboBox     m_wndComboSelPrinter;
    CTabDockWnd         m_wndTabDockBar;
	COutputBar          m_wndOutputBar;

	void DoEvents();
	CMSComm   m_commctrl;
	CLeaveMessage*	 pLeaveMessage;
	CBookingTip*     pBookingTip;
	CAdviceTip*		 pAdviceTip;
	//留言
	long count_old;
	//冰冻预约
	long count_bd_old;

	CString user_old;
// Operations
public:
	CRecordSound*    m_pRecordSound;
	CPlaySound*      m_pPlaySound;
	CWriteSoundFile* m_pWriteSound;
	CPlayMMSound*    m_pPlayMMSound;
	

    void Sound_Startrecord(CString);
    void Sound_Startplay(CString);
	void Sound_StopPlaying();
	void Sound_Open();
	void Sound_Close();
 	void Sound_Play(CString);
    cSound  i_Sound;
	BOOL m_bRecording;

    CRecordTiming *pRecordTiming;

	bool m_frame_created;

    void AddNewImage (HDIB);
    void AddNewImage (CString);

	// MVC3000采集卡
	BYTE   *m_pRGBData;	//24bitRGB数据指针
	BYTE   *m_pRawData; //用于存放RawData数据
	HANDLE m_hMVC3000;
	struct CapInfoStruct m_CapInfo; //视频属性
	BOOL   m_AE;  // 自动曝光

	// DirectShow采集设备支持
	CVideoCap	m_VideoCap;
	int m_DeviceIndex, m_VideoInputIndex, m_VideoFormatIndex;

    //  TWAIN  Properties
	virtual void TwainError(TW_ERR e);
	virtual void DibReceived(HGLOBAL hdib);
	virtual int  NegotiateCapabilities(void);
	int         m_nRes;           // preferred resolution (dpi)
	int         m_nType;          // preferred pixel type (see PT_* in TWAIN.H)
	int         m_nXfer;          // xfer count

	// OK CARD Properties
    HANDLE	hBoard;					//okapi 采集卡句柄
	long    m_Ok_Unmatch;           //ok 加密标志
    CGDIThread *pExtThread;
	bool  m_realtime;
	HDIB  CaptureOneImage();
	void  TerminateTimer(bool BeFreshWnd = true);
	int InputWnd_Width,InputWnd_Height;

	// VFW Properties
	CFrameGrabber 	*m_pFrameGrabber;
	UINT  m_TimerHandle;

	//////////////////////////////////

	CCaptureCanonS5Dlg	*m_pWndCanonS5Capture;

	/////////////////////////////////

	// ****    DH-HVDevice ****
	HHV m_hhv;

	BYTE *m_pRawBuffer;
	BYTE *m_pImageBuffer;

	HV_RESOLUTION m_Resolution;
	HV_SNAP_MODE  m_SnapMode;
	long m_lGain[4];
	int m_lADCLevel;
	
	//White balance ratio
	double m_dRatioR;
	double m_dRatioG;
	double m_dRatioB;
	
	HV_IMAGE_MODE   m_ImageMode;
	HV_BAYER_LAYOUT m_Layout;
	
	//颜色查找表
	BYTE m_pLutR[256] ;
	BYTE m_pLutG[256] ;
	BYTE m_pLutB[256] ;

	BYTE m_pLutGamma[256] ;
	BOOL m_bIsGammaCorrect;
	//曝光时间的分子和分母
	long m_lTintUpper;
	long m_lTintLower;
	
	//the conversion method of raw to rgb
	HV_BAYER_CONVERT_TYPE m_ConversionType;
	//the variable to control the snap speed
	HV_SNAP_SPEED m_SnapSpeed;
	int m_nHBlanking;
	int m_nVBlanking;

	double m_dGammRatio;         //gamma 校正系数
	BOOL m_bIsSnapSpeedSprted;   //本摄像机是否支持设置采集速度
	BOOL m_bIsBlkSizeSprted;     //本摄像机是否支持设置取得消隐取值范围

	void SetExposureTime(int nWindWidth,long lTintUpper,long lTintLower);
	BOOL SetLutTable(double dRatioR,double dRatioG,double dRatioB);
	void InitParamFromINI();
	void InitHVDevice();
	void SaveParamToINI();
	//call back function
	static int CALLBACK SnapCallback(HV_SNAP_INFO *pInfo);
	int nMaxWidMd0, nMaxHeiMd0;
	void SetBlanking(int nHBlanking,int nVBlanking);
	// ************************

	bool   m_DigitalOut_NeedUpdateWnd;

	CString RandomCode();

	UINT  m_Close_TimerHandle;
	COleDateTime m_LastTime, m_LastComPortTime;

	void SaveModifiedRecord(bool IsEchoOn = true);
	void ShowRecordInfo(CString);
	void GetSfExt(CString str_MedicalNo, int &lknum, int &cgnum, int &bdnum, int &tjnum);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members

	void AddLogo();
	void RemoveLogo();

	CXTLogoPane      m_wndLogoPane;
	CXTStatusBar     m_wndStatusBar;
	CXTToolBar       m_wndToolBar;

	CImageList       m_ilToolBarGray;
	CBitmap          m_bmpToolBarGray;

// Generated message map functions
protected:
	CXTWindowPos     m_wndPosition;

public:
	int m_blkCount;
	// Overrode CWnd implementation to restore saved window position.
	BOOL ShowWindowEx(int nCmdShow);

	void RefreshLogo();
	void RefreshStatusBar();

protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnViewToolbar();
	afx_msg void OnUpdateViewToolbar(CCmdUI* pCmdUI);
	afx_msg void OnViewMedlist();
	afx_msg void OnUpdateViewMedlist(CCmdUI* pCmdUI);
	afx_msg void OnFieldNotification();
	afx_msg void OnMaintanceReport();
	afx_msg void OnReportDiscribe();
	afx_msg void OnReportRegion();
	afx_msg void OnReportDia();
	afx_msg void OnReportRysj();
	afx_msg void OnAcquire();
	afx_msg void OnUpdateAcquire(CCmdUI* pCmdUI);
	afx_msg void OnSelectSource();
	afx_msg void OnUpdateSelectSource(CCmdUI* pCmdUI);
	afx_msg void OnImgCapturestp();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnImgCaptureFormat();
	afx_msg void OnViewOutput();
	afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
	afx_msg void OnStandardView();
	afx_msg void OnStandardAdd();
	afx_msg void OnUpdateDocManager(CCmdUI *pCmdUI);
	afx_msg void OnDocManager();
	afx_msg void OnFileRptmodify();
	afx_msg void OnAppGetcode();
	afx_msg void OnUpdateAppGetcode(CCmdUI* pCmdUI);
	afx_msg void OnDhSetup();
	afx_msg void OnDhWb();
	afx_msg void OnDhGamma();
	afx_msg void OnDhCancelgamma();
	afx_msg void OnDhMode0();
	afx_msg void OnDhMode1();
	afx_msg void OnDhMode2();
	afx_msg void OnDhConversionGb();
	afx_msg void OnDhConversionGr();
	afx_msg void OnDhConversionBg();
	afx_msg void OnDhConversionRg();
	afx_msg void OnUpdateDhMode0(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDhMode1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDhMode2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDhConversionBg(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDhConversionGb(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDhConversionGr(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDhConversionRg(CCmdUI* pCmdUI);
	afx_msg void OnMaintanceSelffield();
	afx_msg void OnDocLogo();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	afx_msg void OnFileDbsetup();
	afx_msg void OnUpdateFileDbsetup(CCmdUI* pCmdUI);
	afx_msg void OnDocQuery();
	afx_msg void OnZpglCkyj();
	afx_msg void OnZpglYzdy();
	afx_msg void OnZpglQpyj();
	afx_msg void OnZpglZpqktj();
	afx_msg void OnUpdateImgCapturestp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMaintanceSelffield(CCmdUI* pCmdUI);
	afx_msg void OnMaintanceSelfshow();
	afx_msg void OnUpdateMaintanceSelfshow(CCmdUI* pCmdUI);
	afx_msg void OnWdmDevice();
	afx_msg void OnImgOlympus();
	afx_msg void OnZpglSfqr();
	afx_msg void OnZpglZCSfqr();
	afx_msg void OnZpglCgsfqr();
	afx_msg void OnFileRptview();
	afx_msg void OnMaintenancePicnote();
	//}}AFX_MSG

	afx_msg void OnUpdateChangeMedicalTable(CCmdUI *pCmdUI);
	afx_msg void OnChangeMedicalTable(UINT nID);

	afx_msg LRESULT ResizeVideoWindow(WPARAM wParam, LPARAM lParam);  // user message
	afx_msg LRESULT OnDeviceChanged(WPARAM wParam, LPARAM lParam);    // user message
	afx_msg LRESULT OnCaptureBitmap(WPARAM wParam, LPARAM lParam);    // user message
	afx_msg void OnSelchangeComboRulerMark();
	afx_msg void OnSelchangeComboSelPrinter();
	afx_msg LRESULT SwitchOn(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OkCapture(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT VfwCapture(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT DhhvCapture(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT DShowCapture(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT MVC3000Preview(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT MYDSHOWPreview(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT CanonS3S5Capture(WPARAM wParam, LPARAM lParam);

	LRESULT OnSnapMessage(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetRatioMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCommEvent();
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__1294D021_A6CE_4D17_905B_BE473EAEC302__INCLUDED_)
