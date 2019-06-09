// AdviceGrid.cpp: implementation of the CAdviceGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "PathologyView.h"
#include "PathologyDoc.h"
#include "AdviceGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CAdviceGrid::CAdviceGrid()
{

}

CAdviceGrid::~CAdviceGrid()
{

}

void CAdviceGrid::OnSetup()
{
	m_font.CreateFont(16,0,0,0,500,0,0,0,0,0,0,0,0,"Arial");
	SetDefFont(&m_font);

    ResetGridSize();

	SetSH_NumberCols(0);
	SetSH_Width(0);

	SetVScrollMode( UG_SCROLLTRACKING );
	SetHScrollMode( UG_SCROLLTRACKING );

	SetHighlightRow(TRUE);

	SetDoubleBufferMode(TRUE);

	SetDoubleBufferMode(TRUE);

	SetCurrentCellMode(2);
}

void CAdviceGrid::ResetGridSize()
{
	theApp.ConnectDatabase();
	
	int row_num = 0;
	if(!theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		try
		{
			SAString cmdstr = "Select count(*) from " + theApp.TABLE_ADVICE + " where medicalno = :medicalno and (yzlx = '常规' or yzlx = '补取' or yzlx = '重切' or yzlx = '深切' or yzlx = '薄切' or yzlx = '连切' or yzlx = '切全' or yzlx = '不削连切')";
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
	SetNumberCols(10);

	if(!theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_ADVICE + " where medicalno = :medicalno and (yzlx = '常规' or yzlx = '补取' or yzlx = '重切' or yzlx = '深切' or yzlx = '薄切' or yzlx = '连切' or yzlx = '切全' or yzlx = '不削连切') order by yzh asc";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Execute();
			
			int index = 0;
			while( g_dbcommand.FetchNext() )
			{
				QuickSetText(0, index , g_dbcommand.Field("yzh").asString());
				QuickSetText(1, index , g_dbcommand.Field("yzlx").asString());
				QuickSetText(2, index , g_dbcommand.Field("lkh").asString());
				CString str;
				str.Format("%d", g_dbcommand.Field("qps").asLong());
				QuickSetText(3, index , str);
				QuickSetText(4, index , g_dbcommand.Field("sqys").asString());

				COleDateTime curt;
				SADateTime st = g_dbcommand.Field("sqrq").asDateTime();
				curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());

				QuickSetText(5, index , curt.Format("%Y-%m-%d"));
				
				if(g_dbcommand.Field("yzzt").asBool())
				{
					QuickSetText(6, index , "已执行");
				}
				else
				{
					QuickSetText(6, index , "未执行");
				}

				QuickSetText(7, index , g_dbcommand.Field("bz").asString());

				QuickSetText(8, index , g_dbcommand.Field("zxr").asString());

				st = g_dbcommand.Field("zxrq").asDateTime();
				curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());

				if(g_dbcommand.Field("zxrq").isNull())
					QuickSetText(9, index , " ");
				else
					QuickSetText(9, index , curt.Format("%Y-%m-%d"));
				
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

	for(int i = 0; i < 10; i++)  SetColWidth(i, 80);
}

void CAdviceGrid::OnGetCell(int col,long row,CUGCell *cell)
{
	if( row == -1 )     //  Display table head information of pRecordSet_Define
	{
		if( col >= 0 )
		{
			switch(col) {
			case 0:
				cell->SetText( "医嘱号" );
				break;
			case 1:
				cell->SetText( "类型" );
				break;
			case 2:
				cell->SetText( "蜡块号" );
				break;
			case 3:
				cell->SetText( "数量" );
				break;
			case 4:
				cell->SetText( "申请医生" );
				break;
			case 5:
				cell->SetText( "申请日期" );
				break;
			case 6:
				cell->SetText( "状态" );
				break;
			case 7:
				cell->SetText( "备注" );
				break;
			case 8:
				cell->SetText( "执行人" );
				break;
			case 9:
				cell->SetText( "执行时间" );
				break;
			}
		}
		return;
	}
}

int  CAdviceGrid::OnCanSizeRow(long row)
{
	return FALSE;
}

int  CAdviceGrid::OnCanSizeCol(int col)
{
	return TRUE;
}

void CAdviceGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	if((col == 2 || col == 3 || col == 4) && row >= 0)
	{
		CString CheckState = QuickGetText(6, row);
		if(CheckState.CompareNoCase("未执行") == 0)
		{
			StartEdit();
		}
	}
}

int CAdviceGrid::OnEditFinish(int col, long row,CWnd * edit, LPCTSTR string,BOOL cancelFlag)
{
	if(cancelFlag)
	{
		QuickSetText(col,row,string);
	}

	if((col == 2 || col == 3 || col == 4) && row >= 0)
	{
		CString CheckState = QuickGetText(6, row);
		if(CheckState.CompareNoCase("未执行") == 0)
		{
			theApp.ConnectDatabase();
			
			CString yzh = QuickGetText(0 ,row);
			
			SAString cmdstr;
			try
			{
				switch(col) {
				case 2:
					cmdstr = "Update " + theApp.TABLE_ADVICE + " set lkh=:lkh where medicalno = :medicalno and yzh = :yzh and yzzt = 0";
					break;
				case 3:
					cmdstr = "Update " + theApp.TABLE_ADVICE + " set qps=:qps where medicalno = :medicalno and yzh = :yzh and yzzt = 0";
					break;
				case 4:
					cmdstr = "Update " + theApp.TABLE_ADVICE + " set sqys=:sqys where medicalno = :medicalno and yzh = :yzh and yzzt = 0";
					break;
				}
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
				g_dbcommand.Param("yzh").setAsString()       = yzh;
				switch(col) {
				case 2:
					g_dbcommand.Param("lkh").setAsString()  = string;
					break;
				case 3:
					g_dbcommand.Param("qps").setAsLong()    = atoi(string);
					break;
				case 4:
					g_dbcommand.Param("sqys").setAsString() = string;
					break;
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
				return FALSE;
			}
		}
	}
	
	return TRUE;
}
