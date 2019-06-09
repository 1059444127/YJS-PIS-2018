// Pathology.h : main header file for the Pathology application
//

#if !defined(AFX_PATHOLOGY_H__3FE98AE4_AFA4_4C57_93BB_B1DAC6B1DACF__INCLUDED_)
#define AFX_AFX_PATHOLOGY_H__3FE98AE4_AFA4_4C57_93BB_B1DAC6B1DACF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "EnumPrinters.h"
#include "cameracontrol.h"
#include "NoUse.h"
#include "CheckLicense.h"

#include "HISConnectClient.h"
#include "HISConnectClient1.h"


#define WM_LISTSELECT           WM_USER + 3
#define WM_CONTENTSAVE          WM_USER + 4
#define WM_CONTENTREFRESH       WM_USER + 5
#define WM_SWITCHON             WM_USER + 6
#define WM_OKCAPTURE            WM_USER + 7
#define WM_VFWCAPTURE           WM_USER + 8
#define WM_HVCAPTURE            WM_USER + 9
#define WM_DSHOWCAPTURE         WM_USER + 10
#define WM_VIDEOIN              WM_USER + 11
#define WM_MVC3000              WM_USER + 12
#define WM_MYDSHOW              WM_USER + 13
#define WM_CANONS3S5			WM_USER + 14

#define WM_CHANGE_SNAP		    WM_USER + 100
#define WM_GET_RATIO	     	WM_USER + 101

#define BITFLAG(k,i)   ((k >> i) & 0x01)
#define RIGHT_QXGL(k)         BITFLAG(k,0)      // Ȩ�޹���
#define RIGHT_SH(k)           BITFLAG(k,1)      // ���Ȩ��
#define RIGHT_BGDY(k)         BITFLAG(k,2)      // �����ӡȨ��
#define RIGHT_HQSJMM(k)       BITFLAG(k,3)      // ��ȡ�������Ȩ��
#define RIGHT_SCJL(k)         BITFLAG(k,4)      // ɾ����¼Ȩ��
#define RIGHT_SCTXYY(k)       BITFLAG(k,5)      // ɾ��ͼ�������Ȩ��
#define RIGHT_TXDC(k)         BITFLAG(k,6)      // ͼ�񵼳�Ȩ��
#define RIGHT_DAGL(k)         BITFLAG(k,7)      // ��������Ȩ��
#define RIGHT_XGXTSZ(k)       BITFLAG(k,8)      // �޸�ϵͳ����Ȩ��
#define RIGHT_FZCX(k)         BITFLAG(k,9)      // ���Ӳ�ѯȨ��
#define RIGHT_XGBH(k)         BITFLAG(k,10)     // �޸ı��Ȩ��
#define RIGHT_FSBG(k)         BITFLAG(k,11)     // ���淢��
#define RIGHT_SCCDLR(k)       BITFLAG(k,12)     // ɾ���ʵ�����
#define RIGHT_SCXTRZ(k)       BITFLAG(k,13)     // ɾ��ϵͳ��־
#define RIGHT_DLDJGZZ(k)      BITFLAG(k,14)     // ��¼�Ǽǹ���վ
#define RIGHT_DLQCGZZ(k)      BITFLAG(k,15)     // ��¼ȡ�Ĺ���վ
#define RIGHT_DLZDGZZ(k)      BITFLAG(k,16)     // ��¼��Ϲ���վ
#define RIGHT_DLZPGZZ(k)      BITFLAG(k,17)     // ��¼��Ƭ����վ
#define RIGHT_DLTBSGZZ(k)     BITFLAG(k,18)     // ��¼ϸ��ѧ����վ
#define RIGHT_BJBGGS(k)       BITFLAG(k,19)     // �༭�����ʽ
#define RIGHT_SCQPJL(k)       BITFLAG(k,20)     // �޸Ļ�ɾ����Ƭ��¼
#define RIGHT_ZDYKBJ(k)       BITFLAG(k,21)     // �Զ����༭
#define RIGHT_DLCGQPGZZ(k)    BITFLAG(k,22)     // ��¼������Ƭ����վ
#define RIGHT_DLTJGZZ(k)      BITFLAG(k,23)     // ��¼�ؼ칤��վ
#define RIGHT_DLXBXZPGZZ(k)   BITFLAG(k,24)     // ��¼ϸ��ѧ��Ƭ����վ
#define RIGHT_FSBDBG(k)       BITFLAG(k,25)     // ���ͱ�������
#define RIGHT_XGSCQCJL(k)     BITFLAG(k,26)     // ɾ��ȡ�ļ�¼

