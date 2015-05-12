// See http://support.microsoft.com/kb/306962 for MAPI profile creation
#include <windows.h>

#include "mapistuff.h"
#include "inistuff.h"
#include "resource.h"

// CreateProfileWithIProfAdmin function: This uses the MAPI IProfAdmin to 
// programmatically create a profile. No UI is displayed.
BOOL CreateProfileWithIProfAdmin(_TCHAR *szProfile, _TCHAR *szDataFile)
{
    HRESULT         hRes = S_OK;            // Result from MAPI calls.
    LPPROFADMIN     lpProfAdmin = NULL;     // Profile Admin object.
    LPSERVICEADMIN  lpSvcAdmin = NULL;      // Service Admin object.
    LPMAPITABLE     lpMsgSvcTable = NULL;   // Table to hold services.
    LPSRowSet       lpSvcRows = NULL;       // Rowset to hold results of table query.
    SPropValue      rgval[2];               // Property structure to hold values we want to set.
    SRestriction    sres;                   // Restriction structure.
    SPropValue      SvcProps;               // Property structure for restriction.
	ULONG			ulFlags = MAPI_UNICODE; // should be 0 if not UNICODE needed
	BOOL result = TRUE;

	INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_PROFILECR, _T("-->1"));
    // This indicates columns we want returned from HrQueryAllRows.
    enum {iSvcName, iSvcUID, cptaSvc};
    SizedSPropTagArray(cptaSvc,sptCols) = { cptaSvc, PR_SERVICE_NAME, PR_SERVICE_UID };

//	_tprintf(_T("-->szProfile: %s, szDataDir: %s, szDataFile: %s\n"), szProfile, szDataDir, szDataFile);

    // Initialize MAPI.
	// For this to work properly make sure that in Internet Explorer options
	// you have specified Outlook as default mail client!
    if (FAILED(hRes = MAPIInitialize(NULL)))
    {
//        cout<<"Error initializing MAPI.";
        goto error;
    }

    // Get an IProfAdmin interface.
	INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_PROFILECR, _T("-->2"));

//	_tprintf(_T("-->CreateProfile1\n"));
	if (FAILED(hRes = MAPIAdminProfiles(0,              // Flags.
                                        &lpProfAdmin))) // Pointer to new IProfAdmin.
    {
//        cout<<"Error getting IProfAdmin interface.";
        goto error;
    }

//	_tprintf(_T("-->CreateProfile2\n"));
    // Create a new profile.
	// MAPI_UNICODE flag doesn't work!
	INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_PROFILECR, _T("-->3"));
#if defined(UNICODE)
	char szProfileA[128];
	wcstombs(szProfileA, szProfile, _tcslen(szProfile));
	szProfileA[_tcslen(szProfile)] = '\0';
	INI_LogWriteWithResourcePrefixA(IDS_LOG_MAPI_PROFILECR, szProfileA);
//	WideCharToMultiByte( CP_ACP, 0, szProfile, -1, szProfileA, sizeof(szProfileA), NULL, NULL);
    if (FAILED(hRes = lpProfAdmin->CreateProfile((LPTSTR)szProfileA,     // Name of new profile.
                                                 NULL,          // Password for profile.
                                                 NULL,          // Handle to parent window.
                                                 0 )))        // Flags.
#else
    if (FAILED(hRes = lpProfAdmin->CreateProfile(szProfile,     // Name of new profile.
                                                 NULL,          // Password for profile.
                                                 NULL,          // Handle to parent window.
                                                 0 )))        // Flags.
#endif
    {
//        cout<<"Error creating profile.";
        goto error;
    }

    // Get an IMsgServiceAdmin interface off of the IProfAdmin interface.

//	_tprintf(_T("-->CreateProfile3\n"));
	INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_PROFILECR, _T("-->4"));
#if defined(UNICODE)
    if (FAILED(hRes = lpProfAdmin->AdminServices((LPTSTR)szProfileA,     // Profile that we want to modify.
                                                 NULL,          // Password for that profile.
                                                 NULL,          // Handle to parent window.
                                                 0,             // Flags.
                                                 &lpSvcAdmin))) // Pointer to new IMsgServiceAdmin.
