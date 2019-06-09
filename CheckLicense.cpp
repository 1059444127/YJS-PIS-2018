#include "stdafx.h"
#include <winsock2.h>
#include "CheckLicense.h"

CString CCheckLicense::GetHostName()
{
	hostent *p;
	char s[128];
	char *p2;
	CString sHostName;

	WORD wVersionRequested;
	WSADATA wsaData;

	//Start up WinSock
	wVersionRequested = MAKEWORD(1, 1);
	WSAStartup(wVersionRequested, &wsaData);

	//Get the computer name
	gethostname(s, 128);
	p = gethostbyname(s);

	if (p != NULL)
	{
		sHostName = p->h_name;
	}
	else
	{
		sHostName = "Error";
	}

	sHostName.MakeUpper();

	//g_sHostName = sHostName;
	//Get the IpAddress
	//g_sHostIP = inet_ntoa(*((in_addr *)p->h_addr));

	WSACleanup();

	return sHostName;
}

//---------------------------------------------------------------------------

CString CCheckLicense::GetHostIP()
{
	hostent *p;
	char s[128];
	char *p2;
	CString sHostIP;

	WORD wVersionRequested;
	WSADATA wsaData;

	//Start up WinSock
	wVersionRequested = MAKEWORD(1, 1);
	WSAStartup(wVersionRequested, &wsaData);

	//Get the computer name
	gethostname(s, 128);
	p = gethostbyname(s);


	//Get the IpAddress
	sHostIP = inet_ntoa(*((in_addr *)p->h_addr));

	WSACleanup();

	return sHostIP;
}


CCheckLicense::CCheckLicense()
{
	m_sDBType		= "";
	m_sDBHostName	= "";
	m_sDBProvider	= "";
	m_sDBName		= "";
	m_sSchemaName	= "";
	m_sDBUserId		= "";
	m_sDBPswd		= "";
	m_sStartDate	= "";
	m_sStartTime	= "";
	m_sAppTitle		= "";
	m_sCompanyName	= "";
	m_sWebSite		= "";
	m_sContactMe	= "";
	m_sSupport1		= "";
	m_sSupoort2		= "";
	m_sLicInfo		= "";

	m_bLicMode		= FALSE;
	m_bDemoMode		= FALSE;

	m_nExpiryYear	= 0;
	m_nExpiryMonth	= 0;
}


CCheckLicense::~CCheckLicense()
{

}


int CCheckLicense::Check(CString sSystemId, CString sLicHostName, int nPortNo, BOOL bCheckLocalDongle)
{
	CString sMsg,sKey,sTmp,sHost,sHostIP,sInformStatus,sAutoCode,sPort,sDate,sTime;
	CString sSchemaName("");
	SOCKET sock;
	sockaddr p;
	unsigned int nPassword = 710217 * 2;
	unsigned int nDongleSN;
	DWORD dwStatus;
	unsigned int dwDongleData0,dwDongleData1;
	BOOL bOk;
	char sConvertBuf[20];
	char sFreeHost[60];
	int nConvertBufLen;
	COleDateTime today;
	unsigned short nY,nM,nD,nH,nMin,nS,nMs;
	char lpszDay[12 * 3 + 1] = "1234567890abcdefghijklmnopqrstuvwxyz";
	CString sLicSystem;

	struct sockaddr_in sa;
	struct hostent *host;
	struct timeval tv;
	int nTimeout = 30000;
	int size,len,n,nPos,nRet,nKeyLen,ni;
	char buf[4096];

	WORD wVersionRequested;
	WSADATA wsaData;

	nRet = -1;

	//Start up WinSock
	wVersionRequested = MAKEWORD(1, 1);
	WSAStartup(wVersionRequested, &wsaData);

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == -1)
	{
		WSACleanup();
		return -1;
	}

	setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO,(const char *) &nTimeout, sizeof(nTimeout));
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO,(const char *) &nTimeout, sizeof(nTimeout));

	sHost = GetHostName();

	memset(sFreeHost,0,60);
	strcpy(sFreeHost,sHost);

	sMsg.Format("LOGIN:%s;%s",sHost,sSystemId);

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(nPortNo);

	if (sLicHostName.Find(".") > 0)
	{
		sHostIP = sLicHostName;
	}
	else
	{
		host = gethostbyname(sLicHostName);
		sHostIP = inet_ntoa(*((in_addr *)host->h_addr));
	}

	sa.sin_addr.s_addr = inet_addr(sHostIP);

	connect(sock, (sockaddr *)&sa, sizeof(sa));

	len = sMsg.GetLength();

	m_sReceivedText = "";

	size = send(sock, sMsg, len, 0);

	if (size != len) 
	{
		closesocket(sock);
		WSACleanup();

		return -1;
	}

	memset(buf,0,4096);
	len = recv(sock, buf, 4096, 0);

	if (len < 2)
	{
		closesocket(sock);
		WSACleanup();
		return -1;
	}

	for (n = 1; n < len ; n ++) buf[n] = buf[n] ^ buf[0];

	buf[0] = ' ';

	if (buf[1] == '1')
	{
		buf[1] = ' ';

		sMsg = CString(buf);

		m_sReceivedText = sMsg;
		
		ReadParams(sMsg);
		
		nRet = 1;

	}
	else if (buf[1] == '2')
	{
		nRet = 2;
	}
	else
	{
		nRet = 0;
	}
	
	closesocket(sock);
	WSACleanup();

	return nRet;
}