/////////////////////////////////////////////////////////////////////////////
// CPathologyApp:
// See Pathology.cpp for the implementation of this class
//

#define _CONTROLLER_ERRORMSG(e) \
	(e.ErrorInfo() ? wcslen(e.Description()) ? AfxMessageBox(e.Description(), MB_ICONEXCLAMATION) : \
	AfxMessageBox(e.ErrorMessage(), MB_ICONEXCLAMATION) : \
	AfxMessageBox(e.ErrorMessage(), MB_ICONEXCLAMATION))

// Trace Log 
#define _CONTROLLER_ERRORMSG_LOG(e) \
	(e.ErrorInfo() ? wcslen(e.Description()) ? TRACE(TEXT("%s (0x%lx)\n"), e.Description(), e.Error()) : \
	TRACE(TEXT("%s (0x%lx)\n"), e.ErrorMessage(), e.Error()) : \
	TRACE(TEXT("%s (0x%lx)\n"), e.ErrorMessage(), e.Error()))



class CPathologyView;

class CPathologyApp : public CWinApp
{
protected:
	CCameraControl m_CameraControl;
	void GetParam(CString sKeyName, CString &sKeyValue);

private:
	HMODULE		m_hStainingMachineInterfaceDll;

	int	 (*GetStainingMachineModuleCount_Dll)(void);
	void (*InitDBConnectionParams_Dll)(const char *szDBType, const char *szDBHostName, const char *szDBName, const char *szDBUserID, const char *szDBPswd);
	BOOL (*SendSlideIDs2StainingMachine_Dll)(const char *slidesIDs2Machine);


public:
	
	BOOL  IsStainingMachineModuleAvailable();
	BOOL  SendSlideIDs2StainingMachine(const char *slidesIDs2Machine);

private:

	int GB2U_Char(wchar_t * pOut, char *pText); 
	int GB2U_Str(wchar_t *pOut, char *pText, int len); 
	
public:

	CCheckLicense g_objLicense;


	CNoUse* noUse;
	void MicroWriterIniLoading();
	
	int hisStatus;
	CPathologyApp();

	inline CCameraControl* GetCamera() {return &m_CameraControl;}
	int m_ToCheck;

	void NotifyRecordStatus(CString);
	
	CWnd *m_MainThread_FrameWnd;
	bool IsLogOut;
    CString m_Exe_Working_Dir;
	char m_SysTmpPath[500];

	bool ConnectDatabase();
	void DisconnectDatabase();
	
	bool GetDatabaseValue(CString MedicalNo,CString Field, CString &Value, bool RealValue = false);

	CEnumPrinters	m_PrinterControl ;
	bool            m_PrinterSeting_Restored;
	bool			SetNewPrinter(int index) ;
	CString			GetDefaultPrinter() ;

	// �����, ���, ���
	// 54303    A1   ���� 
	void MakeEmbedBoxFile(const char *medicalNo, const char *serialNo, const char *mark);


	// ����ţ� ��ţ� ��Ƭ�Ӻţ� ��ע�� �����,  ���
	// 54303    A1     54303-A1   HE     5430302, ����
	void MakeSlidePrintFile(const char *medicalNo, const char *serialNo, const char *slideNo, const char *memo, const char *barCodeNo, const char *mark);


	CString  m_BdName;

	///////////////////////////////////////////////////////
	BOOL	m_bHideTreeEmptyNode;
	///////////////////////////////////////////////////////
	CString m_sDBType;
	CString m_sSchemaName;

	CString m_sLicHostName;
	int		m_nLicHostPort;

	///////////////////////////////////////////////////////



	void IniLoading();
	char ServerName[100];
	char DATABASENAME[100];
    char DATABASE_USERNAME[100];
    char DATABASE_PASSWORD[100];

	CStringArray  m_fnamelist;
	CStringArray  m_fdescriptionlist;
	CStringArray  m_finfolist;
	CUIntArray    m_ftypeList;

	CStringArray  m_DBName_list;
    CUIntArray    m_DBNo_list;

