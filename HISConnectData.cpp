//---------------------------------------------------------------------------

#include "stdafx.h"

#include <string>
#include <stdio.h>

#include "HISConnectData.h"
#include "Markup.h"

//---------------------------------------------------------------------------

CHISConnectData::CHISConnectData()
{
    Clear();
	m_sSystemCode = "";
	m_sSystemName = "";
	m_sUserID = "";
	m_sUserName = "";
	m_sHospitalID = "";
	m_sHospitalName = "";
}

CHISConnectData::~CHISConnectData()
{
    m_sXMLDoc = "";
}


void CHISConnectData::Clear()
{
	m_sStudyGUID = "";
    m_sStudyID = "";
    m_sStudyUID = "";
	m_sSeriesUID = "";
    m_sModality = "";
	
	m_sAccNo	= "";
	m_sStudyDate = "";
	m_sStudyTime = "";

    m_sPatientID = "";
    m_sName = "";
    m_sSex = "";
    m_nAge = 0;

	m_sDOB	   = "";
    m_sAgeUnit = "";
    m_sRefDept = "";
    m_sRefDoctor = "";

	m_sSampleName= "";

    m_sOrderType = "";
    m_sOrderID = "";
	m_sOrderNo = "";

    m_sOrderItems = "";
    m_sClinicDiag = "";
    m_sHistory = "";
    m_sInpatientNo = "";
    m_sOutpatientNo = "";
	m_sTelNo	= "";
	m_sAddress	= "";

    m_sZoneName = "";
    m_sBedNo = "";
    m_sAnalysis = "";
    m_sDiagnosis = "";
    m_sDoctorName = "";
    m_sReportDate = "";
	m_sReportTime = "";
	
	m_sReviewDate = "";
	m_sReviewTime = "";

	m_sReviewDoctorName = "";

	m_fFee	= 0.0;

	m_nPositiveFlag = 0;
	m_nSendTimes = 0;
}

