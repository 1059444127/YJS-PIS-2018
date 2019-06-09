// CreateReport.cpp: implementation of the CCreateReport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pathology.h"
#include "CreateReport.h"
#include "fstream.h"

#include "NumInput.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPathologyApp theApp;

CCreateReport::CCreateReport()
{
	start_line=0;located=FALSE;SelectMode='c';
	segment=0;choice=0;item=0;segmento=-1;choiceo=-1;itemo=-1;
	cWidth=10;
	m_See=m_Result=NULL;
	m_pWnd=NULL;m_pEdit=NULL;
}

CCreateReport::CCreateReport(CWnd* pParent,CWnd* pEdit ,CString *See,CString *Result)
{
	start_line=0;located=FALSE;SelectMode='c';
	segment=0;choice=0;item=0;segmento=-1;choiceo=-1;itemo=-1;
	cWidth=10;
	m_See=See,m_Result=Result;
	m_pWnd=pParent;
	m_pEdit=pEdit;
}

CCreateReport::~CCreateReport()
{
}

BEGIN_MESSAGE_MAP(CCreateReport, CWnd)
	//{{AFX_MSG_MAP(CCreateReport)
	ON_WM_PAINT()
	ON_COMMAND(ID_SEG_SELECT, OnSegSelect)
	ON_COMMAND(ID_CHOICE_SELECT, OnChoiceSelect)
	ON_COMMAND(ID_SHOW, OnShow)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_BACKCOLOR, OnBackcolor)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_SELECTCHOICE, OnSelectchoice)
	ON_COMMAND(ID_SELECTTEXT, OnSelecttext)
	ON_COMMAND(ID_UNSELECTCHOICE, OnUnselectchoice)
	ON_COMMAND(ID_UNSELECTTEXT, OnUnselecttext)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateReport message handlers
BOOL CCreateReport::Datain_sub2(char *Strbuf,CPtrArray &paData)
{       
	CString LeftStr,ChoiceStr;
	BOOL end;int site;char ch;
	linedata *lineptr;
	CStringArray *ChoiceArray;
	CByteArray *pAChoiceStatus;
	site=strlen(Strbuf)-1;if (Strbuf[site]=='\r') Strbuf[site]=0;
	end=FALSE;LeftStr=Strbuf;
	lineptr=(linedata*)new linedata;
	lineptr->status=FALSE;
	while (!end) {
		site=LeftStr.FindOneOf("([{");
		if (site!=-1) {
			lineptr->Text.Add(LeftStr.Left(site));
			ch=LeftStr[site];
			LeftStr=LeftStr.Right(LeftStr.GetLength()-site-1);
			switch (ch) {
				case '(':			//单选
					site=LeftStr.FindOneOf(")([{");	
					if ((site==-1) || (LeftStr[site]!=')')) {
						LeftStr=Strbuf;LeftStr+="  格式错!   ( )不配对。";
						AfxMessageBox(LeftStr); return FALSE;
					}
					ChoiceStr=LeftStr.Left(site);
					LeftStr=LeftStr.Right(LeftStr.GetLength()-site-1);
					site=ChoiceStr.FindOneOf(")]}([{");	
					if (site!=-1) {
						LeftStr=Strbuf;LeftStr+="  格式错!  ( )不配对。";
						AfxMessageBox(LeftStr); return FALSE;
					}
					ChoiceArray=new CStringArray;
					pAChoiceStatus=new CByteArray;
					do {
						site=ChoiceStr.Find(";");	
						if (site!=-1) {
							ChoiceArray->Add(ChoiceStr.Left(site));
							ChoiceStr=ChoiceStr.Right(ChoiceStr.GetLength()-site-1);
						} else ChoiceArray->Add(ChoiceStr);
						pAChoiceStatus->Add(0);
					} while (site!=-1);
					lineptr->Choice.Add(ChoiceArray);
					lineptr->ChoiceStatus.Add(pAChoiceStatus);
					lineptr->type.Add(0);
					ChoiceArray->FreeExtra();
					pAChoiceStatus->FreeExtra();
					break;
				case '[':			//多选
					site=LeftStr.FindOneOf("]([{");	
					if ((site==-1) || (LeftStr[site]!=']')) {
						LeftStr=Strbuf;LeftStr+="  格式错!  [ ]不配对。";
						AfxMessageBox(LeftStr); return FALSE;
					}
					ChoiceStr=LeftStr.Left(site);
					LeftStr=LeftStr.Right(LeftStr.GetLength()-site-1);
					site=ChoiceStr.FindOneOf(")]}([{");	
					if (site!=-1) {
						LeftStr=Strbuf;LeftStr+="  格式错!  [ ]不配对。";
						AfxMessageBox(LeftStr); return FALSE;
					}
					ChoiceArray=new CStringArray;
					pAChoiceStatus=new CByteArray;
					do {
						site=ChoiceStr.Find(";");	
						if (site!=-1) {
							ChoiceArray->Add(ChoiceStr.Left(site));
							ChoiceStr=ChoiceStr.Right(ChoiceStr.GetLength()-site-1);
						} else ChoiceArray->Add(ChoiceStr);
						pAChoiceStatus->Add(0);
					} while (site!=-1);
					lineptr->Choice.Add(ChoiceArray);
					lineptr->ChoiceStatus.Add(pAChoiceStatus);
					lineptr->type.Add(1);
					ChoiceArray->FreeExtra();
					pAChoiceStatus->FreeExtra();
					break;
				case '{':			//数字
					site=LeftStr.FindOneOf("}");	
					if (site==-1) {
						LeftStr=Strbuf;LeftStr+="  格式错!  { }不配对。";
						AfxMessageBox(LeftStr); return FALSE;
					}
					ChoiceStr=LeftStr.Left(site);
					LeftStr=LeftStr.Right(LeftStr.GetLength()-site-1);
					ChoiceArray=new CStringArray;
					pAChoiceStatus=new CByteArray;
					//find left text
					site=ChoiceStr.FindOneOf("(");	
					if (site==-1) {
						LeftStr=Strbuf;LeftStr+="  格式错!  { }内无(";
						AfxMessageBox(LeftStr); return FALSE;
					}
					ChoiceArray->Add(ChoiceStr.Left(site));
					ChoiceStr=ChoiceStr.Right(ChoiceStr.GetLength()-site-1);
					//find Original Number
					site=ChoiceStr.FindOneOf(")");	
					if (site==-1) {
						LeftStr=Strbuf;LeftStr+="  格式错!  { }内( )不配对。";
						AfxMessageBox(LeftStr); return FALSE;
					}
					ChoiceArray->Add(ChoiceStr.Left(site));
					if (ChoiceStr.Left(site).GetLength()>0) {
						pAChoiceStatus->Add(1);pAChoiceStatus->Add(1);pAChoiceStatus->Add(1);
					} else {
						pAChoiceStatus->Add(2);pAChoiceStatus->Add(2);pAChoiceStatus->Add(2);
					}
					ChoiceStr=ChoiceStr.Right(ChoiceStr.GetLength()-site-1);
					//find left text
					ChoiceArray->Add(ChoiceStr);
					lineptr->Choice.Add(ChoiceArray);
					lineptr->ChoiceStatus.Add(pAChoiceStatus);
					lineptr->type.Add(2);   
					ChoiceArray->FreeExtra();
					pAChoiceStatus->FreeExtra();
					break;  
				default:end=TRUE;break;
			}
		} else {lineptr->Text.Add(LeftStr);end=TRUE;}
	}
	paData.Add(lineptr); 
	lineptr->Text.FreeExtra();
	return TRUE;
}
    
BOOL CCreateReport::Datain_sub(CString Buwei,CString Bingzhong)
{
	char Strbuf[2048];
	CString filenm;
	fstream f;
	CFileStatus status; 
	char Bingzhong0[40],Bingzhong1[40],Bingzhong2[40],Bingzhong3[40];
	filenm=theApp.m_Exe_Working_Dir+"system\\";filenm+=Buwei;filenm+=".dat";
	if (!CFile::GetStatus(filenm,status))
	{
		filenm+="不存在！";
		AfxMessageBox(filenm);return FALSE;
	}
	strcpy(Bingzhong2,Bingzhong);strcat(Bingzhong2,"、");
	strcpy(Bingzhong3,"、");strcat(Bingzhong3,Bingzhong);
	strcpy(Bingzhong0,Bingzhong);strcat(Bingzhong0,"\r");
	strcpy(Bingzhong1,Bingzhong);strcat(Bingzhong1,"\r\n");
	f.open(filenm,ios::in|ios::nocreate);
	strcpy(Strbuf,"");
	if (f.good()) {               
		while(!f.eof() && (strcmp(Strbuf,(const char *)Bingzhong)!=0) && (strstr(Strbuf,Bingzhong0)==NULL) && (strstr(Strbuf,Bingzhong1)==NULL) && (strstr(Strbuf,Bingzhong2)==NULL) && (strstr(Strbuf,Bingzhong3)==NULL)) {		
			while(!f.eof() && (strstr(strlwr(Strbuf),"end")==NULL)) {
				f.getline(Strbuf,2040);
			}
			if (!f.eof()) f.getline(Strbuf,2040);
		}
		if (!f.eof()) f.getline(Strbuf,2040);
		else {
			filenm="无"+Bingzhong+"报告书！";
			AfxMessageBox(filenm);
		}
		while(!f.eof() && (strstr(Strbuf,"诊断意见：")==NULL)) {		
			Datain_sub2(Strbuf,paSee);
			if (!f.eof()) f.getline(Strbuf,2040);
		}
		if (!f.eof()) f.getline(Strbuf,2040);
		while(!f.eof() && (strstr(Strbuf,"end")==NULL) && (strstr(Strbuf,"END")==NULL)) {		
			Datain_sub2(Strbuf,paResult);
			if (!f.eof()) f.getline(Strbuf,2040);
		}
	}
	f.close();
	return TRUE;
}

