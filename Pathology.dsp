# Microsoft Developer Studio Project File - Name="Pathology" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Pathology - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Pathology.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Pathology.mak" CFG="Pathology - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Pathology - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Pathology - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Pathology - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Od /I "d:\pathology\devtools\codejock\include" /I "d:\pathology\devtools\barcode\include" /I "d:\pathology\devtools\sqlapi\include" /I "d:\pathology\devtools\fo\include" /I "d:\pathology\devtools\psapi\include" /I "..\include\gdal" /I "..\include\utility" /I "..\include\UG97_grid" /I "..\include\HtmlHelp" /I "..\include\Capturer\okapi" /I "..\include\Capturer\DH-HVDevice" /I "..\include\Capturer\dshow" /I "..\include\Capturer\MVC3000" /I "..\include\gain" /I "..\include\VeriSdk" /I "..\include\ftp" /I "..\include\pdflib" /I "..\include\xml" /I "..\include\leadtools" /I "..\include\PSReccSDK" /I "..\jpeg-7" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_DIAGRAM_STATIC_" /D "_XT_STATICLINK" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /i "d:\pathology\devtools\fo\include" /i "d:\pathology\devtools\codejock\include" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ug97.lib TBarCode.lib sqlapis.lib tinyxml.lib gdal_i.lib UtilityLib.lib ws2_32.lib HtmlHelp.lib OKAPI32.lib gain.lib VFPSimg.lib imgstuff.lib vmatcher.lib fpfltr5.lib kyqual.lib HVDAILT.lib HVUtil.lib Raw2Rgb.lib MVCAPI.lib DShowGrab.lib pdflib.lib Version.lib psapi.lib jpeg.lib prsdk.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /out:".\bin\Release/Pathology.exe" /libpath:"d:\pathology\devtools\codejock\lib" /libpath:"d:\pathology\devtools\barcode\lib" /libpath:"d:\pathology\devtools\sqlapi\lib" /libpath:"d:\pathology\devtools\fo\lib" /libpath:"d:\pathology\devtools\psapi\lib" /libpath:"..\lib\release" /libpath:"..\Jpeg-7\release"

!ELSEIF  "$(CFG)" == "Pathology - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "d:\pathology\devtools\codejock\include" /I "d:\pathology\devtools\barcode\include" /I "d:\pathology\devtools\sqlapi\include" /I "d:\pathology\devtools\fo\include" /I "d:\pathology\devtools\psapi\include" /I "d:\pathology\devtools\dcrf32\include" /I "..\include\gdal" /I "..\include\utility" /I "..\include\UG97_grid" /I "..\include\HtmlHelp" /I "..\include\Capturer\okapi" /I "..\include\Capturer\DH-HVDevice" /I "..\include\Capturer\dshow" /I "..\include\Capturer\MVC3000" /I "..\include\gain" /I "..\include\VeriSdk" /I "..\include\ftp" /I "..\include\pdflib" /I "..\include\xml" /I "..\include\leadtools" /I "..\include\PSReccSDK" /I "..\jpeg-7" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_DIAGRAM_STATIC_" /D "_XT_STATICLINK" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /i "d:\pathology\devtools\fo\include" /i "d:\pathology\devtools\codejock\include" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ug97.lib TBarCode.lib sqlapis.lib tinyxml.lib gdal_i.lib UtilityLib.lib ws2_32.lib HtmlHelp.lib OKAPI32.lib gain.lib VFPSimg.lib imgstuff.lib vmatcher.lib fpfltr5.lib kyqual.lib HVDAILT.lib HVUtil.lib Raw2Rgb.lib MVCAPI.lib DShowGrab.lib pdflib.lib Version.lib psapi.lib prsdk.lib jpeg.lib /nologo /subsystem:windows /debug /machine:I386 /out:".\bin\Debug/Pathology.exe" /pdbtype:sept /libpath:"d:\pathology\devtools\codejock\lib" /libpath:"d:\pathology\devtools\barcode\lib" /libpath:"d:\pathology\devtools\sqlapi\lib" /libpath:"d:\pathology\devtools\fo\lib" /libpath:"d:\pathology\devtools\psapi\lib" /libpath:"d:\pathology\devtools\dcrf32\lib" /libpath:"..\lib\release" /libpath:"..\Jpeg-7\release"

!ENDIF 

# Begin Target

# Name "Pathology - Win32 Release"
# Name "Pathology - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\activemovie.cpp
# End Source File
# Begin Source File

SOURCE=.\AdjustDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AdviceDelReason.cpp
# End Source File
# Begin Source File

