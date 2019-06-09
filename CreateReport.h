// CreateReport.h: interface for the CCreateReport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CREATEREPORT_H__C73EA2AC_3D6A_4323_9E9D_887E0475B019__INCLUDED_)
#define AFX_CREATEREPORT_H__C73EA2AC_3D6A_4323_9E9D_887E0475B019__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct {
	BOOL status;
	CStringArray Text;    
	int Texty;
	CPtrArray Choice;
	CPtrArray ChoiceStatus;
	CWordArray x,y,type;
} linedata;	

class CCreateReport : public CWnd  
{
// Construction
public:
	CCreateReport();
	CCreateReport(CWnd* pParent,CWnd* p_Edit,CString *See,CString *Result);

// Attributes
public:
	CString *m_See,*m_Result;
	CWnd *m_pWnd,*m_pEdit;
	CPtrArray paSee,paResult;
	LOGFONT m_Selecttextlogfont,m_Selectchoicelogfont,m_UnSelecttextlogfont,m_UnSelectchoicelogfont;
	COLORREF m_SelecttextColor,m_SelectchoiceColor,m_UnSelecttextColor,m_UnSelectchoiceColor,m_BackColor;

	char SelectMode;
	int start_line,lHeight,cWidth,sHeight,sWidth;
	int Maxline,Pagesize;
	int segment,choice,item,segmento,choiceo,itemo;
	CBrush newBrush;
	CRect CurArea;
	BOOL located;
// Operations
public:
	BOOL Datain_sub2(char *Strbuf,CPtrArray &paData);
	BOOL Datain_sub(CString Buwei,CString Bingzhong);
	BOOL mTextOut(CDC *pDC,int &x,int &y,CString OutStr,COLORREF Color,CFont &Font);
	BOOL Cacuxy(CDC *pDC,int &x,int &y,CString OutStr);
	BOOL LocateRect(CDC* pDC);
	void CacuNumSite(CDC* pDC,CPoint &topleft);
	void Label(CDC* pDC,int x,int y,BOOL selected);
	BOOL PreventOutScreen(BOOL MoveStartline);
	void FreshLocalRect(void);
	BOOL LastSeg(void);
	BOOL NextSeg(void);
	BOOL NextChoice(void);
	BOOL LastItem(void);
	BOOL NextItem(void);
	void ChangeStatus(int type,CStringArray *ChoiceArray,CByteArray *pAChoiceStatus,int CurItem);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateReport)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCreateReport();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCreateReport)
	afx_msg void OnPaint();
	afx_msg void OnSegSelect();
	afx_msg void OnChoiceSelect();
	afx_msg void OnShow();
	afx_msg void OnAppExit();
	afx_msg void OnBackcolor();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSelectchoice();
	afx_msg void OnSelecttext();
	afx_msg void OnUnselectchoice();
	afx_msg void OnUnselecttext();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_CREATEREPORT_H__C73EA2AC_3D6A_4323_9E9D_887E0475B019__INCLUDED_)
