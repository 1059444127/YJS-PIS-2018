
#include "stdafx.h"
#include "Pathology.h"
#include "PathologyView.h"
#include "PathologyDoc.h"

#include "CannedTextTree.h"
#include "sqlapi.h"
#include "StrEdit.h"

#include <vector>

#define ID_CANNEDTEXTTREE_REFRESH		WM_USER + 51
#define ID_CANNEDTEXTTREE_HIDEEMPTY		WM_USER + 50
#define ID_CANNEDTEXTTREE_BODYPART_ADD	WM_USER + 52
#define ID_CANNEDTEXTTREE_BODYPART_DEL	WM_USER + 53
#define ID_CANNEDTEXTTREE_DISCEASE_ADD	WM_USER + 54
#define ID_CANNEDTEXTTREE_DISCEASE_DEL	WM_USER + 55
#define ID_CANNEDTEXTTREE_SAMPLE_ADD	WM_USER + 56
#define ID_CANNEDTEXTTREE_SAMPLE_DEL	WM_USER + 57
#define ID_CANNEDTEXTTREE_SAMPLE_UPDATE	WM_USER + 58
#define ID_CANNEDTEXTTREE_BODYPART_UP	WM_USER + 59
#define ID_CANNEDTEXTTREE_BODYPART_DOWN	WM_USER + 60
#define ID_CANNEDTEXTTREE_SAMPLE_PICKUP	WM_USER + 61

CCannedText::CCannedText()
{	
	m_sBodyPartName = _T("");
	m_sDisceaseName = _T("");
	m_sCannedText	= _T("");
}

CCannedText::~CCannedText()
{
}


IMPLEMENT_DYNAMIC(CCannedTextTree, CXTTreeCtrl)

BEGIN_MESSAGE_MAP(CCannedTextTree, CXTTreeCtrl)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRClick)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CANNEDTEXTTREE_REFRESH,				OnTreeRefresh)
	ON_COMMAND(ID_CANNEDTEXTTREE_HIDEEMPTY,				OnTreeHideEmptyNode)
	ON_COMMAND(ID_CANNEDTEXTTREE_BODYPART_ADD,			OnTreeAddBodyPart)
	ON_COMMAND(ID_CANNEDTEXTTREE_BODYPART_DEL,			OnTreeDelBodyPart)
	ON_COMMAND(ID_CANNEDTEXTTREE_DISCEASE_ADD,			OnTreeAddDiscease)
	ON_COMMAND(ID_CANNEDTEXTTREE_DISCEASE_DEL,			OnTreeDelDiscease)
	ON_COMMAND(ID_CANNEDTEXTTREE_SAMPLE_ADD,			OnTreeAddSample)
	ON_COMMAND(ID_CANNEDTEXTTREE_SAMPLE_DEL,			OnTreeDelSample)
	ON_COMMAND(ID_CANNEDTEXTTREE_SAMPLE_UPDATE,			OnTreeUpdateSample)
	ON_COMMAND(ID_CANNEDTEXTTREE_BODYPART_UP,			OnTreeMoveUpBodyPart)
	ON_COMMAND(ID_CANNEDTEXTTREE_BODYPART_DOWN,			OnTreeMoveDnBodyPart)
	ON_COMMAND(ID_CANNEDTEXTTREE_SAMPLE_PICKUP,			OnTreePickupCannedText)
END_MESSAGE_MAP()


CCannedTextTree::CCannedTextTree()
{
	m_nMode = 0;
	m_nEditCtrlID = 0;
	m_pEditCtrlDlgWnd = NULL;
	m_bHideEmptyNode = TRUE;
	m_pMaterialListWnd = NULL;
}

CCannedTextTree::~CCannedTextTree()
{

}

void CCannedTextTree::Initialize(int nMode, UINT nEditCtrlID, CWnd *pDlgWnd, CMaterialGrid *pGridWnd)
{
	if (nMode >= 1 && nMode <= 4)
		m_nMode = nMode;
	else
		m_nMode = 0;

	m_nEditCtrlID		= nEditCtrlID;
	m_pEditCtrlDlgWnd	= pDlgWnd;
	m_pMaterialListWnd	= pGridWnd;
}

const char * CCannedTextTree::GetTitle()
{
	const char *szTitles[] = {"肉眼所见", "光镜所见", "诊断意见", ""};

	if (m_nMode >= 1 && m_nMode <= 3)
		return szTitles[m_nMode - 1];

	return "";
}

std::vector <CCannedText *>::iterator CCannedTextTree::FindFirstItemInCannedTextList(std::vector<CCannedText *> &lst, CString sBodyPartName)
{
	std::vector <CCannedText *> ::iterator it, itFirst;
	BOOL bFound;

	itFirst = lst.end();

	for (it = lst.begin(), bFound = FALSE; it != lst.end() && !bFound; it ++)
	{
		if ((*it)->m_sBodyPartName == sBodyPartName)
		{
			bFound = TRUE;
			itFirst = it;
		}
	}
	
	return itFirst;
}


