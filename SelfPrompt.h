// SelfPrompt.h: interface for the CSelfPrompt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELFPROMPT_H__62B820DA_1562_47FD_83F7_8EEAE78B5FDB__INCLUDED_)
#define AFX_SELFPROMPT_H__62B820DA_1562_47FD_83F7_8EEAE78B5FDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSelfPrompt : public CUGCtrl  
{
public:
	CSelfPrompt();
	~CSelfPrompt();

	virtual void OnSetup();
	virtual void OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);
	virtual void OnRowChange(long oldrow,long newrow);
	virtual void OnMenuCommand(int col,long row,int section,int item);

public:
	CFont m_font;
	int m_Type;
};

#endif // !defined(AFX_SELFPROMPT_H__62B820DA_1562_47FD_83F7_8EEAE78B5FDB__INCLUDED_)
