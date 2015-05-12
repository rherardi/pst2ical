// ResourcesMgr.cpp: implementation of the CResourcesMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdio.h>
#include "ResourcesMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CResourcesMgr::CResourcesMgr()
{

}

CResourcesMgr::~CResourcesMgr()
{

}

void CResourcesMgr::GetVersion(_TCHAR* productName, _TCHAR* versionInfo)
{
	HINSTANCE hInst = ::GetModuleHandle(NULL);

	LPCTSTR pVer = (LPCTSTR)VS_VERSION_INFO;
	HRSRC hVer = ::FindResourceEx(hInst, RT_VERSION, pVer, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));
	if (hVer != NULL) {
		WORD *pResource = (WORD *)::LockResource(::LoadResource(hInst, hVer));
		
		VS_FIXEDFILEINFO sApp;

		if ( pResource[1] )
		{ 
			for ( DWORD* pFor = (DWORD*)pResource; *(++pFor) != 0xFEEF04BD; )
			{ } 
			(void)memcpy( &sApp, pFor, sizeof( sApp ) ); 
		} 
		DWORD lVer[] =
		{ 
			sApp.dwProductVersionMS >> 16, sApp.dwProductVersionMS & 0xFF, 
				sApp.dwProductVersionLS >> 16, sApp.dwProductVersionLS & 0xFF 
		};

		::_stprintf(versionInfo, _TEXT("(%d.%d.%d.%d)"), lVer[0], lVer[1], lVer[2], lVer[3]);

		struct LANGANDCODEPAGE {
		  WORD wLanguage;
		  WORD wCodePage;
		} *lpTranslate;

		_TCHAR SubBlock[256];
		_TCHAR buffer[256];
		_TCHAR *pSubBlock = SubBlock;

		LPVOID lpBuffer = buffer;
		UINT dwBytes;

		// Read the list of languages and code pages.
		UINT cbTranslate = 0;

		::VerQueryValue(pResource, 
					  _TEXT("\\VarFileInfo\\Translation"),
					  (LPVOID*)&lpTranslate,
					  &cbTranslate);

		// Read the file description for each language and code page.

		for(UINT i=0; i < (cbTranslate/sizeof(struct LANGANDCODEPAGE)) && i<1; i++ )
		{
			wsprintf(pSubBlock, 
					_TEXT("\\StringFileInfo\\%04x%04x\\ProductName"),
					lpTranslate[i].wLanguage,
					lpTranslate[i].wCodePage);

		  // Retrieve file description for language and code page "i". 
			::VerQueryValue(pResource, 
						pSubBlock, 
						&lpBuffer, 
						&dwBytes); 
			::_stprintf(productName, _TEXT("%s"), lpBuffer);
			break;
		}	

	}
}

int CResourcesMgr::ExtractConfig(int resourceID, _TCHAR* fullConfigName)
{
	HINSTANCE hInst = ::GetModuleHandle(NULL);
	HRSRC hConfig = ::FindResourceEx(hInst, _T("Config"), MAKEINTRESOURCE(resourceID), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));
	HGLOBAL hResource;
	FILE *f;

	if (hConfig != NULL) {
		unsigned long configSize = SizeofResource(hInst, hConfig);
		// now get a handle to the resource
		if ((hResource = LoadResource(hInst, hConfig)) == NULL) {
			return -3;
		}
		// finally get and return a pointer to the resource
		UCHAR* pResource = ((UCHAR*)LockResource(hResource));

		f = _tfopen(fullConfigName, _TEXT("wb"));

		if (f == NULL) {
			return -1;
		}

		unsigned int itemsWritten = fwrite(pResource, sizeof(UCHAR), configSize, f);
		if (itemsWritten < configSize) {
			fclose(f);
			return -1;
		}

		fclose(f);
	}
	else {
		return -2;
	}

	return 0;
}
