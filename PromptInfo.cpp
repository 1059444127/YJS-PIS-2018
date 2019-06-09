// PromptInfo.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "PromptInfo.h"

#include "RecordInput.h"
#include "StrEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPromptInfo
extern CPathologyApp theApp;

CPromptInfo::CPromptInfo()
{
	m_Type = 0;
}

CPromptInfo::~CPromptInfo()
{

}

void CPromptInfo::OnSetup()
{
	m_font.CreateFont(16,0,0,0,500,0,0,0,0,0,0,0,0,"Arial");
	SetDefFont(&m_font);

    CRecordInput *ParentWnd = (CRecordInput *)GetParent();

	SetNumberRows(0);
	SetNumberCols(1);

	SetSH_Width(0);

	CRect rt;
	GetClientRect(&rt);
	SetColWidth(0,rt.Width() - 20);

	SetVScrollMode( UG_SCROLLTRACKING );
	SetHScrollMode( UG_SCROLLTRACKING );

	SetHighlightRow(TRUE);

	SetDoubleBufferMode(TRUE);

	switch(m_Type) {
	case 1:
		QuickSetText(0, -1, "一级提示");
		break;
	case 2:
		QuickSetText(0, -1, "二级提示");
		break;
	default:;
	}
	QuickSetAlignment(0,-1,UG_ALIGNCENTER);

	CString firstname;
	CUGCell  cell;
	CStringArray strlist;
	CString str,tmp;
	int i, index = ParentWnd->m_RecordInfo_Ctrl.GetCurrentRow();
	if(ParentWnd->m_FieldList.GetAt(index).GetLength() > 0)
	{
		char ch = ParentWnd->m_FieldList.GetAt(index).GetAt(0);
		switch(m_Type) {
		case 1:
			if(ch == '{')
			{
				str = ParentWnd->m_FieldList.GetAt(index);
				str = str.Right(str.GetLength() - 1);
				while(1)
				{
					tmp = str.Left(str.Find('}'));
					strlist.Add(tmp);
					str = str.Right(str.GetLength() - str.Find('}') - 1);
					if( str.Find('{') == -1)
					{
						break;
					}
					else
					{
						str = str.Right(str.GetLength() - str.Find('{') - 1);
					}
				}
			}
			else
			{
				str = ParentWnd->m_FieldList.GetAt(index);
				while(1)
				{
					if(str.Find('&') == -1)  break;
					tmp = str.Left(str.Find('&'));
					strlist.Add(tmp);
					str = str.Right(str.GetLength() - str.Find('&') - 1);
				}
			}
			
			break;
		case 2:
			if(ch != '{')  break;
			ParentWnd->m_PromptInfo_Ctrl_First.GetCellIndirect(0 , ParentWnd->m_PromptInfo_Ctrl_First.GetCurrentRow(), &cell);
			cell.GetText(&firstname);
			firstname = "{" + firstname + "}";

			str = ParentWnd->m_FieldList.GetAt(index);
			if( str.Find(firstname) == -1 )  break;
            str = str.Right(str.GetLength() - str.Find(firstname));
            str = str.Right(str.GetLength() - str.Find('}') - 1);

			if( str.Find('{') != -1)
			{
				str = str.Left(str.Find('{'));
			}

			while(1)
			{
				if(str.Find('&') == -1)  break;
				tmp = str.Left(str.Find('&'));
				strlist.Add(tmp);
				str = str.Right(str.GetLength() - str.Find('&') - 1);
			}

			break;
		default:;
		}
	}

	SetNumberRows(strlist.GetSize());
	for(i = 0; i < strlist.GetSize(); i ++ )
	{
		QuickSetText(0 , i , strlist.GetAt(i));
		QuickSetAlignment(0 , i, UG_ALIGNLEFT);
	}
	if( strlist.GetSize() > 0 )
		GotoRow(0);
	strlist.RemoveAll();

	AddMenuItem(1000,"删除");
	AddMenuItem(1001,"添加(在下方)");
	AddMenuItem(1002,"添加(在上方)");

	EnableMenu(1);	
}

