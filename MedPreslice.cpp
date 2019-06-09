// CMedPreslice : implementation file
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "Pathology.h"
#include "PathologyView.h"
#include "PathologyDoc.h"
#include "MedPreslice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMedPreslice
extern CPathologyApp theApp;

CMedPreslice::CMedPreslice()
	: CXTResizeFormView(CMedPreslice::IDD)
{
	//{{AFX_DATA_INIT(CMedPreslice)
	//}}AFX_DATA_INIT

	m_bIsFirstInit = true;
}

CMedPreslice::~CMedPreslice()
{
	// TODO: add destruction code here.

}

IMPLEMENT_DYNCREATE(CMedPreslice, CXTResizeFormView)

BEGIN_MESSAGE_MAP(CMedPreslice, CXTResizeFormView)
	//{{AFX_MSG_MAP(CMedPreslice)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnButtonQuery)
	ON_BN_CLICKED(IDC_CHECK_SELECTALL, OnCheckSelectall)
	ON_BN_CLICKED(IDC_BUTTON_QPQR, OnButtonQpqr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMedPreslice message handlers

void CMedPreslice::DoDataExchange(CDataExchange* pDX) 
{
	CXTResizeFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMedPreslice)
	DDX_Control(pDX, IDC_HYP_NOTE, m_HypLink_Note);
	//}}AFX_DATA_MAP
}

void CMedPreslice::OnInitialUpdate() 
{
	CXTResizeFormView::OnInitialUpdate();
	
	if (m_bIsFirstInit)
	{
		m_bIsFirstInit = false;

		CRect rt;
		GetDlgItem(IDC_DATE_BMRQ)->GetWindowRect(&rt);
		ScreenToClient(&rt);
		if (!m_Time_Bmrq.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | DTS_SHORTDATEFORMAT, //,DTS_LONGDATEFORMAT
			rt, this, 112))
		{
			return;
		}

		m_Time_Bmrq.SetTime(theApp.GetOleServerDateTime());

		m_PreSliceList.AttachGrid(this,IDC_STATIC_QPXX);
		m_PreSliceList.SetParent(this);
		m_PreSliceList.ResetGridSize();
		m_PreSliceList.RedrawAll();

		m_HypLink_Note.SetURL("");
		m_HypLink_Note.SetColors(RGB(0x00, 0x00, 0xFF), RGB(0x00, 0x00, 0xFF), RGB(0x00, 0x00, 0xFF));
		m_HypLink_Note.SetUnderline(true);
		m_HypLink_Note.EnableShellExecute(false);

		SetDlgItemText(IDC_EDIT_TIMESPAN, "0");
		return;
	}	

	m_PreSliceList.ResetGridSize();
	m_PreSliceList.RedrawAll();
}

void CMedPreslice::Refresh()
{
	m_PreSliceList.ResetGridSize();
	m_PreSliceList.RedrawAll();
}

extern void ExplainString(CString inputstr, CStringArray& StrList, CStringArray& UdList);

void CMedPreslice::OnButtonQuery() 
{
	// TODO: Add your control notification handler code here
	CString mno; mno.Empty();
	GetDlgItemText(IDC_EDIT_BLH, mno);
	mno.TrimLeft(); mno.TrimRight();
	SetDlgItemText(IDC_EDIT_BLH, "");

	if(mno.IsEmpty())
	{
		//  取材日期查询
		COleDateTime m_time;
		m_Time_Bmrq.GetTime(m_time);

		CString start = m_time.Format("%Y-%m-%d 00:00:00");
		CString end   = m_time.Format("%Y-%m-%d 23:59:59");

		m_PreSliceList.m_WhereBetween_Cmd.Format(" where bmrq >= '%s' and bmrq <= '%s'", start, end);
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			m_PreSliceList.m_WhereBetween_Cmd.Format(" where bmrq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and bmrq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", start, end);
		}
	}
	else
	{
		//  病理号查询
		CStringArray StrList, UdList;
		ExplainString(mno, StrList, UdList);
		if(StrList.GetSize() == 1 && UdList.GetSize() == 0)
		{
			m_PreSliceList.m_WhereBetween_Cmd.Format(" where medicalno = '%s'", mno);
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

			m_PreSliceList.m_WhereBetween_Cmd.Format(" where (%s)", mstr);
		}

		StrList.RemoveAll();
		UdList.RemoveAll();
	}

	m_PreSliceList.ResetGridSize();
	m_PreSliceList.RedrawAll();
}

