// FOFieldShape.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FOFieldShape.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CFOFieldShape Class
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CFOFieldShape, CFOStaticShape, 0)
CFOFieldShape::CFOFieldShape()
{
	//	SetType(Your own ID); you must asign your own ID here.
	SetType(FO_COMP_FIELD);
	SetTextHorzAlignment(TextLeft);
	SetBkColor(RGB(255,255,255));
	SetNullPen(TRUE);
	SetTransparent(TRUE);
	SetBrushType(0);
	m_pEdit				= NULL;
	m_pParentView		= NULL;
	m_crOldText			= RGB(0,0,0);
	m_bNeedCreate		= TRUE;
	m_nSaveBulleted		= 0;
	SetMultiLine(TRUE);
	m_pTextBoxFont		= NULL;
	m_bAutoJustSize		= TRUE;
	ShowFontProp(TRUE);
	SetOver4ResizeHandles(FALSE);
}

CFOFieldShape::CFOFieldShape(const CFOFieldShape& src)
{
	m_pTextBoxFont		=	NULL;
	m_pEdit				=	NULL;
	m_bNeedCreate		=	TRUE;
	m_nSaveBulleted		=	0;
	m_bAutoJustSize		=	src.IsAutoSize();
	*this				=	src;
}

CFOFieldShape::~CFOFieldShape()
{
	DoEndEdit();
}

/////////////////////////////////////////////////////////////////////////////
// CFOFieldShape diagnostics

#ifdef _DEBUG
void CFOFieldShape::AssertValid() const
{
	CFOStaticShape::AssertValid();
}

void CFOFieldShape::Dump(CDumpContext& dc) const
{
	CFOStaticShape::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CFOFieldShape::Create(CRect &rcPos,CString strCaption)
{
	CFOStaticShape::Create(rcPos,strCaption);

	// FODO: Add your own code here.

}

void CFOFieldShape::Serialize(CArchive& ar)
{
	CFOStaticShape::Serialize(ar);
	if(ar.IsStoring())
	{
		//FODO:Add your own code here.

	}
	else
	{
		//FODO:Add your own code here.

	}
}

/////////////////////////////////////////////////////////////////////////////
// CPrintBaseJob serialization
CFile* CFOFieldShape::GetFile(LPCTSTR lpszFileName, UINT nOpenFlags,
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

void CFOFieldShape::ReleaseFile(CFile* pFile, BOOL bAbort)
{
	ASSERT_KINDOF(CFile, pFile);
	if (bAbort)
		pFile->Abort(); // will not throw an exception
	else
		pFile->Close();
	delete pFile;
}

BOOL CFOFieldShape::OpenDocument(LPCTSTR lpszPathName)
{
	
	CFileException fe;
	CFile* pFile = GetFile(lpszPathName,
		CFile::modeRead|CFile::shareDenyWrite, &fe);
	if (pFile == NULL)
	{
		return FALSE;
	}
	
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

BOOL CFOFieldShape::SaveDocument(LPCTSTR lpszPathName)
{
	
	CFileException fe;
	CFile* pFile = NULL;
	pFile = GetFile(lpszPathName, CFile::modeCreate |
		CFile::modeReadWrite | CFile::shareExclusive, &fe);
	
	if (pFile == NULL)
	{
		
		return FALSE;
	}
	
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

CFOFieldShape& CFOFieldShape::operator=(const CFOFieldShape& src)
{
	//(*(CFOStaticShape*)this) = src;
	(*(CFOFieldShape*)this) = src;
	return (*this);
}

CFODrawShape* CFOFieldShape::Copy() const
{
	return (new CFOFieldShape(*this));
}

void CFOFieldShape::PrepareDC(CDC* pDC)
{
	CFOStaticShape::PrepareDC(pDC);

	//FODO:Add your own code below.
	
}

void CFOFieldShape::OnDrawTrackCustom(CDC *pDC)
{
	CFOStaticShape::OnDrawTrackCustom(pDC);

	// Add your own code here.

}

void CFOFieldShape::OnDrawShadow(CDC *pDC)
{
	CFOStaticShape::OnDrawShadow(pDC);
	
	//FODO:Add your own code below.
	
}

void CFOFieldShape::OnDrawFlat(CDC *pDC)
{
	CFOStaticShape::OnDrawFlat(pDC);

	//FODO:Add your own code below.
	
}

void CFOFieldShape::OnDraw3d(CDC *pDC)
{
	CFOStaticShape::OnDraw3d(pDC);

	//FODO:Add your own code below.

}

void CFOFieldShape::ClearDC(CDC* pDC)
{
	CFOStaticShape::ClearDC(pDC);

	//FODO:Add your own code below.

}

void CFOFieldShape::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CFOStaticShape::OnLButtonDown(nFlags,point);
	
	//FODO:Add your own code below.

}

void CFOFieldShape::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CFOStaticShape::OnLButtonUp(nFlags,point);

	//FODO:Add your own code below.

}

void CFOFieldShape::GeometryUpdated(CFOArea* pRgn)
{
	CFOStaticShape::GeometryUpdated(pRgn);

	//FODO:Add your own code below.

}

CSize CFOFieldShape::GenTextSize(CDC *pDC, CString value)
{
	CRect rcTextBox = CRect(m_lpShapePoints[0],m_lpShapePoints[2]);
	CSize sizeText  = GetTextSize(pDC,rcTextBox,value);
	return sizeText;
}
