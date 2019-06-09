#if !defined(AFX_CAPTURECANONS5DLG_H__990FEAA0_C1FC_4E84_8C5B_D106A63FB1F9__INCLUDED_)
#define AFX_CAPTURECANONS5DLG_H__990FEAA0_C1FC_4E84_8C5B_D106A63FB1F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CaptureCanonS5Dlg.h : header file
//

#include <afxmt.h>

#include "prType.h"
#include "prError.h"
#include "prAPI.h"
#include "prFuncType.h"

#include "jpeglib.h"


#define		VIEWFINDER_WIDTH		320
#define		VIEWFINDER_HEIGHT		240
#define		VIEWFINDER_MUTEXNAME	"ViewFineder Mutex"

#define		FILEFORMAT_JPG			0
#define		FILEFORMAT_BMP			1

#define		CAMERAEVENTMESSAGESTRING_RELEASE_ON		"Camera Event ReleaseOn"
#define		CAMERAEVENTMESSAGESTRING_RELEASE_COMP	"Camera Event ReleaseComplete"
#define		CAMERAEVENTMESSAGESTRING_ABORT_PC_EVF	"Camera Event Abort PC EVF"
#define		CAMERAEVENTMESSAGESTRING_CHANGE_BY_UI	"Camera Event Change By UI"
#define		CAMERAEVENTMESSAGESTRING_RELEASE_DATA	"Camera Event Get Release Data"
#define		CAMERAEVENTMESSAGESTRING_SHUT_DOWN		"Camera Event Shut Down"

#ifndef		GETERRORID
#define		GETERRORID( x )		(cdERROR_ERRORID_MASK&x)
#endif

#define		MY_BUFFER_SIZE		20480L	/* 20KB */
#define		VF_BUFFER_SIZE		100 * 1024L	/* 100KB */

#define		FULL_VIEW_REL	0x0001		// Full View Release
#define		THMB_VIEW_REL	0x0002		// Thumbnail View Release

/* Information on a picture */
typedef	struct	tagDIBData
{
	HBITMAP		hBmp;
	LPVOID		vpBits;
}DIBData;

typedef struct {
	prUInt32		ContainerLength;
	prUInt16		ContainerType;
	prUInt16		Code;
	prUInt32		TransactionID;
	prUInt32		Parameter[prANY];
} EVENT_GENERIC_CONTAINER;

typedef struct{
	struct jpeg_decompress_struct	cinfo;
	struct jpeg_error_mgr			jerr;
	POINT		wh;
} PROJ;


/////////////////////////////////////////////////////////////////////////////
// CCaptureCanonS5Dlg dialog

class CCaptureCanonS5Dlg : public CDialog
{
// Construction
public:
	
	BOOL IsCreated() { return m_bCreated; }

	BOOL Connect();
	BOOL Disconnect();
	void HideWindow();


	CCaptureCanonS5Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCaptureCanonS5Dlg)
	enum { IDD = IDD_CAPTURE_CANON_S5 };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCaptureCanonS5Dlg)
	public:
	virtual BOOL Create(CWnd* pParentWnd);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	virtual void OnOK() { };
// Implementation
protected:

	int		m_nMinWidth, m_nMinHeight;
	int		m_nMaxWidth, m_nMaxHeight;

	BOOL	m_bCreated;
	CStatic	m_staticinformation;
	CStatic	m_Static_PhotoSize;
	CStatic	m_Static_PhotoQuality;
	CComboBox	m_CPhotQuality;
	CComboBox	m_CPhotoSize;
	CStatic	m_Static_ShootingMode;
	CButton	m_Static_ReleaseParam;
	CStatic	m_Static_ISOSpeed;
	CStatic	m_Static_Flash;
	CStatic	m_Static_ExpoComp;
	CComboBox	m_CShootingMode;
	CComboBox	m_CISOSpeed;
	CComboBox	m_CFlash;
	CComboBox	m_CExpoComp;
	CButton	m_CBrowse;
	CButton	m_CAEAF;
	CStatic	m_CViewImage;
	CButton	m_CViewFinder;
	CButton	m_CRelease;

	CButton	m_CDisconnect;
	CButton	m_CConnect;
	
	CString	m_CInfoString;
    CSliderCtrl	m_sliderf;



	/*	PRSEK CallBack Functions & Thread Function */
	static prResponse prSTDCALL CamCallBackFunc(	prHandle		CameraHandle,
										prContext		Context,
										prVoid*			pEventData);

	static prResponse prSTDCALL ViewFinderCallBackFun(	prHandle		CameraHandle,
												prContext		Context,
												prUInt32		Size,
												prVoid*			pEventData);
	static UINT WINAPI ViewFinderProc( LPVOID	vpParam );


