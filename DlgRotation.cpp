// DlgRotation.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "DlgRotation.h"

#define PI 3.141592654
#define RADIAN(angle) ((angle)*PI/180.0)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRotation dialog
extern CPathologyApp theApp;

CDlgRotation::CDlgRotation(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRotation::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRotation)
	m_nRotateStyle = 0;
	m_editAngle = 0.0f;
	//}}AFX_DATA_INIT
	m_File_List.RemoveAll();
}

CDlgRotation::~CDlgRotation()
{
	m_File_List.RemoveAll();
}

void CDlgRotation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRotation)
	DDX_Control(pDX, IDC_EDIT_ANGLE, m_edtRotationAngle);
	DDX_Control(pDX, IDC_STATIC_IMAGE2, m_staticImage2);
	DDX_Control(pDX, IDC_STATIC_IMAGE1, m_staticImage1);
	DDX_Control(pDX, IDC_SPIN_ANGLE, m_spinAngle);
	DDX_Radio(pDX, IDC_RADIO_RIGHT_ANGLE, m_nRotateStyle);
	DDX_Text(pDX, IDC_EDIT_ANGLE, m_editAngle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRotation, CDialog)
	//{{AFX_MSG_MAP(CDlgRotation)
	ON_BN_CLICKED(IDC_RADIO_RIGHT_ANGLE, OnRadioRotateStyle)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_STRAIGHT_ANGLE, OnRadioRotateStyle)
	ON_BN_CLICKED(IDC_RADIO_LEFT_ANGLE, OnRadioRotateStyle)
	ON_BN_CLICKED(IDC_RADIO_ANGLE, OnRadioRotateStyle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRotation message handlers

BOOL CDlgRotation::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(FALSE);

	CRect Rect;
	m_staticImage1.GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	Rect.bottom = Rect.top + 48;
	Rect.right = Rect.left + 48;
	m_staticImage1.MoveWindow(Rect);
	m_staticImage1.SetBitmap(IDB_ORIG);

	m_staticImage2.GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	Rect.bottom = Rect.top + 48;
	Rect.right = Rect.left + 48;
	m_staticImage2.MoveWindow(Rect);
	m_staticImage2.SetBitmap(IDB_ROTATE90);

	m_spinAngle.SetRange(0, 360);

	GetDlgItem(IDC_EDIT_ANGLE)->EnableWindow(FALSE);

	m_edtRotationAngle.SetRange(0,360);
	m_edtRotationAngle.ModifyFlags(CAMSNumericEdit::OnKillFocus_SetFocus|CAMSNumericEdit::OnKillFocus_ShowMessage,0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRotation::OnRadioRotateStyle() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	switch(m_nRotateStyle)
	{
	case 0:
		m_staticImage2.SetBitmap(IDB_ROTATE90);
		GetDlgItem(IDC_EDIT_ANGLE)->EnableWindow(FALSE);
		break;
	case 1:
		m_staticImage2.SetBitmap(IDB_ROTATE180);
		GetDlgItem(IDC_EDIT_ANGLE)->EnableWindow(FALSE);
		break;
	case 2:
		m_staticImage2.SetBitmap(IDB_ROTATE270);
		GetDlgItem(IDC_EDIT_ANGLE)->EnableWindow(FALSE);
		break;
	case 3:
		GetDlgItem(IDC_EDIT_ANGLE)->EnableWindow(TRUE);
	}

	CRect Rect;
	m_staticImage2.GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	RedrawWindow(&Rect, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE|RDW_ALLCHILDREN);
	UpdateData(FALSE);
}

void CDlgRotation::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	BeginWaitCursor();

	for( int i = 0; i < m_File_List.GetSize(); i ++ )
	{
		GDALDataset * m_pSrcDataset = (GDALDataset *) GDALOpen ( m_File_List.GetAt(i), GA_ReadOnly );
		if( m_pSrcDataset == NULL )  continue;
		CString m_Input_FileType = GDALGetDriverShortName( m_pSrcDataset->GetDriver() );

		CString tmpimg = theApp.m_SysTmpPath;
		tmpimg += "00000001.bmp";
		remove(tmpimg);

		GDALDataset *m_pDstDataset = NULL;

		int nNewWidth, nNewHeight;
		float fRotateAngle;
		
		switch(m_nRotateStyle)
		{
		case 0:
			m_pDstDataset = GS_CustomCreateWithHeader("BMP", tmpimg, m_pSrcDataset,
				                                      m_pSrcDataset->GetRasterYSize(), 
													  m_pSrcDataset->GetRasterXSize(), 
													  m_pSrcDataset->GetRasterCount(), 
													  (m_pSrcDataset->GetRasterBand(1))->GetRasterDataType(), 
													  NULL);
			if(m_pDstDataset != NULL)
			{
				RotateRightAngle(m_pSrcDataset, m_pDstDataset);
			}
			break;
		case 1:
			m_pDstDataset = GS_CustomCreateWithHeader("BMP", tmpimg, m_pSrcDataset,
													  m_pSrcDataset->GetRasterXSize(), 
				                                      m_pSrcDataset->GetRasterYSize(), 
													  m_pSrcDataset->GetRasterCount(), 
													  (m_pSrcDataset->GetRasterBand(1))->GetRasterDataType(), 
													  NULL);
			if(m_pDstDataset != NULL)
			{
				RotateStraightAngle(m_pSrcDataset, m_pDstDataset);
			}
			break;
		case 2:
			m_pDstDataset = GS_CustomCreateWithHeader("BMP", tmpimg, m_pSrcDataset,
				                                      m_pSrcDataset->GetRasterYSize(), 
													  m_pSrcDataset->GetRasterXSize(), 
													  m_pSrcDataset->GetRasterCount(), 
													  (m_pSrcDataset->GetRasterBand(1))->GetRasterDataType(), 
													  NULL);
			if(m_pDstDataset != NULL)
			{
				RotateLeftAngle(m_pSrcDataset, m_pDstDataset);
			}
			break;
		case 3:
			fRotateAngle = (float) RADIAN(m_editAngle);
			BeforeRotate(m_pSrcDataset->GetRasterXSize(), m_pSrcDataset->GetRasterYSize(), nNewWidth, nNewHeight, fRotateAngle);
			m_pDstDataset = GS_CustomCreateWithHeader("BMP", tmpimg, m_pSrcDataset,
				                                      nNewWidth, nNewHeight, 
													  m_pSrcDataset->GetRasterCount(), 
													  (m_pSrcDataset->GetRasterBand(1))->GetRasterDataType(), 
													  NULL);
			if(m_pDstDataset != NULL)
			{
				Rotate(m_pSrcDataset, m_pDstDataset, fRotateAngle);
			}
			break;
		}
		
		GDALClose(m_pSrcDataset);	m_pSrcDataset = NULL;
		GDALClose(m_pDstDataset);	m_pDstDataset = NULL;

		m_pSrcDataset = (GDALDataset *) GDALOpen ( tmpimg, GA_ReadOnly );
		
		m_pDstDataset = GS_CreateCopy( m_Input_FileType, m_File_List.GetAt(i), m_pSrcDataset);
		
		GDALClose(m_pSrcDataset);	m_pSrcDataset = NULL;
		GDALClose(m_pDstDataset);	m_pDstDataset = NULL;
		remove(tmpimg);
	}

	EndWaitCursor();
		
	CDialog::OnOK();
}

