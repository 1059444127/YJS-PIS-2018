// NormalSfqr.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "NormalSfqr.h"
#include "INI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNormalSfqr dialog
extern CPathologyApp theApp;

CNormalSfqr::CNormalSfqr(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CNormalSfqr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNormalSfqr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNormalSfqr::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNormalSfqr)
	DDX_Control(pDX, IDC_HYP_NOTE, m_HypLink_Note);
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNCREATE(CNormalSfqr, CXTResizeDialog)

BEGIN_MESSAGE_MAP(CNormalSfqr, CXTResizeDialog)
	//{{AFX_MSG_MAP(CNormalSfqr)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnButtonQuery)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULTQUERY, OnButtonDefaultquery)
	ON_BN_CLICKED(IDC_CHECK_SELECTALL, OnCheckSelectall)
	ON_BN_CLICKED(IDC_BUTTON_YZQR, OnButtonYzqr)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNormalSfqr message handlers

BOOL CNormalSfqr::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rt;
	GetDlgItem(IDC_DATE_QCRQ)->GetWindowRect(&rt);
	ScreenToClient(&rt);
	if (!m_Time_Qcrq.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT, //,DTS_LONGDATEFORMAT
		rt, this, 112))
	{
		return FALSE;
	}
	
	m_Time_Qcrq.SetTime(theApp.GetOleServerDateTime());
	
	m_WaxList.AttachGrid(this,IDC_STATIC_RECORDLIST);
	m_WaxList.SetParent(this);
	
	((CComboBox *)GetDlgItem(IDC_COMBO_SFZT))->SetCurSel(0);

	for(int i = 0; i < theApp.m_fnamelist.GetSize(); i++)
	{
		if(theApp.m_fnamelist.GetAt(i).CompareNoCase("qcys") == 0)
		{
			CString m_FieldDescription = theApp.m_finfolist.GetAt(i);
			
			char ch = m_FieldDescription.GetAt(0);
			if(ch != '{')
			{
				while(1)
				{
					if(m_FieldDescription.Find('&') == -1)  break;
					
					((CComboBox *)GetDlgItem(IDC_COMBO_QCYS))->AddString(m_FieldDescription.Left(m_FieldDescription.Find('&')));
					m_FieldDescription = m_FieldDescription.Right(m_FieldDescription.GetLength() - m_FieldDescription.Find('&') - 1);
				}
			}
			break;
		}
	}
	
	m_HypLink_Note.SetURL("");
	m_HypLink_Note.SetColors(RGB(0x00, 0x00, 0xFF), RGB(0x00, 0x00, 0xFF), RGB(0x00, 0x00, 0xFF));
	m_HypLink_Note.SetUnderline(true);
	m_HypLink_Note.EnableShellExecute(false);
	
	SetResize(IDC_STATIC_BLH, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_BLH, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_QCYS, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO_QCYS, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_QCRQ, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_DATE_QCRQ, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_SFZT, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO_SFZT, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_CHECK_SELECTALL, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_YZQR, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_HYP_NOTE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_QUERY, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_DEFAULTQUERY, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_RECORDLIST, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_KB, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO_DB, SZ_TOP_LEFT, SZ_TOP_LEFT);

	for(i = 0; i < theApp.m_DBName_list.GetSize(); i++)
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_DB))->AddString(theApp.m_DBName_list.GetAt(i));
	}
	CIniReader m_IniReader;
	m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");
	CString dbsel = m_IniReader.getKeyValue("常规收费确认数据库选择","Settings");
	int dbno = 0;
	if(!dbsel.IsEmpty())  dbno = atoi(dbsel);
	if(dbno >= 0 && dbno < theApp.m_DBName_list.GetSize())
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_DB))->SetCurSel(dbno);
	}

	m_WaxList.m_WhereBetween_Cmd.Format(" where (fxjg IS NULL or fxjg <> '已收费')");
	
	CString dbquery;
    dbquery.Format(" and medicalno in (select medicalno from cmipsmain where bgzt IS NOT NULL and bgzt > 0 and dbtype=%d)", theApp.m_DBNo_list.GetAt(((CComboBox *)GetDlgItem(IDC_COMBO_DB))->GetCurSel()));
	m_WaxList.m_WhereBetween_Cmd += dbquery;
	m_WaxList.ResetGridSize();
	m_WaxList.RedrawAll();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

extern void ExplainString(CString inputstr, CStringArray& StrList, CStringArray& UdList);

