// CMedAdvice : implementation file
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "Pathology.h"
#include "PathologyView.h"
#include "PathologyDoc.h"
#include "MedAdvice.h"
#include "MedPreslice.h"
#include "Gzddy.h"
#include "AdviceDelReason.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMedAdvice
extern CPathologyApp theApp;

CMedAdvice::CMedAdvice()
	: CXTResizeFormView(CMedAdvice::IDD)
{
	//{{AFX_DATA_INIT(CMedAdvice)
	//}}AFX_DATA_INIT

	m_bIsFirstInit = true;
}

CMedAdvice::~CMedAdvice()
{
	// TODO: add destruction code here.

}

IMPLEMENT_DYNCREATE(CMedAdvice, CXTResizeFormView)

BEGIN_MESSAGE_MAP(CMedAdvice, CXTResizeFormView)
	//{{AFX_MSG_MAP(CMedAdvice)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnButtonQuery)
	ON_BN_CLICKED(IDC_CHECK_SELECTALL, OnCheckSelectall)
	ON_BN_CLICKED(IDC_BUTTON_YZZH, OnButtonYzzh)
	ON_BN_CLICKED(IDC_BUTTON_TRANSTABLE, OnButtonTranstable)
	ON_BN_CLICKED(IDC_BUTTON_YZQR, OnButtonYzqr)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULTQUERY, OnButtonDefaultquery)
	ON_BN_CLICKED(IDC_BUTTON_ADVICEDEL, OnButtonAdvicedel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMedAdvice message handlers

void CMedAdvice::DoDataExchange(CDataExchange* pDX) 
{
	CXTResizeFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMedAdvice)
	//}}AFX_DATA_MAP
}

