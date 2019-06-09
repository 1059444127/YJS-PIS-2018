//==============================================================================================
//	FILE      :	StreamPage.h
//	ABSTRACT  :	Handles interactions with the CameraController object for the property page 
//			hosted by the Stream Tab
//	HISTORY   :	2005.11.05 Nikon Corp. - Created
//
//	Copyright (c) 2005, Nikon Corp.  All rights reserved.
//==============================================================================================

#if !defined(AFX_STREAMPAGE_H__B53AF36F_42F0_412D_B58F_47EF172867D5__INCLUDED_)
#define AFX_STREAMPAGE_H__B53AF36F_42F0_412D_B58F_47EF172867D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CStreamPage Dialog

#include "controlleredit.h"
#include "controllercombobox.h"
#include "eventsink.h"
#include "filewriter.h"

class CStreamPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CStreamPage)

public:
	void OnNewImage(long FrameNumber);
	void ChangeDevice();
	CStreamPage();
	~CStreamPage();
	void start();
	CString m_blh;

protected:
	CCameraControl* m_pCamera;				// reference to a wrapper class for the CameraController object
	CWnd*			m_pPreviewWnd;			// low-resolution live preview window
	CDeviceEventSink*		m_pEventSink;	// to receive camera events
	CFileWriter*	m_pFileWriter;			// file output
	BOOL			m_bCapturing;			// image stream is in progress

	//{{AFX_DATA(CStreamPage)
	enum { IDD = IDD_DIALOG_STREAM };
	CComboBox	m_cmbQuality;
	CComboBox	m_cmbShift;
	CControllerComboBox	m_clTrigger;
	CComboBox	m_MeteringAreaList;
	CComboBox	m_ComboSaveShadingPosition;
	CEdit	m_EditWindow;
	CComboBox	m_ComboRender;
	CEdit	m_EditFilepath;
	CEdit	m_EditBufferpath;
	CControllerEdit	m_EditYField;
	CControllerEdit	m_EditXField;
	CControllerComboBox	m_ComboResolution;
	CControllerComboBox	m_ComboFormat;
	CControllerComboBox	m_ComboField;
	int		m_iAppendix;
	//}}AFX_DATA


// override
	//{{AFX_VIRTUAL(CStreamPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV
	//}}AFX_VIRTUAL

// implementation
protected:
	HRESULT SetWindowHandle(IImageStream* pImageStream);
	BOOL CreatePreviewWindow(IImageStream* pImageStream = NULL);
	//{{AFX_MSG(CStreamPage)
	afx_msg void OnButtonSnapshot();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonStop();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnButtonWindowhandle();
	afx_msg void OnButtonContinousshot();
	afx_msg void OnRadioAppAdd();
	afx_msg void OnRadioAppRemove();
	afx_msg void OnButtonFocusSet();
	afx_msg void OnButtonMtSet();
	afx_msg void OnButtonShadingSet();
	afx_msg void OnButtonSnapshotEx();
	afx_msg void OnButtonShiftSnap();
	afx_msg void OnButtonRawSnapshot();
	//}}AFX_MSG
	afx_msg HRESULT OnCapturingEvent(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	void SetFocusRange();
	void GetImageSize(DWORD & dwWidth, DWORD & dwHeight);
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_STREAMPAGE_H__B53AF36F_42F0_412D_B58F_47EF172867D5__INCLUDED_)
