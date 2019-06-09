// PathologyDoc.cpp : implementation of the CPathologyDoc class
//

#include "stdafx.h"
#include "Pathology.h"
#include "PathologyDoc.h"
#include "PathologyView.h"
#include "MainFrm.h"
#include "MedPerson.h"
#include "MedImage.h"
#include "MedSound.h"
#include "ImageView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPathologyDoc

IMPLEMENT_DYNCREATE(CPathologyDoc, CDocument)

BEGIN_MESSAGE_MAP(CPathologyDoc, CDocument)
	//{{AFX_MSG_MAP(CPathologyDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPathologyDoc construction/destruction
extern CPathologyApp theApp;

CPathologyDoc::CPathologyDoc()
{
	// TODO: add one-time construction code here
    m_IsNewRecord = true;
	m_blh.Empty();

	m_bNewRegister = FALSE;
}

CPathologyDoc::~CPathologyDoc()
{
}

BOOL CPathologyDoc::OnNewDocument()
{
	((CMainFrame *)AfxGetMainWnd())->SaveModifiedRecord(false);

	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
 	((CMainFrame *)AfxGetMainWnd())->SetActiveView(theApp.GetView());

	m_IsNewRecord = true;
	m_blh.Empty();

	CWnd *pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CMedImage));
	if(pWnd != NULL)  ((CMedImage *)pWnd)->LoadThumbnail();

	pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CMedSound));
	if(pWnd != NULL)  ((CMedSound *)pWnd)->LoadThumbnail();

    if(((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
	{
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.PostMessage(WM_CONTENTREFRESH);

		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.ReflectSelection();
	}
	
	if(theApp.GetView()->m_ImageDockWnd.GetSafeHwnd())
	{
		CImageView *pImageView = NULL;
		CWnd *pWnd = theApp.GetView()->m_ImageDockWnd.GetWindow(GW_CHILD);
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
			pImageView->PostMessage(WM_CONTENTREFRESH);
		}
	}

	SetTitle("");

    theApp.m_Refresh_Flag_Check = theApp.m_Refresh_Flag_Collect = true;

	pWnd = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedPerson));
	if(pWnd != NULL) 
	{
		theApp.GetView()->m_tabCtrl.SetActiveView(pWnd);
		m_bNewRegister = TRUE;
	}
	
	((CMainFrame *)AfxGetMainWnd())->RefreshStatusBar();

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CPathologyDoc serialization

void CPathologyDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPathologyDoc diagnostics

#ifdef _DEBUG
void CPathologyDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPathologyDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPathologyDoc commands

