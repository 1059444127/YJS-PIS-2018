#if !defined(AFX_PROMPTINFO_H__08DD9CF3_E0AF_47E1_B74F_B7632F5D1482__INCLUDED_)
#define AFX_PROMPTINFO_H__08DD9CF3_E0AF_47E1_B74F_B7632F5D1482__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PromptInfo.h : header file
//

class CPromptInfo : public CUGCtrl  
{
public:
	CPromptInfo();
	~CPromptInfo();

	virtual void OnSetup();
	virtual void OnRowChange(long oldrow,long newrow);
	virtual void OnMenuCommand(int col,long row,int section,int item);

public:
	CFont m_font;
	int m_Type;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROMPTINFO_H__08DD9CF3_E0AF_47E1_B74F_B7632F5D1482__INCLUDED_)
