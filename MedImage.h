// CMedImage : header file
/////////////////////////////////////////////////////////////////////////////

#ifndef __MEDIMAGE_H
#define __MEDIMAGE_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CMedImage class

class CMedImage : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CMedImage)

// Construction / destruction
public:
	
	int index;
	CStringArray m_quickInfo;
	bool m_PrinterSelMode;
	CToolTipCtrl     m_ToolTip;
	// Constructs a CMedImage object.
	CMedImage();
	void LoadThumbnail();
	void LoadNewThumb(HDIB, int);
	void ReLoading();
	void SaveModifiedImg(CString);
	void RefreshTabText();

	void LoadImageFromFile(CString sFileName);

	// Destroys a CMedImage object, handles cleanup and de-allocation.
	virtual ~CMedImage();

	void ShowImgItem(int nItem);

	//{{AFX_DATA(CMedImage)
	enum { IDD = IDD_DIALOG_MEDIMAGE };
	CXTButton	m_btPrinter;
	CXTButton	m_btPhoto;
	//}}AFX_DATA

// Member variables
protected:
	BOOL		 m_bDragging;	//T during a drag operation
	CImageList * m_pDragImage;	//For creating and managing the drag-image
	CImageList   m_ImageListThumb;		// image list holding the thumbnails

	int m_Current_Img_Serialno;

	void DeleteListItem();
	void ShowSelectedItem();

    
private:
	bool	m_bIsFirstInit;

	

// Member functions
public:
	void setNotDefault();
	void setDefault();

	//{{AFX_VIRTUAL(CMedImage)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMedImage)
	afx_msg void OnBegindragListThumbnail(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListThumbnail(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnButtonPhoto();
	afx_msg void OnDblclkListThumbnail(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListThumbnail(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPicRotate();
	afx_msg void OnPicMirror();
	afx_msg void OnPicFusion();
	afx_msg void OnPicResample();
	afx_msg void OnPicNote();
	afx_msg void OnPicOutput();
	afx_msg void OnPicExchange();
	afx_msg void OnPicPrint();
	afx_msg void OnPicUnprint();
	afx_msg void OnPicSaveas();
	afx_msg void OnKeydownListThumbnail(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonPrinter();
	afx_msg void OnFullscreen();
	afx_msg void OnNoteQcmx();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __MEDIMAGE_H

