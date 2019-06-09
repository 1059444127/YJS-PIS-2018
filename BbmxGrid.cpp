// BbmxGrid.cpp: implementation of the CBbmxGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "BbmxGrid.h"
#include "BbmxDetail.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBbmxGrid::CBbmxGrid()
{

}

CBbmxGrid::~CBbmxGrid()
{

}

void CBbmxGrid::OnSetup()
{
	m_font.CreateFont(12,0,0,0,400,0,0,0,0,0,0,0,0,"MS Sans Serif");
	SetDefFont(&m_font);	

	//setup the grid
	SetNumberCols(8);

    CBbmxDetail *pParent = (CBbmxDetail *)GetParent();
	int rownum = 0;
	try
	{
		SAString cmdstr = "Select count(*) from EXAM.PATH_TEST_SPECIMEN where TEST_NO = :TEST_NO";
		pParent->hiscmd.setCommandText(cmdstr);
		pParent->hiscmd.Param("TEST_NO").setAsString() = pParent->m_Sqh;
		
		pParent->hiscmd.Execute();
		
		if( pParent->hiscmd.FetchNext() )
		{
			rownum = pParent->hiscmd.Field(1).asLong();
		}
		
		pParent->hiscon.Commit();
	}
	catch(SAException &x)
	{
		try
		{
			pParent->hiscon.Rollback();
		}
		catch(SAException &)
		{
		}
		AfxMessageBox((const char*)x.ErrText());
		return;
	}
	SetNumberRows(rownum);

	pParent->m_Bbmc.Empty();
	try
	{
		SAString cmdstr = "Select * from EXAM.PATH_TEST_SPECIMEN where TEST_NO = :TEST_NO order by SPECIMEN_NO asc";
		pParent->hiscmd.setCommandText(cmdstr);
		pParent->hiscmd.Param("TEST_NO").setAsString() = pParent->m_Sqh;
		
		pParent->hiscmd.Execute();
		
		int index = 0;
		while( pParent->hiscmd.FetchNext() )
		{
			CString str;
			str.Format("%d", pParent->hiscmd.Field("SPECIMEN_NO").asLong());
			QuickSetText(0,index,str);

			QuickSetText(1,index,pParent->hiscmd.Field("SPECIMEN_ID").asString());
			QuickSetText(2,index,pParent->hiscmd.Field("SPECIMEN_NAME").asString());
			if(!pParent->m_Bbmc.IsEmpty())  pParent->m_Bbmc += "\\"; // "、";
			pParent->m_Bbmc += pParent->hiscmd.Field("SPECIMEN_NAME").asString();
			str = pParent->hiscmd.Field("SPECIMEN_CLASS").asString();
			if(str.CompareNoCase("0") == 0)  str = "小标本";
			if(str.CompareNoCase("1") == 0)  str = "大标本";
			if(str.CompareNoCase("2") == 0)  str = "冰冻";
			QuickSetText(3,index,str);
			str.Format("%d", pParent->hiscmd.Field("SPECIMEN_CONTAINER_NO").asLong());
			QuickSetText(4,index,str);
			QuickSetText(5,index,pParent->hiscmd.Field("SPECIMEN_ACCEPTOR").asString());
			if(pParent->hiscmd.Field("SPECIMEN_ACCEPT_DATE").isNull())
			{
				QuickSetText(6,index," ");
			}
			else
			{
				SADateTime st = pParent->hiscmd.Field("SPECIMEN_ACCEPT_DATE").asDateTime();
				str.Format("%d年%d月%d日 %d:%d", st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute());
				QuickSetText(6,index,str);
			}
			QuickSetText(7,index,pParent->hiscmd.Field("SPECIMEN_CODE").asString());

			index++;
		}
		
		pParent->hiscon.Commit();
	}
	catch(SAException &x)
	{
		try
		{
			pParent->hiscon.Rollback();
		}
		catch(SAException &)
		{
		}
		AfxMessageBox((const char*)x.ErrText());
		return;
	}

	SetSH_NumberCols(1);
	SetSH_Width(25);
	SetSH_ColWidth(0,25);

	SetTH_NumberRows(1);

	SetColWidth(0,50);  // 序号
	SetColWidth(1,120); // 标本标识
	SetColWidth(2,200); // 标本名称
	SetColWidth(3,80);  // 标本类型
	SetColWidth(4,50);  // 容器号
	SetColWidth(5,100); // 标本接收者
	SetColWidth(6,180); // 标本接收时间
	SetColWidth(7,150); // 标本说明
	
	SetHighlightRow(TRUE);
	
	SetVScrollMode( UG_SCROLLTRACKING );
	SetHScrollMode( UG_SCROLLTRACKING );
}

void CBbmxGrid::OnGetCell(int col,long row,CUGCell *cell)
{
	static char buf[_MAX_PATH];

	if( row == -1 )     //  Display table head information of pRecordSet_Define
	{
		if( col >= 0 )
		{
			switch ( col )
			{
			case 0:
				strcpy(buf,"序号");
				break;
			case 1:
				strcpy(buf,"标本标识");
				break;
			case 2:
				strcpy(buf,"标本名称");
				break;
			case 3:
				strcpy(buf,"标本类型");
				break;
			case 4:
				strcpy(buf,"容器号");
				break;
			case 5:
				strcpy(buf,"标本接收者");
				break;
			case 6:
				strcpy(buf,"标本接收时间");
				break;
			case 7:
				strcpy(buf,"标本说明");
				break;
			}
			cell->SetText(buf);
		}
		return;
	}
	if( col == -1 )
	{
		if( row >= 0 )
		{
			if( row == GetCurrentRow() )
				cell->SetCellType(UGCT_ARROW);
		}
		return;
	}

	return;
}

void CBbmxGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	if(row >= 0 && col == 7)
	{
		StartEdit();
	}
}

void CBbmxGrid::OnCharDown(UINT *vcKey,BOOL processed)
{
	if(GetCurrentRow() >= 0 && GetCurrentCol() == 7)
	{
		StartEdit(*vcKey);
	}
}

int CBbmxGrid::OnEditFinish(int col, long row,CWnd * edit, LPCTSTR string,BOOL cancelFlag)
{
	if(cancelFlag)
	{
		QuickSetText(col,row,string);
	}
	if(row >= 0 && col == 7)
	{
		CBbmxDetail *pParent = (CBbmxDetail *)GetParent();
		int xh = atoi(QuickGetText(0, row));

		try
		{
			pParent->hiscmd.setCommandText("Update EXAM.PATH_TEST_SPECIMEN set SPECIMEN_CODE = :SPECIMEN_CODE  where TEST_NO = :TEST_NO and SPECIMEN_NO = :SPECIMEN_NO");
			pParent->hiscmd.Param("TEST_NO").setAsString()       = pParent->m_Sqh;
			pParent->hiscmd.Param("SPECIMEN_NO").setAsShort()    = xh;
			pParent->hiscmd.Param("SPECIMEN_CODE").setAsString() = string;
			
			pParent->hiscmd.Execute();
			pParent->hiscon.Commit();
		}
		catch(SAException &x)
		{
			try
			{
				pParent->hiscon.Rollback();
			}
			catch(SAException &)
			{
			}
			AfxMessageBox((const char*)x.ErrText());
		}
	}
	
	return TRUE;
}
