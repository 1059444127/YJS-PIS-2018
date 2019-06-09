// MyPreviewView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyPreviewView view
#define MAX_DLLS			50

class CPathologyView;

class CMyPreviewView : public CPreviewView
{
protected:
	CMyPreviewView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyPreviewView)

// Attributes
public:

protected:
// Operations
	CPathologyView*	m_pView;
public:
// Overrides
	void	SetViewPointer(CPathologyView* pView);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyPreviewView)
protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyPreviewView();
#ifdef _DEBUG
	virtual void	AssertValid() const;
	virtual void	Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyPreviewView)
	//}}AFX_MSG
	afx_msg void	OnPreviewPrint();
	afx_msg void	OnNextPage();
	afx_msg void	OnPrevPage();
	afx_msg void	OnZoomIn();
	afx_msg void	OnZoomOut();
	afx_msg void	OnNumPageChange();
protected:
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
