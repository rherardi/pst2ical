// NotesProc.cpp: implementation of the CPSTProc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define DONT_USE_OUTPUT_UNICODE
#define OUTLOOK_UNICODE

#include "mapistuff.h"

#include <initguid.h>
#include "PSTProc.h"
#include "resource.h"
#include "PST2ICALApp.hpp"
#include "Config.hpp"
#include "Calendar.hpp"
#include "CalendarItem.hpp"
#include "XMLNameValue.hpp"
#include "EntryTransformer.hpp"
#include "OutlineInfo.hpp"
#include "ObjectArray.hpp"
#include "DirUtils.hpp"
#include "OutlookCondition.hpp"
#include "ConversionRuleEntry.hpp"

#include "JESCommon.h"
#include "inistuff.h"
#include "list.hpp"

static char smtp[] = "SMTP";

#if (NOTESAPI_MAJOR_VERSION == 5)
static char *abbrMonths[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
static char *abbrDaysOfWeek[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
#endif

#define XML_PROP_TAG(NAME)  { _T(#NAME), NAME }
#define XML_PROP_TAG1(NAME)  { _T(#NAME)}

struct PropTag {
	_TCHAR *prop;
	ULONG value;
};

struct PropTag PropTags[] = {
	XML_PROP_TAG(PR_MESSAGE_CLASS),
	XML_PROP_TAG(PR_MESSAGE_FLAGS),
	XML_PROP_TAG(PR_SENDER_ENTRYID),
	XML_PROP_TAG(PR_SENDER_NAME),
	XML_PROP_TAG(PR_SENDER_EMAIL_ADDRESS),
	XML_PROP_TAG(PR_SENDER_ADDRTYPE),
	XML_PROP_TAG(PR_SENT_REPRESENTING_ENTRYID),
	XML_PROP_TAG(PR_SENT_REPRESENTING_NAME),
	XML_PROP_TAG(PR_SENT_REPRESENTING_EMAIL_ADDRESS),
	XML_PROP_TAG(PR_SENT_REPRESENTING_ADDRTYPE),
	XML_PROP_TAG(PR_IMPORTANCE),
	XML_PROP_TAG(PR_CREATION_TIME),
	XML_PROP_TAG(PR_LAST_MODIFICATION_TIME),
	XML_PROP_TAG(PR_DISPLAY_TO),
	XML_PROP_TAG(PR_MESSAGE_ATTACHMENTS),
	XML_PROP_TAG(PR_CLIENT_SUBMIT_TIME),
	XML_PROP_TAG(PR_MESSAGE_DELIVERY_TIME),
	XML_PROP_TAG(PR_DISPLAY_NAME),
	XML_PROP_TAG(PR_RECIPIENT_TYPE),
	XML_PROP_TAG(PR_SUBJECT),
	XML_PROP_TAG(PR_BODY),
	XML_PROP_TAG(PR_ATTACH_METHOD),
	XML_PROP_TAG(PR_ATTACH_FILENAME),
	XML_PROP_TAG(PR_ATTACH_LONG_FILENAME)
};

// The array is the list of named properties to be set.
NamedProperty aulCalProps[] = {
	{PidLidCleanGlobalObjectId, PT_BINARY, (LPGUID)&PSETID_Meeting, NULL},
	{PidLidLocation, PT_STRING8, (LPGUID)&PSETID_Appointment, NULL},
	{PidLidAppointmentTimeZoneDefinitionStartDisplay, PT_BINARY, (LPGUID)&PSETID_Appointment, NULL},
	{PidLidCommonStart, PT_SYSTIME, (LPGUID)&PSETID_Common, NULL},
	{PidLidCommonEnd, PT_SYSTIME, (LPGUID)&PSETID_Common, NULL},
	{PidLidContacts,  PT_MV_STRING8, (LPGUID)&PSETID_Common, NULL},
	{0,  PT_MV_STRING8, (LPGUID)&PSETID_Calendar, _T("Keywords")}
};
#define ulCalProps (sizeof(aulCalProps)/sizeof(aulCalProps [0]))

// For PR_MESSAGE_ATTACHMENTS
// http://support.microsoft.com/kb/173353

// Outlook timezones structure
// http://blogs.msdn.com/stephen_griffin/archive/2006/12/06/outlook-2007-timezone-structures.aspx

// Item categories
// http://www.dimastr.com/Redemption/utils.htm

#ifndef MAX_LINE
#define MAX_LINE 256
#endif

/* constants */
#define     READMAIL_BODY_LINELEN   40
char szDriveLetter_blank[] = "";
char szExtensionName_TMP[] = "TMP";
char szNewline_Terminate[] = "\r\n";
#define CHARS_PER_LINE 80

#define SHORT_PRODUCT_TITLE	"pst2ical"
#define SHORT_PRODUCT_TITLE_W	L"pst2ical"

LIST *lst;
LIST *notFoundLst;

CPSTProc *pPSTProc;
LPSPropTagArray lpNamedPropTags;

#define STRING_LENGTH   256

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPSTProc::CPSTProc()
{
	m_verbose = false;
	m_language[0] = _T('\0');
	m_server[0] = _T('\0');
	m_basedn[0] = _T('\0');
	m_container[0] = _T('\0');
	m_groupcontainer[0] = _T('\0');
	m_pstfilePassword[0] = _T('\0');
	m_inputdir[0] = _T('\0');
	m_logFile[0] = _T('\0');
	m_outputdir[0] = _T('\0');
	m_onlyPSTFile[0] = _T('\0');
	m_smtpAddrFile[0] = _T('\0');
	pPSTProc = this;

	listSuppl = new CList();
}

CPSTProc::~CPSTProc()
{

}

// Get/Set m_language
_TCHAR const * CPSTProc::GetLanguage()
{
	return &m_language[0];
}
void CPSTProc::SetLanguage(const _TCHAR *language)
{
	if (language != NULL) {
		::_tcscpy(m_language, language);
	}
}

// Get/Set m_filter
_TCHAR const * CPSTProc::GetFilter()
{
	return m_filter;
}
void CPSTProc::SetFilter(const _TCHAR *filter)
{
	if (filter != NULL) {
		::_tcscpy(m_filter, filter);
	}
}

// Get/Set m_container
_TCHAR const * CPSTProc::GetContainer()
{
	return m_container;
}
void CPSTProc::SetContainer(const _TCHAR *container)
{
	if (container != NULL) {
		::_tcscpy(m_container, container);
	}
}

// Get/Set m_groupcontainer
_TCHAR const * CPSTProc::GetGroupContainer()
{
	return m_groupcontainer;
}
void CPSTProc::SetGroupContainer(const _TCHAR *groupcontainer)
{
	if (groupcontainer != NULL) {
		::_tcscpy(m_groupcontainer, groupcontainer);
	}
}

// Get/Set m_pstfilePassword
_TCHAR const * CPSTProc::GetPSTFilePassword()
{
	return &m_pstfilePassword[0];
}
void CPSTProc::SetPSTFilePassword(const _TCHAR *pstfilePassword)
{
	if (pstfilePassword != NULL) {
		::_tcscpy(m_pstfilePassword, pstfilePassword);
	}
}

// Get/Set m_inputdir
_TCHAR const * CPSTProc::GetInputDir()
{
	return &m_inputdir[0];
}
void CPSTProc::SetInputDir(const _TCHAR *inputDir)
{
	if (inputDir != NULL) {
		::_tcscpy(m_inputdir, inputDir);
	}
}

// Get/Set m_logFile
_TCHAR const * CPSTProc::GetLogFile()
{
	return &m_logFile[0];
}
void CPSTProc::SetLogFile(const _TCHAR *logFile)
{
	if (logFile != NULL) {
		::_tcscpy(m_logFile, logFile);
	}
}

// Get/Set m_logSizeWrap
unsigned int CPSTProc::GetLogSizeWrap()
{
	return m_logSizeWrap;
}
void CPSTProc::SetLogSizeWrap(unsigned int logSizeWrap)
{
	m_logSizeWrap = logSizeWrap;
}

// Get/Set m_outputdir
_TCHAR const * CPSTProc::GetOutputDir()
{
	return &m_outputdir[0];
}
void CPSTProc::SetOutputDir(const _TCHAR *outputDir)
{
	if (outputDir != NULL) {
		::_tcscpy(m_outputdir, outputDir);
	}
}

// Get/Set m_onlyPSTFile
_TCHAR const * CPSTProc::GetOnlyPSTFile()
{
	return &m_onlyPSTFile[0];
}
void CPSTProc::SetOnlyPSTFile(const _TCHAR *onlyPSTFile)
{
	if (onlyPSTFile != NULL) {
		::_tcscpy(m_onlyPSTFile, onlyPSTFile);
	}
}

// Get/Set m_smtpAddrFile
_TCHAR const * CPSTProc::GetSmtpAddrFile()
{
	return &m_smtpAddrFile[0];
}
void CPSTProc::SetSmtpAddrFile(const _TCHAR *smtpAddrFile)
{
	if (smtpAddrFile != NULL) {
		::_tcscpy(m_smtpAddrFile, smtpAddrFile);
	}
}

// Get/Set m_smtpPartsSep
_TCHAR const * CPSTProc::GetSmtpPartsSep()
{
	return &m_smtpPartsSep[0];
}
void CPSTProc::SetSmtpPartsSep(const _TCHAR *smtpPartsSep)
{
	if (smtpPartsSep != NULL) {
		::_tcscpy(m_smtpPartsSep, smtpPartsSep);
	}
}

// Get/Set m_verbose
bool const CPSTProc::GetVerbose()
{
	return m_verbose;
}
void CPSTProc::SetVerbose(const bool verbose)
{
	m_verbose = verbose;
}

// Get/Set m_appendLogFile
bool const CPSTProc::GetAppendLogFile()
{
	return m_appendLogFile;
}
void CPSTProc::SetAppendLogFile(const bool appendLogFile)
{
	m_appendLogFile = appendLogFile;
}

int CPSTProc::Start(_TCHAR* exec, CPST2ICALApp *theApp)
{
	_TCHAR usage[256];
	HINSTANCE hInst = ::GetModuleHandle(NULL);
	_TCHAR current_method[] = _TEXT("CPSTProc::Start");
	if (m_verbose){
		::LoadString(hInst, IDS_ENTER_METHOD, usage, 256);
		::_tprintf(_TEXT("%s %s\n"), usage, current_method);
	}

	int retCode = MIGRATION_SUCCESS;
	HRESULT hr;

	bool localMode = true;

	theParentApp = theApp;
	calendar = theParentApp->getCalendarInfo();
	
	if (!localMode) {
		//retCode = PerformMigration(fullpath_name);
	}
	else {
		retCode = PerformMigrationLocal((_TCHAR *)this->GetInputDir());
	}

cleanup:
	if (FAILED(hr)) {
//		char szErrorString[512];
//		OSLoadString(NULLHANDLE, ERR(error),
//			szErrorString, 512);
//		::LoadString(hInst, IDS_ERR_OUTLOOK, usage, 256);
//		::_tprintf(_TEXT("%s "), usage);
//		printf("%s\n", szErrorString);
	}
	if (m_verbose){
		::LoadString(hInst, IDS_EXIT_METHOD, usage, 256);
		::_tprintf(_TEXT("%s %s %d\n"), usage, current_method, retCode);
	}
	return retCode;
}

// Go through local pst files and process them
int CPSTProc::PerformMigrationLocal(_TCHAR *inputDir)
{
	_TCHAR usage[256];
	HINSTANCE hInst = ::GetModuleHandle(NULL);
	_TCHAR current_method[] = _TEXT("CPSTProc::PerformMigrationLocal");
	if (m_verbose){
		::LoadString(hInst, IDS_ENTER_METHOD, usage, 256);
		::_tprintf(_TEXT("%s %s\n"), usage, current_method);
	}
	int retCode = MIGRATION_STEP_SUCCESS;
	HRESULT hr;
	WIN32_FIND_DATA fd;
	HANDLE hSearch;
	bool fFinished = FALSE;
	_TCHAR fullmask[1024];
	_TCHAR pstFileFull[1024];
	UINT totalFiles = 0, totalFilesError = 0;

	if (inputDir == NULL || _tcslen(inputDir) == 0) {
		theParentApp->PrefixWithCurDir(_T(""), fullmask);
	}
	else {
		_tcscpy(fullmask, inputDir);
		_tcscat(fullmask, _T("/"));
	}
	_tcscat(fullmask, PSTMASK);

	INI_LogWriteWithResourcePrefix(IDS_LOG_FULLMASK, fullmask);

	if (theParentApp->IsSMTPLookupRequested()) {
		if (this->GetSmtpAddrFile() != NULL && _tcslen(this->GetSmtpAddrFile()) > 0)
			parseSMTPFile(this->GetSmtpAddrFile(), this->GetSmtpPartsSep());
	}

	if (this->GetOnlyPSTFile() != NULL && _tcslen(this->GetOnlyPSTFile()) > 0) {
		_TCHAR outputdir[512];
		_tcscpy(outputdir, this->GetOutputDir());
		_tcscat(outputdir, _T("/"));
		PerformOneMailboxMigration(NULL, NULL, this->GetOnlyPSTFile(), outputdir, &totalFiles, &totalFilesError);
	}
	else
	{
		// go through all files with .nsf extension in inputDir
		INI_LogWriteWithResourcePrefix(IDS_LOG_FIND_HANDLE_OPEN, _T(""));
		hSearch = FindFirstFile(fullmask, &fd);
		if (hSearch != INVALID_HANDLE_VALUE) {
			while (!fFinished) { 
				if (fd.dwFileAttributes == FILE_ATTRIBUTE_ARCHIVE ||
					fd.dwFileAttributes == FILE_ATTRIBUTE_HIDDEN ||
					fd.dwFileAttributes == FILE_ATTRIBUTE_COMPRESSED ||
					fd.dwFileAttributes == FILE_ATTRIBUTE_NORMAL) {
					_TCHAR outputdir[512];
					_tcscpy(outputdir, this->GetOutputDir());
					_tcscat(outputdir, _T("/"));
					if (inputDir == NULL || _tcslen(inputDir) == 0) {
						theParentApp->PrefixWithCurDir(_T(""), pstFileFull);
					}
					else {
						_tcscpy(pstFileFull, inputDir);
						_tcscat(pstFileFull, _T("/"));
					}
					_tcscat(pstFileFull, fd.cFileName);
					INI_LogWriteWithResourcePrefix(IDS_LOG_FIND_HANDLE_FILE, _T(""));
//					_tprintf(_T("-->pstFileFull: %s\n"), pstFileFull);
					PerformOneMailboxMigration(NULL, NULL, pstFileFull, outputdir, &totalFiles, &totalFilesError);
				}
				if (!FindNextFile(hSearch, &fd)) {
					INI_LogWriteWithResourcePrefix(IDS_LOG_FIND_HANDLE_CLOSE, _T(""));
					FindClose(hSearch);
					fFinished = TRUE;
				}
			}
		}
	}

	_tprintf(_T("\n"));
	INI_LogWriteWithResourcePrefixNum(IDS_LOG_TOTAL_FILES_DONE, totalFiles);
	INI_LogWriteWithResourcePrefixNum(IDS_LOG_TOTAL_FILES_ERROR, totalFilesError);

	if (totalFilesError > 0)
		retCode = MIGRATION_SUCCESS_SOME_ERRORS;

	if (m_verbose){
		::LoadString(hInst, IDS_EXIT_METHOD, usage, 256);
		::_tprintf(_TEXT("%s %s %d\n"), usage, current_method, retCode);
	}
	return retCode;
}
//-------------------------------------------------------
// Open standard Names.nsf database and process all Persons/Groups entries into LDIF format
int CPSTProc::PerformMigration(char *dbFullPath)
{
	_TCHAR usage[256];
	HINSTANCE hInst = ::GetModuleHandle(NULL);
	_TCHAR current_method[] = _TEXT("CPSTProc::PerformMigration");
	if (m_verbose){
		::LoadString(hInst, IDS_ENTER_METHOD, usage, 256);
		::_tprintf(_TEXT("%s %s\n"), usage, current_method);
	}
	
	int retCode = MIGRATION_STEP_SUCCESS;
	HRESULT hr;

	if (m_verbose){
		::LoadString(hInst, IDS_EXIT_METHOD, usage, 256);
		::_tprintf(_TEXT("%s %s %d\n"), usage, current_method, retCode);
	}
	return retCode;
}

int CPSTProc::PerformOneMailboxMigration(LPMAPISESSION pSes, LPMDB pMdb, const _TCHAR *pstFile, const _TCHAR *outputdir,
										   UINT *totalFiles, UINT *totalFilesError)
{
	int retCode = MIGRATION_STEP_SUCCESS;
	_TCHAR usage[1024];
	HINSTANCE hInst = ::GetModuleHandle(NULL);
	_TCHAR current_method[] = _TEXT("CPSTProc::PerformOneMailboxMigration");
	if (m_verbose){
		::LoadString(hInst, IDS_ENTER_METHOD, usage, 256);
		::_tprintf(_TEXT("%s %s\n"), usage, current_method);
	}
//	::_tprintf(_TEXT("-->outputdir: %s\n"), outputdir);
	HRESULT hr;
	LPMAPISESSION pses = NULL;
	LPMDB pmdb = NULL;

	UINT NumItems = 0;
	UINT totalMsgs = 0, totalMsgsDone = 0, 
		totalMsgsSkipped = 0, totalMsgsError = 0;
	TCHAR icsFile[512];
	int slashOffset = 0;
	_TCHAR *ptrPst = NULL;
	int k = 0;

	CObjectArray resolvedFolders;
	int findex = 0;

	char field_text[256];
	char field_textUTF8[256];
	wchar_t field_textW[256];


	LIST *lstelem = NULL, *pstname;

	LPMAPIFOLDER lpfRootF = NULL;
	ULONG  ulObjType = 0;

	const _TCHAR *output_dir;
	bool local = (pstFile != NULL ? true : false);

	if (local) {
		::INI_LogWriteWithResourcePrefix(IDS_LOG_LOCAL_PSTFILE, (_TCHAR *)pstFile);
	}
	else {
		goto cleanup;
	}

    // Open the message file.

//	else {
//		// pstFile here is with path, drop it before passing
//		::INI_LogWriteWithResourcePrefix(IDS_LOG_NOTES_MAILFILE, _T("-->1"));
//		_tcscpy(firstLast, pstFile);
//		_TCHAR *pos = _tcsrchr(firstLast, _T('.'));
//		if (pos != NULL)
//			*pos = _T('\0');
//		pos =_tcsrchr(firstLast, _T('\\'));
//		if (pos != NULL)
//			_tcscpy(firstLast, pos + 1);
//		pos = _tcsrchr(firstLast, _T('/'));
//		if (pos != NULL)
//			_tcscpy(firstLast, pos + 1);
//
//		::INI_LogWriteWithResourcePrefix(IDS_LOG_NOTES_MAILFILE, _T("-->2"));
//		::INI_LogWriteWithResourcePrefix(IDS_LOG_NOTES_MAILFILE, firstLast);
//		// Retrieve FirstName/LastName/Domain from first Memo message in ($Sent) folder
//		RetrieveFirstLastDomainFromSent(hMessageFile, firstLast);
//		::INI_LogWriteWithResourcePrefix(IDS_LOG_NOTES_MAILFILE, _T("-->3"));
//	}

	output_dir = this->GetOutputDir();
	if (CDirUtils::MakeDir(output_dir) != NO_ERROR) {
		::LoadString(hInst, IDS_ERR_DIR_NOT_CREATED1, usage, 512);
		::_tprintf(usage, output_dir);
		goto cleanup;
	}

	if (pses == NULL || pmdb == NULL) {
		// First - delete same profile if exists
		INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_PROFILECHECK, PROFILE_NAME);
		if (!DeleteProfileWithIProfAdmin(PROFILE_NAME)) {
			retCode = MIGRATION_ERR_FAILED;
			goto cleanup;
		}

		// Create MAPI profile
		pstname = listSuppl->LST_Lookup(&lst, ID_OPT_PSTFILENAME);
		INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_PROFILECR, PROFILE_NAME);
		if (!CreateProfileWithIProfAdmin(PROFILE_NAME, (_TCHAR *)pstFile)) {
			retCode = MIGRATION_ERR_FAILED;
			goto cleanup;
		}
		INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_PROFILECR, _T("-->Done"));
	}

	if (pSes != NULL) {
		pses = pSes;
	}
	else {
		INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_INIT, _T("-->1"));
		MAPIInitialize(NULL);
		INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_INIT, _T("-->2"));
