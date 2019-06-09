//��Ȩ˵����

//����˵����

//�汾�����

#include "stdafx.h"
#include "GS_FileIO.h"

#include "DriverChecker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//func 1
/************************************************************************/
/* ��������GS_Open                                                      */
/* �������ܣ�Դ�ļ������ⷽʽ�򿪣���������Dataset��ָ��                */
/* ����˵����                                                           */
/*     CString strSrcFileName  Դ�ļ�����                               */
/*     GDALAccess eAccess      ��Դ�ļ��ķ������ԣ��μ�gdal.h           */
/*     int nBandCount          �������Ĳ�����(Ĭ��-1)                 */
/*     int *pnBandList         Դ�ļ���Щ����(Ĭ��NULL)                 */
/*     int *anWin              Դ�ļ��ĸ��������ϽǺ����½ǵ�����     */
/*                             (Ĭ��NULL)                               */
/*     GDALDataType eDataType  ����������������(Ĭ��GDT_Unknown)      */
/*     int nScaleSrcMin        Դ�ļ�������Χ��Сֵ(Ĭ��0)              */
/*     int nScaleSrcMax        Դ�ļ�������Χ���ֵ(Ĭ��0)              */
/*     int nScaleDstMin        �������������Χ��Сֵ(Ĭ��0)            */
/*     int nScaleDstMax        �������������Χ���ֵ(Ĭ��0)            */
/*                             (���췽��Ϊ��������)                     */
/* ����ֵ��                                                             */
/*     GDALDataset *           ����Dataset����ָ��                      */
/*                             ��ʧ�ܻ᷵�ؿ�ָ��                     */
/************************************************************************/
GDALDataset *GS_Open( CString strSrcFileName, 
					  GDALAccess eAccess, 
					  int nBandCount/* = -1 */, 
					  int *pnBandList/* = NULL */, 
					  int *anWin/* = NULL */,
					  GDALDataType eDataType/* = GDT_UnKnown */, 
					  double dScaleSrcMin/* = 0 */,
					  double dScaleSrcMax/* = 0 */, 
					  double dScaleDstMin/* = 0 */, 
					  double dScaleDstMax/* = 0 */ )
{
	GDALDatasetH hDataset = NULL;

	hDataset = GDALOpen(strSrcFileName,eAccess);

	if(hDataset == NULL)
	{
		return NULL;
	}

	GDALDatasetH hDatasetVrt = NULL;

	hDatasetVrt = GDALVrtOpen( hDataset, nBandCount,pnBandList, 
		                       anWin, eDataType,
							   dScaleSrcMin,dScaleSrcMax,
							   dScaleDstMin,dScaleDstMax);
	
	if( hDataset != hDatasetVrt )
	{
		GDALClose( hDataset );
	}

	return (GDALDataset *)hDatasetVrt;
}

//func 2
/************************************************************************/
/* ��������GS_DefOpen                                                   */
/* �������ܣ����Զ����ʽ�ļ�                                         */
/* ����˵����                                                           */
/*     CString strSrcFileName Դ�ļ�����                                */
/*     GDALAccess eAccess     ��Դ�ļ��ķ������ԣ��μ�gdal.h            */
/*     struct DefParaList     �Զ����ʽ�Ĳ����ṹ                      */
/* ����ֵ��                                                             */
/*     GDALDataset *          Dataset����ָ��                           */
/*                            ��ʧ�ܻ᷵�ؿ�ָ��                      */
/************************************************************************/
GDALDataset *GS_DefOpen( CString strFileName, 
				         GDALAccess eAccess,
				         struct DefParaList ParaList )
{
	GDALOpenInfo oOpenInfo( strFileName, eAccess);

    CPLErrorReset();

	if( oOpenInfo.fp == NULL )
        return NULL;

	GDALDataset *poDS = NULL;
	poDS = DefDataset::Open( &oOpenInfo, ParaList );

	return poDS;
}

