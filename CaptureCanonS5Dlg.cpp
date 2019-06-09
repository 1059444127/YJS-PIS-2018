// CaptureCanonS5Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "pathology.h"
#include "CaptureCanonS5Dlg.h"
#include "Progress.h"
#include "MainFrm.h"
#include "MedImage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <jpeglib.h>
#include <jerror.h>
#include <setjmp.h>
struct my_error_mgr {
     struct jpeg_error_mgr pub;
     jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr *my_error_ptr;

/*--------------
  A hack to hijack JPEG's innards to write into a memory buffer
----------------
/  this defines a new destination manager to store images in memory
/  derived by jdatadst.c */
typedef struct {
  struct jpeg_destination_mgr pub;      /* public fields */
  JOCTET *buffer;                                       /* start of buffer */
  int bufsize;                                          /* buffer size */
  int datacount;                                        /* finale data size */
} memory_destination_mgr;

typedef memory_destination_mgr *mem_dest_ptr;

/*----------------------------------------------------------------------------
  /  Initialize destination --- called by jpeg_start_compress before any data is actually written. */

METHODDEF(void)
init_destination (j_compress_ptr cinfo)
{
  mem_dest_ptr dest = (mem_dest_ptr) cinfo->dest;
  dest->pub.next_output_byte = dest->buffer;
  dest->pub.free_in_buffer = dest->bufsize;
  dest->datacount=0;
}





/*----------------------------------------------------------------------------
  /  Empty the output buffer --- called whenever buffer fills up. */
METHODDEF(boolean)
empty_output_buffer (j_compress_ptr cinfo)
{
  mem_dest_ptr dest = (mem_dest_ptr) cinfo->dest;
  dest->pub.next_output_byte = dest->buffer;
  dest->pub.free_in_buffer = dest->bufsize;

  return TRUE;
}

/*----------------------------------------------------------------------------
  /  Terminate destination --- called by jpeg_finish_compress
  /  after all data has been written.  Usually needs to flush buffer. */
METHODDEF(void)
term_destination (j_compress_ptr cinfo)
{
  /* expose the finale compressed image size */
  
  mem_dest_ptr dest = (mem_dest_ptr) cinfo->dest;
  dest->datacount = dest->bufsize - dest->pub.free_in_buffer;
  
}

static void my_error_exit(j_common_ptr cinfo)
{
     my_error_ptr myerr = (my_error_ptr) cinfo->err;
     (*cinfo->err->output_message)(cinfo);
     longjmp(myerr->setjmp_buffer, 1);
}

GLOBAL(void)
jpeg_memory_dest(j_compress_ptr cinfo, JOCTET *buffer,int bufsize)
{
  mem_dest_ptr dest;
  if (cinfo->dest == NULL) {    /* first time for this JPEG object? */
    cinfo->dest = (struct jpeg_destination_mgr *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
                                  sizeof(memory_destination_mgr));
  }
  
  dest = (mem_dest_ptr) cinfo->dest;
  dest->bufsize=bufsize;
  dest->buffer=buffer;
  dest->pub.init_destination = init_destination;
  dest->pub.empty_output_buffer = empty_output_buffer;
  dest->pub.term_destination = term_destination;
}

int
jpeg_compress(char *dst, char *src, int width, int height, int dstsize, int quality)
{
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  unsigned char *dataRGB = (unsigned char *)src;
  JSAMPROW row_pointer=(JSAMPROW)dataRGB;
  JOCTET *jpgbuff;
  mem_dest_ptr dest;
  int csize=0;

  /* zero out the compresion info structures and
     allocate a new compressor handle */
  memset (&cinfo,0,sizeof(cinfo));
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
 
  /* Setup JPEG datastructures */
  cinfo.image_width = width;      /* image width and height, in pixels */
  cinfo.image_height = height;
  cinfo.input_components = 3;   /* # of color components per pixel=3 RGB */
  cinfo.in_color_space = JCS_RGB;               
  jpgbuff = (JOCTET*)dst;

  /* Setup compression and do it */
  jpeg_memory_dest(&cinfo,jpgbuff,dstsize);
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality (&cinfo, quality, TRUE);
  jpeg_start_compress(&cinfo, TRUE);
  /* compress each scanline one-at-a-time */
  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer = (JSAMPROW)(dataRGB+(cinfo.next_scanline*3*width));
    jpeg_write_scanlines(&cinfo, &row_pointer, 1);
  }
  jpeg_finish_compress(&cinfo);
  /* Now extract the size of the compressed buffer */
  dest=(mem_dest_ptr)cinfo.dest;
  csize=dest->datacount; /* the actual compressed datasize */
  /* destroy the compressor handle */
  jpeg_destroy_compress(&cinfo);
  return csize;
}

static void 
init_source(j_decompress_ptr cinfo)
{
    /* nothing to do */
}

static boolean
fill_input_buffer(j_decompress_ptr cinfo)
{
    /* can't fill */
    return FALSE; 
}

static void
skip_input_data(j_decompress_ptr cinfo, long num_bytes)
{
    if ((size_t)num_bytes > cinfo->src->bytes_in_buffer) {
        cinfo->src->next_input_byte = NULL;
        cinfo->src->bytes_in_buffer = 0;
    } else {
        cinfo->src->next_input_byte += (size_t) num_bytes;
        cinfo->src->bytes_in_buffer -= (size_t) num_bytes;
    }
}

static void
term_source(j_decompress_ptr cinfo)
{
    /* nothing to do */
} 


/**
 * set momory-jpeg image to JPEG lib Info struct
 * @param cinfo  JPEG lib decompress infomation structure
 * @param ptr    JPEG image 
 * @param size   JPEG image size
 */
extern void
jpeg_memory_src(j_decompress_ptr cinfo, unsigned char *ptr, size_t size)
{
    struct jpeg_source_mgr *src;
    src = cinfo->src = (struct jpeg_source_mgr *)
        (*cinfo->mem->alloc_small) ((j_common_ptr)cinfo, 
                                    JPOOL_PERMANENT,
                                    sizeof(*src));
    src->init_source       = init_source;
    src->fill_input_buffer = fill_input_buffer;
    src->skip_input_data   = skip_input_data;
    src->resync_to_restart = jpeg_resync_to_restart;
    src->term_source     = term_source;
    src->next_input_byte = ptr;
    src->bytes_in_buffer = size;
}

void
jpeg_decompress(unsigned char *dst, int dst_size, unsigned char *src, int src_size) {
  struct jpeg_decompress_struct cinfo;
  //struct jpeg_error_mgr jerr;
  struct my_error_mgr mderr;
  int line_size,y;
  unsigned char *dstcur;

  //cinfo.err = jpeg_std_error(&jerr);
  cinfo.err = jpeg_std_error(&mderr.pub);
  mderr.pub.error_exit = my_error_exit;
  if(setjmp(mderr.setjmp_buffer)){
       jpeg_destroy_decompress(&cinfo);
       fprintf(stderr, "sonething very bad has happened\n");
       return;
  }
  jpeg_create_decompress(&cinfo);
  jpeg_memory_src(&cinfo, src, src_size);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  /*
  *w = cinfo.output_width;
  *h = cinfo.output_height;
  */
  line_size = cinfo.output_width*cinfo.output_components;
  assert(line_size * cinfo.output_height <= dst_size);

  dstcur = dst;
  for (y = 0; y < cinfo.output_height ; y++) {
    jpeg_read_scanlines(&cinfo,(JSAMPARRAY) &dstcur,1);
    dstcur += line_size;
  }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
}

void
jpeg_decompress_from_file(unsigned char *dst, char *file, int size, int *w, int *h) {
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  int line_size,y;
  unsigned char *dstcur;
  FILE *infile;

  infile = fopen(file, "rb");

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  *w = cinfo.output_width;
  *h = cinfo.output_height;
  line_size = cinfo.output_width*cinfo.output_components;

  dstcur = dst;
  for (y = 0; y < cinfo.output_height ; y++) {
    jpeg_read_scanlines(&cinfo,(JSAMPARRAY) &dstcur,1);
    dstcur += line_size;
  }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);
}


/////////////////////////////////////////////////////////////////////////////
// CCaptureCanonS5Dlg dialog


CCaptureCanonS5Dlg::CCaptureCanonS5Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCaptureCanonS5Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCaptureCanonS5Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bCreated = FALSE;

	m_nMinWidth = 100;
	m_nMinHeight = 100;

	m_nMaxWidth = 800;
	m_nMaxHeight = 600;

	m_hCamera = NULL;
}


void CCaptureCanonS5Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCaptureCanonS5Dlg)
	///DDX_Control(pDX, IDC_STATIC_INFOMATION, m_staticinformation);
	DDX_Control(pDX, IDC_STATIC_PHOTO_SIZE, m_Static_PhotoSize);
	DDX_Control(pDX, IDC_STATIC_PHOTO_QUALITY, m_Static_PhotoQuality);
	DDX_Control(pDX, IDC_PHOTO_QUALITY, m_CPhotQuality);
	DDX_Control(pDX, IDC_PHOTO_SIZE, m_CPhotoSize);
	DDX_Control(pDX, IDC_STATIC_SHOOTING_MODE, m_Static_ShootingMode);
	DDX_Control(pDX, IDC_STATIC_RELEASE_PARAM, m_Static_ReleaseParam);
	DDX_Control(pDX, IDC_STATIC_ISO_SPEED, m_Static_ISOSpeed);
	DDX_Control(pDX, IDC_STATIC_FLASH, m_Static_Flash);
	DDX_Control(pDX, IDC_STATIC_EXPO_COMP, m_Static_ExpoComp);
	DDX_Control(pDX, IDC_SHOOTING_MODE, m_CShootingMode);
	DDX_Control(pDX, IDC_ISO_SPEED, m_CISOSpeed);
	DDX_Control(pDX, IDC_FLASH, m_CFlash);
	DDX_Control(pDX, IDC_EXPO_COMP, m_CExpoComp);
	///DDX_Control(pDX, IDC_SAVEPATH, m_CSavePath);
	///DDX_Control(pDX, IDC_BROWSE, m_CBrowse);
	DDX_Control(pDX, IDC_AEAF, m_CAEAF);
	DDX_Control(pDX, IDC_VIEWIMG, m_CViewImage);
	///DDX_Control(pDX, IDC_VIEWFINDER, m_CViewFinder);
	DDX_Control(pDX, IDC_RELEASE, m_CRelease);
	///DDX_Control(pDX, IDC_RELCHECK4, m_CRelChk4);
	///DDX_Control(pDX, IDC_RELCHECK3, m_CRelChk3);
	///DDX_Control(pDX, IDC_RELCHECK2, m_CRelChk2);
	///DDX_Control(pDX, IDC_END, m_CEnd);
	///DDX_Control(pDX, IDC_DISCONNECT, m_CDisconnect);
	///DDX_Control(pDX, IDC_CONNECT, m_CConnect);
	///DDX_Text(pDX, IDC_INFORMATION, m_CInfoString);
	DDX_Control(pDX, IDC_SLIDERF, m_sliderf);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCaptureCanonS5Dlg, CDialog)
	//{{AFX_MSG_MAP(CCaptureCanonS5Dlg)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_AEAF, OnAeaf)
	ON_BN_CLICKED(IDC_RELEASE, OnRelease)
	ON_CBN_SELCHANGE(IDC_SHOOTING_MODE, OnSelchangeShootingMode)
	ON_CBN_SELCHANGE(IDC_EXPO_COMP, OnSelchangeExpoComp)
	ON_CBN_SELCHANGE(IDC_FLASH, OnSelchangeFlash)
	ON_CBN_SELCHANGE(IDC_ISO_SPEED, OnSelchangeISOSpeed)
	ON_CBN_SELCHANGE(IDC_PHOTO_SIZE, OnSelchangePhotoSize)
	ON_CBN_SELCHANGE(IDC_PHOTO_QUALITY, OnSelchangePhotoQuality)

	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERF, OnCustomdrawSliderf)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDERF, OnSliderReleaseCapture)

	//ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CCaptureCanonS5Dlg::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

/////////////////////////////////////////////////////////////////////////////
// CCaptureCanonS5Dlg message handlers

int CCaptureCanonS5Dlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
		
	m_bCreated = TRUE;

	return 0;
}


void CCaptureCanonS5Dlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	DisconnectCamera();
	Sleep(50);

	if (m_hCamera != 0L)
	{
		// Disconnect the camera device 
		(prVoid)PR_DisconnectCamera(m_hCamera);
		// Release the event callback function 
		(prVoid)PR_ClearEventCallBack(m_hCamera);
		// Delete the camera handle
		(prVoid)PR_DestroyCameraObject(m_hCamera);
	}
}


