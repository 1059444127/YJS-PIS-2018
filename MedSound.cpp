// CMedSound : implementation file
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "Pathology.h"
#include "Pathologyview.h"
#include "Pathologydoc.h"
#include "MedSound.h"
#include "MainFrm.h"
#include "CanvasImage.h"
#include "CWAVInfo.h"

#define SNDICON_WIDTH  72
#define SNDICON_HEIGHT 72

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMedSound
extern CRect SizeRectWithConstantAspectRatio(CRect* rcScreen,  CSize  sizePicture,  BOOL   bCenter);

CMedSound::CMedSound()
	: CXTResizeFormView(CMedSound::IDD)
{
	//{{AFX_DATA_INIT(CMedSound)
	//}}AFX_DATA_INIT

	m_bIsFirstInit = true;
}

CMedSound::~CMedSound()
{
	// TODO: add destruction code here.

}

IMPLEMENT_DYNCREATE(CMedSound, CXTResizeFormView)

BEGIN_MESSAGE_MAP(CMedSound, CXTResizeFormView)
	//{{AFX_MSG_MAP(CMedSound)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_THUMBNAIL, OnBegindragListThumbnail)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, OnButtonRecord)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_THUMBNAIL, OnDblclkListThumbnail)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_THUMBNAIL, OnKeydownListThumbnail)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMedSound message handlers
extern CPathologyApp theApp;

void CMedSound::DoDataExchange(CDataExchange* pDX) 
{
	CXTResizeFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMedSound)
	DDX_Control(pDX, IDC_BUTTON_STOP, m_btStop);
	DDX_Control(pDX, IDC_BUTTON_RECORD, m_btRecord);
	//}}AFX_DATA_MAP
}

void CMedSound::OnInitialUpdate() 
{
	CXTResizeFormView::OnInitialUpdate();
	
	if (m_bIsFirstInit)
	{
		m_bIsFirstInit = false;
		
		SetResize(IDC_LIST_THUMBNAIL, SZ_TOP_LEFT,  SZ_BOTTOM_RIGHT);
		SetResize(IDC_RECYCLE, SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
		SetResize(IDC_BUTTON_RECORD, SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
		SetResize(IDC_BUTTON_STOP, SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
		
		m_ImageListThumb.Create(SNDICON_WIDTH, SNDICON_HEIGHT, ILC_COLOR24, 0, 1);
		((CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL))->SetImageList(&m_ImageListThumb, LVSIL_NORMAL);
		((CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL))->SetIconSpacing(110,100); // 2006.08.05 ÉèÖÃÓ°ÏñËõÂÔÀ¸Í¼Ïñ×Ý/ºáÅÅÁÐ¼ä¾à
		
		m_bDragging = FALSE;
		m_pDragImage = NULL;
		
		const DWORD STYLE_ALL = BS_LEFT | BS_RIGHT | BS_CENTER;
		m_btRecord.SetBitmap(CSize(32,32),IDB_BITMAP_MIC);
		m_btRecord.SetXButtonStyle(BS_XT_XPFLAT); 
		m_btRecord.ModifyStyle(STYLE_ALL & ~BS_CENTER, BS_CENTER);
		
		m_btStop.SetBitmap(CSize(32,32),IDB_BITMAP_STOP);
		m_btStop.SetXButtonStyle(BS_XT_XPFLAT); 
		m_btStop.ModifyStyle(STYLE_ALL & ~BS_CENTER, BS_CENTER);

		m_ToolTip.Create(this);
		m_ToolTip.Activate(TRUE);
		m_ToolTip.AddTool(&m_btRecord, "Â¼Òô");
		m_ToolTip.AddTool(&m_btStop, "Í£Ö¹Â¼Òô/²¥·Å");
	}
}

void CMedSound::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bDragging)
	{
		// Release mouse capture, so that other controls can get control/messages
		ReleaseCapture ();

		// Note that we are NOT in a drag operation
		m_bDragging = FALSE;

		// End dragging image
		m_pDragImage->DragLeave (GetDesktopWindow ());
		m_pDragImage->EndDrag ();

		delete m_pDragImage; //must delete it because it was created at the beginning of the drag
	
		CPoint pt(point);	//get our current mouse coordinates
		ClientToScreen(&pt); //convert to screen coordinates
		CRect Recycle_Area;
		GetDlgItem(IDC_RECYCLE)->GetWindowRect(&Recycle_Area);
		if(Recycle_Area.PtInRect(pt))
		{
			DeleteListItem();
		}
	}
	
	CXTResizeFormView::OnLButtonUp(nFlags, point);
}

void CMedSound::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bDragging)
	{
		CPoint pt(point);	//get our current mouse coordinates
		ClientToScreen(&pt); //convert to screen coordinates
		m_pDragImage->DragMove(pt); //move the drag image to those coordinates
		// Unlock window updates (this allows the dragging image to be shown smoothly)
		m_pDragImage->DragShowNolock(false);

		//If we are hovering over a CListCtrl we need to adjust the highlights
		CRect Recycle_Area;
		GetDlgItem(IDC_RECYCLE)->GetWindowRect(&Recycle_Area);
		if(Recycle_Area.PtInRect(pt))
		{			
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		}
		else
		{
			SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_HANDON));
		}
		// Lock window updates
		m_pDragImage->DragShowNolock(true);
	}
	
	CXTResizeFormView::OnMouseMove(nFlags, point);
}

