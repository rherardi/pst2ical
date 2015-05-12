// NotesProc.h: interface for the CPSTProc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PSTPROC_H__8CD32256_505C_4973_9A04_DBC9BF1C4FA2__INCLUDED_)
#define AFX_PSTPROC_H__8CD32256_505C_4973_9A04_DBC9BF1C4FA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DATEMASK_DWK_ALL			_T("DWK, DD MMM YYYY HH:MI:SS TZHH")
#define DATEMASK_DWK2				_T("DWK MMM DD HH:MI:SS YYYY")
#define DATEMASK_YYYYMMDDHHMI		_T("YYYYMMDDHHMI")
#define DATEMASK_YYYYMMDDHHMISS		_T("YYYYMMDDHHMISS")
#define DATEMASK_YYYYMMDDTHHMISSZ	_T("YYYYMMDDTHHMISSZ")

#define MODIFIER_BASE64				_T("base64")
#define MODIFIER_SMTPLOOKUP			_T("smtpLookup")
#define MODIFIER_TIMEZONENAME		_T("timezoneName")
#define MODIFIER_16BYTES			_T("16bytes")

#define PSTMASK _T("*.pst")
#define PST_DEFAULT_SUFFIX _T(".pst")
#define ICS_DEFAULT_SUFFIX _T(".ics")

#define BUFFER_SIZE 8192
#define ENTRY_LEN 256


// http://blogs.msdn.com/heaths/archive/2005/07/21/441391.aspx
#define SMTP_LOOKUP_FAILED MAKE_HRESULT( SEVERITY_ERROR, FACILITY_ITF, 0x200 + 0 )

// See here:
// http://msdn.microsoft.com/en-us/library/cc678348.aspx

// PSETID_Address GUID {00062004-0000-0000-c000-000000000046}
// Propset used for contact items
DEFINE_OLEGUID(PSETID_Address, MAKELONG(0x2000+(0x04),0x0006),0,0);

DEFINE_OLEGUID(PSETID_Common, MAKELONG(0x2000+(0x08),0x0006),0,0);

DEFINE_OLEGUID(PSETID_Calendar, MAKELONG(0x0329,0x0002),0,0);

DEFINE_OLEGUID(PSETID_Appointment, MAKELONG(0x2000+(0x02),0x0006),0,0);

DEFINE_OLEGUID(PSETID_Meeting, MAKELONG(0xDA90,0x6ED8),0,0);

//public static readonly NMapiGuid PSETID_Address = Guids.DefineOleGuid (
//			0x00062004, (short) 0x0000, (short) 0x0000);
//
//public static readonly NMapiGuid PSETID_Calendar = Guids.DefineOleGuid (
//	0x00020329, (short)0x0000, (short)0x0000);
//
//public static readonly NMapiGuid PSETID_Appointment = Guids.DefineOleGuid (
//			0x00062002, (short) 0x0000, (short) 0x0000);

#define PidLidCleanGlobalObjectId							0x0023
#define PidLidLocation										0x8208
#define PidLidAppointmentTimeZoneDefinitionStartDisplay		0x825E
#define PidLidCommonStart									0x8516
#define PidLidCommonEnd										0x8517
#define PidLidContacts										0x853A

typedef struct {
	ULONG pID;
	ULONG pType;
	LPGUID pGuid;
	_TCHAR *pName;
} NamedProperty;

// The enum is to aid in building the property values for SetProps.
// The beginning of the list must match aulContactProps. All non-named properties must come at the end.
enum {
	p_PidLidCleanGlobalObjectId,
	p_PidLidAppointmentTimeZoneDefinitionStartDisplay,
	NUM_PROPS
};

#ifndef TZDEFINITION
// TZREG
// =====================
//   This is an individual description that defines when a daylight
//   saving shift, and the return to standard time occurs, and how
//   far the shift is.  This is basically the same as
//   TIME_ZONE_INFORMATION documented in MSDN, except that the strings
//   describing the names "daylight" and "standard" time are omitted.
//
typedef struct RenTimeZone
{
    long        lBias;           // offset from GMT
    long        lStandardBias;   // offset from bias during standard time
    long        lDaylightBias;   // offset from bias during daylight time
    SYSTEMTIME  stStandardDate;  // time to switch to standard time
    SYSTEMTIME  stDaylightDate;  // time to switch to daylight time
} TZREG;

// TZRULE
// =====================
//   This structure represents both a description when a daylight. 
//   saving shift occurs, and in addition, the year in which that
//   timezone rule came into effect. 
//
typedef struct
{
    WORD        wFlags;   // indicates which rule matches legacy recur
    SYSTEMTIME  stStart;  // indicates when the rule starts
    TZREG       TZReg;    // the timezone info
} TZRULE;

