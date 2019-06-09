// BqlbGrid.cpp: implementation of the CBqlbGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "YQJLGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CYQJLGrid::CYQJLGrid()
{
	if(theApp.m_DBType.CompareNoCase("oracle") == 0)
	{
		m_WhereBetween_Cmd.Format(" where bgzt>=0 and bgzt<4 and wfyy is not null");
	}
	else
	{
		m_WhereBetween_Cmd.Format(" where bgzt>=0 and bgzt<4 and wfyy is not null and wfyy <> ''");
	}
	
	m_OrderBy_Cmd.Format(" order by wfyy");
}

CYQJLGrid::~CYQJLGrid()
{

}

void CYQJLGrid::ResetGridSize()
{
	theApp.ConnectDatabase();

	int row_num = 0;
	int col_num = 10;

	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + m_WhereBetween_Cmd;
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


	for(i = 0 ; i < col_num; i++ )
	{
		SetColWidth(i, 75);
	}

	if( row_num > 0 )
	{
		//GotoRow(0);
	}
}

void CYQJLGrid::OnSetup()
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

CString CYQJLGrid::Perform()
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

void CYQJLGrid::OnGetCell(int col,long row,CUGCell *cell)
{
	if( row == -1 )     //  Display table head information of pRecordSet_Define
	{
		if( col >= 0 )
		{
			switch(col) {
			case 0:
				cell->SetText( "延期原因" );
				break;
			case 1:
				cell->SetText( "病理号" );
				break;
			case 2:
				cell->SetText( "姓名" );
				break;
			case 3:
				cell->SetText( "性别" );
				break;
			case 4:
				cell->SetText( "年龄" );
				break;
			case 5:
				cell->SetText( "送检科室" );
				break;
			case 6:
				cell->SetText( "送检医师" );
				break;
			case 7:
				cell->SetText( "送检日期" );
				break;
			case 8:
				cell->SetText( "送检材料" );
				break;
			case 9:
				cell->SetText( "诊断医师" );
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

			SAString cmdstr = "select medicalno from " + theApp.TABLE_MAIN + m_WhereBetween_Cmd + m_OrderBy_Cmd;
			cmdstr = "select ROWNUM RNO,t.* from (" + cmdstr + ") t" + (SAString)rowlimit_right;
			cmdstr = "select * from (" + cmdstr + ")" + (SAString)rowlimit_left;
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Execute();
			
			CStringArray mnolist;//, xhlist;
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
					//xhlist.Add("");
					continue;
				}
				
				CString FieldStr;	FieldStr.Empty();
				FieldStr = g_dbcommand.Field("medicalno").asString();
				if( FieldStr.IsEmpty() )   FieldStr.Format(" ");
				
				if(rowno == row)   cell->SetText(FieldStr);
				
				QuickSetText(1, rowno, FieldStr);
				
				FieldStr.TrimLeft();  FieldStr.TrimRight();
				mnolist.Add(FieldStr);
				
				//xhlist.Add(g_dbcommand.Field("yzh").asString());
				
				rowno++;
			}
			
			CString mno;
			for(int k = 0; k < mnolist.GetSize(); k++ )
			{
				mno = mnolist.GetAt(k);
				if(mno.IsEmpty())  continue;
				
				try
				{
					cmdstr = "Select * from " + theApp.TABLE_MAIN + " where medicalno = :medicalno ";//and yzh = :yzh
					g_dbcommand.setCommandText( cmdstr );
					g_dbcommand.Param("medicalno").setAsString() = mno;
					//g_dbcommand.Param("yzh").setAsString() = xhlist.GetAt(k);
					g_dbcommand.Execute();
					
					if( g_dbcommand.FetchNext() )
					{
						QuickSetText( 0, toprow + k, g_dbcommand.Field("wfyy").asString() );
						QuickSetText( 1, toprow + k, mno );
						QuickSetText( 2, toprow + k, g_dbcommand.Field("xm").asString() );
						QuickSetText( 3, toprow + k, g_dbcommand.Field("xb").asString() );
						QuickSetText( 4, toprow + k, g_dbcommand.Field("nl").asString() );
						QuickSetText( 5, toprow + k, g_dbcommand.Field("sjks").asString() );
						QuickSetText( 6, toprow + k, g_dbcommand.Field("sjys").asString() );
						COleDateTime curt;
						SADateTime st = g_dbcommand.Field("sjrq").asDateTime();
						curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
						
						QuickSetText( 7, toprow + k, curt.Format("%Y-%m-%d") );
						
						QuickSetText( 8, toprow + k, g_dbcommand.Field("bbmc").asString() );
						
						CString zdys,temp;zdys.Empty();temp.Empty();
						if( !(temp = g_dbcommand.Field("czys").asString()).IsEmpty()){
							zdys += temp;
						}
						if(!(temp = g_dbcommand.Field("zzys").asString().IsEmpty())){
							zdys += "、";
							zdys += temp;
						}
						if(!(temp = g_dbcommand.Field("shys").asString().IsEmpty())){
							zdys += "、";
							zdys += temp;
						}
						QuickSetText( 9, toprow + k, zdys );
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
//			xhlist.RemoveAll();
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
			
			g_dbcommand.Execute();
			
			cmdstr = "select medicalno,  IDENTITY(int,0,1) as Num into ##qq" + theApp.m_LocalIp + " from ##mm" + theApp.m_LocalIp;
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
			
			CStringArray mnolist;//, xhlist;
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
			//		xhlist.Add("");
					continue;
				}
				
				CString FieldStr;	FieldStr.Empty();
				FieldStr = g_dbcommand.Field("medicalno").asString();
				if( FieldStr.IsEmpty() )   FieldStr.Format(" ");
				
				if(rowno == row)   cell->SetText(FieldStr);
				
				QuickSetText(0, rowno, FieldStr);
				
				FieldStr.TrimLeft();  FieldStr.TrimRight();
				mnolist.Add(FieldStr);
				
			//	xhlist.Add(g_dbcommand.Field("yzh").asString());
				
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
					cmdstr = "Select * from " + theApp.TABLE_MAIN + " where medicalno = :medicalno ";
					g_dbcommand.setCommandText( cmdstr );
					g_dbcommand.Param("medicalno").setAsString() = mno;
					//g_dbcommand.Param("yzh").setAsString() = xhlist.GetAt(k);
					g_dbcommand.Execute();
					
					if( g_dbcommand.FetchNext() )
					{
						//AfxMessageBox(g_dbcommand.Field("wfyy").asString()); 
						QuickSetText( 0, toprow + k, g_dbcommand.Field("wfyy").asString() );
						QuickSetText( 1, toprow + k, mno );
						QuickSetText( 2, toprow + k, g_dbcommand.Field("xm").asString() );
						QuickSetText( 3, toprow + k, g_dbcommand.Field("xb").asString() );
						QuickSetText( 4, toprow + k, g_dbcommand.Field("nl").asString() );
						QuickSetText( 5, toprow + k, g_dbcommand.Field("sjks").asString() );
						QuickSetText( 6, toprow + k, g_dbcommand.Field("sjys").asString() );
						COleDateTime curt;
						SADateTime st = g_dbcommand.Field("sjrq").asDateTime();
						curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
						
						QuickSetText( 7, toprow + k, curt.Format("%Y-%m-%d") );
						
						QuickSetText( 8, toprow + k, g_dbcommand.Field("bbmc").asString() );
						
						CString zdys,temp;zdys.Empty();temp.Empty();
						if( !(temp = g_dbcommand.Field("czys").asString()).IsEmpty()){
							zdys += temp;
						}
						if(!(temp = g_dbcommand.Field("zzys").asString().IsEmpty())){
							zdys += "、";
							zdys += temp;
						}
						if(!(temp = g_dbcommand.Field("shys").asString().IsEmpty())){
							zdys += "、";
							zdys += temp;
						}
						QuickSetText( 9, toprow + k, zdys );					
					
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
	//		xhlist.RemoveAll();
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

int  CYQJLGrid::OnCanSizeRow(long row)
{
	return FALSE;
}

int CYQJLGrid::OnCanSizeTopHdg()
{
	return FALSE;
}

int CYQJLGrid::OnCanSizeSideHdg()
{
	return FALSE;
}

void CYQJLGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
/*	if(!(row >= 0 && row < GetNumberRows())) return;
	ClearSelections();
	Select( 0, row );
	
	CString blh = QuickGetText(1, row);
//	AfxMessageBox(blh);
	theApp.OpenDocumentFile(blh);
	
	GetParent()->PostMessage(WM_CLOSE);*/
	if(row >= 0 && col == 0){
		StartEdit();
	}
}

int CYQJLGrid::OnEditFinish(int col, long row,CWnd * edit, LPCTSTR string,BOOL cancelFlag)
{
	if(cancelFlag)
	{
		QuickSetText(col,row,string);
	}
	if(row >= 0 && col == 0)
	{
		CString  mno = QuickGetText(1, row);
		
		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set wfyy = :wfyy where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = mno;
			g_dbcommand.Param("wfyy").setAsString()        = string;  
			
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
	ResetGridSize();
	RedrawAll();
	RedrawAll();
	//Select(1,1);
	return TRUE;
}

