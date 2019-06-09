// StatisticDummyView.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "DlgDummyView.h"
#include "PrintPreviewDialog.h"
#include "DlgPreviewView.h"
#include "FOBarCodeShape.h"
#include "FOFieldShape.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDummyView

IMPLEMENT_DYNCREATE(CDlgDummyView, CFODrawView)

CDlgDummyView::CDlgDummyView()
{
	m_pDialog = NULL;
	m_pOldFrame = NULL;
}

CDlgDummyView::~CDlgDummyView()
{
	m_pDialog = NULL;
	m_pOldFrame = NULL;
}


BEGIN_MESSAGE_MAP(CDlgDummyView, CFODrawView)
	//{{AFX_MSG_MAP(CDlgDummyView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDummyView drawing

void CDlgDummyView::OnDraw(CDC* pDC)
{
	Draw(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CDlgDummyView diagnostics

#ifdef _DEBUG
void CDlgDummyView::AssertValid() const
{
	CFODrawView::AssertValid();
}

void CDlgDummyView::Dump(CDumpContext& dc) const
{
	CFODrawView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDlgDummyView message handlers
void CDlgDummyView::SetPrintDialog(CPrintPreviewDialog* pDialog)
{
	m_pDialog = pDialog;
}

BOOL CDlgDummyView::OnPreparePrinting(CPrintInfo* pInfo)
{
	if(m_pDialog)
	{
		if(m_pCurDataModel == NULL)
		{
			SetCurrentModel( m_pDialog->m_pFormDataModel );
		}
	}

	bool done = false;
	if( pInfo->m_bPreview == FALSE)
	{
		done = true;
		pInfo->m_bPreview = TRUE;
	}

	BOOL rt = CFODrawView::OnPreparePrinting(pInfo);

	if( done )
	{
		pInfo->m_bPreview = FALSE;
	}

	return rt;

	//return CFODrawView::OnPreparePrinting(pInfo);
	//return DoPreparePrinting(pInfo);
}

void CDlgDummyView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if(m_pDialog)
	{
		if(m_pCurDataModel == NULL)
		{
			SetCurrentModel( m_pDialog->m_pFormDataModel );
		}
/*
		CFODrawShape *pShape = NULL;
		POSITION pos = m_pDialog->m_pFormDataModel->GetFormObjects()->GetHeadPosition();
		if(pos != NULL)	pShape = (CFODrawShape *)m_pDialog->m_pFormDataModel->GetFormObjects()->GetNext(pos);
		UINT m_Shape_Type = pShape->GetType();

		CRect ShapeRect = pShape->GetMaxRect();

		int ActuralWidth  = pShape->GetMaxRect().Width();
		int ActuralHeight = pShape->GetMaxRect().Height();
		
		int SideWidth = 10, PageBorderSize = 20;
		
		CRect rcPrinter = GetCurrentModel()->GetPrintPosition();
		
		int HorNum = (rcPrinter.Width()  - PageBorderSize * 2) / (ActuralWidth  + SideWidth);
		int VerNum = (rcPrinter.Height() - PageBorderSize * 2) / (ActuralHeight + SideWidth);
		
		int PageNum = m_pDialog->m_strlist.GetSize() / (HorNum * VerNum) + 1;
		
		GetCurrentModel()->ChangePositionWithPages( 1, PageNum );
*/
	
		//==============================================================
		CFODrawShapeList *pShapeList = m_pDialog->m_pFormDataModel->GetFormObjects();
		POSITION pos = pShapeList->GetHeadPosition();
		while ( pos )
		{
			CFODrawShape *pShape = pShapeList->GetNext(pos);
			if( pShape == NULL )   continue;
			UINT ObjType = pShape->GetType();
			switch( ObjType ) {
			case FO_COMP_FIELD:
				{
					CString value,title;
					value.Empty();
					bool rt = false;
					for(int i = 0; i < m_pDialog->m_titlelist.GetSize(); i++)
					{
						if(m_pDialog->m_titlelist.GetAt(i).CompareNoCase(pShape->GetObjectCaption()) == 0)
						{
							value = m_pDialog->m_strlist.GetAt(i);
							rt    = true;
							break;
						}
					}
					if( rt )
					{
						//===========================================
						CSize sz = ((CFOFieldShape *)pShape)->GenTextSize(pDC,value);
						CSize minsize = ((CFOFieldShape *)pShape)->GenTextSize(pDC,"Ã÷");
						CRect ShapeRect = pShape->GetRect();
						CString newstr,newline;
						newstr.Empty();  newline.Empty();
						char enter[3];
						enter[0] = 0x0d; enter[1]= 0x0a; enter[2] = 0x00;
						if(sz.cx > ShapeRect.Width())
						{
							while(!value.IsEmpty())
							{
								if((((CFOFieldShape *)pShape)->GenTextSize(pDC,newline).cx + 2*minsize.cx ) > ShapeRect.Width() )
								{
									BYTE ch = newline.GetAt(newline.GetLength() -1);
									if((ch&0x80) != 0 )
									{
										int k = 0;
										for(int i = (newline.GetLength() -1); i >=0; i-- )
										{
											ch = newline.GetAt(i);
											if((ch&0x80) == 0 )   break;
											k++;
										}
										if((k%2) != 0 )
										{
											value = newline.Right(1) + value;
											newline = newline.Left(newline.GetLength() - 1);
										}
									}
									newline += enter;
									newstr += newline;
									newline.Empty();
								}
								else
								{
									newline += value.Left(1);
									value = value.Right(value.GetLength() - 1);
								}
							}
							if(!newline.IsEmpty())
							{
								newstr += newline;
								newline.Empty();
							}
							pShape->SetObjectCaption( newstr );
							break;
						}
						//===========================================
						pShape->SetObjectCaption( value );
					}
					else
					{
						pShape->SetVisible(FALSE);
					}
				}
				break;
			}
		}
		//==============================================================
	}

	CFODrawView::OnBeginPrinting(pDC,pInfo);
}

void CDlgDummyView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	CFODrawView::OnPrint(pDC, pInfo);
}

void CDlgDummyView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CFODrawView::OnEndPrinting(pDC,pInfo);
}