int CCreateReport::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	int width,height;
	width=GetSystemMetrics(SM_CXSCREEN);height=GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(NULL,0,0 ,width,height-24,SWP_NOZORDER);	
	SetScrollRange(SB_VERT,0,60);
	CString filenm,bw,bz,strbuf;
	CFile DisFile;
	CFileStatus status;
	WORD len,i,site;
	CFile SysParaFile;

//Load Color Font	
	filenm=theApp.m_Exe_Working_Dir+"system/repcolor.dat";
	if (CFile::GetStatus(filenm,status)) {
		BYTE *temptr,Tem;
		if (SysParaFile.Open(filenm,CFile::modeRead))
		{
			TRY
			{
				CArchive archive(&SysParaFile,CArchive::load);
				archive>>m_BackColor;          
				
				archive>>m_SelecttextColor;
				temptr=(BYTE *)&m_Selecttextlogfont;
				for (i=0;i<sizeof(LOGFONT);i++) {archive>>Tem;*temptr++=Tem;}
				
				archive>>m_UnSelecttextColor;       
				temptr=(BYTE *)&m_UnSelecttextlogfont;
				for (i=0;i<sizeof(LOGFONT);i++) {archive>>Tem;*temptr++=Tem;}
				
				archive>>m_SelectchoiceColor;
				temptr=(BYTE *)&m_Selectchoicelogfont;
				for (i=0;i<sizeof(LOGFONT);i++) {archive>>Tem;*temptr++=Tem;}
				
				archive>>m_UnSelectchoiceColor;
				temptr=(BYTE *)&m_UnSelectchoicelogfont;
				for (i=0;i<sizeof(LOGFONT);i++) {archive>>Tem;*temptr++=Tem;}
				archive.Close();SysParaFile.Close();
			}
			CATCH (CArchiveException,e)
			{
				m_BackColor=12632256l;
				m_SelecttextColor=0l;
				m_UnSelecttextColor=16711680l;
				m_SelectchoiceColor=255l;
				m_UnSelectchoiceColor=16744448l; 
				HFONT myFont=CreateFont(-16,0,0,0,400,FALSE,FALSE,FALSE,ANSI_CHARSET,OUT_TT_PRECIS
					,CLIP_TT_ALWAYS,DEFAULT_QUALITY,DEFAULT_PITCH,"仿宋_GB2312");
				GetObject(myFont,sizeof(LOGFONT),(LPSTR)&m_Selecttextlogfont);
				GetObject(myFont,sizeof(LOGFONT),(LPSTR)&m_UnSelecttextlogfont);
				GetObject(myFont,sizeof(LOGFONT),(LPSTR)&m_Selectchoicelogfont);
				GetObject(myFont,sizeof(LOGFONT),(LPSTR)&m_UnSelectchoicelogfont);
			}
			END_CATCH
		}
	} else {    
		m_BackColor=12632256l;
		m_SelecttextColor=0l;
		m_UnSelecttextColor=16711680l;
		m_SelectchoiceColor=255l;
		m_UnSelectchoiceColor=16744448l;
		HFONT myFont=CreateFont(-16,0,0,0,400,FALSE,FALSE,FALSE,ANSI_CHARSET,OUT_TT_PRECIS
			,CLIP_TT_ALWAYS,DEFAULT_QUALITY,DEFAULT_PITCH,"仿宋_GB2312");
		GetObject(myFont,sizeof(LOGFONT),(LPSTR)&m_Selecttextlogfont);
		GetObject(myFont,sizeof(LOGFONT),(LPSTR)&m_UnSelecttextlogfont);
		GetObject(myFont,sizeof(LOGFONT),(LPSTR)&m_Selectchoicelogfont);
		GetObject(myFont,sizeof(LOGFONT),(LPSTR)&m_UnSelectchoicelogfont);
	}

	CRect clientRect;
	GetClientRect(&clientRect);
	sHeight=clientRect.Height();sWidth=clientRect.Width()-cWidth*2;
	//Cacu line Height
	lHeight=abs(m_Selecttextlogfont.lfHeight)+2;
	Pagesize=sHeight/max(1,lHeight);
	located=FALSE;
//Load Report
	filenm=theApp.m_Exe_Working_Dir+"system/bwbz.dat";len=0;
	if (CFile::GetStatus(filenm,status))
	{   
		DisFile.Open(filenm,CFile::modeRead);
		CArchive archive(&DisFile,CArchive::load);
		TRY {
			archive>>len;
			for (i=0;i<len;i++) {
				archive>>strbuf;     
				site=strbuf.Find(' ');
				if (site>0) {
					bw=strbuf.Left(site);
					bz=strbuf.Right(strbuf.GetLength()-site-1);
					Datain_sub(bw,bz);
				}
			}                       
		} 
		CATCH (CArchiveException,e)
		{
			archive.Close();DisFile.Close();
			return FALSE;
		}
		END_CATCH		
		archive.Close();DisFile.Close();
	}           
	if (paSee.GetSize()+paResult.GetSize()<1) return FALSE;else return TRUE;
}

void CCreateReport::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect clientRect;
	GetClientRect(&clientRect);
	sHeight=clientRect.Height();sWidth=clientRect.Width()-cWidth*2;
	//Cacu line Height
	lHeight=abs(m_Selecttextlogfont.lfHeight)+2;
	Pagesize=sHeight/max(1,lHeight);
	located=FALSE;
}

void CCreateReport::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	BOOL change=FALSE;
	switch (nSBCode)
	{
		case SB_LINEUP:if (start_line>0) {start_line--;change=TRUE;}break;
		case SB_LINEDOWN:if (start_line<Maxline-Pagesize) {start_line++;change=TRUE;}break;	
		case SB_BOTTOM:if (start_line<Maxline-Pagesize) {start_line=Maxline-Pagesize;change=TRUE;}break;
		case SB_TOP:if (start_line>0) {start_line=0;change=TRUE;}break;
		case SB_PAGEUP:if (start_line>0) {start_line-=Pagesize;change=TRUE;}if (start_line<0) start_line=0;break;
		case SB_PAGEDOWN:if (start_line<Maxline) {start_line+=Pagesize;change=TRUE;}if (start_line>Maxline) start_line=Maxline;break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK: 
			if (start_line!=(int)nPos) {
				start_line=nPos;change=TRUE;
				if (start_line<0) start_line=0;
				if (start_line>Maxline) start_line=Maxline;
			}
			break;
	}
	SetScrollPos(SB_VERT,start_line);
	PreventOutScreen(FALSE);
	if (change) Invalidate(TRUE);	
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

// output a string in the start point (x,y)
BOOL CCreateReport::mTextOut(CDC *pDC,int &x,int &y,CString OutStr,COLORREF Color,CFont &Font)
{                                      
	CString temstr; 
	COLORREF oldColor;
	CFont *pOldFont;
	oldColor=pDC->SetTextColor(Color);
	pOldFont=pDC->SelectObject(&Font);
	int clen=OutStr.GetLength();
	int slen=pDC->GetTextExtent(OutStr,OutStr.GetLength()).cx;
	if ((y>=(int)(start_line*lHeight)) && (y<(int)(start_line*lHeight+sHeight))) {
		if ((slen+x)>(int)sWidth) {
			int leftlen=0;
			do {
				if (OutStr[leftlen]>0) leftlen++;else leftlen+=2;
				temstr=OutStr.Left(leftlen);
			} while ((x+pDC->GetTextExtent(temstr,temstr.GetLength()).cx<sWidth) && (leftlen<clen-1));
			pDC->TextOut(x,(y-start_line*lHeight),temstr,temstr.GetLength());
			x=36;y+=lHeight;
			temstr=OutStr.Right(clen-leftlen);
			pDC->TextOut(x,(y-start_line*lHeight),temstr,temstr.GetLength());
			x+=pDC->GetTextExtent(temstr,temstr.GetLength()).cx;
		} else {
			pDC->TextOut(x,(y-start_line*lHeight),OutStr,OutStr.GetLength());
			x+=pDC->GetTextExtent(OutStr,OutStr.GetLength()).cx;
		}
	} else {
		if ((slen+x)>(int)sWidth) {
			int leftlen=0;
			do {
				if (OutStr[leftlen]>0) leftlen++;else leftlen+=2;
				temstr=OutStr.Left(leftlen);
			} while ((x+pDC->GetTextExtent(temstr,temstr.GetLength()).cx<sWidth) && (leftlen<clen-1));
			x=36;y+=lHeight;
			temstr=OutStr.Right(clen-leftlen);
			x+=pDC->GetTextExtent(temstr,temstr.GetLength()).cx;
		} else x+=pDC->GetTextExtent(OutStr,OutStr.GetLength()).cx;
	}
	if (pOldFont!=NULL) pDC->SelectObject(pOldFont);	
	pDC->SetTextColor(oldColor);
	return TRUE;
}   

