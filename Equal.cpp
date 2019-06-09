// Equal.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "ImageView.h"
#include "Equal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEqual dialog
int equalmax = 400;

void WINAPI  AdjustColor(HDIB m_hDIB,int channel, int curvalue,int orivalue)
{
	if (m_hDIB==NULL)	return;

	LPSTR    lpDIBHdr;            // Pointer to BITMAPINFOHEADER
	BYTE *    lpDIBBits;           // Pointer to DIB bits
	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
	DWORD width  = ::DIBWidth(lpDIBHdr);
	DWORD height = ::DIBHeight(lpDIBHdr);
	
	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB
	
	/* point to the header (whether Win 3.0 and old) */
	
	lpbmi = (LPBITMAPINFOHEADER)lpDIBHdr;
	lpbmc = (LPBITMAPCOREHEADER)lpDIBHdr;
	
	WORD m_BitCount;
	/* return the DIB width if it is a Win 3.0 DIB */
	if (IS_WIN30_DIB(lpDIBHdr))
	{
		m_BitCount = lpbmi->biBitCount;
	}
	else  /* it is an other-style DIB, so return its width */
	{
		m_BitCount = lpbmc->bcBitCount;
	}
	if(m_BitCount != 24)
	{
		AfxMessageBox("本功能只对24位真彩色图象进行处理 ！");
		::GlobalUnlock((HGLOBAL) m_hDIB);
		return;
	}
	
	DWORD  ByteCount = width*3; // the same as "width*24/8"
	if(((width*24)%32) != 0)
	{
		ByteCount = ByteCount+4-((width*3)%4);
	}

	int i,j,k;
	BYTE * ptr;
	for(i=0;i<height;i++)
	{
		ptr = lpDIBBits+(height-i-1)*ByteCount;
		for(j=0;j<width;j++)
		{
			k = (unsigned char)*(ptr + j*3 + channel);
			k += (curvalue - orivalue);
			if(k < 0)   k = 0;
			if(k > 255) k = 255;
			*(ptr + j*3 + channel) = k;
		}
	}
	
	GlobalUnlock((HGLOBAL) m_hDIB);
}

BOOL WINAPI  AdjustBrightness(HDIB m_hDIB,int Percentage,int equalmax)
{
	if (m_hDIB==NULL || Percentage<1 || Percentage>equalmax)
		return FALSE;
	
	LPSTR    lpDIBHdr;            // Pointer to BITMAPINFOHEADER
	BYTE *    lpDIBBits;           // Pointer to DIB bits
	
	/* Check for valid DIB handle */
	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
	DWORD width  = ::DIBWidth(lpDIBHdr);
	DWORD height = ::DIBHeight(lpDIBHdr);
	
	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB
	
	/* point to the header (whether Win 3.0 and old) */
	
	lpbmi = (LPBITMAPINFOHEADER)lpDIBHdr;
	lpbmc = (LPBITMAPCOREHEADER)lpDIBHdr;
	
	WORD m_BitCount;
	/* return the DIB width if it is a Win 3.0 DIB */
	if (IS_WIN30_DIB(lpDIBHdr))
	{
		m_BitCount = lpbmi->biBitCount;
	}
	else  /* it is an other-style DIB, so return its width */
	{
		m_BitCount = lpbmc->bcBitCount;
	}
	if(m_BitCount != 8 && m_BitCount != 24)
	{
		AfxMessageBox("本系统只对8位Gray图象和24位真彩色图象进行处理 ！");
		::GlobalUnlock((HGLOBAL) m_hDIB);
		return false;
	}
	
	DWORD  ByteCount;
	if(m_BitCount == 8)
	{
		ByteCount = width; 
		if(((width*8)%32) != 0)
		{
			ByteCount = ByteCount+4-(width%4);
		}
	}
	else
	{
		ByteCount = width*3; // the same as "width*24/8"
		if(((width*24)%32) != 0)
		{
			ByteCount = ByteCount+4-((width*3)%4);
		}
	}
	int i,j,k;
	BYTE buf[256];
	for(i=0;i<256;i++)
	{
		k = min(i*Percentage/100, 255);
		buf[i] = (BYTE)k;
	}
	BYTE * ptr;
	if(m_BitCount == 8)
	{
		for(i=0;i<height;i++)
		{
			ptr = lpDIBBits+(height-i-1)*ByteCount;
			for(j=0;j<width;j++)
			{
				k = (unsigned char)*(ptr+j);
				*(ptr+j) = buf[k];
			}
		}
	}
	else
	{
		for(i=0;i<height;i++)
		{
			ptr = lpDIBBits+(height-i-1)*ByteCount;
			for(j=0;j<width;j++)
			{
				k = (unsigned char)*(ptr+j*3);
				*(ptr+j*3) = buf[k];
				k = (unsigned char)*(ptr+j*3+1);
				*(ptr+j*3+1) = buf[k];
				k = (unsigned char)*(ptr+j*3+2);
				*(ptr+j*3+2) = buf[k];
			}
		}
	}
	GlobalUnlock((HGLOBAL) m_hDIB);
	
	return true;
}

