// OutlookCondition.cpp: implementation of the COutlookCondition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "mapistuff.h"
#include "OutlookCondition.hpp"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COutlookCondition::COutlookCondition()
{

}

COutlookCondition::~COutlookCondition()
{

}

bool COutlookCondition::IsOutlookCondition(LPMESSAGE pmsg, const _TCHAR *condition, HRESULT *error)
{
	bool checkCondition = false;
	if (_tcscmp(condition, CONDITION_NOATTACHMENTS) == 0) {
		return !CheckAttachmentExists(pmsg, error);
	}
	else if (_tcscmp(condition, CONDITION_ATTACHMENTEXISTS) == 0) {
		return CheckAttachmentExists(pmsg, error);
	}
	else if (_tcscmp(condition, CONDITION_TEXTPART) == 0) {
	}
	else if (_tcscmp(condition, CONDITION_ATTACHMENTPART) == 0) {
	}
	return checkCondition;
}

bool COutlookCondition::CheckAttachmentExists(LPMESSAGE pmsg, HRESULT *error)
{
	*error = S_OK;
	return false;
}
