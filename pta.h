// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// IiTrusPTA wrapper class

class IiTrusPTA : public COleDispatchDriver
{
public:
	IiTrusPTA() {}		// Calls COleDispatchDriver default constructor
	IiTrusPTA(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IiTrusPTA(const IiTrusPTA& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetFilter();
	LPDISPATCH GetMyCertificates();
	LPDISPATCH GetRootCertificates();
	LPDISPATCH GetCACertificates();
	LPDISPATCH GetOtherCertificates();
	CString DecryptMessage(LPCTSTR p7msg, long opt);
	LPDISPATCH VerifySignature(LPCTSTR msg, LPCTSTR sign, long opt);
};
/////////////////////////////////////////////////////////////////////////////
// ICertFilter wrapper class

class ICertFilter : public COleDispatchDriver
{
public:
	ICertFilter() {}		// Calls COleDispatchDriver default constructor
	ICertFilter(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ICertFilter(const ICertFilter& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void Clear();
	CString GetSubject();
	void SetSubject(LPCTSTR lpszNewValue);
	CString GetIssuer();
	void SetIssuer(LPCTSTR lpszNewValue);
	CString GetSerialNumber();
	void SetSerialNumber(LPCTSTR lpszNewValue);
	CString GetCsp();
	void SetCsp(LPCTSTR lpszNewValue);
	long GetUseLocalMachineCertStore();
	void SetUseLocalMachineCertStore(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// ICertificates wrapper class

class ICertificates : public COleDispatchDriver
{
public:
	ICertificates() {}		// Calls COleDispatchDriver default constructor
	ICertificates(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ICertificates(const ICertificates& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPUNKNOWN Get_NewEnum();
	VARIANT GetItem(long idx);
	long GetCount();
	void Add(LPUNKNOWN cert);
	void Remove(long idx);
	CString EncryptMessage(LPCTSTR msg, long opt);
};
/////////////////////////////////////////////////////////////////////////////
// ICertificate wrapper class

class ICertificate : public COleDispatchDriver
{
public:
	ICertificate() {}		// Calls COleDispatchDriver default constructor
	ICertificate(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ICertificate(const ICertificate& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString GetCommonName();
	CString GetSubject();
	CString GetIssuer();
	CString GetCsp();
	CString GetKeyContainer();
	CString GetSerialNumber();
	DATE GetValidFrom();
	DATE GetValidTo();
	long GetKeyUsage();
	CString SignMessage(LPCTSTR msg, long opt);
	void CreateFromStream(LPCTSTR stream);
	CString GetEncodedCert(long enc);
	long Verify(long checkCRL);
};