void CNormalSfqr::OnButtonQuery() 
{
	// Clear all selection
	m_WaxList.ClearSelections();

	CString mno; mno.Empty();
	GetDlgItemText(IDC_EDIT_BLH, mno);
	mno.TrimLeft(); mno.TrimRight();
	SetDlgItemText(IDC_EDIT_BLH, "");

	CString qcys; qcys.Empty();
	GetDlgItemText(IDC_COMBO_QCYS, qcys);
	qcys.TrimLeft(); qcys.TrimRight();
	SetDlgItemText(IDC_COMBO_QCYS, "");

	if(mno.IsEmpty())
	{
		//  取材日期查询
		COleDateTime m_time;
		m_Time_Qcrq.GetTime(m_time);

		CString start = m_time.Format("%Y-%m-%d 00:00:00");
		CString end   = m_time.Format("%Y-%m-%d 23:59:59");

		if(qcys.IsEmpty())
			m_WaxList.m_WhereBetween_Cmd.Format(" where qcrq >= '%s' and qcrq <= '%s'", start, end);
		else
			m_WaxList.m_WhereBetween_Cmd.Format(" where qcrq >= '%s' and qcrq <= '%s' and qcys = '%s'", start, end, qcys);

		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			if(qcys.IsEmpty())
				m_WaxList.m_WhereBetween_Cmd.Format(" where qcrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and qcrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", start, end);
			else
				m_WaxList.m_WhereBetween_Cmd.Format(" where qcrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and qcrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss') and qcys = '%s'", start, end, qcys);
		}
	}
	else
	{
		//  病理号查询
		CStringArray StrList, UdList;
		ExplainString(mno, StrList, UdList);
		if(StrList.GetSize() == 1 && UdList.GetSize() == 0)
		{
			if(qcys.IsEmpty())
				m_WaxList.m_WhereBetween_Cmd.Format(" where medicalno = '%s'", mno);
			else
				m_WaxList.m_WhereBetween_Cmd.Format(" where medicalno = '%s' and qcys = '%s'", mno, qcys);
		}
		else
		{
			CString mstr; mstr.Empty();
			for(int i = 0; i < StrList.GetSize(); i++)
			{
				if(!mstr.IsEmpty())  mstr += " or ";
				CString tmp;
				tmp.Format("medicalno = '%s'", StrList.GetAt(i));
				mstr += tmp;
			}

			for(i = 0; i < UdList.GetSize(); i += 2)
			{
				if(!mstr.IsEmpty())  mstr += " or ";
				CString tmp;
				tmp.Format("(medicalno >= '%s' and medicalno <= '%s')", UdList.GetAt(i), UdList.GetAt(i+1));
				mstr += tmp;
			}

			if(qcys.IsEmpty())
				m_WaxList.m_WhereBetween_Cmd.Format(" where (%s)", mstr);
			else
				m_WaxList.m_WhereBetween_Cmd.Format(" where (%s) and qcys = '%s'", mstr, qcys);
		}

		StrList.RemoveAll();
		UdList.RemoveAll();
	}

	switch(((CComboBox *)GetDlgItem(IDC_COMBO_SFZT))->GetCurSel()) {
	case 0:
		m_WaxList.m_WhereBetween_Cmd += " and (fxjg IS NULL or fxjg <> '已收费')";
		break;
	case 1:
		m_WaxList.m_WhereBetween_Cmd += " and fxjg = '已收费'";
		break;
	case 2:
		break;
	}

	CString dbquery;
    dbquery.Format(" and medicalno in (select medicalno from cmipsmain where dbtype=%d)", theApp.m_DBNo_list.GetAt(((CComboBox *)GetDlgItem(IDC_COMBO_DB))->GetCurSel()));
	m_WaxList.m_WhereBetween_Cmd += dbquery;

	m_WaxList.ResetGridSize();
	m_WaxList.RedrawAll();
}

void CNormalSfqr::OnButtonDefaultquery() 
{
	// Clear all selection
	m_WaxList.ClearSelections();

	m_WaxList.m_WhereBetween_Cmd.Format(" where (fxjg IS NULL or fxjg <> '已收费')");
	
	CString dbquery;
    dbquery.Format(" and medicalno in (select medicalno from cmipsmain where bgzt IS NOT NULL and bgzt > 0 and dbtype=%d)", theApp.m_DBNo_list.GetAt(((CComboBox *)GetDlgItem(IDC_COMBO_DB))->GetCurSel()));
	m_WaxList.m_WhereBetween_Cmd += dbquery;

	m_WaxList.ResetGridSize();
	m_WaxList.RedrawAll();
}

