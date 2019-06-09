// ExtDataModel.cpp: implementation of the CFormDataModel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FormDataModel.h"
#include "resource.h"
#include "FOFieldShape.h"
#include "FOPictureShape.h"
#include "FOBarCodeShape.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////
// CFormDataModel construction/destruction

IMPLEMENT_SERIAL(CFormDataModel, CFODataModel, 0)

CFormDataModel::CFormDataModel()
{
	//FOTO:Add you code here.
	qLineWidth = 1;
	qLineType = PS_SOLID;
	qLineColor = RGB(0,0,0);
}

CFormDataModel::CFormDataModel(const CFormDataModel& /*source*/)
{
}

CFormDataModel::~CFormDataModel()
{
	//FOTO:Add you code here.

}

void CFormDataModel::DoInitData()
{
	CFODataModel::DoInitData();

	//FOTO:Add you code here.

}

void CFormDataModel::NotifyObserver(LPARAM lHint, CObject*pHint)
{
	CFODataModel::NotifyObserver(lHint,pHint);

	//FOTO:Add you code here.

}

void CFormDataModel::UpdateTitle()
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

void CFormDataModel::DoActionChange(const CFOBaseAction* pAction)
{
	CFODataModel::DoActionChange(pAction);
	
	// FODO:Add your own specify code here.

}


CString CFormDataModel::CreateBaseName(UINT nType)
{
	switch(nType)
	{
	case FO_COMP_FIELD:
		{
			return FB_FIELD;
		}
		break;

	case FO_COMP_PICTURE:
		{
			return FB_PICTURE;
		}
		break;

	case FO_COMP_BARCODE:
		{
			return FB_BARCODE;
		}
		break;

	default:
		{
			return CFODataModel::CreateBaseName(nType);
		}
		break;
	}

}

CString CFormDataModel::CreateBaseCaption(UINT nType)
{
	switch(nType)
	{
	case FO_COMP_FIELD:
		{
			return FB_FIELD;
		}
		break;

	case FO_COMP_PICTURE:
		{
			return FB_PICTURE;
		}
		break;
	case FO_COMP_BARCODE:
		{
			return FB_BARCODE;
		}
		break;

	default:
		{
			return CFODataModel::CreateBaseCaption(nType);
		}
		break;
	}

}

CString CFormDataModel::GetUniqueName(UINT nType)
{
	return CFODataModel::GetUniqueName(nType);
}

CString CFormDataModel::GetUniqueCaption(UINT nType)
{

	return CFODataModel::GetUniqueCaption(nType);
}

CFODrawShape *CFormDataModel::DoCreateShapeByType(UINT m_drawshape,
														CRect &rcCreate,
														CString strFileName,
														CFOToolBoxItem *pCurItem)
{
	CFODrawShape *pReturn = NULL;
	pReturn = CFODataModel::DoCreateShapeByType(m_drawshape,rcCreate,strFileName);
	if(pReturn != NULL)
	{
		((CFODrawShape *)pReturn)->SetBrushType(PS_SOLID);
		((CFODrawShape *)pReturn)->SetLineColor(qLineColor);
		((CFODrawShape *)pReturn)->SetLineWidth(qLineWidth);
		((CFODrawShape *)pReturn)->SetPenStyle(qLineType);
		return pReturn;
	}

	CString strCaption;
	CString strName;
	strCaption = _T("");
	strName = _T("");
	CPoint ptTrack = rcCreate.TopLeft();

	if(m_drawshape != FO_COMP_NONE)
	{
		CRect rc(rcCreate);
		rc.NormalizeRect();
		if(rc.IsRectEmpty()||(rc.Width() <= 10 && rc.Height() < 20))
		{
			switch( m_drawshape ) {
			case FO_COMP_FIELD:
				{
					rc = CRect(ptTrack.x-25,ptTrack.y-10,ptTrack.x+25,ptTrack.y+11);
				}
				break;
			case FO_COMP_PICTURE:
				{
					rc = CRect(ptTrack.x-70,ptTrack.y-70,ptTrack.x+70,ptTrack.y+70);
				}
				break;
			case FO_COMP_BARCODE:
				{
					rc = CRect(ptTrack.x-70,ptTrack.y-70,ptTrack.x+70,ptTrack.y+70);
				}
				break;
			default:
				break;
			}
		}

		switch( m_drawshape ) {
		case FO_COMP_FIELD:
			{
				pReturn = new CFOFieldShape;
				pReturn->AddRef();
				pReturn->Create(rc,_T(""));
				strCaption = GetUniqueCaption(pReturn->GetType());
				strName = GetUniqueName(pReturn->GetType());
				pReturn->SetObjectCaption(strCaption);
				pReturn->SetObjectName(strName);
				CFOFieldShape *pField = (CFOFieldShape *)pReturn;
				pField->AdjustTextBoxSize(NULL);
			}
			break;
		case FO_COMP_PICTURE:
			{
				pReturn = new CFOPictureShape;
				pReturn->AddRef();
				pReturn->Create(rc,_T(""));

				((CFOImageShape *)pReturn)->LoadImage(IDB_BITMAP_PICTURE);
				
				strCaption = GetUniqueCaption(pReturn->GetType());
				strName = GetUniqueName(pReturn->GetType());
				pReturn->SetObjectCaption(strCaption);
				pReturn->SetObjectName(strName);				
			}
			break;
		case FO_COMP_BARCODE:
			{
				pReturn = new CFOBarCodeShape;
				pReturn->AddRef();
				pReturn->Create(rc,_T(""));

				strCaption.Format("1234567890");
				strName = GetUniqueName(pReturn->GetType());
				pReturn->SetObjectCaption(strCaption);
				pReturn->SetObjectName(strName);				
			}
			break;
		default:
			break;
		}
	}

	return pReturn;
}