BOOL  WINAPI AdjustContrast(HDIB m_hDIB,int Percentage,int equalmax)
{
	if (m_hDIB==NULL || Percentage<1 || Percentage>equalmax)
		return FALSE;
	
	LPSTR    lpDIBHdr;            // Pointer to BITMAPINFOHEADER
	BYTE *    lpDIBBits;           // Pointer to DIB bits
	
	/* Check for valid DIB handle */
	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
	DWORD width  = ::DIBWidth(lpDIBHdr);
	DWORD height = ::DIBHeight(lpDIBHdr);
	
	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB
	
	/* point to the header (whether Win 3.0 and old) */
	
	lpbmi = (LPBITMAPINFOHEADER)lpDIBHdr;
	lpbmc = (LPBITMAPCOREHEADER)lpDIBHdr;
	
	WORD m_BitCount;
	/* return the DIB width if it is a Win 3.0 DIB */
	if (IS_WIN30_DIB(lpDIBHdr))
	{
		m_BitCount = lpbmi->biBitCount;
	}
	else  /* it is an other-style DIB, so return its width */
	{
		m_BitCount = lpbmc->bcBitCount;
	}
	if(m_BitCount != 8 && m_BitCount != 24)
	{
		AfxMessageBox("本系统只对8位Gray图象和24位真彩色图象进行处理 ！");
		::GlobalUnlock((HGLOBAL) m_hDIB);
		return false;
	}
	
	DWORD  ByteCount;
	if(m_BitCount == 8)
	{
		ByteCount = width; 
		if(((width*8)%32) != 0)
		{
			ByteCount = ByteCount+4-(width%4);
		}
	}
	else
	{
		ByteCount = width*3; // the same as "width*24/8"
		if(((width*24)%32) != 0)
		{
			ByteCount = ByteCount+4-((width*3)%4);
		}
	}
	int i,j,k;
	BYTE buf[256];
	for(i=0;i<256;i++)
	{
		k = 128 + ((i - 128)*Percentage/100);
		if(k < 0) k = 0;
		k = min(k, 255);
		buf[i] = (BYTE)k;
	}
	BYTE * ptr;
	if(m_BitCount == 8)
	{
		for(i=0;i<height;i++)
		{
			ptr = lpDIBBits+(height-i-1)*ByteCount;
			for(j=0;j<width;j++)
			{
				k = (unsigned char)*(ptr+j);
				*(ptr+j) = buf[k];
			}
		}
	}
	else
	{
		for(i=0;i<height;i++)
		{
			ptr = lpDIBBits+(height-i-1)*ByteCount;
			for(j=0;j<width;j++)
			{
				k = (unsigned char)*(ptr+j*3);
				*(ptr+j*3) = buf[k];
				k = (unsigned char)*(ptr+j*3+1);
				*(ptr+j*3+1) = buf[k];
				k = (unsigned char)*(ptr+j*3+2);
				*(ptr+j*3+2) = buf[k];
			}
		}
	}
	GlobalUnlock((HGLOBAL) m_hDIB);
	return true;
}