void CMedAdvice::OnInitialUpdate() 
{
	CXTResizeFormView::OnInitialUpdate();
	
	if (m_bIsFirstInit)
	{
		m_bIsFirstInit = false;

		CRect rt;
		GetDlgItem(IDC_DATE_SQRQ)->GetWindowRect(&rt);
		ScreenToClient(&rt);
		if (!m_Time_Sqrq.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT, //,DTS_LONGDATEFORMAT
			rt, this, 112))
		{
			return;
		}

		m_Time_Sqrq.SetTime(theApp.GetOleServerDateTime());

		m_AdviceList.AttachGrid(this,IDC_STATIC_YZXX);
		m_AdviceList.SetParent(this);
		m_AdviceList.ResetGridSize();
		m_AdviceList.RedrawAll();

		((CComboBox *)GetDlgItem(IDC_COMBO_YZZT))->SetCurSel(0);
		
		if(theApp.m_StationType.CompareNoCase("������Ƭ����վ") == 0)
		{
			
			if(theApp.m_CantainTsrs.CompareNoCase("��") == 0){
				((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->DeleteString(5);
				for(int k = 0; k < 3; k++)
				{
					((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->DeleteString(6);
				}
			} else {
				for(int k = 0; k < 5; k++)
				{
					((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->DeleteString(5);
				}
			}
		}

		((CButton *)GetDlgItem(IDC_CHECK_ADDBJW))->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ADVICEDEL)->ShowWindow(SW_HIDE);
		if(theApp.m_StationType.CompareNoCase("�ؼ칤��վ") == 0)
		{
			((CButton *)GetDlgItem(IDC_CHECK_ADDBJW))->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BUTTON_ADVICEDEL)->ShowWindow(SW_SHOW);
			if(!RIGHT_SCQPJL(theApp.m_Cpp_Right))
			{
				GetDlgItem(IDC_BUTTON_ADVICEDEL)->EnableWindow(FALSE);
			}
			for(int k = 0; k < 5; k++)
			{
				((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->DeleteString(0);
			}

			for(k = 0; k < 2; k++)
			{
				((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->DeleteString(5);
			}

			if(theApp.m_CantainTsrs.CompareNoCase("��") == 0)
			{
				((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->DeleteString(1);
			}
			
		}

		((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->SetCurSel(((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->GetCount()-1);
		return;
	}	

	m_AdviceList.ResetGridSize();
	m_AdviceList.RedrawAll();
}

void CMedAdvice::Refresh()
{
	m_AdviceList.ResetGridSize();
	m_AdviceList.RedrawAll();
}

extern void ExplainString(CString inputstr, CStringArray& StrList, CStringArray& UdList);

void CMedAdvice::OnButtonQuery() 
{
	// TODO: Add your control notification handler code here
	CString mno; mno.Empty();
	GetDlgItemText(IDC_EDIT_BLH, mno);
	mno.TrimLeft(); mno.TrimRight();
	SetDlgItemText(IDC_EDIT_BLH, "");

	if(mno.IsEmpty())
	{
		//  ȡ�����ڲ�ѯ
		COleDateTime m_time;
		m_Time_Sqrq.GetTime(m_time);

		CString start = m_time.Format("%Y-%m-%d 00:00:00");
		CString end   = m_time.Format("%Y-%m-%d 23:59:59");

		m_AdviceList.m_WhereBetween_Cmd.Format(" where sqrq >= '%s' and sqrq <= '%s'", start, end);
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			m_AdviceList.m_WhereBetween_Cmd.Format(" where sqrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and sqrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", start, end);
		}
	}
	else
	{
		//  ����Ų�ѯ
		CStringArray StrList, UdList;
		ExplainString(mno, StrList, UdList);
		if(StrList.GetSize() == 1 && UdList.GetSize() == 0)
		{
			m_AdviceList.m_WhereBetween_Cmd.Format(" where medicalno = '%s'", mno);
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

			m_AdviceList.m_WhereBetween_Cmd.Format(" where (%s)", mstr);
		}
	
		StrList.RemoveAll();
		UdList.RemoveAll();
	}

	switch(((CComboBox *)GetDlgItem(IDC_COMBO_YZZT))->GetCurSel()) {
	case 0:
		m_AdviceList.m_WhereBetween_Cmd += " and yzzt = 0";
		break;
	case 1:
		m_AdviceList.m_WhereBetween_Cmd += " and yzzt = 1";
		break;
	case 2:
		break;
	}

	if(theApp.m_StationType.CompareNoCase("��Ƭ����վ") == 0)
	{
		switch(((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->GetCurSel()) {
		case 0:
			m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '����'";
			break;
		case 1:
			m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '����'";
			break;
		case 2:
			m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '����'";
			break;
		case 3:
			m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '����'";
			break;
		case 4:
			m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '����'";
			break;
		case 5:
			m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '�����黯'";
			break;
		case 6:
			m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '����Ⱦɫ'";
			break;
		case 7:
			m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '���Ӳ���'";
			break;
		case 8:
			m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '�羵'";
			break;
		case 9:
			m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '����'";
			break;
		case 10:
			m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '��ȫ'";
			break;
		case 11:
			m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '��������'";
			break;
		case 12:
			break;
		}
	}

	if(theApp.m_StationType.CompareNoCase("������Ƭ����վ") == 0)
	{
		if(theApp.m_CantainTsrs.CompareNoCase("��") == 0){
			switch(((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->GetCurSel()) {
			case 0:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '����'";
				break;
			case 1:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '����'";
				break;
			case 2:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '����'";
				break;
			case 3:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '����'";
				break;
			case 4:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '����'";
				break;
			case 5:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '����Ⱦɫ'";
				break;
			case 6:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '��ȫ'";
				break;
			case 7:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '��������'";
				break;
			case 8:
				m_AdviceList.m_WhereBetween_Cmd += " and (yzlx = '����' or yzlx = '����' or yzlx = '����' or yzlx = '����' or yzlx = '����' or yzlx = '����Ⱦɫ' or yzlx = '��ȫ' or yzlx = '��������')";
				break;
			
			}
		} else {
			switch(((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->GetCurSel()) {
			case 0:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '����'";
				break;
			case 1:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '����'";
				break;
			case 2:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '����'";
				break;
			case 3:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '����'";
				break;
			case 4:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '����'";
				break;
			case 5:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '��ȫ'";
				break;
			case 6:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '��������'";
				break;
			case 7:
				m_AdviceList.m_WhereBetween_Cmd += " and (yzlx = '����' or yzlx = '����' or yzlx = '����' or yzlx = '����' or yzlx = '����' or yzlx = '��ȫ' or yzlx = '��������')";
				break;
			}
		}
		
	}

	if(theApp.m_StationType.CompareNoCase("�ؼ칤��վ") == 0)
	{
		if(theApp.m_CantainTsrs.CompareNoCase("��") == 0){
			switch(((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->GetCurSel()) {
			case 0:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '�����黯'";
				break;
			case 1:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '���Ӳ���'";
				break;
			case 2:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '�羵'";
				break;
			case 3:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '����'";
				break;
			case 4:
				m_AdviceList.m_WhereBetween_Cmd += " and (yzlx = '�����黯' or yzlx = '���Ӳ���' or yzlx = '�羵' or yzlx = '����')";
				break;
			}
		} 
		else 
		{
			switch(((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->GetCurSel()) {
			case 0:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '�����黯'";
				break;
			case 1:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '����Ⱦɫ'";
				break;
			case 2:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '���Ӳ���'";
				break;
			case 3:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '�羵'";
				break;
			case 4:
				m_AdviceList.m_WhereBetween_Cmd += " and yzlx = '����'";
				break;
			case 5:
				m_AdviceList.m_WhereBetween_Cmd += " and (yzlx = '�����黯' or yzlx = '����Ⱦɫ' or yzlx = '���Ӳ���' or yzlx = '�羵' or yzlx = '����')";
				break;
			}
		}
		
	}

	m_AdviceList.ResetGridSize();
	m_AdviceList.RedrawAll();
}

void CMedAdvice::OnCheckSelectall() 
{
	// TODO: Add your control notification handler code here
	if(((CButton *)GetDlgItem(IDC_CHECK_SELECTALL))->GetState() & 0x0003)
	{
		// Select all
		m_AdviceList.SelectRange(0, 0, m_AdviceList.GetNumberCols()-1,m_AdviceList.GetNumberRows()-1);
	}
	else
	{
		// Clear all selection
		m_AdviceList.ClearSelections();
	}
}

void CMedAdvice::OnButtonYzzh() 
{
	if(AfxMessageBox("ѡ�����ҽ��ִ�У��ù��̲����棬�Ƿ������",MB_OKCANCEL|MB_ICONQUESTION) == IDCANCEL)  return;

	CStringArray  MedicalNolist;
	CStringArray  XhNolist;
	CUIntArray  rowlist;
	int nCol, nRet;
	long nRow;
	CString sSlideIDs2Machine("");

	nRet = m_AdviceList.EnumFirstSelected( &nCol, &nRow );
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
				m_AdviceList.GetCellIndirect(0,nRow,&cell);
				cell.GetText(&str_MedicalNo);

				CString str_Yzh;
				m_AdviceList.GetCellIndirect(1,nRow,&cell);
				cell.GetText(&str_Yzh);
				
				rowlist.Add(nRow);
				MedicalNolist.Add(str_MedicalNo);
				XhNolist.Add(str_Yzh);
			}
		}
		
		nRet = m_AdviceList.EnumNextSelected( &nCol, &nRow );
	}

	int curnum = m_AdviceList.GetCurrentRow();
	for(int i = 0; i < rowlist.GetSize(); i++)
	{
		m_AdviceList.GotoRow(rowlist.GetAt(i));
	}
	m_AdviceList.GotoRow(curnum);

	rowlist.RemoveAll();

	if( MedicalNolist.GetSize() <= 0 )
	{
		return;
	}

	if(((CButton *)GetDlgItem(IDC_CHECK_ADDBJW))->IsWindowVisible() && 
		(((CButton *)GetDlgItem(IDC_CHECK_ADDBJW))->GetState() & 0x0003))
	{
		if(MedicalNolist.GetSize() != 2 || MedicalNolist.GetAt(0) != MedicalNolist.GetAt(1))
		{
			AfxMessageBox("�ϲ������ʧ�ܣ�");
			MedicalNolist.RemoveAll();
			XhNolist.RemoveAll();
			return;
		}
		CString yzlx, lkh, bjw;
		int qps = 0;
		bool yzzt;
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_ADVICE + " where medicalno = :medicalno and yzh = :yzh";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(0);
			g_dbcommand.Param("yzh").setAsString()       = XhNolist.GetAt(0);

			g_dbcommand.Execute();
			if(g_dbcommand.FetchNext())
			{
				yzlx  = g_dbcommand.Field("yzlx").asString();
				lkh   = g_dbcommand.Field("lkh").asString();
				yzzt  = g_dbcommand.Field("yzzt").asBool();
				qps   = g_dbcommand.Field("qps").asLong();
				bjw   = g_dbcommand.Field("bjw").asString();
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

		CString yzlx1, lkh1, bjw1;
		int qps1 = 0;
		bool yzzt1;
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_ADVICE + " where medicalno = :medicalno and yzh = :yzh";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(1);
			g_dbcommand.Param("yzh").setAsString()       = XhNolist.GetAt(1);

			g_dbcommand.Execute();
			if(g_dbcommand.FetchNext())
			{
				yzlx1  = g_dbcommand.Field("yzlx").asString();
				lkh1   = g_dbcommand.Field("lkh").asString();
				yzzt1  = g_dbcommand.Field("yzzt").asBool();
				qps1   = g_dbcommand.Field("qps").asLong();
				bjw1   = g_dbcommand.Field("bjw").asString();
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

		if( lkh == lkh1 && yzlx == yzlx1 && qps == 1 && qps1 == 1 && yzzt == false && yzzt1 == false)
		{
			SADateTime ServerTime = theApp.GetServerDateTime();
			CString    zxr = theApp.m_Cpp_User;
			SADateTime zxrq = ServerTime;

			for(i = 0; i < MedicalNolist.GetSize(); i++)
			{
				try
				{
					SAString cmdstr = "Update " + theApp.TABLE_ADVICE + " set yzzt = 1, zxr = :zxr, zxrq = :zxrq where medicalno = :medicalno and yzh = :yzh and yzzt = 0";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(i);
					g_dbcommand.Param("yzh").setAsString()       = XhNolist.GetAt(i);
					g_dbcommand.Param("zxr").setAsString()       = theApp.m_Cpp_User;  
					g_dbcommand.Param("zxrq").setAsDateTime()    = ServerTime;
					
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

			CString NewSliceNo = CMedPreslice::GenNewSliceNo(MedicalNolist.GetAt(0));
			try
			{
				SAString cmdstr = "Insert into " + theApp.TABLE_SLICE + " (medicalno,rwly,lkh,qpsm,qptmh,qpczy,qprq,zpzt,dyzt) values(:medicalno,:rwly,:lkh,:qpsm,:qptmh,:qpczy,:qprq,3,0)";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(0);
				g_dbcommand.Param("rwly").setAsString() = yzlx;
				g_dbcommand.Param("lkh").setAsString()  = lkh;
				
				g_dbcommand.Param("qpsm").setAsString() = bjw + "," + bjw1;
				
				g_dbcommand.Param("qptmh").setAsString()  = NewSliceNo;
				g_dbcommand.Param("qpczy").setAsString()  = zxr;
				g_dbcommand.Param("qprq").setAsDateTime() = zxrq;
								
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

			m_AdviceList.ResetGridSize();
			m_AdviceList.RedrawAll();
		}
		else
		{
			AfxMessageBox("�ϲ������ʧ�ܣ�");
		}

		MedicalNolist.RemoveAll();
		XhNolist.RemoveAll();
		return;
	}

	for(int index = 0; index < MedicalNolist.GetSize(); index++)
	{
		CString yzlx, lkh, bjw, zxr;
	    SADateTime zxrq;
		int qps = 0;
		bool yzzt;
		
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_ADVICE + " where medicalno = :medicalno and yzh = :yzh";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
			g_dbcommand.Param("yzh").setAsString()       = XhNolist.GetAt(index);

			g_dbcommand.Execute();
			if(g_dbcommand.FetchNext())
			{
				yzlx  = g_dbcommand.Field("yzlx").asString();
				lkh   = g_dbcommand.Field("lkh").asString();
				yzzt  = g_dbcommand.Field("yzzt").asBool();
				qps   = g_dbcommand.Field("qps").asLong();
				bjw   = g_dbcommand.Field("bjw").asString();
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

		if(yzzt == false)
		{
			SADateTime ServerTime = theApp.GetServerDateTime();
			try
			{
				SAString cmdstr = "Update " + theApp.TABLE_ADVICE + " set yzzt = 1, zxr = :zxr, zxrq = :zxrq where medicalno = :medicalno and yzh = :yzh and yzzt = 0";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
				g_dbcommand.Param("yzh").setAsString()       = XhNolist.GetAt(index);
				g_dbcommand.Param("zxr").setAsString()       = theApp.m_Cpp_User;  
				
				zxr = theApp.m_Cpp_User;
				zxrq = ServerTime;

				g_dbcommand.Param("zxrq").setAsDateTime() = zxrq;
				
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

			for(int i = 0; i < qps; i++)
			{
				CString NewSliceNo = CMedPreslice::GenNewSliceNo(MedicalNolist.GetAt(index));
				try
				{
					SAString cmdstr = "Insert into " + theApp.TABLE_SLICE + " (medicalno,rwly,lkh,qpsm,qptmh,qpczy,qprq,zpzt,dyzt) values(:medicalno,:rwly,:lkh,:qpsm,:qptmh,:qpczy,:qprq,3,0)";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
					g_dbcommand.Param("rwly").setAsString() = yzlx;
					g_dbcommand.Param("lkh").setAsString()  = lkh;

					if(yzlx.CompareNoCase("����") == 0)
					{
						CString str;
						str.Format("����-%d", i+1);
						g_dbcommand.Param("qpsm").setAsString() = str;
					}
					else
					{
						if(yzlx.CompareNoCase("����") == 0)
						{
							CString str;
							str.Format("����-%d", i+1);
							g_dbcommand.Param("qpsm").setAsString() = str;
						}
						else
						{
							if(yzlx.CompareNoCase("����") == 0)
							{
								CString str;
								str.Format("����-%d", i+1);
								g_dbcommand.Param("qpsm").setAsString() = str;
							}
							else
							{
								if(yzlx.CompareNoCase("����") == 0)
								{
									CString str;
									str.Format("����-%d", i+1);
									g_dbcommand.Param("qpsm").setAsString() = str;
								}
								else
								{
									if(yzlx.CompareNoCase("����") == 0)
									{
										CString str;
										str.Format("����-%d", i+1);
										g_dbcommand.Param("qpsm").setAsString() = str;
									}
									else
									{
										if(yzlx.CompareNoCase("�����黯") == 0)
										{
											g_dbcommand.Param("qpsm").setAsString() = bjw;
											
											if (!sSlideIDs2Machine.IsEmpty())
												sSlideIDs2Machine = sSlideIDs2Machine + ";";

											sSlideIDs2Machine = sSlideIDs2Machine + NewSliceNo;
										}
										else
										{
											//g_dbcommand.Param("qpsm").setAsString() = bjw;
											if(yzlx.CompareNoCase("��ȫ") == 0)
											{
												CString str;
												str.Format("��ȫ-%d", i+1);
												g_dbcommand.Param("qpsm").setAsString() = str;
											}
											else
											{
												if(yzlx.CompareNoCase("��������") == 0)
												{
													CString str;
													str.Format("��������-%d", i+1);
													g_dbcommand.Param("qpsm").setAsString() = str;
												}
												else
												{
													g_dbcommand.Param("qpsm").setAsString() = bjw;
												}
											}
										}
									}
								}
							}
						}
					}

					g_dbcommand.Param("qptmh").setAsString() = NewSliceNo;
					g_dbcommand.Param("qpczy").setAsString() = zxr;
					g_dbcommand.Param("qprq").setAsDateTime() = zxrq;
					
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
	}

	MedicalNolist.RemoveAll();
	XhNolist.RemoveAll();

	m_AdviceList.ResetGridSize();
	m_AdviceList.RedrawAll();

	// ���Ͳ�Ƭ���ݵ�Ⱦɫ���豸

	if (!sSlideIDs2Machine.IsEmpty()) {
		theApp.SendSlideIDs2StainingMachine(sSlideIDs2Machine);
	}
}

void CMedAdvice::OnButtonTranstable() 
{
	CGzddy mydlg(this);
	mydlg.DoModal();
}

BOOL CMedAdvice::PreTranslateMessage(MSG* pMsg) 
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

void CMedAdvice::OnButtonYzqr() 
{
	if(AfxMessageBox("ѡ������շ�ȷ�ϣ��ù��̲����棬�Ƿ������",MB_OKCANCEL|MB_ICONQUESTION) == IDCANCEL)  return;

	CStringArray  MedicalNolist;
	CStringArray  XhNolist;
	CUIntArray  rowlist;
	int nCol, nRet;
	long nRow;

	nRet = m_AdviceList.EnumFirstSelected( &nCol, &nRow );
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
				m_AdviceList.GetCellIndirect(0,nRow,&cell);
				cell.GetText(&str_MedicalNo);

				CString str_Yzh;
				m_AdviceList.GetCellIndirect(1,nRow,&cell);
				cell.GetText(&str_Yzh);
				
				rowlist.Add(nRow);
				MedicalNolist.Add(str_MedicalNo);
				XhNolist.Add(str_Yzh);
			}
		}
		
		nRet = m_AdviceList.EnumNextSelected( &nCol, &nRow );
	}

	int curnum = m_AdviceList.GetCurrentRow();
	for(int i = 0; i < rowlist.GetSize(); i++)
	{
		m_AdviceList.GotoRow(rowlist.GetAt(i));
	}
	m_AdviceList.GotoRow(curnum);

	rowlist.RemoveAll();

	if( MedicalNolist.GetSize() <= 0 )
	{
		return;
	}

	for(int index = 0; index < MedicalNolist.GetSize(); index++)
	{
		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_ADVICE + " set fxjg = '���շ�' where medicalno = :medicalno and yzh = :yzh";
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

	m_AdviceList.ResetGridSize();
	m_AdviceList.RedrawAll();
}

void CMedAdvice::OnButtonDefaultquery() 
{
	if(theApp.m_StationType.CompareNoCase("��Ƭ����վ") == 0)
	{
		m_AdviceList.m_WhereBetween_Cmd.Format(" where yzzt = 0");
	}

	if(theApp.m_StationType.CompareNoCase("������Ƭ����վ") == 0)
	{
		if(theApp.m_CantainTsrs.CompareNoCase("��") == 0){
			m_AdviceList.m_WhereBetween_Cmd.Format(" where yzzt = 0 and (yzlx = '����' or yzlx = '����Ⱦɫ' or yzlx = '����' or yzlx = '����' or yzlx = '����' or yzlx = '����' or yzlx = '��ȫ' or yzlx = '��������')");
		} else {
			m_AdviceList.m_WhereBetween_Cmd.Format(" where yzzt = 0 and (yzlx = '����' or yzlx = '����' or yzlx = '����' or yzlx = '����' or yzlx = '����' )");
		}
		
	}

	if(theApp.m_StationType.CompareNoCase("�ؼ칤��վ") == 0)
	{
		if(theApp.m_TjyzMode.CompareNoCase("��") == 0){
			if(theApp.m_CantainTsrs.CompareNoCase("��") == 0){
				m_AdviceList.m_WhereBetween_Cmd.Format(" where fxjg = '���շ�' and yzzt = 0 and (yzlx = '�����黯' or yzlx = '����Ⱦɫ' or yzlx = '���Ӳ���' or yzlx = '�羵')");
			} else {
				m_AdviceList.m_WhereBetween_Cmd.Format(" where fxjg = '���շ�' and yzzt = 0 and (yzlx = '�����黯' or yzlx = '���Ӳ���' or yzlx = '�羵')");
			}
			
		} else {
			if(theApp.m_CantainTsrs.CompareNoCase("��") == 0){
				m_AdviceList.m_WhereBetween_Cmd.Format(" where yzzt = 0 and (yzlx = '�����黯' or yzlx = '����Ⱦɫ' or yzlx = '���Ӳ���' or yzlx = '�羵')");
			} else {
				m_AdviceList.m_WhereBetween_Cmd.Format(" where yzzt = 0 and (yzlx = '�����黯' or yzlx = '���Ӳ���' or yzlx = '�羵')");
			}
			//m_AdviceList.m_WhereBetween_Cmd.Format(" where yzzt = 0 and (yzlx = '�����黯' or yzlx = '����Ⱦɫ' or yzlx = '���Ӳ���' or yzlx = '�羵')");
		}
			
	}

	m_AdviceList.ResetGridSize();
	m_AdviceList.RedrawAll();
}

void CMedAdvice::PerformQuery()
{
	OnButtonQuery();
	
	// Select all
	m_AdviceList.SelectRange(0, 0, m_AdviceList.GetNumberCols()-1,m_AdviceList.GetNumberRows()-1);
}

//DEL void CMedAdvice::OnBatchupadate() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	int col; long row,row_old;
//DEL 	int result;
//DEL 	CString qptmh; qptmh="where qptmh in(";
//DEL 	result = m_AdviceList.EnumFirstSelected(&col,&row);
//DEL 	
//DEL 	while(result == UG_SUCCESS){
//DEL 		if(qptmh.CompareNoCase("where qptmh in(")){
//DEL 			qptmh += ",";
//DEL 		}
//DEL 		CString temp = m_AdviceList.QuickGetText(4,row);
//DEL 		if(!temp.IsEmpty()){
//DEL 			qptmh += "'";
//DEL 			qptmh += temp;
//DEL 			qptmh += "'";
//DEL 		}
//DEL 		
//DEL 		result = m_AdviceList.EnumNextSelected(&col,&row);
//DEL 	}
//DEL 	qptmh += ")";
//DEL 	
//DEL 	
//DEL 	
//DEL }

void CMedAdvice::OnButtonAdvicedel() //��ɾ�� Ȼ��������
{
	// TODO: Add your control notification handler code here
	if(AfxMessageBox("��ȷ��ɾ����ѡ���ļ�¼��ɾ���󲻿ɻָ���",MB_OKCANCEL) == IDCANCEL)
	{
		return;
	}
	CString reason;
	CAdviceDelReason dlg;
	int result = dlg.DoModal();
	if(result == IDOK)
	{
		reason = dlg.m_reason;
	}
	else
	{
		return;
	}
	CString mno; CStringArray noList,yzlxList,lkhList,bjwList,sqysList,yzhList;
	long row,old_row; int col,ret;
	ret = m_AdviceList.EnumFirstSelected(&col,&row);
	noList.Add(m_AdviceList.QuickGetText(0,row));
	yzhList.Add(m_AdviceList.QuickGetText(1,row));
	yzlxList.Add(m_AdviceList.QuickGetText(2,row));
	lkhList.Add(m_AdviceList.QuickGetText(3,row));
	bjwList.Add(m_AdviceList.QuickGetText(5,row));
	sqysList.Add(m_AdviceList.QuickGetText(6,row));
	old_row = row;
	while(ret == UG_SUCCESS)
	{
		if(old_row < row)
		{
			noList.Add(m_AdviceList.QuickGetText(0,row));
			yzhList.Add(m_AdviceList.QuickGetText(1,row));
			yzlxList.Add(m_AdviceList.QuickGetText(2,row));
			lkhList.Add(m_AdviceList.QuickGetText(3,row));
			bjwList.Add(m_AdviceList.QuickGetText(5,row));
			sqysList.Add(m_AdviceList.QuickGetText(6,row));
			old_row = row;
		}
		ret = m_AdviceList.EnumNextSelected(&col,&row);			
	}

	CString batchDel;
	batchDel.Empty();
	batchDel += "(";
	for(int i = 0; i < yzhList.GetSize();i++)
	{
		if(i > 0)
		{
			batchDel += ",";
		}
		batchDel += "'";
		batchDel += yzhList.GetAt(i);
		batchDel += "'";
	}
	batchDel += ")";
	
	try
	{
		SAString cmdstr = "delete from " + theApp.TABLE_ADVICE + " where yzh in" + batchDel;
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Execute();
		g_dbconnection.Commit();
		
	}
	catch (SAException &e)
	{
		AfxMessageBox((const char*)e.ErrText());
	}

	m_AdviceList.ResetGridSize();
	m_AdviceList.RedrawAll();

	for(i = 0; i < noList.GetSize(); i++)
	{
		CString sx;
		sx.Format("��ɾ����������š�%s;��ҽ�����͡�%s;������š�%s;������%s��ҽ����¼��\r\nɾ��ԭ��%s",noList.GetAt(i),yzlxList.GetAt(i),lkhList.GetAt(i),bjwList.GetAt(i),reason);
		timeb time;
		ftime(&time);
		SADateTime currentTime(time);
		try
		{
			SAString cmdstr = "insert into "+ theApp.TABLE_NOTEPAD + " values(:fbsj,:fbr,:sx,:jsr,0)";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("fbsj").setAsDateTime() = currentTime;
			g_dbcommand.Param("fbr").setAsString() = theApp.m_Cpp_User;
			g_dbcommand.Param("sx").setAsString() = sx;
			g_dbcommand.Param("jsr").setAsString() = sqysList.GetAt(i);
			g_dbcommand.Execute();
			g_dbconnection.Commit();
		}
		catch (SAException &e)
		{
			AfxMessageBox((const char*)e.ErrText());
		}
	}

	noList.RemoveAll();
	yzlxList.RemoveAll();
	lkhList.RemoveAll();
	bjwList.RemoveAll();
	sqysList.RemoveAll();
}
