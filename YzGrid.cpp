// YzGrid.cpp: implementation of the CYzGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "YzGrid.h"
#include "MainFrm.h"
#include "ChargeConfirm.h"
#include "MedAdvice.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CYzGrid::CYzGrid()
{
	if(theApp.m_StationType.CompareNoCase("制片工作站") == 0)
	{
		m_WhereBetween_Cmd.Format(" where yzzt = 0");
	}

	if(theApp.m_StationType.CompareNoCase("常规切片工作站") == 0)
	{
		if(theApp.m_CantainTsrs.CompareNoCase("是") == 0)
		{
			m_WhereBetween_Cmd.Format(" where yzzt = 0 and (yzlx = '重切' or yzlx = '特殊染色' or yzlx = '深切' or yzlx = '薄切' or yzlx = '连切' or yzlx = '常规' or yzlx = '切全' or yzlx = '不削连切')");
		} 
		else 
		{
			m_WhereBetween_Cmd.Format(" where yzzt = 0 and (yzlx = '重切' or yzlx = '深切' or yzlx = '薄切' or yzlx = '连切' or yzlx = '常规' or yzlx = '切全' or yzlx = '不削连切')");
		}
		
	}

	if(theApp.m_StationType.CompareNoCase("特检工作站") == 0)
	{
		if(theApp.m_TjyzMode.CompareNoCase("是") == 0)
		{
			if(theApp.m_CantainTsrs.CompareNoCase("否") == 0)
			{
				m_WhereBetween_Cmd.Format(" where fxjg = '已收费' and yzzt = 0 and (yzlx = '免疫组化' or yzlx = '特殊染色' or yzlx = '分子病理' or yzlx = '电镜' or yzlx = '其它')");
			} 
			else 
			{
				m_WhereBetween_Cmd.Format(" where fxjg = '已收费' and yzzt = 0 and (yzlx = '免疫组化' or yzlx = '分子病理' or yzlx = '电镜' or yzlx = '其它')");
			}
		} 
		else 
		{
			if(theApp.m_CantainTsrs.CompareNoCase("否") == 0)
			{
				m_WhereBetween_Cmd.Format(" where yzzt = 0 and (yzlx = '免疫组化' or yzlx = '特殊染色' or yzlx = '分子病理' or yzlx = '电镜' or yzlx = '其它')");
			} 
			else 
			{
				m_WhereBetween_Cmd.Format(" where yzzt = 0 and (yzlx = '免疫组化' or yzlx = '分子病理' or yzlx = '电镜' or yzlx = '其它')");
			}
		}
			
	}

	m_OrderBy_Cmd.Format(" order by medicalno desc, sqrq desc");
}

CYzGrid::~CYzGrid()
{

}