BOOL CCreateReport::Cacuxy(CDC *pDC,int &x,int &y,CString OutStr)
{                                      
	CString temstr;
	int clen=OutStr.GetLength();
	CFont *pOldFont,SelectchoiceFont;
	SelectchoiceFont.CreateFontIndirect(&m_Selectchoicelogfont);
	pOldFont=pDC->SelectObject(&SelectchoiceFont);
	int slen=pDC->GetTextExtent(OutStr,OutStr.GetLength()).cx;
	if ((slen+x)>(int)sWidth) {
		int leftlen=0;
		do {
			if (OutStr[leftlen]>0) leftlen++;else leftlen+=2;
			temstr=OutStr.Left(leftlen);
		} while ((x+pDC->GetTextExtent(temstr,temstr.GetLength()).cx<sWidth) && (leftlen<clen-1));
		x=36;y+=lHeight;
		temstr=OutStr.Right(clen-leftlen);
		x+=pDC->GetTextExtent(temstr,temstr.GetLength()).cx;
	} else x+=pDC->GetTextExtent(OutStr,OutStr.GetLength()).cx;
	if (pOldFont!=NULL) pDC->SelectObject(pOldFont);	
	return TRUE;
}   

BOOL CCreateReport::LocateRect(CDC* pDC)
{
	int i,x,y;
	CString temstr;    
	linedata *lineptr;
	CStringArray *ChoiceArray;
	CFont *pOldFont,SelectchoiceFont;
	SelectchoiceFont.CreateFontIndirect(&m_Selectchoicelogfont);
	pOldFont=pDC->SelectObject(&SelectchoiceFont);
	if (segment<paSee.GetSize()) lineptr=(linedata *)paSee[segment];
		else {
			if (segment>=paSee.GetSize()+paResult.GetSize()) return FALSE;
			lineptr=(linedata *)paResult[segment-paSee.GetSize()];
		}
	if (choice==0) {
		CurArea.top=lineptr->Texty-start_line*lHeight;CurArea.bottom=CurArea.top+lHeight-2;
		CurArea.left=0;CurArea.right=16;
	} else {
		if (choice>lineptr->y.GetSize()) return FALSE;		//not show
		y=lineptr->y[choice-1]-start_line*lHeight;
		ChoiceArray=(CStringArray *)lineptr->Choice[choice-1];
		if ((lineptr->type[choice-1]!=2) || (SelectMode=='s')) {
			x=lineptr->x[choice-1];
			for (i=0;i<min(item,ChoiceArray->GetSize()-1);i++) {
				temstr=ChoiceArray->GetAt(i);temstr+="、";				//???????
				Cacuxy(pDC,x,y,temstr);
			} 
			temstr=ChoiceArray->GetAt(i);
			CurArea.left=x;
			x+=pDC->GetTextExtent(temstr,temstr.GetLength()).cx;
			if ((CurArea.left+x>sWidth) && (sWidth-CurArea.left<x-sWidth)) {CurArea.left=36;y+=lHeight;x=20+(x-sWidth);}
			CurArea.right=x;
		} else {
			CurArea.left=x=lineptr->x[choice-1];
			for (i=0;i<3;i++) {
				temstr=ChoiceArray->GetAt(i);
				x+=pDC->GetTextExtent(temstr,temstr.GetLength()).cx;
				if (i==1) x+=pDC->GetTextExtent("1",1).cx*max(0,6-temstr.GetLength());
			} 
			if ((CurArea.left+x>sWidth) && (sWidth-CurArea.left<x-sWidth)) {CurArea.left=36;y+=lHeight;x=20+(x-sWidth);}
			CurArea.right=x;
		}
		CurArea.top=y;
		CurArea.bottom=CurArea.top+lHeight-2;
	}
	if (pOldFont!=NULL) pDC->SelectObject(pOldFont);	
	return TRUE;
}

//Label segment select(unselect) label
void CCreateReport::Label(CDC* pDC,int xs,int ys,BOOL selected)
{
	int x,y;
	if ((ys>=(int)(start_line*lHeight)) && (ys<(int)(start_line*lHeight+sHeight))) {
		x=xs-2;y=ys-start_line*lHeight;
		pDC->Rectangle(x,y,x+16,y+lHeight-2);
		if (selected) {
			pDC->MoveTo(x+3,y+8);pDC->LineTo(x+6,y+12);pDC->LineTo(x+12,y+4);
			pDC->MoveTo(x+4,y+8);pDC->LineTo(x+7,y+12);pDC->LineTo(x+13,y+4);
		}
	}
}

void CCreateReport::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	int i,j,k,x,y,choicestatus;
	linedata *lineptr;BOOL first;
	CStringArray *ChoiceArray;          
	CByteArray *pAChoiceStatus;
	CFont SelecttextFont,SelectchoiceFont,UnSelecttextFont,UnSelectchoiceFont;
	SelecttextFont.CreateFontIndirect(&m_Selecttextlogfont);
	UnSelecttextFont.CreateFontIndirect(&m_UnSelecttextlogfont);
	SelectchoiceFont.CreateFontIndirect(&m_Selectchoicelogfont);
	UnSelectchoiceFont.CreateFontIndirect(&m_UnSelectchoicelogfont);

//Brush Background color
	CRect rect;
	GetClientRect(rect); 
	CBrush BackBrush;
	BackBrush.CreateSolidBrush(m_BackColor);
	CBrush *pOldBrush=dc.SelectObject(&BackBrush);
	dc.PatBlt(rect.left,rect.top,rect.Width(),rect.Height(),PATCOPY);
	if (pOldBrush!=NULL) dc.SelectObject(pOldBrush);
	if (paSee.GetSize()+paResult.GetSize()<1) return;

	dc.SetBkColor(m_BackColor);	

	x=20;y=0;
	for (i=0;i<paSee.GetSize();i++)
	{       
		lineptr=(linedata *)paSee[i]; 
		if (!located) {
			lineptr->x.RemoveAll();lineptr->y.RemoveAll();
		}
		if ((SelectMode=='c') || (lineptr->status)) {
			if (!located) lineptr->Texty=y;
			if (SelectMode!='s') Label(&dc,x-18,y,lineptr->status);
			if (lineptr->status) {mTextOut(&dc,x,y,lineptr->Text[0],m_SelecttextColor,SelecttextFont);}
				else {mTextOut(&dc,x,y,lineptr->Text[0],m_UnSelecttextColor,UnSelecttextFont);}
			for (j=0;j<lineptr->Choice.GetSize();j++)
			{	
				ChoiceArray=(CStringArray *)lineptr->Choice[j];
				pAChoiceStatus=(CByteArray *)lineptr->ChoiceStatus[j];
				if (!located) {lineptr->x.Add(x);lineptr->y.Add(y);}
				first=TRUE;
				for (k=0;k<ChoiceArray->GetSize();k++)           
				{   
					if ((SelectMode!='s') || (pAChoiceStatus->GetAt(k)==1)) {
						if (lineptr->status) {
							switch (pAChoiceStatus->GetAt(k)) {
								case 0:choicestatus=0;break;
								case 1:choicestatus=1;break;
								case 2:choicestatus=0;break;
							}
						} else {
							switch (pAChoiceStatus->GetAt(k)) {
								case 0:choicestatus=0;break;
								case 1:choicestatus=0;break;
								case 2:choicestatus=0;break;
							}
						}
						switch (choicestatus) {
							case 0:
								if ((!first) && (lineptr->type[j]!=2)) mTextOut(&dc,x,y,"、",m_UnSelectchoiceColor,UnSelectchoiceFont);
								mTextOut(&dc,x,y,ChoiceArray->GetAt(k),m_UnSelectchoiceColor,UnSelectchoiceFont);
								break;
							case 1:
								if ((!first) && (lineptr->type[j]!=2)) mTextOut(&dc,x,y,"、",m_SelectchoiceColor,SelectchoiceFont);
								mTextOut(&dc,x,y,ChoiceArray->GetAt(k),m_SelectchoiceColor,SelectchoiceFont);
								break;
						}
						if ((k==1) && (lineptr->type[j]==2) && (SelectMode!='s')) {
							int temchrlen=dc.GetTextExtent("1",1).cx;
							for (int ii=0;ii<max(0,6-ChoiceArray->GetAt(k).GetLength());ii++)
								if (x+temchrlen<=(int)sWidth) x+=temchrlen; else {x=36;y+=lHeight;}
						}
						first=FALSE;
					}
				}
				if (lineptr->status) mTextOut(&dc,x,y,lineptr->Text[j+1],m_SelecttextColor,SelecttextFont);
					else mTextOut(&dc,x,y,lineptr->Text[j+1],m_UnSelecttextColor,UnSelecttextFont);
			}
			y+=lHeight+6;x=20;
		}
	}
	y=y+lHeight;
	for (i=0;i<paResult.GetSize();i++)
	{                 
		lineptr=(linedata *)paResult[i];
		if (!located) {lineptr->x.RemoveAll();lineptr->y.RemoveAll();}
		if ((SelectMode=='c') || (lineptr->status)) {
			if (!located) lineptr->Texty=y;
			if (SelectMode!='s') Label(&dc,x-18,y,lineptr->status);
			if (lineptr->status) {mTextOut(&dc,x,y,lineptr->Text[0],m_SelecttextColor,SelecttextFont);}
				else {mTextOut(&dc,x,y,lineptr->Text[0],m_UnSelecttextColor,UnSelecttextFont);}
			for (j=0;j<lineptr->Choice.GetSize();j++)
			{	
				ChoiceArray=(CStringArray *)lineptr->Choice[j];
				pAChoiceStatus=(CByteArray *)lineptr->ChoiceStatus[j];
				if (!located) {lineptr->x.Add(x);lineptr->y.Add(y);}
				first=TRUE;
				for (k=0;k<ChoiceArray->GetSize();k++)           
				{                      
					if ((SelectMode!='s') || (pAChoiceStatus->GetAt(k)==1)) {
						if (lineptr->status) {
							switch (pAChoiceStatus->GetAt(k)) {
								case 0:choicestatus=0;break;
								case 1:choicestatus=1;break;
								case 2:choicestatus=0;break;
							}
						} else {
							switch (pAChoiceStatus->GetAt(k)) {
								case 0:choicestatus=0;break;
								case 1:choicestatus=0;break;
								case 2:choicestatus=0;break;
							}
						}
						switch (choicestatus) {
							case 0:
								if ((!first) && (lineptr->type[j]!=2)) mTextOut(&dc,x,y,"、",m_UnSelectchoiceColor,UnSelectchoiceFont);
								mTextOut(&dc,x,y,ChoiceArray->GetAt(k),m_UnSelectchoiceColor,UnSelectchoiceFont);
								break;
							case 1:
								if ((!first) && (lineptr->type[j]!=2)) mTextOut(&dc,x,y,"、",m_SelectchoiceColor,SelectchoiceFont);
								mTextOut(&dc,x,y,ChoiceArray->GetAt(k),m_SelectchoiceColor,SelectchoiceFont);
								break;
						}
						if ((k==1) && (lineptr->type[j]==2) && (SelectMode!='s')) {
							int temchrlen=dc.GetTextExtent("1",1).cx;
							for (int ii=0;ii<max(0,6-ChoiceArray->GetAt(k).GetLength());ii++)
								if (x+temchrlen<=(int)sWidth) x+=temchrlen; else {x=36;y+=lHeight;}
						}
						first=FALSE;
					}
				}
				if (lineptr->status) mTextOut(&dc,x,y,lineptr->Text[j+1],m_SelecttextColor,SelecttextFont);
					else mTextOut(&dc,x,y,lineptr->Text[j+1],m_UnSelecttextColor,UnSelecttextFont);
			}
			y+=lHeight+6;x=20;
		}
	}
	LocateRect(&dc);
	CBrush tem1Brush;
	tem1Brush.CreateSolidBrush(RGB(255,255,255));
	dc.FrameRect(&CurArea,&tem1Brush);
	if (!located) {
		Maxline=y/max(1,lHeight);
		SetScrollRange(SB_VERT,0,y/max(1,lHeight));
		SetScrollPos(SB_VERT,start_line);
	}
	located=TRUE;	
	// Do not call CWnd::OnPaint() for painting messages
}

