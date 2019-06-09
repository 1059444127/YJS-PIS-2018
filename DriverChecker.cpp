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
		m_strErrorMsg.Format("ѡ�������ļ���ʽΪ��ǰ�汾��֧�ֵĸ�ʽ");
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
	pChk->pstrMessage = "Arc/Info ASCII Grid(*.asc)��ʽ:\n����������Ϊ1��\n�������ͱ���Ϊ�޷���8λ���޷���16λ���з���16λ������32λ\n";
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
	pChk->pstrMessage = "BMP��ʽ(*.BMP)��ʽ:\n����������Ϊ1��3��\n�������ͱ���Ϊ�޷���8λ\n";
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
	pChk->pstrMessage = "��ͼ���ݸ�ʽ(*.kap):\n����������Ϊ1��\n�������ͱ���Ϊ�޷���8λ\n";
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
	pChk->pstrMessage = "DTED �̸߳�ʽ(*.ddf):\n�������ͱ���Ϊ�޷���8λ���޷���16λ���з���16λ\n";
	i++;

	//i = 4;
    pChk = m_aDriverCheck + i;
	pChk->strDriverName = _T("ECW");
	pChk->stcBandChk.eBandLimit = BL_NOLIMIT;
	pChk->stcDataTypeChk.eDataTypeLimit = DTL_LIMIT;
	pChk->stcDataTypeChk.nDataTypeCount = 1;
	pChk->stcDataTypeChk.eDataType[0] = GDT_Byte;//when bStrict == TRUE 
	pChk->pstrMessage = "ERMapper Compressed Wavelets (.ecw)��ʽ:\n�������ͱ���Ϊ�޷���8λ\n";
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
	pChk->pstrMessage = "ELAS��ʽ:\n�������ͱ���Ϊ�޷���8λ������32λ������64λ\n";
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
	pChk->pstrMessage = "FIT��ʽ:\n�������ͱ���Ϊ�޷���8λ���޷���16λ���з���16λ���޷���32λ��\n�з���32λ������32λ������64λ";
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
	pChk->pstrMessage = "GIF��ʽ:\n����������Ϊ1��\n�������ͱ���Ϊ�޷���8λ\n";
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
	pChk->pstrMessage = "GIP(*.gip)��ʽ:\n�������ͱ���Ϊ�޷���8λ���޷���16λ���з���16λ���޷���32λ��\n�з���32λ������32λ������64λ��������32λ��������64λ";
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
	pChk->pstrMessage = "tiff (*.tif)��ʽ:\n�������ͱ���Ϊ�޷���8λ���޷���16λ���з���16λ���޷���32λ��\n�з���32λ������32λ������16λ������32λ��\n������32λ��������64λ";
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
	pChk->pstrMessage = "HDF4Image(*.hdf)��ʽ:\n�������ͱ���Ϊ�޷���8λ���޷���16λ���з���16λ���޷���32λ��\n�з���32λ������32λ������64λ";
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
	pChk->pstrMessage = "ERDAS Imagine IMG(*.img)��ʽ:\n�������ͱ���Ϊ�޷���8λ���޷���16λ���з���16λ���޷���32λ��\n�з���32λ������32λ������64λ��������32λ��������64λ";
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
	pChk->pstrMessage = "JPEG JFIF(*.jpg)��ʽ:\n����������Ϊ1��3���������ͱ���Ϊ�޷���8λ\n";
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
	pChk->pstrMessage = "JPEG2000(*.jp2,*.j2k)��ʽ:\n�������ͱ���Ϊ�޷���8λ���޷���16λ���з���16λ���޷���32λ���з���32λ";
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
	pChk->pstrMessage = "Atlantis MFF Raster(*.hdr)��ʽ:\n�������ͱ���Ϊ�޷���8λ���޷���16λ������32λ������16λ�͸�����32λ";
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
	pChk->pstrMessage = "MFF2(*. )��ʽ:\n�������ͱ���Ϊ�޷���8λ���޷���16λ���з���16λ������32λ\n����16λ������32λ�͸�����32λ";
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
	pChk->pstrMessage = "PCI .aux Labelled(*.pix)��ʽ:\n�������ͱ���Ϊ�޷���8λ���޷���16λ���з���16λ������32λ";
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
	pChk->pstrMessage = "PCIDSK(*.pix)��ʽ:\n�������ͱ���Ϊ�޷���8λ���޷���16λ���з���16λ������32λ";
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
	pChk->pstrMessage = "PNG(*.png)��ʽ:\n����������Ϊ1��2��3��4֮һ��\n�������ͱ���Ϊ�޷���8λ���޷���16λ";
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
	pChk->pstrMessage = "Netpbm(*.pgm, *.ppm)��ʽ:\n����������Ϊ1��3��\n�������ͱ���Ϊ�޷���8λ���޷���16λ";
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
	pChk->pstrMessage = "X11 Pixmap(*.xpm)��ʽ:\n����������Ϊ1��\n�������ͱ���Ϊ�޷���8λ";
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
	pChk->pstrMessage = "National Imagery Transmission Format(*.ntf)��ʽ:\n�������ͱ���Ϊ�޷���8λ���޷���16λ���з���16λ���޷���32λ��\n�з���32λ������32λ��������32λ��������64λ";
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
	pChk->pstrMessage = "ENVI .hdr Labelled(*. )��ʽ:\n�������ͱ���Ϊ�޷���8λ���޷���16λ���з���16λ���޷���32λ��\n�з���32λ������32λ������64λ������16λ��\n����32λ��������32λ��������64λ";
	i++;
}