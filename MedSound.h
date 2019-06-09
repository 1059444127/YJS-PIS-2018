   // CMedSound : header file
/////////////////////////////////////////////////////////////////////////////

#ifndef __MEDSOUND_H
#define __MEDSOUND_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CMedSound class

class CMedSound : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CMedSound)

// Construction / destruction
public:

	// Constructs a CMedSound object.
	CMedSound();
	void LoadThumbnail();
	void RefreshTabText();

	// Destroys a CMedSound object, handles cleanup and de-allocation.
	virtual ~CMedSound();

	//{{AFX_DATA(CMedSound)
	enum { IDD = IDD_DIALOG_MEDSOUND };
	CXTButton	m_btStop;
	CXTButton	m_btRecord;
	//}}AFX_DATA

// Member variables
protected:
	BOOL		 m_bDragging;	//T during a drag operation
	CImageList * m_pDragImage;	//For creating and managing the drag-image
	CImageList   m_ImageListThumb;		// image list holding the thumbnails
	void DeleteListItem();
	void ShowSelectedItem();

    CToolTipCtrl     m_ToolTip;
private:
	bool	m_bIsFirstInit;

// Member functions
public:

	//{{AFX_VIRTUAL(CMedSound)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMedSound)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBegindragListThumbnail(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonRecord();
	afx_msg void OnButtonStop();
	afx_msg void OnDblclkListThumbnail(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownListThumbnail(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __MEDSOUND_H