#else
    if (FAILED(hRes = lpProfAdmin->AdminServices(szProfile,     // Profile that we want to modify.
                                                 NULL,          // Password for that profile.
                                                 NULL,          // Handle to parent window.
                                                 0,             // Flags.
                                                 &lpSvcAdmin))) // Pointer to new IMsgServiceAdmin.
#endif
    {
//        cout<<"Error getting IMsgServiceAdmin interface.";
        goto error;
    }

    // Create the new message service for Exchange.

//	_tprintf(_T("-->CreateProfile4\n"));
	INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_PROFILECR, _T("-->5"));
#if defined(UNICODE)
    if (FAILED(hRes = lpSvcAdmin->CreateMsgService((LPTSTR)"MSUPST MS",     // Name of service from MAPISVC.INF. Was: MSEMS
                                                   NULL,        // Display name of service.
                                                   NULL,        // Handle to parent window.
                                                   0)))      // Flags.
#else
    if (FAILED(hRes = lpSvcAdmin->CreateMsgService(L"MSUPST MS",     // Name of service from MAPISVC.INF. Was: MSEMS
                                                   NULL,        // Display name of service.
                                                   NULL,        // Handle to parent window.
                                                   0)))      // Flags.
#endif
    {
//        cout<<"Error creating Exchange message service.";
        goto error;
    }
        
    // You now have to obtain the entry id for the new service.
    // You can do this by getting the message service table
    // and getting the entry that corresponds to the new service.

//	_tprintf(_T("-->CreateProfile5\n"));
	INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_PROFILECR, _T("-->6"));
    if (FAILED(hRes = lpSvcAdmin->GetMsgServiceTable(0,                 // Flags.
                                                     &lpMsgSvcTable)))  // Pointer to table.
    {
//        cout<<"Error getting Message Service Table.";
        goto error;
    }

//	_tprintf(_T("-->CreateProfile6\n"));
    // Set up restriction to query table.

#if defined(UNICODE)
    sres.rt = RES_CONTENT;
    sres.res.resContent.ulFuzzyLevel = FL_FULLSTRING;
    sres.res.resContent.ulPropTag = PR_SERVICE_NAME_W;
    sres.res.resContent.lpProp = &SvcProps;

    SvcProps.ulPropTag = PR_SERVICE_NAME_W;
    SvcProps.Value.lpszW = _T("MSUPST MS"); // MSEMS
#else
    sres.rt = RES_CONTENT;
    sres.res.resContent.ulFuzzyLevel = FL_FULLSTRING;
    sres.res.resContent.ulPropTag = PR_SERVICE_NAME;
    sres.res.resContent.lpProp = &SvcProps;

    SvcProps.ulPropTag = PR_SERVICE_NAME;
    SvcProps.Value.lpszA = "MSUPST MS"; // MSEMS
#endif

    // Query the table to obtain the entry for the newly created message service.

	INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_PROFILECR, _T("-->7"));
    if (FAILED(hRes = HrQueryAllRows(lpMsgSvcTable,
                                     (LPSPropTagArray)&sptCols,
                                     &sres,
                                     NULL,
                                     0,
                                     &lpSvcRows)))
    {
//        cout<<"Error querying table for new message service.";
        goto error;
    }

//	_tprintf(_T("-->CreateProfile60 %d\n"), lpSvcRows->cRows);
    // Set up a SPropValue array for the properties that you have to configure.

    // First, the server name.
//    ZeroMemory(&rgval[1], sizeof(SPropValue) );
//    rgval[1].ulPropTag = PR_PROFILE_UNRESOLVED_SERVER;
//    rgval[1].Value.lpszA = szServer;

//	_tprintf(_T("-->CreateProfile7\n"));
    // Next, the mailbox name.
    ZeroMemory(&rgval[0], sizeof(SPropValue) );
    rgval[0].ulPropTag = PR_FOLDER_DN_W; 
    rgval[0].Value.lpszW = szDataFile;
	INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_PROFILECR, szDataFile);
//	_tprintf(_T("-->pszDataFile %s\n"), pszDataFile);

//	_tprintf(_T("-->CreateProfile70 %s : %s\n"), pszDataFile, pszFolderName);
    // Configure the message service by using the previous properties.

