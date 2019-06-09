// StatisticPrintPreviewDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "PrintPreviewDialog.h"
#include "DlgDummyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintPreviewDialog dialog
extern CPathologyApp theApp;

void CPrintPreviewDialog::DoDataExchange(CDataExchange* pDX)
{
	CXTCBarDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrintPreviewDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrintPreviewDialog, CXTCBarDialog)
	//{{AFX_MSG_MAP(CPrintPreviewDialog)
	ON_COMMAND(ID_DLG_PRINT, OnDialogPrint)
	ON_COMMAND(ID_DLG_PRINTPREVIEW, OnDialogPrintPreview)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintPreviewDialog message handlers

void CPrintPreviewDialog::OnDialogPrint()
{
	int PrinterNum = theApp.m_PrinterControl.GetPrinterCount();
	if(PrinterNum <= 0)
	{
		AfxMessageBox("没有安装打印机!");
		return;
	}

	if(m_pFormDataModel == NULL)   return;

	m_pFormDataModel->ResetAppPrinter();

	if(m_pFormDataModel)
	{
		m_pFormDataModel->ResetContent();

		theApp.ConnectDatabase();
		
		try
		{
			CString Rptname;
			switch(m_UsageType) {
			case 0:
				Rptname.Format("材块移交表");
				break;
			case 1:
				Rptname.Format("切片移交表");
				break;
			case 2:
				Rptname.Format("医嘱工作单");
				break;
			case 3:
				Rptname.Format("制片情况统计表");
				break;
			}
			g_dbcommand.setCommandText("Select rpt_data from REPORTFORMAT where rpt_name = :rpt_name");
			g_dbcommand.Param("rpt_name").setAsString() = Rptname;
			
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				SAString ss = g_dbcommand.Field("rpt_data").asBLob();
				
				CFile file;
				CFileException fe;
				
				if (file.Open( theApp.m_SysTmpPath + Rptname + ".rpt" , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
				{
					const void * binbuf = (const void *)ss;
					file.WriteHuge( binbuf, ss.GetBinaryLength() );
					file.Close();
				}
				
				if( m_pFormDataModel->OpenDocument(theApp.m_SysTmpPath + Rptname + ".rpt") == FALSE )
				{
					return;
				}
				remove(theApp.m_SysTmpPath + Rptname + ".rpt");
			}
			
			g_dbconnection.Commit();
		}
		catch(SAException &)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			//AfxMessageBox((const char*)x.ErrText());
			return;
		}
		
	}

	GenList();

	CDlgDummyView	view;

	view.SetPrintDialog(this);
	view.OnDoFilePrint();
}

void CPrintPreviewDialog::OnDialogPrintPreview()
{
	int PrinterNum = theApp.m_PrinterControl.GetPrinterCount();
	if(PrinterNum <= 0)
	{
		AfxMessageBox("没有安装打印机!");
		return;
	}

	if(m_pFormDataModel == NULL)   return;

	m_pFormDataModel->ResetAppPrinter();

	if(m_pFormDataModel)
	{
		m_pFormDataModel->ResetContent();

		theApp.ConnectDatabase();
		
		try
		{
			CString Rptname;
			switch(m_UsageType) {
			case 0:
				Rptname.Format("材块移交表");
				break;
			case 1:
				Rptname.Format("切片移交表");
				break;
			case 2:
				Rptname.Format("医嘱工作单");
				break;
			case 3:
				Rptname.Format("制片情况统计表");
				break;
			}
			g_dbcommand.setCommandText("Select rpt_data from REPORTFORMAT where rpt_name = :rpt_name");
			g_dbcommand.Param("rpt_name").setAsString() = Rptname;
			
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				SAString ss = g_dbcommand.Field("rpt_data").asBLob();
				
				CFile file;
				CFileException fe;
				
				if (file.Open( theApp.m_SysTmpPath + Rptname + ".rpt" , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
				{
					const void * binbuf = (const void *)ss;
					file.WriteHuge( binbuf, ss.GetBinaryLength() );
					file.Close();
				}
				
				if( m_pFormDataModel->OpenDocument(theApp.m_SysTmpPath + Rptname + ".rpt") == FALSE )
				{
					return;
				}
				remove(theApp.m_SysTmpPath + Rptname + ".rpt");
			}
			
			g_dbconnection.Commit();
		}
		catch(SAException &)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			//AfxMessageBox((const char*)x.ErrText());
			return;
		}
		
	}

	GenList();

	CFrameWnd* pOldFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd;
	pOldFrame->ShowWindow(SW_HIDE);
	if (!m_pTemplate)
	{
		m_pTemplate = new CSingleDocTemplate(
			128,					// so load fram is happy. This matches the IDR_MAINFRAME value from your application!
			NULL,
			RUNTIME_CLASS(CFrameWnd),
			RUNTIME_CLASS(CDlgDummyView));
		//AfxGetApp()->AddDocTemplate(m_pTemplate);
	}
	CFrameWnd* pFrameWnd = m_pTemplate->CreateNewFrame(NULL, NULL);

	// In derived classes, implement special window handling here
	// Be sure to Unhook Frame Window close if hooked.

	// must not create this on the frame.  Must outlive this function
//	CPrintPreviewState* pState = new CPrintPreviewState;
//	pState;
	m_bPrintPreview = true;
	m_pTemplate->InitialUpdateFrame(pFrameWnd, NULL, FALSE);
	m_pView = (CDlgDummyView*)pFrameWnd->GetActiveView();
	AfxGetApp()->m_pMainWnd = pFrameWnd;
	pFrameWnd->SetWindowText(_T("打印预览"));
	pFrameWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pView->SetPrintDialog(this);
	m_pView->m_pOldFrame = pOldFrame;
	m_pView->OnDoPrintPreview();
}

void CPrintPreviewDialog::EndPreviewMode()
{
	m_bPrintPreview = false;
}

void CPrintPreviewDialog::GenList() 
{
}