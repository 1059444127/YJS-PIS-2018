// CMenuEdit : implementation file
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Pathology.h"
#include "Resource.h"
#include "MenuEdit.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int CX_BUTTON = ( ::GetSystemMetrics( SM_CXHTHUMB ) + 
                      ( ::GetSystemMetrics( SM_CXEDGE ) * 2 ) );

/*
class CXTBrowseBtn : public CXTButton
{
	DECLARE_DYNAMIC(CXTBrowseBtn)

public:

	CXTBrowseBtn()
	{
        m_nBorderGap = 0;    
        m_nImageGap  = 0;
        m_dwBStyle   = NULL;
        m_pEditWnd   = NULL;
        m_dwxStyle   = BS_XT_SHOWFOCUS;
        
        // Load the image list used by control bar frame buttons.
        HIMAGELIST hImageList = ImageList_LoadBitmap( xtAfxData.hInstance, 
            MAKEINTRESOURCE( XT_IDB_SCROLL_ARW ), 11, 1, RGB( 0xFF,0x00,0xFF ) );
        
        if ( hImageList != NULL )
        {
            m_hIconHEnabled = ImageList_ExtractIcon(
                hInstance, hImageList, 4);
            ASSERT(m_hIconHEnabled != NULL);
            
            // Free image list.
            ImageList_Destroy(hImageList);
            hImageList = NULL;
        }
	}
	
    virtual ~CXTBrowseBtn()
	{
	}

protected:
	
	DWORD   		m_dwBStyle;		 // Search type
	UINT			m_nID;			 // Resource id for push button
	UINT			m_nMenu;		 // Popup menu id
	UINT			m_nSubMenuIndx;	 // Index of popup submenu.
	CXTBrowseEdit*	m_pEditWnd;		 // Parent edit control.
    CXTIconHandle   m_hIconHEnabled; // Horz button icon enabled

public:

    BOOL CreateBrowseButton(UINT nID)
    {
        CString string;
        string.LoadString(XT_IDS_DOT);

        DWORD dwStyle = (m_pEditWnd->GetStyle() & WS_VISIBLE) | 
            WS_CHILD | WS_TABSTOP | BS_CENTER | BS_VCENTER;

        if( !CXTButton::Create(string, dwStyle, CRect(0,0,0,0),
			m_pEditWnd->GetParent(), nID))
        {
            TRACE0( "Failed to create browse edit button.\n" );
            return FALSE;
        }
        
        // Set the font for this control.
        SetFont(&xtAfxData.font);
        ModifyStyle(0, BS_OWNERDRAW);
        
        // set the tab order and enable the window based on parent's state.
        SetWindowPos( m_pEditWnd, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE );
        EnableWindow( m_pEditWnd->IsWindowEnabled( ));

        return TRUE;
    }

    BOOL CreatePopupButton(UINT nID)
    {
        DWORD dwStyle = (m_pEditWnd->GetStyle() & WS_VISIBLE) | 
            WS_CHILD | WS_TABSTOP | BS_CENTER | BS_VCENTER | BS_ICON;

        if( !CXTButton::Create(NULL, dwStyle, CRect(0,0,0,0),
			m_pEditWnd->GetParent(), nID))
        {
            TRACE0( "Failed to create browse edit button.\n" );
            return FALSE;
        }
        
        // set the arrow icon that is displayed on the button.
        SetIcon(CSize(10,10), m_hIconHEnabled);
        ModifyStyle(0, BS_OWNERDRAW);
        
        // set the tab order and enable the window based on parent's state.
        SetWindowPos( m_pEditWnd, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE );
        EnableWindow( m_pEditWnd->IsWindowEnabled( ));

        return TRUE;
    }
    
    virtual BOOL Create(CXTBrowseEdit* pEditWnd, DWORD dwBStyle, UINT nID, UINT nMenu=-1, UINT nSubMenuIndx=-1)
    {
        
        ASSERT_VALID(pEditWnd);				 // Must be valid
        ASSERT_VALID(pEditWnd->GetParent());
        
        // Set the pointers to the edit and edit parent windows.
        m_pEditWnd = pEditWnd;
        
        // Set the browse style and menu resource 
        // ID information (if any).
        m_dwBStyle		= dwBStyle;
        m_nMenu			= nMenu;
        m_nSubMenuIndx	= nSubMenuIndx;
        
        switch (m_dwBStyle & (BES_XT_CHOOSEDIR | BES_XT_CHOOSEFILE | BES_XT_POPUPMENU))
        {
        // If this is a browse for directory or file control.
        case BES_XT_CHOOSEDIR:
        case BES_XT_CHOOSEFILE:
            CreateBrowseButton(nID);
			break;

        // If this is a popup menu control.
        case BES_XT_POPUPMENU:
            CreatePopupButton(nID);
			break;

        // Style was not defined return FALSE.
		default:
			return FALSE;
        }

        return TRUE;
	}

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if ( pMsg->message == WM_KEYDOWN ) // activate if space or enter key pressed...
		{
			if ( pMsg->wParam == VK_RETURN ||
				 pMsg->wParam == VK_SPACE )
			{
				OnClicked();
				return TRUE; // eat message...
			}
		}

		return CXTButton::PreTranslateMessage(pMsg);
	}

	afx_msg void OnKillFocus(CWnd* pNewWnd)
	{
		CXTButton::OnKillFocus(pNewWnd);
		
		// NOTE: Future bug fix should come here -
		// for this is a slave of a 'temporary' edit box, we should return the
		// focus to that box (i.e. in a CXTBrowseDialog).
		//
		//if (pNewWnd != m_pEditWnd)
		//{
		//	m_pEditWnd->SetFocus();
		//}
		//
	}
    
    afx_msg void OnClicked()
    {
        m_pEditWnd->OnBrowse();
    }
    
	DECLARE_MESSAGE_MAP()

	friend class CXTBrowseEdit;
};

IMPLEMENT_DYNAMIC(CXTBrowseBtn, CXTButton)

BEGIN_MESSAGE_MAP(CXTBrowseBtn, CXTButton)
	//{{AFX_MSG_MAP(CXTBrowseBtn)
	ON_WM_KILLFOCUS()
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


*/