const WORD TZRULE_FLAG_RECUR_CURRENT_TZREG  = 0x0001; // see dispidApptTZDefRecur
const WORD TZRULE_FLAG_EFFECTIVE_TZREG      = 0x0002;

// TZDEFINITION
// =====================
//   This represents an entire timezone including all historical, current
//   and future timezone shift rules for daylight saving time, etc.  It's
//   identified by a unique GUID.
//
typedef struct
{
    WORD     wFlags;       // indicates which fields are valid
    GUID     guidTZID;     // guid uniquely identifying this timezone
    LPWSTR   pwszKeyName;  // the name of the key for this timezone
    WORD     cRules;       // the number of timezone rules for this definition
    TZRULE*  rgRules;      // an array of rules describing when shifts occur
} TZDEFINITION;

const WORD TZDEFINITION_FLAG_VALID_GUID     = 0x0001; // the guid is valid
const WORD TZDEFINITION_FLAG_VALID_KEYNAME  = 0x0002; // the keyname is valid

const ULONG  TZ_MAX_RULES          = 1024; 
const BYTE   TZ_BIN_VERSION_MAJOR  = 0x02; 
const BYTE   TZ_BIN_VERSION_MINOR  = 0x01; 
#endif

class CPST2ICALApp;
class CCalendar;
class CCalendarItem;
class CObjectArray;
class CMessageItem;
class CList;

typedef struct {
	unsigned long totalBytesProcessed;
	FILE *outputFile;
} CallBackData;

class CPSTProc  
{
public:
	CPSTProc();
	virtual ~CPSTProc();

	int Start(_TCHAR* exec, CPST2ICALApp *theApp);
	int PerformMigration(char *dbFullPath);
	int PerformMigrationLocal(_TCHAR *inputDir);
	int PerformOneMailboxMigration(LPMAPISESSION pses, LPMDB pmdb, const _TCHAR *pstFile, const _TCHAR *outputdir, 
		UINT *totalFiles = NULL, UINT *totalFilesError = NULL);
//	char* ResolveEntry(DBHANDLE hDB, char* token, const _TCHAR* resolveDocField, const _TCHAR* resolveDocForm,
//							   const _TCHAR* resolveDocLookupField, int* retCode);
//	STATUS near pascal GetUniqueFileName(char *Drive, char *Ext,
//                                        char *FileName);
	HRESULT ProcessOneFolder(LPMDB pmdb, 
		LPMAPIFOLDER pf, _TCHAR *output_dir, _TCHAR *icsFile,
		UINT *totalFolderMsgs, UINT *totalFolderMsgsDone, 
		UINT *totalFolderMsgsSkipped, UINT *totalFolderMsgsError);
	HRESULT ProcessOneCalendarDoc(LPMESSAGE lpMsg, FILE *f);
	HRESULT ApplyTransformers(LPMESSAGE lpMsg,  const _TCHAR *name, 
		const _TCHAR *value, FILE *outputFile,
		_TCHAR *text_to_use = NULL, int *index = NULL);
/*
	void PutMessageSender(NOTEHANDLE hNote, LPMESSAGE lpMessage, LPSPropValue pMessagePropValues, UINT *c);
	void PutMessageRecipients(NOTEHANDLE hNote, LPMESSAGE lpMessage, LPSPropValue pMessagePropValues, UINT *c);
	STATUS ProcessBody(NOTEHANDLE hNote, LPMESSAGE lpMessage, UINT *NumNotesEncrypt);
	STATUS ProcessAttachments(NOTEHANDLE hNote, LPMESSAGE lpMessage, UINT *NumNotesEncrypt);
	void PutFileHeader(NOTEHANDLE hNote, BLOCKID value_blockid, FILE *outputFile);
	void PutFilesFooter(NOTEHANDLE hNote, FILE *outputFile);
	void PutTextHeader(NOTEHANDLE hNote, FILE *outputFile);
	STATUS ApplyTransformers(NOTEHANDLE hNote, CMessageItem *item, 
		LPMESSAGE lpMessage, LPSPropValue pMessagePropValues, UINT *c,
		BLOCKID *value_blockid = NULL);
	bool IsDateInRange(DBHANDLE hDB, NOTEHANDLE hNote, _TCHAR const *dateFilterField, 
								_TCHAR const *dateFilterAfter, _TCHAR const *dateFilterBefore, 
								_TCHAR const *dateMask);
*/
	void parseSMTPFile(const _TCHAR *smtpAddrFile, const _TCHAR *smtpPartsSep);
	void DoSmtpLookup(_TCHAR *field_text, WORD *field_len);


protected:
	bool m_verbose;
	_TCHAR m_language[3];
	_TCHAR m_server[256];
	_TCHAR m_basedn[512];
	_TCHAR m_filter[256];
	_TCHAR m_container[512];
	_TCHAR m_groupcontainer[512];
	_TCHAR m_pstfilePassword[128];
	_TCHAR m_inputdir[512];
	_TCHAR m_logFile[512];
	unsigned int m_logSizeWrap;
	_TCHAR m_outputdir[512];
	_TCHAR m_onlyPSTFile[512];
	_TCHAR m_smtpAddrFile[256];
	_TCHAR m_smtpPartsSep[32];
	bool m_appendLogFile;

