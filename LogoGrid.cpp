// LogoGrid.cpp: implementation of the CLogoGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "LogoGrid.h"
#include "TextOut.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CLogoGrid::CLogoGrid()
{
	m_WhereBetween_Cmd.Empty();
	m_OrderBy_Cmd.Format(" order by optime desc");
}

CLogoGrid::~CLogoGrid()
{

}

void CLogoGrid::ResetGridSize()
{
	theApp.ConnectDatabase();

	int row_num = 0;
	int col_num = 5;

	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_LOGO + m_WhereBetween_Cmd;
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


	SetColWidth(0, 75);
	SetColWidth(1, 100);
	SetColWidth(2, 300);
	SetColWidth(3, 150);
	SetColWidth(4, 100);

	if( row_num > 0 )
	{
		//GotoRow(0);
	}
}

void CLogoGrid::OnSetup()
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
}

void CLogoGrid::OnGetCell(int col,long row,CUGCell *cell)
{
	if( row == -1 )     //  Display table head information of pRecordSet_Define
	{
		if( col >= 0 )
		{
			switch(col) {
			case 0:
				cell->SetText( "操作方式" );
				break;
			case 1:
				cell->SetText( "病理号" );
				break;
			case 2:
				cell->SetText( "修改前内容" );
				break;
			case 3:
				cell->SetText( "操作时间" );
				break;
			case 4:
				cell->SetText( "操作人" );
				break;
			}
		}
		return;
	}

	if( row < 0 || col != 0 )  
	{
		return;
	}

	CString str_flag = QuickGetText(0, row);
	str_flag.TrimLeft();  str_flag.TrimRight();
	if( !str_flag.IsEmpty() )  return;

	
	theApp.ConnectDatabase();
	
	SADateTime ServerTime = theApp.GetServerDateTime();

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

			SAString cmdstr = "select optype, blh, optime, person from " + theApp.TABLE_LOGO + m_WhereBetween_Cmd + m_OrderBy_Cmd;
			cmdstr = "select ROWNUM RNO,t.* from (" + cmdstr + ") t" + (SAString)rowlimit_right;
			cmdstr = "select * from (" + cmdstr + ")" + (SAString)rowlimit_left;
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Execute();
			
			
			CStringArray optypelist, blhlist, personlist;
			CArray<SADateTime,SADateTime> optimelist;
			optypelist.RemoveAll();
			blhlist.RemoveAll();
			personlist.RemoveAll();
			optimelist.RemoveAll();
			
			CString pp;
			int rowno = toprow;
			while( g_dbcommand.FetchNext() )
			{
				pp = QuickGetText(0, rowno);
				pp.TrimLeft();  pp.TrimRight();
				if( !pp.IsEmpty() ) 
				{
					rowno++;
					
					optypelist.Add("");
					blhlist.Add("");
					personlist.Add("");
					optimelist.Add(ServerTime);
					continue;
				}
				
				if(rowno == row)   cell->SetText(g_dbcommand.Field("optype").asString());
				
				QuickSetText(0, rowno, g_dbcommand.Field("optype").asString());
				
				optypelist.Add(g_dbcommand.Field("optype").asString());
				blhlist.Add(g_dbcommand.Field("blh").asString());
				personlist.Add(g_dbcommand.Field("person").asString());
				optimelist.Add(g_dbcommand.Field("optime").asDateTime());
				
				rowno++;
			}
			
			
			CString mno;
			for(int k = 0; k < optypelist.GetSize(); k++ )
			{
				try
				{
					cmdstr = "Select * from " + theApp.TABLE_LOGO + " where optype=:optype and blh=:blh and optime=:optime and person=:person";
					g_dbcommand.setCommandText( cmdstr );
					g_dbcommand.Param("optype").setAsString() = optypelist.GetAt(k);
					g_dbcommand.Param("blh").setAsString() = blhlist.GetAt(k);
					g_dbcommand.Param("person").setAsString() = personlist.GetAt(k);
					g_dbcommand.Param("optime").setAsDateTime() = optimelist.GetAt(k);
					g_dbcommand.Execute();
					
					if( g_dbcommand.FetchNext() )
					{
						QuickSetText( 0, toprow + k, g_dbcommand.Field("optype").asString() );
						QuickSetText( 1, toprow + k, g_dbcommand.Field("blh").asString() );
						QuickSetText( 2, toprow + k, g_dbcommand.Field("content").asString() );
						COleDateTime curt;
						SADateTime st = g_dbcommand.Field("optime").asDateTime();
						curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
						QuickSetText( 3, toprow + k, curt.Format("%Y-%m-%d %H:%M:%S") );
						QuickSetText( 4, toprow + k, g_dbcommand.Field("person").asString() );
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
			
			optypelist.RemoveAll();
			blhlist.RemoveAll();
			personlist.RemoveAll();
			optimelist.RemoveAll();
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
			SAString cmdstr = "select optype, blh, optime, person into ##mm" + theApp.m_LocalIp + " from " + theApp.TABLE_LOGO + m_WhereBetween_Cmd + m_OrderBy_Cmd;
			g_dbcommand.setCommandText(cmdstr);
			
			g_dbcommand.Execute();
			
			cmdstr = "select optype, blh, optime, person, IDENTITY(int,0,1) as Num into ##qq" + theApp.m_LocalIp + " from ##mm" + theApp.m_LocalIp;
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
			
			cmdstr = "select optype, blh, optime, person from ##qq" + theApp.m_LocalIp + " where Num >= :Num1 and Num <= :Num2";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("Num1").setAsLong() = toprow;
			g_dbcommand.Param("Num2").setAsLong() = bottomrow;
			g_dbcommand.Execute();
			
			CStringArray optypelist, blhlist, personlist;
			CArray<SADateTime,SADateTime> optimelist;
			optypelist.RemoveAll();
			blhlist.RemoveAll();
			personlist.RemoveAll();
			optimelist.RemoveAll();
			
			CString pp;
			int rowno = toprow;
			while( g_dbcommand.FetchNext() )
			{
				pp = QuickGetText(0, rowno);
				pp.TrimLeft();  pp.TrimRight();
				if( !pp.IsEmpty() ) 
				{
					rowno++;
					
					optypelist.Add("");
					blhlist.Add("");
					personlist.Add("");
					optimelist.Add(ServerTime);
					continue;
				}
				
				if(rowno == row)   cell->SetText(g_dbcommand.Field("optype").asString());
				
				QuickSetText(0, rowno, g_dbcommand.Field("optype").asString());
				
				optypelist.Add(g_dbcommand.Field("optype").asString());
				blhlist.Add(g_dbcommand.Field("blh").asString());
				personlist.Add(g_dbcommand.Field("person").asString());
				optimelist.Add(g_dbcommand.Field("optime").asDateTime());
				
				rowno++;
			}
			
			cmdstr = "drop table ##mm" + theApp.m_LocalIp;
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Execute();
			
			cmdstr = "drop table ##qq" + theApp.m_LocalIp;
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Execute();
			
			CString mno;
			for(int k = 0; k < optypelist.GetSize(); k++ )
			{
				try
				{
					cmdstr = "Select * from " + theApp.TABLE_LOGO + " where optype=:optype and blh=:blh and optime=:optime and person=:person";
					g_dbcommand.setCommandText( cmdstr );
					g_dbcommand.Param("optype").setAsString() = optypelist.GetAt(k);
					g_dbcommand.Param("blh").setAsString() = blhlist.GetAt(k);
					g_dbcommand.Param("person").setAsString() = personlist.GetAt(k);
					g_dbcommand.Param("optime").setAsDateTime() = optimelist.GetAt(k);
					g_dbcommand.Execute();
					
					if( g_dbcommand.FetchNext() )
					{
						QuickSetText( 0, toprow + k, g_dbcommand.Field("optype").asString() );
						QuickSetText( 1, toprow + k, g_dbcommand.Field("blh").asString() );
						QuickSetText( 2, toprow + k, g_dbcommand.Field("content").asString() );
						COleDateTime curt;
						SADateTime st = g_dbcommand.Field("optime").asDateTime();
						curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
						QuickSetText( 3, toprow + k, curt.Format("%Y-%m-%d %H:%M:%S") );
						QuickSetText( 4, toprow + k, g_dbcommand.Field("person").asString() );
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
			
			optypelist.RemoveAll();
			blhlist.RemoveAll();
			personlist.RemoveAll();
			optimelist.RemoveAll();
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

int  CLogoGrid::OnCanSizeRow(long row)
{
	return FALSE;
}

int CLogoGrid::OnCanSizeTopHdg()
{
	return FALSE;
}

int CLogoGrid::OnCanSizeSideHdg()
{
	return FALSE;
}

void CLogoGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	if(!(col == 2 && row >= 0 && row < GetNumberRows())) return;

	CTextOut mydlg(this);
	mydlg.m_Text = QuickGetText(2, row);
	mydlg.DoModal();
}
