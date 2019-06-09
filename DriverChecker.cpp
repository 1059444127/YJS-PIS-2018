// DriverChecker.cpp: implementation of the CDriverChecker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DriverChecker.h"

static CDriverChecker *poDirverCheck = NULL;

CDriverChecker *GS_GetDriverChecker()
{
	if(NULL == poDirverCheck)
		new CDriverChecker;

	return(poDirverCheck);
}

void GS_RegisteDriverChk( void )
{
	GS_GetDriverChecker();
}

void GS_DestroyDriverChk( void )
{
	if(NULL != poDirverCheck)
		delete poDirverCheck;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDriverChecker::CDriverChecker()
{
	if(NULL == poDirverCheck)
		poDirverCheck = this;

	m_pDriverCheck = NULL;
	m_strErrorMsg = _T("");

	InitialDriverCheck();
}

CDriverChecker::~CDriverChecker()
{
	if(poDirverCheck == this)
		poDirverCheck = NULL;
	
	DestroyDriverCheck();
}

int  CDriverChecker::InitialDriverCheck()
{
	FillChkContent();
	
	return TRUE;
}

int  CDriverChecker::DestroyDriverCheck()
{
	return TRUE;
}

CString CDriverChecker::GetErrorMsg()
{
	return m_strErrorMsg;
}

int CDriverChecker::DriverCheck(CString strDriverName, 
								GDALDataType eDataType,
								int nBands)
{
	if(!IsDriverRegisted(strDriverName))
	{
		m_strErrorMsg.Format("选择的输出文件格式为当前版本不支持的格式");
		return FALSE;
	}

	if( !MatchDriverCheck(strDriverName) )
		return FALSE;
	
	if( !DataTypeCheck(eDataType) )
	{
		m_strErrorMsg = m_pDriverCheck->pstrMessage;
		return FALSE;
	}
	
	if( !BandCheck(nBands) )
	{
		m_strErrorMsg = m_pDriverCheck->pstrMessage;
		return FALSE;
	}

	return TRUE;
}

int  CDriverChecker::GetDataTypeRange(GDALDataType eDataType, double &dMin, double &dMax)
{
	switch(eDataType)
	{
	case GDT_Byte:
		dMin = 0;  dMax = 255;
		return 1;
	case GDT_UInt16:
		dMin = 0;  dMax = 65535;
		return 1;
	case GDT_Int16:
		dMin = -32768;  dMax = 32767;
		return 1;
	case GDT_UInt32:
        dMin = 0;  dMax = 4294967295.0;
		return 1;
	case GDT_Int32:
		dMin = -2147483648.0;  dMax = 2147483647.0;
		return 1;
	case GDT_Float32:
		dMin = -4294967295.0; /* not actually accurate */
		dMax = 4294967295.0; /* not actually accurate */
		return 1;
	case GDT_Float64:
		dMin = -4294967295.0; /* not actually accurate */
		dMax = 4294967295.0; /* not actually accurate */
		return 1;
	case GDT_CInt16:
		dMin = -32768;   dMax = 32767;
		return 1;
	case GDT_CInt32:
		dMin = -2147483648.0;  dMax = 2147483647.0;
		return 1;
	case GDT_CFloat32:
		dMin = -4294967295.0; /* not actually accurate */
		dMax = 4294967295.0; /* not actually accurate */
		return 1;
	case GDT_CFloat64:
		dMin = -4294967295.0; /* not actually accurate */
		dMax = 4294967295.0; /* not actually accurate */
		return 1;
	default:
		dMin = dMax = 0.0;
		return 0;
	}
}

int CDriverChecker::IsDriverRegisted(CString strDriverName)
{
	GDALDriverH hDriver = GDALGetDriverByName(strDriverName);

	if(!hDriver)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

int CDriverChecker::MatchDriverCheck(CString strDriverName)
{
	if(m_pDriverCheck)
	{
		if(m_pDriverCheck->strDriverName == strDriverName)
			return TRUE;
	}
	
	for(int i = 0; i < ALLFORMAT_W; i++)
	{
		if( !stricmp(m_aDriverCheck[i].strDriverName, strDriverName) )
		{
			m_pDriverCheck = m_aDriverCheck + i;
			
			return TRUE;
		}
	}
	
	return FALSE;
}

int  CDriverChecker::DataTypeCheck(GDALDataType eDataType)
{
	if(NULL == m_pDriverCheck)
		return FALSE;

	if(DTL_NOLIMIT == m_pDriverCheck->stcDataTypeChk.eDataTypeLimit)
        return TRUE;
	else
	{
		for(int i = 0; i < m_pDriverCheck->stcDataTypeChk.nDataTypeCount; i++)
		{
			if(eDataType == m_pDriverCheck->stcDataTypeChk.eDataType[i])
				return TRUE;
		}

		return FALSE;
	}
}

int  CDriverChecker::BandCheck(int nBands)
{
	if(NULL == m_pDriverCheck)
		return FALSE;

	if(BL_NOLIMIT == m_pDriverCheck->stcBandChk.eBandLimit)
		return TRUE;
	else
	{
		for(int i = 0; i < m_pDriverCheck->stcBandChk.nBandAllowCount; i++)
		{
			if(nBands == m_pDriverCheck->stcBandChk.nBandAllow[i])
				return TRUE;
		}

		return FALSE;
	}
}

void CDriverChecker::FillChkContent()
{
	//fill now
	DRIVERCHECK *pChk = m_aDriverCheck;
	int i = 0;

	//i =0
	pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("AAIGRID");
	pChk->stcBandChk.eBandLimit = BL_LIMIT;
	pChk->stcBandChk.nBandAllowCount = 1;
	pChk->stcBandChk.nBandAllow[0] = 1; 
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
	pChk->stcDataTypeChk.nDataTypeCount = 4;
	pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;
    pChk->stcDataTypeChk.eDataType[1] = GDT_UInt16;
	pChk->stcDataTypeChk.eDataType[2] = GDT_Int16;
	pChk->stcDataTypeChk.eDataType[3] = GDT_Float32;
	pChk->pstrMessage = "Arc/Info ASCII Grid(*.asc)格式:\n波段数必须为1；\n数据类型必须为无符号8位、无符号16位、有符号16位、浮点32位\n";
	i++;

	//i = 1;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("BMP");
	pChk->stcBandChk.eBandLimit = BL_LIMIT;
	pChk->stcBandChk.nBandAllowCount = 2;
	pChk->stcBandChk.nBandAllow[0] = 1;
	pChk->stcBandChk.nBandAllow[1] = 3;
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
	pChk->stcDataTypeChk.nDataTypeCount = 1;
	pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;
	pChk->pstrMessage = "BMP格式(*.BMP)格式:\n波段数必须为1或3；\n数据类型必须为无符号8位\n";
	i++;

	//i = 2;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("BSB");
	pChk->stcBandChk.eBandLimit = BL_LIMIT;
	pChk->stcBandChk.nBandAllowCount = 1;
	pChk->stcBandChk.nBandAllow[0] = 1; 
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;//When bStrict = TRUE;
	pChk->stcDataTypeChk.nDataTypeCount = 1;
	pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;
	pChk->pstrMessage = "海图数据格式(*.kap):\n波段数必须为1；\n数据类型必须为无符号8位\n";
	i++;

	//i = 3;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("DTED");
	pChk->stcBandChk.eBandLimit = BL_NOLIMIT;
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
	pChk->stcDataTypeChk.nDataTypeCount = 3;
    pChk->stcDataTypeChk.eDataType[0] = GDT_Byte; 
	pChk->stcDataTypeChk.eDataType[1] = GDT_UInt16;
	pChk->stcDataTypeChk.eDataType[2] = GDT_Int16; 
	pChk->pstrMessage = "DTED 高程格式(*.ddf):\n数据类型必须为无符号8位、无符号16位、有符号16位\n";
	i++;

	//i = 4;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("ECW");
	pChk->stcBandChk.eBandLimit = BL_NOLIMIT;
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
	pChk->stcDataTypeChk.nDataTypeCount = 1;
	pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;//when bStrict == TRUE 
	pChk->pstrMessage = "ERMapper Compressed Wavelets (.ecw)格式:\n数据类型必须为无符号8位\n";
	i++;

	//i = 5;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("ELAS");
	pChk->stcBandChk.eBandLimit = BL_NOLIMIT; 
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
	pChk->stcDataTypeChk.nDataTypeCount = 3;
	pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;
    pChk->stcDataTypeChk.eDataType[1] = GDT_Float32;
	pChk->stcDataTypeChk.eDataType[2] = GDT_Float64;
	pChk->pstrMessage = "ELAS格式:\n数据类型必须为无符号8位、浮点32位、浮点64位\n";
	i++;

	//i = 6;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("FIT");
	pChk->stcBandChk.eBandLimit = BL_NOLIMIT;
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
	pChk->stcDataTypeChk.nDataTypeCount = 7;
	pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;
	pChk->stcDataTypeChk.eDataType[1] = GDT_UInt16;
	pChk->stcDataTypeChk.eDataType[2] = GDT_Int16;
	pChk->stcDataTypeChk.eDataType[3] = GDT_UInt32;
	pChk->stcDataTypeChk.eDataType[4] = GDT_Int32;
	pChk->stcDataTypeChk.eDataType[5] = GDT_Float32;
	pChk->stcDataTypeChk.eDataType[6] = GDT_Float64;
	pChk->pstrMessage = "FIT格式:\n数据类型必须为无符号8位、无符号16位、有符号16位、无符号32位、\n有符号32位、浮点32位、浮点64位";
	i++;

	//i = 7;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("GIF");
	pChk->stcBandChk.eBandLimit = BL_LIMIT;
	pChk->stcBandChk.nBandAllowCount = 1;
	pChk->stcBandChk.nBandAllow[0] = 1; 
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
	pChk->stcDataTypeChk.nDataTypeCount = 1;
	pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;
	pChk->pstrMessage = "GIF格式:\n波段数必须为1；\n数据类型必须为无符号8位\n";
	i++;

	//i = 8;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("GIP");
	pChk->stcBandChk.eBandLimit = BL_NOLIMIT;
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
	pChk->stcDataTypeChk.nDataTypeCount = 9;
	pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;
	pChk->stcDataTypeChk.eDataType[1] = GDT_UInt16;
	pChk->stcDataTypeChk.eDataType[2] = GDT_Int16;
	pChk->stcDataTypeChk.eDataType[3] = GDT_UInt32;
	pChk->stcDataTypeChk.eDataType[4] = GDT_Int32;
	pChk->stcDataTypeChk.eDataType[5] = GDT_Float32;
	pChk->stcDataTypeChk.eDataType[6] = GDT_Float64;
    pChk->stcDataTypeChk.eDataType[7] = GDT_CFloat32;
	pChk->stcDataTypeChk.eDataType[8] = GDT_CFloat64;
	pChk->pstrMessage = "GIP(*.gip)格式:\n数据类型必须为无符号8位、无符号16位、有符号16位、无符号32位、\n有符号32位、浮点32位、浮点64位、复浮点32位、复浮点64位";
	i++;

	//i = 9;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("GTiff");
	pChk->stcBandChk.eBandLimit = BL_NOLIMIT;
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
	pChk->stcDataTypeChk.nDataTypeCount = 10;
	pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;
	pChk->stcDataTypeChk.eDataType[1] = GDT_UInt16;
	pChk->stcDataTypeChk.eDataType[2] = GDT_Int16;
	pChk->stcDataTypeChk.eDataType[3] = GDT_UInt32;
	pChk->stcDataTypeChk.eDataType[4] = GDT_Int32;
	pChk->stcDataTypeChk.eDataType[5] = GDT_Float32;
	pChk->stcDataTypeChk.eDataType[6] = GDT_CInt16;
    pChk->stcDataTypeChk.eDataType[6] = GDT_CInt32;
    pChk->stcDataTypeChk.eDataType[7] = GDT_CFloat32;
	pChk->stcDataTypeChk.eDataType[8] = GDT_CFloat64;
	pChk->pstrMessage = "tiff (*.tif)格式:\n数据类型必须为无符号8位、无符号16位、有符号16位、无符号32位、\n有符号32位、浮点32位、复整16位、复整32位、\n复浮点32位、复浮点64位";
	i++;

	//i = 10;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("HDF4Image");
	pChk->stcBandChk.eBandLimit = BL_NOLIMIT;
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
	pChk->stcDataTypeChk.nDataTypeCount = 7;
	pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;
	pChk->stcDataTypeChk.eDataType[1] = GDT_UInt16;
	pChk->stcDataTypeChk.eDataType[2] = GDT_Int16;
	pChk->stcDataTypeChk.eDataType[3] = GDT_UInt32;
	pChk->stcDataTypeChk.eDataType[4] = GDT_Int32;
	pChk->stcDataTypeChk.eDataType[5] = GDT_Float32;
	pChk->stcDataTypeChk.eDataType[6] = GDT_Float64;
	pChk->pstrMessage = "HDF4Image(*.hdf)格式:\n数据类型必须为无符号8位、无符号16位、有符号16位、无符号32位、\n有符号32位、浮点32位、浮点64位";
	i++;

    //i = 11;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("HFA");
	pChk->stcBandChk.eBandLimit = BL_NOLIMIT;
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
	pChk->stcDataTypeChk.nDataTypeCount = 9;
	pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;
	pChk->stcDataTypeChk.eDataType[1] = GDT_UInt16;
	pChk->stcDataTypeChk.eDataType[2] = GDT_Int16;
	pChk->stcDataTypeChk.eDataType[3] = GDT_UInt32;
	pChk->stcDataTypeChk.eDataType[4] = GDT_Int32;
	pChk->stcDataTypeChk.eDataType[5] = GDT_Float32;
	pChk->stcDataTypeChk.eDataType[6] = GDT_Float64;
    pChk->stcDataTypeChk.eDataType[7] = GDT_CFloat32;
	pChk->stcDataTypeChk.eDataType[8] = GDT_CFloat64;
	pChk->pstrMessage = "ERDAS Imagine IMG(*.img)格式:\n数据类型必须为无符号8位、无符号16位、有符号16位、无符号32位、\n有符号32位、浮点32位、浮点64位、复浮点32位、复浮点64位";
	i++;

	//i = 12;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("JPEG");
	pChk->stcBandChk.eBandLimit = BL_LIMIT;
	pChk->stcBandChk.nBandAllowCount = 2;
	pChk->stcBandChk.nBandAllow[0] = 1;
	pChk->stcBandChk.nBandAllow[1] = 3;
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
	pChk->stcDataTypeChk.nDataTypeCount = 1;
	pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;
	pChk->pstrMessage = "JPEG JFIF(*.jpg)格式:\n波段数必须为1或3；数据类型必须为无符号8位\n";
	i++;

	//i = 13;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("JPEG2000");
	pChk->stcBandChk.eBandLimit = BL_NOLIMIT;
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
	pChk->stcDataTypeChk.nDataTypeCount = 5;
	pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;
	pChk->stcDataTypeChk.eDataType[1] = GDT_UInt16;
	pChk->stcDataTypeChk.eDataType[2] = GDT_Int16;
	pChk->stcDataTypeChk.eDataType[3] = GDT_UInt32;
	pChk->stcDataTypeChk.eDataType[4] = GDT_Int32;
	pChk->pstrMessage = "JPEG2000(*.jp2,*.j2k)格式:\n数据类型必须为无符号8位、无符号16位、有符号16位、无符号32位、有符号32位";
	i++;

	//i = 14;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("MFF");
	pChk->stcBandChk.eBandLimit = BL_NOLIMIT;
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
	pChk->stcDataTypeChk.nDataTypeCount = 5;
	pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;
	pChk->stcDataTypeChk.eDataType[1] = GDT_UInt16;
	pChk->stcDataTypeChk.eDataType[2] = GDT_Float32;
	pChk->stcDataTypeChk.eDataType[3] = GDT_CInt16;
    pChk->stcDataTypeChk.eDataType[4] = GDT_CFloat32;
	pChk->pstrMessage = "Atlantis MFF Raster(*.hdr)格式:\n数据类型必须为无符号8位、无符号16位、浮点32位、复整16位和复浮点32位";
	i++;

	//i = 15;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("MFF2");
	pChk->stcBandChk.eBandLimit = BL_NOLIMIT;
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
    pChk->stcDataTypeChk.nDataTypeCount = 7;
    pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;
	pChk->stcDataTypeChk.eDataType[1] = GDT_UInt16;
    pChk->stcDataTypeChk.eDataType[2] = GDT_Int16;
	pChk->stcDataTypeChk.eDataType[3] = GDT_Float32;
	pChk->stcDataTypeChk.eDataType[4] = GDT_CInt16;
    pChk->stcDataTypeChk.eDataType[5] = GDT_CInt32;
	pChk->stcDataTypeChk.eDataType[6] = GDT_CFloat32;
	pChk->pstrMessage = "MFF2(*. )格式:\n数据类型必须为无符号8位、无符号16位、有符号16位、浮点32位\n复整16位、复整32位和复浮点32位";
	i++;

	//i = 16;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("PAux");
	pChk->stcBandChk.eBandLimit = BL_NOLIMIT;
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
    pChk->stcDataTypeChk.nDataTypeCount = 4;
    pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;
	pChk->stcDataTypeChk.eDataType[1] = GDT_UInt16;
    pChk->stcDataTypeChk.eDataType[2] = GDT_Int16;
	pChk->stcDataTypeChk.eDataType[3] = GDT_Float32;
	pChk->pstrMessage = "PCI .aux Labelled(*.pix)格式:\n数据类型必须为无符号8位、无符号16位、有符号16位、浮点32位";
	i++;

	//i = 17;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("PCIDSK");
	pChk->stcBandChk.eBandLimit = BL_NOLIMIT;
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
	pChk->stcDataTypeChk.nDataTypeCount = 4;
    pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;
	pChk->stcDataTypeChk.eDataType[1] = GDT_UInt16;
    pChk->stcDataTypeChk.eDataType[2] = GDT_Int16;
	pChk->stcDataTypeChk.eDataType[3] = GDT_Float32;
	pChk->pstrMessage = "PCIDSK(*.pix)格式:\n数据类型必须为无符号8位、无符号16位、有符号16位、浮点32位";
	i++;

	//i = 18;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("PNG");
	pChk->stcBandChk.eBandLimit = BL_LIMIT;
	pChk->stcBandChk.nBandAllowCount = 4;
	pChk->stcBandChk.nBandAllow[0] = 1;
	pChk->stcBandChk.nBandAllow[1] = 2;
	pChk->stcBandChk.nBandAllow[2] = 3;
	pChk->stcBandChk.nBandAllow[3] = 4;
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
    pChk->stcDataTypeChk.nDataTypeCount = 2;
    pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;
	pChk->stcDataTypeChk.eDataType[1] = GDT_UInt16;
	pChk->pstrMessage = "PNG(*.png)格式:\n波段数必须为1、2、3、4之一；\n数据类型必须为无符号8位、无符号16位";
	i++;

	//i = 19;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("PNM");
	pChk->stcBandChk.eBandLimit = BL_LIMIT;
	pChk->stcBandChk.nBandAllowCount = 2;
	pChk->stcBandChk.nBandAllow[0] = 1; 
	pChk->stcBandChk.nBandAllow[1] = 3;
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
	pChk->stcDataTypeChk.nDataTypeCount = 2;
    pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;
	pChk->stcDataTypeChk.eDataType[1] = GDT_UInt16;
	pChk->pstrMessage = "Netpbm(*.pgm, *.ppm)格式:\n波段数必须为1或3；\n数据类型必须为无符号8位、无符号16位";
	i++;

	//i = 20;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("XPM");
	pChk->stcBandChk.eBandLimit = BL_LIMIT;
	pChk->stcBandChk.nBandAllowCount = 1;
	pChk->stcBandChk.nBandAllow[0] = 1; 
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
	pChk->stcDataTypeChk.nDataTypeCount = 1;
    pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;
	pChk->pstrMessage = "X11 Pixmap(*.xpm)格式:\n波段数必须为1；\n数据类型必须为无符号8位";
	i++;

	//i = 21;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("NITF");
	pChk->stcBandChk.eBandLimit = BL_NOLIMIT;
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
	pChk->stcDataTypeChk.nDataTypeCount = 8;
    pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;
    pChk->stcDataTypeChk.eDataType[1] = GDT_UInt16;
	pChk->stcDataTypeChk.eDataType[2] = GDT_Int16;
	pChk->stcDataTypeChk.eDataType[3] = GDT_UInt32;
	pChk->stcDataTypeChk.eDataType[4] = GDT_Int32;
	pChk->stcDataTypeChk.eDataType[5] = GDT_Float32;
    pChk->stcDataTypeChk.eDataType[6] = GDT_CFloat32;
	pChk->stcDataTypeChk.eDataType[7] = GDT_CFloat64;
	pChk->pstrMessage = "National Imagery Transmission Format(*.ntf)格式:\n数据类型必须为无符号8位、无符号16位、有符号16位、无符号32位、\n有符号32位、浮点32位、复浮点32位、复浮点64位";
	i++;

	//i = 22;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("ENVI");
	pChk->stcBandChk.eBandLimit = BL_NOLIMIT;
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
	pChk->stcDataTypeChk.nDataTypeCount = 11;
    pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;
    pChk->stcDataTypeChk.eDataType[1] = GDT_UInt16;
	pChk->stcDataTypeChk.eDataType[2] = GDT_Int16;
	pChk->stcDataTypeChk.eDataType[3] = GDT_UInt32;
	pChk->stcDataTypeChk.eDataType[4] = GDT_Int32;
	pChk->stcDataTypeChk.eDataType[5] = GDT_Float32;
	pChk->stcDataTypeChk.eDataType[6] = GDT_Float64;
	pChk->stcDataTypeChk.eDataType[7] = GDT_CInt16;
	pChk->stcDataTypeChk.eDataType[8] = GDT_CInt32;
    pChk->stcDataTypeChk.eDataType[9] = GDT_CFloat32;
	pChk->stcDataTypeChk.eDataType[10] = GDT_CFloat64;
	pChk->pstrMessage = "ENVI .hdr Labelled(*. )格式:\n数据类型必须为无符号8位、无符号16位、有符号16位、无符号32位、\n有符号32位、浮点32位、浮点64位、复整16位、\n复整32位、复浮点32位、复浮点64位";
	i++;
}