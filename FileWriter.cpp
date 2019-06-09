//==============================================================================================
//	FILE      :	FileWriter.cpp
//	ABSTRACT  :	Manages file output
//	HISTORY   :	2005.11.07 Nikon Corp. - Created
//
//	Copyright (c) 2005, Nikon Corp.  All rights reserved.
//==============================================================================================

#include "stdafx.h"
#include "Pathology.h"
#include "FileWriter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Constructor/Destructor
//////////////////////////////////////////////////////////////////////

//==============================================================================================
//	FUNCTION  :	CFileWriter
//	ABSTRACT  :	Constructor
//	PARAMS    :	Path: path and filename
//	RETURN    :	void
//	NOTE      :	
//	HISTORY   :	2005.11.07 Nikon Corp. - Created
//==============================================================================================
CFileWriter::CFileWriter(LPCTSTR Path)
{
	m_File = NULL;
	try {
		m_File = new CFile(Path, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
	}
	catch (CFileException/** e*/) {
		throw;
	}
}

//==============================================================================================
//	FUNCTION  :	~CDeviceEventSink
//	ABSTRACT  :	Destructor
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	Closes the file handle
//	HISTORY   :	2005.11.07 Nikon Corp. - Created
//==============================================================================================
CFileWriter::~CFileWriter()
{
	if (m_File) {
		m_File->Close();
		delete m_File;
	}
}

//==============================================================================================
//	FUNCTION  :	Write
//	ABSTRACT  :	Writes a data buffer to the currently open file stream
//	PARAMS    :	data: pointer to a write buffer
//			length: length of the write buffer
//	RETURN    : TRUE
//	NOTE      :	
//	HISTORY   :	2005.11.07 Nikon Corp. - Created
//==============================================================================================
BOOL CFileWriter::Write(LPBYTE data, DWORD length)
{
	try {
		m_File->Write(data, (UINT)length);
	}
	catch (CFileException* e) {
		e->Delete();
		return FALSE;
	}
	return TRUE;
}

//==============================================================================================
//	FUNCTION  :	WriteBMP
//	ABSTRACT  :	Writes a data buffer to the currently open file stream
//	PARAMS    :	
//	RETURN    : TRUE
//	NOTE      :	24bit only
//	HISTORY   :	2005.11.07 Nikon Corp. - Created
//==============================================================================================
BOOL CFileWriter::WriteBMP(BYTE * pbyBuffer, DWORD dwWidth, DWORD dwHeight)
{
	DWORD dwLineByte = (((dwWidth*24) + 31) / 32 * 4);

	BITMAPFILEHEADER	tFileHeader;
	BITMAPINFO			tInfoHeader;
	memset(&tFileHeader,0x00,sizeof(BITMAPFILEHEADER));
	memset(&tInfoHeader,0x00,sizeof(BITMAPINFO));

	tFileHeader.bfType	= 'MB';
	tFileHeader.bfSize	= (dwLineByte*dwHeight) + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	tFileHeader.bfReserved1	= 0;
	tFileHeader.bfReserved2	= 0;
	tFileHeader.bfOffBits	= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFO);

	tInfoHeader.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	tInfoHeader.bmiHeader.biWidth		= dwWidth;
	tInfoHeader.bmiHeader.biHeight	= dwHeight;
	tInfoHeader.bmiHeader.biPlanes	= 1;
	tInfoHeader.bmiHeader.biBitCount	= 24;
	tInfoHeader.bmiHeader.biCompression	= BI_RGB;
	tInfoHeader.bmiHeader.biSizeImage	= 0;
	tInfoHeader.bmiHeader.biXPelsPerMeter	= 0;
	tInfoHeader.bmiHeader.biYPelsPerMeter	= 0;
	tInfoHeader.bmiHeader.biClrUsed	= 0;
	tInfoHeader.bmiHeader.biClrImportant	= 0;

	m_File->Write(&tFileHeader,sizeof(BITMAPFILEHEADER));
	m_File->Write(&tInfoHeader,sizeof(BITMAPINFO));

	BYTE* pbyS		= pbyBuffer;
	BYTE* pbyLine	= new BYTE[dwLineByte];
	DWORD dwLine	= dwWidth*3;
	for(DWORD i=0;i<dwHeight;i++){
		CopyMemory(pbyLine,pbyS,dwLine);
		m_File->Write((BYTE *)pbyLine,dwLineByte);
		pbyS+=dwLine;
	}

	delete pbyLine;

	return TRUE;
}
