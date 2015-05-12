// JESCommon.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "JESCommon.h"

//#pragma data_seg(".shared")
//#pragma data_seg()
//#pragma comment(linker, "/section:.shared,RWS")

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			printf("JESCommon DLL_PROCESS_ATTACH\n");
			break;
		case DLL_THREAD_ATTACH:
			printf("JESCommon DLL_THREAD_ATTACH\n");
			break;
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


// This is an example of an exported variable
//JESCOMMON_API int nJESCommon=0;

/*
// This is an example of an exported function.
JESCOMMON_API int fnJESCommon(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see JESCommon.h for the class definition
CJESCommon::CJESCommon()
{ 
	return; 
}

*/