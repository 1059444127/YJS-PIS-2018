// PathologyView.cpp : implementation of the CPathologyView class
//

#include "stdafx.h"
#include "Pathology.h"
#include "MainFrm.h"

#include "PathologyDoc.h"
#include "PathologyView.h"
#include "DummyView.h"
#include "MedPerson.h"
#include "MedMaterial.h"
#include "MedDiagnose.h"
#include "MedWax.h"
#include "MedPreslice.h"
#include "MedAdvice.h"
#include "MedSlice.h"
#include "ImageView.h"
#include "MedImage.h"
#include "MedTbs.h"
#include "AdviceShow.h"
#include "DBList.h"
#include "SelfInput.h"
#include "NormalWord.h"
#include "INI.h"
#include "AreaOut.h"
#include "FmtSelect.h"
#include "Splash.h"
#include "Notify.h"
#include "Zdex.h"
#include "ImageDockWnd.h"
#include "ReportingView.h"

#include "GetStudyUID.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPathologyView

IMPLEMENT_DYNCREATE(CPathologyView, CXTResizeFormView)

BEGIN_MESSAGE_MAP(CPathologyView, CXTResizeFormView)
	//{{AFX_MSG_MAP(CPathologyView)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_NOTIFY(NM_CLICK, IDC_TAB_CTRL, OnClickTabCtrl)
	ON_COMMAND(ID_DOC_QUICKINPUT, OnDocQuickinput)
	ON_UPDATE_COMMAND_UI(ID_DOC_QUICKINPUT, OnUpdateDocQuickinput)
	ON_COMMAND(ID_FILE_RPTCOPY, OnFileRptcopy)
	ON_COMMAND(ID_FILE_RPTCOPYALL, OnFileRptcopyall)
	ON_COMMAND(ID_FILE_RPTCOPYIMG, OnFileRptcopyimg)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, OnViewPrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnViewPrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPathologyView construction/destruction
extern CPathologyApp theApp;

CPathologyView::CPathologyView()
	: CXTResizeFormView(CPathologyView::IDD)
{
	//{{AFX_DATA_INIT(CPathologyView)
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_pTemplate = NULL;
	m_bPrintPreview = false;
	m_pView = NULL;
	m_pFormDataModel = new CFormDataModel;

	m_bIsFirstInit = true;
}

CPathologyView::~CPathologyView()
{
	if(m_pTemplate)
	{
		delete m_pTemplate;
		m_pTemplate = NULL;
	}
	
	if(m_pFormDataModel != NULL)
	{
		delete m_pFormDataModel;
		m_pFormDataModel = NULL;
	}
}

void CPathologyView::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPathologyView)
	DDX_Control(pDX, IDC_TAB_CTRL, m_tabCtrl);
	//}}AFX_DATA_MAP
}

BOOL CPathologyView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CXTResizeFormView::PreCreateWindow(cs))
		return FALSE;

	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	

	return TRUE;
}