BOOL CCaptureCanonS5Dlg::OnInitDialog() 
{
	BOOL		fRes;
	char		szPath[MAX_PATH];
	char		*cpPos;
	prResponse	cErr;
	char		szErrStr[256];
	CRect		rect;

	CDialog::OnInitDialog();

	GetWindowRect(&rect);

	m_nMinWidth = rect.Width();
	m_nMaxWidth = rect.Width() + 400;
	m_nMinHeight = rect.Height();
	m_nMaxHeight = rect.Height();

	m_CpViewFinder = &m_CViewImage;
	memset( &m_BackSurface, 0, sizeof(DIBData) );
	m_fProgramRelease = FALSE;
	m_fCamTypeEOS = FALSE;

	m_fMCard = FALSE;
	m_fVFEnd = FALSE;

	/* PS-Rec SDK Area Initialize */
	m_hCamera = 0L;
	m_CpVFThread = NULL;
	m_fGetRelease = FALSE;

	/* A picture buffer is created. */
	fRes = CreateDIBBuffer();
	if( !fRes )
	{
		goto	apierr;
	}

	/* A message is registered. */
	m_ReleaseOnMessage = RegisterWindowMessage( CAMERAEVENTMESSAGESTRING_RELEASE_ON );
	m_ReleaseCompleteMessage = RegisterWindowMessage( CAMERAEVENTMESSAGESTRING_RELEASE_COMP );
	m_AbortPCEVF = RegisterWindowMessage( CAMERAEVENTMESSAGESTRING_ABORT_PC_EVF );
	m_ChangeByUI = RegisterWindowMessage( CAMERAEVENTMESSAGESTRING_CHANGE_BY_UI );
	m_CameraShutDown = RegisterWindowMessage(CAMERAEVENTMESSAGESTRING_SHUT_DOWN);

	if (	!m_ReleaseOnMessage
		||	!m_ReleaseCompleteMessage
		||	!m_AbortPCEVF
		||	!m_ChangeByUI
		||	!m_CameraShutDown 	)
	{
		goto	apierr;
	}

	/* Dialog controls are initialized. */
	m_CConnect.EnableWindow( TRUE );
	m_CDisconnect.EnableWindow( FALSE );
	m_CRelease.EnableWindow( FALSE );
	m_CViewFinder.EnableWindow( FALSE );
	m_CAEAF.EnableWindow( FALSE );
	
	///m_CRelChk2.EnableWindow( FALSE );
	///m_CRelChk3.EnableWindow( FALSE );
	///m_CRelChk4.EnableWindow( FALSE );
	
	///GetDlgItem(IDC_INFORMATION)->ShowWindow(SW_HIDE/*SW_HIDE*/);
	
	GetDlgItem(IDC_VIEWIMG)->ShowWindow(SW_HIDE/*SW_HIDE*/);

	///m_CRelChk2.SetCheck( 1 );
	///m_CRelChk3.SetCheck( 0 );
	///m_CRelChk4.SetCheck( 0 );
	

	m_sliderf.SetRange(0,100);
	m_sliderf.SetTicFreq(10);
	m_sliderf.SetPageSize(10);
    m_sliderf.ShowWindow(SW_HIDE);
    m_CAEAF.ShowWindow(SW_HIDE);
    m_staticinformation.ShowWindow(SW_HIDE);

	//	m_CInfoString.ShowWindow(SW_HIDE);



//	m_CPhotQuality.ShowWindow(SW_HIDE);
 //	m_CPhotoSize.ShowWindow(cmdshow);

//	m_Static_PhotoSize.ShowWindow(cmdshow);
//	m_Static_PhotoQuality.ShowWindow(cmdshow);

	///ShowReleaseParam(NULL);
	///ShowReleaseParam(NULL);

	m_CViewImage.SetWindowPos(	NULL, 0, 0, 
								VIEWFINDER_WIDTH, VIEWFINDER_HEIGHT, 
								SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER );
	
	GetModuleFileName( NULL, szPath, MAX_PATH );

	cpPos = strrchr( szPath, '\\' );
	if( cpPos )
	{
		*(cpPos+1) = '\0';
	}
	///m_CSavePath.SetWindowText( szPath );
	
	UpdateData(TRUE);
	m_CInfoString = "";
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE

	
apierr:
	MessageBox( "API Error" );
	EndDialog( 0 );
	return	FALSE;
	
camerr:
	
	if ( cErr != prOK )
	{
		wsprintf( szErrStr, "ErrorCode = 0x%08X", cErr );
	}

	MessageBox( szErrStr );

	AfxMessageBox("Init Error");

	//EndDialog( 1 );

	return	FALSE;
}


void CCaptureCanonS5Dlg::OnSelchangeShootingMode() 
{
	prUInt8		wMode;
	prResponse	cErr = prOK;

	wMode = (prUInt8)m_CShootingMode.GetItemData( m_CShootingMode.GetCurSel() );
	if ( m_ShootingModeCntVal== wMode )
	{
		return;
	}

	/* Set values of the Device Properties */
	cErr = PR_SetDevicePropValue(	m_hCamera,
									prPTP_DEV_PROP_EXPOSURE_MODE,
									(prUInt32)sizeof(prUInt8),
									&wMode );
	if ( cErr != prOK )
	{
		goto SDK_Error;
	}

	m_ShootingModeCntVal = wMode;

	/* The process for updating the Device Properties */
	UpdateSetting();

	return;

SDK_Error:
	char	szErrStr[256];

	wsprintf( szErrStr, "ErrorCode = 0x%08X", cErr );
	MessageBox( szErrStr );
}


void CCaptureCanonS5Dlg::OnSelchangeExpoComp() 
{
	prUInt8		wMode;
	prResponse	cErr = prOK;

	wMode = (prUInt8)m_CExpoComp.GetItemData( m_CExpoComp.GetCurSel() );
	if ( m_ExpoCompCntVal == wMode )
	{
		return;
	}

	/* Set values of the Device Properties */
	cErr = PR_SetDevicePropValue(	m_hCamera,
									prPTP_DEV_PROP_EXPOSURE_COMP,
									(prUInt32)sizeof(prUInt8),
									&wMode	);
	if ( cErr != prOK )
	{
		goto SDK_Error;
	}

	m_ExpoCompCntVal = wMode;

	/* The process for updating the Device Properties */
	UpdateSetting();

	return;

SDK_Error:
	char	szErrStr[256];

	wsprintf( szErrStr, "ErrorCode = 0x%08X", cErr );
	MessageBox( szErrStr );
}


void CCaptureCanonS5Dlg::OnSelchangeFlash() 
{
	prUInt8		bMode;
	prResponse	cErr = prOK;


	bMode = (prUInt8)m_CFlash.GetItemData( m_CFlash.GetCurSel() );
	if (m_FlashCntVal == bMode)
	{
		return;
	}

	/* Set values of the Device Properties */
	cErr = PR_SetDevicePropValue(	m_hCamera,
									prPTP_DEV_PROP_STROBE_SETTING,
									(prUInt32)sizeof(prUInt8),
									&bMode	);
	if ( cErr != prOK )
	{
		goto SDK_Error;
	}

	m_FlashCntVal = bMode;

	/* The process for the Device Properties */
	UpdateSetting();

	return;

SDK_Error:
	char	szErrStr[256];

	wsprintf( szErrStr, "ErrorCode = 0x%08X", cErr );
	MessageBox( szErrStr );
}


void CCaptureCanonS5Dlg::OnSelchangeISOSpeed() 
{
	prUInt16	wMode;
	prResponse	cErr = prOK;


	wMode = (prUInt8)m_CISOSpeed.GetItemData( m_CISOSpeed.GetCurSel() );
	if ( m_ISOCntVal == wMode )
	{
		return;
	}

	/* Set values of the Device Properties */
	cErr = PR_SetDevicePropValue(	m_hCamera,
									prPTP_DEV_PROP_ISO,
									(prUInt32)sizeof(prUInt16),
									&wMode	);
	if ( cErr != prOK )
	{
		goto SDK_Error;
	}

	m_ISOCntVal = wMode;

	/* The process of updating the Device Properties */
	UpdateSetting();

	return;

SDK_Error:
	char	szErrStr[256];

	wsprintf( szErrStr, "ErrorCode = 0x%08X", cErr );
	MessageBox( szErrStr );
}



void CCaptureCanonS5Dlg::OnSelchangePhotoSize() 
{
	prUInt16	wMode;
	prResponse	cErr = prOK;

	wMode = (prUInt8)m_CPhotoSize.GetItemData( m_CPhotoSize.GetCurSel() );
	if ( m_ImageSizeCntVal == wMode )
	{
		return;
	}
	
	/* Set values of the Device Properties */
	cErr = PR_SetDevicePropValue(	m_hCamera,
									prPTP_DEV_PROP_IMAGE_SIZE,
									(prUInt32)sizeof(prUInt8),
									&wMode	);
	if ( cErr != prOK )
	{
		goto SDK_Error;
	}

	m_ImageSizeCntVal = wMode;

	/* The process of updating the Device Properties */
	UpdateSetting();

	return;

SDK_Error:
	char	szErrStr[256];

	wsprintf( szErrStr, "ErrorCode = 0x%08X", cErr );
	MessageBox( szErrStr );
	
}


void CCaptureCanonS5Dlg::OnSelchangePhotoQuality() 
{
	prUInt16	wMode;
	prResponse	cErr = prOK;

	wMode = (prUInt8)m_CPhotQuality.GetItemData( m_CPhotQuality.GetCurSel() );
	if ( m_ImageQualityCntVal == wMode )
	{
		return;
	}
	
	/* Set values of the Device Properties */
	cErr = PR_SetDevicePropValue(	m_hCamera,
									prPTP_DEV_PROP_COMP_QUALITY,
									(prUInt32)sizeof(prUInt8),
									&wMode	);
	if ( cErr != prOK )
	{
		goto SDK_Error;
	}

	m_ImageQualityCntVal = wMode;

	/* The process of updating the Device Properties */
	UpdateSetting();

	return;

SDK_Error:
	char	szErrStr[256];

	wsprintf( szErrStr, "ErrorCode = 0x%08X", cErr );
	MessageBox( szErrStr );
}


void CCaptureCanonS5Dlg::OnAeaf() 
{
	prResponse				cErr = prOK;
	prptpAeAfAwbResetFlag	ActivateFlag = 0x00000007;

	/* AE and AF are readjusted. */
	cErr = PR_RC_DoAeAfAwb(m_hCamera,ActivateFlag);
	if ( cErr != prOK )
	{
		goto Error;
	}

	return;

Error:
	char	szErrStr[256];

	wsprintf( szErrStr, "ErrorCode = 0x%08X", cErr );
	MessageBox( szErrStr );
}


void CCaptureCanonS5Dlg::OnRelease() 
{
	prResponse	cErr = prOK;
	CMainFrame *pMainFrame = (CMainFrame *) AfxGetMainWnd();
	char		cSavePath[MAX_PATH];
	BOOL		bRet = TRUE;
	CProgress*	cPhotProg = NULL;
	CProgress*	cThumbProg = NULL;
	CWaitCursor wait;
	CString		sImageFileName;
char path[_MAX_PATH], fileName[_MAX_FNAME];

DWORD pid = 0;
CString strFileName;
	

	m_fGetRelease = TRUE;

	/* A photograph is taken. */
	cErr = PR_RC_Release( m_hCamera );
	if ( cErr != prOK )
	{
		goto SDK_Error;
	}

	memset(&cSavePath,0,MAX_PATH);

	strcpy(cSavePath, "d:\\");
	

	::GetTempPath(_MAX_PATH, path);
	::GetTempFileName(path, "", (UINT) pid, fileName);

	strFileName = CString(fileName);
	strFileName.MakeUpper();
	strFileName.Replace(".TMP", ".JPG");

	/* Save path is acuired. */
	//GetSavePath(&cSavePath[0],MAX_PATH);

	if ( m_ReleaseComp & FULL_VIEW_REL )
	{
		cPhotProg = new CProgress;
		/* The data of the main image is acuired. */

		bRet = cPhotProg->GetReleaseData(	m_hCamera,
											m_PicHandle,
											prPTP_FULL_VIEW_RELEASED,
											m_FileType,
											&cSavePath[0],
											(char *) (LPCTSTR) strFileName
										    );
			

		m_PicHandle = 0;
		cErr = cPhotProg->m_LastErr;
		delete cPhotProg;

		if ( bRet == FALSE )
		{
			m_ReleaseComp = 0;	
			goto SDK_Error;
		}

		CWnd *pWndImage;
		CMedImage *pViewImage;
		
		pWndImage = pMainFrame->m_wndTabDockBar.GetView(RUNTIME_CLASS(CMedImage));
		if (pWndImage != NULL)
		{
			pViewImage = (CMedImage *)pWndImage;

	//		AfxMessageBox(strFileName);

			pViewImage->LoadImageFromFile(strFileName);

			remove(strFileName);
		}
	}


	if ( m_ReleaseComp & THMB_VIEW_REL )
	{
		cThumbProg = new CProgress;
		/* The data of the thumbnail is acuired. */
		bRet = cThumbProg->GetReleaseData(	m_hCamera,
											m_ThmbHandle,
											prPTP_THUMBNAIL_RELEASED,
											m_FileType,
											&cSavePath[0],
											""
											);
		m_ThmbHandle = 0;
		cErr = cThumbProg->m_LastErr;
		delete cThumbProg;

		if ( bRet == FALSE )
		{
			m_ReleaseComp = 0;
			goto SDK_Error;
		}
	}

	m_ReleaseComp = 0;

	/* The information of number of shooting is updated. */
	///GetCameraInformation();

	m_fGetRelease = FALSE;
	
	return;

SDK_Error:
	char	szErrStr[256];
	wsprintf( szErrStr, "ErrorCode = 0x%08X", cErr );
	MessageBox( szErrStr );

	m_fGetRelease = FALSE;
}