	CString m_DBType, m_OwnUser;
	CString m_Export2PdfMode, m_Export2ImgMode, m_CheckAndSendMode,m_AuthorCheckOnlyMode, m_AuthorPrintOnlyMode, m_MyReportQueryMode, m_TjyzMode, m_RecyclePath, m_LsjcMode, m_UnRead_Knly_Notified,m_RedValue,m_GreenValue,m_BlueValue;
	bool m_IsRecordFmtInBatchPrinting;
	CString m_ZdDyMode, m_BcbgAutoLockMode, m_AntiNullRecordMode, m_QpxxMode, m_QcysTbXr, m_TimeResMode, m_MyRptMode, m_LoadSoundTabMode, m_LoadVideoTabMode, m_MedPersonFocusMode, m_StringFormatMode, m_AutoRefreshDBListMode, m_NormalSliceMode, m_BdSliceMode, m_CurrentTimeMode, m_TmhMode, m_BlkNumber, m_ImgNumShowMode, m_TransImgMode, m_PrintAndSendMode, m_InitSubCode, m_Default_Sex, m_BarcodePaperType;
	float   m_ImgZoomRate;
	int     m_ComPort;
	
	///////////////////////////////////
	// HISConnect 
	int     m_nHISConnectCode;
	CString m_sHISConnectEndPoint;
	CString m_sHISConnectFileName;
	CString m_sHISConnectModality;

	///////////////////////////////////
	// HISConnect1 
	int     m_nHISConnect1Code;
	CString m_sHISConnect1EndPoint;
	CString m_sHISConnect1FileName;
	CString m_sHISConnect1Modality;

	///////////////////////////////////

	CString m_sStainingMachineInterfaceFileName;	// �Զ�Ⱦɫ���ӿ��ļ���
	CString m_sStainingMachineInterfaceModuleName;	// �Զ�Ⱦɫ���ӿ�ģ����

	///////////////////////////////////
	
	CString m_FTP_IP, m_FTP_USER, m_FTP_PASSWORD;
	CString m_CzysMode, m_ZzysMode, m_ShysMode, m_QcjlrMode, m_SjdwMode, m_Waiting;
	CString m_HisCfg, m_BggsGenMode, m_SelfDefineWndAutoOpenMode,m_BgyqAntoOpenMode;
	CString m_PrintingMode, m_PrintingMode1;
	CString m_PrintRestrict;
	CString m_PrintTime;
	CString m_NewBlhGenMode;
	CString m_SjrqTimeMode, m_QcrqTimeMode, m_BgrqTimeMode, m_BgrqRefreshMode;
	CString m_StationType;
	CString m_SavingMode;
    CString m_EditingMode;
	CString m_CaptureMode;
	LOGFONT   m_lfFont;        // ͼ�����ֱ�ע����
	COLORREF  m_lfColor;       // ͼ�����ֱ�ע��ɫ
	CString   m_Tag_LeftBottom;
	CString   m_Tag_RightBottom;
	CString	  m_PrintviewType;
	CString   m_CantainTsrs;
	CString	  m_IsUsingCard;	//�Ƿ����ö�����
	CString   m_IsEmbedBoxValid; //�Ƿ����ð����ӡ
	CString   m_EmbedBoxFilePath; //�����ļ���ӡ·��
	CString   m_EmbedBoxFmtCode; //�����ӡ��ʽ����
	CString   m_IsSlidePrintValid; //�Ƿ����ò�Ƭ��ӡ
	CString   m_SlidePrintFilePath; //��Ƭ�ļ���ӡ·��
	CString   m_SlidePrintFmtCode; //��Ƭ��ӡ��ʽ����
	CString   m_MaterialMode;	//ȡ��ɾ��ģʽ
	CString   m_BlhMode;		//������л�ȥ�����һλ
	int m_Aoi_Width, m_Aoi_Height;
	CString m_MicroWriterMode;  //��Ż���ӡģʽ 1��,2��
	CString m_FontOneLine,m_FontFirstLine,m_FontSecondLine,m_BeginIndex,m_FirstLineText; //��Ż�����

    CString TABLE_MAIN;
    CString TABLE_DEFINE;
    CString TABLE_CHARGE;
    CString TABLE_IMAGE;
    CString TABLE_SOUND;
    CString TABLE_HZ;
    CString TABLE_MATERIAL;
    CString TABLE_SLICE;
    CString TABLE_ADVICE;
	CString TABLE_TAG;
	CString TABLE_TC;
	CString TABLE_DBTYPE;
    CString TABLE_LOGO;
    CString TABLE_NOTEPAD;
    CString TABLE_SECURITY;
	CString TABLE_ICD10;
	CString TABLE_ZHANGQI;
	CString TABLE_BINGZHONG;
	CString TABLE_STANDTEXT;
	CString TABLE_BOOKING;
	CString TABLE_BOOKINGSAMPLE;
	CString m_LocalIp;
	CString TABLE_ZC;