void CPathologyView::OnInitialUpdate()
{
	CXTResizeFormView::OnInitialUpdate();

	if (m_bIsFirstInit)
	{
		m_bIsFirstInit = false;

		GetParentFrame()->RecalcLayout();
		ResizeParentToFit();

		SetResize(IDC_TAB_CTRL,     SZ_TOP_LEFT,    SZ_BOTTOM_RIGHT);

		InitTabCtrl();
		AddTabViews();
		

		if(theApp.m_SelfDefineWndAutoOpenMode.CompareNoCase("是") == 0)
		{
			((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_COMMAND, ID_DOC_QUICKINPUT);
		}

		CSplashWnd::DestorySplashScreen();

/*		if(!theApp.m_UnRead_Knly_Notified.IsEmpty())
		{
			CNotify  *pNotify = NULL;
			pNotify = new CNotify(this);
			pNotify->m_Title.Format("留言提示");
			pNotify->m_info = theApp.m_UnRead_Knly_Notified;
			pNotify->m_TimeDelay = 6000;
			pNotify->Create();
			pNotify->SetActiveWindow();
			pNotify->ShowWindow(SW_SHOW);
			pNotify->CenterWindow();
		}   */
	}

/*	if(theApp.m_StationType.CompareNoCase("诊断工作站") == 0 || theApp.m_StationType.CompareNoCase("取材工作站")==0 || theApp.m_StationType.CompareNoCase("细胞学工作站")==0 ){
		m_tabCtrl.SetActiveView(1);
		CWnd *pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CDBList));
		if(pWnd != NULL)
		{
			((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.SetActiveView(pWnd);
			((CEdit *)pWnd->GetDlgItem(IDC_EDIT_BLH))->SetFocus();
		}
	}
*/
}

int CPathologyView::InitTabCtrl()
{
	// you can use this to change the tab orentation

	/*
	m_tabCtrl.ModifyTabStyle(NULL, TCS_BOTTOM);
	*/

	// Have the tab control send WM_INITIALUPDATE to views.
	m_tabCtrl.SendInitialUpdate(TRUE);

	// you can use something like the following snippet to remove an image
	// from a tab that's already been created...

	/*
	TCITEM tci;
	tci.mask = TCIF_IMAGE;
	tci.iImage = -1;
	m_tabCtrl.SetItem(1, &tci);
	*/

	m_tabCtrl.SetPadding(CSize(6, 3));

	// Create the image list used by the tab control.
	if (!m_TabImageList.Create( 16, 16, ILC_COLOR24 | ILC_MASK , 10, 1))
	{
		TRACE0("Failed to create image list.\n");
		return -1;
	}
	CBitmap * pImage = new CBitmap();		 
	pImage->LoadBitmap(IDB_IMAGELIST);
	m_TabImageList.Add(pImage, RGB( 212,208,200 ));
	SAFE_DELETE(pImage);

/*
	if (!m_TabImageList.Create( IDB_IMAGELIST, 16, 1, RGB( 0x00,0xFF,0x00 )))
	{
		TRACE0("Failed to create image list.\n");
		return -1;
	}
*/

	// Set the tab controls image list.
	m_tabCtrl.SetImageList(&m_TabImageList);

	// Set the tab to auto condense (shrink) its tabs when sized.
	m_tabCtrl.SetAutoCondense(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_tabCtrl.ModifyStyle(0L, WS_CLIPCHILDREN|WS_CLIPSIBLINGS);

	::SetWindowPos(m_tabCtrl.m_hWnd, NULL, 0,0,0,0,	SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
	m_tabCtrl.Invalidate(FALSE);

	UpdateData();
	return 0;
}

int CPathologyView::AddTabViews()
{
	// add views to the tab control.
	//AfxMessageBox(theApp.m_StationType);
	if(theApp.m_StationType.CompareNoCase("登记工作站") == 0 || theApp.m_StationType.CompareNoCase("取材工作站") == 0 || theApp.m_StationType.CompareNoCase("诊断工作站") == 0 || theApp.m_StationType.CompareNoCase("细胞学工作站") == 0)
	{
		if (!m_tabCtrl.AddView(_T("病历信息"), RUNTIME_CLASS(CMedPerson), NULL, NULL, m_tabCtrl.GetItemCount(), 0))
		{
			TRACE0("Failed to create CMedPerson.\n");
			return -1;
		}
	}
	
	if(theApp.m_StationType.CompareNoCase("取材工作站") == 0)
	{
		if (!m_tabCtrl.AddView(_T("取材信息"), RUNTIME_CLASS(CMedMaterial), NULL, NULL, m_tabCtrl.GetItemCount(), 1))
		{
			TRACE0("Failed to create CMedMaterial.\n");
			return -1;
		}
	}

	if(theApp.m_StationType.CompareNoCase("诊断工作站") == 0)
	{
		if (!m_tabCtrl.AddView(_T("病理诊断"), RUNTIME_CLASS(CMedDiagnose), NULL, NULL, m_tabCtrl.GetItemCount(), 2))
		{
			TRACE0("Failed to create CMedDiagnose.\n");
			return -1;
		}
	}

	if(theApp.m_StationType.CompareNoCase("特检工作站") == 0)
	{
		if (!m_tabCtrl.AddView(_T("医嘱列表"), RUNTIME_CLASS(CMedAdvice), NULL, NULL, m_tabCtrl.GetItemCount(), 5))
		{
			TRACE0("Failed to create CMedAdvice.\n");
			return -1;
		}
		if (!m_tabCtrl.AddView(_T("已制片列表"), RUNTIME_CLASS(CMedSlice), NULL, NULL, m_tabCtrl.GetItemCount(), 6))
		{
			TRACE0("Failed to create CMedSlice.\n");
			return -1;
		}
	}

	if(theApp.m_StationType.CompareNoCase("细胞学制片工作站") == 0)
	{
		if (!m_tabCtrl.AddView(_T("已制片列表"), RUNTIME_CLASS(CMedSlice), NULL, NULL, m_tabCtrl.GetItemCount(), 6))
		{
			TRACE0("Failed to create CMedSlice.\n");
			return -1;
		}
	}
	
	if(theApp.m_StationType.CompareNoCase("制片工作站")     == 0 ||
	   theApp.m_StationType.CompareNoCase("常规切片工作站") == 0 )
	{
		if (!m_tabCtrl.AddView(_T("待包埋列表"), RUNTIME_CLASS(CMedWax), NULL, NULL, m_tabCtrl.GetItemCount(), 3))
		{
			TRACE0("Failed to create CMedWax.\n");
			return -1;
		}
		if (!m_tabCtrl.AddView(_T("待切片列表"), RUNTIME_CLASS(CMedPreslice), NULL, NULL, m_tabCtrl.GetItemCount(), 4))
		{
			TRACE0("Failed to create CMedPreslice.\n");
			return -1;
		}
		if (!m_tabCtrl.AddView(_T("医嘱列表"), RUNTIME_CLASS(CMedAdvice), NULL, NULL, m_tabCtrl.GetItemCount(), 5))
		{
			TRACE0("Failed to create CMedAdvice.\n");
			return -1;
		}
		if (!m_tabCtrl.AddView(_T("已制片列表"), RUNTIME_CLASS(CMedSlice), NULL, NULL, m_tabCtrl.GetItemCount(), 6))
		{
			TRACE0("Failed to create CMedSlice.\n");
			return -1;
		}
	}

	if(theApp.m_StationType.CompareNoCase("细胞学工作站") == 0)
	{
		if (!m_tabCtrl.AddView(_T("细胞学诊断"), RUNTIME_CLASS(CMedTbs), NULL, NULL, m_tabCtrl.GetItemCount(), 7))
		{
			TRACE0("Failed to create CMedTbs.\n");
			return -1;
		}
	}

	if(theApp.m_StationType.CompareNoCase("取材工作站") == 0 || theApp.m_StationType.CompareNoCase("诊断工作站") == 0 || theApp.m_StationType.CompareNoCase("细胞学工作站") == 0)
	{
		if (!m_ImageDockWnd.Create(this, AFX_IDW_TOOLBAR + 6, _T(""),
			CSize(150, 150), CBRS_BOTTOM))
		{
			TRACE0("Failed to create image ImageDockWnd dock window\n");
			return -1;		// fail to create
		}
		
		if(!m_tabCtrl.AddControl(_T("病理图像"),&m_ImageDockWnd, m_tabCtrl.GetItemCount(), 8))
		{
			TRACE0("Failed to create CImageDockWnd.\n");
			return -1;
		}
	}

	m_tabCtrl.SetActiveView(0);

	if(theApp.m_StationType.CompareNoCase("制片工作站")     == 0 || 
	   theApp.m_StationType.CompareNoCase("常规切片工作站") == 0 ||
	   theApp.m_StationType.CompareNoCase("特检工作站")     == 0 ||
	   theApp.m_StationType.CompareNoCase("细胞学制片工作站") == 0 )
	{
		CWnd *pWnd = m_tabCtrl.GetView(RUNTIME_CLASS(CMedSlice));
		if(pWnd != NULL)  m_tabCtrl.SetActiveView(pWnd);
	}
	
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CPathologyView diagnostics

#ifdef _DEBUG
void CPathologyView::AssertValid() const
{
	CXTResizeFormView::AssertValid();
}

void CPathologyView::Dump(CDumpContext& dc) const
{
	CXTResizeFormView::Dump(dc);
}

CPathologyDoc* CPathologyView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPathologyDoc)));
	return (CPathologyDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPathologyView message handlers
bool CPathologyView::LoadReportFormat(CString &m_Fmt)
{
	CString Rptname; Rptname.Empty();
	if(Rptname.IsEmpty())
	{
		CWnd *pWnd = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedDiagnose));
		if(pWnd != NULL)  ((CMedDiagnose *)pWnd)->GetDlgItemText(IDC_COMBO_BGGS, Rptname);
		
		Rptname.TrimLeft(); Rptname.TrimRight();
		if(Rptname.CompareNoCase("请选择报告格式") == 0)  Rptname.Empty();
	}
	
	if(Rptname.IsEmpty())
	{
		CWnd *pWnd = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedTbs));
		if(pWnd != NULL)  ((CMedTbs *)pWnd)->GetDlgItemText(IDC_COMBO_BGGS, Rptname);
		
		Rptname.TrimLeft(); Rptname.TrimRight();
		if(Rptname.CompareNoCase("请选择报告格式") == 0)  Rptname.Empty();
	}

	//--------------------------------------------
	if( theApp.Is_Batch_Printing )
	{
		Rptname = theApp.m_FmtName_in_Batch;
	}
	//--------------------------------------------
	m_Fmt = Rptname;

	//-------------------------------------------
	if(theApp.m_PrintType == 1)
	{
		Rptname.Format("制片条码");
	}

	if(theApp.m_PrintType == 2)
	{
		Rptname = theApp.m_BdName;
	}

	if(theApp.m_PrintType == 3)
	{
		Rptname.Format("冰冻报告");
	}
	
	if(theApp.m_PrintType == 4)
	{
		Rptname.Format("补充报告");
	}
	//-------------------------------------------
	
	if(Rptname.IsEmpty())   
	{
		if(theApp.m_PrintType == 0 && !theApp.Is_Batch_Printing)
		{
			if(theApp.m_BggsGenMode.CompareNoCase("自动记忆上一记录报告格式") == 0)
			{
				CIniReader m_IniReader;
				m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");
				CString LastFmt = m_IniReader.getKeyValue("上一打印记录报告格式","运行参数设置");
				
				//----------------------------------------------------------------
				bool IsBelonging = false;
				
				if(!LastFmt.IsEmpty())
				{
					CStringArray bdlist; bdlist.RemoveAll();
					CStdioFile File;
					if ( File.Open(theApp.m_Exe_Working_Dir + "system\\表单.txt", CFile::modeRead | CFile::typeText) )
					{
						CString tmp;
						while(1)
						{
							if( File.ReadString(tmp) == FALSE ) break;
							tmp.TrimLeft(); tmp.TrimRight();
							if(!tmp.IsEmpty())
							{
								bdlist.Add(tmp);
							}
						}
						File.Close();
					}
					
					try
					{
						g_dbcommand.setCommandText("Select rpt_name from REPORTFORMAT where dbtype = :dbtype");
						g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
						g_dbcommand.Execute();
						
						while( g_dbcommand.FetchNext() )
						{
							CString str = g_dbcommand.Field("rpt_name").asString();
							if(str.CompareNoCase("制片条码") == 0 || str.CompareNoCase("回执单") == 0 || str.CompareNoCase("冰冻报告") == 0 || str.CompareNoCase("材块移交表") == 0 || str.CompareNoCase("切片移交表") == 0 || str.CompareNoCase("医嘱工作单") == 0 || str.CompareNoCase("制片情况统计表") == 0)  continue;
							
							if(bdlist.GetSize() > 0)
							{
								for(int nx = 0; nx < bdlist.GetSize(); nx++)
								{
									if(bdlist.GetAt(nx).CompareNoCase(str) == 0)  break;
								}
								if(nx < bdlist.GetSize())  continue;
							}
							
							if(g_dbcommand.Field("rpt_name").asString().CompareNoCase(LastFmt) == 0)
							{
								IsBelonging = true;
								break;
							}
						}
						
						g_dbconnection.Commit();
					}
					catch(SAException &x)
					{
						try
						{
							g_dbconnection.Rollback();
						}
						catch(SAException &)
						{
						}
						AfxMessageBox((const char*)x.ErrText());
					}
					
					bdlist.RemoveAll();
				}
				//----------------------------------------------------------------
				
				if(LastFmt.IsEmpty() == FALSE && IsBelonging)
				{
					m_Fmt = Rptname = LastFmt;
				}
				else
				{
					//AfxMessageBox("您未选择报告格式!");
					CFmtSelect mydlg(this);
					mydlg.m_Mode = 1;
					if(mydlg.DoModal() == IDOK)
					{
						m_Fmt = Rptname = mydlg.fmtstr;
					}
				}
			}

			if(theApp.m_BggsGenMode.CompareNoCase("自动按照报告图像数选择报告格式") == 0)
			{
				CStringArray bdlist; bdlist.RemoveAll();
				CStdioFile File;
				if ( File.Open(theApp.m_Exe_Working_Dir + "system\\表单.txt", CFile::modeRead | CFile::typeText) )
				{
					CString tmp;
					while(1)
					{
						if( File.ReadString(tmp) == FALSE ) break;
						tmp.TrimLeft(); tmp.TrimRight();
						if(!tmp.IsEmpty())
						{
							bdlist.Add(tmp);
						}
					}
					File.Close();
				}

				CStringArray ListFmt;
				CUIntArray   ListImgNum;
				ListFmt.RemoveAll();
				ListImgNum.RemoveAll();

				try
				{
					g_dbcommand.setCommandText("Select rpt_name, pic_num from REPORTFORMAT where dbtype = :dbtype");
					g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
					g_dbcommand.Execute();
					
					while( g_dbcommand.FetchNext() )
					{
						CString str = g_dbcommand.Field("rpt_name").asString();
						if(str.CompareNoCase("制片条码") == 0 || str.CompareNoCase("回执单") == 0 || str.CompareNoCase("冰冻报告") == 0 || str.CompareNoCase("材块移交表") == 0 || str.CompareNoCase("切片移交表") == 0 || str.CompareNoCase("医嘱工作单") == 0 || str.CompareNoCase("制片情况统计表") == 0)  continue;

						if(bdlist.GetSize() > 0)
						{
							for(int nx = 0; nx < bdlist.GetSize(); nx++)
							{
								if(bdlist.GetAt(nx).CompareNoCase(str) == 0)  break;
							}
							if(nx < bdlist.GetSize())  continue;
						}
						
						ListFmt.Add( g_dbcommand.Field("rpt_name").asString() );
						ListImgNum.Add( g_dbcommand.Field("pic_num").asLong() );
					}
					
					g_dbconnection.Commit();
				}
				catch(SAException &x)
				{
					try
					{
						g_dbconnection.Rollback();
					}
					catch(SAException &)
					{
					}
					AfxMessageBox((const char*)x.ErrText());
					return false;
				}

				bdlist.RemoveAll();

				int picnum = 0;
				try
				{
					SAString cmdstr = "Select count(*) from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno and printflag = 1";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = GetDocument()->m_blh;
					
					g_dbcommand.Execute();
					if(g_dbcommand.FetchNext())
					{
						picnum = g_dbcommand.Field(1).asLong();
					}
					
					g_dbconnection.Commit();
				}
				catch(SAException &x)
				{
					try
					{
						g_dbconnection.Rollback();
					}
					catch(SAException &)
					{
					}
					AfxMessageBox((const char*)x.ErrText());
					return false;
				}

				if(picnum < 1)
				{
					try
					{
						SAString cmdstr = "Select count(*) from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno";
						g_dbcommand.setCommandText(cmdstr);
						g_dbcommand.Param("medicalno").setAsString() = GetDocument()->m_blh;
						
						g_dbcommand.Execute();
						if(g_dbcommand.FetchNext())
						{
							picnum = g_dbcommand.Field(1).asLong();
						}
						
						g_dbconnection.Commit();
					}
					catch(SAException &x)
					{
						try
						{
							g_dbconnection.Rollback();
						}
						catch(SAException &)
						{
						}
						AfxMessageBox((const char*)x.ErrText());
						return false;
					}
				}
				
				for( UINT i = 0; i < ListImgNum.GetSize(); i++ )
				{
					if( ListImgNum.GetAt(i) == picnum )
					{
						m_Fmt = Rptname = ListFmt.GetAt( i );
						break;
					}
				}
				
				if( Rptname.IsEmpty() )
				{
					int kepi = -1;
					int dis = 10000;
					for( UINT i = 0; i < ListImgNum.GetSize(); i++ )
					{
						if( ListImgNum.GetAt(i) > picnum )  continue;
						if( (picnum - ListImgNum.GetAt(i)) < dis )
						{
							kepi = i;
							dis = picnum - ListImgNum.GetAt(i);
						}
					}
					if( kepi >= 0)
					{
						m_Fmt = Rptname = ListFmt.GetAt( kepi );
					}
				}
				
				if( Rptname.IsEmpty() )
				{
					int kepi = -1;
					int dis = 10000;
					for( UINT i = 0; i < ListImgNum.GetSize(); i++ )
					{
						if( ListImgNum.GetAt(i) <= picnum )  continue;
						if( (ListImgNum.GetAt(i) - picnum) < dis )
						{
							kepi = i;
							dis = ListImgNum.GetAt(i) - picnum;
						}
					}
					if( kepi >= 0)
					{
						m_Fmt = Rptname = ListFmt.GetAt( kepi );
					}
				}
				if( Rptname.IsEmpty() && ListFmt.GetSize() > 0)
				{
					m_Fmt = Rptname = ListFmt.GetAt( 0 );
				}

				ListFmt.RemoveAll();
				ListImgNum.RemoveAll();

				if( Rptname.IsEmpty() )
				{
					//AfxMessageBox("您未选择报告格式!");
					CFmtSelect mydlg(this);
					mydlg.m_Mode = 1;
					if(mydlg.DoModal() == IDOK)
					{
						Rptname = mydlg.fmtstr;
						m_Fmt = Rptname;
					}
				}
			}
		}
	}


	if(Rptname.IsEmpty())	return false;

	if(theApp.m_PrintType == 0 && !theApp.Is_Batch_Printing)
	{
		CIniReader m_IniReader;
		m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");
		m_IniReader.setKey(Rptname,"上一打印记录报告格式","运行参数设置");
	}
	
	try
	{
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
				return false;
			}
			remove(theApp.m_SysTmpPath + Rptname + ".rpt");
		}
		else
		{
			return false;
		}
		
		g_dbconnection.Commit();
	}
	catch(SAException &x)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
		AfxMessageBox((const char*)x.ErrText());
		return false;
	}

	//-------------------------------------------
	if(theApp.m_PrintType == 1)
	{
		POSITION pos = m_pFormDataModel->GetFormObjects()->GetHeadPosition();
		
		//------------------------------------------------------------------
		theApp.m_BarCodeNum = 0;
		while ( pos )
		{
			POSITION oldpos = pos;
			CFODrawShape *pLocalShape = m_pFormDataModel->GetFormObjects()->GetNext(pos);
			if( pLocalShape == NULL )   continue;
			if( pLocalShape->GetType() == FO_COMP_BARCODE )  
			{
				theApp.m_BarCodeNum++;
				continue;
			}
			
			if( pLocalShape->GetType() == FO_COMP_GROUP )
			{
				CFODrawShapeList * pShapeList = ((CFOGroupShape *)pLocalShape)->GetCompList();
				POSITION ps = pShapeList->GetHeadPosition();
				for( int i = 0; i < pShapeList->GetCount(); i ++ )
				{
					CFODrawShape *pShape = (CFODrawShape *)pShapeList->GetNext(ps);
					if( pShape->GetType() == FO_COMP_BARCODE )  break;
				}
				if( i < pShapeList->GetCount() )
				{
					theApp.m_BarCodeNum++;
					continue;
				}
			}
			
			m_pFormDataModel->GetFormObjects()->RemoveAt(oldpos);
		}
		if( !theApp.m_BarCodeNum )  return false;
	}
	//-------------------------------------------

	return true;
}

