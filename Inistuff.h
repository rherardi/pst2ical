/* INISTUFF.H */
#ifndef __INISTUFF_H
#define __INISTUFF_H

//#define LOG_C		1
//#define LOG_R		2
//#define LOG_S		3

#define MAX_STRING	256
#define DEFAULT_LOG_NAME _T("pst2mbox")
#define DEFAULT_LOG_EXT _T(".log")

char *INI_CurDir(char *path);
char *INI_WinDir(char *path);
int INI_GetIniFullName(char *fullname);

extern char *glFileName;
extern int glLine;

void INI_LogInit(const _TCHAR *logPath, bool isFullName = false, bool isAppend = false);
void INI_LogWriteWithResourcePrefix(int prefix, _TCHAR *txt);
void INI_LogWriteWithPrefixNum(_TCHAR * prefix, long number, bool console = false);
void INI_LogWriteWithResourcePrefixA(int prefix, char *txt);
void INI_LogWriteWithResourcePrefixP(int prefix, _TCHAR *txt, _TCHAR *param);
void INI_LogWriteWithResourcePrefixNum(int prefix, long number, bool isUnsigned = false);
void INI_LogWriteWithResourcePrefixNumP(int prefix, long number, _TCHAR *param, bool console = false);
void INI_LogWriteWithResourcePrefixNumAP(int prefix, long number, char *param, bool console = false);
void INI_LogWrite(_TCHAR *txt);
void INI_LogClose();
void INI_SetLogSizeWrap(unsigned int logSizeWrap);

#endif /* __INISTUFF_H */