//		printf("-->here 00\n");
#if defined(UNICODE)
		if (FAILED(hr = MAPILogonEx((ULONG)NULL, PROFILE_NAME, NULL,
			MAPI_UNICODE | MAPI_EXTENDED | MAPI_EXPLICIT_PROFILE |	MAPI_NEW_SESSION, &pses))) {
#else
		if (FAILED(hr = MAPILogonEx((ULONG)NULL, PROFILE_NAME, NULL,
			MAPI_EXTENDED | MAPI_EXPLICIT_PROFILE |	MAPI_NEW_SESSION, &pses))) {
#endif
//			item2 = LST_LookupAdd(&lst, ID_OPT_TEXT2, MAX_LINE);
//			LoadString(hInst, IDS_PROFILE_NOTOPEN, item2->data, MAX_LINE);
//				MessageBox(hDlg, PROFILE_NAME, item2->data, MB_OK);
			::INI_LogWriteWithResourcePrefix(IDS_PROFILE_NOTOPEN, PROFILE_NAME);
			return FALSE;
		}
	}

//	printf("-->here 01\n");

	if (pMdb != NULL) {
		pmdb = pMdb;
	}
	else {
//		printf("-->here 02\n");
		INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_OPENSTORE, _T("-->1"));
		pmdb = OpenDefaultStore(pses);
		INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_OPENSTORE, _T("-->2"));

		// Open Root Folder first, i.e. "Personal Folders"
