//版权说明块

//程序说明块

//版本变更块

#ifndef GS_FILEIO_H_INCLUDED
#define GS_FILEIO_H_INCLUDED

#include "gdal.h"
#include "defdataset.h"

/* -------------------------------------------------------------------- */
/*    1 of Prototypes  ----   GS_Open                                   */
/* -------------------------------------------------------------------- */
/************************************************************************/
/* 函数名：GS_Open                                                      */
/* 函数功能：源文件以虚拟方式打开，返回虚拟Dataset的指针                */
/* 参数说明：                                                           */
/*     CString strSrcFileName  源文件名称                               */
/*     GDALAccess eAccess      对源文件的访问属性，参见gdal.h           */
/*     int nBandCount          虚拟对象的波段数(默认-1)                 */
/*     int *pnBandList         源文件哪些波段(默认NULL)                 */
/*     int *anWin              源文件哪个区域，左上角和右下角的坐标     */
/*                             (默认NULL)                               */
/*     GDALDataType eDataType  虚拟对象的数据类型(默认GDT_Unknown)      */
/*     int nScaleSrcMin        源文件量化范围最小值(默认0)              */
/*     int nScaleSrcMax        源文件量化范围最大值(默认0)              */
/*     int nScaleDstMin        虚拟对象量化范围最小值(默认0)            */
/*     int nScaleDstMax        虚拟对象量化范围最大值(默认0)            */
/*                             (拉伸方法为线性拉伸)                     */
/* 返回值：                                                             */
/*     GDALDataset *           虚拟Dataset对象指针                      */
/*                             打开失败会返回空指针                     */
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
/* 函数名：GS_DefOpen                                                   */
/* 函数功能：源文件以自定义方式打开                                     */
/* 参数说明：                                                           */
/*     CString strSrcFileName 源文件名称                                */
/*     GDALAccess eAccess     对源文件的访问属性，参见gdal.h            */
/*     struct DefParaList     自定义格式的参数结构                      */
/* 返回值：                                                             */
/*     GDALDataset *          Dataset对象指针                           */
/*                            打开失败会返回空指针                      */
/************************************************************************/
GDALDataset *GS_DefOpen( CString strFileName, GDALAccess eAccess,
				         struct DefParaList ParaList);


/* -------------------------------------------------------------------- */
/*     3 of Prototypes  ----   GS_CreateWithHeader                      */
/* -------------------------------------------------------------------- */
/************************************************************************/
/* 函数名：GS_CreateWithHeader                                          */
/* 函数功能：创建目标文件Dataset。该Dataset包含源文件Datase中除图像数据 */
/*           之外的其它信息。例如投影信息、元数据、LUT表等              */
/* 参数说明：                                                           */
/*     CString strDriverName  目标文件格式的Driver短名字，参见gdal.h    */
/*     CString strDstFileName 目标文件名称                              */
/*     GDALDataset *poDataset 源文件Dataset指针                         */
/*     const char **papszCreateOptions 目标文件创建选项(默认NULL)       */
/*     const char **papszMetadatas     向目标文件添加的元数据(默认NULL) */
/* 返回值：                                                             */
/*     GDALDataset *                   目标文件Dataset                  */
/*                                     创建失败会返回空指针             */
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
/* 函数名：GS_CustomCreateWithHeader                                    */
/* 函数功能：自定义创建目标文件Dataset。该Dataset包含源文件Datase中除图 */
/*           像数据之外的其它信息。例如投影信息、元数据、LUT表等        */
/* 参数说明：                                                           */
/*     CString strDriverName  目标文件格式的Driver短名字，参见gdal.h    */
/*     CString strDstFileName 目标文件名称                              */
/*     GDALDataset *poDataset 源文件Dataset指针                         */
/*     int  nRasterXSize      目标文件的宽                              */
/*     int  nRasterYSize      目标文件的高                              */
/*     int  nBandCount        目标文件的波段数                          */
/*     GDALDataType eDataType 目标文件的数据类型                        */
/*     const char **papszCreateOptions 目标文件创建选项(默认NULL)       */
/* 返回值：                                                             */
/*     GDALDataset *                   目标文件Dataset                  */
/*                                     创建失败会返回空指针             */
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
/* 函数名：GS_CreateCopy                                                */
/* 函数功能：创建目标文件Dataset。该Dataset与源文件Datase含有相同信息   */
/* 参数说明：                                                           */
/*     CString strDriverName   目标文件格式的Driver短名字，参见gdal.h   */
/*     CString strDstFileName  目标文件名称                             */
/*     GDALDataset *poDataset  源文件Dataset指针                        */
/*     BOOL bStrict            复制限制的标志(默认FALSE)                */
/*     const char **papszCreateOptions 目标文件创建选项(默认NULL)       */
/*     const char **papszMetadatas     向目标文件添加的元数据(默认NULL) */
/*     GDALProgressFunc pfnProgress    回调函数指针(默认NULL)           */
/*     void * pProgressData            回调函数显示的提示文字(默认NULL) */
/* 返回值：                                                             */
/*     GDALDataset *                   目标文件Dataset                  */
/*                                     创建失败会返回空指针             */
/************************************************************************/
GDALDataset *GS_CreateCopy( CString strDriverName,
						    CString strDstFileName,
							GDALDataset *poDataset,
							BOOL bStrict = FALSE,
							char **papszCreateOptions = NULL,
							GDALProgressFunc pfnProgress = NULL,
							void * pProgressData = NULL);

#endif