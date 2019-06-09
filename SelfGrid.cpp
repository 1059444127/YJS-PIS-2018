// SelfGrid.cpp: implementation of the CSelfGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "SelfGrid.h"
#include "SelfInput.h"
#include "TextOut.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CSelfGrid::CSelfGrid()
{

}
 
CSelfGrid::~CSelfGrid()
{

}

void CSelfGrid::OnSetup()
{
	m_font.CreateFont(16,0,0,0,500,0,0,0,0,0,0,0,0,"Arial");
	SetDefFont(&m_font);

    CSelfInput *ParentWnd = (CSelfInput *)GetParent();

	SetNumberRows(ParentWnd->m_FieldName.GetSize());
	SetNumberCols(1);

	SetSH_Width(100);
	SetSH_NumberCols(1);

	CRect rt;
	GetClientRect(&rt);
	SetColWidth(0,rt.Width() - 100);

	SetVScrollMode( UG_SCROLLTRACKING );
	SetHScrollMode( UG_SCROLLTRACKING );

	SetHighlightRow(TRUE);

	SetDoubleBufferMode(TRUE);

	SetTH_Height(0);
/*
	QuickSetText(-1,-1, "域名");
	QuickSetAlignment(-1,-1,UG_ALIGNCENTER);
	QuickSetText(0, -1, "域值");
	QuickSetAlignment(0,-1,UG_ALIGNCENTER);
*/

	for(int i = 0; i < ParentWnd->m_FieldName.GetSize(); i ++ )
	{
		QuickSetText(-1 , i , ParentWnd->m_FieldName.GetAt(i));
		QuickSetAlignment(-1 , i, UG_ALIGNRIGHT);
	}

	for(i = 0; i < ParentWnd->m_FieldValue.GetSize(); i ++ )
	{
		QuickSetText(0 , i , ParentWnd->m_FieldValue.GetAt(i));
		QuickSetAlignment(0 , i, UG_ALIGNLEFT);
	}

	GotoRow(0);

	AddMenuItem(1008,"文本显示");
	EnableMenu(1);	
}

void CSelfGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	if(col == 0 && row >= 0)
	{
		CSelfInput *ParentWnd = (CSelfInput *)GetParent();
		if(!( ParentWnd->m_FieldValue.GetAt(row).Compare("是") == 0 || ParentWnd->m_FieldValue.GetAt(row).Compare("否") == 0))
		{
			StartEdit();
		}
		else
		{
			CString name;
			CUGCell  cell;
			GetCellIndirect(col , row, &cell);
			cell.GetText(&name);
			if(name.Compare("是") == 0)
			{
				QuickSetText(0 , row , "否");
			}
			else
			{
				QuickSetText(0 , row , "是");
			}
		    RedrawRow(row);
		}
	}
}

void CSelfGrid::OnCharDown(UINT *vcKey,BOOL processed)
{
	if(*vcKey == VK_TAB)   return;

	if(GetCurrentCol() == 0 && GetCurrentRow() >= 0)
	{
		CSelfInput *ParentWnd = (CSelfInput *)GetParent();
		if(!( ParentWnd->m_FieldValue.GetAt(GetCurrentRow()).Compare("是") == 0 || 
			  ParentWnd->m_FieldValue.GetAt(GetCurrentRow()).Compare("否") == 0) )
		{
			StartEdit(*vcKey);
		}
		else
		{
			CString name;
			CUGCell  cell;
			GetCellIndirect(GetCurrentCol() , GetCurrentRow(), &cell);
			cell.GetText(&name);
			if(name.Compare("是") == 0)
			{
				QuickSetText(0 , GetCurrentRow() , "否");
			}
			else
			{
				QuickSetText(0 , GetCurrentRow() , "是");
			}
		    RedrawRow(GetCurrentRow());
		}
	}
}

