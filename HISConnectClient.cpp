//---------------------------------------------------------------------------


#include "stdafx.h"

#include "HISConnectClient.h"
#include "Markup.h"
#include "HISConnectData.h"


/////////////////
// Status:


// 1 -- 已预约
// 2 -- 取消预约
// 3 -- 已报到
// 4 -- 已检查
// 5 -- 已报告(未审核)
// 6 -- 已报告(已审核)
// 7 -- 报告作废
// 8 -- 保留
// 9 -- 保留


//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////

CHISConnectClient::CHISConnectClient()
{
    m_hHISConnectDll = NULL;
    m_sXMLDoc = "";
	m_sSystemCode = "";
}


CHISConnectClient::~CHISConnectClient()
{

}

void CHISConnectClient::LoadClient(const char *szFileName)
{
    char szHISConnectDll[MAX_PATH + 1], *p;

    memset(szHISConnectDll, 0, MAX_PATH + 1);

    if (m_hHISConnectDll != NULL)
    {
        FreeLibrary(m_hHISConnectDll);
        m_hHISConnectDll = NULL;
    }

    SetPDFReportPath_Dll = NULL;
    Initialize_Dll = NULL;
    SetOwnerWnd_Dll = NULL;
	NotifyExit_Dll = NULL;
    SetSenderInfo_Dll       = NULL;
    SetShowSubmitResultMessage_Dll = NULL;
    ReadMedicalOrder_Dll = NULL;
    UpdateMedicalOrder_Dll = NULL;
    NewPDFReportTask_Dll   = NULL;
	ShowMedicalOrder_Dll	= NULL;

    if (strlen(szFileName) == 0)
    {
        GetModuleFileName(NULL, szHISConnectDll, MAX_PATH);

        p = strrchr(szHISConnectDll, '\\');
        *(p+1) = '\0';
        strcat(szHISConnectDll, "HISConnectClient.dll");
    }
    else
    {
        strcpy(szHISConnectDll, szFileName);
    }


    m_hHISConnectDll = LoadLibrary(szHISConnectDll);

    if (m_hHISConnectDll)
    {
    	SetPDFReportPath_Dll = (void (*)(const char *)) GetProcAddress(m_hHISConnectDll,"SetPDFReportPath");
        Initialize_Dll = (void (*)(int , const char *, const char *)) GetProcAddress(m_hHISConnectDll, "Initialize");
		NotifyExit_Dll = (void (*)(void)) GetProcAddress(m_hHISConnectDll, "NotifyExit");
        SetOwnerWnd_Dll = (void (*)(HWND)) GetProcAddress(m_hHISConnectDll, "SetOwnerWnd");
        SetSenderInfo_Dll = (void (*) (const char *,const char *)) GetProcAddress(m_hHISConnectDll, "SetSenderInfo");
        SetShowSubmitResultMessage_Dll = (void (*)(bool bShow)) GetProcAddress(m_hHISConnectDll, "SetShowSubmitResultMessage");
        ReadMedicalOrder_Dll = (const char *(* )()) GetProcAddress(m_hHISConnectDll, "ReadMedicalOrder");
        UpdateMedicalOrder_Dll = (BOOL (*)(const char *, const char *)) GetProcAddress(m_hHISConnectDll, "UpdateMedicalOrder");
        NewPDFReportTask_Dll	= (void (*)(const char *)) GetProcAddress(m_hHISConnectDll, "NewPDFReportTask");
		ShowMedicalOrder_Dll	= (void (*)(const char *, const char *)) GetProcAddress(m_hHISConnectDll, "ShowMedicalOrder");
    }
}


void CHISConnectClient::Initialize(int HISCode, const char *szModality, const char *szEndPoint, const char *szSystemCode)
{
    if (Initialize_Dll)
		Initialize_Dll(HISCode, szModality, szEndPoint);
        
	m_sSystemCode = szSystemCode;

    return;
}


void CHISConnectClient::SetOwnerWnd(HWND hWnd)
{
    if (SetOwnerWnd_Dll)
    	SetOwnerWnd_Dll(hWnd);

    return;
}

void CHISConnectClient::NotifyExit(void)
{
	if (NotifyExit_Dll)
		NotifyExit_Dll();

	return;
}


void CHISConnectClient::SetSenderInfo(const char *szUserId, const char *szUserName)
{
    if (SetSenderInfo_Dll)
        SetSenderInfo_Dll(szUserId, szUserName);

    return;
}

void CHISConnectClient::SetShowSubmitResultMessage(bool bShow)
{
    if (SetShowSubmitResultMessage_Dll)
        SetShowSubmitResultMessage_Dll(bShow);

    return;
}

void CHISConnectClient::SetPDFReportPath(const char *szPDFReportPath)
{
	if (SetPDFReportPath_Dll)
    	SetPDFReportPath_Dll(szPDFReportPath);

    return;
}

