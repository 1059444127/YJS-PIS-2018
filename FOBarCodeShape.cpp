// FOBarCodeShape.cpp: implementation of the CFOBarCodeShape class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FOBarCodeShape.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CFOBarCodeShape Class
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CFOBarCodeShape, CFOImageShape, 0)
CFOBarCodeShape::CFOBarCodeShape()
{
	//	SetType(Your own ID); you must asign your own ID here.
	SetType(FO_COMP_BARCODE);
	SetOver4ResizeHandles(TRUE);
	SetMultiLine(TRUE);

	m_bAbove = FALSE;
	m_bPrint = TRUE;
	m_strFormat = _T("");
	m_strModul  = _T("");
	m_strRatio  = _T("");
	m_nGuardWidth = 0;
	nIndexBC   = 20;        // init bar code type = Code 128
    nIndexCD   = 1;         // Default check digit method = 1
	m_Orient   = 0;

	t_BarCode*	m_pbarCode;
	BCAlloc (&m_pbarCode);
	m_lf = *BCGetLogFont(m_pbarCode);
	BCFree (m_pbarCode);
}

CFOBarCodeShape::CFOBarCodeShape(const CFOBarCodeShape& src)
{
	m_pImagePtr				=	NULL;
	m_pImagePtrAndMask		=	NULL;
	m_pImagePtrOrMask		=	NULL;
	m_bImageTransparent		=	src.IsImageTransparent();
	m_crImageTransparent	=	src.GetImageTransparentColor();

	m_bAbove      = src.m_bAbove;
	m_bPrint      = src.m_bPrint;
	m_strFormat   = src.m_strFormat;
	m_strModul    = src.m_strModul;
	m_strRatio    = src.m_strRatio;
	m_nGuardWidth = src.m_nGuardWidth;
	nIndexBC      = src.nIndexBC;        // init bar code type = Code 128
    nIndexCD      = src.nIndexCD;         // Default check digit method = 1
	m_Orient      = src.m_Orient;
	m_lf          = src.m_lf;

	// added by me
	SetOver4ResizeHandles(TRUE);
	*this					=	src;
}

CFOBarCodeShape::~CFOBarCodeShape()
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
// CFOBarCodeShape diagnostics

#ifdef _DEBUG
void CFOBarCodeShape::AssertValid() const
{
	CFOImageShape::AssertValid();
}