SOURCE=.\AdviceGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\AdvicePreGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\AdviceShow.cpp
# End Source File
# Begin Source File

SOURCE=.\AdviceTip.cpp
# End Source File
# Begin Source File

SOURCE=.\AreaOut.cpp
# End Source File
# Begin Source File

SOURCE=.\BatchPrintCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BbmxDetail.cpp
# End Source File
# Begin Source File

SOURCE=.\BbmxGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\BBYYTIP.cpp
# End Source File
# Begin Source File

SOURCE=.\Bcbg.cpp
# End Source File
# Begin Source File

SOURCE=.\Bdbg.cpp
# End Source File
# Begin Source File

SOURCE=.\BdSel.cpp
# End Source File
# Begin Source File

SOURCE=.\BdyyGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\BdyyShow.cpp
# End Source File
# Begin Source File

SOURCE=.\BitmapPicture.cpp
# End Source File
# Begin Source File

SOURCE=.\BookingTip.cpp
# End Source File
# Begin Source File

SOURCE=.\Bpsmhd.cpp
# End Source File
# Begin Source File

SOURCE=.\BqlbGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\BqlbShow.cpp
# End Source File
# Begin Source File

SOURCE=.\BuweiBZ.cpp
# End Source File
# Begin Source File

SOURCE=.\CameraControl.cpp
# End Source File
# Begin Source File

SOURCE=.\CannedTextTree.cpp
# End Source File
# Begin Source File

SOURCE=.\CanvasImage.cpp
# End Source File
# Begin Source File

SOURCE=.\CaptureCanonS5Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CgsfqrGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\ChargeConfirm.cpp
# End Source File
# Begin Source File

SOURCE=.\ChargeDetail.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckLicense.cpp
# End Source File
# Begin Source File

SOURCE=.\Ckyj.cpp
# End Source File
# Begin Source File

SOURCE=.\CompositeQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\ControllerComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ControllerComboBox.h
# End Source File
# Begin Source File

SOURCE=.\ControllerEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\ControllerEdit.h
# End Source File
# Begin Source File

SOURCE=.\CPropPage.cpp
# End Source File
# Begin Source File

SOURCE=.\CPropPage.h
# End Source File
# Begin Source File

SOURCE=.\CreateReport.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomItemFont.cpp
# End Source File
# Begin Source File

SOURCE=.\DalsuRichEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\DbfStandard.cpp
# End Source File
# Begin Source File

SOURCE=.\DBGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\DBList.cpp
# End Source File
# Begin Source File

SOURCE=.\DbSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\DbType.cpp
# End Source File
# Begin Source File

SOURCE=.\DCCache.cpp
# End Source File
# Begin Source File

SOURCE=.\devices.cpp
# End Source File
# Begin Source File

SOURCE=.\DevicesPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DevicesPage.h
# End Source File
# Begin Source File

SOURCE=.\DIBAPI.CPP
# End Source File
# Begin Source File

SOURCE=.\DlgDummyView.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGamma1.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgListInput.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPreviewView.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPropertyLine.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPropertyText.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgReflection.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRotation.cpp
# End Source File
# Begin Source File

SOURCE=.\DriverChecker.cpp
# End Source File
# Begin Source File

SOURCE=.\DsControllerMainSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\DShowSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\DummyView.cpp
# End Source File
# Begin Source File

SOURCE=.\EditReport.cpp
# End Source File
# Begin Source File

SOURCE=.\EnumPrinters.cpp
# End Source File
# Begin Source File

SOURCE=.\Equal.cpp
# End Source File
# Begin Source File

SOURCE=.\EventSink.cpp
# End Source File
# Begin Source File

SOURCE=.\EventSink.h
# End Source File
# Begin Source File

SOURCE=.\ExtDataModel.cpp
# End Source File
# Begin Source File

SOURCE=.\FalseNoQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\FBDefines.h
# End Source File
# Begin Source File

SOURCE=.\FieldOut.cpp
# End Source File
# Begin Source File

SOURCE=.\Figure.cpp
# End Source File
# Begin Source File

SOURCE=.\FigureView.cpp
# End Source File
# Begin Source File

SOURCE=.\FileWriter.cpp
# End Source File
# Begin Source File

SOURCE=.\FileWriter.h
# End Source File
# Begin Source File

SOURCE=.\FmtSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\FOBarCodeShape.cpp
# End Source File
# Begin Source File

SOURCE=.\FOFieldShape.cpp
# End Source File
# Begin Source File

SOURCE=.\FOPictureShape.cpp
# End Source File
# Begin Source File

