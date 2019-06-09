// CMyTabCtrlBar : header file
/////////////////////////////////////////////////////////////////////////////

#ifndef __MYTABCTRLBAR_H
#define __MYTABCTRLBAR_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrlBar class

class CMyTabCtrlBar : public CXTTabCtrlBar
{
	DECLARE_DYNAMIC(CMyTabCtrlBar)

// Construction / destruction
public:

	// Constructs a CMyTabCtrlBar object.
	CMyTabCtrlBar();

	// Destroys a CMyTabCtrlBar object, handles cleanup and de-allocation.
	virtual ~CMyTabCtrlBar();

// Member variables
protected:

// Member functions
public:
    void SetFloatSize(CSize sizeFloat);

	//{{AFX_VIRTUAL(CMyTabCtrlBar)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMyTabCtrlBar)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __MYTABCTRLBAR_H

