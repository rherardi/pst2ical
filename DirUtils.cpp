// DirUtils.cpp: implementation of the CDirUtils class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <stdlib.h>
#include <direct.h>
#include <fcntl.h>
#include <io.h>

#include "DirUtils.hpp"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDirUtils::CDirUtils()
{

}

CDirUtils::~CDirUtils()
{

}

/* 0 - OK, !0 - Error */
int CDirUtils::MakeDir(const _TCHAR *path)
{
	_TCHAR *ptr;
	_TCHAR *token = _TEXT("\\");
	int res = 0;
	_TCHAR tmppath[_MAX_PATH];
	_TCHAR curdir[_MAX_PATH];

	lstrcpy(tmppath, path);
	/* change all backslashes to normal slashes*/
	for (unsigned int i=0; i<_tcslen(tmppath); i++) {
		if (tmppath[i] == _T('/')) {
			tmppath[i] = _T('\\');
		}
	}
	/* first get drive letter */
	ptr = _tcstok(tmppath, token);
	lstrcpy(curdir, ptr);
	lstrcat(curdir, token);

	/* get directory name */
	while((ptr = _tcstok(NULL, token)) != NULL) {
		lstrcat(curdir, ptr);
		if (ChangeDir(curdir) != 0) {
#if defined(UNICODE)
			res = _wmkdir(curdir);
#else
			res = _mkdir(curdir);
#endif
			if (res != 0)
				return res;
			ChangeDir(curdir);
		}
		lstrcat(curdir, token);
	}
	return res;
}/* MakeDir */

/* 0 - OK, !0 - Error */
int CDirUtils::MakeDir2(const _TCHAR *path1, const _TCHAR *path2)
{
	_TCHAR tmppath[_MAX_PATH];
	_tcscpy(tmppath, path1);
	_tcscat(tmppath, _TEXT("/"));
	_tcscat(tmppath, path2);
	return MakeDir(tmppath);
}

/* 0 - OK, !0 - Error */
int CDirUtils::MakeDir3(const _TCHAR *path1, const _TCHAR *path2, const _TCHAR *path3)
{
	_TCHAR tmppath[_MAX_PATH];
	_tcscpy(tmppath, path1);
	_tcscat(tmppath, _TEXT("/"));
	_tcscat(tmppath, path2);
	_tcscat(tmppath, _TEXT("/"));
	_tcscat(tmppath, path3);
	return MakeDir(tmppath);
}

int CDirUtils::ChangeDir(_TCHAR *path)
{
#if defined(UNICODE)
	return _wchdir(path);
#else
	return _chdir(path);
#endif
}/* ChangeDir */

FILE* CDirUtils::OpenFile(const _TCHAR *path1, const _TCHAR *path2, const _TCHAR *filename, const _TCHAR* mode)
{
	_TCHAR tmppath[_MAX_PATH];
	_tcscpy(tmppath, path1);
	if (path2 != NULL) {
		_tcscat(tmppath, _TEXT("/"));
		_tcscat(tmppath, path2);
	}
	_tcscat(tmppath, _TEXT("/"));
	_tcscat(tmppath, filename);
	for (unsigned int i=0; i<_tcslen(tmppath); i++) {
		if (tmppath[i] == _T('/')) {
			tmppath[i] = _T('\\');
		}
	}
	return _tfopen(tmppath, mode);
}

/************************************************************************

    FUNCTION:   GetUniqueFileName

    INPUTS:
          Drive - Drive letter string pointer.
          Ext - Extension name string pointer.

    OUTPUTS:
       FileName - Unqiue file name.

*************************************************************************/


#ifdef UNIX  /* UNIX platforms */

bool CDirUtils::GetUniqueFileName(char *Drive, char *Ext,
                                        char *FileName)

{
    int     file;
    WORD    Num;
    char    Name[17];
    char    cwd[_MAX_PATH];
    char   *Dir;

    /* Increment through numbered file names until a non-existent one found. */
   getcwd(cwd, _MAX_PATH);
   Dir = (char *)&cwd;

    for (Num = 0; Num <= 32767; Num++)
    {
         sprintf(Name, "%i", Num); /*_itoa(Num, Name, 10);*/
         sprintf(FileName, "%s%s.%s", Dir,Name,Ext); /* _makepath(FileName, Drive, Dir, Name, Ext);*/
        if ((file = open(FileName, O_RDONLY, 0666)) == -1)
            return TRUE;
        close(file);
    }
    FileName[0] = '\0';
    return FALSE;
}

#else  /* Wintel */

bool CDirUtils::GetUniqueFileName(char *Drive, char *Ext,
                                        char *FileName)

{
    int     file;
    WORD    Num;
    char    Name[17];
    char    cwd[_MAX_PATH];
    char   *Dir;

    /* Increment through numbered file names until a non-existent one found. */
   getcwd(cwd, _MAX_PATH);
   Dir = (char *)&cwd;

    for (Num = 0; Num <= 32767; Num++)
    {
        _itoa(Num, Name, 10);
        _makepath(FileName, Drive, Dir, Name, Ext);
        if ((file = open(FileName, O_BINARY | O_RDONLY)) == -1)
            return TRUE;
        close(file);
    }
    FileName[0] = '\0';
    return FALSE;
}

#endif
