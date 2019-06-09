//---------------------------------------------------------------------------

#include <stdafx.h>
#include <snmp.h>
#include <process.h>     /* needed for declaration of getpid() */
#include <nb30.h>
#include <sys/types.h>
#include <winsock2.h>     /* include winsock.h directly i.e. on MacOS */
#include <stdio.h>
#include "ofcrc32.h"




#pragma hdrstop

#include "GetStudyUID.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


static long gethostid(void);


typedef int(WINAPI *pSnmpUtilOidCpy) (
        OUT AsnObjectIdentifier *pOidDst,
        IN AsnObjectIdentifier *pOidSrc);

typedef int(WINAPI *pSnmpUtilOidNCmp) (
        IN AsnObjectIdentifier *pOid1,
        IN AsnObjectIdentifier *pOid2,
        IN UINT nSubIds);

typedef void(WINAPI *pSnmpUtilVarBindFree) (
        IN OUT SnmpVarBind *pVb);

typedef bool(WINAPI *pSnmpExtensionInit) (
        IN DWORD dwTimeZeroReference,
        OUT HANDLE *hPollForTrapEvent,
        OUT AsnObjectIdentifier *supportedView);

typedef bool(WINAPI *pSnmpExtensionTrap) (
        OUT AsnObjectIdentifier *enterprise,
        OUT AsnInteger32 *genericTrap,
        OUT AsnInteger32 *specificTrap,
        OUT AsnTimeticks *timeStamp,
        OUT SnmpVarBindList *variableBindings);

typedef bool(WINAPI *pSnmpExtensionQuery) (
        IN BYTE requestType,
        IN OUT SnmpVarBindList *variableBindings,
        OUT AsnInteger32 *errorStatus,
        OUT AsnInteger32 *errorIndex);

typedef bool(WINAPI *pSnmpExtensionInitEx) (
        OUT AsnObjectIdentifier *supportedView);

typedef struct _ASTAT_
{
    ADAPTER_STATUS adapt;
    NAME_BUFFER    NameBuff[30];
} ASTAT, *PASTAT;


static unsigned long hostIdentifier = 0;


/*
** char* generateUniqueIdentifer(char* buf)
** Creates a Unique Identifer in buf and returns buf.
** buf must be at least 65 bytes.
*/



static unsigned int counterOfCurrentUID = 1;

static const int maxUIDLen = 64;    /* A UID may be 64 chars or less */

static char* stripTrailing(char* s, char c)
{
    int i, n;

    if (s == NULL) return s;

    n = strlen(s);
    for (i = n - 1; (i >= 0) && (s[i] == c); i--)
        s[i] = '\0';
    return s;
}



