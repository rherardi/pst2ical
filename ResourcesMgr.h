// ResourcesMgr.h: interface for the CResourcesMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESOURCESMGR_H__9B360F2F_F60B_490C_9327_A9DC91E22FD0__INCLUDED_)
#define AFX_RESOURCESMGR_H__9B360F2F_F60B_490C_9327_A9DC91E22FD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CResourcesMgr  
{
public:
	CResourcesMgr();
	virtual ~CResourcesMgr();
	void GetVersion(_TCHAR* productName, _TCHAR* versionInfo);
	int ExtractConfig(int resourceID, _TCHAR* fullConfigName);
};

#define RESOURCE_FIND_PROBLEM	-2
#define RESOURCE_WRITE_PROBLEM	-1

#endif // !defined(AFX_RESOURCESMGR_H__9B360F2F_F60B_490C_9327_A9DC91E22FD0__INCLUDED_)
