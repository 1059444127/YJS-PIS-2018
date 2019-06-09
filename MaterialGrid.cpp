// MaterialGrid.cpp: implementation of the CMaterialGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "PathologyView.h"
#include "PathologyDoc.h"
#include "MaterialGrid.h"
#include "MedMaterial.h"
#include "Qcmx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CMaterialGrid::CMaterialGrid()
{

}

CMaterialGrid::~CMaterialGrid()
{

}

void CMaterialGrid::OnSetup()
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

	SetMultiSelectMode(TRUE);

	SetCurrentCellMode(2);

	
}

void CMaterialGrid::ResetGridSize()
{
	theApp.ConnectDatabase();
	
	int row_num = 0;
	if(!theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		try
		{
			SAString cmdstr = "Select count(*) from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno";
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
	SetNumberCols(12);



/*	for(int i = 0;i<row_num;i++){
		QuickSetCellType(2,i,UGCT_DROPLIST);
		QuickSetCellTypeEx(2,i,UGCT_DROPLISTHIDEBUTTON);
		QuickSetLabelText(2,i,"－\n+\n++\n+++\n±\n+＜25%\n+25～50%\n+50～75%\n+＞75%\n");
	}   */
	CString qcys;
	for(int i = 0; i < theApp.m_fnamelist.GetSize(); i++)
	{		
		if(theApp.m_fnamelist.GetAt(i).CompareNoCase("qcys") == 0)
		{
			qcys = theApp.m_finfolist.GetAt(i);
		}
	}
	
	qcys.Replace("&","\n");

	CString bgzt;
	try
	{
		SAString cmdstr = "Select bgzt from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		while(g_dbcommand.FetchNext())
		{
			bgzt = g_dbcommand.Field(1).asLong();
		}
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


	if(bgzt < 8)
	{
		for( i = 0 ; i<row_num ; i++)
		{
			QuickSetCellType(4,i,UGCT_DROPLIST);
			QuickSetCellTypeEx(4,i,UGCT_DROPLISTHIDEBUTTON);
			QuickSetLabelText(4,i,qcys);
		}
	} 
	else
	{
		for( i = 0 ; i<row_num ; i++)
		{
			QuickSetCellType(4,i,UGCT_NORMAL);
			//QuickSetCellTypeEx(4,i,UGCT_DROPLISTHIDEBUTTON);
			QuickSetLabelText(4,i,qcys);
		}
	}
	
	CUGCell defcell;
	GetGridDefault(&defcell);
	int cks_count = 0;

	if(!theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno order by qcrq asc";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Execute();
			
			int index = 0;

			while( g_dbcommand.FetchNext() )
			{
				if(g_dbcommand.Field("rwly").asString().CompareNoCase("脱钙") == 0)
				{
					CUGCell cell;
					GetCell(0,index,&cell);
					cell.SetTextColor(RGB(255,0,0));
					cell.SetText("脱钙");
					SetCell(0,index,&cell);
				}
				else
				{
					//CUGCell cell;
					//GetCell(0,index,&cell);
					//cell.SetTextColor(AFX_IDC_COLOR_BLACK);
				//	if(old != NULL)
				//	{
					SetCell(0,index,&defcell);
				//	}					
					QuickSetText(0, index , g_dbcommand.Field("rwly").asString());
				}				
				QuickSetText(1, index , g_dbcommand.Field("xh").asString());
				QuickSetText(2, index , g_dbcommand.Field("qcbw").asString());
				int k = g_dbcommand.Field("cks").asLong();
				cks_count += k;
				CString str;
				str.Format("%d", k);
				QuickSetText(3, index , str);
				QuickSetText(4, index , g_dbcommand.Field("qcys").asString());

				COleDateTime curt;
				SADateTime st;
				if(g_dbcommand.Field("qcrq").isNull())
				{
					QuickSetText(5, index , " ");
				}
				else
				{
					st = g_dbcommand.Field("qcrq").asDateTime();
					curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
					QuickSetText(5, index , curt.Format("%Y-%m-%d"));
				}

				switch(g_dbcommand.Field("zpzt").asLong()) {
				case 0:
					QuickSetText(6, index , "待核对");
					break;
				case 1:
					QuickSetText(6, index , "待包埋");
					break;
				case 2:
					QuickSetText(6, index , "待切片");
					break;
				case 3:
					QuickSetText(6, index , "已切片");
					break;
				}

				QuickSetText(7, index , g_dbcommand.Field("ckhd").asString());

				if(g_dbcommand.Field("hdrq").isNull())
				{
					QuickSetText(8, index , " ");
				}
				else
				{
					st = g_dbcommand.Field("hdrq").asDateTime();
					curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
					QuickSetText(8, index , curt.Format("%Y-%m-%d"));
				}

				QuickSetText(9, index , g_dbcommand.Field("bmczy").asString());

				if(g_dbcommand.Field("bmrq").isNull())
				{
					QuickSetText(10, index , " ");
				}
				else
				{
					st = g_dbcommand.Field("bmrq").asDateTime();
					curt.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
					QuickSetText(10, index , curt.Format("%Y-%m-%d"));
				}

				QuickSetText(11, index , g_dbcommand.Field("bz").asString());
				
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

	CWnd *pParent = GetParent();
	if(pParent->IsKindOf(RUNTIME_CLASS(CMedMaterial)))
	{
		CRect rt;
		pParent->GetDlgItem(IDC_COMBO_RWLY)->GetWindowRect(&rt);
		SetColWidth(0, rt.Width());
		pParent->GetDlgItem(IDC_COMBO_XH)->GetWindowRect(&rt);
		SetColWidth(1, rt.Width());
		pParent->GetDlgItem(IDC_EDIT_QCBW)->GetWindowRect(&rt);
		SetColWidth(2, rt.Width());
		pParent->GetDlgItem(IDC_EDIT_CKS)->GetWindowRect(&rt);
		SetColWidth(3, rt.Width());
		pParent->GetDlgItem(IDC_EDIT_QCYS)->GetWindowRect(&rt);
		SetColWidth(4, rt.Width());
		pParent->GetDlgItem(IDC_DATE_QCRQ)->GetWindowRect(&rt);
		SetColWidth(5, rt.Width());
		SetColWidth(6, 100);

		SetColWidth(7, 100);
		SetColWidth(8, 100);
		SetColWidth(9, 100);
		SetColWidth(10, 100);
		SetColWidth(11, 100);

		pParent->SetDlgItemInt(IDC_EDIT_LKZS, row_num);
		pParent->SetDlgItemInt(IDC_EDIT_CKZS, cks_count);
	}
	else
	{
		for(int i = 0; i < 12; i++)  SetColWidth(i, 100);
	}
}

void CMaterialGrid::OnGetCell(int col,long row,CUGCell *cell)
{
	if( row == -1 )     //  Display table head information of pRecordSet_Define
	{
		if( col >= 0 )
		{
			switch(col) {
			case 0:
				cell->SetText( "任务来源" );
				break;
			case 1:
				cell->SetText( "序号" );
				break;
			case 2:
				cell->SetText( "取材部位" );
				break;
			case 3:
				cell->SetText( "块数" );
				break;
			case 4:
				cell->SetText( "取材医生" );
				break;
			case 5:
				cell->SetText( "取材日期" );
				break;
			case 6:
				cell->SetText( "制片状态" );
				break;
			case 7:
				cell->SetText( "材块核对" );
				break;
			case 8:
				cell->SetText( "核对时间" );
				break;
			case 9:
				cell->SetText( "包埋人" );
				break;
			case 10:
				cell->SetText( "包埋时间" );
				break;
			case 11:
				cell->SetText( "备注" );
				break;
			}
		}
		return;
	}
}

int  CMaterialGrid::OnCanSizeRow(long row)
{
	return FALSE;
}

int  CMaterialGrid::OnCanSizeCol(int col)
{
	return TRUE;
}

void CMaterialGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	if(theApp.m_MaterialMode.CompareNoCase("是") == 0)
	{
		CString qcjlr;
		try
		{
			SAString cmdstr = "select qcjlr from " + theApp.TABLE_MAIN + " where medicalno=:medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Execute();
			while(g_dbcommand.FetchNext())
			{
				qcjlr = g_dbcommand.Field("qcjlr").asString();
			}
			g_dbconnection.Commit();
		}
		catch (SAException& e)
		{
			AfxMessageBox((const char*)e.ErrText());	
		}

		if((theApp.m_Cpp_User.CompareNoCase(QuickGetText(4,row)) != 0) && (theApp.m_Cpp_User.CompareNoCase(qcjlr) != 0))
		{
			AfxMessageBox("您不是当前病例的取材医师或记录人,不能修改记录!");
			return;
		}

	}
	CString bgzt;
	try
	{
		SAString cmdstr = "Select bgzt from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		while(g_dbcommand.FetchNext())
		{
			bgzt = g_dbcommand.Field(1).asLong();
		}
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
	CString zpzt = QuickGetText(6,row);
//	if(RIGHT_XGSCQCJL(theApp.m_Cpp_Right) == 1){
	if(!theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		StartEdit(); 
	}
//	} else {
//	CWnd *pParent = GetParent();
//	if(!pParent->IsKindOf(RUNTIME_CLASS(CMedMaterial)))  return;

//	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty()) return;
 //   if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh)) return;

//	if(((col >= 1 && col <= 4) || col == 11) && row >= 0)
//	{
		CString CheckState = QuickGetText(6, row);
//		if(CheckState.CompareNoCase("待核对") == 0)
//		{
//			StartEdit();
//		}
//	}
//	}
}

int CMaterialGrid::OnEditFinish(int col, long row,CWnd * edit, LPCTSTR string,BOOL cancelFlag)
{
	if(cancelFlag)
	{
		return TRUE;
		//QuickSetText(col,row,string);
	}

	if(((col >= 0 && col <= 4) || col == 11) && row >= 0)
	{
		CString CheckState = QuickGetText(6, row);
		if(CheckState.CompareNoCase("待核对") == 0 || RIGHT_XGSCQCJL(theApp.m_Cpp_Right) == 1)
		{
			//---------------------------------------
			
			if(col == 1)
			{
				int num = 0;
				try
				{
					SAString cmdstr = "Select count(*) from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno and xh = :xh";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
					g_dbcommand.Param("xh").setAsString() = string;
					
					g_dbcommand.Execute();
					
					if( g_dbcommand.FetchNext() )
					{
						num = g_dbcommand.Field(1).asLong();
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
				if(num > 0)
				{
					AfxMessageBox("该病例已经存在相同序号的取材记录，请按【ESC】键取消！");
					return FALSE;
				}
			}
			//---------------------------------------
			CString xh = QuickGetText(1, row);
			SAString cmdstr;
			try
			{
				switch(col) {
				case 0:
					if(RIGHT_XGSCQCJL(theApp.m_Cpp_Right) == 1){
						cmdstr = "Update " + theApp.TABLE_MATERIAL + " set rwly=:rwly where medicalno = :medicalno and xh=:xh";
					}
					break;
				case 1:
					cmdstr = "Update " + theApp.TABLE_MATERIAL + " set xh=:xh1    where medicalno = :medicalno and xh = :xh ";//and zpzt=0
					break;
				case 2:
					cmdstr = "Update " + theApp.TABLE_MATERIAL + " set qcbw=:qcbw where medicalno = :medicalno and xh = :xh ";
					break;
				case 3:
					cmdstr = "Update " + theApp.TABLE_MATERIAL + " set cks=:cks   where medicalno = :medicalno and xh = :xh ";
					break;
				case 4:
					cmdstr = "Update " + theApp.TABLE_MATERIAL + " set qcys=:qcys where medicalno = :medicalno and xh = :xh ";
					break;
				case 11:
					cmdstr = "Update " + theApp.TABLE_MATERIAL + " set bz=:bz     where medicalno = :medicalno and xh = :xh ";
					break;
				}
				g_dbcommand.setCommandText(cmdstr);
				switch(col) {
				case 0:
					g_dbcommand.Param("rwly").setAsString() = string;
					break;
				case 1:
					g_dbcommand.Param("xh1").setAsString()   = string;
					break;
				case 2:
					g_dbcommand.Param("qcbw").setAsString()  = string;
					break;
				case 3:
					g_dbcommand.Param("cks").setAsLong()     = atoi(string);
					break;
				case 4:
					g_dbcommand.Param("qcys").setAsString()  = string;
					break;
				case 11:
					g_dbcommand.Param("bz").setAsString()    = string;
					break;
				}
				g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
				g_dbcommand.Param("xh").setAsString() = xh;
				
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

			ResetGridSize();
			RedrawAll();
		}
	}

	return TRUE;
}

int CMaterialGrid::OnCellTypeNotify(long ID,int col,long row,long msg,long param){
	if(col == 4 && row >= 0)
	{
		if(msg == UGCT_DROPLISTSELECT)
		{	
			
			CString *string = (CString*)param;
			
			theApp.ConnectDatabase();
			
		//	CString yzh = QuickGetText(0 ,row);
			try
			{
				SAString cmdstr = "Update " + theApp.TABLE_MATERIAL + " set qcys = :qcys where medicalno = :medicalno and rwly = :rwly and xh= :xh";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
				g_dbcommand.Param("rwly").setAsString()       = QuickGetText(0 ,row);
				g_dbcommand.Param("xh").setAsString()  = QuickGetText(1 ,row);
				g_dbcommand.Param("qcys").setAsString()        = *string;  
				
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

void CMaterialGrid::OnRClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed){
	int ncol;long nrow; 
	int result = EnumFirstSelected(&ncol,&nrow);
	
	for(int i = 0;i<12;i++){
		EnumNextSelected(&ncol,&nrow);
	}
	int result1 = EnumNextSelected(&ncol,&nrow);
	
	if(result1 == UG_SUCCESS){
		isSingleSel = FALSE;
	}else{
		isSingleSel = TRUE;
	}
	
	if(!result == UG_SUCCESS){
		return;
	}

	CMenu* menu = GetMenu();
	menu->LoadMenu(IDR_MENU_MATERIAL);

	CMenu* pPop = menu->GetSubMenu(0);
	if(GetParent()->IsKindOf(RUNTIME_CLASS(CQcmx)))
	{
		pPop->DeleteMenu(0,MF_BYPOSITION);
		pPop->DeleteMenu(0,MF_BYPOSITION);
		pPop->DeleteMenu(1,MF_BYPOSITION);
		pPop->DeleteMenu(1,MF_BYPOSITION);
	}
	ClientToScreen(point);
	pPop->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point->x ,point->y + GetRowHeight(0),GetParent());
}