void CFormDataModel::AddPolylineShape(UINT m_drawshape,CArray<CPoint,CPoint> *arPoints,BOOL bArrow)
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
	/*
	if(bArrow)
	{
		int nType = 3;
		pReturn->SetEndArrowType(nType);
	}
	*/
    pReturn->SetEndArrowType(FALSE);
	InsertShape(pReturn);
	pReturn->SetLineColor(qLineColor);
	pReturn->SetLineWidth(qLineWidth);
	pReturn->SetPenStyle(qLineType);
	pReturn->Release();
	pReturn=NULL;
	SetModifiedFlag(TRUE);
}

void CFormDataModel::AddLineShape(UINT m_drawshape,LPPOINT pptPoints, int nCount,BOOL bArrow)
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
	}
	InsertShape(pReturn);

	pReturn->SetLineColor(qLineColor);
	pReturn->SetLineWidth(qLineWidth);
	pReturn->SetPenStyle(qLineType);
	pReturn->Release();
	pReturn=NULL;
	SetModifiedFlag(TRUE);
}

void CFormDataModel::AddPolygonShape(UINT m_drawshape,CArray<CPoint,CPoint> *arPoints)
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

	pReturn->SetLineColor(qLineColor);
	pReturn->SetLineWidth(qLineWidth);
	pReturn->SetPenStyle(qLineType);
	pReturn->Release();
	pReturn=NULL;
	SetModifiedFlag(TRUE);
}

CFOCompositeShape *CFormDataModel::DoCreateShapeShapeByType(UINT m_drawshape,
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

	return pReturn;
}


void CFormDataModel::SetModifiedFlag(BOOL b)
{
	CFODataModel::SetModifiedFlag(b);
	
	//FOTO:Add you code here.

}

BOOL CFormDataModel::IsModified()
{

	//FOTO:Add you code here.

	return CFODataModel::IsModified();
}

