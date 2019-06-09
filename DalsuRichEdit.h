#if !defined(AFX_DALSURICHEDIT_H__6081508D_AA8B_11D2_9D7B_00207415044C__INCLUDED_)
#define AFX_DALSURICHEDIT_H__6081508D_AA8B_11D2_9D7B_00207415044C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DalsuRichEdit.h : header file (In Korean Version)...
//
/*************************************************************************
	
	CDalsuRichEdit에 대하여
	사용목적:일반적으로 chatter를 구현할 경우, 각각의 메시지는 아래로부터
			 추가되어야 하며, 모두가 뷰의 범위를 넘을 경우, 자동적으로 한 
			 칸씩 아래로 스크롤되어야 한다.
			 또한 대화자의 이름과 대화내용, chatter에서 추가되는 메시지는 
			 구분된다.
	함수에 대한 설명:
			 1. AddName(CString &strName, COLORREF &crColor);
				이름과 이름의 색깔을 지정하면된다.
				단, 이름을 만들때, 일정한 간격을 유지하도록 하면 더욱 좋다.
				(예제 참조)

			 2. AddText(CString &strTextIn, COLORREF &crColor);
				추가할 text와 text의 색깔을 지정한다.
				단, 반드시 text의 끝에는 '\r\n'이 지정되어 있어야 한다. 그래야
				다음글이 다음줄로 이어진다.

			 3. AddMsg(CString &strMsg, COLORREF &crColor, BOOL bUnderLine, BOOL bBold);
				추가할 메시지와 메시지의 색깔, 밑줄의 유무, 굵은글자 등을 지정할 수 있다.
				마찬가지로 끝에는 '\r\n'이 지정되어 있어야 한다.

  To Use the CDalsuRichEdit...
  1. Copy "DalsuRichEdit.h", "DalsuRichEdit.cpp" in your work directory
  2. Add your project these two files
  3. Add a member data ex:m_pDalRich or m_pDalRich
  4. Create the CDalRichEdit's Window with appropriate style

  The Description of member function
  1. AddName(CString &strName, COLORREF &crColor);
		Specify the User Name and its color

  2. AddText(CString &strTextIn, COLORREF &crColor);
		Specify text string and its color
		Don't forget! There are "\r\n" characters on the tail of this string.

  3. AddMsg(CString &strMsg, COLORREF &crColor, BOOL bUnderLine, BOOL bBold);
		It's general system message. ex:A User joined this room...
		In the same manner, there are "\r\n" characters on the tail of this string.

and I thank for Zafir Anjum, Tom Moor, Steve Dunn, Juraj Rojko 's codes...
  
***************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// CDalsuRichEdit window

class CDalsuRichEdit : public CRichEditCtrl
{
	DECLARE_DYNCREATE(CDalsuRichEdit)

// Construction
public:
	CDalsuRichEdit();

// Attributes
public:

// Operations
public:
	void AddText(LPCTSTR szTextIn, COLORREF &crNewColor);
	void AddText(CString &strTextIn, COLORREF &crNewColor);
	void AddName(CString &strName, COLORREF &crNewColor);
	void AddMsg(CString &strMSG, COLORREF &crNewColor, BOOL bUnderLine = FALSE, BOOL bBold = FALSE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDalsuRichEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDalsuRichEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDalsuRichEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DALSURICHEDIT_H__6081508D_AA8B_11D2_9D7B_00207415044C__INCLUDED_)
