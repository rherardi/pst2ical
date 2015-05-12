// XGetopt.h  Version 1.2
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
//
// This software is released into the public domain.
// You are free to use it in any way you like.
//
// This software is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XGETOPT_H
#define XGETOPT_H

#ifdef JESCOMMON_EXPORTS
#define JESCOMMON_API __declspec(dllexport)
#else
#define JESCOMMON_API __declspec(dllimport)
#endif

extern JESCOMMON_API int optind;
extern int opterr;
extern _TCHAR *optarg;

JESCOMMON_API int getopt(int argc, _TCHAR *argv[], _TCHAR *optstring);


#endif //XGETOPT_H