BOOL CPathologyDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	//-----------------------------------------
	if(theApp.Is_Batch_Printing == false && !theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		theApp.GetView()->ForceSave();
	}
	//-----------------------------------------

	DeleteContents();

	((CMainFrame *)AfxGetMainWnd())->SetActiveView(theApp.GetView());

	m_IsNewRecord = false;

	m_bNewRegister = FALSE;

	char		m_szDrive[_MAX_DRIVE];
	char		m_szDir[_MAX_DIR];
	char		m_szFname[_MAX_FNAME];
	char		m_szExt[_MAX_EXT];
	_splitpath(lpszPathName, m_szDrive, m_szDir, m_szFname, m_szExt);

	m_blh = m_szFname;

	//-----------------------------------------------------------------
	int RecordDbtype = -1;
	try
	{
		SAString cmdstr = "Select dbtype from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = m_blh;
		
		g_dbcommand.Execute();
		
		if(g_dbcommand.FetchNext())
		{
			RecordDbtype = g_dbcommand.Field("dbtype").asLong();
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
	if(RecordDbtype >= 0 && RecordDbtype != theApp.m_DB_Type_No)
	{
		CString RecordDbname;  RecordDbname.Empty();
		for(int i = 0; i < theApp.m_DBNo_list.GetSize(); i++)
		{
			if(theApp.m_DBNo_list.GetAt(i) == RecordDbtype)
			{
				RecordDbname = theApp.m_DBName_list.GetAt(i);
				break;
			}
		}

		if(!RecordDbname.IsEmpty())
		{
			theApp.ReflectDBChange(RecordDbtype, RecordDbname, false);
		}
	}
	//-----------------------------------------------------------------

	CWnd *pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CMedImage));
	if(pWnd != NULL)  ((CMedImage *)pWnd)->LoadThumbnail();

	pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CMedSound));
	if(pWnd != NULL)  ((CMedSound *)pWnd)->LoadThumbnail();


	if(((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
	{
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.PostMessage(WM_CONTENTREFRESH);

		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.ReflectSelection();
	}
	
	if(theApp.GetView()->m_ImageDockWnd.GetSafeHwnd())
	{
		CImageView *pImageView = NULL;
		CWnd *pWnd = theApp.GetView()->m_ImageDockWnd.GetWindow(GW_CHILD);
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
			pImageView->PostMessage(WM_CONTENTREFRESH);
		}
	}

	SetTitle("");

    theApp.m_Refresh_Flag_Check = theApp.m_Refresh_Flag_Collect = true;

	((CMainFrame *)AfxGetMainWnd())->RefreshStatusBar();
	return TRUE;
}

CString CPathologyDoc::GenTitleString()
{
	CString nTitle;  nTitle.Empty();
	if(theApp.m_StationType.CompareNoCase("制片工作站")     == 0 || 
	   theApp.m_StationType.CompareNoCase("常规切片工作站") == 0 ||
	   theApp.m_StationType.CompareNoCase("特检工作站")     == 0 ||
	   theApp.m_StationType.CompareNoCase("细胞学制片工作站") == 0 )
	{
		return nTitle;
	}

	if(m_blh.IsEmpty())
	{
		nTitle.Format("登录新病例");
	}
	else
	{
		nTitle.Format("病理号: ");
		nTitle += m_blh;
		CString xm, xb, nl, nldw, zyh, sjks,shys, zzys, czys;
		xm.Empty(); xb.Empty(); nl.Empty(); nldw.Empty(); zyh.Empty(); shys.Empty(); zzys.Empty(); czys.Empty();
		sjks.Empty();
		theApp.ConnectDatabase();
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = m_blh;
			
			g_dbcommand.Execute();
			
			if(g_dbcommand.FetchNext())
			{
				xm   = g_dbcommand.Field("xm").asString();
				xb   = g_dbcommand.Field("xb").asString();
				nl   = g_dbcommand.Field("nl").asString();
				nldw = g_dbcommand.Field("nldw").asString();
				zyh  = g_dbcommand.Field("zyh").asString();
				sjks = g_dbcommand.Field("sjks").asString();
				shys = g_dbcommand.Field("shys").asString();
				zzys = g_dbcommand.Field("zzys").asString();
				czys = g_dbcommand.Field("czys").asString();
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
		
		nTitle += "  ";
		if(!xm.IsEmpty())
		{
			nTitle += xm;
			nTitle += "  ";
		}
		if(!xb.IsEmpty())
		{
			nTitle += xb;
			nTitle += "  ";
		}
		if(!nl.IsEmpty())
		{
			nTitle += nl;
			nTitle += nldw;
			nTitle += "  ";
		}
		if(!zyh.IsEmpty())
		{
			nTitle += "住院号: ";
			nTitle += zyh;
			nTitle += "  ";
		}
		if(!sjks.IsEmpty()){
			nTitle += sjks;
			nTitle += "  ";
		}
		if(!shys.IsEmpty() || !zzys.IsEmpty() || !czys.IsEmpty())
		{
			nTitle += "诊断医师: ";
			//-----------------------
			if(czys == zzys || czys == shys)  czys.Empty();
			if(zzys == shys)  zzys.Empty();
			//-----------------------
			CString ys; ys.Empty();
			if(!shys.IsEmpty())
			{
				if(!ys.IsEmpty()) ys += "、";
				ys += shys;
			}
			if(!zzys.IsEmpty())
			{
				if(!ys.IsEmpty()) ys += "、";
				ys += zzys;
			}
			if(!czys.IsEmpty())
			{
				if(!ys.IsEmpty()) ys += "、";
				ys += czys;
			}
			nTitle += ys;
		}
	//	nTitle += "]";
	}

	nTitle.Replace("病理号", theApp.m_DB_Type_Name);
	return nTitle;
}

void CPathologyDoc::SetTitle(LPCTSTR lpszTitle) 
{
	// TODO: Add your specialized code here and/or call the base class
	CDocument::SetTitle(GenTitleString());	
}