BOOL CNormalSfqr::PreTranslateMessage(MSG* pMsg) 
{
	if(m_hWnd)
	{
		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN && GetDlgItem(IDC_EDIT_BLH) == GetFocus())
		{
			OnButtonQuery();
			return TRUE;
		}
	}
	
	return CXTResizeDialog::PreTranslateMessage(pMsg);
}

void CNormalSfqr::OnCheckSelectall() 
{
	if(((CButton *)GetDlgItem(IDC_CHECK_SELECTALL))->GetState() & 0x0003)
	{
		// Select all
		int curnum = m_WaxList.GetCurrentRow();
		for(int i = 0; i < m_WaxList.GetNumberRows(); i++)
		{
			m_WaxList.GotoRow(i);
		}
		m_WaxList.GotoRow(curnum);

		m_WaxList.SelectRange(0, 0, m_WaxList.GetNumberCols()-1,m_WaxList.GetNumberRows()-1);
	}
	else
	{
		// Clear all selection
		m_WaxList.ClearSelections();
	}
}

void CNormalSfqr::OnButtonYzqr() 
{
	if(AfxMessageBox("选择进行收费确认，该过程不可逆，是否继续？",MB_OKCANCEL|MB_ICONQUESTION) == IDCANCEL)  return;

	CStringArray  MedicalNolist;
	CStringArray  XhNolist;
	CUIntArray  rowlist;
	int nCol, nRet;
	long nRow;

	nRet = m_WaxList.EnumFirstSelected( &nCol, &nRow );
	while( nRet == UG_SUCCESS )
	{
		if(nRow >= 0)
		{
			int rowcount = rowlist.GetSize();
			for(int i = 0; i < rowcount; i ++ )
			{
				if( nRow == rowlist.GetAt(i) )  break;
			}
			if( i >= rowcount )
			{
				CString str_MedicalNo;
				
				CUGCell  cell;
				m_WaxList.GetCellIndirect(0,nRow,&cell);
				cell.GetText(&str_MedicalNo);

				CString str_Yzh;
				m_WaxList.GetCellIndirect(2,nRow,&cell);
				cell.GetText(&str_Yzh);
				
				rowlist.Add(nRow);
				MedicalNolist.Add(str_MedicalNo);
				XhNolist.Add(str_Yzh);
			}
		}
		
		nRet = m_WaxList.EnumNextSelected( &nCol, &nRow );
	}

	int curnum = m_WaxList.GetCurrentRow();
	for(int i = 0; i < rowlist.GetSize(); i++)
	{
		m_WaxList.GotoRow(rowlist.GetAt(i));
	}
	m_WaxList.GotoRow(curnum);

	rowlist.RemoveAll();

	if( MedicalNolist.GetSize() <= 0 )
	{
		return;
	}

	for(int index = 0; index < MedicalNolist.GetSize(); index++)
	{
		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_MATERIAL + " set fxjg = '已收费' where medicalno = :medicalno and xh = :xh";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
			g_dbcommand.Param("xh").setAsString()        = XhNolist.GetAt(index);
			
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

	MedicalNolist.RemoveAll();
	XhNolist.RemoveAll();

	m_WaxList.ResetGridSize();
	m_WaxList.RedrawAll();
}

void CNormalSfqr::PerformQuery()
{
	OnButtonQuery();

	// Select all
	int curnum = m_WaxList.GetCurrentRow();
	for(int i = 0; i < m_WaxList.GetNumberRows(); i++)
	{
		m_WaxList.GotoRow(i);
	}
	m_WaxList.GotoRow(curnum);
	
	m_WaxList.SelectRange(0, 0, m_WaxList.GetNumberCols()-1,m_WaxList.GetNumberRows()-1);
}

void CNormalSfqr::OnDestroy() 
{
	CIniReader m_IniReader;
	m_IniReader.setINIFileName(theApp.m_Exe_Working_Dir + "Pathology.ini");

	CString dbsel;
	dbsel.Format("%d", ((CComboBox *)GetDlgItem(IDC_COMBO_DB))->GetCurSel());
	m_IniReader.setKey(dbsel,"常规收费确认数据库选择","Settings");

	CXTResizeDialog::OnDestroy();
}
