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


	CString m_FtpIp = m_IniReader.getKeyValue("FTP地址(IP)","FTP设置");
	if(m_FtpIp.IsEmpty())  m_FtpIp.Format("127.0.0.1");

	CString m_FtpUser = m_IniReader.getKeyValue("FTP用户","FTP设置");
	if(m_FtpUser.IsEmpty())  m_FtpUser.Format("sa");

	CString m_FtpPass = m_IniReader.getKeyValue("FTP口令","FTP设置");
	if(m_FtpPass.IsEmpty())  m_FtpPass.Format("sa");

	CString m_CaptureMode = m_IniReader.getKeyValue("采集模式","Settings");
	if(m_CaptureMode.IsEmpty())  m_CaptureMode.Format("VFW采集设备支持");

	CString m_PrintingMode = m_IniReader.getKeyValue("打印图像反差","Settings");
	if(m_PrintingMode.IsEmpty())  m_PrintingMode.Format("高反差");

	CString m_PrintingMode1 = m_IniReader.getKeyValue("打印图像比例","Settings");
	if(m_PrintingMode1.IsEmpty())  m_PrintingMode1.Format("等比例输出");

	CString m_SavingMode = m_IniReader.getKeyValue("内部存储格式","Settings");
	if(m_SavingMode.IsEmpty())  m_SavingMode.Format("BMP");

	CString m_EditingMode = m_IniReader.getKeyValue("字符窗口编辑模式","Settings");
	if(m_EditingMode.IsEmpty())  m_EditingMode.Format("覆盖");

	CString m_SjrqTimeMode = m_IniReader.getKeyValue("送检日期设置","Settings");
	if(m_SjrqTimeMode.IsEmpty())  m_SjrqTimeMode.Format("服务器当前时间");

	CString m_QcrqTimeMode = m_IniReader.getKeyValue("取材日期设置","Settings");
	if(m_QcrqTimeMode.IsEmpty())  m_QcrqTimeMode.Format("服务器当前时间");

	CString m_BgrqTimeMode = m_IniReader.getKeyValue("报告日期设置","Settings");
	if(m_BgrqTimeMode.IsEmpty())  m_BgrqTimeMode.Format("服务器当前时间");

	CString m_CzysMode = m_IniReader.getKeyValue("初诊医生自动记忆","Settings");
	if(m_CzysMode.IsEmpty())  m_CzysMode.Format("否");

	CString m_ZzysMode = m_IniReader.getKeyValue("主诊医生自动记忆","Settings");
	if(m_ZzysMode.IsEmpty())  m_ZzysMode.Format("否");

	CString m_ShysMode = m_IniReader.getKeyValue("审核医生自动取当前登录者","Settings");
	if(m_ShysMode.IsEmpty())  m_ShysMode.Format("否");

	CString m_QcjlrMode = m_IniReader.getKeyValue("取材记录人自动取当前登录者","Settings");
	if(m_QcjlrMode.IsEmpty())  m_QcjlrMode.Format("否");

	CString m_SjdwMode = m_IniReader.getKeyValue("送检单位自动记忆","Settings");
	if(m_SjdwMode.IsEmpty())  m_SjdwMode.Format("否");

	CString m_BgrqRefreshMode = m_IniReader.getKeyValue("自动更新报告日期","Settings");
	if(m_BgrqRefreshMode.IsEmpty())  m_BgrqRefreshMode.Format("否");

	CString m_Waiting = m_IniReader.getKeyValue("无操作系统等待时间(分钟)","Settings");
	if(m_Waiting.IsEmpty())  m_Waiting.Format("5");

	LOGFONT   m_lfFont;        // 图像文字标注字体
	COLORREF  m_lfColor;       // 图像文字标注颜色

    CString tmp;
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

	CString m_LeftBottom  = m_IniReader.getKeyValue("图像左下角标注文字","Settings");
	CString m_RightBottom = m_IniReader.getKeyValue("图像右下角标注文字","Settings");
	CString m_AoiInfo     = m_IniReader.getKeyValue("图像裁切区尺寸","Settings");
	if(m_AoiInfo.IsEmpty())  m_AoiInfo.Format("10,10");

	CString m_StationType = m_IniReader.getKeyValue("系统工作站点","Settings");
	if(m_StationType.IsEmpty())  m_StationType.Format("登记工作站");

	CString m_NewBlhType = m_IniReader.getKeyValue("新病理号产生方式","Settings");
	if(m_NewBlhType.IsEmpty())  m_NewBlhType.Format("最大病理号加1");

	CString m_PrintRestrict = m_IniReader.getKeyValue("未审核报告允许打印","Settings");
	if(m_PrintRestrict.IsEmpty())  m_PrintRestrict.Format("是");

	CString m_PrintTime = m_IniReader.getKeyValue("常规报告打印限定时间(分钟)","Settings");
	if(m_PrintTime.IsEmpty()) m_PrintTime.Format("0");

	CString m_HisCfg = m_IniReader.getKeyValue("HISConnectCode","Settings");
	if(m_HisCfg.IsEmpty())  m_HisCfg.Format("无");

	CString m_BggsGenMode = m_IniReader.getKeyValue("单记录打印报告选择模式","Settings");
	if(m_BggsGenMode.IsEmpty())  m_BggsGenMode.Format("自动记忆上一记录报告格式");

	CString m_SelfDefineWndAutoOpenMode = m_IniReader.getKeyValue("系统启动自动打开自定义窗口","Settings");
	if(m_SelfDefineWndAutoOpenMode.IsEmpty())  m_SelfDefineWndAutoOpenMode.Format("否");

	CString m_BgyqAutoOpenMode = m_IniReader.getKeyValue("系统启动自动打开报告延期列表","Settings");
	if(m_SelfDefineWndAutoOpenMode.IsEmpty())  m_BgyqAutoOpenMode.Format("否");

	CString m_ComPortMode = m_IniReader.getKeyValue("外触发COM端口号","Settings");
	if(m_ComPortMode.IsEmpty())  m_ComPortMode.Format("0");

	CString m_ImgZoomRate = m_IniReader.getKeyValue("图像采集放缩比例","Settings");
	if(m_ImgZoomRate.IsEmpty())  m_ImgZoomRate.Format("1");

	CString m_TimeResMode = m_IniReader.getKeyValue("报告单详细时间模式","Settings");
	if(m_TimeResMode.IsEmpty())  m_TimeResMode.Format("否");

	CString m_MyRptMode = m_IniReader.getKeyValue("系统自动检索我的报告","Settings");
	if(m_MyRptMode.IsEmpty())  m_MyRptMode.Format("否");

	CString m_LoadSoundTabMode = m_IniReader.getKeyValue("装载语音管理","Settings");
	if(m_LoadSoundTabMode.IsEmpty())  m_LoadSoundTabMode.Format("否");

	CString m_LoadVideoTabMode = m_IniReader.getKeyValue("装载视频管理","Settings");
	if(m_LoadVideoTabMode.IsEmpty())  m_LoadVideoTabMode.Format("否");
	
	CString m_MedPersonFocusMode = m_IniReader.getKeyValue("新建记录病人信息窗口录入焦点","Settings");
	if(m_MedPersonFocusMode.IsEmpty())  m_MedPersonFocusMode.Format("姓名");

	CString m_StringFormatMode = m_IniReader.getKeyValue("报告单诊断信息首行缩进","Settings");
	if(m_StringFormatMode.IsEmpty())  m_StringFormatMode.Format("否");

	CString m_AutoRefreshDBListMode = m_IniReader.getKeyValue("自动刷新记录列表","Settings");
	if(m_AutoRefreshDBListMode.IsEmpty())  m_AutoRefreshDBListMode.Format("否");

	CString m_NormalSliceMode = m_IniReader.getKeyValue("常规切片说明","Settings");
	if(m_NormalSliceMode.IsEmpty())  m_NormalSliceMode.Format("无");

	CString m_BdSliceMode = m_IniReader.getKeyValue("冰冻切片说明","Settings");
	if(m_BdSliceMode.IsEmpty())  m_BdSliceMode.Format("冰冻");

	CString m_CurrentTimeMode = m_IniReader.getKeyValue("本站点当前时间模式","Settings");
	if(m_CurrentTimeMode.IsEmpty())  m_CurrentTimeMode.Format("服务器时间");

	CString m_TmhMode = m_IniReader.getKeyValue("玻片条码号打印紧缩模式","Settings");
	if(m_TmhMode.IsEmpty())  m_TmhMode.Format("否");

	CString m_BlkNumber = m_IniReader.getKeyValue("病理科科室代码","Settings");
	if(m_BlkNumber.IsEmpty())  m_BlkNumber.Format("");

	CString m_ImgNumShowMode = m_IniReader.getKeyValue("病例浏览显示记录对应图像数","Settings");
	if(m_ImgNumShowMode.IsEmpty())  m_ImgNumShowMode.Format("是");

	CString m_TransImgMode = m_IniReader.getKeyValue("提交时回传病理图像数据","Settings");
	if(m_TransImgMode.IsEmpty())  m_TransImgMode.Format("是");

	CString m_PrintAndSendMode = m_IniReader.getKeyValue("打印即发送病理报告","Settings");
	if(m_PrintAndSendMode.IsEmpty())  m_PrintAndSendMode.Format("否");

	CString m_InitSubCode = m_IniReader.getKeyValue("蜡块初始序号","Settings");
	if(m_InitSubCode.IsEmpty())  m_InitSubCode.Format("");

	CString m_Default_Sex = m_IniReader.getKeyValue("性别默认值","Settings");
	if(m_Default_Sex.IsEmpty())  m_Default_Sex.Format("");

	CString m_BarcodePaperType = m_IniReader.getKeyValue("标签打印纸类型","Settings");
	if(m_BarcodePaperType.IsEmpty())  m_BarcodePaperType.Format("连续纸");

	CString m_lzxb = m_IniReader.getKeyValue("鳞状细胞","Settings");

	CString m_jgxb = m_IniReader.getKeyValue("颈管细胞","Settings");

	CString m_hsxb = m_IniReader.getKeyValue("化生细胞","Settings");

	CString m_dcgr = m_IniReader.getKeyValue("滴虫感染","Settings");

	CString m_mjgr = m_IniReader.getKeyValue("霉菌感染","Settings");

	CString m_nzjgr = m_IniReader.getKeyValue("念珠菌感染","Settings");

	CString m_qgjgr = m_IniReader.getKeyValue("球杆菌感染","Settings");

	CString m_fxjgr = m_IniReader.getKeyValue("放线菌感染","Settings");

	CString m_hpvgr = m_IniReader.getKeyValue("HPV感染","Settings");

	CString m_bzbdgr = m_IniReader.getKeyValue("疱疹病毒感染","Settings");

	CString m_lsrtzbdgr = m_IniReader.getKeyValue("类似乳头状病毒感染","Settings");

	CString m_QcysTbXr = m_IniReader.getKeyValue("取材医生同步写入","Settings");
	if(m_QcysTbXr.IsEmpty())  m_QcysTbXr.Format("无");

	CString m_QpxxMode = m_IniReader.getKeyValue("切片信息显示未核对切片记录","Settings");
	if(m_QpxxMode.IsEmpty())  m_QpxxMode.Format("是");

	CString m_AntiNullRecordMode = m_IniReader.getKeyValue("空病例写入防护","Settings");
	if(m_AntiNullRecordMode.IsEmpty())  m_AntiNullRecordMode.Format("打开");

	CString m_BcbgAutoLockMode = m_IniReader.getKeyValue("补充报告自动锁定时间","Settings");
	if(m_BcbgAutoLockMode.IsEmpty())  m_BcbgAutoLockMode.Format("72");

	CString m_ZdDyMode = m_IniReader.getKeyValue("未诊断病例不能预览和打印","Settings");
	if(m_ZdDyMode.IsEmpty())  m_ZdDyMode.Format("是");

	CString m_RedValue = m_IniReader.getKeyValue("打印图像红色偏差","Settings");
	if(m_RedValue.IsEmpty())  m_RedValue.Format("0");

	CString m_GreenValue = m_IniReader.getKeyValue("打印图像绿色偏差","Settings");
	if(m_GreenValue.IsEmpty())  m_GreenValue.Format("0");

	CString m_BlueValue = m_IniReader.getKeyValue("打印图像蓝色偏差","Settings");
	if(m_BlueValue.IsEmpty())  m_BlueValue.Format("0");

	CString m_LsjcMode = m_IniReader.getKeyValue("历次检查记录按姓名性别检索","Settings");
	if(m_LsjcMode.IsEmpty())  m_LsjcMode.Format("是");

	CString m_RecyclePath = m_IniReader.getKeyValue("图像回收站文件夹","Settings");
	if(m_RecyclePath.IsEmpty())  m_RecyclePath.Format("");

	CString m_TjyzMode = m_IniReader.getKeyValue("特检工作站医嘱列表显示已收费未执行的记录","Settings");
	if(m_TjyzMode.IsEmpty())  m_TjyzMode.Format("否");

	CString m_MyReportQueryMode = m_IniReader.getKeyValue("我的报告检索方式","Settings");
	if(m_MyReportQueryMode.IsEmpty())  m_MyReportQueryMode.Format("模糊");

	CString m_AuthorPrintOnlyMode = m_IniReader.getKeyValue("报告打印专属权限","Settings");
	if(m_AuthorPrintOnlyMode.IsEmpty())  m_AuthorPrintOnlyMode.Format("关闭");
	
	//add
	CString m_AuthorCheckOnlyMode = m_IniReader.getKeyValue("报告审核专属权限","Settings");
	if(m_AuthorCheckOnlyMode.IsEmpty()) m_AuthorCheckOnlyMode.Format("关闭");

	CString m_CheckAndSendMode = m_IniReader.getKeyValue("审核并发送","Settings");
	if(m_CheckAndSendMode.IsEmpty())  m_CheckAndSendMode.Format("否");

	CString m_Export2ImgMode = m_IniReader.getKeyValue("报告单图像输出","Settings");
	if(m_Export2ImgMode.IsEmpty())  m_Export2ImgMode.Format("否");

	CString m_Export2PdfMode = m_IniReader.getKeyValue("报告单PDF输出","Settings");
	if(m_Export2PdfMode.IsEmpty())  m_Export2PdfMode.Format("否");

	CString m_PreviewType = m_IniReader.getKeyValue("预览显示","Settings");
	if(m_PreviewType.IsEmpty()) m_PreviewType.Format("报告浏览");

	CString m_IsUsingCard = m_IniReader.getKeyValue("是否启用读卡器","Settings");
	if(m_IsUsingCard.IsEmpty()) m_IsUsingCard.Format("否");

	//包埋盒打印
	CString m_IsEmbedBoxValid = m_IniReader.getKeyValue("是否启用包埋打印","Settings");
	if(m_IsEmbedBoxValid.IsEmpty()) m_IsEmbedBoxValid.Format("否");

	//包埋盒打印文件路径
	CString m_EmbedBoxFilePath = m_IniReader.getKeyValue("包埋打印文件路径","Settings");
	
	//包埋打印格式代码
	CString m_EmbedBoxFmtCode = m_IniReader.getKeyValue("包埋打印格式代码","Settings");

	
	//玻片打印
	CString m_IsSlidePrintValid = m_IniReader.getKeyValue("是否启用玻片打印","Settings");
	if(m_IsSlidePrintValid.IsEmpty()) m_IsSlidePrintValid.Format("否");

	//玻片打印文件路径
	CString m_SlidePrintFilePath = m_IniReader.getKeyValue("玻片打印文件路径","Settings");
	
	//玻片印格式代码
	CString m_SlidePrintFmtCode = m_IniReader.getKeyValue("玻片打印格式代码","Settings");


	CString m_MaterialMode = m_IniReader.getKeyValue("取材记录只有取材医师和记录人可以修改","Settings");
	if(m_MaterialMode.IsEmpty()) m_MaterialMode.Format("否");

	CString m_BlhMode = m_IniReader.getKeyValue("连续编号病例切换去掉病理号最后一位","Settings");
	if(m_BlhMode.IsEmpty()) m_BlhMode.Format("否");

	CString m_CantainTsrs = m_IniReader.getKeyValue("常规制片工作站含特殊染色","Settings");
	if(m_CantainTsrs.IsEmpty()) m_CantainTsrs.Format("否");
	
	if ( m_wndPropertyGrid.Create( rc, this, IDC_PROPERTY_GRID ) )
	{
		XT_LOGFONT lf;
		xtAfxData.font.GetLogFont( &lf );
		m_wndPropertyGrid.ShowHelp(FALSE);

		// create document settings category.

		CXTPropertyGridItem* pRuntime = m_wndPropertyGrid.AddCategory(_T("Settings"));

		CXTPropertyGridItem* pItemHisCfg = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("HISConnectCode"), m_HisCfg));
		
		CXTPropertyGridItem* pItemBlkNumber = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("病理科科室代码"), m_BlkNumber));
		pItemBlkNumber->SetFlags(pgiHasEdit);

		CXTPropertyGridItem* pItemStationType = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("系统工作站点"),m_StationType));
		
		pList = pItemStationType->GetConstraints();
		pList->AddConstraint(_T("登记工作站"));
		pList->AddConstraint(_T("取材工作站"));
		pList->AddConstraint(_T("诊断工作站"));
		pList->AddConstraint(_T("制片工作站"));
		pList->AddConstraint(_T("细胞学工作站"));
		pList->AddConstraint(_T("常规切片工作站"));
		pList->AddConstraint(_T("特检工作站"));
		pList->AddConstraint(_T("细胞学制片工作站"));
		pItemStationType->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemCurrentTimeMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("本站点当前时间模式"), m_CurrentTimeMode));
		pList = pItemCurrentTimeMode->GetConstraints();
		pList->AddConstraint(_T("服务器时间"));
		pList->AddConstraint(_T("本地时间"));
		pItemCurrentTimeMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemLoadSoundTabMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("装载语音管理"), m_LoadSoundTabMode));
		pList = pItemLoadSoundTabMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemLoadSoundTabMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemLoadVideoTabMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("装载视频管理"), m_LoadVideoTabMode));
		pList = pItemLoadVideoTabMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemLoadVideoTabMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemCaptureMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("采集模式"), m_CaptureMode));
		//CXTItemConstraints* pList = pItemCaptureMode->GetConstraints();
		pList = pItemCaptureMode->GetConstraints();
		pList->AddConstraint(_T("VFW采集设备支持"));
		pList->AddConstraint(_T("TWAIN采集设备支持"));
		pList->AddConstraint(_T("Ok图像采集卡"));
		pList->AddConstraint(_T("DH-HVxxxxU系列采集卡"));
		pList->AddConstraint(_T("DirectShow采集设备支持"));
		pList->AddConstraint(_T("MVC3000采集卡"));
		pList->AddConstraint(_T("微软DSHOW设备"));
		pList->AddConstraint(_T("Nikon设备支持"));
		pList->AddConstraint(_T("Canon S3/S5 IS"));

		pItemCaptureMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemSavingMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("内部存储格式"), m_SavingMode));
		pList = pItemSavingMode->GetConstraints();
		pList->AddConstraint(_T("BMP"));
		pList->AddConstraint(_T("JPEG"));
		pList->AddConstraint(_T("GTiff"));
		pItemSavingMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemImgZoomRate = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("图像采集放缩比例"), m_ImgZoomRate));
		pItemImgZoomRate->SetFlags(pgiHasEdit);

		CXTPropertyGridItem* pItemComPortMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("外触发COM端口号"), m_ComPortMode));
		pItemComPortMode->SetFlags(pgiHasEdit);

		CXTPropertyGridItem* pItemFont = pRuntime->AddChildItem(new CCustomItemFont(_T("图像文字标注字体"), m_lfFont, m_lfColor));
		
		CXTPropertyGridItem* pItemLeftBottom = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("图像左下角标注文字"), m_LeftBottom));
		pItemLeftBottom->SetFlags(pgiHasEdit);
		
		CXTPropertyGridItem* pItemRightBottom = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("图像右下角标注文字"), m_RightBottom));
		pItemRightBottom->SetFlags(pgiHasEdit);
		
		CSize aoisize;
		aoisize.cx = atoi(m_AoiInfo.Left(m_AoiInfo.Find(',')));;
		m_AoiInfo = m_AoiInfo.Right(m_AoiInfo.GetLength() - m_AoiInfo.Find(',') - 1);
		aoisize.cy = atoi(m_AoiInfo);
		
        CXTPropertyGridItem* pItemAoiSize = pRuntime->AddChildItem(new CXTPropertyGridItemSize(_T("图像裁切区尺寸"), aoisize));

		CXTPropertyGridItem* pItemRecyclePath = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("图像回收站文件夹"), m_RecyclePath));
		pItemRecyclePath->SetFlags(pgiHasEdit);

		CXTPropertyGridItem* pItemMedPersonFocusMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("新建记录病人信息窗口录入焦点"),m_MedPersonFocusMode));
		pList = pItemMedPersonFocusMode->GetConstraints();
		pList->AddConstraint(_T("姓名"));
		pList->AddConstraint(_T("HIS识别码"));
		pList->AddConstraint(_T("病人ID"));
		pList->AddConstraint(_T("申请号"));
		pList->AddConstraint(_T("住院号"));
		pItemMedPersonFocusMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemAntiNullRecordMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("空病例写入防护"), m_AntiNullRecordMode));
		pList = pItemAntiNullRecordMode->GetConstraints();
		pList->AddConstraint(_T("打开"));
		pList->AddConstraint(_T("关闭"));
		pItemAntiNullRecordMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemNewBlhGenMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("新病理号产生方式"), m_NewBlhType));
		pList = pItemNewBlhGenMode->GetConstraints();
		pList->AddConstraint(_T("最大病理号加1"));
		pList->AddConstraint(_T("最新病理号加1"));
		pList->AddConstraint(_T("最新病理号去掉最后一位加1"));
		pItemNewBlhGenMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemEditingMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("字符窗口编辑模式"),m_EditingMode));
		pList = pItemEditingMode->GetConstraints();
		pList->AddConstraint(_T("覆盖"));
		pList->AddConstraint(_T("插入"));
		pItemEditingMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemLsjcMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("历次检查记录按姓名性别检索"), m_LsjcMode));
		pList = pItemLsjcMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemLsjcMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemSjdwMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("送检单位自动记忆"), m_SjdwMode));
		pList = pItemSjdwMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemSjdwMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemSjrqTimeMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("送检日期设置"), m_SjrqTimeMode));
		pList = pItemSjrqTimeMode->GetConstraints();
		pList->AddConstraint(_T("服务器当前时间"));
		pList->AddConstraint(_T("上一条记录时间"));
		pItemSjrqTimeMode->SetFlags(pgiHasComboButton);
		
		CXTPropertyGridItem* pItemQcrqTimeMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("取材日期设置"), m_QcrqTimeMode));
		pList = pItemQcrqTimeMode->GetConstraints();
		pList->AddConstraint(_T("服务器当前时间"));
		pList->AddConstraint(_T("上一条记录时间"));
		pItemQcrqTimeMode->SetFlags(pgiHasComboButton);
		
		CXTPropertyGridItem* pItemBgrqTimeMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("报告日期设置"), m_BgrqTimeMode));
		pList = pItemBgrqTimeMode->GetConstraints();
		pList->AddConstraint(_T("服务器当前时间"));
		pList->AddConstraint(_T("上一条记录时间"));
		pItemBgrqTimeMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemBgrqRefreshMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("自动更新报告日期"), m_BgrqRefreshMode));
		pList = pItemBgrqRefreshMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemBgrqRefreshMode->SetFlags(pgiHasComboButton);
		
		CXTPropertyGridItem* pItemQcjrlMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("取材记录人自动取当前登录者"), m_QcjlrMode));
		pList = pItemQcjrlMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemQcjrlMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemQcysTbxr = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("取材医生同步写入"), m_QcysTbXr));
		pList = pItemQcysTbxr->GetConstraints();
		pList->AddConstraint(_T("无"));
		pList->AddConstraint(_T("初诊医生"));
		pList->AddConstraint(_T("主诊医生"));
		pItemQcysTbxr->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemCzysMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("初诊医生自动记忆"), m_CzysMode));
		pList = pItemCzysMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemCzysMode->SetFlags(pgiHasComboButton);
		
		CXTPropertyGridItem* pItemZzysMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("主诊医生自动记忆"), m_ZzysMode));
		pList = pItemZzysMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemZzysMode->SetFlags(pgiHasComboButton);
		
		CXTPropertyGridItem* pItemShysMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("审核医生自动取当前登录者"), m_ShysMode));
		pList = pItemShysMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemShysMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemAutoRefreshDBListMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("自动刷新记录列表"), m_AutoRefreshDBListMode));
		pList = pItemAutoRefreshDBListMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemAutoRefreshDBListMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemMyRptMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("系统自动检索我的报告"), m_MyRptMode));
		pList = pItemMyRptMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemMyRptMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemMyReportQueryMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("我的报告检索方式"), m_MyReportQueryMode));
		pList = pItemMyReportQueryMode->GetConstraints();
		pList->AddConstraint(_T("模糊"));
		pList->AddConstraint(_T("精确"));
		pItemMyReportQueryMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemInitSubCode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("蜡块初始序号"), m_InitSubCode));
		pItemInitSubCode->SetFlags(pgiHasEdit);

		CXTPropertyGridItem* pItemNormalSliceMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("常规切片说明"),m_NormalSliceMode));
		pList = pItemNormalSliceMode->GetConstraints();
		pList->AddConstraint(_T("无"));
		pList->AddConstraint(_T("取材部位"));
		pList->AddConstraint(_T("HE"));
		pItemNormalSliceMode->SetFlags(pgiHasComboButton);
		
		CXTPropertyGridItem* pItemBdSliceMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("冰冻切片说明"),m_BdSliceMode));
		pList = pItemBdSliceMode->GetConstraints();
		pList->AddConstraint(_T("冰冻"));
		pList->AddConstraint(_T("“冰”+冰冻号"));
		pList->AddConstraint(_T("冰冻号"));
		pItemBdSliceMode->SetFlags(pgiHasComboButton);
				
		CXTPropertyGridItem* pItemTmhMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("玻片条码号打印紧缩模式"), m_TmhMode));
		pList = pItemTmhMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemTmhMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemBarcodePaperType = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("标签打印纸类型"), m_BarcodePaperType));
		pList = pItemBarcodePaperType->GetConstraints();
		pList->AddConstraint(_T("连续纸"));
		pList->AddConstraint(_T("非连续纸"));
		pItemBarcodePaperType->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemQpxxMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("切片信息显示未核对切片记录"), m_QpxxMode));
		pList = pItemQpxxMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemQpxxMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemCantainTsrs = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("常规制片工作站含特殊染色"), m_CantainTsrs));
		pList = pItemCantainTsrs->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemCantainTsrs->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemTjyzMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("特检工作站医嘱列表显示已收费未执行的记录"), m_TjyzMode));
		pList = pItemTjyzMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemTjyzMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemPrintingMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("打印图像反差"), m_PrintingMode));
		pList = pItemPrintingMode->GetConstraints();
		pList->AddConstraint(_T("高反差"));
		pList->AddConstraint(_T("普通"));
		pList->AddConstraint(_T("屏幕"));
		pList->AddConstraint(_T("RGB调色"));
		pItemPrintingMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemRed = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("打印图像红色偏差"), m_RedValue));
		pItemRed->SetFlags(pgiHasEdit);

		CXTPropertyGridItem* pItemGreen = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("打印图像绿色偏差"), m_GreenValue));
		pItemGreen->SetFlags(pgiHasEdit);
		
		CXTPropertyGridItem* pItemBlue = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("打印图像蓝色偏差"), m_BlueValue));
		pItemBlue->SetFlags(pgiHasEdit);

		CXTPropertyGridItem* pItemPrintingMode1 = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("打印图像比例"), m_PrintingMode1));
		pList = pItemPrintingMode1->GetConstraints();
		pList->AddConstraint(_T("等比例输出"));
		pList->AddConstraint(_T("指定尺寸输出"));
		pItemPrintingMode1->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemImgNumShowMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("病例浏览显示记录对应图像数"), m_ImgNumShowMode));
		pList = pItemImgNumShowMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemImgNumShowMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemAuthorPrintOnlyMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("报告打印专属权限"), m_AuthorPrintOnlyMode));
		pList = pItemAuthorPrintOnlyMode->GetConstraints();
		pList->AddConstraint(_T("打开"));
		pList->AddConstraint(_T("关闭"));
		pItemAuthorPrintOnlyMode->SetFlags(pgiHasComboButton);
		pRuntime->Expand();
		
		//add
		CXTPropertyGridItem* pItemAuthorCheckOnlyMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("报告审核专属权限"),m_AuthorCheckOnlyMode));
		pList = pItemAuthorCheckOnlyMode->GetConstraints();
		pList->AddConstraint(_T("打开"));
		pList->AddConstraint(_T("关闭"));
		pItemAuthorCheckOnlyMode->SetFlags(pgiHasComboButton);
		pRuntime->Expand();	

		CXTPropertyGridItem* pItemExport2ImgMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("报告单图像输出"), m_Export2ImgMode));
		pList = pItemExport2ImgMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemExport2ImgMode->SetFlags(pgiHasComboButton);	
		
		CXTPropertyGridItem* pItemExport2PdfMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("报告单PDF输出"), m_Export2PdfMode));
		pList = pItemExport2PdfMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemExport2PdfMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemBcbgAutoLock = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("补充报告自动锁定时间"), m_BcbgAutoLockMode));
		pItemBcbgAutoLock->SetFlags(pgiHasEdit);

		CXTPropertyGridItem* pItemBggsGenMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("单记录打印报告选择模式"), m_BggsGenMode));
		pList = pItemBggsGenMode->GetConstraints();
		pList->AddConstraint(_T("自动记忆上一记录报告格式"));
		pList->AddConstraint(_T("自动按照报告图像数选择报告格式"));
		pItemBggsGenMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemStringFormatMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("报告单诊断信息首行缩进"), m_StringFormatMode));
		pList = pItemStringFormatMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemStringFormatMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemTimeResMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("报告单详细时间模式"), m_TimeResMode));
		pList = pItemTimeResMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemTimeResMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemZddyMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("未诊断病例不能预览和打印"), m_ZdDyMode));
		pList = pItemZddyMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemZddyMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemPrintRestrictMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("未审核报告允许打印"), m_PrintRestrict));
		pList = pItemPrintRestrictMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemPrintRestrictMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemPrintTime = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("常规报告打印限定时间(分钟)"), m_PrintTime));
		pItemBlkNumber->SetFlags(pgiHasEdit);

		CXTPropertyGridItem* pItemCheckAndSendMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("审核并发送"), m_CheckAndSendMode));
		pList = pItemCheckAndSendMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemCheckAndSendMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemPrintAndSendMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("打印即发送病理报告"), m_PrintAndSendMode));
		pList = pItemPrintAndSendMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemPrintAndSendMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemTransImgMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("提交时回传病理图像数据"), m_TransImgMode));
		pList = pItemTransImgMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemTransImgMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemSelfDefineWndAutoOpenMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("系统启动自动打开自定义窗口"), m_SelfDefineWndAutoOpenMode));
		pList = pItemSelfDefineWndAutoOpenMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		pItemSelfDefineWndAutoOpenMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemBgyqWndAutoOpenMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("系统启动自动打开报告延期列表"), m_BgyqAutoOpenMode));
		pList = pItemBgyqWndAutoOpenMode->GetConstraints();
		pList->AddConstraint(_T("是")); 
		pList->AddConstraint(_T("否"));
		pItemBgyqWndAutoOpenMode->SetFlags(pgiHasComboButton);

		CXTPropertyGridItem* pItemWaiting = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("无操作系统等待时间(分钟)"), m_Waiting));
		pItemWaiting->SetFlags(pgiHasEdit);

		CXTPropertyGridItem* pItemDefaultSex = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("性别默认值"), m_Default_Sex));
		pList = pItemDefaultSex->GetConstraints();
		pList->AddConstraint(_T("男"));
		pList->AddConstraint(_T("女"));
		pItemDefaultSex->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* pLzxb = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("鳞状细胞"), m_lzxb));
		pList = pLzxb->GetConstraints();
		pList->AddConstraint(_T("无"));
		pList->AddConstraint(_T("少量"));
		pList->AddConstraint(_T("未见"));
		pLzxb->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* pJgxb = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("颈管细胞"), m_jgxb));
		pList = pJgxb->GetConstraints();
		pList->AddConstraint(_T("无"));
		pList->AddConstraint(_T("少量"));
		pList->AddConstraint(_T("未见"));
		pJgxb->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* pHsxb = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("化生细胞"), m_hsxb));
		pList = pHsxb->GetConstraints();
		pList->AddConstraint(_T("无"));
		pList->AddConstraint(_T("少量"));
		pList->AddConstraint(_T("未见"));
		pHsxb->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* pdcgr = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("滴虫感染"), m_dcgr));
		pList = pdcgr->GetConstraints();
		pList->AddConstraint(_T("无"));
		pList->AddConstraint(_T("少量"));
		pList->AddConstraint(_T("未见"));
		pdcgr->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* pmjgr = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("霉菌感染"), m_mjgr));
		pList = pmjgr->GetConstraints();
		pList->AddConstraint(_T("无"));
		pList->AddConstraint(_T("少量"));
		pList->AddConstraint(_T("未见"));
		pmjgr->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* pnzjgr = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("念珠菌感染"), m_nzjgr));
		pList = pnzjgr->GetConstraints();
		pList->AddConstraint(_T("无"));
		pList->AddConstraint(_T("少量"));
		pList->AddConstraint(_T("未见"));
		pnzjgr->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* pqgjgr = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("球杆菌感染"), m_qgjgr));
		pList = pqgjgr->GetConstraints();
		pList->AddConstraint(_T("无"));
		pList->AddConstraint(_T("少量"));
		pList->AddConstraint(_T("未见"));
		pqgjgr->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* pfxjgr = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("放线菌感染"), m_fxjgr));
		pList = pfxjgr->GetConstraints();
		pList->AddConstraint(_T("无"));
		pList->AddConstraint(_T("少量"));
		pList->AddConstraint(_T("未见"));
		pfxjgr->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* phpvgr = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("HPV感染"), m_hpvgr));
		pList = phpvgr->GetConstraints();
		pList->AddConstraint(_T("无"));
		pList->AddConstraint(_T("少量"));
		pList->AddConstraint(_T("未见"));
		phpvgr->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* pbzbdgr = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("疱疹病毒感染"), m_bzbdgr));
		pList = pbzbdgr->GetConstraints();
		pList->AddConstraint(_T("无"));
		pList->AddConstraint(_T("少量"));
		pList->AddConstraint(_T("未见"));
		pbzbdgr->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* plsrtzbdgr = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("类似乳头状病毒感染"), m_lsrtzbdgr));
		pList = plsrtzbdgr->GetConstraints();
		pList->AddConstraint(_T("无"));
		pList->AddConstraint(_T("少量"));
		pList->AddConstraint(_T("未见"));
		plsrtzbdgr->SetFlags(pgiHasComboButton | pgiHasEdit);
		
		CXTPropertyGridItem* preViewType = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("预览显示"), m_PreviewType));
		pList = preViewType->GetConstraints();
		pList->AddConstraint(_T("报告浏览"));
		pList->AddConstraint(_T("打印预览"));
		preViewType->SetFlags(pgiHasComboButton | pgiHasEdit);
		
		CXTPropertyGridItem* isUsingCard = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("是否启用读卡器"), m_IsUsingCard));
		pList = isUsingCard->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		isUsingCard->SetFlags(pgiHasComboButton | pgiHasEdit);

		//包埋盒打印
		CXTPropertyGridItem* isEmbedBoxValid = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("是否启用包埋打印"), m_IsEmbedBoxValid));
		pList = isEmbedBoxValid->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		isEmbedBoxValid->SetFlags(pgiHasComboButton | pgiHasEdit);

		//包埋盒打印文件路径
		CXTPropertyGridItem* embedBoxFilePath = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("包埋打印文件路径"), m_EmbedBoxFilePath));
		embedBoxFilePath->SetFlags( pgiHasEdit);

		//包埋打印格式代码
		CXTPropertyGridItem* embedBoxFmtCode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("包埋打印格式代码"), m_EmbedBoxFmtCode));
		embedBoxFmtCode->SetFlags( pgiHasEdit);


		//玻片打印
		CXTPropertyGridItem* isSlidePrintValid = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("是否启用玻片打印"), m_IsSlidePrintValid));
		pList = isSlidePrintValid->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		isSlidePrintValid->SetFlags(pgiHasComboButton | pgiHasEdit);

		//玻片打印文件路径
		CXTPropertyGridItem* slidePrintFilePath = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("玻片打印文件路径"), m_SlidePrintFilePath));
		slidePrintFilePath->SetFlags( pgiHasEdit);

		//玻片打印格式代码
		CXTPropertyGridItem* slidePrintFmtCode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("玻片打印格式代码"), m_SlidePrintFmtCode));
		slidePrintFmtCode->SetFlags( pgiHasEdit);


		CXTPropertyGridItem* materialMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("取材记录只有取材医师和记录人可以修改"), m_MaterialMode));
		pList = materialMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
		materialMode->SetFlags(pgiHasComboButton | pgiHasEdit);

		CXTPropertyGridItem* blhMode = pRuntime->AddChildItem(new CXTPropertyGridItem(_T("连续编号病例切换去掉病理号最后一位"), m_BlhMode));
		pList = blhMode->GetConstraints();
		pList->AddConstraint(_T("是"));
		pList->AddConstraint(_T("否"));
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

	pItem = m_wndPropertyGrid.FindItem(_T("采集模式"));	
	m_IniReader.setKey(pItem->GetValue(),"采集模式","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("打印图像反差"));	
	m_IniReader.setKey(pItem->GetValue(),"打印图像反差","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("打印图像比例"));	
	m_IniReader.setKey(pItem->GetValue(),"打印图像比例","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("内部存储格式"));	
	m_IniReader.setKey(pItem->GetValue(),"内部存储格式","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("字符窗口编辑模式"));	
	m_IniReader.setKey(pItem->GetValue(),"字符窗口编辑模式","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("送检日期设置"));	
	m_IniReader.setKey(pItem->GetValue(),"送检日期设置","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("取材日期设置"));	
	m_IniReader.setKey(pItem->GetValue(),"取材日期设置","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("报告日期设置"));	
	m_IniReader.setKey(pItem->GetValue(),"报告日期设置","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("初诊医生自动记忆"));	
	m_IniReader.setKey(pItem->GetValue(),"初诊医生自动记忆","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("主诊医生自动记忆"));	
	m_IniReader.setKey(pItem->GetValue(),"主诊医生自动记忆","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("审核医生自动取当前登录者"));	
	m_IniReader.setKey(pItem->GetValue(),"审核医生自动取当前登录者","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("取材记录人自动取当前登录者"));	
	m_IniReader.setKey(pItem->GetValue(),"取材记录人自动取当前登录者","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("送检单位自动记忆"));	
	m_IniReader.setKey(pItem->GetValue(),"送检单位自动记忆","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("自动更新报告日期"));	
	m_IniReader.setKey(pItem->GetValue(),"自动更新报告日期","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("无操作系统等待时间(分钟)"));	
	m_IniReader.setKey(pItem->GetValue(),"无操作系统等待时间(分钟)","Settings");

	LOGFONT   m_lfFont;        // 图像文字标注字体
	COLORREF  m_lfColor;       // 图像文字标注颜色

	pItem = m_wndPropertyGrid.FindItem(_T("图像文字标注字体"));	
	((CCustomItemFont *)pItem)->GetFont(&m_lfFont);
	m_lfColor = ((CCustomItemFont *)pItem)->m_Color;

	CString tmp;
	tmp.Format("%d", m_lfFont.lfHeight);
	m_IniReader.setKey(tmp,"lfHeight","图像文字标注字体");
	tmp.Format("%d", m_lfFont.lfWidth);
	m_IniReader.setKey(tmp,"lfWidth","图像文字标注字体");
	tmp.Format("%d", m_lfFont.lfEscapement);
	m_IniReader.setKey(tmp,"lfEscapement","图像文字标注字体");
	tmp.Format("%d", m_lfFont.lfOrientation);
	m_IniReader.setKey(tmp,"lfOrientation","图像文字标注字体");
	tmp.Format("%d", m_lfFont.lfWeight);
	m_IniReader.setKey(tmp,"lfWeight","图像文字标注字体");
	tmp.Format("%d", m_lfFont.lfItalic);
	m_IniReader.setKey(tmp,"lfItalic","图像文字标注字体");
	tmp.Format("%d", m_lfFont.lfUnderline);
	m_IniReader.setKey(tmp,"lfUnderline","图像文字标注字体");
	tmp.Format("%d", m_lfFont.lfStrikeOut);
	m_IniReader.setKey(tmp,"lfStrikeOut","图像文字标注字体");
	tmp.Format("%d", m_lfFont.lfCharSet);
	m_IniReader.setKey(tmp,"lfCharSet","图像文字标注字体");
	tmp.Format("%d", m_lfFont.lfOutPrecision);
	m_IniReader.setKey(tmp,"lfOutPrecision","图像文字标注字体");
	tmp.Format("%d", m_lfFont.lfClipPrecision);
	m_IniReader.setKey(tmp,"lfClipPrecision","图像文字标注字体");
	tmp.Format("%d", m_lfFont.lfQuality);
	m_IniReader.setKey(tmp,"lfQuality","图像文字标注字体");
	tmp.Format("%d", m_lfFont.lfPitchAndFamily);
	m_IniReader.setKey(tmp,"lfPitchAndFamily","图像文字标注字体");
	m_IniReader.setKey(m_lfFont.lfFaceName,"lfFaceName","图像文字标注字体");

	tmp.Format("%d", GetRValue(m_lfColor));
	m_IniReader.setKey(tmp,"红色分量","图像文字标注字体");
	tmp.Format("%d", GetGValue(m_lfColor));
	m_IniReader.setKey(tmp,"绿色分量","图像文字标注字体");
	tmp.Format("%d", GetBValue(m_lfColor));
	m_IniReader.setKey(tmp,"蓝色分量","图像文字标注字体");

	pItem = m_wndPropertyGrid.FindItem(_T("图像左下角标注文字"));	
	m_IniReader.setKey(pItem->GetValue(),"图像左下角标注文字","Settings");
	pItem = m_wndPropertyGrid.FindItem(_T("图像右下角标注文字"));	
	m_IniReader.setKey(pItem->GetValue(),"图像右下角标注文字","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("图像裁切区尺寸"));
	CSize m_AoiSize = ((CXTPropertyGridItemSize *)pItem)->GetSize();
	CString m_AoiInfo;
	m_AoiInfo.Format("%d,%d", m_AoiSize.cx, m_AoiSize.cy);
	m_IniReader.setKey(m_AoiInfo,"图像裁切区尺寸","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("系统工作站点"));	
	m_IniReader.setKey(pItem->GetValue(),"系统工作站点","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("新病理号产生方式"));	
	m_IniReader.setKey(pItem->GetValue(),"新病理号产生方式","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("未审核报告允许打印"));	
	m_IniReader.setKey(pItem->GetValue(),"未审核报告允许打印","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("常规报告打印限定时间(分钟)"));
	m_IniReader.setKey(pItem->GetValue(),"常规报告打印限定时间(分钟)","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("打印即发送病理报告"));	
	m_IniReader.setKey(pItem->GetValue(),"打印即发送病理报告","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("HISConnectCode"));	
	m_IniReader.setKey(pItem->GetValue(),"HISConnectCode","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("单记录打印报告选择模式"));	
	m_IniReader.setKey(pItem->GetValue(),"单记录打印报告选择模式","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("系统启动自动打开自定义窗口"));	
	m_IniReader.setKey(pItem->GetValue(),"系统启动自动打开自定义窗口","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("系统启动自动打开报告延期列表"));	
	m_IniReader.setKey(pItem->GetValue(),"系统启动自动打开报告延期列表","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("外触发COM端口号"));	
	m_IniReader.setKey(pItem->GetValue(),"外触发COM端口号","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("图像采集放缩比例"));	
	m_IniReader.setKey(pItem->GetValue(),"图像采集放缩比例","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("报告单详细时间模式"));	
	m_IniReader.setKey(pItem->GetValue(),"报告单详细时间模式","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("系统自动检索我的报告"));	
	m_IniReader.setKey(pItem->GetValue(),"系统自动检索我的报告","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("装载语音管理"));	
	m_IniReader.setKey(pItem->GetValue(),"装载语音管理","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("装载视频管理"));	
	m_IniReader.setKey(pItem->GetValue(),"装载视频管理","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("新建记录病人信息窗口录入焦点"));	
	m_IniReader.setKey(pItem->GetValue(),"新建记录病人信息窗口录入焦点","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("报告单诊断信息首行缩进"));	
	m_IniReader.setKey(pItem->GetValue(),"报告单诊断信息首行缩进","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("自动刷新记录列表"));	
	m_IniReader.setKey(pItem->GetValue(),"自动刷新记录列表","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("常规切片说明"));	
	m_IniReader.setKey(pItem->GetValue(),"常规切片说明","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("冰冻切片说明"));	
	m_IniReader.setKey(pItem->GetValue(),"冰冻切片说明","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("本站点当前时间模式"));	
	m_IniReader.setKey(pItem->GetValue(),"本站点当前时间模式","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("玻片条码号打印紧缩模式"));	
	m_IniReader.setKey(pItem->GetValue(),"玻片条码号打印紧缩模式","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("病理科科室代码"));	
	m_IniReader.setKey(pItem->GetValue(),"病理科科室代码","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("病例浏览显示记录对应图像数"));	
	m_IniReader.setKey(pItem->GetValue(),"病例浏览显示记录对应图像数","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("提交时回传病理图像数据"));	
	m_IniReader.setKey(pItem->GetValue(),"提交时回传病理图像数据","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("蜡块初始序号"));	
	m_IniReader.setKey(pItem->GetValue(),"蜡块初始序号","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("性别默认值"));	
	m_IniReader.setKey(pItem->GetValue(),"性别默认值","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("标签打印纸类型"));	
	m_IniReader.setKey(pItem->GetValue(),"标签打印纸类型","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("鳞状细胞"));	
	m_IniReader.setKey(pItem->GetValue(),"鳞状细胞","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("颈管细胞"));	
	m_IniReader.setKey(pItem->GetValue(),"颈管细胞","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("化生细胞"));	
	m_IniReader.setKey(pItem->GetValue(),"化生细胞","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("滴虫感染"));	
	m_IniReader.setKey(pItem->GetValue(),"滴虫感染","Settings");
	
	pItem = m_wndPropertyGrid.FindItem(_T("霉菌感染"));	
	m_IniReader.setKey(pItem->GetValue(),"霉菌感染","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("念珠菌感染"));	
	m_IniReader.setKey(pItem->GetValue(),"念珠菌感染","Settings");
	
	pItem = m_wndPropertyGrid.FindItem(_T("球杆菌感染"));	
	m_IniReader.setKey(pItem->GetValue(),"球杆菌感染","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("放线菌感染"));	
	m_IniReader.setKey(pItem->GetValue(),"放线菌感染","Settings");
	
	pItem = m_wndPropertyGrid.FindItem(_T("HPV感染"));	
	m_IniReader.setKey(pItem->GetValue(),"HPV感染","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("疱疹病毒感染"));	
	m_IniReader.setKey(pItem->GetValue(),"疱疹病毒感染","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("类似乳头状病毒感染"));	
	m_IniReader.setKey(pItem->GetValue(),"类似乳头状病毒感染","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("取材医生同步写入"));	
	m_IniReader.setKey(pItem->GetValue(),"取材医生同步写入","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("切片信息显示未核对切片记录"));	
	m_IniReader.setKey(pItem->GetValue(),"切片信息显示未核对切片记录","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("常规制片工作站含特殊染色"));	
	m_IniReader.setKey(pItem->GetValue(),"常规制片工作站含特殊染色","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("空病例写入防护"));	
	m_IniReader.setKey(pItem->GetValue(),"空病例写入防护","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("补充报告自动锁定时间"));	
	m_IniReader.setKey(pItem->GetValue(),"补充报告自动锁定时间","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("未诊断病例不能预览和打印"));	
	m_IniReader.setKey(pItem->GetValue(),"未诊断病例不能预览和打印","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("打印图像红色偏差"));	
	m_IniReader.setKey(pItem->GetValue(),"打印图像红色偏差","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("打印图像绿色偏差"));	
	m_IniReader.setKey(pItem->GetValue(),"打印图像绿色偏差","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("打印图像蓝色偏差"));	
	m_IniReader.setKey(pItem->GetValue(),"打印图像蓝色偏差","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("历次检查记录按姓名性别检索"));	
	m_IniReader.setKey(pItem->GetValue(),"历次检查记录按姓名性别检索","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("图像回收站文件夹"));	
	m_IniReader.setKey(pItem->GetValue(),"图像回收站文件夹","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("特检工作站医嘱列表显示已收费未执行的记录"));	
	m_IniReader.setKey(pItem->GetValue(),"特检工作站医嘱列表显示已收费未执行的记录","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("我的报告检索方式"));	
	m_IniReader.setKey(pItem->GetValue(),"我的报告检索方式","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("报告打印专属权限"));	
	m_IniReader.setKey(pItem->GetValue(),"报告打印专属权限","Settings");

	//add
	pItem = m_wndPropertyGrid.FindItem(_T("报告审核专属权限"));
	m_IniReader.setKey(pItem->GetValue(),"报告审核专属权限","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("审核并发送"));	
	m_IniReader.setKey(pItem->GetValue(),"审核并发送","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("报告单图像输出"));	
	m_IniReader.setKey(pItem->GetValue(),"报告单图像输出","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("报告单PDF输出"));	
	m_IniReader.setKey(pItem->GetValue(),"报告单PDF输出","Settings");
	

	pItem = m_wndPropertyGrid.FindItem(_T("预览显示"));
	m_IniReader.setKey(pItem->GetValue(),"预览显示","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("是否启用读卡器"));
	m_IniReader.setKey(pItem->GetValue(),"是否启用读卡器","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("取材记录只有取材医师和记录人可以修改"));
	m_IniReader.setKey(pItem->GetValue(),"取材记录只有取材医师和记录人可以修改","Settings");

	pItem = m_wndPropertyGrid.FindItem(_T("连续编号病例切换去掉病理号最后一位"));
	m_IniReader.setKey(pItem->GetValue(),"连续编号病例切换去掉病理号最后一位","Settings");

	CXTResizeDialog::OnOK();

}
