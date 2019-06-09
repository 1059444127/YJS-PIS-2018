// MicroWriterThread.h: interface for the CMicroWriterThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MICROWRITERTHREAD_H__0B429927_EBD5_4799_AA29_FDDB532A173E__INCLUDED_)
#define AFX_MICROWRITERTHREAD_H__0B429927_EBD5_4799_AA29_FDDB532A173E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Thread.h"

class CMicroWriterThread : public CThread  
{
public:
	CStringArray* commandList;
	
public:
	void setCommandList(CStringArray* data);
	virtual DWORD ThreadHandler();
	CMicroWriterThread();
	virtual ~CMicroWriterThread();

};

#endif // !defined(AFX_MICROWRITERTHREAD_H__0B429927_EBD5_4799_AA29_FDDB532A173E__INCLUDED_)