/* get the MAC address of the (first) Ethernet adapter (6 bytes) */
static unsigned char *getMACAddress(unsigned char buffer[6])
{
    BOOL success = FALSE;
    /* init return variable */
    memset(buffer, 0, sizeof(buffer));
    NCB ncb;
    memset(&ncb, 0, sizeof(ncb));
    /* reset the LAN adapter */
    ncb.ncb_command = NCBRESET;
    /* it is considered bad practice to hardcode the LANA number (should enumerate
       adapters first), but at least this approach also works on Windows 9x */
    ncb.ncb_lana_num = 0;
    if (Netbios(&ncb) == NRC_GOODRET)
    {
        ASTAT Adapter;
        /* prepare to get the adapter status block */
        memset(&ncb, 0, sizeof(ncb));
        ncb.ncb_command = NCBASTAT;
        /* it is considered bad practice to hardcode the LANA number (should enumerate
           adapters first), but at least this approach also works on Windows 9x */
        ncb.ncb_lana_num = 0;
        strcpy((char *)ncb.ncb_callname, "*");
        ncb.ncb_buffer = (unsigned char *)&Adapter;
        ncb.ncb_length = sizeof(Adapter);
        /* get the adapter's info */
        if (Netbios(&ncb) == 0)
        {
            /* store the MAC address */
            buffer[0] = Adapter.adapt.adapter_address[0];
            buffer[1] = Adapter.adapt.adapter_address[1];
            buffer[2] = Adapter.adapt.adapter_address[2];
            buffer[3] = Adapter.adapt.adapter_address[3];
            buffer[4] = Adapter.adapt.adapter_address[4];
            buffer[5] = Adapter.adapt.adapter_address[5];
            success = TRUE;
        }
    }
    /* check whether NetBIOS routines succeeded, if not try the SNMP approach */
    if (!success)
    {
        HINSTANCE m_hInst1, m_hInst2;
        /* load the "SNMP Utility Library" dll and get the addresses of the functions necessary */
        m_hInst1 = LoadLibrary("snmpapi.dll");
        if (m_hInst1 >= (HINSTANCE)HINSTANCE_ERROR)
        {
            pSnmpUtilOidCpy m_Copy = (pSnmpUtilOidCpy)GetProcAddress(m_hInst1, "SnmpUtilOidCpy");
            pSnmpUtilOidNCmp m_Compare = (pSnmpUtilOidNCmp)GetProcAddress(m_hInst1, "SnmpUtilOidNCmp");
            pSnmpUtilVarBindFree m_BindFree = (pSnmpUtilVarBindFree)GetProcAddress(m_hInst1, "SnmpUtilVarBindFree");
            /* load the "SNMP Internet MIB" dll and get the addresses of the functions necessary */
            m_hInst2 = LoadLibrary("inetmib1.dll");
            if (m_hInst2 >= (HINSTANCE)HINSTANCE_ERROR)
            {
                HANDLE PollForTrapEvent;
                AsnObjectIdentifier SupportedView;
                UINT OID_ifEntryType[] = {1, 3, 6, 1, 2, 1, 2, 2, 1, 3};
                UINT OID_ifEntryNum[] = {1, 3, 6, 1, 2, 1, 2, 1};
                UINT OID_ipMACEntAddr[] = {1, 3, 6, 1, 2, 1, 2, 2, 1, 6};
                AsnObjectIdentifier MIB_ifMACEntAddr = {sizeof(OID_ipMACEntAddr) / sizeof(UINT), OID_ipMACEntAddr};
                AsnObjectIdentifier MIB_ifEntryType = {sizeof(OID_ifEntryType) / sizeof(UINT), OID_ifEntryType};
                AsnObjectIdentifier MIB_ifEntryNum = {sizeof(OID_ifEntryNum) / sizeof(UINT), OID_ifEntryNum};
                SnmpVarBindList varBindList;
                SnmpVarBind varBind[2];
                AsnInteger32 errorStatus;
                AsnInteger32 errorIndex;
                AsnObjectIdentifier MIB_NULL = {0, 0};
                int ret;
                int dtmp;
                int j = 0;
                pSnmpExtensionInit m_Init = (pSnmpExtensionInit)GetProcAddress(m_hInst2, "SnmpExtensionInit");
                /* pSnmpExtensionInitEx m_InitEx = (pSnmpExtensionInitEx)GetProcAddress(m_hInst2, "SnmpExtensionInitEx"); */
                pSnmpExtensionQuery m_Query = (pSnmpExtensionQuery)GetProcAddress(m_hInst2, "SnmpExtensionQuery");
                /* pSnmpExtensionTrap m_Trap = (pSnmpExtensionTrap)GetProcAddress(m_hInst2, "SnmpExtensionTrap"); */
                m_Init(GetTickCount(), &PollForTrapEvent, &SupportedView);
                /* initialize the variable list to be retrieved by m_Query */
                varBindList.list = varBind;
                varBind[0].name = MIB_NULL;
                varBind[1].name = MIB_NULL;
                /* copy in the OID to find the number of entries in the inteface table */
                varBindList.len = 1;        /* only retrieving one item */
                m_Copy(&varBind[0].name, &MIB_ifEntryNum);
                ret = m_Query(SNMP_PDU_GETNEXT, &varBindList, &errorStatus, &errorIndex);
                varBindList.len = 2;
                /* copy in the OID of ifType, the type of interface */
                m_Copy(&varBind[0].name, &MIB_ifEntryType);
                /* copy in the OID of ifPhysAddress, the address */
                m_Copy(&varBind[1].name, &MIB_ifMACEntAddr);
                do {
                    /* Submit the query.  Responses will be loaded into varBindList.
                       We can expect this call to succeed a # of times corresponding
                       to the # of adapters reported to be in the system */
                    ret = m_Query(SNMP_PDU_GETNEXT, &varBindList, &errorStatus, &errorIndex);
                    if (!ret)
                        ret = 1;
                    else
                    {
                        /* confirm that the proper type has been returned */
                        ret = m_Compare(&varBind[0].name, &MIB_ifEntryType, MIB_ifEntryType.idLength);
                    }
                    if (!ret)
                    {
                        j++;
                        dtmp = varBind[0].value.asnValue.number;
                        /* type 6 describes ethernet interfaces */
                        if (dtmp == 6)
                        {
                            /* confirm that we have an address here */
                            ret = m_Compare(&varBind[1].name, &MIB_ifMACEntAddr,MIB_ifMACEntAddr.idLength);
                            if ((!ret) && (varBind[1].value.asnValue.address.stream != NULL))
                            {
                                if ((varBind[1].value.asnValue.address.stream[0] == 0x44) &&
                                    (varBind[1].value.asnValue.address.stream[1] == 0x45) &&
                                    (varBind[1].value.asnValue.address.stream[2] == 0x53) &&
                                    (varBind[1].value.asnValue.address.stream[3] == 0x54) &&
                                    (varBind[1].value.asnValue.address.stream[4] == 0x00))
                                {
                                    /* ignore all dial-up networking adapters */
                                    continue;
                                }
                                if ((varBind[1].value.asnValue.address.stream[0] == 0x00) &&
                                    (varBind[1].value.asnValue.address.stream[1] == 0x00) &&
                                    (varBind[1].value.asnValue.address.stream[2] == 0x00) &&
                                    (varBind[1].value.asnValue.address.stream[3] == 0x00) &&
                                    (varBind[1].value.asnValue.address.stream[4] == 0x00) &&
                                    (varBind[1].value.asnValue.address.stream[5] == 0x00))
                                {
                                    /* ignore NULL addresses returned by other network interfaces */
                                    continue;
                                }
                                /* store the MAC address */
                                buffer[0] = varBind[1].value.asnValue.address.stream[0];
                                buffer[1] = varBind[1].value.asnValue.address.stream[1];
                                buffer[2] = varBind[1].value.asnValue.address.stream[2];
                                buffer[3] = varBind[1].value.asnValue.address.stream[3];
                                buffer[4] = varBind[1].value.asnValue.address.stream[4];
                                buffer[5] = varBind[1].value.asnValue.address.stream[5];
                                ret = 1;    // we found an address -> exit
                            }
                        }
                    }
                } while (!ret);  /* Stop only on an error. An error will occur when we
                                    go exhaust the list of interfaces to be examined */
                FreeLibrary(m_hInst2);
                /* free the bindings */
                m_BindFree(&varBind[0]);
                m_BindFree(&varBind[1]);
            }
            FreeLibrary(m_hInst1);
        }
    }
    return buffer;
}