SOURCE=.\FormDataModel.cpp
# End Source File
# Begin Source File

SOURCE=.\FrameGrabber.cpp
# End Source File
# Begin Source File

SOURCE=.\GDIThread.cpp
# End Source File
# Begin Source File

SOURCE=.\GetCode.cpp
# End Source File
# Begin Source File

SOURCE=.\GetStudyUID.cpp
# End Source File
# Begin Source File

SOURCE=.\GS_FileIO.cpp
# End Source File
# Begin Source File

SOURCE=.\Gzddy.cpp
# End Source File
# Begin Source File

SOURCE=.\HISConnectClient.cpp
# End Source File
# Begin Source File

SOURCE=.\HISConnectClient1.cpp
# End Source File
# Begin Source File

SOURCE=.\HISConnectData.cpp
# End Source File
# Begin Source File

SOURCE=.\HistoryGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\HistoryShow.cpp
# End Source File
# Begin Source File

SOURCE=.\HzlbGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\HzlbShow.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageDockWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageView.cpp
# End Source File
# Begin Source File

SOURCE=.\INI.cpp
# End Source File
# Begin Source File

SOURCE=.\Input.cpp
# End Source File
# Begin Source File

SOURCE=.\Jsyz.cpp
# End Source File
# Begin Source File

SOURCE=.\Knhz.cpp
# End Source File
# Begin Source File

SOURCE=.\KnlyGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\KnlyShow.cpp
# End Source File
# Begin Source File

SOURCE=.\KSFileDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\LeaveMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\ListInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\LKBS.cpp
# End Source File
# Begin Source File

SOURCE=.\LogoGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\LogoShow.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MantenancePicNote.cpp
# End Source File
# Begin Source File

SOURCE=.\Markup.cpp
# End Source File
# Begin Source File

SOURCE=.\MaterialGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\md5.cpp
# End Source File
# Begin Source File

SOURCE=.\MeasureDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MeasureGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\MedAdvice.cpp
# End Source File
# Begin Source File

SOURCE=.\MedDiagnose.cpp
# End Source File
# Begin Source File

SOURCE=.\MedImage.cpp
# End Source File
# Begin Source File

SOURCE=.\MedMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\MedPerson.cpp
# End Source File
# Begin Source File

SOURCE=.\MedPreslice.cpp
# End Source File
# Begin Source File

SOURCE=.\MedSlice.cpp
# End Source File
# Begin Source File

SOURCE=.\MedSound.cpp
# End Source File
# Begin Source File

SOURCE=.\MedTbs.cpp
# End Source File
# Begin Source File

SOURCE=.\MedWax.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MicroWriterConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\MicroWriterThread.cpp
# End Source File
# Begin Source File

SOURCE=.\MipPropLink.h
# End Source File
# Begin Source File

SOURCE=.\MovieShow.cpp
# End Source File
# Begin Source File

SOURCE=.\mscomm.cpp
# End Source File
# Begin Source File

SOURCE=.\MVCSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\MyEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MYFILE.CPP
# End Source File
# Begin Source File

SOURCE=.\MyFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\MyPreviewView.cpp
# End Source File
# Begin Source File

SOURCE=.\MyProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTabCtrlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\NewUser.cpp
# End Source File
# Begin Source File

SOURCE=.\NormalSfqr.cpp
# End Source File
# Begin Source File

SOURCE=.\NormalWord.cpp
# End Source File
# Begin Source File

SOURCE=.\NotePad.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify.cpp
# End Source File
# Begin Source File

SOURCE=.\NoUse.cpp
# End Source File
# Begin Source File

SOURCE=.\NumInput.cpp
# End Source File
# Begin Source File

SOURCE=.\ofcrc32.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputBar.cpp
# End Source File
# Begin Source File

SOURCE=.\PassManage.cpp
# End Source File
# Begin Source File

SOURCE=.\PassModify.cpp
# End Source File
# Begin Source File

SOURCE=.\Pathology.cpp
# End Source File
# Begin Source File

SOURCE=.\Pathology.rc
# End Source File
# Begin Source File

SOURCE=.\PathologyDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\PathologyView.cpp
# End Source File
# Begin Source File

SOURCE=.\Picnote.cpp
# End Source File
# Begin Source File

SOURCE=.\PicNoteGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\PicNoteMaintenance.cpp
# End Source File
# Begin Source File

SOURCE=.\PreSliceGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintPreviewDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PROGRESS.CPP
# End Source File
# Begin Source File

