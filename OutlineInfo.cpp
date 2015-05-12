// OutlineInfo.cpp: implementation of the COutlineInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OutlineInfo.hpp"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COutlineInfo::COutlineInfo()
{
	m_Name[0] = _T('\0');
	m_ContentValue[0] = _T('\0');
	m_ImageName[0] = _T('\0');
}

COutlineInfo::~COutlineInfo()
{

}

COutlineInfo& COutlineInfo::operator = (COutlineInfo& info)
{
//	printf("-->I'm called here also!\n");
//	if (info.m_Name[0] != _T('\0')) {
//		_tprintf(_TEXT("info.name: %x %x\n"), info.m_Name[0], info.m_Name[1]);
//	}
	_tcscpy(this->m_Name, info.m_Name);
	_tcscpy(this->m_ContentValue, info.m_ContentValue);
	return *this;
}

