// ChargeDetail.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "MainFrm.h"
#include "PathologyView.h"
#include "PathologyDoc.h"
#include "ChargeDetail.h"
#include "MedPerson.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChargeDetail dialog
extern CPathologyApp theApp;

CChargeDetail::CChargeDetail(CWnd* pParent /*=NULL*/)
               : CXTCBarDialog(CChargeDetail::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChargeDetail)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDB_ICON_CHARGEDETAIL);
	m_pParent = (CMedPerson *)pParent; 
}


BEGIN_MESSAGE_MAP(CChargeDetail, CXTCBarDialog)
	//{{AFX_MSG_MAP(CChargeDetail)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_CHARGEITEM_NEW, OnChargeitemNew)
	ON_COMMAND(ID_CHARGEITEM_DELETE, OnChargeitemDelete)
	ON_COMMAND(ID_CHARGEITEM_ALLITEM, OnChargeitemAllitem)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE( TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
    ON_NOTIFY_EX_RANGE( TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChargeDetail message handlers

BOOL CChargeDetail::OnInitDialog() 
{
	CXTCBarDialog::OnInitDialog();
	
	ChargeDBArray.RemoveAll();
	m_LabelText.Empty();

	theApp.ConnectDatabase();
	try
	{
		SAString cmdstr = "Select * from chargedetail";
		
		g_dbcommand.setCommandText(cmdstr);
		
		g_dbcommand.Execute();
		
		while( g_dbcommand.FetchNext() )
		{
			CHARGEITEM_RECORD tmp;
			SAString str;
			str = g_dbcommand.Field("code").asString();
			str.TrimLeft();  str.TrimRight();
			strcpy(tmp.code, str);
			str = g_dbcommand.Field("item").asString();
			str.TrimLeft();  str.TrimRight();
			strcpy(tmp.item, str);

			m_LabelText += str;
			m_LabelText += "\n";

			tmp.SinglePrice = (float)g_dbcommand.Field("singleprice").asDouble();
			tmp.Type        = g_dbcommand.Field("type").asLong();
			str = g_dbcommand.Field("unit").asString();
			str.TrimLeft();  str.TrimRight();
			strcpy(tmp.Unit, str);
			tmp.Num   = 1;
			tmp.price = 0;
			str = g_dbcommand.Field("note").asString();
			str.TrimLeft();  str.TrimRight();
			strcpy(tmp.Note, str);

			strcpy(tmp.Recorder, theApp.m_Cpp_User);

			ChargeDBArray.Add(tmp);
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

	try
	{
		SAString cmdstr = "Select * from " + theApp.TABLE_CHARGE + " where medicalno = :medicalno";
		
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = m_blh;
		
		g_dbcommand.Execute();
		
		while( g_dbcommand.FetchNext() )
		{
			CHARGEITEM_RECORD  tmp;
			SAString str;
			str = g_dbcommand.Field("code").asString();
			strcpy(tmp.code, str);
			str = g_dbcommand.Field("item").asString();
			str.TrimLeft();  str.TrimRight();
			strcpy(tmp.item, str);
			tmp.SinglePrice = (float)g_dbcommand.Field("singleprice").asDouble();
			tmp.Type        = g_dbcommand.Field("type").asLong();
			str = g_dbcommand.Field("unit").asString();
			str.TrimLeft();  str.TrimRight();
			strcpy(tmp.Unit, str);
			tmp.Num   = g_dbcommand.Field("num").asLong();
			tmp.price = (float)g_dbcommand.Field("price").asDouble();
			str = g_dbcommand.Field("note").asString();
			str.TrimLeft();  str.TrimRight();
			strcpy(tmp.Note, str);
			str = g_dbcommand.Field("recorder").asString();
			str.TrimLeft();  str.TrimRight();
			strcpy(tmp.Recorder, str);

			SfDetailArray.Add(tmp);
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

	if(theApp.m_HisCfg.CompareNoCase("山东济宁医附院") == 0)
	{
		CString nIp, nDATABASENAME,nDATABASE_USERNAME,nDATABASE_PASSWORD;
		
		CStdioFile File;
		if ( File.Open(theApp.m_Exe_Working_Dir + "system\\pacsconnect.cfg", CFile::modeRead | CFile::typeText) )
		{
			CString tmp;
			int index = 0;
			while(1)
			{
				if( File.ReadString(tmp) == FALSE ) break;
				tmp.TrimLeft();	tmp.TrimRight();
				if( tmp.IsEmpty() )   continue;
				switch( index ) {
				case 0:
					nIp = tmp;
					break;
				case 1:
					nDATABASENAME = tmp;
					break;
				case 2:
					nDATABASE_USERNAME = tmp;
					break;
				case 3:
					nDATABASE_PASSWORD = tmp;
					break;
				}
				index++;
			}
			File.Close();

			SAConnection pacscon;
			SACommand    pacscmd;
			try
			{
				pacscon.Connect( (SAString)nDATABASENAME, (SAString)nDATABASE_USERNAME, (SAString)nDATABASE_PASSWORD, SA_Oracle_Client );
				pacscmd.setConnection( &pacscon );
			}
			catch(SAException &x)
			{
				try
				{
					pacscon.Rollback();
				}
				catch(SAException &)
				{
				}
				AfxMessageBox((const char*)x.ErrText());
			}
			
			if(pacscon.isConnected())  
			{
				ChargeDBArray.RemoveAll();
				SfDetailArray.RemoveAll();
				m_LabelText.Empty();
				
				bool IsUserExist = false;
				try
				{
					pacscmd.setCommandText("Select * from users where user_dept = '0406' and user_name = '" + (SAString)theApp.m_Cpp_User + "'");
					pacscmd.Execute();
					
					if( pacscmd.FetchNext() )
					{
						IsUserExist = true;
					}
					
					pacscon.Commit();
				}
				catch(SAException &)
				{
					try
					{
						pacscon.Rollback();
					}
					catch(SAException &)
					{
					}
				}

				if(!IsUserExist)
				{
					if(pacscon.isConnected())    pacscon.Disconnect();

					AfxMessageBox("当前用户在HIS的USERS表中没有定义！");
					EndDialog(IDCANCEL);
					return FALSE;
				}

				CString his_zymzbs, his_Mrid, his_sqh, his_fb;
				m_pParent->GetDlgItemText(IDC_EDIT_MZ,   his_zymzbs);
				his_zymzbs.TrimLeft(); his_zymzbs.TrimRight();
				m_pParent->GetDlgItemText(IDC_EDIT_BRID, his_Mrid);
				his_Mrid.TrimLeft();    his_Mrid.TrimRight();
				m_pParent->GetDlgItemText(IDC_EDIT_SQH,  his_sqh);
				his_sqh.TrimLeft();          his_sqh.TrimRight();
				m_pParent->GetDlgItemText(IDC_EDIT_FB,   his_fb);
				his_fb.TrimLeft();           his_fb.TrimRight();

				if(his_zymzbs.CompareNoCase("住院") == 0)
				{
					try
					{
						pacscmd.setCommandText("Select * from price_list where class_on_reckoning = 'D24' and stop_date IS NULL");
						pacscmd.Execute();
						
						while( pacscmd.FetchNext() )
						{
							CHARGEITEM_RECORD tmp;
							SAString str;
							str = pacscmd.Field("item_code").asString();
							str.TrimLeft();  str.TrimRight();
							strcpy(tmp.code, str);
							str = pacscmd.Field("item_name").asString();
							str.TrimLeft();  str.TrimRight();
							strcpy(tmp.item, str);
							
							m_LabelText += str;
							m_LabelText += "\n";
							
							tmp.SinglePrice = (float)pacscmd.Field("price").asDouble();
							tmp.Type        = 0;
							str = pacscmd.Field("units").asString();
							str.TrimLeft();  str.TrimRight();
							strcpy(tmp.Unit, str);
							tmp.Num   = 1;
							tmp.price = 0;
							str = pacscmd.Field("memo").asString();
							str.TrimLeft();  str.TrimRight();
							strcpy(tmp.Note, str);

							str = pacscmd.Field("item_class").asString();
							str.TrimLeft();  str.TrimRight();
							strcpy(tmp.Reserved, str);

							str = pacscmd.Field("item_spec").asString();
							str.TrimLeft();  str.TrimRight();
							strcpy(tmp.Reserved1, str);

							strcpy(tmp.Recorder, theApp.m_Cpp_User);
							
							ChargeDBArray.Add(tmp);
						}
						
						pacscon.Commit();
					}
					catch(SAException &x)
					{
						try
						{
							pacscon.Rollback();
						}
						catch(SAException &)
						{
						}
						AfxMessageBox((const char*)x.ErrText());
					}

					CString his_Patientid;
					try
					{
						pacscmd.setCommandText("Select Patientid from PathologyApp where flowId = :flowId");
						pacscmd.Param("flowId").setAsString() = his_sqh;
						pacscmd.Execute();
						
						if( pacscmd.FetchNext() )
						{
							his_Patientid = pacscmd.Field("Patientid").asString();
						}
						
						pacscon.Commit();
					}
					catch(SAException &x)
					{
						try
						{
							pacscon.Rollback();
						}
						catch(SAException &)
						{
						}
						AfxMessageBox((const char*)x.ErrText());
					}

					try
					{
						pacscmd.setCommandText("Select * from Inp_bill_detail where patient_id=:patient_id and Visit_id=:Visit_id and Performed_by = '0406'");
						pacscmd.Param("patient_id").setAsString() = his_Mrid;
						pacscmd.Param("Visit_id").setAsShort()    = atoi(his_Patientid.Right(his_Patientid.GetLength() - his_Patientid.Find("_") - 1));
						
						pacscmd.Execute();
						
						while( pacscmd.FetchNext() )
						{
							CHARGEITEM_RECORD  tmp;
							SAString str;
							str = pacscmd.Field("ITEM_CODE").asString();
							strcpy(tmp.code, str);
							float SinglePrice = 0.0;
							for(int k = 0; k < ChargeDBArray.GetSize(); k++)
							{
								CString pp = ChargeDBArray.GetAt(k).code;
								pp.TrimLeft();  pp.TrimRight();
								if(pp.CompareNoCase(str) == 0)
								{
									SinglePrice = ChargeDBArray.GetAt(k).SinglePrice;
									break;
								}
							}
							str = pacscmd.Field("ITEM_NAME").asString();
							str.TrimLeft();  str.TrimRight();
							strcpy(tmp.item, str);
							tmp.SinglePrice = SinglePrice;
							tmp.Type        = 0;
							str = pacscmd.Field("UNITS").asString();
							str.TrimLeft();  str.TrimRight();
							strcpy(tmp.Unit, str);
							tmp.Num   = pacscmd.Field("AMOUNT").asLong();
							tmp.price = (float)pacscmd.Field("CHARGES").asDouble();
							str = pacscmd.Field("OPERATOR_NO").asString();
							str.TrimLeft();  str.TrimRight();
							strcpy(tmp.Recorder, str);

							SfDetailArray.Add(tmp);
						}
						
						pacscon.Commit();
					}
					catch(SAException &)
					{
						try
						{
							pacscon.Rollback();
						}
						catch(SAException &)
						{
						}
					}

					for(int k = 0; k < SfDetailArray.GetSize(); k++)
					{
						CHARGEITEM_RECORD  tmp = SfDetailArray.GetAt(k);
						SAString  str = tmp.Recorder;
						str.TrimLeft();  str.TrimRight();

						if(!str.IsEmpty())
						{
							try
							{
								pacscmd.setCommandText("Select * from users where user_id = '" + str +"'");
								pacscmd.Execute();
								
								if( pacscmd.FetchNext() )
								{
									strcpy(tmp.Recorder, pacscmd.Field("user_name").asString());
								}
								
								pacscon.Commit();
							}
							catch(SAException &)
							{
								try
								{
									pacscon.Rollback();
								}
								catch(SAException &)
								{
								}
							}
							
							SfDetailArray.SetAt(k, tmp);
						}
					}

					short visit_id = 0;
					try
					{
						pacscmd.setCommandText("select visit_id from pats_in_hospital where patient_id = :patient_id");
						pacscmd.Param("patient_id").setAsString() = his_Mrid;
						pacscmd.Execute();
						
						if( pacscmd.FetchNext() )
						{
							visit_id = pacscmd.Field("visit_id").asShort();
						}
						
						pacscon.Commit();
					}
					catch(SAException &x)
					{
						try
						{
							pacscon.Rollback();
						}
						catch(SAException &)
						{
						}
						AfxMessageBox((const char*)x.ErrText());
					}

					try
					{
						pacscmd.setCommandText("select charge_type from pat_visit where patient_id = :patient_id and visit_id = :visit_id");
						pacscmd.Param("patient_id").setAsString() = his_Mrid;
						pacscmd.Param("visit_id").setAsShort()    = visit_id;
						pacscmd.Execute();
						
						if( pacscmd.FetchNext() )
						{
							his_fb = pacscmd.Field("charge_type").asString();
						}
						
						pacscon.Commit();
					}
					catch(SAException &x)
					{
						try
						{
							pacscon.Rollback();
						}
						catch(SAException &)
						{
						}
						AfxMessageBox((const char*)x.ErrText());
					}

					try
					{
						SAString cmdstr = "Select * from CHARGE_SPECIAL_ITEM_DICT where CHARGE_TYPE = '" + his_fb + "'";
						pacscmd.setCommandText(cmdstr);
						pacscmd.Execute();
						
						while( pacscmd.FetchNext() )
						{
							SAString item_code, item_class;
							item_code  = pacscmd.Field("item_code").asString();
							item_class = pacscmd.Field("item_class").asString();
							for(int k = 0; k < ChargeDBArray.GetSize(); k++)
							{
								CString pp = ChargeDBArray.GetAt(k).code;
								pp.TrimLeft();  pp.TrimRight();
								CString LocalClass = ChargeDBArray.GetAt(k).Reserved;
								LocalClass.TrimLeft();  LocalClass.TrimRight();
								if(pp.CompareNoCase(item_code) == 0 || (item_class.CompareNoCase("D")==0 && item_code.CompareNoCase("*")==0 && LocalClass.CompareNoCase("D")==0))
								{
									ITEM_CODE_List.Add(pp);
									ParamList.Add(1.0 * pacscmd.Field("PROPORTION_NUMERATOR").asLong() / pacscmd.Field("PROPORTION_DENOMINATOR").asLong());
									if(pp.CompareNoCase(item_code) == 0)  break;
								}
							}
						}
						
						pacscon.Commit();
					}
					catch(SAException &x)
					{
						try
						{
							pacscon.Rollback();
						}
						catch(SAException &)
						{
						}
						AfxMessageBox((const char*)x.ErrText());
					}
				}
				
				if(his_zymzbs.CompareNoCase("门诊") == 0)
				{
					try
					{
						pacscmd.setCommandText("Select * from zd_list where flag2 = 1 and type2 = '病理费'");
						pacscmd.Execute();
						
						while( pacscmd.FetchNext() )
						{
							CHARGEITEM_RECORD tmp;
							SAString str;
							str = pacscmd.Field("charge_id").asString();
							str.TrimLeft();  str.TrimRight();
							strcpy(tmp.code, str);
							str = pacscmd.Field("name").asString();
							str.TrimLeft();  str.TrimRight();
							strcpy(tmp.item, str);
							
							m_LabelText += str;
							m_LabelText += "\n";
							
							tmp.SinglePrice = (float)pacscmd.Field("amount").asDouble();
							tmp.Type        = 1;
							str = pacscmd.Field("specify").asString();
							str.TrimLeft();  str.TrimRight();
							strcpy(tmp.Unit, str);
							tmp.Num   = 1;
							tmp.price = 0;
							
							strcpy(tmp.Recorder, theApp.m_Cpp_User);

							ChargeDBArray.Add(tmp);
						}
						
						pacscon.Commit();
					}
					catch(SAException &x)
					{
						try
						{
							pacscon.Rollback();
						}
						catch(SAException &)
						{
						}
						AfxMessageBox((const char*)x.ErrText());
					}

					SAString Mz_winprg, Mz_process_NO;
					Mz_winprg.Empty();  Mz_process_NO.Empty();
					try
					{
						pacscmd.setCommandText("Select Mz_winprg, Mz_process_NO from MZ_PRESC_MASTER where dr_winprg like '" + (SAString)his_sqh + "_%' order by timeprg desc");
						pacscmd.Execute();
						
						if( pacscmd.FetchNext() )
						{
							Mz_winprg = pacscmd.Field("Mz_winprg").asString();
							Mz_process_NO = pacscmd.Field("Mz_process_NO").asString();
						}
						
						pacscon.Commit();
					}
					catch(SAException &x)
					{
						try
						{
							pacscon.Rollback();
						}
						catch(SAException &)
						{
						}
						AfxMessageBox((const char*)x.ErrText());
					}

					if(Mz_process_NO.IsEmpty() || Mz_winprg.IsEmpty())
					{
						SetWindowText("收费明细表 （门诊病人未缴费）");
					}
					else
					{
						SetWindowText("收费明细表 （门诊病人已缴费）");

						try
						{
							pacscmd.setCommandText("Update PathologyApp set item7 = 1 where flowId = :flowId");
							pacscmd.Param("flowId").setAsString() = his_sqh;
							
							pacscmd.Execute();
							pacscon.Commit();
						}
						catch(SAException &x)
						{
							try
							{
								pacscon.Rollback();
							}
							catch(SAException &)
							{
							}
							AfxMessageBox((const char*)x.ErrText());
						}
					}

					try
					{
						pacscmd.setCommandText("Select * from MZ_PRESC_DETAIL where adviceid = :adviceid");
						pacscmd.Param("adviceid").setAsString() = his_sqh;
						
						pacscmd.Execute();
						
						while( pacscmd.FetchNext() )
						{
							CHARGEITEM_RECORD  tmp;
							SAString str;
							str = pacscmd.Field("Itemid").asString();
							strcpy(tmp.code, str);
							str = pacscmd.Field("Itemname").asString();
							str.TrimLeft();  str.TrimRight();
							strcpy(tmp.item, str);
							tmp.SinglePrice = (float)pacscmd.Field("Price").asDouble();
							tmp.Type        = 1;
							strcpy(tmp.Unit, pacscmd.Field("unit").asString());
							tmp.Num   = pacscmd.Field("Quantity").asLong();
							tmp.price = (float)pacscmd.Field("Realpay").asDouble();
							
							SfDetailArray.Add(tmp);
						}
						
						pacscon.Commit();
					}
					catch(SAException &x)
					{
						try
						{
							pacscon.Rollback();
						}
						catch(SAException &)
						{
						}
						AfxMessageBox((const char*)x.ErrText());
					}
				}
			}
			else
			{
				AfxMessageBox("数据库服务器连接失败");
			}

			if(pacscon.isConnected())    pacscon.Disconnect();
		}
		else
		{
			AfxMessageBox("没有找到PACS联结配置文件!");
		}
	}


	m_Original_Item_Num = SfDetailArray.GetSize();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),AFX_IDW_TOOLBAR+24)||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_CHARGEDETAIL))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;      // fail to create
	}
    m_wndToolBar.AutoSizeToolbar();

	CRect rect;
	GetClientRect(&rect);
	CRect rt = rect;
	rt.bottom -= 30;
    if(!m_SfDetailCtrl.CreateGrid(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rt,this,118))
	{
		TRACE0( "Failed to create m_SfDetailCtrl.\n" );
		return FALSE;
	}

    SetResize(IDOK,     SZ_BOTTOM_CENTER,  SZ_BOTTOM_CENTER);
    SetResize(IDCANCEL, SZ_BOTTOM_CENTER,  SZ_BOTTOM_CENTER);

	// Move control bars to the correct position.
    RecalcBarLayout();

	m_SfDetailCtrl.GridSetup();

	//---------------------------------------------------
	if(theApp.IsRecordLocked(theApp.GetView()->GetDocument()->m_blh))
	{
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
	//---------------------------------------------------

	int lknum = 0, cgnum = 0, bdnum = 0, tjnum = 0;
	//((CMainFrame *)AfxGetMainWnd())->GetSfExt(m_blh, lknum, cgnum, bdnum, tjnum);
	//CString m_Title;
	//m_Title.Format("收费明细表【蜡块%d、常规%d、冰冻%d、特检%d】", lknum, cgnum, bdnum, tjnum);
	SetWindowText(getTitle());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HCURSOR CChargeDetail::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CChargeDetail::OnSize(UINT nType, int cx, int cy) 
{
	CXTCBarDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	::MoveWindow(m_SfDetailCtrl.m_hWnd,0,30,cx,cy-60,TRUE);
}

void CChargeDetail::OnDestroy() 
{
    SfDetailArray.RemoveAll();
	ChargeDBArray.RemoveAll();
	
	ITEM_CODE_List.RemoveAll();
	ParamList.RemoveAll();

	CXTCBarDialog::OnDestroy();
}

void CChargeDetail::OnOK() 
{
	theApp.ConnectDatabase();
	
	try
	{
		SAString cmdstr = "Delete from " + theApp.TABLE_CHARGE + " where medicalno = :medicalno";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = m_blh;
		
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

	float nSum = 0, nPayment = 0;
	for(int index = 0; index < SfDetailArray.GetSize(); index++)
	{
		CHARGEITEM_RECORD  tmp = SfDetailArray.GetAt(index);
		try
		{
			SAString cmdstr = "Insert into " + theApp.TABLE_CHARGE + "(medicalno,code,item,singleprice,type,unit,num,price,note,recorder) values(:medicalno,:code,:item,:singleprice,:type,:unit,:num,:price,:note,:recorder)";
			
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("medicalno").setAsString()   = m_blh;
			SAString str = tmp.code;
			str.TrimLeft();  str.TrimRight();
			g_dbcommand.Param("code").setAsString()        = str;
			str = tmp.item;
			str.TrimLeft();  str.TrimRight();
			g_dbcommand.Param("item").setAsString()        = str;
			g_dbcommand.Param("singleprice").setAsDouble() = tmp.SinglePrice;
			g_dbcommand.Param("type").setAsLong()          = tmp.Type;
			str = tmp.Unit;
			str.TrimLeft();  str.TrimRight();
			g_dbcommand.Param("unit").setAsString()        = str;
			g_dbcommand.Param("num").setAsLong()           = tmp.Num;
			g_dbcommand.Param("price").setAsDouble()       = tmp.price;
			str = tmp.Note;
			str.TrimLeft();  str.TrimRight();
			g_dbcommand.Param("note").setAsString()        = str;
			str = tmp.Recorder;
			str.TrimLeft();  str.TrimRight();
			g_dbcommand.Param("recorder").setAsString()    = str;

            nSum += tmp.price;
			nPayment += (tmp.Num * tmp.SinglePrice);
			
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

	ASSERT(m_pParent != NULL);
	ASSERT(m_pParent->IsKindOf(RUNTIME_CLASS(CMedPerson)));

	CString str;
	str.Format("%.2f", nSum);
	m_pParent->SetDlgItemText(IDC_EDIT_SF, str);
	
	if(theApp.m_HisCfg.CompareNoCase("山东济宁医附院") == 0)
	{
		m_pParent->SetDlgItemText(IDC_EDIT_SF, "");

		CString nIp, nDATABASENAME,nDATABASE_USERNAME,nDATABASE_PASSWORD;
		
		CStdioFile File;
		if ( File.Open(theApp.m_Exe_Working_Dir + "system\\pacsconnect.cfg", CFile::modeRead | CFile::typeText) )
		{
			CString tmp;
			int index = 0;
			while(1)
			{
				if( File.ReadString(tmp) == FALSE ) break;
				tmp.TrimLeft();	tmp.TrimRight();
				if( tmp.IsEmpty() )   continue;
				switch( index ) {
				case 0:
					nIp = tmp;
					break;
				case 1:
					nDATABASENAME = tmp;
					break;
				case 2:
					nDATABASE_USERNAME = tmp;
					break;
				case 3:
					nDATABASE_PASSWORD = tmp;
					break;
				}
				index++;
			}
			File.Close();

			SAConnection pacscon;
			SACommand    pacscmd;
			try
			{
				pacscon.Connect( (SAString)nDATABASENAME, (SAString)nDATABASE_USERNAME, (SAString)nDATABASE_PASSWORD, SA_Oracle_Client );
				pacscmd.setConnection( &pacscon );
			}
			catch(SAException &x)
			{
				try
				{
					pacscon.Rollback();
				}
				catch(SAException &)
				{
				}
				AfxMessageBox((const char*)x.ErrText());
			}
			
			if(pacscon.isConnected())  
			{
				SADateTime ServerTime = theApp.GetServerDateTime();
				
				CString his_zymzbs, his_sqh, his_Name, his_sex, his_age, his_Mrid;
				m_pParent->GetDlgItemText(IDC_EDIT_MZ, his_zymzbs);
				his_zymzbs.TrimLeft();       his_zymzbs.TrimRight();
				m_pParent->GetDlgItemText(IDC_EDIT_SQH,his_sqh);
				his_sqh.TrimLeft();          his_sqh.TrimRight();
				m_pParent->GetDlgItemText(IDC_EDIT_XM, his_Name);
				his_Name.TrimLeft();         his_Name.TrimRight();
				m_pParent->GetDlgItemText(IDC_EDIT_XB, his_sex);
				his_sex.TrimLeft();          his_sex.TrimRight();
				m_pParent->GetDlgItemText(IDC_EDIT_NL, his_age);
				his_age.TrimLeft();          his_age.TrimRight();
				m_pParent->GetDlgItemText(IDC_EDIT_BRID, his_Mrid);
				his_Mrid.TrimLeft();    his_Mrid.TrimRight();

				if(his_zymzbs.CompareNoCase("住院") == 0)
				{
					CString his_Patientid, sectionCode;
					try
					{
						pacscmd.setCommandText("Select Patientid, sectionCode from PathologyApp where flowId = :flowId");
						pacscmd.Param("flowId").setAsString() = his_sqh;
						pacscmd.Execute();
						
						if( pacscmd.FetchNext() )
						{
							his_Patientid = pacscmd.Field("Patientid").asString();
							sectionCode   = pacscmd.Field("sectionCode").asString();
						}
						
						pacscon.Commit();
					}
					catch(SAException &x)
					{
						try
						{
							pacscon.Rollback();
						}
						catch(SAException &)
						{
						}
						AfxMessageBox((const char*)x.ErrText());
					}

					int ItemCount = 0;
					try
					{
						pacscmd.setCommandText("Select count(*) from pats_in_hospital where Patient_id=:Patient_id and Visit_id=:Visit_id");
						pacscmd.Param("Patient_id").setAsString() = his_Mrid;
						pacscmd.Param("Visit_id").setAsShort()    = atoi(his_Patientid.Right(his_Patientid.GetLength() - his_Patientid.Find("_") - 1));
						pacscmd.Execute();
						
						if( pacscmd.FetchNext() )
						{
							ItemCount = pacscmd.Field(1).asLong();
						}
						
						pacscon.Commit();
					}
					catch(SAException &x)
					{
						try
						{
							pacscon.Rollback();
						}
						catch(SAException &)
						{
						}
						AfxMessageBox((const char*)x.ErrText());
					}

					if(ItemCount)
					{
						/*int Max_No = 0;
						try
						{
							pacscmd.setCommandText("Select max(item_no) from Inp_bill_detail where patient_id=:patient_id and Visit_id=:Visit_id");
							pacscmd.Param("patient_id").setAsString() = his_Mrid;
							pacscmd.Param("Visit_id").setAsShort()    = atoi(his_Patientid.Right(his_Patientid.GetLength() - his_Patientid.Find("_") - 1));
							pacscmd.Execute();
							
							if( pacscmd.FetchNext() )
							{
								Max_No = pacscmd.Field(1).asLong();
							}
							
							pacscon.Commit();
						}
						catch(SAException &x)
						{
							try
							{
								pacscon.Rollback();
							}
							catch(SAException &)
							{
							}
							AfxMessageBox((const char*)x.ErrText());
						}*/

						CStringArray nUserCode_List, nUserName_list;
						try
						{
							pacscmd.setCommandText("Select * from users where user_dept = '0406'");
							pacscmd.Execute();
							
							while( pacscmd.FetchNext() )
							{
								nUserCode_List.Add(pacscmd.Field("user_id").asString());
								nUserName_list.Add(pacscmd.Field("user_name").asString());
							}
							
							pacscon.Commit();
						}
						catch(SAException &)
						{
							try
							{
								pacscon.Rollback();
							}
							catch(SAException &)
							{
							}
						}

						bool IsUploadSuccess = false;
						for(int index = m_Original_Item_Num; index < SfDetailArray.GetSize(); index++)
						{
							CHARGEITEM_RECORD  tmp = SfDetailArray.GetAt(index);
							SAString str, item_class, item_spec;
							item_class.Empty();  item_spec.Empty();

							SAString ItemCode = tmp.code;
							float SinglePrice = 0.0;
							ItemCode.TrimLeft(); ItemCode.TrimRight();
							for(int k = 0; k < ChargeDBArray.GetSize(); k++)
							{
								str = ChargeDBArray.GetAt(k).code;
								str.TrimLeft(); str.TrimRight();
								if(str.CompareNoCase(ItemCode) == 0)
								{
									item_class  = ChargeDBArray.GetAt(k).Reserved;
									SinglePrice = ChargeDBArray.GetAt(k).SinglePrice;
									item_spec   = ChargeDBArray.GetAt(k).Reserved1;
									break;
								}
							}
							
							try
							{
								pacscmd.setCommandText("inpbill.inp_bill_detail_add");
								pacscmd.Param("patientid").setAsString()           = his_Mrid;
								pacscmd.Param("visitid").setAsShort()              = atoi(his_Patientid.Right(his_Patientid.GetLength() - his_Patientid.Find('_') -1));
								pacscmd.Param("item_class").setAsString()          = item_class;
								str = tmp.item;
								pacscmd.Param("item_name").setAsString()           = str;
								str = tmp.code;
								pacscmd.Param("item_code").setAsString()           = str;
								pacscmd.Param("item_spec").setAsString()           = item_spec;
								pacscmd.Param("amount").setAsDouble()              = tmp.Num;
								str = tmp.Unit;
								pacscmd.Param("units").setAsString()               = str;
								pacscmd.Param("ordered_by").setAsString()          = sectionCode;
								pacscmd.Param("performed_by").setAsString()        = "0406";
								pacscmd.Param("costs").setAsDouble()               = tmp.Num * SinglePrice;
								pacscmd.Param("charges").setAsDouble()             = tmp.price;
								//pacscmd.Param("billing_date_time").setAsDateTime() = ServerTime;
								str = tmp.Recorder;
								
								CString UserCode = "0406";
								for(k = 0; k < nUserName_list.GetSize(); k++)
								{
									if(str.CompareNoCase(nUserName_list.GetAt(k)) == 0) break;
								}
								if(k < nUserName_list.GetSize())
									UserCode = nUserCode_List.GetAt(k);
								
								pacscmd.Param("operator_no").setAsString()         = UserCode;

								pacscmd.Execute();
								pacscon.Commit();
							}
							catch(SAException &x)
							{
								try
								{
									pacscon.Rollback();
								}
								catch(SAException &)
								{
								}
								AfxMessageBox((const char*)x.ErrText());
							}

							/*try
							{
								SAString cmdstr = "Insert into Inp_bill_detail_temp (patient_id,visit_id,item_no,item_class,item_name,item_code,item_spec,amount,units,ordered_by,performed_by,costs,charges,billing_date_time,Operator_no) values(:patient_id,:visit_id,:item_no,:item_class,:item_name,:item_code,:item_spec,:amount,:units,:ordered_by,'0406',:costs,:charges,:billing_date_time,:Operator_no)";
								pacscmd.setCommandText(cmdstr);
								pacscmd.Param("patient_id").setAsString()          = his_Mrid;
								pacscmd.Param("visit_id").setAsShort()             = atoi(his_Patientid.Right(his_Patientid.GetLength() - his_Patientid.Find('_') -1));
								pacscmd.Param("item_no").setAsLong()               = ++Max_No;
								pacscmd.Param("item_class").setAsString()          = item_class;
								str = tmp.item;
								pacscmd.Param("item_name").setAsString()           = str;
								str = tmp.code;
								pacscmd.Param("item_code").setAsString()           = str;
								pacscmd.Param("item_spec").setAsString()           = item_spec;
								pacscmd.Param("amount").setAsShort()               = tmp.Num;
								str = tmp.Unit;
								pacscmd.Param("units").setAsString()               = str;
								pacscmd.Param("ordered_by").setAsString()          = sectionCode;
								pacscmd.Param("costs").setAsDouble()               = tmp.Num * SinglePrice;
								pacscmd.Param("charges").setAsDouble()             = tmp.price;
								pacscmd.Param("billing_date_time").setAsDateTime() = ServerTime;
								str = tmp.Recorder;
								
								CString UserCode = "0406";
								for(k = 0; k < nUserName_list.GetSize(); k++)
								{
									if(str.CompareNoCase(nUserName_list.GetAt(k)) == 0) break;
								}
								if(k < nUserName_list.GetSize())
									UserCode = nUserCode_List.GetAt(k);

								pacscmd.Param("Operator_no").setAsString()         = UserCode;
								
								pacscmd.Execute();
								
								pacscon.Commit();

								IsUploadSuccess = true;
							}
							catch(SAException &x)
							{
								try
								{
									pacscon.Rollback();
								}
								catch(SAException &)
								{
								}
								AfxMessageBox((const char*)x.ErrText());
							}*/
						}

						nUserCode_List.RemoveAll();
						nUserName_list.RemoveAll();

						if(IsUploadSuccess)
						{
							try
							{
								pacscmd.setCommandText("Update PathologyApp set item7 = 1 where flowId = :flowId");
								pacscmd.Param("flowId").setAsString() = his_sqh;
								
								pacscmd.Execute();
								pacscon.Commit();
							}
							catch(SAException &x)
							{
								try
								{
									pacscon.Rollback();
								}
								catch(SAException &)
								{
								}
								AfxMessageBox((const char*)x.ErrText());
								return;
							}
						}

						str.Format("%.2f", nSum);
						m_pParent->SetDlgItemText(IDC_EDIT_SF, str);
					}
					else
					{
						AfxMessageBox("病人已经出院!");
					}
				}

                if(his_zymzbs.CompareNoCase("门诊") == 0)
				{
					CString  doctor, Mrid, his_Patientid;
					try
					{
						pacscmd.setCommandText("Select DoctorId, Mrid, Patientid from PathologyApp where flowId = :flowId");
						pacscmd.Param("flowId").setAsString() = his_sqh;
						pacscmd.Execute();
						
						if( pacscmd.FetchNext() )
						{
							doctor = pacscmd.Field("DoctorId").asString();
							Mrid   = pacscmd.Field("Mrid").asString();
							his_Patientid = pacscmd.Field("Patientid").asString();
						}
						
						pacscon.Commit();
					}
					catch(SAException &x)
					{
						try
						{
							pacscon.Rollback();
						}
						catch(SAException &)
						{
						}
						AfxMessageBox((const char*)x.ErrText());
					}

					SAString Mz_winprg, Mz_process_NO, dr_winprg;
					Mz_winprg.Empty();  Mz_process_NO.Empty(); dr_winprg.Empty();
					float payment = 0, realpay = 0;
					bool IsRecordFound = false;
					try
					{
						pacscmd.setCommandText("Select dr_winprg, Mz_winprg, Mz_process_NO, payment, realpay from MZ_PRESC_MASTER where dr_winprg like '" + (SAString)his_sqh + "_%' order by timeprg desc");
						pacscmd.Execute();
						
						if( pacscmd.FetchNext() )
						{
							Mz_winprg     = pacscmd.Field("Mz_winprg").asString();
							Mz_process_NO = pacscmd.Field("Mz_process_NO").asString();
							dr_winprg     = pacscmd.Field("dr_winprg").asString();
							payment       = pacscmd.Field("payment").asDouble();
							realpay       = pacscmd.Field("realpay").asDouble();

							IsRecordFound = true;
						}
						
						pacscon.Commit();
					}
					catch(SAException &x)
					{
						try
						{
							pacscon.Rollback();
						}
						catch(SAException &)
						{
						}
						AfxMessageBox((const char*)x.ErrText());
					}

					bool IsNewRecord = false;
					if(IsRecordFound)
					{
						/*
						if(Mz_process_NO.IsEmpty() || Mz_winprg.IsEmpty())
						{
							// use old dr_winprg
							IsNewRecord = false;
						}
						else
						{
							int dr_winprg_xh = atoi(dr_winprg.Right(dr_winprg.GetLength() - dr_winprg.Find("_") - 1));
							dr_winprg_xh++;
							dr_winprg.Format("%s_%d", dr_winprg.Left(dr_winprg.Find("_")), dr_winprg_xh);
							
							payment = 0;
							realpay = 0;

							IsNewRecord = true;
						}*/

						// 为了解决门诊划价误操作和病人钱带的不够无法交费的问题，
						// 系统改成每次点击"确定"就生成新处方，即在MZ_PRESC_MASTER中添加新记录。
						// 不用考虑是否已交费。
						int dr_winprg_xh = atoi(dr_winprg.Right(dr_winprg.GetLength() - dr_winprg.Find("_") - 1));
						dr_winprg_xh++;
						dr_winprg.Format("%s_%d", dr_winprg.Left(dr_winprg.Find("_")), dr_winprg_xh);
						
						payment = 0;
						realpay = 0;
						
						IsNewRecord = true;
					}
					else
					{
						// create first main record
						dr_winprg = his_sqh + "_1";
						IsNewRecord = true;
					}

					for(int index = m_Original_Item_Num; index < SfDetailArray.GetSize(); index++)
					{
						CHARGEITEM_RECORD  tmp = SfDetailArray.GetAt(index);
						realpay += tmp.price;
						payment += (tmp.Num * tmp.SinglePrice);
					}

					try
					{
						SAString cmdstr;
						if(IsNewRecord)
							cmdstr = "Insert into MZ_PRESC_MASTER (patient_name,patient_sex,patient_age,doctor,dr_dept,dr_winprg,dr_process_no,type1,type2,flag_fee,items,payment,realpay,timeprg,flag6,code,type3,patient_no,counts,prescript_no,Gh_Winprg,Gh_process_NO,flag1,flag2,flag3,flag4,flag5) values(:patient_name,:patient_sex,:patient_age,:doctor,'病理科',:dr_winprg,0,'检查费','病理费',0,:items,:payment,:realpay,:timeprg,4,:code,'病理费',:patient_no,1,1,:Gh_Winprg,:Gh_process_NO,0,0,0,0,0)";
						else
							cmdstr = "Update MZ_PRESC_MASTER set payment=:payment,realpay=:realpay where dr_winprg = :dr_winprg";
						pacscmd.setCommandText(cmdstr);
						if(IsNewRecord)
						{
							pacscmd.Param("patient_name").setAsString() = his_Name;
							pacscmd.Param("patient_sex").setAsString()  = his_sex;
							pacscmd.Param("patient_age").setAsString()  = his_age;
							pacscmd.Param("doctor").setAsNumeric()      = doctor;
							pacscmd.Param("items").setAsShort()         = SfDetailArray.GetSize();
							pacscmd.Param("timeprg").setAsDateTime()    = ServerTime;
							pacscmd.Param("code").setAsString()         = dr_winprg;
							pacscmd.Param("patient_no").setAsString()   = Mrid;

							pacscmd.Param("Gh_process_NO").setAsLong()  = atoi(his_Patientid.Left(his_Patientid.Find("_")));
							pacscmd.Param("Gh_Winprg").setAsLong()      = atoi(his_Patientid.Right(his_Patientid.GetLength() - his_Patientid.Find('_') -1));
						}
						pacscmd.Param("dr_winprg").setAsString()    = dr_winprg;
						pacscmd.Param("payment").setAsDouble()      = payment;
						pacscmd.Param("realpay").setAsDouble()      = realpay;
						
						pacscmd.Execute();
						
						pacscon.Commit();
					}
					catch(SAException &x)
					{
						try
						{
							pacscon.Rollback();
						}
						catch(SAException &)
						{
						}
						AfxMessageBox((const char*)x.ErrText());
						return;
					}
				
					for(index = m_Original_Item_Num; index < SfDetailArray.GetSize(); index++)
					{
						CHARGEITEM_RECORD  tmp = SfDetailArray.GetAt(index);
						SAString str;

						try
						{
							SAString cmdstr = "Insert into MZ_PRESC_DETAIL(act_id,prescript_no,itemname,itemid,price,quantity,payment,realpay,adviceid,dr_process_no,dr_dept,dr_winprg,type1,Gh_Winprg,Gh_process_NO,timeprg,unit) values(0,1,:itemname,:itemid,:price,:quantity,:payment,:realpay,:adviceid,0,'病理科',:dr_winprg,'病理费',:Gh_Winprg,:Gh_process_NO,:timeprg,:unit)";
							pacscmd.setCommandText(cmdstr);
							str = tmp.item;
							pacscmd.Param("itemname").setAsString()  = str;
							str = tmp.code;
							pacscmd.Param("itemid").setAsString()    = str;
							pacscmd.Param("price").setAsDouble()     = tmp.SinglePrice;
							pacscmd.Param("quantity").setAsShort()   = tmp.Num;
							pacscmd.Param("payment").setAsDouble()   = tmp.Num * tmp.SinglePrice;
							pacscmd.Param("realpay").setAsDouble()   = tmp.price;
							pacscmd.Param("adviceid").setAsString()  = his_sqh;
							pacscmd.Param("dr_winprg").setAsString() = dr_winprg;
							pacscmd.Param("Gh_process_NO").setAsShort() = atoi(his_Patientid.Left(his_Patientid.Find("_")));
							pacscmd.Param("Gh_Winprg").setAsShort()     = atoi(his_Patientid.Right(his_Patientid.GetLength() - his_Patientid.Find('_') -1));
							pacscmd.Param("timeprg").setAsDateTime()    = ServerTime;
							str = tmp.Unit;
							pacscmd.Param("unit").setAsString() = str;
							
							pacscmd.Execute();
							
							pacscon.Commit();
						}
						catch(SAException &x)
						{
							try
							{
								pacscon.Rollback();
							}
							catch(SAException &)
							{
							}
							AfxMessageBox((const char*)x.ErrText());
							return;
						}
					}

					str.Format("%.2f", nSum);
					m_pParent->SetDlgItemText(IDC_EDIT_SF, str);
				}
			}
			else
			{
				AfxMessageBox("数据库服务器连接失败");
			}

			if(pacscon.isConnected())    pacscon.Disconnect();
		}
		else
		{
			AfxMessageBox("没有找到PACS联结配置文件!");
		}
	}

	CXTCBarDialog::OnOK();
}

void CChargeDetail::OnChargeitemNew() 
{
	// TODO: Add your command handler code here
	CHARGEITEM_RECORD tmp;
	strcpy(tmp.Recorder, theApp.m_Cpp_User);
	SfDetailArray.Add(tmp);
	m_SfDetailCtrl.GridSetup();
}

void CChargeDetail::OnChargeitemDelete() 
{
	int row = m_SfDetailCtrl.GetCurrentRow();
	int CurRecordNum = m_SfDetailCtrl.GetNumberRows();
	
	if( CurRecordNum > 0 && row >= 0 && row < CurRecordNum )
	{
		if(theApp.m_HisCfg.CompareNoCase("山东济宁医附院") == 0)
		{
			if(row < m_Original_Item_Num)
			{
				AfxMessageBox("以前的费用记录不能删改");
				return;
			}
		}

		SfDetailArray.RemoveAt(row);
		m_SfDetailCtrl.GridSetup();
	}
}

BOOL CChargeDetail::OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult )
{
    ASSERT ( pNMHDR->code == TTN_NEEDTEXTA || TTN_NEEDTEXTW );

    TOOLTIPTEXTA* pTTTA = ( TOOLTIPTEXTA *)pNMHDR;
    TOOLTIPTEXTW* pTTTW = ( TOOLTIPTEXTW *)pNMHDR;

    CString strTipText;
    CString strMessage;

    if( GetToolText(pNMHDR->idFrom, strTipText, strMessage))
    {
#ifndef _UNICODE
        if(pNMHDR->code == TTN_NEEDTEXTA)
            lstrcpyn(pTTTA->szText,strTipText,_countof(pTTTA->szText));
        else
            _mbstowcsz(pTTTW->szText,strTipText,_countof(pTTTW->szText));
#else
        if(pNMHDR->code == TTN_NEEDTEXTA)
            _wcstombsz(pTTTA->szText,strTipText,_countof(pTTTA->szText));
        else
            lstrcpyn(pTTTW->szText,strTipText,_countof(pTTTW->szText));

#endif


     // SetMessageText(strMessage); // change status bar message here needs a pointer to CMainFrame
      
		// ok we handled the message, the buck stops here
		return TRUE;

    }

	// we did not handle the message, pass it on
    return CChargeDetail::OnToolTipText(nID,pNMHDR,pResult);

}

BOOL CChargeDetail::GetToolText( UINT nID, CString& strTipText, CString& /*strMessage*/)
{
    CString strFullString;
  
	switch( nID )
	{
	case ID_CHARGEITEM_NEW:
		strTipText.Format("创建新记录");
		break;
	case ID_CHARGEITEM_DELETE:
		strTipText.Format("删除记录");
		break;
	case ID_CHARGEITEM_ALLITEM:
		strTipText.Format("追加所有收费项");
		break;
	}
    return TRUE;
}

void CChargeDetail::OnChargeitemAllitem() 
{
	for(int index = 0; index < ChargeDBArray.GetSize(); index++)
	{
		CHARGEITEM_RECORD tmp = ChargeDBArray.GetAt(index);
		tmp.price = tmp.SinglePrice * tmp.Num;
		if(theApp.m_HisCfg.CompareNoCase("山东济宁医附院") == 0)
		{
			CString pp = tmp.code;
			pp.TrimLeft();  pp.TrimRight();
			for(int i = 0; i < ITEM_CODE_List.GetSize(); i++)
			{
				if(pp.CompareNoCase(ITEM_CODE_List.GetAt(i)) == 0)
				{
					tmp.price *= ParamList.GetAt(i);
					break;
				}
			}
		}

		SfDetailArray.Add(tmp);
	}
	m_SfDetailCtrl.GridSetup();
}

//获取窗口title
CString CChargeDetail::getTitle()
{
	CStringArray rwlyList,countList;

	int sum = 0;
	try
	{
		SAString cmdstr = "select rwly,count(rwly) from "+theApp.TABLE_MATERIAL+" where medicalno=:medicalno group by rwly";
		g_dbcommand.setCommandText(cmdstr);
		g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
		g_dbcommand.Execute();
		
		while(g_dbcommand.FetchNext())
		{
			rwlyList.Add(g_dbcommand.Field(1).asString());
			CString temp; temp.Format("%d",g_dbcommand.Field(2).asLong());
			sum += g_dbcommand.Field(2).asLong();
			countList.Add(temp);
		}

	}
	catch (SAException &e)
	{
		AfxMessageBox((const char*)e.ErrText());
	}

	CString title;
	title = "收费明细表【";
	
	CString temp;temp.Format("%d",sum);
	title = title + "蜡块总数" + temp;
	if(rwlyList.GetSize() > 0)
	{
		title += "：";
	}
	for(int k = 0; k < rwlyList.GetSize(); k++)
	{ 
		title += rwlyList.GetAt(k);
		title += countList.GetAt(k);
		if(k < rwlyList.GetSize() - 1)
		{
			title += "、";		
		}
	}
	title += "】";
	return title;
}
