// Tjyz.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "Pathologyview.h"
#include "Pathologydoc.h"
#include "Tjyz.h"
#include "TagShow.h"
#include "MedDiagnose.h"
#include "MedTbs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTjyz dialog
extern CPathologyApp theApp;

CTjyz::CTjyz(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CTjyz::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTjyz)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
}


void CTjyz::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTjyz)
	DDX_Control(pDX, IDC_BUTTON_BLH, m_btBlh);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTjyz, CXTResizeDialog)
	//{{AFX_MSG_MAP(CTjyz)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_SELBJW, OnButtonSelbjw)
	ON_BN_CLICKED(IDC_BUTTON_TBBJW, OnButtonTbbjw)
	ON_BN_CLICKED(IDC_BUTTON_YZQR, OnButtonYzqr)
	ON_BN_CLICKED(IDC_BUTTON_BLH, OnButtonBlh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTjyz message handlers

BOOL CTjyz::OnInitDialog() 
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
	SetResize(IDC_STATIC_BZ, SZ_BOTTOM_LEFT ,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_BZ, SZ_BOTTOM_LEFT ,  SZ_BOTTOM_LEFT);

	SetResize(IDC_BUTTON_SELBJW, SZ_BOTTOM_LEFT ,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_BJW, SZ_BOTTOM_LEFT ,  SZ_BOTTOM_LEFT);

	SetResize(IDC_BUTTON_ADD, SZ_BOTTOM_RIGHT ,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_DELETE, SZ_BOTTOM_RIGHT ,  SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT ,  SZ_BOTTOM_RIGHT);

	SetResize(IDC_BUTTON_TBBJW, SZ_BOTTOM_RIGHT ,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_COMBO_TB, SZ_BOTTOM_RIGHT ,  SZ_BOTTOM_RIGHT);

	((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->AddString("免疫组化");
	((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->AddString("特殊染色");
	((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->AddString("分子病理");
	((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->AddString("电镜");
	((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->AddString("其它");
	((CComboBox *)GetDlgItem(IDC_COMBO_YZLX))->SetCurSel(0);

	((CComboBox *)GetDlgItem(IDC_COMBO_TB))->AddString("诊断意见");
	((CComboBox *)GetDlgItem(IDC_COMBO_TB))->AddString("光镜所见");
	((CComboBox *)GetDlgItem(IDC_COMBO_TB))->AddString("补充报告");
	((CComboBox *)GetDlgItem(IDC_COMBO_TB))->SetCurSel(0);

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
	
	//---------------------------------------------------
/*
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_TBBJW)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_TBBJW)->EnableWindow(TRUE);
	}
*/
	//---------------------------------------------------
	if(!theApp.GetView()->GetDocument()->m_blh.IsEmpty())
	{
		CString yzlx,lkh,ktxx,zyh,bz;
	    try {
			SAString cmdStr = "select yzlx,lkh,ktxx,zyh,bz from " + theApp.TABLE_ZC + " where medicalno=:medicalno";
			g_dbcommand.setCommandText(cmdStr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Execute();
			while(g_dbcommand.FetchNext()){
				yzlx = g_dbcommand.Field("yzlx").asString();
				lkh = g_dbcommand.Field("lkh").asString();
				ktxx = g_dbcommand.Field("ktxx").asString();
				zyh = g_dbcommand.Field("zyh").asString();
				bz = g_dbcommand.Field("bz").asString();
			}
			g_dbconnection.Commit();
		} catch (SAException &s) {

		}
		//if(zyh.CompareNoCase("已收费") == 0)
		CDialog::SetWindowText("特检医嘱("+zyh+")");
		
		if(ktxx.IsEmpty()){
			CStdioFile File;
			if ( File.Open(theApp.m_Exe_Working_Dir + "system\\" + theApp.GetView()->GetDocument()->m_blh + ".tj", CFile::modeRead | CFile::typeText) )
			{
				CString bjw,lkh,bz;
				File.ReadString(bjw);
				File.ReadString(lkh);
				File.ReadString(bz);
				File.Close();
				
				SetDlgItemText(IDC_EDIT_BJW,  bjw);
				SetDlgItemText(IDC_COMBO_LKH, lkh);
				SetDlgItemText(IDC_EDIT_BZ,   bz);
			}
		} else {
			SetDlgItemText(IDC_EDIT_BJW,  ktxx);
			SetDlgItemText(IDC_COMBO_LKH, lkh);
			SetDlgItemText(IDC_EDIT_BZ,   bz);
		}
		
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTjyz::OnButtonAdd() 
{
	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty()) return;
	
	CString bjw,lkh,bz,yzlx; int kts;
	GetDlgItemText(IDC_EDIT_BJW, bjw);       bjw.TrimLeft();   bjw.TrimRight();
	GetDlgItemText(IDC_COMBO_LKH, lkh);      lkh.TrimLeft();   lkh.TrimRight();
	GetDlgItemText(IDC_EDIT_BZ, bz);         bz.TrimLeft();    bz.TrimRight();
	GetDlgItemText(IDC_COMBO_YZLX,yzlx);	 yzlx.TrimLeft();  yzlx.TrimRight();
	
	int count = 0,index;CString ktxx = bjw;
	while((index = ktxx.Find(",")) >= 0){
		ktxx = ktxx.Right(ktxx.GetLength() - index - 1);
		count++;
	}
	kts = count; 
	CString blh,xm,brid,zyh;
	blh = theApp.GetView()->GetDocument()->m_blh;
	try{
	SAString cmdStr = "select xm,brid,zyh from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
	g_dbcommand.setCommandText(cmdStr);
	g_dbcommand.Param("medicalno").setAsString() = blh;
	g_dbcommand.Execute();

	while(g_dbcommand.FetchNext()){
		xm = g_dbcommand.Field("xm").asString();
		brid = g_dbcommand.Field("brid").asString();
		zyh = g_dbcommand.Field("zyh").asString();
	}
	  g_dbconnection.Commit();
	}catch (SAException &s){
		try{
			g_dbconnection.Rollback();
		} catch (SAException &s1){

		}
	}
	
	if(bjw.IsEmpty())
	{
		remove(theApp.m_Exe_Working_Dir + "system\\" + theApp.GetView()->GetDocument()->m_blh + ".tj");
	//	return;
	}

	  SADateTime servertime = theApp.GetServerDateTime();
		CString zyh1;
	  SAString cmdStr = "select zyh from cmipsmain_zc where medicalno='"+blh+"'";
	  g_dbcommand.setCommandText(cmdStr);
	  g_dbcommand.Execute();
	  while(g_dbcommand.FetchNext()){
		 zyh1 = g_dbcommand.Field("zyh").asString();
	  }
	  g_dbconnection.Commit();
	  //AfxMessageBox(zyh1);
	  if(zyh1.CompareNoCase("已收费") == 0){
		return;
	  } else {
		  SAString cmdstr = "delete from cmipsmain_zc where medicalno='"+blh+"'";
		  g_dbcommand.setCommandText(cmdstr);
		  g_dbcommand.Execute();
		  g_dbconnection.Commit();
	  }

 try{ 
	  SAString cmdStr = "insert into "+ theApp.TABLE_ZC + " values(:medicalno,:xm,:brid,'',:yzlx,:lkh,:ktxx,:kts,:bz,:kdys,:yzrq)";
	  g_dbcommand.setCommandText(cmdStr);
	  g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
	  g_dbcommand.Param("xm").setAsString() = xm;
	  g_dbcommand.Param("brid").setAsString() = brid;
	  //g_dbcommand.Param("zyh").setAsString() = "未收费";
	  g_dbcommand.Param("yzlx").setAsString() = yzlx;
	  g_dbcommand.Param("lkh").setAsString() = lkh;
	  g_dbcommand.Param("ktxx").setAsString() = bjw;
	  g_dbcommand.Param("kts").setAsLong() = kts;
	  g_dbcommand.Param("bz").setAsString() = bz;
	  g_dbcommand.Param("kdys").setAsString() = theApp.m_Cpp_User;
	  g_dbcommand.Param("yzrq").setAsDateTime() = servertime;
	  g_dbcommand.Execute();
	  g_dbconnection.Commit();
	  AfxMessageBox("当前医嘱信息已存入暂存列表!");
	} catch(SAException &s) {
		g_dbconnection.Rollback();
		CStdioFile File;
		if ( File.Open(theApp.m_Exe_Working_Dir + "system\\" + theApp.GetView()->GetDocument()->m_blh + ".tj", CFile::modeWrite |CFile::modeCreate | CFile::typeText) )
		{
			File.WriteString(bjw);
			File.WriteString("\n");
			File.WriteString(lkh);
			File.WriteString("\n");
			File.WriteString(bz);
			File.WriteString("\n");
			File.Close();
		}
       	AfxMessageBox("当前医嘱信息已保存到本地!");
	}
	
}

CString CTjyz::GenNewAdviceNo(CString blh)
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

void CTjyz::OnButtonDelete() 
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

void CTjyz::OnButtonSelbjw() 
{
	// TODO: Add your control notification handler code here
    CTagShow mydlg(this);
	mydlg.m_UsageType = 1;
	if(mydlg.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_EDIT_BJW, mydlg.m_TagString);
	}
}

void CTjyz::OnButtonTbbjw() 
{
	if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedDiagnose)) || m_pParent->IsKindOf(RUNTIME_CLASS(CMedTbs)))
	{
		if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedDiagnose)))
			((CMedDiagnose *)m_pParent)->SetModifiedFlag();
		else
			((CMedTbs *)m_pParent)->SetModifiedFlag();
	}

	theApp.ConnectDatabase();

	CString tbinfo; tbinfo.Empty();
	CString OriginYzlx; OriginYzlx.Empty();
	try
	{
		SAString cmdstr = "Select bjw, bx, yzlx from " + theApp.TABLE_ADVICE + " where medicalno = :medicalno and (yzlx = '免疫组化' or yzlx = '特殊染色' or yzlx = '分子病理' or yzlx = '电镜') order by yzh asc";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		while( g_dbcommand.FetchNext() )
		{
			
			CString tmpstr = g_dbcommand.Field("yzlx").asString();
			if(OriginYzlx.CompareNoCase(tmpstr))
			{
				OriginYzlx = tmpstr;

				tbinfo += tmpstr;
				tbinfo += "结果显示:";
			}

			tbinfo += g_dbcommand.Field("bjw").asString();
			tbinfo += '(';
			tbinfo += g_dbcommand.Field("bx").asString();
			tbinfo += ')';
			tbinfo += ',';
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
		return;
	}

	CString outtype;
	GetDlgItemText(IDC_COMBO_TB, outtype);
	if(outtype.CompareNoCase("诊断意见") == 0)
	{
		if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedDiagnose)) || m_pParent->IsKindOf(RUNTIME_CLASS(CMedTbs)))
		{
			
			if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
			{
				AfxMessageBox("当前病历已审核，特检结果不能写入诊断意见！");
			}
			else
			{
				
				
				CString tmp;
				m_pParent->GetDlgItemText(IDC_EDIT_ZDYJ, tmp);
				if(tmp.IsEmpty()){
					m_pParent->SetDlgItemText(IDC_EDIT_ZDYJ, tmp + tbinfo);
				}else {
					m_pParent->SetDlgItemText(IDC_EDIT_ZDYJ, tmp + "\r\n" + tbinfo);
				}
				

				AfxMessageBox("分析结果已成功写入诊断意见！");
			}
		}
	}

	if(outtype.CompareNoCase("光镜所见") == 0)
	{
		if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedDiagnose)) || m_pParent->IsKindOf(RUNTIME_CLASS(CMedTbs)))
		{
			if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
			{
				AfxMessageBox("当前病历已审核，特检结果不能写入光镜所见！");
			}
			else
			{
				if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedDiagnose)))
				{
					CString tmp;
					m_pParent->GetDlgItemText(IDC_EDIT_GJSJ, tmp);
					if(tmp.IsEmpty()){
						m_pParent->SetDlgItemText(IDC_EDIT_GJSJ, tmp + tbinfo);
					} else {
						m_pParent->SetDlgItemText(IDC_EDIT_GJSJ, tmp + "\r\n" + tbinfo);
					}
					
				}

				if(m_pParent->IsKindOf(RUNTIME_CLASS(CMedTbs)))
				{
					CString gjsj;  gjsj.Empty();
					try
					{
						SAString cmdstr = "Select gjsj from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
						g_dbcommand.setCommandText(cmdstr);
						g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
						g_dbcommand.Execute();
						
						if( g_dbcommand.FetchNext() )
						{
							gjsj = g_dbcommand.Field("gjsj").asString();
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
					
					try
					{
						SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set gjsj=:gjsj where medicalno = :medicalno";
						g_dbcommand.setCommandText(cmdstr);
						g_dbcommand.Param("gjsj").setAsString() = gjsj + "\r\n" + tbinfo;
						g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
						
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

				AfxMessageBox("分析结果已成功写入光镜所见！");
			}
		}
	}
	
	if(outtype.CompareNoCase("补充报告") == 0)
	{
		CString bczd;  bczd.Empty();
		try
		{
			SAString cmdstr = "Select bczd from " + theApp.TABLE_MAIN + " where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Execute();
			
			if( g_dbcommand.FetchNext() )
			{
				bczd = g_dbcommand.Field("bczd").asString();
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

		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set bczd=:bczd where medicalno = :medicalno";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("bczd").setAsString() = bczd + "\r\n" + tbinfo;
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			
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

		AfxMessageBox("分析结果已成功写入补充报告！");
	}
}

void CTjyz::OnButtonYzqr() 
{
	if(theApp.GetView()->GetDocument()->m_blh.IsEmpty()) return;
	
	CString yzlx,lkh,bz,bjw;
	GetDlgItemText(IDC_COMBO_YZLX, yzlx);    yzlx.TrimLeft();  yzlx.TrimRight();
	GetDlgItemText(IDC_COMBO_LKH, lkh);      lkh.TrimLeft();   lkh.TrimRight();
	GetDlgItemText(IDC_EDIT_BZ, bz);         bz.TrimLeft();    bz.TrimRight();
	GetDlgItemText(IDC_EDIT_BJW, bjw);       bjw.TrimLeft();   bjw.TrimRight();
	int qps = 1;
	
	CString sfqr;
	CDialog::GetWindowText(sfqr);
	if(sfqr.CompareNoCase("特检医嘱()") == 0 || sfqr.CompareNoCase("特检医嘱(未收费)") == 0){
		sfqr = "";
	} else {
		sfqr = "已收费";
	}

	if(yzlx.IsEmpty() || bjw.IsEmpty() || lkh.IsEmpty())
	{
		AfxMessageBox("输入了无效的数据,请检查输入值的有效性!");
		return;
	}
	
	CStringArray taglist; 
	while(!bjw.IsEmpty())
	{
		if(bjw.Find(',') < 0)  break;
		taglist.Add(bjw.Left(bjw.Find(',')));
		bjw = bjw.Right(bjw.GetLength() - bjw.Find(',') - 1);
		bjw.TrimLeft();  bjw.TrimRight();
	}
	
	if(!bjw.IsEmpty())
	{
		taglist.Add(bjw);
		bjw.Empty();
	}
	
	SADateTime ServerTime = theApp.GetServerDateTime();
	
	for(int index = 0; index < taglist.GetSize(); index++)
	{
		CString yzh = GenNewAdviceNo(theApp.GetView()->GetDocument()->m_blh);
		try
		{
			SAString cmdstr = "Insert into " + theApp.TABLE_ADVICE + " (medicalno,yzh,yzlx,lkh,bjw,qps,sqys,sqrq,yzzt,bz,fxjg) values(:medicalno,:yzh,:yzlx,:lkh,:bjw,:qps,:sqys,:sqrq,0,:bz,:fxjg)";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Param("yzh").setAsString() = yzh;
			g_dbcommand.Param("yzlx").setAsString() = yzlx;
			g_dbcommand.Param("lkh").setAsString() = lkh;
			g_dbcommand.Param("bjw").setAsString() = taglist.GetAt(index);
			g_dbcommand.Param("qps").setAsLong() = qps;
			g_dbcommand.Param("sqys").setAsString() = theApp.m_Cpp_User;
			g_dbcommand.Param("sqrq").setAsDateTime() = ServerTime;
			g_dbcommand.Param("bz").setAsString() = bz;
			g_dbcommand.Param("fxjg").setAsString() = (sfqr.CompareNoCase("已收费")==0) ? "已收费" : "待收费"; 
			
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
	
	taglist.RemoveAll();
	
	m_AdviceList.ResetGridSize();
	m_AdviceList.RedrawAll();

	SetDlgItemText(IDC_EDIT_BJW, "");

	try{
		SAString cmdStr = "delete from "+ theApp.TABLE_ZC + " where medicalno = '" + theApp.GetView()->GetDocument()->m_blh +"'";
		g_dbcommand.setCommandText(cmdStr);
		g_dbcommand.Execute();
		g_dbconnection.Commit();
	} catch (SAException &s) {
		try{
		  g_dbconnection.Rollback();
		}catch(SAException &s){

		}
	}
	remove(theApp.m_Exe_Working_Dir + "system\\" + theApp.GetView()->GetDocument()->m_blh + ".tj");
}

void CTjyz::OnButtonBlh() 
{
	SetDlgItemText(IDC_COMBO_LKH, theApp.GetView()->GetDocument()->m_blh);
}
