// ExtDataModel.cpp: implementation of the CExtDataModel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExtDataModel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtDataModel construction/destruction

IMPLEMENT_SERIAL(CExtDataModel, CFODataModel, 0)

CExtDataModel::CExtDataModel()
{
	//FOTO:Add you code here.
	qLineWidth = 2;
	qLineType = PS_SOLID;
	qLineColor = RGB(255,255,0);

	StaticFont.m_strFaceName = "ËÎÌו";
	StaticFont.m_nPointSize  = 18;
	StaticFont.m_crColor     = RGB(255,255,0);
	StaticFont.m_bItalic     = 0;
	StaticFont.m_bUnderline  = 0;
	StaticFont.m_nWeight     = 400;

	m_Static_bMultiLine      = 1;
	m_Static_bHorzAlign      = 1;
	m_Static_bVertAlign      = 1;

	SetPointLimitWithPage(TRUE);
}

CExtDataModel::CExtDataModel(const CExtDataModel& /*source*/)
{
}

CExtDataModel::~CExtDataModel()
{
	//FOTO:Add you code here.

}

void CExtDataModel::DoInitData()
{
	CFODataModel::DoInitData();

	//FOTO:Add you code here.

}

void CExtDataModel::NotifyObserver(LPARAM lHint, CObject*pHint)
{
	CFODataModel::NotifyObserver(lHint,pHint);

	//FOTO:Add you code here.

}

void CExtDataModel::UpdateTitle()
{
	CFODataModel::UpdateTitle();

//	if(m_pOwner == NULL)
//	{
//		return;
//	}
//
//	if(m_pOwner->GetSafeHwnd() != NULL)
//	{
//		if(m_pOwner ->IsKindOf(RUNTIME_CLASS(CFODrawView)))
//		{
//			CFODrawView *pView = (CFODrawView *) m_pOwner;
//			CDocument *m_pDocument = (CDocument *)pView->GetDocument();
//			if(m_pDocument != NULL)
//			{
//				if (m_strTitleOrg.IsEmpty() && !m_pDocument->GetTitle().IsEmpty())
//					m_strTitleOrg = m_pDocument->GetTitle();
//				
//				if (!m_strTitleOrg.IsEmpty())
//				{
//					TCHAR szStr[256+_MAX_PATH];
//					_tcscpy(szStr, m_strTitleOrg);
//					if (IsDirty())
//					{
//						if(m_strTitleOrg.Find(_T("*"))<0)
//						{
//							lstrcat(szStr, _T(" *"));
//						}
//					}
//					if (m_pDocument->GetTitle() != szStr)
//					{
//						m_pDocument->SetTitle(szStr);
//						m_pDocument->UpdateFrameCounts();
//					}
//				}
//			}
//		}
//	}
}

void CExtDataModel::DoActionChange(const CFOBaseAction* pAction)
{
	CFODataModel::DoActionChange(pAction);
	
	// FODO:Add your own specify code here.

}


CString CExtDataModel::CreateBaseName(UINT nType)
{
	/*switch(nType)
	{
	case MY_DROPMENUBUTTON:
		{
			return _T("DropMenuButton");
		}
		break;

	case MY_DROPCOLORBUTTON:
		{
			return _T("DropColorButton");
		}
		break;

	default:
		{
			return CFODataModel::CreateBaseName(nType);
		}
		break;
	}*/
	
	return _T("");
}

CString CExtDataModel::CreateBaseCaption(UINT nType)
{
	/*switch(nType)
	{
	case MY_DROPMENUBUTTON:
		{
			return _T("Drop Menu Button");
		}
		break;

	case MY_DROPCOLORBUTTON:
		{
			return _T("Color");
		}
		break;

	default:
		{
			return CFODataModel::CreateBaseCaption(nType);
		}
		break;
	}*/

	return _T("");
}

CString CExtDataModel::GetUniqueName(UINT nType)
{
	return CFODataModel::GetUniqueName(nType);
}

CString CExtDataModel::GetUniqueCaption(UINT nType)
{

	return CFODataModel::GetUniqueCaption(nType);
}