void CCaptureCanonS5Dlg::OnSliderReleaseCapture(NMHDR* pNMHDR, LRESULT* pResult)
{
	int pos=0;	
	prResponse	cErr = prOK;
	prUInt16	Focusvalue= 0;
	prUInt16		cValue8;
	prUInt32	BufferSize1 = 0L;
	BufferSize1 = sizeof(cValue8);

	Focusvalue = m_sliderf.GetPos();
    m_sliderf.SetPos(Focusvalue);

	cErr = PR_GetDevicePropValue(	m_hCamera,
									prPTP_DEV_PROP_ZOOM_POS,
									&BufferSize1,
								    &cValue8	);
    // Focusvalue=pos;
	/* Transfer Fullview Image to PC */
	cErr = PR_SetDevicePropValue(	m_hCamera,
									prPTP_DEV_PROP_ZOOM_POS,//	prPTP_DEV_PROP_CAPTURE_TRANSFER_MODE,
									sizeof(Focusvalue),
	 								&Focusvalue	);						

	*pResult = 0;
}


void CCaptureCanonS5Dlg::OnCustomdrawSliderf(NMHDR* pNMHDR, LRESULT* pResult)
{
	int pos=0;	
	prResponse	cErr = prOK;
	prUInt16	Focusvalue= 0;
	prUInt16		cValue8;
	prUInt32	BufferSize1 = 0L;
	BufferSize1 = sizeof(cValue8);

	Focusvalue = m_sliderf.GetPos();
    m_sliderf.SetPos(Focusvalue);

	cErr = PR_GetDevicePropValue(	m_hCamera,
									prPTP_DEV_PROP_ZOOM_POS,
									&BufferSize1,
								    &cValue8	);
    // Focusvalue=pos;
	/* Transfer Fullview Image to PC */
	cErr = PR_SetDevicePropValue(	m_hCamera,
									prPTP_DEV_PROP_ZOOM_POS,//	prPTP_DEV_PROP_CAPTURE_TRANSFER_MODE,
									sizeof(Focusvalue),
	 								&Focusvalue	);						

	*pResult = 0;
}


void CCaptureCanonS5Dlg::ConnectCamera()
{
	//SDK_AND_INFO	cSelectInfo;
	///CComSettingDlg* pComSettingDialog= NULL;
	prResponse  eErr = prOK;
	prUInt32	BufferSize = 0L;

	prUInt8      Buffer[MY_BUFFER_SIZE];
	prUInt8      DeviceListBuffer[MY_BUFFER_SIZE];
	
	prDeviceList *pDeviceList = NULL;

	prUInt16	wMode = 0;
	prUInt16	wCurrentValue = 0;
	prBoolean	IsRC,IsEVF,IsAeaf,IsExpoMode,IsExpoComp,IsStrobeSet,IsISO,IsPhotoEffect,IsRet;

	IsRC = FALSE;
	IsEVF = FALSE;
	IsAeaf = FALSE;
	IsExpoMode = FALSE;
	IsExpoComp = FALSE;
	IsStrobeSet = FALSE;
	IsISO = FALSE;
	IsPhotoEffect = FALSE;

	/* Create Buffer */
	
	///pComSettingDialog = new CComSettingDlg( this );

	///pComSettingDialog->SelectSource( &cSelectInfo );

	///delete pComSettingDialog;

  //  m_CAEAF.ShowWindow(SW_SHOW);
//	 m_staticinformation.ShowWindow(SW_SHOW);
	//m_CInfoString.ShowWindow(SW_SHOW);

	///if ( cSelectInfo.m_SelectedSDK == 0 )
	///{
	///	return;
	///}


	/* Enumerate camera devices */
	BufferSize = MY_BUFFER_SIZE;
	memset(Buffer, 0, MY_BUFFER_SIZE);
	eErr = PR_GetDeviceList(&BufferSize, (prDeviceList *)Buffer);
	
	if (eErr != prOK) 
	{
		goto ErrorFinish;
	}

	memset(DeviceListBuffer, 0, MY_BUFFER_SIZE);
	memcpy(DeviceListBuffer, Buffer, BufferSize);
	pDeviceList = (prDeviceList *)DeviceListBuffer;

	if (pDeviceList->NumList == 0L)
	{
		/* No camera is connected */
		goto ErrorFinish;
	}

	/* Making of camera steering wheel */
	eErr = PR_CreateCameraObject( &(pDeviceList->DeviceInfo[0]), &m_hCamera );
	if ( eErr != prOK )
	{
		goto ErrorFinish;
	}

	/* Registration of event callback function */
	eErr = PR_SetEventCallBack(	m_hCamera,
								(prContext)this,
								(prSetEventCB*)&CamCallBackFunc );
	if ( eErr != prOK )
	{
		goto ErrorFinish;
	}

	/* Connection of camera device */
	eErr = PR_ConnectCamera(m_hCamera);
	if ( eErr != prOK )
	{
		goto ErrorFinish;
	}

	/* Device information is acuired. */
	memset(Buffer,0,MY_BUFFER_SIZE );
	BufferSize = MY_BUFFER_SIZE;
	eErr = PR_GetDeviceInfo(m_hCamera, &BufferSize, Buffer);
	if ( eErr != prOK )
	{
		goto ErrorFinish;
	}

	/* Check if Release Control is supported */
	IsSupportedRCandEVF( Buffer ,&IsRC ,&IsEVF ,&IsAeaf );
	if ( IsRC == FALSE )	/* not support Release Control */
	{
		goto ErrorFinish;
	}

	/* Release Control mode start */
	eErr = PR_InitiateReleaseControl( m_hCamera );
	if ( eErr != prOK )
	{
		goto ErrorFinish;
	}

	m_CRelease.EnableWindow( TRUE );
	m_fInReleaseControl = TRUE;

	if ( IsEVF == TRUE )		/* Support Viewfinder */
	{
		m_CViewFinder.EnableWindow( TRUE );
	}
	if ( IsAeaf == TRUE )	/* Support PR_RC_DoAeAfAwb */
	{
		m_CAEAF.EnableWindow( TRUE );
	}

	/* Device information is acuired. */
	memset( Buffer, 0, MY_BUFFER_SIZE);
	BufferSize = MY_BUFFER_SIZE;
	eErr = PR_GetDeviceInfo( m_hCamera, &BufferSize, Buffer );
	if ( eErr != prOK )
	{
		goto ErrorFinish;
	}

	IsSupportedCapRelPrm( Buffer,
						  &IsExpoMode,
						  &IsExpoComp,
						  &IsStrobeSet,
						  &IsISO,
						  &IsPhotoEffect );

	/* EXPOSURE_MODE is enable. */
	if ( IsExpoMode == TRUE )
	 {
		m_Static_ShootingMode.ShowWindow(SW_SHOW);
		m_CShootingMode.ShowWindow(SW_SHOW);
		m_Static_ReleaseParam.ShowWindow(SW_SHOW);
		SetShootingModeBox();
	 }

	/* EXPOSURE_COMP is enable. */
	if ( IsExpoComp == TRUE )
	{
		m_Static_ExpoComp.ShowWindow(SW_SHOW);
		m_CExpoComp.ShowWindow(SW_SHOW);
		m_Static_ReleaseParam.ShowWindow(SW_SHOW);
		SetExpoCompBox();
	}

	/* STROBE_SETTING is enable. */
	if ( IsStrobeSet == TRUE )
	{
		m_Static_Flash.ShowWindow(SW_SHOW);
		m_CFlash.ShowWindow(SW_SHOW);
		m_Static_ReleaseParam.ShowWindow(SW_SHOW);
		SetFlashBox();
	}

	/* ISO is enable. */
	if ( IsISO == TRUE )
	{
		m_Static_ISOSpeed.ShowWindow(SW_SHOW);
		m_CISOSpeed.ShowWindow(SW_SHOW);
		m_Static_ReleaseParam.ShowWindow(SW_SHOW);
		SetISOSpeedBox();
	}

	 m_CPhotQuality.ShowWindow(SW_SHOW);
	 m_CPhotoSize.ShowWindow(SW_SHOW);

     m_CAEAF.ShowWindow(SW_SHOW);
	 m_staticinformation.ShowWindow(SW_SHOW);

	m_Static_PhotoSize.ShowWindow(SW_SHOW);
	m_Static_PhotoQuality.ShowWindow(SW_SHOW);

	////GetDlgItem(IDC_INFORMATION)->ShowWindow(SW_SHOW/*SW_HIDE*/);
    //GetDlgItem(IDC_INFORMATION)->ShowWindow(SW_HIDE/*SW_HIDE*/);
	GetDlgItem(IDC_VIEWIMG)->ShowWindow(SW_SHOW/*SW_HIDE*/);
   
	SetImageSizeBox();
   
	SetImageQualityBox();
 
	/* CameraInformation */
	GetCameraInformation();

	/* Capture mode is acuired. */
	memset( Buffer, 0, MY_BUFFER_SIZE);
	BufferSize = MY_BUFFER_SIZE;

	eErr = PR_GetDevicePropDesc(	m_hCamera,
									prPTP_DEV_PROP_CAPTURE_TRANSFER_MODE,
									&BufferSize,
									Buffer	);
	if ( eErr != prOK )
	{
		goto ErrorFinish;
	}

	/* Check which mode is supported */
	IsRet = IsSuppotedCapTransMode( Buffer,&wCurrentValue,&wMode );
	if ( IsRet == FALSE )
	{
		goto ErrorFinish;
	}

	/* Initiate check boxes */
	/*
	
	m_CRelChk2.SetCheck(0);
	m_CRelChk3.SetCheck(0);
	m_CRelChk4.SetCheck(0);
	if ( wMode & 0x0002 )
	{
		m_CRelChk2.EnableWindow(TRUE);
	}
	if ( wMode & 0x0004 )
	{
		m_CRelChk3.EnableWindow(TRUE);
	}
	if ( wMode & 0x0008 )
	{
		m_CRelChk4.EnableWindow(TRUE);
	}

	*/

	/* Set Default Capture Transfer Mode */
	//if(wMode & 0x0002)
	{
		prUInt16	wTransMode = 0x0002;	/* Transfer Fullview Image to PC */
		eErr = PR_SetDevicePropValue(	m_hCamera,
										prPTP_DEV_PROP_CAPTURE_TRANSFER_MODE,
										sizeof(wTransMode),
										&wTransMode	);
	}

	/*
	if ( wCurrentValue & 0x0002 )
	{
		m_CRelChk2.SetCheck(1);
	}
	if ( wCurrentValue & 0x0004 )
	{
		m_CRelChk3.SetCheck(1);
	}
	if ( wCurrentValue & 0x0008 )
	{
		m_CRelChk4.SetCheck(1);
	}

	*/


	/* Check the image type */
	memset( Buffer,0,MY_BUFFER_SIZE );
	BufferSize = MY_BUFFER_SIZE;
	eErr = PR_GetDevicePropDesc(	m_hCamera,
									prPTP_DEV_PROP_FULLVIEW_FILE_FORMAT,
									&BufferSize,
									Buffer	);
	if ( eErr != prOK )
	{
		goto ErrorFinish;
	}
	/* Check the image type for current shooting */
	IsRet = IsReleasePictureType( Buffer );
	if ( IsRet == FALSE )
	{
		goto ErrorFinish;
	}

    ZoomPos();
	
	m_CConnect.EnableWindow( FALSE );
	m_CDisconnect.EnableWindow( TRUE );

	OpenViewFinder();

	return;

ErrorFinish:

	DisconnectCamera();

	if ( eErr != prOK )
	{
		char	szErrStr[256];
		wsprintf( szErrStr, "ErrorCode = 0x%08X", eErr );
		MessageBox( szErrStr );
	}
	else
	{
		MessageBox( "Not Supported or Data Error" );
	}
}




void CCaptureCanonS5Dlg::DisconnectCamera()
{
	CWaitCursor wait;

	prResponse	cErr = prOK;

	if ( m_hCamera != 0L )
	{
		/* Is Viewfinder running? */
		if ( m_fVFEnd == TRUE )
		{
			/* A view finder is ended. */
			///OnViewfinder();

			m_fVFEnd = FALSE;
			/* End the thread for displaying a picture */
			if ( m_CpVFThread )
			{
				m_CpVFThread->ResumeThread();
				WaitForSingleObject( m_CpVFThread->m_hThread, INFINITE );
				::CloseHandle(m_CpVFThread->m_hThread);
				m_CpVFThread = NULL;
			}

			Invalidate();
			UpdateWindow();

			/* Terminate Viewfinder */
			PR_RC_TermViewFinder(m_hCamera);
		}

		/* Terminate Release Control mode */
		(prVoid)PR_TerminateReleaseControl(m_hCamera);
		/* Disconnect the camera */
		(prVoid)PR_DisconnectCamera(m_hCamera);
		/* Release the camera events */
		(prVoid)PR_ClearEventCallBack(m_hCamera);
		/* Destory camera object */
		(prVoid)PR_DestroyCameraObject(m_hCamera);

		m_hCamera = NULL;
		m_fInReleaseControl = FALSE;
	}
}


