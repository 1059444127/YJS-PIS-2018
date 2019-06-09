// SfDetailGrid.cpp: implementation of the CSfDetailGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "SfDetailGrid.h"
#include "ChargeDetail.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CSfDetailGrid::CSfDetailGrid()
{

}

CSfDetailGrid::~CSfDetailGrid()
{

}

void CSfDetailGrid::OnSetup()
{
	m_font.CreateFont(12,0,0,0,400,0,0,0,0,0,0,0,0,"MS Sans Serif");
	SetDefFont(&m_font);	

	//setup the grid
	SetNumberRows(0);
	SetNumberCols(9);

	SetSH_NumberCols(2);
	SetSH_Width(50);
	SetSH_ColWidth(0,25);
	SetSH_ColWidth(-1,25);


	SetTH_NumberRows(1);

	SetColWidth(0,80);  // �շѴ���
	SetColWidth(1,200); // �շ�����
	SetColWidth(2,120); // �����շѣ�Ԫ��
	SetColWidth(3,50);  // ������� 0-סԺ 1-����
	SetColWidth(4,100); // ��λ
	SetColWidth(5,50);  // ����
	SetColWidth(6,120); // �����ܷ��ã�Ԫ��
	SetColWidth(7,150); // ������ע
	SetColWidth(8,50);  // �Ʒ�Ա
	
	SetHighlightRow(TRUE);
	
	SetVScrollMode( UG_SCROLLTRACKING );
	SetHScrollMode( UG_SCROLLTRACKING );
}

void CSfDetailGrid::GridSetup()
{
    CChargeDetail *pParent = (CChargeDetail *)GetParent();

	int row_num = pParent->SfDetailArray.GetSize();
	int col_num = 9;

	SetNumberRows(row_num);
	SetNumberCols(col_num);

	for(int index = 0; index < row_num; index ++)
	{
		CHARGEITEM_RECORD tmp = pParent->SfDetailArray.GetAt(index);
		CString str;
		
		QuickSetCellType(1,index,UGCT_DROPLIST);
		QuickSetCellTypeEx(1,index,UGCT_DROPLISTHIDEBUTTON);
		QuickSetLabelText(1,index,pParent->m_LabelText);

		str.Format("%s",tmp.code);
		QuickSetText(0,index,str);

		str.Format("%s",tmp.item);
		QuickSetText(1,index,str);

		str.Format("%.2f",tmp.SinglePrice);
		QuickSetText(2,index,str);

		switch(tmp.Type) {
		case 0:
			QuickSetText(3,index,"סԺ");
			break;
		case 1:
			QuickSetText(3,index,"����");
			break;
		default:
			QuickSetText(3,index," ");
		}

		str.Format("%s",tmp.Unit);
		QuickSetText(4,index,str);

		str.Format("%d",tmp.Num);
		QuickSetText(5,index,str);

		str.Format("%.2f",tmp.price);
		QuickSetText(6,index,str);

		str.Format("%s",tmp.Note);
		QuickSetText(7,index,str);

		str.Format("%s",tmp.Recorder);
		QuickSetText(8,index,str);
	}


	if( row_num > 0 )
	{
		GotoRow(0);
	}
	
	RedrawAll();
}

void CSfDetailGrid::OnGetCell(int col,long row,CUGCell *cell)
{
	static char buf[_MAX_PATH];

	if( row == -1 )     //  Display table head information of pRecordSet_Define
	{
		if( col >= 0 )
		{
			switch ( col )
			{
			case 0:
				strcpy(buf,"�շѴ���");
				break;
			case 1:
				strcpy(buf,"�շ�����");
				break;
			case 2:
				strcpy(buf,"�����շѣ�Ԫ��");
				break;
			case 3:
				strcpy(buf,"�������");
				break;
			case 4:
				strcpy(buf,"��λ");
				break;
			case 5:
				strcpy(buf,"����");
				break;
			case 6:
				strcpy(buf,"ʵ�շ��ã�Ԫ��");
				break;
			case 7:
				strcpy(buf,"������ע");
				break;
			case 8:
				strcpy(buf,"�Ʒ�Ա");
				break;
			}
			cell->SetText(buf);
		}
		return;
	}
	if( col == -2 )
	{
		if( row >= 0 )
		{
			if( row == GetCurrentRow() )
				cell->SetCellType(UGCT_ARROW);
		}
		return;
	}

	if( col == -1 )
	{
		if( row >= 0 )
		{
			CString str;
			str.Format("%d", row + 1);
			cell->SetText(str);
		}
		return;
	}

	return;
}

int CSfDetailGrid::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	if(col == 1 && row >= 0)
	{
		if(msg == UGCT_DROPLISTSELECT)
		{
			CChargeDetail *pParent = (CChargeDetail *)GetParent();
			CHARGEITEM_RECORD tmp = pParent->SfDetailArray.GetAt(row);
			
			CString *string = (CString*)param;
			(*string).TrimLeft();  (*string).TrimRight();

			strcpy(tmp.item, *string);

			for(int i = 0; i < pParent->ChargeDBArray.GetSize(); i++)
			{
				CHARGEITEM_RECORD  t;
				t = pParent->ChargeDBArray.GetAt(i);
                if(strcmp(tmp.item, t.item) == 0)  break;
			}

			if(i < pParent->ChargeDBArray.GetSize())
			{
				tmp = pParent->ChargeDBArray.GetAt(i);
				tmp.price = tmp.SinglePrice * tmp.Num;
				if(theApp.m_HisCfg.CompareNoCase("ɽ������ҽ��Ժ") == 0)
				{
					CString pp = tmp.code;
					pp.TrimLeft();  pp.TrimRight();
					for(int i = 0; i < pParent->ITEM_CODE_List.GetSize(); i++)
					{
						if(pp.CompareNoCase(pParent->ITEM_CODE_List.GetAt(i)) == 0)
						{
							tmp.price *= pParent->ParamList.GetAt(i);
							break;
						}
					}
				}
			}
			
			pParent->SfDetailArray.SetAt(row,tmp);
			
			GridSetup();
		}
	}
	
	return TRUE;
}

