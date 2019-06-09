/////////////////////////////////////////////////////////////////////////////////////
// WORKER THREAD DERIVED CLASS - produced by Worker Thread Class Generator Rel 1.0
// GDIThread.h: Interface for the CGDIThread Class.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GDITHREAD_H__A4C9C0B8_CD6D_11D2_BB7E_37D91443__INCLUDED_)
#define AFX_GDITHREAD_H__A4C9C0B8_CD6D_11D2_BB7E_37D91443__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Thread.h"

class CMainFrame;

class CGDIThread : public CThread  
{
public:
	DECLARE_DYNAMIC(CGDIThread)

// Construction & Destruction
    CGDIThread();
	virtual ~CGDIThread();

// Attributes

   CMainFrame *m_pFrame;
   bool m_Stop_Flag;

	   
// Operations & Overridables
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Abstract 'Draw Entity' method
	virtual bool SingleStep();

protected:
// Overridables
	/////////////////////////////////////////////////////////////////////////////////
	// Main Thread Handler
	// The only method that must be implemented
	virtual DWORD ThreadHandler();
	/////////////////////////////////////////////////////////////////////////////////
	virtual void OnInitThread();
	virtual void OnExitThread();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDITHREAD_H__A4C9C0B8_CD6D_11D2_BB7E_37D91443__INCLUDED_)
