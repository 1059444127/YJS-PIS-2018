// Reporting.h: interface for the CReporting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REPORTING_H__DCEB697B_C86B_45AD_811F_9FA4DFBFFF1C__INCLUDED_)
#define AFX_REPORTING_H__DCEB697B_C86B_45AD_811F_9FA4DFBFFF1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ReportingFrame.h"
#include "ReportingView.h"
#include "FormDataModel.h"

class CReporting  
{
public:
	CReporting(CString MedicalNo);
	virtual ~CReporting();
	CFormDataModel  *m_pFormDataModel;

	CReportingFrame * m_pReportingFrame;
	CReportingView  * m_pReportingView;
};

#endif // !defined(AFX_REPORTING_H__DCEB697B_C86B_45AD_811F_9FA4DFBFFF1C__INCLUDED_)
