// SelfInput.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "PathologyView.h"
#include "MainFrm.h"
#include "SelfInput.h"
#include "MedPerson.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelfInput dialog
extern CPathologyApp theApp;

CSelfInput::CSelfInput(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CSelfInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelfInput)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_FieldName.RemoveAll();
    m_NameList.RemoveAll();
	m_FieldValue.RemoveAll();
	m_FieldList.RemoveAll();
	m_TypeList.RemoveAll();
	
	m_IsFirstTimePaint = true;
	m_InputBlh.Empty();
	m_Operation_Blh.Empty();
	m_ExitCode = 0;
}


void CSelfInput::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelfInput)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelfInput, CXTResizeDialog)
	//{{AFX_MSG_MAP(CSelfInput)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDNEW, OnNew)
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_PREVIOUS, OnPrevious)
	ON_BN_CLICKED(ID_NEXT, OnNext)
	ON_BN_CLICKED(ID_CAPTURE, OnCapture)
	ON_BN_CLICKED(ID_PRINT, OnPrint)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelfInput message handlers

BOOL CSelfInput::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	//-------------------------------------------------
	m_FieldName.RemoveAll();
    m_NameList.RemoveAll();
	m_FieldValue.RemoveAll();
	m_FieldList.RemoveAll();
	m_TypeList.RemoveAll();
	
	CStdioFile File;
	if ( File.Open(theApp.m_Exe_Working_Dir + "system\\selffield.txt", CFile::modeRead | CFile::typeText) )
	{
		CString tmp;
		while(1)
		{
			if( File.ReadString(tmp) == FALSE ) break;
			tmp.TrimLeft(); tmp.TrimRight();
			if(!tmp.IsEmpty())
			{
				for(int i = 0; i < theApp.m_fdescriptionlist.GetSize(); i++)
				{
					if(theApp.m_fdescriptionlist.GetAt(i).CompareNoCase(tmp) == 0)
					{
						m_NameList.Add(theApp.m_fnamelist.GetAt(i));
						m_TypeList.Add(theApp.m_ftypeList.GetAt(i));
						m_FieldName.Add(theApp.m_fdescriptionlist.GetAt(i));
						m_FieldValue.Add("");
						m_FieldList.Add(theApp.m_finfolist.GetAt(i));

						break;
					}
				}
			}
		}
		File.Close();
	}

	//-------------------------------------------------
	if(m_NameList.GetSize() < 1)
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if(m_NameList.GetAt(0).CompareNoCase("medicalno") != 0)
	{
		m_NameList.InsertAt(0, "medicalno");
		m_TypeList.InsertAt(0, SA_dtString);
		m_FieldName.InsertAt(0, "病理号");
		m_FieldValue.InsertAt(0, "");
		m_FieldList.InsertAt(0, "");

		for(int i = 1; i < m_NameList.GetSize(); i++)
		{
			if(m_NameList.GetAt(i).CompareNoCase("medicalno") == 0)
			{
				m_NameList.RemoveAt(i);
				m_TypeList.RemoveAt(i);
				m_FieldName.RemoveAt(i);
				m_FieldValue.RemoveAt(i);
				m_FieldList.RemoveAt(i);
			}
		}
	}

	if(!m_InputBlh.IsEmpty())
	{
		m_FieldValue.SetAt(0, m_InputBlh);
		for(int i = 1; i < m_NameList.GetSize(); i++)
		{
			CString value;  value.Empty();
			bool rt = theApp.GetDatabaseValue(m_InputBlh,m_NameList.GetAt(i),value,true);
			if( rt )
			{
				m_FieldValue.SetAt(i, value);
			}
		}

		m_Operation_Blh = m_InputBlh;
	}
	//-------------------------------------------------

	m_PromptInfo_Ctrl_First.m_Type  = 1;
	m_PromptInfo_Ctrl_Second.m_Type = 2;
	
	CRect rect,rt;
	GetClientRect(&rect);
	rect.bottom -= 30;
	rt = rect;
	rt.right = rect.left + rect.Width() / 3 - 5;
    if(!m_RecordInfo_Ctrl.CreateGrid(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rt,this,111))
	{
		TRACE0( "Failed to create m_RecordInfo_Ctrl.\n" );
		return FALSE;
	}

	rt = rect;
	rt.left  = rect.left + rect.Width() / 3 + 5;
	rt.right = rt.left + (rect.Width() - rt.left) * 3 / 5 - 5;

    if(!m_PromptInfo_Ctrl_First.CreateGrid(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rt,this,112))
	{
		TRACE0( "Failed to create m_PromptInfo_Ctrl_First.\n" );
		return FALSE;
	}

	rt.left  = rt.right + 5;
	rt.right = rt.left + (rect.Width() - rt.left);

    if(!m_PromptInfo_Ctrl_Second.CreateGrid(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rt,this,113))
	{
		TRACE0( "Failed to create m_PromptInfo_Ctrl_Second.\n" );
		return FALSE;
	}

    SetResize(IDNEW,     SZ_BOTTOM_CENTER,  SZ_BOTTOM_CENTER);
    SetResize(IDOK,      SZ_BOTTOM_CENTER,  SZ_BOTTOM_CENTER);
    SetResize(IDCANCEL,  SZ_BOTTOM_CENTER,  SZ_BOTTOM_CENTER);
    SetResize(ID_PREVIOUS, SZ_BOTTOM_CENTER,  SZ_BOTTOM_CENTER);
    SetResize(ID_NEXT,     SZ_BOTTOM_CENTER,  SZ_BOTTOM_CENTER);
    SetResize(ID_PRINT,    SZ_BOTTOM_CENTER,  SZ_BOTTOM_CENTER);
    SetResize(ID_CAPTURE,  SZ_BOTTOM_CENTER,  SZ_BOTTOM_CENTER);

	CRect ParentRect;
	theApp.GetView()->GetWindowRect(&ParentRect);
	SetWindowPos(NULL, ParentRect.left, ParentRect.top ,ParentRect.Width(),ParentRect.Height(),SWP_NOZORDER);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelfInput::OnSize(UINT nType, int cx, int cy) 
{
	CXTResizeDialog::OnSize(nType, cx, cy);
	
	CRect rect,rt;
	rect.left = 0;  rect.right = cx;  rect.top = 0;  rect.bottom = cy;
	rect.bottom -= 30;
	rt = rect;
	rt.right = rect.left + rect.Width() / 3 - 5;
	::MoveWindow(m_RecordInfo_Ctrl.m_hWnd,rt.left,rt.top,rt.Width(),rt.Height(),TRUE);
	m_RecordInfo_Ctrl.SetColWidth(0,rt.Width() - 100);

	rt = rect;
	rt.left  = rect.left + rect.Width() / 3 + 5;
	rt.right = rt.left + (rect.Width() - rt.left) * 3 / 5 - 5;
	::MoveWindow(m_PromptInfo_Ctrl_First.m_hWnd,rt.left,rt.top,rt.Width(),rt.Height(),TRUE);
	m_PromptInfo_Ctrl_First.SetColWidth(0,rt.Width());// - 20);

	rt.left  = rt.right + 5;
	rt.right = rt.left + (rect.Width() - rt.left);
	::MoveWindow(m_PromptInfo_Ctrl_Second.m_hWnd,rt.left,rt.top,rt.Width(),rt.Height(),TRUE);
	m_PromptInfo_Ctrl_Second.SetColWidth(0,rt.Width());// - 20);
}