SOURCE=.\PromptInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyBag.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyBag.h
# End Source File
# Begin Source File

SOURCE=.\Qcmx.cpp
# End Source File
# Begin Source File

SOURCE=.\QcyzBZ.cpp
# End Source File
# Begin Source File

SOURCE=.\Qppj.cpp
# End Source File
# Begin Source File

SOURCE=.\QPSMINPUT.cpp
# End Source File
# Begin Source File

SOURCE=.\Qpyj.cpp
# End Source File
# Begin Source File

SOURCE=.\QuickQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\RecordInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\RecordInput.cpp
# End Source File
# Begin Source File

SOURCE=.\RecordTiming.cpp
# End Source File
# Begin Source File

SOURCE=.\Reporting.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportingFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportingView.cpp
# End Source File
# Begin Source File

SOURCE=.\Resize.cpp
# End Source File
# Begin Source File

SOURCE=.\RightGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\Security.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectBZ.cpp
# End Source File
# Begin Source File

SOURCE=.\SelfDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\SelfGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\SelfInput.cpp
# End Source File
# Begin Source File

SOURCE=.\SelfPrompt.cpp
# End Source File
# Begin Source File

SOURCE=.\SfDetailGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\Sfjg.cpp
# End Source File
# Begin Source File

SOURCE=.\SflbGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\SflbShow.cpp
# End Source File
# Begin Source File

SOURCE=.\SliceGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\SliceQualityGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StandInput.cpp
# End Source File
# Begin Source File

SOURCE=.\Statistic.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StreamPage.cpp
# End Source File
# Begin Source File

SOURCE=.\StreamPage.h
# End Source File
# Begin Source File

SOURCE=.\StrEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\SyncSerialComm.cpp
# End Source File
# Begin Source File

SOURCE=.\SysConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\TabDockWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\TagGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\TagShow.cpp
# End Source File
# Begin Source File

SOURCE=.\TextOut.cpp
# End Source File
# Begin Source File

SOURCE=.\Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\Tjyz.cpp
# End Source File
# Begin Source File

SOURCE=.\UserGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\videocap.cpp
# End Source File
# Begin Source File

SOURCE=.\videoformats.cpp
# End Source File
# Begin Source File

SOURCE=.\videoinputs.cpp
# End Source File
# Begin Source File

SOURCE=.\Wax2.cpp
# End Source File
# Begin Source File

SOURCE=.\WaxGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\WBDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Xgyj.cpp
# End Source File
# Begin Source File

SOURCE=.\YQJL.CPP
# End Source File
# Begin Source File

SOURCE=.\YQJLGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\YzGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\YzRealTimeGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\ZCGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\ZCQR.cpp
# End Source File
# Begin Source File

SOURCE=.\Zdex.cpp
# End Source File
# Begin Source File

SOURCE=.\ZhengDuan.cpp
# End Source File
# Begin Source File

SOURCE=.\Zpqk.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\activemovie.h
# End Source File
# Begin Source File

SOURCE=.\AdjustDlg.h
# End Source File
# Begin Source File

SOURCE=.\AdviceDelReason.h
# End Source File
# Begin Source File

SOURCE=.\AdviceGrid.h
# End Source File
# Begin Source File

SOURCE=.\AdvicePreGrid.h
# End Source File
# Begin Source File

SOURCE=.\AdviceShow.h
# End Source File
# Begin Source File

SOURCE=.\AdviceTip.h
# End Source File
# Begin Source File

SOURCE=.\AreaOut.h
# End Source File
# Begin Source File

SOURCE=.\BatchPrintCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BbmxDetail.h
# End Source File
# Begin Source File

SOURCE=.\BbmxGrid.h
# End Source File
# Begin Source File

SOURCE=.\BBYYTIP.h
# End Source File
# Begin Source File

SOURCE=.\Bcbg.h
# End Source File
# Begin Source File

SOURCE=.\Bdbg.h
# End Source File
# Begin Source File

SOURCE=.\BdSel.h
# End Source File
# Begin Source File

SOURCE=.\BdyyGrid.h
# End Source File
# Begin Source File

SOURCE=.\BdyyShow.h
# End Source File
# Begin Source File

SOURCE=.\BitmapPicture.h
# End Source File
# Begin Source File

SOURCE=.\BookingTip.h
# End Source File
# Begin Source File

SOURCE=.\Bpsmhd.h
# End Source File
# Begin Source File

SOURCE=.\BqlbGrid.h
# End Source File
# Begin Source File

SOURCE=.\BqlbShow.h
# End Source File
# Begin Source File