	CString m_Cpp_User, m_Cpp_Password;
	int m_Cpp_Right;
	COleDateTime m_Login_Time;

	int   m_BarCodeNum;
	int   m_PrintType;
	bool  InPrinting;
	bool  Is_Batch_Printing;
	CString m_FmtName_in_Batch;
	CStringArray m_QptmhList, m_QpsmList, m_LkhList;

	int m_DB_Type_No;
	CString m_DB_Type_Name;

	CPathologyView * GetView();

	bool m_Refresh_Flag_Check, m_Refresh_Flag_Collect;

	bool IsRecordLocked(CString);

    void LoadPhoto(CString fname, CString blh, int sno); 
    void LoadSound(CString fname, CString blh, int sno); 

    //Veridicom -- Sensor related parameters
	SensorHandle_t m_nSensorHandle;
	long m_nNumRows;
	long m_nNumCols;
	long m_nMode;
	long m_nFlags;
	long VeriInit();

	SADateTime   GetServerDateTime();
	COleDateTime GetOleServerDateTime();
	void RemoveOldNotepad();

	bool isLogoTableExist;
	void OperationLogoRecord(CString optype, CString blh);

	void  SubmitReport(CStringArray &blhList);

    CString GetNewInstancdUID(void);

	void  ReflectDBChange(int, CString, bool IsGenNew = true);

	bool  Export2Image(CFODataModel *pFODataModel, CString Fname);
	bool  Export2PDF(CString ImgFname, CString PDFFname);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPathologyApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CPathologyApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFilePrint();
	afx_msg void OnAppCamera();
	afx_msg void OnFileSave();
	afx_msg void OnHelp();
	afx_msg void OnFileSyssetup();
	afx_msg void OnSetupTag();
	afx_msg void OnUpdateAppCamera(CCmdUI* pCmdUI);
	afx_msg void OnFileOpen();
	afx_msg void OnAppLast();
	afx_msg void OnAppNext();
	afx_msg void OnAppCheck();
	afx_msg void OnUpdateAppCheck(CCmdUI* pCmdUI);
	afx_msg void OnAppCollect();
	afx_msg void OnUpdateAppCollect(CCmdUI* pCmdUI);
	afx_msg void OnDocClose();
	afx_msg void OnUpdateDocClose(CCmdUI* pCmdUI);
	afx_msg void OnFileInput();
	afx_msg void OnFileMail();
	afx_msg void OnFileOutput();
	afx_msg void OnAppDbselect();
	afx_msg void OnDocSaveas();
	afx_msg void OnUpdateDocSaveas(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnFileSecuritysetup();
	afx_msg void OnUpdateFileSecuritysetup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSyssetup(CCmdUI* pCmdUI);
	afx_msg void OnAppRefresh();
	afx_msg void OnAppLogout();
	afx_msg void OnUpdateAppLast(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAppNext(CCmdUI* pCmdUI);
	afx_msg void OnAppSend();
	afx_msg void OnUpdateAppSend(CCmdUI* pCmdUI);
	afx_msg void OnAppUncheck();
	afx_msg void OnUpdateAppUncheck(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	afx_msg void OnReference();
	afx_msg void OnDocQuickinput();
	afx_msg void OnUpdateDocQuickinput(CCmdUI* pCmdUI);
	afx_msg void OnDocNewOrder();
	afx_msg void OnUpdateDocNewOrder(CCmdUI* pCmdUI);
	afx_msg void OnDocNewOrder1();
	afx_msg void OnUpdateDocNewOrder1(CCmdUI* pCmdUI);
	afx_msg void OnAppNotepad();
	afx_msg void OnFileRptcopy();
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg void OnFileRptcopyall();
	afx_msg void OnFileRptcopyimg();
	afx_msg void OnReportOutput();
	afx_msg void OnAppDZBL();
	afx_msg void OnAppYQBG();
	afx_msg void OnAppNew();
	afx_msg void OnWriterPrintSetup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CHISConnectClient g_HISConnectClient;
extern CHISConnectClient1 g_HISConnectClient1;

extern CPathologyApp theApp;


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATHOLOGY_H__3FE98AE4_AFA4_4C57_93BB_B1DAC6B1DACF__INCLUDED_)