//if start_line change return TRUE otherwise return FALSE;
BOOL CCreateReport::PreventOutScreen(BOOL MoveStartline)
{   
	int i,j,k,x,y,old_start_line;
	linedata *lineptr;
	CStringArray *ChoiceArray;
	CString temstr;
	CClientDC myDC(this);
	old_start_line=start_line;
	if (MoveStartline) {
		if (segment<paSee.GetSize()) {
			if (choice==0) {
				lineptr=(linedata *)paSee[segment];          
				y=lineptr->Texty;
			} else {
				lineptr=(linedata *)paSee[segment];          
				ChoiceArray=(CStringArray *)lineptr->Choice[choice-1];
				x=lineptr->x[choice-1];y=lineptr->y[choice-1];
				for (k=0;k<item;k++)
				{
					temstr=ChoiceArray->GetAt(k);
					if (lineptr->type[choice-1]!=2) temstr+="、";
					Cacuxy(&myDC,x,y,temstr);
				}
			}
			if (y<start_line*lHeight) start_line=y/max(1,lHeight);
				else if (y>(start_line-1+Pagesize)*lHeight) start_line=y/max(1,lHeight)-Pagesize+1;
		} else {
			if (choice==0) {
				lineptr=(linedata *)paResult[segment-paSee.GetSize()];          
				y=lineptr->Texty;
			} else {
				lineptr=(linedata *)paResult[segment-paSee.GetSize()];          
				ChoiceArray=(CStringArray *)lineptr->Choice[choice-1];
				x=lineptr->x[choice-1];y=lineptr->y[choice-1];
				for (k=0;k<item;k++)
				{
					temstr=ChoiceArray->GetAt(k);
					if (lineptr->type[choice-1]!=2) temstr+="、";
					Cacuxy(&myDC,x,y,temstr);
				}
			}
			if (y<start_line*lHeight) start_line=y/max(1,lHeight);
				else if (y>(start_line-1+Pagesize)*lHeight) start_line=y/max(1,lHeight)-Pagesize+1;
		}
	} else {
		int start_seg,start_choice,start_item,end_seg,end_choice,end_item;
		BOOL start_find=FALSE;
		start_seg=start_choice=start_item=0;
		end_seg=paSee.GetSize()+paResult.GetSize()-1;
		end_choice=end_item=0;
		for (i=0;i<paSee.GetSize();i++)
		{       
			lineptr=(linedata *)paSee[i];          
			if ((SelectMode=='c') || (lineptr->status)) {
				if ((lineptr->Texty-start_line*lHeight>=0) && (!start_find)) {
					start_seg=i;start_choice=0;start_item=0;start_find=TRUE;
				}
				if (lineptr->Texty-(start_line-1)*lHeight<Pagesize*lHeight) {
					end_seg=i;end_choice=0;end_item=0;
				}
				for (j=0;j<lineptr->Choice.GetSize();j++)
				{	
					ChoiceArray=(CStringArray *)lineptr->Choice[j];
					x=lineptr->x[j];y=lineptr->y[j];
					for (k=0;k<ChoiceArray->GetSize();k++)
					{
						if ((y-start_line*lHeight>=0) && (!start_find)) {
							start_seg=i;start_choice=j;start_item=k;start_find=TRUE;
						}
						if (lineptr->Texty-(start_line-1)*lHeight<Pagesize*lHeight) {
							end_seg=i;end_choice=j;end_item=k;
						}
						temstr=ChoiceArray->GetAt(k);
						if (lineptr->type[j]!=2) temstr+="、";
						Cacuxy(&myDC,x,y,temstr);						
					}
				}
			}
		}
		for (i=0;i<paResult.GetSize();i++)
		{       
			lineptr=(linedata *)paResult[i];          
			if ((SelectMode=='c') || (lineptr->status)) {
				if ((lineptr->Texty-start_line*lHeight>=0) && (!start_find)) {
					start_seg=i+paSee.GetSize();start_choice=0;start_item=0;start_find=TRUE;
				}
				if (lineptr->Texty-(start_line-1)*lHeight<Pagesize*lHeight) {
					end_seg=i+paSee.GetSize();end_choice=0;end_item=0;
				}
				for (j=0;j<lineptr->Choice.GetSize();j++)
				{	
					ChoiceArray=(CStringArray *)lineptr->Choice[j];
					x=lineptr->x[j];y=lineptr->y[j];
					for (k=0;k<ChoiceArray->GetSize();k++)
					{
						if ((y-start_line*lHeight>=0) && (!start_find)) {
							start_seg=i+paSee.GetSize();start_choice=j;start_item=k;start_find=TRUE;
						}
						if (lineptr->Texty-(start_line-1)*lHeight<Pagesize*lHeight) {
							end_seg=i+paSee.GetSize();end_choice=j;end_item=k;
						}
						temstr=ChoiceArray->GetAt(k);
						if (lineptr->type[j]!=2) temstr+="、";
						Cacuxy(&myDC,x,y,temstr);						
					}
				}
			}
		}
		if ((segment<start_seg) || ((segment==start_seg) && (choice<start_choice))
			 || ((segment==start_seg) && (choice==start_choice) && (item<start_item))) {
			segmento=segment;choiceo=choice;itemo=item;	
			 segment=start_seg;choice=start_choice;item=start_item;
		}
		if ((segment>end_seg) || ((segment==end_seg) && (choice>end_choice))
			 || ((segment==end_seg) && (choice==end_choice) && (item>end_item))) {
			segmento=segment;choiceo=choice;itemo=item;	
			 segment=end_seg;choice=0;item=0;
		}
		if (segment>=paSee.GetSize()+paResult.GetSize()) segment=paSee.GetSize()+paResult.GetSize()-1;
		if (segment<0) segment=0;
		if (segment<paSee.GetSize()) lineptr=(linedata *)paSee[segment];
			else lineptr=(linedata *)paResult[segment-paSee.GetSize()];
		if (choice>=lineptr->Choice.GetSize()) choice=lineptr->Choice.GetSize()-1;
		if (choice<0) choice=0;
		if ((lineptr->Choice.GetSize()>0) && (choice>0)) {
			ChoiceArray=(CStringArray *)lineptr->Choice[choice-1];
			if (item>ChoiceArray->GetSize()) item=ChoiceArray->GetSize()-1;
			if (item<0) item=0;
		} else item=0;
	}
	if (old_start_line!=start_line) return TRUE;else return FALSE;
}