//							pmdb->lpVtbl->OpenEntry(pmdb, 0, (ULONG)NULL, NULL, MAPI_MODIFY, &ulObjType, (LPUNKNOWN FAR *)&lpfRootF);
//							spvMsg.ulPropTag = PR_DISPLAY_NAME;
//							spvMsg.Value.lpszA = "Renamed Folder";
//							HrSetOneProp((LPMAPIPROP)lpfRootF, &spvMsg);
//							ULRELEASE(lpfRootF);
	}

	// Open "Top Of Personal Folders\Calendar" folder
	INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_OPENFOLDER, _T("-->1"));
	lpfRootF = OpenMsgStoreFolderByName(pmdb, MAPI_FOLDER_CALENDAR);
	if (lpfRootF == NULL) {
		INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_CALENDAR_ERR, _T(""));
		retCode = MIGRATION_ERR_FAILED;
		goto cleanup;
	}
	INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_OPENFOLDER, _T("-->2"));

//	if (hr = ResolveFolders(pmdb, lpfRootF, MAPI_FOLDER_TOP, 
//		(_TCHAR *)output_dir,
//		resolvedFolders, 
//		&totalMsgs, &totalMsgsDone, 
//		&totalMsgsSkipped, &totalMsgsError)) {
//		goto cleanup;
//	}
	for (k = _tcslen(pstFile)-1; k>=0; k--) {
		if (pstFile[k] == _T('/') || pstFile[k] == _T('\\')) {
			slashOffset = k;
			break;
		}
	}
	_tcscpy(icsFile, pstFile + slashOffset + 1);
	ptrPst = _tcsstr(icsFile, PST_DEFAULT_SUFFIX);
	if (ptrPst != NULL)
		*ptrPst = _T('\0');
	_tcscat(icsFile, ICS_DEFAULT_SUFFIX);

	if (hr = ProcessOneFolder(pmdb, lpfRootF, (_TCHAR *)output_dir, icsFile, 
		&totalMsgs, &totalMsgsDone, 
		&totalMsgsSkipped, &totalMsgsError)) {
		goto cleanup;
	}
	_tprintf(_T("\n"));
	INI_LogWriteWithResourcePrefixNumP(IDS_LOG_TOTAL_MSGS_FILE, totalMsgs, (_TCHAR *)pstFile, true);
	INI_LogWriteWithResourcePrefixNumP(IDS_LOG_TOTAL_MSGS_FILE_DONE, totalMsgsDone, (_TCHAR *)pstFile, true);
	INI_LogWriteWithResourcePrefixNumP(IDS_LOG_TOTAL_MSGS_FILE_SKIPPED, totalMsgsSkipped, (_TCHAR *)icsFile, true);
	INI_LogWriteWithResourcePrefixNumP(IDS_LOG_TOTAL_MSGS_FILE_ERROR, totalMsgsError, (_TCHAR *)pstFile, true);

	if (local) {
		(*totalFiles) += 1;
		if (totalMsgsError > 0)
			(*totalFilesError) += 1;
	}

cleanup:
	ULRELEASE(lpfRootF);
	if (pSes == NULL && pses != NULL) {
		ULRELEASE(pmdb);
		pses->Logoff((ULONG)NULL, (ULONG)NULL, 0);
		INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_LOGOFF, _T(""));

		ULRELEASE(pses);
		MAPIUninitialize();
		pmdb = NULL;
		pses = NULL;
	}
	if (pSes == NULL) {
		// Delete profile if still exists
		INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_PROFILEDELETED, _T("-->10"));
		DeleteProfileWithIProfAdmin(PROFILE_NAME);
		INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_PROFILEDELETED, _T("-->11"));
	}

	if (m_verbose){
		::LoadString(hInst, IDS_EXIT_METHOD, usage, 256);
		::_tprintf(_TEXT("%s %s %d\n"), usage, current_method, retCode);
	}
	return retCode;
}

/* Local constants */

#define  MAX_ITEMS          20
#define  MAX_ITEM_LEN       1024
#define  DATATYPE_SIZE      sizeof(USHORT)
#define  ITEM_LENGTH_SIZE   sizeof(USHORT)

