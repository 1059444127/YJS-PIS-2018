// SliceQualityGrid.cpp: implementation of the CSliceQualityGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "PathologyView.h"
#include "PathologyDoc.h"
#include "SliceQualityGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CSliceQualityGrid::CSliceQualityGrid()
{

}

CSliceQualityGrid::~CSliceQualityGrid()
{

}

void CSliceQualityGrid::OnSetup()
{
	m_font.CreateFont(16,0,0,0,500,0,0,0,0,0,0,0,0,"Arial");
	SetDefFont(&m_font);

    ResetGridSize();

	SetSH_NumberCols(0);
	SetSH_Width(0);

	SetVScrollMode( UG_SCROLLTRACKING );
	SetHScrollMode( UG_SCROLLTRACKING );

	SetMultiSelectMode(TRUE);
	SetHighlightRow(TRUE);

	SetDoubleBufferMode(TRUE);

	SetDoubleBufferMode(TRUE);

	SetCurrentCellMode(2);
}

int CSliceQualityGrid::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{

	if(col == 5 && row >= 0)
	{
		if(msg == UGCT_DROPLISTSELECT)
		{
			CString *string = (CString*)param;

			theApp.ConnectDatabase();

			CString qptmh = QuickGetText(3 ,row);
			try
			{
				SAString cmdstr = "Update " + theApp.TABLE_SLICE + " set qppj = :qppj, qppjr = :qppjr where medicalno = :medicalno and qptmh = :qptmh";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
				g_dbcommand.Param("qptmh").setAsString()     = qptmh;
				g_dbcommand.Param("qppj").setAsString()      = *string;  
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
				return FALSE;
			}
		}
	}
	
	return TRUE;
}

void CSliceQualityGrid::ResetGridSize()
{
	theApp.ConnectDatabase();
	
	int row_num = 0;
	if(!theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		try
		{
			SAString cmdstr;
			if(theApp.m_QpxxMode.CompareNoCase("是") == 0)
				cmdstr = "Select count(*) from " + theApp.TABLE_SLICE + " where medicalno = :medicalno";
			else
				cmdstr = "Select count(*) from " + theApp.TABLE_SLICE + " where medicalno = :medicalno and bphd IS NOT NULL";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
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
	}


	SetNumberRows(row_num);
	SetNumberCols(16);

	for(int i = 0; i < row_num; i++ )
	{
		QuickSetCellType(5,i,UGCT_DROPLIST);
		QuickSetCellTypeEx(5,i,UGCT_DROPLISTHIDEBUTTON);
		QuickSetLabelText(5,i,"Ⅰ级\nⅡ级\nⅢ级\n");
	}

	if(!theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		try
		{
			SAString cmdstr;
			if(theApp.m_QpxxMode.CompareNoCase("是") == 0)
				cmdstr = "Select * from " + theApp.TABLE_SLICE + " where medicalno = :medicalno order by qptmh asc";
			else
				cmdstr = "Select * from " + theApp.TABLE_SLICE + " where medicalno = :medicalno and bphd IS NOT NULL order by qptmh asc";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Execute();
			
			int index = 0;
			while( g_dbcommand.FetchNext() )
			{
				QuickSetText(0, index , g_dbcommand.Field("medicalno").asString());
				QuickSetText(1, index , g_dbcommand.Field("rwly").asString());
				QuickSetText(2, index , g_dbcommand.Field("lkh").asString());
				QuickSetText(3, index , g_dbcommand.Field("qptmh").asString());
				QuickSetText(4, index , g_dbcommand.Field("qpsm").asString());
				QuickSetText(5, index , g_dbcommand.Field("qppj").asString());
				QuickSetText(6, index , g_dbcommand.Field("qppjr").asString());
				QuickSetText(7, index , g_dbcommand.Field("other").asString());
				//QuickSetText(8, index , "已完成");
			/*	if(g_dbcommand.Field("dyzt").asBool())
				{
					QuickSetText(8, index , "已完成");
				}
				else
				{
					QuickSetText(8, index , "执行中");
				}   */ 
				if(g_dbcommand.Field("bphd").asString().IsEmpty()){
					QuickSetText(8,index,"执行中");
				} else {
					QuickSetText(8,index,"已完成");
				}
				QuickSetText(9, index , g_dbcommand.Field("qpczy").asString());

				COleDateTime curt;
				SADateTime st;
				if(g_dbcommand.Field("qprq").isNull())
				{
					QuickSetText(10, index , " ");
				}
				else
				{
					st = g_dbcommand.Field("qprq").asDateTime();
					curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
					QuickSetText(10, index , curt.Format("%Y-%m-%d"));
				}


				QuickSetText(11, index , g_dbcommand.Field("bphd").asString());

				if(g_dbcommand.Field("hdrq").isNull())
				{
					QuickSetText(12, index , " ");
				}
				else
				{
					st = g_dbcommand.Field("hdrq").asDateTime();
					curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
					QuickSetText(12, index , curt.Format("%Y-%m-%d"));
				}

				QuickSetText(13, index, g_dbcommand.Field("qpzt").asString());
				QuickSetText(14, index, g_dbcommand.Field("qpgdr").asString());
				QuickSetText(15, index, g_dbcommand.Field("qpcfwz").asString());

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
	}

	for(i = 0; i < GetNumberCols(); i++)  SetColWidth(i, 85);
	SetColWidth(0, 0);

//	QuickSetTextColor(5,-1,RGB(255,0,0));
}

void CSliceQualityGrid::OnGetCell(int col,long row,CUGCell *cell)
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
				cell->SetText( "蜡块号" );
				break;
			case 3:
				cell->SetText( "切片条码号" );
				break;
			case 4:
				cell->SetText( "切片说明" );
				break;
			case 5:
				cell->SetText( "切片评级" );
				break;
			case 6:
				cell->SetText( "评级人" );
				break;
			case 7:
				cell->SetText( "备注" );
				break;
			case 8:
				cell->SetText( "切片状态" );
				break;
			case 9:
				cell->SetText( "切片操作员" );
				break;
			case 10:
				cell->SetText( "切片时间" );
				break;
			case 11:
				cell->SetText( "玻片核对" );
				break;
			case 12:
				cell->SetText( "核对时间" );
				break;
			case 13:
				cell->SetText( "归档状态" );
				break;
			case 14:
				cell->SetText( "归档人" );
				break;
			case 15:
				cell->SetText( "存放位置" );
				break;
			}
		}
		return;
	}
}

int  CSliceQualityGrid::OnCanSizeRow(long row)
{
	return FALSE;
}

int  CSliceQualityGrid::OnCanSizeCol(int col)
{
	return TRUE;
}

void CSliceQualityGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	if(row >= 0 && col == 7)
	{
		StartEdit();
	}
}

void CSliceQualityGrid::OnCharDown(UINT *vcKey,BOOL processed)
{
	if(GetCurrentRow() >= 0 && GetCurrentCol() == 7)
	{
		StartEdit(*vcKey);
	}
}

int CSliceQualityGrid::OnEditFinish(int col, long row,CWnd * edit, LPCTSTR string,BOOL cancelFlag)
{
	if(cancelFlag)
	{
		QuickSetText(col,row,string);
	}
	if(row >= 0 && col == 7)
	{
		CString  qptmh = QuickGetText(3, row);

		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_SLICE + " set other = :other where qptmh = :qptmh";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("qptmh").setAsString() = qptmh;
			g_dbcommand.Param("other").setAsString() = string;  

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
