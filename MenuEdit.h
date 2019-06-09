   // CMenuEdit : header file
/////////////////////////////////////////////////////////////////////////////

#ifndef __MENUEDIT_H
#define __MENUEDIT_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CMenuEdit class

class CMenuEdit : public CXTBrowseEdit
{
	DECLARE_DYNAMIC(CMenuEdit)

// Construction / destruction
public:
	// Constructs a CMenuEdit object.
	CMenuEdit();

	// Destroys a CMenuEdit object, handles cleanup and de-allocation.
	virtual ~CMenuEdit();

    virtual void PopupMenu();

	CString m_info;

	CString m_TableName;


// Member variables
protected:
   bool mute;
   virtual void PositionBrowseButton(bool bSizeEdit=false);

// Member functions
public:
	bool did(BOOL);

	//{{AFX_VIRTUAL(CMenuEdit)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMenuEdit)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __MENUEDIT_H

