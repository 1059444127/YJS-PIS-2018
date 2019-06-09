#if !defined(_CANNEDTEXTTREE_H_)
#define _CANNEDTEXTTREE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CannedTextTree.h : header file
//

#include <vector>
#include "MaterialGrid.h"

class CCannedText
{
public:
	CCannedText();
	~CCannedText();
public:
	CString m_sBodyPartName;
	CString m_sDisceaseName;
	CString m_sCannedText;
};


class CCannedTextTree : public CXTTreeCtrl
{
	DECLARE_DYNAMIC(CCannedTextTree)
public:
	CCannedTextTree();
	~CCannedTextTree();
	void Initialize(int nMode, UINT nEditCtrlID, CWnd* pDlgWnd, CMaterialGrid *pGridWnd = NULL);
	void BuildTree();
protected:
	int		m_nMode;
	UINT	m_nEditCtrlID;
	CWnd	*m_pEditCtrlDlgWnd;
	CMaterialGrid  *m_pMaterialListWnd;

	BOOL	m_bHideEmptyNode;
private:
	void GetSampleBodyPartsInfo(CString &sInfo);
	void GetBodyPartsList(CStringArray &list);
	void GetCannedText(std::vector<CCannedText *> &lst);
	std::vector <CCannedText *>::iterator FindFirstItemInCannedTextList(std::vector<CCannedText *> &lst, CString sBodyPartName);
	void TransferCannedText2Editor(HTREEITEM hItem);
	void UpdateUGGrid();
	void CollapseAllItemsBut(HTREEITEM hItem);
	const char *GetTitle();
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);
	afx_msg void OnTreeRefresh();
	afx_msg void OnTreeHideEmptyNode();
	afx_msg void OnTreeAddBodyPart();
	afx_msg void OnTreeDelBodyPart();
	afx_msg void OnTreeAddDiscease();
	afx_msg void OnTreeDelDiscease();
	afx_msg void OnTreeAddSample();
	afx_msg void OnTreeDelSample();
	afx_msg void OnTreeUpdateSample();
	afx_msg void OnTreeMoveUpBodyPart();
	afx_msg void OnTreeMoveDnBodyPart();
	afx_msg void OnTreePickupCannedText();	
};

#endif