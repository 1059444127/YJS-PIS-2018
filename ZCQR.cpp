// ZCQR.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "ZCQR.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZCQR dialog
extern CPathologyApp theApp;

CZCQR::CZCQR(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CZCQR::IDD, pParent)
{
	//{{AFX_DATA_INIT(CZCQR)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CZCQR::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZCQR)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNCREATE(CZCQR, CXTResizeDialog)

BEGIN_MESSAGE_MAP(CZCQR, CDialog)
//{{AFX_MSG_MAP(CZCQR)
ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
ON_BN_CLICKED(IDC_BUTTON_SEARCH2, OnButtonDefaultSearch)
ON_BN_CLICKED(IDC_BUTTON_CONFIRM, OnButtonConfirm)
	ON_BN_CLICKED(IDC_BUTTON_CONFIRM2, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_YZQR, OnButtonYzqr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZCQR message handlers

BOOL CZCQR::OnInitDialog(){
	
	CXTResizeDialog::OnInitDialog();
	
	m_grid.AttachGrid(this,IDC_STATIC_ZCLIST);
	m_grid.SetParent(this);
	m_grid.ResetGridSize();
	m_grid.RedrawAll();
	
	SetResize(IDC_STATIC_BLH, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_BLH, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_BRID,SZ_TOP_LEFT,SZ_TOP_LEFT);
	SetResize(IDC_EDIT_BRID,SZ_TOP_LEFT,SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_SEARCH,SZ_TOP_LEFT,SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_SEARCH2,SZ_TOP_LEFT,SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_CONFIRM,SZ_TOP_LEFT,SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_CONFIRM2,SZ_TOP_LEFT,SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_YZQR,SZ_TOP_LEFT,SZ_TOP_LEFT);
	SetResize(IDC_STATIC_ZCLIST, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	
	return TRUE;
}

void CZCQR::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	m_grid.ClearSelections();
	CString blh,brid;
	GetDlgItemText(IDC_EDIT_BLH,blh);
	GetDlgItemText(IDC_EDIT_BRID,brid);
	if(blh.IsEmpty()){
		if(!brid.IsEmpty()){
			m_grid.m_WhereBetween_Cmd = " where brid ='"+brid+"'";
		}
	} else {
		m_grid.m_WhereBetween_Cmd = " where medicalno ='"+blh+"'";
	}
	m_grid.ResetGridSize();
	m_grid.RedrawAll();
	m_grid.RedrawAll();
	SetDlgItemText(IDC_EDIT_BLH,"");
}

void CZCQR::OnButtonDefaultSearch() 
{
	// TODO: Add your control notification handler code here
	m_grid.ClearSelections();
	m_grid.m_WhereBetween_Cmd = "";
	m_grid.ResetGridSize();
	m_grid.RedrawAll();
	m_grid.RedrawAll();
}

void CZCQR::OnButtonConfirm() 
{
	// TODO: Add your control notification handler code here
	if(AfxMessageBox("将进行收费确认操作，您确认？",MB_OKCANCEL) == IDCANCEL){
		return;
	}
	CStringArray medicalno_list;
	int nCol,nRet;
	long nRow;
	
	nRet = m_grid.EnumFirstSelected(&nCol,&nRow);
	while( nRet == UG_SUCCESS){
		CString mno;
		CUGCell cell;
		m_grid.GetCellIndirect(0,nRow,&cell);
		cell.GetText(&mno);
		medicalno_list.Add(mno);
		nRet = m_grid.EnumNextSelected(&nCol,&nRow);
	}
	for(int i = 0 ; i < medicalno_list.GetSize() - 1; i++){
		try{
			SAString cmdstr = "update "+ theApp.TABLE_ZC + " set zyh='已收费' where medicalno=:medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = medicalno_list.GetAt(i);
			g_dbcommand.Execute();
			g_dbconnection.Commit();
		} catch (SAException &e){
			AfxMessageBox((const char*)e.ErrText());
		}
	}
	m_grid.ResetGridSize();
	m_grid.RedrawAll();
	m_grid.RedrawAll();
}

int CZCQR::PreTranslateMessage(MSG* pMsg){
	return NULL;
}

void CZCQR::OnButtonDelete() 
{
	AfxMessageBox("将进行删除操作，确认？",MB_OKCANCEL);
	CStringArray medicalno_list;
	int nCol,nRet;
	long nRow;

	nRet = m_grid.EnumFirstSelected(&nCol,&nRow);
	while( nRet == UG_SUCCESS){
		CString mno;
		CUGCell cell;
		m_grid.GetCellIndirect(0,nRow,&cell);
		cell.GetText(&mno);
		medicalno_list.Add(mno);
		nRet = m_grid.EnumNextSelected(&nCol,&nRow);
	}

	for(int i = 0 ; i < medicalno_list.GetSize() - 1; i++){
		try{
			SAString cmdstr = "delete from "+ theApp.TABLE_ZC + " where medicalno=:medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = medicalno_list.GetAt(i);
			g_dbcommand.Execute();
			g_dbconnection.Commit();
		} catch (SAException &e){
			AfxMessageBox((const char*)e.ErrText());
		}
		
	}
	m_grid.ResetGridSize();
	m_grid.RedrawAll();
	m_grid.RedrawAll();
}

CString CZCQR::GenNewAdviceNo(CString blh)
{
	theApp.ConnectDatabase();
	
	//CString nBlh = blh + "-001";
	CString nBlh = blh + "-01";
	//========== Generate proper medicalno automatically ===============
	try
	{
		SAString cmdstr = "Select yzh from " + theApp.TABLE_ADVICE + " where medicalno = :medicalno order by yzh desc";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = blh;
		
		g_dbcommand.Execute();
		
		if( g_dbcommand.FetchNext() )
		{
			CString curno = g_dbcommand.Field("yzh").asString();
			
			CString str,str1;
			char strbuf[300];
			int Len=curno.GetLength(),k;
			BYTE ch;
			for(k=(Len-1);k>=0;k--)
			{
				ch = curno.GetAt(k); 
				if(!(ch >= 0x30 && ch <= 0x39))  break;
			}
			k = Len-1-k;
			ltoa(atol(curno.Right(k))+1,strbuf,10);
			str1.Format("%s",strbuf);
			str = curno.Left(Len-k);
			Len = k - str1.GetLength();
			for(k=0;k<Len;k++)  str += "0";
			str += str1;
			nBlh = str;
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
	
	return nBlh;
}

void CZCQR::OnButtonYzqr() 
{
	// TODO: Add your control notification handler code here
	CStringArray medicalno_list,yzlx_list,lkh_list,ktxx_list,sfzt_list,bz_list,sqys_list;
	int nCol,nRet;
	long nRow;
	nRet = m_grid.EnumFirstSelected(&nCol,&nRow);
		CString mno;
		CUGCell cell;
		m_grid.GetCellIndirect(0,nRow,&cell);
		cell.GetText(&mno);		
		medicalno_list.Add(mno);
		m_grid.GetCellIndirect(3,nRow,&cell);
		cell.GetText(&mno);
		yzlx_list.Add(mno);
		m_grid.GetCellIndirect(4,nRow,&cell);
		cell.GetText(&mno);
		lkh_list.Add(mno);
		m_grid.GetCellIndirect(5,nRow,&cell);
		cell.GetText(&mno);
		ktxx_list.Add(mno);
		m_grid.GetCellIndirect(7,nRow,&cell);
		cell.GetText(&mno);
		sfzt_list.Add(mno);
		m_grid.GetCellIndirect(9,nRow,&cell);
		cell.GetText(&mno);
		bz_list.Add(mno);
		m_grid.GetCellIndirect(8,nRow,&cell);
		cell.GetText(&mno);
		sqys_list.Add(mno);
		//nRet = m_grid.EnumNextSelected(&nCol,&nRow);
		if(sfzt_list.GetAt(0).IsEmpty() || sfzt_list.GetAt(0).Compare("未收费") == 0){
			AfxMessageBox("请先进行收费确认!");
			return;
		}
	SADateTime ServerTime = theApp.GetServerDateTime();
	
	for(int i=0; i < medicalno_list.GetSize(); i++){
		if(ktxx_list.GetAt(i).IsEmpty()) return;
		
		CString temp,ktxx; int index;
		ktxx = ktxx_list.GetAt(i);
		while((index = ktxx.Find(',')) >= 0){
			temp = ktxx.Left(index);
			CString yzh = GenNewAdviceNo(medicalno_list.GetAt(i));
			try{
				SAString cmdstr = "insert into " + theApp.TABLE_ADVICE + " (medicalno,yzh,yzlx,lkh,bjw,qps,sqys,sqrq,yzzt,fxjg,bz) values(:medicalno,:yzh,:yzlx,:lkh,:bjw,:qps,:sqys,:sqrq,0,'已收费',:bz)";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("medicalno").setAsString() = medicalno_list.GetAt(i);
				g_dbcommand.Param("yzh").setAsString() = yzh;
				g_dbcommand.Param("yzlx").setAsString() = yzlx_list.GetAt(i);
				g_dbcommand.Param("lkh").setAsString() = lkh_list.GetAt(i);
				g_dbcommand.Param("bjw").setAsString() = temp;
				g_dbcommand.Param("qps").setAsLong() = 1;
				g_dbcommand.Param("sqys").setAsString() = sqys_list.GetAt(i);
				g_dbcommand.Param("sqrq").setAsDateTime() = ServerTime;
				g_dbcommand.Param("bz").setAsString() = bz_list.GetAt(i);
				g_dbcommand.Execute();
				g_dbconnection.Commit();
			} catch (SAException &e){
				try{
					g_dbconnection.Rollback();
				} catch (SAException &e){
					
				}
				AfxMessageBox((const char*)e.ErrText());
			}
			ktxx = ktxx.Right(ktxx.GetLength() - index - 1);
		}
		try{
			SAString cmdStr = "delete from "+ theApp.TABLE_ZC + " where medicalno = '" + medicalno_list.GetAt(i) +"'";
			g_dbcommand.setCommandText(cmdStr);
			g_dbcommand.Execute();
			g_dbconnection.Commit();
		} catch (SAException &s) {
			try{
				g_dbconnection.Rollback();
			}catch(SAException &s){
				
			}
		}
		m_grid.ResetGridSize();
		m_grid.RedrawAll();
		m_grid.RedrawAll();
    	remove(theApp.m_Exe_Working_Dir + "system\\" + medicalno_list.GetAt(i) + ".tj");
	}

	
}
