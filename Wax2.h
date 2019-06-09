// Wax2.h: interface for the CWax2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAX2_H__8CC901D6_4444_41F4_ADD7_D197E101DC8C__INCLUDED_)
#define AFX_WAX2_H__8CC901D6_4444_41F4_ADD7_D197E101DC8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MedWax.h"

class CWax2 : public CMedWax  
{
public:
	virtual BOOL Create(LPCTSTR s, LPCTSTR s1, DWORD d,const RECT& rect,CWnd* w,UINT i,CCreateContext* c);
	CWax2();
	virtual ~CWax2();

};

#endif // !defined(AFX_WAX2_H__8CC901D6_4444_41F4_ADD7_D197E101DC8C__INCLUDED_)
