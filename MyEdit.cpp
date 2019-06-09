// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "MyEdit.h"
#include "resource.h"
#include "PathologyView.h"
#include "PathologyDoc.h"
#include "Input.h"
#include "HistoryShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyEdit

CMyEdit::CMyEdit()
{
}

CMyEdit::~CMyEdit()
{
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CMyEdit)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_COPY_GJSJ, OnCopyGjsj)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_BDJG, OnBdjg)
	ON_COMMAND(ID_BBMC, OnBbmc)
	ON_COMMAND(ID_COPY_RYSJ, OnCopyRysj)
	ON_COMMAND(ID_COPY_ZDYJ, OnCopyZdyj)
	ON_COMMAND(ID_INSERT_BSZY, OnInsertBszy)
	ON_COMMAND(ID_INSERT_RYSJ, OnInsertRysj)
	ON_COMMAND(ID_INSERT_ZDYJ, OnInsertZdyj)
	ON_COMMAND(ID_EDIT_CUT,OnCut)
	ON_COMMAND(ID_EDIT_COPY,OnCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEdit message handlers
extern CPathologyApp theApp;

void CMyEdit::OnContextMenu(CWnd*, CPoint point)
{

	// CG: This block was added by the Pop-up Menu component
	{
		if (point.x == -1 && point.y == -1){
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

		CMenu menu;
		VERIFY(menu.LoadMenu(IDM_MY_EDIT));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
	}
}

void CMyEdit::OnCut(){
	
	CEdit::Cut();
}

void CMyEdit::OnCopy(){
	CEdit::Copy();
}

void CMyEdit::OnEditPaste() 
{
	// TODO: Add your command handler code here
	CEdit::Paste();
}

//DEL void CMyEdit::OnRButtonDblClk(UINT nFlags, CPoint point) 
//DEL {
//DEL 	// TODO: Add your message handler code here and/or call default
//DEL 	
//DEL 	CEdit::OnRButtonDblClk(nFlags, point);
//DEL }

void CMyEdit::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (point.x == -1 && point.y == -1){
		//keystroke invocation
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);
		
		point = rect.TopLeft();
		point.Offset(5, 5);
	}

	if(GetParent()->IsKindOf(RUNTIME_CLASS(CHistoryShow))){
		CMenu menu;
		VERIFY(menu.LoadMenu(IDM_MY_EDIT));

		CMenu* pPopup = menu.GetSubMenu(1);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		ClientToScreen((tagPOINT*)&point);
		
		int start,end; 
		GetSel(start,end);
		if(start == end) {
			pPopup->EnableMenuItem(ID_EDIT_CUT,MF_GRAYED);
			pPopup->EnableMenuItem(ID_EDIT_COPY,MF_GRAYED);
		} 
		
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
		
		CEdit::OnRButtonDown(nFlags, point);
		return;
	}

	CMenu menu;
	VERIFY(menu.LoadMenu(IDM_MY_EDIT));
	
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;
	
//	while (pWndPopupOwner->GetStyle() & WS_CHILD)
//		pWndPopupOwner = pWndPopupOwner->GetParent();
	ClientToScreen((tagPOINT*)&point);

	int start,end; 
	GetSel(start,end);
	if(start == end) 
	{
		pPopup->EnableMenuItem(ID_EDIT_CUT,MF_GRAYED);
		pPopup->EnableMenuItem(ID_EDIT_COPY,MF_GRAYED);
	} 
	
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
	
	
	CEdit::OnRButtonDown(nFlags, point);
}

void CMyEdit::OnBdjg() 
{
	// TODO: Add your command handler code here
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		AfxMessageBox("本记录已经锁定,无法进行修改操作.");
		return;
	}

	CString bdjg;
	try{
		SAString cmdstr = "select bdjg from " + theApp.TABLE_MAIN + " where medicalno=:medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		while(g_dbcommand.FetchNext()){
			bdjg = g_dbcommand.Field("bdjg").asString();
		}
	}catch(SAException &e){
		AfxMessageBox((const char*)e.ErrText());
	}
	
	if(bdjg.IsEmpty()){
		if(AfxMessageBox("提示：冰冻结果为空，插入将并无任何改变，是否继续？", MB_YESNO) == IDNO){
			return;
		}
		ReplaceSel(bdjg);
		return;
	}
	ReplaceSel(bdjg,TRUE);
}

