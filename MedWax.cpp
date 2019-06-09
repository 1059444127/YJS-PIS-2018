// CMedWax : implementation file
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "Pathology.h"
#include "PathologyView.h"
#include "PathologyDoc.h"
#include "MedWax.h"
#include "Ckyj.h"
#include "MedMaterial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMedWax
extern CPathologyApp theApp;

CMedWax::CMedWax()
: CXTResizeFormView(CMedWax::IDD)
{
	//{{AFX_DATA_INIT(CMedWax)
	//}}AFX_DATA_INIT

	m_bIsFirstInit = true;
}

CMedWax::~CMedWax()
{
	// TODO: add destruction code here.

}

IMPLEMENT_DYNCREATE(CMedWax, CXTResizeFormView)

BEGIN_MESSAGE_MAP(CMedWax, CXTResizeFormView)
	//{{AFX_MSG_MAP(CMedWax)
	ON_BN_CLICKED(IDC_CHECK_SELECTALL, OnCheckSelectall)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnButtonQuery)
	ON_BN_CLICKED(IDC_BUTTON_CKHD, OnButtonCkhd)
	ON_BN_CLICKED(IDC_BUTTON_BMQR, OnButtonBmqr)
	ON_BN_CLICKED(IDC_BUTTON_TRANSTABLE, OnButtonTranstable)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULTQUERY, OnButtonDefaultquery)
	ON_BN_CLICKED(IDC_BUTTON_BDQUERY, OnButtonBdquery)
	ON_BN_CLICKED(IDC_BUTTON_BQDY, OnButtonBqdy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMedWax message handlers

void CMedWax::DoDataExchange(CDataExchange* pDX) 
{
	CXTResizeFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMedWax)
	DDX_Control(pDX, IDC_HYP_NOTE, m_HypLink_Note);
	//}}AFX_DATA_MAP
}

void CMedWax::OnInitialUpdate() 
{
	CXTResizeFormView::OnInitialUpdate();
	
	if (m_bIsFirstInit)
	{
		m_bIsFirstInit = false;

		CRect rt;
		GetDlgItem(IDC_DATE_QCRQ)->GetWindowRect(&rt);
		ScreenToClient(&rt);
		if (!m_Time_Qcrq.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT, //,DTS_LONGDATEFORMAT
			rt, this, 112))
		{
			return;
		}

		m_Time_Qcrq.SetTime(theApp.GetOleServerDateTime());

		m_WaxList.AttachGrid(this,IDC_STATIC_BMXX);
		m_WaxList.SetParent(this);
		m_WaxList.ResetGridSize();
		m_WaxList.RedrawAll();

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

		SetDlgItemText(IDC_EDIT_TIMESPAN, "0");
		return;
	}	

	m_WaxList.ResetGridSize();
	m_WaxList.RedrawAll();

}

