//---------------------------------------------------------------------------

#ifndef HISConnectDataH
#define HISConnectDataH
//---------------------------------------------------------------------------

class CHISConnectData
{
public:
    CHISConnectData();
    ~CHISConnectData();

    void DecodeFromXMLDoc(const char *xmlDoc);
    void DecodeFromXMLDoc_LX(const char *szXML);

    const char *EncodeToXMLDoc();
	const char *EncodeToXMLDoc_LX1();
	const char *EncodeToXMLDoc_LX2();
    void Clear();

	void SetSystemCode(const char *szSystemCode) { m_sSystemCode = szSystemCode;}
	void SetSystemName(const char *szSystemName) { m_sSystemName = szSystemName;}
	void SetUserID(const char *szUserID) { m_sUserID = szUserID; }
	void SetUserName(const char *szUserName) { m_sUserName = szUserName; }
	void SetHospitalID(const char *szHospitalID) { m_sHospitalID = szHospitalID; }
	void SetHospitalName(const char *szHospitalName) { m_sHospitalName = szHospitalName; }

private:
    CString m_sXMLDoc;
	CString m_sSystemCode;
	CString m_sSystemName;
	CString m_sUserID;
	CString m_sUserName;
	CString m_sHospitalID;
	CString m_sHospitalName;
	
public:
	CString m_sStudyGUID;
    CString m_sStudyID;
    CString m_sStudyUID;
    CString m_sSeriesUID;
    CString m_sModality;

	CString m_sAccNo;
    
	CString m_sStudyDate;
	CString m_sStudyTime;

    CString m_sPatientID;
    CString m_sName;
    CString m_sSex;

    int        m_nAge;

    CString m_sAgeUnit;
	CString m_sAgeWithUnit;
    CString m_sRefDept;
    CString m_sRefDoctor;

	CString	m_sSampleName;	//±ê±¾Ãû³Æ 

    CString m_sOrderType;

    CString m_sOrderID;
	CString m_sOrderNo;

	CString m_sDOB;

    CString m_sOrderItems;
    CString m_sClinicDiag;
    CString m_sHistory;
    CString m_sInpatientNo;
    CString m_sOutpatientNo;
	CString m_sTelNo;
	CString m_sAddress;
    CString m_sZoneName;
    CString m_sBedNo;
    CString m_sAnalysis;
    CString m_sDiagnosis;
    CString m_sDoctorName;
    CString m_sReportDate;
	CString m_sReportTime;

	CString m_sReviewDate;
	CString m_sReviewTime;
	CString m_sReviewDoctorName;

	int		m_nPositiveFlag;

	float	m_fFee;

	int		m_nSendTimes;

};

#endif
 