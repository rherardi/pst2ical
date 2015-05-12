#include "stdafx.h"
#include "resources.h"

void CResources::ShowTitle(unsigned int titleStrID, unsigned int versionStrID)
{
	_TCHAR title[256];
	HINSTANCE hInst = ::GetModuleHandle(NULL);
	::LoadString(hInst, titleStrID, title, 256);
	::_tprintf(_TEXT("%s"), title);

	LPCTSTR pVer = (LPCTSTR)VS_VERSION_INFO;
	HRSRC hVer = ::FindResourceEx(hInst, RT_VERSION, pVer, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));
	if (hVer != NULL) {
		_TCHAR version[256];
		::LoadString(hInst, versionStrID, version, 256);

		_TCHAR copyright[256];
		_TCHAR versionInfo[256];
		GetVersion(NULL, versionInfo, copyright);
		::_tprintf(_TEXT(" %s: %s\n"), version, versionInfo);
		::_tprintf(_TEXT("%s\n"), copyright);
	}
	::_tprintf(_TEXT("\n"));
}

void CResources::GetVersion(_TCHAR* productName, _TCHAR* versionInfo, _TCHAR* copyright)
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

		::_stprintf(versionInfo, _TEXT("%d.%d.%d.%d"), lVer[0], lVer[1], lVer[2], lVer[3]);

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
			if (productName != NULL) {
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
			}
			if (copyright != NULL) {
				wsprintf(pSubBlock, 
						_TEXT("\\StringFileInfo\\%04x%04x\\LegalCopyright"),
						lpTranslate[i].wLanguage,
						lpTranslate[i].wCodePage);

			  // Retrieve file description for language and code page "i". 
				::VerQueryValue(pResource, 
							pSubBlock, 
							&lpBuffer, 
							&dwBytes); 
				::_stprintf(copyright, _TEXT("%s"), lpBuffer);
			}
			break;
		}	

	}
}

