// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__04CC6E98_BCA8_4A42_BC34_F3E3123A60ED__INCLUDED_)
#define AFX_STDAFX_H__04CC6E98_BCA8_4A42_BC34_F3E3123A60ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#if _MSC_VER >= 1200
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxinet.h>	    // MFC Internet support


#include <direct.h>

#include <XTToolKit.h>      // Xtreme Toolkit MFC Extensions

#include <SQLAPI.h>

extern SAConnection g_dbconnection;
extern SACommand    g_dbcommand;

#include "ugctrl.h"         // Dundas Ultimate Grid Control Header Files
#include "ugmedit.h"
#include "ugctelps.h"

#include "fo.h"
#include "FBDefines.h"

#include <gdal_priv.h> 
#include "GS_FileIO.h"
#include "DriverChecker.h"

#include "dibapi.h"

#include "CanvasImage.h"

#include "FileProcess.h"

#define	TECIT_DLLIMPORT
#include "TBarCode.h"
#undef	TECIT_DLLIMPORT

#define	THUMBNAIL_WIDTH		100
#define	THUMBNAIL_HEIGHT	75

//Veridicom Include
#include "winapr.h"
#include "VFPSlwlv.h"
#include "VFPSimg.h"
#include "fpapi.h"
#include "imgstuff.h"
#include "idqual.h"

#include <afxwin.h>         // MFC core and standard include files
#include <afxext.h>         // MFC component extensions
#include <afxdisp.h>        // MFC automation classes
#include <afxdtctl.h>		// Internet Explorer 4 common-control MFC classes
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows common controls for MFC applications
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>

#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include "psapi.h"
#include <dcrf32.h>

// If the $(ProgramFiles) environment variable is not "C:\Program Files", 
// please change the following lines:
#import "C:\Program Files\Nikon\Shared\Bin\mipparam2.dll" no_namespace, named_guids
#import "C:\Program Files\Nikon\Shared\Bin\dscamera2.dll" no_namespace, named_guids
#import "C:\Program Files\Nikon\Shared\Bin\dsu2.exe" raw_interfaces_only // prevent duplicate symbols
#define DIID__IDeviceEvents __uuidof(DSCONTROLLERLib::_IDeviceEvents)


#import "ComFileProcessing.dll" no_namespace					//导入Dicom文件解析组件
#import "ComDicomNet.dll" no_namespace							//导入通讯组件

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#include <atlbase.h>

#endif // !defined(AFX_STDAFX_H__04CC6E98_BCA8_4A42_BC34_F3E3123A60ED__INCLUDED_)
