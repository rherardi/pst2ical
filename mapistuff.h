#ifndef _MAPISTUFF_H_
#define _MAPISTUFF_H_

#include <mapix.h>
#include <mapiutil.h>
#include <edk.h>
//#include <edkmapi.h>
//#include <edkmdb.h>

// Important folder
#define MAPI_FOLDER_TOP _T("Top of Personal Folders")
#define MAPI_FOLDER_CALENDAR _T("Top of Personal Folders\\Calendar")

#define PROFILE_NAME _T("PST2ICAL profile")
#define PST_DEFAULT_SUFFIX _T(".pst")

// From Outlook Spy tool
#define PR_FOLDER_DN_W 0x6700001F

//#ifdef __cplusplus
//extern "C" {
//#endif
	
BOOL CreateProfileWithIProfAdmin(_TCHAR *szProfile, _TCHAR *szDataFile);
BOOL DeleteProfileWithIProfAdmin(_TCHAR *szProfile);
LPMDB OpenDefaultStore(LPMAPISESSION pses);
LPMAPIFOLDER OpenMsgStoreFolderByName(LPMDB	lpMDB, LPTSTR lpszFolder);
BOOL DeleteFolder(LPMAPIFOLDER lpfRootF, _TCHAR *szSubfolderName);

//#ifdef __cplusplus
//} /* closing brace for extern "C" */
//#endif

#endif
