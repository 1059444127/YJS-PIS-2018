// PathologyDoc.h : interface of the CPathologyDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATHOLOGYDOC_H__A8B1C312_30F1_4F5B_A83F_1B8EBEC92245__INCLUDED_)
#define AFX_PATHOLOGYDOC_H__A8B1C312_30F1_4F5B_A83F_1B8EBEC92245__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPathologyDoc : public CDocument
{
protected: // create from serialization only
	CPathologyDoc();
	DECLARE_DYNCREATE(CPathologyDoc)

// Attributes
public:
    bool m_IsNewRecord;
	CString m_blh;

	// 标记是否新建的申请单
	BOOL m_bNewRegister;
	
	CString GenTitleString();	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPathologyDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void SetTitle(LPCTSTR lpszTitle);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPathologyDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPathologyDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATHOLOGYDOC_H__A8B1C312_30F1_4F5B_A83F_1B8EBEC92245__INCLUDED_)