CFODrawShape *CExtDataModel::DoCreateShapeByType(UINT m_drawshape,
														CRect &rcCreate,
														CString strFileName,
														CFOToolBoxItem *pCurItem)
{
	CFODrawShape *pReturn = NULL;
	/*CString strCaption;
	CString strName;
	strCaption = "";
	strName = "";
	CPoint ptTrack = rcCreate.TopLeft();
	if(m_drawshape != FO_COMP_NONE)
	{
		CRect rc(rcCreate);
		rc.NormalizeRect();
		if(rc.IsRectEmpty()||(rc.Width()<=10 && rc.Height()<20))
		{
			switch(m_drawshape)
			{
			case MY_DROPMENUBUTTON:
				{
					rc = CRect(ptTrack.x-80,ptTrack.y-15,ptTrack.x+80,ptTrack.y+15);
				}
				break;

			case MY_DROPCOLORBUTTON:
				{
					rc = CRect(ptTrack.x-80,ptTrack.y-15,ptTrack.x+80,ptTrack.y+15);
				}
				break;
				
			default:
				{
				}
				break;
			}
		}

		switch(m_drawshape)
		{
		case MY_DROPMENUBUTTON:
			{
				pReturn = new CMyDropMenuButtonShape;
				pReturn->AddRef();
				pReturn->Create(rc,"");
				pReturn->UpdatePosition();
				strCaption = GetUniqueCaption(pReturn->GetType());
				strName = GetUniqueName(pReturn->GetType());
				pReturn->SetObjectCaption(strCaption);
				pReturn->SetObjectName(strName);
			}
			break;
			
		case MY_DROPCOLORBUTTON:
			{
				pReturn = new CMyColorPickerShape;
				pReturn->AddRef();
				pReturn->Create(rc,"");
				pReturn->UpdatePosition();
				strCaption = GetUniqueCaption(pReturn->GetType());
				strName = GetUniqueName(pReturn->GetType());
				pReturn->SetObjectCaption(strCaption);
				pReturn->SetObjectName(strName);
			}
			break;

		default:
			{*/
				pReturn = CFODataModel::DoCreateShapeByType(m_drawshape,rcCreate,strFileName);
			/*}
			break;
		}
	}*/
				if(pReturn != NULL)
				{
					((CFODrawShape *)pReturn)->SetBrushType(PS_SOLID);
					//((CFODrawShape *)pReturn)->SetLineColor(RGB(255,255,255)); //COLORREF cr= RGB(255,255,255)

					((CFODrawShape *)pReturn)->SetLineColor(qLineColor);
					((CFODrawShape *)pReturn)->SetLineWidth(qLineWidth);
					((CFODrawShape *)pReturn)->SetPenStyle(qLineType);

					if(((CFODrawShape *)pReturn)->GetType() == FO_COMP_STATIC)
					{
/*
						((CFOStaticShape *)pReturn)->SetFaceName("ËÎÌו");
						((CFOStaticShape *)pReturn)->SetPointSize(18);
						((CFOStaticShape *)pReturn)->SetFontColor(RGB(255,255,0));
						((CFOStaticShape *)pReturn)->SetItalic(0);
						((CFOStaticShape *)pReturn)->SetUnderline(0);
						((CFOStaticShape *)pReturn)->SetWeight(400);
						
						((CFOStaticShape *)pReturn)->SetMultiLine(1);
						((CFOStaticShape *)pReturn)->SetTextHorzAlignment(1);
						((CFOStaticShape *)pReturn)->SetTextVertAlignment(1);
*/
						((CFOStaticShape *)pReturn)->SetFaceName(StaticFont.m_strFaceName);
						((CFOStaticShape *)pReturn)->SetPointSize(StaticFont.m_nPointSize);
						((CFOStaticShape *)pReturn)->SetFontColor(StaticFont.m_crColor);
						((CFOStaticShape *)pReturn)->SetItalic(StaticFont.m_bItalic);
						((CFOStaticShape *)pReturn)->SetUnderline(StaticFont.m_bUnderline);
						((CFOStaticShape *)pReturn)->SetWeight(StaticFont.m_nWeight);
						
						((CFOStaticShape *)pReturn)->SetMultiLine(m_Static_bMultiLine);
						((CFOStaticShape *)pReturn)->SetTextHorzAlignment(m_Static_bHorzAlign);
						((CFOStaticShape *)pReturn)->SetTextVertAlignment(m_Static_bVertAlign);
						
						((CFOStaticShape *)pReturn)->AdjustTextBoxSize(NULL);
					}
				}
				
	return pReturn;
}

CFOCompositeShape *CExtDataModel::DoCreateShapeShapeByType(UINT m_drawshape,
																  CRect &rcCreate,
																  CArray<FOPORTVALUE,FOPORTVALUE> *arInitPorts,
																  CString strFileName,
																  UINT nResID,
																  CFOToolBoxItem *pCurItem)
{
	CFOCompositeShape *pReturn = CFODataModel::DoCreateShapeShapeByType(m_drawshape,
															rcCreate,
															arInitPorts,
															strFileName,
															nResID,
															pCurItem);
	// FODO: Add your own specify code here.
	pReturn->LockWidthAndHeight(TRUE);

	return pReturn;
}


void CExtDataModel::SetModifiedFlag(BOOL b)
{
	CFODataModel::SetModifiedFlag(b);
	
	//FOTO:Add you code here.

}

BOOL CExtDataModel::IsModified()
{

	//FOTO:Add you code here.

	return CFODataModel::IsModified();
}