void CMedSound::OnBegindragListThumbnail(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	m_pDragImage = new CImageList;
	if( m_pListThumbnail->GetSelectedCount() > 1 )
	{
		m_pDragImage->Create(IDB_MULTI_DOC, 32, 1, RGB(0,128,128));
	}
	else
	{
		m_pDragImage->Create(IDB_SINGLE_DOC, 32, 1, RGB(0,128,128));
	}

	m_pDragImage->BeginDrag(0, CPoint(4, 0));
	m_pDragImage->DragEnter(GetDesktopWindow(), pNMListView->ptAction);
	
	m_bDragging = TRUE;
	SetCapture ();

	*pResult = 0;
}

void CMedSound::LoadThumbnail()
{
	if(m_ImageListThumb.GetSafeHandle() == 0)  return;
	
	theApp.ConnectDatabase();
	//----------------------------------------------------
	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
	// hold the window update to avoid flicking
	m_pListThumbnail->SetRedraw(FALSE);

	// reset our image list
	for(int i=0; i< m_ImageListThumb.GetImageCount(); i++)
		m_ImageListThumb.Remove(i);	

	// remove all items from list view
	if( m_pListThumbnail->GetItemCount() != 0 )
		m_pListThumbnail->DeleteAllItems();
	
	// set the size of the image list
	int SoundCount = 0;
	if(!theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		try
		{
			SAString cmdstr = "Select count(*) from " + theApp.TABLE_SOUND + "  where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			
			g_dbcommand.Execute();
			g_dbcommand.FetchNext();
			SoundCount = g_dbcommand.Field(1).asLong();
			
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
			return;
		}
	}

	m_ImageListThumb.SetImageCount(SoundCount);

	if(SoundCount > 0)
	{
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_SOUND + "  where medicalno = :medicalno order by serialno asc";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			
			g_dbcommand.Execute();
			int index = 0;
			while(g_dbcommand.FetchNext())
			{
				SAString Bstr = g_dbcommand.Field("snddataset").asBLob();
				if( !Bstr.IsEmpty() )
				{
					const void * binbuf = (const void *)Bstr;
					BYTE * dataptr = (BYTE *)binbuf;

					CString transname;
					transname.Format("%s_cmipstrans.wav", theApp.GetView()->GetDocument()->m_blh);

					CFile file;
					CFileException fe;
					
					CString fstr = theApp.m_SysTmpPath;
					fstr += transname;
					remove(fstr);
					if (file.Open( fstr , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
					{
						file.WriteHuge( dataptr , Bstr.GetLength());
						file.Close();
					}

					CWAVInfo wavinfo;
					wavinfo.loadInfo( fstr );
					int seconds = wavinfo.getLengthInSeconds();

					CString ExtInfo;
                    ExtInfo.Format("(%d ·Ö %d Ãë)", seconds/60, seconds - (seconds/60) * 60);

					CBitmap * pImage = new CBitmap();		 
					pImage->LoadBitmap(IDB_BITMAP_WAV);
					
					// add bitmap to our image list
					m_ImageListThumb.Replace( index, pImage, NULL );
					
					// put item to display
					// set the image file name as item text
					int SerialNo = g_dbcommand.Field("serialno").asLong();
					CString DispStr;
					DispStr.Format("%d", SerialNo);
					DispStr += ExtInfo;
					
					m_pListThumbnail->InsertItem( index, DispStr, index );
					index++;
					
					delete pImage;
					//*********************************************************
					
					remove(fstr);
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
			return;
		}
	}

	// let's show the new thumbnails
	m_pListThumbnail->SetRedraw(TRUE); 

	//m_pListThumbnail->EnsureVisible(NewID,FALSE);
	RefreshTabText();
}

void CMedSound::OnButtonRecord() 
{
	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		AfxMessageBox("ÄúÎ´´ò¿ª²¡Àý£¬Çë´ò¿ªºóÔÙ²Ù×÷£¡");
		return;
	}

	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))  return;

	CString transname;
	transname.Format("%s_cmipstrans.wav", theApp.GetView()->GetDocument()->m_blh);
	
	CString Song = theApp.m_SysTmpPath;
	Song += transname;
					
	remove(Song);

	((CMainFrame*)AfxGetMainWnd())->Sound_Startrecord(Song);
}