BOOL  WINAPI AdjustGammaCorrection(HDIB m_hDIB,float Value)
{
	if (m_hDIB==NULL || Value<0 || Value>10)
		return FALSE;
	
	LPSTR    lpDIBHdr;            // Pointer to BITMAPINFOHEADER
	BYTE *    lpDIBBits;           // Pointer to DIB bits
	
	/* Check for valid DIB handle */
	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	lpDIBBits = (BYTE *)::FindDIBBits(lpDIBHdr);
	DWORD width  = ::DIBWidth(lpDIBHdr);
	DWORD height = ::DIBHeight(lpDIBHdr);
	
	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB
	
	/* point to the header (whether Win 3.0 and old) */
	
	lpbmi = (LPBITMAPINFOHEADER)lpDIBHdr;
	lpbmc = (LPBITMAPCOREHEADER)lpDIBHdr;
	
	WORD m_BitCount;
	/* return the DIB width if it is a Win 3.0 DIB */
	if (IS_WIN30_DIB(lpDIBHdr))
	{
		m_BitCount = lpbmi->biBitCount;
	}
	else  /* it is an other-style DIB, so return its width */
	{
		m_BitCount = lpbmc->bcBitCount;
	}
	if(m_BitCount != 8 && m_BitCount != 24)
	{
		AfxMessageBox("本系统只对8位Gray图象和24位真彩色图象进行处理 ！");
		::GlobalUnlock((HGLOBAL) m_hDIB);
		return false;
	}
	
	DWORD  ByteCount;
	if(m_BitCount == 8)
	{
		ByteCount = width; 
		if(((width*8)%32) != 0)
		{
			ByteCount = ByteCount+4-(width%4);
		}
	}
	else
	{
		ByteCount = width*3; // the same as "width*24/8"
		if(((width*24)%32) != 0)
		{
			ByteCount = ByteCount+4-((width*3)%4);
		}
	}
	double MaxRange = pow((double) 255, (double) Value) / 255;
	double tmp;
	int i,j,k;
	BYTE buf[256];
	for(i=0;i<256;i++)
	{
		tmp = pow((double) i, (double) Value) / MaxRange;
		k = min((int) (tmp+0.5), 255);
		if(k < 0) k = 0;
		buf[i] = (BYTE)k;
	}
	BYTE * ptr;
	if(m_BitCount == 8)
	{
		for(i=0;i<height;i++)
		{
			ptr = lpDIBBits+(height-i-1)*ByteCount;
			for(j=0;j<width;j++)
			{
				k = (unsigned char)*(ptr+j);
				*(ptr+j) = buf[k];
			}
		}
	}
	else
	{
		for(i=0;i<height;i++)
		{
			ptr = lpDIBBits+(height-i-1)*ByteCount;
			for(j=0;j<width;j++)
			{
				k = (unsigned char)*(ptr+j*3);
				*(ptr+j*3) = buf[k];
				k = (unsigned char)*(ptr+j*3+1);
				*(ptr+j*3+1) = buf[k];
				k = (unsigned char)*(ptr+j*3+2);
				*(ptr+j*3+2) = buf[k];
			}
		}
	}
	GlobalUnlock((HGLOBAL) m_hDIB);
	return true;
}

CEqual::CEqual(CWnd* pParent /*=NULL*/)
	: CDialog(CEqual::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEqual)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_TimerHandle = 0;
	m_pParent = pParent;
}


void CEqual::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEqual)
	DDX_Control(pDX, IDC_SLIDER_GREEN, m_slider_green);
	DDX_Control(pDX, IDC_SLIDER_BLUE, m_slider_blue);
	DDX_Control(pDX, IDC_SLIDER_RED, m_slider_red);
	DDX_Control(pDX, IDC_SLIDER_BRIGHTNESS, m_slider_brightness);
	DDX_Control(pDX, IDC_SLIDER_CONTRAST, m_slider_contrast);
	DDX_Control(pDX, IDC_SLIDER_GAMMA, m_slider_gamma);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEqual, CDialog)
	//{{AFX_MSG_MAP(CEqual)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEqual message handlers

BOOL CEqual::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    hBakDIB = (HDIB) CopyHandle(((CImageView *)m_pParent)->m_hDIB);

    m_slider_brightness.SetRange(1,equalmax);
	m_slider_brightness.SetPageSize(10);
	m_slider_brightness.SetTicFreq(10);
	m_slider_brightness.SetPos(100);

    m_slider_contrast.SetRange(1,equalmax);
	m_slider_contrast.SetPageSize(10);
	m_slider_contrast.SetTicFreq(10);
	m_slider_contrast.SetPos(100);

    m_slider_gamma.SetRange(0,1000);
	m_slider_gamma.SetPageSize(10);
	m_slider_gamma.SetTicFreq(10);
	m_slider_gamma.SetPos(100);

    m_slider_red.SetRange(0,255);
	m_slider_red.SetPageSize(10);
	m_slider_red.SetTicFreq(10);
	m_slider_red.SetPos(100);

    m_slider_green.SetRange(0,255);
	m_slider_green.SetPageSize(10);
	m_slider_green.SetTicFreq(10);
	m_slider_green.SetPos(100);

    m_slider_blue.SetRange(0,255);
	m_slider_blue.SetPageSize(10);
	m_slider_blue.SetTicFreq(10);
	m_slider_blue.SetPos(100);
	
    prethreshold_brightness = 100;
    prethreshold_contrast = 100;
    prethreshold_gamma = 100;

    prethreshold_red = 100;
    prethreshold_green = 100;
    prethreshold_blue = 100;

	m_TimerHandle = SetTimer(1, 10, NULL);
	if(m_TimerHandle == 0)
	{
		AfxMessageBox("定时器设置错误 !");
		return FALSE;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEqual::OnOK() 
{
	// TODO: Add extra validation here
	if(hBakDIB != NULL)
	{
		::GlobalFree(hBakDIB);
		hBakDIB = NULL;
	}
	
	CDialog::OnOK();
}

void CEqual::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(hBakDIB != NULL)
	{
		::GlobalFree(((CImageView *)m_pParent)->m_hDIB);
		((CImageView *)m_pParent)->m_hDIB = (HDIB) CopyHandle(hBakDIB);

		::GlobalFree(hBakDIB);
		hBakDIB = NULL;
	}
	
	CDialog::OnCancel();
}