/////////////////////////////////////////////////////////////////////////////
// CMenuEdit
extern CPathologyApp theApp;

CMenuEdit::CMenuEdit()
{
	// TODO: add construction code here.
    m_info.Empty();
	mute = false;
	
	m_TableName.Empty();

}

CMenuEdit::~CMenuEdit()
{
	// TODO: add destruction code here.

}

IMPLEMENT_DYNAMIC(CMenuEdit, CXTBrowseEdit)

BEGIN_MESSAGE_MAP(CMenuEdit, CXTBrowseEdit)
	//{{AFX_MSG_MAP(CMenuEdit)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuEdit message handlers
void CMenuEdit::PopupMenu()
{



	if( m_info.IsEmpty() )  return;


	if(mute)  return;
	mute = true;

    if (m_pBrowseBtn->GetSafeHwnd())
        m_pBrowseBtn->SetState(TRUE);
    
    CRect rect;
    m_pBrowseBtn->GetWindowRect(&rect);
    
    CXTMenu menu;
	menu.CreatePopupMenu();

	int cmdid = 3;

	char ch = m_info.GetAt(0);
	if(ch == '{')
	{
		CStringArray flist;
		CString str = m_info;
		str = str.Right(str.GetLength() - 1);
		while(1)
		{
			CString tmp = str.Left(str.Find('}'));
			flist.Add(tmp);
			str = str.Right(str.GetLength() - str.Find('}') - 1);
			if( str.Find('{') == -1)
			{
				break;
			}
			else
			{
				str = str.Right(str.GetLength() - str.Find('{') - 1);
			}
		}
		for(int i = 0; i < flist.GetSize(); i++ )
		{
			CString firstname = flist.GetAt(i);
			firstname = "{" + firstname + "}";
			
			CXTMenu submenu;
			submenu.CreatePopupMenu();
			
			str = m_info;
			if( str.Find(firstname) != -1 )
			{
				str = str.Right(str.GetLength() - str.Find(firstname));
				str = str.Right(str.GetLength() - str.Find('}') - 1);
				
				if( str.Find('{') != -1)
				{
					str = str.Left(str.Find('{'));
				}
				
				while(1)
				{
					if(str.Find('&') == -1)  break;
					CString tmp = str.Left(str.Find('&'));
					
			        submenu.AppendMenu(MF_STRING,cmdid,tmp);
					cmdid++;
					
					str = str.Right(str.GetLength() - str.Find('&') - 1);
				}
			}

			menu.AppendMenu(MF_POPUP,(UINT)submenu.m_hMenu,flist.GetAt(i));
		}
		flist.RemoveAll();
	}
	else
	{
		CString str = m_info;
		while(1)
		{
			if(str.Find('&') == -1)  break;
			CString tmp = str.Left(str.Find('&'));
	        menu.AppendMenu(MF_STRING,cmdid,tmp);
			cmdid++;
			str = str.Right(str.GetLength() - str.Find('&') - 1);
		}
	}
		
	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

    DWORD SelectionMade = menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD| TPM_VERTICAL, //TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
        rect.right, rect.top, pWndPopupOwner, &rect );

	if( SelectionMade )
	{
		CString str;
		menu.GetMenuString(SelectionMade,str,MF_BYCOMMAND);

		if(theApp.m_EditingMode.CompareNoCase("²åÈë") == 0)
		{
			CString OriginStr;
			GetWindowText(OriginStr);
			CString tp = OriginStr;
			tp.TrimLeft(); tp.TrimRight();
			if(!tp.IsEmpty())
				SetWindowText(OriginStr + "¡¢" + str);
			else
				SetWindowText(OriginStr + str);
		}
		else
		{
			SetWindowText(str);
		}
	}
	
    // Return the button state to normal.
    if (m_pBrowseBtn->GetSafeHwnd())
        m_pBrowseBtn->SetState(FALSE);

	mute = false;
}

void CMenuEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CXTBrowseEdit::OnSetFocus(pOldWnd);
	// TODO: Add your message handler code here
/*
   if (m_pBrowseBtn->GetSafeHwnd())
	{
		m_pBrowseBtn->ShowWindow(SW_SHOW);
		SetFocus();
	}
*/
}

void CMenuEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CXTBrowseEdit::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here
/*
    if (m_pBrowseBtn->GetSafeHwnd())
	{
		if(m_pBrowseBtn == GetFocus())
		{
			m_pBrowseBtn->ShowWindow(SW_SHOW);
			m_pBrowseBtn->SetFocus();
		}
		else
		{
			m_pBrowseBtn->ShowWindow(SW_HIDE);
		}
	}
*/
	if(m_info.IsEmpty())   
	{
		CString str;
		GetWindowText(str);
		str.TrimLeft();  str.TrimRight();
		if(str.IsEmpty())   return;

		//------------------------------
		if(!m_TableName.IsEmpty())
		{
			SAString indexname;
			indexname.Empty();

			theApp.ConnectDatabase();

			try
			{
				SAString cmdstr = "Select name from " + m_TableName + " where id = :id";
				
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("id").setAsString() = str;
				
				g_dbcommand.Execute();
				
				if( g_dbcommand.FetchNext() )
				{
					indexname = g_dbcommand.Field("name").asString();
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
			}

			if(!indexname.IsEmpty())
			{
				SetWindowText(indexname);
				return;
			}
		}
		//------------------------------

		return;
	}
	char ch = m_info.GetAt(0);
	if(ch != '{')
	{
		CString str;
		GetWindowText(str);
		str.TrimLeft();  str.TrimRight();
		if(str.IsEmpty())   return;
	
		//------------------------------
		if(!m_TableName.IsEmpty())
		{
			SAString indexname;
			indexname.Empty();

			theApp.ConnectDatabase();

			try
			{
				SAString cmdstr = "Select name from " + m_TableName + " where id = :id";
				
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("id").setAsString() = str;
				
				g_dbcommand.Execute();
				
				if( g_dbcommand.FetchNext() )
				{
					indexname = g_dbcommand.Field("name").asString();
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
			}

			if(!indexname.IsEmpty())
			{
				SetWindowText(indexname);
				return;
			}
		}
		//------------------------------

		for(int i = 0; i < str.GetLength(); i++ )
		{
			BYTE bch = str.GetAt(i);
			if( bch < 0x30 || bch > 0x39 )   break;
		}
		if(i < str.GetLength())  return;

		int index = atoi(str);

		CString result;
		result.Empty();

		str = m_info;
		i = 0;
		while(i < index)
		{
			if(str.Find('&') == -1)  break;
			CString tmp = str.Left(str.Find('&'));
			i++;
			if(i == index)
			{
				result = tmp;
				break;
			}
			str = str.Right(str.GetLength() - str.Find('&') - 1);
		}

		if(!result.IsEmpty())
			SetWindowText(result);
	}
}

void CMenuEdit::OnSize(UINT nType, int cx, int cy) 
{
	CXTBrowseEdit::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    if(this->GetSafeHwnd())	
	{
		if(this == GetFocus() || m_pBrowseBtn == GetFocus())
		{
			m_pBrowseBtn->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pBrowseBtn->ShowWindow(SW_HIDE);
		}
	}
}

bool CMenuEdit::did(BOOL value)
{
	if(value)
	{
		m_pBrowseBtn->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pBrowseBtn->ShowWindow(SW_HIDE);
	}
	return true;
}


void CMenuEdit::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
    if (m_pBrowseBtn && ::IsWindow(m_pBrowseBtn->m_hWnd))
	{
        //lpwndpos->cx -= CX_BUTTON + m_nGap;
	}

	CXTEdit::OnWindowPosChanging(lpwndpos);
}

