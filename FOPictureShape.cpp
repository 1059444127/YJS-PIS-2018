// FOPictureShape.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FOPictureShape.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CFOPictureShape Class
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CFOPictureShape, CFOImageShape, 0)
CFOPictureShape::CFOPictureShape()
{
	//	SetType(Your own ID); you must asign your own ID here.
	SetType(FO_COMP_PICTURE);
	SetOver4ResizeHandles(TRUE);
	SetMultiLine(TRUE);

	m_keepRatio = false;
}

CFOPictureShape::CFOPictureShape(const CFOPictureShape& src)
{
	m_pImagePtr				=	NULL;
	m_pImagePtrAndMask		=	NULL;
	m_pImagePtrOrMask		=	NULL;
	m_bImageTransparent		=	src.IsImageTransparent();
	m_crImageTransparent	=	src.GetImageTransparentColor();

	// added by me
	SetOver4ResizeHandles(TRUE);
	*this					=	src;

	m_keepRatio = false;
}

CFOPictureShape::~CFOPictureShape()
{
	if(m_pImagePtrOrMask != NULL)
	{
		delete m_pImagePtrOrMask;
		m_pImagePtrOrMask = NULL;
	}

	if(m_pImagePtrAndMask != NULL)
	{
		delete m_pImagePtrAndMask;
		m_pImagePtrAndMask = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFOPictureShape diagnostics

#ifdef _DEBUG
void CFOPictureShape::AssertValid() const
{
	CFOImageShape::AssertValid();
}

void CFOPictureShape::Dump(CDumpContext& dc) const
{
	CFOImageShape::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CFOPictureShape::Create(CRect &rcPos,CString strCaption)
{
	AllocMemory(4);

	m_lpShapePoints[0] = CPoint(rcPos.left,  rcPos.top);
	m_lpShapePoints[1] = CPoint(rcPos.right, rcPos.top);
	m_lpShapePoints[2] = CPoint(rcPos.right, rcPos.bottom);
	m_lpShapePoints[3] = CPoint(rcPos.left,  rcPos.bottom);
	SetObjectCaption(strCaption);
	UpdateComp();
	UpdatePosition();
	CreateDefaultPort(0.5,0.5);
	CreateDefaultPort(0,0.5);
	CreateDefaultPort(0.5,1);
	CreateDefaultPort(1,0.5);
	CreateDefaultPort(0.5,0);
}

CRect CFOPictureShape::DoInitImagePos(int nImageWidth,int nImageHeight)
{
	CString strText = GetObjectCaption();
	CRect rcTemp;
	if(strText.IsEmpty())
		rcTemp = CRect(m_rectPosition.left,m_rectPosition.top,m_rectPosition.left+m_rectPosition.Width(),m_rectPosition.top +m_rectPosition.Height());
	else
	{
		nImageHeight += 20;
		rcTemp = CRect(m_rectPosition.left,m_rectPosition.top,m_rectPosition.left+nImageWidth,m_rectPosition.top +nImageHeight);
	}
	return rcTemp;

/*
	nImageHeight += 20;
	CRect rcTemp;
	rcTemp = CRect(m_rectPosition.left,m_rectPosition.top,m_rectPosition.left+nImageWidth,m_rectPosition.top +nImageHeight);
	return rcTemp;
*/
}

void CFOPictureShape::Serialize(CArchive& ar)
{
	CFOImageShape::Serialize(ar);
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
BOOL CFOPictureShape::MingLoadImage(CString strImagePath,COLORREF crTransparent)
{
	ASSERT(AfxCheckMemory());

	CFODrawImageData pImagePointer = new CFODrawImage();
	BOOL bIsLoaded = pImagePointer->m_image.Read(strImagePath);

	if(!bIsLoaded)
	{
		bIsLoaded = pImagePointer->m_image.LoadIconFile(strImagePath,crTransparent);
	}

	if(bIsLoaded)
	{
		pImagePointer->m_strImageDes = strImagePath;

		//m_pImagePtr = m_ImageData.FindImage(pImagePointer);

        m_ImageData.AddTail(pImagePointer);
		m_ImageData.RemoveData();
		m_pImagePtr = pImagePointer;
		
		CRect rcTemp = DoInitImagePos(m_pImagePtr->m_image.GetWidth(),m_pImagePtr->m_image.GetHeight());

		PositionShape(&rcTemp);
	}

	if(m_pImagePtrOrMask != NULL)
	{
		delete m_pImagePtrOrMask;
		m_pImagePtrOrMask = NULL;
	}
	if(m_pImagePtrAndMask != NULL)
	{
		delete m_pImagePtrAndMask;
		m_pImagePtrAndMask = NULL;
	}
	ASSERT(AfxCheckMemory());
	return bIsLoaded;
}

/////////////////////////////////////////////////////////////////////////////
// CPrintBaseJob serialization
CFile* CFOPictureShape::GetFile(LPCTSTR lpszFileName, UINT nOpenFlags,
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

void CFOPictureShape::ReleaseFile(CFile* pFile, BOOL bAbort)
{
	ASSERT_KINDOF(CFile, pFile);
	if (bAbort)
		pFile->Abort(); // will not throw an exception
	else
		pFile->Close();
	delete pFile;
}

BOOL CFOPictureShape::OpenDocument(LPCTSTR lpszPathName)
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

BOOL CFOPictureShape::SaveDocument(LPCTSTR lpszPathName)
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

CFOPictureShape& CFOPictureShape::operator=(const CFOPictureShape& src)
{
	(*(CFOPictureShape*)this) = src;
	return (*this);
}

CFODrawShape* CFOPictureShape::Copy() const
{
	return (new CFOPictureShape(*this));
}

void CFOPictureShape::PrepareDC(CDC* pDC)
{
	CFOImageShape::PrepareDC(pDC);

	//FODO:Add your own code below.
	
}

void CFOPictureShape::OnDrawTrackCustom(CDC *pDC)
{
	CFOImageShape::OnDrawTrackCustom(pDC);

	// Add your own code here.

}

void CFOPictureShape::OnDrawShadow(CDC *pDC)
{
	CFOImageShape::OnDrawShadow(pDC);
	
	//FODO:Add your own code below.
	
}

void CFOPictureShape::OnDrawFlat(CDC *pDC)
{
	CRect rcTemp;
	rcTemp = CRect(m_lpShapePoints[0],m_lpShapePoints[2]);
	rcTemp.NormalizeRect();

	CString strText = GetObjectCaption();
	CSize sizeText = GetTextSize(pDC,rcTemp,strText);

	rcTemp.bottom -= sizeText.cy;
	DoDrawImage(pDC,rcTemp);

	CRect rcText;
	rcText = rcTemp;
	rcText.top = rcTemp.bottom;
	rcText.bottom = rcText.top + sizeText.cy;
	pDC->Rectangle(&rcText);
	DrawText(pDC,GetObjectCaption(),&rcText,GetDrawFormatType());
}

CSize CFOPictureShape::GetTextSize(CDC* pDC,CRect rcPos,CString strText)
{
	CSize sizeText(0,0);
	
	CFont* pFont = NULL;
	CFont* pPrevFont = NULL;
	
	CFOCompProperties* pFontProp = (CFOCompProperties*)GetDefaultProperty();
	if (pFontProp != NULL)
	{
		pFont = pFontProp->GetFont();
		pPrevFont = (CFont*) pDC->SelectObject(pFont);
	}
	
	CStringArray setBreaks;
	CreateTextArray(pDC,strText,rcPos,setBreaks);
	
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	
	for (int nBreak = 0; nBreak < setBreaks.GetSize(); nBreak++)
	{
		CString strLine = setBreaks.GetAt(nBreak);
		CSize sizeLine(0,0);
		
		if (strLine.GetLength() > 0)
		{
			sizeLine = pDC->GetTextExtent(strLine);
		}
		else
		{
			sizeLine.cx = 1;
			sizeLine.cy = tm.tmHeight;
		}
		
		sizeText.cx = FORMMAX(sizeText.cx, sizeLine.cx);
		sizeText.cy += sizeLine.cy;
	}
	
	pDC->SelectObject(pPrevFont);
	
	return sizeText;
}

UINT CFOPictureShape::GetDrawFormatType()
{
	UINT nTextHorz = GetTextHorzAlignment();
	UINT nTextVert = GetTextVertAlignment();
	UINT nAlign;
	if(IsMultiLine())
	{
		nAlign = DT_WORDBREAK;
		switch(nTextHorz)
		{
		case TextLeft: 
			{
				nAlign |= DT_LEFT;
			}
			break;
		case TextMiddle: 
			{
				nAlign |= DT_CENTER;
			}
			break;
		case TextRight:
			{
				nAlign |= DT_RIGHT;
			}
			break;
			
		}
	}
	else
	{
		nAlign = DT_SINGLELINE;
		switch(nTextHorz)
		{
		case TextLeft: 
			{
				nAlign |= DT_LEFT;
			}
			break;
		case TextMiddle: 
			{
				nAlign |= DT_CENTER;
			}
			break;
		case TextRight:
			{
				nAlign |= DT_RIGHT;
			}
			break;
			
		}
		
		switch(nTextVert)
		{
		case TextTop: 
			{
				nAlign |= DT_TOP;
			}
			break;
		case TextCenter: 
			{
				nAlign |= DT_VCENTER;
			}
			break;
		case TextBottom:
			{
				nAlign |= DT_BOTTOM;
			}
			break;
			
		}
	}
	return nAlign;
}

int CFOPictureShape::CreateTextArray(CDC* pDC, CString strText,CRect rcBox,CStringArray& arLines)
{
	CString strBuffer = strText;
	
	LPCTSTR lpszBuffer = strBuffer;
	int nBufLen = strBuffer.GetLength();
	
	UINT nFormat = GetDrawFormatType();
	
	if (nFormat & DT_SINGLELINE)
	{
		arLines.Add(strBuffer);
		return 1;
	}
	
	BOOL bWrapText = FALSE;//nFormat & DT_WORDBREAK;
	
	int nLineWidth = GetLineWidth();
	rcBox.DeflateRect(nLineWidth,nLineWidth,nLineWidth,nLineWidth);
	CRect rcMargins = CRect(4,4,4,4);//GetDrawTextSpace(pDC);
	rcBox.DeflateRect(rcMargins.left, rcMargins.top, rcMargins.right, rcMargins.bottom);
	
	int nRight = CRect(rcBox).Width() + 1;
	
	int nLineBreaks = 0;
	
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	
	CSize szLine;
	int nBufIdx = 0;
	CString strTemp = _T("");
	
	while (nBufIdx < nBufLen)
	{
		int nWordCount = 0;
		int nLineIdx = nBufIdx;
		int nIndexLastWord;
		int nLineLength;
		int nLineLengthLastWord;
		
		nLineLength = 0;
		
		do
		{
			nLineLengthLastWord = nLineLength;
			
			nIndexLastWord = nLineIdx;
			
			while (nLineIdx < nBufLen && lpszBuffer[nLineIdx] == _T(' '))
			{
				nLineIdx++;
				nLineLength++;
			}
			
			while (nLineIdx < nBufLen && _tcschr(_T(" \r\n"), lpszBuffer[nLineIdx]) == NULL)
			{
				nLineLength += _tclen(lpszBuffer + nLineIdx);
				nLineIdx += _tclen(lpszBuffer + nLineIdx);
			}
			
			szLine = pDC->GetTextExtent(lpszBuffer + nBufIdx, nLineLength);
			
			nWordCount++;
			
			while ((nLineIdx) < nBufLen && lpszBuffer[nLineIdx] == _T(' '))
			{
				nLineIdx++;
				nLineLength++;
			}
			
		} while ( (szLine.cx <= nRight || !bWrapText)
			&& nLineIdx < nBufLen && lpszBuffer[nLineIdx] != _T('\r') && lpszBuffer[nLineIdx] != _T('\n'));
		
		rcBox.top += szLine.cy + 1;
		if (lpszBuffer[nLineIdx] == _T('\r'))
		{
			szLine = pDC->GetTextExtent(lpszBuffer + nBufIdx, nLineLength);
			strTemp = strText.Mid(nBufIdx, nLineLength);
			arLines.Add(strTemp);
			nLineBreaks++;
			
			if (lpszBuffer[nBufIdx = nLineIdx + 1] == _T('\n'))
				nBufIdx++;
			
			if (nBufIdx == nBufLen)
			{
				strTemp = strText.Mid(nBufIdx, 0);
				arLines.Add(strTemp);
				nLineBreaks++;
			}
		}
		else if (lpszBuffer[nLineIdx] == _T('\n'))
		{
			szLine = pDC->GetTextExtent(lpszBuffer + nBufIdx, nLineLength);
			strTemp = strText.Mid(nBufIdx, nLineLength);
			arLines.Add(strTemp);
			nLineBreaks++;
			nBufIdx = nLineIdx + 1;
			
			if (nBufIdx == nBufLen)
			{
				strTemp = strText.Mid(nBufIdx, 0);
				arLines.Add(strTemp);
				nLineBreaks++;
			}
		}
		else if (szLine.cx <= nRight)
		{
			nLineBreaks++;
			strTemp = strText.Mid(nBufIdx, nLineLength);
			arLines.Add(strTemp);
			break;
		}
		else if (nWordCount > 1 && bWrapText)
		{
			nLineBreaks++;
			strTemp = strText.Mid(nBufIdx, nLineLengthLastWord);
			arLines.Add(strTemp);
			nBufIdx = nIndexLastWord;
		}
		else
		{
			szLine = pDC->GetTextExtent(lpszBuffer + nBufIdx, nLineLength);
			strTemp = strText.Mid(nBufIdx, nLineLength);
			arLines.Add(strTemp);
			nLineBreaks++;
			nBufIdx = nLineIdx+1;
		}
	}
	
	return nLineBreaks;
}

void CFOPictureShape::OnDraw3d(CDC *pDC)
{
	CRect rcTemp;
	rcTemp = CRect(m_lpShapePoints[0],m_lpShapePoints[2]);
	rcTemp.NormalizeRect();

	CString strText = GetObjectCaption();
	CSize sizeText = GetTextSize(pDC,rcTemp,strText);

	rcTemp.bottom -= sizeText.cy;
	DoDrawImage(pDC,rcTemp);


	CRect rcText;
	rcText = rcTemp;
	rcText.top = rcTemp.bottom;
	rcText.bottom = rcText.top + sizeText.cy;
	pDC->Rectangle(&rcText);
	DrawText(pDC,GetObjectCaption(),&rcText,GetDrawFormatType());

}

void CFOPictureShape::ClearDC(CDC* pDC)
{
	CFOImageShape::ClearDC(pDC);

	//FODO:Add your own code below.

}

void CFOPictureShape::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CFOImageShape::OnLButtonDown(nFlags,point);
	
	//FODO:Add your own code below.

}

void CFOPictureShape::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CFOImageShape::OnLButtonUp(nFlags,point);

	//FODO:Add your own code below.

}

void CFOPictureShape::GeometryUpdated(CFOArea* pRgn)
{
//	CFOImageShape::GeometryUpdated(pRgn);

	//FODO:Add your own code below.
	if (m_lpShapePoints && pRgn)
	{
		pRgn->BuildPolygonArea(m_lpShapePoints, m_nCompPtCount);
		m_rectPosition = pRgn->GetRect();
	}

}

//下面定义组件类的基本操作
void CFOPictureShape::GetNormalSpotLocation(CArray<CPoint,CPoint>* parSpot)
{
	// Clear the list
	parSpot->RemoveAll();

	CRect rcPos = m_CompArea.GetRect();
	CPoint ptCenter = rcPos.CenterPoint();

	// By default, a shape has eight handles
	parSpot->SetSize(8);
	parSpot->SetAt(0, rcPos.TopLeft());
	parSpot->SetAt(1, CPoint(ptCenter.x, rcPos.top));
	parSpot->SetAt(2, CPoint(rcPos.right, rcPos.top));
	parSpot->SetAt(3, CPoint(rcPos.right, ptCenter.y));
	parSpot->SetAt(4, rcPos.BottomRight());
	parSpot->SetAt(5, CPoint(ptCenter.x, rcPos.bottom));
	parSpot->SetAt(6, CPoint(rcPos.left, rcPos.bottom));
	parSpot->SetAt(7, CPoint(rcPos.left, ptCenter.y));
}

CRect CFOPictureShape::GetRect()
{
	//m_rectPosition = GetCompRgn().GetRect();
	CRect rcBounds;

	if (m_nCompPtCount > 0)
	{
		rcBounds.left = rcBounds.right  = m_lpShapePoints[0].x;
		rcBounds.top  = rcBounds.bottom = m_lpShapePoints[0].y;
		
		for (int i=1; i<m_nCompPtCount; i++)
		{
			rcBounds.left   = min(rcBounds.left,   m_lpShapePoints[i].x);
			rcBounds.top    = min(rcBounds.top,    m_lpShapePoints[i].y);
			rcBounds.right  = max(rcBounds.right,  m_lpShapePoints[i].x);
			rcBounds.bottom = max(rcBounds.bottom, m_lpShapePoints[i].y);
		}
	}
	m_rectPosition = rcBounds;
	return m_rectPosition;
}

void CFOPictureShape::DoDrawImage(CDC *pDC,CRect rcPos)
{
	if(m_pImagePtr->m_image.IsValidate())
	{
		extern CRect SizeRectWithConstantAspectRatio(CRect* rcScreen,  CSize  sizePicture,  BOOL   bCenter);
		CRect ScreenWnd;
		ScreenWnd.SetRect( 0 , 0 ,  rcPos.Width()  , rcPos.Height() );
		CSize sizePicture;
		sizePicture.cx = m_pImagePtr->m_image.GetWidth();
		sizePicture.cy = m_pImagePtr->m_image.GetHeight();
		
		if(m_keepRatio)
		{
			ScreenWnd = SizeRectWithConstantAspectRatio(&ScreenWnd,  sizePicture,  TRUE);
		}

		rcPos.left += ScreenWnd.left;
		rcPos.top  += ScreenWnd.top;
		rcPos.right  = rcPos.left + ScreenWnd.Width();
		rcPos.bottom = rcPos.top  + ScreenWnd.Height();
	
		::SetStretchBltMode(pDC->m_hDC , HALFTONE);
		::StretchDIBits(pDC->m_hDC,                           // hDC
			            rcPos.left,                           // DestX
			            rcPos.top,                            // DestY
			            rcPos.Width(),                        // nDestWidth
			            rcPos.Height(),                       // nDestHeight
			            0,                                    // SrcX
			            0,                                    // SrcY
			            m_pImagePtr->m_image.GetWidth(),      // wSrcWidth
			            m_pImagePtr->m_image.GetHeight(),     // wSrcHeight
			            m_pImagePtr->m_image.m_lpBuf,         // lpBits
			            m_pImagePtr->m_image.m_lpBMI,         // lpBitsInfo
			            DIB_RGB_COLORS,                       // wUsage
			            SRCCOPY);                             // dwROP
	}
	else
	{
		pDC->MoveTo(rcPos.TopLeft());
		pDC->LineTo(rcPos.BottomRight());
		pDC->MoveTo(rcPos.right, rcPos.top);
		pDC->LineTo(rcPos.left, rcPos.bottom);
	}
}