HRESULT CPSTProc::ProcessOneFolder(LPMDB pmdb, LPMAPIFOLDER pf, _TCHAR *output_dir, _TCHAR *icsFile,
									UINT *totalFolderMsgs, UINT *totalFolderMsgsDone, 
									UINT *totalFolderMsgsSkipped, UINT *totalFolderMsgsError)
{
	_TCHAR usage[256];
	HINSTANCE hInst = ::GetModuleHandle(NULL);

	HRESULT hr = S_OK;
	_TCHAR fullFolderName[_MAX_PATH];
	_TCHAR folderName[_MAX_PATH];
	_TCHAR fullFolderNamePath[_MAX_PATH];
	_TCHAR fileName[512];

	ULONG    items_found = 0;             	/* items of notes found */
	UINT NumItemsTotal = 0, NumItemsError = 0, NumItemsSkipped = 0;
	LPMAPIFOLDER folderF;
	LPMAPITABLE lpContentsTable = NULL;

	FILE *f;
	_TCHAR *ptr = NULL;

	folderF = pf;

	static const enum {IENTRYID};
	static SizedSPropTagArray( 1, rgColProps) = { 1, { 
		PR_ENTRYID}};
	LPSRowSet lpRow = NULL;
	LPSPropValue lpRowProp = NULL;
	ULONG i = 0L;
	ULONG ulObjType;
	LPMESSAGE lpMsg = NULL;
	ULONG cbEid = 0;          // number of bytes in entry identifier
	LPENTRYID lpEid = NULL;

	MAPINAMEID  rgnmid[ulCalProps];
	LPMAPINAMEID rgpnmid[ulCalProps];

	static const enum {ICOUNT};
	static SizedSPropTagArray(1, cols) = { 1, {PR_CONTENT_COUNT} };
	ULONG pcount; 
	SPropValue *props;
	hr = folderF->GetProps((SPropTagArray*)&cols, 0, &pcount, &props);
	_tprintf(_T("-->000: %lx<--\n"), hr);
	if (FAILED(hr))
	{
		goto cleanup;
	}

	items_found = props[ICOUNT].Value.ul;
	MAPIFreeBuffer(props);

	// Don't use _tcstok here, cause MakeDir uses it and nested strtok are not possible!!!
	_tcscpy(fullFolderNamePath, output_dir);
	_tcscpy(folderName, MAPI_FOLDER_CALENDAR);

	//_tprintf(_T("-->fullFolderNamePath: %s, folderToken: %s\n"), fullFolderNamePath, folderToken);
	f = CDirUtils::OpenFile(output_dir, NULL, icsFile, _TEXT("w+b"));
	if (f == NULL) {
		::LoadString(hInst, IDS_ERR_FILE_NOT_CREATED2, usage, 512);
		::_tprintf(usage, fullFolderNamePath, icsFile);
		goto cleanup;
	}
#ifndef DONT_USE_OUTPUT_UNICODE
	_ftprintf(f, _TEXT("%c"), 0xfeff);
#endif

	if (S_OK != PutCalendarHeader(f))
		goto cleanup;

	if (items_found > 0) {
		// Go through messages
		if(FAILED(hr = folderF->GetContentsTable(0L, &lpContentsTable)))
		{
			goto cleanup;
		}

		hr = HrQueryAllRows( lpContentsTable, (LPSPropTagArray)&rgColProps, NULL, NULL, 0L, &lpRow);
		if( FAILED( hr))
		{
			goto cleanup;
		}

		for(i=0; i < lpRow->cRows; i++)
		{
			lpRowProp = lpRow->aRow[i].lpProps;
//			TitleW = lpRowProp[IDISPNAME].Value.lpszW;
//			printf("%s\n",lpRow->aRow[i].lpProps[1].Value.lpszA);
			cbEid = lpRowProp[IENTRYID].Value.bin.cb;
			lpEid = (LPENTRYID)lpRowProp[IENTRYID].Value.bin.lpb;
			hr = folderF->OpenEntry(cbEid, lpEid, NULL, MAPI_MODIFY, &ulObjType, (LPUNKNOWN FAR *)&lpMsg);
			_tprintf(_T("-->000: %lx %lx<--\n"), cbEid, hr);
			if( FAILED( hr))
			{
				goto cleanup;
			}
			INI_LogWrite(_T("-->ProcessOneCalendarDoc"));

	if (lpMsg != NULL) {
//	_tprintf(_T("-->at000 %s, %s<--\n"), name, value);
			for (ULONG i = 0 ; i < ulCalProps ; i++)
			{
				rgnmid[i].lpguid = aulCalProps[i].pGuid;
				rgnmid[i].ulKind = MNID_ID;
				if (aulCalProps[i].pID != 0)
					rgnmid[i].Kind.lID = aulCalProps[i].pID;
				if (aulCalProps[i].pID == 0)
					rgnmid[i].Kind.lpwstrName = aulCalProps[i].pName;
				rgpnmid[i] = &rgnmid[i];
			}

//	_tprintf(_T("-->at00 %s, %s<--\n"), name, value);
			hr = lpMsg->GetIDsFromNames(
				ulCalProps,
				(LPMAPINAMEID*) &rgpnmid,
				MAPI_CREATE, // NULL,
				&lpNamedPropTags);
			if (FAILED(hr))	{
				::INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_CANT_GET_ID, _T(""));
				goto cleanup;
			}
	}

			if (calendar == NULL)
				calendar = theParentApp->getCalendarInfo();
			if (calendar->GetDateFilterAfter() != 0 ||
				calendar->GetDateFilterBefore() != 0) {
//				if (!IsDateInRange(lpMsg, calendar->GetDateFilterField(), 
//					calendar->GetDateFilterStart(), calendar->GetDateMask())) {
//					if (m_verbose) {
//						::LoadString(hInst, IDS_NOTE_SKIPPED_NOT_REQUIRED_DATERANGE, usage, 256);
//						::_tprintf(_TEXT("%s\n"), usage);
//					}
//					INI_LogWriteWithResourcePrefix(IDS_NOTE_SKIPPED_NOT_REQUIRED_DATERANGE, _T(""));
//					NumItemsSkipped++;
//					continue;
//				}
			}

			hr = ProcessOneCalendarDoc(lpMsg, f);
			if (FAILED(hr))
				NumItemsError++;
			else
				NumItemsTotal++;
			INI_LogWrite(_T("<--ProcessOneCalendarDoc"));
		}

	}

	if (S_OK != PutCalendarFooter(f))
		goto cleanup;

	ptr = _tcsstr(folderName, MAPI_FOLDER_TOP);
	if (ptr != NULL)
		_tcscpy(fullFolderName, ptr + _tcslen(MAPI_FOLDER_TOP) + 1);
	else
		_tcscpy(fullFolderName, folderName);
	INI_LogWriteWithResourcePrefixNumP(IDS_LOG_TOTAL_MSGS, items_found, fullFolderName);
	INI_LogWriteWithResourcePrefixNumP(IDS_LOG_TOTAL_MSGS_DONE, NumItemsTotal, fullFolderName);
	INI_LogWriteWithResourcePrefixNumP(IDS_LOG_TOTAL_MSGS_SKIPPED, NumItemsSkipped, fullFolderName);
	INI_LogWriteWithResourcePrefixNumP(IDS_LOG_TOTAL_MSGS_ERROR, NumItemsError, fullFolderName);
	if (m_verbose) {
		::LoadString(hInst, IDS_LOG_TOTAL_MSGS, usage, sizeof(usage));
		::_tprintf(usage, fullFolderName);
		::_tprintf(_TEXT(" %ld\n"), items_found);
		::LoadString(hInst, IDS_LOG_TOTAL_MSGS_DONE, usage, sizeof(usage));
		::_tprintf(usage, fullFolderName);
		::_tprintf(_TEXT(" %ld\n"), NumItemsTotal);
		::LoadString(hInst, IDS_LOG_TOTAL_MSGS_SKIPPED, usage, sizeof(usage));
		::_tprintf(usage, fullFolderName);
		::_tprintf(_TEXT(" %ld\n"), NumItemsSkipped);
		::LoadString(hInst, IDS_LOG_TOTAL_MSGS_ERROR, usage, sizeof(usage));
		::_tprintf(usage, fullFolderName);
		::_tprintf(_TEXT(" %ld\n"), NumItemsError);
	}
	(*totalFolderMsgs) += items_found;
	(*totalFolderMsgsDone) += NumItemsTotal;
	(*totalFolderMsgsSkipped) += NumItemsSkipped;
	(*totalFolderMsgsError) += NumItemsError;

cleanup:
	if (f != NULL) {
		fclose(f);
	}

	return hr;
}

HRESULT CPSTProc::ProcessOneCalendarDoc(LPMESSAGE lpMsg, FILE *outputFile)
{
	HRESULT error = S_OK;

	CCalendarItem* item;
	if (theParentApp->IsSMTPLookupRequired()) {
		// here we need to check in advance if SMTP will succeed or not
		theParentApp->resetEntryListIndex();
		INI_LogWriteWithResourcePrefix(IDS_LOG_SMTP_LOOKUP_CHECKING, _T(""));
		while ((item = theParentApp->GetNextCalendarSMTPRequiredItem()) != NULL) {
			_tprintf(_T("-->before transformers: %s, %s\n"), item->GetName(), item->GetValue());
			const _TCHAR* condition = item->GetCondition();

			// special case for multivalue attendee
			if (condition != NULL && _tcscmp(condition, CONDITION_EVENTMEETING_ATTENDEE) == 0) {
				error = ProcessAttendee(item, lpMsg, NULL);
			}
			else
				error = ApplyTransformers(lpMsg, item->GetName(), item->GetValue(), NULL);
//			printf("-->error: %d\n", error);
			if (error == SMTP_LOOKUP_FAILED) {
				delete(item);
				return error;
			}
			delete(item);
		}
	}

	theParentApp->resetEntryListIndex();
	calendar = theParentApp->getCalendarInfo();
//	theParentApp->InitializeCalendarItemList();

	INI_LogWrite(_T("-->Process item properties"));
	while ((item = theParentApp->GetNextCalendarItem()) != NULL) {
		_tprintf(_T("-->before transformers: %s, %s\n"), item->GetName(), item->GetValue());
		const _TCHAR* condition = item->GetCondition();

		if (condition != NULL && _tcslen(condition) > 0 && 
			!COutlookCondition::IsOutlookCondition(lpMsg, condition, &error)) {
			delete(item);
			continue;
		}

		// special case for Ncal, cause we need to have attachments inline
		if (condition != NULL && _tcscmp(condition, CONDITION_ATTACHMENTPART) == 0) {
			// preserve it, cause will be broken with ProcessAttachments
			int currentEntryIndex = theParentApp->GetCurrentEntryIndex();
			// For Outlook 2007 we need that tabulator
			bool tab = true;
			ProcessAttachments(lpMsg, outputFile, tab);
			theParentApp->SetCurrentEntryIndex(currentEntryIndex + 1);
			//continue;
			delete(item);
			break;
		}

		ApplyTransformers(lpMsg, item->GetName(), item->GetValue(), outputFile);
		delete(item);
	}
	INI_LogWrite(_T("<--Process item properties"));

	// now process message body
	INI_LogWrite(_T("-->ProcessBody"));
//	error = ProcessBody(hNote, outputFile, NumNotesEncrypt);
	INI_LogWrite(_T("<--ProcessBody"));
//	if (error) {
//		goto cleanup;
//	}

	// now process file attachments
	INI_LogWrite(_T("-->ProcessAttachments"));
//	error = ProcessAttachments(hNote, outputFile);
	INI_LogWrite(_T("<--ProcessAttachments"));

cleanup:
	return error;
}