void CMedSound::OnButtonStop() 
{
	((CMainFrame*)AfxGetMainWnd())->Sound_StopPlaying();
}

void CMedSound::RefreshTabText()
{
	CWnd * pMainWnd = AfxGetMainWnd();
	if(pMainWnd == NULL || !pMainWnd->IsKindOf(RUNTIME_CLASS(CMainFrame)))  return;

	CWnd *pWnd = ((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.GetView(RUNTIME_CLASS(CMedSound));
	if(pWnd != NULL)
	{
		CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
		CString str;
		if(m_pListThumbnail->GetItemCount() > 0)
		{
			str.Format("ÓïÒô(%d)", m_pListThumbnail->GetItemCount());
		}
		else
		{
			str.Format("ÓïÒô");
		}
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.GetTabCtrl().SetTabText(pWnd, str);
	}
}

BOOL CMedSound::PreTranslateMessage(MSG* pMsg) 
{
	if( m_hWnd )
    {
        m_ToolTip.RelayEvent(pMsg);
		return CXTResizeFormView::PreTranslateMessage(pMsg);
    }
    return (FALSE);
	
	//return CXTResizeFormView::PreTranslateMessage(pMsg);
}

void CMedSound::OnDblclkListThumbnail(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ShowSelectedItem();
	
	*pResult = 0;
}

void CMedSound::OnKeydownListThumbnail(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	switch(pLVKeyDow->wVKey) {
	case VK_DELETE:
		DeleteListItem();
		break;
	case VK_SPACE:
		ShowSelectedItem();
		break;
	}
	
	*pResult = 0;
}

void CMedSound::DeleteListItem()
{
	if(!(RIGHT_SCTXYY(theApp.m_Cpp_Right) && !theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh)))  return;

	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
	int nItem = -1;
	for(int i = 0; i < m_pListThumbnail->GetSelectedCount(); i ++ )
	{
		nItem = m_pListThumbnail->GetNextItem(nItem, LVNI_SELECTED);
		if( nItem >= 0 )
		{
			CString str = m_pListThumbnail->GetItemText(nItem, 0);
			str = str.Left(str.Find('('));
			
			theApp.ConnectDatabase();
			try
			{
				SAString cmdstr = "Delete from " + theApp.TABLE_SOUND + "  where medicalno = :medicalno and serialno = :serialno";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
				g_dbcommand.Param("serialno").setAsLong()    = atoi(str);
				
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
				return;
			}
		}
	}
	
	if(m_pListThumbnail->GetSelectedCount() > 0)
	{
		((CMainFrame*)AfxGetMainWnd())->Sound_Play("recycle.wav");
		
		LoadThumbnail();
	}
}

void CMedSound::ShowSelectedItem()
{
	CListCtrl *m_pListThumbnail = (CListCtrl *)GetDlgItem(IDC_LIST_THUMBNAIL);
	if( m_pListThumbnail->GetSelectedCount() > 0 )
	{
		int nItem = -1;
		nItem = m_pListThumbnail->GetNextItem(nItem, LVNI_SELECTED);
		if( nItem >= 0 )
		{
			if(m_pListThumbnail->GetItemCount() > 0 && nItem < m_pListThumbnail->GetItemCount())
			{
				CString str = m_pListThumbnail->GetItemText(nItem, 0);
				str = str.Left(str.Find('('));

				theApp.ConnectDatabase();
				try
				{
					SAString cmdstr = "Select * from " + theApp.TABLE_SOUND + "  where medicalno = :medicalno and serialno = :serialno";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
					g_dbcommand.Param("serialno").setAsLong()    = atoi(str);
					
					g_dbcommand.Execute();
					if(g_dbcommand.FetchNext())
					{
						SAString Bstr = g_dbcommand.Field("snddataset").asBLob();
						if( !Bstr.IsEmpty() )
						{
							const void * binbuf = (const void *)Bstr;
							BYTE * dataptr = (BYTE *)binbuf;
							
							CString transname;
							transname.Format("%s_cmipstrans.wav", theApp.GetView()->GetDocument()->m_blh);
							
							CFile file;
							CFileException fe;
							
							CString fstr = theApp.m_SysTmpPath;
							fstr += transname;
							remove(fstr);
							if (file.Open( fstr , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
							{
								file.WriteHuge( dataptr , Bstr.GetLength());
								file.Close();
							}
							
							// Play Selected Sound File
							((CMainFrame*)AfxGetMainWnd())->Sound_Startplay(fstr);
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
					return;
				}
			}
		}
	}
}
