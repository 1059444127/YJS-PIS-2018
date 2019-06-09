// Bpsmhd.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "Bpsmhd.h"
#include "MedSlice.h"
#include "Qppj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBpsmhd dialog
extern CPathologyApp theApp;

CBpsmhd::CBpsmhd(CWnd* pParent /*=NULL*/)
	: CDialog(CBpsmhd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBpsmhd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = (CMedSlice *)pParent;
}


void CBpsmhd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBpsmhd)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBpsmhd, CDialog)
	//{{AFX_MSG_MAP(CBpsmhd)
	ON_BN_CLICKED(IDCONFORM, OnConform)
	ON_EN_CHANGE(IDC_EDIT_CODE, OnChangeEditCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBpsmhd message handlers
BOOL CBpsmhd::PreTranslateMessage(MSG* pMsg) 
{
	if( m_hWnd && pMsg->wParam == VK_RETURN)
	{
		OnConform();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CBpsmhd::OnConform()  
{
	
	if(((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck() == 1){
		 
		CString code;
		GetDlgItemText(IDC_EDIT_CODE, code);
		code.TrimLeft();  code.TrimRight();
		
		if(!code.IsEmpty())
		{
			SADateTime ServerTime = theApp.GetServerDateTime();
			
			theApp.ConnectDatabase();
			
			try
			{
				SAString cmdstr = "Update " + theApp.TABLE_SLICE + " set bphd = :bphd, hdrq = :hdrq where qptmh = :qptmh";
				g_dbcommand.setCommandText(cmdstr);
				g_dbcommand.Param("qptmh").setAsString()     = code;
				g_dbcommand.Param("bphd").setAsString()      = theApp.m_Cpp_User;  
				
				SADateTime st = ServerTime;
				g_dbcommand.Param("hdrq").setAsDateTime()    = st;  
				
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
				return;
			}
			
			if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedSlice)))
			{
				m_pParent->Refresh();
			}
		}
		
		m_CodeString.Empty();
		SetDlgItemText(IDC_EDIT_CODE, "");
    	((CEdit *)GetDlgItem(IDC_EDIT_CODE))->SetFocus();

	} 
	else if (((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck() == 1)
	{
		if(AfxMessageBox("该操作不可逆，您是否要进行切片核对确认？",MB_OKCANCEL) == IDCANCEL) return;
		if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedSlice)))
		{
			int nRet; int col; long row;
			CStringArray qptmhList,sqysList,mnoList;
			CUIntArray  rowlist;
			nRet = m_pParent->m_SliceList.EnumFirstSelected(&col,&row);
			while(nRet == UG_SUCCESS){
				if(row >= 0)
				{
					int rowcount = rowlist.GetSize();
					for(int i = 0; i < rowcount; i ++ )
					{
						if( row == rowlist.GetAt(i) )  break;
					}
					if( i >= rowcount )
					{
						CString qptmh;
						
						CUGCell  cell;
						m_pParent->m_SliceList.GetCellIndirect(4,row,&cell);
						cell.GetText(&qptmh);
						rowlist.Add(row);
						qptmhList.Add(qptmh);
						sqysList.Add(m_pParent->m_SliceList.QuickGetText(5,row));
						mnoList.Add(m_pParent->m_SliceList.QuickGetText(0,row));
					}
				}
				nRet = m_pParent->m_SliceList.EnumNextSelected(&col,&row);
			}

			int curnum = m_pParent->m_SliceList.GetCurrentRow();
			for(int i = 0; i < rowlist.GetSize(); i++)
			{
				m_pParent->m_SliceList.GotoRow(rowlist.GetAt(i));
			}
			m_pParent->m_SliceList.GotoRow(curnum);
			
			rowlist.RemoveAll();
			
			if( qptmhList.GetSize() <= 0 )
			{
				return;
			}
			
	        SADateTime ServerTime = theApp.GetServerDateTime();
			
			for(int index = 0; index < qptmhList.GetSize(); index++)
			{
				
				try
				{
					SAString cmdstr = "Update " + theApp.TABLE_SLICE + " set bphd = :bphd, hdrq = :hdrq where qptmh = :qptmh";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("qptmh").setAsString()     = qptmhList.GetAt(index);
					g_dbcommand.Param("bphd").setAsString()      = theApp.m_Cpp_User;  
					
					SADateTime st = ServerTime;
					g_dbcommand.Param("hdrq").setAsDateTime()    = ServerTime;  
					
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
					return;
				}
			}

		/*	CString ly;
			ly.Format("病理号%s的特检医嘱申请已执行完成!",);
			try
			{
				SAString cmdstr = "insert"
			}
			catch (SAException& e)
			{
				
			}
		*/	
			qptmhList.RemoveAll();
			
			m_pParent->m_SliceList.ResetGridSize();
        	m_pParent->m_SliceList.RedrawAll();
			OnOK();
		} 
		else 
		{
			CQppj* m_pParent = (CQppj *)GetParent();
			int nRet; int col; long row;
			CStringArray qptmhList;
			CUIntArray  rowlist;
			nRet = m_pParent->m_SliceList.EnumFirstSelected(&col,&row);
			while(nRet == UG_SUCCESS){
				if(row >= 0)
				{
					int rowcount = rowlist.GetSize();
					for(int i = 0; i < rowcount; i ++ )
					{
						if( row == rowlist.GetAt(i) )  break;
					}
					if( i >= rowcount )
					{
						CString qptmh;
						
						CUGCell  cell;
						m_pParent->m_SliceList.GetCellIndirect(3,row,&cell);
						cell.GetText(&qptmh);
						rowlist.Add(row);
						qptmhList.Add(qptmh);
					}
				}
				nRet = m_pParent->m_SliceList.EnumNextSelected(&col,&row);
			}
			
			int curnum = m_pParent->m_SliceList.GetCurrentRow();
			for(int i = 0; i < rowlist.GetSize(); i++)
			{
				m_pParent->m_SliceList.GotoRow(rowlist.GetAt(i));
			}
			m_pParent->m_SliceList.GotoRow(curnum);
			
			rowlist.RemoveAll();
			
			if( qptmhList.GetSize() <= 0 )
			{
				return;
			}
			
			SADateTime ServerTime = theApp.GetServerDateTime();
			for(int index = 0; index < qptmhList.GetSize(); index++)
			{
				try
				{
					SAString cmdstr = "Update " + theApp.TABLE_SLICE + " set bphd = :bphd, hdrq = :hdrq where qptmh = :qptmh";
					g_dbcommand.setCommandText(cmdstr);
					g_dbcommand.Param("qptmh").setAsString()     = qptmhList.GetAt(index);
					g_dbcommand.Param("bphd").setAsString()      = theApp.m_Cpp_User;  
					
					SADateTime st = ServerTime;
					g_dbcommand.Param("hdrq").setAsDateTime()    = ServerTime;  
					
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
					return;
				}
			}
			
			qptmhList.RemoveAll();
			
			m_pParent->m_SliceList.ResetGridSize();
			m_pParent->m_SliceList.RedrawAll();
			OnOK();
		}
		
	} 
	else 
	{
		AfxMessageBox("请选择方式！");
	}
/*	CString code;
	GetDlgItemText(IDC_EDIT_CODE, code);
	code.TrimLeft();  code.TrimRight();
    
	if(!code.IsEmpty())
	{
		SADateTime ServerTime = theApp.GetServerDateTime();

		theApp.ConnectDatabase();

		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_SLICE + " set bphd = :bphd, hdrq = :hdrq where qptmh = :qptmh";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("qptmh").setAsString()     = code;
			g_dbcommand.Param("bphd").setAsString()      = theApp.m_Cpp_User;  
			
			SADateTime st = ServerTime;
			g_dbcommand.Param("hdrq").setAsDateTime()    = st;  
			
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
			return;
		}

		if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedSlice)))
		{
			m_pParent->Refresh();
		}
	}

	m_CodeString.Empty();
	SetDlgItemText(IDC_EDIT_CODE, "");
	((CEdit *)GetDlgItem(IDC_EDIT_CODE))->SetFocus(); */
}

void CBpsmhd::OnChangeEditCode() 
{
	CString str1;
	GetDlgItemText(IDC_EDIT_CODE, str1);
	str1.TrimLeft();  str1.TrimRight();
	if((str1.GetLength() - m_CodeString.GetLength()) > 3)
	{
		OnConform();
	}
	else
	{
		m_CodeString = str1;
	}
}

BOOL CBpsmhd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_CodeString.Empty();
	((CButton* )GetDlgItem(IDC_RADIO1))->SetCheck(true);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