private:

	void				ZoomPos();
	void				SetShootingModeBox();	
	void				SetExpoCompBox();
	void				SetFlashBox();	
	void				SetISOSpeedBox();	
	void				SetImageSizeBox();
	void				SetImageQualityBox();

	void				GetCameraInformation();

	BOOL				CreateDIBBuffer();
	prVoid				UpdateSetting();

	prVoid				IsSupportedCapRelPrm(prUInt8*	pDeviceInfo,
											 prBoolean*	pExpoureMode,
											 prBoolean*	pExpoureComp,
											 prBoolean*	pSTrobeSet,
											 prBoolean*	pISO,
											 prBoolean*	pPhotoEffect);

	prVoid				IsSupportedRCandEVF(prUInt8* pDeviceInfo,
											prBoolean* pbRC,
											prBoolean* pbEVF,
											prBoolean* pbAwb);

	prBoolean			IsSuppotedCapTransMode(prUInt8* pDeviceInfo,
											   prUInt16* pwCurrentValue,
											   prUInt16* pwMode);

	prBoolean			IsReleasePictureType(prUInt8* pDeviceInfo);


	BOOL				m_fVFEnd;			/* The flag of a view finder of operation */
	CStatic				*m_CpViewFinder;	/* The pointer to the class which shows the view finder picture */
	DIBData				m_BackSurface;		/* Information on a picture buffer */
	BOOL				m_fProgramRelease;	/* The release flag by the program */
	BOOL				m_fCamTypeEOS;
	BOOL				m_fMCard;

	UINT				m_ReleaseOnMessage;		/* A message when the shutter release of a camera is pushed */
	UINT				m_ReleaseCompleteMessage;/* A message when photography is completed */
	UINT				m_AbortPCEVF;
	CWinThread*			m_CpVFThread;			/* The class of a picture display thread */
	CMutex				m_CVFMutex;				/* The synchronous class of image data */
	UINT				m_ChangeByUI;			/* A message when the capture parameter of the camera is changed 
												   by operation of the camera UI. */
	UINT				m_CameraShutDown;		/*  */

	prHandle			m_hCamera;				/*	CameraHandle	*/

	prUInt16             m_min;
	prUInt16             m_max;
	prUInt16             m_step;

	prUInt8				m_ExpoCompCntVal;
	prUInt8				m_ShootingModeCntVal;
	prUInt16			m_FlashCntVal;
	prUInt16			m_ISOCntVal;
	prUInt16  			m_ImageSizeCntVal;
	prUInt16  			m_ImageQualityCntVal;
    prUInt16            m_ZoomPosCntVal;

	prObjectHandle		m_PicHandle;
	prObjectHandle		m_ThmbHandle;
	prUInt16			m_ReleaseComp;
	PROJ				m_Proj;
	prUInt8				m_ViewFinderData[VF_BUFFER_SIZE];
	prUInt32			m_VF_DataSize;
	prptpObjectFormatCode	m_FileType;
	prBoolean			m_fGetRelease;
	prBoolean			m_fInReleaseControl;



	void OpenViewFinder();
	void CloseViewFinder();

	void DisconnectCamera();
	void ConnectCamera();


protected:


	// Generated message map functions
	//{{AFX_MSG(CCaptureCanonS5Dlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnAeaf();
	afx_msg void OnRelease();
	afx_msg void OnSelchangeShootingMode();
	afx_msg void OnSelchangeExpoComp();
	afx_msg void OnSelchangeFlash();
	afx_msg void OnSelchangeISOSpeed();
	afx_msg void OnSelchangePhotoSize();
	afx_msg void OnSelchangePhotoQuality();
	afx_msg void OnCustomdrawSliderf(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSliderReleaseCapture(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAPTURECANONS5DLG_H__990FEAA0_C1FC_4E84_8C5B_D106A63FB1F9__INCLUDED_)
