// BbmxDetail.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "PathologyDoc.h"
#include "PathologyView.h"
#include "BbmxDetail.h"
#include "MedPerson.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBbmxDetail dialog
extern CPathologyApp theApp;

CBbmxDetail::CBbmxDetail(CWnd* pParent /*=NULL*/)
	: CXTCBarDialog(CBbmxDetail::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBbmxDetail)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDB_ICON_CHARGEDETAIL);
	m_pParent = (CMedPerson *)pParent; 
}


void CBbmxDetail::DoDataExchange(CDataExchange* pDX)
{
	CXTCBarDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBbmxDetail)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBbmxDetail, CXTCBarDialog)
	//{{AFX_MSG_MAP(CBbmxDetail)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDCHECK, OnCheck)
	ON_BN_CLICKED(IDREJECT, OnReject)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBbmxDetail message handlers

BOOL CBbmxDetail::OnInitDialog() 
{
	CXTCBarDialog::OnInitDialog();

	m_Sqh.TrimLeft();  m_Sqh.TrimRight();
	if(m_Sqh.IsEmpty())
	{
		AfxMessageBox("申请号为空!");
		EndDialog(IDCANCEL);
		return FALSE;
	}

	CString nIp, nDATABASENAME,nDATABASE_USERNAME,nDATABASE_PASSWORD;
	nIp.Empty(); nDATABASENAME.Empty(); nDATABASE_USERNAME.Empty(); nDATABASE_PASSWORD.Empty();
	
	CStdioFile File;
	if ( File.Open(theApp.m_Exe_Working_Dir + "system\\hisconnect.cfg", CFile::modeRead | CFile::typeText) )
	{
		CString tmp;
		int index = 0;
		while(1)
		{
			if( File.ReadString(tmp) == FALSE ) break;
			tmp.TrimLeft();
			tmp.TrimRight();
			if( tmp.IsEmpty() )   continue;
			switch( index ) {
			case 0:
				nIp = tmp;
				break;
			case 1:
				nDATABASENAME = tmp;
				break;
			case 2:
				nDATABASE_USERNAME = tmp;
				break;
			case 3:
				nDATABASE_PASSWORD = tmp;
				break;
			}
			index++;
		}
		File.Close();
	}
	else
	{
		AfxMessageBox("没有找到HIS联结配置文件!");
		EndDialog(IDCANCEL);
		return FALSE;
	}

	try
	{
		hiscon.Connect( (SAString)nDATABASENAME, (SAString)nDATABASE_USERNAME, (SAString)nDATABASE_PASSWORD, SA_Oracle_Client );
		hiscmd.setConnection( &hiscon );
	}
	catch(SAException &x)
	{
		try
		{
			hiscon.Rollback();
		}
		catch(SAException &)
		{
		}
		AfxMessageBox((const char*)x.ErrText());
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CRect rect;
	GetClientRect(&rect);
	CRect rt = rect;
	rt.bottom -= 30;
    if(!m_DetailCtrl.CreateGrid(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rt,this,118))
	{
		TRACE0( "Failed to create m_DetailCtrl.\n" );
		return FALSE;
	}

    SetResize(IDOK,     SZ_BOTTOM_CENTER,  SZ_BOTTOM_CENTER);
    SetResize(IDCANCEL, SZ_BOTTOM_CENTER,  SZ_BOTTOM_CENTER);
    SetResize(IDCHECK,  SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
    SetResize(IDREJECT, SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
    SetResize(IDC_EDIT_BBMC,  SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);

	// Move control bars to the correct position.
    RecalcBarLayout();

	((CEdit *)GetDlgItem(IDC_EDIT_BBMC))->SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HCURSOR CBbmxDetail::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBbmxDetail::OnSize(UINT nType, int cx, int cy) 
{
	CXTCBarDialog::OnSize(nType, cx, cy);
	
	::MoveWindow(m_DetailCtrl.m_hWnd,0,0,cx,cy-30,TRUE);
}

void CBbmxDetail::OnDestroy() 
{
	CXTCBarDialog::OnDestroy();
	
	if(hiscon.isConnected())
		hiscon.Disconnect();
}

void CBbmxDetail::OnCheck() 
{
	int row = m_DetailCtrl.GetCurrentRow();
	int CurRecordNum = m_DetailCtrl.GetNumberRows();
	
	if( CurRecordNum > 0 && row >= 0 && row < CurRecordNum )
	{
		CString str = m_DetailCtrl.QuickGetText(5, row);
		str.TrimLeft(); str.TrimRight();
		if(str.IsEmpty())
		{
			m_DetailCtrl.QuickSetText(5, row, theApp.m_Cpp_User);
			SADateTime st = theApp.GetServerDateTime();
			str.Format("%d年%d月%d日 %d:%d", st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute());
			m_DetailCtrl.QuickSetText(6, row, str);

			int xh = atoi(m_DetailCtrl.QuickGetText(0, row));

			try
			{
				hiscmd.setCommandText("Update EXAM.PATH_TEST_SPECIMEN set SPECIMEN_ACCEPTOR=:SPECIMEN_ACCEPTOR,SPECIMEN_ACCEPT_DATE=:SPECIMEN_ACCEPT_DATE where TEST_NO=:TEST_NO and SPECIMEN_NO=:SPECIMEN_NO");
				hiscmd.Param("TEST_NO").setAsString()       = m_Sqh;
				hiscmd.Param("SPECIMEN_NO").setAsShort()    = xh;
				hiscmd.Param("SPECIMEN_ACCEPTOR").setAsString() = theApp.m_Cpp_User;
				hiscmd.Param("SPECIMEN_ACCEPT_DATE").setAsDateTime() = st;
				
				m_pParent->SetDlgItemText(IDC_EDIT_BBQS, theApp.m_Cpp_User); 
				COleDateTime olest;
				olest.SetDateTime(st.GetYear(), st.GetMonth(),st.GetDay(),st.GetHour(),st.GetMinute(),st.GetSecond());
				m_pParent->m_Time_Qcrq.SetTime(olest);
				m_pParent->SetModifiedFlag(true);

				hiscmd.Execute();
				hiscon.Commit();
			}
			catch(SAException &x)
			{
				try
				{
					hiscon.Rollback();
				}
				catch(SAException &)
				{
				}
				AfxMessageBox((const char*)x.ErrText());
			}

			m_DetailCtrl.RedrawAll();
		}
		else
		{
			AfxMessageBox("记录已核对！");
		}
	}

	((CEdit *)GetDlgItem(IDC_EDIT_BBMC))->SetFocus();
}

BOOL CBbmxDetail::PreTranslateMessage(MSG* pMsg) 
{
	if(m_hWnd)
	{
		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		{
			CString nContent;
			GetDlgItemText(IDC_EDIT_BBMC, nContent);
			nContent.TrimLeft(); nContent.TrimRight();
			if(!nContent.IsEmpty())
			{
				SADateTime st = theApp.GetServerDateTime();
				try
				{
					hiscmd.setCommandText("Update EXAM.PATH_TEST_SPECIMEN set SPECIMEN_ACCEPTOR=:SPECIMEN_ACCEPTOR,SPECIMEN_ACCEPT_DATE=:SPECIMEN_ACCEPT_DATE where SPECIMEN_ID=:SPECIMEN_ID");
					hiscmd.Param("SPECIMEN_ID").setAsString() = nContent;
					hiscmd.Param("SPECIMEN_ACCEPTOR").setAsString() = theApp.m_Cpp_User;
					hiscmd.Param("SPECIMEN_ACCEPT_DATE").setAsDateTime() = st;

					m_pParent->SetDlgItemText(IDC_EDIT_BBQS, theApp.m_Cpp_User); 
					COleDateTime olest;
					olest.SetDateTime(st.GetYear(), st.GetMonth(),st.GetDay(),st.GetHour(),st.GetMinute(),st.GetSecond());
					m_pParent->m_Time_Qcrq.SetTime(olest);
					m_pParent->SetModifiedFlag(true);
					
					hiscmd.Execute();
					hiscon.Commit();
				}
				catch(SAException &x)
				{
					try
					{
						hiscon.Rollback();
					}
					catch(SAException &)
					{
					}
					AfxMessageBox((const char*)x.ErrText());
				}

				for(int index = 0; index < m_DetailCtrl.GetNumberRows(); index++)
				{
					CString SPECIMEN_ID = m_DetailCtrl.QuickGetText(1, index);
					if(SPECIMEN_ID == nContent)
					{
						m_DetailCtrl.QuickSetText(5, index, theApp.m_Cpp_User);
						CString str;
						str.Format("%d年%d月%d日 %d:%d", st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute());
						m_DetailCtrl.QuickSetText(6, index, str);
						break;
					}
				}

				if(index >= m_DetailCtrl.GetNumberRows())
				{
					AfxMessageBox("标本核对失败！");
				}
				
				m_DetailCtrl.RedrawAll();
			}

			SetDlgItemText(IDC_EDIT_BBMC, "");
			((CEdit *)GetDlgItem(IDC_EDIT_BBMC))->SetFocus();
			return FALSE;
		}
	}
	
	return CXTCBarDialog::PreTranslateMessage(pMsg);
}

void CBbmxDetail::OnOK() 
{
	m_Bbmc.Empty();
	bool IsAllChecked = true;
	try
	{
		SAString cmdstr = "Select * from EXAM.PATH_TEST_SPECIMEN where TEST_NO = :TEST_NO order by SPECIMEN_NO asc";
		hiscmd.setCommandText(cmdstr);
		hiscmd.Param("TEST_NO").setAsString() = m_Sqh;
		
		hiscmd.Execute();
		
		while( hiscmd.FetchNext() )
		{
			CString str = hiscmd.Field("SPECIMEN_ACCEPTOR").asString();
			str.TrimLeft();  str.TrimRight();
			if(!str.IsEmpty())
			{
				if(!m_Bbmc.IsEmpty())  m_Bbmc += "\\"; // "、";
				m_Bbmc += hiscmd.Field("SPECIMEN_NAME").asString();
			}
			else
			{
				IsAllChecked = false;
			}
		}
		
		hiscon.Commit();
	}
	catch(SAException &x)
	{
		try
		{
			hiscon.Rollback();
		}
		catch(SAException &)
		{
		}
		AfxMessageBox((const char*)x.ErrText());
	}
	
	if(IsAllChecked)
	{
		if(AfxMessageBox("核对完成，是否同意接收？",MB_OKCANCEL|MB_ICONQUESTION) == IDCANCEL)
		{
			m_Bbmc.Empty();
		}
	}
	else
	{
		if(AfxMessageBox("当前核对标本与实际标本数不相符，是否拒收？",MB_OKCANCEL|MB_ICONQUESTION) == IDOK)
		{
			OnReject();
			return;
		}
	}

	CXTCBarDialog::OnOK();
}

void CBbmxDetail::OnReject() 
{
	CString value;  value.Empty();
	int nStatusWord = 0;
	bool rt = theApp.GetDatabaseValue(theApp.GetView()->GetDocument()->m_blh,"statusword",value,true);
	if(rt && !value.IsEmpty())
	{
		nStatusWord = atoi(value);
	}
	if(nStatusWord & 0x01)
	{
		AfxMessageBox("已接收成功的记录不能再做拒收操作!");
		return;
	}

	if(AfxMessageBox("是否退回当前病人标本？",MB_OKCANCEL|MB_ICONQUESTION) == IDCANCEL) return;

	for(int index = 0; index < m_DetailCtrl.GetNumberRows(); index++)
	{
		m_DetailCtrl.QuickSetText(5, index, " ");
		m_DetailCtrl.QuickSetText(6, index, " ");
		m_DetailCtrl.QuickSetText(7, index, "拒收");
		
		int xh = atoi(m_DetailCtrl.QuickGetText(0, index));
		
		try
		{
			hiscmd.setCommandText("Update EXAM.PATH_TEST_SPECIMEN set SPECIMEN_ACCEPTOR = NULL,SPECIMEN_ACCEPT_DATE = NULL where TEST_NO=:TEST_NO and SPECIMEN_NO=:SPECIMEN_NO");
			hiscmd.Param("TEST_NO").setAsString()       = m_Sqh;
			hiscmd.Param("SPECIMEN_NO").setAsShort()    = xh;
			
			hiscmd.Execute();
			hiscon.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				hiscon.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
		}
		
		m_DetailCtrl.RedrawAll();
	}
}
