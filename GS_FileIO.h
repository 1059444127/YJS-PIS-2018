//��Ȩ˵����

//����˵����

//�汾�����

#ifndef GS_FILEIO_H_INCLUDED
#define GS_FILEIO_H_INCLUDED

#include "gdal.h"
#include "defdataset.h"

/* -------------------------------------------------------------------- */
/*    1 of Prototypes  ----   GS_Open                                   */
/* -------------------------------------------------------------------- */
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
					  int nBandCount = -1, 
					  int *pnBandList = NULL, 
					  int *anWin = NULL,
					  GDALDataType eDataType = GDT_Unknown, 
					  double dScaleSrcMin = 0,
					  double dScaleSrcMax = 0, 
					  double dScaleDstMin = 0, 
					  double dScaleDstMax = 0 );


/* -------------------------------------------------------------------- */
/*    2 of Prototypes  ----   GS_DefOpen                                */
/* -------------------------------------------------------------------- */
/************************************************************************/
/* ��������GS_DefOpen                                                   */
/* �������ܣ�Դ�ļ����Զ��巽ʽ��                                     */
/* ����˵����                                                           */
/*     CString strSrcFileName Դ�ļ�����                                */
/*     GDALAccess eAccess     ��Դ�ļ��ķ������ԣ��μ�gdal.h            */
/*     struct DefParaList     �Զ����ʽ�Ĳ����ṹ                      */
/* ����ֵ��                                                             */
/*     GDALDataset *          Dataset����ָ��                           */
/*                            ��ʧ�ܻ᷵�ؿ�ָ��                      */
/************************************************************************/
GDALDataset *GS_DefOpen( CString strFileName, GDALAccess eAccess,
				         struct DefParaList ParaList);


/* -------------------------------------------------------------------- */
/*     3 of Prototypes  ----   GS_CreateWithHeader                      */
/* -------------------------------------------------------------------- */
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
								  char **papszCreateOptions = NULL,
								  char **papszMetadatas = NULL);


/* -------------------------------------------------------------------- */
/*     4 of Prototypes  ----   GS_CustomCreateWithHeader()              */
/* -------------------------------------------------------------------- */
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
									   char **papszCreateOptions = NULL);


/* -------------------------------------------------------------------- */
/*     5 of Prototypes  ----   GS_CreateCopy                            */
/* -------------------------------------------------------------------- */
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
							BOOL bStrict = FALSE,
							char **papszCreateOptions = NULL,
							GDALProgressFunc pfnProgress = NULL,
							void * pProgressData = NULL);

#endif