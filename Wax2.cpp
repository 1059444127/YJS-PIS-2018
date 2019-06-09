// Wax2.cpp: implementation of the CWax2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "Wax2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWax2::CWax2()
{

}

CWax2::~CWax2()
{

}

BOOL CWax2::Create(LPCTSTR s, LPCTSTR s1, DWORD d,const RECT& rect,CWnd* w,UINT i,CCreateContext* c)
{
	return Create(s,s1,d,rect,w,i,c);
}
