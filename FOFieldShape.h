#if !defined(FO_FOFIELDSHAPE_H__53197B2A_EAE2_40CD_B034_372344798F14__INCLUDED_)
#define AFC_FOFIELDSHAPE_H__53197B2A_EAE2_40CD_B034_372344798F14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//------------------------------------------------------
// Shape used to load DBF field values
// Author: Ming Delie.
//------------------------------------------------------

class CFOFieldShape : public CFOStaticShape  
{
protected:
	DECLARE_SERIAL(CFOFieldShape);
public:

	// constructor
	CFOFieldShape();

	// Copy constructor.
	CFOFieldShape(const CFOFieldShape& src);

	// Destructor.
	virtual ~CFOFieldShape();

	// Creates the button shape from a CRect object.
	virtual void Create(CRect &rcPos,CString strCaption = _T(""));

	CSize GenTextSize(CDC *pDC, CString value);

public:

	// Assignment operator.
	CFOFieldShape& operator=(const CFOFieldShape& src);

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

	// Frees GDI objects and restores the state of the device context.
	virtual void ClearDC(CDC* pDC);
	// Implementation

public:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};


#endif // !defined(AFC_FOFIELDSHAPE_H__53197B2A_EAE2_40CD_B034_372344798F14__INCLUDED_)