void CYzGrid::ResetGridSize()
{
	theApp.ConnectDatabase();

	int row_num = 0;
	int col_num = 13;

	CString tail;
	if(m_WhereBetween_Cmd.IsEmpty())
		tail.Format(" where yzlx != '补取' ");
	else
		tail.Format(" and yzlx != '补取'");

	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_ADVICE + m_WhereBetween_Cmd + tail;
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

	//--------------------------------------------------
	CString LastBlh; LastBlh.Empty();
	bool UsingBackgroundColor = false;
	int  index = 0;
	CUGCell defcell;
	GetGridDefault(&defcell);

	try
	{
		SAString cmdstr = "Select medicalno from " + theApp.TABLE_ADVICE + m_WhereBetween_Cmd + tail + m_OrderBy_Cmd;
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

	for(i = 0 ; i < col_num; i++ )
	{
		SetColWidth(i, 75);
	}
	SetColWidth(1, 0);
	SetColWidth(7, 120);
	if( row_num > 0 )
	{
		//GotoRow(0);
	}
}

void CYzGrid::OnSetup()
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

void CYzGrid::OnGetCell(int col,long row,CUGCell *cell)
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
				cell->SetText( "蜡块号" );
				break;
			case 4:
				cell->SetText( "切片数" );
				break;
			case 5:
				cell->SetText( "标记物" );
				break;
			case 6:
				cell->SetText( "申请医生" );
				break;
			case 7:
				cell->SetText( "申请日期" );
				break;
			case 8:
				cell->SetText( "收费情况" );
				break;
			case 9:
				cell->SetText( "医嘱状态" );
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

	if( row < 0 || col != 0 )  
	{
		return;
	}

	CString str_MedicalNo = QuickGetText(0, row);
	str_MedicalNo.TrimLeft();  str_MedicalNo.TrimRight();
	if( !str_MedicalNo.IsEmpty() )  return;

	CString tail;
	if(m_WhereBetween_Cmd.IsEmpty())
		tail.Format(" where yzlx != '补取' ");
	else
		tail.Format(" and yzlx != '补取'");
	
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

			SAString cmdstr = "select medicalno, yzh from " + theApp.TABLE_ADVICE + m_WhereBetween_Cmd + tail + m_OrderBy_Cmd;
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
						QuickSetText( 3, toprow + k, g_dbcommand.Field("lkh").asString() );
						
						CString qpsstr;
						qpsstr.Format("%d", g_dbcommand.Field("qps").asLong());
						QuickSetText( 4, toprow + k, qpsstr );
						
						QuickSetText( 5, toprow + k, g_dbcommand.Field("bjw").asString() );
						QuickSetText( 6, toprow + k, g_dbcommand.Field("sqys").asString() );
						
						COleDateTime curt;
						SADateTime st = g_dbcommand.Field("sqrq").asDateTime();
						curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
						QuickSetText( 7, toprow + k, curt.Format("%Y-%m-%d %H:%M:%S") );
						
						QuickSetText( 8, toprow + k, g_dbcommand.Field("fxjg").asString() );
						
						if(g_dbcommand.Field("yzzt").asBool())
						{
							QuickSetText(9, toprow + k , "已执行");
						}
						else
						{
							QuickSetText(9, toprow + k , "未执行");
						}
						QuickSetText( 10, toprow + k, g_dbcommand.Field("bz").asString() );
						
						QuickSetText( 11, toprow + k, g_dbcommand.Field("zxr").asString() );
						
						if(g_dbcommand.Field("zxrq").isNull())
						{
							QuickSetText( 12, toprow + k, " " );
						}
						else
						{
							st = g_dbcommand.Field("zxrq").asDateTime();
							curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
							QuickSetText( 12, toprow + k, curt.Format("%Y-%m-%d") );
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
			SAString cmdstr = "select medicalno, yzh into ##mm" + theApp.m_LocalIp + " from " + theApp.TABLE_ADVICE + m_WhereBetween_Cmd + tail + m_OrderBy_Cmd;
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
						QuickSetText( 3, toprow + k, g_dbcommand.Field("lkh").asString() );
						
						CString qpsstr;
						qpsstr.Format("%d", g_dbcommand.Field("qps").asLong());
						QuickSetText( 4, toprow + k, qpsstr );
						
						QuickSetText( 5, toprow + k, g_dbcommand.Field("bjw").asString() );
						QuickSetText( 6, toprow + k, g_dbcommand.Field("sqys").asString() );
						
						COleDateTime curt;
						SADateTime st = g_dbcommand.Field("sqrq").asDateTime();
						curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
						QuickSetText( 7, toprow + k, curt.Format("%Y-%m-%d %H:%M:%S") );
						
						QuickSetText( 8, toprow + k, g_dbcommand.Field("fxjg").asString() );
						
						if(g_dbcommand.Field("yzzt").asBool())
						{
							QuickSetText(9, toprow + k , "已执行");
						}
						else
						{
							QuickSetText(9, toprow + k , "未执行");
						}
						QuickSetText( 10, toprow + k, g_dbcommand.Field("bz").asString() );
						
						QuickSetText( 11, toprow + k, g_dbcommand.Field("zxr").asString() );
						
						if(g_dbcommand.Field("zxrq").isNull())
						{
							QuickSetText( 12, toprow + k, " " );
						}
						else
						{
							st = g_dbcommand.Field("zxrq").asDateTime();
							curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
							QuickSetText( 12, toprow + k, curt.Format("%Y-%m-%d") );
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

int  CYzGrid::OnCanSizeRow(long row)
{
	return FALSE;
}

int CYzGrid::OnCanSizeTopHdg()
{
	return FALSE;
}

int CYzGrid::OnCanSizeSideHdg()
{
	return FALSE;
}

void CYzGrid::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
/*	if( row >= 0 && col == 10){
		CMenu* menu = GetMenu();
		menu->LoadMenu(IDR_MENU_SLICE);
		CMenu* pPopup = menu->GetSubMenu(0);
		ClientToScreen(point);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point->x ,point->y + GetRowHeight(0),GetParent());
		return;
	}
*/
	CString str_MedicalNo = QuickGetText(0, row);
	((CMainFrame *)theApp.m_pMainWnd)->ShowRecordInfo(str_MedicalNo);
}

void CYzGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	if(row >= 0)
	{
		if(col == 10)
		{
			StartEdit();
		}
		else
		{
			CWnd *pParent = GetParent();
			if(pParent->IsKindOf(RUNTIME_CLASS(CChargeConfirm)))
			{
				CString str_MedicalNo = QuickGetText(0, row);
				((CChargeConfirm *)pParent)->SetDlgItemText(IDC_EDIT_BLH, str_MedicalNo);
				((CChargeConfirm *)pParent)->PerformQuery();
			} else if(pParent->IsKindOf(RUNTIME_CLASS(CMedAdvice))){
				CString str_MedicalNo = QuickGetText(0, row);
				((CMedAdvice *)pParent)->SetDlgItemText(IDC_EDIT_BLH, str_MedicalNo);
				((CMedAdvice *)pParent)->PerformQuery();
			}
		}
	}
}

void CYzGrid::OnCharDown(UINT *vcKey,BOOL processed)
{
	if(GetCurrentRow() >= 0 && GetCurrentCol() == 10)
	{
		StartEdit(*vcKey);
	}
}

int CYzGrid::OnEditFinish(int col, long row,CWnd * edit, LPCTSTR string,BOOL cancelFlag)
{
	if(cancelFlag)
	{
		QuickSetText(col,row,string);
	}
	if(row >= 0 && col == 10)
	{
		CString  mno = QuickGetText(0, row);
		CString  yzh = QuickGetText(1, row);

		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_ADVICE + " set bz = :bz where medicalno = :medicalno and yzh = :yzh";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = mno;
			g_dbcommand.Param("yzh").setAsString()       = yzh;
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
		}
	}
	
	return TRUE;
}