void CHISConnectData::DecodeFromXMLDoc(const char *xml)
{
    CMarkup xmlDoc;
    std::string sAge,sFee;

    xmlDoc.SetDoc(xml);

    Clear();

    if (xmlDoc.FindChildElem("MedicalOrder"))
    {
        xmlDoc.IntoElem();

		if (xmlDoc.FindChildElem("SYSTEMCODE"))
			m_sSystemCode = xmlDoc.GetChildData();

        xmlDoc.ResetChildPos();

        if (xmlDoc.FindChildElem("STUDYID"))
            m_sStudyID = xmlDoc.GetChildData();

        xmlDoc.ResetChildPos();

        if (xmlDoc.FindChildElem("STUDYUID"))
            m_sStudyUID = xmlDoc.GetChildData();

        xmlDoc.ResetChildPos();

        if (xmlDoc.FindChildElem("SERIESUID"))
            m_sSeriesUID = xmlDoc.GetChildData();

        xmlDoc.ResetChildPos();

        if (xmlDoc.FindChildElem("MODALITY"))
            m_sModality = xmlDoc.GetChildData();

        xmlDoc.ResetChildPos();

		if (xmlDoc.FindChildElem("ACCNO"))
			m_sAccNo	= xmlDoc.GetChildData();

		xmlDoc.ResetChildPos();
			
		if (xmlDoc.FindChildElem("STUDYDATE"))
			m_sStudyDate = xmlDoc.GetChildData();

		xmlDoc.ResetChildPos();

		if (xmlDoc.FindChildElem("STUDYTIME"))
			m_sStudyTime = xmlDoc.GetChildData();

		xmlDoc.ResetChildPos();

        if (xmlDoc.FindChildElem("PATIENTID"))
            m_sPatientID = xmlDoc.GetChildData();

        xmlDoc.ResetChildPos();

        if (xmlDoc.FindChildElem("ORDERITEMS"))
            m_sOrderItems = xmlDoc.GetChildData();

        xmlDoc.ResetChildPos();

        if (xmlDoc.FindChildElem("NAME"))
            m_sName = xmlDoc.GetChildData();

       xmlDoc.ResetChildPos();

        if (xmlDoc.FindChildElem("SEX"))
            m_sSex = xmlDoc.GetChildData();

         xmlDoc.ResetChildPos();

        if (xmlDoc.FindChildElem("AGE"))
        {
            sAge = xmlDoc.GetChildData();
            m_nAge = atoi(sAge.c_str());
        }

        xmlDoc.ResetChildPos();

        if (xmlDoc.FindChildElem("AGEUNIT"))
            m_sAgeUnit = xmlDoc.GetChildData();

        xmlDoc.ResetChildPos();

		if (xmlDoc.FindChildElem("AGEWITHUNIT"))
			m_sAgeWithUnit = xmlDoc.GetChildData();

		xmlDoc.ResetChildPos();
		
		if (xmlDoc.FindChildElem("DOB"))
			m_sDOB = xmlDoc.GetChildData();

		xmlDoc.ResetChildPos();

		if (xmlDoc.FindChildElem("REFDEPT"))
            m_sRefDept = xmlDoc.GetChildData();

        xmlDoc.ResetChildPos();

        if (xmlDoc.FindChildElem("REFDOCTOR"))
            m_sRefDoctor = xmlDoc.GetChildData();

        xmlDoc.ResetChildPos();

        if (xmlDoc.FindChildElem("CLINICDIAG"))
            m_sClinicDiag = xmlDoc.GetChildData();

		xmlDoc.ResetChildPos();

		if (xmlDoc.FindChildElem("SAMPLENAME"))
			m_sSampleName = xmlDoc.GetChildData();

        xmlDoc.ResetChildPos();

		if (xmlDoc.FindChildElem("INPATIENTNO"))
			m_sInpatientNo = xmlDoc.GetChildData();

		xmlDoc.ResetChildPos();

		if (xmlDoc.FindChildElem("OUTPATIENTNO"))
			m_sOutpatientNo = xmlDoc.GetChildData();
		
		xmlDoc.ResetChildPos();

		if (xmlDoc.FindChildElem("TELNO"))
			m_sTelNo = xmlDoc.GetChildData();

		xmlDoc.ResetChildPos();

		if (xmlDoc.FindChildElem("ADDRESS"))
			m_sAddress = xmlDoc.GetChildData();

		xmlDoc.ResetChildPos();

        if (xmlDoc.FindChildElem("BEDNO"))
            m_sBedNo = xmlDoc.GetChildData();

        xmlDoc.ResetChildPos();

        if (xmlDoc.FindChildElem("ZONENAME"))
            m_sZoneName = xmlDoc.GetChildData();

        xmlDoc.ResetChildPos();

        if (xmlDoc.FindChildElem("ORDERID"))
            m_sOrderID = xmlDoc.GetChildData();

        xmlDoc.ResetChildPos();

		if (xmlDoc.FindChildElem("ORDERNO"))
			m_sOrderNo	= xmlDoc.GetChildData();

		xmlDoc.ResetChildPos();

        if (xmlDoc.FindChildElem("ORDERTYPE"))
            m_sOrderType = xmlDoc.GetChildData();

        xmlDoc.ResetChildPos();

        if (xmlDoc.FindChildElem("HISTORY"))
            m_sHistory = xmlDoc.GetChildData();

        xmlDoc.ResetChildPos();

        if (xmlDoc.FindChildElem("ANALYSIS"))
            m_sAnalysis = xmlDoc.GetChildData();

        xmlDoc.ResetChildPos();

        if (xmlDoc.FindChildElem("DIAGNOSIS"))
            m_sDiagnosis = xmlDoc.GetChildData();

        xmlDoc.ResetChildPos();


        if (xmlDoc.FindChildElem("DOCTORNAME"))
            m_sDoctorName = xmlDoc.GetChildData();

        xmlDoc.ResetChildPos();

        if (xmlDoc.FindChildElem("REPORTDATE"))
            m_sReportDate = xmlDoc.GetChildData();

        xmlDoc.ResetChildPos();

		if (xmlDoc.FindChildElem("REPORTTIME"))
			m_sReportTime = xmlDoc.GetChildData();

		xmlDoc.ResetChildPos();

		if (xmlDoc.FindChildElem("FEE"))
		{
			sFee = xmlDoc.GetChildData();
			m_fFee = atof(sFee.c_str());
		}	
    }
}