void CSelfInput::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if( m_IsFirstTimePaint && m_RecordInfo_Ctrl.GetSafeHwnd())
	{
		m_IsFirstTimePaint = false;

		m_RecordInfo_Ctrl.SetFocus();

		if(m_RecordInfo_Ctrl.GetCurrentRow() >= 0)
		{
			m_RecordInfo_Ctrl.GotoRow(1);
			m_RecordInfo_Ctrl.OnRowChange(m_RecordInfo_Ctrl.GetCurrentRow(),m_RecordInfo_Ctrl.GetCurrentRow());
		}
	}
	
	// Do not call CXTResizeDialog::OnPaint() for painting messages
}

BOOL CSelfInput::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->wParam == VK_ESCAPE)
    {
		// don't allow dialog to process return or escape keys.
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);

        return TRUE;
    }

	if ( pMsg->wParam == VK_TAB || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN)
	{
		m_RecordInfo_Ctrl.SetFocus();

		if(m_RecordInfo_Ctrl.GetCurrentRow() >= 0)
		{
			m_RecordInfo_Ctrl.OnRowChange(m_RecordInfo_Ctrl.GetCurrentRow(),m_RecordInfo_Ctrl.GetCurrentRow());
		}
	}

	if( pMsg->message == WM_KEYDOWN){
		UINT nChar = pMsg->wParam;
		if(nChar == VK_F2){
			GetDlgItem(IDNEW)->SetFocus();
			OnNew();
		}
		
		if( nChar == VK_F3){
			GetDlgItem(IDOK)->SetFocus();
			OnOK();
			m_RecordInfo_Ctrl.RedrawAll();
		}
		
		if( nChar == VK_F5){
			GetDlgItem(ID_PREVIOUS)->SetFocus();
			OnPrevious();
			m_RecordInfo_Ctrl.RedrawAll();
		}
		
		if( nChar == VK_F6){
			//PostMessage(WM_COMMAND,ID_NEXT);
			GetDlgItem(ID_NEXT)->SetFocus();
			OnNext();
			m_RecordInfo_Ctrl.RedrawAll();
		} 

	}

	
	return CXTResizeDialog::PreTranslateMessage(pMsg);
}

