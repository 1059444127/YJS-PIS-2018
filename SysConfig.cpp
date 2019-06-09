// SysConfig.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "SysConfig.h"
#include "CustomItemFont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysConfig dialog
extern CPathologyApp theApp;

CSysConfig::CSysConfig(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CSysConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSysConfig)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSysConfig::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysConfig)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysConfig, CXTResizeDialog)
	//{{AFX_MSG_MAP(CSysConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysConfig message handlers

BOOL CSysConfig::OnInitDialog()  
{
	CXTItemConstraints* pList;

	CXTResizeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rc;
	GetDlgItem(IDC_PLACEHOLDER)->GetWindowRect(&rc);
	ScreenToClient( &rc );

	m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");


	CString m_FtpIp = m_IniReader.getKeyValue("FTP��ַ(IP)","FTP����");
	if(m_FtpIp.IsEmpty())  m_FtpIp.Format("127.0.0.1");

	CString m_FtpUser = m_IniReader.getKeyValue("FTP�û�","FTP����");
	if(m_FtpUser.IsEmpty())  m_FtpUser.Format("sa");

	CString m_FtpPass = m_IniReader.getKeyValue("FTP����","FTP����");
	if(m_FtpPass.IsEmpty())  m_FtpPass.Format("sa");

	CString m_CaptureMode = m_IniReader.getKeyValue("�ɼ�ģʽ","Settings");
	if(m_CaptureMode.IsEmpty())  m_CaptureMode.Format("VFW�ɼ��豸֧��");

	CString m_PrintingMode = m_IniReader.getKeyValue("��ӡͼ�񷴲�","Settings");
	if(m_PrintingMode.IsEmpty())  m_PrintingMode.Format("�߷���");

	CString m_PrintingMode1 = m_IniReader.getKeyValue("��ӡͼ�����","Settings");
	if(m_PrintingMode1.IsEmpty())  m_PrintingMode1.Format("�ȱ������");

	CString m_SavingMode = m_IniReader.getKeyValue("�ڲ��洢��ʽ","Settings");
	if(m_SavingMode.IsEmpty())  m_SavingMode.Format("BMP");

	CString m_EditingMode = m_IniReader.getKeyValue("�ַ����ڱ༭ģʽ","Settings");
	if(m_EditingMode.IsEmpty())  m_EditingMode.Format("����");

	CString m_SjrqTimeMode = m_IniReader.getKeyValue("�ͼ���������","Settings");
	if(m_SjrqTimeMode.IsEmpty())  m_SjrqTimeMode.Format("��������ǰʱ��");

	CString m_QcrqTimeMode = m_IniReader.getKeyValue("ȡ����������","Settings");
	if(m_QcrqTimeMode.IsEmpty())  m_QcrqTimeMode.Format("��������ǰʱ��");

	CString m_BgrqTimeMode = m_IniReader.getKeyValue("������������","Settings");
	if(m_BgrqTimeMode.IsEmpty())  m_BgrqTimeMode.Format("��������ǰʱ��");

	CString m_CzysMode = m_IniReader.getKeyValue("����ҽ���Զ�����","Settings");
	if(m_CzysMode.IsEmpty())  m_CzysMode.Format("��");

	CString m_ZzysMode = m_IniReader.getKeyValue("����ҽ���Զ�����","Settings");
	if(m_ZzysMode.IsEmpty())  m_ZzysMode.Format("��");

	CString m_ShysMode = m_IniReader.getKeyValue("���ҽ���Զ�ȡ��ǰ��¼��","Settings");
	if(m_ShysMode.IsEmpty())  m_ShysMode.Format("��");

	CString m_QcjlrMode = m_IniReader.getKeyValue("ȡ�ļ�¼���Զ�ȡ��ǰ��¼��","Settings");
	if(m_QcjlrMode.IsEmpty())  m_QcjlrMode.Format("��");

	CString m_SjdwMode = m_IniReader.getKeyValue("�ͼ쵥λ�Զ�����","Settings");
	if(m_SjdwMode.IsEmpty())  m_SjdwMode.Format("��");

	CString m_BgrqRefreshMode = m_IniReader.getKeyValue("�Զ����±�������","Settings");
	if(m_BgrqRefreshMode.IsEmpty())  m_BgrqRefreshMode.Format("��");

	CString m_Waiting = m_IniReader.getKeyValue("�޲���ϵͳ�ȴ�ʱ��(����)","Settings");
	if(m_Waiting.IsEmpty())  m_Waiting.Format("5");

	LOGFONT   m_lfFont;        // ͼ�����ֱ�ע����
	COLORREF  m_lfColor;       // ͼ�����ֱ�ע��ɫ

    CString tmp;
	tmp = m_IniReader.getKeyValue("lfHeight","ͼ�����ֱ�ע����");
	m_lfFont.lfHeight = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfWidth","ͼ�����ֱ�ע����");
	m_lfFont.lfWidth = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfEscapement","ͼ�����ֱ�ע����");
	m_lfFont.lfEscapement = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfOrientation","ͼ�����ֱ�ע����");
	m_lfFont.lfOrientation = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfWeight","ͼ�����ֱ�ע����");
	m_lfFont.lfWeight = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfItalic","ͼ�����ֱ�ע����");
	m_lfFont.lfItalic = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfUnderline","ͼ�����ֱ�ע����");
	m_lfFont.lfUnderline = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfStrikeOut","ͼ�����ֱ�ע����");
	m_lfFont.lfStrikeOut = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfCharSet","ͼ�����ֱ�ע����");
	m_lfFont.lfCharSet = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfOutPrecision","ͼ�����ֱ�ע����");
	m_lfFont.lfOutPrecision = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfClipPrecision","ͼ�����ֱ�ע����");
	m_lfFont.lfClipPrecision = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfQuality","ͼ�����ֱ�ע����");
	m_lfFont.lfQuality = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfPitchAndFamily","ͼ�����ֱ�ע����");
	m_lfFont.lfPitchAndFamily = atoi(tmp);
	tmp = m_IniReader.getKeyValue("lfFaceName","ͼ�����ֱ�ע����");
	strcpy(m_lfFont.lfFaceName,tmp);
	tmp = m_IniReader.getKeyValue("��ɫ����","ͼ�����ֱ�ע����");
	int r = atoi(tmp);
	tmp = m_IniReader.getKeyValue("��ɫ����","ͼ�����ֱ�ע����");
	int g = atoi(tmp);
	tmp = m_IniReader.getKeyValue("��ɫ����","ͼ�����ֱ�ע����");
	int b = atoi(tmp);
	m_lfColor = RGB(r,g,b);

	CString m_LeftBottom  = m_IniReader.getKeyValue("ͼ�����½Ǳ�ע����","Settings");
	CString m_RightBottom = m_IniReader.getKeyValue("ͼ�����½Ǳ�ע����","Settings");
	CString m_AoiInfo     = m_IniReader.getKeyValue("ͼ��������ߴ�","Settings");
	if(m_AoiInfo.IsEmpty())  m_AoiInfo.Format("10,10");

	CString m_StationType = m_IniReader.getKeyValue("ϵͳ����վ��","Settings");
	if(m_StationType.IsEmpty())  m_StationType.Format("�Ǽǹ���վ");

	CString m_NewBlhType = m_IniReader.getKeyValue("�²���Ų�����ʽ","Settings");
	if(m_NewBlhType.IsEmpty())  m_NewBlhType.Format("�����ż�1");

	CString m_PrintRestrict = m_IniReader.getKeyValue("δ��˱��������ӡ","Settings");
	if(m_PrintRestrict.IsEmpty())  m_PrintRestrict.Format("��");

	CString m_PrintTime = m_IniReader.getKeyValue("���汨���ӡ�޶�ʱ��(����)","Settings");
	if(m_PrintTime.IsEmpty()) m_PrintTime.Format("0");

	CString m_HisCfg = m_IniReader.getKeyValue("HISConnectCode","Settings");
	if(m_HisCfg.IsEmpty())  m_HisCfg.Format("��");

	CString m_BggsGenMode = m_IniReader.getKeyValue("����¼��ӡ����ѡ��ģʽ","Settings");
	if(m_BggsGenMode.IsEmpty())  m_BggsGenMode.Format("�Զ�������һ��¼�����ʽ");

	CString m_SelfDefineWndAutoOpenMode = m_IniReader.getKeyValue("ϵͳ�����Զ����Զ��崰��","Settings");
	if(m_SelfDefineWndAutoOpenMode.IsEmpty())  m_SelfDefineWndAutoOpenMode.Format("��");

	CString m_BgyqAutoOpenMode = m_IniReader.getKeyValue("ϵͳ�����Զ��򿪱��������б�","Settings");
	if(m_SelfDefineWndAutoOpenMode.IsEmpty())  m_BgyqAutoOpenMode.Format("��");

	CString m_ComPortMode = m_IniReader.getKeyValue("�ⴥ��COM�˿ں�","Settings");
	if(m_ComPortMode.IsEmpty())  m_ComPortMode.Format("0");

	CString m_ImgZoomRate = m_IniReader.getKeyValue("ͼ��ɼ���������","Settings");
	if(m_ImgZoomRate.IsEmpty())  m_ImgZoomRate.Format("1");

	CString m_TimeResMode = m_IniReader.getKeyValue("���浥��ϸʱ��ģʽ","Settings");
	if(m_TimeResMode.IsEmpty())  m_TimeResMode.Format("��");

	CString m_MyRptMode = m_IniReader.getKeyValue("ϵͳ�Զ������ҵı���","Settings");
	if(m_MyRptMode.IsEmpty())  m_MyRptMode.Format("��");

	CString m_LoadSoundTabMode = m_IniReader.getKeyValue("װ����������","Settings");
	if(m_LoadSoundTabMode.IsEmpty())  m_LoadSoundTabMode.Format("��");

	CString m_LoadVideoTabMode = m_IniReader.getKeyValue("װ����Ƶ����","Settings");
	if(m_LoadVideoTabMode.IsEmpty())  m_LoadVideoTabMode.Format("��");
	
	CString m_MedPersonFocusMode = m_IniReader.getKeyValue("�½���¼������Ϣ����¼�뽹��","Settings");
	if(m_MedPersonFocusMode.IsEmpty())  m_MedPersonFocusMode.Format("����");

	CString m_StringFormatMode = m_IniReader.getKeyValue("���浥�����Ϣ��������","Settings");
	if(m_StringFormatMode.IsEmpty())  m_StringFormatMode.Format("��");

	CString m_AutoRefreshDBListMode = m_IniReader.getKeyValue("�Զ�ˢ�¼�¼�б�","Settings");
	if(m_AutoRefreshDBListMode.IsEmpty())  m_AutoRefreshDBListMode.Format("��");

	CString m_NormalSliceMode = m_IniReader.getKeyValue("������Ƭ˵��","Settings");
	if(m_NormalSliceMode.IsEmpty())  m_NormalSliceMode.Format("��");

	CString m_BdSliceMode = m_IniReader.getKeyValue("������Ƭ˵��","Settings");
	if(m_BdSliceMode.IsEmpty())  m_BdSliceMode.Format("����");

	CString m_CurrentTimeMode = m_IniReader.getKeyValue("��վ�㵱ǰʱ��ģʽ","Settings");
	if(m_CurrentTimeMode.IsEmpty())  m_CurrentTimeMode.Format("������ʱ��");

	CString m_TmhMode = m_IniReader.getKeyValue("��Ƭ����Ŵ�ӡ����ģʽ","Settings");
	if(m_TmhMode.IsEmpty())  m_TmhMode.Format("��");

	CString m_BlkNumber = m_IniReader.getKeyValue("����ƿ��Ҵ���","Settings");
	if(m_BlkNumber.IsEmpty())  m_BlkNumber.Format("");

	CString m_ImgNumShowMode = m_IniReader.getKeyValue("���������ʾ��¼��Ӧͼ����","Settings");
	if(m_ImgNumShowMode.IsEmpty())  m_ImgNumShowMode.Format("��");

	CString m_TransImgMode = m_IniReader.getKeyValue("�ύʱ�ش�����ͼ������","Settings");
	if(m_TransImgMode.IsEmpty())  m_TransImgMode.Format("��");

	CString m_PrintAndSendMode = m_IniReader.getKeyValue("��ӡ�����Ͳ�����","Settings");
	if(m_PrintAndSendMode.IsEmpty())  m_PrintAndSendMode.Format("��");

	CString m_InitSubCode = m_IniReader.getKeyValue("�����ʼ���","Settings");
	if(m_InitSubCode.IsEmpty())  m_InitSubCode.Format("");

	CString m_Default_Sex = m_IniReader.getKeyValue("�Ա�Ĭ��ֵ","Settings");
	if(m_Default_Sex.IsEmpty())  m_Default_Sex.Format("");

	CString m_BarcodePaperType = m_IniReader.getKeyValue("��ǩ��ӡֽ����","Settings");
	if(m_BarcodePaperType.IsEmpty())  m_BarcodePaperType.Format("����ֽ");

	CString m_lzxb = m_IniReader.getKeyValue("��״ϸ��","Settings");

	CString m_jgxb = m_IniReader.getKeyValue("����ϸ��","Settings");

	CString m_hsxb = m_IniReader.getKeyValue("����ϸ��","Settings");

	CString m_dcgr = m_IniReader.getKeyValue("�γ��Ⱦ","Settings");

	CString m_mjgr = m_IniReader.getKeyValue("ù����Ⱦ","Settings");

	CString m_nzjgr = m_IniReader.getKeyValue("�������Ⱦ","Settings");

	CString m_qgjgr = m_IniReader.getKeyValue("��˾���Ⱦ","Settings");

	CString m_fxjgr = m_IniReader.getKeyValue("���߾���Ⱦ","Settings");

	CString m_hpvgr = m_IniReader.getKeyValue("HPV��Ⱦ","Settings");

	CString m_bzbdgr = m_IniReader.getKeyValue("�������Ⱦ","Settings");

	CString m_lsrtzbdgr = m_IniReader.getKeyValue("������ͷ״������Ⱦ","Settings");

	CString m_QcysTbXr = m_IniReader.getKeyValue("ȡ��ҽ��ͬ��д��","Settings");
	if(m_QcysTbXr.IsEmpty())  m_QcysTbXr.Format("��");

	CString m_QpxxMode = m_IniReader.getKeyValue("��Ƭ��Ϣ��ʾδ�˶���Ƭ��¼","Settings");
	if(m_QpxxMode.IsEmpty())  m_QpxxMode.Format("��");

	CString m_AntiNullRecordMode = m_IniReader.getKeyValue("�ղ���д�����","Settings");
	if(m_AntiNullRecordMode.IsEmpty())  m_AntiNullRecordMode.Format("��");

	CString m_BcbgAutoLockMode = m_IniReader.getKeyValue("���䱨���Զ�����ʱ��","Settings");
	if(m_BcbgAutoLockMode.IsEmpty())  m_BcbgAutoLockMode.Format("72");

	CString m_ZdDyMode = m_IniReader.getKeyValue("δ��ϲ�������Ԥ���ʹ�ӡ","Settings");
	if(m_ZdDyMode.IsEmpty())  m_ZdDyMode.Format("��");

	CString m_RedValue = m_IniReader.getKeyValue("��ӡͼ���ɫƫ��","Settings");
	if(m_RedValue.IsEmpty())  m_RedValue.Format("0");

	CString m_GreenValue = m_IniReader.getKeyValue("��ӡͼ����ɫƫ��","Settings");
	if(m_GreenValue.IsEmpty())  m_GreenValue.Format("0");

	CString m_BlueValue = m_IniReader.getKeyValue("��ӡͼ����ɫƫ��","Settings");
	if(m_BlueValue.IsEmpty())  m_BlueValue.Format("0");

	CString m_LsjcMode = m_IniReader.getKeyValue("���μ���¼�������Ա����","Settings");
	if(m_LsjcMode.IsEmpty())  m_LsjcMode.Format("��");

	CString m_RecyclePath = m_IniReader.getKeyValue("ͼ�����վ�ļ���","Settings");
	if(m_RecyclePath.IsEmpty())  m_RecyclePath.Format("");

	CString m_TjyzMode = m_IniReader.getKeyValue("�ؼ칤��վҽ���б���ʾ���շ�δִ�еļ�¼","Settings");
	if(m_TjyzMode.IsEmpty())  m_TjyzMode.Format("��");

	CString m_MyReportQueryMode = m_IniReader.getKeyValue("�ҵı��������ʽ","Settings");
	if(m_MyReportQueryMode.IsEmpty())  m_MyReportQueryMode.Format("ģ��");

	CString m_AuthorPrintOnlyMode = m_IniReader.getKeyValue("�����ӡר��Ȩ��","Settings");
	if(m_AuthorPrintOnlyMode.IsEmpty())  m_AuthorPrintOnlyMode.Format("�ر�");
	
	//add
	CString m_AuthorCheckOnlyMode = m_IniReader.getKeyValue("�������ר��Ȩ��","Settings");
	if(m_AuthorCheckOnlyMode.IsEmpty()) m_AuthorCheckOnlyMode.Format("�ر�");

	CString m_CheckAndSendMode = m_IniReader.getKeyValue("��˲�����","Settings");
	if(m_CheckAndSendMode.IsEmpty())  m_CheckAndSendMode.Format("��");

	CString m_Export2ImgMode = m_IniReader.getKeyValue("���浥ͼ�����","Settings");
	if(m_Export2ImgMode.IsEmpty())  m_Export2ImgMode.Format("��");

	CString m_Export2PdfMode = m_IniReader.getKeyValue("���浥PDF���","Settings");
	if(m_Export2PdfMode.IsEmpty())  m_Export2PdfMode.Format("��");

	CString m_PreviewType = m_IniReader.getKeyValue("Ԥ����ʾ","Settings");
	if(m_PreviewType.IsEmpty()) m_PreviewType.Format("�������");

	CString m_IsUsingCard = m_IniReader.getKeyValue("�Ƿ����ö�����","Settings");
	if(m_IsUsingCard.IsEmpty()) m_IsUsingCard.Format("��");

	//����д�ӡ
	CString m_IsEmbedBoxValid = m_IniReader.getKeyValue("�Ƿ����ð����ӡ","Settings");
	if(m_IsEmbedBoxValid.IsEmpty()) m_IsEmbedBoxValid.Format("��");

	//����д�ӡ�ļ�·��
	CString m_EmbedBoxFilePath = m_IniReader.getKeyValue("�����ӡ�ļ�·��","Settings");
	
	//�����ӡ��ʽ����
	CString m_EmbedBoxFmtCode = m_IniReader.getKeyValue("�����ӡ��ʽ����","Settings");

	
	//��Ƭ��ӡ
	CString m_IsSlidePrintValid = m_IniReader.getKeyValue("�Ƿ����ò�Ƭ��ӡ","Settings");
	if(m_IsSlidePrintValid.IsEmpty()) m_IsSlidePrintValid.Format("��");

	//��Ƭ��ӡ�ļ�·��
	CString m_SlidePrintFilePath = m_IniReader.getKeyValue("��Ƭ��ӡ�ļ�·��","Settings");
	
	//��Ƭӡ��ʽ����
	CString m_SlidePrintFmtCode = m_IniReader.getKeyValue("��Ƭ��ӡ��ʽ����","Settings");


	CString m_MaterialMode = m_IniReader.getKeyValue("ȡ�ļ�¼ֻ��ȡ��ҽʦ�ͼ�¼�˿����޸�","Settings");
	if(m_MaterialMode.IsEmpty()) m_MaterialMode.Format("��");

	CString m_BlhMode = m_IniReader.getKeyValue("������Ų����л�ȥ����������һλ","Settings");
	if(m_BlhMode.IsEmpty()) m_BlhMode.Format("��");

	CString m_CantainTsrs = m_IniReader.getKeyValue("������Ƭ����վ������Ⱦɫ","Settings");
	if(m_CantainTsrs.IsEmpty()) m_CantainTsrs.Format("��");
	
	if ( m_wndPropertyGrid.Create( rc, this, IDC_PROPERTY_GRID ) )
	{
		XT_LOGFONT lf;
		xtAfxData.font.GetLogFont( &lf );
		m_wndPropertyGrid.ShowHelp(FALSE);

		// create document settings category.

		CXTPropertyGridItem* pRuntime = m_wndPropertyGrid.AddCategory(_T("Settings"));

		CXTPropertyGridItem* pItemHisCfg = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("HISConnectCode"), m_HisCfg));
		
		CXTPropertyGridItem* pItemBlkNumber = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("����ƿ��Ҵ���"), m_BlkNumber));
		pItemBlkNumber->SetFlags(pgiHasEdit);

		CXTPropertyGridItem* pItemStationType = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("ϵͳ����վ��"),m_StationType));
		
		pList = pItemStationType->GetConstraints();
		pList->AddConstraint(_T("�Ǽǹ���վ"));
		pList->AddConstraint(_T("ȡ�Ĺ���վ"));
		pList->AddConstraint(_T("��Ϲ���վ"));
		pList->AddConstraint(_T("��Ƭ����վ"));
		pList->AddConstraint(_T("ϸ��ѧ����վ"));
		pList->AddConstraint(_T("������Ƭ����վ"));
		pList->AddConstraint(_T("�ؼ칤��վ"));
		pList->AddConstraint(_T("ϸ��ѧ��Ƭ����վ"));
		pItemStationType->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemCurrentTimeMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("��վ�㵱ǰʱ��ģʽ"), m_CurrentTimeMode));
		pList = pItemCurrentTimeMode->GetConstraints();
		pList->AddConstraint(_T("������ʱ��"));
		pList->AddConstraint(_T("����ʱ��"));
		pItemCurrentTimeMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemLoadSoundTabMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("װ����������"), m_LoadSoundTabMode));
		pList = pItemLoadSoundTabMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemLoadSoundTabMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemLoadVideoTabMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("װ����Ƶ����"), m_LoadVideoTabMode));
		pList = pItemLoadVideoTabMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemLoadVideoTabMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemCaptureMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�ɼ�ģʽ"), m_CaptureMode));
		//CXTItemConstraints* pList = pItemCaptureMode->GetConstraints();
		pList = pItemCaptureMode->GetConstraints();
		pList->AddConstraint(_T("VFW�ɼ��豸֧��"));
		pList->AddConstraint(_T("TWAIN�ɼ��豸֧��"));
		pList->AddConstraint(_T("Okͼ��ɼ���"));
		pList->AddConstraint(_T("DH-HVxxxxUϵ�вɼ���"));
		pList->AddConstraint(_T("DirectShow�ɼ��豸֧��"));
		pList->AddConstraint(_T("MVC3000�ɼ���"));
		pList->AddConstraint(_T("΢��DSHOW�豸"));
		pList->AddConstraint(_T("Nikon�豸֧��"));
		pList->AddConstraint(_T("Canon S3/S5 IS"));

		pItemCaptureMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemSavingMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�ڲ��洢��ʽ"), m_SavingMode));
		pList = pItemSavingMode->GetConstraints();
		pList->AddConstraint(_T("BMP"));
		pList->AddConstraint(_T("JPEG"));
		pList->AddConstraint(_T("GTiff"));
		pItemSavingMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemImgZoomRate = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("ͼ��ɼ���������"), m_ImgZoomRate));
		pItemImgZoomRate->SetFlags(pgiHasEdit);

		CXTPropertyGridItem* pItemComPortMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�ⴥ��COM�˿ں�"), m_ComPortMode));
		pItemComPortMode->SetFlags(pgiHasEdit);

		CXTPropertyGridItem* pItemFont = pRuntime->AddChildItem(new CCustomItemFont(_T("ͼ�����ֱ�ע����"), m_lfFont, m_lfColor));
		
		CXTPropertyGridItem* pItemLeftBottom = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("ͼ�����½Ǳ�ע����"), m_LeftBottom));
		pItemLeftBottom->SetFlags(pgiHasEdit);
		
		CXTPropertyGridItem* pItemRightBottom = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("ͼ�����½Ǳ�ע����"), m_RightBottom));
		pItemRightBottom->SetFlags(pgiHasEdit);
		
		CSize aoisize;
		aoisize.cx = atoi(m_AoiInfo.Left(m_AoiInfo.Find(',')));;
		m_AoiInfo = m_AoiInfo.Right(m_AoiInfo.GetLength() - m_AoiInfo.Find(',') - 1);
		aoisize.cy = atoi(m_AoiInfo);
		
        CXTPropertyGridItem* pItemAoiSize = pRuntime->AddChildItem(new CXTPropertyGridItemSize(_T("ͼ��������ߴ�"), aoisize));

		CXTPropertyGridItem* pItemRecyclePath = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("ͼ�����վ�ļ���"), m_RecyclePath));
		pItemRecyclePath->SetFlags(pgiHasEdit);

		CXTPropertyGridItem* pItemMedPersonFocusMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�½���¼������Ϣ����¼�뽹��"),m_MedPersonFocusMode));
		pList = pItemMedPersonFocusMode->GetConstraints();
		pList->AddConstraint(_T("����"));
		pList->AddConstraint(_T("HISʶ����"));
		pList->AddConstraint(_T("����ID"));
		pList->AddConstraint(_T("�����"));
		pList->AddConstraint(_T("סԺ��"));
		pItemMedPersonFocusMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemAntiNullRecordMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�ղ���д�����"), m_AntiNullRecordMode));
		pList = pItemAntiNullRecordMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("�ر�"));
		pItemAntiNullRecordMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemNewBlhGenMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�²���Ų�����ʽ"), m_NewBlhType));
		pList = pItemNewBlhGenMode->GetConstraints();
		pList->AddConstraint(_T("�����ż�1"));
		pList->AddConstraint(_T("���²���ż�1"));
		pList->AddConstraint(_T("���²����ȥ�����һλ��1"));
		pItemNewBlhGenMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemEditingMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�ַ����ڱ༭ģʽ"),m_EditingMode));
		pList = pItemEditingMode->GetConstraints();
		pList->AddConstraint(_T("����"));
		pList->AddConstraint(_T("����"));
		pItemEditingMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemLsjcMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("���μ���¼�������Ա����"), m_LsjcMode));
		pList = pItemLsjcMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemLsjcMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemSjdwMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�ͼ쵥λ�Զ�����"), m_SjdwMode));
		pList = pItemSjdwMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemSjdwMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemSjrqTimeMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�ͼ���������"), m_SjrqTimeMode));
		pList = pItemSjrqTimeMode->GetConstraints();
		pList->AddConstraint(_T("��������ǰʱ��"));
		pList->AddConstraint(_T("��һ����¼ʱ��"));
		pItemSjrqTimeMode->SetFlags(pgiHasComboButton);
		
		CXTPropertyGridItem* pItemQcrqTimeMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("ȡ����������"), m_QcrqTimeMode));
		pList = pItemQcrqTimeMode->GetConstraints();
		pList->AddConstraint(_T("��������ǰʱ��"));
		pList->AddConstraint(_T("��һ����¼ʱ��"));
		pItemQcrqTimeMode->SetFlags(pgiHasComboButton);
		
		CXTPropertyGridItem* pItemBgrqTimeMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("������������"), m_BgrqTimeMode));
		pList = pItemBgrqTimeMode->GetConstraints();
		pList->AddConstraint(_T("��������ǰʱ��"));
		pList->AddConstraint(_T("��һ����¼ʱ��"));
		pItemBgrqTimeMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemBgrqRefreshMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�Զ����±�������"), m_BgrqRefreshMode));
		pList = pItemBgrqRefreshMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemBgrqRefreshMode->SetFlags(pgiHasComboButton);
		
		CXTPropertyGridItem* pItemQcjrlMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("ȡ�ļ�¼���Զ�ȡ��ǰ��¼��"), m_QcjlrMode));
		pList = pItemQcjrlMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemQcjrlMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemQcysTbxr = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("ȡ��ҽ��ͬ��д��"), m_QcysTbXr));
		pList = pItemQcysTbxr->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("����ҽ��"));
		pList->AddConstraint(_T("����ҽ��"));
		pItemQcysTbxr->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemCzysMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("����ҽ���Զ�����"), m_CzysMode));
		pList = pItemCzysMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemCzysMode->SetFlags(pgiHasComboButton);
		
		CXTPropertyGridItem* pItemZzysMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("����ҽ���Զ�����"), m_ZzysMode));
		pList = pItemZzysMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemZzysMode->SetFlags(pgiHasComboButton);
		
		CXTPropertyGridItem* pItemShysMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("���ҽ���Զ�ȡ��ǰ��¼��"), m_ShysMode));
		pList = pItemShysMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemShysMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemAutoRefreshDBListMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�Զ�ˢ�¼�¼�б�"), m_AutoRefreshDBListMode));
		pList = pItemAutoRefreshDBListMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemAutoRefreshDBListMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemMyRptMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("ϵͳ�Զ������ҵı���"), m_MyRptMode));
		pList = pItemMyRptMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemMyRptMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemMyReportQueryMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�ҵı��������ʽ"), m_MyReportQueryMode));
		pList = pItemMyReportQueryMode->GetConstraints();
		pList->AddConstraint(_T("ģ��"));
		pList->AddConstraint(_T("��ȷ"));
		pItemMyReportQueryMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemInitSubCode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�����ʼ���"), m_InitSubCode));
		pItemInitSubCode->SetFlags(pgiHasEdit);

		CXTPropertyGridItem* pItemNormalSliceMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("������Ƭ˵��"),m_NormalSliceMode));
		pList = pItemNormalSliceMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("ȡ�Ĳ�λ"));
		pList->AddConstraint(_T("HE"));
		pItemNormalSliceMode->SetFlags(pgiHasComboButton);
		
		CXTPropertyGridItem* pItemBdSliceMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("������Ƭ˵��"),m_BdSliceMode));
		pList = pItemBdSliceMode->GetConstraints();
		pList->AddConstraint(_T("����"));
		pList->AddConstraint(_T("������+������"));
		pList->AddConstraint(_T("������"));
		pItemBdSliceMode->SetFlags(pgiHasComboButton);
				
		CXTPropertyGridItem* pItemTmhMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("��Ƭ����Ŵ�ӡ����ģʽ"), m_TmhMode));
		pList = pItemTmhMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemTmhMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemBarcodePaperType = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("��ǩ��ӡֽ����"), m_BarcodePaperType));
		pList = pItemBarcodePaperType->GetConstraints();
		pList->AddConstraint(_T("����ֽ"));
		pList->AddConstraint(_T("������ֽ"));
		pItemBarcodePaperType->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemQpxxMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("��Ƭ��Ϣ��ʾδ�˶���Ƭ��¼"), m_QpxxMode));
		pList = pItemQpxxMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemQpxxMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemCantainTsrs = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("������Ƭ����վ������Ⱦɫ"), m_CantainTsrs));
		pList = pItemCantainTsrs->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemCantainTsrs->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemTjyzMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�ؼ칤��վҽ���б���ʾ���շ�δִ�еļ�¼"), m_TjyzMode));
		pList = pItemTjyzMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemTjyzMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemPrintingMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("��ӡͼ�񷴲�"), m_PrintingMode));
		pList = pItemPrintingMode->GetConstraints();
		pList->AddConstraint(_T("�߷���"));
		pList->AddConstraint(_T("��ͨ"));
		pList->AddConstraint(_T("��Ļ"));
		pList->AddConstraint(_T("RGB��ɫ"));
		pItemPrintingMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemRed = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("��ӡͼ���ɫƫ��"), m_RedValue));
		pItemRed->SetFlags(pgiHasEdit);

		CXTPropertyGridItem* pItemGreen = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("��ӡͼ����ɫƫ��"), m_GreenValue));
		pItemGreen->SetFlags(pgiHasEdit);
		
		CXTPropertyGridItem* pItemBlue = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("��ӡͼ����ɫƫ��"), m_BlueValue));
		pItemBlue->SetFlags(pgiHasEdit);

		CXTPropertyGridItem* pItemPrintingMode1 = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("��ӡͼ�����"), m_PrintingMode1));
		pList = pItemPrintingMode1->GetConstraints();
		pList->AddConstraint(_T("�ȱ������"));
		pList->AddConstraint(_T("ָ���ߴ����"));
		pItemPrintingMode1->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemImgNumShowMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("���������ʾ��¼��Ӧͼ����"), m_ImgNumShowMode));
		pList = pItemImgNumShowMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemImgNumShowMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemAuthorPrintOnlyMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�����ӡר��Ȩ��"), m_AuthorPrintOnlyMode));
		pList = pItemAuthorPrintOnlyMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("�ر�"));
		pItemAuthorPrintOnlyMode->SetFlags(pgiHasComboButton);
		pRuntime->Expand();
		
		//add
		CXTPropertyGridItem* pItemAuthorCheckOnlyMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�������ר��Ȩ��"),m_AuthorCheckOnlyMode));
		pList = pItemAuthorCheckOnlyMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("�ر�"));
		pItemAuthorCheckOnlyMode->SetFlags(pgiHasComboButton);
		pRuntime->Expand();	

		CXTPropertyGridItem* pItemExport2ImgMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("���浥ͼ�����"), m_Export2ImgMode));
		pList = pItemExport2ImgMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemExport2ImgMode->SetFlags(pgiHasComboButton);	
		
		CXTPropertyGridItem* pItemExport2PdfMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("���浥PDF���"), m_Export2PdfMode));
		pList = pItemExport2PdfMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemExport2PdfMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemBcbgAutoLock = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("���䱨���Զ�����ʱ��"), m_BcbgAutoLockMode));
		pItemBcbgAutoLock->SetFlags(pgiHasEdit);

		CXTPropertyGridItem* pItemBggsGenMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("����¼��ӡ����ѡ��ģʽ"), m_BggsGenMode));
		pList = pItemBggsGenMode->GetConstraints();
		pList->AddConstraint(_T("�Զ�������һ��¼�����ʽ"));
		pList->AddConstraint(_T("�Զ����ձ���ͼ����ѡ�񱨸��ʽ"));
		pItemBggsGenMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemStringFormatMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("���浥�����Ϣ��������"), m_StringFormatMode));
		pList = pItemStringFormatMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemStringFormatMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemTimeResMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("���浥��ϸʱ��ģʽ"), m_TimeResMode));
		pList = pItemTimeResMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemTimeResMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemZddyMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("δ��ϲ�������Ԥ���ʹ�ӡ"), m_ZdDyMode));
		pList = pItemZddyMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemZddyMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemPrintRestrictMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("δ��˱��������ӡ"), m_PrintRestrict));
		pList = pItemPrintRestrictMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemPrintRestrictMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemPrintTime = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("���汨���ӡ�޶�ʱ��(����)"), m_PrintTime));
		pItemBlkNumber->SetFlags(pgiHasEdit);

		CXTPropertyGridItem* pItemCheckAndSendMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("��˲�����"), m_CheckAndSendMode));
		pList = pItemCheckAndSendMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemCheckAndSendMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemPrintAndSendMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("��ӡ�����Ͳ�����"), m_PrintAndSendMode));
		pList = pItemPrintAndSendMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemPrintAndSendMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemTransImgMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�ύʱ�ش�����ͼ������"), m_TransImgMode));
		pList = pItemTransImgMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemTransImgMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemSelfDefineWndAutoOpenMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("ϵͳ�����Զ����Զ��崰��"), m_SelfDefineWndAutoOpenMode));
		pList = pItemSelfDefineWndAutoOpenMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		pItemSelfDefineWndAutoOpenMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemBgyqWndAutoOpenMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("ϵͳ�����Զ��򿪱��������б�"), m_BgyqAutoOpenMode));
		pList = pItemBgyqWndAutoOpenMode->GetConstraints();
		pList->AddConstraint(_T("��")); 
		pList->AddConstraint(_T("��"));
		pItemBgyqWndAutoOpenMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemWaiting = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�޲���ϵͳ�ȴ�ʱ��(����)"), m_Waiting));
		pItemWaiting->SetFlags(pgiHasEdit);

		CXTPropertyGridItem* pItemDefaultSex = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�Ա�Ĭ��ֵ"), m_Default_Sex));
		pList = pItemDefaultSex->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("Ů"));
		pItemDefaultSex->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* pLzxb = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("��״ϸ��"), m_lzxb));
		pList = pLzxb->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("����"));
		pList->AddConstraint(_T("δ��"));
		pLzxb->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* pJgxb = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("����ϸ��"), m_jgxb));
		pList = pJgxb->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("����"));
		pList->AddConstraint(_T("δ��"));
		pJgxb->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* pHsxb = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("����ϸ��"), m_hsxb));
		pList = pHsxb->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("����"));
		pList->AddConstraint(_T("δ��"));
		pHsxb->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* pdcgr = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�γ��Ⱦ"), m_dcgr));
		pList = pdcgr->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("����"));
		pList->AddConstraint(_T("δ��"));
		pdcgr->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* pmjgr = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("ù����Ⱦ"), m_mjgr));
		pList = pmjgr->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("����"));
		pList->AddConstraint(_T("δ��"));
		pmjgr->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* pnzjgr = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�������Ⱦ"), m_nzjgr));
		pList = pnzjgr->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("����"));
		pList->AddConstraint(_T("δ��"));
		pnzjgr->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* pqgjgr = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("��˾���Ⱦ"), m_qgjgr));
		pList = pqgjgr->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("����"));
		pList->AddConstraint(_T("δ��"));
		pqgjgr->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* pfxjgr = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("���߾���Ⱦ"), m_fxjgr));
		pList = pfxjgr->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("����"));
		pList->AddConstraint(_T("δ��"));
		pfxjgr->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* phpvgr = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("HPV��Ⱦ"), m_hpvgr));
		pList = phpvgr->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("����"));
		pList->AddConstraint(_T("δ��"));
		phpvgr->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* pbzbdgr = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�������Ⱦ"), m_bzbdgr));
		pList = pbzbdgr->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("����"));
		pList->AddConstraint(_T("δ��"));
		pbzbdgr->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* plsrtzbdgr = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("������ͷ״������Ⱦ"), m_lsrtzbdgr));
		pList = plsrtzbdgr->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("����"));
		pList->AddConstraint(_T("δ��"));
		plsrtzbdgr->SetFlags(pgiHasComboButton | pgiHasEdit);
		
		CXTPropertyGridItem* preViewType = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("Ԥ����ʾ"), m_PreviewType));
		pList = preViewType->GetConstraints();
		pList->AddConstraint(_T("�������"));
		pList->AddConstraint(_T("��ӡԤ��"));
		preViewType->SetFlags(pgiHasComboButton | pgiHasEdit);
		
		CXTPropertyGridItem* isUsingCard = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�Ƿ����ö�����"), m_IsUsingCard));
		pList = isUsingCard->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		isUsingCard->SetFlags(pgiHasComboButton | pgiHasEdit);

		//����д�ӡ
		CXTPropertyGridItem* isEmbedBoxValid = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�Ƿ����ð����ӡ"), m_IsEmbedBoxValid));
		pList = isEmbedBoxValid->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		isEmbedBoxValid->SetFlags(pgiHasComboButton | pgiHasEdit);

		//����д�ӡ�ļ�·��
		CXTPropertyGridItem* embedBoxFilePath = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�����ӡ�ļ�·��"), m_EmbedBoxFilePath));
		embedBoxFilePath->SetFlags( pgiHasEdit);

		//�����ӡ��ʽ����
		CXTPropertyGridItem* embedBoxFmtCode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�����ӡ��ʽ����"), m_EmbedBoxFmtCode));
		embedBoxFmtCode->SetFlags( pgiHasEdit);


		//��Ƭ��ӡ
		CXTPropertyGridItem* isSlidePrintValid = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("�Ƿ����ò�Ƭ��ӡ"), m_IsSlidePrintValid));
		pList = isSlidePrintValid->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		isSlidePrintValid->SetFlags(pgiHasComboButton | pgiHasEdit);

		//��Ƭ��ӡ�ļ�·��
		CXTPropertyGridItem* slidePrintFilePath = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("��Ƭ��ӡ�ļ�·��"), m_SlidePrintFilePath));
		slidePrintFilePath->SetFlags( pgiHasEdit);

		//��Ƭ��ӡ��ʽ����
		CXTPropertyGridItem* slidePrintFmtCode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("��Ƭ��ӡ��ʽ����"), m_SlidePrintFmtCode));
		slidePrintFmtCode->SetFlags( pgiHasEdit);


		CXTPropertyGridItem* materialMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("ȡ�ļ�¼ֻ��ȡ��ҽʦ�ͼ�¼�˿����޸�"), m_MaterialMode));
		pList = materialMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		materialMode->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* blhMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("������Ų����л�ȥ����������һλ"), m_BlhMode));
		pList = blhMode->GetConstraints();
		pList->AddConstraint(_T("��"));
		pList->AddConstraint(_T("��"));
		blhMode->SetFlags(pgiHasComboButton | pgiHasEdit);
			 
    } 
	
	SetResize(IDC_PROPERTY_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	
	SetResize(IDOK,       SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDCANCEL,   SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSysConfig::OnOK() 
{
	// TODO: Add extra validation here
	m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");
	CXTPropertyGridItem* pItem;

	pItem = m_wndPropertyGrid.FindItem(_T("�ɼ�ģʽ"));	
	m_IniReader.setKey(pItem->GetValue(),"�ɼ�ģʽ","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("��ӡͼ�񷴲�"));	
	m_IniReader.setKey(pItem->GetValue(),"��ӡͼ�񷴲�","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("��ӡͼ�����"));	
	m_IniReader.setKey(pItem->GetValue(),"��ӡͼ�����","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("�ڲ��洢��ʽ"));	
	m_IniReader.setKey(pItem->GetValue(),"�ڲ��洢��ʽ","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("�ַ����ڱ༭ģʽ"));	
	m_IniReader.setKey(pItem->GetValue(),"�ַ����ڱ༭ģʽ","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("�ͼ���������"));	
	m_IniReader.setKey(pItem->GetValue(),"�ͼ���������","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("ȡ����������"));	
	m_IniReader.setKey(pItem->GetValue(),"ȡ����������","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("������������"));	
	m_IniReader.setKey(pItem->GetValue(),"������������","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("����ҽ���Զ�����"));	
	m_IniReader.setKey(pItem->GetValue(),"����ҽ���Զ�����","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("����ҽ���Զ�����"));	
	m_IniReader.setKey(pItem->GetValue(),"����ҽ���Զ�����","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("���ҽ���Զ�ȡ��ǰ��¼��"));	
	m_IniReader.setKey(pItem->GetValue(),"���ҽ���Զ�ȡ��ǰ��¼��","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("ȡ�ļ�¼���Զ�ȡ��ǰ��¼��"));	
	m_IniReader.setKey(pItem->GetValue(),"ȡ�ļ�¼���Զ�ȡ��ǰ��¼��","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("�ͼ쵥λ�Զ�����"));	
	m_IniReader.setKey(pItem->GetValue(),"�ͼ쵥λ�Զ�����","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("�Զ����±�������"));	
	m_IniReader.setKey(pItem->GetValue(),"�Զ����±�������","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("�޲���ϵͳ�ȴ�ʱ��(����)"));	
	m_IniReader.setKey(pItem->GetValue(),"�޲���ϵͳ�ȴ�ʱ��(����)","Settings");

	LOGFONT   m_lfFont;        // ͼ�����ֱ�ע����
	COLORREF  m_lfColor;       // ͼ�����ֱ�ע��ɫ

	pItem = m_wndPropertyGrid.FindItem(_T("ͼ�����ֱ�ע����"));	
	((CCustomItemFont *)pItem)->GetFont(&m_lfFont);
	m_lfColor = ((CCustomItemFont *)pItem)->m_Color;

	CString tmp;
	tmp.Format("%d", m_lfFont.lfHeight);
	m_IniReader.setKey(tmp,"lfHeight","ͼ�����ֱ�ע����");
	tmp.Format("%d", m_lfFont.lfWidth);
	m_IniReader.setKey(tmp,"lfWidth","ͼ�����ֱ�ע����");
	tmp.Format("%d", m_lfFont.lfEscapement);
	m_IniReader.setKey(tmp,"lfEscapement","ͼ�����ֱ�ע����");
	tmp.Format("%d", m_lfFont.lfOrientation);
	m_IniReader.setKey(tmp,"lfOrientation","ͼ�����ֱ�ע����");
	tmp.Format("%d", m_lfFont.lfWeight);
	m_IniReader.setKey(tmp,"lfWeight","ͼ�����ֱ�ע����");
	tmp.Format("%d", m_lfFont.lfItalic);
	m_IniReader.setKey(tmp,"lfItalic","ͼ�����ֱ�ע����");
	tmp.Format("%d", m_lfFont.lfUnderline);
	m_IniReader.setKey(tmp,"lfUnderline","ͼ�����ֱ�ע����");
	tmp.Format("%d", m_lfFont.lfStrikeOut);
	m_IniReader.setKey(tmp,"lfStrikeOut","ͼ�����ֱ�ע����");
	tmp.Format("%d", m_lfFont.lfCharSet);
	m_IniReader.setKey(tmp,"lfCharSet","ͼ�����ֱ�ע����");
	tmp.Format("%d", m_lfFont.lfOutPrecision);
	m_IniReader.setKey(tmp,"lfOutPrecision","ͼ�����ֱ�ע����");
	tmp.Format("%d", m_lfFont.lfClipPrecision);
	m_IniReader.setKey(tmp,"lfClipPrecision","ͼ�����ֱ�ע����");
	tmp.Format("%d", m_lfFont.lfQuality);
	m_IniReader.setKey(tmp,"lfQuality","ͼ�����ֱ�ע����");
	tmp.Format("%d", m_lfFont.lfPitchAndFamily);
	m_IniReader.setKey(tmp,"lfPitchAndFamily","ͼ�����ֱ�ע����");
	m_IniReader.setKey(m_lfFont.lfFaceName,"lfFaceName","ͼ�����ֱ�ע����");

	tmp.Format("%d", GetRValue(m_lfColor));
	m_IniReader.setKey(tmp,"��ɫ����","ͼ�����ֱ�ע����");
	tmp.Format("%d", GetGValue(m_lfColor));
	m_IniReader.setKey(tmp,"��ɫ����","ͼ�����ֱ�ע����");
	tmp.Format("%d", GetBValue(m_lfColor));
	m_IniReader.setKey(tmp,"��ɫ����","ͼ�����ֱ�ע����");

	pItem = m_wndPropertyGrid.FindItem(_T("ͼ�����½Ǳ�ע����"));	
	m_IniReader.setKey(pItem->GetValue(),"ͼ�����½Ǳ�ע����","Settings");
	pItem = m_wndPropertyGrid.FindItem(_T("ͼ�����½Ǳ�ע����"));	
	m_IniReader.setKey(pItem->GetValue(),"ͼ�����½Ǳ�ע����","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("ͼ��������ߴ�"));
	CSize m_AoiSize = ((CXTPropertyGridItemSize *)pItem)->GetSize();
	CString m_AoiInfo;
	m_AoiInfo.Format("%d,%d", m_AoiSize.cx, m_AoiSize.cy);
	m_IniReader.setKey(m_AoiInfo,"ͼ��������ߴ�","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("ϵͳ����վ��"));	
	m_IniReader.setKey(pItem->GetValue(),"ϵͳ����վ��","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("�²���Ų�����ʽ"));	
	m_IniReader.setKey(pItem->GetValue(),"�²���Ų�����ʽ","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("δ��˱��������ӡ"));	
	m_IniReader.setKey(pItem->GetValue(),"δ��˱��������ӡ","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("���汨���ӡ�޶�ʱ��(����)"));
	m_IniReader.setKey(pItem->GetValue(),"���汨���ӡ�޶�ʱ��(����)","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("��ӡ�����Ͳ�����"));	
	m_IniReader.setKey(pItem->GetValue(),"��ӡ�����Ͳ�����","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("HISConnectCode"));	
	m_IniReader.setKey(pItem->GetValue(),"HISConnectCode","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("����¼��ӡ����ѡ��ģʽ"));	
	m_IniReader.setKey(pItem->GetValue(),"����¼��ӡ����ѡ��ģʽ","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("ϵͳ�����Զ����Զ��崰��"));	
	m_IniReader.setKey(pItem->GetValue(),"ϵͳ�����Զ����Զ��崰��","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("ϵͳ�����Զ��򿪱��������б�"));	
	m_IniReader.setKey(pItem->GetValue(),"ϵͳ�����Զ��򿪱��������б�","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("�ⴥ��COM�˿ں�"));	
	m_IniReader.setKey(pItem->GetValue(),"�ⴥ��COM�˿ں�","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("ͼ��ɼ���������"));	
	m_IniReader.setKey(pItem->GetValue(),"ͼ��ɼ���������","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("���浥��ϸʱ��ģʽ"));	
	m_IniReader.setKey(pItem->GetValue(),"���浥��ϸʱ��ģʽ","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("ϵͳ�Զ������ҵı���"));	
	m_IniReader.setKey(pItem->GetValue(),"ϵͳ�Զ������ҵı���","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("װ����������"));	
	m_IniReader.setKey(pItem->GetValue(),"װ����������","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("װ����Ƶ����"));	
	m_IniReader.setKey(pItem->GetValue(),"װ����Ƶ����","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("�½���¼������Ϣ����¼�뽹��"));	
	m_IniReader.setKey(pItem->GetValue(),"�½���¼������Ϣ����¼�뽹��","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("���浥�����Ϣ��������"));	
	m_IniReader.setKey(pItem->GetValue(),"���浥�����Ϣ��������","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("�Զ�ˢ�¼�¼�б�"));	
	m_IniReader.setKey(pItem->GetValue(),"�Զ�ˢ�¼�¼�б�","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("������Ƭ˵��"));	
	m_IniReader.setKey(pItem->GetValue(),"������Ƭ˵��","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("������Ƭ˵��"));	
	m_IniReader.setKey(pItem->GetValue(),"������Ƭ˵��","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("��վ�㵱ǰʱ��ģʽ"));	
	m_IniReader.setKey(pItem->GetValue(),"��վ�㵱ǰʱ��ģʽ","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("��Ƭ����Ŵ�ӡ����ģʽ"));	
	m_IniReader.setKey(pItem->GetValue(),"��Ƭ����Ŵ�ӡ����ģʽ","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("����ƿ��Ҵ���"));	
	m_IniReader.setKey(pItem->GetValue(),"����ƿ��Ҵ���","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("���������ʾ��¼��Ӧͼ����"));	
	m_IniReader.setKey(pItem->GetValue(),"���������ʾ��¼��Ӧͼ����","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("�ύʱ�ش�����ͼ������"));	
	m_IniReader.setKey(pItem->GetValue(),"�ύʱ�ش�����ͼ������","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("�����ʼ���"));	
	m_IniReader.setKey(pItem->GetValue(),"�����ʼ���","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("�Ա�Ĭ��ֵ"));	
	m_IniReader.setKey(pItem->GetValue(),"�Ա�Ĭ��ֵ","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("��ǩ��ӡֽ����"));	
	m_IniReader.setKey(pItem->GetValue(),"��ǩ��ӡֽ����","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("��״ϸ��"));	
	m_IniReader.setKey(pItem->GetValue(),"��״ϸ��","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("����ϸ��"));	
	m_IniReader.setKey(pItem->GetValue(),"����ϸ��","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("����ϸ��"));	
	m_IniReader.setKey(pItem->GetValue(),"����ϸ��","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("�γ��Ⱦ"));	
	m_IniReader.setKey(pItem->GetValue(),"�γ��Ⱦ","Settings");
	
	pItem = m_wndPropertyGrid.FindItem(_T("ù����Ⱦ"));	
	m_IniReader.setKey(pItem->GetValue(),"ù����Ⱦ","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("�������Ⱦ"));	
	m_IniReader.setKey(pItem->GetValue(),"�������Ⱦ","Settings");
	
	pItem = m_wndPropertyGrid.FindItem(_T("��˾���Ⱦ"));	
	m_IniReader.setKey(pItem->GetValue(),"��˾���Ⱦ","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("���߾���Ⱦ"));	
	m_IniReader.setKey(pItem->GetValue(),"���߾���Ⱦ","Settings");
	
	pItem = m_wndPropertyGrid.FindItem(_T("HPV��Ⱦ"));	
	m_IniReader.setKey(pItem->GetValue(),"HPV��Ⱦ","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("�������Ⱦ"));	
	m_IniReader.setKey(pItem->GetValue(),"�������Ⱦ","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("������ͷ״������Ⱦ"));	
	m_IniReader.setKey(pItem->GetValue(),"������ͷ״������Ⱦ","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("ȡ��ҽ��ͬ��д��"));	
	m_IniReader.setKey(pItem->GetValue(),"ȡ��ҽ��ͬ��д��","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("��Ƭ��Ϣ��ʾδ�˶���Ƭ��¼"));	
	m_IniReader.setKey(pItem->GetValue(),"��Ƭ��Ϣ��ʾδ�˶���Ƭ��¼","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("������Ƭ����վ������Ⱦɫ"));	
	m_IniReader.setKey(pItem->GetValue(),"������Ƭ����վ������Ⱦɫ","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("�ղ���д�����"));	
	m_IniReader.setKey(pItem->GetValue(),"�ղ���д�����","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("���䱨���Զ�����ʱ��"));	
	m_IniReader.setKey(pItem->GetValue(),"���䱨���Զ�����ʱ��","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("δ��ϲ�������Ԥ���ʹ�ӡ"));	
	m_IniReader.setKey(pItem->GetValue(),"δ��ϲ�������Ԥ���ʹ�ӡ","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("��ӡͼ���ɫƫ��"));	
	m_IniReader.setKey(pItem->GetValue(),"��ӡͼ���ɫƫ��","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("��ӡͼ����ɫƫ��"));	
	m_IniReader.setKey(pItem->GetValue(),"��ӡͼ����ɫƫ��","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("��ӡͼ����ɫƫ��"));	
	m_IniReader.setKey(pItem->GetValue(),"��ӡͼ����ɫƫ��","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("���μ���¼�������Ա����"));	
	m_IniReader.setKey(pItem->GetValue(),"���μ���¼�������Ա����","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("ͼ�����վ�ļ���"));	
	m_IniReader.setKey(pItem->GetValue(),"ͼ�����վ�ļ���","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("�ؼ칤��վҽ���б���ʾ���շ�δִ�еļ�¼"));	
	m_IniReader.setKey(pItem->GetValue(),"�ؼ칤��վҽ���б���ʾ���շ�δִ�еļ�¼","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("�ҵı��������ʽ"));	
	m_IniReader.setKey(pItem->GetValue(),"�ҵı��������ʽ","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("�����ӡר��Ȩ��"));	
	m_IniReader.setKey(pItem->GetValue(),"�����ӡר��Ȩ��","Settings");

	//add
	pItem = m_wndPropertyGrid.FindItem(_T("�������ר��Ȩ��"));
	m_IniReader.setKey(pItem->GetValue(),"�������ר��Ȩ��","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("��˲�����"));	
	m_IniReader.setKey(pItem->GetValue(),"��˲�����","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("���浥ͼ�����"));	
	m_IniReader.setKey(pItem->GetValue(),"���浥ͼ�����","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("���浥PDF���"));	
	m_IniReader.setKey(pItem->GetValue(),"���浥PDF���","Settings");
	

	pItem = m_wndPropertyGrid.FindItem(_T("Ԥ����ʾ"));
	m_IniReader.setKey(pItem->GetValue(),"Ԥ����ʾ","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("�Ƿ����ö�����"));
	m_IniReader.setKey(pItem->GetValue(),"�Ƿ����ö�����","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("ȡ�ļ�¼ֻ��ȡ��ҽʦ�ͼ�¼�˿����޸�"));
	m_IniReader.setKey(pItem->GetValue(),"ȡ�ļ�¼ֻ��ȡ��ҽʦ�ͼ�¼�˿����޸�","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("������Ų����л�ȥ����������һλ"));
	m_IniReader.setKey(pItem->GetValue(),"������Ų����л�ȥ����������һλ","Settings");

	CXTResizeDialog::OnOK();

}