//func3
/************************************************************************/
/* ��������GS_CreateWithHeader                                          */
/* �������ܣ�����Ŀ���ļ�Dataset����Dataset����Դ�ļ�Datase�г�ͼ������ */
/*           ֮���������Ϣ������ͶӰ��Ϣ��Ԫ���ݡ�LUT���              */
/* ����˵����                                                           */
/*     CString strDriverName  Ŀ���ļ���ʽ��Driver�����֣��μ�gdal.h    */
/*     CString strDstFileName Ŀ���ļ�����                              */
/*     GDALDataset *poDataset Դ�ļ�Datasetָ��                         */
/*     const char **papszCreateOptions Ŀ���ļ�����ѡ��(Ĭ��NULL)       */
/*     const char **papszMetadatas     ��Ŀ���ļ���ӵ�Ԫ����(Ĭ��NULL) */
/* ����ֵ��                                                             */
/*     GDALDataset *                   Ŀ���ļ�Dataset                  */
/*                                     ����ʧ�ܻ᷵�ؿ�ָ��             */
/************************************************************************/
GDALDataset *GS_CreateWithHeader( CString strDriverName,
								  CString strDstFileName,
								  GDALDataset *poDataset,
								  char **papszCreateOptions/* = NULL */,
								  char **papszMetadatas/* = NULL */ )
{
	if( NULL == poDataset)
	{
		//AfxMessageBox("��ȷ��Դ�ļ��Ѿ��򿪣�");
		return NULL;
	}

	GDALDriverH hDriver = NULL;
	hDriver = GDALGetDriverByName(strDriverName);
	
	if( NULL == hDriver )
	{
		//AfxMessageBox("��ȷ�ϸø�ʽ�Ƿ�Ϊ֧�ֵĸ�ʽ��");
		return NULL;
	}

	int nCheck = 
		GS_GetDriverChecker()->DriverCheck(strDriverName,
		                                   poDataset->GetRasterBand(1)->GetRasterDataType(),
									       poDataset->GetRasterCount());

	if(FALSE == nCheck)
	{
		CString strErrorMsg = GS_GetDriverChecker()->GetErrorMsg();
		AfxMessageBox(strErrorMsg);

		return NULL;
	}

	GDALDatasetH hOutDS = NULL;
	hOutDS = GDALCreateWithHeader(hDriver, 
		                          strDstFileName, 
		                          (GDALDatasetH)poDataset,
								  papszMetadatas,
		                          papszCreateOptions);

	return (GDALDataset *)hOutDS;
}

//func4
/************************************************************************/
/* ��������GS_CustomCreateWithHeader                                    */
/* �������ܣ��Զ��崴��Ŀ���ļ�Dataset����Dataset����Դ�ļ�Datase�г�ͼ */
/*           ������֮���������Ϣ������ͶӰ��Ϣ��Ԫ���ݡ�LUT���        */
/* ����˵����                                                           */
/*     CString strDriverName  Ŀ���ļ���ʽ��Driver�����֣��μ�gdal.h    */
/*     CString strDstFileName Ŀ���ļ�����                              */
/*     GDALDataset *poDataset Դ�ļ�Datasetָ��                         */
/*     int  nRasterXSize      Ŀ���ļ��Ŀ�                              */
/*     int  nRasterYSize      Ŀ���ļ��ĸ�                              */
/*     int  nBandCount        Ŀ���ļ��Ĳ�����                          */
/*     GDALDataType eDataType Ŀ���ļ�����������                        */
/*     const char **papszCreateOptions Ŀ���ļ�����ѡ��(Ĭ��NULL)       */
/* ����ֵ��                                                             */
/*     GDALDataset *                   Ŀ���ļ�Dataset                  */
/*                                     ����ʧ�ܻ᷵�ؿ�ָ��             */
/************************************************************************/
GDALDataset *GS_CustomCreateWithHeader(CString strDriverName,
								       CString strDstFileName,
								       GDALDataset *poDataset,
									   int nRasterXSize,
									   int nRasterYSize,
									   int nBandCount,
									   GDALDataType eDataType,
								       char **papszCreateOptions)
{
	if( NULL == poDataset)
	{
		//AfxMessageBox("��ȷ��Դ�ļ��Ѿ��򿪣�");
		return NULL;
	}
	
	GDALDriverH hDriver = NULL;
	hDriver = GDALGetDriverByName(strDriverName);
	
	if( NULL == hDriver )
	{
		//AfxMessageBox("��ȷ�ϸø�ʽ�Ƿ�Ϊ֧�ֵĸ�ʽ��");
		return NULL;
	}

	int nCheck = 
		GS_GetDriverChecker()->DriverCheck(strDriverName, eDataType, nBandCount);
	
	if(FALSE == nCheck)
	{
		CString strErrorMsg = GS_GetDriverChecker()->GetErrorMsg();
		AfxMessageBox(strErrorMsg);
		
		return NULL;
	}
	
	GDALDatasetH hOutDS = NULL;
	hOutDS = GDALCustomCreateWithHeader( hDriver, 
		                                  strDstFileName, 
		                                  (GDALDatasetH)poDataset,
										  nRasterXSize,
										  nRasterYSize,
										  nBandCount,
										  eDataType,
		                                  papszCreateOptions);
	
	return (GDALDataset *)hOutDS;
}