void CMedWax::OnCheckSelectall() 
{
	// TODO: Add your control notification handler code here
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

void CMedWax::Refresh()
{
	m_WaxList.ResetGridSize();
	m_WaxList.RedrawAll();
}

void ExplainString(CString inputstr, CStringArray& StrList, CStringArray& UdList)
{
	//112、2323、232～222
	inputstr.Replace("，","、");
	inputstr.Replace(",","、");
	inputstr.Replace("~","～");

	StrList.RemoveAll();
	UdList.RemoveAll();
    while(inputstr.Find("、") >= 0)
	{
		CString substr = inputstr.Left(inputstr.Find("、"));
		inputstr = inputstr.Right(inputstr.GetLength() - inputstr.Find("、") - 2);
		substr.TrimLeft();   substr.TrimRight();
		inputstr.TrimLeft(); inputstr.TrimRight();

		if(substr.Find("～") >= 0)
		{
			CString upstr    =  substr.Left(substr.Find("～"));;
			CString downstr  =  substr.Right(substr.GetLength() - substr.Find("～") - 2);
			upstr.TrimLeft();   upstr.TrimRight();
			downstr.TrimLeft(); downstr.TrimRight();

			UdList.Add(upstr);
			UdList.Add(downstr);
		}
		else
		{
			StrList.Add(substr);
		}
	}
    //---------------------------------------------
	CString substr = inputstr;
	substr.TrimLeft();   substr.TrimRight();
	
	if(substr.Find("～") >= 0)
	{
		CString upstr    =  substr.Left(substr.Find("～"));;
		CString downstr  =  substr.Right(substr.GetLength() - substr.Find("～") - 2);
		upstr.TrimLeft();   upstr.TrimRight();
		downstr.TrimLeft(); downstr.TrimRight();
		
		UdList.Add(upstr);
		UdList.Add(downstr);
	}
	else
	{
		StrList.Add(substr);
	}
    //---------------------------------------------
}

void CMedWax::OnButtonQuery() 
{
	// TODO: Add your control notification handler code here
	CString mno; mno.Empty();
	GetDlgItemText(IDC_EDIT_BLH, mno);
	mno.TrimLeft(); mno.TrimRight();
	SetDlgItemText(IDC_EDIT_BLH, "");

	CString qcys; qcys.Empty();
	GetDlgItemText(IDC_COMBO_QCYS, qcys);
	qcys.TrimLeft(); qcys.TrimRight();
	SetDlgItemText(IDC_COMBO_QCYS, "");

	CString group;
	GetDlgItemText(IDC_COMBO_GROUP,group);
	group.TrimLeft();group.TrimRight();
	SetDlgItemText(IDC_COMBO_GROUP,"");

	if(mno.IsEmpty())
	{
		//  取材日期查询
		COleDateTime m_time;
		m_Time_Qcrq.GetTime(m_time);

		CString start = m_time.Format("%Y-%m-%d 00:00:00");
		CString end   = m_time.Format("%Y-%m-%d 23:59:59");

		int isCheck = ((CButton*)GetDlgItem(IDC_CHECK_NOTPRINT))->GetCheck();

		

		if(group.IsEmpty())
		{
			if(qcys.IsEmpty())
				m_WaxList.m_WhereBetween_Cmd.Format(" where a.qcrq >= '%s' and a.qcrq <= '%s'", start, end);
			else
				m_WaxList.m_WhereBetween_Cmd.Format(" where a.qcrq >= '%s' and a.qcrq <= '%s' and a.qcys = '%s'", start, end, qcys);
			
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				if(qcys.IsEmpty())
					m_WaxList.m_WhereBetween_Cmd.Format(" where a.qcrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and a.qcrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", start, end);
				else
					m_WaxList.m_WhereBetween_Cmd.Format(" where a.qcrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and a.qcrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss') and qcys = '%s'", start, end, qcys);
			}
		}
		else
		{
			if(qcys.IsEmpty())
				m_WaxList.m_WhereBetween_Cmd.Format(" where a.qcrq >= '%s' and a.qcrq <= '%s' and a.medicalno like '%%%s'", start, end, group);
			else
				m_WaxList.m_WhereBetween_Cmd.Format(" where a.qcrq >= '%s' and a.qcrq <= '%s' and a.qcys = '%s' and a.medicalno like '%%%s'", start, end, qcys, group);

			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				if(qcys.IsEmpty())
					m_WaxList.m_WhereBetween_Cmd.Format(" where a.qcrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and a.qcrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss') and a.medicalno like '%%%s'", start, end, group);
				else
					m_WaxList.m_WhereBetween_Cmd.Format(" where a.qcrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and a.qcrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss') and a.qcys = '%s' and a.medicalno like '%%%s'", start, end, qcys,group);
			}
		}
		if(isCheck == 1)
		{
			if(qcys.IsEmpty())
				m_WaxList.m_WhereBetween_Cmd.Format(" where a.qcrq >= '%s' and a.qcrq <= '%s' and a.dyzt = 0", start, end);
			else
				m_WaxList.m_WhereBetween_Cmd.Format(" where a.qcrq >= '%s' and a.qcrq <= '%s' and a.qcys = '%s' and a.dyzt = 0", start, end, qcys);
			
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				if(qcys.IsEmpty())
					m_WaxList.m_WhereBetween_Cmd.Format(" where a.qcrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and a.qcrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss') and a.dyzt = 0", start, end);
				else
					m_WaxList.m_WhereBetween_Cmd.Format(" where a.qcrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and a.qcrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss') and a.qcys = '%s' and a.dyzt = 0", start, end, qcys);
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
			if(qcys.IsEmpty())
				m_WaxList.m_WhereBetween_Cmd.Format(" where a.medicalno = '%s'", mno);
			else
				m_WaxList.m_WhereBetween_Cmd.Format(" where a.medicalno = '%s' and a.qcys = '%s'", mno, qcys);
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
				tmp.Format("(a.medicalno >= '%s' and a.medicalno <= '%s')", UdList.GetAt(i), UdList.GetAt(i+1));
				mstr += tmp;
			}

			if(qcys.IsEmpty())
				m_WaxList.m_WhereBetween_Cmd.Format(" where (%s)", mstr);
			else
				m_WaxList.m_WhereBetween_Cmd.Format(" where (%s) and a.qcys = '%s'", mstr, qcys);
		}

		StrList.RemoveAll();
		UdList.RemoveAll();
	}

	m_WaxList.ResetGridSize();
	m_WaxList.RedrawAll();
}