SOURCE=.\BuweiBZ.h
# End Source File
# Begin Source File

SOURCE=.\CameraControl.h
# End Source File
# Begin Source File

SOURCE=.\CannedTextTree.h
# End Source File
# Begin Source File

SOURCE=.\CanvasImage.h
# End Source File
# Begin Source File

SOURCE=.\CaptureCanonS5Dlg.h
# End Source File
# Begin Source File

SOURCE=.\CgsfqrGrid.h
# End Source File
# Begin Source File

SOURCE=.\ChargeConfirm.h
# End Source File
# Begin Source File

SOURCE=.\ChargeDetail.h
# End Source File
# Begin Source File

SOURCE=.\CheckLicense.h
# End Source File
# Begin Source File

SOURCE=.\Ckyj.h
# End Source File
# Begin Source File

SOURCE=.\CompositeQuery.h
# End Source File
# Begin Source File

SOURCE=.\CreateReport.h
# End Source File
# Begin Source File

SOURCE=.\CustomItemFont.h
# End Source File
# Begin Source File

SOURCE=.\DalsuRichEdit.h
# End Source File
# Begin Source File

SOURCE=.\DbfStandard.h
# End Source File
# Begin Source File

SOURCE=.\DBGrid.h
# End Source File
# Begin Source File

SOURCE=.\DBList.h
# End Source File
# Begin Source File

SOURCE=.\DbSelect.h
# End Source File
# Begin Source File

SOURCE=.\DbType.h
# End Source File
# Begin Source File

SOURCE=.\DCCache.h
# End Source File
# Begin Source File

SOURCE=.\devices.h
# End Source File
# Begin Source File

SOURCE=.\DIBAPI.H
# End Source File
# Begin Source File

SOURCE=.\DlgDummyView.h
# End Source File
# Begin Source File

SOURCE=.\DlgGamma1.h
# End Source File
# Begin Source File

SOURCE=.\DlgListInput.h
# End Source File
# Begin Source File

SOURCE=.\DlgPreviewView.h
# End Source File
# Begin Source File

SOURCE=.\DlgPropertyLine.h
# End Source File
# Begin Source File

SOURCE=.\DlgPropertyText.h
# End Source File
# Begin Source File

SOURCE=.\DlgReflection.h
# End Source File
# Begin Source File

SOURCE=.\DlgRotation.h
# End Source File
# Begin Source File

SOURCE=.\DriverChecker.h
# End Source File
# Begin Source File

SOURCE=.\DsControllerMainSheet.h
# End Source File
# Begin Source File

SOURCE=.\DShowSetup.h
# End Source File
# Begin Source File

SOURCE=.\DummyView.h
# End Source File
# Begin Source File

SOURCE=.\EditReport.h
# End Source File
# Begin Source File

SOURCE=.\EnumPrinters.h
# End Source File
# Begin Source File

SOURCE=.\Equal.h
# End Source File
# Begin Source File

SOURCE=.\ExtDataModel.h
# End Source File
# Begin Source File

SOURCE=.\FalseNoQuery.h
# End Source File
# Begin Source File

SOURCE=.\FieldOut.h
# End Source File
# Begin Source File

SOURCE=.\Figure.h
# End Source File
# Begin Source File

SOURCE=.\FigureView.h
# End Source File
# Begin Source File

SOURCE=.\FmtSelect.h
# End Source File
# Begin Source File

SOURCE=.\FOBarCodeShape.h
# End Source File
# Begin Source File

SOURCE=.\FOFieldShape.h
# End Source File
# Begin Source File

SOURCE=.\FOPictureShape.h
# End Source File
# Begin Source File

SOURCE=.\FormDataModel.h
# End Source File
# Begin Source File

SOURCE=.\FrameGrabber.h
# End Source File
# Begin Source File

SOURCE=.\GDIThread.h
# End Source File
# Begin Source File

SOURCE=.\GetCode.h
# End Source File
# Begin Source File

SOURCE=.\GetStudyUID.h
# End Source File
# Begin Source File

SOURCE=.\GS_FileIO.h
# End Source File
# Begin Source File

SOURCE=.\Gzddy.h
# End Source File
# Begin Source File

SOURCE=.\hasphl.h
# End Source File
# Begin Source File

SOURCE=.\HISConnectClient.h
# End Source File
# Begin Source File

SOURCE=.\HISConnectClient1.h
# End Source File
# Begin Source File

SOURCE=.\HISConnectData.h
# End Source File
# Begin Source File

SOURCE=.\HistoryGrid.h
# End Source File
# Begin Source File

