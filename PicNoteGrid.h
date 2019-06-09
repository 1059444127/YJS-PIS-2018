// PicNoteGrid.h: interface for the CPicNoteGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICNOTEGRID_H__7DDF75CD_7CB9_4DBF_9E1A_9B482440A576__INCLUDED_)
#define AFX_PICNOTEGRID_H__7DDF75CD_7CB9_4DBF_9E1A_9B482440A576__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPicNoteGrid : public CUGCtrl  
{
public:
	virtual void OnMenuCommand(int col,long row,int section,int item);
	virtual void OnGetCell(int col,long row,CUGCell * cell);
	virtual void OnSetup();
	CPicNoteGrid();
	virtual ~CPicNoteGrid();

};

#endif // !defined(AFX_PICNOTEGRID_H__7DDF75CD_7CB9_4DBF_9E1A_9B482440A576__INCLUDED_)