//if (Segment Choice or item change return TRUE otherwise return FALSE;
BOOL CCreateReport::LastSeg(void)
{   
	int i;
	linedata *lineptr;
	BOOL change; 
	segmento=segment;choiceo=choice;itemo=item;	
	change=FALSE;i=segment;
	if (i>0) {
		do {
			i--;
			if (i<paSee.GetSize()) lineptr=(linedata *)paSee[i];
				else lineptr=(linedata *)paResult[i-paSee.GetSize()];
		} while ((i>0) && (SelectMode!='c') && (lineptr->status==FALSE));
		if ((SelectMode=='c') || (lineptr->status==TRUE)) {
			segment=i;change=TRUE;
		} else if (choice || item) change=TRUE;
	} else if (choice || item) change=TRUE;
	choice=0;item=0;
	return change;
}

//if (Segment Choice or item change return TRUE otherwise return FALSE;
BOOL CCreateReport::NextSeg(void)
{   
	int i;
	linedata *lineptr;
	BOOL change; 
	segmento=segment;choiceo=choice;itemo=item;	
	change=FALSE;i=segment;
	if (i<paSee.GetSize()+paResult.GetSize()-1) {
		do {
			i++;
			if (i<paSee.GetSize()) lineptr=(linedata *)paSee[i];
				else lineptr=(linedata *)paResult[i-paSee.GetSize()];
		} while ((i<paSee.GetSize()+paResult.GetSize()-1) && (SelectMode!='c') && (lineptr->status==FALSE));
		if ((SelectMode=='c') || (lineptr->status==TRUE)) {
			segment=i;change=TRUE;
		} else if (choice || item) change=TRUE;
	} else if (choice || item) change=TRUE;
	choice=0;item=0;
	return change;
}

//if (Segment Choice or item change return TRUE otherwise return FALSE;
BOOL CCreateReport::NextChoice(void)
{   
	linedata *lineptr;
	CStringArray *ChoiceArray;
	CByteArray *pAChoiceStatus;
	BOOL change,loop; 
	int Tsegment,Tchoice,Titem,segmentos,choiceos,itemos;
	if (SelectMode=='c') return NextSeg();
	segmentos=segment;choiceos=choice;itemos=item;	
	if (segment<paSee.GetSize()) lineptr=(linedata *)paSee[segment];
		else lineptr=(linedata *)paResult[segment-paSee.GetSize()];
	do {
		Tsegment=segment;Tchoice=choice;Titem=item;
		if (choice==0) {
			if (lineptr->Choice.GetSize()>0) {choice++;item=0;}
				else if (segment<paSee.GetSize()+paResult.GetSize()-1) NextSeg();
			if ((Tsegment==segment) && (Tchoice==choice) && (Titem==item))
			{
				segment=segmento;choice=choiceo;item=itemo;	
				break;
			}	
		} else {
			ChoiceArray=(CStringArray *)lineptr->Choice[choice-1];
			if (choice<lineptr->Choice.GetSize()) {choice++;item=0;}
				else if (segment<paSee.GetSize()+paResult.GetSize()-1) {
					NextSeg();
					if ((segment<Tsegment) || ((segment==Tsegment) && (choice<Tchoice))
		 				|| ((segment==Tsegment) && (choice==Tchoice) && (item<Titem))) {
		 				segment=Tsegment;choice=Tchoice;item=Titem;						//Reach End
						break;
		 			}
				}
		}
		if (segment<paSee.GetSize()) lineptr=(linedata *)paSee[segment];
			else lineptr=(linedata *)paResult[segment-paSee.GetSize()];
		if (choice>0) {
			if (lineptr->type[choice-1]==2) {
				ChoiceArray=(CStringArray *)lineptr->Choice[choice-1];
				pAChoiceStatus=(CByteArray *)lineptr->ChoiceStatus[choice-1];
				ChangeStatus(lineptr->type[choice-1],ChoiceArray,pAChoiceStatus,item);	//input number data
				item=2;					//skip whole number context
				FreshLocalRect();
			}
		}   
		if (choice>0) loop=(lineptr->type[choice-1]==2) && ((Tsegment!=segment) || (Tchoice!=choice) || (Titem!=item));
			else if (SelectMode=='c')  loop=FALSE; else loop=TRUE;
	} while (loop);
	segmento=segmentos;choiceo=choiceos;itemo=itemos;	
	return change;
}

//if (Segment Choice or item change return TRUE otherwise return FALSE;
BOOL CCreateReport::LastItem(void)
{   
	linedata *lineptr;
	CStringArray *ChoiceArray;
	BOOL change;int oldseg,segmentos,choiceos,itemos;
	change=FALSE;
	if (SelectMode=='c') return LastSeg();
	segmentos=segment;choiceos=choice;itemos=item;	
	if (choice==0) {
		oldseg=segment;
		LastSeg(); 
		if (oldseg!=segment) {
			change=TRUE;
			if (segment<paSee.GetSize()) lineptr=(linedata *)paSee[segment];
				else lineptr=(linedata *)paResult[segment-paSee.GetSize()];
			choice=lineptr->Choice.GetSize();
			if (choice>0) {
				ChoiceArray=(CStringArray *)lineptr->Choice[choice-1];
				item=ChoiceArray->GetSize()-1;
			} else item=0;
		} 
	} else {
		change=TRUE;
		if (segment<paSee.GetSize()) lineptr=(linedata *)paSee[segment];
			else lineptr=(linedata *)paResult[segment-paSee.GetSize()];
		ChoiceArray=(CStringArray *)lineptr->Choice[choice-1];
		if (item>0) item--;
			else if (lineptr->Choice.GetSize()>0) {
				choice--;
				if (choice>0) {
					ChoiceArray=(CStringArray *)lineptr->Choice[choice-1];
					item=ChoiceArray->GetSize()-1;
				} else item=0;
			}
	}
	segmento=segmentos;choiceo=choiceos;itemo=itemos;	
	return change;
}

//if (Segment Choice or item change return TRUE otherwise return FALSE;
BOOL CCreateReport::NextItem(void)
{   
	linedata *lineptr;
	CStringArray *ChoiceArray;
	CByteArray *pAChoiceStatus;
	BOOL change,loop; 
	int Tsegment,Tchoice,Titem,segmentos,choiceos,itemos;
	if (SelectMode=='c') return NextSeg();
	segmentos=segment;choiceos=choice;itemos=item;	
	if (segment<paSee.GetSize()) lineptr=(linedata *)paSee[segment];
		else lineptr=(linedata *)paResult[segment-paSee.GetSize()];
	do {
		Tsegment=segment;Tchoice=choice;Titem=item;
		if (choice==0) {
			if ((lineptr->Choice.GetSize()>0) && (lineptr->status!=FALSE)) {choice++;item=0;}
				else {if (segment<paSee.GetSize()+paResult.GetSize()-1) NextSeg();}
			if ((Tsegment==segment) && (Tchoice==choice) && (Titem==item))
			{
				segment=segmento;choice=choiceo;item=itemo;	
				break;
			}	
		} else {
			ChoiceArray=(CStringArray *)lineptr->Choice[choice-1];
			if (item<ChoiceArray->GetSize()-1) item++;
				else if (choice<lineptr->Choice.GetSize()) {choice++;item=0;}
					else if (segment<paSee.GetSize()+paResult.GetSize()-1) {
						NextSeg();
						if ((segment<Tsegment) || ((segment==Tsegment) && (choice<Tchoice))
			 				|| ((segment==Tsegment) && (choice==Tchoice) && (item<Titem))) {
			 				segment=Tsegment;choice=Tchoice;item=Titem;						//Reach End
							break;
			 			}
					}
		}
		if (segment<paSee.GetSize()) lineptr=(linedata *)paSee[segment];
			else lineptr=(linedata *)paResult[segment-paSee.GetSize()];
		if (choice>0) {
			if (lineptr->type[choice-1]==2) {
				ChoiceArray=(CStringArray *)lineptr->Choice[choice-1];
				pAChoiceStatus=(CByteArray *)lineptr->ChoiceStatus[choice-1];
				ChangeStatus(lineptr->type[choice-1],ChoiceArray,pAChoiceStatus,item);	//input number data
				item=2;					//skip whole number context
				FreshLocalRect();
			}
		}   
		if (choice>0) loop=(lineptr->type[choice-1]==2) && ((Tsegment!=segment) || (Tchoice!=choice) || (Titem!=item));
			else if (SelectMode=='c')  loop=FALSE; else loop=TRUE;
	} while (loop);
	segmento=segmentos;choiceo=choiceos;itemo=itemos;	
	return change;
}