void CCaptureCanonS5Dlg::OpenViewFinder()
{
	if (m_fVFEnd) return;
	
	if (m_CpVFThread == NULL)
	{		
		/* Start the thread for displaying a picutre */
		m_CpVFThread = AfxBeginThread(	(AFX_THREADPROC)ViewFinderProc,
										(LPVOID)this,
										0,
										0,
										CREATE_SUSPENDED,
										NULL	);
	
		/* Start Viewfinder */
		PR_RC_StartViewFinder(m_hCamera,
										(prContext)this,
										(prViewFinderCB*)&ViewFinderCallBackFun	);


		m_fVFEnd = TRUE;
	}

	///m_CViewFinder.SetWindowText( "停止预览" );
}


void CCaptureCanonS5Dlg::CloseViewFinder()
{
	m_fVFEnd = FALSE;
	/* End the thread for displaying a picture */

	if ( m_CpVFThread )
	{		
		m_CpVFThread->ResumeThread();
		WaitForSingleObject( m_CpVFThread->m_hThread, INFINITE );
		::CloseHandle(m_CpVFThread->m_hThread);

		m_CpVFThread = NULL;
	}

	Invalidate();
	UpdateWindow();

	if (m_hCamera != NULL)
	{
		/* Terminate Viewfinder */
		PR_RC_TermViewFinder(m_hCamera);
	
		///m_CViewFinder.SetWindowText( "预览" );	
	}
}

BOOL CCaptureCanonS5Dlg::Connect()
{
	BOOL bRet = FALSE;

	if (m_hCamera == NULL) ConnectCamera();
	
	OpenViewFinder();
	
	ShowWindow(SW_SHOW);

	return TRUE;
}


BOOL CCaptureCanonS5Dlg::Disconnect()
{
	BOOL bRet = FALSE;

	CloseViewFinder();

	DisconnectCamera();

	ShowWindow(SW_HIDE);

	return TRUE;
}


void CCaptureCanonS5Dlg::HideWindow()
{
	CloseViewFinder();

	ShowWindow(SW_HIDE);
}

/////////////////////////////////////////////////////////////////////////////
// Call back function

prResponse prSTDCALL CCaptureCanonS5Dlg::CamCallBackFunc(prHandle		CameraHandle,
										prContext		Context,
										prVoid*			pEventData)
{
	CCaptureCanonS5Dlg		*CpThis;
	EVENT_GENERIC_CONTAINER		*pEventDataTemp;
		
	CpThis = (CCaptureCanonS5Dlg*)Context;
	pEventDataTemp = (EVENT_GENERIC_CONTAINER *)pEventData;

	switch ( pEventDataTemp->Code )
	{
		case prCAL_SHUTDOWN:
			CpThis->PostMessage(CpThis->m_CameraShutDown);
			break;
		case prPTP_ABORT_PC_EVF:
			CpThis->PostMessage(CpThis->m_AbortPCEVF);
			break;
		case prPTP_FULL_VIEW_RELEASED:
			CpThis->m_PicHandle = pEventDataTemp->Parameter[0];
			CpThis->m_ReleaseComp |= FULL_VIEW_REL;
			break;
		case prPTP_THUMBNAIL_RELEASED:
			CpThis->m_ThmbHandle = pEventDataTemp->Parameter[0];
			CpThis->m_ReleaseComp |= THMB_VIEW_REL;
			break;
		case prPTP_CAPTURE_COMPLETE:
			break;
		case prPTP_PUSHED_RELEASE_SW:
			CpThis->PostMessage( CpThis->m_ReleaseOnMessage );
			break;
		case prPTP_RC_PROP_CHANGED:
		case prPTP_DEVICE_PROP_CHANGED:
			CpThis->PostMessage( CpThis->m_ChangeByUI );
			break;
	}
	
	return	prOK;
}

prResponse prSTDCALL CCaptureCanonS5Dlg::ViewFinderCallBackFun(prHandle		CameraHandle,
														 prContext		Context,
														 prUInt32		Size,
														 prVoid*		pEventData)
{
	CCaptureCanonS5Dlg		*CpThis;

	CpThis = (CCaptureCanonS5Dlg*)Context;

	CpThis->m_CVFMutex.Lock();

	memset( &CpThis->m_ViewFinderData[0],0,VF_BUFFER_SIZE );

	/* Copy the image data */
	memcpy( &CpThis->m_ViewFinderData[0],(prUInt8*)pEventData, Size );
	CpThis->m_VF_DataSize = Size;

	CpThis->m_CVFMutex.Unlock();

	if ( CpThis->m_CpVFThread )
	{
		/* Resume the thread for displaying a picture */
		CpThis->m_CpVFThread->ResumeThread();
		
	}

	return prOK;
}

/////////////////////////////////////////////////////////////////////////////
// Thread function

/* The thread for displaying a picture */
UINT WINAPI	CCaptureCanonS5Dlg::ViewFinderProc( LPVOID	vpParam )
{
	BOOL			*fpEnd;
	HDC				hdcDest;
	prUInt8			cBmpVuffer[VIEWFINDER_WIDTH * VIEWFINDER_HEIGHT * 3], cv;
	prUInt8*		cpBM_Buffer;
	prUInt8*		cpVF_Buffer;
	prUInt8*		cpScanLine_x;
	prUInt32		lCnt, ly;
	prUInt32		SavePoint;
	prUInt32*		cpVF_Size;
	CCaptureCanonS5Dlg		*CpThis;

	BITMAPINFO	bmiDIB;

	CpThis = (CCaptureCanonS5Dlg*)vpParam;
	fpEnd = (BOOL*)(&CpThis->m_fVFEnd);

	cpVF_Buffer = &CpThis->m_ViewFinderData[0];

	cpVF_Size = &CpThis->m_VF_DataSize;

	memset( &bmiDIB, 0, sizeof(bmiDIB) );
	bmiDIB.bmiHeader.biSize				= sizeof(BITMAPINFOHEADER);
	bmiDIB.bmiHeader.biWidth			= VIEWFINDER_WIDTH;
	bmiDIB.bmiHeader.biHeight			= VIEWFINDER_HEIGHT * -1;
	bmiDIB.bmiHeader.biPlanes			= 1;
	bmiDIB.bmiHeader.biBitCount			= 24;
	bmiDIB.bmiHeader.biCompression		= BI_RGB;

	while ( *fpEnd)
	{
		CpThis->m_CVFMutex.Lock();

		// A picture is displayed. 
		hdcDest = ::GetDC( CpThis->m_CpViewFinder->GetSafeHwnd() );	

		// Initiate the Jpeg library 
		memset( &CpThis->m_Proj, 0, sizeof(CpThis->m_Proj) );

		CpThis->m_Proj.cinfo.err	= jpeg_std_error( &CpThis->m_Proj.jerr);

		jpeg_create_decompress( &CpThis->m_Proj.cinfo);

		memset( &cBmpVuffer[0], 0, sizeof(cBmpVuffer));

		jpeg_memory_src(&CpThis->m_Proj.cinfo, cpVF_Buffer, *cpVF_Size);
		
		// Read JPEG header 
		jpeg_read_header( &CpThis->m_Proj.cinfo, TRUE);
		jpeg_start_decompress( &CpThis->m_Proj.cinfo);

	
		for ( lCnt = 0; lCnt < CpThis->m_Proj.cinfo.image_height; lCnt++ )
		{
			SavePoint = lCnt * 3 * VIEWFINDER_WIDTH;
			cpBM_Buffer = &cBmpVuffer[SavePoint];
			
			// Convert JPEG into BMP 
			jpeg_read_scanlines( &CpThis->m_Proj.cinfo, &cpBM_Buffer, 1);


			// Swap B,R channel data 
			cpScanLine_x = cpBM_Buffer;
			for (ly = 0; ly < VIEWFINDER_WIDTH; ly ++)
			{
				cv = *cpScanLine_x;
				*(cpScanLine_x) = *(cpScanLine_x + 2);
				*(cpScanLine_x + 2) = cv;
				cpScanLine_x += 3;
			}			
		}

		// The process for displaying a picture 
		::SetDIBitsToDevice(
			hdcDest,
			0, 0, VIEWFINDER_WIDTH, VIEWFINDER_HEIGHT,
			0, 0,
			0 , VIEWFINDER_HEIGHT,
			&cBmpVuffer[0], &bmiDIB,
			DIB_RGB_COLORS
		);
		
		jpeg_finish_decompress( &CpThis->m_Proj.cinfo);

		// finish the Jpeg library
		jpeg_destroy_decompress( &CpThis->m_Proj.cinfo);

		::ReleaseDC( CpThis->m_CpViewFinder->GetSafeHwnd(), hdcDest );

		CpThis->m_CVFMutex.Unlock();
		

		/* A thread is suspended. */
		CpThis->m_CpVFThread->SuspendThread();

	}


	return	0;
}



///////////////////////////////////////////////////////////////////////////////
BOOL	CCaptureCanonS5Dlg::CreateDIBBuffer()
{
	BITMAPINFO	BmpInfo;

	memset( &BmpInfo, 0, sizeof(BITMAPINFO) );
	BmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BmpInfo.bmiHeader.biWidth = VIEWFINDER_WIDTH;
	BmpInfo.bmiHeader.biHeight = VIEWFINDER_HEIGHT;
	BmpInfo.bmiHeader.biPlanes = 1;
	BmpInfo.bmiHeader.biBitCount = 24;
	BmpInfo.bmiHeader.biCompression = BI_RGB;

	m_BackSurface.hBmp = ::CreateDIBSection(	NULL,
												&BmpInfo,
												DIB_RGB_COLORS,
												&m_BackSurface.vpBits,
												NULL,
												0	);
	if( !m_BackSurface.hBmp )
	{
		return	FALSE;
	}
	
	return	TRUE;
}



///////////////////////////////////////////////////////////////////////////////

void CCaptureCanonS5Dlg::ZoomPos()
{
   prUInt8*			pDataBuffer = NULL;
	prUInt8*			pBuffer = NULL;
	prUInt8				bGetSet, bFormFlag;
//	prUInt16			index;
	prUInt16			wDataType;
//	prUInt16			wNum,i;
//	prUInt16			wMode;
	prUInt32			BufferSize = 0L;
	prResponse			err = prOK;
	prptpDevicePropCode	DevicePropCode;

//	if ( !m_CISOSpeed.IsWindowVisible() )
//	{
//		return;
//	}
//	m_CISOSpeed.EnableWindow( FALSE );
	// Remove all items from the list box.
//	m_CISOSpeed.ResetContent();

    m_sliderf.ShowWindow(SW_SHOW);
	pDataBuffer = new prUInt8 [MY_BUFFER_SIZE];
	memset(pDataBuffer,0,MY_BUFFER_SIZE);
	BufferSize = MY_BUFFER_SIZE;
	err = PR_GetDevicePropDesc(	m_hCamera,
								prPTP_DEV_PROP_ZOOM_POS,
								&BufferSize,
								pDataBuffer	);
	if ( err != prOK )
	{
		goto SDK_Error;
	}

	/* Analysis of DevicePropDesc */
	pBuffer = pDataBuffer;

	/* DevicePropCode */
	DevicePropCode = *((prptpDevicePropCode *)pBuffer);
	if ( DevicePropCode != prPTP_DEV_PROP_ZOOM_POS )
	{
		/* Error, if not prPTP_DEV_PROP_ISO */
		goto SDK_Error;
	}
	pBuffer += sizeof(prptpDevicePropCode);

	/* DataType */
	wDataType = *((prUInt16 *)pBuffer);
	if ( wDataType != 0x0004 )
	{
		/* Error, if not prUInt16 */
		goto SDK_Error;
	}
	
	pBuffer += sizeof(prUInt16);

	/* GetSet */
	bGetSet = *((prUInt8 *)pBuffer);
	if ( bGetSet != 0x01 )
	{
		/* Error, if not GetSet */
		goto SDK_Error;
	}
	pBuffer += sizeof(prUInt8);

	/* FactoryDefaultValue */
	pBuffer +=  sizeof(prUInt16);

	/* CurrentValue */
	m_ZoomPosCntVal = *((prUInt16 *)pBuffer);
	pBuffer += sizeof(prUInt16);

	/* FormFlag */
	bFormFlag = *((prUInt8 *)pBuffer);
	if ( bFormFlag != 0x01)
	{
		/* Error, if not Enumulation-Form */
		goto SDK_Error;
	}
	
	pBuffer += sizeof(prUInt8);

	/* Range-Form */
	m_min = *((prUInt16 *)pBuffer);
	pBuffer += sizeof(prUInt16);
	m_max = *((prUInt16 *)pBuffer);
	pBuffer += sizeof(prUInt16);
    m_step = *((prUInt16*)pBuffer);

	m_sliderf.SetRange(0,m_max);
	m_sliderf.SetTicFreq(m_max/10);

	m_sliderf.SetPageSize(m_max/10);//m_max/10

//	m_CISOSpeed.SetItemData( index, wMode );
	//	if ( m_ZoomPosCntVal == wMode )
	//	{
			// Set current setting to edit control of the Combo box 
	//		m_CISOSpeed.SetCurSel( index );
	//	}
		pBuffer += sizeof(prUInt16);
///	}
	if ( pDataBuffer )
	{
		delete [] pDataBuffer;
	}
//	m_CISOSpeed.EnableWindow(TRUE);

	return;

SDK_Error:
	if ( pDataBuffer )
	{
		delete [] pDataBuffer;
	}

	if ( err != prOK )
	{
		char	szErrStr[256];
		wsprintf( szErrStr, "ErrorCode = 0x%08X", err );
		MessageBox( szErrStr );
	}
}