static void addUIDComponent(char* uid, const char* s)
{
    int charsLeft = 64 - strlen(uid);

    if (charsLeft > 0)
    {
        /* copy into uid as much of the contents of s as possible */
        int slen = strlen(s);
        int use = charsLeft;
        if (slen < charsLeft) use = slen;
            strncat(uid, s, use);
    }

///    stripTrailing(uid, '.');
}


char* dcmGenerateUniqueIdentifier(char* uid, const char* prefix)
{
    char buf[128]; /* be very safe */

    uid[0] = '\0'; /* initialise */

    if (hostIdentifier == 0)
        hostIdentifier = (unsigned long) gethostid();

    unsigned int counter = counterOfCurrentUID++;

    if (prefix != NULL )
    {
        addUIDComponent(uid, prefix);
    }
    else
    {
        addUIDComponent(uid, SITE_INSTANCE_UID_ROOT);
    }

    sprintf(buf, ".%lu", hostIdentifier);
    addUIDComponent(uid, buf);

    sprintf(buf, ".%lu", (unsigned long) getpid());
    addUIDComponent(uid, buf);

    sprintf(buf, ".%lu", (unsigned long) time(NULL));
    addUIDComponent(uid, buf);

    sprintf(buf, ".%u", counter);

    addUIDComponent(uid, buf);

    return uid;
}


static long gethostid(void)
{
    long result = 0;
    char name[1024];
    struct hostent *hent = NULL;
    char **p = NULL;
    struct in_addr in;

    WSAData winSockData;
    /* we need at least version 1.1 */
    WORD winSockVersionNeeded = MAKEWORD(1, 1);
    WSAStartup(winSockVersionNeeded, &winSockData);

    /*
    ** Define the hostid to be the system's main TCP/IP address.
    ** This is not perfect but it is better than nothing (i.e. using zero)
    */
    if (gethostname(name, 1024) == 0)
    {
        if ((hent = gethostbyname(name)) != NULL)
        {
            p = hent->h_addr_list;
            if (p && *p)
            {
                (void) memcpy(&in.s_addr, *p, sizeof(in.s_addr));
                result = (long)in.s_addr;
            }
        }
    }

    WSACleanup();

    /* on Windows systems determine some system specific information (e.g. MAC address) */

    OFCRC32 crc;
    /* get some processor specific information in addition to the MAC address */
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    /* get volume information of the system drive */
    char systemDrive[MAX_PATH];
    DWORD serialNumber = 0;
    if (GetSystemDirectory(systemDrive, sizeof(systemDrive)) >= 0)
    {
        /* check for proper pathname */
        if ((strlen(systemDrive) >= 3) && (systemDrive[1] == ':') && (systemDrive[2] == '\\'))
        {
            /* truncate the pathname directly after the drive specification */
            systemDrive[3] = 0;
            if (!GetVolumeInformation(systemDrive, NULL, 0, &serialNumber, NULL, NULL, NULL, 0))
                serialNumber = 0;
        }
    }
    /* concatenate the host specific elements and compute a 32-bit checksum */
    crc.addBlock(&result /*ip address*/, sizeof(result));
    crc.addBlock(&serialNumber, sizeof(serialNumber));
    crc.addBlock(&systemInfo.wProcessorLevel, sizeof(systemInfo.wProcessorLevel));
    crc.addBlock(&systemInfo.wProcessorRevision, sizeof(systemInfo.wProcessorRevision));
    crc.addBlock(&systemInfo.dwProcessorType, sizeof(systemInfo.dwProcessorType));
    result = (long)crc.getCRC32();
    /* 'artificial' hostid: on Windows system a CRC32 checksum over some host specific
       information (e.g. MAC address), the 4 bytes TCP/IP address otherwise.
    */
    return result;
}

