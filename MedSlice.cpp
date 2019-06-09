// CMedSlice : implementation file
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "Pathology.h"
#include "PathologyView.h"
#include "PathologyDoc.h"
#include "MedSlice.h"
#include "Bpsmhd.h"
#include "MedPreslice.h"
#include "Qpyj.h"
#include "QPSMINPUT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMedSlice
extern CPathologyApp theApp;

CMedSlice::CMedSlice()
	: CXTResizeFormView(CMedSlice::IDD)
{
	//{{AFX_DATA_INIT(CMedSlice)
	//}}AFX_DATA_INIT
	m_bIsFirstInit = true;
}

CMedSlice::~CMedSlice()
{
	// TODO: add destruction code here.

}

IMPLEMENT_DYNCREATE(CMedSlice, CXTResizeFormView)

BEGIN_MESSAGE_MAP(CMedSlice, CXTResizeFormView)
	//{{AFX_MSG_MAP(CMedSlice)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnButtonQuery)
	ON_BN_CLICKED(IDC_CHECK_SELECTALL, OnCheckSelectall)
	ON_BN_CLICKED(IDC_BUTTON_BPSMHD, OnButtonBpsmhd)
	ON_BN_CLICKED(IDC_BUTTON_BQDY, OnButtonBqdy)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BLH, OnDeltaposSpinBlh)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_TRANSTABLE, OnButtonTranstable)
	ON_COMMAND(IDM_BATCHUPADATE, OnBatchupadate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMedSlice message handlers

void CMedSlice::DoDataExchange(CDataExchange* pDX) 
{
	CXTResizeFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMedSlice)
	//}}AFX_DATA_MAP
}

