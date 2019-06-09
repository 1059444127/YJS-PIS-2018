#if !defined(AFX_CHECKLICENSE_H__188308B4_0AAD_11D2_8A47_0000E81D3D27__INCLUDED_)
#define AFX_CHECKLICENSE_H__188308B4_0AAD_11D2_8A47_0000E81D3D27__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CCheckLicense
{
public:
	CCheckLicense();
	~CCheckLicense();

	int  Check(CString sSystemId, CString sHost, int nPort, BOOL bCheckLocalDongle);

	CString GetReceivedText() {return m_sReceivedText;}

private:

	CString GetHostName();
	CString GetHostIP();

	void GetKeyValue(CString sMsg, CString sKey, int &nValue);
	void GetKeyValue(CString sMsg, CString sKey, CString &sValue);
	void ReadParams(CString sMsg);

private:
	CString m_sReceivedText;	
public:
	CString m_sDBType;
	CString m_sDBHostName;
	CString m_sDBProvider;
	CString m_sDBName;
	CString m_sSchemaName;
	CString m_sDBUserId;
	CString m_sDBPswd;
	CString m_sStartDate;
	CString m_sStartTime;
	CString m_sAppTitle;
	CString m_sCompanyName;
	CString m_sWebSite;
	CString m_sContactMe;
	CString m_sSupport1;
	CString m_sSupoort2;
	CString m_sLicInfo;

	BOOL	m_bLicMode;
	BOOL	m_bDemoMode;

	int		m_nExpiryYear;
	int		m_nExpiryMonth;
};


#endif