// CMyTabCtrlBar : implementation file
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"
#include "MyTabCtrlBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrlBar

CMyTabCtrlBar::CMyTabCtrlBar()
{
	// TODO: add construction code here.

}

CMyTabCtrlBar::~CMyTabCtrlBar()
{
	// TODO: add destruction code here.

}

IMPLEMENT_DYNAMIC(CMyTabCtrlBar, CXTTabCtrlBar)

BEGIN_MESSAGE_MAP(CMyTabCtrlBar, CXTTabCtrlBar)
	//{{AFX_MSG_MAP(CMyTabCtrlBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrlBar message handlers
void CMyTabCtrlBar::SetFloatSize(CSize sizeFloat)
{
    m_bp.sizeFloat = sizeFloat;
}
