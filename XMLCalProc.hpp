// XMLCalProc.h: interface for the CXMLCalProc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLCALPROC_H__A27B6A75_17DA_4814_AD79_FE431D325F4C__INCLUDED_)
#define AFX_XMLCALPROC_H__A27B6A75_17DA_4814_AD79_FE431D325F4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef NCALXML_EXPORTS
#define NCALXML_API __declspec(dllexport)
#else
#define NCALXML_API __declspec(dllimport)
#endif

#ifdef NCALXML_EXPORTS
XERCES_CPP_NAMESPACE_USE
XALAN_CPP_NAMESPACE_USE
#endif

#define CALENDAR_SELECTOR				_T("//calendar")
#define CALENDAR_CALENDAR_ITEM_SELECTOR	_T("//calendar/calendarItem")
#define CALENDAR_CALENDAR_ITEM_CSELECTOR	_T("//calendar/calendarItem[@condition=\"%s\"]")
#define CALENDAR_CALENDAR_ITEM_CSELECTOR2	_T("//calendar/calendarItem[@condition=\"%s\" or @condition=\"%s\"]")
#define CALENDAR_ITEM_SMTPLOOKUP_REQUIRED	_T("//calendar/calendarItem[child::lotusDocField[@valueModifier=\"smtpLookup\" and @smtpLookupRequired=\"true\"]]")

#define CONVERSION_RULE_ENTRY_SELECTOR			_T("child::conversionRule/child::*")

#define SMTPLOOKUP_SELECTOR 	_T("//calendar/calendarItem/lotusDocField[@valueModifier=\"smtpLookup\"]")
#define SMTPLOOKUP_REQUIRED_SELECTOR 	_T("//calendar/calendarItem/lotusDocField[@valueModifier=\"smtpLookup\" and @smtpLookupRequired=\"true\"]")

class CCalendar;
class CCalendarItem;
class CEntryTransformer;
class CConversionRuleEntry;

class CXMLCalProc : public CXMLProc  
{
public:
	NCALXML_API CXMLCalProc();
	virtual ~CXMLCalProc();

	NCALXML_API CCalendar* getCalendarInfo();
	NCALXML_API CCalendarItem * GetNextCalendarItem(_TCHAR *partCondition = NULL, _TCHAR *partCondition2 = NULL);
	NCALXML_API CCalendarItem* GetNextCalendarSMTPRequiredItem();
	NCALXML_API CEntryTransformer* getNextEntryTransformer();
	NCALXML_API CConversionRuleEntry* getNextConversionRuleEntry();
	NCALXML_API void resetConversionRuleIndex();
	NCALXML_API void resetEntryListIndex();
	NCALXML_API void resetEntryTransformerListIndex();
	NCALXML_API void InitializeCalendarItemList();
	NCALXML_API bool IsSMTPLookupRequested();
	NCALXML_API bool IsSMTPLookupRequired(_TCHAR *condition = NULL);
	NCALXML_API int GetCurrentEntryIndex();
	NCALXML_API void SetCurrentEntryIndex(int currentEntryIndex);
};

#endif // !defined(AFX_XMLCALPROC_H__A27B6A75_17DA_4814_AD79_FE431D325F4C__INCLUDED_)