void CExtDataModel::Serialize(CArchive& ar)
{
	CFODataModel::Serialize(ar);
	if(ar.IsStoring())
	{
		//FOTO:Add you code here.

	}
	else
	{
		//FOTO:Add you code here.

	}
}

/////////////////////////////////////////////////////////////////////////////
// CPrintBaseJob serialization
CFile* CExtDataModel::GetFile(LPCTSTR lpszFileName, UINT nOpenFlags,
							 CFileException* pError)
{
	CMirrorFile* pFile = new CMirrorFile;
	ASSERT(pFile != NULL);
	if (!pFile->Open(lpszFileName, nOpenFlags, pError))
	{
		delete pFile;
		pFile = NULL;
	}
	return pFile;
}

void CExtDataModel::ReleaseFile(CFile* pFile, BOOL bAbort)
{
	ASSERT_KINDOF(CFile, pFile);
	if (bAbort)
		pFile->Abort(); // will not throw an exception
	else
		pFile->Close();
	delete pFile;
}

BOOL CExtDataModel::OpenDocument(LPCTSTR lpszPathName)
{
	CFileException fe;
	CFile* pFile = GetFile(lpszPathName,
		CFile::modeRead|CFile::shareDenyWrite, &fe);
	if (pFile == NULL)
	{
		return FALSE;
	}
	strCurOpenFormFile = CString(lpszPathName);
	CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_bForceFlat = FALSE;
	TRY
	{
		CWaitCursor wait;
		if (pFile->GetLength() != 0)
			Serialize(loadArchive);     // load me
		loadArchive.Close();
		ReleaseFile(pFile, FALSE);
	}
	CATCH_ALL(e)
	{
		ReleaseFile(pFile, TRUE);
		
		return FALSE;
	}
	END_CATCH_ALL
		
		
	return TRUE;
}

BOOL CExtDataModel::SaveDocument(LPCTSTR lpszPathName)
{
	CFileException fe;
	CFile* pFile = NULL;
	pFile = GetFile(lpszPathName, CFile::modeCreate |
		CFile::modeReadWrite | CFile::shareExclusive, &fe);
	
	if (pFile == NULL)
	{
		
		return FALSE;
	}
	strCurOpenFormFile = CString(lpszPathName);
	CArchive saveArchive(pFile, CArchive::store | CArchive::bNoFlushOnDelete);
	saveArchive.m_bForceFlat = FALSE;
	TRY
	{
		CWaitCursor wait;
		Serialize(saveArchive);     // save me
		saveArchive.Close();
		ReleaseFile(pFile, FALSE);
	}
	CATCH_ALL(e)
	{
		ReleaseFile(pFile, TRUE);
		
		return FALSE;
	}
	END_CATCH_ALL
		
	return TRUE;        // success
}

void CExtDataModel::OnDraw(CDC *pDC)
{
	CFODataModel::OnDraw(pDC);
	//FOTO:Add you code here.

}

void CExtDataModel::OnDrawBack(CDC *pDC)
{
	CFODataModel::OnDrawBack(pDC);
	//FOTO:Add you code here.

}


/////////////////////////////////////////////////////////////////////////////
// CExtDataModel diagnostics

#ifdef _DEBUG
void CExtDataModel::AssertValid() const
{
	CFODataModel::AssertValid();
}

void CExtDataModel::Dump(CDumpContext& dc) const
{
	CFODataModel::Dump(dc);
}
#endif //_DEBUG

void CExtDataModel::AddPolygonShape(UINT m_drawshape,CArray<CPoint,CPoint> *arPoints)
{
	m_drawshape;
	CString strCaption;
	CString strName;
	strCaption = _T("");
	strName = _T("");
	
	CFOPolygonShape *pReturn = new CFOPolygonShape;
	pReturn->AddRef();
	pReturn->Create(arPoints);
	pReturn->UpdatePosition();
	strCaption = GetUniqueCaption(FO_COMP_POLYGON);
	strName = GetUniqueName(FO_COMP_POLYGON);
	pReturn->SetObjectCaption(strCaption);
	pReturn->SetObjectName(strName);
	InsertShape(pReturn);
	pReturn->SetBrushType(PS_SOLID);
	//pReturn->SetLineColor(RGB(255,255,255));
	pReturn->SetLineColor(qLineColor);
	pReturn->SetLineWidth(qLineWidth);
	pReturn->SetPenStyle(qLineType);
	pReturn->Release();
	pReturn=NULL;
	SetModifiedFlag(TRUE);
}