void CSelfGrid::OnRowChange(long oldrow,long newrow)
{
	if( oldrow < 0 )  return;
    CSelfInput *ParentWnd = (CSelfInput *)GetParent();
	//-------------------------------------------
	if(oldrow == 0 && newrow > oldrow)
	{
		CString mno = QuickGetText(0, 0);
		int num = 0;
		try
		{
			SAString cmdstr = "Select count(*) from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("medicalno").setAsString() = mno;
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
			ParentWnd->SetWindowText("快速信息录入（此病理号已经存在，请核对后再编辑！）");
		}
		else
		{
			ParentWnd->SetWindowText("快速信息录入");
		}
	}
	
	//-------------------------------------------
	CStringArray  firstlist, secondlist;
	if(ParentWnd->m_FieldList.GetAt(newrow).GetLength() > 0)
	{
		char ch = ParentWnd->m_FieldList.GetAt(newrow).GetAt(0);
		if(ch == '{')
		{
			CString str = ParentWnd->m_FieldList.GetAt(newrow);
			str = str.Right(str.GetLength() - 1);
			while(1)
			{
				CString tmp = str.Left(str.Find('}'));
				firstlist.Add(tmp);
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
			CString firstname;
			if(ParentWnd->m_NameList.GetAt(newrow).CompareNoCase("sjys") == 0){
				int i = 0;
				while(1){
					if(ParentWnd->m_NameList.GetAt(i).CompareNoCase("sjks") == 0){
						break;
					}else{
						i++;
					}
				}
				CString str;
				this->QuickGetText(0,i,&str);
				firstname = "{" + str + "}";
				if(firstname.CompareNoCase("{}") == 0){
					str = ParentWnd->m_FieldList.GetAt(newrow);
					str = str.Right(str.GetLength() - 1);
					firstname = str.Left(str.Find('}'));
			     	firstname = "{" + firstname + "}";
				}
			}else if(ParentWnd->m_NameList.GetAt(newrow).CompareNoCase("lczd") == 0)
			{
				int i = 0;
				while(1){
					if(ParentWnd->m_NameList.GetAt(i).CompareNoCase("lczd") == 0){
						break;
					}else{
						i++;
					}
				}

				CString str;
				this->QuickGetText(0,i,&str);
				if(str.CompareNoCase("") == 0){
					str = ParentWnd->m_FieldList.GetAt(newrow);
					str = str.Right(str.GetLength() - 1);
					firstname = str.Left(str.Find('}'));
			     	firstname = "{" + firstname + "}";
				} else {
				CString str1;
				str1 = ParentWnd->m_FieldList.GetAt(i);
				str1 = str1.Left(str1.Find(str));
				while(1){
					int j = str1.Find('{');
					if( j != -1){
						str1 = str1.Right(str1.GetLength() - j -1);
					}else{
						break;
					}
				}	
	
				str1 = str1.Left(str1.Find('}'));
				firstname = "{" + str1 + "}";
				}

			}else{
				str = ParentWnd->m_FieldList.GetAt(newrow);
				str = str.Right(str.GetLength() - 1);
				firstname = str.Left(str.Find('}'));
				firstname = "{" + firstname + "}";
			}

			
			
		//	str = ParentWnd->m_FieldList.GetAt(newrow);
		//	str = str.Right(str.GetLength() - 1);
		//	CString firstname = str.Left(str.Find('}'));
		//	firstname = "{" + firstname + "}";
			str = ParentWnd->m_FieldList.GetAt(newrow);
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
					CString tmp = str.Left(str.Find('&'));
					secondlist.Add(tmp);
					str = str.Right(str.GetLength() - str.Find('&') - 1);
				}
			}
		}
		else
		{
			CString str = ParentWnd->m_FieldList.GetAt(newrow);
			int i = 1;
			CString  ia;
			while(1)
			{
				if(str.Find('&') == -1)  break;
				CString tmp = str.Left(str.Find('&'));
				ia.Format("%2d-", i);
				i++;
				firstlist.Add(ia + tmp);
				str = str.Right(str.GetLength() - str.Find('&') - 1);
			}
		}
	}

	ParentWnd->m_PromptInfo_Ctrl_First.SetNumberRows(firstlist.GetSize());
	for(int i = 0; i < firstlist.GetSize(); i ++ )
	{
		ParentWnd->m_PromptInfo_Ctrl_First.QuickSetText(0 , i , firstlist.GetAt(i));
		ParentWnd->m_PromptInfo_Ctrl_First.QuickSetAlignment(0 , i, UG_ALIGNLEFT);
	}
	if( firstlist.GetSize() > 0 )
		ParentWnd->m_PromptInfo_Ctrl_First.GotoRow(0);

	ParentWnd->m_PromptInfo_Ctrl_Second.SetNumberRows(secondlist.GetSize());
	for(i = 0; i < secondlist.GetSize(); i ++ )
	{
		ParentWnd->m_PromptInfo_Ctrl_Second.QuickSetText(0 , i , secondlist.GetAt(i));
		ParentWnd->m_PromptInfo_Ctrl_Second.QuickSetAlignment(0 , i, UG_ALIGNLEFT);
	}
	if( secondlist.GetSize() > 0 )
		ParentWnd->m_PromptInfo_Ctrl_Second.GotoRow(0);

	firstlist.RemoveAll();
	secondlist.RemoveAll();

	ParentWnd->m_PromptInfo_Ctrl_First.RedrawAll();
	ParentWnd->m_PromptInfo_Ctrl_Second.RedrawAll();

	//------------------------------------------------
	if(oldrow >= 0 && oldrow < GetNumberRows())
	{
		CString ftitle = ParentWnd->m_NameList.GetAt(oldrow);
		ftitle.MakeUpper();
		CString FieldCodeTable;  FieldCodeTable.Empty();
		if(ftitle.CompareNoCase("sjks") == 0)   FieldCodeTable = ftitle;
		if(ftitle.CompareNoCase("sjys") == 0)   FieldCodeTable = ftitle;
		if(ParentWnd->m_FieldList.GetAt(oldrow).GetLength() > 0)  FieldCodeTable.Empty();
		CString str = QuickGetText(0, oldrow);
		str.TrimLeft(); str.TrimRight();
		if(!FieldCodeTable.IsEmpty() && !str.IsEmpty())
		{
			CString m_like;
			m_like.Format("%s%%",str);
			try
			{
				SAString cmdstr = "select id, name from " + FieldCodeTable + " where id like :id";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("id").setAsString() = m_like;
				g_dbcommand.Execute();
				
				if( g_dbcommand.FetchNext() )
				{
					str = g_dbcommand.Field(2).asString();
					QuickSetText(0,oldrow,g_dbcommand.Field(2).asString());
					RedrawCell(0,oldrow);
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
		}
	}
	//------------------------------------------------
    
	SetFocus();

	if(newrow >= 0 && !( ParentWnd->m_FieldValue.GetAt(newrow).Compare("是") == 0 || ParentWnd->m_FieldValue.GetAt(newrow).Compare("否") == 0))
	{
		StartEdit();
	}
}

void CSelfGrid::OnSetCell(int col,long row,CUGCell *cell)
{
    CSelfInput *ParentWnd = (CSelfInput *)GetParent();
	if( col >= 0 )  
	{
		if(ParentWnd->m_FieldList.GetAt(row).GetLength() > 0 )  // 存在提示
		{
			char ch = ParentWnd->m_FieldList.GetAt(row).GetAt(0);
			if(ch != '{')                                        // 并且只有一级提示
			{
				CString str;
				cell->GetText(&str);
				str.TrimLeft();  str.TrimRight();
				if(str.GetLength() > 0 && str.GetLength() < 10)
				{
					for(int i = 0; i < str.GetLength(); i++ )
					{
						BYTE bch = str.GetAt(i);
						if( bch < 0x30 || bch > 0x39 )   break;
					}
					if( i >= str.GetLength() )
					{
						i = atoi(str);
						i--;
						if( i >= 0 && i < ParentWnd->m_PromptInfo_Ctrl_First.GetNumberRows() )
						{
							CUGCell  Localcell;
							ParentWnd->m_PromptInfo_Ctrl_First.GetCellIndirect(0 , i, &Localcell);
							Localcell.GetText(&str);
							//--------------------------------------
							CString ftitle = ParentWnd->m_NameList.GetAt(row);
							ftitle.MakeUpper();
							bool isICD10 = false;
							if( ftitle.Find("MICD10") >=0 || ftitle.Find("SICD10") >=0 )
							{
								isICD10 = true;
							}
							
							if( !isICD10 )
							{
								if(str.Find('-') >= 0)
								{
									str = str.Right(str.GetLength() - str.Find('-') - 1);
								}
							}
							//--------------------------------------
							QuickSetText(col,row,str);
						}
					}
				}
			}
		}
		else
		{
			CString str;
			cell->GetText(&str);
			str.TrimLeft();  str.TrimRight();
			if(str.GetLength() > 0)
			{
				SAString indexname;
				indexname.Empty();
				
				try
				{
					SAString cmdstr = "Select name from " + ParentWnd->m_NameList.GetAt(row) + " where id = :id";
					
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("id").setAsString() = str;
					
					g_dbcommand.Execute();
					
					if( g_dbcommand.FetchNext() )
					{
						indexname = g_dbcommand.Field("name").asString();
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
				
				if(!indexname.IsEmpty())
				{
					QuickSetText(col,row,indexname);
				}
			}
		}
	}
}

int CSelfGrid::OnEditFinish(int col, long row,CWnd * edit, LPCTSTR string,BOOL cancelFlag)
{
	if(cancelFlag)
	{
		QuickSetText(col,row,string);
	}

	return TRUE;
}

void CSelfGrid::OnKeyDown(UINT *vcKey,BOOL processed)
{
	if(*vcKey == VK_TAB)
	{
		int cutrow = GetCurrentRow();
		int numrow = GetNumberRows();
		if( numrow && cutrow < (numrow - 1) )
		{
			GotoRow(cutrow + 1);
		}
	}
}

int CSelfGrid::OnEditVerify(int col,long row,CWnd *edit,UINT *vcKey)
{
	if(*vcKey == VK_TAB)
	{
		UINT i = VK_TAB;
		OnKeyDown(&i,TRUE);
	}

	if(col == 0 && row >= 0 && row < GetNumberRows())
	{
		CSelfInput *ParentWnd = (CSelfInput *)GetParent();
		CString ftitle = ParentWnd->m_NameList.GetAt(row);
		ftitle.MakeUpper();
		CString FieldCodeTable;  FieldCodeTable.Empty();
		if(ftitle.CompareNoCase("sjks") == 0)   FieldCodeTable = ftitle;
		if(ftitle.CompareNoCase("sjys") == 0)   FieldCodeTable = ftitle;
		if(ParentWnd->m_FieldList.GetAt(row).GetLength() > 0)  FieldCodeTable.Empty();
		if(!FieldCodeTable.IsEmpty())
		{
			CString m_EditStr;
			m_editCtrl->GetWindowText(m_EditStr);
			m_EditStr.TrimLeft();   m_EditStr.TrimRight();
			if(*vcKey == VK_BACK)
			{
				m_EditStr = m_EditStr.Left(m_EditStr.GetLength() - 1);
			}
			else
			{
				CString tmp;
				tmp.Format("%c", *vcKey);
				m_EditStr += tmp;
			}
			CString m_like;
			//m_like.Format("%%%s%%",m_EditStr);
			m_like.Format("%s%%",m_EditStr);
			
			try
			{
				SAString cmdstr = "select count(*) from " + FieldCodeTable + " where id like :id";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("id").setAsString() = m_like;
				g_dbcommand.Execute();
				g_dbcommand.FetchNext();
				int numrow = g_dbcommand.Field(1).asLong();
				ParentWnd->m_PromptInfo_Ctrl_First.SetNumberRows(numrow);
				
				cmdstr = "select id, name from " + FieldCodeTable + " where id like :id";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("id").setAsString() = m_like;
				g_dbcommand.Execute();
				
				int i = 0;
				while( g_dbcommand.FetchNext() )
				{
					ftitle.Format("%s [%s]",g_dbcommand.Field(2).asString(), g_dbcommand.Field(1).asString());
					ParentWnd->m_PromptInfo_Ctrl_First.QuickSetText(0 , i , ftitle);
					i++;
				}
				if( i > 0 ) 	ParentWnd->m_PromptInfo_Ctrl_First.GotoRow(0);
				ParentWnd->m_PromptInfo_Ctrl_First.RedrawAll();
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
	}

	return TRUE;
}

void CSelfGrid::OnMenuCommand(int col,long row,int section,int item)
{
    if(col < 0 || col >= GetNumberCols()) return;

	if( item == 1008 )               //  1008 - 文本编辑窗
	{
        if(col < 0 || col >= GetNumberCols()) return;
        if(row < 0 || row >= GetNumberRows()) return;

		CSelfInput *ParentWnd = (CSelfInput *)GetParent();

		CTextOut mydlg(this);
		CUGCell  cell;
		GetCellIndirect(col , row, &cell);
		cell.GetText(&mydlg.m_Text);
		mydlg.m_Title.Format("%s",ParentWnd->m_FieldName.GetAt(row));
		mydlg.DoModal();
		return;
	}
}