/***************************************************
OnRowChange
	Sent whenever the current row changes
	The old and the new rows are given
****************************************************/
void CPromptInfo::OnRowChange(long oldrow,long newrow)
{
	if(m_Type == 1 && oldrow >= 0)
	{
		CRecordInput *ParentWnd = (CRecordInput *)GetParent();
		int index = ParentWnd->m_RecordInfo_Ctrl.GetCurrentRow();
		CString firstname,tmp;
		CUGCell  cell;
		CStringArray strlist;

		if(ParentWnd->m_FieldList.GetAt(index).GetLength() <= 0)  return;
		
		char ch = ParentWnd->m_FieldList.GetAt(index).GetAt(0);
		if(ch != '{')  return;
		GetCellIndirect(0 , newrow, &cell);
		cell.GetText(&firstname);
		firstname = "{" + firstname + "}";
		
		CString str = ParentWnd->m_FieldList.GetAt(index);
		if( str.Find(firstname) != -1 )
		{
			str = str.Right(str.GetLength() - str.Find(firstname));
			str = str.Right(str.GetLength() - str.Find('}') - 1);
			
			if( str.Find('{') != -1)
			{
				str = str.Left(str.Find('{'));
			}
			
			while(1)
			{
				if(str.Find('&') == -1)  break;
				tmp = str.Left(str.Find('&'));
				strlist.Add(tmp);
				str = str.Right(str.GetLength() - str.Find('&') - 1);
			}
		}
		
		ParentWnd->m_PromptInfo_Ctrl_Second.SetNumberRows(strlist.GetSize());
		for(int i = 0; i < strlist.GetSize(); i ++ )
		{
			ParentWnd->m_PromptInfo_Ctrl_Second.QuickSetText(0 , i , strlist.GetAt(i));
			ParentWnd->m_PromptInfo_Ctrl_Second.QuickSetAlignment(0 , i, UG_ALIGNLEFT);
		}
		if( strlist.GetSize() > 0 )
			ParentWnd->m_PromptInfo_Ctrl_Second.GotoRow(0);
		strlist.RemoveAll();
		ParentWnd->m_PromptInfo_Ctrl_Second.RedrawAll();
	}		
}