void CFormDataModel::Serialize(CArchive& ar)
{
	CFODataModel::Serialize(ar);
	if(ar.IsStoring())
	{
		//FOTO:Add you code here.
		ar << m_rcPrintPage;
		ar << m_printDlg.Flags;
		ar << m_printDlg.ptPaperSize;
		ar << m_printDlg.rtMinMargin;
		ar << m_printDlg.rtMargin;

		LPDEVMODE lpSourceMode = (LPDEVMODE) ::GlobalLock(m_printDlg.hDevMode);
		if (lpSourceMode != NULL)
		{
			ar << lpSourceMode->dmPaperSize;
			ar << lpSourceMode->dmOrientation;
			ar << lpSourceMode->dmPaperLength;
			ar << lpSourceMode->dmPaperWidth;
			::GlobalUnlock(m_printDlg.hDevMode);
		}
	}
	else
	{
		//FOTO:Add you code here.
		ar >> m_rcPrintPage;
		ar >> m_printDlg.Flags;
		ar >> m_printDlg.ptPaperSize;
		ar >> m_printDlg.rtMinMargin;
		ar >> m_printDlg.rtMargin;

		LPDEVMODE lpSourceMode = (LPDEVMODE) ::GlobalLock(m_printDlg.hDevMode);
		if (lpSourceMode != NULL)
		{
			ar >> lpSourceMode->dmPaperSize;
			ar >> lpSourceMode->dmOrientation;
			ar >> lpSourceMode->dmPaperLength;
			ar >> lpSourceMode->dmPaperWidth;
			::GlobalUnlock(m_printDlg.hDevMode);
		}
		
		UpdatePrinter();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPrintBaseJob serialization
CFile* CFormDataModel::GetFile(LPCTSTR lpszFileName, UINT nOpenFlags,
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

void CFormDataModel::ReleaseFile(CFile* pFile, BOOL bAbort)
{
	ASSERT_KINDOF(CFile, pFile);
	if (bAbort)
		pFile->Abort(); // will not throw an exception
	else
		pFile->Close();
	delete pFile;
}

BOOL CFormDataModel::OpenDocument(LPCTSTR lpszPathName)
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

BOOL CFormDataModel::SaveDocument(LPCTSTR lpszPathName)
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

void CFormDataModel::OnDraw(CDC *pDC)
{
	CFODataModel::OnDraw(pDC);
	//FOTO:Add you code here.

}

void CFormDataModel::OnDrawBack(CDC *pDC)
{
	CFODataModel::OnDrawBack(pDC);
	//FOTO:Add you code here.

}


/////////////////////////////////////////////////////////////////////////////
// CFormDataModel diagnostics

#ifdef _DEBUG
void CFormDataModel::AssertValid() const
{
	CFODataModel::AssertValid();
}

void CFormDataModel::Dump(CDumpContext& dc) const
{
	CFODataModel::Dump(dc);
}
#endif //_DEBUG

BOOL SetFieldProperty(CFOFieldShape *pObj)
{
	return TRUE;
}

BOOL SetPictureProperty(CFOPictureShape *pObj)
{
	return TRUE;
}

BOOL SetBarcodeProperty(CFOBarCodeShape *pObj)
{
	return TRUE;
}

void CFormDataModel::SetShapePropety(CFODrawShape *pObj)
{
	UINT nType = 0;
	nType = pObj->GetType();

	BOOL  ModifiedFlag = FALSE;
    
	if(nType >= FO_COMP_STATICRECT && nType <= FO_COMP_BEZIERLINK)
	{
		ModifiedFlag = SetDrawShapeProety(pObj);
	}
	else
	{
		if( pObj != NULL )
		{
			if(pObj->IsKindOf(RUNTIME_CLASS(CFOFieldShape)))
			{
				CFOFieldShape *pShape = (CFOFieldShape *)pObj;
				ModifiedFlag = SetFieldProperty(pShape);
			}
			if(pObj->IsKindOf(RUNTIME_CLASS(CFOPictureShape)))
			{
				CFOPictureShape *pShape = (CFOPictureShape *)pObj;
				ModifiedFlag = SetPictureProperty(pShape);
			}
			if(pObj->IsKindOf(RUNTIME_CLASS(CFOBarCodeShape)))
			{
				CFOBarCodeShape *pShape = (CFOBarCodeShape *)pObj;
				ModifiedFlag = SetBarcodeProperty(pShape);
			}
		}
	}

	if( ModifiedFlag )	SetModifiedFlag(TRUE);
}

BOOL SetStaticProperty(CFOStaticShape *pObj)
{
	return TRUE;
}

BOOL SetLineProperty(CFODrawShape *pObj)
{
	return TRUE;
}

BOOL CFormDataModel::SetDrawShapeProety(CFODrawShape *pObj)
{
	if(pObj == NULL)
		return FALSE;

	BOOL  rt = FALSE;
	if(pObj->IsKindOf(RUNTIME_CLASS(CFOStaticShape)))
	{
		CFOStaticShape *pShape = (CFOStaticShape *)pObj;
		rt = SetStaticProperty(pShape);
	}
	else if(pObj->IsKindOf(RUNTIME_CLASS(CFOLineShape)))
	{
		CFOLineShape *pShape = (CFOLineShape *)pObj;
		rt = SetLineProperty(pShape);
	}
	else if(pObj->IsKindOf(RUNTIME_CLASS(CFORectShape)))
	{
		CFORectShape *pShape = (CFORectShape *)pObj;
		rt = SetLineProperty(pShape);
	}
	else if(pObj->IsKindOf(RUNTIME_CLASS(CFOPolygonShape)))
	{
		CFOPolygonShape *pShape = (CFOPolygonShape *)pObj;
		rt = SetLineProperty(pShape);
	}
	else if(pObj->IsKindOf(RUNTIME_CLASS(CFOEllipseShape)))
	{
		CFOEllipseShape *pShape = (CFOEllipseShape *)pObj;
		rt = SetLineProperty(pShape);
	}
	return rt;
}
