// DummyView.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "PathologyDoc.h"
#include "PathologyView.h"
#include "DummyView.h"
#include "MyPreviewView.h"
#include "FOBarCodeShape.h"
#include "StrEdit.h"

#include "FOPictureShape.h"
#include "FOBarCodeShape.h"
#include "FOFieldShape.h"
#include "MyProgressBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDummyView
extern CPathologyApp theApp;

IMPLEMENT_DYNCREATE(CDummyView, CFODrawView)

CDummyView::CDummyView()
{
	m_pView = NULL;
	m_pOldFrame = NULL;

	m_Local_Name_List.RemoveAll();
	m_Local_Value_List.RemoveAll();
}

CDummyView::~CDummyView()
{
	m_pView = NULL;
	m_pOldFrame = NULL;
}


BEGIN_MESSAGE_MAP(CDummyView, CFODrawView)
	//{{AFX_MSG_MAP(CDummyView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDummyView drawing

void CDummyView::OnDraw(CDC* pDC)
{
	Draw(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CDummyView diagnostics

#ifdef _DEBUG
void CDummyView::AssertValid() const
{
	CFODrawView::AssertValid();
}

void CDummyView::Dump(CDumpContext& dc) const
{
	CFODrawView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDummyView message handlers
void CDummyView::SetPrintView(CPathologyView* pView)
{
	m_pView = pView;
}

BOOL CDummyView::OnPreparePrinting(CPrintInfo* pInfo)
{
	if(m_pView)
	{
		if(m_pCurDataModel == NULL)
		{
			SetCurrentModel( m_pView->m_pFormDataModel );
		}
	}

	if(theApp.m_PrintType == 0 || theApp.m_PrintType == 2 || theApp.m_PrintType == 3 || theApp.m_PrintType == 4)
	{
		m_Local_Name_List.RemoveAll();
		m_Local_Value_List.RemoveAll();
		
		CFODrawShapeList *pShapeList = m_pView->m_pFormDataModel->GetFormObjects();
		POSITION pos = pShapeList->GetHeadPosition();
		while ( pos )
		{
			CFODrawShape *pShape = pShapeList->GetNext(pos);
			if( pShape == NULL )   continue;
			if( pShape->GetType() != FO_COMP_PICTURE )  continue;
			m_Local_Name_List.Add( pShape->GetObjectName() );
			m_Local_Value_List.Add(0);
		}
		
		//-----------------------------------------------------------
		int count = 0;
		try
		{
			SAString cmdstr = "Select count(*) from " + theApp.TABLE_IMAGE + " where medicalno = :medicalno and printflag = 1";
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			
			g_dbcommand.Execute();
			if(g_dbcommand.FetchNext())
			{
				count = g_dbcommand.Field(1).asLong();
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
		
		SAString cmdstr;
		if(count >= m_Local_Name_List.GetSize())
			cmdstr = "Select serialno from " + theApp.TABLE_IMAGE + " where medicalno = :medicalno and printflag = 1 order by serialno asc";
		else
			cmdstr = "Select serialno from " + theApp.TABLE_IMAGE + " where medicalno = :medicalno order by serialno asc";
		
		try
		{
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			
			g_dbcommand.Execute();
			int k = 0;
			while(g_dbcommand.FetchNext())
			{
				if(k >= m_Local_Name_List.GetSize())  break;
				m_Local_Value_List.SetAt(k, g_dbcommand.Field("serialno").asLong());
				k++;
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
		//-----------------------------------------------------------
	}

	if(theApp.m_PrintType == 1)
	{
		int m_SkipNum = 0;
		if(theApp.m_BarcodePaperType.CompareNoCase("非连续纸") == 0)
		{
			StrEdit mydlg("请输入第一页需跳过的条码位置数目",this);
			mydlg.m_EditStr = "0";
			if(mydlg.DoModal() == IDOK)
			{
				m_SkipNum = atoi(mydlg.m_EditStr);
				m_SkipNum = (m_SkipNum % theApp.m_BarCodeNum);
			}
		}

		CRect rcPrinter = GetCurrentModel()->GetPrintPosition();

		CArray<CFODrawShape*, CFODrawShape*>  nOriginBarCodeList;
		POSITION pos = m_pView->m_pFormDataModel->GetFormObjects()->GetHeadPosition();
		while(pos != NULL)
		{
			CFODrawShape *pShape = (CFODrawShape *)m_pView->m_pFormDataModel->GetFormObjects()->GetNext(pos);
			if( pShape == NULL )   continue;

			CRect ShapeRect = pShape->GetMaxRect();
			int   ShapeRect_Weight = ShapeRect.top * GetCurrentModel()->GetFormSize().cx + ShapeRect.left;
			for(int i = 0; i < nOriginBarCodeList.GetSize(); i++)
			{
				CFODrawShape *pLp = nOriginBarCodeList.GetAt(i);
				CRect rt = pLp->GetMaxRect();
				int   rt_Weight = rt.top * GetCurrentModel()->GetFormSize().cx + rt.left;
				if(rt_Weight > ShapeRect_Weight)  break;
			}
			if(i >= nOriginBarCodeList.GetSize())
				nOriginBarCodeList.Add(pShape);
			else
				nOriginBarCodeList.InsertAt(i, pShape);
		}
		
		int HorNum = theApp.m_BarCodeNum;
		int VerNum = 1;

		int PageNum = (m_SkipNum + theApp.m_QptmhList.GetSize()) / (HorNum * VerNum);
		if( ((m_SkipNum + theApp.m_QptmhList.GetSize()) % (HorNum * VerNum)) != 0 )
		{
			PageNum++;
		}
		
		GetCurrentModel()->ChangePositionWithPages( 1, PageNum );
		
		CMyProgressBar  *pProgressBar = NULL;
		if(theApp.m_QptmhList.GetSize() > 0)
		{
			pProgressBar = new CMyProgressBar(this);
			pProgressBar->Create();
			pProgressBar->SetActiveWindow();
			pProgressBar->ShowWindow(SW_SHOW);
			pProgressBar->CenterWindow(this);
			
			pProgressBar->m_bar.SetRange(0,theApp.m_QptmhList.GetSize());
			pProgressBar->m_bar.SetPos(0);
			pProgressBar->m_bar.SetStep(1);
			((CStatic *) pProgressBar->GetDlgItem(IDC_STATIC_MSG))->SetWindowText("请稍后...");
		}

		pos = m_pView->m_pFormDataModel->GetFormObjects()->GetHeadPosition();
		CFODrawShape *pShape = (CFODrawShape *)m_pView->m_pFormDataModel->GetFormObjects()->GetNext(pos);
		CSharedFile Memfile;
		CArchive ar(&Memfile, CArchive::store);
		pShape->Serialize(ar);
		ar.Close();

		for(int i = 0; i < (m_SkipNum + theApp.m_QptmhList.GetSize()); i++ )
		{
			CFODrawShape *pNewShape = NULL;
			if( i < theApp.m_BarCodeNum )
			{
				pNewShape = nOriginBarCodeList.GetAt(i);
			}
			else
			{
				int pnum = i / (HorNum * VerNum);
				int k = i % (HorNum * VerNum);
				CRect ps = ((CFODrawShape*)nOriginBarCodeList.GetAt(k))->GetMaxRect();
				ps.top    += (pnum * rcPrinter.Height());
				ps.bottom += (pnum * rcPrinter.Height());

				UINT m_Shape_Type = ((CFODrawShape*)nOriginBarCodeList.GetAt(k))->GetType();

				//------------------------------------------
				if(m_Shape_Type == FO_COMP_GROUP)
				{
					Memfile.SeekToBegin();
					CArchive ar(&Memfile, CArchive::load);
					CFODrawShape *pDuplicateOne = new CFOGroupShape();
					pDuplicateOne->Serialize(ar);
					ar.Close();

					GetCurrentModel()->GetFormObjects()->AddTail(pDuplicateOne);
				}
				else
				{
					CRect MaxRect = ((CFODrawShape*)nOriginBarCodeList.GetAt(k))->GetMaxRect();
					CFODrawShape *pDuplicateOne = GetCurrentModel()->DoCreateShapeByType(m_Shape_Type,MaxRect,"",NULL);
					GetCurrentModel()->GetFormObjects()->AddTail(pDuplicateOne);
				}
				//------------------------------------------
				
				CFODrawShapeList* pMyShapeList = GetCurrentModel()->GetFormObjects();
				POSITION mypos = pMyShapeList->GetTailPosition();
				pNewShape = pMyShapeList->GetPrev(mypos);

				pNewShape->PositionShape( ps );
			}
			
			if(i < m_SkipNum)
			{
				if( pNewShape->GetType() == FO_COMP_GROUP)
				{
					CFODrawShape * pLocalShape = NULL;
					CFODrawShapeList * pShapeList = ((CFOGroupShape *)pNewShape)->GetCompList();
					pos = pShapeList->GetHeadPosition();
					for(int k = 0; k < pShapeList->GetCount(); k ++ )
					{
						pLocalShape = (CFODrawShape *)pShapeList->GetNext(pos);
						pLocalShape->SetVisible(FALSE);
					}
				}
				else
				{
					pNewShape->SetVisible(FALSE);
				}
				continue;
			}
					
			switch( pNewShape->GetType() ) {
			case FO_COMP_BARCODE:
				if(theApp.m_TmhMode.CompareNoCase("是") == 0)
				{
					((CFOBarCodeShape *)pNewShape)->SetObjectCaption(theApp.m_QptmhList.GetAt(i-m_SkipNum).Left(theApp.m_QptmhList.GetAt(i-m_SkipNum).GetLength() - 2));
				}
				else
				{
					((CFOBarCodeShape *)pNewShape)->SetObjectCaption(theApp.m_QptmhList.GetAt(i-m_SkipNum));
				}
				break;
			case FO_COMP_GROUP:
				{
					CFODrawShape * pLocalShape = NULL;
					CFODrawShapeList * pShapeList = ((CFOGroupShape *)pNewShape)->GetCompList();
					pos = pShapeList->GetHeadPosition();
					for(int k = 0; k < pShapeList->GetCount(); k ++ )
					{
						pLocalShape = (CFODrawShape *)pShapeList->GetNext(pos);
						if( pLocalShape->GetType() == FO_COMP_BARCODE )  break;
					}
					if( k <  pShapeList->GetCount() )
					{
						if(theApp.m_TmhMode.CompareNoCase("是") == 0)
						{
							((CFOBarCodeShape *)pLocalShape)->SetObjectCaption(theApp.m_QptmhList.GetAt(i-m_SkipNum).Left(theApp.m_QptmhList.GetAt(i-m_SkipNum).GetLength() - 2));
						}
						else
						{
							((CFOBarCodeShape *)pLocalShape)->SetObjectCaption(theApp.m_QptmhList.GetAt(i-m_SkipNum));
						}
					}

					pos = pShapeList->GetHeadPosition();
					for( k = 0; k < pShapeList->GetCount(); k ++ )
					{
						pLocalShape = (CFODrawShape *)pShapeList->GetNext(pos);
						if( pLocalShape->GetType() == FO_COMP_FIELD )
						{
							if(pLocalShape->GetObjectCaption().CompareNoCase("切片说明") == 0)
							{
								pLocalShape->SetObjectCaption( theApp.m_QpsmList.GetAt(i-m_SkipNum) );
							}
							if(pLocalShape->GetObjectCaption().CompareNoCase("蜡块号") == 0)
							{
								pLocalShape->SetObjectCaption( theApp.m_LkhList.GetAt(i-m_SkipNum) );
							}
							if(pLocalShape->GetObjectCaption().CompareNoCase("病理号") == 0)
							{
								pLocalShape->SetObjectCaption( theApp.m_QptmhList.GetAt(i-m_SkipNum).Left(theApp.m_QptmhList.GetAt(i-m_SkipNum).GetLength() - 2) );
							}
							if(pLocalShape->GetObjectCaption().CompareNoCase("切片编号") == 0)
							{
								pLocalShape->SetObjectCaption( theApp.m_QptmhList.GetAt(i-m_SkipNum));
							}
						}
					}
				}
				break;
			}

			CString msg;
			msg.Format("%d %%", (i-m_SkipNum) * 100 / theApp.m_QptmhList.GetSize());
			((CStatic *) pProgressBar->GetDlgItem(IDC_STATIC_VALUE))->SetWindowText(msg);
			pProgressBar->m_bar.StepIt();
		}

		if((m_SkipNum + theApp.m_QptmhList.GetSize()) < nOriginBarCodeList.GetSize())
		{
			for(i = (m_SkipNum + theApp.m_QptmhList.GetSize()); i < nOriginBarCodeList.GetSize(); i++)
			{
				CFODrawShape *pNewShape = nOriginBarCodeList.GetAt(i);
				if( pNewShape->GetType() == FO_COMP_GROUP)
				{
					CFODrawShape * pLocalShape = NULL;
					CFODrawShapeList * pShapeList = ((CFOGroupShape *)pNewShape)->GetCompList();
					pos = pShapeList->GetHeadPosition();
					for(int k = 0; k < pShapeList->GetCount(); k ++ )
					{
						pLocalShape = (CFODrawShape *)pShapeList->GetNext(pos);
						pLocalShape->SetVisible(FALSE);
					}
				}
				else
				{
					pNewShape->SetVisible(FALSE);
				}
			}
		}
		
		nOriginBarCodeList.RemoveAll();

		Memfile.Detach();

		if (pProgressBar != NULL)
		{
			pProgressBar->DestroyWindow();
			delete pProgressBar;
			pProgressBar = NULL;
		}
	}


	if(theApp.m_PrintType == 1)
	{
		m_pView->m_OnPreparePrinting_Return = CFODrawView::OnPreparePrinting(pInfo);
		return m_pView->m_OnPreparePrinting_Return;
	}
	else
	{
		bool done = false;
		if( pInfo->m_bPreview == FALSE)
		{
			done = true;
			pInfo->m_bPreview = TRUE;
		}
		
		BOOL rt = CFODrawView::OnPreparePrinting(pInfo);
		
		if( done )
		{
			pInfo->m_bPreview = FALSE;
		}
		
		return rt;
	}

	
}

void CDummyView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{

	if(theApp.m_PrintType == 0 || theApp.m_PrintType == 2 || theApp.m_PrintType == 3 || theApp.m_PrintType == 4)
	{
		extern CRect SizeRectWithConstantAspectRatio(CRect* rcScreen,  CSize  sizePicture,  BOOL   bCenter);
		
		//-------------------------------------------------------------
		COleDateTime ServerTime = theApp.GetOleServerDateTime();
		CStringArray Charge_Item_List, Charge_Title_List;
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_CHARGE + " where medicalno = :medicalno";
			
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			
			g_dbcommand.Execute();
			int i = 0;
			while( g_dbcommand.FetchNext() )
			{
				CString str;
				
				str.Format("%s", g_dbcommand.Field("item").asString());
				Charge_Item_List.Add(str);
				
				str.Format("sf_%d_1", i+1);
				Charge_Title_List.Add(str);
				
				str.Format("%.2f", g_dbcommand.Field("price").asDouble());
				Charge_Item_List.Add(str);
				
				str.Format("sf_%d_2", i+1);
				Charge_Title_List.Add(str);
				
				i++;
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
			//AfxMessageBox((const char*)x.ErrText());
		}

		CStringArray Qcmx_Item_List, Qcmx_Title_List;
		try
		{
			SAString cmdstr = "Select * from " + theApp.TABLE_MATERIAL + " where medicalno = :medicalno";
			
			g_dbcommand.setCommandText(cmdstr);
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			
			g_dbcommand.Execute();
			int i = 0;
			while( g_dbcommand.FetchNext() )
			{
				CString str;
				
				Qcmx_Item_List.Add(g_dbcommand.Field("xh").asString());
				
				str.Format("QCMX_%d_1", i+1);
				Qcmx_Title_List.Add(str);
				
				Qcmx_Item_List.Add(g_dbcommand.Field("qcbw").asString());
				
				str.Format("QCMX_%d_2", i+1);
				Qcmx_Title_List.Add(str);

				str.Format("%d", g_dbcommand.Field("cks").asLong());
				Qcmx_Item_List.Add(str);
				
				str.Format("QCMX_%d_3", i+1);
				Qcmx_Title_List.Add(str);

				Qcmx_Item_List.Add(g_dbcommand.Field("qcys").asString());
				
				str.Format("QCMX_%d_4", i+1);
				Qcmx_Title_List.Add(str);

				SADateTime st = g_dbcommand.Field("qcrq").asDateTime();
				COleDateTime st1;
				st1.SetDateTime(st.GetYear(), st.GetMonth(), st.GetDay(), st.GetHour(), st.GetMinute(), st.GetSecond());
				Qcmx_Item_List.Add(st1.Format("%Y-%m-%d %H:%M"));
				
				str.Format("QCMX_%d_5", i+1);
				Qcmx_Title_List.Add(str);
				
				i++;
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
			//AfxMessageBox((const char*)x.ErrText());
		}
		//-------------------------------------------------------------
		
		
		CFODrawShapeList *pShapeList = m_pView->m_pFormDataModel->GetFormObjects();
		POSITION pos = pShapeList->GetHeadPosition();
		while ( pos )
		{
			CFODrawShape *pShape = pShapeList->GetNext(pos);
			if( pShape == NULL )   continue;
			UINT ObjType = pShape->GetType();
			switch( ObjType ) {
			case FO_COMP_FIELD:
				{
					CString value;  value.Empty();
					
					bool rt = theApp.GetDatabaseValue(theApp.GetView()->GetDocument()->m_blh,pShape->GetObjectName(),value,true);

					if(rt && theApp.m_StringFormatMode.CompareNoCase("是") == 0 && (pShape->GetObjectName().CompareNoCase("rysj") == 0 || pShape->GetObjectName().CompareNoCase("gjsj") == 0 || pShape->GetObjectName().CompareNoCase("zdyj") == 0))
					{
						value = "    " + value;
					}
					
					if(rt == false)
					{
						CString imgtitle = pShape->GetObjectCaption();
						int imgno = 0;
						for(int i = 0; i < m_Local_Name_List.GetSize(); i++ )		
						{
							if( imgtitle.CompareNoCase(m_Local_Name_List.GetAt(i)) == 0 )
							{
								imgno = m_Local_Value_List.GetAt(i);
								break;
							}
						}
						if(i < m_Local_Name_List.GetSize() && imgno > 0)
						{
							value.Empty();
							
							try
							{
								SAString cmdstr = "Select imgnote from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno and serialno = :serialno";
								g_dbcommand.setCommandText(cmdstr);
								g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
								g_dbcommand.Param("serialno").setAsLong()    = imgno;
								
								g_dbcommand.Execute();
								if(g_dbcommand.FetchNext())
								{
									value = g_dbcommand.Field("imgnote").asString();
									rt = true;
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
						}
					}
					
					if(rt == false)
					{
						for(int i = 0; i < Charge_Title_List.GetSize(); i++)
						{
							if(Charge_Title_List.GetAt(i).CompareNoCase(pShape->GetObjectCaption()) == 0)  break;
						}
						
						if(i < Charge_Title_List.GetSize())
						{
							value = Charge_Item_List.GetAt(i);
							
							rt = true;
						}
					}

					if(rt == false)
					{
						for(int i = 0; i < Qcmx_Title_List.GetSize(); i++)
						{
							if(Qcmx_Title_List.GetAt(i).CompareNoCase(pShape->GetObjectCaption()) == 0)  break;
						}
						
						if(i < Qcmx_Title_List.GetSize())
						{
							value = Qcmx_Item_List.GetAt(i);
							
							rt = true;
						}
					}
					if(rt == false)
					{
						if(pShape->GetObjectCaption().CompareNoCase("当前时间") == 0)
						{
							COleDateTime curtime = ServerTime;
							value = curtime.Format("%Y-%m-%d %H:%M");

							rt = true;
						}
						if(pShape->GetObjectCaption().CompareNoCase("诊断医师") == 0)
						{
							value.Empty();
							CString shys, zzys, czys;
							theApp.GetDatabaseValue(theApp.GetView()->GetDocument()->m_blh,"shys",shys,true);
							theApp.GetDatabaseValue(theApp.GetView()->GetDocument()->m_blh,"zzys",zzys,true);
							theApp.GetDatabaseValue(theApp.GetView()->GetDocument()->m_blh,"czys",czys,true);
							//-----------------------
							if(czys == zzys || czys == shys)  czys.Empty();
							if(zzys == shys)  zzys.Empty();
							//-----------------------
							if(!shys.IsEmpty())
							{
								if(!value.IsEmpty()) value += "、";
								value += shys;
							}
							if(!zzys.IsEmpty())
							{
								if(!value.IsEmpty()) value += "、";
								value += zzys;
							}
							if(!czys.IsEmpty())
							{
								if(!value.IsEmpty()) value += "、";
								value += czys;
							}

							if(!value.IsEmpty())
							{
								rt = true;
							}
						}

						if(pShape->GetObjectCaption().CompareNoCase("切片数") == 0)
						{
							try
							{
								SAString cmdstr = "select count(*) from " + theApp.TABLE_SLICE + " where medicalno=:medicalno";
								g_dbcommand.setCommandText(cmdstr);
								g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
								g_dbcommand.Execute();
								while(g_dbcommand.FetchNext())
								{
									value.Format("%d",g_dbcommand.Field(1).asLong());
								}
								g_dbconnection.Commit();
							}
							catch(SAException &e)
							{
								AfxMessageBox((const char*)e.ErrText());
							}
							if(!value.IsEmpty())
							{
								rt = true;
							}
						}
						
						if(pShape->GetObjectCaption().CompareNoCase("特检医嘱") == 0)
						{
							value.Empty();
							CString temp;
							try
							{
								SAString cmdstr = "select yzlx,count(*) from " + theApp.TABLE_ADVICE + " where medicalno=:medicalno and yzlx in('免疫组化','特殊染色','分子病理','电镜','其它') group by yzlx";
								g_dbcommand.setCommandText(cmdstr);
								g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
								g_dbcommand.Execute();
								while(g_dbcommand.FetchNext())
								{
									value += g_dbcommand.Field(1).asString();
									temp.Format("%d项、",g_dbcommand.Field(2).asLong());
									value += temp;
								}
								g_dbconnection.Commit();
							}
							catch(SAException &e)
							{
								AfxMessageBox((const char*)e.ErrText());
							}
							if(!value.IsEmpty())
							{
								rt = true;
							}
						}
						
						if(pShape->GetObjectCaption().CompareNoCase("技术医嘱") == 0)
						{
							value.Empty();
							CString temp;
							try
							{
								SAString cmdstr = "select yzlx,count(*) from " + theApp.TABLE_ADVICE + " where medicalno=:medicalno and yzlx in('常规','补取','重切','深切','薄切','连切','切全','不削连切') group by yzlx";
								g_dbcommand.setCommandText(cmdstr);
								g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
								g_dbcommand.Execute();
								while(g_dbcommand.FetchNext())
								{
									value += g_dbcommand.Field(1).asString();
									temp.Format("%d项、",g_dbcommand.Field(2).asLong());
									value += temp;
								}
								g_dbconnection.Commit();
							}
							catch(SAException &e)
							{
								AfxMessageBox((const char*)e.ErrText());
							}
							if(!value.IsEmpty())
							{
								rt = true;
							}
						}

						if(pShape->GetObjectCaption().CompareNoCase("冰冻制片完成时间") == 0)
						{
							value.Empty();
							SADateTime time;
							try
							{
								SAString cmdstr = "select hdrq from " + theApp.TABLE_SLICE + " where medicalno=:medicalno and hdrq is not null and rwly = '冰冻' order by hdrq desc";
								g_dbcommand.setCommandText(cmdstr);
								g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
								g_dbcommand.Execute();
								if(g_dbcommand.FetchNext())
								{
									time = g_dbcommand.Field(1).asDateTime();
									value.Format("%d-%d-%d %d:%d",time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute());
								}
								g_dbconnection.Commit();
							}
							catch (SAException& e)
							{
								AfxMessageBox((const char*)e.ErrText());
							}
							if(!value.IsEmpty())
							{
								rt = true;
							}
						}
					}
					
					if( rt )
					{
						CSize sz = ((CFOFieldShape *)pShape)->GenTextSize(pDC,value);
						CRect ShapeRect = pShape->GetRect();
						if(sz.cx > ShapeRect.Width())
						{
							CSize minsize = ((CFOFieldShape *)pShape)->GenTextSize(pDC,"明");
							CString newstr,newline;
							newstr.Empty();  newline.Empty();
							char enter[3];
							enter[0] = 0x0d; enter[1]= 0x0a; enter[2] = 0x00;
							
							while(!value.IsEmpty())
							{
								if((((CFOFieldShape *)pShape)->GenTextSize(pDC,newline).cx + 2*minsize.cx ) > ShapeRect.Width() )
								{
									BYTE ch = newline.GetAt(newline.GetLength() -1);
									if((ch&0x80) != 0 )
									{
										int k = 0;
										for(int i = (newline.GetLength() -1); i >=0; i-- )
										{
											ch = newline.GetAt(i);
											if((ch&0x80) == 0 )   break;
											k++;
										}
										if((k%2) != 0 )
										{
											value = newline.Right(1) + value;
											newline = newline.Left(newline.GetLength() - 1);
										}
									}
									newline += enter;
									newstr += newline;
									newline.Empty();
								}
								else
								{
									newline += value.Left(1);
									value = value.Right(value.GetLength() - 1);
								}
							}
							if(!newline.IsEmpty())
							{
								newstr += newline;
								newline.Empty();
							}
							pShape->SetObjectCaption( newstr );
						}
						else
						{
							pShape->SetObjectCaption( value );
						}
					}
					else
					{
						pShape->SetVisible(FALSE);
					}
				}
				break;
		    case FO_COMP_PICTURE:
				{
					int imgno = 0;
					CString name = pShape->GetObjectName();
					for(int i = 0; i < m_Local_Name_List.GetSize(); i++ )		
					{
						if( name.CompareNoCase(m_Local_Name_List.GetAt(i)) == 0 )
						{
							imgno = m_Local_Value_List.GetAt(i);
							break;
						}
					}
					
					if(i < m_Local_Name_List.GetSize() && imgno > 0)
					{
						bool ImgLoaded = false;
						try
						{
							SAString cmdstr = "Select imgdataset from " + theApp.TABLE_IMAGE + "  where medicalno = :medicalno and serialno = :serialno";
							g_dbcommand.setCommandText(cmdstr);
							g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
							g_dbcommand.Param("serialno").setAsLong()    = imgno;
							
							g_dbcommand.Execute();
							if(g_dbcommand.FetchNext())
							{
								SAString Bstr = g_dbcommand.Field("imgdataset").asBLob();
								if( !Bstr.IsEmpty() )
								{
									const void * binbuf = (const void *)Bstr;
									BYTE * dataptr = (BYTE *)binbuf;
									
									CString transname;
									transname.Format("%s_%d_cmipstrans.dat", theApp.GetView()->GetDocument()->m_blh, imgno);
									
									CFile file;
									CFileException fe;
									
									CString fstr = theApp.m_SysTmpPath;
									fstr += transname;
									remove(fstr);
									if (file.Open( fstr , CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
									{
										file.WriteHuge( dataptr , Bstr.GetLength());
										file.Close();
									}
									
									//----------------------------------
									GDALDataset * m_pDataset = (GDALDataset *) GDALOpen ( fstr , GA_ReadOnly );
									
									if( m_pDataset != NULL )
									{
										ImgLoaded = true;
										
										HDIB hDIB = ::CreateDIBFile( m_pDataset->GetRasterXSize(), m_pDataset->GetRasterYSize(), 24 );
										LPSTR    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
										BYTE *   lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
										int LineByteWidth = WIDTHBYTES( m_pDataset->GetRasterXSize() * 24 );
										memset( lpDIBBits, 255, LineByteWidth * m_pDataset->GetRasterYSize() );
										
										BYTE *pRBits = lpDIBBits + 2;
										BYTE *pGBits = lpDIBBits + 1;
										BYTE *pBBits = lpDIBBits + 0;
										
										int BandNo[3];
										int BandNum = m_pDataset->GetRasterCount();
										if( BandNum == 1)
										{
											BandNo[0] = 1;	BandNo[1] = 1;	BandNo[2] = 1;
										}
										else
										{
											BandNo[0] = 1;	BandNo[1] = 2;	BandNo[2] = 3;
										}
										
										m_pDataset->GetRasterBand(BandNo[0])->RasterIO( GF_Read, 0, 0, 
											                                            m_pDataset->GetRasterXSize(), 
											                                            m_pDataset->GetRasterYSize(), 
											                                            (void *)(pRBits), 
																						m_pDataset->GetRasterXSize(), 
																						m_pDataset->GetRasterYSize(), 
											                                            GDT_Byte, 
											                                            3 , LineByteWidth );
										m_pDataset->GetRasterBand(BandNo[1])->RasterIO( GF_Read, 0, 0, 
											                                            m_pDataset->GetRasterXSize(), 
											                                            m_pDataset->GetRasterYSize(), 
											                                            (void *)(pGBits), 
																						m_pDataset->GetRasterXSize(), 
																						m_pDataset->GetRasterYSize(), 
											                                            GDT_Byte, 
											                                            3 , LineByteWidth );
										m_pDataset->GetRasterBand(BandNo[2])->RasterIO( GF_Read, 0, 0, 
											                                            m_pDataset->GetRasterXSize(), 
											                                            m_pDataset->GetRasterYSize(), 
											                                            (void *)(pBBits), 
																						m_pDataset->GetRasterXSize(), 
																						m_pDataset->GetRasterYSize(), 
											                                            GDT_Byte, 
											                                            3 , LineByteWidth );
										GDALClose(m_pDataset);
										
										//--------------------------------------------------------------
										if(theApp.m_PrintingMode.CompareNoCase("高反差") == 0 || theApp.m_PrintingMode.CompareNoCase("普通") == 0)
										{
											long hist[257];
											for(i = 0; i < 256; i++)   hist[i] = 0;
											for (i = 10; i < (m_pDataset->GetRasterYSize()-10); i++)
											{       
												BYTE *ptr = lpDIBBits + i * LineByteWidth;
												for (int j = 10; j < (m_pDataset->GetRasterXSize()-10)*3; j++) 
												{
													int grey = *ptr++;
													hist[grey]++;
												}
											}
											hist[256] = (long)(m_pDataset->GetRasterXSize()-20) * (m_pDataset->GetRasterYSize()-20) * 3;
											int sum=0, low=0, high=255;
											for (i = 0; i < 256; i++)	
											{
												sum += hist[i];
												if ((sum*1000/hist[256]>1) && (low==0)) low=i;
												if (sum*1000/hist[256]<999) high=i;
											}
											int Tab[256];    
											for (i = 0; i < 256; i++)
											{
												int grey;
												if(theApp.m_PrintingMode.CompareNoCase("高反差") == 0)
												{
													grey=(int)(((long)i-(long)low)*255l/(long)(high-low));       
													if (grey<0) grey=0;if (grey>255) grey=255;
													Tab[i]=grey;
												}
												if(theApp.m_PrintingMode.CompareNoCase("普通") == 0)
												{
													grey=(int)(((long)i-(long)low)*215l/(long)(high-low))+20;       
													if (grey<0) grey=0;if (grey>255) grey=255;
													Tab[i]=grey;
												}
											}
											for (i=0;i<m_pDataset->GetRasterYSize();i++)
											{  
												BYTE *ptr = lpDIBBits + i * LineByteWidth;
												for (int j=0;j<m_pDataset->GetRasterXSize()*3;j++) *ptr++=Tab[*ptr];
											}
										}

										if(theApp.m_PrintingMode.CompareNoCase("RGB调色") == 0)
										{
											int rvalue = atoi(theApp.m_RedValue);
											int gvalue = atoi(theApp.m_GreenValue);
											int bvalue = atoi(theApp.m_BlueValue);
											for (i=0; i<m_pDataset->GetRasterYSize();i++)
											{  
												BYTE *ptr = lpDIBBits + i * LineByteWidth;
												for (int j=0;j<m_pDataset->GetRasterXSize();j++)
												{
													int mm = ptr[j * 3 + 2];
													mm += rvalue;
													if(mm < 0)   mm = 0;
													if(mm > 255) mm = 255;
													ptr[j * 3 + 2] = mm;

													mm = ptr[j * 3 + 1];
													mm += gvalue;
													if(mm < 0)   mm = 0;
													if(mm > 255) mm = 255;
													ptr[j * 3 + 1] = mm;

													mm = ptr[j * 3 + 0];
													mm += bvalue;
													if(mm < 0)   mm = 0;
													if(mm > 255) mm = 255;
													ptr[j * 3 + 0] = mm;
												}
											}
										}
										//--------------------------------------------------------------
										BYTE *buf = new BYTE[LineByteWidth];
										BYTE *u1,*u2;
										for(int j = 0; j < (m_pDataset->GetRasterYSize()/2); j++ )
										{
											u1 = lpDIBBits + j * LineByteWidth;
											u2 = lpDIBBits + (m_pDataset->GetRasterYSize() - j - 1) * LineByteWidth;
											memcpy(buf, u1,  LineByteWidth);
											memcpy(u1,  u2,  LineByteWidth);
											memcpy(u2,  buf, LineByteWidth);
										}
										delete buf;
													
										::GlobalUnlock((HGLOBAL) hDIB);
										
										CString strText = theApp.m_SysTmpPath;
										strText += "00002.bmp";
										remove(strText);
										
										if ( file.Open( strText, CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
										{
											::SaveDIB( hDIB, file );
											file.Close();
										}
										::GlobalFree((HGLOBAL) hDIB);
										
										if( ((CFOPictureShape *)pShape)->MingLoadImage(strText) == FALSE )
										{
											((CFOPictureShape *)pShape)->SetVisible(FALSE);
										}

										if(theApp.m_PrintingMode1.CompareNoCase("等比例输出") == 0)
										{
											((CFOPictureShape *)pShape)->m_keepRatio = true;
										}
										else
										{
											((CFOPictureShape *)pShape)->m_keepRatio = false;
										}
										remove(strText);
										//----------------------------------
									}
									
									remove(fstr);
								} 
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
					
						if(!ImgLoaded)
						{
							((CFOPictureShape *)pShape)->SetVisible(FALSE);
						}
					}
					else
					{
						((CFOPictureShape *)pShape)->SetVisible(FALSE);
					}
				}
			    break;
		    case FO_COMP_BARCODE:
				{
					CString value;
					bool rt = theApp.GetDatabaseValue(theApp.GetView()->GetDocument()->m_blh,pShape->GetObjectName(),value);
					if( rt )
					{
						((CFOBarCodeShape *)pShape)->SetObjectCaption(value);
					}
				}
				break;
			default:;
			}
		}
	
		m_Local_Name_List.RemoveAll();
		m_Local_Value_List.RemoveAll();
		
		Charge_Item_List.RemoveAll();
		Charge_Title_List.RemoveAll();

		Qcmx_Item_List.RemoveAll();
		Qcmx_Title_List.RemoveAll();
	}

	CFODrawView::OnBeginPrinting(pDC,pInfo);
}

void CDummyView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	CFODrawView::OnPrint(pDC, pInfo);
/*
	pInfo;
	CRect rcUpdate = GetCurrentPrintPosition(pInfo->m_nCurPage);
	CRect rcTruePrint(rcUpdate);
	::LPtoDP(pDC->m_hDC, (LPPOINT) ((LPRECT)rcTruePrint), 2);

	CRgn rgnClip;
	rgnClip.CreateRectRgn(rcTruePrint.TopLeft().x, rcTruePrint.TopLeft().y,
	                      rcTruePrint.BottomRight().x, rcTruePrint.BottomRight().y);
	pDC->SelectClipRgn(&rgnClip);

	// Draw shapes.
	BOOL bDesign = GetCurrentModel()->IsDesignMode();
	GetCurrentModel()->OnDraw(pDC);
	
	// Draw tab order.
	if(bTabOrder)
	{
		if(bDesign)
		{
			GetCurrentModel()->OnDrawTabOrder(pDC);
		}
	}
	
	if(!pDC->IsPrinting())
	{
		if(bDesign && m_bShowSelectHandle)
		{
			DoDrawSelectShapes(pDC);
		}
	}
	
	pDC->SelectClipRgn(NULL);
	DoDrawPrintHeaderAndFooter(pDC,pInfo);
*/
}

void CDummyView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if(theApp.m_PrintType == 0 && theApp.m_Export2ImgMode.CompareNoCase("是") == 0)
	{
		CString fname = theApp.m_SysTmpPath;
		fname += "export2img.jpg";
		remove(fname);

		theApp.Export2Image(GetCurrentModel(), fname);

		SAString  imgdataset;
		imgdataset.Empty();
		CFile file;
		CFileException fe;
		if (file.Open(fname, CFile::modeRead | CFile::shareDenyNone, &fe))
		{
			char sBuf[32*1024];
			while( 1 )
			{
				if( file.GetPosition() >= file.GetLength() )  break;
				DWORD nRead = file.ReadHuge(sBuf, sizeof(sBuf) );
				imgdataset += SAString( sBuf, nRead );
			}
			file.Close();
		}
	
		try
		{
			SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set export2img = :export2img where medicalno = :medicalno ";
			g_dbcommand.setCommandText( cmdstr );
			g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
			g_dbcommand.Param("export2img").setAsBLob()  = imgdataset;
			
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
		
		imgdataset.Empty();

		if(theApp.m_Export2PdfMode.CompareNoCase("是") == 0)
		{
			CString pdffname = theApp.m_SysTmpPath;
			pdffname += "export2pdf.pdf";
			remove(pdffname);

			if(true == theApp.Export2PDF(fname, pdffname))
			{
				SAString  imgdataset;
				imgdataset.Empty();
				CFile file;
				CFileException fe;
				if (file.Open(pdffname, CFile::modeRead | CFile::shareDenyNone, &fe))
				{
					char sBuf[32*1024];
					while( 1 )
					{
						if( file.GetPosition() >= file.GetLength() )  break;
						DWORD nRead = file.ReadHuge(sBuf, sizeof(sBuf) );
						imgdataset += SAString( sBuf, nRead );
					}
					file.Close();
				}
				
				try
				{
					SAString cmdstr = "Update " + theApp.TABLE_MAIN + " set export2pdf = :export2pdf where medicalno = :medicalno ";
					g_dbcommand.setCommandText( cmdstr );
					g_dbcommand.Param("medicalno").setAsString() = theApp.GetView()->GetDocument()->m_blh;
					g_dbcommand.Param("export2pdf").setAsBLob()  = imgdataset;
					
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
				
				imgdataset.Empty();
			}

			remove(pdffname);
		}

		remove(fname);
	}

	CFODrawView::OnEndPrinting(pDC,pInfo);
}

void CDummyView::OnDoPrintPreview()
{
	// In derived classes, implement special window handling here
	// Be sure to Unhook Frame Window close if hooked.

	// must not create this on the frame.  Must outlive this function
	CPrintPreviewState* pState = new CPrintPreviewState;

	// DoPrintPreview's return value does not necessarily indicate that
	// Print preview succeeded or failed, but rather what actions are necessary
	// at this point.  If DoPrintPreview returns TRUE, it means that
	// OnEndPrintPreview will be (or has already been) called and the
	// pState structure will be/has been deleted.
	// If DoPrintPreview returns FALSE, it means that OnEndPrintPreview
	// WILL NOT be called and that cleanup, including deleting pState
	// must be done here.

	if (!DoPrintPreview(AFX_IDD_PREVIEW_TOOLBAR, this,
							RUNTIME_CLASS(CMyPreviewView), pState))
	{
		// In derived classes, reverse special window handling here for
		// Preview failure case

		TRACE0("Error: DoPrintPreview failed.\n");
		AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
		delete pState;      // preview failed to initialize, delete State now
	}
	// the preview view should be the active view, set the pointer for it
	// we need to do this so that the ID_FILE_PRINT command will goto the dialog
	// and not any global view.
	CFrameWnd *pWnd = (CFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMyPreviewView *pMyPreview = (CMyPreviewView*)pWnd->GetActiveView();
	pMyPreview->SetViewPointer(m_pView);
}

void CDummyView::OnDoFilePrint()
{
	// get default print info
	CPrintInfo printInfo;

	ASSERT(printInfo.m_pPD != NULL);    // must be set

	if (LOWORD(GetCurrentMessage()->wParam) == ID_FILE_PRINT_DIRECT)
	{
		CCommandLineInfo* pCmdInfo = AfxGetApp()->m_pCmdInfo;

		if (pCmdInfo != NULL)
		{
			if (pCmdInfo->m_nShellCommand == CCommandLineInfo::FilePrintTo)
			{
				printInfo.m_pPD->m_pd.hDC = ::CreateDC(pCmdInfo->m_strDriverName,
					pCmdInfo->m_strPrinterName, pCmdInfo->m_strPortName, NULL);
				if (printInfo.m_pPD->m_pd.hDC == NULL)
				{
					AfxMessageBox(AFX_IDP_FAILED_TO_START_PRINT);
					return;
				}
			}
		}

		printInfo.m_bDirect = TRUE;
	}

	if (OnPreparePrinting(&printInfo))
	{
		// hDC must be set (did you remember to call DoPreparePrinting?)
		ASSERT(printInfo.m_pPD->m_pd.hDC != NULL);

		// gather file to print to if print-to-file selected
		CString strOutput;
		if (printInfo.m_pPD->m_pd.Flags & PD_PRINTTOFILE && !printInfo.m_bDocObject)
		{
			// construct CFileDialog for browsing
			CString strDef(MAKEINTRESOURCE(AFX_IDS_PRINTDEFAULTEXT));
			CString strPrintDef(MAKEINTRESOURCE(AFX_IDS_PRINTDEFAULT));
			CString strFilter(MAKEINTRESOURCE(AFX_IDS_PRINTFILTER));
			CString strCaption(MAKEINTRESOURCE(AFX_IDS_PRINTCAPTION));
			CFileDialog dlg(FALSE, strDef, strPrintDef,
				OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFilter);
			dlg.m_ofn.lpstrTitle = strCaption;

			if (dlg.DoModal() != IDOK)
				return;

			// set output device to resulting path name
			strOutput = dlg.GetPathName();
		}

		// set up document info and start the document printing process
		CString strTitle;
		CDocument* pDoc = GetDocument();
		if (pDoc != NULL)
			strTitle = pDoc->GetTitle();
		else
			m_pView->GetWindowText(strTitle);
		if (strTitle.GetLength() > 31)
			strTitle.ReleaseBuffer(31);
		DOCINFO docInfo;
		memset(&docInfo, 0, sizeof(DOCINFO));
		docInfo.cbSize = sizeof(DOCINFO);
		docInfo.lpszDocName = strTitle;
		CString strPortName;
		int nFormatID;
		if (strOutput.IsEmpty())
		{
			docInfo.lpszOutput = NULL;
			strPortName = printInfo.m_pPD->GetPortName();
			nFormatID = AFX_IDS_PRINTONPORT;
		}
		else
		{
			docInfo.lpszOutput = strOutput;
			AfxGetFileTitle(strOutput,
				strPortName.GetBuffer(_MAX_PATH), _MAX_PATH);
			nFormatID = AFX_IDS_PRINTTOFILE;
		}

		// setup the printing DC
		CDC dcPrint;
		if (!printInfo.m_bDocObject)
		{
			dcPrint.Attach(printInfo.m_pPD->m_pd.hDC);  // attach printer dc
			dcPrint.m_bPrinting = TRUE;
		}
		OnBeginPrinting(&dcPrint, &printInfo);

		//if (!printInfo.m_bDocObject)
		//	dcPrint.SetAbortProc(AbortProc);

		// disable main window while printing & init printing status dialog
		AfxGetMainWnd()->EnableWindow(FALSE);
		//CPrintingDialog dlgPrintStatus(this);

		//CString strTemp;
		//dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_DOCNAME, strTitle);
		//dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PRINTERNAME,
		//	printInfo.m_pPD->GetDeviceName());
		//AfxFormatString1(strTemp, nFormatID, strPortName);
		//dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PORTNAME, strTemp);
		//dlgPrintStatus.ShowWindow(SW_SHOW);
		//dlgPrintStatus.UpdateWindow();

		// start document printing process
		if (!printInfo.m_bDocObject && dcPrint.StartDoc(&docInfo) == SP_ERROR)
		{
			// enable main window before proceeding
			AfxGetMainWnd()->EnableWindow(TRUE);

			// cleanup and show error message
			OnEndPrinting(&dcPrint, &printInfo);
			//dlgPrintStatus.DestroyWindow();
			dcPrint.Detach();   // will be cleaned up by CPrintInfo destructor
			AfxMessageBox(AFX_IDP_FAILED_TO_START_PRINT);
			return;
		}

		// Guarantee values are in the valid range
		UINT nEndPage = printInfo.GetToPage();
		UINT nStartPage = printInfo.GetFromPage();

		if (nEndPage < printInfo.GetMinPage())
			nEndPage = printInfo.GetMinPage();
		if (nEndPage > printInfo.GetMaxPage())
			nEndPage = printInfo.GetMaxPage();

		if (nStartPage < printInfo.GetMinPage())
			nStartPage = printInfo.GetMinPage();
		if (nStartPage > printInfo.GetMaxPage())
			nStartPage = printInfo.GetMaxPage();

		int nStep = (nEndPage >= nStartPage) ? 1 : -1;
		nEndPage = (nEndPage == 0xffff) ? 0xffff : nEndPage + nStep;

		//VERIFY(strTemp.LoadString(AFX_IDS_PRINTPAGENUM));

		// If it's a doc object, we don't loop page-by-page
		// because doc objects don't support that kind of levity.

		BOOL bError = FALSE;
		if (printInfo.m_bDocObject)
		{
			OnPrepareDC(&dcPrint, &printInfo);
			OnPrint(&dcPrint, &printInfo);
		}
		else
		{
			// begin page printing loop
			for (printInfo.m_nCurPage = nStartPage;
				printInfo.m_nCurPage != nEndPage; printInfo.m_nCurPage += nStep)
			{
				OnPrepareDC(&dcPrint, &printInfo);

				// check for end of print
				if (!printInfo.m_bContinuePrinting)
					break;

				// write current page
				//TCHAR szBuf[80];
				//wsprintf(szBuf, strTemp, printInfo.m_nCurPage);
				//dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PAGENUM, szBuf);

				// set up drawing rect to entire page (in logical coordinates)
				printInfo.m_rectDraw.SetRect(0, 0,
					dcPrint.GetDeviceCaps(HORZRES),
					dcPrint.GetDeviceCaps(VERTRES));
				dcPrint.DPtoLP(&printInfo.m_rectDraw);

				// attempt to start the current page
				if (dcPrint.StartPage() < 0)
				{
					bError = TRUE;
					break;
				}

				// must call OnPrepareDC on newer versions of Windows because
				// StartPage now resets the device attributes.
				if (afxData.bMarked4)
					OnPrepareDC(&dcPrint, &printInfo);

				ASSERT(printInfo.m_bContinuePrinting);

				// page successfully started, so now render the page
				OnPrint(&dcPrint, &printInfo);
				if (dcPrint.EndPage() < 0)// || !AbortProc(dcPrint.m_hDC, 0))
				{
					bError = TRUE;
					break;
				}
			}
		}

		// cleanup document printing process
		if (!printInfo.m_bDocObject)
		{
			if (!bError)
				dcPrint.EndDoc();
			else
				dcPrint.AbortDoc();
		}

		AfxGetMainWnd()->EnableWindow();    // enable main window

		OnEndPrinting(&dcPrint, &printInfo);    // clean up after printing
		//dlgPrintStatus.DestroyWindow();

		dcPrint.Detach();   // will be cleaned up by CPrintInfo destructor
	}
}

void CDummyView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView)
{
	CFODrawView::OnEndPrintPreview(pDC, pInfo, point, pView);
	// show the original frame
	m_pOldFrame->ShowWindow(SW_SHOW);
	// restore main frame pointer
	AfxGetApp()->m_pMainWnd = m_pOldFrame;
	m_pView->EndPreviewMode();
	m_pView->SetFocus();
	GetParentFrame()->DestroyWindow();
}

void CDummyView::OnInitialUpdate() 
{
	CFODrawView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	
}
