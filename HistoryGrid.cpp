// HistoryGrid.cpp: implementation of the CHistoryGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "HistoryGrid.h"
#include "HistoryShow.h"
#include "Figure.h"
#include "DBList.h"
#include "PathologyView.h"
#include "MedPerson.h"
#include "PathologyDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CHistoryGrid::CHistoryGrid()
{
	m_WhereBetween_Cmd.Format(" where medicalno='tianjinwen'");
	m_OrderBy_Cmd.Format(" order by medicalno desc");
}

CHistoryGrid::~CHistoryGrid()
{
}

void CHistoryGrid::ResetGridSize()
{
	theApp.ConnectDatabase();

	int row_num = 0;
	int col_num = 11;

	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + m_WhereBetween_Cmd;
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
		{
			row_num = g_dbcommand.Field(1).asLong();
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


	SetNumberRows(row_num);
	SetNumberCols(col_num);

	for(int i = 0; i < row_num; i++ )
	{
		QuickSetText( 0, i, "" );
	}


	for(i = 0 ; i < (col_num-1); i++ )
	{
		SetColWidth(i, 80);
	}
	SetColWidth(col_num - 1, 200);

	if( row_num > 0 )
	{
		GotoRow(0);
		RedrawCell(0, 0);
        OnRowChange(0,0);
	}
}

void CHistoryGrid::OnSetup()
{
	m_font.CreateFont(16,0,0,0,500,0,0,0,0,0,0,0,0,"Arial");
	SetDefFont(&m_font);


	CUGCell cell;
	GetGridDefault(&cell);
	cell.SetAlignment(UG_ALIGNLEFT);
	cell.SetBorder(UG_BDR_RECESSED);
	cell.SetBackColor(RGB(220,220,220));
	cell.SetHBackColor(RGB(255,192,192));
	cell.SetHTextColor(RGB(0,0,0));
	SetGridDefault(&cell);

    ResetGridSize();

	SetSH_NumberCols(0);
	SetSH_Width(0);

	SetMultiSelectMode(FALSE);
	SetHighlightRow(TRUE);

	SetDoubleBufferMode(TRUE);

	AddMenuItem(1000,"图像显示");
	AddMenuItem(1001,"打开病例");
	AddMenuItem(1002,"复制病历");

	EnableMenu(1);	

	SetCurrentCellMode(2);
}

void CHistoryGrid::OnMenuCommand(int col,long row,int section,int item)
{
	if( item == 1000 )        //  1000 - 图像显示
	{
		if(row >= 0 && row < GetNumberRows())
		{
			CString curno = QuickGetText(0, row);
			curno.TrimLeft(); curno.TrimRight();
			if(!curno.IsEmpty())
			{
				try
				{
					SAString cmdstr = "Select * from " + theApp.TABLE_IMAGE + " where medicalno = :medicalno order by serialno desc";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = curno;
					
					g_dbcommand.Execute();
					if(g_dbcommand.FetchNext())
					{
						SAString Bstr = g_dbcommand.Field("imgdataset").asBLob();
						if( !Bstr.IsEmpty() )
						{
							const void * binbuf = (const void *)Bstr;
							BYTE * dataptr = (BYTE *)binbuf;
							
							CFile file;
							CFileException fe;
							
							CString fstr = theApp.m_SysTmpPath;
							fstr += "pic.dat";
							remove(fstr);
							if (file.Open( fstr , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
							{
								file.WriteHuge( dataptr , Bstr.GetLength());
								file.Close();
							}
							
							CFigure figure(fstr,
								           190,80,  //window位置
								           640,480, //显示大小
								           curno, g_dbcommand.Field("serialno").asLong());
							
							remove(fstr);
						}
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
			}
		}
		return;
	}

	if(item == 1001)		//打开
	{
		int col; long row;

		int result = EnumFirstSelected(&col,&row);
		if(result == UG_SUCCESS)
		{
			CString mno = QuickGetText(0,row);
			
			((CHistoryShow*)GetParent())->SendMessage(WM_CLOSE);
			theApp.OpenDocumentFile(mno);
			//EndDialog(GetParent()->m_hWnd,1);
		//	((CDBList*)(GetParent()->GetParent()))->openLcjc();
			if(theApp.m_StationType.CompareNoCase("登记工作站")) //不为登记站执行
			{
				theApp.GetView()->m_tabCtrl.SetActiveView(1);
			}
			
		}
	}

	if(item == 1002)		//复制病历
	{
		int col; long row; CString mno;

		int result = EnumFirstSelected(&col,&row);
		if( result == UG_SUCCESS )
		{
			mno = QuickGetText(0,row);
		}
		
		
		if(mno.IsEmpty())  return;
		int bgzt;
		try
		{
			SAString cmdstr = "select bgzt from " + theApp.TABLE_MAIN + " where medicalno=:medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Execute();
			while(g_dbcommand.FetchNext())
			{
				bgzt = g_dbcommand.Field("bgzt").asLong();
			}
		}
		catch (SAException &e)
		{
			AfxMessageBox((const char*)e.ErrText());
		}
		
		CString	n_hisid;
		CString n_ordertype;
		CString	n_sqh;
		CString	n_brid;
		CString	n_bdh;
		CString	n_ybh;
		CString	n_mzh;
		CString	n_zyh;
		CString	n_xm;
		CString	n_xb;
		CString	n_nl;
		CString n_nldw;
		CString n_csrq;
		CString	n_mz;
		CString	n_hf;
		CString	n_zy;
		CString	n_sfzh;
		CString	n_lxdz;
		CString n_lxdh;
		CString	n_sjdw;
		CString	n_sjks;
		CString	n_sjys;
		int n_bgzt;



		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("medicalno").setAsString() = mno;
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				n_hisid = g_dbcommand.Field("hisid").asString();
				n_ordertype = g_dbcommand.Field("orderType").asString();
				n_sqh = g_dbcommand.Field("sqh").asString();
				n_brid = g_dbcommand.Field("brid").asString();
				n_bdh = g_dbcommand.Field("bdh").asString();
				n_ybh = g_dbcommand.Field("ybh").asString();
				n_mzh = g_dbcommand.Field("mzh").asString();
				n_zyh = g_dbcommand.Field("zyh").asString();
				n_xm = g_dbcommand.Field("xm").asString();
				n_xb = g_dbcommand.Field("xb").asString();
				n_nl = g_dbcommand.Field("nl").asString();
				n_nldw = g_dbcommand.Field("nldw").asString();
				n_csrq = g_dbcommand.Field("csrq").asString();
				n_mz = g_dbcommand.Field("mz").asString();
				n_hf = g_dbcommand.Field("hf").asString();
				n_zy = g_dbcommand.Field("zy").asString();
				n_sfzh = g_dbcommand.Field("sfzh").asString();
				n_lxdz = g_dbcommand.Field("lxdz").asString();
				n_lxdh = g_dbcommand.Field("lxdh").asString();
				n_sjdw = g_dbcommand.Field("sjdw").asString();
				n_sjks = g_dbcommand.Field("sjks").asString();
				n_sjys = g_dbcommand.Field("sjys").asString();
				n_bgzt = g_dbcommand.Field("bgzt").asLong();
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
		
		if(bgzt >= 1)
		{
			AfxMessageBox("当前记录不是新登记病例，不能复制!");
			return;
		}

		CWnd *pWnd;
		pWnd = theApp.GetView()->m_tabCtrl.GetView(RUNTIME_CLASS(CMedPerson));
		if(pWnd != NULL) 
		{
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_ORDERTYPE, n_hisid);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_SQH,   n_sqh);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_BRID,  n_brid);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_BDH,   n_bdh);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_YBH,   n_ybh);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_MZH,   n_mzh);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_ZYH,   n_zyh);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_XM,    n_xm);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_XB,    n_xb);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_NL,    n_nl);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_NLDW,  n_nldw);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_CSRQ,  n_csrq);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_MZ,    n_mz);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_HF,    n_hf);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_ZY,    n_zy);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_SFZH,  n_sfzh);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_LXDZ,  n_lxdz);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_LXDH,  n_lxdh);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_SJDW,  n_sjdw);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_SJKS,  n_sjks);
			((CMedPerson *)pWnd)->SetDlgItemText(IDC_EDIT_SJYS,  n_sjys);			
			((CMedPerson *)pWnd)->SetModifiedFlag(true);		
		}
		AfxMessageBox("复制病历成功！");
	}
	
}

