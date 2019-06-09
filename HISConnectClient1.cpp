//---------------------------------------------------------------------------

#include "stdafx.h"
#pragma hdrstop
#include <stdio.h>

#include "HISConnectClient1.h"
#include "HISConnectData.h"
#include "Markup.h"

//---------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////

extern BOOL g_bDebugMode;
////////////////////////////////////////////////////////////////////////////////

extern CString g_sUserId,g_sUserName;


CHISConnectClient1::CHISConnectClient1()
{
	m_hHISConnectDll = NULL;
	m_sSystemCode = "";
	m_sSystemName = "";
	m_sUserID = "";
	m_sUserName = "";
	m_sHospitalID = "";
	m_sHospitalName = "";
	m_sXMLDoc = "";
}


CHISConnectClient1::~CHISConnectClient1()
{

}

void CHISConnectClient1::LoadClient(const char *szFileName)
{
	char szHISConnectDll[MAX_PATH + 1], *p;

	memset(szHISConnectDll, 0, MAX_PATH + 1);

	if (m_hHISConnectDll != NULL)
	{
		FreeLibrary(m_hHISConnectDll);
		m_hHISConnectDll = NULL;
	}

	Initialize_Dll = NULL;
	SetOwnerWnd_Dll = NULL;
	SetSenderInfo_Dll       = NULL;
	SetHospitalInfo_Dll     = NULL;
	SetExtraSQLWhereExp_Dll = NULL;
	SetShowSubmitResultMessage_Dll = NULL;
	GetMedicalOrder_Dll = NULL;
	UpdateMedicalOrder_Dll = NULL;


	ReadOrderImageFromUrl_Dll = NULL;
	NewReportUploadTask_Dll = NULL;
	ConvertJPEG2PDF_Dll = NULL;

	if (stricmp(szFileName,"Auto") == 0 || stricmp(szFileName, "$Auto") == 0) {
		GetModuleFileName(NULL, szHISConnectDll, MAX_PATH);

		p = strrchr(szHISConnectDll, '\\');
		*(p+1) = '\0';

#ifdef _WIN64
		strcat(szHISConnectDll, "hisconnectclient1_x64.dll");
#else
		strcat(szHISConnectDll, "hisconnectclient1.dll");
#endif

	}
	else if (strlen(szFileName) == 0)
	{
		GetModuleFileName(NULL, szHISConnectDll, MAX_PATH);

		p = strrchr(szHISConnectDll, '\\');
		*(p+1) = '\0';
		strcat(szHISConnectDll, "hisconnectclient1.dll");
	}
	else
	{
		strcpy(szHISConnectDll, szFileName);
	}

	m_hHISConnectDll = LoadLibrary(szHISConnectDll);

	if (m_hHISConnectDll)
	{
		Initialize_Dll = (void ( *)(int , char *, char *)) GetProcAddress(m_hHISConnectDll, "Initialize");
		SetOwnerWnd_Dll = (void ( *)(HWND)) GetProcAddress(m_hHISConnectDll, "SetOwnerWnd");
		NotifyExit_Dll = (void ( *)(void)) GetProcAddress(m_hHISConnectDll, "NotifyExit");
		SetSenderInfo_Dll = (void ( *) (const char *,const char *)) GetProcAddress(m_hHISConnectDll, "SetSenderInfo");
		SetHospitalInfo_Dll = (void ( *)(const char *, const char *)) GetProcAddress(m_hHISConnectDll, "SetHospitalInfo");
		SetShowSubmitResultMessage_Dll = (void ( *)(bool bShow)) GetProcAddress(m_hHISConnectDll, "SetShowSubmitResultMessage");
		GetMedicalOrder_Dll = (const char *( * )(void)) GetProcAddress(m_hHISConnectDll, "GetMedicalOrder");

		UpdateMedicalOrder_Dll = (BOOL ( *)(const char *, const char *)) GetProcAddress(m_hHISConnectDll, "UpdateMedicalOrder");


		ReadOrderImageFromUrl_Dll = (BOOL ( *) (const char *, unsigned char **, unsigned long &)) GetProcAddress(m_hHISConnectDll, "ReadOrderImageFromUrl");
		NewReportUploadTask_Dll = (BOOL ( *) (const char *)) GetProcAddress(m_hHISConnectDll, "NewReportUploadTask");

		ConvertJPEG2PDF_Dll = (BOOL ( *)(const char *, const char *, int, const char *, const char *, BOOL)) GetProcAddress(m_hHISConnectDll, "ConvertJPEG2PDF");
	}
}


void CHISConnectClient1::Initialize(int HISCode, char *szModality, char *szEndPoint, char *szSystemCode, char *szSystemName)
{
	if (Initialize_Dll)
		Initialize_Dll(HISCode, szModality, szEndPoint);

	m_sSystemCode = szSystemCode;
	m_sSystemName = szSystemName;

	return;
}


