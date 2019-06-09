// Jsyz.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "Pathologyview.h"
#include "Pathologydoc.h"
#include "Jsyz.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJsyz dialog
extern CPathologyApp theApp;

CJsyz::CJsyz(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CJsyz::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJsyz)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CJsyz::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJsyz)
	DDX_Control(pDX, IDC_BUTTON_BLH, m_btBlh);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJsyz, CXTResizeDialog)
	//{{AFX_MSG_MAP(CJsyz)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_CBN_SELCHANGE(IDC_COMBO_YZLX, OnSelchangeComboYzlx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_BLH, OnButtonBlh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJsyz message handlers

BOOL CJsyz::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	const DWORD STYLE_ALL = BS_LEFT | BS_RIGHT | BS_CENTER;
	m_btBlh.SetBitmap(CSize(16,16),IDB_BITMAP_HISIDIN);
	m_btBlh.SetXButtonStyle(BS_XT_XPFLAT); 
	m_btBlh.ModifyStyle(STYLE_ALL & ~BS_CENTER, BS_CENTER);

	m_AdviceList.AttachGrid(this,IDC_STATIC_JSYZ);
	m_AdviceList.SetParent(this);
	m_AdviceList.ResetGridSize();
	m_AdviceList.RedrawAll();
	
	SetResize(IDC_STATIC_JSYZ, SZ_TOP_LEFT ,  SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC_YZLX, SZ_BOTTOM_LEFT ,  SZ_BOTTOM_LEFT);
	SetResize(IDC_COMBO_YZLX, SZ_BOTTOM_LEFT ,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_LKH, SZ_BOTTOM_LEFT ,  SZ_BOTTOM_LEFT);
	SetResize(IDC_COMBO_LKH, SZ_BOTTOM_LEFT ,  SZ_BOTTOM_LEFT);

	SetResize(IDC_STATIC_QPS, SZ_BOTTOM_LEFT ,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_QPS, SZ_BOTTOM_LEFT ,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_BZ, SZ_BOTTOM_LEFT ,  SZ_BOTTOM_LEFT);
	SetResize(IDC_COMBO_BZ, SZ_BOTTOM_LEFT ,  SZ_BOTTOM_LEFT);

	SetResize(IDC_BUTTON_ADD, SZ_BOTTOM_RIGHT ,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_DELETE, SZ_BOTTOM_RIGHT ,  SZ_BOTTOM_RIGHT);

	((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->AddString("常规");
	((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->AddString("补取");
	((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->AddString("重切");
	((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->AddString("深切");
	((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->AddString("薄切");
	((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->AddString("连切");
	((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->AddString("切全");
	((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->AddString("不削连切");

	((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->SetCurSel(0);

	SetDlgItemText(IDC_EDIT_QPS, "1");

	theApp.ConnectDatabase();
	try
	{
		SAString cmdstr = "Select lkh from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno and zpzt >= 2 order by lkh asc";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		while(g_dbcommand.FetchNext())
		{
			((CComboBox *)GetDlgItem(IDC_COMBO_LKH))->AddString(g_dbcommand.Field("lkh").asString());
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
		return FALSE;
	}

	try
	{
		SAString cmdstr = "Select qcbw from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno order by qcrq asc";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		int index = 0;
		while( g_dbcommand.FetchNext() )
		{
			m_QcbwList.Add(g_dbcommand.Field("qcbw").asString());
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

	OnSelchangeComboYzlx();

	//---------------------------------------------------
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
	}
	//---------------------------------------------------
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CJsyz::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty()) return;

	CString yzlx,lkh,bz;
	GetDlgItemText(IDC_COMBO_YZLX, yzlx);    yzlx.TrimLeft();  yzlx.TrimRight();
	GetDlgItemText(IDC_COMBO_LKH, lkh);      lkh.TrimLeft();   lkh.TrimRight();
	int qps = GetDlgItemInt(IDC_EDIT_QPS);
	GetDlgItemText(IDC_COMBO_BZ, bz);         bz.TrimLeft();    bz.TrimRight();

	if(yzlx.IsEmpty() || (yzlx.CompareNoCase("补取") == 0 && bz.IsEmpty()) || (yzlx.CompareNoCase("补取") && (qps <= 0 || lkh.IsEmpty())))
	{
		AfxMessageBox("输入了无效的数据,请检查输入值的有效性!");
		return;
	}

	if(yzlx.CompareNoCase("补取") == 0)
	{
		CString nQcfy;
		try
		{
			SAString cmdstr = "Select qcfy from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Execute();
			
			int index = 0;
			if( g_dbcommand.FetchNext() )
			{
				nQcfy = g_dbcommand.Field("qcfy").asString();
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

		if(nQcfy.CompareNoCase("已用完") == 0)
		{
			AfxMessageBox("标本已用完！");
			return;
		}
	}

	SADateTime ServerTime = theApp.GetServerDateTime();
    CString yzh = GenNewAdviceNo(theApp.GetView()->GetDocument()->m_blh);
	try
	{
		SAString cmdstr = "Insert into " + theApp.TABLE_ADVICE + " (medicalno,yzh,yzlx,lkh,qps,sqys,sqrq,yzzt,bz) values(:medicalno,:yzh,:yzlx,:lkh,:qps,:sqys,:sqrq,0,:bz)";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Param("yzh").setAsString() = yzh;
		g_dbcommand.Param("yzlx").setAsString() = yzlx;
		g_dbcommand.Param("lkh").setAsString() = lkh;
		g_dbcommand.Param("qps").setAsLong() = qps;
		g_dbcommand.Param("sqys").setAsString() = theApp.m_Cpp_User;
		g_dbcommand.Param("sqrq").setAsDateTime() = ServerTime;
		g_dbcommand.Param("bz").setAsString() = bz;
		
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

	m_AdviceList.ResetGridSize();
	m_AdviceList.RedrawAll();
}

CString CJsyz::GenNewAdviceNo(CString blh)
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

void CJsyz::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	CStringArray  XhNolist;
	CUIntArray  rowlist;
	int nCol, nRet;
	long nRow;

	nRet = m_AdviceList.EnumFirstSelected( &nCol, &nRow );
	while( nRet == UG_SUCCESS )
	{
		if(nRow >= 0)
		{
			int rowcount = rowlist.GetSize();
			for(int i = 0; i < rowcount; i ++ )
			{
				if( nRow == rowlist.GetAt(i) )  break;
			}
			if( i >= rowcount )
			{
				CString str_Xh;
				
				CUGCell  cell;
				m_AdviceList.GetCellIndirect(0,nRow,&cell);
				cell.GetText(&str_Xh);
				
				rowlist.Add(nRow);
				XhNolist.Add(str_Xh);
			}
		}
		
		nRet = m_AdviceList.EnumNextSelected( &nCol, &nRow );
	}

	rowlist.RemoveAll();

	if( XhNolist.GetSize() <= 0 )
	{
		return;
	}

	for(int index = 0; index < XhNolist.GetSize(); index++)
	{
		try
		{
			SAString cmdstr = "Delete from " + theApp.TABLE_ADVICE + "  where medicalno = :medicalno and yzh = :yzh and yzzt = 0";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Param("yzh").setAsString()        = XhNolist.GetAt(index);
			
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

	XhNolist.RemoveAll();

	m_AdviceList.ResetGridSize();
	m_AdviceList.RedrawAll();
}

void CJsyz::OnSelchangeComboYzlx() 
{
	CString yzlx;
	GetDlgItemText(IDC_COMBO_YZLX, yzlx);    yzlx.TrimLeft();  yzlx.TrimRight();
	
	if(yzlx.CompareNoCase("补取") == 0)
	{
		SetDlgItemText(IDC_COMBO_LKH, "");

		GetDlgItem(IDC_COMBO_LKH)->EnableWindow(FALSE);

		for(int i = 0; i < m_QcbwList.GetSize(); i++)
		{
			((CComboBox *)GetDlgItem(IDC_COMBO_BZ))->AddString(m_QcbwList.GetAt(i));
		}

		if(m_QcbwList.GetSize() > 0)
		{
			SetDlgItemText(IDC_COMBO_BZ, m_QcbwList.GetAt(0));
		}
	}
	else
	{
		GetDlgItem(IDC_COMBO_LKH)->EnableWindow(TRUE);

        if(((CComboBox *)GetDlgItem(IDC_COMBO_LKH))->GetCount() > 0)
		{
			((CComboBox *)GetDlgItem(IDC_COMBO_LKH))->SetCurSel(0);
		}

		int num = ((CComboBox *)GetDlgItem(IDC_COMBO_BZ))->GetCount();
		for(int i = 0; i < num; i++)
		{
			((CComboBox *)GetDlgItem(IDC_COMBO_BZ))->DeleteString(0);
		}
		SetDlgItemText(IDC_COMBO_BZ, "");
	}
}

void CJsyz::OnDestroy() 
{
	CXTResizeDialog::OnDestroy();
	
	m_QcbwList.RemoveAll();
}

void CJsyz::OnButtonBlh() 
{
	SetDlgItemText(IDC_COMBO_LKH, theApp.GetView()->GetDocument()->m_blh);
}
