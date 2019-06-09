//---------------------------------------------------------------------------

#ifndef __HISConnectClient1_H__
#define __HISConnectClient1_H__

#include <string>

#include "sqlapi.h"

//---------------------------------------------------------------------------

class CHISConnectClient1
{
public:
	CHISConnectClient1();
	~CHISConnectClient1();

public:
	void LoadClient(const char *szFileName = "");
   //	void SetPDFReportPath(const char *szPDFReportPath);
	void Initialize(int HISCode, char *szModality, char *szEndPoint, char *szSystemCode, char *szSystemName);
	void SetOwnerWnd(HWND hWnd);
	void NotifyExit(void);
	void SetSenderInfo(const char *szUserId, const char *szUserName);
	void SetHospitalInfo(const char *szHospitalID, const char *szHospitalName);
	void SetExtraSQLWhereExp(const char *szExp);
	void SetShowSubmitResultMessage(bool bShow);

	const char *ReadMedicalOrder();

	BOOL UpdateMedicalOrder(const char *sXmlDoc, const char *statusCode);

	BOOL Connected() { return (m_hHISConnectDll != NULL); }

	BOOL ReadOrderImageFromUrl(const char *szUrl, unsigned char **lpData, unsigned long &nDataLen);
	BOOL NewReportUploadTask(const char *szXmlDoc);
	BOOL ConvertJPEG2PDF(const char *szPathName, const char *szJPEGTemplateName, int nImageCount, const char *szPaperSize, const char *szPDFFileName, BOOL bDeleteJPEG);

	const char * GenerateXMLDocFromDataset(SACommand &cmd);

private:
	void ( *SetOwnerWnd_Dll)(HWND hWnd);
	void ( *NotifyExit_Dll)(void);
	void ( *ShowMedicalOrder_Dll)(const char *orderNo, const char *orderType,const char *inpatientNo, const char *outpatientNo);
	void ( *ShowMedicalOrderEx_Dll)(const char *orderNo, const char *orderType, const char *inpatientNo, const char *outpatientNo, HWND hParentWnd);
	void ( *Initialize_Dll)(int nHISCode, char *szModality, char *szEndPoint);
	const char * ( *GetMedicalOrder_Dll)(void);

	void ( *SetSenderInfo_Dll)(const char *szUserId, const char *szUserName);
	void ( *SetHospitalInfo_Dll)(const char *szHospitalID, const char *szHospitalName);
	void ( *SetExtraSQLWhereExp_Dll)(const char *szExp);
	void ( *SetShowSubmitResultMessage_Dll)(bool bShow);
	BOOL ( *UpdateMedicalOrder_Dll)(const char *xmlDoc, const char *statusCode);


	BOOL ( *ReadOrderImageFromUrl_Dll)(const char *szUrl, unsigned char **lpData, unsigned long &nDataLen);
	BOOL ( *NewReportUploadTask_Dll) (const char *szXmlDoc);

	BOOL ( *ConvertJPEG2PDF_Dll)(const char *szPatName, const char *szJPEGTemplateName, int nImageCount, const char *szPaperSize, const char *szPDFFileName, BOOL bDeleteJPEG);


private:

	HMODULE m_hHISConnectDll;
	CString m_sHospitalID;
	CString m_sHospitalName;

	CString m_sSystemCode;
	CString m_sSystemName;
	CString m_sUserID;
	CString m_sUserName;
	CString m_sXMLDoc;
};


extern CHISConnectClient1 g_HISConnectClient1;


#endif
