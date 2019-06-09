// MVCSetup.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "MVCSetup.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMVCSetup dialog


CMVCSetup::CMVCSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CMVCSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMVCSetup)
	m_Size = 0;
	m_AE = FALSE;
	m_Sensitive = 0;
	//}}AFX_DATA_INIT
}


void CMVCSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMVCSetup)
	DDX_Radio(pDX, IDC_RADIO1, m_Size);
	DDX_Check(pDX, IDC_CHECK_AE, m_AE);
	DDX_Radio(pDX, IDC_RADIO9, m_Sensitive);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMVCSetup, CDialog)
	//{{AFX_MSG_MAP(CMVCSetup)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK_AE, OnCheckAe)
	ON_BN_CLICKED(IDC_RADIO9, OnRadio9)
	ON_BN_CLICKED(IDC_RADIO10, OnRadio10)
	ON_BN_CLICKED(IDC_RADIO11, OnRadio11)
	ON_BN_CLICKED(IDC_CHECK_RGBBGR, OnCheckRgbbgr)
	ON_BN_CLICKED(IDC_CHECK_FLIP, OnCheckFlip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMVCSetup message handlers

BOOL CMVCSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	if(pFrame->m_hMVC3000 == NULL)
	{
		AfxMessageBox("无MVC3000连接设备!");
		EndDialog(IDCANCEL);
		return FALSE;
	}
	memcpy((CapInfoStruct*)(&m_TempCapInfo),(CapInfoStruct*)(&(pFrame->m_CapInfo)),sizeof(struct CapInfoStruct));
	m_AE = pFrame->m_AE;
	
	CString strRG;
	CSliderCtrl* pSliderRG = (CSliderCtrl*)GetDlgItem(IDC_SLIDERR);
	pSliderRG->SetRange(1,48);
	pSliderRG->SetPos(m_TempCapInfo.Gain[0]);
	strRG.Format("%d",pSliderRG->GetPos());
	SetDlgItemText(IDC_STATICR,strRG);

	CString strGG;
	CSliderCtrl* pSliderGG = (CSliderCtrl*)GetDlgItem(IDC_SLIDERG);
	pSliderGG->SetRange(1,48);
	pSliderGG->SetPos(m_TempCapInfo.Gain[1]);
	strGG.Format("%d",pSliderGG->GetPos());
	SetDlgItemText(IDC_STATICG,strGG);

	CString strBG;
	CSliderCtrl* pSliderBG = (CSliderCtrl*)GetDlgItem(IDC_SLIDERB);
	pSliderBG->SetRange(1,48);
	pSliderBG->SetPos(m_TempCapInfo.Gain[2]);
	strBG.Format("%d",pSliderBG->GetPos());
	SetDlgItemText(IDC_STATICB,strBG);

	CString strE;
	CSliderCtrl* pSliderE = (CSliderCtrl*)GetDlgItem(IDC_SLIDERE);

#ifdef MVC3000
	pSliderE->SetRange(1,10000);
#else
	pSliderE->SetRange(1,500);
#endif

	pSliderE->SetPos(m_TempCapInfo.Exposure);
	strE.Format("%d",pSliderE->GetPos());
	SetDlgItemText(IDC_STATICE,strE);

	CString strXO;
	CSliderCtrl* pSliderXO = (CSliderCtrl*)GetDlgItem(IDC_SLIDERXO);
	pSliderXO->SetRange(0,MAXWIDTH-m_TempCapInfo.Width);
	pSliderXO->SetPos(m_TempCapInfo.HorizontalOffset);
	strXO.Format("%d",pSliderXO->GetPos());
	SetDlgItemText(IDC_STATICXO,strXO);


	CString strYO;
	CSliderCtrl* pSliderYO = (CSliderCtrl*)GetDlgItem(IDC_SLIDERYO);
	pSliderYO->SetRange(0,1024-m_TempCapInfo.Height);
	pSliderYO->SetPos(m_TempCapInfo.VerticalOffset);
	strYO.Format("%d",pSliderYO->GetPos());
	SetDlgItemText(IDC_STATICYO,strYO);

	switch(m_TempCapInfo.Width) {
	case 320:
		m_Size = 0;
		break;
	case 640:
		m_Size = 1;
		break;
	case 768:
		m_Size = 2;
		break;
	case 800:
		m_Size = 3;
		break;
	case 1024:
		m_Size = 4;
		break;
	case 1280:
		m_Size = 5;
		break;
	case 1600:
		m_Size = 6;
		break;
	case 2048:
		m_Size = 7;
		break;
	}

	if(m_TempCapInfo.Control&0x02)
	{
		m_Sensitive = 1;
	}
	else
	{
		if(m_TempCapInfo.Control&0x06)
			m_Sensitive = 2;
		else
			m_Sensitive = 0;
	}

	if(m_TempCapInfo.Control&0x20)
	{
		((CButton *)GetDlgItem(IDC_CHECK_FLIP))->SetCheck(1);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_CHECK_FLIP))->SetCheck(0);
	}

	if(m_TempCapInfo.Control&0x40)
	{
		((CButton *)GetDlgItem(IDC_CHECK_RGBBGR))->SetCheck(1);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_CHECK_RGBBGR))->SetCheck(0);
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMVCSetup::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;
	CString strValue;
	int nValue = pSlider->GetPos();
	switch(pScrollBar->GetDlgCtrlID())
	{
	case IDC_SLIDERR:
		strValue.Format("%d",pSlider->GetPos());
		SetDlgItemText(IDC_STATICR,strValue);
		m_TempCapInfo.Gain[0]=nValue;
		MV_Usb2SetPartOfCapInfo(pFrame->m_hMVC3000, &m_TempCapInfo);
		break;
	case IDC_SLIDERG:
		strValue.Format("%d",pSlider->GetPos());
		SetDlgItemText(IDC_STATICG,strValue);
		m_TempCapInfo.Gain[1]=nValue;
		MV_Usb2SetPartOfCapInfo(pFrame->m_hMVC3000, &m_TempCapInfo); 
		break;
	case IDC_SLIDERB:
		strValue.Format("%d",pSlider->GetPos());
		SetDlgItemText(IDC_STATICB,strValue);
		m_TempCapInfo.Gain[2]=nValue;
		MV_Usb2SetPartOfCapInfo(pFrame->m_hMVC3000, &m_TempCapInfo); 
		break;
	case IDC_SLIDERE:
		strValue.Format("%d",pSlider->GetPos());
		SetDlgItemText(IDC_STATICE,strValue);
		m_TempCapInfo.Exposure=nValue;
		MV_Usb2SetPartOfCapInfo(pFrame->m_hMVC3000, &m_TempCapInfo); 
		break;
	case IDC_SLIDERXO:
		strValue.Format("%d",pSlider->GetPos());
		SetDlgItemText(IDC_STATICXO,strValue);
		m_TempCapInfo.HorizontalOffset=nValue;
		MV_Usb2SetPartOfCapInfo(pFrame->m_hMVC3000, &m_TempCapInfo); 
		break;
	case IDC_SLIDERYO:
		strValue.Format("%d",pSlider->GetPos());
		SetDlgItemText(IDC_STATICYO,strValue);
		m_TempCapInfo.VerticalOffset=nValue;
		MV_Usb2SetPartOfCapInfo(pFrame->m_hMVC3000, &m_TempCapInfo); 
		break;
	}
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMVCSetup::OnOK() 
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	memcpy((CapInfoStruct*)(&(pFrame->m_CapInfo)),(CapInfoStruct*)(&m_TempCapInfo),sizeof(struct CapInfoStruct));

	UpdateData(TRUE);
	switch(m_Size) {
	case 0:
		pFrame->m_CapInfo.Width  = 320;
		pFrame->m_CapInfo.Height = 240;
		break;
	case 1:
		pFrame->m_CapInfo.Width  = 640;
		pFrame->m_CapInfo.Height = 480;
		break;
	case 2:
		pFrame->m_CapInfo.Width  = 768;
		pFrame->m_CapInfo.Height = 576;
		break;
	case 3:
		pFrame->m_CapInfo.Width  = 800;
		pFrame->m_CapInfo.Height = 600;
		break;
	case 4:
		pFrame->m_CapInfo.Width  = 1024;
		pFrame->m_CapInfo.Height = 768;
		break;
	case 5:
		pFrame->m_CapInfo.Width  = 1280;
		pFrame->m_CapInfo.Height = 1024;
		break;
	case 6:
		pFrame->m_CapInfo.Width  = 1600;
		pFrame->m_CapInfo.Height = 1200;
		break;
	case 7:
		pFrame->m_CapInfo.Width  = 2048;
		pFrame->m_CapInfo.Height = 1536;
		break;
	}

	if((pFrame->m_CapInfo.Width + pFrame->m_CapInfo.HorizontalOffset) >= MAXWIDTH)
		pFrame->m_CapInfo.HorizontalOffset=0;

	if((pFrame->m_CapInfo.Height + pFrame->m_CapInfo.VerticalOffset) >= MAXHEIGHT)
		pFrame->m_CapInfo.VerticalOffset=0;

	MV_Usb2SetPartOfCapInfo(pFrame->m_hMVC3000, &(pFrame->m_CapInfo)); 
	pFrame->m_AE = m_AE;
	
	CDialog::OnOK();
}

