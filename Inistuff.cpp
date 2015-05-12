#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <dos.h>
#include <io.h>
#include <string.h>
#include <errno.h>
#ifdef WIN32
#include <direct.h>
#endif

#include "inistuff.h"
#include "resource.h"
#include "Resources.h"

extern HINSTANCE hInst;

FILE *glLogFile;
unsigned int glLogSizeWrap = 0;
unsigned int glLogPartCounter = 1;
unsigned long glBytesWritten = 0;
_TCHAR logFullName[_MAX_PATH];
char *glBuffer;

int INI_FileExists(char *fname)
{
	if (access(fname, 0) == 0)
		return 1;
	return 0;
}/* INI_FileExists */

void INI_LogInit(const _TCHAR *logPath, bool isFullName, bool isAppend) {
	_TCHAR buf[MAX_STRING];
	DWORD   dwVerInfoSize = 0;
	DWORD   dwVerHandle   = 0;
	LPVOID  lpVersionInfo = NULL;
	BOOL    fRet          = 0;
	UINT             dwBytes            = 0;
	LPVOID           lpBuffer           = 0;
//	VS_FIXEDFILEINFO *lpvsFixedFileInfo = {0};
//	DWORD dwFileVersionMS;
//	DWORD dwFileVersionLS;

	lstrcpy(logFullName, logPath);
	if (!isFullName) {
		if (logFullName[lstrlen(logFullName)-1] != '\\')
			lstrcat(logFullName, _T("\\"));
		lstrcat(logFullName, DEFAULT_LOG_NAME);
		if (glLogSizeWrap > 0) {
			_TCHAR partNumber[4];
			wsprintf(partNumber, _T("%03d"), glLogPartCounter);
			lstrcat(logFullName, partNumber);
		}
		lstrcat(logFullName, DEFAULT_LOG_EXT);
	}

//	glLogFile = _tfopen(logFullName, _T("a"));
	if (isAppend)
		glLogFile = _tfopen(logFullName, _T("a+b"));
	else
		glLogFile = _tfopen(logFullName, _T("w+b"));
	if (glLogFile == NULL) {
		_tprintf(_T("Error! Log file %s cannot be opened!\n"), logFullName);
		return;
	}
	glBuffer = (char *)malloc(8192 * sizeof(TCHAR));
	if (glBuffer == NULL) {
		_tprintf(_T("Error! Log buffer cannot be allocated!\n"));
	}
	else {
		if( setvbuf( glLogFile, glBuffer, _IOFBF, sizeof( 8192 * sizeof(TCHAR) ) ) != 0 )
			_tprintf(_T("Incorrect type or size of buffer for log file\n"));
	}
#ifdef _UNICODE
	_ftprintf(glLogFile, _TEXT("%c"), 0xfeff);
#endif
//	wsprintf(buf, _T("(Build %03d)"), BUILDNO);
	_TCHAR versionInfo[256];
	CResources::GetVersion(NULL, versionInfo, NULL);
	wsprintf(buf, _T("(Version: %s)"), versionInfo);
	INI_LogWriteWithResourcePrefix(IDS_LOG_CONV_START, buf);

	if (GetModuleFileName(NULL, buf, MAX_PATH) == 0) {
//		wsprintf(buf, "GetModuleFileName failed (%d)\n", GetLastError()); 
//		INI_LogWriteWithResourcePrefix(IDS_LOG_VIM_PATH, buf);
		return; // do nothing
	}
	INI_LogWriteWithResourcePrefix(IDS_LOG_EXEC_PATH, buf);

	dwVerInfoSize = GetFileVersionInfoSize(buf, &dwVerHandle);
	if(dwVerInfoSize != 0) {
		lpVersionInfo =
			(LPVOID)malloc(dwVerInfoSize);

		if(lpVersionInfo == NULL) {
			return;
		}
    
		fRet = GetFileVersionInfo(
			buf,
			0,
			dwVerInfoSize,
			lpVersionInfo);

        if(fRet == FALSE) {
			goto cleanup;
        }

		fRet = VerQueryValue(lpVersionInfo,
					  TEXT("\\"),
					  &lpBuffer,
					  &dwBytes);

        if(fRet == FALSE) {
			goto cleanup;
        }

		fRet = VerQueryValue(lpVersionInfo,
					  TEXT("\\StringFileInfo\\040904e4\\FileVersion"),
					  &lpBuffer,
					  &dwBytes);

        if(fRet == FALSE) {
			goto cleanup;
        }
		else {
//			lpvsFixedFileInfo = (VS_FIXEDFILEINFO *)lpBuffer;
//
//			dwFileVersionMS = lpvsFixedFileInfo->dwFileVersionMS;
//			dwFileVersionLS = lpvsFixedFileInfo->dwFileVersionLS;
			INI_LogWriteWithResourcePrefix(IDS_LOG_EXEC_VERSION, (_TCHAR *)(lpBuffer));
		}
	}

cleanup:
	if(lpVersionInfo != NULL)
	{
		free(lpVersionInfo);
	}
}

void INI_LogWriteWithResourcePrefix(int prefix, _TCHAR *txt)
{
	if (txt != NULL) {
		_TCHAR buf[2*MAX_STRING];
		LoadString(hInst, prefix, buf, sizeof(buf));
		lstrcat(buf, _T(" "));
		lstrcat(buf, txt);
		INI_LogWrite(buf);
	}
}

void INI_LogWriteWithResourcePrefixP(int prefix, _TCHAR *txt, _TCHAR *param)
{
	if (txt != NULL) {
		_TCHAR mask[2*MAX_STRING];
		_TCHAR buf[2*MAX_STRING];
		LoadString(hInst, prefix, mask, sizeof(mask));
		wsprintf(buf, mask, param);
		lstrcat(buf, _T(" "));
		lstrcat(buf, txt);
		INI_LogWrite(buf);
	}
}