void CHISConnectData::DecodeFromXMLDoc_LX(const char *szXML)
{
    CMarkup xml;
	CString sValue("");
	int nPos;
  
    Clear();

	xml.SetDoc(szXML);
	xml.ResetPos();

	// XML : <XISLINK><MedicalOrder>.....<MedicalOrder/><XISLINK/>

	if (xml.FindElem("XISLink"))
	{
		xml.IntoElem();

		if (xml.FindChildElem("OrderNo")) {
			m_sOrderNo = xml.GetChildData();
			m_sOrderID = m_sOrderNo;
		}
		xml.ResetChildPos();

		if (xml.FindChildElem("PatientID"))
			m_sPatientID = xml.GetChildData();
		xml.ResetChildPos();

		if (xml.FindChildElem("PatientName"))
			m_sName = xml.GetChildData();
		xml.ResetChildPos();

		if (xml.FindChildElem("PatientSex"))
			m_sSex = xml.GetChildData();
		xml.ResetChildPos();

		if (xml.FindChildElem("PatientAge")) {
			m_nAge = atoi(xml.GetChildData());
		}
		xml.ResetChildPos();

		if (xml.FindChildElem("PatientAgeUnit")) {
			m_sAgeUnit = xml.GetChildData();
			if (m_sAgeUnit == "H")
				m_sAgeUnit = "时";
			else if (m_sAgeUnit == "D")
				m_sAgeUnit = "天";
			else if (m_sAgeUnit == "M")
				m_sAgeUnit = "月";
			else if (m_sAgeUnit == "Y")
				m_sAgeUnit = "岁";
		}

		xml.ResetChildPos();

		if (xml.FindChildElem("OrderDetails"))
			m_sOrderItems = xml.GetChildData();
		xml.ResetChildPos();
		
		if (xml.FindChildElem("PatientDOB")) {
			m_sDOB = xml.GetChildData();
			if (m_sDOB.GetLength() > 10)
				m_sDOB = m_sDOB.Mid(0,10);
		}

		xml.ResetChildPos();

		if (xml.FindChildElem("RefDeptName"))
            m_sRefDept = xml.GetChildData();

        xml.ResetChildPos();

        if (xml.FindChildElem("RefDoctorName"))
            m_sRefDoctor = xml.GetChildData();

        xml.ResetChildPos();

        if (xml.FindChildElem("ClinicDiagDesc"))
            m_sClinicDiag = xml.GetChildData();

		xml.ResetChildPos();

		if (xml.FindChildElem("SampleName"))
			m_sSampleName = xml.GetChildData();

        xml.ResetChildPos();

		if (xml.FindChildElem("InpatientNo"))
			m_sInpatientNo = xml.GetChildData();

		xml.ResetChildPos();

		if (xml.FindChildElem("OutpatientNo"))
			m_sOutpatientNo = xml.GetChildData();
		
		xml.ResetChildPos();

		if (xml.FindChildElem("TelephoneNo"))
			m_sTelNo = xml.GetChildData();

		xml.ResetChildPos();

		if (xml.FindChildElem("Address"))
			m_sAddress = xml.GetChildData();

		xml.ResetChildPos();

        if (xml.FindChildElem("BedNo"))
            m_sBedNo = xml.GetChildData();

        xml.ResetChildPos();

        if (xml.FindChildElem("Ward"))
            m_sZoneName = xml.GetChildData();

        xml.ResetChildPos();

        if (xml.FindChildElem("OrderTypeName"))
            m_sOrderType = xml.GetChildData();

        xml.ResetChildPos();

		if (xml.FindChildElem("HISItemName")) {
			sValue = xml.GetChildData();
			nPos = sValue.Find("^");
			if (nPos >= 0) 
				m_sSampleName = sValue.Mid(nPos + 1);
		}

		xml.ResetChildPos();


        if (xml.FindChildElem("History"))
            m_sHistory = xml.GetChildData();

        xml.ResetChildPos();

 
		if (xml.FindChildElem("AmountFee"))
		{
			sValue = xml.GetChildData();
			m_fFee = atof(sValue);
		}	
    }
}