void CExtDataModel::AddCloseBezierShape(UINT m_drawshape,CArray<CPoint,CPoint> *arPoints)
{
	m_drawshape;
	CString strCaption;
	CString strName;
	strCaption = _T("");
	strName = _T("");
	
	CFOCloseBezierShape *pReturn = new CFOCloseBezierShape;
	pReturn->AddRef();
	pReturn->Create(arPoints);
	pReturn->UpdatePosition();
	strCaption = GetUniqueCaption(FO_COMP_CLOSEBEZIER);
	strName = GetUniqueName(FO_COMP_CLOSEBEZIER);
	pReturn->SetObjectCaption(strCaption);
	pReturn->SetObjectName(strName);
	InsertShape(pReturn);
	pReturn->SetBrushType(PS_SOLID);
	pReturn->Release();
	pReturn=NULL;
	SetModifiedFlag(TRUE);
}

void CExtDataModel::AddLineShape(UINT m_drawshape,LPPOINT pptPoints, int nCount,BOOL bArrow)
{
	m_drawshape;
	CString strCaption;
	CString strName;
	strCaption = _T("");
	strName = _T("");
	
	CFOLineShape *pReturn = new CFOLineShape;
	pReturn->AddRef();
	pReturn->Create(pptPoints,nCount);
	pReturn->UpdatePosition();
	strCaption = GetUniqueCaption(FO_COMP_LINE);
	strName = GetUniqueName(FO_COMP_LINE);
	pReturn->SetObjectCaption(strCaption);
	pReturn->SetObjectName(strName);
	if(bArrow)
	{
		int nType = 3;
		pReturn->SetEndArrowType(nType);

		//------------------------------------
	    CFOCompProperties *prop = (CFOCompProperties *)pReturn->FindProperty(FO_DEFAULT_PROP_ID);
	    CFOCompProperties *propNew = (CFOCompProperties *)prop->Copy();
		propNew->SetLineColor(qLineColor);
		propNew->SetLineWidth(qLineWidth);
		propNew->SetPenStyle(qLineType);
		propNew->SetBkColor(qLineColor);
		propNew->SetEndArrowType(nType);
		propNew->SetType(FO_COMP_ARROWLINE);

		CFOCompsPropAction *pAction = new CFOCompsPropAction(this,pReturn);
		pAction->SetCurrentProp(propNew);
		this->Do(pAction);
		if(propNew != NULL)
		{
			delete propNew;
			propNew = NULL;
		}
		//------------------------------------
	}
	InsertShape(pReturn);
	//pReturn->SetLineColor(RGB(255,255,255));
	pReturn->SetLineColor(qLineColor);
	pReturn->SetLineWidth(qLineWidth);
	pReturn->SetPenStyle(qLineType);
	pReturn->Release();
	pReturn=NULL;
	SetModifiedFlag(TRUE);
}

void CExtDataModel::AddFreeLineShape(UINT m_drawshape,CArray<CPoint,CPoint> *arPoints,BOOL bArrow)
{
	m_drawshape;
	CString strCaption;
	CString strName;
	strCaption = _T("");
	strName = _T("");
	
	CFOFreeLineShape *pReturn = new CFOFreeLineShape;
	pReturn->AddRef();
	pReturn->Create(arPoints);
	pReturn->UpdatePosition();
	strCaption = GetUniqueCaption(FO_COMP_FREELINE);
	strName = GetUniqueName(FO_COMP_FREELINE);
	pReturn->SetObjectCaption(strCaption);
	pReturn->SetObjectName(strName);
	if(bArrow)
	{
		int nType = 3;
		pReturn->SetEndArrowType(nType);
	}
	InsertShape(pReturn);
	//pReturn->SetLineColor(RGB(255,255,255));
	pReturn->SetLineColor(qLineColor);
	pReturn->SetLineWidth(qLineWidth);
	pReturn->SetPenStyle(qLineType);
	pReturn->Release();
	pReturn=NULL;
	SetModifiedFlag(TRUE);
}

void CExtDataModel::AddPolylineShape(UINT m_drawshape,CArray<CPoint,CPoint> *arPoints,BOOL bArrow)
{
	m_drawshape;
	CString strCaption;
	CString strName;
	strCaption = _T("");
	strName = _T("");
	
	CFOLineShape *pReturn = new CFOLineShape;
	pReturn->AddRef();
	pReturn->Create(arPoints);
	pReturn->UpdatePosition();
	strCaption = GetUniqueCaption(FO_COMP_LINE);
	strName = GetUniqueName(FO_COMP_LINE);
	pReturn->SetObjectCaption(strCaption);
	pReturn->SetObjectName(strName);
	/* MingDL20030721*/
	/*if(bArrow)
	{
		int nType = 3;
		pReturn->SetEndArrowType(nType);
	}*/
	InsertShape(pReturn);
	//pReturn->SetLineColor(RGB(255,255,255));
	pReturn->SetLineColor(qLineColor);
	pReturn->SetLineWidth(qLineWidth);
	pReturn->SetPenStyle(qLineType);
	pReturn->Release();
	pReturn=NULL;
	SetModifiedFlag(TRUE);
}