void CFOBarCodeShape::Dump(CDumpContext& dc) const
{
	CFOImageShape::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CFOBarCodeShape::Create(CRect &rcPos,CString strCaption)
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

CRect CFOBarCodeShape::DoInitImagePos(int nImageWidth,int nImageHeight)
{
	CRect rcTemp;
	rcTemp = CRect(m_rectPosition.left,m_rectPosition.top,m_rectPosition.left+nImageWidth,m_rectPosition.top +nImageHeight);
	return rcTemp;
}

void CFOBarCodeShape::Serialize(CArchive& ar)
{
	CFOImageShape::Serialize(ar);
	if(ar.IsStoring())
	{
		//FODO:Add your own code here.
		ar << m_bAbove;
		ar << m_bPrint;
		ar << m_strFormat;
		ar << m_strModul;
		ar << m_strRatio;
		ar << m_nGuardWidth;
		ar << nIndexBC;
		ar << nIndexCD;
		ar << m_Orient;
		ar << m_lf.lfHeight;
		ar << m_lf.lfWidth;
		ar << m_lf.lfEscapement;
		ar << m_lf.lfOrientation;
		ar << m_lf.lfWeight;
		ar << m_lf.lfItalic;
		ar << m_lf.lfUnderline;
		ar << m_lf.lfStrikeOut;
		ar << m_lf.lfCharSet;
		ar << m_lf.lfOutPrecision;
		ar << m_lf.lfClipPrecision;
		ar << m_lf.lfQuality;
		ar << m_lf.lfPitchAndFamily;
		for(int i = 0; i < LF_FACESIZE; i++ )
		{
			ar << m_lf.lfFaceName[i];
		}
	}
	else
	{
		//FODO:Add your own code here.
		ar >> m_bAbove;
		ar >> m_bPrint;
		ar >> m_strFormat;
		ar >> m_strModul;
		ar >> m_strRatio;
		ar >> m_nGuardWidth;
		ar >> nIndexBC;
		ar >> nIndexCD;
		ar >> m_Orient;
		ar >> m_lf.lfHeight;
		ar >> m_lf.lfWidth;
		ar >> m_lf.lfEscapement;
		ar >> m_lf.lfOrientation;
		ar >> m_lf.lfWeight;
		ar >> m_lf.lfItalic;
		ar >> m_lf.lfUnderline;
		ar >> m_lf.lfStrikeOut;
		ar >> m_lf.lfCharSet;
		ar >> m_lf.lfOutPrecision;
		ar >> m_lf.lfClipPrecision;
		ar >> m_lf.lfQuality;
		ar >> m_lf.lfPitchAndFamily;
		for(int i = 0; i < LF_FACESIZE; i++ )
		{
			ar >> m_lf.lfFaceName[i];
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPrintBaseJob serialization
CFile* CFOBarCodeShape::GetFile(LPCTSTR lpszFileName, UINT nOpenFlags,
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

void CFOBarCodeShape::ReleaseFile(CFile* pFile, BOOL bAbort)
{
	ASSERT_KINDOF(CFile, pFile);
	if (bAbort)
		pFile->Abort(); // will not throw an exception
	else
		pFile->Close();
	delete pFile;
}

BOOL CFOBarCodeShape::OpenDocument(LPCTSTR lpszPathName)
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

BOOL CFOBarCodeShape::SaveDocument(LPCTSTR lpszPathName)
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

CFOBarCodeShape& CFOBarCodeShape::operator=(const CFOBarCodeShape& src)
{
	(*(CFOBarCodeShape*)this) = src;
	return (*this);
}

CFODrawShape* CFOBarCodeShape::Copy() const
{
	return (new CFOBarCodeShape(*this));
}

void CFOBarCodeShape::PrepareDC(CDC* pDC)
{
	CFOImageShape::PrepareDC(pDC);

	//FODO:Add your own code below.
	
}

void CFOBarCodeShape::OnDrawTrackCustom(CDC *pDC)
{
	CFOImageShape::OnDrawTrackCustom(pDC);

	// Add your own code here.

}

void CFOBarCodeShape::OnDrawShadow(CDC *pDC)
{
	CFOImageShape::OnDrawShadow(pDC);
	
	//FODO:Add your own code below.
	
}

void CFOBarCodeShape::OnDrawFlat(CDC *pDC)
{
	CRect rcTemp;
	rcTemp = CRect(m_lpShapePoints[0],m_lpShapePoints[2]);
	rcTemp.NormalizeRect();

	DoDrawImage(pDC,rcTemp);
}

CSize CFOBarCodeShape::GetTextSize(CDC* pDC,CRect rcPos,CString strText)
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

UINT CFOBarCodeShape::GetDrawFormatType()
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

int CFOBarCodeShape::CreateTextArray(CDC* pDC, CString strText,CRect rcBox,CStringArray& arLines)
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

void CFOBarCodeShape::OnDraw3d(CDC *pDC)
{
	CRect rcTemp;
	rcTemp = CRect(m_lpShapePoints[0],m_lpShapePoints[2]);
	rcTemp.NormalizeRect();

	DoDrawImage(pDC,rcTemp);
}

void CFOBarCodeShape::DoDrawImage(CDC *pDC,CRect rcPos)
{
	CString bartext = GetObjectCaption();
	bartext.TrimLeft();  bartext.TrimRight();
	if(!bartext.IsEmpty())
	{
		t_BarCode*	m_pbarCode;
		BCAlloc (&m_pbarCode);
		
		ERRCODE eCode = S_OK;
		
		// get text to encode into the bar code
		BCSetText (m_pbarCode, bartext, bartext.GetLength());
		
		// get the current selected bar code symbology
		// the list box was initialized (filled) according to the enumeration of bar code types
		BCSetBCType(m_pbarCode, (nIndexBC < 0) ? eBC_None : static_cast <e_BarCType>(nIndexBC));
		
		// get the current selected check digit method
		BCSetCDMethod(m_pbarCode, (nIndexCD < 0) ? eCDStandard : static_cast <e_CDMethod>(nIndexCD));
		
		BCSetModWidth	(m_pbarCode, m_strModul); 	// set the module width (unit is 1/1000 mm); "200" = 0.2 mms
		BCSetFormat		(m_pbarCode, m_strFormat);  // format string (optional)
		BCSetRatio		(m_pbarCode, m_strRatio);   // print ratio   (optional)

		///BCSetGuardWidth	(m_pbarCode, m_nGuardWidth);// guard bar width (optional)
		
		BCSetPrintText	(m_pbarCode, m_bPrint, m_bAbove); // print text yes/no
		BCSetMirror		(m_pbarCode, FALSE);              // mirror mode yes/no
		
		switch ( m_Orient )     // orientation of the symbol
		{
		case 0	: BCSetRotation (m_pbarCode, deg0);		break;
		case 1	: BCSetRotation (m_pbarCode, deg90);	break;
		case 2	: BCSetRotation (m_pbarCode, deg180);	break;
		case 3	: BCSetRotation (m_pbarCode, deg270);	break;
		default	: BCSetRotation (m_pbarCode, deg0);		break;
		}
		
		BCSetColorBC (m_pbarCode, RGB(0,0,0));
		BCSetLogFont (m_pbarCode, &m_lf);
		BCSetColorFont (m_pbarCode, RGB(0,0,0));
		
		eCode = (eCode == S_OK) ? BCCheck	(m_pbarCode) : eCode;     // check bar code parameters (MUST)
		eCode = (eCode == S_OK) ? BCCalcCD	(m_pbarCode) : eCode;     // calculate check digit (MUST)
		eCode = (eCode == S_OK) ? BCCreate	(m_pbarCode) : eCode;     // create internal bar code structure (MUST)
		
		if( eCode == S_OK )
		{
			CClientDC   hScreenDC(AfxGetApp()->GetMainWnd());
			
			//BCSetOptResolution(m_pbarCode, TRUE);  // set optimize resolution if "-o" was specified at g_dbcommand line
			
			LOGFONTA* plfFont  = BCGetLogFont(m_pbarCode);
			int nOldFontHeight = plfFont->lfHeight;
			plfFont->lfHeight = -MulDiv(plfFont->lfHeight, 72, hScreenDC.GetDeviceCaps(LOGPIXELSY));
			BCSetLogFont(m_pbarCode, plfFont);
			
			LPPOINT lpShapePoints = GetPoints();
			CRect rcTemp;
			rcTemp = CRect(lpShapePoints[0],lpShapePoints[2]);
			rcTemp.NormalizeRect();
			
			int PIC_WIDTH  = rcTemp.Width();
			int PIC_HEIGHT = rcTemp.Height();
			int m_nHorzRes = hScreenDC.GetDeviceCaps(LOGPIXELSX);			  // ~96 dpi horizontal resolution
			int m_nVertRes = hScreenDC.GetDeviceCaps(LOGPIXELSY);			  // ~96 dpi vertical resolution
			
			BCDraw(m_pbarCode, pDC->GetSafeHdc(), &rcTemp);
			//eCode = BCSaveImage (m_pbarCode, imgpath, eIMBmp, PIC_WIDTH, PIC_HEIGHT, m_nHorzRes, m_nVertRes);
			
			// restore
			plfFont->lfHeight = nOldFontHeight;
			BCSetLogFont(m_pbarCode, plfFont);
		}
		else
		{
			pDC->MoveTo(rcPos.TopLeft());
			pDC->LineTo(rcPos.BottomRight());
			pDC->MoveTo(rcPos.right, rcPos.top);
			pDC->LineTo(rcPos.left, rcPos.bottom);
		}
		
		BCFree (m_pbarCode);
	}
	else
	{
		pDC->MoveTo(rcPos.TopLeft());
		pDC->LineTo(rcPos.BottomRight());
		pDC->MoveTo(rcPos.right, rcPos.top);
		pDC->LineTo(rcPos.left, rcPos.bottom);
	}
}

void CFOBarCodeShape::ClearDC(CDC* pDC)
{
	CFOImageShape::ClearDC(pDC);

	//FODO:Add your own code below.

}

void CFOBarCodeShape::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CFOImageShape::OnLButtonDown(nFlags,point);
	
	//FODO:Add your own code below.

}

void CFOBarCodeShape::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CFOImageShape::OnLButtonUp(nFlags,point);

	//FODO:Add your own code below.

}

void CFOBarCodeShape::GeometryUpdated(CFOArea* pRgn)
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
void CFOBarCodeShape::GetNormalSpotLocation(CArray<CPoint,CPoint>* parSpot)
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

CRect CFOBarCodeShape::GetRect()
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