void CCaptureCanonS5Dlg::SetShootingModeBox()
{
	prUInt8*			pDataBuffer = NULL;
	prUInt8*			pBuffer = NULL;
	prUInt8				bFormFlag,bGetSet;
	prUInt8				bMode;
	prUInt16			index;
	prUInt16			wDataType;
	prUInt16			wNum,i;
	prUInt32			BufferSize = 0L;
	prResponse			err = prOK;
	prptpDevicePropCode	DevicePropCode;

	m_CShootingMode.EnableWindow( FALSE );
	// Remove all items from the list box.
	m_CShootingMode.ResetContent();

	pDataBuffer = new prUInt8 [MY_BUFFER_SIZE];
	memset(pDataBuffer,0,MY_BUFFER_SIZE);
	BufferSize = MY_BUFFER_SIZE;

	err = PR_GetDevicePropDesc(	m_hCamera,
								prPTP_DEV_PROP_EXPOSURE_MODE,
								&BufferSize,
								pDataBuffer );
	if ( err!= prOK )
	{
		/* Error, if not prPTP_DEV_PROP_EXPOSURE_MODE */
		goto SDK_Error;
	}

	/* Analysis of DevicePropDesc */
	pBuffer = pDataBuffer;

	/* DevicePropCode */
	DevicePropCode = *((prptpDevicePropCode *)pBuffer);
	if ( DevicePropCode != prPTP_DEV_PROP_EXPOSURE_MODE )
	{
		/* Error, if not prPTP_DEV_PROP_EXPOSURE_MODE */
		goto SDK_Error;
	}
	pBuffer += sizeof(prptpDevicePropCode);

	/* DataType */
	wDataType = *((prUInt16 *)pBuffer);
	if ( wDataType != 0x0002 )
	{
		/* Error, if not prUInt8 */
		goto SDK_Error;
	}
	pBuffer += sizeof(prUInt16);

	/* GetSet */
	bGetSet = *((prUInt8 *)pBuffer);
	if ( bGetSet != 0x01 )
	{
		/* Error, if not GetSet */
		goto SDK_Error;
	}
	pBuffer += sizeof(prUInt8);

	/* FactoryDefaultValue */
	pBuffer +=  sizeof(prUInt8);

	/* CurrentValue */
	m_ShootingModeCntVal = *((prUInt8 *)pBuffer);
	pBuffer += sizeof(prUInt8);

	/* FormFlag */
	bFormFlag = *((prUInt8 *)pBuffer);
	if ( bFormFlag != 0x02 )
	{
		/* Error, if not Enumulation */
		goto SDK_Error;
	}
	pBuffer += sizeof(prUInt8);

	/* Enumeration-Form */
	wNum = *((prUInt16 *)pBuffer);
	pBuffer += sizeof(prUInt16);
	for ( i = 0; i < wNum; i++ )
	{
		bMode = *((prUInt8 *)pBuffer);
		switch ( bMode )
		{
			case 0x00:	index = m_CShootingMode.AddString( "Auto" );			break;
			case 0x01:	index = m_CShootingMode.AddString( "P" );				break;
			case 0x02:	index = m_CShootingMode.AddString( "Tv" );				break;
			case 0x03:	index = m_CShootingMode.AddString( "Av" );				break;
			case 0x04:	index = m_CShootingMode.AddString( "M" );				break;
			case 0x05:	index = m_CShootingMode.AddString( "A_DEP" );			break;
			case 0x06:	index = m_CShootingMode.AddString( "M_DEP" );			break;
			case 0x07:	index = m_CShootingMode.AddString( "Bulb" );			break;
			case 0x80:	index = m_CShootingMode.AddString( "CAMERAM" );			break;
			case 0x81:	index = m_CShootingMode.AddString( "MYCOLOR" );			break;
			case 0x82:	index = m_CShootingMode.AddString( "PORTRAIT" );		break;
			case 0x83:	index = m_CShootingMode.AddString( "LANDSCAPE" );		break;
			case 0x84:	index = m_CShootingMode.AddString( "NIGHTSCENE" );		break;
			case 0x85:	index = m_CShootingMode.AddString( "FOREST" );			break;
			case 0x86:	index = m_CShootingMode.AddString( "SNOW" );			break;
			case 0x87:	index = m_CShootingMode.AddString( "BEACH" );			break;
			case 0x88:	index = m_CShootingMode.AddString( "FIREWORKS" );		break;
			case 0x89:	index = m_CShootingMode.AddString( "PARTY" );			break;
			case 0x8a:	index = m_CShootingMode.AddString( "NIGHTSNAP" );		break;
			case 0x8b:	index = m_CShootingMode.AddString( "STITCH" );			break;
			case 0x8c:	index = m_CShootingMode.AddString( "MOVIE" );			break;
			case 0x8d:	index = m_CShootingMode.AddString( "CUSTOM" );			break;
			case 0x8e:	index = m_CShootingMode.AddString( "INTERVAL" );		break;
			case 0x8f:	index = m_CShootingMode.AddString( "DIGITALMACRO" );	break;
			case 0x90:	index = m_CShootingMode.AddString( "LONGSHUTTER" );		break;
			case 0x91:	index = m_CShootingMode.AddString( "UNDERWATER" );		break;
			case 0x92:	index = m_CShootingMode.AddString( "KIDSANDPETS" );		break;
			case 0x93:	index = m_CShootingMode.AddString( "FASTSHUTTER" );		break;
			case 0x94:	index = m_CShootingMode.AddString( "SLOWSHUTTER" );		break;
			case 0x95:	index = m_CShootingMode.AddString( "CUSTOM1" );			break;
			case 0x96:	index = m_CShootingMode.AddString( "CUSTOM2" );			break;
			case 0x97:	index = m_CShootingMode.AddString( "NEUTRAL" );			break;
			case 0x98:	index = m_CShootingMode.AddString( "GRAY" );			break;
			case 0x99:	index = m_CShootingMode.AddString( "SEPIA" );			break;
			case 0x9A:	index = m_CShootingMode.AddString( "VIVID" );			break;
			case 0x9B:	index = m_CShootingMode.AddString( "SPORTS" );			break;
			case 0x9C:	index = m_CShootingMode.AddString( "MACRO" );			break;
			case 0x9D:	index = m_CShootingMode.AddString( "SUPERMACRO" );		break;
			case 0x9E:	index = m_CShootingMode.AddString( "PANFOCUS" );		break;
			case 0x9F:	index = m_CShootingMode.AddString( "BW" );				break;
			case 0xA0:	index = m_CShootingMode.AddString( "FLASHINHIBIT" );	break;
			default:	index = m_CShootingMode.AddString( "(Error)" );			break;
		};
		m_CShootingMode.SetItemData( index, bMode );
		if ( m_ShootingModeCntVal == bMode )
		{
			// Set current setting to edit control of the Combo box 
			m_CShootingMode.SetCurSel( index );
		}
		pBuffer += sizeof(prUInt8);
	}
	if ( pDataBuffer )
	{
		delete [] pDataBuffer;
	}
	m_CShootingMode.EnableWindow( TRUE );

	return;

SDK_Error:
	if ( pDataBuffer )
	{
		delete [] pDataBuffer;
	}

	if ( err != prOK )
	{
		char	szErrStr[256];
		wsprintf( szErrStr, "ErrorCode = 0x%08X", err );
		MessageBox( szErrStr );
	}
}

void CCaptureCanonS5Dlg::SetExpoCompBox()
{

	prUInt8*			pDataBuffer = NULL;
	prUInt8*			pBuffer = NULL;
	prUInt8				bGetSet, bFormFlag;
	prUInt8				bMode;
	prUInt16			index;
	prUInt16			wDataType;
	prUInt16			wNum,i;
	prUInt32			BufferSize = 0L;
	prResponse			err = prOK;
	prptpDevicePropCode	DevicePropCode;

	
	m_CExpoComp.EnableWindow( FALSE );
	// Remove all items from the list box.
	m_CExpoComp.ResetContent();
	
	pDataBuffer = new prUInt8 [MY_BUFFER_SIZE];
	memset(pDataBuffer,0,MY_BUFFER_SIZE);
	BufferSize = MY_BUFFER_SIZE;
	err = PR_GetDevicePropDesc(	m_hCamera,
								prPTP_DEV_PROP_EXPOSURE_COMP,
								&BufferSize,
								pDataBuffer	);
	if ( err!= prOK )
	{
		goto SDK_Error;
	}

	/* Analysis of DevicePropDesc */
	pBuffer = pDataBuffer;

	/* DevicePropCode */
	DevicePropCode = *((prptpDevicePropCode *)pBuffer);
	if ( DevicePropCode != prPTP_DEV_PROP_EXPOSURE_COMP )
	{
		/* Error, if not prPTP_DEV_PROP_EXPOSURE_COMP */
		goto SDK_Error;
	}
	pBuffer += sizeof(prptpDevicePropCode);

	/* DataType */
	wDataType = *((prUInt16 *)pBuffer);
	if ( wDataType != 0x0002 )
	{
		/* Error, if not prUInt8 */
		goto SDK_Error;
	}
	pBuffer += sizeof(prUInt16);

	/* GetSet */
	bGetSet = *((prUInt8 *)pBuffer);
	if ( bGetSet != 0x01 )
	{
		/* Error, if not GetSet */
		goto SDK_Error;
	}
	pBuffer += sizeof(prUInt8);

	/* FactoryDefaultValue */
	pBuffer +=  sizeof(prUInt8);

	/* CurrentValue */
	m_ExpoCompCntVal = *((prUInt8 *)pBuffer);
	pBuffer += sizeof(prUInt8);

	/* FormFlag */
	bFormFlag = *((prUInt8 *)pBuffer);
	if ( bFormFlag != 0x02 )
	{
		/* Error, if not Enumulation */
		goto SDK_Error;
	}
	pBuffer += sizeof(prUInt8);

	/* Enumeration-Form */
	wNum = *((prUInt16 *)pBuffer);
	pBuffer += sizeof(prUInt16);
	for ( i = 0; i < wNum; i++ )
	{
		bMode = *((prUInt8 *)pBuffer);
		/* add set data to the the combo box */
		switch ( bMode )
		{
			case 0x00:	index = m_CExpoComp.AddString( "+3" );		break;
			case 0x03:	index = m_CExpoComp.AddString( "+2(2/3)" );	break;
			case 0x04:	index = m_CExpoComp.AddString( "+2(1/2)" );	break;
			case 0x05:	index = m_CExpoComp.AddString( "+2(1/3)" );	break;
			case 0x08:	index = m_CExpoComp.AddString( "+2" );		break;
			case 0x0b:	index = m_CExpoComp.AddString( "+1(2/3)" );	break;
			case 0x0c:	index = m_CExpoComp.AddString( "+1(1/2)" );	break;
			case 0x0d:	index = m_CExpoComp.AddString( "+1(1/3)" );	break;
			case 0x10:	index = m_CExpoComp.AddString( "+1" );		break;
			case 0x13:	index = m_CExpoComp.AddString( "+2/3" );	break;
			case 0x14:	index = m_CExpoComp.AddString( "+1/2" );	break;
			case 0x15:	index = m_CExpoComp.AddString( "+1/3" );	break;
			case 0x18:	index = m_CExpoComp.AddString( "0" );		break;
			case 0x1b:	index = m_CExpoComp.AddString( "-1/3" );	break;
			case 0x1c:	index = m_CExpoComp.AddString( "-1/2" );	break;
			case 0x1d:	index = m_CExpoComp.AddString( "-2/3" );	break;
			case 0x20:	index = m_CExpoComp.AddString( "-1" );		break;
			case 0x23:	index = m_CExpoComp.AddString( "-1(1/3)" );	break;
			case 0x24:	index = m_CExpoComp.AddString( "-1(1/2)" );	break;
			case 0x25:	index = m_CExpoComp.AddString( "-1(2/3)" );	break;
			case 0x28:	index = m_CExpoComp.AddString( "-2" );		break;
			case 0x2b:	index = m_CExpoComp.AddString( "-2(1/3)" );	break;
			case 0x2c:	index = m_CExpoComp.AddString( "-2(1/2)" );	break;
			case 0x2d:	index = m_CExpoComp.AddString( "-2(2/3)" );	break;
			case 0x30:	index = m_CExpoComp.AddString( "-3" );		break;
			default:	index = m_CExpoComp.AddString( "(Error)" );	break;	
		};
		m_CExpoComp.SetItemData( index, bMode );
		if ( m_ExpoCompCntVal == bMode )
		{
			// Set current setting to edit control of the Combo box 
			m_CExpoComp.SetCurSel( index );
		}
		pBuffer += sizeof(prUInt8);
	}
	if ( pDataBuffer )
	{
		delete [] pDataBuffer;
	}
	m_CExpoComp.EnableWindow(TRUE);

	return;

SDK_Error:
	if ( pDataBuffer )
	{
		delete [] pDataBuffer;
	}

	if ( err != prOK )
	{
		char	szErrStr[256];
		wsprintf( szErrStr, "ErrorCode = 0x%08X", err );
		MessageBox( szErrStr );
	}

}

