// ChargeConfirm.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "ChargeConfirm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChargeConfirm dialog
extern CPathologyApp theApp;

CChargeConfirm::CChargeConfirm(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CChargeConfirm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChargeConfirm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CChargeConfirm::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChargeConfirm)
	DDX_Control(pDX, IDC_HYP_NOTE, m_HypLink_Note);
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNCREATE(CChargeConfirm, CXTResizeDialog)

BEGIN_MESSAGE_MAP(CChargeConfirm, CXTResizeDialog)
	//{{AFX_MSG_MAP(CChargeConfirm)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnButtonQuery)
	ON_BN_CLICKED(IDC_BUTTON_YZQR, OnButtonYzqr)
	ON_BN_CLICKED(IDC_CHECK_SELECTALL, OnCheckSelectall)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULTQUERY, OnButtonDefaultquery)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChargeConfirm message handlers

BOOL CChargeConfirm::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	CRect rt;
	GetDlgItem(IDC_DATE_SQRQ)->GetWindowRect(&rt);
	ScreenToClient(&rt);
	if (!m_Time_Sqrq.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT, //,DTS_LONGDATEFORMAT
		rt, this, 112))
	{
		return FALSE;
	}
	
	m_Time_Sqrq.SetTime(theApp.GetOleServerDateTime());

	m_grid.AttachGrid(this,IDC_STATIC_RECORDLIST);
	m_grid.SetParent(this);
	
	m_grid.m_WhereBetween_Cmd.Format(" where (fxjg IS NULL or fxjg <> '已收费') and (yzlx = '免疫组化' or yzlx = '特殊染色' or yzlx = '分子病理' or yzlx = '电镜' or yzlx = '其它')");
	m_grid.ResetGridSize();
	m_grid.RedrawAll();

	((CComboBox *)GetDlgItem(IDC_COMBO_SFZT))->SetCurSel(0);

	SetResize(IDC_STATIC_BLH, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_BLH, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_SQRQ, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_DATE_SQRQ, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_SFZT, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO_SFZT, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_QUERY, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_CHECK_SELECTALL, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_YZQR, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_RECORDLIST, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_HYP_NOTE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_DEFAULTQUERY, SZ_TOP_LEFT, SZ_TOP_LEFT);
	
	m_HypLink_Note.SetURL("");
	m_HypLink_Note.SetColors(RGB(0x00, 0x00, 0xFF), RGB(0x00, 0x00, 0xFF), RGB(0x00, 0x00, 0xFF));
	m_HypLink_Note.SetUnderline(false);
	m_HypLink_Note.EnableShellExecute(false);

	CString info;
	info.Format("当前查询记录数：%d", m_grid.GetNumberRows());
	m_HypLink_Note.SetWindowText(info);
	m_HypLink_Note.Invalidate();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChargeConfirm::OnButtonYzqr() 
{
	if(AfxMessageBox("选择进行收费确认，该过程不可逆，是否继续？",MB_OKCANCEL|MB_ICONQUESTION) == IDCANCEL)  return;

	CStringArray  MedicalNolist;
	CStringArray  XhNolist;
	CUIntArray  rowlist;
	int nCol, nRet;
	long nRow;

	nRet = m_grid.EnumFirstSelected( &nCol, &nRow );
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
				m_grid.GetCellIndirect(0,nRow,&cell);
				cell.GetText(&str_MedicalNo);

				CString str_Yzh;
				m_grid.GetCellIndirect(1,nRow,&cell);
				cell.GetText(&str_Yzh);
				
				rowlist.Add(nRow);
				MedicalNolist.Add(str_MedicalNo);
				XhNolist.Add(str_Yzh);
			}
		}
		
		nRet = m_grid.EnumNextSelected( &nCol, &nRow );
	}

	int curnum = m_grid.GetCurrentRow();
	for(int i = 0; i < rowlist.GetSize(); i++)
	{
		m_grid.GotoRow(rowlist.GetAt(i));
	}
	m_grid.GotoRow(curnum);

	rowlist.RemoveAll();

	if( MedicalNolist.GetSize() <= 0 )
	{
		return;
	}

	for(int index = 0; index < MedicalNolist.GetSize(); index++)
	{
		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_ADVICE + " set fxjg = '已收费' where medicalno = :medicalno and yzh = :yzh";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
			g_dbcommand.Param("yzh").setAsString()       = XhNolist.GetAt(index);
			
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

	m_grid.ResetGridSize();
	m_grid.RedrawAll();

	CString info;
	info.Format("当前查询记录数：%d", m_grid.GetNumberRows());
	m_HypLink_Note.SetWindowText(info);
	m_HypLink_Note.Invalidate();
}

