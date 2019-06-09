// BdyyGrid.cpp: implementation of the CBdyyGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "BdyyGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CBdyyGrid::CBdyyGrid()
{
	m_WhereBetween_Cmd.Format(" where (state = '1' or state = '2') and bdjg is null");
	m_OrderBy_Cmd.Format(" order by yysjsj asc");
}

CBdyyGrid::~CBdyyGrid()
{

}

void CBdyyGrid::ResetGridSize()
{
	int row_num = 0;
	int col_num = 10;

	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_BOOKING + m_WhereBetween_Cmd;
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

	try
	{
		SAString cmdstr = "Select * from " + theApp.TABLE_BOOKING + m_WhereBetween_Cmd + m_OrderBy_Cmd;
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Execute();
		
		int index = 0;
		while( g_dbcommand.FetchNext() )
		{
			QuickSetText(0, index , g_dbcommand.Field("sqh").asString());
			QuickSetText(1, index , g_dbcommand.Field("sssbh").asString());
			SADateTime st = g_dbcommand.Field("yysjsj").asDateTime();
			COleDateTime ole;
			ole.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
						
			QuickSetText(2, index , ole.Format("%Y-%m-%d %H:%M:%S"));
			QuickSetText(3, index , g_dbcommand.Field("xm").asString());
			QuickSetText(4, index , g_dbcommand.Field("xb").asString());
			QuickSetText(5, index , g_dbcommand.Field("nl").asString());
			QuickSetText(7, index , g_dbcommand.Field("yyhz").asString());
			QuickSetText(8, index , g_dbcommand.Field("lxdh").asString());
			CString str = g_dbcommand.Field("state").asString();
			if(str.Compare("1")==0)  str = "已预约";
			if(str.Compare("2")==0)  str = "已回执";
			if(str.Compare("3")==0)  str = "已接收";
			if(str.Compare("4")==0)  str = "已完成";
			QuickSetText(9, index , str);
			
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

	for(int i = 0; i < row_num; i++)
	{
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_BOOKINGSAMPLE + " where sqh = :sqh";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("sqh").setAsString() = QuickGetText(0, i);
			g_dbcommand.Execute();

			CString bbmc; bbmc.Empty();
			while( g_dbcommand.FetchNext() )
			{
				if(!g_dbcommand.Field("jpbw").isNull() && !g_dbcommand.Field("jpbw").asString().IsEmpty())
				{
					if(!bbmc.IsEmpty())  bbmc += "、";
					bbmc += g_dbcommand.Field("jpbw").asString();
				}
			}
			QuickSetText(6, i , bbmc);
			
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

    for(i = 0 ; i < col_num; i++ )
	{
		SetColWidth(i, 75);
	}

	if( row_num > 0 )
	{
		//GotoRow(0);
	}
}

void CBdyyGrid::OnSetup()
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

	SetCurrentCellMode(2);
}

void CBdyyGrid::OnGetCell(int col,long row,CUGCell *cell)
{
	if( row == -1 )     //  Display table head information of pRecordSet_Define
	{
		if( col >= 0 )
		{
			switch(col) {
			case 0:
				cell->SetText( "申请号" );
				break;
			case 1:
				cell->SetText( "手术室编号" );
				break;
			case 2:
				cell->SetText( "预约时间" );
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
				cell->SetText( "标本名称" );
				break;
			case 7:
				cell->SetText( "预约回执" );
				break;
			case 8:
				cell->SetText( "联系电话" );
				break;
			case 9:
				cell->SetText( "执行状态" );
				break;
			}
		}
		return;
	}

}

int  CBdyyGrid::OnCanSizeRow(long row)
{
	return FALSE;
}

int CBdyyGrid::OnCanSizeTopHdg()
{
	return FALSE;
}

int CBdyyGrid::OnCanSizeSideHdg()
{
	return FALSE;
}

void CBdyyGrid::OnDClicked(int col, long row, RECT *rect, POINT *point, BOOL processed)
{
	if(row >= 0 && col == 1)
	{
		StartEdit();
	}
}

int CBdyyGrid::OnEditFinish(int col, long row, CWnd *edit, LPCTSTR string, BOOL cancelFlag)
{
	if(row >= 0 && col == 1)
	{
		try
		{
			SAString cmdstr = "update " + theApp.TABLE_BOOKING + " set sssbh=:sssbh where sqh=:sqh";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("sssbh").setAsString() = string;
			g_dbcommand.Param("sqh").setAsString() = QuickGetText(0,row);
			g_dbcommand.Execute();
			g_dbconnection.Commit();
		}
		catch (SAException& e)
		{
			g_dbconnection.Rollback();
			AfxMessageBox((const char*)e.ErrText());
			return FALSE;
		}
	}
	return TRUE;
}