void CPathologyView::OnViewPrint()
{
	if(!GetDocument()->m_blh.IsEmpty())
	{
		ForceSave();
	}
	
	theApp.ConnectDatabase();

	if(theApp.m_PrintType == 0)
	{
		CString nBdjg, nZdyj, nczys, nzzys, nshys;SADateTime st; int nbgzt;
		nBdjg.Empty(); nZdyj.Empty(); nczys.Empty(); nzzys.Empty(); nshys.Empty();

		try
		{
			SAString cmdstr = "Select bdjg, zdyj, czys, zzys, shys,bgrq ,bgzt from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				if(!g_dbcommand.Field("bdjg").isNull())
				{
					nBdjg = g_dbcommand.Field("bdjg").asString();
				}
				if(!g_dbcommand.Field("zdyj").isNull())
				{
					nZdyj = g_dbcommand.Field("zdyj").asString();
				}
				if(!g_dbcommand.Field("czys").isNull())
				{
					nczys = g_dbcommand.Field("czys").asString();
				}
				if(!g_dbcommand.Field("zzys").isNull())
				{
					nzzys = g_dbcommand.Field("zzys").asString();
				}
				if(!g_dbcommand.Field("shys").isNull())
				{
					nshys = g_dbcommand.Field("shys").asString();
				}
				st = g_dbcommand.Field("bgrq").asDateTime();
				nbgzt = g_dbcommand.Field("bgzt").asShort();
			}
			
			g_dbconnection.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
		}
		if(nbgzt >= 16 && theApp.m_PrintTime.CompareNoCase("0"))
		{
			SADateTime st_now = theApp.GetServerDateTime();
			int i = st_now.GetDay(); int j = st.GetDay(); int temp = 0;
			temp = temp + (i-j)*24*60;
			i = st_now.GetHour(); j = st.GetHour();
			temp = temp + (i-j)*60;
			i = st_now.GetMinute(); j = st.GetMinute();
			temp = temp + (i-j);
			if(temp > atoi(theApp.m_PrintTime) )
			{
				AfxMessageBox("您当前报告已超过限定时间，系统限制打印！");
				return;
			}
		}

		if(theApp.m_AuthorPrintOnlyMode.CompareNoCase("打开") == 0)
		{
			bool bFounded = false;
			if(!nczys.IsEmpty() && nczys.Find(theApp.m_Cpp_User) >= 0)   bFounded = true;
			if(!nzzys.IsEmpty() && nzzys.Find(theApp.m_Cpp_User) >= 0)   bFounded = true;
			if(!nshys.IsEmpty() && nshys.Find(theApp.m_Cpp_User) >= 0)   bFounded = true;

			if(!bFounded)
			{
				AfxMessageBox("当前报告只有诊断医师自已才能打印！");
				return;
			}
		}

		if(nBdjg.IsEmpty() && nZdyj.IsEmpty() && theApp.m_ZdDyMode.CompareNoCase("是") == 0)
		{
			AfxMessageBox("该病例还未诊断，不能预览和打印！");
			return;
		}
	}

	if(theApp.m_PrintType == 0)
	{
		if(theApp.m_PrintRestrict.CompareNoCase("否") == 0)
		{
			for(int i = 0; i < 2; i++)
			{
				int m_bgzt = 0;
				try
				{
					SAString cmdstr = "Select bgzt from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
					g_dbcommand.setCommandText( cmdstr );
					g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
					g_dbcommand.Execute();
					
					if( g_dbcommand.FetchNext() )
					{
						m_bgzt = g_dbcommand.Field("bgzt").asLong();
					}
					
					g_dbconnection.Commit();
				}
				catch(SAException &x)
				{
					try
					{
						g_dbconnection.Rollback();
					}
					catch(SAException &)
					{
					}
					AfxMessageBox((const char*)x.ErrText());
				}
				
				if(m_bgzt < 8)
				{
					if( i == 0 && RIGHT_SH(theApp.m_Cpp_Right) && (theApp.m_StationType.CompareNoCase("诊断工作站") == 0 || theApp.m_StationType.CompareNoCase("细胞学工作站") == 0))
					{
						if(AfxMessageBox("该病历未审核，必须审核后才能打印，是否审核？",MB_OKCANCEL|MB_ICONQUESTION) == IDCANCEL)  return;
						theApp.m_ToCheck = 1;
						theApp.OnAppCheck();
					}
					else
					{
						AfxMessageBox("该病例未审核，不能打印！");
						return;
					}
				}
				else
				{
					break;
				}
			}
		}
		
		if(!RIGHT_BGDY(theApp.m_Cpp_Right))
		{
			AfxMessageBox("当前用户没有打印报告的权限!");
			return;
		}
	}

	int PrinterNum = theApp.m_PrinterControl.GetPrinterCount();
	if(PrinterNum <= 0)
	{
		AfxMessageBox("没有安装打印机!");
		theApp.m_PrintType = 0;
		return;
	}

	if(m_pFormDataModel == NULL)   return;

	m_pFormDataModel->ResetAppPrinter();

	CString m_Fmt; m_Fmt.Empty();
	if(m_pFormDataModel)
	{
		m_pFormDataModel->ResetContent();

        // Load Report Form
		if(LoadReportFormat(m_Fmt) == false) 
		{
			theApp.m_PrintType = 0;
			return;
		}
	}

	theApp.InPrinting = true;

	CDummyView	view;

	view.SetPrintView(this);
	view.OnDoFilePrint();

	if(theApp.m_PrintType == 0 && !(theApp.Is_Batch_Printing && !theApp.m_IsRecordFmtInBatchPrinting))
	{
		int m_bgzt = 0;
		try
		{
			SAString cmdstr = "Select bgzt from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("medicalno").setAsString() = GetDocument()->m_blh;
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				m_bgzt = g_dbcommand.Field("bgzt").asLong();
			}
			
			g_dbconnection.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
		}
		
		m_bgzt = (m_bgzt | 0x10);
		
		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set bgzt=:bgzt, bgdy=:bgdy, bggs=:bggs where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = GetDocument()->m_blh;
			g_dbcommand.Param("bgzt").setAsLong() = m_bgzt;
			g_dbcommand.Param("bgdy").setAsString() = theApp.m_Cpp_User;
			g_dbcommand.Param("bggs").setAsString() = m_Fmt;
			
			g_dbcommand.Execute();
			g_dbconnection.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
		}

		if(theApp.m_HisCfg.CompareNoCase("通用接口") == 0)
		{
			theApp.NotifyRecordStatus(GetDocument()->m_blh);
		}
		//------------------------------------------------------------
		CWnd *pWnd;
		pWnd = m_tabCtrl.GetView(RUNTIME_CLASS(CMedDiagnose));
		if(pWnd != NULL)  ((CMedDiagnose *)pWnd)->SetDlgItemText(IDC_COMBO_BGGS, m_Fmt);
		
		pWnd = m_tabCtrl.GetView(RUNTIME_CLASS(CMedTbs));
		if(pWnd != NULL)  ((CMedTbs *)pWnd)->SetDlgItemText(IDC_COMBO_BGGS, m_Fmt);
		//------------------------------------------------------------
		
		((CMainFrame *)AfxGetMainWnd())->RefreshStatusBar();
		
		if(theApp.m_AutoRefreshDBListMode.CompareNoCase("是") == 0 && ((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
		{
			((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.ResetGridSize();
			((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
			((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
		}

		if(theApp.m_PrintAndSendMode.CompareNoCase("是") == 0)
		{
			CWnd * pMainWnd = AfxGetMainWnd();
			if(pMainWnd && pMainWnd->IsKindOf(RUNTIME_CLASS(CMainFrame)))
			{
				pMainWnd->PostMessage(WM_COMMAND, ID_APP_SEND);
			}
		}
	}

	theApp.InPrinting  = false;

	theApp.m_PrintType = 0;
	
	if(theApp.GetMainWnd()->IsKindOf(RUNTIME_CLASS(CMainFrame)))
	{
		((CMainFrame*)theApp.GetMainWnd())->m_wndTabDockBar.SetActiveView(RUNTIME_CLASS(CDBList));
	}
	
}

void CPathologyView::OnViewPrintPreview()
{
	

	if(!GetDocument()->m_blh.IsEmpty())
	{
		ForceSave();
	}

	theApp.ConnectDatabase();

	if(theApp.m_PrintType == 0)
	{
		CString nBdjg, nZdyj;
		nBdjg.Empty(); nZdyj.Empty();

		try
		{
			SAString cmdstr = "Select bdjg, zdyj from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				if(!g_dbcommand.Field("bdjg").isNull())
				{
					nBdjg = g_dbcommand.Field("bdjg").asString();
				}
				if(!g_dbcommand.Field("zdyj").isNull())
				{
					nZdyj = g_dbcommand.Field("zdyj").asString();
				}
			}
			
			g_dbconnection.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
		}

		if(nBdjg.IsEmpty() && nZdyj.IsEmpty() && theApp.m_ZdDyMode.CompareNoCase("是") == 0)
		{
			AfxMessageBox("该病例还未诊断，不能预览和打印！");
			return;
		}
	}

	

	

	int PrinterNum = theApp.m_PrinterControl.GetPrinterCount();
	if(PrinterNum <= 0)
	{
		AfxMessageBox("没有安装打印机!");
		return;
	}
	
	if(m_pFormDataModel == NULL)   return;

	m_pFormDataModel->ResetAppPrinter();

	CString m_Fmt; m_Fmt.Empty();
	if(m_pFormDataModel)
	{
		m_pFormDataModel->ResetContent();

        // Load Report Form
		if(LoadReportFormat(m_Fmt) == false)  return;
	}

	try
	{
		SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set bggs=:bggs where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = GetDocument()->m_blh;
		g_dbcommand.Param("bggs").setAsString() = m_Fmt;
		
		g_dbcommand.Execute();
		g_dbconnection.Commit();
	}
	catch(SAException &x)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
		AfxMessageBox((const char*)x.ErrText());
	}
	
	//------------------------------------------------------------
	CWnd *pWnd;
	pWnd = m_tabCtrl.GetView(RUNTIME_CLASS(CMedDiagnose));
	if(pWnd != NULL)  ((CMedDiagnose *)pWnd)->SetDlgItemText(IDC_COMBO_BGGS, m_Fmt);
	
	pWnd = m_tabCtrl.GetView(RUNTIME_CLASS(CMedTbs));
	if(pWnd != NULL)  ((CMedTbs *)pWnd)->SetDlgItemText(IDC_COMBO_BGGS, m_Fmt);
	//------------------------------------------------------------

	if(theApp.m_PrintviewType.CompareNoCase("报告浏览") == 0){	
		
		((CMainFrame*)AfxGetMainWnd())->SendMessage(WM_COMMAND,ID_FILE_RPTVIEW);

		return;
	}

	if(!RIGHT_BGDY(theApp.m_Cpp_Right))
	{
		AfxMessageBox("当前用户没有打印报告的权限!");
		return;
	}

	CFrameWnd* pOldFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd;
	pOldFrame->ShowWindow(SW_HIDE);
	if (!m_pTemplate)
	{
		m_pTemplate = new CSingleDocTemplate(
			128,					// so load fram is happy. This matches the IDR_MAINFRAME value from your application!
			NULL,
			RUNTIME_CLASS(CFrameWnd),
			RUNTIME_CLASS(CDummyView));
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
	m_pView = (CDummyView*)pFrameWnd->GetActiveView();
	AfxGetApp()->m_pMainWnd = pFrameWnd;
	pFrameWnd->SetWindowText(_T("打印预览"));
	pFrameWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pView->SetPrintView(this);
	m_pView->m_pOldFrame = pOldFrame;
	m_pView->OnDoPrintPreview();
}

void CPathologyView::EndPreviewMode()
{
	m_bPrintPreview = false;
}

void CPathologyView::ForceSave()
{
	bool PersonInfoSaved = false;
	if(GetDocument()->m_blh.IsEmpty())
	{
		CWnd * pWnd = m_tabCtrl.GetView(RUNTIME_CLASS(CMedPerson));
		if(pWnd)
		{
			pWnd->SendMessage(WM_CONTENTSAVE);
			PersonInfoSaved = true;
		}
	}

	for(int i = 0; i < m_tabCtrl.GetItemCount(); i++)
	{
		CWnd * pWnd = m_tabCtrl.GetView(i);
		if(pWnd->IsKindOf(RUNTIME_CLASS(CMedPerson)) && PersonInfoSaved)  continue;
		pWnd->SendMessage(WM_CONTENTSAVE);
	}
	
	if(((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
	{
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.SendMessage(WM_CONTENTSAVE);
	}
}

void CPathologyView::OnFileSave() 
{
	bool PersonInfoSaved = false;

	if(GetDocument()->m_blh.IsEmpty())
	{
		CWnd * pWnd = m_tabCtrl.GetView(RUNTIME_CLASS(CMedPerson));
		if(pWnd)
		{
			pWnd->SendMessage(WM_CONTENTSAVE);
			PersonInfoSaved = true;
		}
	}

	if(GetDocument()->m_blh.IsEmpty())  return;

	if(((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
	{
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.SendMessage(WM_CONTENTSAVE);
	}
	
	for(int i = 0; i < m_tabCtrl.GetItemCount(); i++)
	{
		CWnd * pWnd = m_tabCtrl.GetView(i);
		if (pWnd->IsKindOf(RUNTIME_CLASS(CMedPerson)) && PersonInfoSaved)  continue;
		//if(pWnd->IsKindOf(RUNTIME_CLASS(CImageDockWnd))){
	//		
	//		continue;
	//	}
		pWnd->SendMessage(WM_CONTENTSAVE);
	}

	CWnd *pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CDBList));
	if(pWnd != NULL)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.SetActiveView(pWnd);
		((CEdit *)pWnd->GetDlgItem(IDC_EDIT_BLH))->SetFocus();
	}

}

void CPathologyView::LoadImageFile(CString fname, bool IstheSame)
{
	if(m_ImageDockWnd.GetSafeHwnd() == NULL)  return;
	
	CImageView *pImageView = NULL;
	CWnd *pWnd = m_ImageDockWnd.GetWindow(GW_CHILD);
	while( pWnd != NULL )
	{
		if( pWnd->IsKindOf(RUNTIME_CLASS(CImageView)) )
		{
			pImageView = (CImageView *)pWnd;
			break;
		}
		pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);
	}
	if( pImageView )
	{
		pImageView->RefreshDIB(fname, IstheSame);

		CWnd *pWnd = m_tabCtrl.GetView(RUNTIME_CLASS(CImageDockWnd));
		if(pWnd != NULL)  m_tabCtrl.SetActiveView(pWnd);

		CloseNormalWordWnd();
	}
}

void CPathologyView::CloseNormalWordWnd()
{
	//-------用户点击切换TAB页面时候，检查关闭打开的常用词窗口-----------
	CWnd * pWnd = m_tabCtrl.GetView(RUNTIME_CLASS(CMedDiagnose));
	if(pWnd && ((CMedDiagnose *)pWnd)->pNormalWord != NULL && ((CMedDiagnose *)pWnd)->pNormalWord->IsWindowVisible())
	{
		((CMedDiagnose *)pWnd)->pNormalWord->ShowWindow(SW_HIDE);
	}

	pWnd = m_tabCtrl.GetView(RUNTIME_CLASS(CMedMaterial));
	if(pWnd && ((CMedMaterial *)pWnd)->pNormalWord != NULL && ((CMedMaterial *)pWnd)->pNormalWord->IsWindowVisible())
	{
		((CMedMaterial *)pWnd)->pNormalWord->ShowWindow(SW_HIDE);
	}

	pWnd = m_tabCtrl.GetView(RUNTIME_CLASS(CMedTbs));
	if(pWnd && ((CMedTbs *)pWnd)->pNormalWord != NULL && ((CMedTbs *)pWnd)->pNormalWord->IsWindowVisible())
	{
		((CMedTbs *)pWnd)->pNormalWord->ShowWindow(SW_HIDE);
	}
	//-------------------------------------------------------------------
}

void CPathologyView::OnClickTabCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if( ((CMainFrame*)AfxGetMainWnd())->m_realtime )
	{
		((CMainFrame*)AfxGetMainWnd())->TerminateTimer();
	}

	if(GetDocument()->m_blh.IsEmpty())
	{
		CWnd * pWnd = m_tabCtrl.GetView(RUNTIME_CLASS(CMedPerson));
		if(pWnd && ((CEdit *)pWnd->GetDlgItem(IDC_EDIT_BLH))->IsWindowEnabled()) 
		{
			CString str;
			pWnd->GetDlgItemText(IDC_EDIT_BLH, str);
			str.TrimLeft();  str.TrimRight();
			if(!str.IsEmpty())
			{
				((CMedPerson *)pWnd)->SetModifiedFlag(true);
				pWnd->SendMessage(WM_CONTENTSAVE);
			}
		}
	}

	CWnd *pWnd;
	//-------------------------------------------------
	pWnd = m_tabCtrl.GetView(RUNTIME_CLASS(CMedPerson));
	if(pWnd != NULL && ((CMedPerson *)pWnd)->GetModifiedFlag())    ((CMedPerson *)pWnd)->SendMessage(WM_CONTENTSAVE);

	pWnd = m_tabCtrl.GetView(RUNTIME_CLASS(CMedMaterial));
	if(pWnd != NULL && ((CMedMaterial *)pWnd)->GetModifiedFlag())  ((CMedMaterial *)pWnd)->SendMessage(WM_CONTENTSAVE);

	pWnd = m_tabCtrl.GetView(RUNTIME_CLASS(CMedDiagnose));
	if(pWnd != NULL && ((CMedDiagnose *)pWnd)->GetModifiedFlag())  ((CMedDiagnose *)pWnd)->SendMessage(WM_CONTENTSAVE);

	pWnd = m_tabCtrl.GetView(RUNTIME_CLASS(CMedTbs));
	if(pWnd != NULL && ((CMedTbs *)pWnd)->GetModifiedFlag())       ((CMedTbs *)pWnd)->SendMessage(WM_CONTENTSAVE);

	if(((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd() && ((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetModifiedFlag())
	{
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.SendMessage(WM_CONTENTSAVE);
	}
	//-------------------------------------------------


	CloseNormalWordWnd();

    pWnd = m_tabCtrl.GetActiveView();

	// 切换至"病历信息"或"病理图像"页面时自动关闭 诊断工作站"详"细信息窗口
	if(!pWnd->IsKindOf(RUNTIME_CLASS(CMedDiagnose)))
	{
		CWnd *pDiagnoseWnd = m_tabCtrl.GetView(RUNTIME_CLASS(CMedDiagnose));
		if(pDiagnoseWnd && ((CMedDiagnose *)pDiagnoseWnd)->pZdex != NULL && ((CMedDiagnose *)pDiagnoseWnd)->pZdex->IsWindowVisible())
		{
			((CMedDiagnose *)pDiagnoseWnd)->pZdex->ShowWindow(SW_HIDE);
		}
	}

	if(pWnd->IsKindOf(RUNTIME_CLASS(CImageDockWnd)))
	{
		CWnd *pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CMedImage));
		if(pWnd != NULL)  ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.SetActiveView(pWnd);

		((CMedImage *)pWnd)->ShowImgItem(0);
		((CMedImage*)pWnd)->setDefault();
	}
	if(pWnd->IsKindOf(RUNTIME_CLASS(CMedPerson)) || pWnd->IsKindOf(RUNTIME_CLASS(CMedMaterial)) || pWnd->IsKindOf(RUNTIME_CLASS(CMedDiagnose)) || pWnd->IsKindOf(RUNTIME_CLASS(CMedTbs)))
	{
		if(pWnd->IsKindOf(RUNTIME_CLASS(CMedMaterial)))
		{
			//刷新暂存按钮
			((CMedMaterial*)pWnd)->GetDlgItem(IDC_BUTTON_ZC)->EnableWindow(!theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh));
			CWnd *pPerson = m_tabCtrl.GetView(RUNTIME_CLASS(CMedPerson));
			if(pPerson != NULL)
			{
				CString bbmcstr;
				((CMedPerson *)pPerson)->GetDlgItemText(IDC_EDIT_BBMC, bbmcstr);
				((CMedMaterial *)pWnd)->SetDlgItemText(IDC_EDIT_QCBW, bbmcstr);
			}
		}

		CWnd *pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CDBList));
		if(pWnd != NULL)
		{
			((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.SetActiveView(pWnd);
			((CEdit *)pWnd->GetDlgItem(IDC_EDIT_BLH))->SetFocus();
		}
	}
	if(pWnd->IsKindOf(RUNTIME_CLASS(CMedWax)) || pWnd->IsKindOf(RUNTIME_CLASS(CMedPreslice)) || pWnd->IsKindOf(RUNTIME_CLASS(CMedAdvice)) || pWnd->IsKindOf(RUNTIME_CLASS(CMedSlice)))
	{
		CWnd *pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CAdviceShow));
		if(pWnd != NULL)  ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.SetActiveView(pWnd);
	}
	
	*pResult = 0;
}

void CPathologyView::OnDocQuickinput() 
{
	CSelfInput mydlg(this);
	if(!GetDocument()->m_blh.IsEmpty())
	{
		ForceSave();
		mydlg.m_InputBlh = GetDocument()->m_blh;

		((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_COMMAND,ID_FILE_NEW);
	}

	CWnd *pWnd = theApp.GetView()->m_tabCtrl.GetActiveView();

	if(mydlg.DoModal() == IDOK && !mydlg.m_Operation_Blh.IsEmpty())
	{
		theApp.OpenDocumentFile(mydlg.m_Operation_Blh);
		if(pWnd != NULL)  theApp.GetView()->m_tabCtrl.SetActiveView(pWnd);

		switch(mydlg.m_ExitCode)
		{
		case 1:
			((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_COMMAND,ID_APP_CAMERA);
			break;
		case 2:
			PostMessage(WM_COMMAND,ID_FILE_PRINT);
			break;
		default:;
		}
	}
	else
	{
		if(!mydlg.m_InputBlh.IsEmpty())
		{
			theApp.OpenDocumentFile(mydlg.m_InputBlh);
			if(pWnd != NULL)  theApp.GetView()->m_tabCtrl.SetActiveView(pWnd);
		}
	}
}

void CPathologyView::OnUpdateDocQuickinput(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(theApp.m_StationType.CompareNoCase("登记工作站") == 0 || theApp.m_StationType.CompareNoCase("取材工作站") == 0 || theApp.m_StationType.CompareNoCase("诊断工作站") == 0 || theApp.m_StationType.CompareNoCase("细胞学工作站") == 0);
}

void CPathologyView::OnFileRptcopy() 
{
	DuplicateRecord(false, false);
}

void CPathologyView::OnFileRptcopyall() 
{
	
	DuplicateRecord(true, false);
}

void CPathologyView::DuplicateRecord(bool IsAllInfo, bool IsOnlyImg)
{
	char instanceUID[100];


	CAreaOut mydlg(this);
	mydlg.m_type = 1000;
	mydlg.m_FieldName = "medicalno";
	mydlg.m_FieldInfo;
	
	mydlg.m_title.Format("指定病理号");
	
	if( mydlg.DoModal() == IDCANCEL )  return;
	mydlg.m_from.TrimLeft();   mydlg.m_from.TrimRight();
	mydlg.m_to.TrimLeft();     mydlg.m_to.TrimRight();
	
	CString mno = mydlg.m_from;
	if(mno.IsEmpty())  return;

	int num = 0;
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = mno;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			num = g_dbcommand.Field(1).asLong();
		}
		
		g_dbconnection.Commit();
	}
	catch(SAException &x)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
		AfxMessageBox((const char*)x.ErrText());
	}
	
	if(num < 1)
	{
		AfxMessageBox("数据库中不存在指定活检编号的登陆记录");
		return;
	}

	if(AfxMessageBox("确定从病理号为《" + mno + "》的记录复制病例信息？",MB_OKCANCEL|MB_ICONQUESTION) == IDCANCEL)  return;

	CString	n_hisid;
	CString n_ordertype;
	CString	n_sqh;
	CString	n_brid;
	CString	n_bdh;
	CString	n_ybh;
	CString	n_mzh;
	CString	n_zyh;
	CString	n_xm;
	CString	n_xb;
	CString	n_nl;
	CString	n_nldw;
	CString n_csrq;
	CString	n_mz;
	CString	n_hf;
	CString	n_zy;
	CString	n_jg;
	CString	n_sfzh;
	CString	n_lxdz;
	CString n_lxdh;
	CString	n_mcyj;
	CString	n_jj;
	CString	n_sjdw;
	CString	n_sjks;
	CString	n_sjys;
	CString	n_bq;
	CString	n_ch;
	CString	n_bblx;
	CString	n_bbqk;
	CString	n_bbmc;
	CString	n_bhgyy;
	CString	n_bbqs;
	CString	n_fb;
	float	n_sf;
	CString	n_qcys;
	CString	n_lczd;
	CString	n_bszy;
	CString	n_sssj;

	SADateTime n_Sjrq, n_Qcrq;

	CString n_Gjsj, n_Zdyj, n_Rysj;

	if(IsOnlyImg) 
	{
		goto Label_ImgCopy;
	}

	try
	{
		SAString cmdstr = "Select * from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = mno;
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			n_hisid = g_dbcommand.Field("hisid").asString();
			n_ordertype = g_dbcommand.Field("ordertype").asString();
			n_sqh = g_dbcommand.Field("sqh").asString();
			n_brid = g_dbcommand.Field("brid").asString();
			n_bdh = g_dbcommand.Field("bdh").asString();
			n_ybh = g_dbcommand.Field("ybh").asString();
			n_mzh = g_dbcommand.Field("mzh").asString();
			n_zyh = g_dbcommand.Field("zyh").asString();
			n_xm = g_dbcommand.Field("xm").asString();
			n_xb = g_dbcommand.Field("xb").asString();
			n_nl = g_dbcommand.Field("nl").asString();
			n_nldw = g_dbcommand.Field("nldw").asString();
			n_csrq = g_dbcommand.Field("csrq").asString();
			n_mz = g_dbcommand.Field("mz").asString();
			n_hf = g_dbcommand.Field("hf").asString();
			n_zy = g_dbcommand.Field("zy").asString();
			n_sfzh = g_dbcommand.Field("sfzh").asString();
			n_lxdz = g_dbcommand.Field("lxdz").asString();
			n_lxdh = g_dbcommand.Field("lxdh").asString();
			n_mcyj = g_dbcommand.Field("mcyj").asString();
			n_jj = g_dbcommand.Field("jj").asString();
			n_sjdw = g_dbcommand.Field("sjdw").asString();
			n_sjks = g_dbcommand.Field("sjks").asString();
			n_sjys = g_dbcommand.Field("sjys").asString();
			n_bq = g_dbcommand.Field("bq").asString();
			n_ch = g_dbcommand.Field("ch").asString();
			n_bblx = g_dbcommand.Field("bblx").asString();
			n_bbqk = g_dbcommand.Field("bbqk").asString();
			n_bbmc = g_dbcommand.Field("bbmc").asString();
			n_bhgyy = g_dbcommand.Field("bhgyy").asString();
			n_bbqs = g_dbcommand.Field("bbqs").asString();
			n_fb = g_dbcommand.Field("fb").asString();
			n_sf = g_dbcommand.Field("sf").asDouble();
			n_qcys = g_dbcommand.Field("qcys").asString();
			n_lczd = g_dbcommand.Field("lczd").asString();
			n_bszy = g_dbcommand.Field("bszy").asString();
			n_sssj = g_dbcommand.Field("sssj").asString();

			n_Sjrq = g_dbcommand.Field("sjrq").asDateTime();
			n_Qcrq = g_dbcommand.Field("qcrq").asDateTime();

			n_Rysj = g_dbcommand.Field("rysj").asString();
			n_Gjsj = g_dbcommand.Field("gjsj").asString();
			n_Zdyj = g_dbcommand.Field("zdyj").asString();
		}
		
		g_dbconnection.Commit();
	}
	catch(SAException &x)
	{
		try
		{
			g_dbconnection.Rollback();
		}
		catch(SAException &)
		{
		}
		AfxMessageBox((const char*)x.ErrText());
	}

	CWnd *pWnd;
	pWnd = m_tabCtrl.GetView(RUNTIME_CLASS(CMedPerson));
	if(pWnd != NULL) 
	{
		if(IsAllInfo)
		{
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_ORDERTYPE, n_ordertype);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_SQH,   n_sqh);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_BRID,  n_brid);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_BDH,   n_bdh);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_YBH,   n_ybh);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_MZH,   n_mzh);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_ZYH,   n_zyh);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_XM,    n_xm);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_XB,    n_xb);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_NL,    n_nl);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_NLDW,  n_nldw);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_CSRQ,  n_csrq);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_MZ,    n_mz);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_HF,    n_hf);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_ZY,    n_zy);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_SFZH,  n_sfzh);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_LXDZ,  n_lxdz);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_LXDH,  n_lxdh);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_MCYJ,  n_mcyj);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_JJ,    n_jj);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_SJDW,  n_sjdw);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_SJKS,  n_sjks);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_SJYS,  n_sjys);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_BQ,    n_bq);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_CH,    n_ch);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_BBLX,  n_bblx);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_BBQK,  n_bbqk );
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_BBMC,  n_bbmc);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_BHGYY, n_bhgyy);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_BBQS,  n_bbqs);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_FB,    n_fb);
			CString sfstr;  sfstr.Format("%f", n_sf);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_SF,    sfstr);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_QCYS,  n_qcys);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_LCZD,  n_lczd);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_BSZY,  n_bszy);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_SSSJ,  n_sssj);
			
			COleDateTime curt;
			curt.SetDateTime(n_Sjrq.GetYear(), n_Sjrq.GetMonth(), n_Sjrq.GetDay(), n_Sjrq.GetHour(), n_Sjrq.GetMinute(), n_Sjrq.GetSecond());
			((CMedPerson *)pWnd)->m_Time_Sjrq.SetTime(curt);
			
			curt.SetDateTime(n_Qcrq.GetYear(), n_Qcrq.GetMonth(), n_Qcrq.GetDay(), n_Qcrq.GetHour(), n_Qcrq.GetMinute(), n_Qcrq.GetSecond());
			((CMedPerson *)pWnd)->m_Time_Qcrq.SetTime(curt);

			((CMedPerson *)pWnd)->SetModifiedFlag(true);
		}
	}

	pWnd = m_tabCtrl.GetView(RUNTIME_CLASS(CMedDiagnose));
	if(pWnd != NULL) 
	{
		if(IsAllInfo)
		{
			((CMedDiagnose *)pWnd)->SetDlgItemText(IDC_EDIT_RYSJ, n_Rysj);
		}
		
		((CMedDiagnose *)pWnd)->SetDlgItemText(IDC_EDIT_GJSJ, n_Gjsj);
		((CMedDiagnose *)pWnd)->SetDlgItemText(IDC_EDIT_ZDYJ, n_Zdyj);
		
		((CMedDiagnose *)pWnd)->SetModifiedFlag();
	}
	
	pWnd = m_tabCtrl.GetView(RUNTIME_CLASS(CMedTbs));
	if(pWnd != NULL)
	{
		((CMedTbs *)pWnd)->SetDlgItemText(IDC_EDIT_ZDYJ, n_Zdyj);

		((CMedTbs *)pWnd)->SetModifiedFlag();
	}

	pWnd = m_tabCtrl.GetView(RUNTIME_CLASS(CMedMaterial));
	if(pWnd != NULL)
	{
		if(IsAllInfo)
		{
			((CMedMaterial *)pWnd)->SetDlgItemText(IDC_EDIT_RYSJ, n_Rysj);

			((CMedMaterial *)pWnd)->SetModifiedFlag();
		}
	}