void CEqual::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(hBakDIB != NULL)
	{
		::GlobalFree(hBakDIB);
		hBakDIB = NULL;
	}
	
	if(m_TimerHandle)
	{
		if(!KillTimer(m_TimerHandle))
		{
			AfxMessageBox("destroy timer error in CEqual!");
		}
		m_TimerHandle = 0;
	}
}

void CEqual::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	static int   m_TimerMuteFlag=1;
    if(nIDEvent == m_TimerHandle && m_TimerMuteFlag)
	{
		m_TimerMuteFlag = 0;
    
		int BrightnessThreshold = m_slider_brightness.GetPos();
		int ContrastThreshold = m_slider_contrast.GetPos();
		int GammaThreshold = m_slider_gamma.GetPos();

		int RedThreshold = m_slider_red.GetPos();
		int GreenThreshold = m_slider_green.GetPos();
		int BlueThreshold = m_slider_blue.GetPos();

		if(BrightnessThreshold!=prethreshold_brightness || ContrastThreshold!=prethreshold_contrast || GammaThreshold!=prethreshold_gamma || RedThreshold!=prethreshold_red || GreenThreshold!=prethreshold_green || BlueThreshold!=prethreshold_blue)
		{
			CString msg;
			float tmp;
			tmp = (float)(GammaThreshold*1.0/100);
			msg.Format("%.2f",tmp);

			if(BrightnessThreshold != prethreshold_brightness)
			{
				SetDlgItemInt(IDC_STATIC_BRIGHTNESS,BrightnessThreshold);
				prethreshold_brightness = BrightnessThreshold;
			}
			if(ContrastThreshold != prethreshold_contrast)
			{
				SetDlgItemInt(IDC_STATIC_CONTRAST,ContrastThreshold);
				prethreshold_contrast = ContrastThreshold;
			}
			if(GammaThreshold != prethreshold_gamma)
			{
				SetDlgItemText(IDC_STATIC_GAMMA,msg);
				prethreshold_gamma = GammaThreshold;
			}

			int rr = prethreshold_red;
			int gg = prethreshold_green;
			int bb = prethreshold_blue;

			if(RedThreshold != prethreshold_red)
			{
				SetDlgItemInt(IDC_STATIC_RED,RedThreshold);
				prethreshold_red = RedThreshold;
			}
			if(GreenThreshold != prethreshold_green)
			{
				SetDlgItemInt(IDC_STATIC_GREEN,GreenThreshold);
				prethreshold_green = GreenThreshold;
			}
			if(BlueThreshold != prethreshold_blue)
			{
				SetDlgItemInt(IDC_STATIC_BLUE,BlueThreshold);
				prethreshold_blue = BlueThreshold;
			}
			
			GlobalFree((HGLOBAL) ((CImageView *)m_pParent)->m_hDIB);
			((CImageView *)m_pParent)->m_hDIB = (HDIB) CopyHandle(hBakDIB);
            
			if(rr != prethreshold_red || gg != prethreshold_green || bb != prethreshold_blue)
			{
				if(rr != prethreshold_red)
					AdjustColor(((CImageView *)m_pParent)->m_hDIB, 2, prethreshold_red, 100);
				if(gg != prethreshold_green)
					AdjustColor(((CImageView *)m_pParent)->m_hDIB, 1, prethreshold_green, 100);
				if(bb != prethreshold_blue)
					AdjustColor(((CImageView *)m_pParent)->m_hDIB, 0, prethreshold_blue, 100);
			}
			else
			{
				AdjustBrightness(((CImageView *)m_pParent)->m_hDIB,BrightnessThreshold,equalmax);
				AdjustContrast(((CImageView *)m_pParent)->m_hDIB,ContrastThreshold,equalmax);
				AdjustGammaCorrection(((CImageView *)m_pParent)->m_hDIB,tmp);
			}

			((CImageView *)m_pParent)->RedrawWindow();
		}

		m_TimerMuteFlag = 1;
	}
	
	CDialog::OnTimer(nIDEvent);
}