void CMyEdit::OnBbmc() 
{
	// TODO: Add your command handler code here
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		AfxMessageBox("本记录已经锁定,无法进行修改操作.");
		return;
	}
	CString bbmc;
	try
	{
		SAString cmdstr = "select bbmc from " + theApp.TABLE_MAIN + " where medicalno=:medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		while(g_dbcommand.FetchNext())
		{
			bbmc = g_dbcommand.Field("bbmc").asString();
		}
	}
	catch(SAException &e)
	{
		AfxMessageBox((const char*)e.ErrText());
	}

	if(bbmc.IsEmpty())
	{
		if(AfxMessageBox("提示：标本名称为空，插入将并无任何改变，是否继续？", MB_YESNO) == IDNO)
		{
			return;
		}
		ReplaceSel(bbmc);
		return;
	}
	bbmc = "（" + bbmc; 
	bbmc = bbmc + "）";
	ReplaceSel(bbmc,TRUE);
}

void CMyEdit::OnCopyGjsj() 
{
	// TODO: Add your command handler code here
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		AfxMessageBox("本记录已经锁定,无法进行修改操作.");
		return;
	}
	CInput dlg; CString blh;
	if(dlg.DoModal() == IDOK){
		blh = dlg.m_blh; CString gjsj = "gjsj";
		try{
			SAString cmdstr = "select gjsj from "+theApp.TABLE_MAIN + " where medicalno=:medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = blh;
			g_dbcommand.Execute();
			while(g_dbcommand.FetchNext()){
				gjsj = g_dbcommand.Field("gjsj").asString();
			}
			g_dbconnection.Commit();
		} catch (SAException &e){
			AfxMessageBox((const char*)e.ErrText());
		}
		//AfxMessageBox(gjsj);
		if(gjsj.CompareNoCase("gjsj") == 0){
			AfxMessageBox("不存在相应编号的记录!");
			return;
		} else if(gjsj.IsEmpty()){
			if(AfxMessageBox("提示：光景所见为空，插入将并无任何改变，是否继续？", MB_YESNO) == IDNO){
				return;
			}
			ReplaceSel(gjsj,TRUE);
		} else {
			ReplaceSel(gjsj,TRUE);
		}
	}
}

void CMyEdit::OnCopyRysj() 
{
	// TODO: Add your command handler code here
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		AfxMessageBox("本记录已经锁定,无法进行修改操作.");
		return;
	}
	CInput dlg; CString blh;
	if(dlg.DoModal() == IDOK){
		blh = dlg.m_blh; CString rysj = "rysj";
		try{
			SAString cmdstr = "select rysj from "+theApp.TABLE_MAIN + " where medicalno=:medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = blh;
			g_dbcommand.Execute();
			while(g_dbcommand.FetchNext()){
				rysj = g_dbcommand.Field("rysj").asString();
			}
			g_dbconnection.Commit();
		} catch (SAException &e){
			AfxMessageBox((const char*)e.ErrText());
		}
		//AfxMessageBox(gjsj);
		if(rysj.CompareNoCase("rysj") == 0){
			AfxMessageBox("不存在相应编号的记录!");
			return;
		} else if(rysj.IsEmpty()){
			if(AfxMessageBox("提示：肉眼所见为空，插入将并无任何改变，是否继续？", MB_YESNO) == IDNO){
				return;
			}
			ReplaceSel(rysj,TRUE);
		} else {
			ReplaceSel(rysj,TRUE);
		}
	}
}

