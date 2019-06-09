// SliceGrid.cpp: implementation of the CSliceGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "PathologyView.h"
#include "MedSlice.h"
#include "SliceGrid.h"
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

CSliceGrid::CSliceGrid()
{
	if(theApp.m_StationType.CompareNoCase("制片工作站") == 0)
	{
		m_WhereBetween_Cmd.Empty();
	}

	if(theApp.m_StationType.CompareNoCase("常规切片工作站") == 0)
	{
		if(theApp.m_CantainTsrs.CompareNoCase("是") == 0)
		{
			m_WhereBetween_Cmd.Format(" where (rwly <> '免疫组化' and rwly <> '分子病理' and rwly <> '电镜' and rwly <> '细胞涂片' and rwly <> '其它')");
		} 
		else 
		{
			m_WhereBetween_Cmd.Format(" where (rwly <> '免疫组化' and rwly <> '特殊染色' and rwly <> '分子病理' and rwly <> '电镜' and rwly <> '细胞涂片' and rwly <> '其它')");
		}
		
	}

	if(theApp.m_StationType.CompareNoCase("特检工作站") == 0)
	{
		if(theApp.m_CantainTsrs.CompareNoCase("否") == 0)
		{
			m_WhereBetween_Cmd.Format(" where (rwly = '免疫组化' or rwly = '特殊染色' or rwly = '分子病理' or rwly = '电镜' or rwly = '其它')");
		} 
		else 
		{
			m_WhereBetween_Cmd.Format(" where (rwly = '免疫组化' or rwly = '分子病理' or rwly = '电镜' or rwly = '其它')");
		}
		
	}

	if(theApp.m_StationType.CompareNoCase("细胞学制片工作站") == 0)
	{
		m_WhereBetween_Cmd.Format(" where (rwly = '细胞涂片')");
	}

	m_OrderBy_Cmd.Format(" order by medicalno desc, qptmh asc");

	m_sortedAssending = FALSE;
}

CSliceGrid::~CSliceGrid()
{

}

