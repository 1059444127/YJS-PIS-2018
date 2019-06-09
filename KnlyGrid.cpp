// KnlyGrid.cpp: implementation of the CKnlyGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "KnlyGrid.h"
#include "TextOut.h"
#include "NotePad.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CKnlyGrid::CKnlyGrid()
{
	m_WhereBetween_Cmd.Format(" where jsr = \'%s\' or fbr = \'%s\'", theApp.m_Cpp_User, theApp.m_Cpp_User);
	m_OrderBy_Cmd.Format(" order by fbsj desc");
}

CKnlyGrid::~CKnlyGrid()
{

}

void CKnlyGrid::ResetGridSize()
{
	theApp.ConnectDatabase();

	int row_num = 0;
	int col_num = 5;

	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_NOTEPAD + m_WhereBetween_Cmd;
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


	for(i = 0 ; i < col_num ; i++ )
	{
		if(i == 0)
		{
			SetColWidth(i, 150);
		}
		else if(i == 1 || i ==3)
		{
			SetColWidth(i,80);
		}
		else if(i == 2)
		{
			SetColWidth(i,290);
		}
		else if(i == 4)
		{
			SetColWidth(i, 50);
		}
		
	}	

	if( row_num > 0 )
	{
		//GotoRow(0);
	}
	
	CUGCell defcell;
	GetGridDefault(&defcell);

	try
	{
		SAString cmdstr = "Select * from " + theApp.TABLE_NOTEPAD + m_WhereBetween_Cmd + m_OrderBy_Cmd;
		g_dbcommand.setCommandText( cmdstr );
		g_dbcommand.Execute();
		
		int rowindex = 0;
		while ( g_dbcommand.FetchNext() )
		{
			COleDateTime time;
			SADateTime st = g_dbcommand.Field("fbsj").asDateTime();
			time.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
			QuickSetText( 0, rowindex, time.Format("%Y-%m-%d %H:%M:%S") );
			QuickSetText( 1, rowindex, g_dbcommand.Field("fbr").asString() );
			QuickSetText( 2, rowindex, g_dbcommand.Field("sx").asString() );
			QuickSetText( 3, rowindex, g_dbcommand.Field("jsr").asString() );
			
			bool zt = g_dbcommand.Field("zt").asBool();
			if(zt)
			{
				QuickSetText( 4, rowindex, "已读" );

				for(i = 0; i < col_num; i++)
				{
					CUGCell cell;
					GetCell(i, rowindex,&cell);
					cell.ClearPropertySetFlag(UGCELL_BACKCOLOR_SET);
					cell.SetBackColor(defcell.GetBackColor());
					SetCell(i, rowindex,&cell);
				}
			}
			else
			{
				QuickSetText( 4, rowindex, "未读" );

				for(i = 0; i < col_num; i++)
				{
					QuickSetBackColor(i, rowindex, RGB(255,255,0));
				}
			}

			rowindex++;
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

void CKnlyGrid::OnSetup()
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

	AddMenuItem(1001,"删除留言");
	AddMenuItem(1002,"回复留言");
	EnableMenu(1);	

	SetCurrentCellMode(2);
}

void CKnlyGrid::OnMenuCommand(int col,long row,int section,int item)
{
    if(col < 0 || col >= GetNumberCols()) return;

    if( item == 1001 ) // 1001 - 删除留言
	{
		if(row >= 0 && row < GetNumberRows())
		{
			CString str      = QuickGetText(0, row);
			CString str_Fbr  = QuickGetText(1, row);
			CString str_Jsr  = QuickGetText(3, row);

			try
			{
				SAString cmdstr = "Delete from " + theApp.TABLE_NOTEPAD + " where fbsj >= :fbsj1 and fbsj<=:fbsj2 and fbr = :fbr and jsr = :jsr";
				g_dbcommand.setCommandText(cmdstr);
				
				int year = atoi(str.Left(str.Find("-")));
				str = str.Right(str.GetLength() - str.Find("-") - 1);
				
				int month = atoi(str.Left(str.Find("-")));
				str = str.Right(str.GetLength() - str.Find("-") - 1);
				
				str.TrimLeft();
				str += " ";
				int day = atoi(str.Left(str.Find(" ")));
				str = str.Right(str.GetLength() - str.Find(" ") - 1);
				
				int hour = atoi(str.Left(str.Find(":")));
				str = str.Right(str.GetLength() - str.Find(":") - 1);
				
				int minute = atoi(str.Left(str.Find(":")));
				str = str.Right(str.GetLength() - str.Find(":") - 1);
				
				int second = atoi( str );
				
				SADateTime start(year,month,day,hour,minute,second);
				COleDateTimeSpan tspan;
				tspan.SetDateTimeSpan(0, 0, 0, 1);
				SADateTime end = start + tspan;
				
				g_dbcommand.Param("fbsj1").setAsDateTime() = start;
				g_dbcommand.Param("fbsj2").setAsDateTime() = end;
				g_dbcommand.Param("fbr").setAsString()    = str_Fbr;
				g_dbcommand.Param("jsr").setAsString()    = str_Jsr;  
				
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

			ResetGridSize();
			RedrawAll();
		}
		return;
	}

	if(item == 1002)
	{
		CString str_Fbr  = QuickGetText(1, row);
		CString str_Jsr  = QuickGetText(3, row);
		if(theApp.m_Cpp_User.CompareNoCase(str_Jsr) == 0)
		{
			CNotePad mydlg;
			mydlg.status = str_Fbr;
			mydlg.DoModal();
		}
	}
}

void CKnlyGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	if(row < 0 || row >= GetNumberRows() )  return;
	
		CString str      = QuickGetText(0, row);
		CString str_Fbr  = QuickGetText(1, row);
		CString str_Sx   = QuickGetText(2, row);
		CString str_Jsr  = QuickGetText(3, row);
		CString str_Zt   = QuickGetText(4, row);

		if(!theApp.m_Cpp_User.CompareNoCase(str_Jsr) == 0)
		{
			return;
		}

		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_NOTEPAD + " set zt = 1 where fbsj >= :fbsj1 and fbsj<=:fbsj2 and fbr = :fbr and jsr = :jsr";
			g_dbcommand.setCommandText(cmdstr);
			
			int year = atoi(str.Left(str.Find("-")));
			str = str.Right(str.GetLength() - str.Find("-") - 1);
			
			int month = atoi(str.Left(str.Find("-")));
			str = str.Right(str.GetLength() - str.Find("-") - 1);
			
			str.TrimLeft();
			str += " ";
			int day = atoi(str.Left(str.Find(" ")));
			str = str.Right(str.GetLength() - str.Find(" ") - 1);
			
			int hour = atoi(str.Left(str.Find(":")));
			str = str.Right(str.GetLength() - str.Find(":") - 1);
			
			int minute = atoi(str.Left(str.Find(":")));
			str = str.Right(str.GetLength() - str.Find(":") - 1);
			
			int second = atoi( str );
			
			SADateTime start(year,month,day,hour,minute,second);
			COleDateTimeSpan tspan;
			tspan.SetDateTimeSpan(0, 0, 0, 1);
			SADateTime end = start + tspan;
			
			g_dbcommand.Param("fbsj1").setAsDateTime() = start;
			g_dbcommand.Param("fbsj2").setAsDateTime() = end;
			g_dbcommand.Param("fbr").setAsString()    = str_Fbr;
			g_dbcommand.Param("jsr").setAsString()    = str_Jsr;  
			
			g_dbcommand.Execute();
			g_dbconnection.Commit();
		} 
		catch (SAException &e)
		{
			try
			{
				g_dbconnection.Rollback();
			} 
			catch (SAException &e1)
			{

			}
			AfxMessageBox((const char*)e.ErrText());
		}
		ResetGridSize();
		RedrawAll();
		
	
	if(col == 2)
	{		
		CString str_Sx   = QuickGetText(2, row);
	/*	CString str      = QuickGetText(0, row);
		CString str_Fbr  = QuickGetText(1, row);
		CString str_Sx   = QuickGetText(2, row);
		CString str_Jsr  = QuickGetText(3, row);
		CString str_Zt   = QuickGetText(4, row); 
		
		if(str_Zt.CompareNoCase("未读") == 0 && theApp.m_Cpp_User.CompareNoCase(str_Jsr) == 0)
		{
			try
			{
				SAString cmdstr = "Update " + theApp.TABLE_NOTEPAD + " set zt = 1 where fbsj >= :fbsj1 and fbsj<=:fbsj2 and fbr = :fbr and jsr = :jsr";
				g_dbcommand.setCommandText(cmdstr);
				
				int year = atoi(str.Left(str.Find("-")));
				str = str.Right(str.GetLength() - str.Find("-") - 1);
				
				int month = atoi(str.Left(str.Find("-")));
				str = str.Right(str.GetLength() - str.Find("-") - 1);
				
				str.TrimLeft();
				str += " ";
				int day = atoi(str.Left(str.Find(" ")));
				str = str.Right(str.GetLength() - str.Find(" ") - 1);
				
				int hour = atoi(str.Left(str.Find(":")));
				str = str.Right(str.GetLength() - str.Find(":") - 1);
				
				int minute = atoi(str.Left(str.Find(":")));
				str = str.Right(str.GetLength() - str.Find(":") - 1);
				
				int second = atoi( str );
				
				SADateTime start(year,month,day,hour,minute,second);
				COleDateTimeSpan tspan;
				tspan.SetDateTimeSpan(0, 0, 0, 1);
				SADateTime end = start + tspan;
				
				g_dbcommand.Param("fbsj1").setAsDateTime() = start;
				g_dbcommand.Param("fbsj2").setAsDateTime() = end;
				g_dbcommand.Param("fbr").setAsString()    = str_Fbr;
				g_dbcommand.Param("jsr").setAsString()    = str_Jsr;  
				
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
			
			ResetGridSize();
			RedrawAll();
		}
		*/
		CTextOut mydlg(this);
		mydlg.m_Text = str_Sx;
		mydlg.DoModal();
	}

	
}

void CKnlyGrid::OnGetCell(int col,long row,CUGCell *cell)
{
	if( row == -1 )     //  Display table head information of pRecordSet_Define
	{
		if( col >= 0 )
		{
			switch(col) 
			{
				case 0:
					cell->SetText( "发布时间" );
					break;
				case 1:
					cell->SetText( "发布人" );
					break;
				case 2:
					cell->SetText( "事项" );
					break;
				case 3:
					cell->SetText( "接收人" );
					break;
				case 4:
					cell->SetText( "状态" );
					break;
			}
		}
		return;
	}
}

int  CKnlyGrid::OnCanSizeRow(long row)
{
	return FALSE;
}

int CKnlyGrid::OnCanSizeTopHdg()
{
	return FALSE;
}

int CKnlyGrid::OnCanSizeSideHdg()
{
	return FALSE;
}