void CCaptureCanonS5Dlg::SetFlashBox()
{
	prUInt8*			pDataBuffer = NULL;
	prUInt8*			pBuffer = NULL;
	prUInt8				bGetSet, bFormFlag;
	prUInt8				bMode;
	prUInt16			index;
	prUInt16			wDataType;
	prUInt16			wNum,i;
	prUInt32			BufferSize = 0L;
	prResponse			err = prOK;
	prptpDevicePropCode	DevicePropCode;


	m_CFlash.EnableWindow( FALSE );
	// Remove all items from the list box.
	m_CFlash.ResetContent();
	
	pDataBuffer = new prUInt8 [MY_BUFFER_SIZE];
	memset(pDataBuffer,0,MY_BUFFER_SIZE);
	BufferSize = MY_BUFFER_SIZE;
	err = PR_GetDevicePropDesc(	m_hCamera,
								prPTP_DEV_PROP_STROBE_SETTING,
								&BufferSize,
								pDataBuffer	);
	if ( err != prOK )
	{
		goto SDK_Error;
	}

	/* Analysis of DevicePropDesc */
	pBuffer = pDataBuffer;

	/* DevicePropCode */
	DevicePropCode = *((prptpDevicePropCode *)pBuffer);
	if ( DevicePropCode != prPTP_DEV_PROP_STROBE_SETTING )
	{
		/* Error, if not prPTP_DEV_PROP_STROBE_SETTING */
		goto SDK_Error;
	}
	pBuffer += sizeof(prptpDevicePropCode);

	/* DataType */
	wDataType = *((prUInt16 *)pBuffer);
	if ( wDataType != 0x0002 )
	{
		/* Error, if not prUInt8 */
		goto SDK_Error;
	}
	pBuffer += sizeof(prUInt16);

	/* GetSet */
	bGetSet = *((prUInt8 *)pBuffer);
	if ( bGetSet != 0x01 )
	{
		/* Error, if not GetSet */
		goto SDK_Error;
	}
	pBuffer += sizeof(prUInt8);

	/* FactoryDefaultValue */
	pBuffer +=  sizeof(prUInt8);

	/* CurrentValue */
	m_FlashCntVal = *((prUInt8 *)pBuffer);
	pBuffer += sizeof(prUInt8);

	/* FormFlag */
	bFormFlag = *((prUInt8 *)pBuffer);
	if ( bFormFlag != 0x02 )
	{
		/* Error, if not Enumulation-Form */
		goto SDK_Error;
	}
	pBuffer += sizeof(prUInt8);

	/* Enumeration-Form */
	wNum = *((prUInt16 *)pBuffer);
	pBuffer += sizeof(prUInt16);
	for ( i = 0; i < wNum; i++ )
	{
		bMode = *((prUInt8 *)pBuffer);
		/* add set data to the combo box */
		switch ( bMode )
		{
			case 0x00:	index = m_CFlash.AddString( "Off" );			break;
			case 0x01:	index = m_CFlash.AddString( "Auto" );			break;
			case 0x02:	index = m_CFlash.AddString( "On" );				break;
			case 0x03:	index = m_CFlash.AddString( "Red Eye" );		break;
			case 0x04:	index = m_CFlash.AddString( "Slow Sync." );		break;
			case 0x05:	index = m_CFlash.AddString( "Auto (Red Eye)" );	break;
			case 0x06:	index = m_CFlash.AddString( "On (Red Eye)" );	break;
			default:	index = m_CFlash.AddString( "(Error)" );		break;	
		};
		m_CFlash.SetItemData( index, bMode );
		if ( m_FlashCntVal == bMode )
		{
			// Set current setting to edit control of the Combo box 
			m_CFlash.SetCurSel( index );
		}
		pBuffer += sizeof(prUInt8);
	}
	if ( pDataBuffer )
	{
		delete [] pDataBuffer;
	}
	m_CFlash.EnableWindow(TRUE);

	return;

SDK_Error:
	if ( pDataBuffer )
	{
		delete [] pDataBuffer;
	}

	if ( err != prOK )
	{
		char	szErrStr[256];
		wsprintf( szErrStr, "ErrorCode = 0x%08X", err );
		MessageBox( szErrStr );
	}

}



void CCaptureCanonS5Dlg::SetISOSpeedBox()	
{
	prUInt8*			pDataBuffer = NULL;
	prUInt8*			pBuffer = NULL;
	prUInt8				bGetSet, bFormFlag;
	prUInt16			index;
	prUInt16			wDataType;
	prUInt16			wNum,i;
	prUInt16			wMode;
	prUInt32			BufferSize = 0L;
	prResponse			err = prOK;
	prptpDevicePropCode	DevicePropCode;


	
	m_CISOSpeed.EnableWindow( FALSE );
	// Remove all items from the list box.
	m_CISOSpeed.ResetContent();

	pDataBuffer = new prUInt8 [MY_BUFFER_SIZE];
	memset(pDataBuffer,0,MY_BUFFER_SIZE);
	BufferSize = MY_BUFFER_SIZE;
	err = PR_GetDevicePropDesc(	m_hCamera,
								prPTP_DEV_PROP_ISO,
								&BufferSize,
								pDataBuffer	);
	if ( err != prOK )
	{
		goto SDK_Error;
	}

	/* Analysis of DevicePropDesc */
	pBuffer = pDataBuffer;

	/* DevicePropCode */
	DevicePropCode = *((prptpDevicePropCode *)pBuffer);
	if ( DevicePropCode != prPTP_DEV_PROP_ISO )
	{
		/* Error, if not prPTP_DEV_PROP_ISO */
		goto SDK_Error;
	}
	pBuffer += sizeof(prptpDevicePropCode);

	/* DataType */
	wDataType = *((prUInt16 *)pBuffer);
	if ( wDataType != 0x0004 )
	{
		/* Error, if not prUInt16 */
		goto SDK_Error;
	}
	pBuffer += sizeof(prUInt16);

	/* GetSet */
	bGetSet = *((prUInt8 *)pBuffer);
	if ( bGetSet != 0x01 )
	{
		/* Error, if not GetSet */
		goto SDK_Error;
	}
	pBuffer += sizeof(prUInt8);

	/* FactoryDefaultValue */
	pBuffer +=  sizeof(prUInt16);

	/* CurrentValue */
	m_ISOCntVal = *((prUInt16 *)pBuffer);
	pBuffer += sizeof(prUInt16);

	/* FormFlag */
	bFormFlag = *((prUInt8 *)pBuffer);
	if ( bFormFlag != 0x02 )
	{
		/* Error, if not Enumulation-Form */
		goto SDK_Error;
	}
	pBuffer += sizeof(prUInt8);

	/* Enumeration-Form */
	wNum = *((prUInt16 *)pBuffer);
	pBuffer += sizeof(prUInt16);
	for ( i = 0; i < wNum; i++ )
	{
		wMode = *((prUInt16 *)pBuffer);
		/* add set data to the combo box */
		switch ( wMode )
		{
			case 0x0000:	index = m_CISOSpeed.AddString( "AUTO" );		break;
			case 0x0028:	index = m_CISOSpeed.AddString( "6" );			break;
			case 0x0030:	index = m_CISOSpeed.AddString( "12" );			break;
			case 0x0038:	index = m_CISOSpeed.AddString( "25" );			break;
			case 0x0040:	index = m_CISOSpeed.AddString( "50" );			break;
			case 0x0043:	index = m_CISOSpeed.AddString( "64" );			break;
			case 0x0045:	index = m_CISOSpeed.AddString( "80" );			break;
        	case 0x0048:	index = m_CISOSpeed.AddString( "100" );			break;
			case 0x0050:	index = m_CISOSpeed.AddString( "200" );			break;
			case 0x0058:	index = m_CISOSpeed.AddString( "400" );			break;
			case 0x0060:	index = m_CISOSpeed.AddString( "800" );			break;
			case 0x0068:	index = m_CISOSpeed.AddString( "1600" );		break;
			case 0x0070:	index = m_CISOSpeed.AddString( "3200" );		break;
			case 0x0078:	index = m_CISOSpeed.AddString( "6400" );		break;
			default:		index = m_CISOSpeed.AddString( "(Error)" );		break;	
		};
		m_CISOSpeed.SetItemData( index, wMode );
		if ( m_ISOCntVal == wMode )
		{
			// Set current setting to edit control of the Combo box 
			m_CISOSpeed.SetCurSel( index );
		}
		pBuffer += sizeof(prUInt16);
	}
	if ( pDataBuffer )
	{
		delete [] pDataBuffer;
	}
	m_CISOSpeed.EnableWindow(TRUE);

	return;

SDK_Error:
	if ( pDataBuffer )
	{
		delete [] pDataBuffer;
	}

	if ( err != prOK )
	{
		char	szErrStr[256];
		wsprintf( szErrStr, "ErrorCode = 0x%08X", err );
		MessageBox( szErrStr );
	}
}




void CCaptureCanonS5Dlg::GetCameraInformation()
{

	CString		CAddStr;
	prResponse	cErr = prOK;
	prUInt8		cValue8;
	prUInt32	BufferSize = 0L;
	prUInt32	Num;

	UpdateData( TRUE );
	m_CInfoString = "";

	/* Quality is acquired. */
	BufferSize = sizeof(cValue8);
	cErr = PR_GetDevicePropValue(	m_hCamera,
									prPTP_DEV_PROP_COMP_QUALITY,
									&BufferSize,
									&cValue8	);
	if ( cErr == prOK )
	{
		switch ( cValue8 )
		{
			case 0x01:
				CAddStr.Format( "图像质量 = Economy\r\n" );
				break;
			case 0x02:
				CAddStr.Format( "图像质量 = Normal\r\n" );
				break;
			case 0x03:
				CAddStr.Format( "图像质量 = Fine\r\n" );
				break;
			case 0x04:
				CAddStr.Format( "图像质量 = Lossless\r\n" );
				break;
			case 0x05:
				CAddStr.Format( "图像质量 = SuperFine\r\n" );
				break;
			default:
				CAddStr.Format( "图像质量 = Unknown\r\n" );
				break;
		}
		m_CInfoString += CAddStr;
	}
	else
	{
		CAddStr.Format( "<err>prPTP_DEV_PROP_COMP_QUALITY = 0x%08X\r\n", cErr );
		m_CInfoString += CAddStr;
	}

	/* ImageSize is acquired. */
	BufferSize = sizeof(cValue8);
	cErr = PR_GetDevicePropValue(	m_hCamera,
									prPTP_DEV_PROP_IMAGE_SIZE,
									&BufferSize,
									&cValue8	);
	if ( cErr == prOK )
	{
		switch ( cValue8 )
		{
			case 0x00:
				CAddStr.Format( "图像大小 = Large\r\n" );
				break;
			case 0x01:
				CAddStr.Format( "图像大小 = Medium1\r\n" );
				break;
			case 0x02:
				CAddStr.Format( "图像大小 = Small\r\n" );
				break;
			case 0x03:
				CAddStr.Format( "图像大小 = Medium2\r\n" );
				break;
			case 0x07:
				CAddStr.Format( "图像大小 = Medium3\r\n" );
				break;
			default:
				CAddStr.Format( "图像大小 = Unknown\r\n" );
				break;
		}
		m_CInfoString += CAddStr;
	}
	else
	{
		CAddStr.Format( "<err>prPTP_DEV_PROP_IMAGE_SIZE = 0x%08X\r\n", cErr );
		m_CInfoString += CAddStr;
	}

	/* Shooting mode is acquired. */
	BufferSize = sizeof(cValue8);
	cErr = PR_GetDevicePropValue(	m_hCamera,
									prPTP_DEV_PROP_EXPOSURE_MODE,
									&BufferSize,
									&cValue8	);
	if ( cErr == prOK )
	{
		switch ( cValue8 )
		{
			case 0x00:
				CAddStr.Format( "拍摄模式 = Auto\r\n" );
				break;
			case 0x01:
				CAddStr.Format( "拍摄模式 = P\r\n" );
				break;
			case 0x02:
				CAddStr.Format( "拍摄模式 = Tv\r\n" );
				break;
			case 0x03:
				CAddStr.Format( "拍摄模式 = Av\r\n" );
				break;
			case 0x04:
				CAddStr.Format( "拍摄模式 = M\r\n" );
				break;
			case 0x05:
				CAddStr.Format( "拍摄模式 = A_DEP\r\n" );
				break;
			case 0x06:
				CAddStr.Format( "拍摄模式 = M_DEP\r\n" );
				break;
			case 0x07:
				CAddStr.Format( "拍摄模式 = Bulb\r\n" );
				break;
			case 0x80:
				CAddStr.Format( "拍摄模式 = CAMERAM\r\n" );
				break;
			case 0x81:
				CAddStr.Format( "拍摄模式 = MYCOLOR\r\n" );
				break;
			case 0x82:
				CAddStr.Format( "拍摄模式 = PORTRAIT\r\n" );
				break;
			case 0x83:
				CAddStr.Format( "拍摄模式 = LANDSCAPE\r\n" );
				break;
			case 0x84:
				CAddStr.Format( "拍摄模式 = NIGHTSCENE\r\n" );
				break;
			case 0x85:
				CAddStr.Format( "ShootingMode = FOREST\r\n" );
				break;
			case 0x86:
				CAddStr.Format( "拍摄模式 = SNOW\r\n" );
				break;
			case 0x87:
				CAddStr.Format( "拍摄模式 = BEACH\r\n" );
				break;
			case 0x88:
				CAddStr.Format( "拍摄模式 = FIREWORKS\r\n" );
				break;
			case 0x89:
				CAddStr.Format( "拍摄模式 = PARTY\r\n" );
				break;
			case 0x8a:
				CAddStr.Format( "拍摄模式 = NIGHTSNAP\r\n" );
				break;
			case 0x8b:
				CAddStr.Format( "拍摄模式 = STITCH\r\n" );
				break;
			case 0x8c:
				CAddStr.Format( "拍摄模式 = MOVIE\r\n" );
				break;
			case 0x8d:
				CAddStr.Format( "拍摄模式 = CUSTOM\r\n" );
				break;
			case 0x8e:
				CAddStr.Format( "拍摄模式 = INTERVAL\r\n" );
				break;
			case 0x8f:
				CAddStr.Format( "拍摄模式 = DIGITALMACRO\r\n" );
				break;
			case 0x90:
				CAddStr.Format( "拍摄模式 = LONGSHUTTER\r\n" );
				break;
			case 0x91:
				CAddStr.Format( "拍摄模式 = UNDERWATER\r\n" );
				break;
			case 0x92:
				CAddStr.Format( "拍摄模式 = KIDSANDPETS\r\n" );
				break;
			case 0x93:
				CAddStr.Format( "拍摄模式 = FASTSHUTTER\r\n" );
				break;
			case 0x94:
				CAddStr.Format( "拍摄模式 = SLOWSHUTTER\r\n" );
				break;
			case 0x95:
				CAddStr.Format( "拍摄模式 = CUSTOM1\r\n" );
				break;
			case 0x96:
				CAddStr.Format( "拍摄模式 = CUSTOM2\r\n" );
				break;
			case 0x97:
				CAddStr.Format( "拍摄模式 = NEUTRAL\r\n" );
				break;
			case 0x98:
				CAddStr.Format( "拍摄模式 = GRAY\r\n" );
				break;
			case 0x99:
				CAddStr.Format( "拍摄模式 = SEPIA\r\n" );
				break;
			case 0x9A:
				CAddStr.Format( "拍摄模式 = VIVID\r\n" );
				break;
			case 0x9B:
				CAddStr.Format( "拍摄模式 = SPORTS\r\n" );
				break;
			case 0x9C:
				CAddStr.Format( "拍摄模式 = MACRO\r\n" );
				break;
			case 0x9D:
				CAddStr.Format( "拍摄模式 = SUPERMACRO\r\n" );
				break;
			case 0x9E:
				CAddStr.Format( "拍摄模式 = PANFOCUS\r\n" );
				break;
			case 0x9F:
				CAddStr.Format( "拍摄模式 = BW\r\n" );
				break;
			case 0xA0:
				CAddStr.Format( "拍摄模式 = FLASHINHIBIT\r\n" );
				break;
			default:
				CAddStr.Format( "拍摄模式 = (Error)\r\n" );
				break;
		};
		m_CInfoString += CAddStr;
	}
	else
	{
		CAddStr.Format( "<err>prPTP_DEV_PROP_EXPOSURE_MODE = 0x%08X\r\n", cErr );
		m_CInfoString += CAddStr;
	}

	/* The number of sheets which can be remaining photoed is acquired. */
	cErr = PR_RC_GetNumAvailableShot( m_hCamera, &Num );
	if ( cErr == prOK )
	{
		CAddStr.Format( "NumAvailableShot = %d\r\n", Num );
		m_CInfoString += CAddStr;
	}
	else
	{
		CAddStr.Format( "<err>PR_RC_GetNumAvailableShot = 0x%08X\r\n", cErr );
		m_CInfoString += CAddStr;
	}

	UpdateData( FALSE );
}