void CCreateReport::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL change=FALSE;
	WORD Key;          
	linedata *lineptr;
	CStringArray *ChoiceArray;
	CByteArray *pAChoiceStatus;
	if (SelectMode=='s') return;			//模拟显示，不允许选择
	Key=(WORD)nChar+(WORD)nFlags*0x100;
	if (Key==0x1c0d) {																			//Enter
		if (segment<paSee.GetSize()) lineptr=(linedata *)paSee[segment];
			else lineptr=(linedata *)paResult[segment-paSee.GetSize()];
//		if ((choice==0) && (item==0)) {
		if (choice==0) {
			if (lineptr->status) lineptr->status=FALSE;else lineptr->status=TRUE;
			if (SelectMode=='x') {
				located=FALSE;Invalidate();
			}
		} else {
			ChoiceArray=(CStringArray *)lineptr->Choice[choice-1];
			pAChoiceStatus=(CByteArray *)lineptr->ChoiceStatus[choice-1];
			ChangeStatus(lineptr->type[choice-1],ChoiceArray,pAChoiceStatus,item);
		}                                     
		if ((choice==0) && (lineptr->status!=FALSE)) NextItem();
			else {
				if ((choice==0) && (lineptr->status==FALSE)) NextSeg();
				else {
					if (lineptr->type[choice-1]==1) NextItem();else NextChoice();
				}
			}
		CRect rect;
		rect.left=0;rect.right=GetSystemMetrics(SM_CXSCREEN)-1;
		if (segment<paSee.GetSize())
		{       
			lineptr=(linedata *)paSee[segment];          
			rect.top=lineptr->Texty-start_line*lHeight;
			if (segment<paSee.GetSize()-1) lineptr=(linedata *)paSee[segment+1];          
				else lineptr=(linedata *)paResult[segment+1-paSee.GetSize()];          
			rect.bottom=lineptr->Texty-start_line*lHeight;
		} else {
			lineptr=(linedata *)paResult[segment-paSee.GetSize()];          
			rect.top=lineptr->Texty-start_line*lHeight;
			if (segment<paSee.GetSize()+paResult.GetSize()-1) {
				lineptr=(linedata *)paResult[segment+1-paSee.GetSize()];
				rect.bottom=lineptr->Texty-start_line*lHeight;
			} else rect.bottom=rect.top+3*lHeight;
		}
		FreshLocalRect();
	} else {
		switch (Key)
		{   
			case 0x4826:																			//Arrow Up
				change=LastSeg();
				if (change) change=PreventOutScreen(TRUE);
				break;
			case 0x5028:																			//Arrow Down
				change=NextSeg();
				if (change) change=PreventOutScreen(TRUE);
				break;	                                                    
			case 0x4b25:                                                                            //Arrow Left	
				change=LastItem();
				if (segment<paSee.GetSize()) lineptr=(linedata *)paSee[segment];
					else lineptr=(linedata *)paResult[segment-paSee.GetSize()];
				if (choice>0) {
					if (lineptr->type[choice-1]==2) {
						ChoiceArray=(CStringArray *)lineptr->Choice[choice-1];
						pAChoiceStatus=(CByteArray *)lineptr->ChoiceStatus[choice-1];
						ChangeStatus(lineptr->type[choice-1],ChoiceArray,pAChoiceStatus,item);	//input number data
						item=0;										//skip whole number context
					}
				}
				if (change) change=PreventOutScreen(TRUE);																									
				break;
			case 0x4d27:																			//Arrow Right
				change=NextItem();
				if (change) change=PreventOutScreen(TRUE);
				break;

			case 0x4f23:if (start_line<Maxline-Pagesize) {                                          //End
					start_line=Maxline-Pagesize;change=TRUE;
					PreventOutScreen(FALSE);
				}                                                                                   
				break;					                                                            
			case 0x4724:if (start_line>0) {                                                         //Home
					start_line=0;change=TRUE;
					PreventOutScreen(FALSE);
				}
				break;													
			case 0x4921:if (start_line>0) {                                                         //Page Up
					start_line-=Pagesize;
					if (start_line<0) start_line=0;
					change=TRUE;
					PreventOutScreen(FALSE);
				}
				break;						
			case 0x5122:if (start_line<Maxline-Pagesize) {                                          //Page Down
					start_line+=Pagesize;
					if (start_line>Maxline) start_line=Maxline;
					change=TRUE;
					PreventOutScreen(FALSE);
				}
				break;	
		}
		if (change) {
			SetScrollPos(SB_VERT,start_line);
			Invalidate(TRUE);	
		} else FreshLocalRect();
	}
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCreateReport::CacuNumSite(CDC* pDC,CPoint &topleft)
{
	int i,x,y;
	CString temstr;    
	linedata *lineptr;
	CStringArray *ChoiceArray;
	if (segment<paSee.GetSize()) lineptr=(linedata *)paSee[segment];
		else lineptr=(linedata *)paResult[segment-paSee.GetSize()];
	if (choice==0) {
		topleft.y=lineptr->Texty-start_line*lHeight;
		topleft.x=0;
	} else {
		y=lineptr->y[choice-1]-start_line*lHeight;
		ChoiceArray=(CStringArray *)lineptr->Choice[choice-1];
		if ((lineptr->type[choice-1]!=2) || (SelectMode=='s')) {
			x=lineptr->x[choice-1];
			for (i=0;i<min(item,ChoiceArray->GetSize()-1);i++) {
				temstr=ChoiceArray->GetAt(i);temstr+="、";				//???????
				Cacuxy(pDC,x,y,temstr);
			} 
			temstr=ChoiceArray->GetAt(i);
			topleft.x=x;
			x+=pDC->GetTextExtent(temstr,temstr.GetLength()).cx;
			if ((topleft.x+x>sWidth) && (sWidth-topleft.x<x-sWidth)) {topleft.x=36;y+=lHeight;x=20+(x-sWidth);}
		} else {
			topleft.x=x=lineptr->x[choice-1];
			temstr=ChoiceArray->GetAt(0);
			x+=pDC->GetTextExtent(temstr,temstr.GetLength()).cx;
			if ((topleft.x+x>sWidth) && (sWidth-topleft.x<x-sWidth)) {topleft.x=36;y+=lHeight;x=20+(x-sWidth);}
			topleft.x=x;
		}
		topleft.y=y;
	}
}
    
void CCreateReport::ChangeStatus(int type,CStringArray *ChoiceArray,CByteArray *pAChoiceStatus,int CurItem)
{
	int l;CPoint leftop;
	CClientDC dc(this);
	CacuNumSite(&dc,leftop);
	NumInput Dlg(leftop.x,leftop.y);
	switch (type) {
		case 0:
			if (pAChoiceStatus->GetAt(CurItem)!=1) {
				for (l=0;l<ChoiceArray->GetSize();l++)
					pAChoiceStatus->SetAt(l,2);
				pAChoiceStatus->SetAt(CurItem,1);
			} else {
				pAChoiceStatus->SetAt(CurItem,2);
			}
			break;
		case 1:
			if (pAChoiceStatus->GetAt(CurItem)==1) pAChoiceStatus->SetAt(CurItem,0);else pAChoiceStatus->SetAt(CurItem,1);
			break;
		case 2:
			Dlg.m_Data=ChoiceArray->GetAt(1);
			if (Dlg.DoModal()==IDOK) {
				ChoiceArray->SetAt(1,Dlg.m_Data);
				located=FALSE;
				if (Dlg.m_Data.GetLength()>0) {
					for (l=0;l<3;l++)
						pAChoiceStatus->SetAt(l,1);
				} else {
					for (l=0;l<3;l++)
						pAChoiceStatus->SetAt(l,2);
				}
			}
			break;
	}
}

void CCreateReport::FreshLocalRect(void)
{
	CRect rect;
	linedata *lineptr;
	rect.left=0;rect.right=GetSystemMetrics(SM_CXSCREEN)-1;
	if (segmento<0) return;
	if (segmento<paSee.GetSize())
	{       
		lineptr=(linedata *)paSee[segmento];          
		rect.top=lineptr->Texty-start_line*lHeight;
		if (segmento<paSee.GetSize()-1) {
			lineptr=(linedata *)paSee[segmento+1];          
			rect.bottom=lineptr->Texty-start_line*lHeight;
		} else 
			if (paResult.GetSize()>0) {
				lineptr=(linedata *)paResult[segmento+1-paSee.GetSize()];          
				rect.bottom=lineptr->Texty-start_line*lHeight;
			} else {
				rect.bottom=rect.top+3*lHeight;
			}

	} else {
		lineptr=(linedata *)paResult[segmento-paSee.GetSize()];          
		rect.top=lineptr->Texty-start_line*lHeight;
		if (segmento+1<paSee.GetSize()+paResult.GetSize()) {
			lineptr=(linedata *)paResult[segmento+1-paSee.GetSize()];
			rect.bottom=lineptr->Texty-start_line*lHeight;
		} else rect.bottom=rect.top+3*lHeight;
	}
	InvalidateRect(rect,TRUE);

	if (segment<paSee.GetSize())
	{       
		lineptr=(linedata *)paSee[segment];          
		rect.top=lineptr->Texty-start_line*lHeight;
		if (segment<paSee.GetSize()-1) {
			lineptr=(linedata *)paSee[segment+1];          
			rect.bottom=lineptr->Texty-start_line*lHeight;
		} else 
			if (paResult.GetSize()>0) {
				lineptr=(linedata *)paResult[segment+1-paSee.GetSize()];          
				rect.bottom=lineptr->Texty-start_line*lHeight;
			} else {
				rect.bottom=rect.top+3*lHeight;
			}
	} else {
		lineptr=(linedata *)paResult[segment-paSee.GetSize()];          
		rect.top=lineptr->Texty-start_line*lHeight;
		if (segment+1<paSee.GetSize()+paResult.GetSize()) {
			lineptr=(linedata *)paResult[segment+1-paSee.GetSize()];
			rect.bottom=lineptr->Texty-start_line*lHeight;
		} else rect.bottom=rect.top+3*lHeight;
	}
	InvalidateRect(rect,TRUE);
}

