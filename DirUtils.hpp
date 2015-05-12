// DirUtils.hpp: interface for the CDirUtils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRUTILS_HPP__D9ADA5A4_996E_4408_82FD_4E3F6EF19C4F__INCLUDED_)
#define AFX_DIRUTILS_HPP__D9ADA5A4_996E_4408_82FD_4E3F6EF19C4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef JESCOMMON_EXPORTS
#define JESCOMMON_API __declspec(dllexport)
#else
#define JESCOMMON_API __declspec(dllimport)
#endif

class CDirUtils  
{
public:
	CDirUtils();
	virtual ~CDirUtils();

	JESCOMMON_API static int MakeDir(const _TCHAR *path);
	JESCOMMON_API static int MakeDir2(const _TCHAR *path1, const _TCHAR *path2);
	JESCOMMON_API static int MakeDir3(const _TCHAR *path1, const _TCHAR *path2, const _TCHAR *path3);
	JESCOMMON_API static FILE* OpenFile(const _TCHAR *path1, const _TCHAR *path2, const _TCHAR *filename, const _TCHAR* mode);
	JESCOMMON_API static int ChangeDir(_TCHAR *path);
	JESCOMMON_API bool GetUniqueFileName(char *Drive, char *Ext, char *FileName);
};

#endif // !defined(AFX_DIRUTILS_HPP__D9ADA5A4_996E_4408_82FD_4E3F6EF19C4F__INCLUDED_)