const char *CHISConnectData::EncodeToXMLDoc()
{
    CMarkup xmlDoc;
    char szAge[20];


    xmlDoc.InsertElem("HISConnect");
    xmlDoc.InsertChildElem("MedicalOrder");
    xmlDoc.IntoElem();

	xmlDoc.AddChildElem("SYSTEMCODE",	m_sSystemCode);

    xmlDoc.AddChildElem("STUDYID",		m_sStudyID);
    xmlDoc.AddChildElem("STUDYUID",		m_sStudyUID);
	xmlDoc.AddChildElem("SERIESUID",	m_sSeriesUID);
    xmlDoc.AddChildElem("MODALITY",		m_sModality);
	xmlDoc.AddChildElem("ACCNO",		m_sAccNo);
	xmlDoc.AddChildElem("STUDYDATE",	m_sStudyDate);
	xmlDoc.AddChildElem("STUDYTIME",	m_sStudyTime);

    xmlDoc.AddChildElem("PATIENTID",	m_sPatientID);
    xmlDoc.AddChildElem("ORDERITEMS",	m_sOrderItems);
    xmlDoc.AddChildElem("NAME",			m_sName);
    xmlDoc.AddChildElem("SEX",			m_sSex);

    sprintf(szAge, "%d", m_nAge);
    
	xmlDoc.AddChildElem("AGE",			szAge);
    xmlDoc.AddChildElem("AGEUNIT",		m_sAgeUnit);
	xmlDoc.AddChildElem("AGEWITHUNIT",	m_sAgeWithUnit);
	xmlDoc.AddChildElem("DOB",			m_sDOB);
    xmlDoc.AddChildElem("REFDEPT",		m_sRefDept);
    xmlDoc.AddChildElem("REFDOCTOR",	m_sRefDoctor);

	xmlDoc.AddChildElem("SAMPLENAME",	m_sSampleName);


    xmlDoc.AddChildElem("CLINICDIAG",	m_sClinicDiag);
	xmlDoc.AddChildElem("TELNO",		m_sTelNo);
	xmlDoc.AddChildElem("ADDRESS",		m_sAddress);
    xmlDoc.AddChildElem("BEDNO",		m_sBedNo);
    xmlDoc.AddChildElem("ZONENAME",		m_sZoneName);
    xmlDoc.AddChildElem("ORDERID",		m_sOrderID);
	xmlDoc.AddChildElem("ORDERNO",		m_sOrderNo);
    xmlDoc.AddChildElem("ORDERTYPE",	m_sOrderType);
    xmlDoc.AddChildElem("HISTORY",		m_sHistory);
    xmlDoc.AddChildElem("ANALYSIS",		m_sAnalysis);
    xmlDoc.AddChildElem("DIAGNOSIS",	m_sDiagnosis);
    xmlDoc.AddChildElem("DOCTORNAME",	m_sDoctorName);
    xmlDoc.AddChildElem("REPORTDATE",	m_sReportDate);
	xmlDoc.AddChildElem("REPORTTIME",	m_sReportTime);

    m_sXMLDoc = xmlDoc.GetDoc();

    return m_sXMLDoc;
}