void CCannedTextTree::TransferCannedText2Editor(HTREEITEM hItem)
{
	CString sOriginText;
	CString sCannedText;
	HTREEITEM hCannedTextItem;
	int nIdx1 = 0, nIdx2 = 0;
	CEdit *pWndEdit;

	if (hItem == NULL) return;

	if (m_nMode == 4)
	{
		UpdateUGGrid();
		return;
	}

	if (m_pEditCtrlDlgWnd == NULL) return;

	GetItemImage(hItem, nIdx1, nIdx2);
	
	if (ItemHasChildren(hItem))
		hCannedTextItem = GetChildItem(hItem);
	else
		hCannedTextItem = hItem;

	sCannedText = GetItemText(hCannedTextItem);

	pWndEdit = (CEdit *) m_pEditCtrlDlgWnd->GetDlgItem(m_nEditCtrlID);
	if (pWndEdit == NULL) return;

	if (pWndEdit->IsWindowEnabled() && pWndEdit->IsWindowVisible())
	{		
		pWndEdit->GetWindowText(sOriginText);
		pWndEdit->SetWindowText(sOriginText + sCannedText);
		pWndEdit->RedrawWindow();

		m_pEditCtrlDlgWnd->SendMessage(WM_COMMAND, MAKEWPARAM(m_nEditCtrlID, EN_CHANGE), (long) pWndEdit->GetSafeHwnd());
	}
}


void CCannedTextTree::GetSampleBodyPartsInfo(CString &sInfo)
{
	int ni;
	CString sBodyParts("");

	for (ni = 0; ni < theApp.m_fnamelist.GetSize(); ni++)
	{
		if (theApp.m_fnamelist.GetAt(ni).CompareNoCase("qcbw") == 0) 
			sBodyParts = theApp.m_finfolist.GetAt(ni);
	}

	sInfo = sBodyParts;

	return;
}

