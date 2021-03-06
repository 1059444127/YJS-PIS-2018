// SflbGrid.cpp: implementation of the CSflbGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "SflbGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CSflbGrid::CSflbGrid()
{
	m_WhereBetween_Cmd.Format(" where jhsfsj IS NOT NULL and jhsfsj > '1949-10-01 00:00:00'");
	if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
	{
		m_WhereBetween_Cmd.Format(" where jhsfsj IS NOT NULL and jhsfsj > to_date('1949-10-01 00:00:00','yyyy-mm-dd hh24:mi:ss')");
	}
	m_OrderBy_Cmd.Format(" order by medicalno asc");
}

CSflbGrid::~CSflbGrid()
{

}

void CSflbGrid::ResetGridSize()
{
	theApp.ConnectDatabase();

	int row_num = 0;
	int col_num = 7;

	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + m_WhereBetween_Cmd;
		g_dbcommand.setCommandText(cmdstr);
		//g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
		
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
		SetColWidth(i, 75);
	}

	SetColWidth(col_num-1, 200);

	if( row_num > 0 )
	{
		//GotoRow(0);
	}
}

void CSflbGrid::OnSetup()
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

	SetCurrentCellMode(1);
}

void CSflbGrid::OnGetCell(int col,long row,CUGCell *cell)
{
	if( row == -1 )     //  Display table head information of pRecordSet_Define
	{
		if( col >= 0 )
		{
			switch(col) {
			case 0:
				cell->SetText( "随访时间" );
				break;
			case 1:
				cell->SetText( "随访医生" );
				break;
			case 2:
				cell->SetText( "病理号" );
				break;
			case 3:
				cell->SetText( "姓名" );
				break;
			case 4:
				cell->SetText( "性别" );
				break;
			case 5:
				cell->SetText( "年龄" );
				break;
			case 6:
				cell->SetText( "诊断意见" );
				break;
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
	
	COleDateTime ServerTime = theApp.GetOleServerDateTime();
	
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
						COleDateTime curt;
						SADateTime st = g_dbcommand.Field("jhsfsj").asDateTime();
						curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
						if(g_dbcommand.Field("jhsfsj").isNull())
							QuickSetText(0, toprow + k , " ");
						else
							QuickSetText(0, toprow + k , curt.Format("%Y-%m-%d"));
						
						QuickSetText( 1, toprow + k, g_dbcommand.Field("sfys").asString() );
						
						QuickSetText( 2, toprow + k, mno );
						QuickSetText( 3, toprow + k, g_dbcommand.Field("xm").asString() );
						QuickSetText( 4, toprow + k, g_dbcommand.Field("xb").asString() );
						
						CString str;
						str.Format("%s %s", g_dbcommand.Field("nl").asString(),g_dbcommand.Field("nldw").asString());
						QuickSetText( 5, toprow + k, str );
						
						QuickSetText( 6, toprow + k, g_dbcommand.Field("zdyj").asString() );
						
						COleDateTime ct = ServerTime;
						ct.SetDateTime(ct.GetYear(),ct.GetMonth(),ct.GetDay(),23,59,59);
						
						if(curt <= ct && !g_dbcommand.Field("jhsfsj").isNull())
						{
							for(int x = 0; x < GetNumberCols(); x++)
								QuickSetBackColor(x, toprow + k,RGB(255,132,132));
						}
						else
						{
							for(int x = 0; x < GetNumberCols(); x++)
								QuickSetBackColor(x, toprow + k,RGB(220,220,220));
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
			//g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
			
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
						COleDateTime curt;
						SADateTime st = g_dbcommand.Field("jhsfsj").asDateTime();
						curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
						if(g_dbcommand.Field("jhsfsj").isNull())
							QuickSetText(0, toprow + k , " ");
						else
							QuickSetText(0, toprow + k , curt.Format("%Y-%m-%d"));
						
						QuickSetText( 1, toprow + k, g_dbcommand.Field("sfys").asString() );
						
						QuickSetText( 2, toprow + k, mno );
						QuickSetText( 3, toprow + k, g_dbcommand.Field("xm").asString() );
						QuickSetText( 4, toprow + k, g_dbcommand.Field("xb").asString() );
						
						CString str;
						str.Format("%s %s", g_dbcommand.Field("nl").asString(),g_dbcommand.Field("nldw").asString());
						QuickSetText( 5, toprow + k, str );
						
						QuickSetText( 6, toprow + k, g_dbcommand.Field("zdyj").asString() );
						
						COleDateTime ct = ServerTime;
						ct.SetDateTime(ct.GetYear(),ct.GetMonth(),ct.GetDay(),23,59,59);
						
						if(curt <= ct && !g_dbcommand.Field("jhsfsj").isNull())
						{
							for(int x = 0; x < GetNumberCols(); x++)
								QuickSetBackColor(x, toprow + k,RGB(255,132,132));
						}
						else
						{
							for(int x = 0; x < GetNumberCols(); x++)
								QuickSetBackColor(x, toprow + k,RGB(220,220,220));
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

int  CSflbGrid::OnCanSizeRow(long row)
{
	return FALSE;
}

int CSflbGrid::OnCanSizeTopHdg()
{
	return FALSE;
}

int CSflbGrid::OnCanSizeSideHdg()
{
	return FALSE;
}

void CSflbGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	if(!(row >= 0 && row < GetNumberRows())) return;
	ClearSelections();
	Select( 0, row );

	CString blh = QuickGetText(2, row);
	theApp.OpenDocumentFile(blh);

	GetParent()->PostMessage(WM_CLOSE);
}