void CCheckLicense::GetKeyValue(CString sMsg, CString sKey, CString &sValue)
{
	CString sKeyWithEqual,sTmp,sKeyValue("");
	int nKeyLen, nPos;

	sKeyWithEqual.Format("%s=", sKey);

	nKeyLen = sKeyWithEqual.GetLength();
	nPos = sMsg.Find(sKeyWithEqual);

	if (nPos >= 0)
	{
		sTmp = sMsg.Mid(nPos,sMsg.GetLength() - nPos);

		nPos = sTmp.Find(";");
		if (nPos > nKeyLen)
		{
			sKeyValue = sTmp.Mid(nKeyLen,nPos - nKeyLen);
			sKeyValue.TrimLeft();
			sKeyValue.TrimRight();
		}
	}

	sValue = sKeyValue;

	return;
}

void CCheckLicense::GetKeyValue(CString sMsg, CString sKey, int &nValue)
{
	CString sValue("");

	GetKeyValue(sMsg, sKey, sValue);
	nValue = atoi(sValue);

	return;
}

void CCheckLicense::ReadParams(CString sMsg)
{
	CString sText("");

	GetKeyValue(sMsg, "DBType",		m_sDBType);
	GetKeyValue(sMsg, "HostName",	m_sDBHostName);
	GetKeyValue(sMsg, "Provider",	m_sDBProvider);
	GetKeyValue(sMsg, "Database",	m_sDBName);
	GetKeyValue(sMsg, "SchemaName", m_sSchemaName);
	GetKeyValue(sMsg, "UserName",	m_sDBUserId);
	GetKeyValue(sMsg, "Password",	m_sDBPswd);
	GetKeyValue(sMsg, "Date",		m_sStartDate);
	GetKeyValue(sMsg, "Time",		m_sStartTime);
	GetKeyValue(sMsg, "AppTitle",	m_sAppTitle);
	GetKeyValue(sMsg, "CompanyName",m_sCompanyName);
	GetKeyValue(sMsg, "WebSite",	m_sWebSite);
	GetKeyValue(sMsg, "ContactMe",	m_sContactMe);
	GetKeyValue(sMsg, "Support1",	m_sSupport1);
	GetKeyValue(sMsg, "Support2",	m_sSupoort2);
	GetKeyValue(sMsg, "LicInfo",	m_sLicInfo);
	GetKeyValue(sMsg, "LicOk",		sText);
	
	m_bLicMode = (sText == "T" ? TRUE : FALSE);
	
	GetKeyValue(sMsg, "DemoMode", sText);
	m_bDemoMode	= (sText == "T" ? TRUE : FALSE);

	GetKeyValue(sMsg, "ExpiryYear",	m_nExpiryYear);
	GetKeyValue(sMsg, "ExpiryMonth",m_nExpiryMonth);

	return;
}

