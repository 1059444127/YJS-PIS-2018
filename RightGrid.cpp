// RightGrid.cpp: implementation of the CRightGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "RightGrid.h"
#include "PassManage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define RIGHTCOUNT		27
char *pRightList[] = { 	"Ȩ�޹���",
						"���Ȩ��",
						"�����ӡ",
						"��ȡ�������",
						"ɾ����¼",
						"ɾ��ͼ�������",
						"ͼ�񵼳�",
						"��������",
						"�޸�ϵͳ����",
						"���Ӳ�ѯ",
						"�޸ı��",
						"���淢��",
						"ɾ���ʵ�����",
						"ɾ��ϵͳ��־",
						"��¼�Ǽǹ���վ",
						"��¼ȡ�Ĺ���վ",
						"��¼��Ϲ���վ",
						"��¼��Ƭ����վ",
						"��¼ϸ��ѧ����վ",
						"�༭�����ʽ",
						"�޸Ļ�ɾ����Ƭ��¼",
						"�Զ����༭",
						"��¼������Ƭ����վ",
						"��¼�ؼ칤��վ",
						"��¼ϸ��ѧ��Ƭ����վ",
						"���ͱ�������",
						"ɾ��ȡ�ļ�¼"
						};
 
//////////////////////////////////////////////////////////////////////
//					 Construction/Destruction						//
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CRightGrid::CRightGrid()
{

}

CRightGrid::~CRightGrid()
{

}

int CRightGrid::OnCanSizeRow(long row)
{
	return FALSE;
}

int CRightGrid::OnCanSizeTopHdg()
{
	return FALSE;
}

int CRightGrid::OnCanSizeSideHdg()
{
	return FALSE;
}

void CRightGrid::OnSetup()
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

	SetHighlightRow(TRUE);
	SetDoubleBufferMode(TRUE);

	SetNumberCols(1);
	SetNumberRows(RIGHTCOUNT);

	for(int i = 0; i < RIGHTCOUNT; i++ )
	{
		QuickSetLabelText(0,i,pRightList[i]);
	}

	CRect rt;
	GetWindowRect(&rt);
	SetColWidth(0,rt.Width() - 20);

    CPassManage *pParent = (CPassManage *)GetParent();
	if(pParent->m_UserCtrl.GetNumberRows() <= 0)
	{
		for(int i = 0; i < GetNumberRows(); i++)
		{
			QuickSetText(0,i,"0");
		}
	}
	else
	{
		CString str;
		CUGCell  cell;
		pParent->m_UserCtrl.GetCellIndirect(2 , pParent->m_UserCtrl.GetCurrentRow(), &cell);
		cell.GetText(&str);
		int right = atoi(str);
		for(int i = 0; i < GetNumberRows(); i++)
		{
			if( (right >> i) & 0x00000001 )
				QuickSetText( 0, i, "1");
			else
				QuickSetText( 0, i, "0");
		}
	}
}

int CRightGrid::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	if(!(col == 0 && row >= 0 && row < GetNumberRows()))   return TRUE;
	
	if( msg == UGCT_CHECKBOXSET )
	{
		CString str;
		CUGCell  cell;
		int value = 0, k;
		for(int i = 0; i < GetNumberRows(); i++ )
		{
			GetCellIndirect(0 , i, &cell);
			cell.GetText(&str);
			k = atoi(str);
			if( k )
			{
				k = 1;
				k = (k << i);
				value = (value | k);
			}
		}

		CPassManage *pParent = (CPassManage *)GetParent();
		k = pParent->m_UserCtrl.GetCurrentRow();
	    if(k >= 0 && k < pParent->m_UserCtrl.GetNumberRows())
		{
			CString username;
			pParent->m_UserCtrl.GetCellIndirect(0 , k, &cell);
			cell.GetText(&username);

			theApp.ConnectDatabase();

			try
			{
				SAString cmdstr = "Update " + theApp.TABLE_SECURITY + " set userright = :userright where username = :username";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("username").setAsString() = username;
				g_dbcommand.Param("userright").setAsLong()  = value;
				
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
				return TRUE;
			}

			str.Format("%d", value);
			pParent->m_UserCtrl.QuickSetText(2,k,str);
			pParent->m_UserCtrl.RedrawCell(2,k);
		}
	}
	
	return TRUE;
}