void CChargeConfirm::OnCheckSelectall() 
{
	if(((CButton *)GetDlgItem(IDC_CHECK_SELECTALL))->GetState() & 0x0003)
	{
		// Select all
		m_grid.SelectRange(0, 0, m_grid.GetNumberCols()-1,m_grid.GetNumberRows()-1);
	}
	else
	{
		// Clear all selection
		m_grid.ClearSelections();
	}
}

BOOL CChargeConfirm::PreTranslateMessage(MSG* pMsg) 
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

extern void ExplainString(CString inputstr, CStringArray& StrList, CStringArray& UdList);
void CChargeConfirm::OnButtonQuery() 
{
	// Clear all selection
	m_grid.ClearSelections();

	CString mno; mno.Empty();
	GetDlgItemText(IDC_EDIT_BLH, mno);
	mno.TrimLeft(); mno.TrimRight();
	SetDlgItemText(IDC_EDIT_BLH, "");

	if(mno.IsEmpty())
	{
		//  取材日期查询
		COleDateTime m_time;
		m_Time_Sqrq.GetTime(m_time);

		CString start = m_time.Format("%Y-%m-%d 00:00:00");
		CString end   = m_time.Format("%Y-%m-%d 23:59:59");

		m_grid.m_WhereBetween_Cmd.Format(" where sqrq >= '%s' and sqrq <= '%s'", start, end);
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			m_grid.m_WhereBetween_Cmd.Format(" where sqrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and sqrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", start, end);
		}
	}
	else
	{
		//  病理号查询
		CStringArray StrList, UdList;
		ExplainString(mno, StrList, UdList);
		if(StrList.GetSize() == 1 && UdList.GetSize() == 0)
		{
			m_grid.m_WhereBetween_Cmd.Format(" where medicalno = '%s'", mno);
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

			m_grid.m_WhereBetween_Cmd.Format(" where (%s)", mstr);
		}
	
		StrList.RemoveAll();
		UdList.RemoveAll();
	}

	switch(((CComboBox *)GetDlgItem(IDC_COMBO_SFZT))->GetCurSel()) {
	case 0:
		m_grid.m_WhereBetween_Cmd += " and (fxjg IS NULL or fxjg <> '已收费')";
		break;
	case 1:
		m_grid.m_WhereBetween_Cmd += " and fxjg = '已收费'";
		break;
	case 2:
		break;
	}


	m_grid.m_WhereBetween_Cmd += " and (yzlx = '免疫组化' or yzlx = '特殊染色' or yzlx = '分子病理' or yzlx = '电镜' or yzlx = '其它')";

	m_grid.ResetGridSize();
	m_grid.RedrawAll();

	CString info;
	info.Format("当前查询记录数：%d", m_grid.GetNumberRows());
	m_HypLink_Note.SetWindowText(info);
	m_HypLink_Note.Invalidate();
}

void CChargeConfirm::OnButtonDefaultquery() 
{
	// Clear all selection
	m_grid.ClearSelections();

	m_grid.m_WhereBetween_Cmd.Format(" where (fxjg IS NULL or fxjg <> '已收费') and (yzlx = '免疫组化' or yzlx = '特殊染色' or yzlx = '分子病理' or yzlx = '电镜')");
	m_grid.ResetGridSize();
	m_grid.RedrawAll();

	CString info;
	info.Format("当前查询记录数：%d", m_grid.GetNumberRows());
	m_HypLink_Note.SetWindowText(info);
	m_HypLink_Note.Invalidate();
}

void CChargeConfirm::PerformQuery()
{
	OnButtonQuery();

	// Select all
	m_grid.SelectRange(0, 0, m_grid.GetNumberCols()-1,m_grid.GetNumberRows()-1);
}
