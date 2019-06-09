// WaxGrid.cpp: implementation of the CWaxGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "WaxGrid.h"
#include "MedWax.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CWaxGrid::CWaxGrid()
{
	m_WhereBetween_Cmd.Empty();
	///m_OrderBy_Cmd.Format(" order by qcrq asc");
	m_OrderBy_Cmd.Format(" order by a.medicalno desc, a.qcrq asc");
}

CWaxGrid::~CWaxGrid()
{

}

void CWaxGrid::ResetGridSize()
{
	theApp.ConnectDatabase();

	int row_num = 0;
	int col_num = 12;

	CString tail;
	if(m_WhereBetween_Cmd.IsEmpty())
		tail.Format(" where a.medicalno = b.medicalno and a.zpzt < 2 and b.bgzt<>0");
	else
		tail.Format(" and a.medicalno = b.medicalno and a.zpzt < 2 and b.bgzt<>0");
		
	int cks_sum = 0;

	try
	{
		SAString cmdstr = "Select count(*), sum(cks) from " + theApp.TABLE_MATERIAL + " a," +theApp.TABLE_MAIN + " b "+ m_WhereBetween_Cmd + tail;
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
		{
			row_num = g_dbcommand.Field(1).asLong();
			cks_sum = g_dbcommand.Field(2).asLong();
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

	CWnd *pParent = GetParent();
	if(pParent->IsKindOf(RUNTIME_CLASS(CMedWax)))
	{
		CString info;
		info.Format("当前待包埋数：%d   材块数：%d", row_num, cks_sum);
		((CMedWax *)pParent)->m_HypLink_Note.SetWindowText(info);
		((CMedWax *)pParent)->m_HypLink_Note.SetURL("");
		((CMedWax *)pParent)->m_HypLink_Note.Invalidate();
	}

	SetNumberRows(row_num);
	SetNumberCols(col_num);

	for(int i = 0; i < row_num; i++ )
	{
		QuickSetText( 0, i, "" );
	}

	//--------------------------------------------------
	CString LastBlh; LastBlh.Empty();
	bool UsingBackgroundColor = false;
	int  index = 0;
	CUGCell defcell;
	GetGridDefault(&defcell);

	try
	{
		SAString cmdstr = "Select a.medicalno from " + theApp.TABLE_MATERIAL + " a," +theApp.TABLE_MAIN + " b " + m_WhereBetween_Cmd + tail + m_OrderBy_Cmd;
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();

		while( g_dbcommand.FetchNext() )
		{
			CString blh = g_dbcommand.Field(1).asString();
			if(blh.CompareNoCase(LastBlh))
			{
				UsingBackgroundColor = !UsingBackgroundColor;
				LastBlh = blh;
			}
			if(UsingBackgroundColor)
			{
				for(i = 0; i < col_num; i++)
				{
					CUGCell cell;
					GetCell(i,index,&cell);
					cell.ClearPropertySetFlag(UGCELL_BACKCOLOR_SET);
					cell.SetBackColor(defcell.GetBackColor());
					SetCell(i,index,&cell);
				}
			}
			else
			{
				for(i = 0; i < col_num; i++)
				{
					QuickSetBackColor(i,index,RGB(170,170,255));
				}
			}

			index++;
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
	//--------------------------------------------------

	for(i = 0 ; i < col_num; i++ )
	{
		SetColWidth(i, 80);
		if( i == 2)
		{
			SetColWidth(i,40);
		}
		if( i == 10)
		{
			SetColWidth(i,120);
		}
	}

	if( row_num > 0 )
	{
		//GotoRow(0);
	}
}

void CWaxGrid::OnSetup()
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

	SetMultiSelectMode(TRUE);
	SetHighlightRow(TRUE);

	SetDoubleBufferMode(TRUE);

	SetCurrentCellMode(2);

	LockColumns(1);
}

void CWaxGrid::OnGetCell(int col,long row,CUGCell *cell)
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
				cell->SetText( "任务来源" );
				break;
			case 2:
				cell->SetText( "序号" );
				break;
			case 3:
				cell->SetText( "取材部位" );
				break;
			case 4:
				cell->SetText( "材块数" );
				break;
			case 5:
				cell->SetText( "取材医生" );
				break;
			case 6:
				cell->SetText( "取材日期" );
				break;
			case 7:
				cell->SetText( "材块核对" );
				break;
			case 8:
				cell->SetText( "打印状态" );
				break;
			case 9:
				cell->SetText( "制片状态" );
				break;
			case 10:
				cell->SetText( "备注" );
				break;
			case 11:
				cell->SetText( "核对时间" );
			}
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

	CString tail;
	if(m_WhereBetween_Cmd.IsEmpty())
		tail.Format(" where a.medicalno = b.medicalno and a.zpzt < 2 and b.bgzt<>0");
	else
		tail.Format(" and a.medicalno = b.medicalno and a.zpzt < 2 and b.bgzt<>0");
	
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

			SAString cmdstr = "select a.medicalno, a.xh from " + theApp.TABLE_MATERIAL + " a," + theApp.TABLE_MAIN + " b" + m_WhereBetween_Cmd + tail + m_OrderBy_Cmd;
			cmdstr = "select ROWNUM RNO,t.* from (" + cmdstr + ") t" + (SAString)rowlimit_right;
			cmdstr = "select * from (" + cmdstr + ")" + (SAString)rowlimit_left;
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Execute();
			
			CStringArray mnolist, xhlist;
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
					xhlist.Add("");
					continue;
				}
				
				CString FieldStr;	FieldStr.Empty();
				FieldStr = g_dbcommand.Field("medicalno").asString();
				if( FieldStr.IsEmpty() )   FieldStr.Format(" ");
				
				if(rowno == row)   cell->SetText(FieldStr);
				
				QuickSetText(0, rowno, FieldStr);
				
				FieldStr.TrimLeft();  FieldStr.TrimRight();
				mnolist.Add(FieldStr);
				
				xhlist.Add(g_dbcommand.Field("xh").asString());
				
				rowno++;
			}
			
			CString mno;
			for(int k = 0; k < mnolist.GetSize(); k++ )
			{
				mno = mnolist.GetAt(k);
				if(mno.IsEmpty())  continue;
				
				try
				{
					if(xhlist.GetAt(k).IsEmpty())
					{
						cmdstr = "Select * from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno and xh is null";
					}
					else
					{
						cmdstr = "Select * from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno and xh = :xh";
					}
					
					g_dbcommand.setCommandText( cmdstr );
					g_dbcommand.Param("medicalno").setAsString() = mno;
					if(!xhlist.GetAt(k).IsEmpty())
					{
						g_dbcommand.Param("xh").setAsString() = xhlist.GetAt(k);
					}
					
					g_dbcommand.Execute();
					
					if( g_dbcommand.FetchNext() )
					{
						QuickSetText( 0, toprow + k, mno );
						if(g_dbcommand.Field("rwly").asString().CompareNoCase("脱钙") == 0)
						{
							CUGCell cell;
							GetCell(1,toprow + k,&cell);
							cell.SetTextColor(RGB(255,0,0));
							cell.SetText("脱钙");
							SetCell(1,toprow + k,&cell);
						}
						else
						{
							CUGCell cell;
							GetCell(1,toprow + k,&cell);
							cell.SetTextColor(AFX_IDC_COLOR_BLACK);
							SetCell(1,toprow + k,&cell);
							QuickSetText(1, toprow + k , g_dbcommand.Field("rwly").asString());
						}

						QuickSetText( 2, toprow + k, g_dbcommand.Field("xh").asString() );
						QuickSetText( 3, toprow + k, g_dbcommand.Field("qcbw").asString() );
						
						CString cksstr;
						cksstr.Format("%d", g_dbcommand.Field("cks").asLong());
						QuickSetText( 4, toprow + k, cksstr );
						
						QuickSetText( 5, toprow + k, g_dbcommand.Field("qcys").asString() );
						
						COleDateTime curt;
						SADateTime st = g_dbcommand.Field("qcrq").asDateTime();
						curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
						QuickSetText( 6, toprow + k, curt.Format("%Y-%m-%d") );
						
						QuickSetText( 7, toprow + k, g_dbcommand.Field("ckhd").asString() );

						if(g_dbcommand.Field("dyzt").asLong() == 0)
						{
							QuickSetText( 8, toprow + k, "未打印");
						}
						else
						{
							QuickSetText( 8, toprow + k, "已打印");
						}
						
						if(g_dbcommand.Field("hdrq").isNull())
						{
							QuickSetText( 11, toprow + k, " " );
						}
						else
						{
							st = g_dbcommand.Field("hdrq").asDateTime();
							curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
							QuickSetText( 11, toprow + k, curt.Format("%Y-%m-%d") );
						}
						
						switch(g_dbcommand.Field("zpzt").asLong()) {
						case 0:
							QuickSetText(9, toprow + k , "待核对");
							break;
						case 1:
							QuickSetText(9, toprow + k , "待包埋");
							break;
						case 2:
							QuickSetText(9, toprow + k , "待切片");
							break;
						case 3:
							QuickSetText(9, toprow + k , "已切片");
							break;
						}
						
						if(g_dbcommand.Field("bz").asString().IsEmpty())
						{
							QuickSetText( 10, toprow + k, "");
						}
						else
						{
							QuickSetText( 10, toprow + k, g_dbcommand.Field("bz").asString() );
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
			}
			
			mnolist.RemoveAll();
			xhlist.RemoveAll();
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
			SAString cmdstr = "select a.medicalno, a.xh into ##mm" + theApp.m_LocalIp + " from " + theApp.TABLE_MATERIAL +" a," + theApp.TABLE_MAIN + " b " + m_WhereBetween_Cmd + tail + m_OrderBy_Cmd;
			//AfxMessageBox(cmdstr);
			g_dbcommand.setCommandText(cmdstr);
			
			g_dbcommand.Execute();
			
			cmdstr = "select medicalno, xh, IDENTITY(int,0,1) as Num into ##qq" + theApp.m_LocalIp + " from ##mm" + theApp.m_LocalIp;
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
			
			cmdstr = "select medicalno, xh from ##qq" + theApp.m_LocalIp + " where Num >= :Num1 and Num <= :Num2";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("Num1").setAsLong() = toprow;
			g_dbcommand.Param("Num2").setAsLong() = bottomrow;
			g_dbcommand.Execute();
			
			CStringArray mnolist, xhlist;
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
					xhlist.Add("");
					continue;
				}
				
				CString FieldStr;	FieldStr.Empty();
				FieldStr = g_dbcommand.Field("medicalno").asString();
				if( FieldStr.IsEmpty() )   FieldStr.Format(" ");
				
				if(rowno == row)   cell->SetText(FieldStr);
				
				QuickSetText(0, rowno, FieldStr);
				
				FieldStr.TrimLeft();  FieldStr.TrimRight();
				mnolist.Add(FieldStr);
				
				xhlist.Add(g_dbcommand.Field("xh").asString());
				
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
					cmdstr = "Select * from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno and xh = :xh";
					g_dbcommand.setCommandText( cmdstr );
					g_dbcommand.Param("medicalno").setAsString() = mno;
					g_dbcommand.Param("xh").setAsString() = xhlist.GetAt(k);
					g_dbcommand.Execute();
					
					if( g_dbcommand.FetchNext() )
					{
						QuickSetText( 0, toprow + k, mno );
						if(g_dbcommand.Field("rwly").asString().CompareNoCase("脱钙") == 0)
						{
							CUGCell cell;
							GetCell(1,toprow + k,&cell);
							cell.SetTextColor(RGB(255,0,0));
							cell.SetText("脱钙");
							SetCell(1,toprow + k,&cell);
						}
						else
						{
							CUGCell cell;
							GetCell(1,toprow + k,&cell);
							cell.SetTextColor(AFX_IDC_COLOR_BLACK);
							SetCell(1,toprow + k,&cell);
							QuickSetText(1, toprow + k , g_dbcommand.Field("rwly").asString());
						}
						QuickSetText( 2, toprow + k, g_dbcommand.Field("xh").asString() );
						QuickSetText( 3, toprow + k, g_dbcommand.Field("qcbw").asString() );
						
						CString cksstr;
						cksstr.Format("%d", g_dbcommand.Field("cks").asLong());
						QuickSetText( 4, toprow + k, cksstr );
						
						QuickSetText( 5, toprow + k, g_dbcommand.Field("qcys").asString() );
						
						COleDateTime curt;
						SADateTime st = g_dbcommand.Field("qcrq").asDateTime();
						curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
						QuickSetText( 6, toprow + k, curt.Format("%Y-%m-%d") );
						
						QuickSetText( 7, toprow + k, g_dbcommand.Field("ckhd").asString() );
						
						if(g_dbcommand.Field("dyzt").asLong() == 0)
						{
							QuickSetText( 8, toprow + k, "未打印");
						}
						else
						{
							QuickSetText( 8, toprow + k, "已打印");
						}
						
						if(g_dbcommand.Field("hdrq").isNull())
						{
							QuickSetText( 11, toprow + k, " " );
						}
						else
						{
							st = g_dbcommand.Field("hdrq").asDateTime();
							curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
							QuickSetText( 11, toprow + k, curt.Format("%Y-%m-%d") );
						}
						
						switch(g_dbcommand.Field("zpzt").asLong()) {
						case 0:
							QuickSetText(9, toprow + k , "待核对");
							break;
						case 1:
							QuickSetText(9, toprow + k , "待包埋");
							break;
						case 2:
							QuickSetText(9, toprow + k , "待切片");
							break;
						case 3:
							QuickSetText(9, toprow + k , "已切片");
							break;
						}
						
						QuickSetText( 10, toprow + k, g_dbcommand.Field("bz").asString() );
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
			xhlist.RemoveAll();
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

int  CWaxGrid::OnCanSizeRow(long row)
{
	return FALSE;
}

int CWaxGrid::OnCanSizeTopHdg()
{
	return FALSE;
}

int CWaxGrid::OnCanSizeSideHdg()
{
	return FALSE;
}

void CWaxGrid::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	CString str_MedicalNo = QuickGetText(0, row);
	((CMainFrame *)theApp.m_pMainWnd)->ShowRecordInfo(str_MedicalNo);
}

void CWaxGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	//if(row >= 0 && col == 10)
	//{
	//	StartEdit();
	//}
	if(row >= 0)
	{
		if(col == 10) 
		{
			StartEdit();
		}
		else
		{ 
			CWnd *pParent = GetParent();
			if(pParent->IsKindOf(RUNTIME_CLASS(CMedWax)))
			{
				CString str_MedicalNo = QuickGetText(0, row);
				((CMedWax *)pParent)->SetDlgItemText(IDC_EDIT_BLH, str_MedicalNo);
				((CMedWax *)pParent)->PerformQuery();
			}
		}
	}
}

void CWaxGrid::OnCharDown(UINT *vcKey,BOOL processed)
{
	if(GetCurrentRow() >= 0 && GetCurrentCol() == 10)
	{
		StartEdit(*vcKey);
	}
}

int CWaxGrid::OnEditFinish(int col, long row,CWnd * edit, LPCTSTR string,BOOL cancelFlag)
{
	if(cancelFlag)
	{
		QuickSetText(col,row,string);
	}
	if(row >= 0 && col == 10)
	{
		CString  mno = QuickGetText(0, row);
		CString  xh  = QuickGetText(2, row);

		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_MATERIAL + " set bz = :bz where medicalno = :medicalno and xh = :xh";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = mno;
			g_dbcommand.Param("xh").setAsString()        = xh;
			g_dbcommand.Param("bz").setAsString()        = string;  

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
	
	return TRUE;
}
