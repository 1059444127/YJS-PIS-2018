// DriverChecker.h: interface for the CDriverChecker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRIVERCHECKER_H__B73FF146_48C9_4CEA_9673_805C9EFF3699__INCLUDED_)
#define AFX_DRIVERCHECKER_H__B73FF146_48C9_4CEA_9673_805C9EFF3699__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "gdal.h"

#ifndef ALLFORMAT_W
#define ALLFORMAT_W   23
#endif

typedef enum
{
	BL_NOLIMIT = 0,
	BL_LIMIT = 1
}BANDLIMIT;

typedef struct
{
	BANDLIMIT eBandLimit;
	int nBandAllowCount;
	int nBandAllow[20];
}BANDCHECK;

typedef enum
{
	DTL_NOLIMIT = 0,
	DTL_LIMIT = 1
}DATATYPELIMIT;

typedef struct
{
	DATATYPELIMIT eDataTypeLimit;
	int nDataTypeCount;
	GDALDataType eDataType[GDT_TypeCount];
}DATATYPECHECK;

typedef struct
{
	CString strDriverName;
	DATATYPECHECK stcDataTypeChk;
	BANDCHECK stcBandChk;
	
	CString pstrMessage;
}DRIVERCHECK;

class  CDriverChecker;
CDriverChecker *GS_GetDriverChecker( void );

void GS_RegisteDriverChk( void );
void GS_DestroyDriverChk( void );

class CDriverChecker  
{
public:

	CDriverChecker();
	virtual ~CDriverChecker();

	CString GetErrorMsg();

	int  DriverCheck(CString, GDALDataType, int);

	int  GetDataTypeRange(GDALDataType, double &, double &);

protected:
	int  InitialDriverCheck();
	int  DestroyDriverCheck();

	//Fill the driver checker array with content of all drivers
	void FillChkContent();

	//Judge whether the driver has been registed
	int  IsDriverRegisted(CString strDriverName);
	//Match the driver with the driver checker array
	int  MatchDriverCheck(CString strDriverName);
	//Judge whether the data type given is supported
	int  DataTypeCheck(GDALDataType eDataType);
	//Judge whether the band count given is supported
	int  BandCheck(int nBands);

protected:
	DRIVERCHECK  m_aDriverCheck[ALLFORMAT_W];
    DRIVERCHECK *m_pDriverCheck;
	CString m_strErrorMsg;
};

#endif // !defined(AFX_DRIVERCHECKER_H__B73FF146_48C9_4CEA_9673_805C9EFF3699__INCLUDED_)
