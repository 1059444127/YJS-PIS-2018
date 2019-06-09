// UserGrid.cpp: implementation of the CUserGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "UserGrid.h"

#include "PassManage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CUserGrid::CUserGrid()
{

}

CUserGrid::~CUserGrid()
{

}

void CUserGrid::OnSetup()
{
	m_font.CreateFont(16,0,0,0,500,0,0,0,0,0,0,0,0,"Arial");
	SetDefFont(&m_font);

	CUGCell cell;
	GetGridDefault(&cell);
	cell.SetAlignment(UG_ALIGNRIGHT);
	cell.SetBorder(UG_BDR_RECESSED);
	cell.SetBackColor(RGB(220,220,220));
	SetGridDefault(&cell);

	SetVScrollMode( UG_SCROLLTRACKING );
	SetHScrollMode( UG_SCROLLTRACKING );

	SetSH_Width(25);
	SetSH_NumberCols(1);

	//SetMultiSelectMode(TRUE);
	SetHighlightRow(TRUE);
	SetDoubleBufferMode(TRUE);

	GridSetup();
}

void CUserGrid::GridSetup()
{
	int row_num = 0;

    theApp.ConnectDatabase();
	
	try
	{
		g_dbcommand.setCommandText("Select count(*) from " + (SAString)theApp.TABLE_SECURITY);
		
		g_dbcommand.Execute();
		
		g_dbcommand.FetchNext();
		row_num = g_dbcommand.Field(1).asLong();
		
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

	int col_num = 4;

	SetNumberRows(row_num);
	SetNumberCols(col_num);

	CRect rt;
	GetWindowRect(&rt);

	for (int index = 0; index < col_num; index++)
	{
		if(index == 2)
			SetColWidth(index, 0);
		else
			SetColWidth(index, (rt.Width() - 45) / 3);
	}

	QuickSetText(0, -1 , "用户名");
	QuickSetText(1, -1 , "用户密码");
	QuickSetText(2, -1 , "用户权限设置参数");
	QuickSetText(3, -1 , "用户信息");

    CPassManage *pParent = (CPassManage *)GetParent();

	try
	{
		g_dbcommand.setCommandText("Select * from " + (SAString)theApp.TABLE_SECURITY);
		g_dbcommand.Execute();
		
		CString str;
	    index = 0;
		while( g_dbcommand.FetchNext() )
		{
			QuickSetText(0, index, g_dbcommand.Field("username").asString());

			// QuickSetText(1, index, g_dbcommand.Field("password").asString());
			QuickSetText(1, index, "******");

            str.Format("%d",g_dbcommand.Field("userright").asLong());
			QuickSetText(2, index, str);
			QuickSetText(3, index, g_dbcommand.Field("userinfo").asString());
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
	}

	if( row_num > 0 )
	{
		GotoRow(0);
        OnRowChange(0,0);
	}
	
	RedrawAll();
}

void CUserGrid::OnGetCell(int col,long row,CUGCell *cell)
{
	if( col == -1 )
	{
		if( row >= 0 )
		{
			if( row == GetCurrentRow() )
				cell->SetCellType(UGCT_ARROW);
		}
		return;
	}
}

int CUserGrid::OnCanSizeRow(long row)
{
	return FALSE;
}

int CUserGrid::OnCanSizeTopHdg()
{
	return FALSE;
}

int CUserGrid::OnCanSizeSideHdg()
{
	return FALSE;
}

void CUserGrid::OnRowChange(long oldrow,long newrow)
{
    CPassManage *pParent = (CPassManage *)GetParent();
	if(pParent->m_RightCtrl.m_hWnd == NULL)  return;
	
	if(newrow < 0 || newrow >= GetNumberRows())   
	{
		for(int i = 0; i < pParent->m_RightCtrl.GetNumberRows(); i++)
		{
			pParent->m_RightCtrl.QuickSetText(0,i,"0");
		}
		pParent->m_RightCtrl.RedrawAll();
		pParent->m_RightCtrl.EnableWindow(TRUE);
		return;
	}

	CString str;
	CUGCell  cell;
	GetCellIndirect(2 , newrow, &cell);
	cell.GetText(&str);
	int right = atoi(str);
	for(int i = 0; i < pParent->m_RightCtrl.GetNumberRows(); i++)
	{
		if( (right >> i) & 0x00000001 )
			pParent->m_RightCtrl.QuickSetText( 0, i, "1");
		else
			pParent->m_RightCtrl.QuickSetText( 0, i, "0");
	}
	pParent->m_RightCtrl.RedrawAll();

	GetCellIndirect(0 , newrow, &cell);
	cell.GetText(&str);
	str.MakeLower();
	if(str.CompareNoCase("administrator") == 0)
	{
		pParent->m_RightCtrl.EnableWindow(FALSE);
	}
	else
	{
		pParent->m_RightCtrl.EnableWindow(TRUE);
	}
}
