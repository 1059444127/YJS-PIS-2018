// TimeQuery.cpp : implementation file
//

#include "stdafx.h"
#include "Pathology.h"
#include "TimeQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTimeQuery dialog
extern CPathologyApp theApp;

CTimeQuery::CTimeQuery(CWnd* pParent /*=NULL*/)
	: CDialog(CTimeQuery::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTimeQuery)
	m_enddate = COleDateTime::GetCurrentTime();
	m_endtime = COleDateTime::GetCurrentTime();
	m_startdate = COleDateTime::GetCurrentTime();
	m_starttime = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
}


void CTimeQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimeQuery)
	DDX_DateTimeCtrl(pDX, IDC_DT_ENDDATE, m_enddate);
	DDX_DateTimeCtrl(pDX, IDC_DT_ENDTIME, m_endtime);
	DDX_DateTimeCtrl(pDX, IDC_DT_STARTDATE, m_startdate);
	DDX_DateTimeCtrl(pDX, IDC_DT_STARTTIME, m_starttime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTimeQuery, CDialog)
	//{{AFX_MSG_MAP(CTimeQuery)
	ON_BN_CLICKED(IDC_BUTTON_TODAY, OnButtonToday)
	ON_BN_CLICKED(IDC_BUTTON_YESTERDAY, OnButtonYesterday)
	ON_BN_CLICKED(IDC_BUTTON_THREE, OnButtonThree)
	ON_BN_CLICKED(IDC_BUTTON_WEEK, OnButtonWeek)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimeQuery message handlers

void CTimeQuery::OnButtonToday() 
{
	COleDateTime curt = theApp.GetOleServerDateTime();
	m_startdate.SetDate(curt.GetYear(), curt.GetMonth(), curt.GetDay());
	m_starttime.SetTime(0,0,0);
	m_enddate.SetDate(curt.GetYear(), curt.GetMonth(), curt.GetDay());
	m_endtime.SetTime(23,59,59);

	UpdateData(FALSE);
}

void CTimeQuery::OnButtonYesterday() 
{
	COleDateTime curt = theApp.GetOleServerDateTime();
	COleDateTimeSpan tspan;
	tspan.SetDateTimeSpan(1, 0, 0, 0);
	curt -= tspan;
	m_startdate.SetDate(curt.GetYear(), curt.GetMonth(), curt.GetDay());
	m_starttime.SetTime(0,0,0);
	m_enddate.SetDate(curt.GetYear(), curt.GetMonth(), curt.GetDay());
	m_endtime.SetTime(23,59,59);
	
	UpdateData(FALSE);
}

void CTimeQuery::OnButtonThree() 
{
	COleDateTime curt = theApp.GetOleServerDateTime();
	COleDateTimeSpan tspan;
	tspan.SetDateTimeSpan(3, 0, 0, 0);
	curt -= tspan;
	m_startdate.SetDate(curt.GetYear(), curt.GetMonth(), curt.GetDay());
	m_starttime.SetTime(0,0,0);
	curt = theApp.GetOleServerDateTime();
	m_enddate.SetDate(curt.GetYear(), curt.GetMonth(), curt.GetDay());
	m_endtime.SetTime(23,59,59);
	
	UpdateData(FALSE);
}

void CTimeQuery::OnButtonWeek() 
{
	COleDateTime curt = theApp.GetOleServerDateTime();
	COleDateTimeSpan tspan;
	tspan.SetDateTimeSpan(7, 0, 0, 0);
	curt -= tspan;
	m_startdate.SetDate(curt.GetYear(), curt.GetMonth(), curt.GetDay());
	m_starttime.SetTime(0,0,0);
	curt = theApp.GetOleServerDateTime();
	m_enddate.SetDate(curt.GetYear(), curt.GetMonth(), curt.GetDay());
	m_endtime.SetTime(23,59,59);
	
	UpdateData(FALSE);
	/*COleDateTime curt = theApp.GetOleServerDateTime();
	COleDateTimeSpan tspan;
	int dayindex = curt.GetDayOfWeek() - 1;
	if(dayindex == 0)  dayindex = 7;
	
	tspan.SetDateTimeSpan(dayindex - 1, 0, 0, 0);
	
	COleDateTime uptime,downtime;
	uptime   = curt - tspan;
	downtime = curt;

	m_startdate.SetDate(uptime.GetYear(), uptime.GetMonth(), uptime.GetDay());
	m_starttime.SetTime(0,0,0);
	curt = theApp.GetOleServerDateTime();
	m_enddate.SetDate(downtime.GetYear(), downtime.GetMonth(), downtime.GetDay());
	m_endtime.SetTime(23,59,59);
	
	UpdateData(FALSE);*/
}