//	_tprintf(_T("-->CreateProfile8\n"));
	INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_PROFILECR, _T("-->8"));
	if (FAILED(hRes = lpSvcAdmin->ConfigureMsgService(
        (LPMAPIUID)lpSvcRows->aRow->lpProps[iSvcUID].Value.bin.lpb, // Entry ID of service to configure.
        NULL,                                                       // Handle to parent window.
        0,                                                          // Flags.
        2,                                                          // Number of properties we are setting.
        rgval)))                                                    // Pointer to SPropValue array.
    {
//        cout<<"Error configuring message service.";
		printf("--> Error; %x\n", hRes);
        goto error;
    }

//	_tprintf(_T("-->CreateProfile done!\n"));
    goto cleanup;

error:
//    cout<<" hRes = 0x"<<hex<<hRes<<dec<<endl;
	INI_LogWriteWithResourcePrefixNum(IDS_LOG_MAPI_PROFILECREATEERR, GetScode(hRes), true);
	result = FALSE;

cleanup:
	INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_PROFILECR, _T("-->9"));
    // Clean up.
    if (lpSvcRows) FreeProws(lpSvcRows);
    if (lpMsgSvcTable) lpMsgSvcTable->Release();
    if (lpSvcAdmin) lpSvcAdmin->Release();
    if (lpProfAdmin) lpProfAdmin->Release();

	INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_PROFILECR, _T("-->10"));
    MAPIUninitialize();
	INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_PROFILECR, _T("-->11"));
    return result;

}

BOOL DeleteProfileWithIProfAdmin(_TCHAR *szProfile)
{
    HRESULT         hRes = S_OK;            // Result from MAPI calls.
    LPPROFADMIN     lpProfAdmin = NULL;     // Profile Admin object.
	BOOL result = TRUE;

    // Initialize MAPI.
    if (FAILED(hRes = MAPIInitialize(NULL)))
    {
//        cout<<"Error initializing MAPI.";
        goto error;
    }

    // Get an IProfAdmin interface.

    if (FAILED(hRes = MAPIAdminProfiles(0,              // Flags.
                                        &lpProfAdmin))) // Pointer to new IProfAdmin.
    {
//        cout<<"Error getting IProfAdmin interface.";
        goto error;
    }

    // Delete the existing profile.

#if defined(UNICODE)
	char szProfileA[128];
	wcstombs(szProfileA, szProfile, _tcslen(szProfile));
	szProfileA[_tcslen(szProfile)] = '\0';
    if (FAILED(hRes = lpProfAdmin->DeleteProfile((LPTSTR)szProfileA,     // Name of new profile.
                                                 NULL)))        // Flags.
#else
    if (FAILED(hRes = lpProfAdmin->DeleteProfile(szProfile,     // Name of new profile.
                                                 NULL)))        // Flags.
#endif
    {
//        cout<<"Error deleting profile.";
		if (hRes == MAPI_E_NOT_FOUND) {// The specified profile does not exist.
			INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_PROFILENOTFOUND, _T(""));
			goto cleanup;
		}

        goto error;
    }

	INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_PROFILEDELETED, _T(""));
    goto cleanup;

error:
//    cout<<" hRes = 0x"<<hex<<hRes<<dec<<endl;
	INI_LogWriteWithResourcePrefixNum(IDS_LOG_MAPI_PROFILEDELETEERR, GetScode(hRes), true);
    result = FALSE;

cleanup:
    // Clean up.
	INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_PROFILEDELETED, _T("-->1"));
    if (lpProfAdmin) lpProfAdmin->Release();
	INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_PROFILEDELETED, _T("-->2"));
    MAPIUninitialize();
	INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_PROFILEDELETED, _T("-->3"));
    return result;

}

