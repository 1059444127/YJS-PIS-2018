// RecordInput.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "RecordInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecordInput dialog

CRecordInput::CRecordInput(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CRecordInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecordInput)
	//}}AFX_DATA_INIT
	m_FieldName.RemoveAll();
	m_FieldList.RemoveAll();
	m_NameList.RemoveAll();
	m_IsFirstTimePaint = true;
}


void CRecordInput::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecordInput)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecordInput, CXTResizeDialog)
	//{{AFX_MSG_MAP(CRecordInput)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordInput message handlers

BOOL CRecordInput::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_PromptInfo_Ctrl_First.m_Type  = 1;
	m_PromptInfo_Ctrl_Second.m_Type = 2;
	
	m_RecordInfo_Ctrl.AttachGrid(this,IDC_STATIC_FIELD);
	m_RecordInfo_Ctrl.SetParent(this);
	
	m_PromptInfo_Ctrl_First.AttachGrid(this,IDC_STATIC_PROMPT1);
	m_PromptInfo_Ctrl_First.SetParent(this);
	
	m_PromptInfo_Ctrl_Second.AttachGrid(this,IDC_STATIC_PROMPT2);
	m_PromptInfo_Ctrl_Second.SetParent(this);

	CRect rect,rt1,rt2,rt3;
	GetWindowRect(&rect);

	GetDlgItem(IDC_STATIC_FIELD)->GetWindowRect(&rt1);
	GetDlgItem(IDC_STATIC_PROMPT1)->GetWindowRect(&rt2);
	GetDlgItem(IDC_STATIC_PROMPT2)->GetWindowRect(&rt3);

	SetResize(IDC_STATIC_FIELD, CXTResizePoint( 0, 0), CXTResizePoint( rt1.right*1.0 / rect.Width(), 1));
	SetResize(IDC_STATIC_PROMPT1, CXTResizePoint( rt2.left*1.0 / rect.Width(), 0), CXTResizePoint( rt2.right*1.0 / rect.Width(), 1));
	SetResize(IDC_STATIC_PROMPT2, CXTResizePoint( rt3.left*1.0 / rect.Width(), 0), CXTResizePoint( 1, 1));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CRecordInput::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
    if (pMsg->wParam == VK_RETURN ||
		pMsg->wParam == VK_ESCAPE)
    {
		// don't allow dialog to process return or escape keys.
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);

        return TRUE;
    }

	if ( pMsg->wParam == VK_TAB )
	{
		m_RecordInfo_Ctrl.SetFocus();

		if(m_RecordInfo_Ctrl.GetCurrentRow() >= 0)
		{
			m_RecordInfo_Ctrl.OnRowChange(m_RecordInfo_Ctrl.GetCurrentRow(),m_RecordInfo_Ctrl.GetCurrentRow());
		}
	}
	
	return CXTResizeDialog::PreTranslateMessage(pMsg);
}

void CRecordInput::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if( m_IsFirstTimePaint )
	{
		m_IsFirstTimePaint = false;
		m_RecordInfo_Ctrl.SetFocus();

		if(m_RecordInfo_Ctrl.GetCurrentRow() >= 0)
		{
			m_RecordInfo_Ctrl.GotoRow(1);
			m_RecordInfo_Ctrl.OnRowChange(m_RecordInfo_Ctrl.GetCurrentRow(),m_RecordInfo_Ctrl.GetCurrentRow());
		}
	}
	// Do not call CXTResizeDialog::OnPaint() for painting messages
}

void CRecordInput::OnDestroy() 
{
	CXTResizeDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_FieldName.RemoveAll();
	m_FieldList.RemoveAll();
	m_NameList.RemoveAll();
}
