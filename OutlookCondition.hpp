// OutlookCondition.hpp: interface for the COutlookCondition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTLOOKCONDITION_HPP__79ADC945_2F9D_48D8_A931_DD242A441425__INCLUDED_)
#define AFX_OUTLOOKCONDITION_HPP__79ADC945_2F9D_48D8_A931_DD242A441425__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef NCALXML_EXPORTS
#define NCALXML_API __declspec(dllexport)
#else
#define NCALXML_API __declspec(dllimport)
#endif

#define CONDITION_NOATTACHMENTS			_T("NoAttachments")
#define CONDITION_ATTACHMENTEXISTS		_T("AttachmentExists")
#define CONDITION_TEXTPART				_T("TextPart")
#define CONDITION_ATTACHMENTPART		_T("AttachmentPart")
#define CONDITION_ENDOFPARTS			_T("EndOfParts")
#define CONDITION_CALHEADER				_T("CalendarHeader")
#define CONDITION_CALFOOTER				_T("CalendarFooter")
#define CONDITION_EVENTFOOTER			_T("EventFooter")
#define CONDITION_EVENTMEETING			_T("Meeting")
#define CONDITION_EVENTMEETING_ATTENDEE _T("MeetingAttendee")

class COutlookCondition  
{
public:
	COutlookCondition();
	virtual ~COutlookCondition();

	NCALXML_API static bool IsOutlookCondition(LPMESSAGE pmsg, const _TCHAR *condition, HRESULT *error);
	NCALXML_API static bool CheckAttachmentExists(LPMESSAGE pmsg, HRESULT *error);

};

#endif // !defined(AFX_OUTLOOKCONDITION_HPP__79ADC945_2F9D_48D8_A931_DD242A441425__INCLUDED_)