SOURCE=.\HistoryShow.h
# End Source File
# Begin Source File

SOURCE=.\HzlbGrid.h
# End Source File
# Begin Source File

SOURCE=.\HzlbShow.h
# End Source File
# Begin Source File

SOURCE=.\ImageDockWnd.h
# End Source File
# Begin Source File

SOURCE=.\ImageView.h
# End Source File
# Begin Source File

SOURCE=.\INI.h
# End Source File
# Begin Source File

SOURCE=.\Input.h
# End Source File
# Begin Source File

SOURCE=.\Jsyz.h
# End Source File
# Begin Source File

SOURCE=.\Knhz.h
# End Source File
# Begin Source File

SOURCE=.\KnlyGrid.h
# End Source File
# Begin Source File

SOURCE=.\KnlyShow.h
# End Source File
# Begin Source File

SOURCE=.\KSFileDialog.h
# End Source File
# Begin Source File

SOURCE=.\LeaveMessage.h
# End Source File
# Begin Source File

SOURCE=.\ListInfo.h
# End Source File
# Begin Source File

SOURCE=.\LKBS.h
# End Source File
# Begin Source File

SOURCE=.\LogoGrid.h
# End Source File
# Begin Source File

SOURCE=.\LogoShow.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MantenancePicNote.h
# End Source File
# Begin Source File

SOURCE=.\Markup.h
# End Source File
# Begin Source File

SOURCE=.\MaterialGrid.h
# End Source File
# Begin Source File

SOURCE=.\md5.h
# End Source File
# Begin Source File

SOURCE=.\MeasureDialog.h
# End Source File
# Begin Source File

SOURCE=.\MeasureGrid.h
# End Source File
# Begin Source File

SOURCE=.\MedAdvice.h
# End Source File
# Begin Source File

SOURCE=.\MedDiagnose.h
# End Source File
# Begin Source File

SOURCE=.\MedImage.h
# End Source File
# Begin Source File

SOURCE=.\MedMaterial.h
# End Source File
# Begin Source File

SOURCE=.\MedPerson.h
# End Source File
# Begin Source File

SOURCE=.\MedPreslice.h
# End Source File
# Begin Source File

SOURCE=.\MedSlice.h
# End Source File
# Begin Source File

SOURCE=.\MedSound.h
# End Source File
# Begin Source File

SOURCE=.\MedTbs.h
# End Source File
# Begin Source File

SOURCE=.\MedWax.h
# End Source File
# Begin Source File

SOURCE=.\MenuEdit.h
# End Source File
# Begin Source File

SOURCE=.\MicroWriterConfig.h
# End Source File
# Begin Source File

SOURCE=.\MicroWriterThread.h
# End Source File
# Begin Source File

SOURCE=.\MovieShow.h
# End Source File
# Begin Source File

SOURCE=.\mscomm.h
# End Source File
# Begin Source File

SOURCE=.\MVCSetup.h
# End Source File
# Begin Source File

SOURCE=.\MyEdit.h
# End Source File
# Begin Source File

SOURCE=.\MyFrame.h
# End Source File
# Begin Source File

SOURCE=.\MyPreviewView.h
# End Source File
# Begin Source File

SOURCE=.\MyProgressBar.h
# End Source File
# Begin Source File

SOURCE=.\MyTabCtrlBar.h
# End Source File
# Begin Source File

SOURCE=.\NewUser.h
# End Source File
# Begin Source File

SOURCE=.\NormalSfqr.h
# End Source File
# Begin Source File

SOURCE=.\NormalWord.h
# End Source File
# Begin Source File

SOURCE=.\NotePad.h
# End Source File
# Begin Source File

SOURCE=.\Notify.h
# End Source File
# Begin Source File

SOURCE=.\NoUse.h
# End Source File
# Begin Source File

SOURCE=.\NumInput.h
# End Source File
# Begin Source File

SOURCE=.\ofcrc32.h
# End Source File
# Begin Source File

SOURCE=.\OutputBar.h
# End Source File
# Begin Source File

SOURCE=.\PassManage.h
# End Source File
# Begin Source File

SOURCE=.\PassModify.h
# End Source File
# Begin Source File

SOURCE=.\Pathology.h
# End Source File
# Begin Source File

SOURCE=.\PathologyDoc.h
# End Source File
# Begin Source File

SOURCE=.\PathologyView.h
# End Source File
# Begin Source File

SOURCE=.\Picnote.h
# End Source File
# Begin Source File

SOURCE=.\PicNoteGrid.h
# End Source File
# Begin Source File

