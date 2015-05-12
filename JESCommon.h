
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the JESCOMMON_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// JESCOMMON_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef JESCOMMON_EXPORTS
#define JESCOMMON_API __declspec(dllexport)
#else
#define JESCOMMON_API __declspec(dllimport)
#endif

/*
// This class is exported from the JESCommon.dll
class JESCOMMON_API CJESCommon {
public:
	CJESCommon(void);
	// TODO: add your methods here.
};

//extern JESCOMMON_API int nJESCommon;

JESCOMMON_API int fnJESCommon(void);
*/

extern "C" {
JESCOMMON_API int to64(FILE *infile, FILE *outfile, long limit);
JESCOMMON_API int to64buf(const unsigned char *inbuf, unsigned long inbuflen, FILE *outfile, long limit);
JESCOMMON_API int to64buf2(const unsigned char *inbuf, unsigned long inbuflen, unsigned char *outbuf, long limit) ;
}