void CMenuEdit::PositionBrowseButton(bool bSizeEdit/*=false*/)
{
    // if the browse button exists, move it to the correct
    // location in relation to the browse edit window.
    if (m_pBrowseBtn && ::IsWindow(m_pBrowseBtn->m_hWnd))
    {
        CWnd* pParentWnd = GetParent();
        ASSERT_VALID(pParentWnd);

        CRect r;
        GetWindowRect(&r);
        pParentWnd->ScreenToClient(&r);

        if (bSizeEdit)
        {
            // call MoveWindow, this causes WM_WINDOWPOSCHANGING to
            // get called which will adjust the size of the browse edit window.
            // WM_WINDOWPOSCHANGED is then called which moves the browse button.

			::SetWindowPos( m_hWnd, NULL, r.left, r.top,
				r.Width(), r.Height(), SWP_FRAMECHANGED|SWP_NOREDRAW );
        }
        else
        {
            // move the browse button.
            //r.right += CX_BUTTON + m_nGap;
			r.right -= m_nGap;
            r.left   = r.right - CX_BUTTON;
			r.top += m_nGap;
			r.bottom -= m_nGap;
            
			// set the window position and make sure tab order is correct.
			::SetWindowPos( m_pBrowseBtn->m_hWnd, m_hWnd, r.left, r.top,
				r.Width(), r.Height(), SWP_FRAMECHANGED|SWP_NOREDRAW );
        }

		// Refresh the parent window.
		pParentWnd->RedrawWindow( NULL, NULL,
			RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN );
    }
}

void CMenuEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint pt;
	GetCursorPos(&pt);

	CRect rt;
	GetWindowRect(&rt);
	rt.right -= (CX_BUTTON + m_nGap);

/*
	if(rt.PtInRect(pt))
	{
		m_pBrowseBtn->ShowWindow(SW_SHOW);
		BringWindowToTop();
	}
	else
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		
		m_pBrowseBtn->BringWindowToTop();
		RedrawWindow();
		m_pBrowseBtn->RedrawWindow();
	}
*/
	if(!rt.PtInRect(pt))
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}

	m_pBrowseBtn->ShowWindow(SW_SHOW);
	m_pBrowseBtn->BringWindowToTop();
	RedrawWindow();
	m_pBrowseBtn->RedrawWindow();
	
	CXTBrowseEdit::OnMouseMove(nFlags, point);
}

void CMenuEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint pt;
	GetCursorPos(&pt);

	CRect rt;
	GetWindowRect(&rt);
	rt.right -= (CX_BUTTON + m_nGap);

	if(rt.PtInRect(pt))
	{
		RedrawWindow();
	}
	
	CXTBrowseEdit::OnLButtonDown(nFlags, point);
}

void CMenuEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	RedrawWindow();
	CXTBrowseEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

