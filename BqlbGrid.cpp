// BqlbGrid.cpp: implementation of the CBqlbGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "BqlbGrid.h"
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

CBqlbGrid::CBqlbGrid()
{
	m_WhereBetween_Cmd.Format(" where yzlx = '补取' and yzzt = 0");
	m_OrderBy_Cmd.Format(" order by sqrq asc");
}

CBqlbGrid::~CBqlbGrid()
{

}

void CBqlbGrid::ResetGridSize()
{
	theApp.ConnectDatabase();

	int row_num = 0;
	int col_num = 10;

	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_ADVICE + m_WhereBetween_Cmd;
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

	CString LastBlh; LastBlh.Empty();
	bool UsingBackgroundColor = false;
	int  index = 0;
	CUGCell defcell;
	GetGridDefault(&defcell);
	try
	{
		SAString cmdstr = "Select medicalno from " + theApp.TABLE_ADVICE + m_WhereBetween_Cmd;
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

void CBqlbGrid::OnSetup()
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

CString CBqlbGrid::Perform()
{
	CString zxblh; zxblh.Empty();

	CStringArray  MedicalNolist;
	CStringArray  XhNolist;
	CUIntArray  rowlist;
	int nCol, nRet;
	long nRow;
	
	nRet = EnumFirstSelected( &nCol, &nRow );
	while( nRet == UG_SUCCESS )
	{
		if(nRow >= 0)
		{
			int rowcount = rowlist.GetSize();
			for(int i = 0; i < rowcount; i ++ )
			{
				if( nRow == rowlist.GetAt(i) )  break;
			}
			if( i >= rowcount )
			{
				CString str_MedicalNo;
				
				CUGCell  cell;
				GetCellIndirect(0,nRow,&cell);
				cell.GetText(&str_MedicalNo);
				
				CString str_Yzh;
				GetCellIndirect(1,nRow,&cell);
				cell.GetText(&str_Yzh);
				
				rowlist.Add(nRow);
				MedicalNolist.Add(str_MedicalNo);
				XhNolist.Add(str_Yzh);
			}
		}
		
		nRet = EnumNextSelected( &nCol, &nRow );
	}
	
	int curnum = GetCurrentRow();
	for(int i = 0; i < rowlist.GetSize(); i++)
	{
		GotoRow(rowlist.GetAt(i));
	}
	GotoRow(curnum);
	
	rowlist.RemoveAll();
	
	if( MedicalNolist.GetSize() <= 0 )
	{
		return zxblh;
	}
	
	SADateTime ServerTime = theApp.GetServerDateTime();

	for(int index = 0; index < MedicalNolist.GetSize(); index++)
	{
		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_ADVICE + " set yzzt = 1, zxr = :zxr, zxrq = :zxrq where medicalno = :medicalno and yzh = :yzh and yzzt = 0";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
			g_dbcommand.Param("yzh").setAsString()       = XhNolist.GetAt(index);
			g_dbcommand.Param("zxr").setAsString()       = theApp.m_Cpp_User;  
			g_dbcommand.Param("zxrq").setAsDateTime()    = ServerTime;
			
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
			return zxblh;
		}
	}

	zxblh = MedicalNolist.GetAt(0);
	
	MedicalNolist.RemoveAll();
	XhNolist.RemoveAll();
	
	ResetGridSize();
	RedrawAll();

	return zxblh;
}

void CBqlbGrid::OnGetCell(int col,long row,CUGCell *cell)
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
				cell->SetText( "医嘱号" );
				break;
			case 2:
				cell->SetText( "医嘱类型" );
				break;
			case 3:
				cell->SetText( "取材部位" );
				break;
			case 4:
				cell->SetText( "数量" );
				break;
			case 5:
				cell->SetText( "申请医生" );
				break;
			case 6:
				cell->SetText( "申请日期" );
				break;
			case 7:
				cell->SetText( "执行人" );
				break;
			case 8:
				cell->SetText( "执行时间" );
				break;
			case 9:
				cell->SetText( "医嘱状态" );
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

			SAString cmdstr = "select medicalno, yzh from " + theApp.TABLE_ADVICE + m_WhereBetween_Cmd + m_OrderBy_Cmd;
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
				
				xhlist.Add(g_dbcommand.Field("yzh").asString());
				
				rowno++;
			}
			
			CString mno;
			for(int k = 0; k < mnolist.GetSize(); k++ )
			{
				mno = mnolist.GetAt(k);
				if(mno.IsEmpty())  continue;
				
				try
				{
					cmdstr = "Select * from " + theApp.TABLE_ADVICE + " where medicalno = :medicalno and yzh = :yzh";
					g_dbcommand.setCommandText( cmdstr );
					g_dbcommand.Param("medicalno").setAsString() = mno;
					g_dbcommand.Param("yzh").setAsString() = xhlist.GetAt(k);
					g_dbcommand.Execute();
					
					if( g_dbcommand.FetchNext() )
					{
						QuickSetText( 0, toprow + k, mno );
						QuickSetText( 1, toprow + k, g_dbcommand.Field("yzh").asString() );
						QuickSetText( 2, toprow + k, g_dbcommand.Field("yzlx").asString() );
						QuickSetText( 3, toprow + k, g_dbcommand.Field("bz").asString() );
						
						CString qpsstr;
						qpsstr.Format("%d", g_dbcommand.Field("qps").asLong());
						QuickSetText( 4, toprow + k, qpsstr );
						
						QuickSetText( 5, toprow + k, g_dbcommand.Field("sqys").asString() );
						
						COleDateTime curt;
						SADateTime st = g_dbcommand.Field("sqrq").asDateTime();
						curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
						QuickSetText( 6, toprow + k, curt.Format("%Y-%m-%d") );
						
						QuickSetText( 7, toprow + k, g_dbcommand.Field("zxr").asString() );
						
						if(g_dbcommand.Field("zxrq").isNull())
						{
							QuickSetText( 8, toprow + k, " " );
						}
						else
						{
							st = g_dbcommand.Field("zxrq").asDateTime();
							curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
							QuickSetText( 8, toprow + k, curt.Format("%Y-%m-%d") );
						}
						
						if(g_dbcommand.Field("yzzt").asBool())
						{
							QuickSetText(9, toprow + k , "已执行");
						}
						else
						{
							QuickSetText(9, toprow + k , "未执行");
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
			SAString cmdstr = "select medicalno, yzh into ##mm" + theApp.m_LocalIp + " from " + theApp.TABLE_ADVICE + m_WhereBetween_Cmd + m_OrderBy_Cmd;
			g_dbcommand.setCommandText(cmdstr);
			
			g_dbcommand.Execute();
			
			cmdstr = "select medicalno, yzh, IDENTITY(int,0,1) as Num into ##qq" + theApp.m_LocalIp + " from ##mm" + theApp.m_LocalIp;
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
			
			cmdstr = "select medicalno, yzh from ##qq" + theApp.m_LocalIp + " where Num >= :Num1 and Num <= :Num2";
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
				
				xhlist.Add(g_dbcommand.Field("yzh").asString());
				
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
					cmdstr = "Select * from " + theApp.TABLE_ADVICE + " where medicalno = :medicalno and yzh = :yzh";
					g_dbcommand.setCommandText( cmdstr );
					g_dbcommand.Param("medicalno").setAsString() = mno;
					g_dbcommand.Param("yzh").setAsString() = xhlist.GetAt(k);
					g_dbcommand.Execute();
					
					if( g_dbcommand.FetchNext() )
					{
						QuickSetText( 0, toprow + k, mno );
						QuickSetText( 1, toprow + k, g_dbcommand.Field("yzh").asString() );
						QuickSetText( 2, toprow + k, g_dbcommand.Field("yzlx").asString() );
						QuickSetText( 3, toprow + k, g_dbcommand.Field("bz").asString() );
						
						CString qpsstr;
						qpsstr.Format("%d", g_dbcommand.Field("qps").asLong());
						QuickSetText( 4, toprow + k, qpsstr );
						
						QuickSetText( 5, toprow + k, g_dbcommand.Field("sqys").asString() );
						
						COleDateTime curt;
						SADateTime st = g_dbcommand.Field("sqrq").asDateTime();
						curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
						QuickSetText( 6, toprow + k, curt.Format("%Y-%m-%d") );
						
						QuickSetText( 7, toprow + k, g_dbcommand.Field("zxr").asString() );
						
						if(g_dbcommand.Field("zxrq").isNull())
						{
							QuickSetText( 8, toprow + k, " " );
						}
						else
						{
							st = g_dbcommand.Field("zxrq").asDateTime();
							curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
							QuickSetText( 8, toprow + k, curt.Format("%Y-%m-%d") );
						}
						
						if(g_dbcommand.Field("yzzt").asBool())
						{
							QuickSetText(9, toprow + k , "已执行");
						}
						else
						{
							QuickSetText(9, toprow + k , "未执行");
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
}

int  CBqlbGrid::OnCanSizeRow(long row)
{
	return FALSE;
}

int CBqlbGrid::OnCanSizeTopHdg()
{
	return FALSE;
}

int CBqlbGrid::OnCanSizeSideHdg()
{
	return FALSE;
}

void CBqlbGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	if(!(row >= 0 && row < GetNumberRows())) return;
	ClearSelections();
	Select( 0, row );
	
	CString blh = QuickGetText(0, row);
//	AfxMessageBox(blh);
	theApp.OpenDocumentFile(blh);
	
	GetParent()->PostMessage(WM_CLOSE);
}


void CBqlbGrid::OnRClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	if(row >= 0)
	{
		CString str_MedicalNo = QuickGetText(0, row);
		((CMainFrame *)theApp.m_pMainWnd)->ShowRecordInfo(str_MedicalNo);
	}
}