void CMyEdit::OnCopyZdyj() 
{
	// TODO: Add your command handler code here
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		AfxMessageBox("本记录已经锁定,无法进行修改操作.");
		return;
	}
	CInput dlg; CString blh;
	if(dlg.DoModal() == IDOK){
		blh = dlg.m_blh; CString zdyj = "zdyj";
		try{
			SAString cmdstr = "select zdyj from "+theApp.TABLE_MAIN + " where medicalno=:medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = blh;
			g_dbcommand.Execute();
			while(g_dbcommand.FetchNext()){
				zdyj = g_dbcommand.Field("zdyj").asString();
			}
			g_dbconnection.Commit();
		} catch (SAException &e){
			AfxMessageBox((const char*)e.ErrText());
		}
		//AfxMessageBox(gjsj);
		if(zdyj.CompareNoCase("zdyj") == 0){
			AfxMessageBox("不存在相应编号的记录!");
			return;
		} else if(zdyj.IsEmpty()){
			if(AfxMessageBox("提示：诊断意见为空，插入将并无任何改变，是否继续？", MB_YESNO) == IDNO){
				return;
			}
			ReplaceSel(zdyj,TRUE);
		} else {
			ReplaceSel(zdyj,TRUE);
		}
	}
}

void CMyEdit::OnInsertBszy() 
{
	// TODO: Add your command handler code here
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		AfxMessageBox("本记录已经锁定,无法进行修改操作.");
		return;
	}
	CString temp;
	GetWindowText(temp);
	CString old;
	
	try{
		SAString cmdstr = "select bszy from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		while(g_dbcommand.FetchNext()){
			old = g_dbcommand.Field("bszy").asString();
		}
		g_dbconnection.Commit();
		//AfxMessageBox("~~~");
		temp = old + "\r\n" + temp;
		cmdstr = "update "+theApp.TABLE_MAIN + " set bszy = '"+temp+"' where medicalno =:medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		g_dbconnection.Commit();
	} catch (SAException &e){
		AfxMessageBox((const char*)e.ErrText());
	}
	AfxMessageBox("当前信息已成功插入!");
	//theApp.OnAppRefresh();
}

void CMyEdit::OnInsertRysj() 
{
	// TODO: Add your command handler code here
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		AfxMessageBox("本记录已经锁定,无法进行修改操作.");
		return;
	}
	CString temp;
	GetWindowText(temp);
	CString old;
	try{
		SAString cmdstr = "select rysj from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		while(g_dbcommand.FetchNext()){
			old = g_dbcommand.Field("rysj").asString();
		}
		g_dbconnection.Commit();
		temp = old + "\r\n" + temp;
		cmdstr = "update "+theApp.TABLE_MAIN + " set rysj = '"+temp+"' where medicalno =:medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		g_dbconnection.Commit();
	} catch (SAException &e){
		AfxMessageBox((const char*)e.ErrText());
	}
	AfxMessageBox("当前信息已成功插入!");
}

void CMyEdit::OnInsertZdyj() 
{
	// TODO: Add your command handler code here
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		AfxMessageBox("本记录已经锁定,无法进行修改操作.");
		return;
	}
	CString temp;
	GetWindowText(temp);
	CString old;
	try{
		SAString cmdstr = "select zdyj from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		while(g_dbcommand.FetchNext()){
			old = g_dbcommand.Field("zdyj").asString();
		}
		g_dbconnection.Commit();
		temp = old + "\r\n" + temp;
		cmdstr = "update "+theApp.TABLE_MAIN + " set zdyj = '"+temp+"' where medicalno =:medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		g_dbconnection.Commit();
	} catch (SAException &e){
		AfxMessageBox((const char*)e.ErrText());
	}
	AfxMessageBox("当前信息已成功插入!");
}

//DEL void CMyEdit::OnChange() 
//DEL {
//DEL 	// TODO: If this is a RICHEDIT control, the control will not
//DEL 	// send this notification unless you override the CEdit::OnInitDialog()
//DEL 	// function and call CRichEditCtrl().SetEventMask()
//DEL 	// with the ENM_CHANGE flag ORed into the mask.
//DEL 	
//DEL 	// TODO: Add your control notification handler code here
//DEL 	
//DEL }