void CSliceGrid::ResetGridSize()
{
	theApp.ConnectDatabase();

	int row_num = 0;
	int col_num = 15;

	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_SLICE + m_WhereBetween_Cmd;
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


	char buffer[200], tmp[20];

    _itoa( row_num, tmp, 10 );
	strcpy(buffer, "已制片列表（");
	strcat(buffer, tmp);
	strcat(buffer, "）");
	theApp.GetView()->m_tabCtrl.SetTabText(RUNTIME_CLASS(CMedSlice),  buffer);

	for(int i = 0; i < row_num; i++ )
	{
		QuickSetText( 0, i, "" );
	}

	CString LastBlh; LastBlh.Empty();
	bool UsingBackgroundColor = false;
	int  index = 0;
	CUGCell defcell;
	GetGridDefault(&defcell);
	try
	{
		SAString cmdstr = "Select medicalno from " + theApp.TABLE_SLICE + m_WhereBetween_Cmd + m_OrderBy_Cmd;
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();

		while( g_dbcommand.FetchNext() )
		{
			CString blh = g_dbcommand.Field("medicalno").asString();
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

	for(i = 0 ; i < col_num; i++ )
	{
		SetColWidth(i, 75);
	}

	if( row_num > 0 )
	{
		//GotoRow(0);
	}
}

void CSliceGrid::OnSetup()
{
	m_font.CreateFont(16,0,0,0,500,0,0,0,0,0,0,0,0,"Arial");
	SetDefFont(&m_font);

	m_sArrowIndex = AddCellType(&m_sortArrow);

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

	QuickSetCellType(0,-1,m_sArrowIndex);
	if(m_sortedAssending){
		QuickSetCellTypeEx(0,-1,UGCT_SORTARROWDOWN);
	}
	else{
		QuickSetCellTypeEx(0,-1,UGCT_SORTARROWUP);
	}

	SetCurrentCellMode(2);

	LockColumns(1);
}

void CSliceGrid::OnGetCell(int col,long row,CUGCell *cell)
{

	if( row == -1 )     //  Display table head information of pRecordSet_Define
	{
		if( col >= 0 )
		{
				switch(col) 
				{
				case 0:
					cell->SetText( "病理号" );
					break;
				case 1:
					cell->SetText( "任务来源" );
					break;
				case 2:
					cell->SetText( "制片子号" );   //( "蜡块号" );
					break;
				case 3:
					cell->SetText( "制片说明" );   //( "切片说明" );
					break;
				case 4:
					cell->SetText( "制片条码号" ); //( "切片条码号" );
					break;				
				case 5:
					cell->SetText( "申请人" );		//申请医师
					break;
				case 6:
					cell->SetText( "制片人" );     //( "切片操作员" );
					break;
				case 7:
					cell->SetText( "制片时间" );   //( "切片时间" );
					break;
				case 8:
					cell->SetText( "制片状态" );
					break;
				case 9:
					cell->SetText( "打印状态" );
					break;
				case 10:
					cell->SetText( "备注" );
					break;
				case 11:
					cell->SetText( "玻片核对" );
					break;
				case 12:
					cell->SetText( "核对时间" );
					break;
				case 13:
					cell->SetText( "切片评级" );
					break;
				case 14:
					cell->SetText( "评级人" );
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

			SAString cmdstr = "select medicalno, qptmh from " + theApp.TABLE_SLICE + m_WhereBetween_Cmd + m_OrderBy_Cmd;
			cmdstr = "select ROWNUM RNO,t.* from (" + cmdstr + ") t" + (SAString)rowlimit_right;
			cmdstr = "select * from (" + cmdstr + ")" + (SAString)rowlimit_left;
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Execute();
			
			CStringArray mnolist, xhlist;
			CString pp;
			int rowno = toprow;
			while( g_dbcommand.FetchNext() )
			{
				//已存在的记录不重新QuickSetText
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
				
				xhlist.Add(g_dbcommand.Field("qptmh").asString());
				
				rowno++;
			}
			
			CString mno;
			CStringArray rwlys,lkhs,bjws;
			for(int k = 0; k < mnolist.GetSize(); k++ )
			{
				mno = mnolist.GetAt(k);
				if(mno.IsEmpty())  continue;
				
				try
				{
					cmdstr = "Select * from " + theApp.TABLE_SLICE + " where medicalno = :medicalno and qptmh = :qptmh";
					g_dbcommand.setCommandText( cmdstr );
					g_dbcommand.Param("medicalno").setAsString() = mno;
					g_dbcommand.Param("qptmh").setAsString() = xhlist.GetAt(k);
					g_dbcommand.Execute();
					
					if( g_dbcommand.FetchNext() )
					{
						rwlys.Add(g_dbcommand.Field(2).asString());
						lkhs.Add(g_dbcommand.Field(3).asString());
						bjws.Add(g_dbcommand.Field(4).asString());
						QuickSetText( 0, toprow + k, mno );
						QuickSetText( 1, toprow + k, g_dbcommand.Field("rwly").asString() );
						QuickSetText( 2, toprow + k, g_dbcommand.Field("lkh").asString() );
						QuickSetText( 3, toprow + k, g_dbcommand.Field("qpsm").asString() );
						QuickSetText( 4, toprow + k, g_dbcommand.Field("qptmh").asString() );
						QuickSetText( 6, toprow + k, g_dbcommand.Field("qpczy").asString() );
						
						COleDateTime curt;
						SADateTime st = g_dbcommand.Field("qprq").asDateTime();
						curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
						QuickSetText( 7, toprow + k, curt.Format("%Y-%m-%d") );
						//QuickSetText( 7, toprow + k, "已完成" );
						if(g_dbcommand.Field("bphd").asString().IsEmpty()){
							QuickSetText(8,toprow + k,"执行中");
						} else {
							QuickSetText(8,toprow + k,"已完成");
						}

						if(g_dbcommand.Field("dyzt").asLong() == 1)
						{
							QuickSetText(9, toprow + k , "已打印");
						//	QuickSetText(7, toprow + k,  "已完成" );
						}
						else
						{
							QuickSetText(9, toprow + k , "未打印");
						//	QuickSetText(7, toprow + k,  "执行中" );
						}
						
						QuickSetText( 10, toprow + k, g_dbcommand.Field("other").asString() );
						
						QuickSetText( 11, toprow + k, g_dbcommand.Field("bphd").asString() );
						
						if(g_dbcommand.Field("hdrq").isNull())
						{
							QuickSetText( 12, toprow + k, " " );
						}
						else
						{
							st = g_dbcommand.Field("hdrq").asDateTime();
							curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
							QuickSetText( 12, toprow + k, curt.Format("%Y-%m-%d") );
						}
						
						QuickSetText( 13, toprow + k, g_dbcommand.Field("qppj").asString() );
						QuickSetText( 14, toprow + k, g_dbcommand.Field("qppjr").asString() );
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
			

			//已存在的记录不重新QuickSetText
			int j = 0;
			for(int i = 0; i < mnolist.GetSize() ; i++)
			{
				if(mnolist.GetAt(i).IsEmpty()) continue;
				CString rwly = rwlys.GetAt(j);
				
				if(rwly.CompareNoCase("免疫组化") == 0 || rwly.CompareNoCase("特殊染色") == 0 || rwly.CompareNoCase("分子病理") == 0 || rwly.CompareNoCase("电镜") == 0 || rwly.CompareNoCase("其它") == 0)  //特检医嘱
				{
					SAString cmdstr = "select sqys from " + theApp.TABLE_ADVICE + " where medicalno=:medicalno and lkh=:lkh and bjw=:bjw";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = mnolist.GetAt(i);
					g_dbcommand.Param("lkh").setAsString() = lkhs.GetAt(j);
					CString bjw;
					bjw = bjws.GetAt(j);
					if(bjw.Find(",") > 0)
					{
						bjw = bjw.Left(bjw.Find(","));
					}
					g_dbcommand.Param("bjw").setAsString() = bjw;
					g_dbcommand.Execute();
					int index = 0;
					while(g_dbcommand.FetchNext())
					{
						index++;
						QuickSetText( 5, toprow + i , g_dbcommand.Field("sqys").asString());
					}
					if(index == 0)
					{
						QuickSetText( 5, toprow + i , "");
					}
				}
				else if(rwly.CompareNoCase("重切") == 0 || rwly.CompareNoCase("深切") == 0 || rwly.CompareNoCase("薄切") == 0 || rwly.CompareNoCase("连切") == 0)		//技术医嘱
				{
					SAString cmdstr = "select sqys from " + theApp.TABLE_ADVICE + " where medicalno=:medicalno and lkh=:lkh";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = mnolist.GetAt(i);
					g_dbcommand.Param("lkh").setAsString() = lkhs.GetAt(j);
					//		g_dbcommand.Param("bjw").setAsString() = bjws.GetAt(j);
					g_dbcommand.Execute();
					while(g_dbcommand.FetchNext())
					{
						QuickSetText( 5, toprow + i , g_dbcommand.Field("sqys").asString());
					}
				} 
				else if(rwly.CompareNoCase("常规") == 0 )
				{	
					if(bjws.GetAt(j).Find("常规-") < 0)			//取材
					{
						QuickSetText( 5, toprow + i, "");
						j++;
						continue;
					}
					//技术医嘱
					SAString cmdstr = "select sqys from " + theApp.TABLE_ADVICE + " where medicalno=:medicalno and lkh=:lkh";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = mnolist.GetAt(i);
					g_dbcommand.Param("lkh").setAsString() = lkhs.GetAt(j);
					//		g_dbcommand.Param("bjw").setAsString() = bjws.GetAt(j);
					g_dbcommand.Execute();
					QuickSetText( 5, toprow + i , "");
					while(g_dbcommand.FetchNext())
					{
						QuickSetText( 5, toprow + i , g_dbcommand.Field("sqys").asString());
					}
				}
				else
				{
					QuickSetText( 5, toprow + i,"");
				}
				j++;	
			}	
			
			mnolist.RemoveAll();
			xhlist.RemoveAll();
			rwlys.RemoveAll();
			lkhs.RemoveAll();
			bjws.RemoveAll();
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
			SAString cmdstr = "select medicalno, qptmh into ##mm" + theApp.m_LocalIp + " from " + theApp.TABLE_SLICE + m_WhereBetween_Cmd + m_OrderBy_Cmd;
			g_dbcommand.setCommandText(cmdstr);
			
			g_dbcommand.Execute();
			
			cmdstr = "select medicalno, qptmh, IDENTITY(int,0,1) as Num into ##qq" + theApp.m_LocalIp + " from ##mm" + theApp.m_LocalIp;
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
			
			cmdstr = "select medicalno, qptmh from ##qq" + theApp.m_LocalIp + " where Num >= :Num1 and Num <= :Num2";
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
				
				xhlist.Add(g_dbcommand.Field("qptmh").asString());
				
				rowno++;
			}
			
			cmdstr = "drop table ##mm" + theApp.m_LocalIp;
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Execute();
			
			cmdstr = "drop table ##qq" + theApp.m_LocalIp;
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Execute();
			
			CString mno;
			CStringArray rwlys,lkhs,bjws;

			for(int k = 0; k < mnolist.GetSize(); k++ )
			{
				mno = mnolist.GetAt(k);
				if(mno.IsEmpty())  continue;
				CString filed = "medicalno,rwly,lkh,qpsm,qptmh,qpczy,qprq,bphd,dyzt,other,bphd,hdrq,qppj,qppjr";
				try
				{
					cmdstr = "Select "+ filed + " from " + theApp.TABLE_SLICE + " where medicalno = :medicalno and qptmh = :qptmh";
					g_dbcommand.setCommandText( cmdstr );
					g_dbcommand.Param("medicalno").setAsString() = mno;
					g_dbcommand.Param("qptmh").setAsString() = xhlist.GetAt(k);
					g_dbcommand.Execute();
					
					if( g_dbcommand.FetchNext() )
					{
						QuickSetText( 0, toprow + k, mno );					
						QuickSetText( 1, toprow + k, g_dbcommand.Field(2).asString() );
						QuickSetText( 2, toprow + k, g_dbcommand.Field(3).asString() );
						rwlys.Add(g_dbcommand.Field(2).asString());
						lkhs.Add(g_dbcommand.Field(3).asString());
						bjws.Add(g_dbcommand.Field(4).asString());
						
						QuickSetText( 3, toprow + k, g_dbcommand.Field(4).asString() );
						QuickSetText( 4, toprow + k, g_dbcommand.Field(5).asString() );
						//QuickSetText( 4, toprow + k, g_dbcommand.Field("qptmh").asString() );
						
						QuickSetText( 6, toprow + k, g_dbcommand.Field(6).asString() );
						
						COleDateTime curt;
						SADateTime st = g_dbcommand.Field(7).asDateTime();
						curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
						QuickSetText( 7, toprow + k, curt.Format("%Y-%m-%d") );
						//QuickSetText( 7, toprow + k, "已完成" );
						
						if(g_dbcommand.Field(8).asString().IsEmpty())
						{
							QuickSetText(8,toprow + k,"执行中");
						} 
						else 
						{
							QuickSetText(8,toprow + k,"已完成");
						}
						if(g_dbcommand.Field(9).asBool())
						{
							QuickSetText(9, toprow + k , "已打印");
						//	QuickSetText(7, toprow + k,  "已完成" );
						}
						else
						{
							QuickSetText(9, toprow + k , "未打印");
						//	QuickSetText(7, toprow + k,  "执行中" );
						}
						
						QuickSetText( 10, toprow + k, g_dbcommand.Field(10).asString() );
						
						QuickSetText( 11, toprow + k, g_dbcommand.Field(11).asString() );
						
						if(g_dbcommand.Field(12).isNull())
						{
							QuickSetText( 12, toprow + k, " " );
						}
						else
						{
							st = g_dbcommand.Field(12).asDateTime();
							curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
							QuickSetText( 12, toprow + k, curt.Format("%Y-%m-%d") );
						}
						
						QuickSetText( 13, toprow + k, g_dbcommand.Field(13).asString() );
						QuickSetText( 14, toprow + k, g_dbcommand.Field(14).asString() );
						
					}
					
					g_dbconnection.Commit();
				}
				catch(SAException &e)
				{
					
					try
					{
						g_dbconnection.Rollback();
					}
					catch(SAException &)
					{
					}
					AfxMessageBox((const char*)e.ErrText());
				}
			}
			
			//已存在的记录不重新QuickSetText
			int j = 0;
			for(int i = 0; i < mnolist.GetSize() ; i++)
			{
				if(mnolist.GetAt(i).IsEmpty()) continue;
				CString rwly = rwlys.GetAt(j);
				
				if(rwly.CompareNoCase("免疫组化") == 0 || rwly.CompareNoCase("特殊染色") == 0 || rwly.CompareNoCase("分子病理") == 0 || rwly.CompareNoCase("电镜") == 0 || rwly.CompareNoCase("其它") == 0)  //特检医嘱
				{
					SAString cmdstr = "select sqys from " + theApp.TABLE_ADVICE + " where medicalno=:medicalno and lkh=:lkh and bjw=:bjw";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = mnolist.GetAt(i);
					g_dbcommand.Param("lkh").setAsString() = lkhs.GetAt(j);
					CString bjw;
					bjw = bjws.GetAt(j);
					if(bjw.Find(",") > 0)
					{
						bjw = bjw.Left(bjw.Find(","));
					}
					g_dbcommand.Param("bjw").setAsString() = bjw;
					g_dbcommand.Execute();
					int index = 0;
					while(g_dbcommand.FetchNext())
					{
						index++;
						QuickSetText( 5, toprow + i , g_dbcommand.Field("sqys").asString());
					}
					if(index == 0)
					{
						QuickSetText( 5, toprow + i , "");
					}
				}
				else if(rwly.CompareNoCase("重切") == 0 || rwly.CompareNoCase("深切") == 0 || rwly.CompareNoCase("薄切") == 0 || rwly.CompareNoCase("连切") == 0)		//技术医嘱
				{
					SAString cmdstr = "select sqys from " + theApp.TABLE_ADVICE + " where medicalno=:medicalno and lkh=:lkh";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = mnolist.GetAt(i);
					g_dbcommand.Param("lkh").setAsString() = lkhs.GetAt(j);
					//		g_dbcommand.Param("bjw").setAsString() = bjws.GetAt(j);
					g_dbcommand.Execute();
					while(g_dbcommand.FetchNext())
					{
						QuickSetText( 5, toprow + i , g_dbcommand.Field("sqys").asString());
					}
				}
				else if(rwly.CompareNoCase("常规") == 0 )
				{	
					if(bjws.GetAt(j).Find("常规-") < 0)			//取材
					{
						QuickSetText( 5, toprow + i, "");
						j++;
						continue;
					}
					//技术医嘱
					SAString cmdstr = "select sqys from " + theApp.TABLE_ADVICE + " where medicalno=:medicalno and lkh=:lkh";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = mnolist.GetAt(i);
					g_dbcommand.Param("lkh").setAsString() = lkhs.GetAt(j);
					//		g_dbcommand.Param("bjw").setAsString() = bjws.GetAt(j);
					g_dbcommand.Execute();
					QuickSetText( 5, toprow + i , "");
					while(g_dbcommand.FetchNext())
					{
						QuickSetText( 5, toprow + i , g_dbcommand.Field("sqys").asString());
					}
				}
				else
				{
					QuickSetText( 5, toprow + i,"");
				}
				j++;	
			}	
			mnolist.RemoveAll();
			xhlist.RemoveAll();
			rwlys.RemoveAll();
			lkhs.RemoveAll();
			bjws.RemoveAll();
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

int  CSliceGrid::OnCanSizeRow(long row)
{
	return FALSE;
}

int CSliceGrid::OnCanSizeTopHdg()
{
	return FALSE;
}

int CSliceGrid::OnCanSizeSideHdg()
{
	return FALSE;
}

void CSliceGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{

	if(RIGHT_SCQPJL(theApp.m_Cpp_Right) && (col == 2 || col == 3 || col == 6 || col == 10) && row >= 0)
	{
/*
		CString PrintState = QuickGetText(8, row);
		if(PrintState.CompareNoCase("未打印") == 0)
*/
		CString BphdState = QuickGetText(10, row);
		BphdState.TrimLeft();  BphdState.TrimRight();
		if(BphdState.IsEmpty() || col == 10)
		{
			StartEdit();
		}
	} 
	else 
	{
		CWnd *pParent = GetParent();
		if(pParent->IsKindOf(RUNTIME_CLASS(CMedSlice)))
		{
			CString str_MedicalNo = QuickGetText(0, row);
			//((CMedSlice *)pParent)->SetDlgItemText(IDC_EDIT_BLH, str_MedicalNo);
			//((CMedSlice *)pParent)->PerformQuery();
			
			COleDateTime m_time;
			
			((CMedSlice *)pParent)->m_Time_Qprq.GetTime(m_time);
			
			CString start = m_time.Format("%Y-%m-%d 00:00:00");
			CString end   = m_time.Format("%Y-%m-%d 23:59:59");
			
			m_WhereBetween_Cmd.Format(" where qprq >= '%s' and qprq <= '%s'", start, end);
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				m_WhereBetween_Cmd.Format(" where qprq >= to_date('%s','yyyy-mm-dd hh24:mi:ss') and qprq <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", start, end);
			}
			
			m_WhereBetween_Cmd += "and medicalno ='"+str_MedicalNo+"'";
			if(((CButton *)((CMedSlice *)pParent)->GetDlgItem(IDC_CHECK_NOTPRINT))->GetState() & 0x0003)
			{
				m_WhereBetween_Cmd += " and dyzt = 0";
			}
			
			if(((CButton *)((CMedSlice *)pParent)->GetDlgItem(IDC_CHECK_NOTHD))->GetState() & 0x0003)
			{
				m_WhereBetween_Cmd += " and bphd IS NULL";
			}
			
			if(theApp.m_StationType.CompareNoCase("常规切片工作站") == 0)
			{
				m_WhereBetween_Cmd += " and (rwly <> '免疫组化' and rwly <> '特殊染色' and rwly <> '分子病理' and rwly <> '电镜' and rwly <> '细胞涂片')";
			}
			
			if(theApp.m_StationType.CompareNoCase("特检工作站") == 0)
			{
				m_WhereBetween_Cmd += " and (rwly = '免疫组化' or rwly = '特殊染色' or rwly = '分子病理' or rwly = '电镜')";
			}
			
			if(theApp.m_StationType.CompareNoCase("细胞学制片工作站") == 0)
			{
				m_WhereBetween_Cmd += " and (rwly = '细胞涂片')";
			}
			
			ResetGridSize();
	        RedrawAll();
			SelectRange(0,0,GetNumberCols() -1 ,GetNumberRows() - 1);
		}
	}
}

int CSliceGrid::OnEditFinish(int col, long row,CWnd * edit, LPCTSTR string,BOOL cancelFlag)
{
	if(cancelFlag)
	{
		QuickSetText(col,row,string);
	}

	if(RIGHT_SCQPJL(theApp.m_Cpp_Right) && (col == 2 || col == 3 || col == 6 || col == 10) && row >= 0)
	{
/*
		CString PrintState = QuickGetText(8, row);
		if(PrintState.CompareNoCase("未打印") == 0)
*/
		CString BphdState = QuickGetText(11, row);
		BphdState.TrimLeft();  BphdState.TrimRight();
		if(BphdState.IsEmpty() || col == 10)
		{
			CString qptmh = QuickGetText(4, row);
			SAString cmdstr;
			try
			{
				switch(col) {
				case 2:
					//cmdstr = "Update " + theApp.TABLE_SLICE + " set lkh=:lkh where qptmh = :qptmh and dyzt = 0";
					cmdstr = "Update " + theApp.TABLE_SLICE + " set lkh=:lkh where qptmh = :qptmh";
					break;
				case 3:
					//cmdstr = "Update " + theApp.TABLE_SLICE + " set qpsm=:qpsm where qptmh = :qptmh and dyzt = 0";
					cmdstr = "Update " + theApp.TABLE_SLICE + " set qpsm=:qpsm where qptmh = :qptmh";
					break;
				case 6:
					//cmdstr = "Update " + theApp.TABLE_SLICE + " set qpczy=:qpczy where qptmh = :qptmh and dyzt = 0";
					cmdstr = "Update " + theApp.TABLE_SLICE + " set qpczy=:qpczy where qptmh = :qptmh";
					break;
				case 10:
					cmdstr = "Update " + theApp.TABLE_SLICE + " set other=:other where qptmh = :qptmh";
					break;
				}
				
				g_dbcommand.setCommandText(cmdstr);
				switch(col) {
				case 2:
					g_dbcommand.Param("lkh").setAsString()   = string;
					break;
				case 3:
					g_dbcommand.Param("qpsm").setAsString()  = string;
					break;
				case 6:
					g_dbcommand.Param("qpczy").setAsString() = string;
					break;
				case 10:
					g_dbcommand.Param("other").setAsString() = string;
					break;
				}
				g_dbcommand.Param("qptmh").setAsString() = qptmh;
				
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

void CSliceGrid::OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	if(updn == 0)
		return;

	if(col != 0)  return;

	QuickSetCellType(0,-1,0);

	if(m_sortedAssending)
		m_sortedAssending = FALSE;
	else
		m_sortedAssending = TRUE;

	QuickSetCellType(0,-1,m_sArrowIndex);

	if(m_sortedAssending)	{
		QuickSetCellTypeEx(0,-1,UGCT_SORTARROWDOWN);
		m_OrderBy_Cmd.Format(" order by medicalno asc, qptmh asc");
	}
	else{
		QuickSetCellTypeEx(0,-1,UGCT_SORTARROWUP);
		m_OrderBy_Cmd.Format(" order by medicalno desc, qptmh asc");
	}
	
	ResetGridSize();
	RedrawAll();
}

void CSliceGrid::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	if(row >= 0 && (col == 3 || col == 10))
	{
		CMenu* menu = GetMenu();
		menu->LoadMenu(IDR_MENU_SLICE);
		CMenu* pPopup = menu->GetSubMenu(0);
		ClientToScreen(point);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point->x ,point->y + GetRowHeight(0),GetParent());
		if(col == 3)
		{
			((CMedSlice*)GetParent())->m_batchType = "qpsm";
		} 
		else if(col == 10)
		{
			((CMedSlice*)GetParent())->m_batchType = "bz";
		}
	} 
	else 
	{
		CString str_MedicalNo = QuickGetText(0, row);
		((CMainFrame *)theApp.m_pMainWnd)->ShowRecordInfo(str_MedicalNo);
	}
	
}