HRESULT CPSTProc::ApplyTransformers(LPMESSAGE lpMsg,  const _TCHAR *name, 
	const _TCHAR *value, FILE *outputFile, 
	_TCHAR *text_to_use, int *index)
{
#ifdef DONT_USE_OUTPUT_UNICODE
	char valueCh[1024];
	char propertyCh[512];
#endif

	HRESULT hr = S_OK;
	WORD field_len = 0;

	_TCHAR formattedValue[512];
	bool ignoreOutput = false;
	int timesTransformed = 0;

	_TCHAR inputValue[512];
	_TCHAR* ptr = inputValue;
	_tcscpy(inputValue, value);
	_TCHAR* ptrprev = inputValue;
	CEntryTransformer *transformerEntry;
	CConversionRuleEntry *conversionEntry;
	_TCHAR *attachName = XML_PROP_TAG1(PR_MESSAGE_ATTACHMENTS);

	_tprintf(_T("-->at0 %s, %s<--\n"), name, value);
	memset(formattedValue, 0, sizeof(formattedValue)*sizeof(TCHAR));
	theParentApp->resetEntryTransformerListIndex();
	_tprintf(_T("-->at00 %s, %s<--\n"), name, value);
	while((transformerEntry = theParentApp->getNextEntryTransformer()) != NULL) {
		_tprintf(_T("-->at01\n"));
		int offset = 0;
		int valueLen = _tcslen(inputValue);
		bool eatNextComma = false;

		const _TCHAR* transformerName = transformerEntry->GetName();
		const _TCHAR* placeholder = transformerEntry->GetPlaceholder();
		int placeholderLen = _tcslen(placeholder);
		_tprintf(_T("-->transformerName: %s, placeholder: %s<--\n"), transformerName, placeholder);

		while ((ptr = _tcsstr(ptr, placeholder)) != NULL) {
			_tcsncpy(formattedValue+offset, ptrprev, ptr - ptrprev);
			offset += (ptr - ptrprev);
			int placeholderValueLen = 0;

			_tprintf(_T("-->at1\n"));
			if (transformerEntry->GetPstItemProperty()) {
				_TCHAR outputW[512];

				_tprintf(_T("-->at2\n"));
				static const enum {IPROP};
				ULONG pcount; 
				LPSPropValue props = NULL;
				ULONG tag = 0;

				// First check if we work with attachment name
				if (_tcscmp(transformerName, attachName) == 0) {
					if (text_to_use != NULL) {
						_tcscpy(outputW, text_to_use);
						field_len = _tcslen(outputW);
					}
				}
				// Now check if it's named property
				else if (transformerEntry->IsNamedProperty()) {
					ULONG namedPropertyHex = (ULONG)_tcstoul(transformerName, NULL, 16);
					int namedPropertyIdx = GetNamedPropertyIdx(namedPropertyHex);
					if (namedPropertyIdx != -1) {
						_tprintf(_T("-->at290: %lx<--\n"), lpNamedPropTags->aulPropTag[namedPropertyIdx]);
						tag = CHANGE_PROP_TYPE(lpNamedPropTags->aulPropTag[namedPropertyIdx], aulCalProps[namedPropertyIdx].pType);
					}
					else {
						// assume PT_BINARY type if not specified explicitly
						tag = CHANGE_PROP_TYPE(GetXMLPropTagValue(transformerName), PT_BINARY);
					}
					//tag = 0x83560102;
					//tag = PR_SUBJECT; //0x0070001E;
					SizedSPropTagArray(1, cols) = { 1, {tag} };
					hr = lpMsg->GetProps((SPropTagArray*)&cols, 0, &pcount, &props);
//					hr = HrGetOneProp(lpMsg,tag,&props);
					_tprintf(_T("-->at291: %d, %lx %lx<--\n"), pcount, tag, hr);
					if (hr != S_OK) {
						if (hr == MAPI_W_ERRORS_RETURNED) {
							if (props[0].Value.ul == MAPI_E_NOT_FOUND) {
								_tprintf(_T("-->000: MAPI_W_ERRORS_RETURNED and MAPI_E_NOT_FOUND<--\n"));
								pcount = 0;
							}
						}
					}

					if (!FAILED(hr))
						_tprintf(_T("-->at291 type: %d %d<--\n"), PROP_TYPE(props[IPROP].ulPropTag), props[IPROP].Value.ul);
				}
				// Now check if it's regular property
				else {
					SizedSPropTagArray(1, cols) = { 1, {GetXMLPropTagValue(transformerName)} };
					hr = lpMsg->GetProps((SPropTagArray*)&cols, 0, &pcount, &props);
					_tprintf(_T("-->at292: %d, %lx<--\n"), pcount, GetXMLPropTagValue(transformerName));
					if (hr != S_OK) {
						if (hr == MAPI_W_ERRORS_RETURNED) {
							if (props[0].Value.ul == MAPI_E_NOT_FOUND) {
								_tprintf(_T("-->000: MAPI_W_ERRORS_RETURNED and MAPI_E_NOT_FOUND<--\n"));
								pcount = 0;
							}
						}
					}
				}

				if (!FAILED(hr) && pcount > 0) {
					_tprintf(_T("-->at3 %d %lx<--\n"), (props != NULL), (props != NULL ? props->ulPropTag:0));
//					if (props[0].Value.lpszW != NULL) {
//						_tprintf(_T("-->at31 Uni: %s<--\n"), props[0].Value.lpszW);
//					}
//					else if (props[0].Value.lpszA != NULL) {
//						printf("-->at31 Asc: %s<--\n", props[0].Value.lpszA);
//					}

					const _TCHAR *valueModifier = NULL;
					_tprintf(_T("-->at32\n"));
					if ((valueModifier = transformerEntry->GetValueModifier()) != NULL &&
						_tcscmp(valueModifier = transformerEntry->GetValueModifier(), MODIFIER_TIMEZONENAME) == 0) {
						ULONG cb = props[IPROP].Value.bin.cb;
						_tprintf(_T("-->at320: %lx\n"), cb);
						// See here:
						// http://blogs.msdn.com/stephen_griffin/archive/2006/12/06/outlook-2007-timezone-structures.aspx
						TZDEFINITION *tzDef = BinToTZDEFINITION(
							props[IPROP].Value.bin.cb, props[0].Value.bin.lpb);
						_tprintf(_T("-->at321: %s\n"), tzDef->pwszKeyName);
						_tcscpy(outputW, tzDef->pwszKeyName);
						field_len = _tcslen(outputW);
					}
					else if ((valueModifier = transformerEntry->GetValueModifier()) != NULL &&
						_tcscmp(valueModifier = transformerEntry->GetValueModifier(), MODIFIER_16BYTES) == 0) {
						ULONG cb = props[IPROP].Value.bin.cb;
						LPTSTR lpszStr = NULL;
						MyHexFromBin(
							props[IPROP].Value.bin.lpb,
							cb > 16 ? 16 : cb,
							&lpszStr);
						if (lpszStr)
						{
							_tcscpy(outputW, lpszStr);
							delete[] lpszStr;
						}
						field_len = _tcslen(outputW);
					}
					else {
						_tprintf(_T("-->at321 type: %lx\n"), PROP_TYPE(props[IPROP].ulPropTag));
						// Here we need to deal with property type
						switch (PROP_TYPE(props[IPROP].ulPropTag)) {
							case PT_STRING8:
								{
									_tprintf(_T("-->at320: %d %d\n"), _tcslen(props[IPROP].Value.lpszW), strlen(props[IPROP].Value.lpszA));
									_tprintf(_T("-->at320 1: %s\n"), (LPTSTR)props[IPROP].Value.lpszA);
									printf("-->at320 2: %s\n", props[IPROP].Value.lpszA);
									mbstowcs(outputW, props[IPROP].Value.lpszA, strlen(props[IPROP].Value.lpszA));
									outputW[strlen(props[IPROP].Value.lpszA)] = _T('\0');
									_tprintf(_T("-->at320: %s\n"), outputW);
									field_len = _tcslen(outputW);
									break;
								}
							case PT_UNICODE:
								{
									_tcscpy(outputW, props[IPROP].Value.lpszW);
									_tprintf(_T("-->at321: %lx\n"), outputW);
									field_len = _tcslen(outputW);
									break;
								}
							case PT_SYSTIME:
								{
									_tprintf(_T("-->at321: PT_SYSTIME\n"));
									FILETIME ft = props[IPROP].Value.ft;
									SYSTEMTIME SysTime = {0};
									FileTimeToSystemTime(&ft, &SysTime);
//									PropString->Format(_T("%02d:%02d:%02d%s  %02d/%02d/%4d"),
//										(SysTime.wHour <= 12)?SysTime.wHour:SysTime.wHour-12,
//										SysTime.wMinute,
//										SysTime.wSecond,
//										(SysTime.wHour <= 12)?_T("AM"):_T("PM"),
//										SysTime.wMonth,
//										SysTime.wDay,
//										SysTime.wYear);
									if (transformerEntry->GetDateMask() != NULL) {
										if (_tcscmp(transformerEntry->GetDateMask(), DATEMASK_YYYYMMDDTHHMISSZ) == 0) {
											_stprintf(outputW, _T("%d%02d%02dT%.2d%.2d%.2dZ"),
												SysTime.wYear, SysTime.wMonth, SysTime.wDay,
												SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
											field_len = _tcslen(outputW);
										}
									}
									break;
								}
							case PT_LONG:
								{
									_stprintf(outputW, _T("%d"), props[IPROP].Value.l);
									field_len = _tcslen(outputW);
									break;
								}
							default:
								break;
						}
					}

					if (field_len == 0) { // for testing
						field_len = _tcslen(_T("test")); //props[0].Value.lpszW);
						_tcscpy(outputW, _T("test")); //props[0].Value.lpszW);
					}
					_tprintf(_T("-->at33\n"));

					// now check if any lotus field conversion is needed
					theParentApp->resetConversionRuleIndex();
					while ((conversionEntry = theParentApp->getNextConversionRuleEntry()) != NULL) {
						if (!conversionEntry->IsDefault()) {
							// not case sensitive right now !!!
//							_tprintf(_TEXT("-->conversion: %s %s\n"), outputW, conversionEntry->GetInput());
							if (_tcsicmp(outputW, conversionEntry->GetInput()) == 0) {
								_tcscpy(outputW, conversionEntry->GetOutput());
								delete(conversionEntry);
								break;
							}
						}
						delete(conversionEntry);
					}
				
					placeholderValueLen = _tcslen(outputW);
					if (placeholderValueLen == 0) {
						eatNextComma = true;
					}
					_tprintf(_T("-->at34\n"));
					_tcsncpy(formattedValue+offset, outputW, placeholderValueLen);
					_tprintf(_T("-->at35\n"));
					offset+=placeholderValueLen;
				}
				else {
					_tprintf(_T("-->at4\n"));
					offset+=placeholderValueLen;
				}
			}
			else {
				const _TCHAR* configSettingValue = NULL;
				configSettingValue = theParentApp->getConfigSettingValue(transformerName);
				placeholderValueLen = _tcslen(configSettingValue);
				_tcsncpy(formattedValue+offset, configSettingValue, placeholderValueLen);
				offset+=placeholderValueLen;
			}
			timesTransformed++;
			ptr = ptr + placeholderLen;
			ptrprev = ptr;
		}
		// remove comma or space after empty value
		if (ptrprev != _T('\0') && _tcslen(ptrprev)>0) {
			if ((*ptrprev == _T(',') && (eatNextComma || offset == 0)) ||
				(*ptrprev == _T(' ') && (eatNextComma || offset == 0))) {
				_tcsncpy(formattedValue+offset, ptrprev+1, _tcslen(ptrprev+1));
				*(formattedValue+offset+_tcslen(ptrprev+1)) = _T('\0');
			}
			else {
				_tcsncpy(formattedValue+offset, ptrprev, _tcslen(ptrprev));
				*(formattedValue+offset+_tcslen(ptrprev)) = _T('\0');
			}
		}
		else {
			*(formattedValue+offset) = _T('\0');
		}
		if (transformerEntry->IsBlankIfAbsent() && _tcscmp(formattedValue, _TEXT("=\?utf-8?B\?\?=")) == 0) {
			*(formattedValue) = _T('\0');
		}
		_tcscpy(inputValue, formattedValue);
		ptr = inputValue;
		ptrprev = ptr;
		delete(transformerEntry);
	}

		_tprintf(_T("-->at5\n"));
		if (!ignoreOutput) {
			if (timesTransformed == 0) {
//				_ftprintf(f, mask2, attribute, value);
				if (name == NULL || _tcslen(name) == 0) {
#ifndef DONT_USE_OUTPUT_UNICODE
					_ftprintf(outputFile, _TEXT("%s\r\n"), value);
#else
					wcstombs(valueCh, value, sizeof(valueCh));
					fprintf(outputFile, "%s\r\n", valueCh);
#endif
				}
				else {
					if (_tcscmp(name, _T("Cc")) == 0) {
						// skip it
					}
					else {
#ifndef DONT_USE_OUTPUT_UNICODE
						_ftprintf(outputFile, _TEXT("%s: %s\r\n"), name, value);
#else
						wcstombs(valueCh, value, sizeof(valueCh));
						wcstombs(propertyCh, name, sizeof(propertyCh));
						fprintf(outputFile, "%s: %s\r\n", propertyCh, valueCh);
#endif
					}
				}
			}
			else {
//				if (m_verbose){
//					::LoadString(hInst, IDS_PROCESSING_CHOICE, usage, 256);
//					::_tprintf(_TEXT("%s formattedValue: %s\n"), usage, formattedValue);
//				}
//				_ftprintf(f, mask2, attribute, formattedValue);
				if (name == NULL || _tcslen(name) == 0) {
#ifndef DONT_USE_OUTPUT_UNICODE
					_ftprintf(outputFile, _TEXT("%s\r\n"), formattedValue);
#else
					wcstombs(valueCh, formattedValue, sizeof(valueCh));
					fprintf(outputFile, "%s\r\n", valueCh);
#endif
				}
				else {
#ifndef DONT_USE_OUTPUT_UNICODE
					_ftprintf(outputFile, _TEXT("%s: %s\r\n"), name, formattedValue);
#else
					wcstombs(valueCh, formattedValue, sizeof(valueCh));
					wcstombs(propertyCh, name, sizeof(propertyCh));
					fprintf(outputFile, "%s: %s\r\n", propertyCh, valueCh);
#endif
				}
			}
		}

cleanup:
	return hr;
}

void CPSTProc::DoSmtpLookup(_TCHAR *field_text, WORD *field_len) 
{
//	char field_textUTF8[512];
	// we could have here Internet email, instead of Lotus DN
	_TCHAR field_text_orig[128];
	_tcscpy(field_text_orig, field_text);
	if (field_text[0] == '<') {
		_tcscpy(field_text, field_text_orig + 1);
		int len = _tcslen(field_text);
		if (field_text[len - 1] == '>')
			field_text[len - 1] = '\0';
	}
	if (this->GetSmtpAddrFile() != NULL && _tcslen(this->GetSmtpAddrFile()) > 0) {
		LIST *item;
		_TCHAR usage[256];
		HINSTANCE hInst = ::GetModuleHandle(NULL);
		if (m_verbose) {
			::LoadString(hInst, IDS_SMTP_NAME_FOR_LOOKUP, usage, 256);
			::_tprintf(_TEXT("%s "), usage);
			printf("%s\n", field_text);
		}

		_TCHAR field_textW[128];
//	#ifndef OUTLOOK_UNICODE
//		OSTranslate(OS_TRANSLATE_LMBCS_TO_UTF8, field_text, strlen(field_text), field_textUTF8, 128);
//		mbstowcs(field_textW, field_textUTF8, 128);
//	//							::_tprintf(_T("-->1 %s<--\n"), field_textW);
//	#else
//		OSTranslate(OS_TRANSLATE_LMBCS_TO_UNICODE, field_text, strlen(field_text), field_textUTF8, 128);
//		_tcscpy(field_textW, (_TCHAR *)field_textUTF8);
//	//							::_tprintf(_T("-->2 %s<--\n"), field_textW);
//	#endif
		_tcscpy(field_textW, field_text);

		item = smtpAdr->LST_Lookup(&notFoundLst, field_textW);
		if (item != NULL) {
			if (m_verbose) {
				::LoadString(hInst, IDS_SMTP_NAME_UNRESOLVED, usage, 256);
				::_tprintf(_TEXT("%s "), usage);
				printf("%s\n", field_text);
			}
		}
		else {
			item = smtpAdr->LST_Lookup(&lst, field_textW, false);
			if (item != NULL) {
				_tcscpy(field_textW, item->data);
//				wcstombs(field_text, field_textW, _tcslen(field_textW));
//				field_text[_tcslen(field_textW)] = '\0';
				_tcscpy(field_text, field_textW);
				*field_len = _tcslen(item->data);
				if (m_verbose) {
					::LoadString(hInst, IDS_SMTP_NAME_SUBSTITUTED, usage, 256);
					::_tprintf(_TEXT("%s %s\n"), usage, field_textW);
				}
			}
			else {
				if (m_verbose) {
					::LoadString(hInst, IDS_SMTP_NAME_ADDTOUNRESOLVED, usage, 256);
					::_tprintf(_TEXT("%s %s\n"), usage, field_textW);
				}
				smtpAdr->LST_LookupAdd(&notFoundLst, field_textW, ENTRY_LEN);
			}
		}
	}

}

bool CPSTProc::IsIgnoredFolder(_TCHAR *ignoredFolders, _TCHAR *folderName)
{
	_TCHAR allIgnoredFolders[1024];
	_tcscpy(allIgnoredFolders, ignoredFolders);
	_TCHAR* folderToken = _tcstok(allIgnoredFolders, _T(","));
	bool ignored = false;
	while (folderToken != NULL) {
		if (_tcscmp(folderToken, folderName) == 0) {
			ignored = true;
			break;
		}
		folderToken = _tcstok(NULL, _T(","));
	}
	return ignored;
}

void CPSTProc::parseSMTPFile(const _TCHAR *smtpAddrFile, const _TCHAR *smtpPartsSep)
{
	_TCHAR usage[256];
	HINSTANCE hInst = ::GetModuleHandle(NULL);
	_TCHAR current_method[] = _TEXT("CPSTProc::parseSMTPFile");
	if (m_verbose){
		::LoadString(hInst, IDS_ENTER_METHOD, usage, 256);
		::_tprintf(_TEXT("%s %s\n"), usage, current_method);
	}
	smtpAdr = new CList();
	FILE *fin = NULL;
	_TCHAR buf[BUFFER_SIZE];
	_TCHAR entry[ENTRY_LEN];
	int bread;
	unsigned long offset = 0;
	int found = 0;
	_TCHAR *ptr1, *ptr2, *ptr3;
	_TCHAR delimPartsSep[32];
	_TCHAR delimLineBreak[] = _T("\r\n");
	int lend1 = 0;
	int lend2 = 0;
	unsigned long line = 1;
	_TCHAR end = _T('\0');
	LIST *item;

	if (smtpAddrFile == NULL || _tcslen(smtpAddrFile) == 0) {
		INI_LogWriteWithResourcePrefix(IDS_LOG_SMTPADR_IGNOREEMPTY, _T(""));
		goto cleanup;
	}

//	wcstombs(smtpAddrFileCh, smtpAddrFile, _tcslen(smtpAddrFile));
//	smtpAddrFileCh[_tcslen(smtpAddrFile)] = '\0';
//	wcstombs(delimPartsSep, smtpPartsSep, _tcslen(smtpPartsSep));
//	delimPartsSep[_tcslen(smtpPartsSep)] = '\0';
	delimPartsSep[0] = _T('\0');
	_tcscpy(delimPartsSep, smtpPartsSep);
	lend1 = _tcslen(delimPartsSep);
	lend2 = _tcslen(delimLineBreak);
	
	fin = _tfopen(smtpAddrFile, _T("rb"));
	if (fin == NULL) {
		_tprintf(_T("Error opening file: %s"), smtpAddrFile);
		goto cleanup;
	}

	/*
	ptr1 - pointer to the begin of our two entries
	ptr2 - pointer to the end of our two entries
	ptr3 - pointer to the SMTP part of entries
	*/
	entry[0] = end;
	while (!feof(fin)) {
		bread = fread(buf, 1, BUFFER_SIZE - 1, fin);
		if (bread == 0)
			break;
		offset += bread;
		buf[bread] = end;
		ptr1 = buf;
#ifdef _UNICODE
		if (line == 1) {
			if (buf[0] == 0xfeff)
				ptr1 = buf + 1;
			else {
				::LoadString(hInst, IDS_LOG_SMTPADR_SKIP, usage, 256);
				_tprintf(_T("%s\n"), usage);
				INI_LogWriteWithResourcePrefix(IDS_LOG_SMTPADR_SKIP, _T(""));
				fclose(fin);
				return;
			}
		}
#endif
		ptr3 = _tcsstr(ptr1, delimLineBreak);
		if (ptr3 == NULL) {
			if (bread >= ENTRY_LEN - 1 || offset >= ENTRY_LEN - 1) {
				// Error
				found = 0;
				break;
			}
			else {
				_tcscat(entry, buf);
				ptr3 = _tcsstr(entry, delimLineBreak);
				if (ptr3 != NULL) {
					entry[ptr3-entry] = end;
					ptr2 = _tcsstr(entry, delimPartsSep);
					if ((ptr2 != NULL) && (_tcslen(ptr2 + lend1) != 0)) {
//						fseek(fin, (int)((ptr3 - entry) - offset + lend2), SEEK_CUR);
						fseek(fin, (long)(((byte *)ptr3 - (byte *)entry) - offset + 
							((byte *)(delimLineBreak + lend2) - (byte *)delimLineBreak)), SEEK_CUR);
#ifdef _UNICODE
						if (buf[0] == 0xfeff)
							fseek(fin, 2, SEEK_CUR);
#endif
						offset = 0;
						found = 1;
						line++;
						*(ptr1+(ptr2-entry)) = end;
						entry[0] = end;
						item = smtpAdr->LST_LookupAdd(&lst, ptr1, ENTRY_LEN);
						if (item != NULL) {
							_tcscpy(item->data, ptr2 + lend1);
							_tprintf(_T("key: %s, value: %s\n"), ptr1, ptr2 + lend1);
						}
					}
					else {
						found = 0;
						::LoadString(hInst, IDS_ERR_SMTP_NO_DELIM, usage, 256);
						::_tprintf(_TEXT("%s %s"), usage, entry);
						fseek(fin, (long)(((byte *)ptr3 - (byte *)entry) - offset + 
							((byte *)(delimLineBreak + lend2) - (byte *)delimLineBreak)), SEEK_CUR);
#ifdef _UNICODE
						if (buf[0] == 0xfeff)
							fseek(fin, 2, SEEK_CUR);
#endif						
						offset = 0;
					}
				}
			}
		}
		else {
			if (ptr3 - ptr1 >= ENTRY_LEN - 1) {
				// Error
				found = 0;
				break;
			}
			else {
				*(ptr1 + (ptr3 - ptr1)) = end;
				_tcscat(entry, ptr1);
				ptr2 = _tcsstr(entry, delimPartsSep);
				if ((ptr2 != NULL) && (_tcslen(ptr2 + lend1) != 0)) {
					fseek(fin, (long)(((byte *)ptr3 - (byte *)ptr1) - bread + ((byte *)(delimLineBreak + lend2) - (byte *)delimLineBreak)), SEEK_CUR);
#ifdef _UNICODE
					if (buf[0] == 0xfeff)
						fseek(fin, 2, SEEK_CUR);
#endif
					offset = 0;
					found = 1;
					line++;
					*(ptr1+(ptr2-entry)) = end;
					entry[0] = end;
					item = smtpAdr->LST_LookupAdd(&lst, ptr1, ENTRY_LEN);
					if (item != NULL) {
						_tcscpy(item->data, ptr2 + lend1);
						_tprintf(_T("key: %s, value: %s\n"), ptr1, ptr2 + lend1);
					}
				}
				else {
					found = 0;
					::LoadString(hInst, IDS_ERR_SMTP_NO_DELIM, usage, 256);
					::_tprintf(_TEXT("%s %s"), usage, entry);
					fseek(fin, (long)(((byte *)ptr3 - (byte *)ptr1) - bread + ((byte *)(delimLineBreak + lend2) - (byte *)delimLineBreak)), SEEK_CUR);
#ifdef _UNICODE
					if (buf[0] == 0xfeff)
						fseek(fin, 2, SEEK_CUR);
#endif						
					offset = 0;
				}
			}
		}
	}

cleanup:
	if (fin != NULL)
		fclose(fin);
	if (m_verbose){
		::LoadString(hInst, IDS_EXIT_METHOD, usage, 256);
		::_tprintf(_TEXT("%s %s\n"), usage, current_method);
	}
}

ULONG CPSTProc::GetXMLPropTagValue(const _TCHAR *propTag)
{
	ULONG found = 0;
	for (int i=0; i<sizeof(PropTags)/sizeof(PropTag); i++) {
		if (_tcscmp((PropTags + i)->prop, propTag) == 0) {
			found = (PropTags + i)->value;
			break;
		}
	}
	// If we need to set special not well known MAPI properties
	if (!found) {
		_tprintf(_T("-->before tcstol: %s\n"), propTag);
		ULONG l1 = (ULONG)_tcstol(propTag, NULL, 0);
		_tprintf(_T("-->after tcstol: %u\n"), l1);
		if (l1 > 0) found = l1;
	}
	return found;
}

int CPSTProc::GetNamedPropertyIdx(ULONG namedProperty)
{
	for (int i = 0; i<ulCalProps; i++) {
		if (aulCalProps[i].pID == namedProperty)
			return i;
	}
	return -1;
}

// Allocates return value with new.
// clean up with delete[].
TZDEFINITION* CPSTProc::BinToTZDEFINITION(ULONG cbDef, LPBYTE lpbDef)
{
    if (!lpbDef) return NULL;

    // Update this if parsing code is changed!
    // this checks the size up to the flags member
    if (cbDef < 2*sizeof(BYTE) + 2*sizeof(WORD)) return NULL;

    TZDEFINITION tzDef = {0};
    TZRULE* lpRules = NULL;
    LPBYTE lpPtr = lpbDef;
    WORD cchKeyName = NULL;
    WCHAR* szKeyName = NULL;
    WORD i = 0;

    BYTE bMajorVersion = *((BYTE*)lpPtr);
    lpPtr += sizeof(BYTE);
    BYTE bMinorVersion = *((BYTE*)lpPtr);
    lpPtr += sizeof(BYTE);

    // We only understand TZ_BIN_VERSION_MAJOR
    if (TZ_BIN_VERSION_MAJOR != bMajorVersion) return NULL;

    // We only understand if >= TZ_BIN_VERSION_MINOR
    if (TZ_BIN_VERSION_MINOR > bMinorVersion) return NULL;

    WORD cbHeader = *((WORD*)lpPtr);
    lpPtr += sizeof(WORD);

    tzDef.wFlags = *((WORD*)lpPtr);
    lpPtr += sizeof(WORD);

    if (TZDEFINITION_FLAG_VALID_GUID & tzDef.wFlags)
    {
        if (lpbDef + cbDef - lpPtr < sizeof(GUID)) return NULL;
        tzDef.guidTZID = *((GUID*)lpPtr);
        lpPtr += sizeof(GUID);
    }

    if (TZDEFINITION_FLAG_VALID_KEYNAME & tzDef.wFlags)
    {
        if (lpbDef + cbDef - lpPtr < sizeof(WORD)) return NULL;
        cchKeyName = *((WORD*)lpPtr);
        lpPtr += sizeof(WORD);
        if (cchKeyName)
        {
            if (lpbDef + cbDef - lpPtr < (BYTE)sizeof(WORD)*cchKeyName) return NULL;
            szKeyName = (WCHAR*)lpPtr;
            lpPtr += cchKeyName*sizeof(WORD);
        }
    }

    if (lpbDef+ cbDef - lpPtr < sizeof(WORD)) return NULL;
    tzDef.cRules = *((WORD*)lpPtr);
    lpPtr += sizeof(WORD);    if (tzDef.cRules)
    {
        lpRules = new TZRULE[tzDef.cRules];
        if (!lpRules) return NULL;

        LPBYTE lpNextRule = lpPtr;
        BOOL bRuleOK = false;
		
        for (i = 0;i < tzDef.cRules;i++)
        {
            bRuleOK = false;
            lpPtr = lpNextRule;
			
            if (lpbDef + cbDef - lpPtr < 
                2*sizeof(BYTE) + 2*sizeof(WORD) + 3*sizeof(long) + 2*sizeof(SYSTEMTIME)) return NULL;
            bRuleOK = true;
            BYTE bRuleMajorVersion = *((BYTE*)lpPtr);
            lpPtr += sizeof(BYTE);
            BYTE bRuleMinorVersion = *((BYTE*)lpPtr);
            lpPtr += sizeof(BYTE);
			
            // We only understand TZ_BIN_VERSION_MAJOR
            if (TZ_BIN_VERSION_MAJOR != bRuleMajorVersion) return NULL;
			
            // We only understand if >= TZ_BIN_VERSION_MINOR
            if (TZ_BIN_VERSION_MINOR > bRuleMinorVersion) return NULL;
			
            WORD cbRule = *((WORD*)lpPtr);
            lpPtr += sizeof(WORD);
			
            lpNextRule = lpPtr + cbRule;
			
            lpRules[i].wFlags = *((WORD*)lpPtr);
            lpPtr += sizeof(WORD);
			
            lpRules[i].stStart = *((SYSTEMTIME*)lpPtr);
            lpPtr += sizeof(SYSTEMTIME);
			
            lpRules[i].TZReg.lBias = *((long*)lpPtr);
            lpPtr += sizeof(long);
            lpRules[i].TZReg.lStandardBias = *((long*)lpPtr);
            lpPtr += sizeof(long);
            lpRules[i].TZReg.lDaylightBias = *((long*)lpPtr);
            lpPtr += sizeof(long);
			
            lpRules[i].TZReg.stStandardDate = *((SYSTEMTIME*)lpPtr);
            lpPtr += sizeof(SYSTEMTIME);
            lpRules[i].TZReg.stDaylightDate = *((SYSTEMTIME*)lpPtr);
            lpPtr += sizeof(SYSTEMTIME);
        }
        if (!bRuleOK)
        {
            delete[] lpRules;
            return NULL;			
        }
    }    // Now we've read everything - allocate a structure and copy it in
    size_t cbTZDef = sizeof(TZDEFINITION) +
        sizeof(WCHAR)*(cchKeyName+1) +
        sizeof(TZRULE)*tzDef.cRules;

    TZDEFINITION* ptzDef = (TZDEFINITION*) new BYTE[cbTZDef];
    
    if (ptzDef)
    {
        // Copy main struct over
        *ptzDef = tzDef;
        lpPtr = (LPBYTE) ptzDef;
        lpPtr += sizeof(TZDEFINITION);

        if (szKeyName)
        {
            ptzDef->pwszKeyName = (WCHAR*)lpPtr;
            memcpy(lpPtr,szKeyName,cchKeyName*sizeof(WCHAR));
            ptzDef->pwszKeyName[cchKeyName] = 0;
    
            lpPtr += (cchKeyName+1)*sizeof(WCHAR);
        }

        if (ptzDef -> cRules)
        {
            ptzDef -> rgRules = (TZRULE*)lpPtr;
            for (i = 0;i < ptzDef -> cRules;i++)
            {
                ptzDef -> rgRules[i] = lpRules[i];
            }
        }
    }
    delete[] lpRules;

    return ptzDef;
}

//returns pointer to a string
//delete with delete[]
void CPSTProc::MyHexFromBin(LPBYTE lpb, size_t cb, LPTSTR* lpsz)
{
	ULONG i = 0;
	ULONG iBinPos = 0;
	if (!lpb || !cb)
	{
//		DebugPrint(DBGGeneric, _T("MyHexFromBin called with null lpb or null cb\n"));
		return;
	}
	*lpsz = new TCHAR[1+2*cb];
	if (*lpsz)
	{
		memset(*lpsz, 0, 1+2*cb);
		for (i = 0; i < cb; i++)
		{
			BYTE bLow;
			BYTE bHigh;
			TCHAR szLow;
			TCHAR szHigh;
			
			bLow = (BYTE) ((lpb[i]) & 0xf);
			bHigh = (BYTE) ((lpb[i] >> 4) & 0xf);
			szLow = (TCHAR) ((bLow <= 0x9) ? _T('0') + bLow : _T('A') + bLow - 0xa);
			szHigh = (TCHAR) ((bHigh <= 0x9) ? _T('0') + bHigh : _T('A') + bHigh - 0xa);
			
			(*lpsz)[iBinPos] = szHigh;
			(*lpsz)[iBinPos+1] = szLow;
			
			iBinPos += 2;
		}
		(*lpsz)[iBinPos] = _T('\0');
	}
}

HRESULT CPSTProc::PutCalendarHeader(FILE *outputFile)
{
	CCalendarItem *item;
	HRESULT error = S_OK;
	theParentApp->resetEntryListIndex();
	while ((item = theParentApp->GetNextCalendarItem(CONDITION_CALHEADER)) != NULL) {
		error = ApplyTransformers(NULL, item->GetName(), item->GetValue(), outputFile);
		if (error)
			break;
		delete(item);
	}
	return error;
}

HRESULT CPSTProc::PutEventFooter(LPMESSAGE lpMsg, FILE *outputFile)
{
	CCalendarItem *item;
	HRESULT error = S_OK;
	theParentApp->resetEntryListIndex();
	while ((item = theParentApp->GetNextCalendarItem(CONDITION_EVENTFOOTER)) != NULL) {
		error = ApplyTransformers(lpMsg, item->GetName(), item->GetValue(), outputFile);
		if (error)
			break;
		delete(item);
	}
	return error;
}

HRESULT CPSTProc::PutCalendarFooter(FILE *outputFile)
{
	CCalendarItem *item;
	HRESULT error = S_OK;
	theParentApp->resetEntryListIndex();
	while ((item = theParentApp->GetNextCalendarItem(CONDITION_CALFOOTER)) != NULL) {
		error = ApplyTransformers(NULL, item->GetName(), item->GetValue(), outputFile);
		if (error)
			break;
		delete(item);
	}
	return error;
}

HRESULT CPSTProc::ProcessAttendee(CCalendarItem *item, LPMESSAGE lpMsg, FILE* outputFile)
{
	HRESULT error = S_OK;
	_TCHAR text_to_use[256];
	int entryIndex = 0;
//	theParentApp->resetEntryListIndex();

	text_to_use[0] = _T('\0');
	do {
		if (error = ApplyTransformers(lpMsg, item->GetName(), item->GetValue(), outputFile, 
			text_to_use, &entryIndex))
			goto cleanup;
	} while (_tcslen(text_to_use) > 0 && entryIndex != -1);

cleanup:
	// after attendees handling this entry list will be broken
//	theParentApp->InitializeCalendarItemList();
	return error;
}

HRESULT CPSTProc::ProcessAttachments(LPMESSAGE lpMsg, FILE* outputFile, bool tab)
{
	HRESULT error = S_OK;
	CCalendarItem *item = NULL;

	SizedSPropTagArray(1,g_sptMsgProps) = {1,
									 PR_HASATTACH};

	LPSPropValue pProps = NULL;
	ULONG cVals = 0;

	if (FAILED(error = lpMsg->GetProps((LPSPropTagArray) &g_sptMsgProps,
							   0,
							   &cVals,
							   &pProps)))
		goto cleanup;
	else
		error = S_OK;

	if (PR_HASATTACH == pProps[0].ulPropTag && pProps[0].Value.b) {
		LPMAPITABLE pAttTbl = NULL;
		LPSRowSet pRows = NULL;
		static SizedSPropTagArray(2,sptCols) = {2,PR_ATTACH_LONG_FILENAME,
												  PR_ATTACH_NUM};

		if (SUCCEEDED(error = lpMsg -> OpenProperty(PR_MESSAGE_ATTACHMENTS,
			&IID_IMAPITable,
			0,
			0,
			(LPUNKNOWN *) &pAttTbl))) {
			if (SUCCEEDED(error = pAttTbl -> SetColumns(
				(LPSPropTagArray) &sptCols,
				TBL_BATCH))) {
				if (SUCCEEDED(error = HrQueryAllRows(pAttTbl,
					(LPSPropTagArray) &sptCols,
					NULL,
					NULL,
					0,
					&pRows))) {
					for (ULONG i = 0; i < pRows -> cRows; i++) {
						LPATTACH lpAttach = NULL;

						// Verify we received a filename from GetProps
						if (! PR_ATTACH_FILENAME ==
							pRows->aRow[i].lpProps[0].ulPropTag)
							break;

						// Verify we received an Attachment Index from GetProps
						if (! PR_ATTACH_NUM == pRows->aRow[i].lpProps[1].ulPropTag)
							break;

						// Open the attachment
						if (SUCCEEDED(error = lpMsg->OpenAttach (
							pRows->aRow[i].lpProps[1].Value.l,
							NULL, MAPI_BEST_ACCESS, &lpAttach))) {
							LPSTREAM pStrmSrc = NULL, pStrmDest = NULL;
							STATSTG StatInfo;

							// Open the property of the attachment
							// containing the file data
							if (FAILED(error = lpAttach->OpenProperty(
								PR_ATTACH_DATA_BIN,
								(LPIID)&IID_IStream,
								0,
								MAPI_MODIFY,
								(LPUNKNOWN *)&pStrmSrc)))
								break;

								theParentApp->resetEntryListIndex();
								while ((item = theParentApp->GetNextCalendarItem(CONDITION_ATTACHMENTPART)) != NULL) {
									if (error = ApplyTransformers(lpMsg, item->GetName(), item->GetValue(), outputFile, pRows->aRow[i].lpProps[0].Value.lpszW))
										break;
									delete(item);
								}

//							// Open an IStream interface and create the file at the
//							// same time. This code will create the file in the
//							// current directory.
//							if (FAILED(error = OpenStreamOnFile(
//								MAPIAllocateBuffer,
//								MAPIFreeBuffer,
//								STGM_CREATE | STGM_READWRITE,
////								pRows->aRow[i].lpProps[0].Value.lpszA,
//								pRows->aRow[i].lpProps[0].Value.lpszW,
//								NULL,
//								&pStrmDest)))
//								break;
//
//							pStrmSrc -> Stat(&StatInfo, STATFLAG_NONAME);
//
//							error = pStrmSrc -> CopyTo(pStrmDest,
//								StatInfo.cbSize,
//								NULL,
//								NULL);
//
//							LARGE_INTEGER dlibMove = {0,0};
//							ULARGE_INTEGER plibNewPosition;
//							pStrmDest -> Seek(dlibMove, STREAM_SEEK_SET, &plibNewPosition);
//
//							// Commit changes to new stream
//							pStrmDest -> Commit(0);
//
//							// Release each of our streams
//							pStrmDest -> Release();

							ULONG bRead = 0;
							unsigned char bytes[8192];
							while (true) {
								error = pStrmSrc ->Read(bytes, sizeof(bytes), &bRead);
								if (FAILED(error))
									break;
								if (bRead == 0)
									break;
								to64buf(bytes, bRead, outputFile, 0);
							}

							pStrmSrc -> Release();
						}

						// Release the attachment
						lpAttach -> Release();
					}
				}
			}
		}

		FreeProws(pRows);

		if (pAttTbl)
			pAttTbl -> Release();
	}

cleanup:
	MAPIFreeBuffer((LPVOID) pProps);
	return error;
}