void CCaptureCanonS5Dlg::SetImageSizeBox()
{
	prUInt8*			pDataBuffer = NULL;
	prUInt8*			pBuffer = NULL;
	prUInt8				bGetSet, bFormFlag;
	prUInt16			index;
	prUInt16			wDataType;
	prUInt16			wNum,i;
	prUInt16			wMode;
	prUInt32			BufferSize = 0L;
	prResponse			err = prOK;
	prptpDevicePropCode	DevicePropCode;

//	if ( !m_CISOSpeed.IsWindowVisible() )
//	{
//		return;
//	}
	m_CPhotoSize.EnableWindow( FALSE );
	// Remove all items from the list box.
	m_CPhotoSize.ResetContent();

	pDataBuffer = new prUInt8 [MY_BUFFER_SIZE];
	memset(pDataBuffer,0,MY_BUFFER_SIZE);
	BufferSize = MY_BUFFER_SIZE;
	err = PR_GetDevicePropDesc(	m_hCamera,
								prPTP_DEV_PROP_IMAGE_SIZE,
								&BufferSize,
								pDataBuffer	);
	if ( err != prOK )
	{
		goto SDK_Error;
	}

	/* Analysis of DevicePropDesc */
	pBuffer = pDataBuffer;

	/* DevicePropCode */
	DevicePropCode = *((prptpDevicePropCode *)pBuffer);
	if ( DevicePropCode != prPTP_DEV_PROP_IMAGE_SIZE )
	{
		/* Error, if not prPTP_DEV_PROP_ISO */
		goto SDK_Error;
	}
	pBuffer += sizeof(prptpDevicePropCode);

	/* DataType */
	wDataType = *((prUInt16 *)pBuffer);
	if ( wDataType != 0x0002 )
	{
		/* Error, if not prUInt16 */
		goto SDK_Error;
	}
	pBuffer += sizeof(prUInt16);

	/* GetSet */
	bGetSet = *((prUInt8 *)pBuffer);
	if ( bGetSet != 0x01 )
	{
		/* Error, if not GetSet */
		goto SDK_Error;
	}
	pBuffer += sizeof(prUInt8);

	/* FactoryDefaultValue */
	pBuffer +=  sizeof(prUInt8);

	/* CurrentValue */
	m_ImageSizeCntVal = *((prUInt8 *)pBuffer);
	pBuffer += sizeof(prUInt8);

	/* FormFlag */
	bFormFlag = *((prUInt8 *)pBuffer);
	if ( bFormFlag != 0x02 )
	{
		/* Error, if not Enumulation-Form */
		goto SDK_Error;
	}
	pBuffer += sizeof(prUInt8);

	/* Enumeration-Form */
	wNum = *((prUInt16 *)pBuffer);
	pBuffer += sizeof(prUInt16);
	for ( i = 0; i < wNum; i++ )
	{
		wMode = *((prUInt8 *)pBuffer);
		/* add set data to the combo box */
		switch ( wMode )
		{
			case 0x00:	index = m_CPhotoSize.AddString( "Large" );		break;
			case 0x01:	index = m_CPhotoSize.AddString( "Medium 1" );	break;
			case 0x02:	index = m_CPhotoSize.AddString( "Small" );		break;
			case 0x03:	index = m_CPhotoSize.AddString( "Medium 2" );	break;
			case 0x07:	index = m_CPhotoSize.AddString( "Medium 3" );	break;
		
			default:		index = m_CPhotoSize.AddString( "(Error)" );	break;	
		};
		m_CPhotoSize.SetItemData( index, wMode );
		if ( m_ImageSizeCntVal == wMode )
		{
			// Set current setting to edit control of the Combo box 
			m_CPhotoSize.SetCurSel( index );
		}
		pBuffer += sizeof(prUInt8);
	}
	if ( pDataBuffer )
	{
		delete [] pDataBuffer;
	}
	m_CPhotoSize.EnableWindow(TRUE);

	return;

SDK_Error:
	if ( pDataBuffer )
	{
		delete [] pDataBuffer;
	}

	if ( err != prOK )
	{
		char	szErrStr[256];
		wsprintf( szErrStr, "ErrorCode = 0x%08X", err );
		MessageBox( szErrStr );
	}
	
}

void CCaptureCanonS5Dlg::SetImageQualityBox()
{
    prUInt8*			pDataBuffer = NULL;
	prUInt8*			pBuffer = NULL;
	prUInt8				bGetSet, bFormFlag;
	prUInt16			index;
	prUInt16			wDataType;
	prUInt16			wNum,i;
	prUInt16			wMode;
	prUInt32			BufferSize = 0L;
	prResponse			err = prOK;
	prptpDevicePropCode	DevicePropCode;

//	if ( !m_CISOSpeed.IsWindowVisible() )
//	{
//		return;
//	}
	m_CPhotQuality.EnableWindow( FALSE );
	// Remove all items from the list box.
	m_CPhotQuality.ResetContent();

	pDataBuffer = new prUInt8 [MY_BUFFER_SIZE];
	memset(pDataBuffer,0,MY_BUFFER_SIZE);
	BufferSize = MY_BUFFER_SIZE;
	err = PR_GetDevicePropDesc(	m_hCamera,
								prPTP_DEV_PROP_COMP_QUALITY,
								&BufferSize,
								pDataBuffer	);
	if ( err != prOK )
	{
		goto SDK_Error;
	}

	/* Analysis of DevicePropDesc */
	pBuffer = pDataBuffer;

	/* DevicePropCode */
	DevicePropCode = *((prptpDevicePropCode *)pBuffer);
	if ( DevicePropCode != prPTP_DEV_PROP_COMP_QUALITY )
	{
		/* Error, if not prPTP_DEV_PROP_ISO */
		goto SDK_Error;
	}
	pBuffer += sizeof(prptpDevicePropCode);

	/* DataType */
	wDataType = *((prUInt16 *)pBuffer);
	if ( wDataType != 0x0002 )
	{
		/* Error, if not prUInt16 */
		goto SDK_Error;
	}
	pBuffer += sizeof(prUInt16);

	/* GetSet */
	bGetSet = *((prUInt8 *)pBuffer);
	if ( bGetSet != 0x01 )
	{
		/* Error, if not GetSet */
		goto SDK_Error;
	}
	pBuffer += sizeof(prUInt8);

	/* FactoryDefaultValue */
	pBuffer +=  sizeof(prUInt8);

	/* CurrentValue */
	m_ImageQualityCntVal = *((prUInt8 *)pBuffer);
	pBuffer += sizeof(prUInt8);

	/* FormFlag */
	bFormFlag = *((prUInt8 *)pBuffer);
	if ( bFormFlag != 0x02 )
	{
		/* Error, if not Enumulation-Form */
		goto SDK_Error;
	}
	pBuffer += sizeof(prUInt8);

	/* Enumeration-Form */
	wNum = *((prUInt16 *)pBuffer);
	pBuffer += sizeof(prUInt16);
	for ( i = 0; i < wNum; i++ )
	{
		wMode = *((prUInt8 *)pBuffer);
		/* add set data to the combo box */
		switch ( wMode )
		{
			case 0x00:	index = m_CPhotQuality.AddString( "not define" );		break;
			case 0x01:	index = m_CPhotQuality.AddString( "Economy" );	break;
			case 0x02:	index = m_CPhotQuality.AddString( "Normal" );		break;
			case 0x03:	index = m_CPhotQuality.AddString( "Fine" );	break;
			case 0x04:	index = m_CPhotQuality.AddString( "Lossless" );	break;
			case 0x05:	index = m_CPhotQuality.AddString( "SuperFine" );	break;
		
			default:		index = m_CPhotQuality.AddString( "(Error)" );	break;	
		};
		m_CPhotQuality.SetItemData( index, wMode );
		if ( m_ImageQualityCntVal == wMode )
		{
			// Set current setting to edit control of the Combo box 
			m_CPhotQuality.SetCurSel( index );
		}
		pBuffer += sizeof(prUInt8);
	}
	if ( pDataBuffer )
	{
		delete [] pDataBuffer;
	}
	m_CPhotQuality.EnableWindow(TRUE);

	return;

SDK_Error:
	if ( pDataBuffer )
	{
		delete [] pDataBuffer;
	}

	if ( err != prOK )
	{
		char	szErrStr[256];
		wsprintf( szErrStr, "ErrorCode = 0x%08X", err );
		MessageBox( szErrStr );
	}
}


///////////////////////////////////////////////////////////////////////////////