void CPromptInfo::OnMenuCommand(int col,long row,int section,int item)
{
    if(! ( ( row >= 0 && row < GetNumberRows() ) ||
           ( GetNumberRows() == 0 && (item == 1001 || item == 1002) ) ) )
	{
		return;
	}

	CRecordInput *ParentWnd = (CRecordInput *)GetParent();
	int index = ParentWnd->m_RecordInfo_Ctrl.GetCurrentRow();
	char ch = 0x00;
	if( ParentWnd->m_FieldList.GetAt(index).GetLength() > 0 )
		ch = ParentWnd->m_FieldList.GetAt(index).GetAt(0);

	CString gridname, localfirst,InputStr;

	CUGCell  cell;
    if(!( row < 0 || row >= GetNumberRows() ))
	{
		GetCellIndirect(0 , row, &cell);
		cell.GetText(&gridname);
	}

	int InsertPos;
	CString str = ParentWnd->m_FieldList.GetAt(index);

	bool IsChanged = false;

	switch(item){
		case 1000:{ //删除
			switch(m_Type) {
			case 1:
				if(ch == '{')
				{
					if( str.Find("{" + gridname + "}") != -1 )
					{
						CString str1 = str.Left( str.Find("{" + gridname + "}") );
						CString str2 = str.Right(str.GetLength() - str.Find("{" + gridname + "}") - 1);
						
						if( str2.Find('{') != -1)
						{
							str2 = str2.Right(str2.GetLength() - str2.Find('{'));
						}
						else
						{
							str2.Empty();
						}
                        ParentWnd->m_FieldList.SetAt( index, str1 + str2 );
						IsChanged = true;
					}
				}
				else
				{
					//----------------------------------------------
					if(gridname.Find('-') >= 0)
					{
						gridname = gridname.Right(gridname.GetLength() - gridname.Find('-') - 1);
					}
					//----------------------------------------------
					if( str.Find( "&" + gridname + "&" ) != -1 )
					{
						CString str1 = str.Left( str.Find( "&" + gridname + "&" ) + 1 );
						CString str2 = str.Right(str.GetLength() - str.Find( "&" + gridname + "&" ) - (gridname.GetLength() + 2) );
                        ParentWnd->m_FieldList.SetAt( index, str1 + str2 );
						IsChanged = true;
					}
					else
					{
						if( str.Find( gridname + "&" ) != -1 )
						{
							CString str1 = str.Left( str.Find( gridname + "&" ) );
							CString str2 = str.Right(str.GetLength() - str.Find( gridname + "&" ) - (gridname.GetLength() + 1) );
							ParentWnd->m_FieldList.SetAt( index, str1 + str2 );
							IsChanged = true;
						}
					}
				}
				break;
			case 2:
				if(ch != '{')   break;
				ParentWnd->m_PromptInfo_Ctrl_First.GetCellIndirect(0 , ParentWnd->m_PromptInfo_Ctrl_First.GetCurrentRow(), &cell);
				cell.GetText(&localfirst);

				if( str.Find("{" + localfirst + "}") != -1 )
				{
					CString str1 = str.Left( str.Find("{" + localfirst + "}") );
					CString str2 = str.Right(str.GetLength() - str.Find("{" + localfirst + "}") - 1);
					CString str3;
					
					if( str2.Find('{') != -1)
					{
						str3 = "{" + str2.Left( str2.Find('{') );
						str2 = str2.Right(str2.GetLength() - str2.Find('{'));
					}
					else
					{
						str3 = "{" + str2;
						str2.Empty();
					}
					
					if( str3.Find( "&" + gridname + "&" ) != -1 )
					{
						CString substr1 = str3.Left( str3.Find( "&" + gridname + "&" ) + 1 );
						CString substr2 = str3.Right(str3.GetLength() - str3.Find( "&" + gridname + "&" ) - (gridname.GetLength() + 2) );
						str3 = substr1 + substr2;
					}
					else
					{
						if( str3.Find( gridname + "&" ) != -1 )
						{
							CString substr1 = str3.Left( str3.Find( gridname + "&" ) );
							CString substr2 = str3.Right(str3.GetLength() - str3.Find( gridname + "&" ) - (gridname.GetLength() + 1) );
							str3 = substr1 + substr2;
						}
					}

					ParentWnd->m_FieldList.SetAt( index, str1 + str3 + str2 );
					IsChanged = true;
				}
				break;
			}
			break;
		}
		case 1001:   //添加(在下方)
		case 1002:   //添加(在上方)
		{ 
 			if(m_Type == 2 && ch != '{')   break;

            StrEdit mydlg("请输入新的提示内容",this);
			if(mydlg.DoModal() == IDCANCEL)   break;
			InputStr = mydlg.m_EditStr;
			
			if( GetNumberRows() == 0 )
			{
				InsertPos = 0;
			}
			else
			{
				InsertPos = row + ( 1002 - item );
			}
			switch(m_Type) {
			case 1:
				if( str.GetLength() == 0 )
				{
					if(AfxMessageBox("该栏目是否有二级提示？",MB_OKCANCEL|MB_ICONQUESTION) == IDOK)
						ch = '{';
				}
				if(ch == '{')
				{
					CString lstr;
					for(int i = 0; i < InsertPos; i++ )
					{
						lstr += str.Left(1);
						str   = str.Right( str.GetLength() - 1 );
						if( str.Find('{') != -1)
						{
							lstr += str.Left( str.Find('{') );
							str   = str.Right( str.GetLength() - str.Find('{') );
						}
						else
						{
							lstr += str;
							str.Empty();
						}
					}
					ParentWnd->m_FieldList.SetAt( index, lstr + "{" + InputStr + "}" + str );
					IsChanged = true;
				}
				else
				{
					CString lstr;
					for(int i = 0; i < InsertPos; i++ )
					{
						if( str.IsEmpty() )  break;
						if( str.Find('&') != -1)
						{
							lstr += str.Left( str.Find('&') + 1);
							str   = str.Right( str.GetLength() - str.Find('&') - 1 );
						}
					}
					ParentWnd->m_FieldList.SetAt( index, lstr + InputStr + "&" + str );
					IsChanged = true;
				}
				break;
			case 2:
				if(ch != '{')   break;
				ParentWnd->m_PromptInfo_Ctrl_First.GetCellIndirect(0 , ParentWnd->m_PromptInfo_Ctrl_First.GetCurrentRow(), &cell);
				cell.GetText(&localfirst);

				if( str.Find("{" + localfirst + "}") != -1 )
				{
					CString str1 = str.Left( str.Find("{" + localfirst + "}") + (localfirst.GetLength() + 2) );
					CString str2 = str.Right(str.GetLength() - str.Find("{" + localfirst + "}") - (localfirst.GetLength() + 2));
					CString str3;
					
					if( str2.Find('{') != -1)
					{
						str3 = str2.Left( str2.Find('{') );
						str2 = str2.Right(str2.GetLength() - str2.Find('{'));
					}
					else
					{
						str3 = str2;
						str2.Empty();
					}
					
					CString lstr;
					for(int i = 0; i < InsertPos; i++ )
					{
						if( str3.IsEmpty() )  break;
						if( str3.Find('&') != -1)
						{
							lstr += str3.Left( str3.Find('&') + 1);
							str3  = str3.Right( str3.GetLength() - str3.Find('&') - 1 );
						}
					}
					str3 = lstr + InputStr + "&" + str3;

					ParentWnd->m_FieldList.SetAt( index, str1 + str3 + str2 );
					IsChanged = true;
				}
				break;
			}
			break;
		}
	}

	if( !IsChanged )   return;

	theApp.ConnectDatabase();
	try
	{
		bool IsFieldDefined = false;
		SAString cmdstr = "Select count(*) from " + theApp.TABLE_DEFINE + "  where FieldName = :FieldName";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("FieldName").setAsString() = ParentWnd->m_NameList.GetAt(index);
		g_dbcommand.Execute();
		if(g_dbcommand.FetchNext())
		{
			int count = g_dbcommand.Field(1).asLong();
			if( count > 0 )
			{
				IsFieldDefined = true;
			}
		}
		
		if( IsFieldDefined )
		{
			try
			{
				SAString cmdstr = "Update " + theApp.TABLE_DEFINE + " set FieldInfo = :FieldInfo where FieldName = :FieldName";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("FieldInfo").setAsString() = ParentWnd->m_FieldList.GetAt(index);
				g_dbcommand.Param("FieldName").setAsString() = ParentWnd->m_NameList.GetAt(index);
				g_dbcommand.Execute();
				g_dbconnection.Commit();
				ParentWnd->m_RecordInfo_Ctrl.OnRowChange(index,index);
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
		else
		{
			CString msg;
			msg.Format("<%s>在字段辅助说明库中没有定义！", ParentWnd->m_NameList.GetAt(index));
			AfxMessageBox(msg);
		}
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
