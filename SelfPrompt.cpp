// SelfPrompt.cpp: implementation of the CSelfPrompt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "SelfPrompt.h"
#include "SelfInput.h"
#include "StrEdit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CSelfPrompt::CSelfPrompt()
{
	m_Type = 0;
}

CSelfPrompt::~CSelfPrompt()
{

}

void CSelfPrompt::OnSetup()
{
	m_font.CreateFont(16,0,0,0,500,0,0,0,0,0,0,0,0,"Arial");
	SetDefFont(&m_font);

    CSelfInput *ParentWnd = (CSelfInput *)GetParent();

	SetNumberRows(0);
	SetNumberCols(1);

	SetSH_Width(0);

	CRect rt;
	GetClientRect(&rt);
	SetColWidth(0,rt.Width());// - 20);

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

	if(m_Type == 1)
	{
		AddMenuItem(1004,"检索输入");
		EnableMenu(1);	
	}
}

void CSelfPrompt::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	if(updn == 0)
		return;

	if(!(col == 0 && row >= 0)) return;
	CSelfInput *ParentWnd = (CSelfInput *)GetParent();
	int index = ParentWnd->m_RecordInfo_Ctrl.GetCurrentRow();
	char ch = 0x00;
	if(ParentWnd->m_FieldList.GetAt(index).GetLength() > 0)
		ch = ParentWnd->m_FieldList.GetAt(index).GetAt(0);
	CString firstname;
	CUGCell  cell;
	GetCellIndirect(col , row, &cell);
	cell.GetText(&firstname);

	CString ftitle = ParentWnd->m_NameList.GetAt(index);
	ftitle.MakeUpper();
	bool isCodeGen = false;
	if( ftitle.Find("MICD10") >=0 || 
		ftitle.Find("SICD10") >=0 || 
		ftitle.CompareNoCase("sjks") == 0 || 
		ftitle.CompareNoCase("sjys") == 0)
	{
		if(firstname.Find('[') >= 0)
		{
			firstname = firstname.Left(firstname.Find('['));
			isCodeGen = true;
		}
	}

	CString oldstr;

	switch( m_Type ) {
	case 1:
		if(ch == '{')  break;
		
		if( !isCodeGen )
		{
			if(firstname.Find('-') >= 0)
			{
				firstname = firstname.Right(firstname.GetLength() - firstname.Find('-') - 1);
			}
		}

		if(theApp.m_EditingMode.CompareNoCase("插入") == 0)
		{
			ParentWnd->m_RecordInfo_Ctrl.GetCellIndirect(0 , index, &cell);
			cell.GetText(&oldstr);
			oldstr.TrimLeft(); oldstr.TrimRight();
			if(!oldstr.IsEmpty())   oldstr += "、";
			oldstr += firstname;
		}
		else
		{
			oldstr = firstname;
		}


		ParentWnd->m_RecordInfo_Ctrl.QuickSetText(0 , index , oldstr);
		ParentWnd->m_RecordInfo_Ctrl.RedrawRow(index);
		break;
	case 2:
		if(ch != '{')  break;

		if(theApp.m_EditingMode.CompareNoCase("插入") == 0)
		{
			ParentWnd->m_RecordInfo_Ctrl.GetCellIndirect(0 , index, &cell);
			cell.GetText(&oldstr);
			oldstr.TrimLeft(); oldstr.TrimRight();
			if(!oldstr.IsEmpty())   oldstr += "、";
			oldstr += firstname;
		}
		else
		{
			oldstr = firstname;
		}

		ParentWnd->m_RecordInfo_Ctrl.QuickSetText(0 , index , oldstr);
		ParentWnd->m_RecordInfo_Ctrl.RedrawRow(index);
		break;
	default:;
	}
}

void CSelfPrompt::OnRowChange(long oldrow,long newrow)
{
	if(m_Type == 1 && oldrow >= 0)
	{
		CSelfInput *ParentWnd = (CSelfInput *)GetParent();
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

void CSelfPrompt::OnMenuCommand(int col,long row,int section,int item)
{
	CSelfInput *ParentWnd = (CSelfInput *)GetParent();
	int index = ParentWnd->m_RecordInfo_Ctrl.GetCurrentRow();

	if(item == 1004)
	{
		CString ftitle = ParentWnd->m_NameList.GetAt(index);
		ftitle.MakeUpper();
		if(!(ftitle.Find("MICD10") >=0 || ftitle.Find("SICD10") >=0))  return;
		StrEdit mydlg("请输入检索关键词",this);
		if(mydlg.DoModal() == IDCANCEL)   return;
		mydlg.m_EditStr.TrimLeft();   mydlg.m_EditStr.TrimRight();
		CString m_like;
		mydlg.m_EditStr.MakeUpper();
		m_like.Format("%%%s%%",mydlg.m_EditStr);
		
		try
		{
			BYTE ch;
			for(int i = 0; i < mydlg.m_EditStr.GetLength(); i++)
			{
				ch = mydlg.m_EditStr.GetAt(i);
				if((ch&0x80) != 0 )   break;
			}
			SAString m_WhereBetween_Cmd, FilterField;
			if( i < mydlg.m_EditStr.GetLength() )
			{
				FilterField.Format("CDISE");
			}
			else
			{
				FilterField.Format("PINYIN");
			}
			m_WhereBetween_Cmd.Format(" where %s like :%s",FilterField,FilterField);
			
			
			SAString cmdstr;
			if(ftitle.Find("MICD10") >=0)
				cmdstr = "select count(*) from " + (SAString)theApp.TABLE_ICD10 + m_WhereBetween_Cmd + " and MICD10 <> '' and MICD10 <> '—'  and MICD10 <> '---' ";
			else
				cmdstr = "select count(*) from " + (SAString)theApp.TABLE_ICD10 + m_WhereBetween_Cmd + " and SICD10 <> '' and SICD10 <> '—'  and SICD10 <> '---' ";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param(( SAString )FilterField).setAsString() = m_like;
			g_dbcommand.Execute();
			g_dbcommand.FetchNext();
			int numrow = g_dbcommand.Field(1).asLong();
			SetNumberRows(numrow);
			
			if(ftitle.Find("MICD10") >=0)
				cmdstr = "select MICD10, CDISE from " + (SAString)theApp.TABLE_ICD10 + m_WhereBetween_Cmd + " and MICD10 <> '' and MICD10 <> '—'  and MICD10 <> '---' ";
			else
				cmdstr = "select SICD10, CDISE from " + (SAString)theApp.TABLE_ICD10 + m_WhereBetween_Cmd + " and SICD10 <> '' and SICD10 <> '—'  and SICD10 <> '---' ";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param(( SAString )FilterField).setAsString() = m_like;
			g_dbcommand.Execute();
			
			i = 0;
			while( g_dbcommand.FetchNext() )
			{
				ftitle.Format("%s [%s]",g_dbcommand.Field(1).asString(), g_dbcommand.Field(2).asString());
				QuickSetText(0 , i , ftitle);
				i++;
			}
			if( i > 0 ) 	GotoRow(0);
			RedrawAll();
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