void CMVCSetup::OnCancel() 
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	MV_Usb2SetPartOfCapInfo(pFrame->m_hMVC3000, &(pFrame->m_CapInfo)); 

	LONG gExposure;
	MV_Usb2AutoExposure(pFrame->m_hMVC3000, pFrame->m_AE, 180, NULL,&gExposure);	
	
	CDialog::OnCancel();
}

void CMVCSetup::OnCheckAe() 
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	LONG gExposure;
	if(((CButton *)GetDlgItem(IDC_CHECK_AE))->GetState() & 0x0003)
	{
		MV_Usb2AutoExposure(pFrame->m_hMVC3000, TRUE, 180, NULL,&gExposure);	
	}
	else
	{
		MV_Usb2AutoExposure(pFrame->m_hMVC3000, FALSE, 180, NULL,&gExposure);	
	}
}

void CMVCSetup::OnRadio9() 
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	m_TempCapInfo.Control &= 0xf9;
	MV_Usb2SetCapInfo(pFrame->m_hMVC3000, &m_TempCapInfo);
}

void CMVCSetup::OnRadio10() 
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	m_TempCapInfo.Control &= 0xf9;
	m_TempCapInfo.Control |= 0x02;
	MV_Usb2SetCapInfo(pFrame->m_hMVC3000, &m_TempCapInfo);
}

void CMVCSetup::OnRadio11() 
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	m_TempCapInfo.Control &= 0xf9;
	m_TempCapInfo.Control |= 0x04;
	MV_Usb2SetCapInfo(pFrame->m_hMVC3000, &m_TempCapInfo);
}

void CMVCSetup::OnCheckRgbbgr() 
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	if(((CButton *)GetDlgItem(IDC_CHECK_RGBBGR))->GetState() & 0x0003)
	{
		m_TempCapInfo.Control |= 0x40;
	}
	else
	{
		m_TempCapInfo.Control &= 0xBF;
	}
	MV_Usb2SetCapInfo(pFrame->m_hMVC3000, &m_TempCapInfo);
}

void CMVCSetup::OnCheckFlip() 
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	if(((CButton *)GetDlgItem(IDC_CHECK_FLIP))->GetState() & 0x0003)
	{
		m_TempCapInfo.Control |= 0x20;
	}
	else
	{
		m_TempCapInfo.Control &= 0xDF;
	}
	MV_Usb2SetCapInfo(pFrame->m_hMVC3000, &m_TempCapInfo);
}
