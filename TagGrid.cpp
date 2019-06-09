// TagGrid.cpp: implementation of the CTagGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "TagGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CTagGrid::CTagGrid()
{

}

CTagGrid::~CTagGrid()
{

}

void CTagGrid::OnSetup()
{
	m_font.CreateFont(16,0,0,0,500,0,0,0,0,0,0,0,0,"Arial");
	SetDefFont(&m_font);

	CUGCell cell;
	GetGridDefault(&cell);
	cell.SetAlignment(UG_ALIGNLEFT);
	cell.SetBorder(UG_BDR_RECESSED);
	cell.SetBackColor(RGB(220,220,220));
	cell.SetCellType(UGCT_CHECKBOX);
	cell.SetCellTypeEx(UGCT_CHECKBOXCHECKMARK);
	SetGridDefault(&cell);

	SetVScrollMode( UG_SCROLLTRACKING );
	SetHScrollMode( UG_SCROLLTRACKING );

	SetSH_Width(0);
	SetTH_Height(0);

	SetHighlightRow(FALSE);
	SetDoubleBufferMode(TRUE);
}

int CTagGrid::OnCanSizeRow(long row)
{
	return FALSE;
}

int CTagGrid::OnCanSizeTopHdg()
{
	return FALSE;
}

int CTagGrid::OnCanSizeSideHdg()
{
	return FALSE;
}

void CTagGrid::ResetGridSize()
{
	int GridRow = 17, GridCol;
	theApp.ConnectDatabase();
	
	int ItemCount = 0;
	try
	{
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_TAG;
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			ItemCount = g_dbcommand.Field(1).asLong();
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

	GridCol = (ItemCount + GridRow - 1)/ GridRow;
	
	SetNumberCols(GridCol);
	SetNumberRows(GridRow);

	int i,j;
	for(i = 0; i < GridCol; i++)
	{
		SetColWidth(i, 150);
		for(j = 0; j < GridRow; j++)
		{
			QuickSetLabelText(i, j, "");
		}
	}
	
	SetVS_Width(GetSystemMetrics(SM_CXVSCROLL));
	SetHS_Height(GetSystemMetrics(SM_CYHSCROLL));
	AdjustComponentSizes();

	int index = 0;
	try
	{
		SAString cmdstr = "Select * from " + theApp.TABLE_TAG + " order by tagname asc";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Execute();
		
		while( g_dbcommand.FetchNext() )
		{
			int cl = index / GridRow;
			int rw = (index % GridRow);
			QuickSetLabelText(cl, rw, g_dbcommand.Field("tagname").asString());
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

	for(i = 0; i < GridCol; i++)
	{
		for(j = 0; j < GridRow; j++)
		{
			QuickSetText(i, j, "0");
		}
	}
}
