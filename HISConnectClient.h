//---------------------------------------------------------------------------

#ifndef __HISConnectClient_H__
#define __HISConnectClient_H__

//---------------------------------------------------------------------------
#include "SQLAPI.h"

class CHISConnectClient
{
public:
    CHISConnectClient();
    ~CHISConnectClient();

public:

    void LoadClient(const char *szFileName = "");
    void SetPDFReportPath(const char *szPDFReportPath);
    void Initialize(int HISCode, const char *szModality, const char *szEndPoint, const char *szSystemCode);
    void SetOwnerWnd(HWND hWnd);
	void NotifyExit(void);
    void SetSenderInfo(const char *szUserId, const char *szUserName);
    void SetShowSubmitResultMessage(bool bShow);
    const char *ReadMedicalOrder(void);
    BOOL UpdateMedicalOrder(const char *orderStatus, const char *xmlDoc);
    void NewPDFReportTask(const char *szFileName);
	void ShowMedicalOrder(const char *szOrderNo,const char *szOrderType);

    BOOL Connected() { return (m_hHISConnectDll != NULL); }


	const char *GenerateXMLDocFromDataset(SACommand &cmd);

private:
    void (*Initialize_Dll)(int nHISCode, const char *szModality, const char *szEndPoint);
    void (*SetOwnerWnd_Dll)(HWND hWnd);
	void (*NotifyExit_Dll)(void);

    const char *(* ReadMedicalOrder_Dll)(void);
	BOOL (*UpdateMedicalOrder_Dll)(const char *orderStatus, const char *xmlDoc);
    void (*SetSenderInfo_Dll)(const char *szUserId, const char *szUserName);
    void (*SetShowSubmitResultMessage_Dll)(bool bShow);
    void (*SetPDFReportPath_Dll)(const char *szPDFReportPath);
    void (*NewPDFReportTask_Dll)(const char *szFileName);
	void (*ShowMedicalOrder_Dll)(const char *szOrderNo, const char *szOrderType);
private:
    HMODULE m_hHISConnectDll;
	CString m_sXMLDoc;
	CString m_sSystemCode;
};

#endif