void INI_LogWriteWithResourcePrefixA(int prefix, char *txt)
{
	if (txt != NULL) {
		_TCHAR buf[2*MAX_STRING];
		_TCHAR txtW[2*MAX_STRING];
		LoadString(hInst, prefix, buf, sizeof(buf));
		lstrcat(buf, _T(" "));
		mbstowcs(txtW, txt, strlen(txt));
		txtW[strlen(txt)] = '\0';
		lstrcat(buf, txtW);
		INI_LogWrite(buf);
	}
}

void INI_LogWriteWithResourcePrefixNum(int prefix, long number, bool isUnsigned)
{
	_TCHAR buf[MAX_STRING];
	_TCHAR buf_number[MAX_STRING];
	LoadString(hInst, prefix, buf, sizeof(buf));
	if (!isUnsigned)
		wsprintf(buf_number, _T(" %ld"), number);
	else
		wsprintf(buf_number, _T(" %lx"), number);
	lstrcat(buf, buf_number);
	INI_LogWrite(buf);
}

void INI_LogWriteWithResourcePrefixNumP(int prefix, long number, _TCHAR *param, bool console)
{
	_TCHAR mask[MAX_STRING];
	_TCHAR buf[MAX_STRING];
	_TCHAR buf_number[MAX_STRING];
	LoadString(hInst, prefix, mask, sizeof(mask));
	wsprintf(buf, mask, param);
	wsprintf(buf_number, _T(" %ld"), number);
	lstrcat(buf, buf_number);
	INI_LogWrite(buf);
	if (console)
		_tprintf(_T("%s\n"), buf);
}

void INI_LogWriteWithPrefixNum(_TCHAR * prefix, long number, bool console)
{
	_TCHAR buf[MAX_STRING];
	_TCHAR buf_number[MAX_STRING];
	wsprintf(buf_number, _T(" %ld"), number);
	lstrcpy(buf, prefix);
	lstrcat(buf, buf_number);
	INI_LogWrite(buf);
	if (console)
		_tprintf(_T("%s\n"), buf);
}

void INI_LogWriteWithResourcePrefixNumAP(int prefix, long number, char *param, bool console)
{
	_TCHAR mask[MAX_STRING];
	_TCHAR buf[MAX_STRING];
	_TCHAR buf_number[MAX_STRING];
	_TCHAR txtW[2*MAX_STRING];
	LoadString(hInst, prefix, mask, sizeof(mask));
	mbstowcs(txtW, param, strlen(param));
	txtW[strlen(param)] = '\0';
	wsprintf(buf, mask, txtW);
	wsprintf(buf_number, _T(" %ld"), number);
	lstrcat(buf, buf_number);
	INI_LogWrite(buf);
	if (console)
		_tprintf(_T("%s\n"), buf);
}

void INI_LogWrite(_TCHAR *txt)
{
//	char crlf[] = "\r\n";
	if (glLogFile != NULL && txt != NULL) {
		_TCHAR lpszOut[1024];
		SYSTEMTIME st;
		_TCHAR mask[] = _T("%d.%02d.%02d %02d:%02d:%02d:%03d %s");

//		GetSystemTime(&st);
		GetLocalTime(&st);

		wsprintf(lpszOut, mask, st.wYear, st.wMonth, st.wDay, 
		  st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, txt);
		int charsWritten = _ftprintf(glLogFile, _T("%s\r\n"), lpszOut);
		if (glLogSizeWrap > 0) {
			glBytesWritten += (charsWritten * sizeof(TCHAR));
			if (glBytesWritten > glLogSizeWrap * 0x100000) {
				glLogPartCounter++;
				if (glLogFile != NULL) {
					_TCHAR buf[MAX_STRING];
					_TCHAR buf_number[MAX_STRING];
					LoadString(hInst, IDS_LOG_CONT_PART, buf, sizeof(buf));
					wsprintf(buf_number, _T(" %ld"), glLogPartCounter);
					lstrcat(buf, buf_number);

					GetLocalTime(&st);
					wsprintf(lpszOut, mask, st.wYear, st.wMonth, st.wDay, 
					  st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, buf);
					_ftprintf(glLogFile, _T("%s\r\n"), lpszOut);
					fclose(glLogFile);
					glLogFile = NULL;
				}
				glBytesWritten = 0;
				_TCHAR *partPtr = logFullName + _tcslen(logFullName) - _tcslen(DEFAULT_LOG_EXT) - 3;
				_TCHAR newPart[4];
				wsprintf(newPart, _T("%03d"), glLogPartCounter);
				_tcsncpy(partPtr, newPart, _tcslen(newPart));
				_tprintf(_T("Log's new part name: %s\n"), logFullName);
				glLogFile = _tfopen(logFullName, _T("w+b"));
				if (glLogFile == NULL) {
					_tprintf(_T("Error! Log file %s cannot be opened!\n"), logFullName);
					return;
				}
			}
		}

//		fwrite(txt, strlen(txt), 1, glLogFile);

//		fwrite(crlf, strlen(crlf), 1, glLogFile);
		fflush(glLogFile);
	}
}

void INI_LogClose() {
	if (glLogFile != NULL) {
		INI_LogWriteWithResourcePrefix(IDS_LOG_CONV_END, _T(""));
		fclose(glLogFile);
		glLogFile = NULL;
		if(glBuffer != NULL)
		{
			free(glBuffer);
		}
	}
}

void INI_SetLogSizeWrap(unsigned int logSizeWrap)
{
	glLogSizeWrap = logSizeWrap;
}