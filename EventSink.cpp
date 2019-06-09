//==============================================================================================
//	FILE      :	EventSink.cpp
//	ABSTRACT  :	Receives camera events
//	HISTORY   :	2005.11.06 Nikon Corp. - Created
//
//	Copyright (c) 2005, Nikon Corp.  All rights reserved.
//==============================================================================================

#include "stdafx.h"
#include "Pathology.h"
#include "initguid.h"
#include "EventSink.h"
#include "streampage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Constructor/Destructor
//////////////////////////////////////////////////////////////////////

//==============================================================================================
//	FUNCTION  :	CDeviceEventSink
//	ABSTRACT  :	Constructor
//	PARAMS    :	pSource: pointer to the IDevice interface of a camera
//			pStreamPage: pointer to the property page hosted by the stream tab
//	RETURN    :	void
//	NOTE      :	Establishes a sink for the camera event stream
//	HISTORY   :	2005.11.06 Nikon Corp. - Created
//==============================================================================================
CDeviceEventSink::CDeviceEventSink(IDevice * pSource, CStreamPage* pStreamPage)
{
	m_pSource = pSource;
	HRESULT hr = DispEventAdvise(pSource);
	m_pStreamPage = pStreamPage;
}

//==============================================================================================
//	FUNCTION  :	~CDeviceEventSink
//	ABSTRACT  :	Destructor
//	PARAMS    :	None
//	RETURN    :	void
//	NOTE      :	Releases the sink for the camera event stream
//	HISTORY   :	2005.11.06 Nikon Corp. - Created
//==============================================================================================
CDeviceEventSink::~CDeviceEventSink()
{
	DispEventUnadvise(m_pSource);
}

//==============================================================================================
//	FUNCTION  :	OnNewImage
//	ABSTRACT  :	Sink for the camera's OnNewImage event
//	PARAMS    :	FrameNumber
//	RETURN    :	void
//	NOTE      :	Only ReadBuffer can be called from this event. DO NOT CALL Start, Stop, Unlock
//	HISTORY   :	2005.11.06 Nikon Corp. - Created
//==============================================================================================
void __stdcall CDeviceEventSink::OnNewImage(long FrameNumber)
{
	m_pStreamPage->OnNewImage(FrameNumber);
	return;
}

//==============================================================================================
//	FUNCTION  :	OnCapturingEvent
//	ABSTRACT  :	Redirects camera events to the class for the stream page
//	PARAMS    :	EventNumber: 0 when a stream stops
//	RETURN    :	void
//	NOTE      :	need return
//	HISTORY   :	2005.11.06 Nikon Corp. - Created
//==============================================================================================
void __stdcall CDeviceEventSink::OnCapturingEvent(long EventNumber)
{
	m_pStreamPage->PostMessage(UM_CAPTURINGEVENT, 0, EventNumber);
	return;
}