Label_ImgCopy:;

	if(IsAllInfo && !GetDocument()->m_blh.IsEmpty())
	{
		// 报告管理报告复制中复制全部信息，图像也能复制
		SAString imgdataset[100];
		int count = 0;
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = mno;
			
			g_dbcommand.Execute();
			while(g_dbcommand.FetchNext())
			{
				imgdataset[count++] = g_dbcommand.Field("imgdataset").asBLob();
				if(count >= 99) break;
			}
			
			g_dbconnection.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
		}

		int maxno = 0;
		try
		{
			SAString cmdstr = "Select serialno from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno order by serialno desc";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = GetDocument()->m_blh;
			
			g_dbcommand.Execute();
			if(g_dbcommand.FetchNext())
			{
				maxno = g_dbcommand.Field("serialno").asLong();
			}
			
			g_dbconnection.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				g_dbconnection.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
		}

		for(int i = 0; i < count; i++)
		{
			if(!imgdataset[i].IsEmpty())
			{
				dcmGenerateUniqueIdentifier(instanceUID, SITE_INSTANCE_UID_ROOT);

				try
				{
					SAString cmdstr = "Insert into " + theApp.TABLE_IMAGE + "(medicalno, instanceUId, serialno, imgdataset) values(:medicalno,:instanceUID, :serialno,:imgdataset)";
					
					g_dbcommand.setCommandText( cmdstr );
					g_dbcommand.Param("medicalno").setAsString() = GetDocument()->m_blh;
					g_dbcommand.Param("instanceUID").setAsString() = instanceUID;
					g_dbcommand.Param("serialno").setAsLong()    = ++maxno;
					g_dbcommand.Param("imgdataset").setAsBLob()  = imgdataset[i];
					
					g_dbcommand.Execute();
					g_dbconnection.Commit();
				}
				catch(SAException &x)
				{
					try
					{
						g_dbconnection.Rollback();
					}
					catch(SAException &)
					{
					}
					AfxMessageBox((const char*)x.ErrText());
				}

				imgdataset[i].Empty();
			}
		}

		if(count)
		{
			CWnd *pWnd = ((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CMedImage));
			if(pWnd != NULL)  ((CMedImage *)pWnd)->LoadThumbnail();
		}
	}
}

void CPathologyView::OnFileRptcopyimg() 
{
	DuplicateRecord(true, true);
}