void CCannedTextTree::GetBodyPartsList(CStringArray &list)
{
	CString sCmdText;
	CString sBodyPartName;

	sCmdText.Format(_T("SELECT Zhangqi From %s "), theApp.TABLE_ZHANGQI);

	theApp.ConnectDatabase();

	try
	{
		g_dbcommand.setCommandText((SAString) sCmdText);
		g_dbcommand.Execute();
		
		while(g_dbcommand.FetchNext() )
		{
			sBodyPartName = g_dbcommand.Field("ZhangQi").asString();
			
			if (!sBodyPartName.IsEmpty())
				list.Add(sBodyPartName);
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


void CCannedTextTree::GetCannedText(std::vector<CCannedText *> &lst)
{
	CString sCmdText;
	CString sBodyPartName;
	CString sDisceaseName;
	CString sCannedText;

	CCannedText *pCannedText;

	if (m_bHideEmptyNode)
	{
		sCmdText.Format(_T(" SELECT b.zhangqi, b.bingzhong, c.fanben \
			from %s  b, %s c  where c.zhangqi = b.zhangqi and c.bingzhong = b.bingzhong and c.usageType = %d \
			order by b.zhangqi, b.bingzhong"), theApp.TABLE_BINGZHONG, theApp.TABLE_STANDTEXT, m_nMode);
	}
	else
	{
		sCmdText.Format(_T(" SELECT b.zhangqi, b.bingzhong, c.fanben \
			from (%s  b left join %s c on c.zhangqi = b.zhangqi and c.bingzhong = b.bingzhong and c.usageType = %d) \
			order by b.zhangqi, b.bingzhong"), theApp.TABLE_BINGZHONG, theApp.TABLE_STANDTEXT, m_nMode);
	}

	theApp.ConnectDatabase();

	try
	{
		g_dbcommand.setCommandText((SAString) sCmdText);
		g_dbcommand.Execute();
		
		while(g_dbcommand.FetchNext() )
		{
			sBodyPartName = g_dbcommand.Field("ZhangQi").asString();
			sDisceaseName = g_dbcommand.Field("BingZhong").asString();
			sCannedText	  = g_dbcommand.Field("fanben").asString();
			
			pCannedText = new CCannedText;
			pCannedText->m_sBodyPartName = sBodyPartName;
			pCannedText->m_sDisceaseName = sDisceaseName;
			pCannedText->m_sCannedText	 = sCannedText;

			lst.push_back(pCannedText);

			g_dbconnection.Commit();
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
	}	
}


void CCannedTextTree::UpdateUGGrid()
{
	HTREEITEM hItem;
	CString bgzt;

	if (m_pMaterialListWnd == NULL) return;

	try
	{
		SAString cmdstr = "Select bgzt from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
			
		while (g_dbcommand.FetchNext())
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
		
	if (theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		return;
	}

	hItem = GetFirstSelectedItem();
	if ( hItem != NULL)
	{
		int col; long row;
		if (ItemHasChildren(hItem) == TRUE) return;
		CString text = GetItemText(hItem);
		int result = m_pMaterialListWnd->EnumFirstSelected(&col,&row);
		
		while (result == UG_SUCCESS)
		{
			m_pMaterialListWnd->QuickSetText(2,row,text);
			
			try
			{
				SAString cmdstr = "update " + theApp.TABLE_MATERIAL + " set qcbw =:qcbw where medicalno=:medicalno and rwly=:rwly and xh=:xh";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("qcbw").setAsString() = text;
				g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
				g_dbcommand.Param("rwly").setAsString() = m_pMaterialListWnd->QuickGetText(0,row);
				g_dbcommand.Param("xh").setAsString() = m_pMaterialListWnd->QuickGetText(1,row);
				g_dbcommand.Execute();
				g_dbconnection.Commit();
			} 
			catch (SAException &e) 
			{
				g_dbconnection.Rollback();
				AfxMessageBox((const char*)e.ErrText());
			}
			
			result = m_pMaterialListWnd->EnumNextSelected(&col,&row);
		}

		m_pMaterialListWnd->ResetGridSize();
		m_pMaterialListWnd->RedrawAll();
	}
}


void CCannedTextTree::BuildTree()
{
	HTREEITEM hLevel1, hLevel2, hItem, hSelectedItem, hParentItem, hChildItem, hNextItem;
	CString sSelectedItem;
	std::vector <CCannedText *> lstCannedText;
	std::vector <CCannedText *>::iterator it, itFirst;
	CStringArray lstBodyParts;
	CString sLevel1_0(""), sLevel2_0(""), sLevel3_0("");
	CString sBodyPartName;
	CString sDisceaseName, sDisceaseName_0;
	CString sCannedText;
	CString sSampleBodyPartsInfo("");
	int ni, nIdx1, nIdx2;
	BOOL bLastFound, bFound;

	hSelectedItem = GetFirstSelectedItem();
	if (hSelectedItem != NULL)
	{
		GetItemImage(hSelectedItem, nIdx1, nIdx2);

		if (nIdx1 == 0)
		{
			sLevel1_0 = GetItemText(hSelectedItem);
			sLevel2_0 = "";
			sLevel3_0 = "";
		}
		else if (nIdx1 == 1)
		{
			hParentItem = GetParentItem(hSelectedItem);
			sLevel1_0 = GetItemText(hParentItem);
			sLevel2_0 = GetItemText(hSelectedItem);
			sLevel3_0 = "";
		}
		else if (nIdx1 == 2)
		{
			sLevel3_0 = GetItemText(hSelectedItem);
			hParentItem = GetParentItem(hSelectedItem);
			sLevel2_0 = GetItemText(hParentItem);
			hParentItem = GetParentItem(hParentItem);
			sLevel1_0 = GetItemText(hParentItem);
		}
		else
		{
			sLevel1_0 = "";
			sLevel2_0 = "";
			sLevel3_0 = "";
		}
	}

	DeleteAllItems();

	if (m_nMode == 4)
	{		
		GetSampleBodyPartsInfo(sSampleBodyPartsInfo);
		
		while (sSampleBodyPartsInfo.Find("}") > 0)
		{
			CString lv1_t = sSampleBodyPartsInfo.Left(sSampleBodyPartsInfo.Find("}"));
			sSampleBodyPartsInfo = sSampleBodyPartsInfo.Right(sSampleBodyPartsInfo.GetLength() - sSampleBodyPartsInfo.Find("}") - 1);
			CString lv1 = lv1_t.Right(lv1_t.GetLength() - 1);
			HTREEITEM item = InsertItem(lv1,0,0);
			SetItemData(item,m_nMode);
			CString lv2_t = sSampleBodyPartsInfo.Left(sSampleBodyPartsInfo.Find("{"));
		
			if (lv2_t.IsEmpty())
			{
				lv2_t = sSampleBodyPartsInfo;
			}
			CString lv2;
			while(lv2_t.Find("&") > 0)
			{
				lv2 = lv2_t.Left(lv2_t.Find("&"));
				HTREEITEM item1 = InsertItem(lv2,1,1,item);
				SetItemData(item1, m_nMode);
				lv2_t = lv2_t.Right(lv2_t.GetLength() - lv2_t.Find("&") - 1);
			}
		
			sSampleBodyPartsInfo = sSampleBodyPartsInfo.Right(sSampleBodyPartsInfo.GetLength() - sSampleBodyPartsInfo.Find("{"));	
		}
	}
	else
	{
		// Step 1, Get BodyParts List;	
		GetBodyPartsList(lstBodyParts);

		// Step2 , Get CannedText
		GetCannedText(lstCannedText);

		for (ni = 0; ni < lstBodyParts.GetSize(); ni ++)
		{
			sDisceaseName_0 = "%^#@%^@#&$#&&@";

			sBodyPartName = lstBodyParts.GetAt(ni);
		
			bLastFound = FALSE;

			itFirst = FindFirstItemInCannedTextList(lstCannedText, sBodyPartName);

			if ( itFirst != lstCannedText.end() || !m_bHideEmptyNode)
			{
				hLevel1 = InsertItem(sBodyPartName, 0,0 );	
				SetItemData(hLevel1, m_nMode);
			}

			for (it = itFirst; it != lstCannedText.end() && !bLastFound; it ++)
			{
				if ( sBodyPartName.Compare((*it)->m_sBodyPartName) == 0)
				{				
					sDisceaseName = (*it)->m_sDisceaseName;
					sCannedText   = (*it)->m_sCannedText;

					if (!sDisceaseName.IsEmpty())
					{
						if (sDisceaseName != sDisceaseName_0)
						{
							sDisceaseName_0 = sDisceaseName;
							hLevel2 = InsertItem( sDisceaseName, 1, 1, hLevel1);
							SetItemData(hLevel2, m_nMode);
						}

						if (!sCannedText.IsEmpty())
						{
							hItem = InsertItem(sCannedText, 2, 2, hLevel2);
							SetItemData(hItem, m_nMode);				
						}
					}
				}
				else
				{
					bLastFound = TRUE;
				}
			}
		}
			
		lstBodyParts.RemoveAll();

		for (it = lstCannedText.begin(); it != lstCannedText.end(); it ++)
		{
			delete *it;
		}

		lstCannedText.clear();
	}

	if (!sLevel1_0.IsEmpty())
	{
		hSelectedItem = NULL;
		
		hNextItem = GetNextItem(NULL, TVGN_ROOT); 
		while (hNextItem != NULL && hSelectedItem == NULL)
		{
			sBodyPartName = GetItemText(hNextItem);
			if (sBodyPartName.Compare(sLevel1_0) == 0)
			{
				hSelectedItem = hNextItem;
			}

			hNextItem = GetNextItem(hNextItem, TVGN_NEXT);
		}
		
		if (hSelectedItem != NULL)
		{
			SelectItem(hSelectedItem);
			Expand(hSelectedItem, TVE_EXPAND);

			if (!sLevel2_0.IsEmpty())
			{
				hChildItem = GetChildItem(hSelectedItem);

				bFound = FALSE;

				while (hChildItem != NULL && !bFound)
				{
					sDisceaseName = GetItemText(hChildItem);
					if (sDisceaseName.Compare(sLevel2_0) == 0)
						bFound = TRUE;
					else
					{
						hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
						hChildItem = hNextItem;
					}
				}

				if (bFound && hChildItem != NULL)
				{
					SelectItem(hChildItem);
					Expand(hChildItem, TVE_EXPAND);
				}
			}
		}
	}
	

	RedrawWindow();

	return;
}


void CCannedTextTree::CollapseAllItemsBut(HTREEITEM hItem)
{
	HTREEITEM hNextItem;

	hNextItem = GetNextItem(NULL, TVGN_ROOT); 
	while (hNextItem != NULL)
	{
		if (hNextItem != hItem)
			Expand(hNextItem, TVE_COLLAPSE);
		else
			Expand(hNextItem, TVE_EXPAND);
		

		hNextItem = GetNextItem(hNextItem, TVGN_NEXT);
	}

	return;
}


void CCannedTextTree::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	HTREEITEM hSelectedItem;

	hSelectedItem = GetFirstSelectedItem();
	if (hSelectedItem == NULL) return;

	if (ItemHasChildren(hSelectedItem))
	{
		Expand(hSelectedItem, TVE_TOGGLE);
		return;
	}
	
	TransferCannedText2Editor(hSelectedItem);
}


void CCannedTextTree::OnRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Send WM_CONTEXTMENU to self

	SendMessage(WM_CONTEXTMENU, (WPARAM) m_hWnd, GetMessagePos());

	// Mark message as handled and suppress default handling

	*pResult = 1;
}


void CCannedTextTree::OnContextMenu(CWnd *pWnd, CPoint pos)
{
	CPoint ptMousePos;
	int nIdx1 = 0, nIdx2 = 0, nChildrenCount = 0;
	HTREEITEM hItem, hChildItem, hNextItem;
	CXTMenu contextMenu;
	UINT uFlags;

	// if Shift-F10
	ptMousePos = pos;

	if (ptMousePos.x == -1 && ptMousePos.y == -1)
	{
		ptMousePos = (CPoint) GetMessagePos();
	}

	ScreenToClient(&ptMousePos);
	

	//hItem = GetFirstSelectedItem();

	hItem = HitTest(ptMousePos, &uFlags );

	if (hItem != NULL)
	{
		SelectItem(hItem);
		GetItemImage(hItem, nIdx1, nIdx2);
		if (nIdx1 == 0)
		{
			CollapseAllItemsBut(hItem);
		}
	}

	contextMenu.CreatePopupMenu();

	contextMenu.AppendMenu(MF_BYCOMMAND, ID_CANNEDTEXTTREE_REFRESH,		"重新装入");
	contextMenu.AppendMenu(MF_SEPARATOR);

	if (m_nMode != 4)
		contextMenu.AppendMenu(MF_BYCOMMAND, ID_CANNEDTEXTTREE_HIDEEMPTY,	"隐藏空节点");
	
	if ((m_nMode != 4) || (m_nMode == 4 && nIdx1 != 0))
		contextMenu.AppendMenu(MF_SEPARATOR);
	
	if (hItem == NULL)
	{
		if (m_nMode != 4)
			contextMenu.AppendMenu(MF_BYCOMMAND, ID_CANNEDTEXTTREE_BODYPART_ADD, "增加新部位");
	}
	else
	{
		switch(nIdx1) 
		{
		case 0:
			if (m_nMode != 4)
			{
				contextMenu.AppendMenu(MF_BYCOMMAND, ID_CANNEDTEXTTREE_BODYPART_ADD,	"增加新部位");
				contextMenu.AppendMenu(MF_BYCOMMAND, ID_CANNEDTEXTTREE_BODYPART_DEL,	"删除部位");
				contextMenu.AppendMenu(MF_BYCOMMAND, ID_CANNEDTEXTTREE_DISCEASE_ADD,	"增加新病种");
				contextMenu.AppendMenu(MF_SEPARATOR);
				contextMenu.AppendMenu(MF_BYCOMMAND, ID_CANNEDTEXTTREE_BODYPART_UP,		"部位上移");
				contextMenu.AppendMenu(MF_BYCOMMAND, ID_CANNEDTEXTTREE_BODYPART_DOWN,	"部位下移");
			}
			break;
		case 1:
			contextMenu.AppendMenu(MF_BYCOMMAND, ID_CANNEDTEXTTREE_SAMPLE_PICKUP,	(m_nMode == 4) ? "选取部位" : "选取范本");
			if (m_nMode != 4)
			{
				contextMenu.AppendMenu(MF_SEPARATOR);
				contextMenu.AppendMenu(MF_BYCOMMAND, ID_CANNEDTEXTTREE_SAMPLE_ADD,		"导入范本");
				contextMenu.AppendMenu(MF_BYCOMMAND, ID_CANNEDTEXTTREE_DISCEASE_DEL,	"删除病种");
			}

			break;
		case 2:
			contextMenu.AppendMenu(MF_BYCOMMAND, ID_CANNEDTEXTTREE_SAMPLE_PICKUP,	"选取范本");
			
			if (m_nMode != 4)
			{
				contextMenu.AppendMenu(MF_SEPARATOR);
				contextMenu.AppendMenu(MF_BYCOMMAND, ID_CANNEDTEXTTREE_SAMPLE_DEL,		"删除范本");
				contextMenu.AppendMenu(MF_BYCOMMAND, ID_CANNEDTEXTTREE_SAMPLE_UPDATE,	"更新范本");
			}

			break;
		}
		
	}
	
	if (m_bHideEmptyNode)
		contextMenu.CheckMenuItem(ID_CANNEDTEXTTREE_HIDEEMPTY, MF_BYCOMMAND | MF_CHECKED);
	
	if (!ItemHasChildren(hItem))
		contextMenu.EnableMenuItem(ID_CANNEDTEXTTREE_SAMPLE_PICKUP, MF_BYCOMMAND | MF_ENABLED);
	else
	{
		hChildItem = GetChildItem(hItem);

		nChildrenCount = 0;
		while (hChildItem != NULL)
		{
			hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
			hChildItem = hNextItem;
			nChildrenCount ++;
		}

		contextMenu.EnableMenuItem(ID_CANNEDTEXTTREE_SAMPLE_PICKUP, MF_BYCOMMAND | (nChildrenCount == 1) ? MF_ENABLED : MF_DISABLED);
	}
	
	ClientToScreen(&ptMousePos);
	contextMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptMousePos.x, ptMousePos.y, this);
}

void CCannedTextTree::OnTreeRefresh()
{
	BuildTree();
}

void CCannedTextTree::OnTreeHideEmptyNode()
{
	m_bHideEmptyNode = !m_bHideEmptyNode;
	BuildTree();
}

void CCannedTextTree::OnTreeAddBodyPart()
{
	HTREEITEM hNewItem;
	CString sCmdText("");
	CString sBodyPartName("");
	BOOL bExisted;
	StrEdit dlgInput("请输入新的脏器名称",this);
	
	theApp.ConnectDatabase();

	if (dlgInput.DoModal() != IDOK) return;
	
	sBodyPartName = dlgInput.m_EditStr;
	sBodyPartName.TrimLeft();
	sBodyPartName.TrimRight();
			
	bExisted = FALSE;

	sCmdText.Format( "Select ZHANGQI from %s where zhangqi = :zhangqi", theApp.TABLE_ZHANGQI);
	
	try
	{
		g_dbcommand.setCommandText((SAString) sCmdText);
		g_dbcommand.Param("zhangqi").setAsString() = sBodyPartName;				
		g_dbcommand.Execute();
				
		if (g_dbcommand.FetchNext())
			bExisted = TRUE;

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
		
	if (bExisted) return;
	
	sCmdText.Format("INSERT INTO %s (ZHANGQI) VALUES(:zhangqi) ", theApp.TABLE_ZHANGQI);

	try
	{
		g_dbcommand.setCommandText((SAString) sCmdText);
		g_dbcommand.Param("zhangqi").setAsString() = sBodyPartName;
					
		g_dbcommand.Execute();
		g_dbconnection.Commit();

		hNewItem = InsertItem(sBodyPartName, 0, 0);
		SetItemData(hNewItem, m_nMode);
		RedrawWindow();
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

void CCannedTextTree::OnTreeDelBodyPart()
{
	CString sCmdText,sMsg;
	CString sBodyPartName, sDisceaseName,  sCannedText;
	HTREEITEM htiCurrent, htiBodyPartName, htiDisceaseName, htiCannedText;
	int nIdx1 = 0, nIdx2 = 0;


	theApp.ConnectDatabase();

	htiCurrent = GetFirstSelectedItem();
	if (htiCurrent == NULL) return;

	GetItemImage(htiCurrent, nIdx1, nIdx2);
	if (nIdx1 != 0) return;

	//删除脏器 
	
	htiBodyPartName = htiCurrent;

	sBodyPartName = GetItemText(htiBodyPartName);

	sMsg.Format("你是否真要删除部位 [ %s ]及其全部 [%s]范本?", sBodyPartName, GetTitle());

	if (AfxMessageBox(sMsg, MB_YESNO | MB_ICONQUESTION) != IDYES) return;

	// Step1, 删除脏器对应的全部X范本
	sCmdText.Format("DELETE FROM %s WHERE ZHANGQI = :zhangqi AND USAGETYPE = %d", theApp.TABLE_STANDTEXT, m_nMode);

	try
	{
		g_dbcommand.setCommandText((SAString) sCmdText);
		g_dbcommand.Param("zhangqi").setAsString()		= sBodyPartName;
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
	}
		
	// Step2, 脏器对应的全部病种  

	sCmdText.Format("DELETE FROM %s WHERE ZHANGQI = :zhangqi AND NOT EXISTS (SELECT DISTINCT ZHANGQI, BINGZHONG FROM %s WHERE ZHANGQI = :zhangqi) ",
			theApp.TABLE_BINGZHONG, theApp.TABLE_STANDTEXT); 
	try
	{
		g_dbcommand.setCommandText((SAString) sCmdText);
		g_dbcommand.Param("zhangqi").setAsString()		= sBodyPartName;
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
	}
			
	// Step3 删除脏器,必须在病种表中没有该脏器的相关记录
 
	sCmdText.Format("DELETE FROM %s  WHERE ZHANGQI = :zhangqi AND ZHANGQI NOT IN (SELECT DISTINCT ZHANGQI FROM %s WHERE ZHANGQI = :zhangqi ) ", 
			theApp.TABLE_ZHANGQI, theApp.TABLE_BINGZHONG);

	try
	{
		g_dbcommand.setCommandText((SAString) sCmdText);
			
		g_dbcommand.Param("zhangqi").setAsString()		= sBodyPartName;

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
	}
	
	DeleteItem(htiCurrent);
}

void CCannedTextTree::OnTreeAddDiscease()
{
	HTREEITEM hItem, hNewItem;
	CString sCmdText;
	CString sBodyPartName, sDisceaseName;
	BOOL bExisted;
	int nIdx1, nIdx2;

	theApp.ConnectDatabase();

	hItem = GetFirstSelectedItem();
	if (hItem == NULL) return;

	GetItemImage(hItem, nIdx1, nIdx2);
	if (nIdx1 != 0) return;

	sBodyPartName = GetItemText(hItem);
	
	StrEdit dlgInput("请输入新的病种(脏器: " + sBodyPartName + ")名称", this);
	
	if (dlgInput.DoModal() != IDOK) return;
	
	sDisceaseName = dlgInput.m_EditStr;
	sDisceaseName.TrimLeft();
	sDisceaseName.TrimRight();

	bExisted = FALSE;

	sCmdText.Format("SELECT ZHANGQI, BINGZHONG FROM %s WHERE ZHANGQI = :zhangqi AND BINGZHONG = :bingzhong", theApp.TABLE_BINGZHONG);

	try
	{
		g_dbcommand.setCommandText((SAString) sCmdText);
		g_dbcommand.Param("zhangqi").setAsString() = sBodyPartName;
		g_dbcommand.Param("bingzhong").setAsString() = sDisceaseName;
						
		g_dbcommand.Execute();
						
		if (g_dbcommand.FetchNext())
			bExisted = TRUE;
						
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
					
	if (bExisted) return;

	sCmdText.Format("INSERT INTO %s (ZHANGQI, BINGZHONG) VALUES(:zhangqi, :bingzhong)", theApp.TABLE_BINGZHONG);

	try
	{
		g_dbcommand.setCommandText((SAString) sCmdText);
		g_dbcommand.Param("zhangqi").setAsString() = sBodyPartName;
		g_dbcommand.Param("bingzhong").setAsString() = sDisceaseName;
							
		g_dbcommand.Execute();
		g_dbconnection.Commit();

		hNewItem = InsertItem(sDisceaseName, nIdx1 + 1, nIdx1 + 1, hItem);
		SetItemData(hNewItem, m_nMode);
		RedrawWindow();

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


void CCannedTextTree::OnTreeDelDiscease()
{
	CString sCmdText, sMsg;
	CString sBodyPartName, sDisceaseName,  sCannedText;
	HTREEITEM htiCurrent, htiBodyPartName, htiDisceaseName, htiCannedText;
	int nIdx1 = 0, nIdx2 = 0;

	// 删除病种

	theApp.ConnectDatabase();

	htiCurrent = GetFirstSelectedItem();
	if (htiCurrent == NULL) return;

	GetItemImage(htiCurrent, nIdx1, nIdx2);

	htiDisceaseName = htiCurrent;
	htiBodyPartName = GetParentItem(htiCurrent);

	sBodyPartName = GetItemText(htiBodyPartName);
	sDisceaseName = GetItemText(htiDisceaseName);

	sMsg.Format("你是否真要删除部位 [ %s ]下 [ %s ]的全部 [%s]范本?", sBodyPartName, sDisceaseName, GetTitle());

	if (AfxMessageBox(sMsg, MB_YESNO | MB_ICONQUESTION) != IDYES) return;

	//  删除x范本
	sCmdText.Format("DELETE FROM %s WHERE ZHANGQI = :zhangqi and BINGZHONG = :bingzhong AND USAGETYPE = %d",
			theApp.TABLE_STANDTEXT, m_nMode);  
		

	try
	{				
		g_dbcommand.setCommandText((SAString) sCmdText);

		g_dbcommand.Param("zhangqi").setAsString()		= sBodyPartName;
		g_dbcommand.Param("bingzhong").setAsString()	= sDisceaseName;

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
	}
				
	//删除x病种
				
	sCmdText.Format("DELETE FROM %s WHERE ZHANGQI = :zhangqi AND BINGZHONG = :bingzhong AND NOT EXISTS \
			(SELECT DISTINCT ZHANGQI, BINGZHONG FROM %s WHERE ZHANGQI = :zhangqi AND BINGZHONG = :bingzhong) ", 
			theApp.TABLE_BINGZHONG, theApp.TABLE_STANDTEXT);
				
	try
	{
		g_dbcommand.setCommandText((SAString) sCmdText);

		g_dbcommand.Param("zhangqi").setAsString()		= sBodyPartName;
		g_dbcommand.Param("bingzhong").setAsString()	= sDisceaseName;
			
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
	}

	DeleteItem(htiCurrent);
}

void CCannedTextTree::OnTreeAddSample()
{
	HTREEITEM hItem, htiParent, hNewItem;
	int nIdx1, nIdx2;
	CString sBodyPartName, sDisceaseName, sCannedText;
	CString sCmdText;
	BOOL bExisted;
	CWnd *pWnd;
	
	theApp.ConnectDatabase();

	hItem = GetFirstSelectedItem();
	if (hItem == NULL) return;

	GetItemImage(hItem, nIdx1, nIdx2);

	if (nIdx1 != 1) return;

	sDisceaseName = GetItemText(hItem);
				
	htiParent = GetParentItem(hItem);
	sBodyPartName = GetItemText(htiParent);

	pWnd = m_pEditCtrlDlgWnd->GetDlgItem(m_nEditCtrlID);
	if (pWnd == NULL) return;

	pWnd->GetWindowText(sCannedText);
	sCannedText.TrimLeft();
	sCannedText.TrimRight();

	if (sCannedText.IsEmpty()) return;

	bExisted = FALSE;

	sCmdText.Format("SELECT ZHANGQI, BINGZHONG, FANBEN FROM %s WHERE ZHANGQI = :zhangqi AND BINGZHONG = :bingzhong AND FANBEN = :fanben AND USAGETYPE = %d", 
			theApp.TABLE_STANDTEXT, m_nMode);

	try
	{
		g_dbcommand.setCommandText((SAString) sCmdText);
		g_dbcommand.Param("zhangqi").setAsString()   = sBodyPartName;
		g_dbcommand.Param("bingzhong").setAsString() = sDisceaseName;
		g_dbcommand.Param("fanben").setAsString()    = sCannedText;
						
		g_dbcommand.Execute();
						
		if (g_dbcommand.FetchNext())
			bExisted = TRUE;
						
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

	if (bExisted) return;

	sCmdText.Format("INSERT INTO %s (ZHANGQI,BINGZHONG,FANBEN,USAGETYPE) VALUES(:zhangqi,:bingzhong,:fanben,:usagetype)" , theApp.TABLE_STANDTEXT);
	
	try
	{
		g_dbcommand.setCommandText((SAString) sCmdText);
		g_dbcommand.Param("zhangqi").setAsString()   = sBodyPartName;
		g_dbcommand.Param("bingzhong").setAsString() = sDisceaseName;
		g_dbcommand.Param("fanben").setAsString()    = sCannedText;
		g_dbcommand.Param("usagetype").setAsLong()   = m_nMode;
							
		g_dbcommand.Execute();
		g_dbconnection.Commit();

		hNewItem = InsertItem(sCannedText, nIdx1 + 1, nIdx1 + 1, hItem);
		SetItemData(hNewItem, m_nMode);
		RedrawWindow();
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


void CCannedTextTree::OnTreeDelSample()
{
	CString sCmdText, sMsg;
	CString sBodyPartName, sDisceaseName,  sCannedText;
	HTREEITEM htiCurrent, htiBodyPartName, htiDisceaseName, htiCannedText;
	int nIdx1 = 0, nIdx2 = 0;

	theApp.ConnectDatabase();

	htiCurrent = GetFirstSelectedItem();
	if (htiCurrent == NULL) return;

	GetItemImage(htiCurrent, nIdx1, nIdx2);

	// 删除x范本 
						
	htiCannedText	= htiCurrent;
	htiDisceaseName = GetParentItem(htiCurrent);
	htiBodyPartName	= GetParentItem(htiDisceaseName);

	sBodyPartName	= GetItemText(htiBodyPartName);
	sDisceaseName	= GetItemText(htiDisceaseName);
	sCannedText		= GetItemText(htiCannedText);
				
	sMsg.Format("你是否真要删除部位 [ %s ]下[ %s ]的当前所选择的 [%s]范本?", sBodyPartName, sDisceaseName, GetTitle());

	if (AfxMessageBox(sMsg, MB_YESNO | MB_ICONQUESTION) != IDYES) return;

			
	sCmdText.Format("DELETE FROM %s WHERE ZHANGQI = :zhangqi AND BINGZHONG = :bingzhong AND FANBEN = :fanben AND USAGETYPE = %d", 
			theApp.TABLE_STANDTEXT, m_nMode);

	try
	{
		g_dbcommand.setCommandText((SAString) sCmdText);

		g_dbcommand.Param("zhangqi").setAsString()		= sBodyPartName;
		g_dbcommand.Param("bingzhong").setAsString()	= sDisceaseName;
		g_dbcommand.Param("fanben").setAsString()		= sCannedText;

		g_dbcommand.Execute();
		g_dbconnection.Commit();

		DeleteItem(htiCurrent);
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

void CCannedTextTree::OnTreeUpdateSample()
{
	theApp.ConnectDatabase();

	HTREEITEM hItem = GetFirstSelectedItem();

	if (hItem == NULL) return;
	int i,j;
	GetItemImage(hItem, i, j);
	if(i != 2)  return;

	HTREEITEM hti = GetParentItem(hItem);
	CString bzname = GetItemText(hti);

	hti = GetParentItem(hti);
	CString zqname = GetItemText(hti);

	int count = 0;
	hti = GetPrevSiblingItem(hItem);
	while(hti != NULL)
	{
		count++;
		hti = GetPrevSiblingItem(hti);
	}

	CString fanben;  fanben.Empty();

	try
	{
		SAString cmdstr = "Select fanben from " + theApp.TABLE_STANDTEXT + " where zhangqi = :zhangqi and bingzhong = :bingzhong and usagetype = :usagetype";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("zhangqi").setAsString()   = zqname;
		g_dbcommand.Param("bingzhong").setAsString() = bzname;
		g_dbcommand.Param("usagetype").setAsLong()   = m_nMode;
		g_dbcommand.Execute();
			
		i = 0;
		while( g_dbcommand.FetchNext() )
		{
			if(count == i)
			{
				fanben = g_dbcommand.Field("fanben").asString();
				break;
			}
			i++;
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

	CString fb; fb.Empty();

	GetDlgItemText(m_nEditCtrlID, fb);

	fb.TrimLeft();  fb.TrimRight();
	
	if (fb.IsEmpty()) return;
	try
	{
		SAString cmdstr = "Update " + theApp.TABLE_STANDTEXT + " set fanben = :fanben1 where zhangqi = :zhangqi and bingzhong = :bingzhong and fanben = :fanben and usagetype = :usagetype";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("zhangqi").setAsString()   = zqname;
		g_dbcommand.Param("bingzhong").setAsString() = bzname;
		g_dbcommand.Param("fanben").setAsString()    = fanben;
		g_dbcommand.Param("usagetype").setAsLong()   = m_nMode;
				
		g_dbcommand.Param("fanben1").setAsString()    = fb;
				
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
	}
}

void CCannedTextTree::OnTreeMoveUpBodyPart()
{
	int nIdx1, nIdx2;
	HTREEITEM hItem = GetFirstSelectedItem();
	
	if ( hItem == NULL) return;

	GetItemImage(hItem, nIdx1, nIdx2);

	if (nIdx1 != 0) return;

	CString zqname = GetItemText(hItem);

	HTREEITEM hPreviousItem = GetNextItem(hItem, TVGN_PREVIOUS);
	if (hPreviousItem == NULL) return;
	
	CString zqname_previous = GetItemText(hPreviousItem);

	try
	{
		SAString cmdstr = "Update " + (SAString)theApp.TABLE_ZHANGQI + " set zhangqi='123456789' where zhangqi = '" + (SAString)zqname + "'";
		g_dbcommand.setCommandText(cmdstr);
						
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
	}
					
	try
	{
		SAString cmdstr = "Update " + (SAString)theApp.TABLE_ZHANGQI + " set zhangqi='" + (SAString)zqname + "' where zhangqi = '" + (SAString)zqname_previous + "'";
		g_dbcommand.setCommandText(cmdstr);
						
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
	}
					
	try
	{
		SAString cmdstr = "Update " + (SAString)theApp.TABLE_ZHANGQI + " set zhangqi='" + (SAString)zqname_previous + "' where zhangqi = '123456789'";
		g_dbcommand.setCommandText(cmdstr);
						
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
	}
	
	BuildTree();
}

void CCannedTextTree::OnTreeMoveDnBodyPart()
{
	int nIdx1, nIdx2;
	HTREEITEM hItem = GetFirstSelectedItem();
	
	if ( hItem == NULL) return;

	GetItemImage(hItem, nIdx1, nIdx2);

	if (nIdx1 != 0) return;


	CString zqname = GetItemText(hItem);

	HTREEITEM hNextItem = GetNextItem(hItem, TVGN_NEXT);
	if (hNextItem == NULL) return;
					
	CString zqname_next = GetItemText(hNextItem);

	try
	{
		SAString cmdstr = "Update " + (SAString)theApp.TABLE_ZHANGQI + " set zhangqi='123456789' where zhangqi = '" + (SAString)zqname + "'";
		g_dbcommand.setCommandText(cmdstr);
						
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
	}
					
	try
	{
		SAString cmdstr = "Update " + (SAString)theApp.TABLE_ZHANGQI + " set zhangqi='" + (SAString)zqname + "' where zhangqi = '" + (SAString)zqname_next + "'";
		g_dbcommand.setCommandText(cmdstr);
						
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
	}
					
	try
	{
		SAString cmdstr = "Update " + (SAString)theApp.TABLE_ZHANGQI + " set zhangqi='" + (SAString)zqname_next + "' where zhangqi = '123456789'";
		g_dbcommand.setCommandText(cmdstr);
						
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
	}

	BuildTree();
}


void CCannedTextTree::OnTreePickupCannedText()
{
	HTREEITEM hItem;

	hItem = GetFirstSelectedItem();

	TransferCannedText2Editor(hItem);
}

/////////////////////////////////////////////////////////////////////