void CHISConnectClient1::SetOwnerWnd(HWND hWnd)
{
	if (SetOwnerWnd_Dll)
		SetOwnerWnd_Dll(hWnd);

	return;
}


void CHISConnectClient1::NotifyExit()
{
	if (NotifyExit_Dll)
		NotifyExit_Dll();
	return;
}



void CHISConnectClient1::SetSenderInfo(const char *szUserId, const char *szUserName)
{
	if (SetSenderInfo_Dll)
		SetSenderInfo_Dll(szUserId, szUserName);

	m_sUserID = szUserId;
	m_sUserName = szUserName;

	return;
}


void CHISConnectClient1::SetHospitalInfo(const char *szHospitalID, const char *szHospitalName)
{
	if (SetHospitalInfo_Dll)
		SetHospitalInfo_Dll(szHospitalID, szHospitalName);

	m_sHospitalID = szHospitalID;
	m_sHospitalName = szHospitalName;
	return;
}

void CHISConnectClient1::SetExtraSQLWhereExp(const char *szExp)
{
	if (SetExtraSQLWhereExp_Dll) {
		SetExtraSQLWhereExp_Dll(szExp);
	}
	return;
}


void CHISConnectClient1::SetShowSubmitResultMessage(bool bShow)
{
	if (SetShowSubmitResultMessage_Dll)
		SetShowSubmitResultMessage_Dll(bShow);

	return;
}


const char * CHISConnectClient1::ReadMedicalOrder()
{
	if (GetMedicalOrder_Dll) {
		return GetMedicalOrder_Dll();
	}

	return "";
}


BOOL CHISConnectClient1::UpdateMedicalOrder(const char *xmlDoc, const char *statusCode)
{
	BOOL bRet = FALSE;

	if (UpdateMedicalOrder_Dll) {
	
		bRet = UpdateMedicalOrder_Dll(xmlDoc, statusCode);
	}

	return bRet;
}



BOOL CHISConnectClient1::ReadOrderImageFromUrl(const char *szUrl, unsigned char **lpData, unsigned long &nDataLen)
{
	if (ReadOrderImageFromUrl_Dll)
		return ReadOrderImageFromUrl_Dll(szUrl, lpData, nDataLen);

	return FALSE;
}


BOOL CHISConnectClient1::NewReportUploadTask(const char *szXmlDoc)
{
	if (NewReportUploadTask_Dll)
		return NewReportUploadTask_Dll(szXmlDoc);

	return FALSE;
}


BOOL CHISConnectClient1::ConvertJPEG2PDF(const char *szPathName, const char *szJPEGTemplateName, int nImageCount, const char *szPaperSize, const char *szPDFFileName, BOOL bDeleteJPEG)
{
	if (ConvertJPEG2PDF_Dll)
		return ConvertJPEG2PDF_Dll(szPathName, szJPEGTemplateName, nImageCount, szPaperSize, szPDFFileName, bDeleteJPEG);

	return FALSE;
}


const char * CHISConnectClient1::GenerateXMLDocFromDataset(SACommand &cmd)
{
	CHISConnectData resultObj;
	CString sAgeUnit, sDtDate, sDtTime;
	CString sAnalysis, sRysj(""), sGjsj("");
	CString sValue(""),sGuid("");
	SADateTime dtReport;
	int nYear,nMonth, nDay;
	int nHour, nMinute;
	char szBuf[60];

	m_sXMLDoc = "";

	resultObj.Clear();

	resultObj.SetSystemCode(m_sSystemCode);
	resultObj.SetSystemName(m_sSystemName);
	resultObj.SetUserID(m_sUserID);
	resultObj.SetUserName(m_sUserName);
	resultObj.SetHospitalID(m_sHospitalID);
	resultObj.SetHospitalName(m_sHospitalName);

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

	sGuid = "PIS:";
	sGuid += resultObj.m_sStudyID;
	resultObj.m_sStudyGUID = sGuid;

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
	sDtTime.Format("%02d:%02d:%02d", dtReport.GetHour(), dtReport.GetMinute(), dtReport.GetSecond());
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

	sValue = cmd.Field("shys").asString();
	sValue.TrimLeft();
	sValue.TrimRight();

	if (!sValue.IsEmpty()) {
		resultObj.m_sReviewDate = resultObj.m_sReportDate;
		resultObj.m_sReviewTime = resultObj.m_sReviewTime;
		resultObj.m_sReviewDoctorName = sValue;
	}
	
	sValue = cmd.Field("sfyx").asString();
	if (sValue == "是" || sValue == "阳性")
		resultObj.m_nPositiveFlag = 1;

	m_sXMLDoc = resultObj.EncodeToXMLDoc_LX2();

	return m_sXMLDoc;
}

