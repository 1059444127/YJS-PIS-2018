// Qppj.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "Pathologyview.h"
#include "Pathologydoc.h"
#include "Qppj.h"
#include "MedPreslice.h"
#include "Bpsmhd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQppj dialog
extern CPathologyApp theApp;

CQppj::CQppj(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CQppj::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQppj)
	m_Radio_State = 0;
	//}}AFX_DATA_INIT
}


void CQppj::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQppj)
	DDX_Radio(pDX, IDC_RADIO_1, m_Radio_State);
	DDX_Control(pDX,IDC_EDIT_QCBW,m_ctrl_qcbw);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQppj, CXTResizeDialog)
	//{{AFX_MSG_MAP(CQppj)
	ON_BN_CLICKED(IDC_CHECK_SELECTALL, OnCheckSelectall)
	ON_BN_CLICKED(IDC_BUTTON_PD, OnButtonPd)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_BPSMHD, OnButtonBpsmhd)
	ON_BN_CLICKED(IDC_BUTTON_BQDY, OnButtonBqdy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQppj message handlers

BOOL CQppj::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	m_SliceList.AttachGrid(this,IDC_STATIC_QPPJ);
	m_SliceList.SetParent(this);
	m_SliceList.ResetGridSize();
	m_SliceList.RedrawAll();
	
	SetResize(IDC_STATIC_QPPJ, SZ_TOP_LEFT ,  SZ_BOTTOM_RIGHT);

	SetResize(IDC_BUTTON_BPSMHD, SZ_TOP_LEFT ,  SZ_TOP_LEFT);
	SetResize(IDC_CHECK_SELECTALL, SZ_TOP_LEFT ,  SZ_TOP_LEFT);
	SetResize(IDC_RADIO_1, SZ_TOP_LEFT ,  SZ_TOP_LEFT);
	SetResize(IDC_RADIO_2, SZ_TOP_LEFT ,  SZ_TOP_LEFT);
	SetResize(IDC_RADIO_3, SZ_TOP_LEFT ,  SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_PD, SZ_TOP_LEFT ,  SZ_TOP_LEFT);
	SetResize(IDC_STATIC_QPZS, SZ_TOP_RIGHT ,  SZ_TOP_RIGHT);

	SetResize(IDC_STATIC_RWLY, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_COMBO_RWLY, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_QPZH, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_COMBO_QPZH, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_QPSM, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_COMBO_QPSM, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_ZPR, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_COMBO_ZPR, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_ZPSJ, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_DATE_ZPSJ, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_ADD, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_DELETE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDCANCEL, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	CString tmp;
	tmp.Format("切片总数：%d", m_SliceList.GetNumberRows());
	SetDlgItemText(IDC_STATIC_QPZS, tmp);

	//------------------------------------------------------------------
	CRect rt;
	GetDlgItem(IDC_DATE_ZPSJ)->GetWindowRect(&rt);
	ScreenToClient(&rt);
	if (!m_Time_Zpsj.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT, //,DTS_LONGDATEFORMAT
		rt, this, 113))
	{
		return FALSE;
	}
	
	m_Time_Zpsj.SetTime(theApp.GetOleServerDateTime());
	
	((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("冰冻");
	((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("常规");	
	((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("冰石");
	((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("冰余");
	((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("脱钙");
	((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("重切");
	((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("深切");
	((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("薄切");
	((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("连切");
	((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("切全");
	((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("补取");
	((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("电镜");
	((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("不削连切");	
	((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("特殊染色");
	((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("免疫组化");
	((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("分子病理");	
	((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("细胞涂片");
	((CComboBox *)GetDlgItem(IDC_COMBO_RWLY))->AddString("其它");
	
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
		//((CComboBox *)GetDlgItem(IDC_COMBO_QPZH))->SetWindowText("60");
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
	//------------------------------------------------------------------
	
	for(int i = 0; i < theApp.m_fnamelist.GetSize(); i++)
	{
		if(theApp.m_fnamelist.GetAt(i).CompareNoCase("qcbw") == 0)  //bbmc
		{
			m_ctrl_qcbw.m_info = theApp.m_finfolist.GetAt(i);
		}
	}
	
	m_ctrl_qcbw.SetBrowseStyle(BES_XT_POPUPMENU,0,0);
	m_ctrl_qcbw.m_TableName.Format("qcbw");

	if(theApp.m_BdSliceMode.CompareNoCase("冰冻") == 0)
	{
		GetDlgItem(IDC_COMBO_QPSM)->SetWindowText("冰冻");		
	} 
	else if(theApp.m_BdSliceMode.CompareNoCase("冰冻号") == 0)
	{
		//g_dbcommand.Param("qpsm").setAsString() = rwly;
		CString nBdh;
		theApp.GetDatabaseValue(theApp.GetView()->GetDocument()->m_blh, "bdh", nBdh, true);
		GetDlgItem(IDC_COMBO_QPSM)->SetWindowText(nBdh);
	}
	else
	{
		CString nBdh;
		theApp.GetDatabaseValue(theApp.GetView()->GetDocument()->m_blh, "bdh", nBdh, true);
		//g_dbcommand.Param("qpsm").setAsString() = "冰" + nBdh;
		CString temp = "冰" + nBdh;
		GetDlgItem(IDC_COMBO_QPSM)->SetWindowText(temp);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CQppj::OnCheckSelectall() 
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

void CQppj::OnButtonPd() 
{
	// TODO: Add your control notification handler code here
	CStringArray  MedicalNolist;
	CStringArray  XhNolist;
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
				CString str_MedicalNo;
				
				CUGCell  cell;
				m_SliceList.GetCellIndirect(0,nRow,&cell);
				cell.GetText(&str_MedicalNo);

				CString str_Qptmh;
				m_SliceList.GetCellIndirect(3,nRow,&cell);
				cell.GetText(&str_Qptmh);
				
				rowlist.Add(nRow);
				MedicalNolist.Add(str_MedicalNo);
				XhNolist.Add(str_Qptmh);
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

	rowlist.RemoveAll();

	if( MedicalNolist.GetSize() <= 0 )
	{
		return;
	}

	CString qppj;
	UpdateData(TRUE);
	switch(m_Radio_State) {
	case 0:
		qppj.Format("Ⅰ级");
		break;
	case 1:
		qppj.Format("Ⅱ级");
		break;
	case 2:
		qppj.Format("Ⅲ级");
		break;
	}

	for(int index = 0; index < MedicalNolist.GetSize(); index++)
	{
		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_SLICE + " set qppj = :qppj, qppjr = :qppjr where medicalno = :medicalno and qptmh = :qptmh";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
			g_dbcommand.Param("qptmh").setAsString()     = XhNolist.GetAt(index);
			g_dbcommand.Param("qppj").setAsString()      = qppj;  
			g_dbcommand.Param("qppjr").setAsString()     = theApp.m_Cpp_User;  
			
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

	m_SliceList.ResetGridSize();
	m_SliceList.RedrawAll();
}

void CQppj::OnSize(UINT nType, int cx, int cy) 
{
	CXTResizeDialog::OnSize(nType, cx, cy);

	if(m_Time_Zpsj.GetSafeHwnd() != NULL)
	{
		CRect rt;
		GetDlgItem(IDC_DATE_ZPSJ)->GetWindowRect(&rt);
		ScreenToClient(&rt);
		m_Time_Zpsj.SetWindowPos(NULL, rt.left, rt.top,rt.Width(),rt.Height(),SWP_NOZORDER);
		m_Time_Zpsj.RedrawWindow();
		GetDlgItem(IDC_DATE_ZPSJ)->RedrawWindow();
	}
}
extern void ExplainString(CString inputstr, CStringArray& StrList, CStringArray& UdList);
void CQppj::OnButtonAdd() 
{
	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  return;

	CString nBlh, nRwly, nQpzh, nQpsm, nZpr,nQcbw;
	nBlh.Empty(); nRwly.Empty(); nQpzh.Empty(); nQpsm.Empty(); nZpr.Empty();

	nBlh = theApp.GetView()->GetDocument()->m_blh;

	GetDlgItemText(IDC_COMBO_RWLY, nRwly);
	nRwly.TrimLeft(); nRwly.TrimRight();

	GetDlgItemText(IDC_COMBO_QPZH, nQpzh);
	nQpzh.TrimLeft(); nQpzh.TrimRight();

	GetDlgItemText(IDC_COMBO_QPSM, nQpsm);
	nQpsm.TrimLeft(); nQpsm.TrimRight();

	GetDlgItemText(IDC_COMBO_ZPR, nZpr);
	nZpr.TrimLeft(); nZpr.TrimRight();

	GetDlgItemText(IDC_EDIT_QCBW,nQcbw);
	nQcbw.TrimLeft();nQcbw.TrimRight();

	//if(nBlh.IsEmpty() || nQpzh.IsEmpty() || nQpsm.IsEmpty())
	//if(nBlh.IsEmpty() || nQpsm.IsEmpty())
	if(nBlh.IsEmpty())
	{
		AfxMessageBox("输入了无效的数据,请检查输入值的有效性!");
		return;
	}

	CStringArray StrList, UdList, QpzhList;
	QpzhList.RemoveAll();
	ExplainString(nQpzh, StrList, UdList);
	int i;
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

	for(i = 0; i < QpzhList.GetSize(); i++)
	{
		CString NewSliceNo = CMedPreslice::GenNewSliceNo(nBlh);
		//AfxMessageBox(NewSliceNo.Right(NewSliceNo.GetLength() - NewSliceNo.Find(nBlh) - 1));

		if(atoi(NewSliceNo.Right(2)) < 60)
		{
			NewSliceNo = nBlh + "60";
		}
		nQpzh = QpzhList.GetAt(i);
		try
		{
			SAString cmdstr = "Insert into " + theApp.TABLE_SLICE + " (medicalno,rwly,lkh,qpsm,qptmh,qpczy,qprq,zpzt,dyzt,other) values(:medicalno,:rwly,:lkh,:qpsm,:qptmh,:qpczy,:qprq,3,0,:other)";
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
			g_dbcommand.Param("other").setAsString() = nQcbw;
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
		m_SliceList.ResetGridSize();
		m_SliceList.RedrawAll();
		m_SliceList.RedrawAll();

		if(m_SliceList.GetNumberRows())
			m_SliceList.GotoRow(m_SliceList.GetNumberRows() - 1);

		//-----------------------------------
		if(QpzhList.GetSize() == 1 && nRwly.Compare("免疫组化") && nRwly.Compare("特殊染色") && nRwly.Compare("分子病理") && nRwly.Compare("电镜") ){
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
			if(nQpzh.GetLength() > 0)
			{
				BYTE ch = nQpzh.GetAt(nQpzh.GetLength() - 1);
				ch += 1;
				nQpzh.SetAt(nQpzh.GetLength() - 1, ch);

				SetDlgItemText(IDC_COMBO_QPZH, nQpzh);
			}
		}
	}
	//-----------------------------------
	CString totalCount = "0";
	try
	{
		SAString cmdstr = "select count(*) from " + theApp.TABLE_SLICE + " where medicalno=:medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		while(g_dbcommand.FetchNext())
		{
			totalCount.Format("%d",g_dbcommand.Field(1).asLong());
		}
		g_dbconnection.Commit();
	}
	catch (SAException& e)
	{
		AfxMessageBox((const char*)e.ErrText());
	}
	SetDlgItemText(IDC_STATIC_QPZS,"切片总数：" + totalCount);
}

void CQppj::OnButtonDelete() 
{
	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  return;

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
		m_SliceList.GetCellIndirect(3,rowlist.GetAt(i),&cell);
		cell.GetText(&str);
		nQptmhList.Add(str);
	}
	rowlist.RemoveAll();

	for(i = 0; i < nQptmhList.GetSize(); i++)
	{
		try
		{
			SAString cmdstr = "Delete from " + theApp.TABLE_SLICE + " where qptmh = :qptmh and dyzt = 0";
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

void CQppj::OnButtonBpsmhd() 
{
	CBpsmhd dlg(this);
	dlg.DoModal();

	m_SliceList.ResetGridSize();
	m_SliceList.RedrawAll();
}

void CQppj::OnButtonBqdy() 
{
	// TODO: Add your control notification handler code here
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
		
		m_SliceList.GetCellIndirect(4,rowlist.GetAt(i),&cell);
		cell.GetText(&str);
		theApp.m_QpsmList.Add(str);
		
		m_SliceList.GetCellIndirect(3,rowlist.GetAt(i),&cell);
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

BOOL CQppj::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_MOUSEMOVE)
	{
		CRect rt;
		CPoint pt;
		GetCursorPos(&pt);
		m_ctrl_qcbw.GetWindowRect(&rt);
		if(rt.PtInRect(pt))
		{
			m_ctrl_qcbw.did(TRUE);
		}
		else
		{
			m_ctrl_qcbw.did(FALSE);
		}
	}
	
	return CXTResizeDialog::PreTranslateMessage(pMsg);
}
