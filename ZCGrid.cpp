// ZCGrid.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "PathologyView.h"
#include "MainFrm.h"
#include "ZCGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ZCGrid dialog

extern CPathologyApp theApp;
ZCGrid::ZCGrid()
{
	
}
ZCGrid::~ZCGrid(){

}

void ZCGrid::ResetGridSize(){
	theApp.ConnectDatabase();

	int row_num = 0;
	int col_num = 10;

	try{
		SAString cmdStr = "select count(*) from "+ theApp.TABLE_ZC + m_WhereBetween_Cmd;
		g_dbcommand.setCommandText(cmdStr);
		g_dbcommand.Execute();
		if(g_dbcommand.FetchNext()){
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

	for(int i = 0; i < row_num; i++){
		QuickSetText(0, i, "");
	}
	CString LastBlh; LastBlh.Empty();
	bool UsingBackgroundColor = false;
	int index = 0;
	CUGCell defcell;
	GetGridDefault(&defcell);
	try{
	SAString cmdstr = "Select medicalno from " + theApp.TABLE_ZC + m_WhereBetween_Cmd;
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
	
    for(i = 0; i < col_num; i++){
		SetColWidth(i,75);
	}
	//SetColWidth(1,0);
	//SetColWidth(7,120);
	if( row_num > 0){
		//GotoRow(0);
	}

}

void ZCGrid::OnSetup(){
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

void ZCGrid::OnGetCell(int col,long row,CUGCell *cell){
	if( row == -1 )     //  Display table head information of pRecordSet_Define
	{
		if( col >= 0 )
		{
			switch(col) {
			case 0:
				cell->SetText( "病理号" );
				break;
			case 1:
				cell->SetText( "姓名" );
				break;
			case 2:
				cell->SetText( "病人ID" );
				break;
			case 3:
				cell->SetText( "医嘱类型" );
				break;
			case 4:
				cell->SetText( "蜡块号" );
				break;
			case 5:
				cell->SetText( "抗体信息" );
				break;
			case 6:
				cell->SetText( "抗体数" );
				break;
			case 7:
				cell->SetText( "收费状态" );
				break;
			case 8:
				cell->SetText( "开单医生" );
				break;
			case 9:
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

	theApp.ConnectDatabase();
	
	try{
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
		CString count; count.Format("%d",bottomrow - toprow);
		CString topRow; topRow.Format("%d",toprow + 1); 
		//----------------------------------------------------------------
	//	SAString cmdstr = "select top " + count +"* from "+theApp.TABLE_ZC+" where medicalno not in (select top " + topRow + " medicalno from " + theApp.TABLE_ZC + ")";
		SAString cmdstr = "select * from "+ theApp.TABLE_ZC + m_WhereBetween_Cmd;
		
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Execute();
		int index = 0; CString s;
		while(g_dbcommand.FetchNext()){
			if(index == 0){
				s = g_dbcommand.Field(1).asString();
			}
			QuickSetText(0,index,g_dbcommand.Field(1).asString());
			QuickSetText(1,index,g_dbcommand.Field(2).asString());
			QuickSetText(2,index,g_dbcommand.Field(3).asString());
			QuickSetText(3,index,g_dbcommand.Field(5).asString());
			QuickSetText(4,index,g_dbcommand.Field(6).asString());
			QuickSetText(5,index,g_dbcommand.Field(7).asString());
			QuickSetText(6,index,g_dbcommand.Field(8).asString());
			QuickSetText(7,index,g_dbcommand.Field(4).asString());
			QuickSetText(8,index,g_dbcommand.Field(10).asString());
			QuickSetText(9,index,g_dbcommand.Field(9).asString());
			index++;
		}
		
		
	} catch (SAException &e){
		AfxMessageBox((const char*)e.ErrText());
	}
}

int  ZCGrid::OnCanSizeRow(long row)
{
	return FALSE;
}

int ZCGrid::OnCanSizeTopHdg()
{
	return FALSE;
}

int ZCGrid::OnCanSizeSideHdg()
{
	return FALSE;
}

void ZCGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed){
	if( col == 9 && row >= 0){
		StartEdit();
	}
}

void ZCGrid::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	CString str_MedicalNo = QuickGetText(0, row);
	((CMainFrame *)theApp.m_pMainWnd)->ShowRecordInfo(str_MedicalNo);
}

int ZCGrid::OnEditFinish(int col, long row,CWnd *edit,LPCTSTR string,BOOL cancelFlag){
    if(row >= 0 && col == 9){
	//	CString bz = QuickGetText(8,row);
		CString blh = QuickGetText(0,row);
	
	
		try{
			SAString cmdstr = "update "+ theApp.TABLE_ZC + " set bz=:bz where medicalno=:medicalno";
			
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("bz").setAsString() = string;
			g_dbcommand.Param("medicalno").setAsString() = blh;
			g_dbcommand.Execute();
			g_dbconnection.Commit();
		} catch (SAException &e){
			try{
				g_dbconnection.Rollback();
			} catch (SAException &e){
				
			}
			AfxMessageBox((const char*)e.ErrText());
		}
	}

	return TRUE;
}

void ZCGrid::OnTH_LClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed){

}
/////////////////////////////////////////////////////////////////////////////
// ZCGrid message handlers