const char *CHISConnectData::EncodeToXMLDoc_LX1()
{
    CMarkup xml;
	CString sOrderType(""); 

	if (m_sOrderType == "MZ")
		sOrderType = "门诊";
	else if (m_sOrderType == "ZY")
		sOrderType = "住院";
	else if (m_sOrderType == "JZ")
		sOrderType = "急诊";
	else if (m_sOrderType == "TJ")
		sOrderType = "体检";
	else
		sOrderType = m_sOrderType;

	xml.SetDoc("");
	xml.InsertElem("XISLink");
	xml.InsertChildElem("MedicalExam");
	xml.IntoElem();

	xml.AddChildElem("StudyGUID",		m_sStudyGUID);
	xml.AddChildElem("SystemCode",      m_sSystemCode);
	xml.AddChildElem("StudyUID",        m_sStudyUID);
	xml.AddChildElem("OrderNo",         m_sOrderNo);
	xml.AddChildElem("OrderType",       sOrderType);
	
	m_sXMLDoc = xml.GetDoc();

	return m_sXMLDoc;
}

const char *CHISConnectData::EncodeToXMLDoc_LX2()
{
    CMarkup xml;
	CString sOrderType(""), sAgeWithUnit(""); 

	if (m_sOrderType == "MZ")
		sOrderType = "门诊";
	else if (m_sOrderType == "ZY")
		sOrderType = "住院";
	else if (m_sOrderType == "JZ")
		sOrderType = "急诊";
	else if (m_sOrderType == "TJ")
		sOrderType = "体检";
	else
		sOrderType = m_sOrderType;

	xml.SetDoc("");
	xml.InsertElem("XISLink");
	xml.InsertChildElem("MedicalExam");
	xml.IntoElem();

	xml.AddChildElem("StudyGUID",		m_sStudyGUID);
	xml.AddChildElem("SystemCode",      m_sSystemCode);
	xml.AddChildElem("SystemName",		m_sSystemName);
	xml.AddChildElem("HosptialID",		m_sHospitalID);
	xml.AddChildElem("HospitalName",	m_sHospitalName);

	xml.AddChildElem("StudyID",			m_sStudyID);
	xml.AddChildElem("StudyUID",        m_sStudyUID);
	xml.AddChildElem("OrderNo",         m_sOrderNo);
	xml.AddChildElem("OrderType",       sOrderType);
	xml.AddChildElem("InpatientNo",		m_sInpatientNo);
	xml.AddChildElem("OutpatientNo",	m_sOutpatientNo);

	xml.AddChildElem("AccessonNo",		m_sAccNo);
	
	xml.AddChildElem("PatientID",		m_sPatientID);
	xml.AddChildElem("PatientName",		m_sName);
	xml.AddChildElem("PatientSex",		m_sSex);
	xml.AddChildElem("PatientDOB",		m_sDOB);

	if (m_nAge > 0)
		sAgeWithUnit.Format("%d%s",		m_nAge, m_sAgeUnit);
	xml.AddChildElem("PatientAge",		sAgeWithUnit);

	xml.AddChildElem("ReportDoctorID",  m_sDoctorName);
	xml.AddChildElem("ReportDoctorName", m_sDoctorName);

	xml.AddChildElem("ReviewDoctorID",	m_sReviewDoctorName);
	xml.AddChildElem("ReviewDoctorName", m_sReviewDoctorName);

	xml.AddChildElem("StudyDate", m_sStudyDate);
	xml.AddChildElem("StudyTime", m_sStudyTime);

	xml.AddChildElem("ReportDate", m_sReportDate);
	xml.AddChildElem("ReportTime", m_sReportTime);

	xml.AddChildElem("ReviewDate", m_sReviewDate);
	xml.AddChildElem("ReviewTime", m_sReviewTime);

	xml.AddChildElem("Findings", m_sAnalysis);
	xml.AddChildElem("Conclusions", m_sDiagnosis);

	xml.AddChildElem("PositiveFlag", m_nPositiveFlag);

	xml.AddChildElem("DoctorEmpNo",	m_sDoctorName);
	xml.AddChildElem("DoctorName", m_sDoctorName);

	xml.AddChildElem("FromUserID", m_sUserID);
	xml.AddChildElem("FromUserName", m_sUserName);

	xml.AddChildElem("SendTimes", m_nSendTimes);


	m_sXMLDoc = xml.GetDoc();

	return m_sXMLDoc;
}