void CSelfInput::OnNew() 
{
	SaveRecord(true);
	
	CString MedicalNo = CMedPerson::GenNewBlh();
	m_FieldValue.SetAt(0, MedicalNo);
	m_RecordInfo_Ctrl.QuickSetText(0, 0, MedicalNo);
	for(int i = 1; i < m_NameList.GetSize(); i++)
	{
		m_FieldValue.SetAt(i, "");
		m_RecordInfo_Ctrl.QuickSetText(0, i, "");
	}

	int num = 0;
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = MedicalNo;
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

	if(num > 0)
	{
		m_FieldValue.SetAt(0, "");
		m_RecordInfo_Ctrl.QuickSetText(0, 0, "");
		AfxMessageBox("系统自动产生的病理号存在重复记录,请手工输入病理号!");

/*
		for(i = 1; i < m_NameList.GetSize(); i++)
		{
			CString value;  value.Empty();
			bool rt = theApp.GetDatabaseValue(MedicalNo,m_NameList.GetAt(i),value,true);
			if( rt )
			{
				m_FieldValue.SetAt(i, value);
				m_RecordInfo_Ctrl.QuickSetText(0, i, value);
			}
		}
*/
	}

	//--------------------------------------------
	COleDateTime curt = theApp.GetOleServerDateTime();

	CString nSjdw;   nSjdw.Empty();
	
	SADateTime nSjrq, nQcrq, nBgrq;
	nSjrq = nQcrq = nBgrq = theApp.GetServerDateTime();
	try
	{
		SAString cmdstr = "Select TOP 1 sjrq, qcrq, bgrq, sjdw from " + theApp.TABLE_MAIN + " where dbtype = :dbtype and dlrq IS NOT NULL order by dlrq desc";
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			cmdstr = "Select * from (Select sjrq, qcrq, bgrq, sjdw from " + theApp.TABLE_MAIN + " where dbtype = :dbtype and dlrq IS NOT NULL order by dlrq desc) where ROWNUM<=1";
		}
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
		
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			if(!g_dbcommand.Field("sjrq").isNull())
			{
				nSjrq = g_dbcommand.Field("sjrq").asDateTime();
			}
			if(!g_dbcommand.Field("qcrq").isNull())
			{
				nQcrq = g_dbcommand.Field("qcrq").asDateTime();
			}
			if(!g_dbcommand.Field("bgrq").isNull())
			{
				nBgrq = g_dbcommand.Field("bgrq").asDateTime();
			}
			if(!g_dbcommand.Field("sjdw").isNull())
			{
				nSjdw = g_dbcommand.Field("sjdw").asString();
			}
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
	
	// 新记录设置系统默认值
	for(i = 1; i < m_NameList.GetSize(); i++)
	{
		if(m_NameList.GetAt(i).CompareNoCase("xb") == 0)
		{
			m_FieldValue.SetAt(i, theApp.m_Default_Sex);
			m_RecordInfo_Ctrl.QuickSetText(0, i, theApp.m_Default_Sex);
		}
		if(m_NameList.GetAt(i).CompareNoCase("nldw") == 0)
		{
			m_FieldValue.SetAt(i, "岁");
			m_RecordInfo_Ctrl.QuickSetText(0, i, "岁");
		}
		if(m_NameList.GetAt(i).CompareNoCase("sjrq") == 0)
		{
			CString tmp;
			if(theApp.m_SjrqTimeMode.CompareNoCase("服务器当前时间") == 0)
			{
				tmp = curt.Format("%Y-%m-%d %H:%M:%S");
			}
			else
			{
				COleDateTime ct;
				ct.SetDateTime(nSjrq.GetYear(), nSjrq.GetMonth(), nSjrq.GetDay(), nSjrq.GetHour(), nSjrq.GetMinute(), nSjrq.GetSecond());
				tmp = ct.Format("%Y-%m-%d %H:%M:%S");
			}
			
			m_FieldValue.SetAt(i, tmp);
			m_RecordInfo_Ctrl.QuickSetText(0, i, tmp);
		}
		
		if(m_NameList.GetAt(i).CompareNoCase("qcrq") == 0)
		{
			CString tmp;
			if(theApp.m_QcrqTimeMode.CompareNoCase("服务器当前时间") == 0)
			{
				tmp = curt.Format("%Y-%m-%d %H:%M:%S");
			}
			else
			{
				COleDateTime ct;
				ct.SetDateTime(nQcrq.GetYear(), nQcrq.GetMonth(), nQcrq.GetDay(), nQcrq.GetHour(), nQcrq.GetMinute(), nQcrq.GetSecond());
				tmp = ct.Format("%Y-%m-%d %H:%M:%S");
			}
			
			m_FieldValue.SetAt(i, tmp);
			m_RecordInfo_Ctrl.QuickSetText(0, i, tmp);
		}
		
		if(m_NameList.GetAt(i).CompareNoCase("bgrq") == 0)
		{
			CString tmp;
			if(theApp.m_BgrqTimeMode.CompareNoCase("服务器当前时间") == 0)
			{
				tmp = curt.Format("%Y-%m-%d %H:%M:%S");
			}
			else
			{
				COleDateTime ct;
				ct.SetDateTime(nBgrq.GetYear(), nBgrq.GetMonth(), nBgrq.GetDay(), nBgrq.GetHour(), nBgrq.GetMinute(), nBgrq.GetSecond());
				tmp = ct.Format("%Y-%m-%d %H:%M:%S");
			}
			
			m_FieldValue.SetAt(i, tmp);
			m_RecordInfo_Ctrl.QuickSetText(0, i, tmp);
		}

		if(m_NameList.GetAt(i).CompareNoCase("sjdw") == 0)
		{
			CString tmp;  tmp.Empty();
			if(theApp.m_SjdwMode.CompareNoCase("是") == 0)
			{
				tmp = nSjdw;
			}
			m_FieldValue.SetAt(i, tmp);
			m_RecordInfo_Ctrl.QuickSetText(0, i, tmp);
		}
	}

	m_RecordInfo_Ctrl.RedrawAll();

	m_IsFirstTimePaint = true;
	RedrawWindow();
}

