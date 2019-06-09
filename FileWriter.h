//==============================================================================================
//	FILE      :	FileWriter.cpp
//	ABSTRACT  :	Manages file output
//	HISTORY   :	2005.11.07 Nikon Corp. - Created
//
//	Copyright (c) 2005, Nikon Corp.  All rights reserved.
//==============================================================================================

#if !defined(AFX_FILEWRITER_H__98F392FC_D1B6_48D4_A020_138E3C23D4C2__INCLUDED_)
#define AFX_FILEWRITER_H__98F392FC_D1B6_48D4_A020_138E3C23D4C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFileWriter  
{
public:
	BOOL WriteBMP(BYTE * pbyBuffer, DWORD dwWidth, DWORD dwHeight);
	BOOL Write(LPBYTE data, DWORD length);
	CFileWriter(LPCTSTR Path);
	virtual ~CFileWriter();

private:
	CFile*	m_File;
};

#endif // !defined(AFX_FILEWRITER_H__98F392FC_D1B6_48D4_A020_138E3C23D4C2__INCLUDED_)
