// DlgReflection.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "DlgReflection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgReflection dialog
extern CPathologyApp theApp;

CDlgReflection::CDlgReflection(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgReflection::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgReflection)
	m_nReflectStyle = 0;
	//}}AFX_DATA_INIT
	m_File_List.RemoveAll();
}

CDlgReflection::~CDlgReflection()
{
	m_File_List.RemoveAll();
}

void CDlgReflection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgReflection)
	DDX_Control(pDX, IDC_STATIC_IMAGE2, m_staticImage2);
	DDX_Control(pDX, IDC_STATIC_IMAGE1, m_staticImage1);
	DDX_Radio(pDX, IDC_RADIO_HORIZONTAL, m_nReflectStyle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgReflection, CDialog)
	//{{AFX_MSG_MAP(CDlgReflection)
	ON_BN_CLICKED(IDC_RADIO_HORIZONTAL, OnRadioReflectStyle)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_VERTICAL, OnRadioReflectStyle)
	ON_BN_CLICKED(IDC_RADIO_HORI_VERTI, OnRadioReflectStyle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgReflection message handlers

BOOL CDlgReflection::OnInitDialog() 
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
	m_staticImage2.SetBitmap(IDB_FLIPHORI);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgReflection::OnRadioReflectStyle() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	switch(m_nReflectStyle)
	{
	case 0:
		m_staticImage2.SetBitmap(IDB_FLIPHORI);
		break;
	case 1:
		m_staticImage2.SetBitmap(IDB_FLIPVERT);
		break;
	case 2:
		m_staticImage2.SetBitmap(IDB_ROTATE180);
		break;
	}

	CRect Rect;
	m_staticImage2.GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	RedrawWindow(&Rect, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE|RDW_ALLCHILDREN);
	UpdateData(FALSE);
}

void CDlgReflection::OnOK() 
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

		GDALDataset *m_pDstDataset = GS_CustomCreateWithHeader("BMP", tmpimg, m_pSrcDataset,
					  							               m_pSrcDataset->GetRasterXSize(), 
			                                                   m_pSrcDataset->GetRasterYSize(), 
												               m_pSrcDataset->GetRasterCount(), 
												               (m_pSrcDataset->GetRasterBand(1))->GetRasterDataType(), 
												               NULL);

		if( m_pDstDataset != NULL )
		{
			switch(m_nReflectStyle)
			{
			case 0:
				ReflectHorizontal(m_pSrcDataset, m_pDstDataset);
				break;
			case 1:
				ReflectVertical(m_pSrcDataset, m_pDstDataset);
				break;
			case 2:
				RotateStraightAngle(m_pSrcDataset, m_pDstDataset);
				break;
			}
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

void CDlgReflection::OnDestroy() 
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	m_File_List.RemoveAll();
}