const char * CHISConnectClient::ReadMedicalOrder(void)
{
    if (ReadMedicalOrder_Dll)
        return ReadMedicalOrder_Dll();

    return "";
}


BOOL CHISConnectClient::UpdateMedicalOrder(const char *orderStatus, const char *xmlDoc)
{
    if (UpdateMedicalOrder_Dll)
        return UpdateMedicalOrder_Dll(orderStatus, xmlDoc);

    return FALSE;
}


void CHISConnectClient::NewPDFReportTask(const char *szFileName)
{
    if (NewPDFReportTask_Dll)
    	NewPDFReportTask_Dll(szFileName);

    return;
}


void CHISConnectClient::ShowMedicalOrder(const char *szOrderNo, const char *szOrderType)
{
	if (ShowMedicalOrder_Dll)
		ShowMedicalOrder_Dll(szOrderNo, szOrderType);

	return;
}


const char * CHISConnectClient::GenerateXMLDocFromDataset(SACommand &cmd)
{
	CHISConnectData resultObj;
	CString sAgeUnit, sDtDate, sDtTime;
	CString sAnalysis, sRysj(""), sGjsj("");
	SADateTime dtReport;
	int nYear,nMonth, nDay;
	int nHour, nMinute;
	char szBuf[60];

	m_sXMLDoc = "";

	resultObj.Clear();

	resultObj.SetSystemCode(m_sSystemCode);

	resultObj.m_sOrderID	= cmd.Field("sqh").asString();

	resultObj.m_sOrderNo	= resultObj.m_sOrderID;

	resultObj.m_sOrderType  = cmd.Field("OrderType").asString();
	resultObj.m_sAccNo		= cmd.Field("medicalno").asString();
	resultObj.m_sStudyID	= cmd.Field("medicalno").asString(); 
	resultObj.m_sStudyUID	= cmd.Field("StudyUID").asString();
	resultObj.m_sSeriesUID	= cmd.Field("SeriesUID").asString();
	resultObj.m_sName		= cmd.Field("xm").asString();
	resultObj.m_sSex		= cmd.Field("xb").asString();
	resultObj.m_nAge		= cmd.Field("nl").asShort();

	sAgeUnit				= cmd.Field("nldw").asString();
	sAgeUnit.TrimLeft();
	sAgeUnit.TrimRight();

	if (sAgeUnit == "岁")
		resultObj.m_sAgeUnit = "Y";
	else if (sAgeUnit == "月")
		resultObj.m_sAgeUnit = "M";
	else if (sAgeUnit == "天")
		resultObj.m_sAgeUnit = "D";
	else if (sAgeUnit == "时")
		resultObj.m_sAgeUnit = "H";
	else
		resultObj.m_sAgeUnit = "Y";

    sprintf(szBuf, "%03d%s", resultObj.m_nAge, resultObj.m_sAgeUnit);
    resultObj.m_sAgeWithUnit	= szBuf;
	
	resultObj.m_sDOB			= cmd.Field("csrq").asString();

	resultObj.m_sAddress		= cmd.Field("lxdz").asString();
	resultObj.m_sTelNo			= cmd.Field("lxdh").asString();

	resultObj.m_sPatientID		= cmd.Field("brid").asString();
	resultObj.m_sRefDept		= cmd.Field("sjks").asString();
	resultObj.m_sRefDoctor		= cmd.Field("sjys").asString();

	resultObj.m_sOrderItems		= cmd.Field("bbmc").asString();

	
	dtReport = cmd.Field("bgrq").asDateTime();
	sDtDate.Format("%d-%02d-%02d", dtReport.GetYear(), dtReport.GetMonth(), dtReport.GetDay());
	sDtTime.Format("%02d:%02d", dtReport.GetHour(), dtReport.GetMinute());
	resultObj.m_sReportDate		= sDtDate;
	resultObj.m_sReportTime		= sDtTime;

	resultObj.m_sStudyDate		= resultObj.m_sReportDate;
	resultObj.m_sStudyTime		= resultObj.m_sReportTime;


	sRysj						= cmd.Field("rysj").asString();
	sGjsj						= cmd.Field("gjsj").asString();

	sRysj.TrimLeft();
	sRysj.TrimRight();
	if (!sRysj.IsEmpty()) { 
		sAnalysis = "[肉眼所见]:\n";
		sAnalysis += sRysj;
	}

	sGjsj.TrimLeft();
	sGjsj.TrimRight();

	if (!sGjsj.IsEmpty()) {
		if (!sAnalysis.IsEmpty())
			sAnalysis += "\n";

		sAnalysis += "\n[光镜所见]:\n";
		sAnalysis += sGjsj;
	}

	resultObj.m_sAnalysis		= sAnalysis; //cmd.Field("rysj").asString();

	resultObj.m_sDiagnosis		= cmd.Field("zdyj").asString();
	resultObj.m_sDoctorName		= cmd.Field("zzys").asString();
	
	
	m_sXMLDoc = resultObj.EncodeToXMLDoc();

	return m_sXMLDoc;
}
