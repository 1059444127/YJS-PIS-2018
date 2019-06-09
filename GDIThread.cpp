/////////////////////////////////////////////////////////////////////////////////////
// WORKER THREAD DERIVED CLASS - produced by Worker Thread Class Generator Rel. 1.0
// GDIThread.cpp: Implementation of the CGDIThread Class.
/////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "GDIThread.h"

#include "MainFrm.h"
#include "okapi32.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CGDIThread, CThread)

/////////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////////////////////
CGDIThread::CGDIThread()
{
	// support Thread-Handler-Oriented Synchronization
	SUPPORT_THREAD_SYNCHRONIZATION(CGDIThread)
	// support Notificable Thread
	SUPPORT_THREAD_NOTIFICATION

	m_pFrame    = NULL;
	m_Stop_Flag = false;
}

CGDIThread::~CGDIThread()
{
}

/////////////////////////////////////////////////////////////////////////////////////
// CGDIThread diagnostics
#ifdef _DEBUG
void CGDIThread::AssertValid() const
{
	CThread::AssertValid();
}

void CGDIThread::Dump(CDumpContext& dc) const
{
	CThread::Dump(dc);
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////////////
void CGDIThread::OnInitThread()
{
}

void CGDIThread::OnExitThread()
{
}


bool CGDIThread::SingleStep()
{
	Lock();

	int trigger = okWaitSignalEvent(m_pFrame->hBoard, EVENT_EXTTRIGGER, 5000); //限时等待触发
	if (trigger == 1) { //检测到高电平
		m_pFrame->SendMessage( WM_SWITCHON );
		while(okGetSignalParam(m_pFrame->hBoard, SIGNAL_EXTTRIGGER) == 1) 
		{
			//等待电平复位
			okSleep(100);
		}
	}

	Unlock();
	return m_Stop_Flag;
}

/////////////////////////////////////////////////////////////////////////////////////
// WORKER THREAD CLASS GENERATOR - Do not remove this method!
// MAIN THREAD HANDLER - The only method that must be implemented.
/////////////////////////////////////////////////////////////////////////////////////
DWORD CGDIThread::ThreadHandler()
{
	BOOL bContinue = TRUE;
	int nIncomingCommand;

	OnInitThread();

	if (okGetSignalParam(m_pFrame->hBoard, SIGNAL_EXTTRIGGER) == -1)
	{
		AfxMessageBox("不支持外触发，外触发失败!");
	}
	else
	{
		while(okGetSignalParam(m_pFrame->hBoard, SIGNAL_EXTTRIGGER) == 1) 
		{
			okSleep(100);
		}
		
		do
		{
			WaitForNotification(nIncomingCommand, 0);
			
			/////////////////////////////////////////////////////////////////////////////
			//	Main Incoming Command Handling
			//
			//	This switch statement is just a basic skeleton showing the mechanism
			//	how to handle incoming commands. Developer may remove or rewrite
			//	necessary parts of this switch to fit his needs.
			/////////////////////////////////////////////////////////////////////////////
			switch (nIncomingCommand)
			{
			case CThread::CMD_TIMEOUT_ELAPSED:	// timeout-elapsing handling
				if (GetActivityStatus() != CThread::THREAD_PAUSED)
				{
					if(SingleStep())
					{
						bContinue = FALSE;
					}
				};
				break;
				
			case CThread::CMD_INITIALIZE:		// initialize the thread task
				// this command should be handled; it is automatically fired when the thread starts
				// UserSpecificOnInitializeHandler();
				HandleCommandImmediately(CMD_RUN);	// fire CThread::CMD_RUN command after proper initialization
				break;
				
			case CThread::CMD_RUN:				// handle 'OnRun' command
				if (GetActivityStatus() != CThread::THREAD_PAUSED)
				{
					SetActivityStatus(CThread::THREAD_RUNNING);
					if(SingleStep())
					{
						bContinue = FALSE;
					}
				}
				break;
				
			case CThread::CMD_PAUSE:			// handle 'OnPause' command
				if (GetActivityStatus() != CThread::THREAD_PAUSED)
				{
					// UserSpecificOnPauseHandler();
					SetActivityStatus(CThread::THREAD_PAUSED);
				};
				
				
				break;
				
			case CThread::CMD_CONTINUE:			// handle 'OnContinue' command
				if (GetActivityStatus() == CThread::THREAD_PAUSED)
				{
					SetActivityStatus(CThread::THREAD_CONTINUING);
					// UserSpecificOnContinueHandler();
					HandleCommandImmediately(CMD_RUN);	// fire CThread::CMD_RUN command
				};
				break;
				
			case CThread::CMD_STOP:				// handle 'OnStop' command
				bContinue = FALSE;
				break;
				
			default:							// handle unknown commands...
				break;
			};
			
		} while (bContinue);
	}

	OnExitThread();

	return (DWORD)CThread::DW_OK;			// if everything OK
}