void CDlgDummyView::OnDoPrintPreview()
{
	// In derived classes, implement special window handling here
	// Be sure to Unhook Frame Window close if hooked.

	// must not create this on the frame.  Must outlive this function
	CPrintPreviewState* pState = new CPrintPreviewState;

	// DoPrintPreview's return value does not necessarily indicate that
	// Print preview succeeded or failed, but rather what actions are necessary
	// at this point.  If DoPrintPreview returns TRUE, it means that
	// OnEndPrintPreview will be (or has already been) called and the
	// pState structure will be/has been deleted.
	// If DoPrintPreview returns FALSE, it means that OnEndPrintPreview
	// WILL NOT be called and that cleanup, including deleting pState
	// must be done here.

	if (!DoPrintPreview(AFX_IDD_PREVIEW_TOOLBAR, this,
							RUNTIME_CLASS(CDlgPreviewView), pState))
	{
		// In derived classes, reverse special window handling here for
		// Preview failure case

		TRACE0("Error: DoPrintPreview failed.\n");
		AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
		delete pState;      // preview failed to initialize, delete State now
	}
	// the preview view should be the active view, set the pointer for it
	// we need to do this so that the ID_FILE_PRINT command will goto the dialog
	// and not any global view.
	CFrameWnd *pWnd = (CFrameWnd*)AfxGetApp()->m_pMainWnd;
	CDlgPreviewView *pMyPreview = (CDlgPreviewView*)pWnd->GetActiveView();
	pMyPreview->SetDialogPointer(m_pDialog);
}

