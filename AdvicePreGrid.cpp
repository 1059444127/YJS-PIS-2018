// AdvicePreGrid.cpp: implementation of the CAdvicePreGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "PathologyView.h"
#include "PathologyDoc.h"
#include "AdvicePreGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CAdvicePreGrid::CAdvicePreGrid()
{

}

CAdvicePreGrid::~CAdvicePreGrid()
{

}

void CAdvicePreGrid::OnSetup()
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

void CAdvicePreGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	if((col == 5 || col == 10 )&& row >= 0)
	{
		StartEdit();
	}

	if((col == 2 || col == 3 || col == 6) && row >= 0)
	{
		CString CheckState = QuickGetText(8, row);
		if(CheckState.CompareNoCase("未执行") == 0)
		{
			StartEdit();
		}
	}
}

void CAdvicePreGrid::OnCharDown(UINT *vcKey,BOOL processed)
{
	if(GetCurrentCol() == 5 && GetCurrentRow() >= 0)
	{
		StartEdit(*vcKey);
	}
}

int CAdvicePreGrid::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{

	if(col == 5 && row >= 0)
	{
		if(msg == UGCT_DROPLISTSELECT)
		{
			CString *string = (CString*)param;

			theApp.ConnectDatabase();

			CString yzh = QuickGetText(0 ,row);
			try
			{
				SAString cmdstr = "Update " + theApp.TABLE_ADVICE + " set bx = :bx where medicalno = :medicalno and yzh = :yzh";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
				g_dbcommand.Param("yzh").setAsString()       = yzh;
				g_dbcommand.Param("bx").setAsString()        = *string;  
				
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

int CAdvicePreGrid::OnEditFinish(int col, long row,CWnd * edit, LPCTSTR string,BOOL cancelFlag)
{
	if(cancelFlag)
	{
		QuickSetText(col,row,string);
	}
	if((col == 5 || col == 10) && row >= 0)
	{
		theApp.ConnectDatabase();
		
		CString yzh = QuickGetText(0 ,row);
		
		try
		{
			SAString cmdstr;
			if(col==5){
				
			    cmdstr = "Update " + theApp.TABLE_ADVICE + " set bx = :bx where medicalno = :medicalno and yzh = :yzh";
			}
			if(col == 10){
				
				cmdstr = "Update " + theApp.TABLE_ADVICE + " set bz = :bz where medicalno = :medicalno and yzh = :yzh";
			
			}
			
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Param("yzh").setAsString()       = yzh;
			
			if(col == 5) g_dbcommand.Param("bx").setAsString()        = string;  
			if(col == 10) g_dbcommand.Param("bz").setAsString() = string;
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

	if((col == 2 || col == 3 || col == 6) && row >= 0)
	{
		CString CheckState = QuickGetText(8, row);
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
					cmdstr = "Update " + theApp.TABLE_ADVICE + " set bjw=:bjw where medicalno = :medicalno and yzh = :yzh and yzzt = 0";
					break;
				case 6:
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
					g_dbcommand.Param("bjw").setAsString()  = string;
					break;
				case 6:
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

void CAdvicePreGrid::ResetGridSize()
{
	theApp.ConnectDatabase();
	
	int row_num = 0;
	if(!theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		try
		{
			SAString cmdstr;
		//	if(theApp.m_CantainTsrs.CompareNoCase("否") == 0){
		//		 cmdstr = "Select count(*) from " + theApp.TABLE_ADVICE + " where medicalno = :medicalno and (yzlx = '免疫组化' or yzlx = '分子病理' or yzlx = '电镜')";
		//	} else {
				 cmdstr = "Select count(*) from " + theApp.TABLE_ADVICE + " where medicalno = :medicalno and (yzlx = '免疫组化' or yzlx = '特殊染色' or yzlx = '分子病理' or yzlx = '电镜' or yzlx = '其它')";
		//	}
			
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
	SetNumberCols(13);

	for(int i = 0; i < row_num; i++ )
	{
		QuickSetCellType(5,i,UGCT_DROPLIST);
		QuickSetCellTypeEx(5,i,UGCT_DROPLISTHIDEBUTTON);
		QuickSetLabelText(5,i,"－\n+\n++\n+++\n±\n+＜25%\n+25～50%\n+50～75%\n+＞75%\n");
	}

	if(!theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		try
		{
			SAString cmdstr;
		//	if(theApp.m_CantainTsrs.CompareNoCase("否") == 0){
		//		 cmdstr = "Select * from " + theApp.TABLE_ADVICE + " where medicalno = :medicalno and (yzlx = '免疫组化' or yzlx = '分子病理' or yzlx = '电镜') order by yzh asc";
		//	} else {
				 cmdstr = "Select * from " + theApp.TABLE_ADVICE + " where medicalno = :medicalno and (yzlx = '免疫组化' or yzlx = '特殊染色' or yzlx = '分子病理' or yzlx = '电镜' or yzlx = '其它') order by yzh asc";
		//	}
			//SAString cmdstr = "Select * from " + theApp.TABLE_ADVICE + " where medicalno = :medicalno and (yzlx = '免疫组化' or yzlx = '特殊染色' or yzlx = '分子病理' or yzlx = '电镜') order by yzh asc";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Execute();
			
			int index = 0;
			while( g_dbcommand.FetchNext() )
			{
				QuickSetText(0, index , g_dbcommand.Field("yzh").asString());
				QuickSetText(1, index , g_dbcommand.Field("yzlx").asString());
				QuickSetText(2, index , g_dbcommand.Field("lkh").asString());
				QuickSetText(3, index , g_dbcommand.Field("bjw").asString());
				CString str;
				str.Format("%d", g_dbcommand.Field("qps").asLong());
				QuickSetText(4, index , str);
				QuickSetText(5, index , g_dbcommand.Field("bx").asString());
				QuickSetText(6, index , g_dbcommand.Field("sqys").asString());

				COleDateTime curt;
				SADateTime st = g_dbcommand.Field("sqrq").asDateTime();
				curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());

				QuickSetText(7, index , curt.Format("%Y-%m-%d"));
				
				if(g_dbcommand.Field("yzzt").asBool())
				{
					QuickSetText(8, index , "已执行");
				}
				else
				{
					QuickSetText(8, index , "未执行");
				}

				QuickSetText(9,  index , g_dbcommand.Field("fxjg").asString());

				QuickSetText(10, index , g_dbcommand.Field("bz").asString());

				QuickSetText(11, index , g_dbcommand.Field("zxr").asString());

				st = g_dbcommand.Field("zxrq").asDateTime();
				curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
				if(g_dbcommand.Field("zxrq").isNull())
					QuickSetText(12, index , " ");
				else
					QuickSetText(12, index , curt.Format("%Y-%m-%d"));
				
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

	for(i = 0; i < GetNumberCols(); i++)  SetColWidth(i, 75);
	SetColWidth(0, 0);
	SetColWidth(5, 100);

	QuickSetTextColor(5,-1,RGB(255,0,0));
}

void CAdvicePreGrid::OnGetCell(int col,long row,CUGCell *cell)
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
				cell->SetText( "标记物" );
				break;
			case 4:
				cell->SetText( "数量" );
				break;
			case 5:
				cell->SetText( "分析结果" );
				break;
			case 6:
				cell->SetText( "申请医生" );
				break;
			case 7:
				cell->SetText( "申请日期" );
				break;
			case 8:
				cell->SetText( "状态" );
				break;
			case 9:
				cell->SetText( "收费情况" );
				break;
			case 10:
				cell->SetText( "备注" );
				break;
			case 11:
				cell->SetText( "执行人" );
				break;
			case 12:
				cell->SetText( "执行时间" );
				break;
			}
		}
		return;
	}
}

int  CAdvicePreGrid::OnCanSizeRow(long row)
{
	return FALSE;
}

int  CAdvicePreGrid::OnCanSizeCol(int col)
{
	if(col== 0)  return FALSE;
	return TRUE;
}