SOURCE=.\PicNoteMaintenance.h
# End Source File
# Begin Source File

SOURCE=.\PreSliceGrid.h
# End Source File
# Begin Source File

SOURCE=.\PrintPreviewDialog.h
# End Source File
# Begin Source File

SOURCE=.\Progress.h
# End Source File
# Begin Source File

SOURCE=.\PromptInfo.h
# End Source File
# Begin Source File

SOURCE=.\PropPage.h
# End Source File
# Begin Source File

SOURCE=.\pta.h
# End Source File
# Begin Source File

SOURCE=.\Qcmx.h
# End Source File
# Begin Source File

SOURCE=.\QcyzBZ.h
# End Source File
# Begin Source File

SOURCE=.\Qppj.h
# End Source File
# Begin Source File

SOURCE=.\QPSMINPUT.h
# End Source File
# Begin Source File

SOURCE=.\Qpyj.h
# End Source File
# Begin Source File

SOURCE=.\QuickQuery.h
# End Source File
# Begin Source File

SOURCE=.\RecordInfo.h
# End Source File
# Begin Source File

SOURCE=.\RecordInput.h
# End Source File
# Begin Source File

SOURCE=.\RecordTiming.h
# End Source File
# Begin Source File

SOURCE=.\Reporting.h
# End Source File
# Begin Source File

SOURCE=.\ReportingFrame.h
# End Source File
# Begin Source File

SOURCE=.\ReportingView.h
# End Source File
# Begin Source File

SOURCE=.\Resize.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RightGrid.h
# End Source File
# Begin Source File

SOURCE=.\Security.h
# End Source File
# Begin Source File

SOURCE=.\SelectBZ.h
# End Source File
# Begin Source File

SOURCE=.\SelfDefine.h
# End Source File
# Begin Source File

SOURCE=.\SelfGrid.h
# End Source File
# Begin Source File

SOURCE=.\SelfInput.h
# End Source File
# Begin Source File

SOURCE=.\SelfPrompt.h
# End Source File
# Begin Source File

SOURCE=.\SfDetailGrid.h
# End Source File
# Begin Source File

SOURCE=.\Sfjg.h
# End Source File
# Begin Source File

SOURCE=.\SflbGrid.h
# End Source File
# Begin Source File

SOURCE=.\SflbShow.h
# End Source File
# Begin Source File

SOURCE=.\SliceGrid.h
# End Source File
# Begin Source File

SOURCE=.\SliceQualityGrid.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\StandInput.h
# End Source File
# Begin Source File

SOURCE=.\Statistic.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StrEdit.h
# End Source File
# Begin Source File

SOURCE=.\SyncSerialComm.h
# End Source File
# Begin Source File

SOURCE=.\SysConfig.h
# End Source File
# Begin Source File

SOURCE=.\TabDockWnd.h
# End Source File
# Begin Source File

SOURCE=.\TagGrid.h
# End Source File
# Begin Source File

SOURCE=.\TagShow.h
# End Source File
# Begin Source File

SOURCE=.\TextOut.h
# End Source File
# Begin Source File

SOURCE=.\Thread.h
# End Source File
# Begin Source File

SOURCE=.\TimeQuery.h
# End Source File
# Begin Source File

SOURCE=.\Tjyz.h
# End Source File
# Begin Source File

SOURCE=.\UserGrid.h
# End Source File
# Begin Source File

SOURCE=.\videocap.h
# End Source File
# Begin Source File

SOURCE=.\videoformats.h
# End Source File
# Begin Source File

SOURCE=.\videoinputs.h
# End Source File
# Begin Source File

SOURCE=.\Wax2.h
# End Source File
# Begin Source File

SOURCE=.\WaxGrid.h
# End Source File
# Begin Source File

SOURCE=.\WBDlg.h
# End Source File
# Begin Source File

SOURCE=.\Xgyj.h
# End Source File
# Begin Source File

SOURCE=.\YQJL.H
# End Source File
# Begin Source File

SOURCE=.\YQJLGrid.h
# End Source File
# Begin Source File

SOURCE=.\YzGrid.h
# End Source File
# Begin Source File

SOURCE=.\YzRealTimeGrid.h
# End Source File
# Begin Source File

SOURCE=.\ZCConfirm.h
# End Source File
# Begin Source File

SOURCE=.\ZCGrid.h
# End Source File
# Begin Source File

SOURCE=.\ZCQR.h
# End Source File
# Begin Source File

SOURCE=.\Zdex.h
# End Source File
# Begin Source File

SOURCE=.\ZhengDuan.h
# End Source File
# Begin Source File