void CMedWax::OnButtonCkhd() 
{
	if(AfxMessageBox("选择进行材块核对，该过程不可逆，是否继续？",MB_OKCANCEL|MB_ICONQUESTION) == IDCANCEL)  return;

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

				CString str_Xh;
				m_WaxList.GetCellIndirect(2,nRow,&cell);
				cell.GetText(&str_Xh);
				
				rowlist.Add(nRow);
				MedicalNolist.Add(str_MedicalNo);
				XhNolist.Add(str_Xh);
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

	

	SADateTime ServerTime = theApp.GetServerDateTime();

	for(int index = 0; index < MedicalNolist.GetSize(); index++)
	{
		try
		{
			CString lkh,qpsm,qptmh,rwly,qcbw; lkh.Empty();qpsm.Empty();qptmh.Empty();rwly.Empty();qcbw.Empty();

			SAString cmdstr = "select lkh,qpsm,qptmh,rwly,qcbw from " + theApp.TABLE_MATERIAL + " where medicalno=:medicalno and xh=:xh";
			if(theApp.m_DBType.CompareNoCase("oracle") == 0 && XhNolist.GetAt(index).IsEmpty())
			{
				cmdstr = "select lkh,qpsm,qptmh,rwly,qcbw from " + theApp.TABLE_MATERIAL + " where medicalno=:medicalno and xh is null";
			}

			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
			
			if(!(theApp.m_DBType.CompareNoCase("oracle") == 0 && XhNolist.GetAt(index).IsEmpty()))
			{
				g_dbcommand.Param("xh").setAsString() = XhNolist.GetAt(index);
			}
			
			g_dbcommand.Execute();
			while(g_dbcommand.FetchNext())
			{
				lkh = g_dbcommand.Field("lkh").asString();
				qpsm = g_dbcommand.Field("qpsm").asString();
				qptmh = g_dbcommand.Field("qptmh").asString();
				rwly = g_dbcommand.Field("rwly").asString();
				qcbw = g_dbcommand.Field("qcbw").asString();
			}
			g_dbconnection.Commit();
			
			if(lkh.IsEmpty() && qpsm.IsEmpty() && qptmh.IsEmpty())
			{
				CString qpsm;
				if(rwly.CompareNoCase("冰冻") == 0 || rwly.CompareNoCase("脱钙") == 0 || rwly.CompareNoCase("冰石") == 0 || rwly.CompareNoCase("冰余") == 0 || rwly.CompareNoCase("补取") == 0)
				{
					if(rwly.CompareNoCase("冰冻") == 0 && theApp.m_BdSliceMode.CompareNoCase("冰冻"))
					{
						CString nBdh;
						theApp.GetDatabaseValue(theApp.GetView()->GetDocument()->m_blh, "bdh", nBdh, true);
						//g_dbcommand.Param("qpsm").setAsString() = "冰" + nBdh;
						CString temp = "冰" + nBdh;
						qpsm = temp;
					} 
					else
					{
						//g_dbcommand.Param("qpsm").setAsString() = rwly;
						qpsm = rwly;
					}
				}
				else
				{
					if(rwly.CompareNoCase("常规") == 0)
					{
						CString qmsmstr;  qmsmstr.Empty();
						if(theApp.m_NormalSliceMode.CompareNoCase("取材部位") == 0)
						{
							qmsmstr = qcbw;
						}
						if(theApp.m_NormalSliceMode.CompareNoCase("HE") == 0)
						{
							qmsmstr = "HE";
						}
						
						//g_dbcommand.Param("qpsm").setAsString() = qmsmstr;
						qpsm = qmsmstr;
					}
				}
				cmdstr = "Update " + theApp.TABLE_MATERIAL + " set zpzt = 1, ckhd ='"+ theApp.m_Cpp_User + "', hdrq=:hdrq, lkh=:lkh, qpsm='" + qpsm + "', qptmh='" + CMedMaterial::GenNewSliceNo(MedicalNolist.GetAt(index)) + "' where medicalno=:medicalno and xh=:xh and zpzt=0";
				if(theApp.m_DBType.CompareNoCase("oracle") == 0 && XhNolist.GetAt(index).IsEmpty())
				{
					cmdstr = "Update " + theApp.TABLE_MATERIAL + " set zpzt = 1, ckhd ='"+ theApp.m_Cpp_User + "', hdrq=:hdrq, lkh=:lkh, qpsm='" + qpsm + "', qptmh='" + CMedMaterial::GenNewSliceNo(MedicalNolist.GetAt(index)) + "' where medicalno=:medicalno and xh is null and zpzt=0";
				}
			}
			else
			{
				cmdstr = "Update " + theApp.TABLE_MATERIAL + " set zpzt = 1, ckhd ='"+ theApp.m_Cpp_User + "', hdrq = :hdrq where medicalno = :medicalno and xh = :xh and zpzt = 0";
				if(theApp.m_DBType.CompareNoCase("oracle") == 0 && XhNolist.GetAt(index).IsEmpty())
				{
					cmdstr = "Update " + theApp.TABLE_MATERIAL + " set zpzt = 1, ckhd ='"+ theApp.m_Cpp_User + "', hdrq = :hdrq where medicalno = :medicalno and xh is null and zpzt = 0";
				}
			}
		
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
			
			if(!(theApp.m_DBType.CompareNoCase("oracle") == 0 && XhNolist.GetAt(index).IsEmpty()))
			{
				g_dbcommand.Param("xh").setAsString() = XhNolist.GetAt(index);
			}
		
			//g_dbcommand.Param("ckhd").setAsString()      = theApp.m_Cpp_User;  
			
			SADateTime st = ServerTime;
			g_dbcommand.Param("hdrq").setAsDateTime() = st;
			
			if(lkh.IsEmpty() && qpsm.IsEmpty() && qptmh.IsEmpty())
			{
				if(XhNolist.GetAt(index).IsEmpty())
				{
					g_dbcommand.Param("lkh").setAsString() = MedicalNolist.GetAt(index);
				}
				else
				{
					g_dbcommand.Param("lkh").setAsString() = MedicalNolist.GetAt(index) + "-" + XhNolist.GetAt(index);
				}
				
			//	g_dbcommand.Param("qpsm").setAsString() = qpsm;
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
			return;
		}
	}

	MedicalNolist.RemoveAll();
	XhNolist.RemoveAll();

	m_WaxList.ResetGridSize();
	m_WaxList.RedrawAll();
}

void CMedWax::OnButtonBmqr() 
{
	if(AfxMessageBox("选择进行包埋确认，该过程不可逆，是否继续？",MB_OKCANCEL|MB_ICONQUESTION) == IDCANCEL)  return;

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

				CString str_Xh;
				m_WaxList.GetCellIndirect(2,nRow,&cell);
				cell.GetText(&str_Xh);
				
				rowlist.Add(nRow);
				MedicalNolist.Add(str_MedicalNo);
				XhNolist.Add(str_Xh);
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
	
	COleDateTime ServerTime = theApp.GetOleServerDateTime();
	COleDateTimeSpan ts;
	CString tstr;
	GetDlgItemText(IDC_EDIT_TIMESPAN, tstr);
	ts.SetDateTimeSpan(atoi(tstr), 0, 0, 0);
	ServerTime += ts;

	for(int index = 0; index < MedicalNolist.GetSize(); index++)
	{
		try
		{	//过渡用
			CString lkh;lkh.Empty();
			SAString cmdstr = "select lkh from " + theApp.TABLE_MATERIAL + " where medicalno=:medicalno and xh=:xh";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
			g_dbcommand.Param("xh").setAsString() = XhNolist.GetAt(index);
			g_dbcommand.Execute();
			while(g_dbcommand.FetchNext())
			{
				lkh = g_dbcommand.Field("lkh").asString();
			}
			g_dbconnection.Commit();

			cmdstr = "Update " + theApp.TABLE_MATERIAL + " set zpzt = 2, bmczy = :bmczy, bmrq = :bmrq, lkh=:lkh, qps = 1 where medicalno = :medicalno and xh = :xh and zpzt = 1";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
			g_dbcommand.Param("xh").setAsString()        = XhNolist.GetAt(index);
			g_dbcommand.Param("bmczy").setAsString()      = theApp.m_Cpp_User;  
			
			SADateTime st = ServerTime;
			g_dbcommand.Param("bmrq").setAsDateTime() = st;
			
			if(lkh.IsEmpty())
			{
				if(XhNolist.GetAt(index).IsEmpty())
					g_dbcommand.Param("lkh").setAsString() = MedicalNolist.GetAt(index);
				else
					g_dbcommand.Param("lkh").setAsString() = MedicalNolist.GetAt(index) + "-" +XhNolist.GetAt(index);
			}
			else
			{
				g_dbcommand.Param("lkh").setAsString() = lkh;
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
			return;
		}
	}

	MedicalNolist.RemoveAll();
	XhNolist.RemoveAll();

	m_WaxList.ResetGridSize();
	m_WaxList.RedrawAll();
}

void CMedWax::OnButtonTranstable() 
{
	CCkyj mydlg(this);
	mydlg.DoModal();
}

BOOL CMedWax::PreTranslateMessage(MSG* pMsg) 
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

void CMedWax::OnButtonDefaultquery() 
{
	m_WaxList.m_WhereBetween_Cmd.Empty();
	m_WaxList.ResetGridSize();
	m_WaxList.RedrawAll();
}

void CMedWax::OnButtonBdquery() 
{
	m_WaxList.m_WhereBetween_Cmd.Format(" where rwly = '冰冻'");
	m_WaxList.ResetGridSize();
	m_WaxList.RedrawAll();
}

void CMedWax::PerformQuery()
{
	OnButtonQuery();
	
	// Select all
	m_WaxList.SelectRange(0, 0, m_WaxList.GetNumberCols()-1,m_WaxList.GetNumberRows()-1);
}

void CMedWax::OnButtonBqdy() 
{
	CUIntArray  rowlist;
	int nCol, nRet;
	long nRow;
	bool bSlidePrintValid = false;
	CWaitCursor wait;

	theApp.m_QptmhList.RemoveAll();
	theApp.m_QpsmList.RemoveAll();
	theApp.m_LkhList.RemoveAll();

	// 检查玻片打印是否有效
	bSlidePrintValid = (theApp.m_IsSlidePrintValid == "是");

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
				rowlist.Add(nRow);
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

	if( rowlist.GetSize() <= 0 )
	{
		return;
	}
	CStringArray mnoList,xhList; 
	mnoList.RemoveAll(); xhList.RemoveAll();
	for(i = 0; i < rowlist.GetSize(); i++)
	{
		CString str;
		CUGCell  cell;
		CString medicalNo(""), serialNo(""), slideNo(""), memo(""), barcodeNo(""), mark("");
		int nPos;

		m_WaxList.GetCellIndirect(0,rowlist.GetAt(i),&cell);
		cell.GetText(&str);
		mnoList.Add(str);
//		theApp.m_LkhList.Add(str);
		
		m_WaxList.GetCellIndirect(2,rowlist.GetAt(i),&cell);
		cell.GetText(&str);
		xhList.Add(str);
//		theApp.m_QpsmList.Add(str);
		
//		m_WaxList.GetCellIndirect(4,rowlist.GetAt(i),&cell);
//		cell.GetText(&str);
//		theApp.m_QptmhList.Add(str);
		// 玻片打印文件
		m_WaxList.GetCellIndirect(0,rowlist.GetAt(i),&cell);
		cell.GetText(&medicalNo);

		m_WaxList.GetCellIndirect(1,rowlist.GetAt(i),&cell);
		cell.GetText(&mark);

		m_WaxList.GetCellIndirect(2,rowlist.GetAt(i),&cell);
		cell.GetText(&serialNo);

		memo = "";
		barcodeNo = "";
		slideNo = medicalNo + "-" + serialNo;

		if (bSlidePrintValid) {
			theApp.MakeSlidePrintFile(medicalNo, serialNo, slideNo, memo, barcodeNo, mark);
			// 延迟200毫秒
			DWORD dwTickCount = GetTickCount();
			while ((GetTickCount() - dwTickCount) < 200)
				Sleep(10);
		}

	}
	rowlist.RemoveAll();
	
	for(i = 0; i < mnoList.GetSize();i++)
	{
		try
		{
			SAString cmdstr = "select lkh,qptmh,qpsm,zpzt from " + theApp.TABLE_MATERIAL + " where medicalno=:medicalno and xh=:xh";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = mnoList.GetAt(i);
			g_dbcommand.Param("xh").setAsString() = xhList.GetAt(i);
			g_dbcommand.Execute();

			while(g_dbcommand.FetchNext())
			{
				if(g_dbcommand.Field("zpzt").asLong() >= 1)
				{				
					theApp.m_LkhList.Add(g_dbcommand.Field("lkh").asString());
					theApp.m_QptmhList.Add(g_dbcommand.Field("qptmh").asString());
					theApp.m_QpsmList.Add(g_dbcommand.Field("qpsm").asString());
				}
				else
				{
					AfxMessageBox("病理号为" + mnoList.GetAt(i) + ",序号为" + xhList.GetAt(i) + "的记录没有材块核对，不能打印!");
				}
			}
		}
		catch (SAException& e)
		{
			AfxMessageBox((const char*)e.ErrText());
		}
	}
	
	if(theApp.m_LkhList.GetSize() <= 0 )
	{
		return;
	}

	if (!bSlidePrintValid) {
		theApp.m_PrintType = 1;
		theApp.GetView()->SendMessage(WM_COMMAND, ID_FILE_PRINT);
	}
	
	if ((!bSlidePrintValid && theApp.GetView()->m_OnPreparePrinting_Return) || (bSlidePrintValid))
	{
		for(i = 0; i < theApp.m_QptmhList.GetSize(); i++)
		{
			try
			{
				SAString cmdstr = "Update " + theApp.TABLE_MATERIAL + " set dyzt = 1 where qptmh = :qptmh";
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
	
	m_WaxList.ResetGridSize();
	m_WaxList.RedrawAll();
	m_WaxList.RedrawAll();

}