void CMedPreslice::OnCheckSelectall() 
{
	// TODO: Add your control notification handler code here
	if(((CButton *)GetDlgItem(IDC_CHECK_SELECTALL))->GetState() & 0x0003)
	{
		// Select all
		int curnum = m_PreSliceList.GetCurrentRow();
		for(int i = 0; i < m_PreSliceList.GetNumberRows(); i++)
		{
			m_PreSliceList.GotoRow(i);
		}
		m_PreSliceList.GotoRow(curnum);

		m_PreSliceList.SelectRange(0, 0, m_PreSliceList.GetNumberCols()-1,m_PreSliceList.GetNumberRows()-1);
	}
	else
	{
		// Clear all selection
		m_PreSliceList.ClearSelections();
	}
}

void CMedPreslice::OnButtonQpqr() 
{
	if(AfxMessageBox("选择进行切片确认，该过程不可逆，是否继续？",MB_OKCANCEL|MB_ICONQUESTION) == IDCANCEL)  return;

	CStringArray  MedicalNolist;
	CStringArray  XhNolist;
	CUIntArray  rowlist;
	int nCol, nRet;
	long nRow;

	nRet = m_PreSliceList.EnumFirstSelected( &nCol, &nRow );
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
				m_PreSliceList.GetCellIndirect(0,nRow,&cell);
				cell.GetText(&str_MedicalNo);

				CString str_Lkh;
				m_PreSliceList.GetCellIndirect(2,nRow,&cell);
				cell.GetText(&str_Lkh);
				
				rowlist.Add(nRow);
				MedicalNolist.Add(str_MedicalNo);
				XhNolist.Add(str_Lkh);
			}
		}
		
		nRet = m_PreSliceList.EnumNextSelected( &nCol, &nRow );
	}

	int curnum = m_PreSliceList.GetCurrentRow();
	for(int i = 0; i < rowlist.GetSize(); i++)
	{
		m_PreSliceList.GotoRow(rowlist.GetAt(i));
	}
	m_PreSliceList.GotoRow(curnum);

	rowlist.RemoveAll();

	if( MedicalNolist.GetSize() <= 0 )
	{
		return;
	}

	if(((CButton*)GetDlgItem(IDC_CHECK_HEBING))->GetState() & 0x0003) //合并操作
	{
		if(AfxMessageBox("目前执行的是合并操作，您确定？",MB_OKCANCEL) == IDCANCEL) return;
		int index = 0;
		CString s;
		while(index < MedicalNolist.GetSize() )
		{
			if(index == MedicalNolist.GetSize() - 1)
			{
				CString rwly, lkh, bz, qpczy, qcbw;
				int zpzt = 0, qps = 0;
		
				try
				{
					SAString cmdstr = "Select * from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno and lkh = :lkh";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
					g_dbcommand.Param("lkh").setAsString()       = XhNolist.GetAt(index);

					g_dbcommand.Execute();
					if(g_dbcommand.FetchNext())
					{
						rwly  = g_dbcommand.Field("rwly").asString();
						lkh   = g_dbcommand.Field("lkh").asString();
						bz    = g_dbcommand.Field("bz").asString();
						zpzt  = g_dbcommand.Field("zpzt").asLong();
						qps   = g_dbcommand.Field("qps").asLong();
						qcbw  = g_dbcommand.Field("qcbw").asString();
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

				if(zpzt == 2)
				{
					COleDateTime ServerTime = theApp.GetOleServerDateTime();
					COleDateTimeSpan ts;
					CString tstr;
					GetDlgItemText(IDC_EDIT_TIMESPAN, tstr);
					ts.SetDateTimeSpan(atoi(tstr), 0, 0, 0);
					ServerTime += ts;
					try
					{
						SAString cmdstr = "Update " + theApp.TABLE_MATERIAL + " set zpzt = 3, qpczy = :qpczy, qprq = :qprq where medicalno = :medicalno and lkh = :lkh and zpzt = 2";
						g_dbcommand.setCommandText(cmdstr);
						g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
						g_dbcommand.Param("lkh").setAsString()       = XhNolist.GetAt(index);
						g_dbcommand.Param("qpczy").setAsString()     = theApp.m_Cpp_User;  
						
						qpczy = theApp.m_Cpp_User;
						SADateTime qprq = ServerTime;

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
						return;
					}

					for(int i = 0; i < qps; i++)
					{
						CString NewSliceNo = GenNewSliceNo(MedicalNolist.GetAt(index));

						CString nBdh; nBdh.Empty();
						if(rwly.CompareNoCase("冰冻") == 0 && theApp.m_BdSliceMode.CompareNoCase("冰冻"))
						{
							theApp.GetDatabaseValue(MedicalNolist.GetAt(index), "bdh", nBdh, true);
						}

						try
						{
							SAString cmdstr = "Insert into " + theApp.TABLE_SLICE + " (medicalno,rwly,lkh,qpsm,qptmh,qpczy,qprq,zpzt,dyzt) values(:medicalno,:rwly,:lkh,:qpsm,:qptmh,:qpczy,:qprq,3,0)";
							g_dbcommand.setCommandText(cmdstr);
							g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
							g_dbcommand.Param("rwly").setAsString() = rwly;
							g_dbcommand.Param("lkh").setAsString()  = lkh;
							
							if(rwly.CompareNoCase("冰冻") == 0 || rwly.CompareNoCase("脱钙") == 0 || rwly.CompareNoCase("冰石") == 0)
							{
								if(rwly.CompareNoCase("冰冻") == 0 && theApp.m_BdSliceMode.CompareNoCase("冰冻"))
								{
									g_dbcommand.Param("qpsm").setAsString() = "冰" + nBdh;
								} 
								else
								{
									g_dbcommand.Param("qpsm").setAsString() = rwly;
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
									
									g_dbcommand.Param("qpsm").setAsString() = qmsmstr;
									
								}
								else
								{
									g_dbcommand.Param("qpsm").setAsString() = bz;
								}
							}
							
							g_dbcommand.Param("qptmh").setAsString() = NewSliceNo;
							g_dbcommand.Param("qpczy").setAsString() = qpczy;
							SADateTime qprq = ServerTime;
							g_dbcommand.Param("qprq").setAsDateTime() = qprq;
							
							g_dbcommand.Execute();
							g_dbconnection.Commit();
						}
						catch(SAException &x)
						{
						/*	try
							{
								g_dbconnection.Rollback();
							}
							catch(SAException &)
							{
							}	*/
							AfxMessageBox((const char*)x.ErrText());
						}
					}
				}
				index++;
				continue;
			}
			
		  if(MedicalNolist.GetAt(index).CompareNoCase(MedicalNolist.GetAt(index+1)) == 0  )
		  {
			CString rwly, lkh, bz, qpczy, qcbw;
		    int zpzt = 0, qps = 0;
			CString rwly1, lkh1, bz1, qpczy1, qcbw1;
		    int zpzt1 = 0, qps1 = 0;
			try
			{
				SAString cmdstr = "Select * from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno and lkh = :lkh";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
				g_dbcommand.Param("lkh").setAsString()       = XhNolist.GetAt(index);
				
				g_dbcommand.Execute();
				if(g_dbcommand.FetchNext())
				{
					rwly  = g_dbcommand.Field("rwly").asString();
					lkh   = g_dbcommand.Field("lkh").asString();
					bz    = g_dbcommand.Field("bz").asString();
					zpzt  = g_dbcommand.Field("zpzt").asLong();
					qps   = g_dbcommand.Field("qps").asLong();
					qcbw  = g_dbcommand.Field("qcbw").asString();
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
			try
			{
				SAString cmdstr = "Select * from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno and lkh = :lkh";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index+1);
				g_dbcommand.Param("lkh").setAsString()       = XhNolist.GetAt(index+1);
				
				g_dbcommand.Execute();
				if(g_dbcommand.FetchNext())
				{
					rwly1  = g_dbcommand.Field("rwly").asString();
					lkh1   = g_dbcommand.Field("lkh").asString();
					bz1    = g_dbcommand.Field("bz").asString();
					zpzt1  = g_dbcommand.Field("zpzt").asLong();
					qps1   = g_dbcommand.Field("qps").asLong();
					qcbw1  = g_dbcommand.Field("qcbw").asString();
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

		if(rwly.CompareNoCase(rwly1) == 0)
		{
			//CString temp = lkh1.Right(lkh1.GetLength() - MedicalNolist.GetAt(index+1).GetLength());
			//lkh = lkh + temp;
			
			if(zpzt == 2){
				COleDateTime ServerTime = theApp.GetOleServerDateTime();
				COleDateTimeSpan ts;
				CString tstr;
				GetDlgItemText(IDC_EDIT_TIMESPAN, tstr);
				ts.SetDateTimeSpan(atoi(tstr), 0, 0, 0);
				ServerTime += ts;
				try
				{
					SAString cmdstr = "Update " + theApp.TABLE_MATERIAL + " set zpzt = 3, qpczy = :qpczy, qprq = :qprq where medicalno = :medicalno and lkh = :lkh and zpzt = 2";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
					g_dbcommand.Param("lkh").setAsString()       = XhNolist.GetAt(index);
					g_dbcommand.Param("qpczy").setAsString()     = theApp.m_Cpp_User;  
					
					qpczy = theApp.m_Cpp_User;
					SADateTime qprq = ServerTime;
					
					g_dbcommand.Param("qprq").setAsDateTime() = qprq;
					
					g_dbcommand.Execute();
					g_dbconnection.Commit();

				    cmdstr = "Update " + theApp.TABLE_MATERIAL + " set zpzt = 3, qpczy = :qpczy, qprq = :qprq where medicalno = :medicalno and lkh = :lkh and zpzt = 2";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index+1);
					g_dbcommand.Param("lkh").setAsString()       = XhNolist.GetAt(index+1);
					g_dbcommand.Param("qpczy").setAsString()     = theApp.m_Cpp_User;  
					
					qpczy = theApp.m_Cpp_User;
				//	qprq = ServerTime;
					
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
					return;
				}
				CString temp = lkh1.Right(lkh1.GetLength() - MedicalNolist.GetAt(index+1).GetLength());
			    lkh = lkh + temp;
				if(qcbw.CompareNoCase(qcbw1) != 0)
				{
					qcbw = qcbw + "," + qcbw1;
				}
				qps = MIN(qps,qps1);
				if(bz.CompareNoCase(bz1) != 0)
				{
					bz = bz + bz1;
				}
				for(int i = 0; i < qps; i++)
				{
					CString NewSliceNo = GenNewSliceNo(MedicalNolist.GetAt(index));
					
					CString nBdh; nBdh.Empty();
					if(rwly.CompareNoCase("冰冻") == 0 && theApp.m_BdSliceMode.CompareNoCase("冰冻"))
					{
						theApp.GetDatabaseValue(MedicalNolist.GetAt(index), "bdh", nBdh, true);
					}
					
					try
					{
						SAString cmdstr = "Insert into " + theApp.TABLE_SLICE + " (medicalno,rwly,lkh,qpsm,qptmh,qpczy,qprq,zpzt,dyzt) values(:medicalno,:rwly,:lkh,:qpsm,:qptmh,:qpczy,:qprq,3,0)";
						g_dbcommand.setCommandText(cmdstr);
						g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
						g_dbcommand.Param("rwly").setAsString() = rwly;
						g_dbcommand.Param("lkh").setAsString()  = lkh;
						if(rwly.CompareNoCase("冰冻") == 0 || rwly.CompareNoCase("脱钙") == 0 || rwly.CompareNoCase("冰石")==0)
						{
							if(rwly.CompareNoCase("冰冻") == 0 && theApp.m_BdSliceMode.CompareNoCase("冰冻") == 0)
							{
								g_dbcommand.Param("qpsm").setAsString() = "冰" + nBdh;
							}
							else
							{
								g_dbcommand.Param("qpsm").setAsString() = rwly;
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
								g_dbcommand.Param("qpsm").setAsString() = qmsmstr;
							}
							else
							{
								g_dbcommand.Param("qpsm").setAsString() = bz;
							}
						}
						
						g_dbcommand.Param("qptmh").setAsString() = NewSliceNo;
						g_dbcommand.Param("qpczy").setAsString() = qpczy;
						SADateTime qprq = ServerTime;
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
			index += 2;
		}
		else
		{	
			CString rwly, lkh, bz, qpczy, qcbw;
				int zpzt = 0, qps = 0;
		
				try
				{
					SAString cmdstr = "Select * from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno and lkh = :lkh";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
					g_dbcommand.Param("lkh").setAsString()       = XhNolist.GetAt(index);

					g_dbcommand.Execute();
					if(g_dbcommand.FetchNext())
					{
						rwly  = g_dbcommand.Field("rwly").asString();
						lkh   = g_dbcommand.Field("lkh").asString();
						bz    = g_dbcommand.Field("bz").asString();
						zpzt  = g_dbcommand.Field("zpzt").asLong();
						qps   = g_dbcommand.Field("qps").asLong();
						qcbw  = g_dbcommand.Field("qcbw").asString();
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

				if(zpzt == 2)
				{
					COleDateTime ServerTime = theApp.GetOleServerDateTime();
					COleDateTimeSpan ts;
					CString tstr;
					GetDlgItemText(IDC_EDIT_TIMESPAN, tstr);
					ts.SetDateTimeSpan(atoi(tstr), 0, 0, 0);
					ServerTime += ts;
					try
					{
						SAString cmdstr = "Update " + theApp.TABLE_MATERIAL + " set zpzt = 3, qpczy = :qpczy, qprq = :qprq where medicalno = :medicalno and lkh = :lkh and zpzt = 2";
						g_dbcommand.setCommandText(cmdstr);
						g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
						g_dbcommand.Param("lkh").setAsString()       = XhNolist.GetAt(index);
						g_dbcommand.Param("qpczy").setAsString()     = theApp.m_Cpp_User;  
						
						qpczy = theApp.m_Cpp_User;
						SADateTime qprq = ServerTime;

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
						return;
					}

					for(int i = 0; i < qps; i++)
					{
						CString NewSliceNo = GenNewSliceNo(MedicalNolist.GetAt(index));

						CString nBdh; nBdh.Empty();
						if(rwly.CompareNoCase("冰冻") == 0 && theApp.m_BdSliceMode.CompareNoCase("冰冻"))
						{
							theApp.GetDatabaseValue(MedicalNolist.GetAt(index), "bdh", nBdh, true);
						}

						try
						{
							SAString cmdstr = "Insert into " + theApp.TABLE_SLICE + " (medicalno,rwly,lkh,qpsm,qptmh,qpczy,qprq,zpzt,dyzt) values(:medicalno,:rwly,:lkh,:qpsm,:qptmh,:qpczy,:qprq,3,0)";
							g_dbcommand.setCommandText(cmdstr);
							g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
							g_dbcommand.Param("rwly").setAsString() = rwly;
							g_dbcommand.Param("lkh").setAsString()  = lkh;
							if(rwly.CompareNoCase("冰冻") == 0 || rwly.CompareNoCase("脱钙") == 0 || rwly.CompareNoCase("冰石") == 0)
							{
								if(rwly.CompareNoCase("冰冻") == 0 && theApp.m_BdSliceMode.CompareNoCase("冰冻"))
								{
									g_dbcommand.Param("qpsm").setAsString() = "冰" + nBdh;
								} 
								else
								{
									g_dbcommand.Param("qpsm").setAsString() = rwly;
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
									g_dbcommand.Param("qpsm").setAsString() = qmsmstr;
								}
								else
								{
									g_dbcommand.Param("qpsm").setAsString() = bz;
								}
							}
							
							g_dbcommand.Param("qptmh").setAsString() = NewSliceNo;
							g_dbcommand.Param("qpczy").setAsString() = qpczy;
							SADateTime qprq = ServerTime;
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
			index++;			
		}
	} 
	else
	{
		CString rwly, lkh, bz, qpczy, qcbw;
		int zpzt = 0, qps = 0;
		
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno and lkh = :lkh";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
			g_dbcommand.Param("lkh").setAsString()       = XhNolist.GetAt(index);

			g_dbcommand.Execute();
			if(g_dbcommand.FetchNext())
			{
				rwly  = g_dbcommand.Field("rwly").asString();
				lkh   = g_dbcommand.Field("lkh").asString();
				bz    = g_dbcommand.Field("bz").asString();
				zpzt  = g_dbcommand.Field("zpzt").asLong();
				qps   = g_dbcommand.Field("qps").asLong();
				qcbw  = g_dbcommand.Field("qcbw").asString();
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

		if(zpzt == 2)
		{
			COleDateTime ServerTime = theApp.GetOleServerDateTime();
			COleDateTimeSpan ts;
			CString tstr;
			GetDlgItemText(IDC_EDIT_TIMESPAN, tstr);
			ts.SetDateTimeSpan(atoi(tstr), 0, 0, 0);
			ServerTime += ts;
			try
			{
				SAString cmdstr = "Update " + theApp.TABLE_MATERIAL + " set zpzt = 3, qpczy = :qpczy, qprq = :qprq where medicalno = :medicalno and lkh = :lkh and zpzt = 2";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
				g_dbcommand.Param("lkh").setAsString()       = XhNolist.GetAt(index);
				g_dbcommand.Param("qpczy").setAsString()     = theApp.m_Cpp_User;  
						
				qpczy = theApp.m_Cpp_User;
				SADateTime qprq = ServerTime;

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
				return;
			}

					for(int i = 0; i < qps; i++)
					{
						CString NewSliceNo = GenNewSliceNo(MedicalNolist.GetAt(index));

						CString nBdh; nBdh.Empty();
						if(rwly.CompareNoCase("冰冻") == 0 && theApp.m_BdSliceMode.CompareNoCase("冰冻"))
						{
							theApp.GetDatabaseValue(MedicalNolist.GetAt(index), "bdh", nBdh, true);
						}

						try
						{
							SAString cmdstr = "Insert into " + theApp.TABLE_SLICE + " (medicalno,rwly,lkh,qpsm,qptmh,qpczy,qprq,zpzt,dyzt) values(:medicalno,:rwly,:lkh,:qpsm,:qptmh,:qpczy,:qprq,3,0)";
							g_dbcommand.setCommandText(cmdstr);
							g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
							g_dbcommand.Param("rwly").setAsString() = rwly;
							g_dbcommand.Param("lkh").setAsString()  = lkh;
							if(rwly.CompareNoCase("冰冻") == 0 || rwly.CompareNoCase("脱钙") == 0 || rwly.CompareNoCase("冰石") == 0)
							{
								if(rwly.CompareNoCase("冰冻") == 0 && theApp.m_BdSliceMode.CompareNoCase("冰冻"))
								{
									g_dbcommand.Param("qpsm").setAsString() = "冰" + nBdh;
								} 
								else
								{
									g_dbcommand.Param("qpsm").setAsString() = rwly;
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
									g_dbcommand.Param("qpsm").setAsString() = qmsmstr;
								}
								else
								{
									g_dbcommand.Param("qpsm").setAsString() = bz;
								}
							}
							
							g_dbcommand.Param("qptmh").setAsString() = NewSliceNo;
							g_dbcommand.Param("qpczy").setAsString() = qpczy;
							SADateTime qprq = ServerTime;
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
		index++;	
	} 
		
	}
	} 
	else //非合并操作
	{	
	for(int index = 0; index < MedicalNolist.GetSize(); index++)
	{
		CString rwly, lkh, bz, qpczy, qcbw, qptmh, qpsm; int dyzt;
		int zpzt = 0, qps = 0;
		
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno and lkh = :lkh";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
			g_dbcommand.Param("lkh").setAsString()       = XhNolist.GetAt(index);

			g_dbcommand.Execute();
			if(g_dbcommand.FetchNext())
			{
				rwly  = g_dbcommand.Field("rwly").asString();
				lkh   = g_dbcommand.Field("lkh").asString();
				bz    = g_dbcommand.Field("bz").asString();
				zpzt  = g_dbcommand.Field("zpzt").asLong();
				qps   = g_dbcommand.Field("qps").asLong();
				qcbw  = g_dbcommand.Field("qcbw").asString();
				qptmh = g_dbcommand.Field("qptmh").asString();
				qpsm = g_dbcommand.Field("qpsm").asString();
				dyzt = g_dbcommand.Field("dyzt").asLong();
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

		if(zpzt == 2)
		{
			COleDateTime ServerTime = theApp.GetOleServerDateTime();
			COleDateTimeSpan ts;
			CString tstr;
			GetDlgItemText(IDC_EDIT_TIMESPAN, tstr);
			ts.SetDateTimeSpan(atoi(tstr), 0, 0, 0);
			ServerTime += ts;
			try
			{
				SAString cmdstr = "Update " + theApp.TABLE_MATERIAL + " set zpzt = 3, qpczy = :qpczy, qprq = :qprq where medicalno = :medicalno and lkh = :lkh and zpzt = 2";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
				g_dbcommand.Param("lkh").setAsString()       = XhNolist.GetAt(index);
				g_dbcommand.Param("qpczy").setAsString()     = theApp.m_Cpp_User;  
				
				qpczy = theApp.m_Cpp_User;
				SADateTime qprq = ServerTime;

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
				return;
			}

//			for(int i = 0; i < qps; i++)
//			{
//				CString NewSliceNo = GenNewSliceNo(MedicalNolist.GetAt(index));

//				CString nBdh; nBdh.Empty();
//				if(rwly.CompareNoCase("冰冻") == 0 && theApp.m_BdSliceMode.CompareNoCase("冰冻"))
//				{
//					theApp.GetDatabaseValue(MedicalNolist.GetAt(index), "bdh", nBdh, true);
//				}
				CString NewSliceNo = GenNewSliceNo(MedicalNolist.GetAt(index));
				
				CString nBdh; nBdh.Empty();
				if(rwly.CompareNoCase("冰冻") == 0 && theApp.m_BdSliceMode.CompareNoCase("冰冻"))
				{
					theApp.GetDatabaseValue(MedicalNolist.GetAt(index), "bdh", nBdh, true);
				}

				try
				{
					SAString cmdstr = "Insert into " + theApp.TABLE_SLICE + " (medicalno,rwly,lkh,qpsm,qptmh,qpczy,qprq,zpzt,dyzt,other) values(:medicalno,:rwly,:lkh,:qpsm,:qptmh,:qpczy,:qprq,3,:dyzt,:bz)";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
					g_dbcommand.Param("rwly").setAsString() = rwly;
					g_dbcommand.Param("lkh").setAsString()  = lkh;
					if(qpsm.IsEmpty())
					{
						if(rwly.CompareNoCase("冰冻") == 0 || rwly.CompareNoCase("脱钙") == 0 || rwly.CompareNoCase("冰石")==0 || rwly.CompareNoCase("冰余") == 0)
						{
							if(rwly.CompareNoCase("冰冻") == 0 && theApp.m_BdSliceMode.CompareNoCase("冰冻"))
							{
								g_dbcommand.Param("qpsm").setAsString() = "冰" + nBdh;
							}
							else
							{
								g_dbcommand.Param("qpsm").setAsString() = rwly;
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
								g_dbcommand.Param("qpsm").setAsString() = qmsmstr;
							}
							else
							{
								g_dbcommand.Param("qpsm").setAsString() = bz;
							}
						}
					}
					else
					{
						g_dbcommand.Param("qpsm").setAsString() = qpsm;
					}
					
	/*				if(rwly.CompareNoCase("冰冻") == 0 || rwly.CompareNoCase("脱钙") == 0 || rwly.CompareNoCase("冰石")==0 || rwly.CompareNoCase("冰余") == 0)
					{
						if(rwly.CompareNoCase("冰冻") == 0 && theApp.m_BdSliceMode.CompareNoCase("冰冻"))
						{
							g_dbcommand.Param("qpsm").setAsString() = "冰" + nBdh;
						}
						else
						{
							g_dbcommand.Param("qpsm").setAsString() = rwly;
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
							g_dbcommand.Param("qpsm").setAsString() = qmsmstr;
						}
						else
						{
							g_dbcommand.Param("qpsm").setAsString() = bz;
						}
					}
		*/			

					if(qptmh.IsEmpty())
					{
						g_dbcommand.Param("qptmh").setAsString() = NewSliceNo;
					}
					else
					{
						g_dbcommand.Param("qptmh").setAsString() = qptmh;
					}
					
					g_dbcommand.Param("qpczy").setAsString() = qpczy;
					SADateTime qprq = ServerTime;
					g_dbcommand.Param("qprq").setAsDateTime() = qprq;
					g_dbcommand.Param("dyzt").setAsLong() = dyzt;
					g_dbcommand.Param("bz").setAsString() = bz;
					
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
		//	}
		}
	}
}
	MedicalNolist.RemoveAll();
	XhNolist.RemoveAll();

	m_PreSliceList.ResetGridSize();
	m_PreSliceList.RedrawAll();
}

CString CMedPreslice::GenNewSliceNo(CString blh, int nStartNo)
{
	CString sIdx, nBlh;
	theApp.ConnectDatabase();

	//CString nBlh = blh + "-001";
	sIdx.Format("%02d", nStartNo);
	nBlh = blh + "01";
	
	//========== Generate proper medicalno automatically ===============
	try
	{
		SAString cmdstr = "Select qptmh from " + theApp.TABLE_SLICE + " where medicalno = :medicalno order by qptmh desc";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = blh;

		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			CString curno = g_dbcommand.Field("qptmh").asString();
			
			CString str,str1;
			char strbuf[300];
			int Len=curno.GetLength(),k;
			
			k = 2;
			
			ltoa(atol(curno.Right(k))+1,strbuf,10);
			str1.Format("%s",strbuf);
			str = curno.Left(Len-k);
			Len = k - str1.GetLength();
			for(k=0;k<Len;k++)  str += "0";
			str += str1;
			nBlh = str;
		}
		else
		{
			nBlh = blh + sIdx;	
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

	return nBlh;
}

BOOL CMedPreslice::PreTranslateMessage(MSG* pMsg) 
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

void CMedPreslice::PerformQuery()
{
	OnButtonQuery();
	
	// Select all
	m_PreSliceList.SelectRange(0, 0, m_PreSliceList.GetNumberCols()-1,m_PreSliceList.GetNumberRows()-1);
}
