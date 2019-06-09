// DBGrid.cpp: implementation of the CDBGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "Pathologyview.h"
#include "Pathologydoc.h"
#include "DBGrid.h"
#include "MainFrm.h"
#include "Statistic.h"
#include "AreaOut.h"
#include "StrEdit.h"
#include "CompositeQuery.h"
#include "QuickQuery.h"
#include "DBList.h"
#include "FmtSelect.h"
#include "BatchPrintCtrl.h"
#include "TextOut.h"
#include "Figure.h"
#include "MyProgressBar.h"
#include "FieldOut.h"
#include "Bdbg.h"
#include "Bcbg.h"
#include "Jsyz.h"
#include "Tjyz.h"
#include "INI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CDBGrid::CDBGrid()
{
	m_CompositeQueryString.Empty();
	m_sortCol = 0;
	m_sortedAssending = FALSE;

	m_OrderBy_Cmd.Format(" order by medicalno desc");
	m_WhereBetween_Cmd.Format(" where medicalno='tianjinwen'");

	COleDateTime curt = theApp.GetOleServerDateTime();
	COleDateTimeSpan tspan;
	tspan.SetDateTimeSpan(7, 0, 0, 0);
	curt -= tspan;
	m_WhereBetween_Cmd.Format(" where sjrq >= '%s'", curt.Format("%Y-%m-%d 00:00:00"));
	if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
	{
		m_WhereBetween_Cmd.Format(" where sjrq >= to_date('%s', 'yyyy-mm-dd hh24:mi:ss')", curt.Format("%Y-%m-%d 00:00:00"));
	}
}

CDBGrid::~CDBGrid()
{

}

int  CDBGrid::OnCanSizeRow(long row)
{
	return FALSE;
}

int  CDBGrid::OnCanSizeCol(int col)
{
	return TRUE;
}

int CDBGrid::OnCanSizeTopHdg()
{
	return FALSE;
}

int CDBGrid::OnCanSizeSideHdg()
{
	return FALSE;
}

void CDBGrid::OnSetup()
{
	m_font.CreateFont(16,0,0,0,500,0,0,0,0,0,0,0,0,"Arial");
	SetDefFont(&m_font);

	m_sArrowIndex = AddCellType(&m_sortArrow);

	CUGCell cell;
	GetGridDefault(&cell);
	cell.SetAlignment(UG_ALIGNLEFT);
	//cell.SetAlignment(UG_ALIGNRIGHT);
	cell.SetBorder(UG_BDR_RECESSED);
	cell.SetBackColor(RGB(220,220,220));
	cell.SetHBackColor(RGB(255,192,192));
	cell.SetHTextColor(RGB(0,0,0));
	SetGridDefault(&cell);

	m_MainRecordnum   = 0;
	m_DefineRecordnum = 0;

    ResetGridSize();

	if(theApp.m_ImgNumShowMode.CompareNoCase("是") == 0)
	{
		SetSH_NumberCols(1);
		SetSH_Width(25);
		SetSH_ColWidth(0,25);
	}
	else
	{
		SetSH_NumberCols(0);
		SetSH_Width(0);
	}

	SetMultiSelectMode(TRUE);
	SetHighlightRow(TRUE);

	SetDoubleBufferMode(TRUE);

	QuickSetCellType(m_sortCol,-1,m_sArrowIndex);
	if(m_sortedAssending){
		QuickSetCellTypeEx(m_sortCol,-1,UGCT_SORTARROWDOWN);
	}
	else{
		QuickSetCellTypeEx(m_sortCol,-1,UGCT_SORTARROWUP);
	}

    RefreshMenuItem();
	
	EnableMenu(1);	

	if(GetNumberCols() > 2)
		LockColumns(2);
	else
		LockColumns(1);

	UseVScrollHints(TRUE);
	SetCurrentCellMode(2);
}

void CDBGrid::RefreshMenuItem()
{
	EmptyMenu();

	AddMenuItem(1000,"定值查询");

	if( RIGHT_FZCX(theApp.m_Cpp_Right) )
	{
		AddMenuItem(1001,"区间查询");
		AddMenuItem(1002,"模糊查询");
		AddMenuItem(1003,"自定义查询");
		AddMenuItem(1004,"组合查询");
		AddMenuItem(1005,"全部记录");
		AddMenuItem(1017,"");
		AddMenuItem(1018,"当天记录");
		AddMenuItem(1019,"昨天记录");
		AddMenuItem(1020,"前天记录");
		AddMenuItem(1021,"本周记录");
		AddMenuItem(1022,"本月记录");
		AddMenuItem(1023,"本年记录");
	}
	AddMenuItem(1044,"");
	AddMenuItem(1040,"书写冰冻结果");
	AddMenuItem(1041,"书写补充报告");
	AddMenuItem(1042,"书写技术医嘱");
	AddMenuItem(1043,"书写特检医嘱");

	AddMenuItem(1006,"");
	AddMenuItem(1007,"我的报告");
	AddMenuItem(1032,"我的未诊断报告");
	AddMenuItem(1008,"我的未审核报告");
	AddMenuItem(1009,"我的未打印报告");
	AddMenuItem(1038,"我的未发报告");
	AddMenuItem(1010,"我的收藏记录");
//	AddMenuItem(1045,"取消收藏记录");
	AddMenuItem(1028,"未审核报告");
	AddMenuItem(1011,"未发送报告");
	AddMenuItem(1029,"冰冻报告");
	AddMenuItem(1037,"特检报告");
	AddMenuItem(1012,"");

	AddMenuItem(1033,"提交初诊医师");
	AddMenuItem(1034,"提交主诊医师");
	AddMenuItem(1035,"提交审核医师");
	AddMenuItem(1036,"");

	AddMenuItem(2125,"电子病历");

	if( RIGHT_FZCX(theApp.m_Cpp_Right) )
	{
		AddMenuItem(1025,"病历浏览");
	}
	
	//AddMenuItem(1026,"图像显示");
	if( RIGHT_SCJL(theApp.m_Cpp_Right) )
	{
		AddMenuItem(1013,"删除记录");
	}
	if( RIGHT_XGBH(theApp.m_Cpp_Right) )
	{
		AddMenuItem(1014,"修改编号");
	}
	AddMenuItem(1030,"数值统计");

	if( RIGHT_TXDC(theApp.m_Cpp_Right) )
	{
		AddMenuItem(1031,"图像导出");
	}
	
	AddMenuItem(1015,"");

	if( RIGHT_SH(theApp.m_Cpp_Right) )
	{
		AddMenuItem(1027,"批量审核");
	}

	AddMenuItem(1016,"批量打印");

	if( RIGHT_FSBG(theApp.m_Cpp_Right) )
	{
		AddMenuItem(1024,"批量发送");
	}
}