void CMedSlice::OnInitialUpdate() 
{
	CXTResizeFormView::OnInitialUpdate();
	
	if (m_bIsFirstInit)
	{
		m_bIsFirstInit = false;

		CRect rt;
		GetDlgItem(IDC_DATE_QPRQ)->GetWindowRect(&rt);
		ScreenToClient(&rt);
		if (!m_Time_Qprq.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT, //,DTS_LONGDATEFORMAT
			rt, this, 112))
		{
			return;
		}

		m_Time_Qprq.SetTime(theApp.GetOleServerDateTime());

		m_SliceList.AttachGrid(this,IDC_STATIC_QPXX);
		m_SliceList.SetParent(this);

		//  切片日期查询
		COleDateTime m_time;
		m_Time_Qprq.GetTime(m_time);
		CString start = m_time.Format("%Y-%m-%d 00:00:00");
		CString end   = m_time.Format("%Y-%m-%d 23:59:59");
		m_SliceList.m_WhereBetween_Cmd.Format(" where qprq >= '%s' and qprq <= '%s'", start, end);
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			m_SliceList.m_WhereBetween_Cmd.Format(" where qprq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and qprq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", start, end);
		}

		if(theApp.m_StationType.CompareNoCase("常规切片工作站") == 0)
		{	
			if(theApp.m_CantainTsrs.CompareNoCase("是") == 0){
				m_SliceList.m_WhereBetween_Cmd += " and (rwly <> '免疫组化' and rwly <> '分子病理' and rwly <> '电镜' and rwly <> '细胞涂片')";
			} else {
				m_SliceList.m_WhereBetween_Cmd += " and (rwly <> '免疫组化' and rwly <> '特殊染色' and rwly <> '分子病理' and rwly <> '电镜' and rwly <> '细胞涂片')";
			}
		//	m_SliceList.m_WhereBetween_Cmd += " and (rwly <> '免疫组化' and rwly <> '特殊染色' and rwly <> '分子病理' and rwly <> '电镜' and rwly <> '细胞涂片')";
		}
		
		if(theApp.m_StationType.CompareNoCase("特检工作站") == 0)
		{
			if(theApp.m_CantainTsrs.CompareNoCase("否") == 0){
				m_SliceList.m_WhereBetween_Cmd += " and (rwly = '免疫组化' or rwly = '特殊染色' or rwly = '分子病理' or rwly = '电镜')";
			} else {
				m_SliceList.m_WhereBetween_Cmd += " and (rwly = '免疫组化' or rwly = '分子病理' or rwly = '电镜')";
			}
		}

		if(theApp.m_StationType.CompareNoCase("细胞学制片工作站") == 0)
		{
			m_SliceList.m_WhereBetween_Cmd += " and (rwly = '细胞涂片')";
		}
		
		m_SliceList.ResetGridSize();
		m_SliceList.RedrawAll();

		//--------------------------------------------------
		GetDlgItem(IDC_DATE_ZPSJ)->GetWindowRect(&rt);
		ScreenToClient(&rt);
		if (!m_Time_Zpsj.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT, //,DTS_LONGDATEFORMAT
			rt, this, 113))
		{
			return;
		}

		m_Time_Zpsj.SetTime(theApp.GetOleServerDateTime());


		if(theApp.m_StationType.CompareNoCase("制片工作站") == 0)
		{
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("常规");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("冰冻");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("冰石");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("脱钙");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("重切");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("深切");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("薄切");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("连切");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("补取");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("免疫组化");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("特殊染色");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("分子病理");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("电镜");
		}

		if(theApp.m_StationType.CompareNoCase("常规切片工作站") == 0)
		{
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("常规");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("冰冻");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("冰石");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("冰余");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("脱钙");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("重切");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("深切");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("薄切");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("连切");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("切全");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("不削全切");
			if(theApp.m_CantainTsrs.CompareNoCase("是") == 0){
				((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("特殊染色");
			}
		}

		if(theApp.m_StationType.CompareNoCase("特检工作站") == 0)
		{
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("免疫组化");
			if(theApp.m_CantainTsrs.CompareNoCase("否") == 0){
				((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("特殊染色");
			}			
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("分子病理");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("电镜");
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("其它");
		}

		if(theApp.m_StationType.CompareNoCase("细胞学制片工作站") == 0)
		{
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("细胞涂片");
		}

		if(((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->GetCount() > 0)
		{
			((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->SetCurSel(0);
		}

		CStdioFile File;
		if ( File.Open(theApp.m_Exe_Working_Dir + "system\\切片子号.txt", CFile::modeRead | CFile::typeText) )
		{
			CString tmp;
			while(1)
			{
				if( File.ReadString(tmp) == FALSE ) break;
				tmp.TrimLeft(); tmp.TrimRight();
				if(!tmp.IsEmpty())
				{
					((CComboBox *)GetDlgItem(IDC_COMBO_QPZH))->AddString(tmp);
				}
			}
			File.Close();
		}

		if(((CComboBox *)GetDlgItem(IDC_COMBO_QPZH))->GetCount() > 0)
		{
			((CComboBox *)GetDlgItem(IDC_COMBO_QPZH))->SetCurSel(0);
		}
		SetDlgItemText(IDC_COMBO_QPZH, theApp.m_InitSubCode);

		if ( File.Open(theApp.m_Exe_Working_Dir + "system\\切片说明.txt", CFile::modeRead | CFile::typeText) )
		{
			CString tmp;
			while(1)
			{
				if( File.ReadString(tmp) == FALSE ) break;
				tmp.TrimLeft(); tmp.TrimRight();
				if(!tmp.IsEmpty())
				{
					((CComboBox *)GetDlgItem(IDC_COMBO_QPSM))->AddString(tmp);
				}
			}
			File.Close();
		}

		if(((CComboBox *)GetDlgItem(IDC_COMBO_QPSM))->GetCount() > 0)
		{
			((CComboBox *)GetDlgItem(IDC_COMBO_QPSM))->SetCurSel(0);
		}

		if ( File.Open(theApp.m_Exe_Working_Dir + "system\\制片人.txt", CFile::modeRead | CFile::typeText) )
		{
			CString tmp;
			while(1)
			{
				if( File.ReadString(tmp) == FALSE ) break;
				tmp.TrimLeft(); tmp.TrimRight();
				if(!tmp.IsEmpty())
				{
					((CComboBox *)GetDlgItem(IDC_COMBO_ZPR))->AddString(tmp);
				}
			}
			File.Close();
		}

		if(((CComboBox *)GetDlgItem(IDC_COMBO_ZPR))->GetCount() > 0)
		{
			((CComboBox *)GetDlgItem(IDC_COMBO_ZPR))->SetCurSel(0);
		}

		if(!RIGHT_SCQPJL(theApp.m_Cpp_Right))
		{
			((CButton *)GetDlgItem(IDC_BUTTON_DELETE))->EnableWindow(FALSE);
		}
		else
		{
			((CButton *)GetDlgItem(IDC_BUTTON_DELETE))->EnableWindow(TRUE);
		}

		if ( File.Open(theApp.m_Exe_Working_Dir + "system\\制片病理号.txt", CFile::modeRead | CFile::typeText) )
		{
			CString tmp;
			if( File.ReadString(tmp))
			{
				tmp.TrimLeft(); tmp.TrimRight();
				SetDlgItemText(IDC_EDIT_EX_BLH, tmp);
			}
			File.Close();
		}

		return;
	}	
	m_SliceList.ResetGridSize();
	m_SliceList.RedrawAll();
}

void CMedSlice::Refresh()
{
	m_SliceList.ResetGridSize();
	m_SliceList.RedrawAll();

	if(!RIGHT_SCQPJL(theApp.m_Cpp_Right))
	{
		((CButton *)GetDlgItem(IDC_BUTTON_DELETE))->EnableWindow(FALSE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_BUTTON_DELETE))->EnableWindow(TRUE);
	}
}

extern void ExplainString(CString inputstr, CStringArray& StrList, CStringArray& UdList);

void CMedSlice::OnButtonQuery() 
{
	
	// TODO: Add your control notification handler code here
	CString mno; mno.Empty();
	GetDlgItemText(IDC_EDIT_BLH, mno);
	mno.TrimLeft(); mno.TrimRight();
	SetDlgItemText(IDC_EDIT_BLH, "");

	CString group;
	GetDlgItemText(IDC_COMBO_GROUP,group);
	group.TrimLeft();group.TrimRight();
	SetDlgItemText(IDC_COMBO_GROUP,"");

	if(mno.IsEmpty())
	{
		//  切片日期查询
		COleDateTime m_time;
		m_Time_Qprq.GetTime(m_time);

		CString start = m_time.Format("%Y-%m-%d 00:00:00");
		CString end   = m_time.Format("%Y-%m-%d 23:59:59");

		if(group.IsEmpty())
		{
			m_SliceList.m_WhereBetween_Cmd.Format(" where qprq >= '%s' and qprq <= '%s'", start, end);
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				m_SliceList.m_WhereBetween_Cmd.Format(" where qprq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and qprq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", start, end);
			}
		}
		else
		{
			m_SliceList.m_WhereBetween_Cmd.Format(" where qprq >= '%s' and qprq <= '%s' and medicalno like '%%%s'", start, end,group);
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				m_SliceList.m_WhereBetween_Cmd.Format(" where qprq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and qprq <= to_date('%s','yyyy-mm-dd hh24:mi:ss') and medicalno like '%%%s'", start, end, group);
			}
		}
	}
	else
	{
		//  病理号查询
		CStringArray StrList, UdList;
		ExplainString(mno, StrList, UdList);
		if(StrList.GetSize() == 1 && UdList.GetSize() == 0)
		{
			m_SliceList.m_WhereBetween_Cmd.Format(" where medicalno = '%s'", mno);
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

			m_SliceList.m_WhereBetween_Cmd.Format(" where (%s)", mstr);
		}
	
		StrList.RemoveAll();
		UdList.RemoveAll();
	}

	if(((CButton *)GetDlgItem(IDC_CHECK_NOTPRINT))->GetState() & 0x0003)
	{
		m_SliceList.m_WhereBetween_Cmd += " and dyzt = 0";
	}

	if(((CButton *)GetDlgItem(IDC_CHECK_NOTHD))->GetState() & 0x0003)
	{
		m_SliceList.m_WhereBetween_Cmd += " and bphd IS NULL";
	}

	if(theApp.m_StationType.CompareNoCase("常规切片工作站") == 0)
	{
		if(theApp.m_CantainTsrs.CompareNoCase("是") == 0){
			m_SliceList.m_WhereBetween_Cmd += " and (rwly <> '免疫组化' and rwly <> '分子病理' and rwly <> '电镜' and rwly <> '细胞涂片')";
		} else {
			m_SliceList.m_WhereBetween_Cmd += " and (rwly <> '免疫组化' and rwly <> '特殊染色' and rwly <> '分子病理' and rwly <> '电镜' and rwly <> '细胞涂片')";
		}
		
	}

	if(theApp.m_StationType.CompareNoCase("特检工作站") == 0)
	{
		if(theApp.m_CantainTsrs.CompareNoCase("否") == 0){
			m_SliceList.m_WhereBetween_Cmd += " and (rwly = '免疫组化' or rwly = '特殊染色' or rwly = '分子病理' or rwly = '电镜')";
		} else {
			m_SliceList.m_WhereBetween_Cmd += " and (rwly = '免疫组化' or rwly = '分子病理' or rwly = '电镜')";
		}
		
	}

	if(theApp.m_StationType.CompareNoCase("细胞学制片工作站") == 0)
	{
		m_SliceList.m_WhereBetween_Cmd += " and (rwly = '细胞涂片')";
	}

	m_SliceList.ResetGridSize();
	m_SliceList.RedrawAll();
	m_SliceList.ClearSelections();
	
}

void CMedSlice::OnCheckSelectall() 
{
	// TODO: Add your control notification handler code here
	if(((CButton *)GetDlgItem(IDC_CHECK_SELECTALL))->GetState() & 0x0003)
	{
		// Select all
		m_SliceList.SelectRange(0, 0, m_SliceList.GetNumberCols()-1,m_SliceList.GetNumberRows()-1);
	}
	else
	{
		// Clear all selection
		m_SliceList.ClearSelections();
	}
}

void CMedSlice::OnButtonBpsmhd() 
{
	CBpsmhd dlg(this);
	dlg.DoModal();
}

void CMedSlice::OnButtonBqdy() 
{
	theApp.m_QptmhList.RemoveAll();
	theApp.m_QpsmList.RemoveAll();
	theApp.m_LkhList.RemoveAll();

	CUIntArray  rowlist;
	int nCol, nRet;
	long nRow;

	nRet = m_SliceList.EnumFirstSelected( &nCol, &nRow );
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
				rowlist.Add(nRow);
			}
		}
		
		nRet = m_SliceList.EnumNextSelected( &nCol, &nRow );
	}

	int curnum = m_SliceList.GetCurrentRow();
	for(int i = 0; i < rowlist.GetSize(); i++)
	{
		m_SliceList.GotoRow(rowlist.GetAt(i));
	}
	m_SliceList.GotoRow(curnum);

	if( rowlist.GetSize() <= 0 )
	{
		return;
	}
	
	for(i = 0; i < rowlist.GetSize(); i++)
	{
		CString str;
		CUGCell  cell;
		m_SliceList.GetCellIndirect(2,rowlist.GetAt(i),&cell);
		cell.GetText(&str);
		theApp.m_LkhList.Add(str);
		
		m_SliceList.GetCellIndirect(3,rowlist.GetAt(i),&cell);
		cell.GetText(&str);
		theApp.m_QpsmList.Add(str);
		
		m_SliceList.GetCellIndirect(4,rowlist.GetAt(i),&cell);
		cell.GetText(&str);
		theApp.m_QptmhList.Add(str);
	}
	rowlist.RemoveAll();

	theApp.m_PrintType = 1;
	theApp.GetView()->SendMessage(WM_COMMAND, ID_FILE_PRINT);

	if(theApp.GetView()->m_OnPreparePrinting_Return)
	{
		for(i = 0; i < theApp.m_QptmhList.GetSize(); i++)
		{
			try
			{
				SAString cmdstr = "Update " + theApp.TABLE_SLICE + " set dyzt = 1 where qptmh = :qptmh";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("qptmh").setAsString() = theApp.m_QptmhList.GetAt(i);
				
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

	m_SliceList.ResetGridSize();
	m_SliceList.RedrawAll();
	m_SliceList.RedrawAll();
}

void CMedSlice::OnDeltaposSpinBlh(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString curno, str, str1;
	GetDlgItemText(IDC_EDIT_EX_BLH, curno);
	curno.TrimLeft(); curno.TrimRight();
	
	if(pNMUpDown->iDelta >= 0)
	{
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
	}
	else
	{
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
	}

	SetDlgItemText(IDC_EDIT_EX_BLH, str);
	
	SetDlgItemText(IDC_COMBO_QPZH, theApp.m_InitSubCode);

	*pResult = 0;
}

extern void ExplainString(CString inputstr, CStringArray& StrList, CStringArray& UdList);

void CMedSlice::OnButtonAdd() 
{
	CString nBlh, nRwly, nQpzh, nQpsm, nZpr;
	nBlh.Empty(); nRwly.Empty(); nQpzh.Empty(); nQpsm.Empty(); nZpr.Empty();

	GetDlgItemText(IDC_EDIT_EX_BLH, nBlh);
	nBlh.TrimLeft(); nBlh.TrimRight();

	GetDlgItemText(IDC_COMBO_RWLY, nRwly);
	nRwly.TrimLeft(); nRwly.TrimRight();

	GetDlgItemText(IDC_COMBO_QPZH, nQpzh);
	nQpzh.TrimLeft(); nQpzh.TrimRight();

	GetDlgItemText(IDC_COMBO_QPSM, nQpsm);
	nQpsm.TrimLeft(); nQpsm.TrimRight();

	GetDlgItemText(IDC_COMBO_ZPR, nZpr);
	nZpr.TrimLeft(); nZpr.TrimRight();

	//if(nBlh.IsEmpty() || nQpzh.IsEmpty() || nQpsm.IsEmpty())
	//if(nBlh.IsEmpty() || nQpsm.IsEmpty())
	if(nBlh.IsEmpty())
	{
		AfxMessageBox("输入了无效的数据,请检查输入值的有效性!");
		return;
	}

	CStringArray StrList, UdList, BlhList;
	BlhList.RemoveAll();
	ExplainString(nBlh, StrList, UdList);
	//--------------------------------
	for(int i = 0; i < StrList.GetSize(); i++)
	{
		BlhList.Add(StrList.GetAt(i));
	}
	
	for(i = 0; i < UdList.GetSize(); i += 2)
	{
		CString upstr = UdList.GetAt(i);
		CString dnstr = UdList.GetAt(i+1);

		int count = 0;

		while(atoi(upstr) <= atoi(dnstr))
		{
			BlhList.Add(upstr);

			CString str,str1;
			char strbuf[300];
			int Len=upstr.GetLength(),k;
			BYTE ch;
			for(k=(Len-1);k>=0;k--)
			{
				ch = upstr.GetAt(k); 
				if(!(ch >= 0x30 && ch <= 0x39))  break;
			}
			k = Len-1-k;
			ltoa(atol(upstr.Right(k))+1,strbuf,10);
			str1.Format("%s",strbuf);
			str = upstr.Left(Len-k);
			Len = k - str1.GetLength();
			for(k=0;k<Len;k++)  str += "0";
			str += str1;
			upstr = str;

			count++;
			if(count > 50)  break;
		}
	}
	StrList.RemoveAll();
	UdList.RemoveAll();
	//--------------------------------
	CStringArray QpzhList;
	QpzhList.RemoveAll();
	
	ExplainString(nQpzh, StrList, UdList);
	
	//--------------------------------
	for(i = 0; i < StrList.GetSize(); i++)
	{
		QpzhList.Add(StrList.GetAt(i));
	}
	
	for(i = 0; i < UdList.GetSize(); i += 2)
	{
		CString upstr = UdList.GetAt(i);
		CString dnstr = UdList.GetAt(i+1);
		int count = 0;
		byte b; bool is;
		b = upstr.GetAt(0);
		if(b >= 0x30 && b <= 0x39){
			is = true;
		} else {
			is =false;
		}
		
		
		while( is ? atoi(upstr) <= atoi(dnstr) : upstr <= dnstr )
		{
			QpzhList.Add(upstr);
			
			CString str,str1;
			char strbuf[300];
			int Len=upstr.GetLength(),k;
			BYTE ch;
			for(k=(Len-1);k>=0;k--)
			{
				ch = upstr.GetAt(k); 
				if(!(ch >= 0x30 && ch <= 0x39))  break;
			}
			if(k == Len - 1){
				str  = upstr.Left(k);
				char c = upstr.GetAt(k) + 1;
				upstr = str + c;
			} else {
			k = Len-1-k;
			ltoa(atol(upstr.Right(k))+1,strbuf,10);
			str1.Format("%s",strbuf);
			
			str = upstr.Left(Len-k);
			Len = k - str1.GetLength();
			for(k=0;k<Len;k++)  str += "0";
			str += str1;
			upstr = str;
			}
			count++;
			if(count > 50)  break;
		}
	}
	StrList.RemoveAll();
	UdList.RemoveAll();
	
	if(BlhList.GetSize() == 1)
	{
		for(i = 0; i < QpzhList.GetSize(); i++)
		{
			if(QpzhList.GetAt(i).IsEmpty()){
				nQpzh = nBlh;
			}else{
			nQpzh = BlhList.GetAt(0) + "-" +QpzhList.GetAt(i);
			}

			CString NewSliceNo = CMedPreslice::GenNewSliceNo(BlhList.GetAt(0), 60);
			
			try
			{
				SAString cmdstr = "Insert into " + theApp.TABLE_SLICE + " (medicalno,rwly,lkh,qpsm,qptmh,qpczy,qprq,zpzt,dyzt) values(:medicalno,:rwly,:lkh,:qpsm,:qptmh,:qpczy,:qprq,3,0)";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = nBlh;
				g_dbcommand.Param("rwly").setAsString() = nRwly;
				if(nQpzh.IsEmpty())
					g_dbcommand.Param("lkh").setAsString()  = nBlh;
				else
					g_dbcommand.Param("lkh").setAsString()  = nQpzh;
				g_dbcommand.Param("qpsm").setAsString() = nQpsm;
				g_dbcommand.Param("qptmh").setAsString() = NewSliceNo;
				g_dbcommand.Param("qpczy").setAsString() = nZpr;
				
				COleDateTime m_time;
				m_Time_Zpsj.GetTime(m_time);
				SADateTime qprq(m_time.GetYear(),m_time.GetMonth(),m_time.GetDay(),m_time.GetHour(),m_time.GetMinute(),m_time.GetSecond());
				g_dbcommand.Param("qprq").setAsDateTime() = qprq;
				
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
	else {
   for(i = 0; i < BlhList.GetSize(); i++)
	{
		nBlh = BlhList.GetAt(i);
		CString NewSliceNo = CMedPreslice::GenNewSliceNo(nBlh, 60);
		try
		{
			SAString cmdstr = "Insert into " + theApp.TABLE_SLICE + " (medicalno,rwly,lkh,qpsm,qptmh,qpczy,qprq,zpzt,dyzt) values(:medicalno,:rwly,:lkh,:qpsm,:qptmh,:qpczy,:qprq,3,0)";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = nBlh;
			g_dbcommand.Param("rwly").setAsString() = nRwly;
			if(nQpzh.IsEmpty())
				g_dbcommand.Param("lkh").setAsString()  = nBlh;
			else
				g_dbcommand.Param("lkh").setAsString()  = nBlh + "-" + nQpzh;
			g_dbcommand.Param("qpsm").setAsString() = nQpsm;
			g_dbcommand.Param("qptmh").setAsString() = NewSliceNo;
			g_dbcommand.Param("qpczy").setAsString() = nZpr;
			
			COleDateTime m_time;
			m_Time_Zpsj.GetTime(m_time);
			SADateTime qprq(m_time.GetYear(),m_time.GetMonth(),m_time.GetDay(),m_time.GetHour(),m_time.GetMinute(),m_time.GetSecond());
			g_dbcommand.Param("qprq").setAsDateTime() = qprq;
			
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
	m_SliceList.ResetGridSize();
	m_SliceList.RedrawAll();
	m_SliceList.RedrawAll();

/*
	if(m_SliceList.GetNumberRows())
		m_SliceList.GotoRow(m_SliceList.GetNumberRows() - 1);
*/
	if(m_SliceList.GetNumberRows())
		m_SliceList.GotoRow(0);
 
	if(BlhList.GetSize() == 1 && QpzhList.GetSize() == 1 && nRwly.Compare("免疫组化") && nRwly.Compare("特殊染色") && nRwly.Compare("分子病理") && nRwly.Compare("电镜") )
	{
		//-----------------------------------
		
		nQpzh = QpzhList.GetAt(0);
		CString str,str1;
		char strbuf[300];
		int Len=nQpzh.GetLength(),k;
		BYTE ch;
		for(k=(Len-1);k>=0;k--)
		{
			ch = nQpzh.GetAt(k); 
			if(!(ch >= 0x30 && ch <= 0x39))  break;
		}
		k = Len-1-k;
		if(k > 0)
		{
			ltoa(atol(nQpzh.Right(k))+1,strbuf,10);
			str1.Format("%s",strbuf);
			str = nQpzh.Left(Len-k);
			Len = k - str1.GetLength();
			for(k=0;k<Len;k++)  str += "0";
			str += str1;
			
			SetDlgItemText(IDC_COMBO_QPZH, str);
		}
		else
		{	
			nQpzh = QpzhList.GetAt(0);
			if(nQpzh.GetLength() > 0)
			{
				BYTE ch = nQpzh.GetAt(nQpzh.GetLength() - 1);
				ch += 1;
				nQpzh.SetAt(nQpzh.GetLength() - 1, ch);
				
				SetDlgItemText(IDC_COMBO_QPZH, nQpzh);
			}
		}
		//-----------------------------------
	}
	
	BlhList.RemoveAll();
}

void CMedSlice::OnButtonDelete() 
{
	CUIntArray  rowlist;
	int nCol, nRet;
	long nRow;

	nRet = m_SliceList.EnumFirstSelected( &nCol, &nRow );
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
				rowlist.Add(nRow);
			}
		}
		
		nRet = m_SliceList.EnumNextSelected( &nCol, &nRow );
	}

	int curnum = m_SliceList.GetCurrentRow();
	for(int i = 0; i < rowlist.GetSize(); i++)
	{
		m_SliceList.GotoRow(rowlist.GetAt(i));
	}
	m_SliceList.GotoRow(curnum);

	if( rowlist.GetSize() <= 0 )
	{
		return;
	}
	
	CString info;
	info.Format("确认删除指定的 %d 条切片记录？", rowlist.GetSize());
	if(AfxMessageBox(info ,MB_OKCANCEL|MB_ICONQUESTION) == IDCANCEL)
	{
		m_SliceList.ClearSelections();
		rowlist.RemoveAll();
		return;
	}

	CStringArray nQptmhList;

	for(i = 0; i < rowlist.GetSize(); i++)
	{
		CString str;
		CUGCell  cell;
		m_SliceList.GetCellIndirect(4,rowlist.GetAt(i),&cell);
		cell.GetText(&str);
		nQptmhList.Add(str);
	}
	rowlist.RemoveAll();

	for(i = 0; i < nQptmhList.GetSize(); i++)
	{
		try
		{
			//SAString cmdstr = "Delete from " + theApp.TABLE_SLICE + " where qptmh = :qptmh and dyzt = 0";
			SAString cmdstr = "Delete from " + theApp.TABLE_SLICE + " where qptmh = :qptmh and bphd IS NULL";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("qptmh").setAsString() = nQptmhList.GetAt(i);
			
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

	nQptmhList.RemoveAll();

	m_SliceList.ResetGridSize();
	m_SliceList.RedrawAll();
	m_SliceList.RedrawAll();
}

void CMedSlice::OnDestroy() 
{
	CXTResizeFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	CStdioFile File;
	if ( File.Open(theApp.m_Exe_Working_Dir + "system\\制片病理号.txt", CFile::modeWrite |CFile::modeCreate | CFile::typeText) )
	{
		CString tmp;
		GetDlgItemText(IDC_EDIT_EX_BLH, tmp);
		File.WriteString(tmp);
		File.Close();
	}
}

void CMedSlice::OnButtonTranstable() 
{
	CQpyj mydlg(this);
	mydlg.DoModal();
}

BOOL CMedSlice::PreTranslateMessage(MSG* pMsg) 
{
	if(m_hWnd)
	{
		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN && GetDlgItem(IDC_EDIT_BLH) == GetFocus())
		{
			OnButtonQuery();
			return TRUE;
		}
	}
	
	return CXTResizeFormView::PreTranslateMessage(pMsg);
}

void CMedSlice::PerformQuery(){
	OnButtonQuery();
	
	// Select all
	m_SliceList.SelectRange(0, 0, m_SliceList.GetNumberCols()-1,m_SliceList.GetNumberRows()-1);
}

void CMedSlice::OnBatchupadate() 
{
	// TODO: Add your command handler code here
	int col; long row,row_old;
	int result;
	CString qptmh; qptmh="where qptmh in(";
	result = m_SliceList.EnumFirstSelected(&col,&row);
	
	while(result == UG_SUCCESS){
		if(qptmh.CompareNoCase("where qptmh in(")){
			qptmh += ",";
		}
		CString temp = m_SliceList.QuickGetText(4,row);
		if(!temp.IsEmpty()){
			qptmh += "'";
			qptmh += temp;
			qptmh += "'";
		}
	
		result = m_SliceList.EnumNextSelected(&col,&row);
	}
	qptmh += ")";

	CQPSMINPUT dlg;
	if(dlg.DoModal() == IDCANCEL){
		return;
	}

	try{

		SAString cmdstr;
		if(m_batchType.CompareNoCase("qpsm") == 0)
		{
			cmdstr = "update "+ theApp.TABLE_SLICE + " set qpsm=:qpsm "+qptmh;
		}
		else if(m_batchType.CompareNoCase("bz") == 0)
		{
			cmdstr = "update "+ theApp.TABLE_SLICE + " set other=:other "+qptmh;
		}
		
		g_dbcommand.setCommandText(cmdstr);
		if(m_batchType.CompareNoCase("qpsm") == 0)
		{
			g_dbcommand.Param("qpsm").setAsString() = dlg.m_qpsm;
		}
		else if(m_batchType.CompareNoCase("bz") == 0)
		{
			g_dbcommand.Param("other").setAsString() = dlg.m_qpsm;
		}
		
		g_dbcommand.Execute();
		g_dbconnection.Commit();
	} catch (SAException &e){
		AfxMessageBox((const char*)e.ErrText());
	}
	m_SliceList.ResetGridSize();
	m_SliceList.RedrawAll();
}
