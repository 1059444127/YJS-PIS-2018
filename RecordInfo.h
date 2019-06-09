// RecordInfo.h: interface for the CRecordInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECORDINFO_H__167A8341_A65F_4413_AD10_2C07ECC98FE6__INCLUDED_)
#define AFX_RECORDINFO_H__167A8341_A65F_4413_AD10_2C07ECC98FE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRecordInfo : public CUGCtrl  
{
public:
	CRecordInfo();
	~CRecordInfo();

	virtual void OnSetup();
	virtual void OnRowChange(long oldrow,long newrow);
	virtual void OnMenuCommand(int col,long row,int section,int item);
	virtual void OnKeyDown(UINT *vcKey,BOOL processed);
	virtual int  OnEditVerify(int col,long row,CWnd *edit, UINT *vcKey);

public:
	CFont m_font;
};

#endif // !defined(AFX_RECORDINFO_H__167A8341_A65F_4413_AD10_2C07ECC98FE6__INCLUDED_)
