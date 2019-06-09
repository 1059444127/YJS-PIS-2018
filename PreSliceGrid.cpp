// PreSliceGrid.cpp: implementation of the CPreSliceGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "PreSliceGrid.h"
#include "MedPreslice.h"
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

CPreSliceGrid::CPreSliceGrid()
{
	m_WhereBetween_Cmd.Empty();
	//m_OrderBy_Cmd.Format(" order by bmrq asc");
	//m_OrderBy_Cmd.Format(" order by medicalno desc, bmrq asc");
	// 由于对待包埋列表中的一批待包埋记录进行包埋确认时，bmrq 是相同的，导致蜡块号排序混乱，因此改为按取材日期排序
	m_OrderBy_Cmd.Format(" order by medicalno desc, qcrq asc");
}

CPreSliceGrid::~CPreSliceGrid()
{

}

void CPreSliceGrid::ResetGridSize()
{
	theApp.ConnectDatabase();

	int row_num = 0;
	int col_num = 8;

	CString tail;
	if(m_WhereBetween_Cmd.IsEmpty())
		tail.Format(" where zpzt = 2 ");
	else
		tail.Format(" and zpzt = 2");
		

	int qps_sum = 0;

	try
	{
		SAString cmdstr = "Select count(*), sum(qps) from " + theApp.TABLE_MATERIAL + m_WhereBetween_Cmd + tail;
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();

		if( g_dbcommand.FetchNext() )
		{
			row_num = g_dbcommand.Field(1).asLong();
			qps_sum = g_dbcommand.Field(2).asLong();
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

	CWnd *pParent = GetParent();
	if(pParent->IsKindOf(RUNTIME_CLASS(CMedPreslice)))
	{
		CString info;
		info.Format("待切片数：%d", qps_sum);
		((CMedPreslice *)pParent)->m_HypLink_Note.SetWindowText(info);
		((CMedPreslice *)pParent)->m_HypLink_Note.SetURL("");
		((CMedPreslice *)pParent)->m_HypLink_Note.Invalidate();
	}

	SetNumberRows(row_num);
	SetNumberCols(col_num);

	for(int i = 0; i < row_num; i++ )
	{
		QuickSetText( 0, i, "" );

		QuickSetCellType(6,i,UGCT_DROPLIST);
		QuickSetCellTypeEx(6,i,UGCT_DROPLISTHIDEBUTTON);
		QuickSetLabelText(6,i,"0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n");
	}

	//--------------------------------------------------
	CString LastBlh; LastBlh.Empty();
	bool UsingBackgroundColor = false;
	int  index = 0;
	CUGCell defcell;
	GetGridDefault(&defcell);

	try
	{
		SAString cmdstr = "Select medicalno from " + theApp.TABLE_MATERIAL + m_WhereBetween_Cmd + tail + m_OrderBy_Cmd;
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
	//--------------------------------------------------

	for(i = 0 ; i < (col_num-1); i++ )
	{
		SetColWidth(i, 80);
	}

	SetColWidth(col_num-1, 200);

	if( row_num > 0 )
	{
		//GotoRow(0);
	}
}

void CPreSliceGrid::OnSetup()
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

	LockColumns(1);
}

void CPreSliceGrid::OnGetCell(int col,long row,CUGCell *cell)
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
				cell->SetText( "包埋操作员" );
				break;
			case 4:
				cell->SetText( "包埋日期" );
				break;
			case 5:
				cell->SetText( "制片状态" );
				break;
			case 6:
				cell->SetText( "切片数" );
				break;
			case 7:
				cell->SetText( "备注" );
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

	CString tail;
	if(m_WhereBetween_Cmd.IsEmpty())
		tail.Format(" where zpzt = 2 ");
	else
		tail.Format(" and zpzt = 2");
	
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

			SAString cmdstr = "select medicalno, lkh from " + theApp.TABLE_MATERIAL + m_WhereBetween_Cmd + tail + m_OrderBy_Cmd;
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
				
				xhlist.Add(g_dbcommand.Field("lkh").asString());
				
				rowno++;
			}
			
			CString mno;
			for(int k = 0; k < mnolist.GetSize(); k++ )
			{
				mno = mnolist.GetAt(k);
				if(mno.IsEmpty())  continue;
				
				try
				{
					cmdstr = "Select * from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno and lkh = :lkh";
					g_dbcommand.setCommandText( cmdstr );
					g_dbcommand.Param("medicalno").setAsString() = mno;
					g_dbcommand.Param("lkh").setAsString() = xhlist.GetAt(k);
					g_dbcommand.Execute();
					
					if( g_dbcommand.FetchNext() )
					{
						QuickSetText( 0, toprow + k, mno );
						QuickSetText( 1, toprow + k, g_dbcommand.Field("rwly").asString() );
						QuickSetText( 2, toprow + k, g_dbcommand.Field("lkh").asString() );
						QuickSetText( 3, toprow + k, g_dbcommand.Field("bmczy").asString() );
						
						COleDateTime curt;
						SADateTime st = g_dbcommand.Field("bmrq").asDateTime();
						curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
						QuickSetText( 4, toprow + k, curt.Format("%Y-%m-%d") );
						
						switch(g_dbcommand.Field("zpzt").asLong()) {
						case 0:
							QuickSetText(5, toprow + k , "待核对");
							break;
						case 1:
							QuickSetText(5, toprow + k , "待包埋");
							break;
						case 2:
							QuickSetText(5, toprow + k , "待切片");
							break;
						case 3:
							QuickSetText(5, toprow + k , "已切片");
							break;
						}
						
						CString qpsstr;
						qpsstr.Format("%d", g_dbcommand.Field("qps").asLong());
						QuickSetText( 6, toprow + k, qpsstr );
						
						QuickSetText( 7, toprow + k, g_dbcommand.Field("bz").asString() );
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
			SAString cmdstr = "select medicalno, lkh into ##mm" + theApp.m_LocalIp + " from " + theApp.TABLE_MATERIAL + m_WhereBetween_Cmd + tail + m_OrderBy_Cmd;
			g_dbcommand.setCommandText(cmdstr);
			
			g_dbcommand.Execute();
			
			cmdstr = "select medicalno, lkh, IDENTITY(int,0,1) as Num into ##qq" + theApp.m_LocalIp + " from ##mm" + theApp.m_LocalIp;
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
			
			cmdstr = "select medicalno, lkh from ##qq" + theApp.m_LocalIp + " where Num >= :Num1 and Num <= :Num2";
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
				
				xhlist.Add(g_dbcommand.Field("lkh").asString());
				
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
					cmdstr = "Select * from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno and lkh = :lkh";
					g_dbcommand.setCommandText( cmdstr );
					g_dbcommand.Param("medicalno").setAsString() = mno;
					g_dbcommand.Param("lkh").setAsString() = xhlist.GetAt(k);
					g_dbcommand.Execute();
					
					if( g_dbcommand.FetchNext() )
					{
						QuickSetText( 0, toprow + k, mno );
						QuickSetText( 1, toprow + k, g_dbcommand.Field("rwly").asString() );
						QuickSetText( 2, toprow + k, g_dbcommand.Field("lkh").asString() );
						QuickSetText( 3, toprow + k, g_dbcommand.Field("bmczy").asString() );
						
						COleDateTime curt;
						SADateTime st = g_dbcommand.Field("bmrq").asDateTime();
						curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
						QuickSetText( 4, toprow + k, curt.Format("%Y-%m-%d") );
						
						switch(g_dbcommand.Field("zpzt").asLong()) {
						case 0:
							QuickSetText(5, toprow + k , "待核对");
							break;
						case 1:
							QuickSetText(5, toprow + k , "待包埋");
							break;
						case 2:
							QuickSetText(5, toprow + k , "待切片");
							break;
						case 3:
							QuickSetText(5, toprow + k , "已切片");
							break;
						}
						
						CString qpsstr;
						qpsstr.Format("%d", g_dbcommand.Field("qps").asLong());
						QuickSetText( 6, toprow + k, qpsstr );
						
						QuickSetText( 7, toprow + k, g_dbcommand.Field("bz").asString() );
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

int  CPreSliceGrid::OnCanSizeRow(long row)
{
	return FALSE;
}

int CPreSliceGrid::OnCanSizeTopHdg()
{
	return FALSE;
}

int CPreSliceGrid::OnCanSizeSideHdg()
{
	return FALSE;
}

void CPreSliceGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	//if(col >= 6 && row >= 0)
	//{
	//	StartEdit();
	//}
	if(row >= 0)
	{
		if(col == 10)
		{
			StartEdit();
		}
		else
		{
			CWnd *pParent = GetParent();
			if(pParent->IsKindOf(RUNTIME_CLASS(CMedPreslice)))
			{
				CString str_MedicalNo = QuickGetText(0, row);
				((CMedPreslice *)pParent)->SetDlgItemText(IDC_EDIT_BLH, str_MedicalNo);
				((CMedPreslice *)pParent)->PerformQuery();
			}
		}
	}
}

void CPreSliceGrid::OnCharDown(UINT *vcKey,BOOL processed)
{
	if(GetCurrentCol() >= 6 && GetCurrentRow() >= 0)
	{
		StartEdit(*vcKey);
	}
}

int CPreSliceGrid::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{

	if(col == 6 && row >= 0)
	{
		if(msg == UGCT_DROPLISTSELECT)
		{
			CString *string = (CString*)param;
			int qps = atoi(*string);

			theApp.ConnectDatabase();

			CString mno = QuickGetText(0 ,row);
			CString lkh = QuickGetText(2 ,row);
			
			try
			{
				SAString cmdstr = "Update " + theApp.TABLE_MATERIAL + " set qps = :qps where medicalno = :medicalno and lkh = :lkh";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = mno;
				g_dbcommand.Param("lkh").setAsString()       = lkh;
				g_dbcommand.Param("qps").setAsLong()         = qps;  
				
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

			int qps_sum = 0;

			CString tail;
			if(m_WhereBetween_Cmd.IsEmpty())
				tail.Format(" where zpzt = 2 ");
			else
				tail.Format(" and zpzt = 2");
			
			try
			{
				SAString cmdstr = "Select sum(qps) from " + theApp.TABLE_MATERIAL + m_WhereBetween_Cmd + tail;
				g_dbcommand.setCommandText(cmdstr);
				
				g_dbcommand.Execute();
				
				if( g_dbcommand.FetchNext() )
				{
					qps_sum = g_dbcommand.Field(1).asLong();
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
				return FALSE;
			}
			
			CWnd *pParent = GetParent();
			if(pParent->IsKindOf(RUNTIME_CLASS(CMedPreslice)))
			{
				CString info;
				info.Format("待切片数：%d", qps_sum);
				((CMedPreslice *)pParent)->m_HypLink_Note.SetWindowText(info);
				((CMedPreslice *)pParent)->m_HypLink_Note.SetURL("");
				((CMedPreslice *)pParent)->m_HypLink_Note.Invalidate();
			}
		}
	}
	
	return TRUE;
}

int CPreSliceGrid::OnEditFinish(int col, long row,CWnd * edit, LPCTSTR string,BOOL cancelFlag)
{
	if(cancelFlag)
	{
		QuickSetText(col,row,string);
	}
	if(col >= 6 && row >= 0)
	{
		switch(col) {
		case 6:
			{
				int qps = atoi(string);
				
				theApp.ConnectDatabase();
				
				CString mno = QuickGetText(0 ,row);
				CString lkh = QuickGetText(2 ,row);
				
				try
				{
					SAString cmdstr = "Update " + theApp.TABLE_MATERIAL + " set qps = :qps where medicalno = :medicalno and lkh = :lkh";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = mno;
					g_dbcommand.Param("lkh").setAsString()       = lkh;
					g_dbcommand.Param("qps").setAsLong()         = qps;  
					
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

				int qps_sum = 0;
				CString tail;
				if(m_WhereBetween_Cmd.IsEmpty())
					tail.Format(" where zpzt = 2 ");
				else
					tail.Format(" and zpzt = 2");
				
				try
				{
					SAString cmdstr = "Select sum(qps) from " + theApp.TABLE_MATERIAL + m_WhereBetween_Cmd + tail;
					g_dbcommand.setCommandText(cmdstr);
					
					g_dbcommand.Execute();
					
					if( g_dbcommand.FetchNext() )
					{
						qps_sum = g_dbcommand.Field(1).asLong();
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
					return FALSE;
				}
				
				CWnd *pParent = GetParent();
				if(pParent->IsKindOf(RUNTIME_CLASS(CMedPreslice)))
				{
					CString info;
					info.Format("待切片数：%d", qps_sum);
					((CMedPreslice *)pParent)->m_HypLink_Note.SetWindowText(info);
					((CMedPreslice *)pParent)->m_HypLink_Note.SetURL("");
					((CMedPreslice *)pParent)->m_HypLink_Note.Invalidate();
				}
			}
			break;
		case 7:
			{
				theApp.ConnectDatabase();
				
				CString mno = QuickGetText(0 ,row);
				CString lkh = QuickGetText(2 ,row);
				
				try
				{
					SAString cmdstr = "Update " + theApp.TABLE_MATERIAL + " set bz = :bz where medicalno = :medicalno and lkh = :lkh";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = mno;
					g_dbcommand.Param("lkh").setAsString()       = lkh;
					g_dbcommand.Param("bz").setAsString()        = string;  
					
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
			break;
		}
	}
	
	return TRUE;
}

void CPreSliceGrid::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	CString str_MedicalNo = QuickGetText(0, row);
	((CMainFrame *)theApp.m_pMainWnd)->ShowRecordInfo(str_MedicalNo);
}