void CSelfInput::OnOK() 
{
	SaveRecord(false);

	//theApp.GetView()->GetDocument()->m_blh = ;
//	CString blh;
//	m_RecordInfo_Ctrl.QuickGetText(0 , 0 , &blh);
//	theApp.OpenDocumentFile(blh);
	CXTResizeDialog::OnOK();
//	theApp.OnAppCamera();
	
	
}

void CSelfInput::OnDestroy() 
{
	CXTResizeDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_FieldName.RemoveAll();
    m_NameList.RemoveAll();
	m_FieldValue.RemoveAll();
	m_FieldList.RemoveAll();
	m_TypeList.RemoveAll();
}

void CSelfInput::SaveRecord(bool IsInsertOnly)
{
	CString m_blh;
	m_RecordInfo_Ctrl.QuickGetText(0 , 0 , &m_blh);
	m_blh.TrimLeft();  m_blh.TrimRight();
	
	
	if(m_blh.IsEmpty())  return;
	CStringArray ids;
	if(m_blh.Find('~')!=-1){
		CString from = m_blh.Left(m_blh.Find('~'));
		CString to = m_blh.Right(m_blh.GetLength() - m_blh.Find('~') - 1);
		CString head_from = "";CString head_to = "";  int count; int size_from; int size_to; int size;
		int _from = 0; int _to = 0;
		while(from.Find("-") != -1){
		
			count = from.Find("-");
		//	index = index + count + 1;
			head_from = head_from + from.Left(count) + "-";
			from = from.Right(from.GetLength() - count - 1);
			head_to = head_to + to.Left(count) + "-";
			to = to.Right(to.GetLength() - count - 1);
		}
	
		TCHAR c;TCHAR c1;
		c = from.GetAt(0);
		c1 = to.GetAt(0);
		while( !(c >= 0x30 && c <= 0x39) ){
			from = from.Right(from.GetLength() - 1);
			to = to.Right(to.GetLength() - 1);
			head_from = head_from + c ;
			head_to = head_to + c1;
			c = from.GetAt(0);
			c1 = to.GetAt(0);
		}
		
		size_from = from.GetLength();
		size_to = to.GetLength();

		if(head_from.CompareNoCase(head_to) != 0 || from.Find("-") != -1 || to.Find("-") != -1){
			AfxMessageBox("'~'符号前后请保持格式一致!");
			return;
		}
		if(size_from < size_to){
			size = size_to;
		} else if(size_from = size_to){
			size = size_from;
		} else {
			size = size_from;
		}
		int i = atoi(from);
		
		int j = atoi(to);
		if(j - i >= 50){
			AfxMessageBox("系统允许最多录入50个连续编号,请重新输入!");
			return;
		}
		while(i <= j){
			CString str;
			str.Format("%d",i);
			while(str.GetLength() < size){
				str = "0" + str; 
			}
			while(str.GetLength() > size){
				str = str.Right(str.GetLength() - 1);
			}
			ids.Add(head_from + str);
			i++;
		}
	}else{
		ids.Add(m_blh);
	}
	for(int n = 0; n < ids.GetSize(); n++){
		m_blh = ids.GetAt(n);
	int num = 0;
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Param("medicalno").setAsString() = m_blh;
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

	m_Operation_Blh = m_blh;

	if(num < 1)
	{
		SADateTime ServerTime = theApp.GetServerDateTime();
		try
		{
			SAString cmdstr = "Insert into " + theApp.TABLE_MAIN + "(medicalno,dbtype,dlrq,bgzt) values(:medicalno,:dbtype,:dlrq,0)";
			
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("medicalno").setAsString() = m_blh;
			g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
			g_dbcommand.Param("dlrq").setAsDateTime() = ServerTime;
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
	}
	else
	{

/*
		if(IsInsertOnly || AfxMessageBox("该记录已经存在，是否保存当前编辑内容？",MB_OKCANCEL|MB_ICONQUESTION) == IDCANCEL)  
		{
			if(IsInsertOnly)
			{
				AfxMessageBox("病历记录已经存在,保存失败!");
			}

			return;
		}
*/
		if(IsInsertOnly)  
		{
			AfxMessageBox("病历记录已经存在,保存失败!");
			
			return;
		}
	}

	if(theApp.IsRecordLocked(m_blh))
	{
		AfxMessageBox("病例记录<"+m_blh+">已经被锁定，保存失败！");
		return;
	}

	for(int i = 0; i < m_NameList.GetSize(); i ++ )
	{
		CString str;
		m_RecordInfo_Ctrl.QuickGetText(0 , i , &str);
		str.TrimLeft();  str.TrimRight();
        m_FieldValue.SetAt(i, str);
	}

	for(int k = 0; k < m_NameList.GetSize(); k++ )
	{
		SADataType_t  DataType = (SADataType_t) m_TypeList.GetAt(k);
		
		if( m_NameList.GetAt(k).CompareNoCase("medicalno") == 0)
		{
			continue;
		}
		
		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set " + m_NameList.GetAt(k) + " = :" + m_NameList.GetAt(k) + " where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = m_blh;
			
			switch( DataType ) {
			case SA_dtShort:
				g_dbcommand.Param(( SAString )m_NameList.GetAt(k)).setAsShort() = atoi(m_FieldValue.GetAt(k));
				break;
			case SA_dtLong:
				g_dbcommand.Param(( SAString )m_NameList.GetAt(k)).setAsLong() = atol(m_FieldValue.GetAt(k));
				break;
			case SA_dtBool:
				if( m_FieldValue.GetAt(k).Compare("是") == 0 )
				{
					g_dbcommand.Param(( SAString )m_NameList.GetAt(k)).setAsBool() = true;
				}
				else
				{
					g_dbcommand.Param(( SAString )m_NameList.GetAt(k)).setAsBool() = false;
				}
				break;
			case SA_dtDouble:
				g_dbcommand.Param(( SAString )m_NameList.GetAt(k)).setAsDouble() = atof(m_FieldValue.GetAt(k));
				break;
			case SA_dtDateTime:
				{	
					CString str = m_FieldValue.GetAt(k);
					int year = atoi(str.Left(str.Find("-")));
					str = str.Right(str.GetLength() - str.Find("-") - 1);
					
					int month = atoi(str.Left(str.Find("-")));
					str = str.Right(str.GetLength() - str.Find("-") - 1);
					
					str.TrimLeft();
					str += " ";
					int day = atoi(str.Left(str.Find(" ")));
					str = str.Right(str.GetLength() - str.Find(" ") - 1);
					
					int hour = atoi(str.Left(str.Find(":")));
					str = str.Right(str.GetLength() - str.Find(":") - 1);
					
					int minute = atoi(str.Left(str.Find(":")));
					str = str.Right(str.GetLength() - str.Find(":") - 1);
					
					int second = atoi( str );
					
					SADateTime dtValue(year,month,day,hour,minute,second);
					g_dbcommand.Param(( SAString )m_NameList.GetAt(k)).setAsDateTime() = dtValue;
				}
				break;
			case SA_dtString:
				g_dbcommand.Param(( SAString )m_NameList.GetAt(k)).setAsString() = m_FieldValue.GetAt(k);
				break;
			case SA_dtLongChar:
				g_dbcommand.Param(( SAString )m_NameList.GetAt(k)).setAsLongChar() = m_FieldValue.GetAt(k);
				break;
			case SA_dtCLob:
				g_dbcommand.Param(( SAString )m_NameList.GetAt(k)).setAsCLob() = m_FieldValue.GetAt(k);
				break;
			default:;
			}
			
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
	}
}
	if(theApp.m_AutoRefreshDBListMode.CompareNoCase("是") == 0 && ((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.GetSafeHwnd())
	{
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.ResetGridSize();
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
		((CMainFrame *)AfxGetMainWnd())->m_wndTabDockBar.m_DBList.m_RecordList.RedrawAll();
	}
}

void CSelfInput::OnPrevious() 
{
	SaveRecord(false);

	CString m_blh;
	m_RecordInfo_Ctrl.QuickGetText(0 , 0 , &m_blh);
	m_blh.TrimLeft();  m_blh.TrimRight();

	CString curno;  curno.Empty();

	if(m_blh.IsEmpty())
	{
		try
		{
			SAString cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where dbtype = :dbtype and dlrq IS NOT NULL order by dlrq desc";
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				cmdstr = "Select * from (Select medicalno from " + theApp.TABLE_MAIN + " where dbtype = :dbtype and dlrq IS NOT NULL order by dlrq desc) where ROWNUM<=1";
			}
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
			
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				curno = g_dbcommand.Field("medicalno").asString();
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

		if(curno.IsEmpty())
		{
			try
			{
				SAString cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where dbtype = :dbtype order by medicalno desc";
				if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
				{
					cmdstr = "Select * from (Select medicalno from " + theApp.TABLE_MAIN + " where dbtype = :dbtype order by medicalno desc) where ROWNUM<=1";
				}
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
				
				g_dbcommand.Execute();
				
				if( g_dbcommand.FetchNext() )
				{
					curno = g_dbcommand.Field("medicalno").asString();
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
		}
	}
	else
	{
		curno = m_blh;
		curno.TrimLeft(); curno.TrimRight();
		CString str, str1;
		
		char strbuf[300];
		int Len=curno.GetLength(),k;
		BYTE ch;
		for(k=(Len-1);k>=0;k--)
		{
			ch = curno.GetAt(k); 
			if(!(ch >= 0x30 && ch <= 0x39))  break;
		}
		k = Len-1-k;
		ltoa(atol(curno.Right(k))-1,strbuf,10);
		str1.Format("%s",strbuf);
		str = curno.Left(Len-k);
		Len = k - str1.GetLength();
		for(k=0;k<Len;k++)  str += "0";
		str += str1;

		curno = str;

		int num = 0;
		try
		{
			SAString cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where medicalno = :medicalno and dbtype = :dbtype";
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("medicalno").setAsString() = curno;
			g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
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
		
		if(num <= 0)
		{
			curno.Empty();
		}
	}

	if(!curno.IsEmpty())
	{
		m_FieldValue.SetAt(0, curno);
		m_RecordInfo_Ctrl.QuickSetText(0, 0, curno);
		for(int i = 1; i < m_NameList.GetSize(); i++)
		{
			CString value;  value.Empty();
			bool rt = theApp.GetDatabaseValue(curno,m_NameList.GetAt(i),value,true);
			if( rt )
			{
				m_FieldValue.SetAt(i, value);
				m_RecordInfo_Ctrl.QuickSetText(0, i, value);
			}
		}
	}

	m_RecordInfo_Ctrl.RedrawAll();

	m_IsFirstTimePaint = true;
	RedrawWindow();
}

void CSelfInput::OnNext() 
{
	SaveRecord(false);

	CString m_blh;
	m_RecordInfo_Ctrl.QuickGetText(0 , 0 , &m_blh);
	m_blh.TrimLeft();  m_blh.TrimRight();

	CString curno;  curno.Empty();

	if(m_blh.IsEmpty())
	{
		try
		{
			SAString cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where dbtype = :dbtype and dlrq IS NOT NULL order by dlrq desc";
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				cmdstr = "Select * from (Select medicalno from " + theApp.TABLE_MAIN + " where dbtype = :dbtype and dlrq IS NOT NULL order by dlrq desc) where ROWNUM<=1";
			}
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
			
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				curno = g_dbcommand.Field("medicalno").asString();
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

		if(curno.IsEmpty())
		{
			try
			{
				SAString cmdstr = "Select TOP 1 medicalno from " + theApp.TABLE_MAIN + " where dbtype = :dbtype order by medicalno desc";
				if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
				{
					cmdstr = "Select * from (Select medicalno from " + theApp.TABLE_MAIN + " where dbtype = :dbtype order by medicalno desc) where ROWNUM<=1";
				}
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
				
				g_dbcommand.Execute();
				
				if( g_dbcommand.FetchNext() )
				{
					curno = g_dbcommand.Field("medicalno").asString();
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
		}
	}
	else
	{
		curno = m_blh;
		curno.TrimLeft(); curno.TrimRight();
		CString str, str1;

		char strbuf[300];
		int Len=curno.GetLength(),k;
		BYTE ch;
		for(k=(Len-1);k>=0;k--)
		{
			ch = curno.GetAt(k); 
			if(!(ch >= 0x30 && ch <= 0x39))  break;
		}
		k = Len-1-k;
		ltoa(atol(curno.Right(k))+1,strbuf,10);
		str1.Format("%s",strbuf);
		str = curno.Left(Len-k);
		Len = k - str1.GetLength();
		for(k=0;k<Len;k++)  str += "0";
		str += str1;

		curno = str;

		int num = 0;
		try
		{
			SAString cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where medicalno = :medicalno and dbtype = :dbtype";
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("medicalno").setAsString() = curno;
			g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
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
		
		if(num <= 0)
		{
			curno.Empty();
		}
	}

	if(!curno.IsEmpty())
	{
		m_FieldValue.SetAt(0, curno);
		m_RecordInfo_Ctrl.QuickSetText(0, 0, curno);
		for(int i = 1; i < m_NameList.GetSize(); i++)
		{
			CString value;  value.Empty();
			bool rt = theApp.GetDatabaseValue(curno,m_NameList.GetAt(i),value,true);
			if( rt )
			{
				m_FieldValue.SetAt(i, value);
				m_RecordInfo_Ctrl.QuickSetText(0, i, value);
			}
		}
	}
	m_RecordInfo_Ctrl.RedrawAll();
	m_RecordInfo_Ctrl.RedrawAll();
	
	m_IsFirstTimePaint = true;
	RedrawWindow();
}

void CSelfInput::OnCapture() 
{
	CString m_blh;
	m_RecordInfo_Ctrl.QuickGetText(0 , 0 , &m_blh);
	m_blh.TrimLeft();  m_blh.TrimRight();
	if(m_blh.IsEmpty())  return;
	
	m_ExitCode = 1;
	OnOK();
}

void CSelfInput::OnPrint() 
{
	CString m_blh;
	m_RecordInfo_Ctrl.QuickGetText(0 , 0 , &m_blh);
	m_blh.TrimLeft();  m_blh.TrimRight();
	if(m_blh.IsEmpty())  return;
	
	m_ExitCode = 2;
	OnOK();
}