SOURCE=.\Zpqk.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\11.bmp
# End Source File
# Begin Source File

SOURCE=.\res\B3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\biggraybar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00015.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_file_1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\database.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fliphori.bmp
# End Source File
# Begin Source File

SOURCE=.\res\flipvert.bmp
# End Source File
# Begin Source File

SOURCE=.\res\GE.ico
# End Source File
# Begin Source File

SOURCE=.\res\H_CROSS.CUR
# End Source File
# Begin Source File

SOURCE=.\res\hand2.CUR
# End Source File
# Begin Source File

SOURCE=.\res\hisidin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00006.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_ok1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_ok2.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\res\il_tab.bmp
# End Source File
# Begin Source File

SOURCE=.\res\imgcap.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ImgTab.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ImgView.bmp
# End Source File
# Begin Source File

SOURCE=.\res\jpg_file_1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\key.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MainIcon.ico
# End Source File
# Begin Source File

SOURCE=.\res\mic.bmp
# End Source File
# Begin Source File

SOURCE=.\res\movie.bmp
# End Source File
# Begin Source File

SOURCE=.\res\multidoc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\orig.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Pathology.ico
# End Source File
# Begin Source File

SOURCE=.\res\Pathology.rc2
# End Source File
# Begin Source File

SOURCE=.\res\PathologyDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\person.bmp
# End Source File
# Begin Source File

SOURCE=.\res\photoin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pp.cur
# End Source File
# Begin Source File

SOURCE=.\res\printer.bmp
# End Source File
# Begin Source File

SOURCE=.\res\printer32.bmp
# End Source File
# Begin Source File

SOURCE=.\res\recycle32.bmp
# End Source File
# Begin Source File

SOURCE=.\res\rotate180.bmp
# End Source File
# Begin Source File

SOURCE=.\res\rotate270.bmp
# End Source File
# Begin Source File

SOURCE=.\res\rotate90.bmp
# End Source File
# Begin Source File

SOURCE=.\res\security.bmp
# End Source File
# Begin Source File

SOURCE=.\res\shoot.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Singleidoc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\stop.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tif.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\videoin.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section Pathology : {AA408ECC-E50B-401F-8036-021E81B9E5A1}
# 	2:21:DefaultSinkHeaderFile:videocap1.h
# 	2:16:DefaultSinkClass:CVideoCap
# End Section
# Section Pathology : {648A5600-2C6E-101B-82B6-000000000014}
# 	2:21:DefaultSinkHeaderFile:mscomm.h
# 	2:16:DefaultSinkClass:CMSComm
# End Section
# Section Pathology : {E6E17E90-DF38-11CF-8E74-00A0C90F26F8}
# 	2:5:Class:CMSComm
# 	2:10:HeaderFile:mscomm.h
# 	2:8:ImplFile:mscomm.cpp
# End Section
# Section Pathology : {B0FB0862-4A4F-4395-BB42-8D31903ED98C}
# 	2:5:Class:CDevices
# 	2:10:HeaderFile:devices1.h
# 	2:8:ImplFile:devices1.cpp
# End Section
# Section Pathology : {72ADFD6C-2C39-11D0-9903-00A0C91BC942}
# 	1:20:CG_IDR_POPUP_MY_EDIT:102
# End Section
# Section Pathology : {43510FCA-3010-43D1-9952-B8B712B9869C}
# 	2:5:Class:CVideoCap
# 	2:10:HeaderFile:videocap1.h
# 	2:8:ImplFile:videocap1.cpp
# End Section
# Section Pathology : {05589FA1-C356-11CE-BF01-00AA0055595A}
# 	2:21:DefaultSinkHeaderFile:activemovie.h
# 	2:16:DefaultSinkClass:CActiveMovie
# End Section
# Section Pathology : {265EC140-AE62-11D1-8500-00A0C91F9CA0}
# 	2:5:Class:CActiveMovie
# 	2:10:HeaderFile:activemovie.h
# 	2:8:ImplFile:activemovie.cpp
# End Section
# Section Pathology : {DD55C4EB-00E7-46AD-8137-E5A3EFA206E8}
# 	2:5:Class:CVideoFormats
# 	2:10:HeaderFile:videoformats1.h
# 	2:8:ImplFile:videoformats1.cpp
# End Section
# Section Pathology : {A6E98D4D-D86D-47D2-9C88-9C9CDA17B09C}
# 	2:5:Class:CVideoInputs
# 	2:10:HeaderFile:videoinputs1.h
# 	2:8:ImplFile:videoinputs1.cpp
# End Section