void CDBGrid::ResetGridSize()
{
	theApp.ConnectDatabase();
	
	CString tail;

    if(((CButton *)((CDBList *)GetParent())->GetDlgItem(IDC_CHECK_ALLDB))->GetState() & 0x0003)
	{
		if(m_WhereBetween_Cmd.IsEmpty())
			tail.Format(" where 1 = 1");
	}
	else
	{
		if(m_WhereBetween_Cmd.IsEmpty())
			tail.Format(" where dbtype = %d", theApp.m_DB_Type_No);
		else
			tail.Format(" and dbtype = %d", theApp.m_DB_Type_No);
	}

	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + m_WhereBetween_Cmd + tail;
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();

		m_MainRecordnum = 0;
		if( g_dbcommand.FetchNext() )
		{
			m_MainRecordnum = g_dbcommand.Field(1).asLong();
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

	m_FieldName_List.RemoveAll();
	m_FieldDescription_List.RemoveAll();
	m_FieldInfo_List.RemoveAll();
	m_FieldType_List.RemoveAll();

	CStdioFile File;
	if ( File.Open(theApp.m_Exe_Working_Dir + "system\\selfshow.txt", CFile::modeRead | CFile::typeText) )
	{
		CString tmp;
		while(1)
		{
			if( File.ReadString(tmp) == FALSE ) break;
			tmp.TrimLeft(); tmp.TrimRight();
			if(!tmp.IsEmpty())
			{
				for(int i = 0; i < theApp.m_fdescriptionlist.GetSize(); i++)
				{
					if(theApp.m_fdescriptionlist.GetAt(i).CompareNoCase(tmp) == 0)
					{
						m_FieldName_List.Add( theApp.m_fnamelist.GetAt(i) );
						m_FieldDescription_List.Add( theApp.m_fdescriptionlist.GetAt(i) );
						m_FieldInfo_List.Add(theApp.m_finfolist.GetAt(i));
						m_FieldType_List.Add(theApp.m_ftypeList.GetAt(i));

						break;
					}
				}
			}
		}
		File.Close();
	}

	if(m_FieldName_List.GetSize() == 0 || m_FieldName_List.GetAt(0).CompareNoCase("medicalno"))
	{
		m_FieldName_List.InsertAt(0, "medicalno");
		m_FieldDescription_List.InsertAt(0, "病理号");
        m_FieldInfo_List.InsertAt(0, "");
		m_FieldType_List.InsertAt(0, SA_dtString);
	}

	m_DefineRecordnum = m_FieldName_List.GetSize();

	int row_num = m_MainRecordnum;
	int col_num = m_DefineRecordnum;

	SetNumberRows(row_num);
	SetNumberCols(col_num);

	for(int i = 0; i < row_num; i++ )
	{
		QuickSetText( 0, i, "" );
	}

	//=============================================================
	CUIntArray  sizelist;
	sizelist.RemoveAll();

	if ( File.Open(theApp.m_Exe_Working_Dir + "system\\cpi_grid.siz", CFile::modeRead | CFile::typeText) )
	{
		CString tmp;
		while(1)
		{
			if( File.ReadString(tmp) == FALSE ) break;
			tmp.TrimLeft();
			tmp.TrimRight();
			if( tmp.IsEmpty() )   continue;
			sizelist.Add(atoi(tmp));
		}
		File.Close();
	}

	if( sizelist.GetSize() > 0 && m_DefineRecordnum == sizelist.GetSize() )
	{
		for(i = 0 ; i < m_DefineRecordnum; i++ )
		{
			SetColWidth(i,sizelist.GetAt(i));
		}
	}

	sizelist.RemoveAll();
	//=============================================================

	for(i = 0 ; i < m_DefineRecordnum; i++ )
	{
		SADataType_t  DataType = (SADataType_t) m_FieldType_List.GetAt( i );
		if( !( DataType == SA_dtShort   || DataType == SA_dtLong     || DataType == SA_dtBool   ||
			   DataType == SA_dtDouble  || DataType == SA_dtDateTime || DataType == SA_dtString ||
			   DataType == SA_dtLongChar|| DataType == SA_dtCLob ))
		{
			SetColWidth(i,0);
		}
	}

	if( row_num > 0 )
	{
		//GotoRow(0);
	}

	CString str;
	str.Format("病历信息管理 ( %d )", row_num);

	if(theApp.m_pMainWnd->GetSafeHwnd() != NULL && theApp.m_pMainWnd->IsKindOf(RUNTIME_CLASS(CMainFrame)))
	{
		((CMainFrame *)theApp.m_pMainWnd)->m_wndTabDockBar.SetWindowText(str);
		((CMainFrame *)theApp.m_pMainWnd)->m_wndTabDockBar.RedrawWindow();
		::SetFocus(((CMainFrame *)theApp.m_pMainWnd)->m_hWnd);
	}
}

void CDBGrid::OnGetCell(int col,long row,CUGCell *cell)
{
	CWnd * pMainWnd = AfxGetMainWnd();
	if(pMainWnd == NULL || !pMainWnd->IsKindOf(RUNTIME_CLASS(CMainFrame)))  return;

	if( !((CMainFrame *)AfxGetMainWnd())->m_frame_created )  return;
	if( row == -1 )     //  Display table head information of pRecordSet_Define
	{
		if( col >= 0 )
		{
			cell->SetText( m_FieldDescription_List.GetAt(col) );
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

	CString tail;

    if(((CButton *)((CDBList *)GetParent())->GetDlgItem(IDC_CHECK_ALLDB))->GetState() & 0x0003)
	{
		if(m_WhereBetween_Cmd.IsEmpty())
			tail.Format(" where 1 = 1");
	}
	else
	{
		if(m_WhereBetween_Cmd.IsEmpty())
			tail.Format(" where dbtype = %d", theApp.m_DB_Type_No);
		else
			tail.Format(" and dbtype = %d", theApp.m_DB_Type_No);
	}

	CUGCell defcell;
	GetGridDefault(&defcell);

	if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
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

		//-----------------------------------------
		CString fieldstr; fieldstr.Empty();
		for(int j = 0; j < m_FieldName_List.GetSize(); j++)
		{
			if(!fieldstr.IsEmpty())  fieldstr += ",";
			fieldstr += m_FieldName_List.GetAt(j);
		}
		fieldstr.MakeLower();
		if(fieldstr.Find("bdh")  < 0)   fieldstr += ",bdh";
		if(fieldstr.Find("bdjg")  < 0)  fieldstr += ",bdjg";
		if(fieldstr.Find("bgzt") < 0)   fieldstr += ",bgzt";
		//-----------------------------------------
		
		try
		{
			//SAString cmdstr = "select  *  from " + theApp.TABLE_MAIN + m_WhereBetween_Cmd + tail + m_OrderBy_Cmd;
			SAString cmdstr = "select " + fieldstr + " from " + theApp.TABLE_MAIN + m_WhereBetween_Cmd + tail + m_OrderBy_Cmd;
			cmdstr = "select ROWNUM RNO,t.* from (" + cmdstr + ") t" + (SAString)rowlimit_right;
			cmdstr = "select * from (" + cmdstr + ")" + (SAString)rowlimit_left;
			
			/* row_number() over (order by 排序) as rno  调试版本
			SAString cmdstr = "select tMain.*,row_number() over (" + m_OrderBy_Cmd + ") as rno from " + theApp.TABLE_MAIN + " tMain "+ m_WhereBetween_Cmd + tail + m_OrderBy_Cmd;
			CString bw;
			bw.Format("between %d and %d", toprow, bottomrow);
			cmdstr = "select * from(" + cmdstr + ") where rno " + (SAString)bw;*/

			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Execute();
			
			CStringArray mnolist, bd_flag_list;
			mnolist.RemoveAll();  bd_flag_list.RemoveAll();
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
					bd_flag_list.Add("");
					continue;
				}
				
				if(g_dbcommand.Field("bdh").asString().IsEmpty() && g_dbcommand.Field("bdjg").asString().IsEmpty())
				{
					bd_flag_list.Add("");
				}
				else
				{
					bd_flag_list.Add("冰");
				}
				
				CString FieldStr;
				FieldStr = g_dbcommand.Field("medicalno").asString();
				if( FieldStr.IsEmpty() )   FieldStr.Format(" ");
				
				if(rowno == row)   cell->SetText(FieldStr);
				
				QuickSetText(0, rowno, FieldStr);
				
				FieldStr.TrimLeft();  FieldStr.TrimRight();
				mnolist.Add(FieldStr);
				
				int m_bgzt = 0;
				if(!g_dbcommand.Field("bgzt").isNull())
				{
					m_bgzt = g_dbcommand.Field("bgzt").asLong();
				}
				
				//CString s;s.Format("%d",m_bgzt);
			//	AfxMessageBox(s);
 
				for(int j = 1; j < m_FieldName_List.GetSize(); j++)
				{
					SADataType_t  DataType = (SADataType_t)m_FieldType_List.GetAt(j);
					
					CString FieldStr;	FieldStr.Empty();
					SAString nFieldName = m_FieldName_List.GetAt(j);
					
					switch( DataType ) {
					case SA_dtShort:
						FieldStr.Format( "%d", g_dbcommand.Field(nFieldName).asShort() );
						break;
					case SA_dtLong:
						FieldStr.Format( "%d", g_dbcommand.Field(nFieldName).asLong() );
						break;
					case SA_dtBool:
						if( g_dbcommand.Field(nFieldName).asBool() )
							FieldStr.Format("是");
						else
							FieldStr.Format("否");
						break;
					case SA_dtDouble:
						FieldStr.Format( "%f", g_dbcommand.Field(nFieldName).asDouble() );
						break;
					case SA_dtDateTime:
						//FieldStr.Format("%d-%d-%d  %d:%d:%d", g_dbcommand.Field(j).asDateTime().GetYear(), g_dbcommand.Field(j).asDateTime().GetMonth(), g_dbcommand.Field(j).asDateTime().GetDay(), g_dbcommand.Field(j).asDateTime().GetHour(), g_dbcommand.Field(j).asDateTime().GetMinute(), g_dbcommand.Field(j).asDateTime().GetSecond());
						if(g_dbcommand.Field(nFieldName).isNull()==false && g_dbcommand.Field(nFieldName).asDateTime().GetYear() > 1950)
						{
							FieldStr.Format("%d-%d-%d", g_dbcommand.Field(nFieldName).asDateTime().GetYear(), g_dbcommand.Field(nFieldName).asDateTime().GetMonth(), g_dbcommand.Field(nFieldName).asDateTime().GetDay());
						}
						else
						{
							FieldStr.Format(" ");
						}
						break;
					case SA_dtString:
						FieldStr = g_dbcommand.Field(nFieldName).asString();
						break;
					case SA_dtBytes:
						FieldStr.Format("二进制流数据");
						break;
					case SA_dtLongBinary:
					case SA_dtBLob:
						FieldStr.Format("长二进制流数据");
						break;
					case SA_dtLongChar:
					case SA_dtCLob:
						FieldStr = g_dbcommand.Field(nFieldName).asString();
						break;
					case SA_dtUnknown:
					case SA_dtNumeric:
						
					case SA_dtCursor:
					case SA_dtSpecificToDBMS:
						FieldStr.Format("系统数据类型");
						break;
					default:;
					}
					if( FieldStr.IsEmpty() )
					{
						FieldStr.Format(" ");
					}
					
					QuickSetText( j, rowno, FieldStr );
				}
				
				if( rowno == row )
				{
					if(m_bgzt < 1)
					{
						cell->SetBackColor(RGB(132,132,255));
						cell->SetTextColor(RGB(0,0,0));
					}
					
					if(m_bgzt >= 1 && m_bgzt < 2)
					{
						cell->SetBackColor(RGB(132,255,132));
						cell->SetTextColor(RGB(0,0,0));
					}
					
					if(m_bgzt >= 2 && m_bgzt < 4)
					{
						cell->SetBackColor(RGB(255,50,50));
						cell->SetTextColor(RGB(0,0,0));
					}
					
					if(m_bgzt >= 4 && m_bgzt < 8)
					{
						cell->SetBackColor(RGB(255,0,255));
						cell->SetTextColor(RGB(0,0,0));
					}
					
					if(m_bgzt >= 8 && m_bgzt < 16)
					{
						cell->SetBackColor(RGB(255,255,0));
						cell->SetTextColor(RGB(0,0,0));
					}
					
					if(m_bgzt >= 16)
					{
						cell->ClearPropertySetFlag(UGCELL_BACKCOLOR_SET);
						cell->SetBackColor(defcell.GetBackColor());
						
						if(m_bgzt >= 32)
						{
							cell->SetTextColor(RGB(0,0,255));
						}
						else
						{
							cell->SetTextColor(RGB(0,0,0));
						}
					}
				}
				if(m_bgzt < 1)
				{
					QuickSetBackColor(0,rowno,RGB(132,132,255));
					QuickSetTextColor(0,rowno, RGB(0,0,0));
				}
				
				if(m_bgzt >= 1 && m_bgzt < 2)
				{
					QuickSetBackColor(0,rowno,RGB(132,255,132));
					QuickSetTextColor(0,rowno, RGB(0,0,0));
				}
				
				if(m_bgzt >= 2 && m_bgzt < 4)
				{
					QuickSetBackColor(0,rowno,RGB(255,50,50));
					QuickSetTextColor(0,rowno, RGB(0,0,0));
				}
				
				if(m_bgzt >= 4 && m_bgzt < 8)
				{
					QuickSetBackColor(0,rowno,RGB(255,0,255));
					QuickSetTextColor(0,rowno, RGB(0,0,0));
				}
				
				if(m_bgzt >= 8 && m_bgzt < 16)
				{
					QuickSetBackColor(0,rowno,RGB(255,255,0));
					QuickSetTextColor(0,rowno, RGB(0,0,0));
				}
				
				if(m_bgzt >= 16)
				{
					CUGCell cell;
					GetCell(0,rowno,&cell);
					cell.ClearPropertySetFlag(UGCELL_BACKCOLOR_SET);
					cell.SetBackColor(defcell.GetBackColor());
					SetCell(0,rowno,&cell);
					
					if(m_bgzt >= 32)
					{
						QuickSetTextColor(0,rowno, RGB(0,0,255));
					}
					else
					{
						QuickSetTextColor(0,rowno, RGB(0,0,0));
					}
				}
				
				rowno++;
			}  
		
			if(theApp.m_ImgNumShowMode.CompareNoCase("是") == 0)
			{
				CString mnostr; mnostr.Empty();
				for(int k = 0; k < mnolist.GetSize(); k++ )
				{
					if(mnolist.GetAt(k).IsEmpty())  continue;
					if(!mnostr.IsEmpty())  mnostr += ",";
					mnostr += ("'" + mnolist.GetAt(k) + "'");
				}
				
				CStringArray  nBlhList;
				CUIntArray    nImgCountList;
				
				if(!mnostr.IsEmpty())
				{
					mnostr = "(" + mnostr + ")";
					cmdstr = "select medicalno,count(*) from " + theApp.TABLE_IMAGE + " where medicalno in  " + mnostr + " group by medicalno";
					g_dbcommand.setCommandText(cmdstr);
					
					g_dbcommand.Execute();
					
					while( g_dbcommand.FetchNext() )
					{
						nBlhList.Add(g_dbcommand.Field(1).asString());
						nImgCountList.Add(g_dbcommand.Field(2).asLong());
					}
				}
				
				CString mno;
				for(k = 0; k < mnolist.GetSize(); k++ )
				{
					mno = mnolist.GetAt(k);
					if(mno.IsEmpty())  continue;
					
					if(!bd_flag_list.GetAt(k).IsEmpty())
					{
						QuickSetText(-1, toprow + k, bd_flag_list.GetAt(k));
						continue;
					}
					
					for(int m = 0; m < nBlhList.GetSize(); m++)
					{
						if(nBlhList.GetAt(m).CompareNoCase(mno) == 0)	break;
					}
					
					CString picstr;
					if(m < nBlhList.GetSize())
					{
						picstr.Format("%d",nImgCountList.GetAt(m));
					}
					else
					{
						picstr.Format("0");
					}
					QuickSetText(-1, toprow + k, picstr);
				}
				nBlhList.RemoveAll();
				nImgCountList.RemoveAll();
			}
			
			bd_flag_list.RemoveAll();
			mnolist.RemoveAll();
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
		CString toprow_str, totalrownum;
		toprow_str.Format(" %d ", toprow);
		totalrownum.Format(" %d ", bottomrow - toprow + 1);
		//----------------------------------------------------------------
		
		try
		{
			SAString cmdstr = "select TOP" + totalrownum + " *  from " + theApp.TABLE_MAIN + m_WhereBetween_Cmd + tail+" and medicalno NOT IN (select TOP"+toprow_str+" medicalno from " + theApp.TABLE_MAIN + m_WhereBetween_Cmd + tail + m_OrderBy_Cmd + ")"+m_OrderBy_Cmd;
			g_dbcommand.setCommandText(cmdstr);
			//AfxMessageBox(cmdstr);
			g_dbcommand.Execute();
			
			CStringArray mnolist, bd_flag_list;
			mnolist.RemoveAll();  bd_flag_list.RemoveAll();
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
					bd_flag_list.Add("");
					continue;
				}
				
				if(g_dbcommand.Field("bdh").asString().IsEmpty() && g_dbcommand.Field("bdjg").asString().IsEmpty())
				{
					bd_flag_list.Add("");
				}
				else
				{
					bd_flag_list.Add("冰");
				}
				
				CString FieldStr;
				FieldStr = g_dbcommand.Field("medicalno").asString();
				if( FieldStr.IsEmpty() )   FieldStr.Format(" ");
				
				if(rowno == row)   cell->SetText(FieldStr);
				
				QuickSetText(0, rowno, FieldStr);
				
				FieldStr.TrimLeft();  FieldStr.TrimRight();
				mnolist.Add(FieldStr);
				
				int m_bgzt = 0;
				if(!g_dbcommand.Field("bgzt").isNull())
				{
					m_bgzt = g_dbcommand.Field("bgzt").asLong();
				}
				
				for(int j = 1; j < m_FieldName_List.GetSize(); j++)
				{
					SADataType_t  DataType = (SADataType_t)m_FieldType_List.GetAt(j);
					
					CString FieldStr;	FieldStr.Empty();
					SAString nFieldName = m_FieldName_List.GetAt(j);
					
					switch( DataType ) {
					case SA_dtShort:
						FieldStr.Format( "%d", g_dbcommand.Field(nFieldName).asShort() );
						break;
					case SA_dtLong:
						FieldStr.Format( "%d", g_dbcommand.Field(nFieldName).asLong() );
						break;
					case SA_dtBool:
						if( g_dbcommand.Field(nFieldName).asBool() )
							FieldStr.Format("是");
						else
							FieldStr.Format("否");
						break;
					case SA_dtDouble:
						FieldStr.Format( "%f", g_dbcommand.Field(nFieldName).asDouble() );
						break;
					case SA_dtDateTime:
						//FieldStr.Format("%d-%d-%d  %d:%d:%d", g_dbcommand.Field(j).asDateTime().GetYear(), g_dbcommand.Field(j).asDateTime().GetMonth(), g_dbcommand.Field(j).asDateTime().GetDay(), g_dbcommand.Field(j).asDateTime().GetHour(), g_dbcommand.Field(j).asDateTime().GetMinute(), g_dbcommand.Field(j).asDateTime().GetSecond());
						if(g_dbcommand.Field(nFieldName).isNull()==false && g_dbcommand.Field(nFieldName).asDateTime().GetYear() > 1950)
						{
							FieldStr.Format("%d-%d-%d", g_dbcommand.Field(nFieldName).asDateTime().GetYear(), g_dbcommand.Field(nFieldName).asDateTime().GetMonth(), g_dbcommand.Field(nFieldName).asDateTime().GetDay());
						}
						else
						{
							FieldStr.Format(" ");
						}
						break;
					case SA_dtString:
						FieldStr = g_dbcommand.Field(nFieldName).asString();
						break;
					case SA_dtBytes:
						FieldStr.Format("二进制流数据");
						break;
					case SA_dtLongBinary:
					case SA_dtBLob:
						FieldStr.Format("长二进制流数据");
						break;
					case SA_dtLongChar:
					case SA_dtCLob:
						FieldStr = g_dbcommand.Field(nFieldName).asString();
						break;
					case SA_dtUnknown:
					case SA_dtNumeric:
					case SA_dtCursor:
					case SA_dtSpecificToDBMS:
						FieldStr.Format("系统数据类型");
						break;
					default:;
					}
					if( FieldStr.IsEmpty() )
					{
						FieldStr.Format(" ");
					}
					
					QuickSetText( j, rowno, FieldStr );
				}
				
				if( rowno == row )
				{
					if(m_bgzt < 1)
					{
						cell->SetBackColor(RGB(132,132,255));
						cell->SetTextColor(RGB(0,0,0));
					}
					
					if(m_bgzt >= 1 && m_bgzt < 2)
					{
						cell->SetBackColor(RGB(132,255,132));
						cell->SetTextColor(RGB(0,0,0));
					}
					
					if(m_bgzt >= 2 && m_bgzt < 4)
					{
						cell->SetBackColor(RGB(255,50,50));
						cell->SetTextColor(RGB(0,0,0));
					}
					
					if(m_bgzt >= 4 && m_bgzt < 8)
					{
						cell->SetBackColor(RGB(255,0,255));
						cell->SetTextColor(RGB(0,0,0));
					}
					
					if(m_bgzt >= 8 && m_bgzt < 16)
					{
						cell->SetBackColor(RGB(255,255,0));
						cell->SetTextColor(RGB(0,0,0));
					}
					
					if(m_bgzt >= 16)
					{
						cell->ClearPropertySetFlag(UGCELL_BACKCOLOR_SET);
						cell->SetBackColor(defcell.GetBackColor());
						
						if(m_bgzt >= 32)
						{
							cell->SetTextColor(RGB(0,0,255));
						}
						else
						{
							cell->SetTextColor(RGB(0,0,0));
						}
					}
				}
				if(m_bgzt < 1)
				{
					QuickSetBackColor(0,rowno,RGB(132,132,255));
					QuickSetTextColor(0,rowno, RGB(0,0,0));
				}
				
				if(m_bgzt >= 1 && m_bgzt < 2)
				{
					QuickSetBackColor(0,rowno,RGB(132,255,132));
					QuickSetTextColor(0,rowno, RGB(0,0,0));
				}
				
				if(m_bgzt >= 2 && m_bgzt < 4)
				{
					QuickSetBackColor(0,rowno,RGB(255,50,50));
					QuickSetTextColor(0,rowno, RGB(0,0,0));
				}
				
				if(m_bgzt >= 4 && m_bgzt < 8)
				{
					QuickSetBackColor(0,rowno,RGB(255,0,255));
					QuickSetTextColor(0,rowno, RGB(0,0,0));
				}
				
				if(m_bgzt >= 8 && m_bgzt < 16)
				{
					QuickSetBackColor(0,rowno,RGB(255,255,0));
					QuickSetTextColor(0,rowno, RGB(0,0,0));
				}
				
				if(m_bgzt >= 16)
				{
					CUGCell cell;
					GetCell(0,rowno,&cell);
					cell.ClearPropertySetFlag(UGCELL_BACKCOLOR_SET);
					cell.SetBackColor(defcell.GetBackColor());
					SetCell(0,rowno,&cell);
					
					if(m_bgzt >= 32)
					{
						QuickSetTextColor(0,rowno, RGB(0,0,255));
					}
					else
					{
						QuickSetTextColor(0,rowno, RGB(0,0,0));
					}
				}
				
				rowno++;
			}  
		
			if(theApp.m_ImgNumShowMode.CompareNoCase("是") == 0)
			{
				CString mnostr; mnostr.Empty();
				for(int k = 0; k < mnolist.GetSize(); k++ )
				{
					if(mnolist.GetAt(k).IsEmpty())  continue;
					if(!mnostr.IsEmpty())  mnostr += ",";
					mnostr += ("'" + mnolist.GetAt(k) + "'");
				}
				
				CStringArray  nBlhList;
				CUIntArray    nImgCountList;
				
				if(!mnostr.IsEmpty())
				{
					mnostr = "(" + mnostr + ")";
					cmdstr = "select medicalno,count(*) from " + theApp.TABLE_IMAGE + " where medicalno in  " + mnostr + " group by medicalno";
					g_dbcommand.setCommandText(cmdstr);
					
					g_dbcommand.Execute();
					
					while( g_dbcommand.FetchNext() )
					{
						nBlhList.Add(g_dbcommand.Field(1).asString());
						nImgCountList.Add(g_dbcommand.Field(2).asLong());
					}
				}
				
				CString mno;
				for(k = 0; k < mnolist.GetSize(); k++ )
				{
					mno = mnolist.GetAt(k);
					if(mno.IsEmpty())  continue;
					
					if(!bd_flag_list.GetAt(k).IsEmpty())
					{
						QuickSetText(-1, toprow + k, bd_flag_list.GetAt(k));
						continue;
					}
					
					for(int m = 0; m < nBlhList.GetSize(); m++)
					{
						if(nBlhList.GetAt(m).CompareNoCase(mno) == 0)	break;
					}
					
					CString picstr;
					if(m < nBlhList.GetSize())
					{
						picstr.Format("%d",nImgCountList.GetAt(m));
					}
					else
					{
						picstr.Format("0");
					}
					QuickSetText(-1, toprow + k, picstr);
				}
				nBlhList.RemoveAll();
				nImgCountList.RemoveAll();
			}
			
			bd_flag_list.RemoveAll();
			mnolist.RemoveAll();
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

void CDBGrid::OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	if(updn == 0)
		return;

	SADataType_t  DataType = (SADataType_t) m_FieldType_List.GetAt( col );
	if( !( DataType == SA_dtShort   || DataType == SA_dtLong     || DataType == SA_dtBool ||
	       DataType == SA_dtDouble  || DataType == SA_dtDateTime || DataType == SA_dtString ))
	{
		//  text、ntext 和 image 数据类型不能用于 ORDER BY 子句中。
		return;
	}

	QuickSetCellType(m_sortCol,-1,0);

	if(col == m_sortCol){
		if(m_sortedAssending)
			m_sortedAssending = FALSE;
		else
			m_sortedAssending = TRUE;
	}
	else{
		m_sortCol = col;
		m_sortedAssending = TRUE;
	}

	QuickSetCellType(m_sortCol,-1,m_sArrowIndex);

	if(m_sortedAssending){
		QuickSetCellTypeEx(m_sortCol,-1,UGCT_SORTARROWDOWN);
	}
	else{
		QuickSetCellTypeEx(m_sortCol,-1,UGCT_SORTARROWUP);
	}
	
	m_OrderBy_Cmd.Format(" order by ");
	m_OrderBy_Cmd += m_FieldName_List.GetAt(m_sortCol);
	if(m_sortedAssending)
		m_OrderBy_Cmd += " asc";
	else
		m_OrderBy_Cmd += " desc";

	int row_num = GetNumberRows();
	int col_num = GetNumberCols();

	for(int i = 0; i < row_num; i++ )
	{
		QuickSetText( 0, i, "" );
	}

	RedrawAll();
}

int CDBGrid::OnVScrollHint(long row,CString *string)
{
	string->Format("%d / %d 条记录", row, m_MainRecordnum);
	return TRUE;
}

void CDBGrid::OnMenuCommand(int col,long row,int section,int item)
{
    if(col < 0 || col >= GetNumberCols()) return;

	if(theApp.m_pMainWnd->GetSafeHwnd() != NULL && theApp.m_pMainWnd->IsKindOf(RUNTIME_CLASS(CMainFrame)) && ((CMainFrame*)theApp.m_pMainWnd)->m_realtime)
	{
		((CMainFrame*)AfxGetMainWnd())->TerminateTimer();
	}

	if(item == 1040){
		int nCol,nRect; long nRow; CString blh;	
		nRect = EnumFirstSelected(&nCol,&nRow);
		CUGCell  cell;
		GetCellIndirect(0,nRow,&cell);
		cell.GetText(&blh);
		theApp.GetView()->GetDocument()->m_blh = blh;
		theApp.OpenDocumentFile(blh);
		CBdbg mydlg(this);
		mydlg.DoModal();
	}

	if(item == 1041){
		int nCol,nRect; long nRow; CString blh;	
		nRect = EnumFirstSelected(&nCol,&nRow);
		CUGCell  cell;
		GetCellIndirect(0,nRow,&cell);
		cell.GetText(&blh);
		theApp.GetView()->GetDocument()->m_blh = blh;
		theApp.OpenDocumentFile(blh);
		CBcbg mydlg(this);
		mydlg.DoModal();
	}

	if(item == 1042){
		int nCol,nRect; long nRow; CString blh;	
		nRect = EnumFirstSelected(&nCol,&nRow);
		CUGCell  cell;
		GetCellIndirect(0,nRow,&cell);
		cell.GetText(&blh);
		theApp.GetView()->GetDocument()->m_blh = blh;
		theApp.OpenDocumentFile(blh);
		if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  return;	
		CJsyz mydlg(this);
		mydlg.DoModal();
		//CMedDiagnose::RefreshBgzt();
	}

	if(item == 1043){
		int nCol,nRect; long nRow; CString blh;	
		nRect = EnumFirstSelected(&nCol,&nRow);
		CUGCell  cell;
		GetCellIndirect(0,nRow,&cell);
		cell.GetText(&blh);
		theApp.GetView()->GetDocument()->m_blh = blh;
		theApp.OpenDocumentFile(blh);
		if(theApp.GetView()->GetDocument()->m_blh.IsEmpty())  return;
		
		CTjyz mydlg(this);
	    mydlg.DoModal();
		//CMedDiagnose::RefreshBgzt();
	}

	if( item == 1000 || item == 1001 )  // 1000 - 定值查询 & 1001 - 区间查询
	{
		SADataType_t  DataType = (SADataType_t) m_FieldType_List.GetAt( col );
		if( !( DataType == SA_dtShort   || DataType == SA_dtLong     || DataType == SA_dtBool   ||
			   DataType == SA_dtDouble  || DataType == SA_dtDateTime || DataType == SA_dtString ||
			   DataType == SA_dtLongChar|| DataType == SA_dtCLob) )
		{
			return;
		}

		

		if(item == 1001 && DataType == SA_dtDateTime) // 日期型区间查询
		{
			CStatistic mydlg(this);  
			mydlg.m_Title.Format("为< %s >指定输出范围",m_FieldDescription_List.GetAt(col));
			if( mydlg.DoModal() == IDCANCEL )
			{
				return;
			}

			mydlg.m_from.TrimLeft();   mydlg.m_from.TrimRight();
			mydlg.m_to.TrimLeft();     mydlg.m_to.TrimRight();
			
			m_WhereBetween_Cmd.Empty();
			
			if( !mydlg.m_from.IsEmpty() )
			{
				switch( DataType ) {
				case SA_dtShort:
					m_WhereBetween_Cmd.Format("%s >= %d", m_FieldName_List.GetAt(col),atoi(mydlg.m_from));
					break;
				case SA_dtLong:
					m_WhereBetween_Cmd.Format("%s >= %d", m_FieldName_List.GetAt(col),atol(mydlg.m_from));
					break;
				case SA_dtBool:
					if( mydlg.m_from.Compare("是") == 0 )
						m_WhereBetween_Cmd.Format("%s >= 1", m_FieldName_List.GetAt(col));
					else
						m_WhereBetween_Cmd.Format("%s >= 0", m_FieldName_List.GetAt(col));
					break;
				case SA_dtDouble:
					m_WhereBetween_Cmd.Format("%s >= %f", m_FieldName_List.GetAt(col),atof(mydlg.m_from));
					break;
				case SA_dtDateTime:
					m_WhereBetween_Cmd.Format("%s >= \'%s\'", m_FieldName_List.GetAt(col), mydlg.m_from);
					if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
					{
						m_WhereBetween_Cmd.Format("%s >= to_date('%s','yyyy-mm-dd hh24:mi:ss')", m_FieldName_List.GetAt(col), mydlg.m_from);
					}
					break;
				case SA_dtString:
				case SA_dtLongChar:
				case SA_dtCLob:
					m_WhereBetween_Cmd.Format("%s >= \'%s\'", m_FieldName_List.GetAt(col), mydlg.m_from);
					break;
				}
			}
			if( !mydlg.m_to.IsEmpty() )
			{
				CString str;
				if( !m_WhereBetween_Cmd.IsEmpty() )
				{
					switch( DataType ) {
					case SA_dtShort:
						str.Format(" and %s <= %d", m_FieldName_List.GetAt(col),atoi(mydlg.m_to));
						break;
					case SA_dtLong:
						str.Format(" and %s <= %d", m_FieldName_List.GetAt(col),atol(mydlg.m_to));
						break;
					case SA_dtBool:
						if( mydlg.m_to.Compare("是") == 0 )
							str.Format(" and %s <= 1", m_FieldName_List.GetAt(col));
						else
							str.Format(" and %s <= 0", m_FieldName_List.GetAt(col));
						break;
					case SA_dtDouble:
						str.Format(" and %s <= %f", m_FieldName_List.GetAt(col),atof(mydlg.m_to));
						break;
					case SA_dtDateTime:
						str.Format(" and %s <= \'%s\'", m_FieldName_List.GetAt(col), mydlg.m_to);
						if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
						{
							str.Format(" and %s <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", m_FieldName_List.GetAt(col), mydlg.m_to);
						}
						break;
					case SA_dtString:
					case SA_dtLongChar:
					case SA_dtCLob:
						str.Format(" and %s <= \'%s\'", m_FieldName_List.GetAt(col), mydlg.m_to);
						break;
					}
				}
				else
				{
					switch( DataType ) {
					case SA_dtShort:
						str.Format(" %s <= %d", m_FieldName_List.GetAt(col),atoi(mydlg.m_to));
						break;
					case SA_dtLong:
						str.Format(" %s <= %d", m_FieldName_List.GetAt(col),atol(mydlg.m_to));
						break;
					case SA_dtBool:
						if( mydlg.m_to.Compare("是") == 0 )
							str.Format(" %s <= 1", m_FieldName_List.GetAt(col));
						else
							str.Format(" %s <= 0", m_FieldName_List.GetAt(col));
						break;
					case SA_dtDouble:
						str.Format(" %s <= %f", m_FieldName_List.GetAt(col),atof(mydlg.m_to));
						break;
					case SA_dtDateTime:
						str.Format(" %s <= \'%s\'", m_FieldName_List.GetAt(col), mydlg.m_to);
						if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
						{
							str.Format(" %s <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", m_FieldName_List.GetAt(col), mydlg.m_to);
						}
						break;
					case SA_dtString:
					case SA_dtLongChar:
					case SA_dtCLob:
						str.Format(" %s <= \'%s\'", m_FieldName_List.GetAt(col), mydlg.m_to);
						break;
					}
				}
				m_WhereBetween_Cmd += str;
			}
			
			if( !m_WhereBetween_Cmd.IsEmpty() )
			{
				m_WhereBetween_Cmd = " where " + m_WhereBetween_Cmd;
			}
		}
		else
		{
			CAreaOut mydlg(this);
			mydlg.m_type = item;
			mydlg.m_FieldName = m_FieldName_List.GetAt(col);
			mydlg.m_FieldInfo = m_FieldInfo_List.GetAt(col);
			
			if(item == 1001)
				mydlg.m_title.Format("为< %s >指定输出范围",m_FieldDescription_List.GetAt(col));
			else
				mydlg.m_title.Format("为< %s >指定输出值",m_FieldDescription_List.GetAt(col));
			
			if(DataType == SA_dtDateTime)
			{
				COleDateTime curt = theApp.GetOleServerDateTime();
				mydlg.m_from = curt.Format("%Y-%m-%d %H:%M:%S");
				mydlg.m_to   = curt.Format("%Y-%m-%d %H:%M:%S");
			}
			
			if( mydlg.DoModal() == IDCANCEL )
			{
				return;
			}
			
			mydlg.m_from.TrimLeft();   mydlg.m_from.TrimRight();
			mydlg.m_to.TrimLeft();     mydlg.m_to.TrimRight();
			
			if( item == 1000 )
			{
				if(mydlg.m_from.IsEmpty() && mydlg.m_to.IsEmpty())
				{
					return;
				}
			}
			
			m_WhereBetween_Cmd.Empty();
			
			if( !mydlg.m_from.IsEmpty() )
			{
				switch( DataType ) {
				case SA_dtShort:
					m_WhereBetween_Cmd.Format("%s >= %d", m_FieldName_List.GetAt(col),atoi(mydlg.m_from));
					break;
				case SA_dtLong:
					m_WhereBetween_Cmd.Format("%s >= %d", m_FieldName_List.GetAt(col),atol(mydlg.m_from));
					break;
				case SA_dtBool:
					if( mydlg.m_from.Compare("是") == 0 )
						m_WhereBetween_Cmd.Format("%s >= 1", m_FieldName_List.GetAt(col));
					else
						m_WhereBetween_Cmd.Format("%s >= 0", m_FieldName_List.GetAt(col));
					break;
				case SA_dtDouble:
					m_WhereBetween_Cmd.Format("%s >= %f", m_FieldName_List.GetAt(col),atof(mydlg.m_from));
					break;
				case SA_dtDateTime:
					m_WhereBetween_Cmd.Format("%s >= \'%s\'", m_FieldName_List.GetAt(col), mydlg.m_from);
					if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
					{
						m_WhereBetween_Cmd.Format("%s >= to_date('%s','yyyy-mm-dd hh24:mi:ss')", m_FieldName_List.GetAt(col), mydlg.m_from);
					}
					break;
				case SA_dtString:
				case SA_dtLongChar:
				case SA_dtCLob:
					m_WhereBetween_Cmd.Format("%s >= \'%s\'", m_FieldName_List.GetAt(col), mydlg.m_from);
					break;
				}
			}
			if( !mydlg.m_to.IsEmpty() )
			{
				CString str;
				if( !m_WhereBetween_Cmd.IsEmpty() )
				{
					switch( DataType ) {
					case SA_dtShort:
						str.Format(" and %s <= %d", m_FieldName_List.GetAt(col),atoi(mydlg.m_to));
						break;
					case SA_dtLong:
						str.Format(" and %s <= %d", m_FieldName_List.GetAt(col),atol(mydlg.m_to));
						break;
					case SA_dtBool:
						if( mydlg.m_to.Compare("是") == 0 )
							str.Format(" and %s <= 1", m_FieldName_List.GetAt(col));
						else
							str.Format(" and %s <= 0", m_FieldName_List.GetAt(col));
						break;
					case SA_dtDouble:
						str.Format(" and %s <= %f", m_FieldName_List.GetAt(col),atof(mydlg.m_to));
						break;
					case SA_dtDateTime:
						str.Format(" and %s <= \'%s\'", m_FieldName_List.GetAt(col), mydlg.m_to);
						if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
						{
							str.Format(" and %s <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", m_FieldName_List.GetAt(col), mydlg.m_to);
						}
						break;
					case SA_dtString:
					case SA_dtLongChar:
					case SA_dtCLob:
						str.Format(" and %s <= \'%s\'", m_FieldName_List.GetAt(col), mydlg.m_to);
						break;
					}
				}
				else
				{
					switch( DataType ) {
					case SA_dtShort:
						str.Format(" %s <= %d", m_FieldName_List.GetAt(col),atoi(mydlg.m_to));
						break;
					case SA_dtLong:
						str.Format(" %s <= %d", m_FieldName_List.GetAt(col),atol(mydlg.m_to));
						break;
					case SA_dtBool:
						if( mydlg.m_to.Compare("是") == 0 )
							str.Format(" %s <= 1", m_FieldName_List.GetAt(col));
						else
							str.Format(" %s <= 0", m_FieldName_List.GetAt(col));
						break;
					case SA_dtDouble:
						str.Format(" %s <= %f", m_FieldName_List.GetAt(col),atof(mydlg.m_to));
						break;
					case SA_dtDateTime:
						str.Format(" %s <= \'%s\'", m_FieldName_List.GetAt(col), mydlg.m_to);
						if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
						{
							str.Format(" %s <= to_date('%s','yyyy-mm-dd hh24:mi:ss')", m_FieldName_List.GetAt(col), mydlg.m_to);
						}
						break;
					case SA_dtString:
					case SA_dtLongChar:
					case SA_dtCLob:
						str.Format(" %s <= \'%s\'", m_FieldName_List.GetAt(col), mydlg.m_to);
						break;
					}
				}
				m_WhereBetween_Cmd += str;
			}
			
			if( !m_WhereBetween_Cmd.IsEmpty() )
			{
				m_WhereBetween_Cmd = " where " + m_WhereBetween_Cmd;
			}
		}
		
		ResetGridSize();
		RedrawAll();

		((CDBList *)GetParent())->m_state0 = false;
		((CDBList *)GetParent())->m_state1 = false;
		((CDBList *)GetParent())->m_state2 = false;
		((CDBList *)GetParent())->m_state3 = false;
		((CDBList *)GetParent())->m_state4 = false;
		((CDBList *)GetParent())->m_state5 = false;
		((CDBList *)GetParent())->m_state6 = false;
		return;
	}

	if( item == 1002 )        //  1002 - 模糊查询
	{
		SADataType_t  DataType = (SADataType_t) m_FieldType_List.GetAt( col );
		if(!( DataType == SA_dtLongChar || DataType == SA_dtCLob || DataType == SA_dtString ) )
		{
			return;
		}

		StrEdit mydlg("请输入（" + m_FieldDescription_List.GetAt(col) +"）模糊查询关键字",this);
		mydlg.m_FieldName = m_FieldName_List.GetAt(col);
		mydlg.m_FieldInfo = m_FieldInfo_List.GetAt(col);
		mydlg.m_type      = 1;

		if(mydlg.DoModal() == IDCANCEL)   return;
		CString str = mydlg.m_EditStr;
		str.TrimLeft();  str.TrimRight();

		m_WhereBetween_Cmd.Empty();

		m_WhereBetween_Cmd.Format(" where %s like \'%%%s%%\'",m_FieldName_List.GetAt(col), str);

		ResetGridSize();
		RedrawAll();

		((CDBList *)GetParent())->m_state0 = false;
		((CDBList *)GetParent())->m_state1 = false;
		((CDBList *)GetParent())->m_state2 = false;
		((CDBList *)GetParent())->m_state3 = false;
		((CDBList *)GetParent())->m_state4 = false;
		((CDBList *)GetParent())->m_state5 = false;
		((CDBList *)GetParent())->m_state6 = false;
		return;
	}

	if( item == 1003 )     //  1003 - 自定义查询
	{
		theApp.ConnectDatabase();
		
		CCompositeQuery mydlg;

		try
		{
			SAString cmdstr = "Select TOP 1 * from " + theApp.TABLE_MAIN;
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				cmdstr = "Select * from " + theApp.TABLE_MAIN + " where ROWNUM<=1";
			}
			g_dbcommand.setCommandText(cmdstr);
			
			g_dbcommand.Execute();
			
			g_dbcommand.FetchNext();
			int FieldCount = g_dbcommand.FieldCount();
			for( int i = 1; i <= FieldCount; i++ )
			{
				CString str = g_dbcommand.Field(i).Name();
				
				//AfxMessageBox(str);

				SADataType_t tt = g_dbcommand.Field(i).FieldType();
				if(!( tt == SA_dtShort    || 
					  tt == SA_dtLong     || 
					  tt == SA_dtDouble   || 
					  tt == SA_dtBool     || 
					  tt == SA_dtDateTime || 
					  tt == SA_dtString   ||
					  tt == SA_dtLongChar ||
					  tt == SA_dtCLob ))
				{
					continue;
				}

				mydlg.m_NameList.Add(g_dbcommand.Field(i).Name());
				mydlg.m_TypeList.Add(g_dbcommand.Field(i).FieldType());
				
				

				for( int k = 0; k < theApp.m_fnamelist.GetSize(); k++ )
				{
					if( str.CompareNoCase( theApp.m_fnamelist.GetAt( k ) ) == 0 )  break;
				}

				if( k < theApp.m_fnamelist.GetSize() )
				{
					mydlg.m_DescriptionList.Add( theApp.m_fdescriptionlist.GetAt(k) );
					mydlg.m_InfoList.Add(theApp.m_finfolist.GetAt(k));
				}
				else
				{
					mydlg.m_DescriptionList.Add( str );
					mydlg.m_InfoList.Add("");
				}
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
		}

		CString tmpstr = m_WhereBetween_Cmd;
		tmpstr.TrimLeft(); tmpstr.TrimRight();
		if(tmpstr.Find("where") >= 0 && !tmpstr.IsEmpty())
		{
			tmpstr = tmpstr.Right(tmpstr.GetLength() - tmpstr.Find("where") - 5);
			mydlg.m_query = tmpstr;
		}
		else
		{
			mydlg.m_query = m_CompositeQueryString;
		}
		//mydlg.m_query = m_CompositeQueryString;

		if(mydlg.DoModal() == IDCANCEL)   return;

		m_CompositeQueryString = mydlg.m_query;

		if(mydlg.m_query.IsEmpty())  return;
		
		m_WhereBetween_Cmd.Empty();

		m_WhereBetween_Cmd = " where (" + mydlg.m_query + ")";

		ResetGridSize();
		RedrawAll();

		((CDBList *)GetParent())->m_state0 = false;
		((CDBList *)GetParent())->m_state1 = false;
		((CDBList *)GetParent())->m_state2 = false;
		((CDBList *)GetParent())->m_state3 = false;
		((CDBList *)GetParent())->m_state4 = false;
		((CDBList *)GetParent())->m_state5 = false;
		((CDBList *)GetParent())->m_state6 = false;
		return;
	}

	if( item == 1005 )    //  1005 - 取消范围指定
	{
		m_WhereBetween_Cmd.Empty();
		ResetGridSize();
		RedrawAll();

		((CDBList *)GetParent())->m_state0 = false;
		((CDBList *)GetParent())->m_state1 = false;
		((CDBList *)GetParent())->m_state2 = false;
		((CDBList *)GetParent())->m_state3 = false;
		((CDBList *)GetParent())->m_state4 = false;
		((CDBList *)GetParent())->m_state5 = false;
		((CDBList *)GetParent())->m_state6 = false;
		return;
	}

	if( item == 1004 )  // 1004 - 组合查询
	{
		CQuickQuery dlg;
		if(dlg.DoModal() == IDCANCEL)  return;

		if(dlg.m_query.IsEmpty())  return;
		
		m_WhereBetween_Cmd.Empty();
		m_WhereBetween_Cmd = " where (" + dlg.m_query + ")";
		ResetGridSize();
		RedrawAll();

		((CDBList *)GetParent())->m_state0 = false;
		((CDBList *)GetParent())->m_state1 = false;
		((CDBList *)GetParent())->m_state2 = false;
		((CDBList *)GetParent())->m_state3 = false;
		((CDBList *)GetParent())->m_state4 = false;
		((CDBList *)GetParent())->m_state5 = false;
		((CDBList *)GetParent())->m_state6 = false;
		return;
	}

	if( item == 1007 ) // 1007 - 我的报告
	{
		m_WhereBetween_Cmd.Empty();

		if(theApp.m_MyReportQueryMode.CompareNoCase("模糊") == 0)
		{
			m_WhereBetween_Cmd.Format(" where (czys LIKE '%%%s%%' or zzys LIKE '%%%s%%' or shys LIKE '%%%s%%')", theApp.m_Cpp_User, theApp.m_Cpp_User, theApp.m_Cpp_User);
		}
		else
		{
			m_WhereBetween_Cmd.Format(" where (czys = '%s' or zzys = '%s' or shys = '%s')", theApp.m_Cpp_User, theApp.m_Cpp_User, theApp.m_Cpp_User);
		}

		ResetGridSize();
		RedrawAll();
		
		((CDBList *)GetParent())->m_state0 = false;
		((CDBList *)GetParent())->m_state1 = false;
		((CDBList *)GetParent())->m_state2 = false;
		((CDBList *)GetParent())->m_state3 = false;
		((CDBList *)GetParent())->m_state4 = false;
		((CDBList *)GetParent())->m_state5 = false;
		((CDBList *)GetParent())->m_state6 = false;
		return;
	}

	if( item == 1032 ) // 1032 - 我的未诊断报告 (未诊断)
	{
		m_WhereBetween_Cmd.Empty();

	/*
		if(theApp.m_MyReportQueryMode.CompareNoCase("模糊") == 0)
			{
				m_WhereBetween_Cmd.Format(" where (bgzt&4)=0 and (czys LIKE '%%%s%%' or zzys LIKE '%%%s%%' or shys LIKE '%%%s%%')", theApp.m_Cpp_User, theApp.m_Cpp_User, theApp.m_Cpp_User);
			}
			else
			{
				m_WhereBetween_Cmd.Format(" where (bgzt&4)=0 and (czys = '%s' or zzys = '%s' or shys = '%s')", theApp.m_Cpp_User, theApp.m_Cpp_User, theApp.m_Cpp_User);
			}
	
			if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
			{
				if(theApp.m_MyReportQueryMode.CompareNoCase("模糊") == 0)
				{
					m_WhereBetween_Cmd.Format(" where bitand(bgzt,4)=0 and (czys LIKE '%%%s%%' or zzys LIKE '%%%s%%' or shys LIKE '%%%s%%')", theApp.m_Cpp_User, theApp.m_Cpp_User, theApp.m_Cpp_User);
				}
				else
				{
					m_WhereBetween_Cmd.Format(" where bitand(bgzt,4)=0 and (czys = '%s' or zzys = '%s' or shys = '%s')", theApp.m_Cpp_User, theApp.m_Cpp_User, theApp.m_Cpp_User);
				}
			}*/
	
		m_WhereBetween_Cmd.Format(" where (bgzt < 8) and (czyj is null) and (czys like '%%%s%%') or (bgzt < 8) and (xgyj is null) and (zzys like '%%%s%%') or (bgzt < 8) and (wyyj is null) and (shys like '%%%s%%')",theApp.m_Cpp_User,theApp.m_Cpp_User,theApp.m_Cpp_User);
		ResetGridSize();
		RedrawAll();

		((CDBList *)GetParent())->m_state0 = false;
		((CDBList *)GetParent())->m_state1 = false;
		((CDBList *)GetParent())->m_state2 = false;
		((CDBList *)GetParent())->m_state3 = false;
		((CDBList *)GetParent())->m_state4 = false;
		((CDBList *)GetParent())->m_state5 = false;
		((CDBList *)GetParent())->m_state6 = false;
		return;
	}

	if( item == 1008 ) // 1008 - 我的未审核报告 (已诊断)
	{
		m_WhereBetween_Cmd.Empty();

		if(theApp.m_MyReportQueryMode.CompareNoCase("模糊") == 0)
		{
			m_WhereBetween_Cmd.Format(" where (bgzt&4)=4 and (bgzt&8)=0 and (czys LIKE '%%%s%%' or zzys LIKE '%%%s%%' or shys LIKE '%%%s%%')", theApp.m_Cpp_User, theApp.m_Cpp_User, theApp.m_Cpp_User);
		}
		else
		{
			m_WhereBetween_Cmd.Format(" where (bgzt&4)=4 and (bgzt&8)=0 and (czys = '%s' or zzys = '%s' or shys = '%s')", theApp.m_Cpp_User, theApp.m_Cpp_User, theApp.m_Cpp_User);
		}

		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			if(theApp.m_MyReportQueryMode.CompareNoCase("模糊") == 0)
			{
				m_WhereBetween_Cmd.Format(" where bitand(bgzt,4)=4 and bitand(bgzt,8)=0 and (czys LIKE '%%%s%%' or zzys LIKE '%%%s%%' or shys LIKE '%%%s%%')", theApp.m_Cpp_User, theApp.m_Cpp_User, theApp.m_Cpp_User);
			}
			else
			{
				m_WhereBetween_Cmd.Format(" where bitand(bgzt,4)=4 and bitand(bgzt,8)=0 and (czys = '%s' or zzys = '%s' or shys = '%s')", theApp.m_Cpp_User, theApp.m_Cpp_User, theApp.m_Cpp_User);
			}
		}

		ResetGridSize();
		RedrawAll();

		((CDBList *)GetParent())->m_state0 = false;
		((CDBList *)GetParent())->m_state1 = false;
		((CDBList *)GetParent())->m_state2 = false;
		((CDBList *)GetParent())->m_state3 = false;
		((CDBList *)GetParent())->m_state4 = false;
		((CDBList *)GetParent())->m_state5 = false;
		((CDBList *)GetParent())->m_state6 = false;
		return;
	}

	if( item == 1009 ) // 1009 - 我的未打印报告 (已诊断)
	{
		m_WhereBetween_Cmd.Empty();

		if(theApp.m_MyReportQueryMode.CompareNoCase("模糊") == 0)
		{
			m_WhereBetween_Cmd.Format(" where (bgzt&4)=4 and (bgzt&16)=0 and (czys LIKE '%%%s%%' or zzys LIKE '%%%s%%' or shys LIKE '%%%s%%')", theApp.m_Cpp_User, theApp.m_Cpp_User, theApp.m_Cpp_User);
		}
		else
		{
			m_WhereBetween_Cmd.Format(" where (bgzt&4)=4 and (bgzt&16)=0 and (czys = '%s' or zzys = '%s' or shys = '%s')", theApp.m_Cpp_User, theApp.m_Cpp_User, theApp.m_Cpp_User);
		}

		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			if(theApp.m_MyReportQueryMode.CompareNoCase("模糊") == 0)
			{
				m_WhereBetween_Cmd.Format(" where bitand(bgzt,4)=4 and bitand(bgzt,16)=0 and (czys LIKE '%%%s%%' or zzys LIKE '%%%s%%' or shys LIKE '%%%s%%')", theApp.m_Cpp_User, theApp.m_Cpp_User, theApp.m_Cpp_User);
			}
			else
			{
				m_WhereBetween_Cmd.Format(" where bitand(bgzt,4)=4 and bitand(bgzt,16)=0 and (czys = '%s' or zzys = '%s' or shys = '%s')", theApp.m_Cpp_User, theApp.m_Cpp_User, theApp.m_Cpp_User);
			}
		}

		ResetGridSize();
		RedrawAll();

		((CDBList *)GetParent())->m_state0 = false;
		((CDBList *)GetParent())->m_state1 = false;
		((CDBList *)GetParent())->m_state2 = false;
		((CDBList *)GetParent())->m_state3 = false;
		((CDBList *)GetParent())->m_state4 = false;
		((CDBList *)GetParent())->m_state5 = false;
		((CDBList *)GetParent())->m_state6 = false;
		return;
	}

	if( item == 1038 ) // 1038 - 我的未发报告
	{
		m_WhereBetween_Cmd.Empty();

		if(theApp.m_MyReportQueryMode.CompareNoCase("模糊") == 0)
		{
			m_WhereBetween_Cmd.Format(" where (bgzt&16)=0 and (czys LIKE '%%%s%%' or zzys LIKE '%%%s%%' or shys LIKE '%%%s%%')", theApp.m_Cpp_User, theApp.m_Cpp_User, theApp.m_Cpp_User);
		}
		else
		{
			m_WhereBetween_Cmd.Format(" where (bgzt&16)=0 and (czys = '%s' or zzys = '%s' or shys = '%s')", theApp.m_Cpp_User, theApp.m_Cpp_User, theApp.m_Cpp_User);
		}

		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			if(theApp.m_MyReportQueryMode.CompareNoCase("模糊") == 0)
			{
				m_WhereBetween_Cmd.Format(" where bitand(bgzt,16)=0 and (czys LIKE '%%%s%%' or zzys LIKE '%%%s%%' or shys LIKE '%%%s%%')", theApp.m_Cpp_User, theApp.m_Cpp_User, theApp.m_Cpp_User);
			}
			else
			{
				m_WhereBetween_Cmd.Format(" where bitand(bgzt,16)=0 and (czys = '%s' or zzys = '%s' or shys = '%s')", theApp.m_Cpp_User, theApp.m_Cpp_User, theApp.m_Cpp_User);
			}
		}

		ResetGridSize();
		RedrawAll();

		((CDBList *)GetParent())->m_state0 = false;
		((CDBList *)GetParent())->m_state1 = false;
		((CDBList *)GetParent())->m_state2 = false;
		((CDBList *)GetParent())->m_state3 = false;
		((CDBList *)GetParent())->m_state4 = false;
		((CDBList *)GetParent())->m_state5 = false;
		((CDBList *)GetParent())->m_state6 = false;
		return;
	}

	if( item == 1010 ) // 1010 - 我的收藏记录
	{
		m_WhereBetween_Cmd.Empty();

		m_WhereBetween_Cmd.Format(" where collectdoctor LIKE '%%%s%%'", theApp.m_Cpp_User);

		ResetGridSize();
		RedrawAll();

		((CDBList *)GetParent())->m_state0 = false;
		((CDBList *)GetParent())->m_state1 = false;
		((CDBList *)GetParent())->m_state2 = false;
		((CDBList *)GetParent())->m_state3 = false;
		((CDBList *)GetParent())->m_state4 = false;
		((CDBList *)GetParent())->m_state5 = false;
		((CDBList *)GetParent())->m_state6 = false;
		return;
	}

	if( item == 1011 ) // 1011 - 未发送报告
	{
		m_WhereBetween_Cmd.Empty();

		m_WhereBetween_Cmd.Format(" where (bgzt&8)=8 and (bgzt&32)=0");

		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			m_WhereBetween_Cmd.Format(" where bitand(bgzt,8)=8 and bitand(bgzt,32)=0");
		}

		ResetGridSize();
		RedrawAll();

		((CDBList *)GetParent())->m_state0 = false;
		((CDBList *)GetParent())->m_state1 = false;
		((CDBList *)GetParent())->m_state2 = false;
		((CDBList *)GetParent())->m_state3 = false;
		((CDBList *)GetParent())->m_state4 = false;
		((CDBList *)GetParent())->m_state5 = false;
		((CDBList *)GetParent())->m_state6 = false;
		return;
	}

	if( item == 1013 ) // 1013 - 删除记录
	{
		CStringArray  MedicalNolist;
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
					
					rowlist.Add(nRow);
					MedicalNolist.Add(str_MedicalNo);
				}
			}
			
			nRet = EnumNextSelected( &nCol, &nRow );
		}

		rowlist.RemoveAll();
		
		if( MedicalNolist.GetSize() <= 0 )
		{
			return;
		}

		if(!RIGHT_HQSJMM(theApp.m_Cpp_Right))
		{
			StrEdit mydlg("请输入随机密码",this);
			if(mydlg.DoModal() == IDCANCEL)  return;
			CString ps = mydlg.m_EditStr;
			ps.TrimLeft();  ps.TrimRight();
			if(ps.Compare(((CMainFrame*)AfxGetMainWnd())->RandomCode()))  return;
		}

		CString str;
		str.Format("确认删除选中的 %d 条病历档案吗？", MedicalNolist.GetSize());
		if(AfxMessageBox(str,MB_OKCANCEL|MB_ICONQUESTION) == IDCANCEL)
		{
			MedicalNolist.RemoveAll();
			return;
		}

		for(int i = 0; i < MedicalNolist.GetSize(); i ++ )
		{
			if(MedicalNolist.GetAt(i).CompareNoCase(theApp.GetView()->GetDocument()->m_blh) == 0)
			{
				((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_COMMAND,ID_FILE_NEW);
				break;
			}
		}

		theApp.ConnectDatabase();
		
		for( i = 0; i < MedicalNolist.GetSize(); i ++ )
		{
			int m_bgzt = 0;
			try
			{
				SAString cmdstr = "Select bgzt from " + theApp.TABLE_MAIN + "  where medicalno = :medicalno";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(i);
				g_dbcommand.Execute();

				if( g_dbcommand.FetchNext() )
				{
					m_bgzt = g_dbcommand.Field("bgzt").asLong();
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
//				AfxMessageBox((const char*)x.ErrText());
//				return;
			}

			if(m_bgzt & 0x08)
			{
				AfxMessageBox("当前记录已锁定，只有审核医师取消审核后才可以删除！");
				continue;
			}

			theApp.OperationLogoRecord("删除", MedicalNolist.GetAt(i));

			try
			{
				SAString cmdstr = "Delete from " + theApp.TABLE_MAIN + "  where medicalno = :medicalno";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(i);
				
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
//				AfxMessageBox((const char*)x.ErrText());
//				return;
			}

			try
			{
				SAString cmdstr = "Delete from " + theApp.TABLE_MATERIAL + "  where medicalno = :medicalno";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(i);
				
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
			}

			if(!theApp.m_RecyclePath.IsEmpty())
			{
				mkdir(theApp.m_RecyclePath);
				mkdir(theApp.m_RecyclePath + MedicalNolist.GetAt(i) + "\\");
				int index = 1;
				try
				{
					SAString cmdstr = "Select * from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(i);
					
					g_dbcommand.Execute();
					while(g_dbcommand.FetchNext())
					{
						SAString Bstr = g_dbcommand.Field("imgdataset").asBLob();
						if( !Bstr.IsEmpty() )
						{
							const void * binbuf = (const void *)Bstr;
							BYTE * dataptr = (BYTE *)binbuf;
							
							CString transname;
							transname.Format("%s_%d_cmipstrans.dat", MedicalNolist.GetAt(i), index);
							
							CFile file;
							CFileException fe;
							
							CString fstr = theApp.m_SysTmpPath;
							fstr += transname;
							remove(fstr);
							if (file.Open( fstr , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
							{
								file.WriteHuge( dataptr , Bstr.GetLength());
								file.Close();
							}
							
							GDALDataset * m_pDataset = (GDALDataset *) GDALOpen ( fstr , GA_ReadOnly );
							CString m_file_type = GDALGetDriverShortName( m_pDataset->GetDriver() );
							GDALClose(m_pDataset);
							
							CString targetFname; targetFname.Empty();
							if(m_file_type == "BMP")
							{
								targetFname.Format("%s_%d.bmp", MedicalNolist.GetAt(i), index);
							}
							if(m_file_type == "JPEG")
							{
								targetFname.Format("%s_%d.jpg", MedicalNolist.GetAt(i), index);
							}
							if(m_file_type == "GTiff") 
							{
								targetFname.Format("%s_%d.tif", MedicalNolist.GetAt(i), index);
							}
							
							if(!targetFname.IsEmpty())
							{
								CopyFile(fstr,theApp.m_RecyclePath + MedicalNolist.GetAt(i) + "\\" + targetFname,FALSE);
							}

							remove(fstr);
							
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
			}

			try
			{
				SAString cmdstr = "Delete from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(i);
				
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
//				AfxMessageBox((const char*)x.ErrText());
//				return;
			}

			try
			{
				SAString cmdstr = "Delete from " + theApp.TABLE_SOUND + "  where medicalno = :medicalno";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(i);
				
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
//				AfxMessageBox((const char*)x.ErrText());
//				return;
			}
		}

		MedicalNolist.RemoveAll();

		ResetGridSize();
		RedrawAll();

		return;
	}

    if( item == 1014 ) // 1014 - 修改编号
	{
		if(row >= 0 && row < GetNumberRows())
		{
			ClearSelections();
			Select( 0, row );

			CString oldno;
			CUGCell  cell;
			GetCellIndirect(0 , row, &cell);
			cell.GetText(&oldno);

			((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_COMMAND,ID_FILE_NEW);

			StrEdit mydlg("请输入新的编号（原编号为：" + oldno +"）",this);
			if(mydlg.DoModal() == IDCANCEL)  return;
			CString newno = mydlg.m_EditStr;
			newno.TrimLeft();  newno.TrimRight();
			if(newno.IsEmpty())  return;

			theApp.ConnectDatabase();
			int row_num = 0;
			
			try
			{
				SAString cmdstr = "Select TOP 1 * from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
				if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
				{
					cmdstr = "Select * from " + theApp.TABLE_MAIN + " where medicalno = :medicalno and ROWNUM<=1";
				}
				g_dbcommand.setCommandText( cmdstr );
				g_dbcommand.Param("medicalno").setAsString() = newno;
				
				g_dbcommand.Execute();
				
				if( g_dbcommand.FetchNext() )
				{
					row_num = 1;
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
			
			if( row_num > 0)
			{
				AfxMessageBox("数据库中已经存在指定活检编号的登陆记录");
				return;
			}

			int bgzt;
			try
			{
				SAString cmdstr = "Select bgzt from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
				if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
				{
					cmdstr = "Select bgzt from " + theApp.TABLE_MAIN + " where medicalno = :medicalno ";
				}
				g_dbcommand.setCommandText( cmdstr );
				g_dbcommand.Param("medicalno").setAsString() = oldno;
				
				g_dbcommand.Execute();
				
				if( g_dbcommand.FetchNext() )
				{
					bgzt = g_dbcommand.Field(1).asLong();
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

			if(bgzt & 0x08){
				AfxMessageBox("报告已审核，不能修改编号!");
				return;
			}

			try
			{
				SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set medicalno = :1 where medicalno = :2";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param(2).setAsString() = oldno;
				
				g_dbcommand.Param(1).setAsString() = newno;
				
				g_dbcommand.Execute();
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

			try
			{
				SAString cmdstr = "Update " + theApp.TABLE_MATERIAL + " set medicalno = :1 where medicalno = :2";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param(2).setAsString() = oldno;
				
				g_dbcommand.Param(1).setAsString() = newno;
				
				g_dbcommand.Execute();
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
			
			try
			{
				SAString cmdstr = "Update " + theApp.TABLE_IMAGE + " set medicalno = :1 where medicalno = :2";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param(2).setAsString() = oldno;
				
				g_dbcommand.Param(1).setAsString() = newno;
				
				g_dbcommand.Execute();
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

			try
			{
				SAString cmdstr = "Update " + theApp.TABLE_SOUND + " set medicalno = :1 where medicalno = :2";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param(2).setAsString() = oldno;
				
				g_dbcommand.Param(1).setAsString() = newno;
				
				g_dbcommand.Execute();
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

			ResetGridSize();
			RedrawAll();
		}

		return;
	}

	if( item == 1016 )     //  1016 - 批量打印
	{
		CStringArray  MedicalNolist;
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
					
					rowlist.Add(nRow);
					MedicalNolist.Add(str_MedicalNo);
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
			return;
		}

		//-------------------------------------------
		CFmtSelect mydlg(this);
		if(mydlg.DoModal() == IDOK)
		{
			CString OriginalBlh = theApp.GetView()->GetDocument()->m_blh;
			if(!OriginalBlh.IsEmpty())
			{
				theApp.GetView()->ForceSave();
			}

			bool StopPreview = false;
			bool StopPrint   = false;
			CBatchPrintCtrl  *pCtrlBar = new CBatchPrintCtrl(AfxGetMainWnd());
			pCtrlBar->pStopPreview = &StopPreview;
			pCtrlBar->pStopPrint   = &StopPrint;
			pCtrlBar->Create();
			pCtrlBar->SetActiveWindow();
			pCtrlBar->ShowWindow(SW_SHOW);

			//===================================================
			CStringArray ListFmt;
			CUIntArray   ListImgNum;
			ListFmt.RemoveAll();
			ListImgNum.RemoveAll();
			
			if(mydlg.m_method == 2)
			{
				CStringArray bdlist; bdlist.RemoveAll();
				CStdioFile File;
				if ( File.Open(theApp.m_Exe_Working_Dir + "system\\表单.txt", CFile::modeRead | CFile::typeText) )
				{
					CString tmp;
					while(1)
					{
						if( File.ReadString(tmp) == FALSE ) break;
						tmp.TrimLeft(); tmp.TrimRight();
						if(!tmp.IsEmpty())
						{
							bdlist.Add(tmp);
						}
					}
					File.Close();
				}

				try
				{
					g_dbcommand.setCommandText("Select rpt_name, pic_num from REPORTFORMAT where dbtype = :dbtype");
					g_dbcommand.Param("dbtype").setAsLong() = theApp.m_DB_Type_No;
					g_dbcommand.Execute();
					
					while( g_dbcommand.FetchNext() )
					{
						CString str = g_dbcommand.Field("rpt_name").asString();
						if(str.CompareNoCase("制片条码") == 0 || str.CompareNoCase("回执单") == 0 || str.CompareNoCase("冰冻报告") == 0 || str.CompareNoCase("材块移交表") == 0 || str.CompareNoCase("切片移交表") == 0 || str.CompareNoCase("医嘱工作单") == 0 || str.CompareNoCase("制片情况统计表") == 0)  continue;

						if(bdlist.GetSize() > 0)
						{
							for(int nx = 0; nx < bdlist.GetSize(); nx++)
							{
								if(bdlist.GetAt(nx).CompareNoCase(str) == 0)  break;
							}
							if(nx < bdlist.GetSize())  continue;
						}
						
						ListFmt.Add( g_dbcommand.Field("rpt_name").asString() );
						ListImgNum.Add( g_dbcommand.Field("pic_num").asLong() );
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

				bdlist.RemoveAll();
			}
			
			//===================================================
			
			theApp.Is_Batch_Printing = true;
		
			for(int index = 0; index < MedicalNolist.GetSize(); index++)
			{
				if( StopPrint )  break;
				
				theApp.m_FmtName_in_Batch.Empty();
				
				//--------------------
				switch(mydlg.m_method) {
				case 0:
					theApp.m_FmtName_in_Batch = mydlg.fmtstr;
					break;
				case 1:
					try
					{
						SAString cmdstr = "Select TOP 1 bggs from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
						if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
						{
							cmdstr = "Select bggs from " + theApp.TABLE_MAIN + " where medicalno = :medicalno and ROWNUM<=1";
						}
						g_dbcommand.setCommandText(cmdstr);
						g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
						
						g_dbcommand.Execute();
						
						if(g_dbcommand.FetchNext())
						{
							theApp.m_FmtName_in_Batch = g_dbcommand.Field("bggs").asString();
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
					}
					break;
				case 2:
					{
						int picnum = 0;
						try
						{
							SAString cmdstr = "Select count(*) from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno";
							g_dbcommand.setCommandText(cmdstr);
							g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
							
							g_dbcommand.Execute();
							if(g_dbcommand.FetchNext())
							{
								picnum = g_dbcommand.Field(1).asLong();
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

						for( UINT i = 0; i < ListImgNum.GetSize(); i++ )
						{
							if( ListImgNum.GetAt(i) == picnum )
							{
								theApp.m_FmtName_in_Batch = ListFmt.GetAt( i );
								break;
							}
						}
						
						if( theApp.m_FmtName_in_Batch.IsEmpty() )
						{
							int kepi = -1;
							int dis = 10000;
							for( UINT i = 0; i < ListImgNum.GetSize(); i++ )
							{
								if( ListImgNum.GetAt(i) > picnum )  continue;
								if( (picnum - ListImgNum.GetAt(i)) < dis )
								{
									kepi = i;
									dis = picnum - ListImgNum.GetAt(i);
								}
							}
							if( kepi >= 0)
							{
								theApp.m_FmtName_in_Batch = ListFmt.GetAt( kepi );
							}
						}
						
						if( theApp.m_FmtName_in_Batch.IsEmpty() )
						{
							int kepi = -1;
							int dis = 10000;
							for( UINT i = 0; i < ListImgNum.GetSize(); i++ )
							{
								if( ListImgNum.GetAt(i) <= picnum )  continue;
								if( (ListImgNum.GetAt(i) - picnum) < dis )
								{
									kepi = i;
									dis = ListImgNum.GetAt(i) - picnum;
								}
							}
							if( kepi >= 0)
							{
								theApp.m_FmtName_in_Batch = ListFmt.GetAt( kepi );
							}
						}
						if( theApp.m_FmtName_in_Batch.IsEmpty() && ListFmt.GetSize() > 0)
						{
							theApp.m_FmtName_in_Batch = ListFmt.GetAt( 0 );
						}
					}
					break;
				}
				//--------------------
				if(theApp.m_FmtName_in_Batch.IsEmpty())  continue;

				theApp.OpenDocumentFile(MedicalNolist.GetAt(index));	

				theApp.GetView()->SendMessage(WM_COMMAND,ID_FILE_PRINT);
				while (theApp.InPrinting) {
					MSG msg;
					if (::PeekMessage((LPMSG)&msg,(HWND)NULL,(WORD)NULL,(WORD)NULL,TRUE))
					{
						TranslateMessage((LPMSG)&msg);
						DispatchMessage((LPMSG)&msg);
					}	
				}
			}
			
			if (pCtrlBar != NULL)
			{
				pCtrlBar->DestroyWindow();
				delete pCtrlBar;
				pCtrlBar = NULL;
			}
			
			if(!OriginalBlh.IsEmpty())
			{
				theApp.OpenDocumentFile(OriginalBlh);	
				((CMainFrame *)AfxGetMainWnd())->SendMessage(WM_COMMAND,ID_APP_REFRESH);
			}

			theApp.Is_Batch_Printing = false;

			ListFmt.RemoveAll();
			ListImgNum.RemoveAll();
		}
		//-------------------------------------------

		MedicalNolist.RemoveAll();

		ResetGridSize();
		RedrawAll();
		return;
	}

	if( item == 1018 )     //  1018 - 当天记录
	{
		m_WhereBetween_Cmd.Empty();

		COleDateTime curt = theApp.GetOleServerDateTime();
		COleDateTime uptime,downtime;
		uptime = downtime = curt;
		uptime.SetDateTime(curt.GetYear(), curt.GetMonth(), curt.GetDay(), 0,0,0);
		downtime.SetDateTime(curt.GetYear(), curt.GetMonth(), curt.GetDay(), 23,59,59);
		CString upstr,downstr;
		upstr.Format("%04d-%02d-%02d %02d:%02d:%02d",uptime.GetYear(),uptime.GetMonth(),uptime.GetDay(),uptime.GetHour(),uptime.GetMinute(),uptime.GetSecond());
		downstr.Format("%04d-%02d-%02d %02d:%02d:%02d",downtime.GetYear(),downtime.GetMonth(),downtime.GetDay(),downtime.GetHour(),downtime.GetMinute(),downtime.GetSecond());

		m_WhereBetween_Cmd.Format(" where sjrq>=\'%s\' and sjrq <=\'%s\'",upstr,downstr);
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			m_WhereBetween_Cmd.Format(" where sjrq>=to_date('%s','yyyy-mm-dd hh24:mi:ss') and sjrq <=to_date('%s','yyyy-mm-dd hh24:mi:ss')",upstr,downstr);
		}

		ResetGridSize();
		RedrawAll();

		((CDBList *)GetParent())->m_state0 = false;
		((CDBList *)GetParent())->m_state1 = false;
		((CDBList *)GetParent())->m_state2 = false;
		((CDBList *)GetParent())->m_state3 = false;
		((CDBList *)GetParent())->m_state4 = false;
		((CDBList *)GetParent())->m_state5 = false;
		((CDBList *)GetParent())->m_state6 = false;
		return;
	}

	if( item == 1019 )     //  1019 - 昨天记录
	{
		m_WhereBetween_Cmd.Empty();

		COleDateTime curt = theApp.GetOleServerDateTime();
		COleDateTimeSpan tspan;
		tspan.SetDateTimeSpan(1, 0, 0, 0);
		curt -= tspan;

		COleDateTime uptime,downtime;
		uptime = downtime = curt;
		uptime.SetDateTime(curt.GetYear(), curt.GetMonth(), curt.GetDay(), 0,0,0);
		downtime.SetDateTime(curt.GetYear(), curt.GetMonth(), curt.GetDay(), 23,59,59);
		CString upstr,downstr;
		upstr.Format("%04d-%02d-%02d %02d:%02d:%02d",uptime.GetYear(),uptime.GetMonth(),uptime.GetDay(),uptime.GetHour(),uptime.GetMinute(),uptime.GetSecond());
		downstr.Format("%04d-%02d-%02d %02d:%02d:%02d",downtime.GetYear(),downtime.GetMonth(),downtime.GetDay(),downtime.GetHour(),downtime.GetMinute(),downtime.GetSecond());

		m_WhereBetween_Cmd.Format(" where sjrq>=\'%s\' and sjrq <=\'%s\'",upstr,downstr);
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			m_WhereBetween_Cmd.Format(" where sjrq>=to_date('%s','yyyy-mm-dd hh24:mi:ss') and sjrq <=to_date('%s','yyyy-mm-dd hh24:mi:ss')",upstr,downstr);
		}

		ResetGridSize();
		RedrawAll();

		((CDBList *)GetParent())->m_state0 = false;
		((CDBList *)GetParent())->m_state1 = false;
		((CDBList *)GetParent())->m_state2 = false;
		((CDBList *)GetParent())->m_state3 = false;
		((CDBList *)GetParent())->m_state4 = false;
		((CDBList *)GetParent())->m_state5 = false;
		((CDBList *)GetParent())->m_state6 = false;
		return;
	}

	if( item == 1020 )     //  1020 - 前天记录
	{
		m_WhereBetween_Cmd.Empty();

		COleDateTime curt = theApp.GetOleServerDateTime();
		COleDateTimeSpan tspan;
		tspan.SetDateTimeSpan(2, 0, 0, 0);
		curt -= tspan;

		COleDateTime uptime,downtime;
		uptime = downtime = curt;
		uptime.SetDateTime(curt.GetYear(), curt.GetMonth(), curt.GetDay(), 0,0,0);
		downtime.SetDateTime(curt.GetYear(), curt.GetMonth(), curt.GetDay(), 23,59,59);
		CString upstr,downstr;
		upstr.Format("%04d-%02d-%02d %02d:%02d:%02d",uptime.GetYear(),uptime.GetMonth(),uptime.GetDay(),uptime.GetHour(),uptime.GetMinute(),uptime.GetSecond());
		downstr.Format("%04d-%02d-%02d %02d:%02d:%02d",downtime.GetYear(),downtime.GetMonth(),downtime.GetDay(),downtime.GetHour(),downtime.GetMinute(),downtime.GetSecond());

		m_WhereBetween_Cmd.Format(" where sjrq>=\'%s\' and sjrq <=\'%s\'",upstr,downstr);
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			m_WhereBetween_Cmd.Format(" where sjrq>=to_date('%s','yyyy-mm-dd hh24:mi:ss') and sjrq <=to_date('%s','yyyy-mm-dd hh24:mi:ss')",upstr,downstr);
		}

		ResetGridSize();
		RedrawAll();

		((CDBList *)GetParent())->m_state0 = false;
		((CDBList *)GetParent())->m_state1 = false;
		((CDBList *)GetParent())->m_state2 = false;
		((CDBList *)GetParent())->m_state3 = false;
		((CDBList *)GetParent())->m_state4 = false;
		((CDBList *)GetParent())->m_state5 = false;
		((CDBList *)GetParent())->m_state6 = false;
		return;
	}

	if( item == 1021 )     //  1021 - 本周记录
	{
		m_WhereBetween_Cmd.Empty();

		COleDateTime curt = theApp.GetOleServerDateTime();
		COleDateTimeSpan tspan;
		int dayindex = curt.GetDayOfWeek() - 1;
		if(dayindex == 0)  dayindex = 7;
		
		tspan.SetDateTimeSpan(dayindex - 1, 0, 0, 0);

		COleDateTime uptime,downtime;
		uptime   = curt - tspan;
		downtime = curt;
		uptime.SetDateTime(uptime.GetYear(), uptime.GetMonth(), uptime.GetDay(), 0,0,0);
		downtime.SetDateTime(downtime.GetYear(), downtime.GetMonth(), downtime.GetDay(), 23,59,59);
		CString upstr,downstr;
		upstr.Format("%04d-%02d-%02d %02d:%02d:%02d",uptime.GetYear(),uptime.GetMonth(),uptime.GetDay(),uptime.GetHour(),uptime.GetMinute(),uptime.GetSecond());
		downstr.Format("%04d-%02d-%02d %02d:%02d:%02d",downtime.GetYear(),downtime.GetMonth(),downtime.GetDay(),downtime.GetHour(),downtime.GetMinute(),downtime.GetSecond());

		m_WhereBetween_Cmd.Format(" where sjrq>=\'%s\' and sjrq <=\'%s\'",upstr,downstr);
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			m_WhereBetween_Cmd.Format(" where sjrq>=to_date('%s','yyyy-mm-dd hh24:mi:ss') and sjrq <=to_date('%s','yyyy-mm-dd hh24:mi:ss')",upstr,downstr);
		}

		ResetGridSize();
		RedrawAll();

		((CDBList *)GetParent())->m_state0 = false;
		((CDBList *)GetParent())->m_state1 = false;
		((CDBList *)GetParent())->m_state2 = false;
		((CDBList *)GetParent())->m_state3 = false;
		((CDBList *)GetParent())->m_state4 = false;
		((CDBList *)GetParent())->m_state5 = false;
		((CDBList *)GetParent())->m_state6 = false;
		return;
	}

	if( item == 1022 )     //  1022 - 本月记录
	{
		m_WhereBetween_Cmd.Empty();

		COleDateTime curt = theApp.GetOleServerDateTime();
		COleDateTime uptime,downtime;
		uptime = downtime = curt;
		uptime.SetDateTime(curt.GetYear(), curt.GetMonth(), 1, 0,0,0);
		downtime.SetDateTime(curt.GetYear(), curt.GetMonth(), curt.GetDay(), 23,59,59);
		CString upstr,downstr;
		upstr.Format("%04d-%02d-%02d %02d:%02d:%02d",uptime.GetYear(),uptime.GetMonth(),uptime.GetDay(),uptime.GetHour(),uptime.GetMinute(),uptime.GetSecond());
		downstr.Format("%04d-%02d-%02d %02d:%02d:%02d",downtime.GetYear(),downtime.GetMonth(),downtime.GetDay(),downtime.GetHour(),downtime.GetMinute(),downtime.GetSecond());

		m_WhereBetween_Cmd.Format(" where sjrq>=\'%s\' and sjrq <=\'%s\'",upstr,downstr);
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			m_WhereBetween_Cmd.Format(" where sjrq>=to_date('%s','yyyy-mm-dd hh24:mi:ss') and sjrq <=to_date('%s','yyyy-mm-dd hh24:mi:ss')",upstr,downstr);
		}

		ResetGridSize();
		RedrawAll();

		((CDBList *)GetParent())->m_state0 = false;
		((CDBList *)GetParent())->m_state1 = false;
		((CDBList *)GetParent())->m_state2 = false;
		((CDBList *)GetParent())->m_state3 = false;
		((CDBList *)GetParent())->m_state4 = false;
		((CDBList *)GetParent())->m_state5 = false;
		((CDBList *)GetParent())->m_state6 = false;
		return;
	}

	if( item == 1023 )     //  1023 - 本年记录
	{
		m_WhereBetween_Cmd.Empty();

		COleDateTime curt = theApp.GetOleServerDateTime();
		COleDateTime uptime,downtime;
		uptime = downtime = curt;
		uptime.SetDateTime(curt.GetYear(), 1, 1, 0,0,0);
		downtime.SetDateTime(curt.GetYear(), 12, 31, 23,59,59);
		CString upstr,downstr;
		upstr.Format("%04d-%02d-%02d %02d:%02d:%02d",uptime.GetYear(),uptime.GetMonth(),uptime.GetDay(),uptime.GetHour(),uptime.GetMinute(),uptime.GetSecond());
		downstr.Format("%04d-%02d-%02d %02d:%02d:%02d",downtime.GetYear(),downtime.GetMonth(),downtime.GetDay(),downtime.GetHour(),downtime.GetMinute(),downtime.GetSecond());

		m_WhereBetween_Cmd.Format(" where sjrq>=\'%s\' and sjrq <=\'%s\'",upstr,downstr);

		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			m_WhereBetween_Cmd.Format(" where sjrq>=to_date('%s','yyyy-mm-dd hh24:mi:ss') and sjrq <=to_date('%s','yyyy-mm-dd hh24:mi:ss')",upstr,downstr);
		}

		ResetGridSize();
		RedrawAll();

		((CDBList *)GetParent())->m_state0 = false;
		((CDBList *)GetParent())->m_state1 = false;
		((CDBList *)GetParent())->m_state2 = false;
		((CDBList *)GetParent())->m_state3 = false;
		((CDBList *)GetParent())->m_state4 = false;
		((CDBList *)GetParent())->m_state5 = false;
		((CDBList *)GetParent())->m_state6 = false;
		return;
	}

	if( item == 1024 )        //  1024 - 批量发送
	{
		CStringArray  MedicalNolist;
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
					
					rowlist.Add(nRow);
					MedicalNolist.Add(str_MedicalNo);
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
			return;
		}

		theApp.SubmitReport(MedicalNolist);

		MedicalNolist.RemoveAll();
		ResetGridSize();
		RedrawAll();
		return;
	}

	if( item == 1025 )        //  1025 - 病历浏览
	{
		CString  runstr;
		runstr = theApp.m_Exe_Working_Dir + "OlympusCam.exe";
		CString  args;  args.Empty();
		args.Format(" %s", QuickGetText(0, row));
		//-----------------------------
		extern HANDLE launchViaCreateProcess(LPCTSTR program, LPCTSTR args);
		launchViaCreateProcess(runstr, args);

/*
		if(row >= 0 && row < GetNumberRows())
		{
			((CMainFrame *)AfxGetMainWnd())->ShowRecordInfo(QuickGetText(0, row));
		}
*/
		return;
	}

	if( item == 1026 )        //  1026 - 图像显示
	{
		if(row >= 0 && row < GetNumberRows())
		{
			CString curno = QuickGetText(0, row);
			curno.TrimLeft(); curno.TrimRight();
			if(!curno.IsEmpty())
			{
				try
				{
					SAString cmdstr = "Select * from " + theApp.TABLE_IMAGE + " where medicalno = :medicalno order by serialno desc";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = curno;
					
					g_dbcommand.Execute();
					if(g_dbcommand.FetchNext())
					{
						SAString Bstr = g_dbcommand.Field("imgdataset").asBLob();
						if( !Bstr.IsEmpty() )
						{
							const void * binbuf = (const void *)Bstr;
							BYTE * dataptr = (BYTE *)binbuf;
							
							CFile file;
							CFileException fe;
							
							CString fstr = theApp.m_SysTmpPath;
							fstr += "pic.dat";
							remove(fstr);
							if (file.Open( fstr , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
							{
								file.WriteHuge( dataptr , Bstr.GetLength());
								file.Close();
							}
							
							CFigure figure(fstr,
								           190,80,  //window位置
								           640,480, //显示大小
								           curno, g_dbcommand.Field("serialno").asLong());
							
							remove(fstr);
						}
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
		}
		return;
	}

	if( item == 1027 )     //  1027 - 批量审核
	{
		if(AfxMessageBox("您确认对当前选中记录进行审核？",MB_OKCANCEL) == IDCANCEL){
		    return;
		}
		CStringArray  MedicalNolist;
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
					
					rowlist.Add(nRow);
					MedicalNolist.Add(str_MedicalNo);
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
			return;
		}

		for(int index = 0; index < MedicalNolist.GetSize(); index++)
		{
		
			CString nZdyj;  nZdyj.Empty();
			int m_bgzt = 0;
			CString czys,zzys,shys;
			try
			{
				SAString cmdstr = "Select TOP 1 bgzt, zdyj, czys, zzys, shys from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
				if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
				{
					cmdstr = "Select bgzt, zdyj from " + theApp.TABLE_MAIN + " where medicalno = :medicalno and ROWNUM<=1";
				}
				g_dbcommand.setCommandText( cmdstr );
				g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
				g_dbcommand.Execute();
				
				if( g_dbcommand.FetchNext() )
				{
					m_bgzt = g_dbcommand.Field("bgzt").asLong();
					nZdyj  = g_dbcommand.Field("zdyj").asString();
					czys = g_dbcommand.Field("czys").asString();
					zzys = g_dbcommand.Field("zzys").asString();
					shys = g_dbcommand.Field("shys").asString();
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
			}
		if(theApp.m_AuthorCheckOnlyMode.CompareNoCase("打开") == 0){
			if(!(shys.IsEmpty()) && theApp.m_Cpp_User.CompareNoCase(shys) == 0){
				
			} else if(shys.IsEmpty() && theApp.m_Cpp_User.CompareNoCase(zzys) == 0){
				
			} else {
				AfxMessageBox("因报告权限控制，您不能审核病例《"+MedicalNolist.GetAt(index) + "》!");
				continue;
			}
		}
			
			if(m_bgzt & 0x08)
			{
				continue;
			}
			
			if(m_bgzt < 4)
			{
				AfxMessageBox("病例《" + MedicalNolist.GetAt(index) + "》还未下诊断意见，不能审核！");
				continue;
			}
			
			nZdyj.TrimLeft();  nZdyj.TrimRight();
			
			if(nZdyj.IsEmpty())
			{
				AfxMessageBox("病例《" + MedicalNolist.GetAt(index) + "》诊断意见为空，不能审核！");
				continue;
			}
			
			m_bgzt = (m_bgzt | 0x08);
			
			try
			{
				SAString cmdstr;
				if(theApp.m_ShysMode.CompareNoCase("是") == 0)
					cmdstr = "Update " + theApp.TABLE_MAIN + " set bgzt=:bgzt, shys=:shys where medicalno = :medicalno";
				else
					cmdstr = "Update " + theApp.TABLE_MAIN + " set bgzt=:bgzt where medicalno = :medicalno";
				g_dbcommand.setCommandText(cmdstr);
				
				if(theApp.m_ShysMode.CompareNoCase("是") == 0)
				{
					g_dbcommand.Param("shys").setAsString() = theApp.m_Cpp_User;
				}
				
				g_dbcommand.Param("bgzt").setAsLong() = m_bgzt;
				g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(index);
				
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

			if(theApp.m_HisCfg.CompareNoCase("通用接口") == 0)
			{
				theApp.NotifyRecordStatus(MedicalNolist.GetAt(index));
			}
		}

		MedicalNolist.RemoveAll();

		ResetGridSize();
		RedrawAll();
		RedrawAll();

		if(	!theApp.GetView()->GetDocument()->m_blh.IsEmpty() )
		{
			CString nBlh = theApp.GetView()->GetDocument()->m_blh;
			((CMainFrame *)AfxGetMainWnd())->SendMessage(WM_COMMAND,ID_FILE_NEW);
			theApp.OpenDocumentFile(nBlh);
		}

		return;
	}

	if( item == 1028 ) // 1028 - 未审核报告
	{
		m_WhereBetween_Cmd.Empty();

		m_WhereBetween_Cmd.Format(" where (bgzt&4)=4 and (bgzt&8)=0");

		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			m_WhereBetween_Cmd.Format(" where bitand(bgzt,4)=4 and bitand(bgzt,8)=0");
		}

		ResetGridSize();
		RedrawAll();

		((CDBList *)GetParent())->m_state0 = false;
		((CDBList *)GetParent())->m_state1 = false;
		((CDBList *)GetParent())->m_state2 = false;
		((CDBList *)GetParent())->m_state3 = false;
		((CDBList *)GetParent())->m_state4 = false;
		((CDBList *)GetParent())->m_state5 = false;
		((CDBList *)GetParent())->m_state6 = false;
		return;
	}

	if( item == 1029 ) // 1029 - 冰冻报告
	{
		m_WhereBetween_Cmd.Empty();

		m_WhereBetween_Cmd.Format(" where (bdh IS NOT NULL and bdh != '') or (bdjg IS NOT NULL and bdjg != '')");
		
		if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
		{
			m_WhereBetween_Cmd.Format(" where (bdh IS NOT NULL) or (bdjg IS NOT NULL)");
		}
		ResetGridSize();
		RedrawAll();

		((CDBList *)GetParent())->m_state0 = false;
		((CDBList *)GetParent())->m_state1 = false;
		((CDBList *)GetParent())->m_state2 = false;
		((CDBList *)GetParent())->m_state3 = false;
		((CDBList *)GetParent())->m_state4 = false;
		((CDBList *)GetParent())->m_state5 = false;
		((CDBList *)GetParent())->m_state6 = false;
		return;
	}

	if( item == 1037 ) // 1037 - 特检报告
	{
		m_WhereBetween_Cmd.Empty();

		m_WhereBetween_Cmd.Format(" where wfyy like '%%免疫组化%%' or wfyy like '%%特殊染色%%' or wfyy like '%%分子病理%%' or wfyy like '%%电镜%%' or wfyy like '%%其它%%'");

		ResetGridSize();
		RedrawAll();

		((CDBList *)GetParent())->m_state0 = false;
		((CDBList *)GetParent())->m_state1 = false;
		((CDBList *)GetParent())->m_state2 = false;
		((CDBList *)GetParent())->m_state3 = false;
		((CDBList *)GetParent())->m_state4 = false;
		((CDBList *)GetParent())->m_state5 = false;
		((CDBList *)GetParent())->m_state6 = false;
		return;
	}

	if( item == 1030 ) // 1030 - 数值统计
	{
		SADataType_t  DataType = (SADataType_t) m_FieldType_List.GetAt( col );
		if( !( DataType == SA_dtShort || DataType == SA_dtLong || DataType == SA_dtDouble ) )
		{
			AfxMessageBox("非数值类型字段，不能进行数值统计！");
			return;
		}

		char ct[3];
		ct[0] = 0x0d; ct[1] = 0x0a;  ct[2] = '\0';

		CString infostr, str;
		infostr.Empty();
		infostr.Format("字段：    %s", m_FieldName_List.GetAt(col));
		infostr += ct;
		str.Format("字段名称：%s", m_FieldDescription_List.GetAt(col));
		infostr += str;
		infostr += ct;
		infostr += ct;
		infostr += "统计结果：";
		infostr += ct;
		infostr += "--------------------------";
		infostr += ct;

		try
		{
			SAString cmdstr;

			for(int k = 0; k < 6; k++ )
			{
				CString substr;
				switch(k) {
				case 0:
					substr.Format("avg(%s)",m_FieldName_List.GetAt(col));
					break;
				case 1:
					substr.Format("max(%s)",m_FieldName_List.GetAt(col));
					break;
				case 2:
					substr.Format("min(%s)",m_FieldName_List.GetAt(col));
					break;
				case 3:
					substr.Format("sum(%s)",m_FieldName_List.GetAt(col));
					break;
				case 4:
					substr.Format("stdev(%s)",m_FieldName_List.GetAt(col));
					break;
				case 5:
					substr.Format("var(%s)",m_FieldName_List.GetAt(col));
					break;
				}

				CString tail;
				if(((CButton *)((CDBList *)GetParent())->GetDlgItem(IDC_CHECK_ALLDB))->GetState() & 0x0003)
				{
					if(m_WhereBetween_Cmd.IsEmpty())
						tail.Format(" where 1 = 1");
				}
				else
				{
					if(m_WhereBetween_Cmd.IsEmpty())
						tail.Format(" where dbtype = %d", theApp.m_DB_Type_No);
					else
						tail.Format(" and dbtype = %d", theApp.m_DB_Type_No);
				}

				cmdstr = "Select " + substr + " from " + theApp.TABLE_MAIN + m_WhereBetween_Cmd + tail;
				
				g_dbcommand.setCommandText(cmdstr);
			    g_dbcommand.Execute();
			
				if( g_dbcommand.FetchNext() )
				{
					switch(k) {
					case 0:
						str.Format("平均值：      (%.4f)",g_dbcommand.Field(1).asDouble());
						break;
					case 1:
						str.Format("最大值：      (%.4f)",g_dbcommand.Field(1).asDouble());
						break;
					case 2:
						str.Format("最小值：      (%.4f)",g_dbcommand.Field(1).asDouble());
						break;
					case 3:
						str.Format("所有值的和：  (%.4f)",g_dbcommand.Field(1).asDouble());
						break;
					case 4:
						str.Format("统计标准偏差：(%.4f)",g_dbcommand.Field(1).asDouble());
						break;
					case 5:
						str.Format("统计方差：    (%.4f)",g_dbcommand.Field(1).asDouble());
						break;
					}
					infostr += str;
					infostr += ct;
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

		infostr += "--------------------------";
		infostr += ct;

		CTextOut mydlg(this);
		mydlg.m_Text = infostr;
		mydlg.m_Title.Format("字段< %s > 统计信息",m_FieldName_List.GetAt(col));
		mydlg.DoModal();

		return;
	}

	if( item == 1031 ) // 1031 - 图像导出
	{
		CStringArray  MedicalNolist;
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
					
					rowlist.Add(nRow);
					MedicalNolist.Add(str_MedicalNo);
				}
			}
			
			nRet = EnumNextSelected( &nCol, &nRow );
		}

		rowlist.RemoveAll();
		
		if( MedicalNolist.GetSize() <= 0 )
		{
			return;
		}

		CXTBrowseDialog dlg;
		dlg.SetTitle(_T("选择输出文件目录"));
		char buf[1000];
		strcpy(buf, theApp.m_Exe_Working_Dir + "mail\\");
		dlg.SetSelPath(buf);
		if (dlg.DoModal() == IDCANCEL)  
		{
			MedicalNolist.RemoveAll();
			return;
		}
		
		CString m_Dir = dlg.GetSelPath();
		m_Dir.TrimLeft(); 	m_Dir.TrimRight();
		if(m_Dir.GetAt(m_Dir.GetLength()-1) != '\\')
		{
			m_Dir += "\\";
		}
		

		bool IsDeleteImg = false;
		if(AfxMessageBox("是否删除数据库中的图像数据？",MB_OKCANCEL|MB_ICONQUESTION) == IDOK)
		{
			if(!RIGHT_HQSJMM(theApp.m_Cpp_Right))
			{
				StrEdit mydlg("请输入随机密码",this);
				if(mydlg.DoModal() == IDOK)
				{
					CString ps = mydlg.m_EditStr;
					ps.TrimLeft();  ps.TrimRight();
					if(ps.Compare(((CMainFrame*)AfxGetMainWnd())->RandomCode()) == 0)
					{
						IsDeleteImg = true;
					}
				}
			}
			else
			{
				IsDeleteImg = true;
			}
		}

		theApp.ConnectDatabase();
		
		CMyProgressBar  *pProgressBar = NULL;
		pProgressBar = new CMyProgressBar(AfxGetMainWnd());
		pProgressBar->Create();
		pProgressBar->SetActiveWindow();
		pProgressBar->ShowWindow(SW_SHOW);
		pProgressBar->CenterWindow(AfxGetMainWnd());
		
		pProgressBar->m_bar.SetRange(0,MedicalNolist.GetSize());
		pProgressBar->m_bar.SetPos(0);
		pProgressBar->m_bar.SetStep(1);
		((CStatic *) pProgressBar->GetDlgItem(IDC_STATIC_MSG))->SetWindowText("请稍后...");

		for(int i = 0; i < MedicalNolist.GetSize(); i ++ )
		{
			try
			{
				SAString cmdstr = "Select * from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(i);
				
				g_dbcommand.Execute();
				while(g_dbcommand.FetchNext())
				{
					SAString Bstr = g_dbcommand.Field("imgdataset").asBLob();
					if( !Bstr.IsEmpty() )
					{
						const void * binbuf = (const void *)Bstr;
						BYTE * dataptr = (BYTE *)binbuf;
						
						CString transname;
						transname.Format("%s_%d_cmipstrans.dat", MedicalNolist.GetAt(i), g_dbcommand.Field("serialno").asLong());
						
						CFile file;
						CFileException fe;
						
						CString fstr = theApp.m_SysTmpPath;
						fstr += transname;
						remove(fstr);
						if (file.Open( fstr , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
						{
							file.WriteHuge( dataptr , Bstr.GetLength());
							file.Close();
						}
						
						GDALDataset * m_pDataset = (GDALDataset *) GDALOpen ( fstr , GA_ReadOnly );
						CString m_file_type = GDALGetDriverShortName( m_pDataset->GetDriver() );
						GDALClose(m_pDataset);
						
						CString targetFname; targetFname.Empty();
						if(m_file_type == "BMP")
						{
							targetFname.Format("%s_%d.bmp", MedicalNolist.GetAt(i), g_dbcommand.Field("serialno").asLong());
						}
						if(m_file_type == "JPEG")
						{
							targetFname.Format("%s_%d.jpg", MedicalNolist.GetAt(i), g_dbcommand.Field("serialno").asLong());
						}
						if(m_file_type == "GTiff") 
						{
							targetFname.Format("%s_%d.tif", MedicalNolist.GetAt(i), g_dbcommand.Field("serialno").asLong());
						}
						
						if(!targetFname.IsEmpty())
						{
							CopyFile(fstr, m_Dir + targetFname,FALSE);
						}
						remove(fstr);
					}
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
			
			if(IsDeleteImg)
			{
				try
				{
					SAString cmdstr = "Delete from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(i);
					
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
			}

			//------------------------------------------------------------------
			CString msg;
			msg.Format("%d / %d", i + 1, MedicalNolist.GetSize());
			((CStatic *) pProgressBar->GetDlgItem(IDC_STATIC_VALUE))->SetWindowText(msg);
			pProgressBar->m_bar.StepIt();
			//------------------------------------------------------------------
		}

		MedicalNolist.RemoveAll();

		if (pProgressBar != NULL)
		{
			pProgressBar->DestroyWindow();
			SAFE_DELETE(pProgressBar);
		}

		if(IsDeleteImg)
		{
			ResetGridSize();
			RedrawAll();
		}

		return;
	}

	if( item == 1033 || item == 1034 || item == 1035 ) //  提交初诊医师、主诊医师、审核医师
	{
		CStringArray  MedicalNolist;
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
					
					rowlist.Add(nRow);
					MedicalNolist.Add(str_MedicalNo);
				}
			}
			
			nRet = EnumNextSelected( &nCol, &nRow );
		}

		rowlist.RemoveAll();
		
		if( MedicalNolist.GetSize() <= 0 )
		{
			return;
		}
		CString temp; CStringArray MedicalNolist1;
		CString s;s.Empty();
		switch(item){
		    case 1033:
				s = "czys";
				break;
			case 1034:
				s = "zzys";
				break;
			case 1035:
				s = "shys";
				break;
		}
		for(int i1 = 0; i1 < MedicalNolist.GetSize() ; i1++){
			try{
				SAString cmdstr = "select "+s+" from "+ theApp.TABLE_MAIN + " where medicalno =:medicalno";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = MedicalNolist.GetAt(i1);
				g_dbcommand.Execute();
				while(g_dbcommand.FetchNext()){
					temp = g_dbcommand.Field((SAString)s).asString();
				}
				if(!temp.IsEmpty()){
					if(AfxMessageBox("病理号为"+MedicalNolist.GetAt(i1)+"的记录已提交给"+temp+"医师,是否覆盖后继续提交?",MB_OKCANCEL) == IDCANCEL){
						
					} else {
						MedicalNolist1.Add(MedicalNolist.GetAt(i1));
					}
				} else {
					MedicalNolist1.Add(MedicalNolist.GetAt(i1));
				}
			} catch (SAException &e){
				AfxMessageBox((const char*)e.ErrText());
			}
			
		}
		if(MedicalNolist1.GetSize() <= 0){
			return;
		}

		int i;
		CString DoctorName, infostring, tmp;

		CFieldOut mydlg;
		switch(item) {
		case 1033:
			mydlg.title.Format("选择初诊医师");
			for(i = 0; i < theApp.m_fnamelist.GetSize(); i++)
			{
				if(theApp.m_fnamelist.GetAt(i).CompareNoCase("czys") == 0)
				{
					infostring = theApp.m_finfolist.GetAt(i);
					break;
				}
			}
			break;
		case 1034:
			mydlg.title.Format("选择主诊医师");
			for(i = 0; i < theApp.m_fnamelist.GetSize(); i++)
			{
				if(theApp.m_fnamelist.GetAt(i).CompareNoCase("zzys") == 0)
				{
					infostring = theApp.m_finfolist.GetAt(i);
					break;
				}
			}
			break;
		case 1035:
			mydlg.title.Format("选择审核医师");
			for(i = 0; i < theApp.m_fnamelist.GetSize(); i++)
			{
				if(theApp.m_fnamelist.GetAt(i).CompareNoCase("shys") == 0)
				{
					infostring = theApp.m_finfolist.GetAt(i);
					break;
				}
			}
			break;
		}

		if(infostring.IsEmpty() || infostring.GetAt(0) == '{')
		{
			MedicalNolist.RemoveAll();
			return;
		}

		CStringArray strlist;
		while(1)
		{
			if(infostring.Find('&') == -1)  break;
			tmp = infostring.Left(infostring.Find('&'));
			mydlg.strlist.Add(tmp);
			strlist.Add(tmp);
			infostring = infostring.Right(infostring.GetLength() - infostring.Find('&') - 1);
		}

		if(mydlg.strlist.GetSize() < 1)
		{
			MedicalNolist.RemoveAll();
			return;
		}
		
		if(mydlg.DoModal() == IDCANCEL)
		{
			MedicalNolist.RemoveAll();
			strlist.RemoveAll();
			return;
		}

		if(mydlg.m_Index >= 0)
		{
			DoctorName = strlist.GetAt(mydlg.m_Index);
		}
		strlist.RemoveAll();

		if(DoctorName.IsEmpty())
		{
			MedicalNolist.RemoveAll();
			return;
		}

		for(i = 0; i < MedicalNolist1.GetSize(); i ++ )
		{
			if(MedicalNolist1.GetAt(i).CompareNoCase(theApp.GetView()->GetDocument()->m_blh) == 0)
			{
				((CMainFrame *)AfxGetMainWnd())->PostMessage(WM_COMMAND,ID_FILE_NEW);
				break;
			}
		}
		
		for( i = 0; i < MedicalNolist1.GetSize(); i ++ )
		{
			SAString cmdstr;
			try
			{
				// 只允许修改未审核报告
				switch(item) {
				case 1033:
					cmdstr = "Update " + theApp.TABLE_MAIN + " set czys=:czys where medicalno = :medicalno and ((bgzt&8)=0 or bgzt IS NULL)";
					break;
				case 1034:
					cmdstr = "Update " + theApp.TABLE_MAIN + " set zzys=:zzys where medicalno = :medicalno and ((bgzt&8)=0 or bgzt IS NULL)";
					break;
				case 1035:
					cmdstr = "Update " + theApp.TABLE_MAIN + " set shys=:shys where medicalno = :medicalno and ((bgzt&8)=0 or bgzt IS NULL)";
					break;
				}
				if(theApp.m_DBType.CompareNoCase("ORACLE") == 0)
				{
					switch(item) {
					case 1033:
						cmdstr = "Update " + theApp.TABLE_MAIN + " set czys=:czys where medicalno = :medicalno and (bitand(bgzt,8)=0 or bgzt IS NULL)";
						break;
					case 1034:
						cmdstr = "Update " + theApp.TABLE_MAIN + " set zzys=:zzys where medicalno = :medicalno and (bitand(bgzt,8)=0 or bgzt IS NULL)";
						break;
					case 1035:
						cmdstr = "Update " + theApp.TABLE_MAIN + " set shys=:shys where medicalno = :medicalno and (bitand(bgzt,8)=0 or bgzt IS NULL)";
						break;
					}
				}
				g_dbcommand.setCommandText(cmdstr);
				switch(item) {
				case 1033:
					g_dbcommand.Param("czys").setAsString() = DoctorName;
					break;
				case 1034:
					g_dbcommand.Param("zzys").setAsString() = DoctorName;
					break;
				case 1035:
					g_dbcommand.Param("shys").setAsString() = DoctorName;
					break;
				}
				g_dbcommand.Param("medicalno").setAsString() = MedicalNolist1.GetAt(i);
				
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

		MedicalNolist1.RemoveAll();

		ResetGridSize();
		RedrawAll();

		switch(item) {
		case 1033:
			AfxMessageBox("您当前选择的记录已成功提交给初诊医生" + DoctorName);
			break;
		case 1034:
			AfxMessageBox("您当前选择的记录已成功提交给主诊医生" + DoctorName);
			break;
		case 1035:
			AfxMessageBox("您当前选择的记录已成功提交给审核医生" + DoctorName);
			break;
		}

		return;
	}


if( item == 2125 ) 
{
	CString ExeDir;
	CString blh,BRID;
	char coffilepath[200];
	
    ExeDir=theApp.m_Exe_Working_Dir +"Pathology.ini";
	
	strcpy(coffilepath,ExeDir);

	int nCol,nRect; long nRow;	
	nRect = EnumFirstSelected(&nCol,&nRow);
	CUGCell  cell;
	GetCellIndirect(0,nRow,&cell);
	cell.GetText(&blh);
	theApp.GetView()->GetDocument()->m_blh = blh;
	theApp.OpenDocumentFile(blh);

/*	
	//theApp.ConnectDatabase();
	
	try
	{
		SAString cmdstr = "Select TOP 1 brid from " + theApp.TABLE_MAIN + " where medicalno=:blh";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("blh").setAsString() =blh;
		
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			BRID = g_dbcommand.Field("brid").asString();
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
	}
    
    CIniReader m_IniReader;
	m_IniReader.setINIFileName(ExeDir);
	
	m_IniReader.setKey(BRID,"brid","病人ID");
	
	HINSTANCE	hLib;
	if( (hLib=LoadLibrary(theApp.m_Exe_Working_Dir + "LoadEMR.dll")) == 0 )
	{
		AfxMessageBox("LoadEMR.dll装载错误！");
		return;
	}
	
	typedef int (*pInfoDownload)(char *); 
	pInfoDownload ProcInDLL;
	ProcInDLL = (pInfoDownload) GetProcAddress(hLib, "LoadEMR");
				
	if( ProcInDLL == NULL )
	{
		AfxMessageBox("LoadEMR函数定位错误！");
		return;
	}
	
	int result = (ProcInDLL) (coffilepath);
	
	FreeLibrary(hLib);
	hLib = 0;			*/
	theApp.OnAppDZBL();
}



}

void CDBGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	if(!(row >= 0 && row < GetNumberRows())) return;
	ClearSelections();
	Select( 0, row );

	CString blh = QuickGetText(0, row);
	theApp.OpenDocumentFile(blh);
}

void CDBGrid::ReflectSelection()
{
	ClearSelections();
	SetCurrentCellMode(1);
	RedrawAll();
}

void CDBGrid::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	if(!(row >= 0 && row < GetNumberRows())) return;
	SetCurrentCellMode(2);
	RedrawAll();
}