void CDlgDummyView::OnDoFilePrint()
{
	// get default print info
	CPrintInfo printInfo;
	ASSERT(printInfo.m_pPD != NULL);    // must be set

	if (LOWORD(GetCurrentMessage()->wParam) == ID_FILE_PRINT_DIRECT)
	{
		CCommandLineInfo* pCmdInfo = AfxGetApp()->m_pCmdInfo;

		if (pCmdInfo != NULL)
		{
			if (pCmdInfo->m_nShellCommand == CCommandLineInfo::FilePrintTo)
			{
				printInfo.m_pPD->m_pd.hDC = ::CreateDC(pCmdInfo->m_strDriverName,
					pCmdInfo->m_strPrinterName, pCmdInfo->m_strPortName, NULL);
				if (printInfo.m_pPD->m_pd.hDC == NULL)
				{
					AfxMessageBox(AFX_IDP_FAILED_TO_START_PRINT);
					return;
				}
			}
		}

		printInfo.m_bDirect = TRUE;
	}

	if (OnPreparePrinting(&printInfo))
	{
		// hDC must be set (did you remember to call DoPreparePrinting?)
		ASSERT(printInfo.m_pPD->m_pd.hDC != NULL);

		// gather file to print to if print-to-file selected
		CString strOutput;
		if (printInfo.m_pPD->m_pd.Flags & PD_PRINTTOFILE && !printInfo.m_bDocObject)
		{
			// construct CFileDialog for browsing
			CString strDef(MAKEINTRESOURCE(AFX_IDS_PRINTDEFAULTEXT));
			CString strPrintDef(MAKEINTRESOURCE(AFX_IDS_PRINTDEFAULT));
			CString strFilter(MAKEINTRESOURCE(AFX_IDS_PRINTFILTER));
			CString strCaption(MAKEINTRESOURCE(AFX_IDS_PRINTCAPTION));
			CFileDialog dlg(FALSE, strDef, strPrintDef,
				OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFilter);
			dlg.m_ofn.lpstrTitle = strCaption;

			if (dlg.DoModal() != IDOK)
				return;

			// set output device to resulting path name
			strOutput = dlg.GetPathName();
		}

		// set up document info and start the document printing process
		CString strTitle;
		CDocument* pDoc = GetDocument();
		if (pDoc != NULL)
			strTitle = pDoc->GetTitle();
		else
			m_pDialog->GetWindowText(strTitle);
		if (strTitle.GetLength() > 31)
			strTitle.ReleaseBuffer(31);
		DOCINFO docInfo;
		memset(&docInfo, 0, sizeof(DOCINFO));
		docInfo.cbSize = sizeof(DOCINFO);
		docInfo.lpszDocName = strTitle;
		CString strPortName;
		int nFormatID;
		if (strOutput.IsEmpty())
		{
			docInfo.lpszOutput = NULL;
			strPortName = printInfo.m_pPD->GetPortName();
			nFormatID = AFX_IDS_PRINTONPORT;
		}
		else
		{
			docInfo.lpszOutput = strOutput;
			AfxGetFileTitle(strOutput,
				strPortName.GetBuffer(_MAX_PATH), _MAX_PATH);
			nFormatID = AFX_IDS_PRINTTOFILE;
		}

		// setup the printing DC
		CDC dcPrint;
		if (!printInfo.m_bDocObject)
		{
			dcPrint.Attach(printInfo.m_pPD->m_pd.hDC);  // attach printer dc
			dcPrint.m_bPrinting = TRUE;
		}
		OnBeginPrinting(&dcPrint, &printInfo);

		//if (!printInfo.m_bDocObject)
		//	dcPrint.SetAbortProc(AbortProc);

		// disable main window while printing & init printing status dialog
		AfxGetMainWnd()->EnableWindow(FALSE);
		//CPrintingDialog dlgPrintStatus(this);

		//CString strTemp;
		//dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_DOCNAME, strTitle);
		//dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PRINTERNAME,
		//	printInfo.m_pPD->GetDeviceName());
		//AfxFormatString1(strTemp, nFormatID, strPortName);
		//dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PORTNAME, strTemp);
		//dlgPrintStatus.ShowWindow(SW_SHOW);
		//dlgPrintStatus.UpdateWindow();

		// start document printing process
		if (!printInfo.m_bDocObject && dcPrint.StartDoc(&docInfo) == SP_ERROR)
		{
			// enable main window before proceeding
			AfxGetMainWnd()->EnableWindow(TRUE);

			// cleanup and show error message
			OnEndPrinting(&dcPrint, &printInfo);
			//dlgPrintStatus.DestroyWindow();
			dcPrint.Detach();   // will be cleaned up by CPrintInfo destructor
			AfxMessageBox(AFX_IDP_FAILED_TO_START_PRINT);
			return;
		}

		// Guarantee values are in the valid range
		UINT nEndPage = printInfo.GetToPage();
		UINT nStartPage = printInfo.GetFromPage();

		if (nEndPage < printInfo.GetMinPage())
			nEndPage = printInfo.GetMinPage();
		if (nEndPage > printInfo.GetMaxPage())
			nEndPage = printInfo.GetMaxPage();

		if (nStartPage < printInfo.GetMinPage())
			nStartPage = printInfo.GetMinPage();
		if (nStartPage > printInfo.GetMaxPage())
			nStartPage = printInfo.GetMaxPage();

		int nStep = (nEndPage >= nStartPage) ? 1 : -1;
		nEndPage = (nEndPage == 0xffff) ? 0xffff : nEndPage + nStep;

		//VERIFY(strTemp.LoadString(AFX_IDS_PRINTPAGENUM));

		// If it's a doc object, we don't loop page-by-page
		// because doc objects don't support that kind of levity.

		BOOL bError = FALSE;
		if (printInfo.m_bDocObject)
		{
			OnPrepareDC(&dcPrint, &printInfo);
			OnPrint(&dcPrint, &printInfo);
		}
		else
		{
			// begin page printing loop
			for (printInfo.m_nCurPage = nStartPage;
				printInfo.m_nCurPage != nEndPage; printInfo.m_nCurPage += nStep)
			{
				OnPrepareDC(&dcPrint, &printInfo);

				// check for end of print
				if (!printInfo.m_bContinuePrinting)
					break;

				// write current page
				//TCHAR szBuf[80];
				//wsprintf(szBuf, strTemp, printInfo.m_nCurPage);
				//dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PAGENUM, szBuf);

				// set up drawing rect to entire page (in logical coordinates)
				printInfo.m_rectDraw.SetRect(0, 0,
					dcPrint.GetDeviceCaps(HORZRES),
					dcPrint.GetDeviceCaps(VERTRES));
				dcPrint.DPtoLP(&printInfo.m_rectDraw);

				// attempt to start the current page
				if (dcPrint.StartPage() < 0)
				{
					bError = TRUE;
					break;
				}

				// must call OnPrepareDC on newer versions of Windows because
				// StartPage now resets the device attributes.
				if (afxData.bMarked4)
					OnPrepareDC(&dcPrint, &printInfo);

				ASSERT(printInfo.m_bContinuePrinting);

				// page successfully started, so now render the page
				OnPrint(&dcPrint, &printInfo);
				if (dcPrint.EndPage() < 0)// || !AbortProc(dcPrint.m_hDC, 0))
				{
					bError = TRUE;
					break;
				}
			}
		}

		// cleanup document printing process
		if (!printInfo.m_bDocObject)
		{
			if (!bError)
				dcPrint.EndDoc();
			else
				dcPrint.AbortDoc();
		}

		AfxGetMainWnd()->EnableWindow();    // enable main window

		OnEndPrinting(&dcPrint, &printInfo);    // clean up after printing
		//dlgPrintStatus.DestroyWindow();

		dcPrint.Detach();   // will be cleaned up by CPrintInfo destructor
	}
}

void CDlgDummyView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView)
{
	CFODrawView::OnEndPrintPreview(pDC, pInfo, point, pView);
	// show the original frame
	m_pOldFrame->ShowWindow(SW_SHOW);
	// restore main frame pointer
	AfxGetApp()->m_pMainWnd = m_pOldFrame;
	m_pDialog->EndPreviewMode();
	m_pDialog->SetFocus();
	GetParentFrame()->DestroyWindow();
}

void CDlgDummyView::OnInitialUpdate() 
{
	CFODrawView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	
}
