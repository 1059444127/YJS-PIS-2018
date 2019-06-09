#if !defined(AFX_EXTDATAMODEL_H__FE361D29_EA1B_4DF8_88E6_89543360A300__INCLUDED_)
#define AFX_EXTDATAMODEL_H__FE361D29_EA1B_4DF8_88E6_89543360A300__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtDataModel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExtDataModel window

class CExtDataModel : public CFODataModel
{
protected:
	DECLARE_SERIAL(CExtDataModel);

public:
	// Constructor.
	CExtDataModel();

	// Copy constructor. 
	CExtDataModel(const CExtDataModel& source);

	// Deconstructor.
	virtual ~CExtDataModel();

	int  qLineWidth;
	int  qLineType;
	COLORREF qLineColor;

	FO_FONT StaticFont;
	BOOL m_Static_bMultiLine;
	BOOL m_Static_bHorzAlign;
	BOOL m_Static_bVertAlign;

public:

	// Init model data.
	virtual void DoInitData();

	// Set modified flag.
	virtual void SetModifiedFlag(BOOL bModified = TRUE);

	// Is modified.
	virtual BOOL IsModified();

	// Notify observer wnd.
	virtual void NotifyObserver(LPARAM lHint, CObject*pHint = NULL);

	// Update Title
	virtual void UpdateTitle();

	// Do action change.
	virtual void DoActionChange(const CFOBaseAction* pAction);

	// Do create a new shape.
	// m_drawshape -- Shape Type id start from FO_COMP_CUSTOM
	// rcCreate -- init position of shape.
	// strFileName -- if being image shape,this is the image file name.
	// pCurItem -- if being drag and drop from toolbox,this is the pointer of current toolbox item.
	virtual CFODrawShape *DoCreateShapeByType(UINT m_drawshape,CRect &rcCreate,CString strFileName = _T(""),CFOToolBoxItem *pCurItem = NULL);

	// Do create a new shape shape.
	// m_drawshape -- Shape Type id start from FO_COMP_CUSTOM
	// rcCreate -- init position of shape.
	// strFileName -- if being image shape,this is the image file name.
	// pCurItem -- if being drag and drop from toolbox,this is the pointer of current toolbox item.
	virtual CFOCompositeShape *DoCreateShapeShapeByType(UINT m_drawshape,CRect &rcCreate,
		CArray<FOPORTVALUE,FOPORTVALUE> *arInitPorts,
		CString strFileName = _T(""),
		UINT nResID = 0,
		CFOToolBoxItem *pCurItem = NULL);

	// Draw shape.
	virtual void OnDraw(CDC *pDC);

	// Draw Back ground.
	virtual void OnDrawBack(CDC *pDC);

	// Get Unique Name
	virtual CString GetUniqueName(UINT nType);

	// Get Unique Caption
	virtual CString GetUniqueCaption(UINT nType);

	// Get base shape name.
	virtual CString CreateBaseName(UINT nType);

	// Get base shape caption.
	virtual CString CreateBaseCaption(UINT nType);

	virtual void        AddCloseBezierShape(UINT m_drawshape,CArray<CPoint,CPoint> *arPoints);
	virtual void		AddPolygonShape(UINT m_drawshape,CArray<CPoint,CPoint> *arPoints);

	virtual void		AddLineShape(UINT m_drawshape,
		LPPOINT ppts, 
		int nCount,
		BOOL bArrow = FALSE);
	virtual void		AddFreeLineShape(UINT m_drawshape,
		CArray<CPoint,CPoint> *arPoints,
		BOOL bArrow = TRUE);
	virtual void		AddPolylineShape(UINT m_drawshape,
		CArray<CPoint,CPoint> *arPoints,
		BOOL bArrow = TRUE);
	
	
public:

	// Serialize data to file.
	virtual void Serialize(CArchive &ar);

	// Save Document to a specify file.
	virtual BOOL SaveDocument(LPCTSTR lpszPathName);

	// Open Document from a specify file.
	virtual BOOL OpenDocument(LPCTSTR lpszPathName);

	// Get the pointer of File.
	CFile *		 GetFile(LPCTSTR lpszFileName, UINT nOpenFlags,CFileException* pError);

	// Release File from memory.
	virtual void ReleaseFile(CFile* pFile, BOOL bAbort);

// Implementation
public:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTDATAMODEL_H__FE361D29_EA1B_4DF8_88E6_89543360A300__INCLUDED_)
