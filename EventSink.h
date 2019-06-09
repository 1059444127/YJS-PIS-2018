//==============================================================================================
//	FILE      :	EventSink.h
//	ABSTRACT  :	Camera Event Manager
//	HISTORY   :	2005.11.06 Nikon Corp. - Created
//
//	Copyright (c) 2005, Nikon Corp.  All rights reserved.
//==============================================================================================

#if !defined(AFX_EVENTSINK_H__3F1E0CEB_E9D3_475E_81FD_CC9BB26BAD1F__INCLUDED_)
#define AFX_EVENTSINK_H__3F1E0CEB_E9D3_475E_81FD_CC9BB26BAD1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define UM_CAPTURINGEVENT (WM_USER + 2)

class CStreamPage;

//Interface definitions
static _ATL_FUNC_INFO OnNewImageInfo = {CC_STDCALL, VT_EMPTY, 1, {VT_UI4}};
static _ATL_FUNC_INFO OnCapturingEventInfo = {CC_STDCALL, VT_EMPTY, 1, {VT_UI4}};

class CDeviceEventSink : IDispEventSimpleImpl<0, CDeviceEventSink, &DIID__IDeviceEvents>
{
public:
	CDeviceEventSink(IDevice * pSource, CStreamPage* pStreamPage);
	virtual ~CDeviceEventSink();

// Set DispatchID of _IDeviceEvents
// OnNewImage:2 OnCapturingEvent:3
BEGIN_SINK_MAP(CDeviceEventSink)
	SINK_ENTRY_INFO(0, DIID__IDeviceEvents, 0x2, OnNewImage, &OnNewImageInfo)
	SINK_ENTRY_INFO(0, DIID__IDeviceEvents, 0x3, OnCapturingEvent, &OnCapturingEventInfo)
END_SINK_MAP()

	void __stdcall OnNewImage(long FrameNumber);
	void __stdcall OnCapturingEvent(long EventNumber);

protected:
	IDevice*	m_pSource;			// a reference to the camera communications object
	CStreamPage* m_pStreamPage;		// a reference to the Stream Page tab
};

#endif // !defined(AFX_EVENTSINK_H__3F1E0CEB_E9D3_475E_81FD_CC9BB26BAD1F__INCLUDED_)
