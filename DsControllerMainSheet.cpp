//==============================================================================================
//	FILE      :	DsControllerMainSheet.cpp
//	ABSTRACT  :	Container class for the Devices, Property, and Stream tabs.
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//
//	Copyright (c) 2005, Nikon Corp.  All rights reserved.
//==============================================================================================

#include "stdafx.h"
#include "Pathology.h"
#include "DsControllerMainSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDsControllerMainSheet

IMPLEMENT_DYNAMIC(CDsControllerMainSheet, CPropertySheet)
extern CPathologyApp theApp;
CDsControllerMainSheet::CDsControllerMainSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	_ASSERT(0);
}

//==============================================================================================
//	FUNCTION  :	CDsControllerMainSheet
//	ABSTRACT  :	Constructor
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
CDsControllerMainSheet::CDsControllerMainSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_pCamera =theApp.GetCamera();

	AddPage(&m_DevicesPage);
	AddPage(&m_PropPage);
	AddPage(&m_StreamPage);

	m_psh.dwFlags ^= PSH_HASHELP;
	m_DevicesPage.m_psp.dwFlags ^= PSP_HASHELP;
	m_PropPage.m_psp.dwFlags ^= PSP_HASHELP;
	m_StreamPage.m_psp.dwFlags ^= PSP_HASHELP;
}

//==============================================================================================
//	FUNCTION  :	~CDsControllerMainSheet
//	ABSTRACT  :	Destructor
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================





CDsControllerMainSheet::~CDsControllerMainSheet()
{
	//GetDebugPriv();
	//KillProcess();
	theApp.OnAppRefresh();
	//m_pCamera->Uninitialize();

}


BEGIN_MESSAGE_MAP(CDsControllerMainSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CDsControllerMainSheet)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDsControllerMainSheet Message Handler

//==============================================================================================
//	FUNCTION  :	OnCreate
//	ABSTRACT  :	Create the PropertySheet and initialize the camera objects
//	PARAMS    :	lpCreateStruct		CREATESTRUCT Structure
//	RETURN    :	returns 0
//	NOTE      :	if failed, Show Dialog
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
int CDsControllerMainSheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// creates CameraController object
	if (FAILED(m_pCamera->Initialize())) {
		EndDialog(-1);
	}

	return 0;
}

//==============================================================================================
//	FUNCTION  :	OnCreate
//	ABSTRACT  :	Destroys the PropertySheet
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
void CDsControllerMainSheet::OnDestroy() 
{
	
	CPropertySheet::OnDestroy();
}

//==============================================================================================
//	FUNCTION  :	ChangeDevice
//	ABSTRACT  :	Notify all tabs if the currently selected camera changes
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
void CDsControllerMainSheet::ChangeDevice()
{
	m_DevicesPage.ChangeDevice();
	m_PropPage.ChangeDevice();
	m_StreamPage.ChangeDevice();
}

//==============================================================================================
//	FUNCTION  :	OnInitDialog
//	ABSTRACT  :	bottons control
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//==============================================================================================
BOOL CDsControllerMainSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	CButton *btnOK = (CButton *)GetDlgItem(IDOK);
	btnOK->ShowWindow(SW_HIDE);

	CButton *btnApply = (CButton *)GetDlgItem(ID_APPLY_NOW);
	CButton *btnCancel = (CButton *)GetDlgItem(IDCANCEL);

	CRect rtApply, rtCancel;

	btnApply->GetWindowRect(rtApply);
	btnCancel->GetWindowRect(rtCancel);

	ScreenToClient(&rtApply);
	ScreenToClient(&rtCancel);

	btnApply->MoveWindow(rtCancel);
	btnCancel->MoveWindow(rtApply);

	btnApply->SetWindowText("Apply");
	btnCancel->SetWindowText("Close");

	/////////////////////////////////////////////
	struct LANGANDCODEPAGE {
		WORD wLanguage;
		WORD wCodePage;
	}*lpTranslate;

	char szExeName[_MAX_PATH];
	sprintf(szExeName, "dscamera2.dll");

	DWORD dwDataLen=0;
	unsigned int uiLen=0;
	dwDataLen = GetFileVersionInfoSize(szExeName, 0);
	char* pcInfo = new char [dwDataLen];
	CString strVersionInfo,strCopyright,strAppName;
	BOOL bRet = GetFileVersionInfo(szExeName, 0, dwDataLen, pcInfo);
	if (bRet){
		bRet = VerQueryValue(pcInfo, TEXT("\\VarFileInfo\\Translation"),(LPVOID*)&lpTranslate, &uiLen);
		if(bRet){
			unsigned int dwByte;
			void *lpBuffer=NULL;
			char SubBlock[256];
			// SDK Puroduct Version
			for(unsigned int i=0; i < (uiLen/sizeof(struct LANGANDCODEPAGE)); i++ ){
				wsprintf( SubBlock, TEXT("\\StringFileInfo\\%04x%04x\\ProductVersion"),lpTranslate[i].wLanguage,lpTranslate[i].wCodePage);
				bRet = VerQueryValue(pcInfo, SubBlock, &lpBuffer, &dwByte);
				if (bRet){
					char cVersionNum[256];
					strcpy(cVersionNum,(const char *)lpBuffer);
					for(int j = 0; j < strlen(cVersionNum); j++){
						if(cVersionNum[j] == ',')
							cVersionNum[j] = '.';
					}
					strVersionInfo.Format("DS-U2/L2 SDK Version %s",cVersionNum);
					break;
				}
			}
		}
	}
	if (pcInfo){
		delete [] pcInfo;
	}

	CString cszTitle;
	GetWindowText(cszTitle);

	cszTitle = cszTitle + " ( " + strVersionInfo + " )";

	SetWindowText(cszTitle);

	CPropertySheet::SetActivePage(2);
	((CStreamPage* )GetPage(2))->start();


	return bResult;
}
