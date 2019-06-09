// RecordInfo.cpp: implementation of the CRecordInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "RecordInfo.h"

#include "KSFileDialog.h"
#include "RecordInput.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CRecordInfo::CRecordInfo()
{

}

CRecordInfo::~CRecordInfo()
{

}

void CRecordInfo::OnSetup()
{
	m_font.CreateFont(16,0,0,0,500,0,0,0,0,0,0,0,0,"Arial");
	SetDefFont(&m_font);

    CRecordInput *ParentWnd = (CRecordInput *)GetParent();

	SetNumberRows(ParentWnd->m_FieldName.GetSize());
	SetNumberCols(1);

	SetSH_Width(0);
	SetSH_NumberCols(0);

	CRect rt;
	GetClientRect(&rt);
	SetColWidth(0,rt.Width() - 20);

	SetVScrollMode( UG_SCROLLTRACKING );
	SetHScrollMode( UG_SCROLLTRACKING );

	SetHighlightRow(TRUE);

	SetDoubleBufferMode(TRUE);

	SetTH_Height(0);

	for(int i = 0; i < ParentWnd->m_FieldName.GetSize(); i ++ )
	{
		QuickSetText(0 , i , ParentWnd->m_FieldName.GetAt(i));
		QuickSetAlignment(0 , i, UG_ALIGNRIGHT);
	}

	GotoRow(0);

	AddMenuItem(1000,"字段提示导入");
	AddMenuItem(1001,"字段提示导出");
	
	EnableMenu(1);	
}

void CRecordInfo::OnRowChange(long oldrow,long newrow)
{
	if( oldrow < 0 )  return;
    CRecordInput *ParentWnd = (CRecordInput *)GetParent();
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
			
			str = ParentWnd->m_FieldList.GetAt(newrow);
			str = str.Right(str.GetLength() - 1);
			CString firstname = str.Left(str.Find('}'));
			firstname = "{" + firstname + "}";
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
    
	SetFocus();
}

void CRecordInfo::OnMenuCommand(int col,long row,int section,int item)
{
    CRecordInput *ParentWnd = (CRecordInput *)GetParent();
    if(row < 0 || row >= ParentWnd->m_FieldName.GetSize()) return;
	
	switch(item){
		case 1000:{ //字段提示导入
			static char Filter[]="字段提示文件 (*.txt)|*.txt||";
			CKSFileDialog MyDlg( TRUE,NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_EXPLORER ,	Filter , this );
			MyDlg.m_ofn.lpstrTitle = _T("字段提示导入");
			if( MyDlg.DoModal() == IDCANCEL )	 return;
			POSITION pos = MyDlg.GetStartPosition();
			CString strPathName = MyDlg.GetNextPathName(pos);
			
			SAString promptinfo;
			promptinfo.Empty();

			CStdioFile File;
			if ( File.Open(strPathName, CFile::modeRead | CFile::typeText) )
			{
				CString str;
				while(1)
				{
					if( File.ReadString(str) == FALSE ) break;
					str.TrimLeft();
					str.TrimRight();
					if( str.IsEmpty() )   continue;

					if( str.GetAt(0) == '{' )
					{
						str = str.Right(str.GetLength() - str.Find('{') - 1);
						str = str.Left(str.Find('}'));
						str.TrimLeft();
						str.TrimRight();
						str = "{" + str + "}";
						promptinfo += str;
					}
					else
					{
						promptinfo += str;
						promptinfo += "&";
					}

				}
				File.Close();
			}

			theApp.ConnectDatabase();
			try
			{
				bool IsFieldDefined = false;

				SAString cmdstr = "Select count(*) from " + theApp.TABLE_DEFINE + "  where FieldName = :FieldName";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("FieldName").setAsString() = ParentWnd->m_NameList.GetAt(row);
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
						g_dbcommand.Param("FieldInfo").setAsString() = promptinfo;
						g_dbcommand.Param("FieldName").setAsString() = ParentWnd->m_NameList.GetAt(row);
						g_dbcommand.Execute();
						g_dbconnection.Commit();
						ParentWnd->m_FieldList.SetAt(row,promptinfo);
						if( GetCurrentRow() == row )
						{
							OnRowChange(row,row);
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
				else
				{
					CString msg;
					msg.Format("<%s>在字段辅助说明库中没有定义！", ParentWnd->m_NameList.GetAt(row));
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

			promptinfo.Empty();
			break;
		}
		case 1001:{ //字段提示导出
			if( ParentWnd->m_FieldList.GetAt(row).GetLength() <= 0 )
			{
				AfxMessageBox("字段提示内容为空！");
				break;
			}

			static char Filter[]="字段提示文件 (*.txt)|*.txt||";
			CKSFileDialog MyDlg( FALSE,"txt", NULL, OFN_EXTENSIONDIFFERENT| OFN_OVERWRITEPROMPT |	OFN_PATHMUSTEXIST | OFN_LONGNAMES | OFN_EXPLORER ,	Filter , this );
			MyDlg.m_ofn.lpstrTitle = _T("字段提示导出");
			if( MyDlg.DoModal() == IDCANCEL )	 return;
			POSITION pos = MyDlg.GetStartPosition();
			CString strPathName = MyDlg.GetNextPathName(pos);

			CStdioFile File;
			if ( File.Open(strPathName, CFile::modeWrite |CFile::modeCreate | CFile::typeText) )
			{
				char ch = ParentWnd->m_FieldList.GetAt(row).GetAt(0);
				if(ch == '{')
				{
					CStringArray flist;
					CString str = ParentWnd->m_FieldList.GetAt(row);
					str = str.Right(str.GetLength() - 1);
					while(1)
					{
						CString tmp = str.Left(str.Find('}'));
						flist.Add(tmp);
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
					for(int i = 0; i < flist.GetSize(); i++ )
					{
						CString firstname = flist.GetAt(i);
						firstname = "{" + firstname + "}";
						
						File.WriteString(firstname);
						File.WriteString("\n");
						
						str = ParentWnd->m_FieldList.GetAt(row);
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
								
								File.WriteString(tmp);
								File.WriteString("\n");
							
								str = str.Right(str.GetLength() - str.Find('&') - 1);
							}
						}
					}
					flist.RemoveAll();
				}
				else
				{
					CString str = ParentWnd->m_FieldList.GetAt(row);
					while(1)
					{
						if(str.Find('&') == -1)  break;
						CString tmp = str.Left(str.Find('&'));
						File.WriteString(tmp);
						File.WriteString("\n");
						str = str.Right(str.GetLength() - str.Find('&') - 1);
					}
				}

				File.Close();
			}
			break;
		}
	}
}

void CRecordInfo::OnKeyDown(UINT *vcKey,BOOL processed)
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

int CRecordInfo::OnEditVerify(int col,long row,CWnd *edit,UINT *vcKey)
{
	if(*vcKey == VK_TAB)
	{
		UINT i = VK_TAB;
		OnKeyDown(&i,TRUE);
	}
	return TRUE;
}
