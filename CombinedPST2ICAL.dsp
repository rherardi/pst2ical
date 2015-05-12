# Microsoft Developer Studio Project File - Name="CombinedPST2ICAL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=CombinedPST2ICAL - Win32 Unicode Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CombinedPST2ICAL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CombinedPST2ICAL.mak" CFG="CombinedPST2ICAL - Win32 Unicode Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CombinedPST2ICAL - Win32 Unicode Release" (based on "Win32 (x86) Console Application")
!MESSAGE "CombinedPST2ICAL - Win32 Unicode Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/CombinedPST2ICAL", CRAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CombinedPST2ICAL - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CombinedPST2ICAL___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "CombinedPST2ICAL___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode Release"
# PROP Intermediate_Dir "Unicode Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp1 /W3 /GX /O2 /I "C:\xerces-c-windows_2000-msvc_60\include" /I "C:\Xalan-C_1_10_0-win32-msvc_60\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_UNICODE" /D "UNICODE" /D "W32" /D "JESCOMMON_EXPORTS" /D "NCALXML_EXPORTS" /YX /FD /c
# ADD BASE RSC /l 0x427 /d "NDEBUG"
# ADD RSC /l 0x427 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib mapi32.lib EXCHSDK.LIB version.lib msvcrt.lib C:\xerces-c-windows_2000-msvc_60\lib\xerces-c_2.lib C:\Xalan-C_1_10_0-win32-msvc_60\lib\Xalan-C_1.lib /nologo /subsystem:console /machine:I386 /nodefaultlib:"libc" /nodefaultlib:"libcmt" /out:"Unicode Release/PST2ICAL.exe"

!ELSEIF  "$(CFG)" == "CombinedPST2ICAL - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CombinedPST2ICAL___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "CombinedPST2ICAL___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode Debug"
# PROP Intermediate_Dir "Unicode Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp1 /W3 /GX /O2 /I "C:\Lotus\notesapi\include" /I "C:\xerces-c-windows_2000-msvc_60\include" /I "C:\Xalan-C_1_10_0-win32-msvc_60\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_UNICODE" /D "UNICODE" /D "W32" /D "JESCOMMON_EXPORTS" /D "NMAILXML_EXPORTS" /YX /FD /c
# ADD CPP /nologo /Zp1 /MTd /W3 /GX /ZI /Od /I "C:\Lotus\notesapi\include" /I "C:\xerces-c-windows_2000-msvc_60\include" /I "C:\Xalan-C_1_10_0-win32-msvc_60\include" /D "WIN32" /D "DEBUG" /D "_CONSOLE" /D "_UNICODE" /D "UNICODE" /D "W32" /D "JESCOMMON_EXPORTS" /D "NMAILXML_EXPORTS" /FR /YX /FD /c
# ADD BASE RSC /l 0x427 /d "NDEBUG"
# ADD RSC /l 0x427 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib mapi32.lib EXCHSDK.LIB version.lib msvcrt.lib C:\Lotus\notesapi\lib\mswin32\notes.lib C:\xerces-c-windows_2000-msvc_60\lib\xerces-c_2.lib C:\Xalan-C_1_10_0-win32-msvc_60\lib\Xalan-C_1.lib /nologo /subsystem:console /machine:I386 /nodefaultlib:"libc" /nodefaultlib:"libcmt" /out:"Unicode Release/NSF2PST.exe"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib mapi32.lib EXCHSDK.LIB version.lib msvcrt.lib C:\Lotus\notesapi\lib\mswin32\notes.lib D:\IBM\rad60\workspace\test1\xerces-2.7\Build\Win32\VC6\Debug\xerces-c_2D.lib D:\IBM\rad60\workspace\test1\xalan-C_1_10_0\Build\Win32\VC6\Debug\Xalan-C_1D.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libc" /nodefaultlib:"libcmt" /out:"Unicode Debug/NSF2PST.exe"

!ENDIF 

# Begin Target

# Name "CombinedPST2ICAL - Win32 Unicode Release"
# Name "CombinedPST2ICAL - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Calendar.cpp
# End Source File
# Begin Source File

SOURCE=.\CalendarItem.cpp
# End Source File
# Begin Source File

SOURCE=.\CODES.C
# End Source File
# Begin Source File

SOURCE=.\ConversionRuleEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\DirUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\EntryTransformer.cpp
# End Source File
# Begin Source File

SOURCE=.\Inistuff.cpp
# End Source File
# Begin Source File

SOURCE=.\JESCommon.cpp
# End Source File
# Begin Source File

SOURCE=.\List.cpp
# End Source File
# Begin Source File

SOURCE=.\Mapi.cpp
# End Source File
# Begin Source File

SOURCE=.\MD5C.C
# End Source File
# Begin Source File

SOURCE=.\ObjectArray.cpp
# End Source File
# Begin Source File

SOURCE=.\OutlineInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\OutlookCondition.cpp
# End Source File
# Begin Source File

SOURCE=.\PST2ICALApp.cpp
# End Source File
# Begin Source File

SOURCE=.\PSTProc.cpp
# End Source File
# Begin Source File

SOURCE=.\Resources.cpp
# End Source File
# Begin Source File

SOURCE=.\SAXErrorHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\XGetopt.cpp
# End Source File
# Begin Source File

SOURCE=.\XMALLOC.C
# End Source File
# Begin Source File

SOURCE=.\XMLCalProc.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLNameValue.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLProc.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Calendar.hpp
# End Source File
# Begin Source File

SOURCE=.\CalendarItem.hpp
# End Source File
# Begin Source File

SOURCE=.\Config.hpp
# End Source File
# Begin Source File

SOURCE=.\ConversionRuleEntry.hpp
# End Source File
# Begin Source File

SOURCE=.\DirUtils.hpp
# End Source File
# Begin Source File

SOURCE=.\EntryTransformer.hpp
# End Source File
# Begin Source File

SOURCE=.\Inistuff.h
# End Source File
# Begin Source File

SOURCE=.\JESCommon.h
# End Source File
# Begin Source File

SOURCE=.\List.hpp
# End Source File
# Begin Source File

SOURCE=.\mapistuff.h
# End Source File
# Begin Source File

SOURCE=.\MD5.H
# End Source File
# Begin Source File

SOURCE=.\ObjectArray.hpp
# End Source File
# Begin Source File

SOURCE=.\OutlineInfo.hpp
# End Source File
# Begin Source File

SOURCE=.\OutlookCondition.hpp
# End Source File
# Begin Source File

SOURCE=.\PST2ICALApp.hpp
# End Source File
# Begin Source File

SOURCE=.\PSTProc.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\Resources.h
# End Source File
# Begin Source File

SOURCE=.\retcodes.h
# End Source File
# Begin Source File

SOURCE=.\SAXErrorHandler.hpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\XGetopt.h
# End Source File
# Begin Source File

SOURCE=.\XMALLOC.H
# End Source File
# Begin Source File

SOURCE=.\XMLCalProc.hpp
# End Source File
# Begin Source File

SOURCE=.\XMLNameValue.hpp
# End Source File
# Begin Source File

SOURCE=.\XMLProc.hpp
# End Source File
# Begin Source File

SOURCE=.\XMLProcSuppl.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\gss_logo.ico
# End Source File
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# End Group
# Begin Group "Related"

# PROP Default_Filter "xml xsd"
# Begin Source File

SOURCE=.\pst2ical.xml
# End Source File
# Begin Source File

SOURCE=.\pst2ical.xsd
# End Source File
# End Group
# End Target
# End Project