void CDlgRotation::BeforeRotate(int nWidth, int nHeight, int &nNewWidth, int &nNewHeight, float fRotateAngle)
{
	float fSina, fCosa;
	
	//	source image coordinates
	float fSrcX1, fSrcY1, fSrcX2, fSrcY2, fSrcX3, fSrcY3, fSrcX4, fSrcY4;
	
	//	dst image coordinates
	float fDstX1, fDstY1, fDstX2, fDstY2, fDstX3, fDstY3, fDstX4, fDstY4;
	
	fSina = (float) sin((double)fRotateAngle);
	
	fCosa = (float) cos((double)fRotateAngle);
	
	fSrcX1 = (float) (- (nWidth  - 1) / 2);
	fSrcY1 = (float) (  (nHeight - 1) / 2);
	fSrcX2 = (float) (  (nWidth  - 1) / 2);
	fSrcY2 = (float) (  (nHeight - 1) / 2);
	fSrcX3 = (float) (- (nWidth  - 1) / 2);
	fSrcY3 = (float) (- (nHeight - 1) / 2);
	fSrcX4 = (float) (  (nWidth  - 1) / 2);
	fSrcY4 = (float) (- (nHeight - 1) / 2);
	
	fDstX1 =  fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 =  fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 =  fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 =  fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;

	nNewWidth  = (int) (max( fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2) ) + 0.5);
	nNewHeight = (int) (max( fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2) )  + 0.5);
}

void CDlgRotation::OnDestroy() 
{
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
	m_File_List.RemoveAll();
}