void CHistoryGrid::OnGetCell(int col,long row,CUGCell *cell)
{
	if( row == -1 )     //  Display table head information of pRecordSet_Define
	{
		if( col >= 0 )
		{
			switch(col) {
			case 0:
				cell->SetText( "病理号" );
				break;
			case 1:
				cell->SetText( "病人ID" );
				break;
			case 2:
				cell->SetText( "姓名" );
				break;
			case 3:
				cell->SetText( "性别" );
				break;
			case 4:
				cell->SetText( "年龄" );
				break;
			case 5:
				cell->SetText( "住院号" );
				break;
			case 6:
				cell->SetText( "初诊医师" );
				break;
			case 7:
				cell->SetText( "主诊医师" );
				break;
			case 8:
				cell->SetText( "审核医师" );
				break;
			case 9:
				cell->SetText( "报告日期" );
				break;
			case 10:
				cell->SetText( "病理诊断" );
				break;
			}
		}
		return;
	}

	if( col == -1 )
	{
		if( row >= 0 )
		{
			if( row == GetCurrentRow() )
				cell->SetCellType(UGCT_ARROW);
		}
		return;
	}
	if( row < 0 || col != 0 )  
	{
		return;
	}

	CString str_MedicalNo = QuickGetText(0, row);
	str_MedicalNo.TrimLeft();  str_MedicalNo.TrimRight();
	if( !str_MedicalNo.IsEmpty() )  return;
	
	theApp.ConnectDatabase();

	if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
	{
		try
		{
			//----------------------------------------------------------------
			CRect ct;
			GetWindowRect(&ct);
			int gridheight = ct.Height();
			int bottomrow  = GetTopRow();
			int tmp = GetTH_Height() + GetRowHeight(bottomrow);
			while( bottomrow < GetNumberRows() )
			{
				if(tmp >= gridheight)  break;
				bottomrow++;
				if(bottomrow < GetNumberRows())
					tmp += GetRowHeight(bottomrow);
			}
			int toprow = GetTopRow();
			if( bottomrow >=  GetNumberRows() )
				bottomrow = GetNumberRows();// - 1;
			//----------------------------------------------------------------
			CString rowlimit_left, rowlimit_right;
			rowlimit_right.Format(" where ROWNUM<=%d", bottomrow);
		    rowlimit_left.Format(" where RNO>=%d", toprow + 1);

			SAString cmdstr = "select medicalno from " + theApp.TABLE_MAIN + m_WhereBetween_Cmd + m_OrderBy_Cmd;
			cmdstr = "select ROWNUM RNO,t.* from (" + cmdstr + ") t" + (SAString)rowlimit_right;
			cmdstr = "select * from (" + cmdstr + ")" + (SAString)rowlimit_left;
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Execute();
			
			CStringArray mnolist;
			CString pp;
			int rowno = toprow;
			while( g_dbcommand.FetchNext() )
			{
				pp = QuickGetText(0, rowno);
				pp.TrimLeft();  pp.TrimRight();
				if( !pp.IsEmpty() ) 
				{
					rowno++;
					mnolist.Add("");
					continue;
				}
				
				CString FieldStr;	FieldStr.Empty();
				FieldStr = g_dbcommand.Field("medicalno").asString();
				if( FieldStr.IsEmpty() )   FieldStr.Format(" ");
				
				if(rowno == row)   cell->SetText(FieldStr);
				
				QuickSetText(0, rowno, FieldStr);
				
				FieldStr.TrimLeft();  FieldStr.TrimRight();
				mnolist.Add(FieldStr);
				
				rowno++;
			}
			
			CString mno;
			for(int k = 0; k < mnolist.GetSize(); k++ )
			{
				mno = mnolist.GetAt(k);
				if(mno.IsEmpty())  continue;
				
				try
				{
					cmdstr = "Select * from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
					g_dbcommand.setCommandText( cmdstr );
					g_dbcommand.Param("medicalno").setAsString() = mno;
					g_dbcommand.Execute();
					
					if( g_dbcommand.FetchNext() )
					{
						QuickSetText( 0, toprow + k, mno );
						QuickSetText( 1, toprow + k, g_dbcommand.Field("brid").asString() );
						QuickSetText( 2, toprow + k, g_dbcommand.Field("xm").asString() );
						QuickSetText( 3, toprow + k, g_dbcommand.Field("xb").asString() );
						QuickSetText( 4, toprow + k, g_dbcommand.Field("nl").asString() );
						QuickSetText( 5, toprow + k, g_dbcommand.Field("zyh").asString() );
						QuickSetText( 6, toprow + k, g_dbcommand.Field("czys").asString() );
						QuickSetText( 7, toprow + k, g_dbcommand.Field("zzys").asString() );
						QuickSetText( 8, toprow + k, g_dbcommand.Field("shys").asString() );
						if(g_dbcommand.Field("bgrq").isNull() || g_dbcommand.Field("bgrq").asDateTime().GetYear() <= 1950)
						{
							QuickSetText( 9, toprow + k, " " );
						}
						else
						{
							SADateTime st = g_dbcommand.Field("bgrq").asDateTime();
							COleDateTime curt;
							curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
							QuickSetText( 9, toprow + k, curt.Format("%Y-%m-%d") );
						}
						QuickSetText( 10, toprow + k, g_dbcommand.Field("zdyj").asString() );
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
			}
			
			mnolist.RemoveAll();
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
		try
		{
			SAString cmdstr = "select medicalno into ##mm" + theApp.m_LocalIp + " from " + theApp.TABLE_MAIN + m_WhereBetween_Cmd + m_OrderBy_Cmd;
			g_dbcommand.setCommandText(cmdstr);
			
			g_dbcommand.Execute();
			
			cmdstr = "select medicalno, IDENTITY(int,0,1) as Num into ##qq" + theApp.m_LocalIp + " from ##mm" + theApp.m_LocalIp;
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Execute();
			
			//----------------------------------------------------------------
			CRect ct;
			GetWindowRect(&ct);
			int gridheight = ct.Height();
			int bottomrow  = GetTopRow();
			int tmp = GetTH_Height() + GetRowHeight(bottomrow);
			while( bottomrow < GetNumberRows() )
			{
				if(tmp >= gridheight)  break;
				bottomrow++;
				if(bottomrow < GetNumberRows())
					tmp += GetRowHeight(bottomrow);
			}
			int toprow = GetTopRow();
			if( bottomrow >=  GetNumberRows() )
				bottomrow = GetNumberRows() - 1;
			
			//----------------------------------------------------------------
			
			cmdstr = "select medicalno from ##qq" + theApp.m_LocalIp + " where Num >= :Num1 and Num <= :Num2";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("Num1").setAsLong() = toprow;
			g_dbcommand.Param("Num2").setAsLong() = bottomrow;
			g_dbcommand.Execute();
			
			CStringArray mnolist;
			CString pp;
			int rowno = toprow;
			while( g_dbcommand.FetchNext() )
			{
				pp = QuickGetText(0, rowno);
				pp.TrimLeft();  pp.TrimRight();
				if( !pp.IsEmpty() ) 
				{
					rowno++;
					mnolist.Add("");
					continue;
				}
				
				CString FieldStr;	FieldStr.Empty();
				FieldStr = g_dbcommand.Field("medicalno").asString();
				if( FieldStr.IsEmpty() )   FieldStr.Format(" ");
				
				if(rowno == row)   cell->SetText(FieldStr);
				
				QuickSetText(0, rowno, FieldStr);
				
				FieldStr.TrimLeft();  FieldStr.TrimRight();
				mnolist.Add(FieldStr);
				
				rowno++;
			}
			
			cmdstr = "drop table ##mm" + theApp.m_LocalIp;
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Execute();
			
			cmdstr = "drop table ##qq" + theApp.m_LocalIp;
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Execute();
			
			CString mno;
			for(int k = 0; k < mnolist.GetSize(); k++ )
			{
				mno = mnolist.GetAt(k);
				if(mno.IsEmpty())  continue;
				
				try
				{
					cmdstr = "Select * from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
					g_dbcommand.setCommandText( cmdstr );
					g_dbcommand.Param("medicalno").setAsString() = mno;
					g_dbcommand.Execute();
					
					if( g_dbcommand.FetchNext() )
					{
						QuickSetText( 0, toprow + k, mno );
						QuickSetText( 1, toprow + k, g_dbcommand.Field("brid").asString() );
						QuickSetText( 2, toprow + k, g_dbcommand.Field("xm").asString() );
						QuickSetText( 3, toprow + k, g_dbcommand.Field("xb").asString() );
						QuickSetText( 4, toprow + k, g_dbcommand.Field("nl").asString() );
						QuickSetText( 5, toprow + k, g_dbcommand.Field("zyh").asString() );
						QuickSetText( 6, toprow + k, g_dbcommand.Field("czys").asString() );
						QuickSetText( 7, toprow + k, g_dbcommand.Field("zzys").asString() );
						QuickSetText( 8, toprow + k, g_dbcommand.Field("shys").asString() );
						if(g_dbcommand.Field("bgrq").isNull() || g_dbcommand.Field("bgrq").asDateTime().GetYear() <= 1950)
						{
							QuickSetText( 9, toprow + k, " " );
						}
						else
						{
							SADateTime st = g_dbcommand.Field("bgrq").asDateTime();
							COleDateTime curt;
							curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
							QuickSetText( 9, toprow + k, curt.Format("%Y-%m-%d") );
						}
						QuickSetText( 10, toprow + k, g_dbcommand.Field("zdyj").asString() );
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
			}
			
			mnolist.RemoveAll();
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

int  CHistoryGrid::OnCanSizeRow(long row)
{
	return FALSE;
}

int CHistoryGrid::OnCanSizeTopHdg()
{
	return FALSE;
}

int CHistoryGrid::OnCanSizeSideHdg()
{
	return FALSE;
}

void CHistoryGrid::OnRowChange(long oldrow,long newrow)
{
    CHistoryShow *pParent = (CHistoryShow *)GetParent();

	if(GetNumberRows() < 1 || newrow < 0 || newrow >= GetNumberRows())   
	{
		pParent->SetDlgItemText(IDC_EDIT_RYSJ, "");
		pParent->SetDlgItemText(IDC_EDIT_ZDYJ, "");
		return;
	}

	CString nblh = QuickGetText(0, newrow);
	if(nblh.IsEmpty())
	{
		pParent->SetDlgItemText(IDC_EDIT_RYSJ, "");
		pParent->SetDlgItemText(IDC_EDIT_ZDYJ, "");
		return;
	}

	theApp.ConnectDatabase();
	CString m_Rysj, m_Zdyj, m_Czyj, m_Xgyj, m_Bdjg, m_bczd;
	m_Rysj.Empty(); m_Zdyj.Empty(); m_Czyj.Empty(); m_Xgyj.Empty(); m_Bdjg.Empty(); m_bczd.Empty();

	try
	{
		SAString cmdstr = "Select rysj,czyj,zdyj,xgyj,bdjg,bczd from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = nblh;
		
		g_dbcommand.Execute();
		
		if(g_dbcommand.FetchNext())
		{
			m_Rysj = g_dbcommand.Field("rysj").asString();
			m_Zdyj = g_dbcommand.Field("zdyj").asString();
			m_Czyj = g_dbcommand.Field("czyj").asString();
			m_Xgyj = g_dbcommand.Field("xgyj").asString();
			m_Bdjg = g_dbcommand.Field("bdjg").asString();
			m_bczd = g_dbcommand.Field("bczd").asString();
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

	pParent->SetDlgItemText(IDC_EDIT_RYSJ, m_Rysj);
	pParent->SetDlgItemText(IDC_EDIT_ZDYJ, m_Zdyj);
	pParent->SetDlgItemText(IDC_EDIT_CZYJ, m_Czyj);
	pParent->SetDlgItemText(IDC_EDIT_XGYJ, m_Xgyj);
	pParent->SetDlgItemText(IDC_EDIT_BDJG, m_Bdjg);
	pParent->SetDlgItemText(IDC_EDIT_BCZD, m_bczd);
}