	CPST2ICALApp *theParentApp;

	CCalendar *calendar;
	CList *listSuppl;
	CList *smtpAdr;

public:
	// Get/Set m_language
	_TCHAR const * GetLanguage();
	void SetLanguage(const _TCHAR *language);

	// Get/Set m_server
	_TCHAR const * GetServer();
	void SetServer(const _TCHAR *server);

	// Get/Set m_filter
	_TCHAR const * GetFilter();
	void SetFilter(const _TCHAR *filter);

	// Get/Set m_container
	_TCHAR const * GetContainer();
	void SetContainer(const _TCHAR *container);

	// Get/Set m_groupcontainer
	_TCHAR const * GetGroupContainer();
	void SetGroupContainer(const _TCHAR *groupcontainer);

	// Get/Set m_idfilePassword
	_TCHAR const * GetPSTFilePassword();
	void SetPSTFilePassword(const _TCHAR *idFilePassword);

	// Get/Set m_inputdir
	_TCHAR const * GetInputDir();
	void SetInputDir(const _TCHAR *inputDir);

	// Get/Set m_logFile
	_TCHAR const * GetLogFile();
	void SetLogFile(const _TCHAR *logFile);

	// Get/Set m_logSizeWrap
	unsigned int GetLogSizeWrap();
	void SetLogSizeWrap(unsigned int logSizeWrap);

	// Get/Set m_outputdir
	_TCHAR const * GetOutputDir();
	void SetOutputDir(const _TCHAR *outputDir);

	// Get/Set m_onlyNSFFile
	_TCHAR const * GetOnlyPSTFile();
	void SetOnlyPSTFile(const _TCHAR *onlyNSFFile);

	// Get/Set m_smtpAddrFile
	_TCHAR const * GetSmtpAddrFile();
	void SetSmtpAddrFile(const _TCHAR *smtpAddrFile);

	// Get/Set m_smtpPartsSep
	_TCHAR const * GetSmtpPartsSep();
	void SetSmtpPartsSep(const _TCHAR *smtpPartsSep);

	// Get/Set m_verbose
	bool const GetVerbose();
	void SetVerbose(const bool verbose);

	// Get/Set m_appendLogFile
	bool const GetAppendLogFile();
	void SetAppendLogFile(const bool appendLogFile);

	int CreateOutlookFile(_TCHAR *targetdir, const _TCHAR *name, int how, bool splitName = TRUE, _TCHAR *nsfFile = NULL);

private:
	void GetPSTMask(_TCHAR *pstmask);
	ULONG GetXMLPropTagValue(const _TCHAR *propTag);
	bool IsIgnoredFolder(_TCHAR *ignoredFolders, _TCHAR *folderName);
	bool IsIgnoredMessageForm(_TCHAR *messageForms, char *messageFormCh);
//	STATUS RetrieveFirstLastDomainFromSent(HANDLE hMessageFile, _TCHAR *firstLast);
	void SetRecipient(CMessageItem *item, LPMESSAGE lpMessage, char *field_text, char *email, LPSPropValue pMessagePropValues, UINT *c);
	int GetNamedPropertyIdx(ULONG namedProperty);
	TZDEFINITION* CPSTProc::BinToTZDEFINITION(ULONG cbDef, LPBYTE lpbDef);
	void MyHexFromBin(LPBYTE lpb, size_t cb, LPTSTR* lpsz);

	HRESULT PutCalendarHeader(FILE *outputFile);
	HRESULT PutEventFooter(LPMESSAGE lpMsg, FILE *outputFile);
	HRESULT PutCalendarFooter(FILE *outputFile);
	HRESULT ProcessAttendee(CCalendarItem *item, LPMESSAGE lpMsg, FILE* outputFile);
	HRESULT ProcessAttachments(LPMESSAGE lpMsg, FILE* outputFile, bool tab);

};

#endif // !defined(AFX_PSTPROC_H__8CD32256_505C_4973_9A04_DBC9BF1C4FA2__INCLUDED_)