LPMDB OpenDefaultStore(LPMAPISESSION pses)
{
    HRESULT hr;
    LPMDB lpmdb = NULL;
    LPMAPITABLE ptable = NULL;
    LPSRowSet prows = NULL;
    LPSPropValue pvalProp = NULL;
    static SizedSPropTagArray(2, columns) =
                { 2, { PR_DEFAULT_STORE, PR_ENTRYID} };
    SPropValue valDefStore;
    SPropertyRestriction restpropDefStore;
    SRestriction restDefStore;

    
    valDefStore.ulPropTag = PR_DEFAULT_STORE;
    valDefStore.dwAlignPad = 0;
    valDefStore.Value.b = TRUE;

    restpropDefStore.relop = RELOP_EQ;
    restpropDefStore.ulPropTag = PR_DEFAULT_STORE;
    restpropDefStore.lpProp = &valDefStore;

    restDefStore.rt = RES_PROPERTY;
    restDefStore.res.resProperty = restpropDefStore;

    hr = pses->GetMsgStoresTable(0, &ptable);
    if (HR_FAILED(hr))
    {
//        MakeMessageBox (hWnd, GetScode(hr), IDS_STORETBLFAIL, NULL, MBS_ERROR);
		INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_STORETBLFAIL, _T(""));
        goto ret;
    }

    
    hr = HrQueryAllRows(ptable, (LPSPropTagArray) &columns, &restDefStore, NULL, 0, &prows);
    if (HR_FAILED(hr))
    {
//        MakeMessageBox (hWnd, GetScode(hr), IDS_QUERYROWFAIL, NULL, MBS_ERROR);
		INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_QUERYROWFAIL, _T(""));
        goto ret;
    }

    if (prows == NULL || prows->cRows == 0
        || prows->aRow[0].lpProps[1].ulPropTag != PR_ENTRYID)
    {
//        MakeMessageBox (hWnd, 0L, IDS_NODEFAULTSTORE, NULL, MBS_ERROR);
		INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_NODEFAULTSTORE, _T(""));
        goto ret;
    }
    
//    Assert(prows->cRows == 1);

    hr = pses->OpenMsgStore((ULONG)NULL,
                        prows->aRow[0].lpProps[1].Value.bin.cb,
                        (LPENTRYID)prows->aRow[0].lpProps[1].Value.bin.lpb,
                        NULL, MDB_WRITE | MAPI_DEFERRED_ERRORS, &lpmdb);
    if (HR_FAILED(hr))
    {
//        if (GetScode(hr) != MAPI_E_USER_CANCEL)
//            MakeMessageBox (hWnd, GetScode(hr), IDS_OPENSTOREFAIL, NULL, MBS_ERROR);
//        Assert(lpmdb == NULL);
		INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_OPENSTOREFAIL, _T(""));
        goto ret;
    }
    if(hr) /*if we have a warning, display it and succeed */
    {
        LPMAPIERROR perr = NULL;

        pses->GetLastError(hr, 0, &perr);
//        MakeMessageBox(hWnd, GetScode(hr), IDS_OPENSTOREWARN, perr, MBS_ERROR);
		INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_OPENSTOREWARN, _T(""));
        MAPIFreeBuffer(perr);
    }


//    Assert(lpmdb != NULL);
	if (lpmdb != NULL) {
		INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_STOREOPENED, _T(""));
	}
                    
ret:
    FreeProws(prows);
//    UlRelease(ptable);

    return lpmdb;
}

LPMAPIFOLDER OpenMsgStoreFolderByName(
	LPMDB			lpMDB,   
	LPTSTR			lpszFolder     // Folder path name to open
)
{
	LPMAPIFOLDER lpFolder = NULL;
    HrMAPIOpenFolderEx( lpMDB, TEXT('\\'), lpszFolder, &lpFolder);
	return lpFolder;
}

BOOL DeleteFolder(LPMAPIFOLDER lpfRootF, _TCHAR *szSubfolderName)
{
//	wchar_t			pszSubfolderName[512];
	DWORD cbeid = 0;
	LPENTRYID lpeid = NULL;
	HRESULT hr;

//	wsprintfW(pszSubfolderName, L"%S", szSubfolderName);
	hr = HrMAPIFindSubfolderEx (lpfRootF, TEXT('\\'), szSubfolderName, &cbeid, &lpeid);
	if (HR_FAILED(hr))
		goto error;

	hr = lpfRootF->DeleteFolder(
		cbeid,
		lpeid,
        NULL,
        NULL,
        DEL_FOLDERS | DEL_MESSAGES);
	if (HR_FAILED(hr))
		goto error;

	INI_LogWriteWithResourcePrefix(IDS_LOG_MAPI_FOLDERDELETED, szSubfolderName);

	return TRUE;
error:
	return FALSE;
}