prVoid	CCaptureCanonS5Dlg::UpdateSetting()
{
	prUInt8*	pBuffer = NULL;
	prUInt8*	pWorkBuffer = NULL;
	prUInt16	cwNum,cwCnt;
	prUInt32	clBufSize;
	prResponse	cErr = prOK;
	prptpDevicePropCode cGetDevCode;

	/* The list of the updated device propertis is acuired. */
	pBuffer = new prUInt8[MY_BUFFER_SIZE];
	memset( pBuffer, 0, MY_BUFFER_SIZE);
	clBufSize = MY_BUFFER_SIZE;

	cErr = PR_RC_GetChangedReleaseParamesList(	m_hCamera,
												&clBufSize,
												pBuffer	);
	if ( cErr != prOK )
	{
		goto SDK_Error;
	}

	/* If there are the updated device propertied, */
        /* the combo boxes about the updated device properties are updated. */
	pWorkBuffer = pBuffer;
	cwNum = *((prUInt16 *)pWorkBuffer);
	pWorkBuffer += sizeof(prUInt16);
	for ( cwCnt = 0;cwCnt < cwNum;cwCnt++ )
	{
		cGetDevCode = *((prptpDevicePropCode *)pWorkBuffer);
		switch ( cGetDevCode )
		{
			case prPTP_DEV_PROP_ISO:
				SetISOSpeedBox();
				break;
            case prPTP_DEV_PROP_IMAGE_SIZE:
				SetImageSizeBox();
				break;
            case prPTP_DEV_PROP_COMP_QUALITY:
				SetImageQualityBox();
				break;			
			case prPTP_DEV_PROP_EXPOSURE_MODE:
				SetShootingModeBox();
				GetCameraInformation();
				break;
			case prPTP_DEV_PROP_STROBE_SETTING:
				SetFlashBox();
				break;
			case prPTP_DEV_PROP_EXPOSURE_COMP:
				SetExpoCompBox();
				break;
		}
		pWorkBuffer += sizeof(prptpDevicePropCode);
	}

	if ( pBuffer )
	{
		delete [] pBuffer;
	}

	return;

SDK_Error:
	if ( pBuffer )
	{
		delete [] pBuffer;
	}

	char	szErrStr[256];
	wsprintf( szErrStr, "ErrorCode = 0x%08X", cErr );
	MessageBox( szErrStr );
}


///////////////////////////////////////////////////////////////////////////////

prVoid	CCaptureCanonS5Dlg::IsSupportedCapRelPrm(
	prUInt8*	pDeviceInfo,	/* (IN) DeviceInfo data set */
	prBoolean*	pExpoureMode,
	prBoolean*	pExpoureComp,
	prBoolean*	pSTrobeSet,
	prBoolean*	pISO,
	prBoolean*	pPhotoEffect)
{
	prUInt8		*pDeviceInfoTmp;
	prUInt8		bNum;
	prUInt32	dwNum, i;
	prptpDevicePropCode	wDeviceProp;

	pDeviceInfoTmp = pDeviceInfo;

	/* Standard version */
	/* Vendor extended ID */
	/* Vendor extended version */
	pDeviceInfoTmp +=   sizeof(prUInt16)
					  + sizeof(prUInt32)
					  + sizeof(prUInt16);

	/* Vendor extended information */
	bNum = *((prUInt8 *)pDeviceInfoTmp);
	pDeviceInfoTmp +=   sizeof(prUInt8)
					  + sizeof(prWChar) * bNum;

	/* Function modes */
	pDeviceInfoTmp +=   sizeof(prUInt16);

	/* Supported operations */
	dwNum = *((prUInt32 *)pDeviceInfoTmp);
	pDeviceInfoTmp +=   sizeof(prUInt32)
					  + sizeof(prOperationCode) * dwNum;

	/* Supported events */
	dwNum = *((prUInt32 *)pDeviceInfoTmp);
	pDeviceInfoTmp +=   sizeof(prUInt32)
					  + sizeof(prUInt16) * dwNum;

	/* check supported device properties */
	dwNum = *((prUInt32 *)pDeviceInfoTmp);
	pDeviceInfoTmp +=   sizeof(prUInt32);
	for ( i = 0L; i < dwNum; i++ )
	{
		wDeviceProp = *((prptpDevicePropCode *)pDeviceInfoTmp);
		/* turn on the flags of the supported device properties */
		switch ( wDeviceProp )
		{
			case prPTP_DEV_PROP_EXPOSURE_MODE:
				*pExpoureMode = TRUE;
				break;
			case prPTP_DEV_PROP_EXPOSURE_COMP:
				*pExpoureComp = TRUE;
				break;
			case prPTP_DEV_PROP_STROBE_SETTING:
				*pSTrobeSet = TRUE;
				break;
			case prPTP_DEV_PROP_ISO:
				*pISO = TRUE;
				break;
			case prPTP_DEV_PROP_PHOTO_EFFECT:
				*pPhotoEffect = TRUE;
				break;
			default:
				break;
		}
		pDeviceInfoTmp += sizeof(prptpDevicePropCode);
	}
}

prBoolean	CCaptureCanonS5Dlg::IsSuppotedCapTransMode(
	prUInt8*	pDeviceInfo,
	prUInt16*	pwCurrentValue,
	prUInt16*	pwMode)
{
	prUInt8*	pDeviceInfoTmp;
	prUInt8		bFormFlag,bGetSet;
	prUInt16	wDataType = 0;
	prUInt16	wNum,i;
	prUInt16	wMode = 0;
	prptpDevicePropCode	DevicePropCode;

	pDeviceInfoTmp = pDeviceInfo;

	/* DevicePropCode */
	DevicePropCode = *((prptpDevicePropCode *)pDeviceInfoTmp);
	if ( DevicePropCode != prPTP_DEV_PROP_CAPTURE_TRANSFER_MODE )
	{
		return FALSE;
	}
	pDeviceInfoTmp += sizeof(prptpDevicePropCode);

	/* DataType */
	wDataType = *((prUInt16 *)pDeviceInfoTmp);
	if (wDataType != 0x0004)
	{
		return FALSE;
	}
	pDeviceInfoTmp += sizeof(prUInt16);

	/* GetSet */
	bGetSet = *((prUInt8 *)pDeviceInfoTmp);
	if ( bGetSet != 0x01 )
	{
		return FALSE;
	}
	pDeviceInfoTmp += sizeof(prUInt8);

	/* FactoryDefaultValue */
	pDeviceInfoTmp +=  sizeof(prUInt16);

	/* CurrentValue */
	*pwCurrentValue = *((prUInt16 *)pDeviceInfoTmp);
	pDeviceInfoTmp += sizeof(prUInt16);

	/* FormFlag */
	bFormFlag = *((prUInt8 *)pDeviceInfoTmp);
	if ( bFormFlag != 0x02 )
	{
		return FALSE;
	}
	pDeviceInfoTmp += sizeof(prUInt8);

	/* Enumeration-Form */
	wNum = *((prUInt16 *)pDeviceInfoTmp);
	pDeviceInfoTmp += sizeof(prUInt16);
	for ( i = 0; i < wNum; i++ )
	{
		wMode = *((prUInt16 *)pDeviceInfoTmp);
		/* check the supported transfer modes */
		switch ( wMode )
		{
			case 0x0001:
				*pwMode |= 0x0001;
				break;
			case 0x0002:
				*pwMode |= 0x0002;
				break;
			case 0x0004:
				*pwMode |= 0x0004;
				break;
			case 0x0008:
				*pwMode |= 0x0008;
				break;
			default:
				break;
		}
		pDeviceInfoTmp += sizeof(prUInt16);
	}

	return TRUE;
}


prVoid CCaptureCanonS5Dlg::IsSupportedRCandEVF(
	prUInt8*	pDeviceInfo,	/* (IN) DeviceInfo data set*/
	prBoolean*	pbRC,
	prBoolean*	pbEVF,
	prBoolean*	pbAwb)
{
	prUInt8		*pDeviceInfoTmp;
	prUInt8		bNum;
	prUInt32	dwNum, i;
	prOperationCode	wOperation;
	
	/* Flag if the operation is support or not */
	prBoolean   fInitiate, fTerminate, fCapture;
	prBoolean	fFvInitiate,fFvTerminate,fAeAfAwb;

	fInitiate = FALSE;
	fTerminate = FALSE;
	fCapture = FALSE;
	fFvInitiate = FALSE;
	fFvTerminate = FALSE;
	fAeAfAwb = FALSE;

	/* Move the potiner to support operation */
	pDeviceInfoTmp = pDeviceInfo;

	/* Standard version */
	/* Vendor extendedID */
	/* Vendor extended version */
	pDeviceInfoTmp +=   sizeof(prUInt16)
					  + sizeof(prUInt32)
					  + sizeof(prUInt16);
	/* Vendor extended information */
	bNum = *((prUInt8 *)pDeviceInfoTmp);
	pDeviceInfoTmp +=   sizeof(prUInt8)
					  + sizeof(prWChar) * bNum;
	/* Function moded */
	pDeviceInfoTmp +=   sizeof(prUInt16);

	/* Suppored operations */
	dwNum = *((prUInt32 *)pDeviceInfoTmp);	/* number of elements */
	pDeviceInfoTmp +=   sizeof(prUInt32);

	/* Loop for the number of elements */
	for ( i = 0L; i < dwNum; i++ )
	{
		wOperation = *((prOperationCode *)pDeviceInfoTmp);
		switch ( wOperation )
		{
			case prPTP_INITIATE_RELEASE_CONTROL:
				fInitiate = TRUE;
				break;
			case prPTP_TERMINATE_RELEASE_CONTROL:
				fTerminate = TRUE;
				break;
			case prPTP_RC_CAPTURE:
				fCapture = TRUE;
				break;
			case prPTP_RC_INITIATE_VIEW_FINDER:
				fFvInitiate = TRUE;
				break;
			case prPTP_RC_TERMINATE_VIEW_FINDER:
				fFvTerminate = TRUE;
				break;
			case prPTP_RC_RELEASE_DO_AE_AF_AWB:
				fAeAfAwb = TRUE;
				break;
			default:
				break;
		}
		pDeviceInfoTmp += sizeof(prOperationCode);
	}
/* The following information is not checked */
	/* Supported events */
	/* Supported device properties */
	/* Supported captured image types */
	/* Supported image types */
	/* Company information */ 
	/* Model name */
	/* Device version */
	/* Serial number */

	/* Is Remote Capture supported? */
	if ( (fInitiate == TRUE) && (fTerminate == TRUE) && (fCapture == TRUE) )
	{
		*pbRC = TRUE;
	}
	/* Is Viewfinder supported? */
	if ( (fFvInitiate == TRUE) && (fFvTerminate == TRUE) )
	{
		*pbEVF = TRUE;
	}
	/* Is AeAfAwb supported? */
	if ( fAeAfAwb == TRUE )
	{
		*pbAwb = TRUE;
	}
}



prBoolean	CCaptureCanonS5Dlg::IsReleasePictureType(prUInt8*	pDeviceInfo)
{
	prUInt8*			pDeviceInfoTmp;
	prUInt16			wDataType;
	prptpDevicePropCode	DevicePropCode;

	if ( !pDeviceInfo )
	{
		return FALSE;
	}

	pDeviceInfoTmp = pDeviceInfo;

	DevicePropCode = *((prptpDevicePropCode *)pDeviceInfoTmp);
	if (DevicePropCode != prPTP_DEV_PROP_FULLVIEW_FILE_FORMAT)
	{
		return FALSE;
	}
	pDeviceInfoTmp += sizeof(prptpDevicePropCode);

	/* Data type */
	wDataType = *((prUInt16 *)pDeviceInfoTmp);
	if ( wDataType != 0x0002 )
	{
		return FALSE;
	}
	pDeviceInfoTmp += sizeof(prUInt16);

	/* Get/Set */
	pDeviceInfoTmp += sizeof(prUInt8);

	/* FactoryDefaultValue */
	pDeviceInfoTmp +=  sizeof(prUInt8);

	/* check the image type for current shooting */
	if ( *((prUInt8 *)pDeviceInfoTmp) == 0x01 )
	{
		m_FileType = prPTP_EXIF_JPEG;
	}
	else if ( *((prUInt8 *)pDeviceInfoTmp) == 0x02 )
	{
		m_FileType = prPTP_CRW;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}


void CCaptureCanonS5Dlg::OnClose() 
{
	m_fVFEnd = TRUE;
 
	CloseViewFinder();

	DisconnectCamera();

	ShowWindow(SW_HIDE);

	CDialog::OnClose();
}



LRESULT CCaptureCanonS5Dlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	prUInt8*	pBuffer = NULL;

	if ( message == m_ReleaseOnMessage )
	{
		/* if it is not shooting, */
		if (m_fGetRelease == FALSE )
		{
			OnRelease();
		}

		return TRUE;
	}
	else if ( message == m_CameraShutDown )
	{
		DisconnectCamera();
		//OnDisconnect();

		return TRUE;
	}
	else if ( message == m_AbortPCEVF )
	{
		/* Is ViewFinder runnning? */
		if ( m_fVFEnd == TRUE )
		{
			/* Terminate ViewFinder */
			CloseViewFinder();
		}

		return TRUE;
	}
	else if ( message == m_ChangeByUI )
	{
		if ( m_fInReleaseControl == TRUE )
		{
			UpdateSetting();
		}
		return TRUE;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}


void CCaptureCanonS5Dlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CRect   rectWindow, rectClient;  
	int nWidthOffset, nHeightOffset;

	/*
	GetWindowRect(&rectWindow);  
    GetClientRect(&rectClient);  
              
	//   get   offset   of   toolbars,   scrollbars,   etc.  
    nWidthOffset   =   rectWindow.Width()   -   rectClient.Width();  
    nHeightOffset   =   rectWindow.Height()   -   rectClient.Height();  

    lpMMI->ptMinTrackSize.x   =  m_nMinWidth    +   nWidthOffset;  
    lpMMI->ptMinTrackSize.y   =  m_nMinHeight   +   nHeightOffset;  
    lpMMI->ptMaxTrackSize.x   =  m_nMaxWidth    +   nWidthOffset;  
    lpMMI->ptMaxTrackSize.y   =  m_nMaxHeight   +   nHeightOffset;   	
	*/

	CDialog::OnGetMinMaxInfo(lpMMI);
}