//func5
/************************************************************************/
/* ��������GS_CreateCopy                                                */
/* �������ܣ�����Ŀ���ļ�Dataset����Dataset��Դ�ļ�Datase������ͬ��Ϣ   */
/* ����˵����                                                           */
/*     CString strDriverName   Ŀ���ļ���ʽ��Driver�����֣��μ�gdal.h   */
/*     CString strDstFileName  Ŀ���ļ�����                             */
/*     GDALDataset *poDataset  Դ�ļ�Datasetָ��                        */
/*     BOOL bStrict            �������Ƶı�־(Ĭ��FALSE)                */
/*     const char **papszCreateOptions Ŀ���ļ�����ѡ��(Ĭ��NULL)       */
/*     const char **papszMetadatas     ��Ŀ���ļ���ӵ�Ԫ����(Ĭ��NULL) */
/*     GDALProgressFunc pfnProgress    �ص�����ָ��(Ĭ��NULL)           */
/*     void * pProgressData            �ص�������ʾ����ʾ����(Ĭ��NULL) */
/* ����ֵ��                                                             */
/*     GDALDataset *                   Ŀ���ļ�Dataset                  */
/*                                     ����ʧ�ܻ᷵�ؿ�ָ��             */
/************************************************************************/
GDALDataset *GS_CreateCopy( CString strDriverName,
						    CString strDstFileName,
						    GDALDataset *poDataset,
						    BOOL bStrict/* = FALSE */,
						    char **papszCreateOptions/* = NULL */,
						    GDALProgressFunc pfnProgress/* = NULL */,
						    void * pProgressData/* = NULL */ )
{
	if( NULL == poDataset)
	{
		//AfxMessageBox("Դ�ļ�û����ȷ�򿪣�");
		return NULL;
	}

	GDALDriverH hDriver = NULL;
	hDriver = GDALGetDriverByName(strDriverName);
	
	if( NULL == hDriver )
	{
		//AfxMessageBox("ָ��Ŀ���ļ��ĸ�ʽ��֧�֣�");
		return NULL;
	}

	int nCheck = 
		GS_GetDriverChecker()->DriverCheck(strDriverName,
										   poDataset->GetRasterBand(1)->GetRasterDataType(),
		                                   poDataset->GetRasterCount());
	
	if(FALSE == nCheck)
	{
		CString strErrorMsg = GS_GetDriverChecker()->GetErrorMsg();
		AfxMessageBox(strErrorMsg);
		
		return NULL;
	}

	GDALDatasetH hOutDS = NULL;
	hOutDS = GDALCreateCopy(hDriver, strDstFileName,
		(GDALDatasetH)poDataset,
		bStrict, papszCreateOptions, 
		pfnProgress, pProgressData );
	
	
	return (GDALDataset *)hOutDS;
}