void CCreateReport::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	int i,j,k,x,y,lenx;
	linedata *lineptr;
	CStringArray *ChoiceArray;
	CByteArray *pAChoiceStatus;
	CClientDC myDC(this);
	CFont SelecttextFont,*pOldFont;
	SelecttextFont.CreateFontIndirect(&m_Selecttextlogfont);
	pOldFont=myDC.SelectObject(&SelecttextFont);
	if (SelectMode=='s') return;			//模拟显示，不允许选择
	CString temstr;
	for (i=0;i<paSee.GetSize();i++)
	{       
		lineptr=(linedata *)paSee[i];          
		y=lineptr->Texty-start_line*lHeight;
		if ((y>=0) && ((SelectMode=='c') || (lineptr->status))) {
			if (point.x<24) {
				if ((point.y>y) && (point.y<y+lHeight)) {
					segmento=segment;choiceo=choice;itemo=item;	
					segment=i;choice=0;item=0;
					if (lineptr->status) lineptr->status=FALSE;else lineptr->status=TRUE;
					if (SelectMode=='x') {
						located=FALSE;Invalidate();
					}
					goto find;
				}
			} else {
				for (j=0;j<lineptr->Choice.GetSize();j++)
				{	
					ChoiceArray=(CStringArray *)lineptr->Choice[j];
					pAChoiceStatus=(CByteArray *)lineptr->ChoiceStatus[j];
					x=lineptr->x[j];y=lineptr->y[j]-start_line*lHeight;
					for (k=0;k<ChoiceArray->GetSize();k++)
					{
						temstr=ChoiceArray->GetAt(k);
						lenx=myDC.GetTextExtent(temstr,temstr.GetLength()).cx;
						if ((k==1) && (lineptr->type[j]==2) && (SelectMode!='s')) lenx+=myDC.GetTextExtent("1",1).cx*max(0,6-ChoiceArray->GetAt(k).GetLength());
						if (x+lenx>sWidth) {
							if (((point.x>x) && (point.x<sWidth+16) && (point.y>y) && (point.y<y+lHeight))
								|| ((point.x>36) && (point.x<36+x+lenx-sWidth) && (point.y>y+lHeight) && (point.y<y+2*lHeight)))
							{
								segmento=segment;choiceo=choice;itemo=item;	
								segment=i;choice=j+1;item=k;
								ChangeStatus(lineptr->type[j],ChoiceArray,pAChoiceStatus,k);
								goto find;
							}
						} else {
							if ((point.x>x) && (point.x<x+lenx) && (point.y>y) && (point.y<y+lHeight))
							{
								segmento=segment;choiceo=choice;itemo=item;	
								segment=i;choice=j+1;item=k;
								ChangeStatus(lineptr->type[j],ChoiceArray,pAChoiceStatus,k);
								goto find;
							}
						}
						if (lineptr->type[j]!=2) temstr+="、";
						Cacuxy(&myDC,x,y,temstr);						
					}
				}
			}
		}
	}
	for (i=0;i<paResult.GetSize();i++)
	{       
		lineptr=(linedata *)paResult[i];          
		y=lineptr->Texty-start_line*lHeight;
		if ((y>=0) && ((SelectMode=='c') || (lineptr->status))) {
			if (point.x<24) {
				if ((point.y>y) && (point.y<y+lHeight)) {
					segmento=segment;choiceo=choice;itemo=item;	
					segment=i+paSee.GetSize();choice=0;item=0;
					if (lineptr->status) lineptr->status=FALSE;else lineptr->status=TRUE;
					if (SelectMode=='x') {
						located=FALSE;Invalidate();
					}
					goto find;
				}
			} else {
				for (j=0;j<lineptr->Choice.GetSize();j++)
				{	
					ChoiceArray=(CStringArray *)lineptr->Choice[j];
					pAChoiceStatus=(CByteArray *)lineptr->ChoiceStatus[j];
					x=lineptr->x[j];y=lineptr->y[j]-start_line*lHeight;
					for (k=0;k<ChoiceArray->GetSize();k++)
					{
						temstr=ChoiceArray->GetAt(k);
						lenx=myDC.GetTextExtent(temstr,temstr.GetLength()).cx;
						if ((k==1) && (lineptr->type[j]==2) && (SelectMode!='s')) lenx+=myDC.GetTextExtent("1",1).cx*max(0,6-ChoiceArray->GetAt(k).GetLength());
						if (x+lenx>sWidth) {
							if (((point.x>x) && (point.x<sWidth) && (point.y>y) && (point.y<y+lHeight))
								|| ((point.x>36) && (point.x<36+x+lenx-sWidth) && (point.y>y+lHeight) && (point.y<y+2*lHeight)))
							{
								segmento=segment;choiceo=choice;itemo=item;	
								segment=i+paSee.GetSize();choice=j+1;item=k;
								ChangeStatus(lineptr->type[j],ChoiceArray,pAChoiceStatus,k);
								goto find;
							}
						} else {
							if ((point.x>x) && (point.x<x+lenx) && (point.y>y) && (point.y<y+lHeight))
							{
								segmento=segment;choiceo=choice;itemo=item;	
								segment=i+paSee.GetSize();choice=j+1;item=k;
								ChangeStatus(lineptr->type[j],ChoiceArray,pAChoiceStatus,k);
								goto find;
							}
						}
						if (lineptr->type[j]!=2) temstr+="、";
						Cacuxy(&myDC,x,y,temstr);						
					}
				}
			}
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
	return;
find:
	FreshLocalRect();
	if (pOldFont!=NULL) myDC.SelectObject(pOldFont);	
	CWnd::OnLButtonDown(nFlags, point);
}

void CCreateReport::OnSegSelect() 
{
	// TODO: Add your command handler code here
	segmento=segment;choiceo=choice;itemo=item;	
	segment=0;choice=0;item=0;start_line=0;
	SelectMode='c';located=FALSE;
	Invalidate(TRUE);	
}

void CCreateReport::OnChoiceSelect() 
{
	// TODO: Add your command handler code here
	int lens,lenr;
	linedata *lineptr;
	segmento=segment;choiceo=choice;itemo=item;	
	segment=0;choice=0;item=0;start_line=0;
	SelectMode='x';located=FALSE;
	lens=paSee.GetSize();lenr=paResult.GetSize();
	if (paSee.GetSize()>0) {
		do {
			lineptr=(linedata *)paSee[segment];
			segment++;
		} while (((lineptr->status==FALSE) || (lineptr->Choice.GetSize()==0)) && (segment<lens));
		while (((lineptr->status==FALSE) || (lineptr->Choice.GetSize()==0)) && (segment<lenr+lens)) {
			lineptr=(linedata *)paResult[segment-lens];
			segment++;
		} 
	} else {
		do {
			lineptr=(linedata *)paResult[segment-lens];
			segment++;
		} while (((lineptr->status==FALSE) || (lineptr->Choice.GetSize()==0)) && (segment<lenr+lens));
	}
	
	if (segment<lenr+lens) {segment--;choice=1;}else {segment=0;choice=0;}
	Invalidate(TRUE);	
}

void CCreateReport::OnShow() 
{
	// TODO: Add your command handler code here
	segmento=segment;choiceo=choice;itemo=item;	
	segment=0;choice=0;item=0;start_line=0;
	SelectMode='s';located=FALSE;
	Invalidate(TRUE);
}

void CCreateReport::OnBackcolor() 
{
	// TODO: Add your command handler code here
	CColorDialog Dlg(m_BackColor,0,NULL);
	if (Dlg.DoModal()==IDOK) m_BackColor=Dlg.GetColor();	
}

void CCreateReport::OnSelecttext() 
{
	// TODO: Add your command handler code here
	CFontDialog FontDlgBox(&m_Selecttextlogfont,CF_EFFECTS|CF_SCREENFONTS,NULL,NULL);
	FontDlgBox.m_cf.rgbColors=m_SelecttextColor;
	if (FontDlgBox.DoModal() == IDOK) {       
		m_SelecttextColor=FontDlgBox.GetColor();
		memcpy(&m_Selecttextlogfont,FontDlgBox.m_cf.lpLogFont,sizeof(LOGFONT));
		m_UnSelecttextlogfont.lfHeight=FontDlgBox.m_cf.lpLogFont->lfHeight;
		m_Selectchoicelogfont.lfHeight=FontDlgBox.m_cf.lpLogFont->lfHeight;
		m_UnSelectchoicelogfont.lfHeight=FontDlgBox.m_cf.lpLogFont->lfHeight;
	}
	located=FALSE;
	lHeight=abs(m_Selecttextlogfont.lfHeight)+2;
	Pagesize=sHeight/max(1,lHeight);
	Invalidate(TRUE);UpdateWindow();
}

void CCreateReport::OnUnselecttext() 
{
	// TODO: Add your command handler code here
	CFontDialog FontDlgBox(&m_UnSelecttextlogfont,CF_EFFECTS|CF_SCREENFONTS,NULL,NULL);
	FontDlgBox.m_cf.rgbColors=m_UnSelecttextColor;
	if (FontDlgBox.DoModal() == IDOK) {       
		m_UnSelecttextColor=FontDlgBox.GetColor();
		memcpy(&m_UnSelecttextlogfont,FontDlgBox.m_cf.lpLogFont,sizeof(LOGFONT));
		m_Selecttextlogfont.lfHeight=FontDlgBox.m_cf.lpLogFont->lfHeight;
		m_Selectchoicelogfont.lfHeight=FontDlgBox.m_cf.lpLogFont->lfHeight;
		m_UnSelectchoicelogfont.lfHeight=FontDlgBox.m_cf.lpLogFont->lfHeight;
	}
	located=FALSE;
	lHeight=abs(m_Selecttextlogfont.lfHeight)+2;
	Pagesize=sHeight/max(1,lHeight);
	Invalidate(TRUE);UpdateWindow();
}

void CCreateReport::OnSelectchoice() 
{
	// TODO: Add your command handler code here
	CFontDialog FontDlgBox(&m_Selectchoicelogfont,CF_EFFECTS|CF_SCREENFONTS,NULL,NULL);
	FontDlgBox.m_cf.rgbColors=m_SelectchoiceColor;
	if (FontDlgBox.DoModal() == IDOK) {       
		m_SelectchoiceColor=FontDlgBox.GetColor();
		memcpy(&m_Selectchoicelogfont,FontDlgBox.m_cf.lpLogFont,sizeof(LOGFONT));
		m_UnSelecttextlogfont.lfHeight=FontDlgBox.m_cf.lpLogFont->lfHeight;
		m_Selecttextlogfont.lfHeight=FontDlgBox.m_cf.lpLogFont->lfHeight;
		m_UnSelectchoicelogfont.lfHeight=FontDlgBox.m_cf.lpLogFont->lfHeight;
	}
	located=FALSE;
	lHeight=abs(m_Selecttextlogfont.lfHeight)+2;
	Pagesize=sHeight/max(1,lHeight);
	Invalidate(TRUE);UpdateWindow();
}

void CCreateReport::OnUnselectchoice() 
{
	// TODO: Add your command handler code here
	CFontDialog FontDlgBox(&m_UnSelectchoicelogfont,CF_EFFECTS|CF_SCREENFONTS,NULL,NULL);
	FontDlgBox.m_cf.rgbColors=m_UnSelectchoiceColor;
	if (FontDlgBox.DoModal() == IDOK) {       
		m_UnSelectchoiceColor=FontDlgBox.GetColor();
		memcpy(&m_UnSelectchoicelogfont,FontDlgBox.m_cf.lpLogFont,sizeof(LOGFONT));
		m_UnSelecttextlogfont.lfHeight=FontDlgBox.m_cf.lpLogFont->lfHeight;
		m_Selecttextlogfont.lfHeight=FontDlgBox.m_cf.lpLogFont->lfHeight;
		m_Selectchoicelogfont.lfHeight=FontDlgBox.m_cf.lpLogFont->lfHeight;
	}
	located=FALSE;
	lHeight=abs(m_Selecttextlogfont.lfHeight)+2;
	Pagesize=sHeight/max(1,lHeight);
	Invalidate(TRUE);UpdateWindow();
}

void CCreateReport::OnAppExit() 
{
	// TODO: Add your command handler code here
	int i,j,k;BOOL first;
	linedata *lineptr;
	CStringArray *ChoiceArray;
	CByteArray *pAChoiceStatus;
	CString Strbuf,filenm;
	//Save Select Text
	if ((m_pEdit!=NULL) && (m_pWnd!=NULL)) {
		for (i=0;i<paSee.GetSize();i++)
		{       
			lineptr=(linedata *)paSee[i]; 
			if (lineptr->status) {
				*m_See+=lineptr->Text[0];
				for (j=0;j<lineptr->Choice.GetSize();j++)
				{	
					ChoiceArray=(CStringArray *)lineptr->Choice[j];
					pAChoiceStatus=(CByteArray *)lineptr->ChoiceStatus[j];
					first=TRUE;
					for (k=0;k<ChoiceArray->GetSize();k++)           
					{   
						if (pAChoiceStatus->GetAt(k)==1) {
							if ((!first) && (lineptr->type[j]!=2)) *m_See+="、";
							*m_See+=ChoiceArray->GetAt(k);first=FALSE;
						}
					}
					*m_See+=lineptr->Text[j+1];
				}
				*m_See+="\r\n";
			}
		}
		
		for (i=0;i<paResult.GetSize();i++)
		{       
			lineptr=(linedata *)paResult[i]; 
			if (lineptr->status) {
				*m_Result+=lineptr->Text[0];
				for (j=0;j<lineptr->Choice.GetSize();j++)
				{	
					ChoiceArray=(CStringArray *)lineptr->Choice[j];
					pAChoiceStatus=(CByteArray *)lineptr->ChoiceStatus[j];
					first=TRUE;
					for (k=0;k<ChoiceArray->GetSize();k++)           
					{   
						if (pAChoiceStatus->GetAt(k)==1) {
							if ((!first) && (lineptr->type[j]!=2)) *m_Result+="、";
							*m_Result+=ChoiceArray->GetAt(k);first=FALSE;
						}
					}
					*m_Result+=lineptr->Text[j+1];
				}
				*m_Result+="\r\n";
			}
		}
	}
	
	//Release data space
	for (i=0;i<paSee.GetSize();i++)
	{                 
		lineptr=(linedata *)paSee[i];
		for (j=0;j<lineptr->Choice.GetSize();j++)
		{	
			ChoiceArray=(CStringArray *)lineptr->Choice[j];
			delete(ChoiceArray);
			pAChoiceStatus=(CByteArray *)lineptr->ChoiceStatus[j];
			delete(pAChoiceStatus);
		}
		delete(lineptr);
	}
	for (i=0;i<paResult.GetSize();i++)
	{                 
		lineptr=(linedata *)paResult[i];
		for (j=0;j<lineptr->Choice.GetSize();j++)	
		{	
			ChoiceArray=(CStringArray *)lineptr->Choice[j];
			delete(ChoiceArray);
			pAChoiceStatus=(CByteArray *)lineptr->ChoiceStatus[j];
			delete(pAChoiceStatus);
		}
		delete(lineptr);
	}	
//Save Color and Text Information
	CFile SysParaFile;
	filenm=theApp.m_Exe_Working_Dir+"system/repcolor.dat";
	if (SysParaFile.Open(filenm,CFile::modeCreate|CFile::modeWrite)) {
		BYTE *temptr,Tem;
		TRY
		{
			CArchive archive(&SysParaFile,CArchive::store);
			archive<<m_BackColor;
			
			archive<<m_SelecttextColor;
			temptr=(BYTE *)&m_Selecttextlogfont;
			for (i=0;i<sizeof(LOGFONT);i++) {Tem=*temptr++;archive<<Tem;}
			
			archive<<m_UnSelecttextColor;       
			temptr=(BYTE *)&m_UnSelecttextlogfont;
			for (i=0;i<sizeof(LOGFONT);i++) {Tem=*temptr++;archive<<Tem;}
				
			archive<<m_SelectchoiceColor;
			temptr=(BYTE *)&m_Selectchoicelogfont;
			for (i=0;i<sizeof(LOGFONT);i++) {Tem=*temptr++;archive<<Tem;}
				
			archive<<m_UnSelectchoiceColor;
			temptr=(BYTE *)&m_UnSelectchoicelogfont;
			for (i=0;i<sizeof(LOGFONT);i++) {Tem=*temptr++;archive<<Tem;}
			
			archive.Close();SysParaFile.Close();
		}
		CATCH (CArchiveException,e)
		{
		}
		END_CATCH
	} 
	if (m_pEdit!=NULL) m_pEdit->UpdateData(FALSE);
	if (m_pWnd!=NULL) m_pWnd->SendMessage(WM_CLOSE);
	SendMessage(WM_CLOSE);
//	m_pWnd->Invalidate(TRUE);
}

void CCreateReport::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CWnd::PostNcDestroy();
}


void CCreateReport::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PreventOutScreen(TRUE);
	located=FALSE;
	Invalidate();UpdateWindow();
	CWnd::OnRButtonDown(nFlags, point);
}
