// Resize.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "Resize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResize dialog
CResize::CResize(CWnd* pParent /*=NULL*/)
	: CDialog(CResize::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResize)
	m_height = 0;
	m_width  = 0;
	m_heightrate = 100;
	m_widthrate  = 100;
	m_distort = FALSE;
	m_SampleMethod = 0;
	m_factor = 1.0f;
	//}}AFX_DATA_INIT
	m_flag = false;
}


void CResize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResize)
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_height);
	DDV_MinMaxInt(pDX, m_height, 0, 900000);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_width);
	DDV_MinMaxInt(pDX, m_width, 0, 900000);
	DDX_Text(pDX, IDC_EDIT_HEIGHTRATE, m_heightrate);
	DDV_MinMaxInt(pDX, m_heightrate, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_WIDTHRATE, m_widthrate);
	DDV_MinMaxInt(pDX, m_widthrate, 0, 1000);
	DDX_Check(pDX, IDC_CHECK_DISTORT, m_distort);
	DDX_CBIndex(pDX, IDC_COMBO_METHOD, m_SampleMethod);
	DDX_Text(pDX, IDC_EDIT_FACTOR, m_factor);
	DDV_MinMaxFloat(pDX, m_factor, 0.f, 100.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResize, CDialog)
	//{{AFX_MSG_MAP(CResize)
	ON_EN_UPDATE(IDC_EDIT_HEIGHT, OnUpdateEditHeight)
	ON_EN_UPDATE(IDC_EDIT_HEIGHTRATE, OnUpdateEditHeightrate)
	ON_EN_UPDATE(IDC_EDIT_WIDTH, OnUpdateEditWidth)
	ON_EN_UPDATE(IDC_EDIT_WIDTHRATE, OnUpdateEditWidthrate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResize message handlers

void CResize::OnUpdateEditHeight() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	float tmp;
	int i = (int)GetDlgItemInt(IDC_EDIT_HEIGHT);
	tmp = float(i*100.0/m_old_height+0.5);
	i = (int)tmp;
	if(i>1000 || i<0)
	{
		SetDlgItemInt(IDC_EDIT_HEIGHT,m_height);
		return;
	}
	if(GetDlgItemInt(IDC_EDIT_HEIGHT) == (UINT)m_height)  return;
	UpdateData();  // 更新 m_IDC_EDIT
    
	tmp = float(m_height*100.0/m_old_height+0.5);
	m_heightrate = (int)tmp;
    SetDlgItemInt(IDC_EDIT_HEIGHTRATE,m_heightrate);
	if(m_distort)  return;
	tmp = float(m_old_width*m_height*1.0/m_old_height+0.5);
	m_width = (int)tmp;
    SetDlgItemInt(IDC_EDIT_WIDTH,m_width);
	tmp = float(m_width*100.0/m_old_width+0.5);
	m_widthrate = (int)tmp;
    SetDlgItemInt(IDC_EDIT_WIDTHRATE,m_widthrate);
}

void CResize::OnUpdateEditHeightrate() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	int i = (int)GetDlgItemInt(IDC_EDIT_HEIGHTRATE);
	if(i>1000 || i<0)
	{
		SetDlgItemInt(IDC_EDIT_HEIGHTRATE,m_heightrate);
		return;
	}
	if(GetDlgItemInt(IDC_EDIT_HEIGHTRATE) == (UINT)m_heightrate)  return;
	UpdateData();  // 更新 m_IDC_EDIT

	if(m_flag)
	{
		if(!m_distort)
		{
			SetDlgItemInt(IDC_EDIT_WIDTHRATE,m_heightrate);
		}
		return;
	}

	float tmp;
	tmp = float(m_old_height*m_heightrate*1.0/100+0.5);
	m_height = (int)tmp;
    SetDlgItemInt(IDC_EDIT_HEIGHT,m_height);
	if(m_distort)  return;
	tmp = float(m_old_width*m_height*1.0/m_old_height+0.5);
	m_width = (int)tmp;
    SetDlgItemInt(IDC_EDIT_WIDTH,m_width);
	tmp = float(m_width*100.0/m_old_width+0.5);
	m_widthrate = (int)tmp;
    SetDlgItemInt(IDC_EDIT_WIDTHRATE,m_widthrate);
}

void CResize::OnUpdateEditWidth() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	float tmp;
	int i = (int)GetDlgItemInt(IDC_EDIT_WIDTH);
	tmp = float(i*100.0/m_old_width+0.5);
	i = (int)tmp;
	if(i>1000 || i<0)
	{
		SetDlgItemInt(IDC_EDIT_WIDTH,m_width);
		return;
	}
	if(GetDlgItemInt(IDC_EDIT_WIDTH) == (UINT)m_width)  return;
	UpdateData();  // 更新 m_IDC_EDIT
	
	tmp = float(m_width*100.0/m_old_width+0.5);
	m_widthrate = (int)tmp;
    SetDlgItemInt(IDC_EDIT_WIDTHRATE,m_widthrate);
	if(m_distort)  return;
	tmp = float(m_old_height*m_width*1.0/m_old_width+0.5);
	m_height = (int)tmp;
    SetDlgItemInt(IDC_EDIT_HEIGHT,m_height);
	tmp = float(m_height*100.0/m_old_height+0.5);
	m_heightrate = (int)tmp;
    SetDlgItemInt(IDC_EDIT_HEIGHTRATE,m_heightrate);
}

void CResize::OnUpdateEditWidthrate() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	int i = (int)GetDlgItemInt(IDC_EDIT_WIDTHRATE);
	if(i>1000 || i<0)
	{
		SetDlgItemInt(IDC_EDIT_WIDTHRATE,m_widthrate);
		return;
	}
	if(GetDlgItemInt(IDC_EDIT_WIDTHRATE) == (UINT)m_widthrate)  return;
	UpdateData();  // 更新 m_IDC_EDIT
	
	if(m_flag)
	{
		if(!m_distort)
		{
			SetDlgItemInt(IDC_EDIT_HEIGHTRATE,m_widthrate);
		}
		return;
	}

	float tmp;
	tmp = float(m_old_width*m_widthrate*1.0/100+0.5);
	m_width = (int)tmp;
    SetDlgItemInt(IDC_EDIT_WIDTH,m_width);
	if(m_distort)  return;
	tmp = float(m_old_height*m_width*1.0/m_old_width+0.5);
	m_height = (int)tmp;
    SetDlgItemInt(IDC_EDIT_HEIGHT,m_height);
	tmp = float(m_height*100.0/m_old_height+0.5);
	m_heightrate = (int)tmp;
    SetDlgItemInt(IDC_EDIT_HEIGHTRATE,m_heightrate);
}

BOOL CResize::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_old_width  = m_width;
	m_old_height = m_height;	

	if(m_flag)
	{
		GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CResize::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CDialog::OnOK();
}
