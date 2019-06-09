// StatisticMyPreviewView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPreviewView view
class CPrintPreviewDialog;

class CDlgPreviewView : public CPreviewView
{
protected:
	CDlgPreviewView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDlgPreviewView)

// Attributes
public:

protected:
// Operations
	CPrintPreviewDialog*	m_pDialog;
public:
// Overrides
	void	SetDialogPointer(CPrintPreviewDialog* pDialog);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPreviewView)
protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDlgPreviewView();
#ifdef _DEBUG
	virtual void	AssertValid() const;
	virtual void	Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDlgPreviewView)
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