void CSfDetailGrid::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	if(col >= 0 && row >= 0 && col != 8)
	{
		if(theApp.m_HisCfg.CompareNoCase("ɽ������ҽ��Ժ") == 0)
		{
			CChargeDetail *pParent = (CChargeDetail *)GetParent();
			if(row < pParent->m_Original_Item_Num)
			{
				return;
			}
		}
		
		StartEdit();
	}
}

void CSfDetailGrid::OnCharDown(UINT *vcKey,BOOL processed)
{
	if(GetCurrentCol() >= 0 && GetCurrentRow() >= 0 && GetCurrentCol() != 8)
	{
		if(theApp.m_HisCfg.CompareNoCase("ɽ������ҽ��Ժ") == 0)
		{
			CChargeDetail *pParent = (CChargeDetail *)GetParent();
			if(GetCurrentRow() < pParent->m_Original_Item_Num)
			{
				return;
			}
		}

		StartEdit(*vcKey);
	}
}

void CSfDetailGrid::OnKeyDown(UINT *vcKey,BOOL processed){
	
	if(*vcKey == VK_DELETE)
	{
		CChargeDetail *pParent = (CChargeDetail *)GetParent();
		pParent->SendMessage(WM_COMMAND,ID_CHARGEITEM_DELETE);
	}
}

int CSfDetailGrid::OnEditFinish(int col, long row,CWnd * edit, LPCTSTR string,BOOL cancelFlag)
{
	if(cancelFlag)
	{
		QuickSetText(col,row,string);
	}
	if(col >= 0 && row >= 0)
	{
        CChargeDetail *pParent = (CChargeDetail *)GetParent();
		CHARGEITEM_RECORD tmp = pParent->SfDetailArray.GetAt(row);
  
		switch(col) {
		case 0:
			{
				CString str = string;
				str.TrimLeft(); str.TrimRight();
				strcpy(tmp.code, str);
				if(strlen(tmp.code) > 0)
				{
					for(int i = 0; i < pParent->ChargeDBArray.GetSize(); i++)
					{
						CHARGEITEM_RECORD  t;
						t = pParent->ChargeDBArray.GetAt(i);
						if(strcmp(tmp.code, t.code) == 0)  break;
					}
					
					if(i < pParent->ChargeDBArray.GetSize())
					{
						tmp = pParent->ChargeDBArray.GetAt(i);
						tmp.price = tmp.SinglePrice * tmp.Num;
						if(theApp.m_HisCfg.CompareNoCase("ɽ������ҽ��Ժ") == 0)
						{
							CString pp = tmp.code;
							pp.TrimLeft();  pp.TrimRight();
							for(int i = 0; i < pParent->ITEM_CODE_List.GetSize(); i++)
							{
								if(pp.CompareNoCase(pParent->ITEM_CODE_List.GetAt(i)) == 0)
								{
									tmp.price *= pParent->ParamList.GetAt(i);
									break;
								}
							}
						}
					}
				}
			}
			break;
		case 1:
			strcpy(tmp.item, string);
			break;
		case 2:
			tmp.SinglePrice = (float)atof(string);
			tmp.price = tmp.SinglePrice * tmp.Num;
			if(theApp.m_HisCfg.CompareNoCase("ɽ������ҽ��Ժ") == 0)
			{
				CString pp = tmp.code;
				pp.TrimLeft();  pp.TrimRight();
				for(int i = 0; i < pParent->ITEM_CODE_List.GetSize(); i++)
				{
					if(pp.CompareNoCase(pParent->ITEM_CODE_List.GetAt(i)) == 0)
					{
						tmp.price *= pParent->ParamList.GetAt(i);
						break;
					}
				}
			}
			break;
		case 3:
			tmp.Type = 0;
			if(strstr(string, "����") != NULL)
			{
				tmp.Type = 1;
			}
			break;
		case 4:
			strcpy(tmp.Unit, string);
			break;
		case 5:
			tmp.Num   = atoi(string);
			tmp.price = tmp.SinglePrice * tmp.Num;
			if(theApp.m_HisCfg.CompareNoCase("ɽ������ҽ��Ժ") == 0)
			{
				CString pp = tmp.code;
				pp.TrimLeft();  pp.TrimRight();
				for(int i = 0; i < pParent->ITEM_CODE_List.GetSize(); i++)
				{
					if(pp.CompareNoCase(pParent->ITEM_CODE_List.GetAt(i)) == 0)
					{
						tmp.price *= pParent->ParamList.GetAt(i);
						break;
					}
				}
			}
			break;
		case 6:
			tmp.price = (float)atof(string);
			break;
		case 7:
			strcpy(tmp.Note, string);
			break;
		}

		pParent->SfDetailArray.SetAt(row,tmp);
		
		GridSetup();
	}
	
	return TRUE;
}

