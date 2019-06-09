// MicroWriterThread.cpp: implementation of the CMicroWriterThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "MicroWriterThread.h"
#include "SyncSerialComm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMicroWriterThread::CMicroWriterThread()
{

}

CMicroWriterThread::~CMicroWriterThread()
{

}

DWORD CMicroWriterThread::ThreadHandler()
{
	CSyncSerialComm com("COM1");

	if(com.Open() != S_OK)
	{
		AfxMessageBox("打开串口失败！");
		commandList->RemoveAll();
		return (DWORD)CThread::DW_OK;
	}

	if(com.ConfigPort(CBR_9600,5) != S_OK)
	{
		AfxMessageBox("初始化串口失败！");
		commandList->RemoveAll();
		return (DWORD)CThread::DW_OK;
	}

	for(int i = 0;i < commandList->GetSize();i++)
	{
		com.Write(commandList->GetAt(i),commandList->GetAt(i).GetLength());
	}
	
	commandList->RemoveAll();
	com.Close();
	return (DWORD)CThread::DW_OK;
}

void CMicroWriterThread::setCommandList(CStringArray* data)
{
	commandList = data;
}


