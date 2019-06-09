// FOBarCodeShape.h: interface for the CFOBarCodeShape class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FOBARCODESHAPE_H__B8EC44F3_E276_4687_8867_66A405EFC98B__INCLUDED_)
#define AFX_FOBARCODESHAPE_H__B8EC44F3_E276_4687_8867_66A405EFC98B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//------------------------------------------------------
// Shape used to display 1D barcode dynamically
// Author: Ming Delie.
//------------------------------------------------------

class CFOBarCodeShape : public CFOImageShape  
{
protected:
	DECLARE_SERIAL(CFOBarCodeShape);
public:
	BOOL	 m_bAbove;
	BOOL	 m_bPrint;
	CString	 m_strFormat;
	CString	 m_strModul;
	CString	 m_strRatio;
	int		 m_nGuardWidth;
	INT		 nIndexBC;
    INT		 nIndexCD;
	int      m_Orient;
	LOGFONTA m_lf;

	// constructor
	CFOBarCodeShape();

	// Copy constructor.
	CFOBarCodeShape(const CFOBarCodeShape& src);

	// Destructor.
	virtual ~CFOBarCodeShape();

	// Creates the button shape from a CRect object.
	virtual void Create(CRect &rcPos,CString strCaption = _T(""));

	// Init image position.
	virtual CRect DoInitImagePos(int nImageWidth,int nImageHeight);

	// Get text box size.
	virtual CSize GetTextSize(CDC* pDC,CRect rcPos,CString strText);

	// Get format type.
	UINT GetDrawFormatType();

	// Calc text line array.
	virtual int CreateTextArray(CDC* pDC, CString strText,CRect rcBox,CStringArray& arLines);

	virtual CRect GetRect();

	// Get the point of control.
	virtual void GetNormalSpotLocation(CArray<CPoint,CPoint>* parSpot);

public:

	// Assignment operator.
	CFOBarCodeShape& operator=(const CFOBarCodeShape& src);

	// Creates a copy of this shape.
	virtual CFODrawShape* Copy() const;

	//Generate Shape Area
	virtual void GeometryUpdated(CFOArea* pRgn);

public:
	
	// WM_LBUTTONDOWN message.
	virtual void OnLButtonDown(UINT nFlags, CPoint point); 

	// WM_LBUTTONUP message.
	virtual void OnLButtonUp(UINT nFlags, CPoint point);

public:

	// Serializes the data.
	virtual void Serialize(CArchive &ar);
	
	// Save document.
	virtual BOOL SaveDocument(LPCTSTR lpszPathName);

	// Open document.
	virtual BOOL OpenDocument(LPCTSTR lpszPathName);
	
	// Get file.
	CFile *GetFile(LPCTSTR lpszFileName, UINT nOpenFlags,CFileException* pError);
	
	// Release file.
	virtual void ReleaseFile(CFile* pFile, BOOL bAbort);

public:

	//Draw flat status.

	// Draws custom tracker.
	virtual void OnDrawTrackCustom(CDC *pDC);

	// Creates GDI objects and sets up the device context for drawing. 
	virtual void PrepareDC(CDC* pDC);

	// Draws the flat status of the shape.
	virtual void OnDrawFlat(CDC *pDC);

	// Draws shadow of shape.
	virtual void OnDrawShadow(CDC *pDC);

	// Draws the 3D status of the shape.
	virtual void OnDraw3d(CDC *pDC);

	// Draw image in a box.
	virtual void DoDrawImage(CDC *pDC,CRect rcPos);

	// Frees GDI objects and restores the state of the device context.
	virtual void ClearDC(CDC* pDC);
	// Implementation

public:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

#endif // !defined(AFX_FOBARCODESHAPE_H__B8EC44F3_E276_4687_8867_66A405EFC98B__INCLUDED_)
